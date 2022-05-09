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

#include "sys_hal.h"
#include "sys_ctrl.h"
#include "bk_icu.h"
#include "icu_hal.h"
#include "power_ll.h"
#include "bk_misc.h"
#include "sys_types.h"

static icu_hal_t s_icu_hal;
//static sys_hal_t s_sys_hal;	//TODO:

/**  Platform Start **/
//Platform

//sys_hal_主语(模块名)_谓语(动作:set/get/enable等)_宾语(status/value)
//该函数在每个芯片目录中都有一份

/** Platform PWM Start **/

void sys_hal_pwm_set_clock(uint32_t mode, uint32_t param)
{
#define ICU_PWM_SET_CLK  (SOC_ICU_REG_BASE + 1 * 4)

	uint32_t reg;

	switch(mode)
	{
		case SYS_HAL_PWM_MUX_LPO:
			{
				reg = REG_READ(ICU_PWM_SET_CLK);
				reg |= (1 << (param));
				REG_WRITE(ICU_PWM_SET_CLK, reg);
				break;
			}
		case SYS_HAL_PWM_MUX_PCLK:
			{
				reg = REG_READ(ICU_PWM_SET_CLK);
				reg &= ~(1 << (param));
				REG_WRITE(ICU_PWM_SET_CLK, reg);
				break;
			}
		default:
			break;
	}
}

/** Platform PWM End **/

/**  Platform Start **/
//Platform
bk_err_t sys_hal_init()
{
	s_icu_hal.hw = (icu_hw_t *)SOC_ICU_REG_BASE;

	//TODO:
	//s_sys_hal.hw = SOC_SYS_CTRL_REG_BASE;
	return BK_OK;
}

void sys_hal_flash_set_dco(void)
{
	uint32_t reg = 0;

	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~FLASH_26M_MUX_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);

}

void sys_hal_flash_set_dpll(void)
{
	uint32_t reg = 0;

	reg = REG_READ(SCTRL_CONTROL);
	reg |= FLASH_26M_MUX_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);
}

/*low power feature start*/
void sys_hal_enter_deep_sleep(void * param)
{

}
void sys_hal_enter_normal_sleep(UINT32 peri_clk)
{

}

void sys_hal_enter_normal_wakeup()
{

}
void sys_hal_module_power_ctrl(power_module_name_t module,power_module_state_t power_state)
{

}
void sys_hal_enter_low_voltage(void)
{

}
void sys_hal_touch_wakeup_enable(uint8_t index)
{

}
void sys_hal_usb_wakeup_enable(uint8_t index)
{

}
void sys_hal_rtc_wakeup_enable(uint32_t value)
{

}
void sys_hal_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{

}
uint32_t sys_hal_cpu_clk_div_get(uint32_t core_index)
{
	return 0;
}
void sys_hal_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value)
{

}
uint32_t sys_hal_all_modules_clk_div_get(clk_div_reg_e reg)
{
	return 0;
}
void sys_hal_low_power_hardware_init()
{

}
/*low power feature end*/
void sys_hal_enable_mac_wakeup_source()
{
    UINT32 reg = 0;
	reg = REG_READ(ICU_R_ARM_WAKEUP_EN);
	reg |= MAC_ARM_WAKEUP_EN_BIT;
	REG_WRITE(ICU_R_ARM_WAKEUP_EN, reg);
}

uint32 sys_hal_get_chip_id(void)
{
	uint32 ret = 0;
	ret = REG_READ(SCTRL_CHIP_ID);
	return ret;
}

uint32 sys_hal_get_device_id(void)
{
	uint32 ret = 0;
	ret = REG_READ(SCTRL_DEVICE_ID);
	return ret;
}

int32 sys_hal_dsp_power_down(void);
int32 sys_hal_dsp_power_up(void);
int32 sys_hal_lbus_power_down(void);
int32 sys_hal_lbus_power_up(void);
int32 sys_hal_rtos_idle_sleep(void *param);
int32 sys_hal_rtos_idle_wakeup(void);
int32 sys_hal_rtos_deep_sleep(void *param);
uint32 sys_hal_get_sctrl_retetion(void);
int32 sys_hal_set_sctrl_retetion(uint32 param);


