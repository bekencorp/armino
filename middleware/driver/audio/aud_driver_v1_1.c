// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#include <common/bk_include.h>
#include "aud_hal.h"
#include "aud_driver.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <driver/aud_types.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <driver/aud.h>
#include <modules/pm.h>
#include "sys_hal.h"

#define SYS_ANA_REG18_ISELAUD_DEFAULT_VAL                      (0x01)
#define SYS_ANA_REG18_AUDCK_RLCEN1V_DEFAULT_VAL                (0x00)
#define SYS_ANA_REG18_LCHCKINVEN1V_DEFAULT_VAL                 (0x01)
#define SYS_ANA_REG18_ENAUDBIAS_DEFAULT_VAL                    (0x00)
#define SYS_ANA_REG18_ENADCBIAS_DEFAULT_VAL                    (0x00)
#define SYS_ANA_REG18_ENMICBIAS_DEFAULT_VAL                    (0x00)
#define SYS_ANA_REG18_ADCCKINVEN1V_DEFAULT_VAL                 (0x00)
#define SYS_ANA_REG18_DACFB2ST0V9_DEFAULT_VAL                  (0x01)
#define SYS_ANA_REG18_NC1_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG18_MICBIAS_TRM_DEFAULT_VAL                  (0x00)
#define SYS_ANA_REG18_MICBIAS_VOC_DEFAULT_VAL                  (0x10)
#define SYS_ANA_REG18_VREFSEL1V_DEFAULT_VAL                    (0x01)
#define SYS_ANA_REG18_CAPSWSPI_DEFAULT_VAL                     (0x1f)
#define SYS_ANA_REG18_ADREF_SEL_DEFAULT_VAL                    (0x02)
#define SYS_ANA_REG18_NC0_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG18_RESERVED_BIT_26_30_DEFAULT_VAL           (0x00)
#define SYS_ANA_REG18_SPI_DACCKPSSEL_DEFAULT_VAL               (0x00)

#define SYS_ANA_REG19_ISEL_DEFAULT_VAL                         (0x02)
#define SYS_ANA_REG19_MICIRSEL1_DEFAULT_VAL                    (0x01)
#define SYS_ANA_REG19_MICDACIT_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICDACIH_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICSINGLEEN_DEFAULT_VAL                  (0x00)
#define SYS_ANA_REG19_DCCOMPEN_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICGAIN_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG19_MICDACEN_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_STG2LSEN1V_DEFAULT_VAL                   (0x00)
#define SYS_ANA_REG19_OPENLOOPCAL1V_DEFAULT_VAL                (0x00)
#define SYS_ANA_REG19_CALLATCH_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_VCMSEL_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG19_DWAMODE_DEFAULT_VAL                      (0x01)
#define SYS_ANA_REG19_R2REN_DEFAULT_VAL                        (0x00)
#define SYS_ANA_REG19_NC_26_27_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG19_MICEN_DEFAULT_VAL                        (0x00)
#define SYS_ANA_REG19_RST_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG19_BPDWA1V_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG19_HCEN1STG_DEFAULT_VAL                     (0x01)

#define SYS_ANA_REG20_HPDAC_DEFAULT_VAL                        (0x01)
#define SYS_ANA_REG20_CALCON_SEL_DEFAULT_VAL                   (0x01)
#define SYS_ANA_REG20_OSCDAC_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG20_OCENDAC_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_VCMSEL_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG20_ADJDACREF_DEFAULT_VAL                    (0x10)
#define SYS_ANA_REG20_DCOCHG_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG20_DIFFEN_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG20_ENDACCAL_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG20_NC2_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG20_LENDCOC_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_NC1_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG20_LENVCMD_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_DACDRVEN_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG20_NC0_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG20_DACLEN_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG20_DACG_DEFAULT_VAL                         (0x0f)
#define SYS_ANA_REG20_DACMUTE_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG20_DACDWAMODE_SEL_DEFAULT_VAL               (0x01)
#define SYS_ANA_REG20_DACSEL_DEFAULT_VAL                       (0x0f)

#define SYS_ANA_REG21_LMDCIN_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG21_NC1_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG21_SPIRST_OVC_DEFAULT_VAL                   (0x00)
#define SYS_ANA_REG21_NC0_DEFAULT_VAL                          (0x00)
#define SYS_ANA_REG21_ENIDACL_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG21_DAC3RDHC0V9_DEFAULT_VAL                  (0x00)
#define SYS_ANA_REG21_HC2S_DEFAULT_VAL                         (0x01)
#define SYS_ANA_REG21_RFB_CTRL_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG21_VCMSEL_DEFAULT_VAL                       (0x01)
#define SYS_ANA_REG21_ENBS_DEFAULT_VAL                         (0x00)
#define SYS_ANA_REG21_CALCK_SEL0V9_DEFAULT_VAL                 (0x00)
#define SYS_ANA_REG21_BPDWA0V9_DEFAULT_VAL                     (0x00)
#define SYS_ANA_REG21_LOOPRST0V9_DEFAULT_VAL                   (0x00)
#define SYS_ANA_REG21_OCT0V9_DEFAULT_VAL                       (0x00)
#define SYS_ANA_REG21_SOUT0V9_DEFAULT_VAL                      (0x00)
#define SYS_ANA_REG21_HC0V9_DEFAULT_VAL                        (0x00)


typedef struct {
	aud_isr_t callback;
	void *param;
} aud_callback_t;


