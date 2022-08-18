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
#include "pwm_hw.h"
#include <driver/hal/hal_gpio_types.h>
#include <common/bk_generic.h>



#ifdef __cplusplus
extern "C" {
#endif

#define ICU_LL_REG_BASE() (SOC_ICU_REG_BASE)

static inline void icu_ll_set_jtag_wr_wifi(icu_hw_t *hw)
{
	hw->jtag_sel = 0x7171F00F;
}

static inline void icu_ll_set_jtag_wr_bt(icu_hw_t *hw)
{
	hw->jtag_sel = 0x7171B00B;
}

static inline void icu_ll_set_jtag_wr_dsp(icu_hw_t *hw)
{
	hw->jtag_sel = 0x7171D00D;
}

static inline void icu_ll_set_jtag_rd_wifi(icu_hw_t *hw)
{
	hw->jtag_sel = 0;
}

static inline void icu_ll_set_jtag_rd_bt(icu_hw_t *hw)
{
	hw->jtag_sel = 1;
}

static inline void icu_ll_set_jtag_rd_dsp(icu_hw_t *hw)
{
	hw->jtag_sel = 2;
}

static inline void icu_ll_enable_uart_interrupt(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->irq_en, BIT(id));
}

static inline void icu_ll_disable_uart_interrupt(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->irq_en, BIT(id));
}

static inline void icu_ll_enable_i2c_interrupt(icu_hw_t *hw, uint32_t id)
{
	if (id == 0) {
		hw->irq_en.fm_i2c = 1;
	} else if (id == 1) {
		hw->irq_en.i2c1 = 1;
	} else {
		hw->irq_en.i2c2 = 1;
	}
}

static inline void icu_ll_disable_i2c_interrupt(icu_hw_t *hw, uint32_t id)
{
	if (id == 0) {
		hw->irq_en.fm_i2c = 0;
	} else if (id == 1) {
		hw->irq_en.i2c1 = 0;
	} else {
		hw->irq_en.i2c2 = 0;
	}
}

static inline void icu_ll_enable_sdio_interrupt(icu_hw_t *hw)
{
	hw->irq_en.sdio = 1;
}

static inline void icu_ll_disable_sdio_interrupt(icu_hw_t *hw)
{
	hw->irq_en.sdio = 0;
}

static inline void icu_ll_enable_usb_interrupt(icu_hw_t *hw, uint32_t id)
{
	if (id == 1)
		hw->irq_en.usb1 = 1;
	else
		hw->irq_en.usb2 = 1;
}

static inline void icu_ll_disable_usb_interrupt(icu_hw_t *hw, uint32_t id)
{
	if (id == 1)
		hw->irq_en.usb1 = 0;
	else
		hw->irq_en.usb2 = 0;
}

static inline void icu_ll_enable_spi_interrupt(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->irq_en, BIT(id + ICU_F_SPI_CHAN_INT_S));
}

static inline void icu_ll_disable_spi_interrupt(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->irq_en, BIT(id + ICU_F_SPI_CHAN_INT_S));
}

static inline void icu_ll_enable_pwm2_interrupt(icu_hw_t *hw)
{
	hw->irq_en.pwm2 = 1;
}

static inline void icu_ll_disable_pwm2_interrupt(icu_hw_t *hw)
{
	hw->irq_en.pwm2 = 0;
}

static inline void icu_ll_enable_pwm_interrupt(icu_hw_t *hw)
{
	hw->irq_en.pwm1 = 1;
}

static inline void icu_ll_disable_pwm_interrupt(icu_hw_t *hw)
{
	hw->irq_en.pwm1 = 0;
}

static inline bool icu_ll_is_pwm_has_two_int(icu_hw_t *hw)
{
	return true;
}

static inline void icu_ll_enable_gpio_interrupt(icu_hw_t *hw)
{
	hw->irq_en.gpio = 1;
}

static inline void icu_ll_disable_gpio_interrupt(icu_hw_t *hw)
{
	hw->irq_en.gpio = 0;
}

static inline void icu_ll_enable_irda_interrupt(icu_hw_t *hw)
{
	hw->irq_en.irda = 1;
}

static inline void icu_ll_disable_irda_interrupt(icu_hw_t *hw)
{
	hw->irq_en.irda = 0;
}

static inline void icu_ll_enable_timer_interrupt(icu_hw_t *hw)
{
	hw->irq_en.timer = 1;
}

static inline void icu_ll_disable_timer_interrupt(icu_hw_t *hw)
{
	hw->irq_en.timer = 0;
}

static inline void icu_ll_enable_saradc_interrupt(icu_hw_t *hw)
{
	hw->irq_en.saradc = 1;
}