int32 sys_hal_unconditional_mac_down(void);
int32 sys_hal_unconditional_mac_up(void);
int32 sys_hal_fix_dpll_div(void);
int32 sys_hal_set_low_pwr_clk(uint32 param);
int32 sys_hal_set_gadc_sel(uint32 param);
int32 sys_hal_clkgating_disable(uint32 param);
int32 sys_hal_clkgating_enable(uint32 param);

int32 sys_hal_int_disable(uint32 param) //CMD_ICU_INT_DISABLE
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_INT_EN);
	reg &= ~(param);
	REG_WRITE(ICU_R_INT_EN, reg);

	return ret;
}

int32 sys_hal_int_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_INT_EN);
	reg |= (param);
	REG_WRITE(ICU_R_INT_EN, reg);

	return ret;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_disable(uint32 param) //CMD_ICU_INT_DISABLE
{
	return 0;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	return 0;
}

int32 sys_hal_fiq_disable(uint32 param)
{
	int32 ret = 0;

#if (CONFIG_SOC_BK7271)
	uint32 reg = 0;

	reg = REG_READ(ICU_R_FIQ_ENABLE);
	reg &= ~(param);
	REG_WRITE(ICU_R_FIQ_ENABLE, reg);
#endif

	return ret;
}

int32 sys_hal_fiq_enable(uint32 param)
{
	int32 ret = 0;

#if (CONFIG_SOC_BK7271)
	uint32 reg = 0;

	reg = REG_READ(ICU_R_FIQ_ENABLE);
	reg |= (param);
	REG_WRITE(ICU_R_FIQ_ENABLE, reg);
#endif

	return ret;
}

int32 sys_hal_global_int_disable(uint32 param)
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_GLOBAL_INT_EN);
	reg &= ~(param);
	REG_WRITE(ICU_R_GLOBAL_INT_EN, reg);

	return ret;
}

int32 sys_hal_global_int_enable(uint32 param)
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_GLOBAL_INT_EN);
	reg |= (param);
	REG_WRITE(ICU_R_GLOBAL_INT_EN, reg);

	return ret;
}

uint32 sys_hal_get_int_status(void) // CMD_GET_INTR_STATUS
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_INT_STATUS);
	return reg;
}

int32 sys_hal_set_int_status(uint32 param) // CMD_CLR_INTR_STATUS
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_INT_STATUS);
	REG_WRITE(ICU_R_INT_STATUS, (reg | param));
	return 0;
}

uint32 sys_hal_get_fiq_reg_status(void)
{
	uint32 reg = 0;
#if (CONFIG_SOC_BK7271)
	reg = REG_READ(ICU_R_FIQ_STATUS);
#endif
	return reg;
}

uint32 sys_hal_set_fiq_reg_status(uint32 param)
{
	int32 ret = 0;
#if (CONFIG_SOC_BK7271)
	uint32 reg = 0;
	reg = REG_READ(ICU_R_FIQ_STATUS);
	REG_WRITE(ICU_R_FIQ_STATUS, (reg | param));
#endif

	return ret;
}

uint32 sys_hal_get_intr_raw_status(void)
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_INT_RAW_STATUS);
	return reg;
}

uint32 sys_hal_set_intr_raw_status(uint32 param) //CMD_CLR_INTR_RAW_STATUS
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_INT_RAW_STATUS);
	REG_WRITE(ICU_R_INT_RAW_STATUS, (reg | param));

	return ret;
}


int32 sys_hal_conf_pwm_plck(uint32 param);
int32 sys_hal_conf_pwm_lpoclk(uint32 param);
int32 sys_hal_conf_plck_26m(uint32 param);
int32 sys_hal_conf_plck_dco(uint32 param);

int32 sys_hal_set_jtag_mode(uint32 param)
{
	int32 ret = 0;

	if (JTAG_ARM_MODE == (param))
	{
		REG_WRITE(ICU_R_JTAG_SELECT, ICU_V_JTAG_SEL_WR_ARM);
	}
	else if (JTAG_TL410_MODE == (param))
	{
		REG_WRITE(ICU_R_JTAG_SELECT, ICU_V_JTAG_SEL_WR_TL4);
	}

#if (CONFIG_SOC_BK7271)
	else if (JTAG_BT_MODE == (param))
	{
		REG_WRITE(ICU_R_JTAG_SELECT, ICU_V_JTAG_SEL_WR_BT);
	}
#endif

	return ret;
}