#define AUD_RETURN_ON_NOT_INIT() do {\
		if (!s_aud_driver_is_init) {\
			return BK_ERR_AUD_NOT_INIT;\
		}\
	} while(0)

#define AUD_RETURN_ON_INVALID_ISR_ID(isr_id) do {\
		if ((isr_id) >= SOC_AUD_ISR_NUM) {\
			return BK_ERR_AUD_ISR_ID;\
		}\
	} while(0)

static bool s_aud_driver_is_init = false;
static aud_adc_work_mode_t adc_mode = AUD_ADC_WORK_MODE_NULL;
static aud_callback_t s_aud_isr[SOC_AUD_ISR_NUM] = {NULL};

static void aud_isr(void);

extern void delay(int num);//TODO fix me

static uint32_t ana_reg18_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG18_ISELAUD_DEFAULT_VAL & SYS_ANA_REG18_ISELAUD_MASK) << SYS_ANA_REG18_ISELAUD_POS);
	value |= ((SYS_ANA_REG18_AUDCK_RLCEN1V_DEFAULT_VAL & SYS_ANA_REG18_AUDCK_RLCEN1V_MASK) << SYS_ANA_REG18_AUDCK_RLCEN1V_POS);
	value |= ((SYS_ANA_REG18_LCHCKINVEN1V_DEFAULT_VAL & SYS_ANA_REG18_LCHCKINVEN1V_MASK) << SYS_ANA_REG18_LCHCKINVEN1V_POS);
	value |= ((SYS_ANA_REG18_ENAUDBIAS_DEFAULT_VAL & SYS_ANA_REG18_ENAUDBIAS_MASK) << SYS_ANA_REG18_ENAUDBIAS_POS);
	value |= ((SYS_ANA_REG18_ENADCBIAS_DEFAULT_VAL & SYS_ANA_REG18_ENADCBIAS_MASK) << SYS_ANA_REG18_ENADCBIAS_POS);
	value |= ((SYS_ANA_REG18_ENMICBIAS_DEFAULT_VAL & SYS_ANA_REG18_ENMICBIAS_MASK) << SYS_ANA_REG18_ENMICBIAS_POS);
	value |= ((SYS_ANA_REG18_ADCCKINVEN1V_DEFAULT_VAL & SYS_ANA_REG18_ADCCKINVEN1V_MASK) << SYS_ANA_REG18_ADCCKINVEN1V_POS);
	value |= ((SYS_ANA_REG18_DACFB2ST0V9_DEFAULT_VAL & SYS_ANA_REG18_DACFB2ST0V9_MASK) << SYS_ANA_REG18_DACFB2ST0V9_POS);
	value |= ((SYS_ANA_REG18_NC1_DEFAULT_VAL & SYS_ANA_REG18_NC1_MASK) << SYS_ANA_REG18_NC1_POS);
	value |= ((SYS_ANA_REG18_MICBIAS_TRM_DEFAULT_VAL & SYS_ANA_REG18_MICBIAS_TRM_MASK) << SYS_ANA_REG18_MICBIAS_TRM_POS);
	value |= ((SYS_ANA_REG18_MICBIAS_VOC_DEFAULT_VAL & SYS_ANA_REG18_MICBIAS_VOC_MASK) << SYS_ANA_REG18_MICBIAS_VOC_POS);
	value |= ((SYS_ANA_REG18_VREFSEL1V_DEFAULT_VAL & SYS_ANA_REG18_VREFSEL1V_MASK) << SYS_ANA_REG18_VREFSEL1V_POS);
	value |= ((SYS_ANA_REG18_CAPSWSPI_DEFAULT_VAL & SYS_ANA_REG18_CAPSWSPI_MASK) << SYS_ANA_REG18_CAPSWSPI_POS);
	value |= ((SYS_ANA_REG18_ADREF_SEL_DEFAULT_VAL & SYS_ANA_REG18_ADREF_SEL_MASK) << SYS_ANA_REG18_ADREF_SEL_POS);
	value |= ((SYS_ANA_REG18_NC0_DEFAULT_VAL & SYS_ANA_REG18_NC0_MASK) << SYS_ANA_REG18_NC0_POS);
	value |= ((SYS_ANA_REG18_RESERVED_BIT_26_30_DEFAULT_VAL & SYS_ANA_REG18_RESERVED_BIT_26_30_MASK) << SYS_ANA_REG18_RESERVED_BIT_26_30_POS);
	value |= ((SYS_ANA_REG18_SPI_DACCKPSSEL_DEFAULT_VAL & SYS_ANA_REG18_SPI_DACCKPSSEL_MASK) << SYS_ANA_REG18_SPI_DACCKPSSEL_POS);

	return value;
}