static inline void icu_ll_disable_saradc_interrupt(icu_hw_t *hw)
{
	hw->irq_en.saradc = 0;
}

static inline void icu_ll_enable_psram_interrupt(icu_hw_t *hw)
{
	hw->irq_en.qspi = 1;
}
static inline void icu_ll_disable_psram_interrupt(icu_hw_t *hw)
{
	hw->irq_en.qspi = 0;
}

static inline void icu_ll_enable_sdio_dma_interrupt(icu_hw_t *hw)
{
	hw->irq_en.sdio_dma = 1;
}

static inline void icu_ll_disable_sdio_dma_interrupt(icu_hw_t *hw)
{
	hw->irq_en.sdio_dma = 0;
}

static inline void icu_ll_enable_dma_interrupt(icu_hw_t *hw)
{
	hw->irq_en.gdma = 1;
}

static inline void icu_ll_disable_dma_interrupt(icu_hw_t *hw)
{
	hw->irq_en.gdma = 0;
}

static inline void icu_ll_enable_mailbox_dsp_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mbox1 = 1;
}

static inline void icu_ll_disable_mailbox_dsp_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mbox1 = 0;
}

static inline void icu_ll_enable_mailbox_bt_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mbox2 = 1;
}

static inline void icu_ll_disable_mailbox_bt_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mbox2 = 0;
}

static inline void icu_ll_enable_modem_interrupt(icu_hw_t *hw)
{
	hw->irq_en.modem = 1;
}

static inline void icu_ll_disable_modem_interrupt(icu_hw_t *hw)
{
	hw->irq_en.modem = 0;
}

static inline void icu_ll_enable_mac_txrx_timer_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_txrx_timer = 1;
}

static inline void icu_ll_disable_mac_txrx_timer_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_txrx_timer = 0;
}

static inline void icu_ll_enable_mac_txrx_misc_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_txrx_misc = 1;
}

static inline void icu_ll_disable_mac_txrx_misc_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_txrx_misc = 0;
}

static inline void icu_ll_enable_mac_rx_trigger_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_rx_trigger = 1;
}

static inline void icu_ll_disable_mac_rx_trigger_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_rx_trigger = 0;
}

static inline void icu_ll_enable_mac_tx_trigger_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_tx_trigger = 1;
}

static inline void icu_ll_disable_mac_tx_trigger_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_tx_trigger = 0;
}

static inline void icu_ll_enable_mac_prot_trigger_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_prot_trigger = 1;
}

static inline void icu_ll_disable_mac_prot_trigger_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_prot_trigger = 0;
}

static inline void icu_ll_enable_mac_general_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_general = 1;
}

static inline void icu_ll_disable_mac_general_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_general = 0;
}

static inline void icu_ll_enable_mac_wakeup_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_wakeup = 1;
}

static inline void icu_ll_disable_mac_wakeup_interrupt(icu_hw_t *hw)
{
	hw->irq_en.mac_wakeup= 0;
}

static inline void icu_ll_enable_security_interrupt(icu_hw_t *hw)
{
	hw->irq_en.security = 1;
}

static inline void icu_ll_disable_security_interrupt(icu_hw_t *hw)
{
	hw->irq_en.security = 0;
}


static inline void icu_ll_enable_pll_unlock_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.pll_unlock = 1;
}

static inline void icu_ll_disable_pll_unlock_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.pll_unlock = 0;
}

static inline void icu_ll_enable_usb_plug_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.usb_plug = 1;
}

static inline void icu_ll_disable_usb_plug_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.usb_plug = 0;
}

static inline void icu_ll_enable_bt_wdt_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.bt_wdt = 1;
}

static inline void icu_ll_disable_bt_wdt_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.bt_wdt = 0;
}

static inline void icu_ll_enable_dsp_wdt_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.dsp_wdt = 1;
}

static inline void icu_ll_disable_dsp_wdt_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.dsp_wdt = 0;
}

static inline void icu_ll_enable_rtc_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.rtc = 1;
}

static inline void icu_ll_disable_rtc_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.rtc = 0;
}

static inline void icu_ll_enable_touch_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.touch = 1;
}

static inline void icu_ll_disable_touch_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.touch = 0;
}

static inline void icu_ll_enable_cec_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.cec = 1;
}

static inline void icu_ll_disable_cec_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.cec = 0;
}

static inline void icu_ll_enable_fiq(icu_hw_t *hw)
{
	hw->int_global_en.fiq = 1;
}

static inline void icu_ll_disable_fiq(icu_hw_t *hw)
{
	hw->int_global_en.fiq = 0;
}