uint32 sys_hal_get_jtag_mode(void)
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_JTAG_SELECT);
	return reg;
}

/*clock power control start*/
void sys_hal_clk_pwr_ctrl(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{
	uint32 v = icu_ll_get_peri_clk_pwr_status(s_icu_hal.hw);

	if(CLK_PWR_CTRL_PWR_UP == power_up)
		v &= ~(1 << dev);
	else
		v |= (1 << dev);

	icu_ll_set_peri_clk_pwr(s_icu_hal.hw, v);
}

void sys_hal_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
	uint32 v = icu_ll_get_clk_sel(s_icu_hal.hw);

	if(CLK_SEL_XTL_26M == clk_sel)
		v |= (1 << dev);
	else
		v &= ~(1 << dev);

	icu_ll_set_clk_sel(s_icu_hal.hw, v);
}

dev_clk_select_t sys_hal_get_clk_select(dev_clk_select_id_t dev)
{
	uint32 v = icu_ll_get_clk_sel(s_icu_hal.hw);

	if(v & (1 << dev))
		return CLK_SEL_XTL_26M;
	else
		return CLK_SEL_DCO;
}

//DCO divider is valid for all of the peri-devices.
void sys_hal_set_dco_div(dev_clk_dco_div_t div)
{
	icu_ll_set_dco_div(s_icu_hal.hw, div);
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_hal_get_dco_div(void)
{
	return icu_ll_get_dco_div(s_icu_hal.hw);
}

/*clock power control end*/

/*wake up control start*/
void sys_hal_arm_wakeup_enable(uint32_t param)
{
	uint32_t reg = 0;

	reg = REG_READ(ICU_R_ARM_WAKEUP_EN);
	reg |= param;
	REG_WRITE(ICU_R_ARM_WAKEUP_EN, reg);
}

void sys_hal_arm_wakeup_disable(uint32_t param)
{
	uint32_t reg = 0;

	reg = REG_READ(ICU_R_ARM_WAKEUP_EN);
	reg &= ~(param);
	REG_WRITE(ICU_R_ARM_WAKEUP_EN, reg);
}

uint32_t sys_hal_get_arm_wakeup(void)
{
	uint32_t ret = 0;

	ret = REG_READ(ICU_R_ARM_WAKEUP_EN);
	return ret;
}
/*wake up control end*/
void sys_hal_sadc_int_enable(void)
{
    icu_ll_enable_saradc_interrupt(s_icu_hal.hw);
}

void sys_hal_sadc_int_disable(void)
{
   icu_ll_disable_saradc_interrupt(s_icu_hal.hw);
}

void sys_hal_sadc_pwr_up(void)
{
    power_ll_pwr_up_saradc(s_icu_hal.hw);
}

void sys_hal_sadc_pwr_down(void)
{
    power_ll_pwr_down_saradc(s_icu_hal.hw);
}
/**  Platform End **/


/**  BT Start **/
//BT
void sys_hal_bt_power_ctrl(bool power_up)
{
    UINT32 reg = 0;
    UINT32 reg_val;
    UINT32 reg_word = 0;

    if (power_up)
    {
        reg = SCTRL_USB_PWR;
        reg_val = REG_READ(SCTRL_USB_PWR);
        reg_val &= ~(BLE_PWD_MASK << BLE_PWD_POSI);
        reg_val |= BLE_PWU << BLE_PWD_POSI;
        reg_word = reg_val;
    }
    else
    {
        reg = SCTRL_USB_PWR;
        reg_val = REG_READ(SCTRL_USB_PWR);
        reg_val &= ~(BLE_PWD_MASK << BLE_PWD_POSI);
        reg_val |= BLE_PWD << BLE_PWD_POSI;
        reg_word = reg_val;
    }

    REG_WRITE(reg, reg_word);
}

uint32_t sys_hal_mclk_mux_get(void)
{
    UINT32 reg = 0;

    reg = ((REG_READ(SCTRL_CONTROL) >> MCLK_MUX_POSI) & MCLK_MUX_MASK);

    return reg;
}

uint32_t sys_hal_mclk_div_get(void)
{
    UINT32 reg = 0;

    reg = ((REG_READ(SCTRL_CONTROL) >> MCLK_DIV_POSI) & MCLK_DIV_MASK);

    return reg;
}

void sys_hal_mclk_select(uint32_t value)
{
    UINT32 reg = 0;

    reg = REG_READ(SCTRL_CONTROL);
    reg &= ~(MCLK_MUX_MASK << MCLK_MUX_POSI);
    reg |= (value & MCLK_MUX_MASK) << MCLK_MUX_POSI;

    REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_mclk_div_set(uint32_t value)
{
    UINT32 reg = 0;

    reg = REG_READ(SCTRL_CONTROL);
    reg &= ~(MCLK_DIV_MASK << MCLK_DIV_POSI);
    reg |= (value & MCLK_DIV_MASK) << MCLK_DIV_POSI;

    REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_bt_rf_ctrl(bool en)
{
    UINT32 reg = 0;

    if (en)
    {
        reg = REG_READ(SCTRL_CONTROL);
        reg |= BLE_RF_EN_BIT;
        REG_WRITE(SCTRL_CONTROL, reg);
    }
    else
    {
        reg = REG_READ(SCTRL_CONTROL);
        reg &= ~(BLE_RF_EN_BIT);
        REG_WRITE(SCTRL_CONTROL, reg);
    }
}

uint32_t sys_hal_bt_rf_status_get(void)
{
    UINT32 reg = 0;

    reg = REG_READ(SCTRL_CONTROL);
    return (reg & (BLE_RF_EN_BIT));
}

uint32_t sys_hal_interrupt_status_get(void)
{
    UINT32 reg = 0;

    reg = REG_READ(ICU_R_INT_STATUS);
    return reg;
}

void sys_hal_interrupt_status_set(uint32_t value)
{
    UINT32 reg = 0;

    reg = REG_READ(ICU_R_INT_STATUS);
    REG_WRITE(ICU_R_INT_STATUS, (reg | value));
}

void sys_hal_bt_clock_ctrl(bool en)
{
    if (en)
    {
        power_ll_pwr_up_ble(s_icu_hal.hw);
    }
    else
    {
        power_ll_pwr_down_ble(s_icu_hal.hw);
    }
}

void sys_hal_btdm_interrupt_ctrl(bool en)
{
    if (en)
    {
        icu_ll_enable_btdm_interrupt(s_icu_hal.hw);
    }
    else
    {
        icu_ll_disable_btdm_interrupt(s_icu_hal.hw);
    }
}

void sys_hal_ble_interrupt_ctrl(bool en)
{
    if (en)
    {
        icu_ll_enable_ble_interrupt(s_icu_hal.hw);
    }
    else
    {
        icu_ll_disable_ble_interrupt(s_icu_hal.hw);
    }
}

void sys_hal_bt_sleep_exit_ctrl(bool en)
{
    if (en)
    {
        *(volatile unsigned int *)(SCTRL_BASE + 0xD * 4) |= 0x1 << 27;
    }
    else
    {
        *(volatile unsigned int *)(SCTRL_BASE + 0xD * 4) &= ~(0x1 << 27);
    }
}

/**  BT End **/




/**  Audio Start **/
//Audio
/**  Audio End **/




/**  Video Start **/
//Video
/**  Video End **/




/**  WIFI Start **/
//WIFI
void sys_hal_cali_dpll_spi_trig_disable(void)
{
		UINT32 param;
		param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
		param &= ~(SPI_TRIG_BIT);
		sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);
}


void sys_hal_cali_dpll_spi_trig_enable(void)
{
	UINT32 param;

	param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
	param |= (SPI_TRIG_BIT);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);
}

void sys_hal_cali_dpll_spi_detect_disable(void)
{
	UINT32 param;

	param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
	param &= ~(SPI_DET_EN);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);
}
void sys_hal_cali_dpll_spi_detect_enable(void)
{
	UINT32 param;

	param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
	param |= (SPI_DET_EN);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);

}