static uint32_t ana_reg19_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG19_ISEL_DEFAULT_VAL & SYS_ANA_REG19_ISEL_MASK) << SYS_ANA_REG19_ISEL_POS);
	value |= ((SYS_ANA_REG19_MICIRSEL1_DEFAULT_VAL & SYS_ANA_REG19_MICIRSEL1_MASK) << SYS_ANA_REG19_MICIRSEL1_POS);
	value |= ((SYS_ANA_REG19_MICDACIT_DEFAULT_VAL & SYS_ANA_REG19_MICDACIT_MASK) << SYS_ANA_REG19_MICDACIT_POS);
	value |= ((SYS_ANA_REG19_MICDACIH_DEFAULT_VAL & SYS_ANA_REG19_MICDACIH_MASK) << SYS_ANA_REG19_MICDACIH_POS);
	value |= ((SYS_ANA_REG19_MICSINGLEEN_DEFAULT_VAL & SYS_ANA_REG19_MICSINGLEEN_MASK) << SYS_ANA_REG19_MICSINGLEEN_POS);
	value |= ((SYS_ANA_REG19_DCCOMPEN_DEFAULT_VAL & SYS_ANA_REG19_DCCOMPEN_MASK) << SYS_ANA_REG19_DCCOMPEN_POS);
	value |= ((SYS_ANA_REG19_MICGAIN_DEFAULT_VAL & SYS_ANA_REG19_MICGAIN_MASK) << SYS_ANA_REG19_MICGAIN_POS);
	value |= ((SYS_ANA_REG19_MICDACEN_DEFAULT_VAL & SYS_ANA_REG19_MICDACEN_MASK) << SYS_ANA_REG19_MICDACEN_POS);
	value |= ((SYS_ANA_REG19_STG2LSEN1V_DEFAULT_VAL & SYS_ANA_REG19_STG2LSEN1V_MASK) << SYS_ANA_REG19_STG2LSEN1V_POS);
	value |= ((SYS_ANA_REG19_OPENLOOPCAL1V_DEFAULT_VAL & SYS_ANA_REG19_OPENLOOPCAL1V_MASK) << SYS_ANA_REG19_OPENLOOPCAL1V_POS);
	value |= ((SYS_ANA_REG19_CALLATCH_DEFAULT_VAL & SYS_ANA_REG19_CALLATCH_MASK) << SYS_ANA_REG19_CALLATCH_POS);
	value |= ((SYS_ANA_REG19_VCMSEL_DEFAULT_VAL & SYS_ANA_REG19_VCMSEL_MASK) << SYS_ANA_REG19_VCMSEL_POS);
	value |= ((SYS_ANA_REG19_DWAMODE_DEFAULT_VAL & SYS_ANA_REG19_DWAMODE_MASK) << SYS_ANA_REG19_DWAMODE_POS);
	value |= ((SYS_ANA_REG19_R2REN_DEFAULT_VAL & SYS_ANA_REG19_R2REN_MASK) << SYS_ANA_REG19_R2REN_POS);
	value |= ((SYS_ANA_REG19_NC_26_27_DEFAULT_VAL & SYS_ANA_REG19_NC_26_27_MASK) << SYS_ANA_REG19_NC_26_27_POS);
	value |= ((SYS_ANA_REG19_MICEN_DEFAULT_VAL & SYS_ANA_REG19_MICEN_MASK) << SYS_ANA_REG19_MICEN_POS);
	value |= ((SYS_ANA_REG19_RST_DEFAULT_VAL & SYS_ANA_REG19_RST_MASK) << SYS_ANA_REG19_RST_POS);
	value |= ((SYS_ANA_REG19_BPDWA1V_DEFAULT_VAL & SYS_ANA_REG19_BPDWA1V_MASK) << SYS_ANA_REG19_BPDWA1V_POS);
	value |= ((SYS_ANA_REG19_HCEN1STG_DEFAULT_VAL & SYS_ANA_REG19_HCEN1STG_MASK) << SYS_ANA_REG19_HCEN1STG_POS);

	return value;
}

static uint32_t ana_reg20_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG20_HPDAC_DEFAULT_VAL & SYS_ANA_REG20_HPDAC_MASK) << SYS_ANA_REG20_HPDAC_POS);
	value |= ((SYS_ANA_REG20_CALCON_SEL_DEFAULT_VAL & SYS_ANA_REG20_CALCON_SEL_MASK) << SYS_ANA_REG20_CALCON_SEL_POS);
	value |= ((SYS_ANA_REG20_OSCDAC_DEFAULT_VAL & SYS_ANA_REG20_OSCDAC_MASK) << SYS_ANA_REG20_OSCDAC_POS);
	value |= ((SYS_ANA_REG20_OCENDAC_DEFAULT_VAL & SYS_ANA_REG20_OCENDAC_MASK) << SYS_ANA_REG20_OCENDAC_POS);
	value |= ((SYS_ANA_REG20_VCMSEL_DEFAULT_VAL & SYS_ANA_REG20_VCMSEL_MASK) << SYS_ANA_REG20_VCMSEL_POS);
	value |= ((SYS_ANA_REG20_ADJDACREF_DEFAULT_VAL & SYS_ANA_REG20_ADJDACREF_MASK) << SYS_ANA_REG20_ADJDACREF_POS);
	value |= ((SYS_ANA_REG20_DCOCHG_DEFAULT_VAL & SYS_ANA_REG20_DCOCHG_MASK) << SYS_ANA_REG20_DCOCHG_POS);
	value |= ((SYS_ANA_REG20_DIFFEN_DEFAULT_VAL & SYS_ANA_REG20_DIFFEN_MASK) << SYS_ANA_REG20_DIFFEN_POS);
	value |= ((SYS_ANA_REG20_ENDACCAL_DEFAULT_VAL & SYS_ANA_REG20_ENDACCAL_MASK) << SYS_ANA_REG20_ENDACCAL_POS);
	value |= ((SYS_ANA_REG20_NC2_DEFAULT_VAL & SYS_ANA_REG20_NC2_MASK) << SYS_ANA_REG20_NC2_POS);
	value |= ((SYS_ANA_REG20_LENDCOC_DEFAULT_VAL & SYS_ANA_REG20_LENDCOC_MASK) << SYS_ANA_REG20_LENDCOC_POS);
	value |= ((SYS_ANA_REG20_NC1_DEFAULT_VAL & SYS_ANA_REG20_NC1_MASK) << SYS_ANA_REG20_NC1_POS);
	value |= ((SYS_ANA_REG20_LENVCMD_DEFAULT_VAL & SYS_ANA_REG20_LENVCMD_MASK) << SYS_ANA_REG20_LENVCMD_POS);
	value |= ((SYS_ANA_REG20_DACDRVEN_DEFAULT_VAL & SYS_ANA_REG20_DACDRVEN_MASK) << SYS_ANA_REG20_DACDRVEN_POS);
	value |= ((SYS_ANA_REG20_NC0_DEFAULT_VAL & SYS_ANA_REG20_NC0_MASK) << SYS_ANA_REG20_NC0_POS);
	value |= ((SYS_ANA_REG20_DACLEN_DEFAULT_VAL & SYS_ANA_REG20_DACLEN_MASK) << SYS_ANA_REG20_DACLEN_POS);
	value |= ((SYS_ANA_REG20_DACG_DEFAULT_VAL & SYS_ANA_REG20_DACG_MASK) << SYS_ANA_REG20_DACG_POS);
	value |= ((SYS_ANA_REG20_DACMUTE_DEFAULT_VAL & SYS_ANA_REG20_DACMUTE_MASK) << SYS_ANA_REG20_DACMUTE_POS);
	value |= ((SYS_ANA_REG20_DACDWAMODE_SEL_DEFAULT_VAL & SYS_ANA_REG20_DACDWAMODE_SEL_MASK) << SYS_ANA_REG20_DACDWAMODE_SEL_POS);
	value |= ((SYS_ANA_REG20_DACSEL_DEFAULT_VAL & SYS_ANA_REG20_DACSEL_MASK) << SYS_ANA_REG20_DACSEL_POS);

	return value;
}

