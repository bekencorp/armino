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

#include "icu_hw.h"
#include "icu_hal.h"
#include "icu_ll.h"
#include "hal_config.h"

//TODO finally we will automatically generate this code

#if CFG_HAL_DEBUG_ICU

void icu_struct_dump(void)
{
	icu_hw_t *hw = (icu_hw_t *)ICU_LL_REG_BASE();
	SOC_LOGI("base=%x\n", (uint32_t)hw);

	SOC_LOGI("peri_clk_mux=%x v=%x\n", &hw->peri_clk_mux, hw->peri_clk_mux.v);
	SOC_LOGI("  uart1: %x\n", hw->peri_clk_mux.uart1);
	SOC_LOGI("  uart2: %x\n", hw->peri_clk_mux.uart2);
	SOC_LOGI("  i2c1: %x\n", hw->peri_clk_mux.i2c1);
	SOC_LOGI("  irda: %x\n", hw->peri_clk_mux.irda);
	SOC_LOGI("  i2c2: %x\n", hw->peri_clk_mux.i2c2);
	SOC_LOGI("  spi: %x\n", hw->peri_clk_mux.spi);
	SOC_LOGI("  saradc: %x\n", hw->peri_clk_mux.saradc);
	SOC_LOGI("  pwms: %x\n", hw->peri_clk_mux.pwms);
	SOC_LOGI("  sdio: %x\n", hw->peri_clk_mux.sdio);
	SOC_LOGI("  saradc_auto_pll: %x\n", hw->peri_clk_mux.saradc_audio_pll);
	SOC_LOGI("  reserved_1: %x\n", hw->peri_clk_mux.reserved_1);
	SOC_LOGI("  qspi: %x\n", hw->peri_clk_mux.qspi);
	SOC_LOGI("  dco_clk_div: %x\n", hw->peri_clk_mux.dco_clk_div);
	SOC_LOGI("  reserved_2: %x\n", hw->peri_clk_mux.reserved_2);

	SOC_LOGI("pwm_chan_clk_mux=%x v=%x\n", &hw->pwm_chan_clk_mux, hw->pwm_chan_clk_mux.v);
	SOC_LOGI("  chan0=%x\n", hw->pwm_chan_clk_mux.chan0);
	SOC_LOGI("  chan1=%x\n", hw->pwm_chan_clk_mux.chan1);
	SOC_LOGI("  chan2=%x\n", hw->pwm_chan_clk_mux.chan2);
	SOC_LOGI("  chan3=%x\n", hw->pwm_chan_clk_mux.chan3);
	SOC_LOGI("  chan4=%x\n", hw->pwm_chan_clk_mux.chan4);
	SOC_LOGI("  chan5=%x\n", hw->pwm_chan_clk_mux.chan5);
	SOC_LOGI("  reserved=%x\n", hw->pwm_chan_clk_mux.reserved);

	SOC_LOGI("clk_pwr_down=%x v=%x\n", &hw->clk_pwr_down, hw->clk_pwr_down.v);
	SOC_LOGI("  uart1=%x\n", hw->clk_pwr_down.uart1);
	SOC_LOGI("  uart2=%x\n", hw->clk_pwr_down.uart2);
	SOC_LOGI("  i2c1=%x\n", hw->clk_pwr_down.i2c1);
	SOC_LOGI("  irda=%x\n", hw->clk_pwr_down.irda);
	SOC_LOGI("  i2s_pcm=%x\n", hw->clk_pwr_down.i2s_pcm);
	SOC_LOGI("  i2c2=%x\n", hw->clk_pwr_down.i2c2);
	SOC_LOGI("  spi=%x\n", hw->clk_pwr_down.spi);
	SOC_LOGI("  saradc=%x\n", hw->clk_pwr_down.saradc);
	SOC_LOGI("  arm_watchdog=%x\n", hw->clk_pwr_down.arm_watchdog);
	SOC_LOGI("  pwm0=%x\n", hw->clk_pwr_down.pwm0);
	SOC_LOGI("  pwm1=%x\n", hw->clk_pwr_down.pwm1);
	SOC_LOGI("  pwm2=%x\n", hw->clk_pwr_down.pwm2);
	SOC_LOGI("  pwm3=%x\n", hw->clk_pwr_down.pwm3);
	SOC_LOGI("  pwm4=%x\n", hw->clk_pwr_down.pwm4);
	SOC_LOGI("  pwm5=%x\n", hw->clk_pwr_down.pwm5);
	SOC_LOGI("  audio=%x\n", hw->clk_pwr_down.audio);
	SOC_LOGI("  tl410_watchdog=%x\n", hw->clk_pwr_down.tl410_watchdog);
	SOC_LOGI("  sdio=%x\n", hw->clk_pwr_down.sdio);
	SOC_LOGI("  usb=%x\n", hw->clk_pwr_down.usb);
	SOC_LOGI("  fft=%x\n", hw->clk_pwr_down.fft);
	SOC_LOGI("  timers=%x\n", hw->clk_pwr_down.timers);
	SOC_LOGI("  jpeg=%x\n", hw->clk_pwr_down.jpeg);
	SOC_LOGI("  reserved=%x\n", hw->clk_pwr_down.reserved);

	SOC_LOGI("peri_clk_gate_disable=%x v=%x\n", &hw->peri_clk_gate_disable, hw->peri_clk_gate_disable.v);
	SOC_LOGI("  icu=%x\n", hw->peri_clk_gate_disable.icu);
	SOC_LOGI("  uart1=%x\n", hw->peri_clk_gate_disable.uart1);
	SOC_LOGI("  uart2=%x\n", hw->peri_clk_gate_disable.uart2);
	SOC_LOGI("  i2c1=%x\n", hw->peri_clk_gate_disable.i2c1);
	SOC_LOGI("  irda=%x\n", hw->peri_clk_gate_disable.irda);
	SOC_LOGI("  i2s_pcm=%x\n", hw->peri_clk_gate_disable.i2s_pcm);
	SOC_LOGI("  i2c2=%x\n", hw->peri_clk_gate_disable.i2c2);
	SOC_LOGI("  spi=%x\n", hw->peri_clk_gate_disable.spi);
	SOC_LOGI("  gpio=%x\n", hw->peri_clk_gate_disable.gpio);
	SOC_LOGI("  watchdog=%x\n", hw->peri_clk_gate_disable.watchdog);
	SOC_LOGI("  pwms_timers=%x\n", hw->peri_clk_gate_disable.pwms_timers);
	SOC_LOGI("  audio=%x\n", hw->peri_clk_gate_disable.audio);
	SOC_LOGI("  sdio=%x\n", hw->peri_clk_gate_disable.sdio);
	SOC_LOGI("  usb=%x\n", hw->peri_clk_gate_disable.fft);
	SOC_LOGI("  mac=%x\n", hw->peri_clk_gate_disable.mac);
	SOC_LOGI("  reserved=%x\n", hw->peri_clk_gate_disable.reserved);

	SOC_LOGI("clk_pwr_down_2=%x v=%x\n", &hw->clk_pwr_down_2, hw->clk_pwr_down_2.v);
	SOC_LOGI("  tl410=%x\n", hw->clk_pwr_down_2.tl410);
	SOC_LOGI("  ble=%x\n", hw->clk_pwr_down_2.ble);
	SOC_LOGI("  reserved%x\n", hw->clk_pwr_down_2.reserved);

	SOC_LOGI("clk26m_div_factor=%x v=%x\n", &hw->clk26m_div_factor, hw->clk26m_div_factor.v);
	SOC_LOGI("  factor=%x\n", hw->clk26m_div_factor.factor);

	SOC_LOGI("jtag_sel=%x v=%x\n", &hw->jtag_sel, hw->jtag_sel.v);

	SOC_LOGI("int_en(%x)=%x raw_status(%x)=%x status(%x)=%x\n", &hw->int_en, hw->int_en.v,
			 &hw->int_raw_status, hw->int_raw_status.v,
			 &hw->int_status, hw->int_status.v);
	SOC_LOGI("  irq_uart1 en=%x raw=%x st=%x\n", hw->int_en.irq_uart1, hw->int_raw_status.irq_uart1, hw->int_status.irq_uart1);
	SOC_LOGI("  irq_uart2 en=%x raw=%x st=%x\n", hw->int_en.irq_uart2, hw->int_raw_status.irq_uart2, hw->int_status.irq_uart2);
	SOC_LOGI("  irq_i2c1 en=%x raw=%x st=%x\n", hw->int_en.irq_i2c1, hw->int_raw_status.irq_i2c1, hw->int_status.irq_i2c1);
	SOC_LOGI("  irq_irda en=%x raw=%x st=%x\n", hw->int_en.irq_irda, hw->int_raw_status.irq_irda, hw->int_status.irq_irda);
	SOC_LOGI("  irq_i2s_pcm en=%x raw=%x st=%x\n", hw->int_en.irq_i2s_pcm, hw->int_raw_status.irq_i2s_pcm, hw->int_status.irq_i2s_pcm);
	SOC_LOGI("  irq_i2c2 en=%x raw=%x st=%x\n", hw->int_en.irq_i2c2, hw->int_raw_status.irq_i2c2, hw->int_status.irq_i2c2);
	SOC_LOGI("  irq_spi en=%x raw=%x st=%x\n", hw->int_en.irq_spi, hw->int_raw_status.irq_spi, hw->int_status.irq_spi);
	SOC_LOGI("  irq_gpio en=%x raw=%x st=%x\n", hw->int_en.irq_gpio, hw->int_raw_status.irq_gpio, hw->int_status.irq_gpio);
	SOC_LOGI("  irq_timer en=%x raw=%x st=%x\n", hw->int_en.irq_timer, hw->int_raw_status.irq_timer, hw->int_status.irq_timer);
	SOC_LOGI("  irq_pwm en=%x raw=%x st=%x\n", hw->int_en.irq_pwm, hw->int_raw_status.irq_pwm, hw->int_status.irq_pwm);
	SOC_LOGI("  irq_audio en=%x raw=%x st=%x\n", hw->int_en.irq_audio, hw->int_raw_status.irq_audio, hw->int_status.irq_audio);
	SOC_LOGI("  irq_saradc en=%x raw=%x st=%x\n", hw->int_en.irq_saradc, hw->int_raw_status.irq_saradc, hw->int_status.irq_saradc);
	SOC_LOGI("  irq_sdio en=%x raw=%x st=%x\n", hw->int_en.irq_sdio, hw->int_raw_status.irq_sdio, hw->int_status.irq_sdio);
	SOC_LOGI("  irq_usb en=%x raw=%x st=%x\n", hw->int_en.irq_usb, hw->int_raw_status.irq_usb, hw->int_status.irq_usb);
	SOC_LOGI("  irq_fft en=%x raw=%x st=%x\n", hw->int_en.irq_fft, hw->int_raw_status.irq_fft, hw->int_status.irq_fft);
	SOC_LOGI("  irq_general_dma en=%x raw=%x st=%x\n", hw->int_en.irq_general_dma, hw->int_raw_status.irq_general_dma, hw->int_status.irq_general_dma);

	SOC_LOGI("  fiq_modem en=%x raw=%x st=%x\n", hw->int_en.fiq_modem, hw->int_raw_status.fiq_modem, hw->int_status.fiq_modem);
	SOC_LOGI("  fiq_mac_txrx_timer en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_txrx_timer, hw->int_raw_status.fiq_mac_txrx_timer, hw->int_status.fiq_mac_txrx_timer);
	SOC_LOGI("  fiq_mac_txrx_misc en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_txrx_misc, hw->int_raw_status.fiq_mac_txrx_misc, hw->int_status.fiq_mac_txrx_misc);
	SOC_LOGI("  fiq_mac_rx_trigger en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_rx_trigger, hw->int_raw_status.fiq_mac_rx_trigger, hw->int_status.fiq_mac_rx_trigger);
	SOC_LOGI("  fiq_mac_tx_trigger en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_tx_trigger, hw->int_raw_status.fiq_mac_tx_trigger, hw->int_status.fiq_mac_tx_trigger);
	SOC_LOGI("  fiq_mac_prot_trigger en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_prot_trigger, hw->int_raw_status.fiq_mac_prot_trigger, hw->int_status.fiq_mac_prot_trigger);
	SOC_LOGI("  fiq_mac_general en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_general, hw->int_raw_status.fiq_mac_general, hw->int_status.fiq_mac_general);
	SOC_LOGI("  fiq_sdio_dma en=%x raw=%x st=%x\n", hw->int_en.fiq_sdio_dma, hw->int_raw_status.fiq_sdio_dma, hw->int_status.fiq_sdio_dma);
	SOC_LOGI("  fiq_usb_plug_in_out en=%x raw=%x st=%x\n", hw->int_en.fiq_usb_plug_in_out, hw->int_raw_status.fiq_usb_plug_in_out, hw->int_status.fiq_usb_plug_in_out);
	SOC_LOGI("  fiq_security en=%x raw=%x st=%x\n", hw->int_en.fiq_security, hw->int_raw_status.fiq_security, hw->int_status.fiq_security);
	SOC_LOGI("  fiq_mac_wakeup en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_wakeup, hw->int_raw_status.fiq_mac_wakeup, hw->int_status.fiq_mac_wakeup);
	SOC_LOGI("  fiq_hsspi_slave en=%x raw=%x st=%x\n", hw->int_en.fiq_hsspi_slave, hw->int_raw_status.fiq_hsspi_slave, hw->int_status.fiq_hsspi_slave);
	SOC_LOGI("  fiq_pll_unlock en=%x raw=%x st=%x\n", hw->int_en.fiq_pll_unlock, hw->int_raw_status.fiq_pll_unlock, hw->int_status.fiq_pll_unlock);
	SOC_LOGI("  fiq_jpeg en=%x raw=%x st=%x\n", hw->int_en.fiq_jpeg, hw->int_raw_status.fiq_jpeg, hw->int_status.fiq_jpeg);
	SOC_LOGI("  fiq_ble en=%x raw=%x st=%x\n", hw->int_en.fiq_ble, hw->int_raw_status.fiq_ble, hw->int_status.fiq_ble);
	SOC_LOGI("  fiq_psram en=%x raw=%x st=%x\n", hw->int_en.fiq_psram, hw->int_raw_status.fiq_psram, hw->int_status.fiq_psram);

	SOC_LOGI("  fiq=%x \n", hw->int_global_en.fiq_en);
	SOC_LOGI("  irq=%x \n", hw->int_global_en.irq_en);

	SOC_LOGI("arm wakeup en(%x)=%x\n", &hw->arm_wakeup_en, hw->arm_wakeup_en.v);
	SOC_LOGI("  uart1=%x\n", hw->arm_wakeup_en.uart1);
	SOC_LOGI("  uart2=%x\n", hw->arm_wakeup_en.uart2);
	SOC_LOGI("  i2c1=%x\n", hw->arm_wakeup_en.i2c1);
	SOC_LOGI("  irda=%x\n", hw->arm_wakeup_en.irda);
	SOC_LOGI("  i2s_pcm=%x\n", hw->arm_wakeup_en.i2s_pcm);
	SOC_LOGI("  i2c2=%x\n", hw->arm_wakeup_en.i2c2);
	SOC_LOGI("  spi=%x\n", hw->arm_wakeup_en.spi);
	SOC_LOGI("  gpio=%x\n", hw->arm_wakeup_en.gpio);
	SOC_LOGI("  timer=%x\n", hw->arm_wakeup_en.timer);
	SOC_LOGI("  pwm=%x\n", hw->arm_wakeup_en.pwm);
	SOC_LOGI("  audio=%x\n", hw->arm_wakeup_en.audio);
	SOC_LOGI("  saradc=%x\n", hw->arm_wakeup_en.saradc);
	SOC_LOGI("  usb=%x\n", hw->arm_wakeup_en.usb);
	SOC_LOGI("  fft=%x\n", hw->arm_wakeup_en.fft);
	SOC_LOGI("  general_dma=%x\n", hw->arm_wakeup_en.general_dma);
	SOC_LOGI("  mac_tx_rx_timer=%x\n", hw->arm_wakeup_en.mac_tx_rx_timer);
	SOC_LOGI("  mac_tx_rx_misc=%x\n", hw->arm_wakeup_en.mac_tx_rx_misc);
	SOC_LOGI("  mac_rx_trigger=%x\n", hw->arm_wakeup_en.mac_rx_trigger);
	SOC_LOGI("  mac_tx_trigger=%x\n", hw->arm_wakeup_en.mac_tx_trigger);
	SOC_LOGI("  mac_prot_trigger=%x\n", hw->arm_wakeup_en.mac_prot_trigger);
	SOC_LOGI("  mac_general=%x\n", hw->arm_wakeup_en.mac_general);
	SOC_LOGI("  sdio_dma=%x\n", hw->arm_wakeup_en.sdio_dma);
	SOC_LOGI("  mailbox=%x\n", hw->arm_wakeup_en.usb_plug_in_out);
	SOC_LOGI("  mac=%x\n", hw->arm_wakeup_en.security);
	SOC_LOGI("  hsspi_slave=%x\n", hw->arm_wakeup_en.hsspi_slave);
	SOC_LOGI("  reserved=%x\n", hw->arm_wakeup_en.reserved);
	SOC_LOGI("  jpeg=%x\n", hw->arm_wakeup_en.jpeg);
	SOC_LOGI("  ble=%x\n", hw->arm_wakeup_en.ble);
	SOC_LOGI("  psram=%x\n", hw->arm_wakeup_en.psram);

	SOC_LOGI("tl410 int en(%x)=%x raw_st(%x)=%x st(%x)=%x\n", &hw->tl410_int_en, hw->tl410_int_en.v,
			 &hw->tl410_int_raw_status, hw->tl410_int_raw_status.v,
			 &hw->tl410_int_status, hw->tl410_int_status.v);
	SOC_LOGI("  uart1=%x\n", hw->tl410_int_en.uart1, hw->tl410_int_raw_status.uart1, hw->tl410_int_status.uart1);
	SOC_LOGI("  uart2=%x\n", hw->tl410_int_en.uart2, hw->tl410_int_raw_status.uart2, hw->tl410_int_status.uart2);
	SOC_LOGI("  i2c1=%x\n", hw->tl410_int_en.i2c1, hw->tl410_int_raw_status.i2c1, hw->tl410_int_status.i2c1);
	SOC_LOGI("  irda=%x\n", hw->tl410_int_en.irda, hw->tl410_int_raw_status.irda, hw->tl410_int_status.irda);
	SOC_LOGI("  i2s_pcm=%x\n", hw->tl410_int_en.i2s_pcm, hw->tl410_int_raw_status.i2s_pcm, hw->tl410_int_status.i2s_pcm);
	SOC_LOGI("  i2c2=%x\n", hw->tl410_int_en.i2c2, hw->tl410_int_raw_status.i2c2, hw->tl410_int_status.i2c2);
	SOC_LOGI("  spi=%x\n", hw->tl410_int_en.spi, hw->tl410_int_raw_status.spi, hw->tl410_int_status.spi);
	SOC_LOGI("  gpio=%x\n", hw->tl410_int_en.gpio, hw->tl410_int_raw_status.gpio, hw->tl410_int_status.gpio);
	SOC_LOGI("  reserved=%x\n", hw->tl410_int_en.reserved, hw->tl410_int_raw_status.reserved, hw->tl410_int_status.reserved);
	SOC_LOGI("  pwm=%x\n", hw->tl410_int_en.pwm, hw->tl410_int_raw_status.pwm, hw->tl410_int_status.pwm);
	SOC_LOGI("  audio=%x\n", hw->tl410_int_en.audio, hw->tl410_int_raw_status.audio, hw->tl410_int_status.audio);
	SOC_LOGI("  saradc=%x\n", hw->tl410_int_en.saradc, hw->tl410_int_raw_status.saradc, hw->tl410_int_status.saradc);
	SOC_LOGI("  sdio=%x\n", hw->tl410_int_en.sdio, hw->tl410_int_raw_status.sdio, hw->tl410_int_status.sdio);
	SOC_LOGI("  usb=%x\n", hw->tl410_int_en.usb, hw->tl410_int_raw_status.usb, hw->tl410_int_status.usb);
	SOC_LOGI("  fft=%x\n", hw->tl410_int_en.fft, hw->tl410_int_raw_status.fft, hw->tl410_int_status.fft);
	SOC_LOGI("  reserved1=%x\n", hw->tl410_int_en.reserved1, hw->tl410_int_raw_status.reserved1, hw->tl410_int_status.reserved1);
	SOC_LOGI("  mailbox=%x\n", hw->tl410_int_en.mailbox, hw->tl410_int_raw_status.mailbox, hw->tl410_int_status.mailbox);
	SOC_LOGI("  reserved2=%x\n", hw->tl410_int_en.reserved2, hw->tl410_int_raw_status.reserved2, hw->tl410_int_status.reserved2);

	SOC_LOGI("tl410 wakeup en(%x)=%x\n", &hw->tl410_wakeup_en, hw->tl410_wakeup_en.v);
	SOC_LOGI("  uart1=%x\n", hw->tl410_wakeup_en.uart1);
	SOC_LOGI("  uart2=%x\n", hw->tl410_wakeup_en.uart2);
	SOC_LOGI("  i2c1=%x\n", hw->tl410_wakeup_en.i2c1);
	SOC_LOGI("  irda=%x\n", hw->tl410_wakeup_en.irda);
	SOC_LOGI("  i2s_pcm=%x\n", hw->tl410_wakeup_en.i2s_pcm);
	SOC_LOGI("  i2c2=%x\n", hw->tl410_wakeup_en.i2c2);
	SOC_LOGI("  spi=%x\n", hw->tl410_wakeup_en.spi);
	SOC_LOGI("  gpio=%x\n", hw->tl410_wakeup_en.gpio);
	SOC_LOGI("  reserved1=%x\n", hw->tl410_wakeup_en.reserved);
	SOC_LOGI("  pwm=%x\n", hw->tl410_wakeup_en.pwm);
	SOC_LOGI("  audio=%x\n", hw->tl410_wakeup_en.audio);
	SOC_LOGI("  saradc=%x\n", hw->tl410_wakeup_en.saradc);
	SOC_LOGI("  sdio=%x\n", hw->tl410_wakeup_en.sdio);
	SOC_LOGI("  usb=%x\n", hw->tl410_wakeup_en.usb);
	SOC_LOGI("  fft=%x\n", hw->tl410_wakeup_en.fft);
	SOC_LOGI("  reserved2=%x\n", hw->tl410_wakeup_en.reserved2);
	SOC_LOGI("  mailbox=%x\n", hw->tl410_wakeup_en.mailbox);
	SOC_LOGI("  reserved3=%x\n", hw->tl410_wakeup_en.reserved3);
}

#endif