static inline void icu_ll_enable_irq(icu_hw_t *hw)
{
	hw->int_global_en.irq = 1;
}

static inline void icu_ll_disable_irq(icu_hw_t *hw)
{
	hw->int_global_en.irq = 0;
}

static inline uint32_t icu_ll_get_int_status(icu_hw_t *hw)
{
	return 0;
}

static inline void icu_ll_clear_int_status(icu_hw_t *hw, uint32_t clr_status)
{
}

static inline uint32_t icu_ll_get_fiq_int_status(icu_hw_t *hw)
{
	return (hw->fiq_st.v & 0xFF);
}

static inline void icu_ll_clear_fiq_int_status(icu_hw_t *hw, uint32_t clr_status)
{
	hw->fiq_st.v = (clr_status | icu_ll_get_fiq_int_status(hw));
}

static inline uint32_t icu_ll_get_irq_int_status(icu_hw_t *hw)
{
	return hw->irq_st.v;
}

static inline void icu_ll_clear_irq_int_status(icu_hw_t *hw, uint32_t clr_status)
{
	hw->irq_st.v = (clr_status | icu_ll_get_irq_int_status(hw));
}

static inline uint32_t icu_ll_get_uart_int_status(icu_hw_t *hw)
{
	uint32_t irq_int_status = icu_ll_get_irq_int_status(hw);

	return (irq_int_status & 0x7);
}

static inline void icu_ll_disable_all_interrupt(icu_hw_t *hw)
{
	hw->irq_en.v = 0;
	hw->fiq_en.v = 0;
}

static inline void icu_ll_enable_interrupt(icu_hw_t *hw, uint32_t index)
{
}

static inline void icu_ll_disable_interrupt(icu_hw_t *hw, uint32_t index)
{
}


static inline void icu_ll_enable_irq_interrupt(icu_hw_t *hw, uint32_t index)
{
	REG_SET_BIT(&hw->irq_en, 1 << index);
}

static inline void icu_ll_disable_irq_interrupt(icu_hw_t *hw, uint32_t index)
{
	REG_CLR_BIT(&hw->irq_en, 1 << index);
}

static inline void icu_ll_enable_fiq_interrupt(icu_hw_t *hw, uint32_t index)
{
	REG_SET_BIT(&hw->fiq_en, 1 << index);
}

static inline void icu_ll_disable_fiq_interrupt(icu_hw_t *hw, uint32_t index)
{
	REG_CLR_BIT(&hw->fiq_en, 1 << index);
}


static inline void icu_ll_enable_uart_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->arm_wakeup_en, BIT(id));
}

static inline void icu_ll_disable_uart_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->arm_wakeup_en, BIT(id));
}

static inline void icu_ll_enable_i2c_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->arm_wakeup_en, BIT(id + ICU_F_I2C_CHAN_INT_S));
}

static inline void icu_ll_disable_i2c_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->arm_wakeup_en, BIT(id + ICU_F_I2C_CHAN_INT_S));
}

static inline void icu_ll_enable_sdio_host_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.sdio_host = 1;
}

static inline void icu_ll_disable_sdio_host_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.sdio_host = 0;
}

static inline void icu_ll_enable_usb_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	if (id == 0)
		hw->arm_wakeup_en.usb1 = 1;
	else
		hw->arm_wakeup_en.usb2 = 1;
}

static inline void icu_ll_disable_usb_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	if (id == 0)
		hw->arm_wakeup_en.usb1 = 0;
	else
		hw->arm_wakeup_en.usb2 = 0;
}

static inline void icu_ll_enable_spi_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->arm_wakeup_en, BIT(id + ICU_F_SPI_CHAN_INT_S));
}

static inline void icu_ll_disable_spi_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->arm_wakeup_en, BIT(id + ICU_F_SPI_CHAN_INT_S));
}

static inline void icu_ll_enable_pwm2_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.pwm2 = 1;
}

static inline void icu_ll_disable_pwm2_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.pwm2 = 0;
}

static inline void icu_ll_enable_pwm_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.pwm1 = 1;
}

static inline void icu_ll_disable_pwm_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.pwm1 = 0;
}

static inline void icu_ll_enable_gpio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.gpio = 1;
}

static inline void icu_ll_disable_gpio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.gpio = 0;
}

static inline void icu_ll_enable_irda_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.irda = 1;
}

static inline void icu_ll_disable_irda_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.irda = 0;
}

static inline void icu_ll_enable_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.timer = 1;
}

static inline void icu_ll_disable_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.timer = 0;
}

static inline void icu_ll_enable_saradc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.saradc = 1;
}

static inline void icu_ll_disable_saradc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.saradc = 0;
}

