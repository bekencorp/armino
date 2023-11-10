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
	SOC_LOGI("  reserved_1: %x\n", hw->peri_clk_mux.reserved_1);
#if CONFIG_SOC_BK7236A
	SOC_LOGI("  jpeg=%x\n", hw->peri_clk_mux.jpeg);
#endif
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
	SOC_LOGI("  reserved_1=%x\n", hw->clk_pwr_down.reserved_1);
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
	SOC_LOGI("  security=%x\n", hw->clk_pwr_down.security);
	SOC_LOGI("  reserved_2=%x\n", hw->clk_pwr_down.reserved_2);
	SOC_LOGI("  sdio=%x\n", hw->clk_pwr_down.sdio);
	SOC_LOGI("  timers=%x\n", hw->clk_pwr_down.timers);
#if CONFIG_SOC_BK7236A
	SOC_LOGI("  jpeg=%x\n", hw->clk_pwr_down.jpeg);
#endif

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
	SOC_LOGI("  timers=%x\n", hw->peri_clk_gate_disable.timers);
	SOC_LOGI("  pwms=%x\n", hw->peri_clk_gate_disable.pwms);
	SOC_LOGI("  saradc=%x\n", hw->peri_clk_gate_disable.saradc);
	SOC_LOGI("  sdio=%x\n", hw->peri_clk_gate_disable.sdio);
	SOC_LOGI("  reserved_1=%x\n", hw->peri_clk_gate_disable.reserved_1);
	SOC_LOGI("  mac=%x\n", hw->peri_clk_gate_disable.mac);
	SOC_LOGI("  reserved_2=%x\n", hw->peri_clk_gate_disable.reserved_2);

	SOC_LOGI("clk_pwr_down_2=%x v=%x\n", &hw->clk_pwr_down_2, hw->clk_pwr_down_2.v);
	SOC_LOGI("  reserved_1=%x\n", hw->clk_pwr_down_2.reserved_1);
	SOC_LOGI("  ble=%x\n", hw->clk_pwr_down_2.ble);
	SOC_LOGI("  reserved_2%x\n", hw->clk_pwr_down_2.reserved_2);

	SOC_LOGI("clk26m_div_factor=%x v=%x\n", &hw->clk26m_div_factor, hw->clk26m_div_factor.v);
	SOC_LOGI("  factor=%x\n", hw->clk26m_div_factor.factor);

	SOC_LOGI("int_en(%x)=%x raw_status(%x)=%x status(%x)=%x\n", &hw->int_en, hw->int_en.v,
			 &hw->int_raw_status, hw->int_raw_status.v,
			 &hw->int_status, hw->int_status.v);
	SOC_LOGI("  irq_uart1 en=%x raw=%x st=%x\n", hw->int_en.irq_uart1, hw->int_raw_status.irq_uart1, hw->int_status.irq_uart1);
	SOC_LOGI("  irq_uqrt2 en=%x raw=%x st=%x\n", hw->int_en.irq_uart2, hw->int_raw_status.irq_uart2, hw->int_status.irq_uart2);
	SOC_LOGI("  irq_i2c1 en=%x raw=%x st=%x\n", hw->int_en.irq_i2c1, hw->int_raw_status.irq_i2c1, hw->int_status.irq_i2c1);
	SOC_LOGI("  irq_irda en=%x raw=%x st=%x\n", hw->int_en.irq_irda, hw->int_raw_status.irq_irda, hw->int_status.irq_irda);
	SOC_LOGI("  reserved_1 en=%x raw=%x st=%x\n", hw->int_en.reserved_1, hw->int_raw_status.reserved_1, hw->int_status.reserved_1);
	SOC_LOGI("  irq_i2c2 en=%x raw=%x st=%x\n", hw->int_en.irq_i2c2, hw->int_raw_status.irq_i2c2, hw->int_status.irq_i2c2);
	SOC_LOGI("  irq_spi en=%x raw=%x st=%x\n", hw->int_en.irq_spi, hw->int_raw_status.irq_spi, hw->int_status.irq_spi);
	SOC_LOGI("  irq_gpio en=%x raw=%x st=%x\n", hw->int_en.irq_gpio, hw->int_raw_status.irq_gpio, hw->int_status.irq_gpio);
	SOC_LOGI("  irq_timer en=%x raw=%x st=%x\n", hw->int_en.irq_timer, hw->int_raw_status.irq_timer, hw->int_status.irq_timer);
	SOC_LOGI("  irq_pwm en=%x raw=%x st=%x\n", hw->int_en.irq_pwm, hw->int_raw_status.irq_pwm, hw->int_status.irq_pwm);