static uint32_t ana_reg21_value_cal(void)
{
	uint32_t value = 0;

	value |= ((SYS_ANA_REG21_LMDCIN_DEFAULT_VAL & SYS_ANA_REG21_LMDCIN_MASK) << SYS_ANA_REG21_LMDCIN_POS);
	value |= ((SYS_ANA_REG21_NC1_DEFAULT_VAL & SYS_ANA_REG21_NC1_MASK) << SYS_ANA_REG21_NC1_POS);
	value |= ((SYS_ANA_REG21_SPIRST_OVC_DEFAULT_VAL & SYS_ANA_REG21_SPIRST_OVC_MASK) << SYS_ANA_REG21_SPIRST_OVC_POS);
	value |= ((SYS_ANA_REG21_NC0_DEFAULT_VAL & SYS_ANA_REG21_NC0_MASK) << SYS_ANA_REG21_NC0_POS);
	value |= ((SYS_ANA_REG21_ENIDACL_DEFAULT_VAL & SYS_ANA_REG21_ENIDACL_MASK) << SYS_ANA_REG21_ENIDACL_POS);
	value |= ((SYS_ANA_REG21_DAC3RDHC0V9_DEFAULT_VAL & SYS_ANA_REG21_DAC3RDHC0V9_MASK) << SYS_ANA_REG21_DAC3RDHC0V9_POS);
	value |= ((SYS_ANA_REG21_HC2S_DEFAULT_VAL & SYS_ANA_REG21_HC2S_MASK) << SYS_ANA_REG21_HC2S_POS);
	value |= ((SYS_ANA_REG21_RFB_CTRL_DEFAULT_VAL & SYS_ANA_REG21_RFB_CTRL_MASK) << SYS_ANA_REG21_RFB_CTRL_POS);
	value |= ((SYS_ANA_REG21_VCMSEL_DEFAULT_VAL & SYS_ANA_REG21_VCMSEL_MASK) << SYS_ANA_REG21_VCMSEL_POS);
	value |= ((SYS_ANA_REG21_ENBS_DEFAULT_VAL & SYS_ANA_REG21_ENBS_MASK) << SYS_ANA_REG21_ENBS_POS);
	value |= ((SYS_ANA_REG21_CALCK_SEL0V9_DEFAULT_VAL & SYS_ANA_REG21_CALCK_SEL0V9_MASK) << SYS_ANA_REG21_CALCK_SEL0V9_POS);
	value |= ((SYS_ANA_REG21_BPDWA0V9_DEFAULT_VAL & SYS_ANA_REG21_BPDWA0V9_MASK) << SYS_ANA_REG21_BPDWA0V9_POS);
	value |= ((SYS_ANA_REG21_LOOPRST0V9_DEFAULT_VAL & SYS_ANA_REG21_LOOPRST0V9_MASK) << SYS_ANA_REG21_LOOPRST0V9_POS);
	value |= ((SYS_ANA_REG21_OCT0V9_DEFAULT_VAL & SYS_ANA_REG21_OCT0V9_MASK) << SYS_ANA_REG21_OCT0V9_POS);
	value |= ((SYS_ANA_REG21_SOUT0V9_DEFAULT_VAL & SYS_ANA_REG21_SOUT0V9_MASK) << SYS_ANA_REG21_SOUT0V9_POS);
	value |= ((SYS_ANA_REG21_HC0V9_DEFAULT_VAL & SYS_ANA_REG21_HC0V9_MASK) << SYS_ANA_REG21_HC0V9_POS);

	return value;
}

