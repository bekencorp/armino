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

#ifdef __cplusplus
extern "C" {
#endif

#define ICU_LL_REG_BASE() (SOC_ICU_REG_BASE)


#define icu_ll_is_pwm_has_two_int(hw) false
#define icu_ll_get_pwm_int_num(hw)	ICU_IRQ_PWM
#define icu_ll_get_pwm_int_prio(hw)	ICU_PRI_IRQ_PWM
#define icu_ll_get_pwm_2nd_int_num(hw)	ICU_IRQ_PWM
#define icu_ll_get_pwm_2nd_int_prio(hw)	ICU_PRI_IRQ_PWM

static inline void icu_ll_set_jtag_wr_arm(icu_hw_t *hw)
{
	hw->jtag_sel.v = 0x7111E968;
}

static inline void icu_ll_set_jtag_wr_tl410(icu_hw_t *hw)
{
	hw->jtag_sel.v = 0x7111E410;
}

static inline void icu_ll_set_jtag_rd_arm(icu_hw_t *hw)
{
	hw->jtag_sel.v = 0;
}

static inline void icu_ll_set_jtag_rd_tl410(icu_hw_t *hw)
{
	hw->jtag_sel.v = 1;
}


static inline void icu_ll_enable_uart_interrupt(icu_hw_t *hw, uint32_t id)
{
	REG_SET_BIT(&hw->int_en, 1 << (id & 0x1));
}

static inline void icu_ll_disable_uart_interrupt(icu_hw_t *hw, uint32_t id)
{
	REG_CLR_BIT(&hw->int_en, 1 << (id & 0x1));
}


static inline void icu_ll_enable_i2c_interrupt(icu_hw_t *hw, uint32_t id)
{
	if (id == 0)
		hw->int_en.irq_i2c1 = 1;
	else
		hw->int_en.irq_i2c2 = 1;
}

static inline void icu_ll_disable_i2c_interrupt(icu_hw_t *hw, uint32_t id)
{
	if (id == 0)
		hw->int_en.irq_i2c1 = 0;
	else
		hw->int_en.irq_i2c2 = 0;
}

static inline void icu_ll_enable_irda_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_irda = 1;
}

static inline void icu_ll_disable_irda_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_irda = 0;
}

static inline void icu_ll_enable_i2s_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_i2s_pcm= 1;
}

static inline void icu_ll_disable_i2s_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_i2s_pcm = 0;
}

static inline void icu_ll_enable_spi_interrupt(icu_hw_t *hw, uint32_t id)
{
	hw->int_en.irq_spi = 1;
}

static inline void icu_ll_disable_spi_interrupt(icu_hw_t *hw, uint32_t id)
{
	hw->int_en.irq_spi = 0;
}

static inline void icu_ll_enable_gpio_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_gpio = 1;
}

static inline void icu_ll_disable_gpio_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_gpio = 0;
}

static inline void icu_ll_enable_timer_interrupt(icu_hw_t *hw)
{
    hw->int_en.irq_timer = 1;
}

static inline void icu_ll_disable_timer_interrupt(icu_hw_t *hw)
{
    hw->int_en.irq_timer = 0;
}

static inline void icu_ll_enable_pwm_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_pwm = 1;
}

static inline void icu_ll_disable_pwm_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_pwm = 0;
}

static inline void icu_ll_enable_audio_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_audio = 1;
}

static inline void icu_ll_disable_audio_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_audio = 0;
}

static inline void icu_ll_enable_saradc_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_saradc = 1;
}

static inline void icu_ll_disable_saradc_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_saradc = 0;
}

static inline void icu_ll_enable_sdio_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_sdio = 1;
}

static inline void icu_ll_disable_sdio_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_sdio = 0;
}

static inline void icu_ll_enable_usb_interrupt(icu_hw_t *hw, uint32_t id)
{
	hw->int_en.irq_usb = 1;
}