static inline void icu_ll_enable_psram_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.qspi = 1;
}
static inline void icu_ll_disable_psram_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.qspi = 0;
}

static inline void icu_ll_enable_sdio_dma_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.sdio_dma = 1;
}

static inline void icu_ll_disable_sdio_dma_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.sdio_dma = 0;
}

static inline void icu_ll_enable_dma_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.gdma = 1;
}

static inline void icu_ll_disable_dma_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.gdma = 0;
}

static inline void icu_ll_enable_mailbox_dsp_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mbox1 = 1;
}

static inline void icu_ll_disable_mailbox_dsp_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mbox1 = 0;
}

static inline void icu_ll_enable_mailbox_bt_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mbox2 = 1;
}

static inline void icu_ll_disable_mailbox_bt_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mbox2 = 0;
}


static inline void icu_ll_enable_modem_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.modem = 1;
}

static inline void icu_ll_disable_modem_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.modem = 0;
}

static inline void icu_ll_enable_mac_txrx_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_txrx_timer = 1;
}

static inline void icu_ll_disable_mac_txrx_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_txrx_timer = 0;
}

static inline void icu_ll_enable_mac_txrx_misc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_txrx_misc = 1;
}

static inline void icu_ll_disable_mac_txrx_misc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_txrx_misc = 0;
}

static inline void icu_ll_enable_mac_rx_trigger_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_rx_trigger = 1;
}

static inline void icu_ll_disable_mac_rx_trigger_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_rx_trigger = 0;
}

static inline void icu_ll_enable_mac_tx_trigger_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_tx_trigger = 1;
}

static inline void icu_ll_disable_mac_tx_trigger_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_tx_trigger = 0;
}

static inline void icu_ll_enable_mac_prot_trigger_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_prot_trigger = 1;
}

static inline void icu_ll_disable_mac_prot_trigger_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_prot_trigger = 0;
}

static inline void icu_ll_enable_mac_general_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_general = 1;
}

static inline void icu_ll_disable_mac_general_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_general = 0;
}

static inline void icu_ll_enable_mac_wakeup_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_wakeup = 1;
}

static inline void icu_ll_disable_mac_wakeup_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_wakeup = 0;
}

static inline void icu_ll_enable_security_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.security = 1;
}

static inline void icu_ll_disable_security_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.security = 0;
}


static inline void icu_ll_enable_pll_unlock_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.pll_unlock = 1;
}

static inline void icu_ll_disable_pll_unlock_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.pll_unlock = 0;
}

static inline void icu_ll_enable_usb_plug_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.usb_plug = 1;
}

static inline void icu_ll_disable_usb_plug_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.usb_plug = 0;
}

static inline void icu_ll_enable_bt_wdt_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.bt_wdt = 1;
}

static inline void icu_ll_disable_bt_wdt_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.bt_wdt = 0;
}

static inline void icu_ll_enable_dsp_wdt_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.dsp_wdt = 1;
}

static inline void icu_ll_disable_dsp_wdt_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.dsp_wdt = 0;
}

static inline void icu_ll_enable_rtc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.rtc = 1;
}

static inline void icu_ll_disable_rtc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.rtc = 0;
}

static inline void icu_ll_enable_touch_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.touch = 1;
}

static inline void icu_ll_disable_touch_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.touch = 0;
}

static inline void icu_ll_enable_cec_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.cec = 1;
}

static inline void icu_ll_disable_cec_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en2.cec = 0;
}


static inline void icu_ll_set_gpio_perial_mode(icu_hw_t *hw, uint32 index, uint32_t mode)
{
	if (index < GPIO_16)
		REG_MCHAN_SET_FIELD(index, &hw->gpio_peri_mode_cfg[0], ICU_GPIO_F_PERIAL_MODE, mode);
	else if (index < GPIO_32)
		REG_MCHAN_SET_FIELD(index-GPIO_16, &hw->gpio_peri_mode_cfg[1], ICU_GPIO_F_PERIAL_MODE, mode);
	else
		REG_MCHAN_SET_FIELD(index-GPIO_32, &hw->gpio_peri_mode_cfg[2], ICU_GPIO_F_PERIAL_MODE, mode);

	/* set gpio fifth function, index start at 0 */
	if (mode == 4) {
		if (index >= GPIO_8) {
			REG_SET_BIT(&hw->gpio_peri_mode_cfg4, BIT(index - GPIO_8));
		}
	} else {
		if (index >= GPIO_8) {
			REG_CLR_BIT(&hw->gpio_peri_mode_cfg4, BIT(index - GPIO_8));
		}
	}
}