uint32_t sys_hal_bias_reg_set(uint32_t param)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_BIAS);
	reg |= (param);
#if (CONFIG_SOC_BK7271)
	sctrl_analog_set(SCTRL_BIAS, reg);
#else
	REG_WRITE(SCTRL_BIAS, reg);
#endif
    return BK_OK;
}




uint32_t sys_hal_bias_reg_clean(uint32_t param)
{

	UINT32 reg;

	reg = REG_READ(SCTRL_BIAS);
	reg &= ~(param);
#if (CONFIG_SOC_BK7271)
	sctrl_analog_set(SCTRL_BIAS, reg);
#else
	REG_WRITE(SCTRL_BIAS, reg);
#endif


    return BK_OK;
}


uint32_t sys_hal_bias_reg_read(void)
{
	UINT32 ret;

	ret = REG_READ(SCTRL_BIAS);
    return ret;
}


uint32_t sys_hal_bias_reg_write(uint32_t param)
{
#if (CONFIG_SOC_BK7271)
		sctrl_analog_set(SCTRL_BIAS, param);
#else
		REG_WRITE(SCTRL_BIAS, param);
#endif
	return BK_OK;

}

uint32_t sys_hal_analog_reg1_get(void)
{
	UINT32 ret;

	ret = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
    return ret;
}