static inline void icu_ll_disable_usb_interrupt(icu_hw_t *hw, uint32_t id)
{
	hw->int_en.irq_usb = 0;
}

static inline void icu_ll_enable_fft_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_fft = 1;
}

static inline void icu_ll_disable_fft_interrupt(icu_hw_t *hw)
{
	hw->int_en.irq_fft = 0;
}

static inline void icu_ll_enable_dma_interrupt(icu_hw_t *hw)
{
    hw->int_en.irq_general_dma = 1;
}

static inline void icu_ll_disable_dma_interrupt(icu_hw_t *hw)
{
    hw->int_en.irq_general_dma = 0;
}

static inline void icu_ll_enable_modem_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_modem = 1;
}

static inline void icu_ll_disable_modem_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_modem = 0;
}

static inline void icu_ll_enable_mac_txrx_timer_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_txrx_timer = 1;
}

static inline void icu_ll_disable_mac_txrx_timer_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_txrx_timer = 0;
}

static inline void icu_ll_enable_mac_txrx_misc_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_mac_txrx_misc = 1;
}

static inline void icu_ll_disable_mac_txrx_misc_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_mac_txrx_misc = 0;
}

static inline void icu_ll_enable_mac_rx_trigger_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_rx_trigger = 1;
}

static inline void icu_ll_disable_mac_rx_trigger_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_rx_trigger = 0;
}


static inline void icu_ll_enable_mac_tx_trigger_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_mac_tx_trigger = 1;
}

static inline void icu_ll_disable_mac_tx_trigger_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_mac_tx_trigger = 0;
}

static inline void icu_ll_enable_mac_prot_trigger_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_prot_trigger = 1;
}

static inline void icu_ll_disable_mac_prot_trigger_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_prot_trigger = 0;
}

static inline void icu_ll_enable_mac_general_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_mac_general = 1;
}

static inline void icu_ll_disable_mac_general_interrupt(icu_hw_t *hw)
{
	hw->int_en.fiq_mac_general = 0;
}

static inline void icu_ll_enable_sdio_dma_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_sdio_dma = 1;
}

static inline void icu_ll_disable_sdio_dma_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_sdio_dma = 0;
}

static inline void icu_ll_enable_usb_plug_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_usb_plug_in_out = 1;
}

static inline void icu_ll_disable_usb_plug_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_usb_plug_in_out = 0;
}

static inline void icu_ll_enable_security_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_security = 1;
}

static inline void icu_ll_disable_security_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_security = 0;
}

static inline void icu_ll_enable_mac_wakeup_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_wakeup = 1;
}

static inline void icu_ll_disable_mac_wakeup_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_mac_wakeup= 0;
}

static inline void icu_ll_enable_hsspi_slave_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_hsspi_slave= 1;
}

static inline void icu_ll_disable_hsspi_slave_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_hsspi_slave = 0;
}

static inline void icu_ll_enable_pll_unlock_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_pll_unlock = 1;
}

static inline void icu_ll_disable_pll_unlock_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_pll_unlock= 0;
}

static inline void icu_ll_enable_jpeg_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_jpeg= 1;
}

static inline void icu_ll_disable_jpeg_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_jpeg = 0;
}

static inline void icu_ll_enable_ble_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_ble = 1;
}

static inline void icu_ll_disable_ble_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_ble= 0;
}

static inline void icu_ll_enable_psram_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_psram = 1;
}

static inline void icu_ll_disable_psram_interrupt(icu_hw_t *hw)
{
    hw->int_en.fiq_psram= 0;
}

static inline void icu_ll_enable_fiq(icu_hw_t *hw)
{
    hw->int_global_en.fiq_en = 1;
}

static inline void icu_ll_disable_fiq(icu_hw_t *hw)
{
    hw->int_global_en.fiq_en = 0;
}

static inline void icu_ll_enable_irq(icu_hw_t *hw)
{
    hw->int_global_en.irq_en = 1;
}

