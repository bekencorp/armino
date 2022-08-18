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

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "prro_hw.h"
#include <driver/hal/hal_prro_types.h>
#include <driver/hal/hal_gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PRRO_LL_REG_BASE(_prro_unit_id)    (SOC_PRRO_REG_BASE)

static inline void prro_ll_soft_reset(prro_hw_t *hw)
{
	hw->global_ctrl.soft_reset = 1;
}

static inline uint32_t prro_ll_get_device_id(prro_hw_t *hw)
{
	return hw->dev_id;
}

static inline uint32_t prro_ll_get_version_id(prro_hw_t *hw)
{
	return hw->dev_version;
}

static inline uint32_t prro_ll_get_dev_status(prro_hw_t *hw)
{
	return hw->dev_status;
}

static inline void prro_ll_enable_sresp(prro_hw_t *hw)
{
	hw->ahb_cfg_sresp.v = 0xff;
	hw->apb_cfg_sresp.v = 0x1f;
}

static inline void prro_ll_init(prro_hw_t *hw)
{
	prro_ll_soft_reset(hw);
	prro_ll_enable_sresp(hw);
}

static inline void prro_ll_set_privilege_attribute(prro_hw_t *hw, prro_dev_t dev, prro_privilege_type_t privilege_type)
{
	switch (dev) {
	case PRRO_DEV_AON:
		hw->ahb_cfg_ap.aonp_ahb_aon_ap = privilege_type;
		break;
	case PRRO_DEV_SYSTEM:
		hw->ahb_cfg_ap.aonp_ahb_sys_ap = privilege_type;
		break;
	case PRRO_DEV_FLASH:
		hw->ahb_cfg_ap.aonp_ahb_flash_ap = privilege_type;
		break;
	case PRRO_DEV_GDMA:
		hw->ahb_cfg_ap.bakp_ahb_gdma_ap = privilege_type;
		break;
	case PRRO_DEV_LA:
		hw->ahb_cfg_ap.bakp_ahb_la_ap = privilege_type;
		break;
	case PRRO_DEV_USB:
		hw->ahb_cfg_ap.ahbp_ahb_usb_ap = privilege_type;
		break;
	case PRRO_DEV_CAN:
		hw->ahb_cfg_ap.ahbp_ahb_can_ap = privilege_type;
		break;
	case PRRO_DEV_QSPI0:
		hw->ahb_cfg_ap.ahbp_ahb_qspi0_ap = privilege_type;
		break;
	case PRRO_DEV_QSPI1:
		hw->ahb_cfg_ap.ahbp_ahb_qspi1_ap = privilege_type;
		break;
	case PRRO_DEV_PSRAM:
		hw->ahb_cfg_ap.ahbp_ahb_psram_ap = privilege_type;
		break;
	case PRRO_DEV_FFT:
		hw->ahb_cfg_ap.audp_ahb_fft_ap = privilege_type;
		break;
	case PRRO_DEV_SBC:
		hw->ahb_cfg_ap.audp_ahb_sbc_ap = privilege_type;
		break;
	case PRRO_DEV_JPEG_ENC:
		hw->ahb_cfg_ap.vidp_ahb_jpge_ap = privilege_type;
		break;
	case PRRO_DEV_JPEG_DEC:
		hw->ahb_cfg_ap.vidp_ahb_jpgd_ap = privilege_type;
		break;
	case PRRO_DEV_YUV:
		hw->ahb_cfg_ap.vidp_ahb_yuv_ap = privilege_type;
		break;
	case PRRO_DEV_LCD_DISP:
		hw->ahb_cfg_ap.vidp_ahb_disp_ap = privilege_type;
		break;
	case PRRO_DEV_DMA2D:
		hw->ahb_cfg_ap.vidp_ahb_dmad_ap = privilege_type;
		break;
	case PRRO_DEV_WIFI_MAC:
		hw->ahb_cfg_ap.wifi_mac_ahb_ap = privilege_type;
		break;
	case PRRO_DEV_WIFI_MODEM:
		hw->ahb_cfg_ap.wifi_mac_ahb_ap = privilege_type;
		break;
	case PRRO_DEV_BTDM_AHB:
		hw->ahb_cfg_ap.btdm_ahb_ap = privilege_type;
		break;
	case PRRO_DEV_MBOX0:
		hw->ahb_cfg_ap.mbox0_ahb_ap = privilege_type;
		break;
	case PRRO_DEV_MBOX1:
		hw->ahb_cfg_ap.mbox1_ahb_ap = privilege_type;
		break;

	case PRRO_DEV_WDT:
		hw->apb_cfg_ap.aonp_apb_wdt_ap = privilege_type;
		break;
	case PRRO_DEV_EFUSE:
		hw->apb_cfg_ap.aonp_apb_efs_ap = privilege_type;
		break;
	case PRRO_DEV_MBIC:
		hw->apb_cfg_ap.aonp_apb_mbic_ap = privilege_type;
		break;
	case PRRO_DEV_SDMADC:
		hw->apb_cfg_ap.bakp_apb_sdmadc_ap = privilege_type;
		break;
	case PRRO_DEV_TIMER0:
		hw->apb_cfg_ap.aonp_apb_timr_ap = privilege_type;
		break;
	case PRRO_DEV_TIMER1:
		hw->apb_cfg_ap.bakp_apb_timer1_ap = privilege_type;
		break;
	case PRRO_DEV_PWM0:
		hw->apb_cfg_ap.bakp_apb_pwm_ap = privilege_type;
		break;
	case PRRO_DEV_PWM1:
		hw->apb_cfg_ap.bakp_apb_pwm1_ap = privilege_type;
		break;
	case PRRO_DEV_UART0:
		hw->apb_cfg_ap.aonp_apb_uart_ap = privilege_type;
		break;
	case PRRO_DEV_UART1:
		hw->apb_cfg_ap.bakp_apb_uart1_ap = privilege_type;
		break;
	case PRRO_DEV_UART2:
		hw->apb_cfg_ap.bakp_apb_uart2_ap = privilege_type;
		break;
	case PRRO_DEV_SPI0:
		hw->apb_cfg_ap.aonp_apb_spi0_ap = privilege_type;
		break;
	case PRRO_DEV_SPI1:
		hw->apb_cfg_ap.bakp_apb_spi1_ap = privilege_type;
		break;
	case PRRO_DEV_I2C0:
		hw->apb_cfg_ap.bakp_apb_i2c0_ap = privilege_type;
		break;
	case PRRO_DEV_I2C1:
		hw->apb_cfg_ap.bakp_apb_i2c1_ap = privilege_type;
		break;
	case PRRO_DEV_SARADC:
		hw->apb_cfg_ap.bakp_apb_sadc_ap = privilege_type;
		break;
	case PRRO_DEV_IRDA:
		hw->apb_cfg_ap.bakp_apb_irda_ap = privilege_type;
		break;
	case PRRO_DEV_TRNG:
		hw->apb_cfg_ap.bakp_apb_trng_ap = privilege_type;
		break;
	case PRRO_DEV_SDIO:
		hw->apb_cfg_ap.bakp_apb_sdio_ap = privilege_type;
		break;
	case PRRO_DEV_SLCD:
		hw->apb_cfg_ap.bakp_apb_slcd_ap = privilege_type;
		break;
	case PRRO_DEV_AUDIO:
		hw->apb_cfg_ap.audp_apb_aud_ap = privilege_type;
		break;
	case PRRO_DEV_I2S0:
		hw->apb_cfg_ap.audp_apb_i2s0_ap = privilege_type;
		break;
	case PRRO_DEV_I2S1:
		hw->apb_cfg_ap.audp_apb_i2s1_ap = privilege_type;
		break;
	case PRRO_DEV_I2S2:
		hw->apb_cfg_ap.audp_apb_i2s2_ap = privilege_type;
		break;
	case PRRO_DEV_VIDP:
		hw->apb_cfg_ap.vidp_apb_ap = privilege_type;
		break;
	case PRRO_DEV_BTDM_APB:
		hw->apb_cfg_ap.btdm_apb_ap = privilege_type;
		break;
	default:
		break;
	}
}