uint32_t sys_hal_analog_reg2_get(void)
{
	UINT32 ret;

	ret = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
    return ret;
}

uint32_t sys_hal_analog_reg4_get(void)
{
	UINT32 ret;

	ret = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
    return ret;
}

uint32_t sys_hal_analog_reg6_get(void)
{
	return 0;
}

uint32_t sys_hal_analog_reg7_get(void)
{
	return 0;
}



void sys_hal_set_ana_reg2_value(uint32_t value)
{
	sctrl_analog_set(SCTRL_ANALOG_CTRL2, (value));
}

void sys_hal_set_ana_reg3_value(uint32_t value)
{
	sctrl_analog_set(SCTRL_ANALOG_CTRL3, (value));
}

void sys_hal_set_ana_reg4_value(uint32_t value)
{
	UINT32 reg;
	reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
	reg |= (value);
	sctrl_analog_set(SCTRL_ANALOG_CTRL4, reg);
}

void sys_hal_set_ana_reg6_value(uint32_t value)
{

}
void sys_hal_set_ana_reg7_value(uint32_t value)
{

}




void sys_hal_set_xtalh_ctune(uint32_t value)
{
	UINT32 reg;

	reg = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
	reg &= ~(XTALH_CTUNE_MASK << XTALH_CTUNE_POSI);
	reg |= (((value) &XTALH_CTUNE_MASK) << XTALH_CTUNE_POSI);
	sctrl_analog_set(SCTRL_ANALOG_CTRL2, reg);
}
void sys_hal_set_ana_reg1_value(uint32_t value)
{
	sctrl_analog_set(SCTRL_ANALOG_CTRL1, (value));
}
void sys_hal_analog_reg4_bits_or(uint32_t value)
{
	UINT32 reg;

	reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
	reg |= (value);
	sctrl_analog_set(SCTRL_ANALOG_CTRL4, reg);
}

//Yantao Add Start
void sys_hal_modem_core_reset(void)
{
	UINT32 ret;
	UINT32 reg;

	ret = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
	ret = ret & (~((MODEM_CORE_RESET_MASK) << MODEM_CORE_RESET_POSI));
	reg = ret | ((MODEM_CORE_RESET_WORD & MODEM_CORE_RESET_MASK)
				 << MODEM_CORE_RESET_POSI);
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

	delay(1);
	reg = ret;
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

	/*resetting, and waiting for done*/
	reg = REG_READ(SCTRL_RESET);
	while (reg & MODEM_CORE_RESET_BIT) {
		delay(10);
		reg = REG_READ(SCTRL_RESET);
	}
}