bk_err_t bk_aud_driver_init(void)
{
	if (s_aud_driver_is_init)
		return BK_OK;

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_AUDIO, PM_POWER_MODULE_STATE_ON);
	sys_drv_aud_select_clock(0);
	bk_pm_clock_ctrl(PM_CLK_ID_AUDIO, CLK_PWR_CTRL_PWR_UP);

	aud_int_config_t int_config_table = {INT_SRC_AUDIO, aud_isr};
	bk_int_isr_register(int_config_table.int_src, int_config_table.isr, NULL);

	/*init audio paramters*/
	sys_hal_aud_int_en(1);
/*
	sys_hal_set_ana_reg18_value(0x00BF8085);
	sys_hal_set_ana_reg19_value(0x81800006);
	sys_hal_set_ana_reg20_value(0xFBC02423);
	sys_hal_set_ana_reg21_value(0x00500000);
*/
	sys_hal_set_ana_reg18_value(ana_reg18_value_cal());
	sys_hal_set_ana_reg19_value(ana_reg19_value_cal());
	sys_hal_set_ana_reg20_value(ana_reg20_value_cal());
	sys_hal_set_ana_reg21_value(ana_reg21_value_cal());

	s_aud_driver_is_init = true;
	adc_mode = AUD_ADC_WORK_MODE_NULL;
	return BK_OK;
}

bk_err_t bk_aud_driver_deinit(void)
{
	if (!s_aud_driver_is_init) {
		return BK_OK;
	}

	//disable adc and dac interrupt
	aud_hal_adc_int_disable();
	aud_hal_dac_int_disable();

	//reset audo configure
	bk_aud_adc_deinit();
	bk_aud_dac_deinit();

	//disable audio interrupt
	sys_hal_aud_int_en(0);
	//ungister isr
	aud_int_config_t int_config_table = {INT_SRC_AUDIO, aud_isr};
	bk_int_isr_unregister(int_config_table.int_src);

	// config analog register
	sys_hal_aud_clock_en(0);
	sys_hal_set_ana_reg18_value(0);
	sys_hal_set_ana_reg19_value(0);
	sys_hal_set_ana_reg20_value(0);
	sys_hal_set_ana_reg21_value(0);

	sys_drv_aud_audpll_en(0);
	bk_pm_clock_ctrl(PM_CLK_ID_AUDIO, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_AUDIO, PM_POWER_MODULE_STATE_OFF);

	s_aud_driver_is_init = false;

	return BK_OK;
}

/*DAC APIs*/

/* get dac fifo port address */
bk_err_t bk_aud_get_dac_fifo_addr(uint32_t *dac_fifo_addr)
{
	*dac_fifo_addr = AUD_DAC_FIFO_PORT;
	return BK_OK;
}

bk_err_t bk_aud_set_dac_samp_rate(aud_dac_samp_rate_t samp_rate)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_dac_set_sample_rate(samp_rate);

	return BK_OK;
}

bk_err_t bk_aud_set_dac_gain(uint32_t value)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_set_dac_config0_dac_set_gain(value);
	return BK_OK;
}

bk_err_t bk_aud_set_dac_chl(aud_dac_chl_enable_t dac_chl)
{
	AUD_RETURN_ON_NOT_INIT();
	os_printf("only support DAC L\r\n");

	return BK_OK;
}

bk_err_t bk_aud_dac_write(uint32_t pcm_value)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_dac_write(pcm_value);

	return BK_OK;
}

bk_err_t bk_aud_set_dac_read_threshold(uint16_t dacl_throld, uint16_t dacr_throld)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_dacr_rd_threshold(dacr_throld);
	aud_hal_set_fifo_config_dacl_rd_threshold(dacl_throld);

	return BK_OK;
}

bk_err_t bk_aud_enable_dac_int(void)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_dac_int_enable();

	return BK_OK;
}

bk_err_t bk_aud_disable_dac_int(void)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_dac_int_disable();

	return BK_OK;
}

bk_err_t bk_aud_get_dac_status(uint32_t *dac_status)
{
	AUD_RETURN_ON_NOT_INIT();

	*dac_status = aud_hal_get_fifo_status_value();

	return BK_OK;
}

bk_err_t bk_aud_start_dac(void)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dac_enable(1);

	return BK_OK;
}

bk_err_t bk_aud_stop_dac(void)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dac_enable(0);

	return BK_OK;
}

bk_err_t bk_aud_dac_init(const aud_dac_config_t *dac_config)
{
	AUD_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(dac_config);

	/*active dac*/
	sys_hal_aud_dcoc_en(1);
	//sys_hal_aud_lmdcin_set(0x80);
	sys_hal_aud_audbias_en(1);
	sys_hal_aud_bias_en(1);
	sys_hal_aud_idac_en(1);
	sys_hal_aud_dacdrv_en(1);

	//enable dacl and dacr
	switch (dac_config->dac_chl) {
		case AUD_DAC_CHL_L_ENABLE:
			sys_drv_aud_dacl_en(1);
			break;

		case AUD_DAC_CHL_R_ENABLE:
			os_printf("only support DAC L\r\n");
			break;

		case AUD_DAC_CHL_LR_ENABLE:
			sys_drv_aud_dacl_en(1);
			break;

		default:
			break;
	}

	//set dac work mode
	if (dac_config->work_mode == AUD_DAC_WORK_MODE_SIGNAL_END) {
		sys_hal_aud_diffen_en(0);
	} else if (dac_config->work_mode == AUD_DAC_WORK_MODE_DIFFEN) {
		sys_hal_aud_diffen_en(1);
	} else {
		return BK_FAIL;
	}

	aud_hal_dac_config(dac_config);

	return BK_OK;
}