static inline void prro_ll_set_gpio_secure_attribute(prro_hw_t *hw, prro_dev_t dev, prro_secure_type_t secure_type)
{
	gpio_id_t gpio_id = dev - PRRO_DEV_GPIO_0;
	if (gpio_id < GPIO_32) {
		if (secure_type == PRRO_NON_SECURE) {
			REG_SET_BIT(&(hw->aon_gio_nonsec0), (1 << gpio_id));
		} else {
			REG_CLR_BIT(&(hw->aon_gio_nonsec0), (1 << gpio_id));
		}
	} else {
		if (secure_type == PRRO_NON_SECURE) {
			REG_SET_BIT(&(hw->aon_gio_nonsec1), (1 << (gpio_id - 32)));
		} else {
			REG_CLR_BIT(&(hw->aon_gio_nonsec1), (1 << (gpio_id - 32)));
		}
	}
}

static inline void prro_ll_set_secure_attribute(prro_hw_t *hw, prro_dev_t dev, prro_secure_type_t secure_type)
{
	switch (dev) {
	case PRRO_DEV_AON:
		hw->ahb_cfg_nsec.aonp_ahb_aon_nsec = secure_type;
		break;
	case PRRO_DEV_SYSTEM:
		hw->ahb_cfg_nsec.aonp_ahb_sys_nsec = secure_type;
		break;
	case PRRO_DEV_FLASH:
		hw->ahb_cfg_nsec.aonp_ahb_flash_nsec = secure_type;
		break;
	case PRRO_DEV_GDMA:
		hw->ahb_cfg_nsec.bakp_ahb_gdma_nsec = secure_type;
		break;
	case PRRO_DEV_LA:
		hw->ahb_cfg_nsec.bakp_ahb_la_nsec = secure_type;
		break;
	case PRRO_DEV_USB:
		hw->ahb_cfg_nsec.ahbp_ahb_usb_nsec = secure_type;
		break;
	case PRRO_DEV_CAN:
		hw->ahb_cfg_nsec.ahbp_ahb_can_nsec = secure_type;
		break;
	case PRRO_DEV_QSPI0:
		hw->ahb_cfg_nsec.ahbp_ahb_qspi0_nsec = secure_type;
		break;
	case PRRO_DEV_QSPI1:
		hw->ahb_cfg_nsec.ahbp_ahb_qspi1_nsec = secure_type;
		break;
	case PRRO_DEV_PSRAM:
		hw->ahb_cfg_nsec.ahbp_ahb_psram_nsec = secure_type;
		break;
	case PRRO_DEV_FFT:
		hw->ahb_cfg_nsec.audp_ahb_fft_nsec = secure_type;
		break;
	case PRRO_DEV_SBC:
		hw->ahb_cfg_nsec.audp_ahb_sbc_nsec = secure_type;
		break;
	case PRRO_DEV_JPEG_ENC:
		hw->ahb_cfg_nsec.vidp_ahb_jpge_nsec = secure_type;
		break;
	case PRRO_DEV_JPEG_DEC:
		hw->ahb_cfg_nsec.vidp_ahb_jpgd_nsec = secure_type;
		break;
	case PRRO_DEV_YUV:
		hw->ahb_cfg_nsec.vidp_ahb_yuv_nsec = secure_type;
		break;
	case PRRO_DEV_LCD_DISP:
		hw->ahb_cfg_nsec.vidp_ahb_disp_nsec = secure_type;
		break;
	case PRRO_DEV_DMA2D:
		hw->ahb_cfg_nsec.vidp_ahb_dmad_nsec = secure_type;
		break;
	case PRRO_DEV_WIFI_MAC:
		hw->ahb_cfg_nsec.wifi_mac_ahb_nsec = secure_type;
		break;
	case PRRO_DEV_WIFI_MODEM:
		hw->ahb_cfg_nsec.wifi_modem_ahb_nsec = secure_type;
		break;
	case PRRO_DEV_BTDM_AHB:
		hw->ahb_cfg_nsec.btdm_ahb_nsec = secure_type;
		break;
	case PRRO_DEV_MBOX0:
		hw->ahb_cfg_nsec.mbox0_ahb_nsec = secure_type;
		break;
	case PRRO_DEV_MBOX1:
		hw->ahb_cfg_nsec.mbox1_ahb_nsec = secure_type;
		break;

	case PRRO_DEV_WDT:
		hw->apb_cfg_nsec.aonp_apb_wdt_nsec = secure_type;
		break;
	case PRRO_DEV_EFUSE:
		hw->apb_cfg_nsec.aonp_apb_efs_nsec = secure_type;
		break;
	case PRRO_DEV_MBIC:
		hw->apb_cfg_nsec.aonp_apb_mbic_nsec = secure_type;
		break;
	case PRRO_DEV_SDMADC:
		hw->apb_cfg_nsec.bakp_apb_sdmadc_nsec = secure_type;
		break;
	case PRRO_DEV_TIMER0:
		hw->apb_cfg_nsec.aonp_apb_timr_nsec = secure_type;
		break;
	case PRRO_DEV_TIMER1:
		hw->apb_cfg_nsec.bakp_apb_timer1_nsec = secure_type;
		break;
	case PRRO_DEV_PWM0:
		hw->apb_cfg_nsec.bakp_apb_pwm_nsec = secure_type;
		break;
	case PRRO_DEV_PWM1:
		hw->apb_cfg_nsec.bakp_apb_pwm1_nsec = secure_type;
		break;
	case PRRO_DEV_UART0:
		hw->apb_cfg_nsec.aonp_apb_uart_nsec = secure_type;
		break;
	case PRRO_DEV_UART1:
		hw->apb_cfg_nsec.bakp_apb_uart1_nsec = secure_type;
		break;
	case PRRO_DEV_UART2:
		hw->apb_cfg_nsec.bakp_apb_uart2_nsec = secure_type;
		break;
	case PRRO_DEV_SPI0:
		hw->apb_cfg_nsec.aonp_apb_spi0_nsec = secure_type;
		break;
	case PRRO_DEV_SPI1:
		hw->apb_cfg_nsec.bakp_apb_spi1_nsec = secure_type;
		break;
	case PRRO_DEV_I2C0:
		hw->apb_cfg_nsec.bakp_apb_i2c0_nsec = secure_type;
		break;
	case PRRO_DEV_I2C1:
		hw->apb_cfg_nsec.bakp_apb_i2c1_nsec = secure_type;
		break;
	case PRRO_DEV_SARADC:
		hw->apb_cfg_nsec.bakp_apb_sadc_nsec = secure_type;
		break;
	case PRRO_DEV_IRDA:
		hw->apb_cfg_nsec.bakp_apb_irda_nsec = secure_type;
		break;
	case PRRO_DEV_TRNG:
		hw->apb_cfg_nsec.bakp_apb_trng_nsec = secure_type;
		break;
	case PRRO_DEV_SDIO:
		hw->apb_cfg_nsec.bakp_apb_sdio_nsec = secure_type;
		break;
	case PRRO_DEV_SLCD:
		hw->apb_cfg_nsec.bakp_apb_slcd_nsec = secure_type;
		break;
	case PRRO_DEV_AUDIO:
		hw->apb_cfg_nsec.audp_apb_aud_nsec = secure_type;
		break;
	case PRRO_DEV_I2S0:
		hw->apb_cfg_nsec.audp_apb_i2s0_nsec = secure_type;
		break;
	case PRRO_DEV_I2S1:
		hw->apb_cfg_nsec.audp_apb_i2s1_nsec = secure_type;
		break;
	case PRRO_DEV_I2S2:
		hw->apb_cfg_nsec.audp_apb_i2s2_nsec = secure_type;
		break;
	case PRRO_DEV_VIDP:
		hw->apb_cfg_nsec.vidp_apb_nsec = secure_type;
		break;
	case PRRO_DEV_BTDM_APB:
		hw->apb_cfg_nsec.btdm_apb_nsec = secure_type;
		break;

	case PRRO_DEV_JPEC_ENC_HNONSEC:
		hw->jpenc_hnonsec.jpenc_hnonsec = secure_type;
		break;
	case PRRO_DEV_JPEC_DEC_HNONSEC:
		hw->jpdec_hnonsec.jpdec_hnonsec = secure_type;
		break;
	case PRRO_DEV_DMA2D_HNONSEC:
		hw->dmad_m_hnonsec.dmad_m_hnonsec = secure_type;
		break;
	case PRRO_DEV_WIFI_MAC_HNONSEC:
		hw->mac_m_hnonsec.mac_m_hnonsec = secure_type;
		break;
	case PRRO_DEV_HSU_HNONSEC:
		hw->hsu_m_hnonsec.hsu_m_hnonsec = secure_type;
		break;
	case PRRO_DEV_BTDM_HNONSEC:
		hw->btdm_m_hnonsec.btdm_m_hnonsec = secure_type;
		break;
	case PRRO_DEV_LA_HNONSEC:
		hw->la_m_hnonsec.la_m_hnonsec = secure_type;
		break;
	case PRRO_DEV_LCD_DISP_HNONSEC:
		hw->disp_m_hnonsec.disp_m_hnonsec = secure_type;
		break;
	case PRRO_DEV_ENC_HNONSEC:
		hw->enc_m_hnonsec.enc_m_hnonsec = secure_type;
		break;
	default:
		break;
	}

	if ((dev >= PRRO_DEV_GPIO_0) && (dev <= PRRO_DEV_GPIO_47)) {
		prro_ll_set_gpio_secure_attribute(hw, dev, secure_type);
	}
}

#ifdef __cplusplus
}
#endif

