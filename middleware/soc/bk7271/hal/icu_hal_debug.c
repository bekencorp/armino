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
	SOC_LOGI("  uart3: %x\n", hw->peri_clk_mux.uart3);
	SOC_LOGI("  irda: %x\n", hw->peri_clk_mux.irda);
	SOC_LOGI("  fm_i2c: %x\n", hw->peri_clk_mux.fm_i2c);
	SOC_LOGI("  i2c1: %x\n", hw->peri_clk_mux.i2c1);
	SOC_LOGI("  i2c2: %x\n", hw->peri_clk_mux.i2c2);
	SOC_LOGI("  spi1: %x\n", hw->peri_clk_mux.spi1);
	SOC_LOGI("  spi2: %x\n", hw->peri_clk_mux.spi2);
	SOC_LOGI("  spi3: %x\n", hw->peri_clk_mux.spi3);
	SOC_LOGI("  pwms: %x\n", hw->peri_clk_mux.pwms);
	SOC_LOGI("  sdio_host: %x\n", hw->peri_clk_mux.sdio);
	SOC_LOGI("  efuse: %x\n", hw->peri_clk_mux.efuse);
	SOC_LOGI("  cec: %x\n", hw->peri_clk_mux.cec);
	SOC_LOGI("  reserved_1: %x\n", hw->peri_clk_mux.reserved_1);
	SOC_LOGI("  qspi: %x\n", hw->peri_clk_mux.qspi);
	SOC_LOGI("  dco_clk_div: %x\n", hw->peri_clk_mux.dco_clk_div);
	SOC_LOGI("  clk26m_div_factor: %x\n", hw->peri_clk_mux.clk26m_div_factor);
	SOC_LOGI("  reserved_2: %x\n", hw->peri_clk_mux.reserved_2);
	SOC_LOGI("  i2s_mclk_div: %x\n", hw->peri_clk_mux.i2s_mclk_div);
	SOC_LOGI("  reserved_3: %x\n", hw->peri_clk_mux.reserved_3);

	SOC_LOGI("pwm_group_saradc_clk_mux=%x v=%x\n", &hw->pwm_group_saradc_clk_mux, hw->pwm_group_saradc_clk_mux.v);
	SOC_LOGI("pwm_group lpo clock select:\r\n");
	SOC_LOGI("  group0=%x\n", hw->pwm_group_saradc_clk_mux.pwm_group0);
	SOC_LOGI("  group1=%x\n", hw->pwm_group_saradc_clk_mux.pwm_group1);
	SOC_LOGI("  group2=%x\n", hw->pwm_group_saradc_clk_mux.pwm_group2);
	SOC_LOGI("  group3=%x\n", hw->pwm_group_saradc_clk_mux.pwm_group3);
	SOC_LOGI("  reserved_1=%x\n", hw->pwm_group_saradc_clk_mux.reserved_1);
	SOC_LOGI("  saradc=%x\n", hw->pwm_group_saradc_clk_mux.saradc);
	SOC_LOGI("  reserved_2=%x\n", hw->pwm_group_saradc_clk_mux.reserved_2);

	SOC_LOGI("clk_pwr_down=%x v=%x\n", &hw->clk_pwr_down, hw->clk_pwr_down.v);
	SOC_LOGI("  uart1=%x\n", hw->clk_pwr_down.uart1);
	SOC_LOGI("  uart2=%x\n", hw->clk_pwr_down.uart2);
	SOC_LOGI("  uart3=%x\n", hw->clk_pwr_down.uart3);
	SOC_LOGI("  fm_i2c=%x\n", hw->clk_pwr_down.fm_i2c);
	SOC_LOGI("  i2c1=%x\n", hw->clk_pwr_down.i2c1);
	SOC_LOGI("  i2c2=%x\n", hw->clk_pwr_down.i2c2);
	SOC_LOGI("  sdio_host=%x\n", hw->clk_pwr_down.sdio_host);
	SOC_LOGI("  reserved_1=%x\n", hw->clk_pwr_down.reserved_1);
	SOC_LOGI("  spi1=%x\n", hw->clk_pwr_down.spi1);
	SOC_LOGI("  spi2=%x\n", hw->clk_pwr_down.spi2);
	SOC_LOGI("  spi3=%x\n", hw->clk_pwr_down.spi3);
	SOC_LOGI("  irda=%x\n", hw->clk_pwr_down.irda);
	SOC_LOGI("  timers=%x\n", hw->clk_pwr_down.timers);
	SOC_LOGI("  pwm_group0=%x\n", hw->clk_pwr_down.pwm_group0);
	SOC_LOGI("  pwm_group1=%x\n", hw->clk_pwr_down.pwm_group1);
	SOC_LOGI("  pwm_group2=%x\n", hw->clk_pwr_down.pwm_group2);
	SOC_LOGI("  pwm_group3=%x\n", hw->clk_pwr_down.pwm_group3);
	SOC_LOGI("  saradc=%x\n", hw->clk_pwr_down.saradc);
	SOC_LOGI("  cec=%x\n", hw->clk_pwr_down.cec);
	SOC_LOGI("  reserved_2=%x\n", hw->clk_pwr_down.reserved_2);

	SOC_LOGI("func_clk_pwr_down=%x v=%x\n", &hw->func_clk_pwr_down, hw->func_clk_pwr_down.v);
	SOC_LOGI("  arm_watchdog=%x\n", hw->func_clk_pwr_down.arm_watchdog);
	SOC_LOGI("  usb1=%x\n", hw->func_clk_pwr_down.usb1);
	SOC_LOGI("  usb2=%x\n", hw->func_clk_pwr_down.usb2);
	SOC_LOGI("  jpeg=%x\n", hw->func_clk_pwr_down.jpeg);
	SOC_LOGI("  reserved_1=%x\n", hw->func_clk_pwr_down.reserved_1);
	SOC_LOGI("  security=%x\n", hw->func_clk_pwr_down.security);
	SOC_LOGI("  sdio_dma=%x\n", hw->func_clk_pwr_down.sdio_dma);
	SOC_LOGI("  reserved_2=%x\n", hw->func_clk_pwr_down.reserved_2);

	SOC_LOGI("peri_clk_gate_disable=%x v=%x\n", &hw->peri_clk_gate_disable, hw->peri_clk_gate_disable.v);
	SOC_LOGI("  icu=%x\n", hw->peri_clk_gate_disable.icu);
	SOC_LOGI("  uart=%x\n", hw->peri_clk_gate_disable.uart);
	SOC_LOGI("  i2c=%x\n", hw->peri_clk_gate_disable.i2c);
	SOC_LOGI("  sdio_host=%x\n", hw->peri_clk_gate_disable.sdio_host);
	SOC_LOGI("  reserved_1=%x\n", hw->peri_clk_gate_disable.reserved_1);
	SOC_LOGI("  spi=%x\n", hw->peri_clk_gate_disable.spi);
	SOC_LOGI("  gpio=%x\n", hw->peri_clk_gate_disable.gpio);
	SOC_LOGI("  watchdog=%x\n", hw->peri_clk_gate_disable.watchdog);
	SOC_LOGI("  timers=%x\n", hw->peri_clk_gate_disable.timers);
	SOC_LOGI("  pwms=%x\n", hw->peri_clk_gate_disable.pwms);
	SOC_LOGI("  saradc=%x\n", hw->peri_clk_gate_disable.saradc);
	SOC_LOGI("  cec=%x\n", hw->peri_clk_gate_disable.cec);
	SOC_LOGI("  reserved_2=%x\n", hw->peri_clk_gate_disable.reserved_2);
	SOC_LOGI("  modem=%x\n", hw->peri_clk_gate_disable.modem);
	SOC_LOGI("  mac=%x\n", hw->peri_clk_gate_disable.mac);
	SOC_LOGI("  usb=%x\n", hw->peri_clk_gate_disable.usb);
	SOC_LOGI("  flash=%x\n", hw->peri_clk_gate_disable.flash);
	SOC_LOGI("  qspi=%x\n", hw->peri_clk_gate_disable.qspi);
	SOC_LOGI("  dcache=%x\n", hw->peri_clk_gate_disable.dcache);
	SOC_LOGI("  ahb_bus_memory=%x\n", hw->peri_clk_gate_disable.ahb_bus_memory);
	SOC_LOGI("  sys_ctrl=%x\n", hw->peri_clk_gate_disable.sys_ctrl);
	SOC_LOGI("  sdio_dma=%x\n", hw->peri_clk_gate_disable.sdio_dma);
	SOC_LOGI("  gdma=%x\n", hw->peri_clk_gate_disable.gdma);
	SOC_LOGI("  logic_analyze=%x\n", hw->peri_clk_gate_disable.logic_analyze);
	SOC_LOGI("  reserved_3=%x\n", hw->peri_clk_gate_disable.reserved_3);

	SOC_LOGI("jtag_sel=%x v=%x\n", &hw->jtag_sel, hw->jtag_sel);

	SOC_LOGI("irq_en(%x)=%x raw_status(%x)=%x status(%x)=%x\n", &hw->irq_en, hw->irq_en.v,
			 &hw->irq_raw_st, hw->irq_raw_st.v,
			 &hw->irq_st, hw->irq_st.v);
	SOC_LOGI("  irq_uart1 en=%x raw=%x st=%x\n", hw->irq_en.uart1, hw->irq_raw_st.uart1, hw->irq_st.uart1);
	SOC_LOGI("  irq_uart2 en=%x raw=%x st=%x\n", hw->irq_en.uart2, hw->irq_raw_st.uart2, hw->irq_st.uart2);
	SOC_LOGI("  irq_uart3 en=%x raw=%x st=%x\n", hw->irq_en.uart3, hw->irq_raw_st.uart3, hw->irq_st.uart3);
	SOC_LOGI("  irq_fm_i2c en=%x raw=%x st=%x\n", hw->irq_en.fm_i2c, hw->irq_raw_st.fm_i2c, hw->irq_st.fm_i2c);
	SOC_LOGI("  irq_i2c1 en=%x raw=%x st=%x\n", hw->irq_en.i2c1, hw->irq_raw_st.i2c1, hw->irq_st.i2c1);
	SOC_LOGI("  irq_i2c2 en=%x raw=%x st=%x\n", hw->irq_en.i2c2, hw->irq_raw_st.i2c2, hw->irq_st.i2c2);
	SOC_LOGI("  irq_sdio en=%x raw=%x st=%x\n", hw->irq_en.sdio, hw->irq_raw_st.sdio, hw->irq_st.sdio);
	SOC_LOGI("  irq_usb1 en=%x raw=%x st=%x\n", hw->irq_en.usb1, hw->irq_raw_st.usb1, hw->irq_st.usb1);
	SOC_LOGI("  irq_spi1 en=%x raw=%x st=%x\n", hw->irq_en.spi1, hw->irq_raw_st.spi1, hw->irq_st.spi1);
	SOC_LOGI("  irq_spi2 en=%x raw=%x st=%x\n", hw->irq_en.spi2, hw->irq_raw_st.spi2, hw->irq_st.spi2);
	SOC_LOGI("  irq_spi3 en=%x raw=%x st=%x\n", hw->irq_en.spi3, hw->irq_raw_st.spi3, hw->irq_st.spi3);
	SOC_LOGI("  irq_gpio en=%x raw=%x st=%x\n", hw->irq_en.gpio, hw->irq_raw_st.gpio, hw->irq_st.gpio);
	SOC_LOGI("  irq_irda en=%x raw=%x st=%x\n", hw->irq_en.irda, hw->irq_raw_st.irda, hw->irq_st.irda);
	SOC_LOGI("  irq_timer en=%x raw=%x st=%x\n", hw->irq_en.timer, hw->irq_raw_st.timer, hw->irq_st.timer);
	SOC_LOGI("  irq_pwm1 en=%x raw=%x st=%x\n", hw->irq_en.pwm1, hw->irq_raw_st.pwm1, hw->irq_st.pwm1);
	SOC_LOGI("  irq_pwm2 en=%x raw=%x st=%x\n", hw->irq_en.pwm2, hw->irq_raw_st.pwm2, hw->irq_st.pwm2);
	SOC_LOGI("  irq_saradc en=%x raw=%x st=%x\n", hw->irq_en.saradc, hw->irq_raw_st.saradc, hw->irq_st.saradc);
	SOC_LOGI("  irq_qspi en=%x raw=%x st=%x\n", hw->irq_en.qspi, hw->irq_raw_st.qspi, hw->irq_st.qspi);
	SOC_LOGI("  irq_usb2 en=%x raw=%x st=%x\n", hw->irq_en.usb2, hw->irq_raw_st.usb2, hw->irq_st.usb2);
	SOC_LOGI("  irq_sdio_dma en=%x raw=%x st=%x\n", hw->irq_en.sdio_dma, hw->irq_raw_st.sdio_dma, hw->irq_st.sdio_dma);
	SOC_LOGI("  irq_gdma en=%x raw=%x st=%x\n", hw->irq_en.gdma, hw->irq_raw_st.gdma, hw->irq_st.gdma);
	SOC_LOGI("  irq_mbox1 en=%x raw=%x st=%x\n", hw->irq_en.mbox1, hw->irq_raw_st.mbox1, hw->irq_st.mbox1);
	SOC_LOGI("  irq_mbox2 en=%x raw=%x st=%x\n", hw->irq_en.mbox2, hw->irq_raw_st.mbox2, hw->irq_st.mbox2);
	SOC_LOGI("  irq_security en=%x raw=%x st=%x\n", hw->irq_en.security, hw->irq_raw_st.security, hw->irq_st.security);
	SOC_LOGI("  irq_modem en=%x raw=%x st=%x\n", hw->irq_en.modem, hw->irq_raw_st.modem, hw->irq_st.modem);
	SOC_LOGI("  irq_mac_tx_rx_timer en=%x raw=%x st=%x\n", hw->irq_en.mac_txrx_timer, hw->irq_raw_st.mac_tx_rx_timer, hw->irq_st.mac_tx_rx_timer);
	SOC_LOGI("  irq_mac_tx_rx_misc en=%x raw=%x st=%x\n", hw->irq_en.mac_txrx_misc, hw->irq_raw_st.mac_tx_rx_misc, hw->irq_st.mac_tx_rx_misc);

	SOC_LOGI("  irq_mac_rx_trigger en=%x raw=%x st=%x\n", hw->irq_en.mac_rx_trigger, hw->irq_raw_st.mac_rx_trigger, hw->irq_st.mac_rx_trigger);
	SOC_LOGI("  irq_mac_tx_trigger en=%x raw=%x st=%x\n", hw->irq_en.mac_tx_trigger, hw->irq_raw_st.mac_tx_trigger, hw->irq_st.mac_tx_trigger);
	SOC_LOGI("  irq_mac_prot_trigger en=%x raw=%x st=%x\n", hw->irq_en.mac_prot_trigger, hw->irq_raw_st.mac_prot_trigger, hw->irq_st.mac_prot_trigger);
	SOC_LOGI("  irq_mac_general en=%x raw=%x st=%x\n", hw->irq_en.mac_general, hw->irq_raw_st.mac_general, hw->irq_st.mac_general);
	SOC_LOGI("  irq_mac_wakeup en=%x raw=%x st=%x\n", hw->irq_en.mac_wakeup, hw->irq_raw_st.mac_wakeup, hw->irq_st.mac_wakeup);

	SOC_LOGI("fiq_en(%x)=%x raw_status(%x)=%x status(%x)=%x\n", &hw->fiq_en, hw->fiq_en.v,
			 &hw->fiq_raw_st, hw->fiq_raw_st.v,
			 &hw->fiq_st, hw->fiq_st.v);
	SOC_LOGI("  fiq_pll_unlock en=%x raw=%x st=%x\n", hw->fiq_en.pll_unlock, hw->fiq_raw_st.pll_unlock, hw->fiq_st.pll_unlock);
	SOC_LOGI("  fiq_usb_plug en=%x raw=%x st=%x\n", hw->fiq_en.usb_plug, hw->fiq_raw_st.usb, hw->fiq_st.usb);
	SOC_LOGI("  fiq_bt_watchdog en=%x raw=%x st=%x\n", hw->fiq_en.bt_wdt, hw->fiq_raw_st.bt_watchdog, hw->fiq_st.bt_watchdog);
	SOC_LOGI("  fiq_dsp_watchdog en=%x raw=%x st=%x\n", hw->fiq_en.dsp_wdt, hw->fiq_raw_st.dsp_watchdog, hw->fiq_st.dsp_watchdog);
	SOC_LOGI("  fiq_rtc en=%x raw=%x st=%x\n", hw->fiq_en.rtc, hw->fiq_raw_st.rtc, hw->fiq_st.rtc);
	SOC_LOGI("  fiq_touch en=%x raw=%x st=%x\n", hw->fiq_en.touch, hw->fiq_raw_st.touch, hw->fiq_st.touch);
	SOC_LOGI("  fiq_cec en=%x raw=%x st=%x\n", hw->fiq_en.cec, hw->fiq_raw_st.cec, hw->fiq_st.cec);
	SOC_LOGI("  fiq_jpeg en=%x raw=%x st=%x\n", hw->fiq_en.jpeg, hw->fiq_raw_st.jpeg, hw->fiq_st.jpeg);
	SOC_LOGI("  fiq_reserved en=%x raw=%x st=%x\n", hw->fiq_en.reserved, hw->fiq_raw_st.reserved, hw->fiq_st.reserved);

	SOC_LOGI("int_global_en(%x)=%x\n", &hw->int_global_en, hw->int_global_en.v);
	SOC_LOGI("  irq=%x\n", hw->int_global_en.irq);
	SOC_LOGI("  fiq=%x\n", hw->int_global_en.fiq);

	SOC_LOGI("arm_wakeup(%x)=%x\n", &hw->arm_wakeup_en, hw->arm_wakeup_en.v);
	SOC_LOGI("  uart1=%x\n", hw->arm_wakeup_en.uart1);
	SOC_LOGI("  uart2=%x\n", hw->arm_wakeup_en.uart2);
	SOC_LOGI("  uart3=%x\n", hw->arm_wakeup_en.uart3);
	SOC_LOGI("  fm_i2c=%x\n", hw->arm_wakeup_en.fm_i2c);
	SOC_LOGI("  i2c1=%x\n", hw->arm_wakeup_en.i2c1);
	SOC_LOGI("  sdio_host=%x\n", hw->arm_wakeup_en.sdio_host);
	SOC_LOGI("  usb1=%x\n", hw->arm_wakeup_en.usb1);
	SOC_LOGI("  spi1=%x\n", hw->arm_wakeup_en.spi1);
	SOC_LOGI("  spi2=%x\n", hw->arm_wakeup_en.spi2);
	SOC_LOGI("  spi3=%x\n", hw->arm_wakeup_en.spi3);
	SOC_LOGI("  gpio=%x\n", hw->arm_wakeup_en.gpio);
	SOC_LOGI("  irda=%x\n", hw->arm_wakeup_en.irda);
	SOC_LOGI("  timer=%x\n", hw->arm_wakeup_en.timer);
	SOC_LOGI("  pwm1=%x\n", hw->arm_wakeup_en.pwm1);
	SOC_LOGI("  pwm2=%x\n", hw->arm_wakeup_en.pwm2);
	SOC_LOGI("  saradc=%x\n", hw->arm_wakeup_en.saradc);
	SOC_LOGI("  qspi=%x\n", hw->arm_wakeup_en.qspi);
	SOC_LOGI("  usb2=%x\n", hw->arm_wakeup_en.usb2);
	SOC_LOGI("  sdio_dma=%x\n", hw->arm_wakeup_en.sdio_dma);
	SOC_LOGI("  gdma=%x\n", hw->arm_wakeup_en.gdma);
	SOC_LOGI("  mbox1=%x\n", hw->arm_wakeup_en.mbox1);
	SOC_LOGI("  security=%x\n", hw->arm_wakeup_en.security);
	SOC_LOGI("  modem=%x\n", hw->arm_wakeup_en.modem);
	SOC_LOGI("  mac_tx_rx_timer=%x\n", hw->arm_wakeup_en.mac_txrx_timer);
	SOC_LOGI("  mac_tx_rx_misc=%x\n", hw->arm_wakeup_en.mac_txrx_misc);
	SOC_LOGI("  mac_rx_trigger=%x\n", hw->arm_wakeup_en.mac_rx_trigger);
	SOC_LOGI("  mac_tx_trigger=%x\n", hw->arm_wakeup_en.mac_tx_trigger);
	SOC_LOGI("  mac_prot_trigger=%x\n", hw->arm_wakeup_en.mac_prot_trigger);
	SOC_LOGI("  mac_general=%x\n", hw->arm_wakeup_en.mac_general);
	SOC_LOGI("  mac_wakeup=%x\n", hw->arm_wakeup_en.mac_wakeup);

	SOC_LOGI("arm_wakeup_en2(%x)=%x\n", &hw->arm_wakeup_en2, hw->arm_wakeup_en2.v);
	SOC_LOGI("  pll_unlock=%x\n", hw->arm_wakeup_en2.pll_unlock);
	SOC_LOGI("  usb_plug=%x\n", hw->arm_wakeup_en2.usb_plug);
	SOC_LOGI("  bt_watchdog=%x\n", hw->arm_wakeup_en2.bt_wdt);
	SOC_LOGI("  dsp_wathdog=%x\n", hw->arm_wakeup_en2.dsp_wdt);
	SOC_LOGI("  rtc=%x\n", hw->arm_wakeup_en2.rtc);
	SOC_LOGI("  touch=%x\n", hw->arm_wakeup_en2.touch);
	SOC_LOGI("  cec=%x\n", hw->arm_wakeup_en2.cec);
	SOC_LOGI("  jpeg=%x\n", hw->arm_wakeup_en2.jpeg);

	for (int i = 0; i < SOC_PWM_CHAN_NUM_PER_GROUP; i++)
		SOC_LOGI("gpio_peri_mode_cfg(%x)=%x\n", &hw->gpio_peri_mode_cfg[i], hw->gpio_peri_mode_cfg[i]);


	SOC_LOGI("  uart2=%x\n", hw->gpio_map_cfg.uart2);
	SOC_LOGI("  spi3=%x\n", hw->gpio_map_cfg.spi3);
	SOC_LOGI("  sdio=%x\n", hw->gpio_map_cfg.sdio);
	SOC_LOGI("  sdio_host_en=%x\n", hw->gpio_map_cfg.sdio_host);
	SOC_LOGI("  pwm6=%x\n", hw->gpio_map_cfg.pwm6);
	SOC_LOGI("  pwm7=%x\n", hw->gpio_map_cfg.pwm7);
	SOC_LOGI("  pwm8=%x\n", hw->gpio_map_cfg.pwm8);
	SOC_LOGI("  pwm9=%x\n", hw->gpio_map_cfg.pwm9);
	SOC_LOGI("  spdif=%x\n", hw->gpio_map_cfg.spdif);

	SOC_LOGI("debug_mux(%x)=%x\n", &hw->debug_mux, hw->debug_mux.v);
	SOC_LOGI("debug_report(%x)=%x\n", &hw->debug_mux, hw->debug_report);
	SOC_LOGI("touch_ctrl(%x)=%x\n", &hw->touch_ctrl, hw->touch_ctrl.v);
	SOC_LOGI("  en=%x\n", hw->touch_ctrl.en);
	SOC_LOGI("  st=%x\n", hw->touch_ctrl.st);
	SOC_LOGI("\r\n");

	SOC_LOGI("gpio_peri_mode_cfg4(%x)=%x\n", &hw->gpio_peri_mode_cfg4, hw->gpio_peri_mode_cfg4.v);
	SOC_LOGI("  gpio_pcfg4: %x\n", hw->gpio_peri_mode_cfg4.gpio_pcfg4);
	SOC_LOGI("  reserved:   %x\n", hw->gpio_peri_mode_cfg4.reserved);
	SOC_LOGI("\r\n");

	SOC_LOGI("gpio_peri_mode_cfg5(%x)=%x\n", &hw->gpio_peri_mode_cfg5, hw->gpio_peri_mode_cfg5.v);
	SOC_LOGI("  gpio_pcfg5: %x\n", hw->gpio_peri_mode_cfg5.gpio_pcfg5);
	SOC_LOGI("  reserved:   %x\n", hw->gpio_peri_mode_cfg5.reserved);
}

#endif