bk_err_t bk_aud_dac_deinit(void)
{
	aud_dac_config_t dac_config;

	dac_config.dac_enable = AUD_DAC_DISABLE;
	dac_config.samp_rate = AUD_DAC_SAMP_RATE_8K;
	dac_config.dac_hpf2_coef_B2 = 0x0;
	dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_DISABLE;
	dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_DISABLE;
	dac_config.dac_set_gain = 0x0;
	dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

	dac_config.dac_hpf2_coef_B0 = 0x0;
	dac_config.dac_hpf2_coef_B1 = 0x0;

	dac_config.dac_hpf2_coef_A1 = 0x0;
	dac_config.dac_hpf2_coef_A2 = 0x0;

	dac_config.dacr_rd_threshold = 0x0;
	dac_config.dacl_rd_threshold = 0x0;
	dac_config.dacr_int_enable = 0x0;
	dac_config.dacl_int_enable = 0x0;

	dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;

	aud_hal_dac_config(&dac_config);

	//enable dacl and dacr
	sys_hal_aud_dacr_en(0);
	sys_hal_aud_dacl_en(0);

	//config system registers
	sys_drv_aud_dacdrv_en(0);
	sys_hal_aud_idac_en(0);
	sys_drv_aud_bias_en(0);
	sys_hal_aud_audbias_en(0);
	sys_hal_aud_dcoc_en(0);
	sys_hal_aud_lmdcin_set(0);

	return BK_OK;
}

/*ADC APIs*/
bk_err_t bk_aud_set_adc_samp_rate(aud_adc_samp_rate_t samp_rate)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_adc_set_sample_rate(samp_rate);
	return BK_OK;
}

bk_err_t bk_aud_set_adc_gain(uint32_t value)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_set_adc_config0_adc_set_gain(value);
	return BK_OK;
}

bk_err_t bk_aud_set_mic_chl(aud_mic_enable_t mic_chl)
{
	os_printf("Only support mic 1\r\n");
	return BK_OK;
}

bk_err_t bk_aud_set_mic_intf_mode(aud_mic_id_t mic_id, aud_adc_intf_mode_t intf_mode)
{
	AUD_RETURN_ON_NOT_INIT();
	if (adc_mode != AUD_ADC_WORK_MODE_ADC) {		return BK_FAIL;
	}

	uint32_t intf_value = 0;

	if (intf_mode == AUD_ADC_INTF_MODE_SIGNAL_END) {
		intf_value = 1;
	} else {
		intf_value = 0;
	}

	switch (mic_id) {
		case AUD_MIC_MIC1:
			sys_drv_aud_mic1_single_en(intf_value);
			break;

		case AUD_MIC_MIC2:
			sys_drv_aud_mic2_single_en(intf_value);
			break;

		case AUD_MIC_BOTH:
			sys_drv_aud_mic1_single_en(intf_value);
			sys_drv_aud_mic2_single_en(intf_value);
			break;

		default:
			break;
	}

	return BK_OK;
}

/* get adc fifo port address */
bk_err_t bk_aud_get_adc_fifo_addr(uint32_t *adc_fifo_addr)
{
	*adc_fifo_addr = AUD_ADC_FIFO_PORT;
	return BK_OK;
}

/* get adc fifo data */
bk_err_t bk_aud_get_adc_fifo_data(uint32_t *adc_data)
{
	AUD_RETURN_ON_NOT_INIT();
	*adc_data = aud_hal_get_adc_fport_value();
	return BK_OK;
}

/* get audio adc fifo and agc status */
bk_err_t bk_aud_get_adc_status(uint32_t *adc_status)
{
	AUD_RETURN_ON_NOT_INIT();
	if (adc_mode != AUD_ADC_WORK_MODE_ADC)
		return BK_ERR_AUD_ADC_MODE;

	*adc_status = aud_hal_get_fifo_status_value();
	return BK_OK;
}

/* start adc to dac test */
bk_err_t bk_aud_start_loop_test(void)
{
	bk_err_t ret = BK_OK;
	AUD_RETURN_ON_NOT_INIT();
	switch (adc_mode){
		case AUD_ADC_WORK_MODE_NULL:
			ret = BK_ERR_AUD_ADC_MODE;
			break;
		case AUD_ADC_WORK_MODE_ADC:
			//check if DAC enable
			aud_hal_set_fifo_config_loop_adc2dac(1);
			break;
		case AUD_ADC_WORK_MODE_DTMF:
			//check if DAC enable
			aud_hal_set_fifo_config_loop_ton2dac(1);
			break;
		default:
			break;
	}

	return ret;
}

/* stop adc to dac test */
bk_err_t bk_aud_stop_loop_test(void)
{
	AUD_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_loop_ton2dac(0);
	aud_hal_set_fifo_config_loop_adc2dac(0);
	return BK_OK;
}

/* enable adc interrupt */
bk_err_t bk_aud_enable_adc_int(void)
{
	bk_err_t ret = BK_OK;
	AUD_RETURN_ON_NOT_INIT();
	sys_hal_aud_int_en(0);
	switch (adc_mode){
		case AUD_ADC_WORK_MODE_NULL:
			ret = BK_ERR_AUD_ADC_MODE;
			break;
		case AUD_ADC_WORK_MODE_ADC:
			aud_hal_adc_int_enable();
			break;
		case AUD_ADC_WORK_MODE_DTMF:
			aud_hal_set_fifo_config_dtmf_int_en(1);
			break;
		default:
			break;
	}
	sys_hal_aud_int_en(1);

	return ret;
}