static inline void icu_ll_disable_irq(icu_hw_t *hw)
{
    hw->int_global_en.irq_en = 0;
}

static inline uint32_t icu_ll_get_int_status(icu_hw_t *hw)
{
	return hw->int_status.v;
}

static inline void icu_ll_clear_int_status(icu_hw_t *hw, uint32_t clr_status)
{
	hw->int_status.v = clr_status;
}

static inline uint32_t icu_ll_get_fiq_int_status(icu_hw_t *hw)
{
	uint32 fiq_int_status;

	fiq_int_status = (icu_ll_get_int_status(hw) & 0xFFFF0000);

	return fiq_int_status;
}

static inline void icu_ll_clear_fiq_int_status(icu_hw_t *hw, uint32_t clr_status)
{
	hw->int_status.v = (clr_status | icu_ll_get_int_status(hw));
}

static inline uint32_t icu_ll_get_irq_int_status(icu_hw_t *hw)
{
	uint32 irq_int_status;

	irq_int_status = (icu_ll_get_int_status(hw) & 0xFFFF);

	return irq_int_status;
}

static inline void icu_ll_clear_irq_int_status(icu_hw_t *hw, uint32_t clr_status)
{
	hw->int_status.v = (clr_status | icu_ll_get_int_status(hw));
}

static inline uint32_t icu_ll_get_uart_int_status(icu_hw_t *hw)
{
	uint32_t irq_int_status = icu_ll_get_irq_int_status(hw);

	return (irq_int_status & 0x3);
}

static inline void icu_ll_disable_all_interrupt(icu_hw_t *hw)
{
	hw->int_en.v = 0;
}

static inline void icu_ll_enable_interrupt(icu_hw_t *hw, uint32_t index)
{
	REG_SET_BIT(&hw->int_en, 1 << index);
}

static inline void icu_ll_disable_interrupt(icu_hw_t *hw, uint32_t index)
{
	REG_CLR_BIT(&hw->int_en, 1 << index);
}


//set arm wakeup type
static inline void icu_ll_enable_arm_wakeup(icu_hw_t *hw, uint32_t index)
{
	REG_SET_BIT(&hw->arm_wakeup_en, 1 << index);
}

static inline void icu_ll_disable_arm_wakeup(icu_hw_t *hw, uint32_t index)
{
	REG_CLR_BIT(&hw->arm_wakeup_en, 1 << index);
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
	if (id == 1)
		hw->arm_wakeup_en.i2c1 = 1;
	else
		hw->arm_wakeup_en.i2c2 = 1;
}

static inline void icu_ll_disable_i2c_arm_wakeup(icu_hw_t *hw, uint32_t id)
{
	if (id == 1)
		hw->arm_wakeup_en.i2c1 = 0;
	else
		hw->arm_wakeup_en.i2c2 = 0;
}

static inline void icu_ll_enable_irda_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.irda = 1;
}

static inline void icu_ll_disable_irda_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.irda = 0;
}

static inline void icu_ll_enable_i2s_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.i2s_pcm = 1;
}

static inline void icu_ll_disable_i2s_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.i2s_pcm = 0;
}


static inline void icu_ll_enable_spi_arm_wakeup(icu_hw_t *hw, uint32_t  id)
{
	hw->arm_wakeup_en.spi = 1;
}

static inline void icu_ll_disable_spi_arm_wakeup(icu_hw_t *hw, uint32_t  id)
{
	hw->arm_wakeup_en.spi = 0;
}

static inline void icu_ll_enable_gpio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.gpio = 1;
}

static inline void icu_ll_disable_gpio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.gpio = 0;
}

static inline void icu_ll_enable_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.timer = 1;
}

static inline void icu_ll_disable_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.timer = 0;
}

static inline void icu_ll_enable_pwm_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.pwm = 1;
}

static inline void icu_ll_disable_pwm_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.pwm = 0;
}

static inline void icu_ll_enable_audio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.audio = 1;
}