void sys_hal_mpif_invert(void)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);
	reg |= MPIF_CLK_INVERT_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_modem_subsys_reset(void)
{
 	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_MODEM_SUBCHIP_RESET_REQ;
	reset_word = MODEM_SUBCHIP_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_mac_subsys_reset(void)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_MAC_SUBSYS_RESET_REQ;
	reset_word = MAC_SUBSYS_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_usb_subsys_reset(void)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_USB_SUBSYS_RESET_REQ;
	reset_word = USB_SUBSYS_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_dsp_subsys_reset(void)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_DSP_SUBSYS_RESET_REQ;
	reset_word = DSP_SUBSYS_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_mac_power_ctrl(bool power_up)
{
 	UINT32 reg_val;
 	UINT32 reg_word = 0;

	reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
	reg_val &= ~(MAC_PWD_MASK << MAC_PWD_POSI);

	if (power_up)
 	{
		reg_val |= MAC_PWU << MAC_PWD_POSI;
	}
	else
	{
		reg_val |= MAC_PWD << MAC_PWD_POSI;
	}
	reg_word = reg_val;
	REG_WRITE(SCTRL_PWR_MAC_MODEM, reg_word);
}

void sys_hal_modem_power_ctrl(bool power_up)
{
	UINT32 reg_val;
	UINT32 reg_word = 0;

	reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
	reg_val &= ~(MODEM_PWD_MASK << MODEM_PWD_POSI);

	if (power_up)
	{
		reg_val |= MODEM_PWU << MODEM_PWD_POSI;
	}
	else
	{
		reg_val |= MODEM_PWD << MODEM_PWD_POSI;
	}
	reg_word = reg_val;
	REG_WRITE(SCTRL_PWR_MAC_MODEM, reg_word);
}

void sys_hal_pta_ctrl(bool pta_en)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);
	if (pta_en)
	{
		reg |= (BLE_RF_PTA_EN_BIT);
	}
	else
	{
		reg &= ~(BLE_RF_PTA_EN_BIT);
	}
	REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_modem_bus_clk_ctrl(bool clk_en)
{
	UINT32 reg;

#if (CONFIG_SOC_BK7271)
	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg |= PHY_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~PHY_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);

	if (clk_en)
	{
		reg |= PHY_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~PHY_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif
}

void sys_hal_modem_clk_ctrl(bool clk_en)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg &= ~MODEM_CLK480M_PWD_BIT;
	}
	else
	{
		reg |= MODEM_CLK480M_PWD_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_mac_bus_clk_ctrl(bool clk_en)
{
	UINT32 reg;

#if (CONFIG_SOC_BK7271)
	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg |= MAC_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~MAC_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);

	if (clk_en)
	{
		reg |= MAC_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~MAC_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif

}

void sys_hal_mac_clk_ctrl(bool clk_en)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg &= ~MAC_CLK480M_PWD_BIT;
	}
	else
	{
		reg |= MAC_CLK480M_PWD_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
}


void sys_hal_set_vdd_value(uint32_t param)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_DIGTAL_VDD);
	reg &= (~(DIG_VDD_ACTIVE_MASK << DIG_VDD_ACTIVE_POSI));
	reg |= param << DIG_VDD_ACTIVE_POSI;
	REG_WRITE(SCTRL_DIGTAL_VDD, reg);
}

uint32_t sys_hal_get_vdd_value(void)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_DIGTAL_VDD);
	return (reg >> DIG_VDD_ACTIVE_POSI) & DIG_VDD_ACTIVE_MASK;
}

//CMD_SCTRL_BLOCK_EN_MUX_SET
void sys_hal_block_en_mux_set(uint32_t param)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_BLOCK_EN_MUX);
	reg &= ~(0x1FF);
	reg |= param;
	REG_WRITE(SCTRL_BLOCK_EN_MUX, reg);
}
void sys_hal_enable_mac_gen_int(void)
{
	icu_ll_enable_mac_general_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_prot_int(void)
{
	icu_ll_enable_mac_prot_trigger_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_tx_trigger_int(void)
{
	icu_ll_enable_mac_tx_trigger_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_rx_trigger_int(void)
{
	icu_ll_enable_mac_rx_trigger_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_txrx_misc_int(void)
{
	icu_ll_enable_mac_txrx_misc_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_txrx_timer_int(void)
{
	icu_ll_enable_mac_txrx_timer_interrupt(s_icu_hal.hw);
}

void sys_hal_enable_modem_int(void)
{
	icu_ll_enable_modem_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_modem_rc_int(void)
{
	//only 7236 has modem rc interrupt
}


//Yantao Add End


/**  WIFI End **/




/**  Misc Start **/
//Misc
/**  Misc End **/