/* disable adc interrupt */
bk_err_t bk_aud_disable_adc_int(void)
{
	bk_err_t ret = BK_OK;
	AUD_RETURN_ON_NOT_INIT();
	switch (adc_mode){
		case AUD_ADC_WORK_MODE_NULL:
			ret = BK_ERR_AUD_ADC_MODE;
			break;
		case AUD_ADC_WORK_MODE_ADC:
			aud_hal_adc_int_disable();
			break;
		case AUD_ADC_WORK_MODE_DTMF:
			aud_hal_set_fifo_config_dtmf_int_en(0);
			break;
		default:
			break;
	}
	return ret;
}

/* enable adc and adc start work */
bk_err_t bk_aud_start_adc(void)
{
	bk_err_t ret = BK_OK;
	AUD_RETURN_ON_NOT_INIT();
	switch (adc_mode){
		case AUD_ADC_WORK_MODE_NULL:
			ret = BK_ERR_AUD_ADC_MODE;
			break;
		case AUD_ADC_WORK_MODE_ADC:
			aud_hal_adc_enable();
			break;
		case AUD_ADC_WORK_MODE_DTMF:
			aud_hal_dtmf_enable();
			break;
		default:
			break;
	}

	return ret;
}

/* disable adc and adc stop work */
bk_err_t bk_aud_stop_adc(void)
{
	bk_err_t ret = BK_OK;
	AUD_RETURN_ON_NOT_INIT();
	switch (adc_mode){
		case AUD_ADC_WORK_MODE_NULL:
			ret = BK_ERR_AUD_ADC_MODE;
			break;
		case AUD_ADC_WORK_MODE_ADC:
			aud_hal_adc_disable();
			break;
		case AUD_ADC_WORK_MODE_DTMF:
			aud_hal_dtmf_disable();
			break;
		default:
			break;
	}

	return ret;
}