#if CONFIG_SOC_BK7236A
	SOC_LOGI("  jpeg en=%x raw=%x st=%x\n", hw->int_en.irq_jpeg, hw->int_raw_status.irq_jpeg, hw->int_status.irq_jpeg);
#endif
	SOC_LOGI("  irq_saradc en=%x raw=%x st=%x\n", hw->int_en.irq_saradc, hw->int_raw_status.irq_saradc, hw->int_status.irq_saradc);
	SOC_LOGI("  irq_sdio en=%x raw=%x st=%x\n", hw->int_en.irq_sdio, hw->int_raw_status.irq_sdio, hw->int_status.irq_sdio);
	SOC_LOGI("  irq_security en=%x raw=%x st=%x\n", hw->int_en.irq_security, hw->int_raw_status.irq_security, hw->int_status.irq_security);
	SOC_LOGI("  irq_la en=%x raw=%x st=%x\n", hw->int_en.irq_la, hw->int_raw_status.irq_la, hw->int_status.irq_la);
	SOC_LOGI("  irq_general_dma en=%x raw=%x st=%x\n", hw->int_en.irq_general_dma, hw->int_raw_status.irq_general_dma, hw->int_status.irq_general_dma);

	SOC_LOGI("  fiq_modem en=%x raw=%x st=%x\n", hw->int_en.fiq_modem, hw->int_raw_status.fiq_modem, hw->int_status.fiq_modem);
	SOC_LOGI("  fiq_mac_txrx_timer en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_txrx_timer, hw->int_raw_status.fiq_mac_txrx_timer, hw->int_status.fiq_mac_txrx_timer);
	SOC_LOGI("  fiq_mac_txrx_misc en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_txrx_misc, hw->int_raw_status.fiq_mac_txrx_misc, hw->int_status.fiq_mac_txrx_misc);
	SOC_LOGI("  fiq_mac_rx_trigger en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_rx_trigger, hw->int_raw_status.fiq_mac_rx_trigger, hw->int_status.fiq_mac_rx_trigger);
	SOC_LOGI("  fiq_mac_tx_trigger en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_tx_trigger, hw->int_raw_status.fiq_mac_tx_trigger, hw->int_status.fiq_mac_tx_trigger);
	SOC_LOGI("  fiq_mac_prot_trigger en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_prot_trigger, hw->int_raw_status.fiq_mac_prot_trigger, hw->int_status.fiq_mac_prot_trigger);
	SOC_LOGI("  fiq_mac_general en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_general, hw->int_raw_status.fiq_mac_general, hw->int_status.fiq_mac_general);
	SOC_LOGI("  fiq_mac_wakeup en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_wakeup, hw->int_raw_status.fiq_mac_wakeup, hw->int_status.fiq_mac_wakeup);
	SOC_LOGI("  reserved_4 en=%x raw=%x st=%x\n", hw->int_en.reserved_4, hw->int_raw_status.reserved_4, hw->int_status.reserved_4);
	SOC_LOGI("  fiq_pll_unlock en=%x raw=%x st=%x\n", hw->int_en.fiq_pll_unlock, hw->int_raw_status.fiq_pll_unlock, hw->int_status.fiq_pll_unlock);
	SOC_LOGI("  fiq_btdm en=%x raw=%x st=%x\n", hw->int_en.fiq_btdm, hw->int_raw_status.fiq_btdm, hw->int_status.fiq_btdm);
	SOC_LOGI("  fiq_ble en=%x raw=%x st=%x\n", hw->int_en.fiq_ble, hw->int_raw_status.fiq_ble, hw->int_status.fiq_ble);
	SOC_LOGI("  fiq_bt en=%x raw=%x st=%x\n", hw->int_en.fiq_bt, hw->int_raw_status.fiq_bt, hw->int_status.fiq_bt);
#if CONFIG_SOC_BK7236A
	SOC_LOGI("  fiq_modem_rc en=%x raw=%x st=%x\n", hw->int_en.fiq_modem_rc, hw->int_raw_status.fiq_modem_rc, hw->int_status.fiq_modem_rc);
	SOC_LOGI("  fiq_mac_hsu en=%x raw=%x st=%x\n", hw->int_en.fiq_mac_hsu, hw->int_raw_status.fiq_mac_hsu, hw->int_status.fiq_mac_hsu);
#endif

	SOC_LOGI("  fiq=%x \n", hw->int_global_en.fiq_en);
	SOC_LOGI("  irq=%x \n", hw->int_global_en.irq_en);

	SOC_LOGI("arm wakeup en(%x)=%x\n", &hw->arm_wakeup_en, hw->arm_wakeup_en.v);
	SOC_LOGI("  uart1=%x\n", hw->arm_wakeup_en.uart1);
	SOC_LOGI("  uart2=%x\n", hw->arm_wakeup_en.uart2);
	SOC_LOGI("  i2c1=%x\n", hw->arm_wakeup_en.i2c1);
	SOC_LOGI("  irda=%x\n", hw->arm_wakeup_en.irda);
	SOC_LOGI("  reserved_1=%x\n", hw->arm_wakeup_en.reserved_1);
	SOC_LOGI("  i2c2=%x\n", hw->arm_wakeup_en.i2c2);
	SOC_LOGI("  spi=%x\n", hw->arm_wakeup_en.spi);
	SOC_LOGI("  gpio=%x\n", hw->arm_wakeup_en.gpio);
	SOC_LOGI("  timer=%x\n", hw->arm_wakeup_en.timer);
	SOC_LOGI("  pwm=%x\n", hw->arm_wakeup_en.pwm);
	SOC_LOGI("  reserved_2=%x\n", hw->arm_wakeup_en.reserved_2);
	SOC_LOGI("  saradc=%x\n", hw->arm_wakeup_en.saradc);
	SOC_LOGI("  sdio=%x\n", hw->arm_wakeup_en.sdio);
	SOC_LOGI("  security=%x\n", hw->arm_wakeup_en.security);
	SOC_LOGI("  la=%x\n", hw->arm_wakeup_en.la);
	SOC_LOGI("  general_dma=%x\n", hw->arm_wakeup_en.general_dma);
	SOC_LOGI("  modem=%x\n", hw->arm_wakeup_en.modem);
	SOC_LOGI("  mac_tx_rx_timer=%x\n", hw->arm_wakeup_en.mac_tx_rx_timer);
	SOC_LOGI("  mac_tx_rx_misc=%x\n", hw->arm_wakeup_en.mac_tx_rx_misc);
	SOC_LOGI("  mac_rx_trigger=%x\n", hw->arm_wakeup_en.mac_rx_trigger);
	SOC_LOGI("  mac_tx_trigger=%x\n", hw->arm_wakeup_en.mac_tx_trigger);
	SOC_LOGI("  mac_prot_trigger=%x\n", hw->arm_wakeup_en.mac_prot_trigger);
	SOC_LOGI("  mac_general=%x\n", hw->arm_wakeup_en.mac_general);
	SOC_LOGI("  reserved_3=%x\n", hw->arm_wakeup_en.reserved_3);
	SOC_LOGI("  mac=%x\n", hw->arm_wakeup_en.mac);
	SOC_LOGI("  reserved_4=%x\n", hw->arm_wakeup_en.reserved_4);
	SOC_LOGI("  pll_unlock=%x\n", hw->arm_wakeup_en.pll_unlock);
	SOC_LOGI("  btdm=%x\n", hw->arm_wakeup_en.btdm);
	SOC_LOGI("  ble=%x\n", hw->arm_wakeup_en.ble);
	SOC_LOGI("  bt=%x\n", hw->arm_wakeup_en.bt);
}

#endif