static inline uint32 icu_ll_get_gpio_perial_mode(icu_hw_t *hw, uint32 index)
{
	if (index < GPIO_16)
		return ((REG_MCHAN_GET_FIELD(index, &hw->gpio_peri_mode_cfg[0], ICU_GPIO_F_PERIAL_MODE)));
	else if (index < GPIO_32)
		return ((REG_MCHAN_GET_FIELD(index-GPIO_16, &hw->gpio_peri_mode_cfg[1], ICU_GPIO_F_PERIAL_MODE)));
	else
		return (REG_MCHAN_GET_FIELD(index-GPIO_32, &hw->gpio_peri_mode_cfg[2], ICU_GPIO_F_PERIAL_MODE));
}

static inline void icu_ll_uart2_gpio_sel(icu_hw_t *hw,  uint32 io_mode)
{

	hw->gpio_map_cfg.uart2 = io_mode;

}

static inline void icu_ll_spi3_gpio_sel(icu_hw_t *hw,  uint32 io_mode)
{

	hw->gpio_map_cfg.spi3 = io_mode;

}

static inline void icu_ll_sdio_gpio_sel(icu_hw_t *hw,  uint32 io_mode)
{

	hw->gpio_map_cfg.sdio= io_mode;

}

static inline void icu_ll_sdio_host_enable(icu_hw_t *hw)
{

	hw->gpio_map_cfg.sdio_host= 1;
}

static inline void icu_ll_sdio_dma_enable(icu_hw_t *hw)
{

	hw->gpio_map_cfg.sdio_host= 0;
}

static inline uint32 icu_ll_pwms_gpio_sel(icu_hw_t *hw, uint32 chan, uint32 io_mode)
{
	if( chan == 6) {
		hw->gpio_map_cfg.pwm6 = io_mode;
	} else if (chan == 7) {
		hw->gpio_map_cfg.pwm7 = io_mode;
	}else if (chan == 8) {
		hw->gpio_map_cfg.pwm8= io_mode;
	}else if (chan == 9) {
		hw->gpio_map_cfg.pwm9= io_mode;
	}else {
		return chan;
	}

	return BK_OK;
}

static inline void icu_ll_spdif_gpio_sel(icu_hw_t *hw,  uint32 io_mode)
{

	hw->gpio_map_cfg.spdif= io_mode;
}


static inline void icu_ll_enable_la_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_la_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_btdm_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_btdm_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_bt_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_bt_arm_wakeup(icu_hw_t *hw)
{
}


static inline void icu_ll_enable_la_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_la_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_btdm_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_btdm_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_bt_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_bt_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_i2s_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_i2s_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_enable_audio_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_audio_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_fft_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_fft_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_hsspi_slave_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_hsspi_slave_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_enable_jpeg_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.jpeg = 1;
}

static inline void icu_ll_disable_jpeg_interrupt(icu_hw_t *hw)
{
	hw->fiq_en.jpeg = 0;
}

static inline void icu_ll_enable_i2s_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_i2s_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_enable_audio_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_audio_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_fft_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_fft_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_hsspi_slave_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_hsspi_slave_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_enable_jpeg_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_jpeg_arm_wakeup(icu_hw_t *hw)
{
}

/* add clk,power start */
static inline void icu_ll_set_peri_clk_pwr(icu_hw_t *hw, uint32 value)
{
	hw->clk_pwr_down.v = value;
}

static inline uint32 icu_ll_get_peri_clk_pwr_status(icu_hw_t *hw)
{
	return hw->clk_pwr_down.v;
}

static inline void icu_ll_set_peri_clk_pwr_group2(icu_hw_t *hw, uint32 value)
{
	hw->func_clk_pwr_down.v = value;
}

static inline uint32 icu_ll_get_peri_clk_pwr_status_group2(icu_hw_t *hw)
{
	return hw->func_clk_pwr_down.v;
}

static inline void icu_ll_set_clk_sel(icu_hw_t *hw, uint32 value)
{
	hw->peri_clk_mux.v = value;
}

static inline uint32 icu_ll_get_clk_sel(icu_hw_t *hw)
{
	return hw->peri_clk_mux.v;
}

static inline void icu_ll_set_dco_div(icu_hw_t *hw, uint32 value)
{
	hw->peri_clk_mux.dco_clk_div = value;
}

static inline uint32 icu_ll_get_dco_div(icu_hw_t *hw)
{
	return hw->peri_clk_mux.dco_clk_div;
}
/* add clk,power end */

static inline void icu_ll_enable_ble_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_ble_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_enable_ble_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_ble_arm_wakeup(icu_hw_t *hw)
{
}

#ifdef __cplusplus
}
#endif