bk_err_t bk_aud_adc_init(aud_adc_work_mode_t adc_work_mode, const aud_adc_config_t *adc_config, const aud_dtmf_config_t *dtmf_config)
{
	AUD_RETURN_ON_NOT_INIT();
	if (adc_work_mode >= AUD_ADC_WORK_MODE_MAX)
		return BK_ERR_AUD_ADC_MODE;

	adc_mode = adc_work_mode;

	switch (adc_work_mode) {
		case AUD_ADC_WORK_MODE_NULL:
			break;
		case AUD_ADC_WORK_MODE_ADC:
			BK_RETURN_ON_NULL(adc_config);

			//enable audio adc power
			sys_hal_aud_audbias_en(1);
			sys_hal_aud_adcbias_en(1);
			sys_hal_aud_micbias_en(1);

			//enable mic1 and mic2
			switch (adc_config->mic_config) {
				case AUD_MIC_ALL_ENABLE:
					sys_hal_aud_mic1_en(1);
					break;

				case AUD_MIC_MIC1_ENABLE:
					sys_hal_aud_mic1_en(1);
					break;

				case AUD_MIC_MIC2_ENABLE:
					os_printf("Do NOT support mic 2.\r\n");
					break;

				default:
					break;
			}

			//reset mic after configuring parameters
			sys_hal_aud_mic_rst_set(1);
			delay(10);
			sys_hal_aud_mic_rst_set(0);

			AUD_LOGI("configure mic and adc\r\n");
			aud_hal_adc_config(adc_config);
			break;
		case AUD_ADC_WORK_MODE_DTMF:
			BK_RETURN_ON_NULL(dtmf_config);
			aud_hal_dtmf_config(dtmf_config);
			break;
		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_aud_adc_deinit(void)
{
	adc_mode = AUD_ADC_WORK_MODE_NULL;
	aud_adc_config_t adc_config;
	aud_dtmf_config_t dtmf_config;

	//config audio adc registers to default value
	adc_config.samp_rate = AUD_ADC_SAMP_RATE_8K;
	adc_config.adc_enable = AUD_ADC_DISABLE;
	adc_config.line_enable = AUD_ADC_LINE_DISABLE;
	adc_config.dtmf_enable = AUD_DTMF_DISABLE;
	adc_config.adc_hpf2_coef_B2 = 0;
	adc_config.adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_DISABLE;
	adc_config.adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_DISABLE;
	adc_config.adc_set_gain = 0x0;
	adc_config.adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
	adc_config.adc_hpf2_coef_B0 = 0;
	adc_config.adc_hpf2_coef_B1 = 0;
	adc_config.adc_hpf2_coef_A0 = 0;
	adc_config.adc_hpf2_coef_A1 = 0;
	adc_config.dtmf_wr_threshold = 0;
	adc_config.adcl_wr_threshold = 0;
	adc_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
	adc_config.adcl_int_enable = AUD_ADCL_INT_DISABLE;
	adc_config.loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
	adc_config.agc_noise_thrd = 0;
	adc_config.agc_noise_high = 0;
	adc_config.agc_noise_low = 0;
	adc_config.agc_noise_min = 0;
	adc_config.agc_noise_tout = 0;
	adc_config.agc_high_dur = 0;
	adc_config.agc_low_dur = 0;
	adc_config.agc_min = 0;
	adc_config.agc_max = 0;
	adc_config.agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
	adc_config.agc_ng_enable = AUD_AGC_NG_DISABLE;
	adc_config.agc_decay_time = AUD_AGC_DECAY_TIME_128;
	adc_config.agc_attack_time = AUD_AGC_ATTACK_TIME_8;
	adc_config.agc_high_thrd = 0;
	adc_config.agc_low_thrd = 0;
	adc_config.agc_iir_coef = AUD_AGC_IIR_COEF_1_32;
	adc_config.agc_enable = AUD_AGC_DISABLE;
	adc_config.manual_pga_value = 0;
	adc_config.manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;

	dtmf_config.adc_enable = AUD_ADC_DISABLE;
	dtmf_config.line_enable = AUD_ADC_LINE_DISABLE;
	dtmf_config.dtmf_enable = AUD_DTMF_DISABLE;
	dtmf_config.tone_pattern = AUD_DTMF_TONE_PATTERN_DTMF;
	dtmf_config.tone_mode = AUD_DTMF_TONE_MODE_SIGNAL;
	dtmf_config.tone_pause_time = 0;
	dtmf_config.tone_active_time = 0;
	dtmf_config.tone1_step = 0;
	dtmf_config.tone1_attu = AUD_DTMF_TONE_ATTU_MINUS_1;
	dtmf_config.tone1_enable = AUD_DTMF_TONE_DISABLE;
	dtmf_config.tone2_step = 0;
	dtmf_config.tone1_attu = AUD_DTMF_TONE_ATTU_MINUS_1;
	dtmf_config.tone1_enable = AUD_DTMF_TONE_DISABLE;
	dtmf_config.dtmf_wr_threshold = 0;
	dtmf_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
	dtmf_config.loop_ton2dac = AUD_LOOP_DTMF2DAC_DISABLE;

	aud_hal_adc_config(&adc_config);
	aud_hal_dtmf_config(&dtmf_config);

	sys_hal_aud_mic1_en(0);

	//disable audio adc power
	sys_hal_aud_audbias_en(0);
	sys_hal_aud_adcbias_en(0);
	sys_hal_aud_micbias_en(0);

	return BK_OK;
}


/* get dtmf fifo port address */
bk_err_t bk_aud_get_dtmf_fifo_addr(uint32_t *dtmf_fifo_addr)
{
	*dtmf_fifo_addr = AUD_DTMF_FIFO_PORT;
	return BK_OK;
}

/* get dtmf fifo data */
bk_err_t bk_aud_get_dtmf_fifo_data(uint32_t *dtmf_data)
{
	AUD_RETURN_ON_NOT_INIT();
	*dtmf_data = aud_hal_get_dtmf_fport_value();
	return BK_OK;
}

/* get audio dtmf fifo status */
bk_err_t bk_aud_get_dtmf_status(uint32_t *dtmf_status)
{
	AUD_RETURN_ON_NOT_INIT();
	if (adc_mode != AUD_ADC_WORK_MODE_DTMF)
		return BK_ERR_AUD_ADC_MODE;

	*dtmf_status = aud_hal_get_fifo_status_value();
	return BK_OK;
}

bk_err_t bk_aud_eq_init(const aud_eq_config_t *eq_config)
{
	AUD_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(eq_config);
	aud_hal_set_extend_cfg_filt_enable(1);
	aud_hal_eq_config(eq_config);

	return BK_OK;
}

bk_err_t bk_aud_eq_deinit(aud_eq_config_t *eq_config)
{
	BK_RETURN_ON_NULL(eq_config);

	os_memset((void *)eq_config, 0, sizeof(aud_eq_config_t));
	aud_hal_eq_config(eq_config);
	aud_hal_set_extend_cfg_filt_enable(0);

	return BK_OK;
}

/* register audio interrupt */
bk_err_t bk_aud_register_aud_isr(aud_isr_id_t isr_id, aud_isr_t isr, void *param)
{
	AUD_RETURN_ON_INVALID_ISR_ID(isr_id);
	uint32_t int_level = rtos_disable_int();
	s_aud_isr[isr_id].callback = isr;
	s_aud_isr[isr_id].param = param;
	rtos_enable_int(int_level);

	return BK_OK;
}

/* audio check interrupt flag and excute correponding isr function when enter interrupt */
static void aud_isr_common(void)
{
	uint32_t adcl_int_status = aud_hal_get_fifo_status_adcl_int_flag();
	uint32_t dtmf_int_status = aud_hal_get_fifo_status_dtmf_int_flag();

	uint32_t dacl_int_status = aud_hal_get_fifo_status_dacl_int_flag();
	uint32_t dacr_int_status = aud_hal_get_fifo_status_dacr_int_flag();

	if (adcl_int_status) {
		if (s_aud_isr[AUD_ISR_ADCL].callback) {
			s_aud_isr[AUD_ISR_ADCL].callback(s_aud_isr[AUD_ISR_ADCL].param);
		}
	}

	if (dtmf_int_status) {
		if (s_aud_isr[AUD_ISR_DTMF].callback) {
			s_aud_isr[AUD_ISR_DTMF].callback(s_aud_isr[AUD_ISR_DTMF].param);
		}
	}

	if (dacl_int_status) {
		if (s_aud_isr[AUD_ISR_DACL].callback) {
			s_aud_isr[AUD_ISR_DACL].callback(s_aud_isr[AUD_ISR_DACL].param);
		}
	}

	if (dacr_int_status) {
		if (s_aud_isr[AUD_ISR_DACR].callback) {
			s_aud_isr[AUD_ISR_DACR].callback(s_aud_isr[AUD_ISR_DACR].param);
		}
	}
}

/* audio interrupt enter*/
static void aud_isr(void)
{
	aud_isr_common();
}