static inline void icu_ll_disable_audio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.audio = 0;
}

static inline void icu_ll_enable_saradc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.saradc = 1;
}

static inline void icu_ll_disable_saradc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.saradc = 0;
}

static inline void icu_ll_enable_sdio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.sdio = 1;
}

static inline void icu_ll_disable_sdio_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.sdio = 0;
}

static inline void icu_ll_enable_usb_arm_wakeup(icu_hw_t *hw, uint32_t  id)
{
	hw->arm_wakeup_en.usb = 1;
}

static inline void icu_ll_disable_usb_arm_wakeup(icu_hw_t *hw, uint32_t  id)
{
	hw->arm_wakeup_en.usb = 0;
}

static inline void icu_ll_enable_fft_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.fft = 1;
}

static inline void icu_ll_disable_fft_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.fft = 0;
}

static inline void icu_ll_enable_dma_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.general_dma = 1;
}

static inline void icu_ll_disable_dma_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.general_dma = 0;
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
	hw->arm_wakeup_en.mac_tx_rx_timer = 1;
}

static inline void icu_ll_disable_mac_txrx_timer_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_tx_rx_timer = 0;
}

static inline void icu_ll_enable_mac_txrx_misc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_tx_rx_misc = 1;
}

static inline void icu_ll_disable_mac_txrx_misc_arm_wakeup(icu_hw_t *hw)
{
	hw->arm_wakeup_en.mac_tx_rx_misc = 0;
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

static inline void icu_ll_enable_sdio_dma_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.sdio_dma = 1;
}

static inline void icu_ll_disable_sdio_dma_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.sdio_dma = 0;
}

static inline void icu_ll_enable_usb_plug_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.usb_plug_in_out = 1;
}

static inline void icu_ll_disable_usb_plug_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.usb_plug_in_out = 0;
}

static inline void icu_ll_enable_security_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.security = 1;
}

static inline void icu_ll_disable_security_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.security = 0;
}

static inline void icu_ll_enable_mac_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.mac_wakeup = 1;
}

static inline void icu_ll_disable_mac_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.mac_wakeup= 0;
}

static inline void icu_ll_enable_hsspi_slave_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.hsspi_slave= 1;
}

static inline void icu_ll_disable_hsspi_slave_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.hsspi_slave = 0;
}

static inline void icu_ll_enable_jpeg_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.jpeg= 1;
}

static inline void icu_ll_disable_jpeg_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.jpeg = 0;
}

static inline void icu_ll_enable_ble_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.ble = 1;
}

static inline void icu_ll_disable_ble_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.ble= 0;
}

static inline void icu_ll_enable_psram_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.psram = 1;
}

static inline void icu_ll_disable_psram_arm_wakeup(icu_hw_t *hw)
{
    hw->arm_wakeup_en.psram= 0;
}



static inline void icu_ll_enable_la_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_la_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_pll_unlock_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_pll_unlock_arm_wakeup(icu_hw_t *hw)
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

static inline void icu_ll_enable_mailbox_dsp_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_mailbox_dsp_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_mailbox_bt_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_mailbox_bt_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_cec_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_cec_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_touch_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_touch_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_rtc_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_rtc_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_dsp_wdt_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_dsp_wdt_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_bt_wdt_interrupt(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_bt_wdt_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_pwm2_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_disable_pwm2_interrupt(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_mailbox_dsp_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_mailbox_dsp_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_mailbox_bt_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_mailbox_bt_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_cec_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_cec_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_touch_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_touch_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_rtc_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_rtc_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_dsp_wdt_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_dsp_wdt_arm_wakeup(icu_hw_t *hw)
{
}

static inline void icu_ll_enable_bt_wdt_arm_wakeup(icu_hw_t *hw)
{
}
static inline void icu_ll_disable_bt_wdt_arm_wakeup(icu_hw_t *hw)
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
#ifdef __cplusplus
}
#endif
