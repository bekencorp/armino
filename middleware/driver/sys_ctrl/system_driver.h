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
#include <components/log.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SYSTEM_TAG "system"
#define SYSTEM_LOGI(...) BK_LOGI(SYSTEM_TAG, ##__VA_ARGS__)
#define SYSTEM_LOGW(...) BK_LOGW(SYSTEM_TAG, ##__VA_ARGS__)
#define SYSTEM_LOGE(...) BK_LOGE(SYSTEM_TAG, ##__VA_ARGS__)
#define SYSTEM_LOGD(...) BK_LOGD(SYSTEM_TAG, ##__VA_ARGS__)

void system_driver_init();

//0x0
uint32_t system_driver_get_device_id();

//0x01
uint32_t system_driver_get_version_id();

//0x02
uint32_t system_driver_get_core0_halted_st();
uint32_t system_driver_get_core1_halted_st();
uint32_t system_driver_get_cpu0_sw_reset_st();
uint32_t system_driver_get_cpu1_sw_reset_st();
uint32_t system_driver_get_cpu0_pwr_dw_st();
uint32_t system_driver_get_cpu1_pwr_dw_st();

//0x03
void system_driver_set_boot_rom_mode();
void system_driver_set_boot_flash_mode();
uint32_t system_driver_get_boot_mode();

void system_driver_set_jtag_connect_core0();
void system_driver_set_jtag_connect_core1();
uint32_t system_driver_get_jtag_connect_core();

void system_driver_set_flash_select_normal_op();
void system_driver_set_flash_select_spi_op();
uint32_t system_driver_get_flash_select_op();

//0x04
void system_driver_set_cpu0_int_mask(bool value);
uint32_t system_driver_get_cpu0_int_mask();

void system_driver_set_core0_halt_indicate(bool value);
uint32_t system_driver_get_core0_halt_indicate();

void system_driver_set_cpu0_clk_div(bool value);
uint32_t system_driver_get_cpu0_clk_div();


//0x05
void system_driver_set_cpu1_int_mask(bool value);
uint32_t system_driver_get_cpu1_int_mask();

void system_driver_set_core1_halt_indicate(bool value);
uint32_t system_driver_get_core1_halt_indicate();

void system_driver_set_cpu1_clk_div(uint32_t value);
uint32_t system_driver_get_cpu1_clk_div();


//0x08
void system_driver_set_core_clk_div(bool value);
uint32_t system_driver_get_core_clk_div();

void system_driver_set_core_clk_sel(bool value);
uint32_t system_driver_get_core_clk_sel();

void system_driver_set_bus_clk_div(bool value);
uint32_t system_driver_get_bus_clk_div();

void system_driver_set_uart0_clk_div(uint32_t value);
uint32_t system_driver_get_uart0_clk_div();
void system_driver_set_uart0_clk_sel(bool value);
uint32_t system_driver_get_uart0_clk_sel();

void system_driver_set_uart1_clk_div(uint32_t value);
uint32_t system_driver_get_uart1_clk_div();
void system_driver_set_uart1_clk_sel(bool value);
uint32_t system_driver_get_uart1_clk_sel();

void system_driver_set_uart2_clk_div(bool value);
uint32_t system_driver_get_uart2_clk_div();
void system_driver_set_uart2_clk_sel(bool value);
uint32_t system_driver_get_uart2_clk_sel();

void system_driver_set_sadc_clk_sel(bool value);
uint32_t system_driver_get_sadc_clk_sel();

void system_driver_set_pwm0_clk_sel(bool value);
uint32_t system_driver_get_pwm0_clk_sel();

void system_driver_set_pwm1_clk_sel(bool value);
uint32_t system_driver_get_pwm1_clk_sel();

void system_driver_set_timer0_clk_sel(bool value);
uint32_t system_driver_get_timer0_clk_sel();

void system_driver_set_timer1_clk_sel(bool value);
uint32_t system_driver_get_timer1_clk_sel();

void system_driver_set_timer2_clk_sel(bool value);
uint32_t system_driver_get_timer2_clk_sel();

void system_driver_set_i2s_clk_sel(bool value);
uint32_t system_driver_get_i2s_clk_sel();

void system_driver_set_aud_clk_sel(bool value);
uint32_t system_driver_get_aud_clk_sel();

void system_driver_set_jpeg_clk_div(uint32_t value);
uint32_t system_driver_get_jpeg_clk_div();
void system_driver_set_jpeg_clk_sel(bool value);
uint32_t system_driver_get_jpeg_clk_sel();


void system_driver_set_disp_l_clk_div(uint32_t value);
uint32_t system_driver_get_disp_l_clk_div();

//0x09
void system_driver_set_disp_h_clk_div(bool value);
uint32_t system_driver_get_disp_h_clk_div();
void system_driver_set_disp_clk_sel(bool value);
uint32_t system_driver_get_disp_clk_sel();


void system_driver_set_psram_clk_div(uint32_t value);
uint32_t system_driver_get_psram_clk_div();
void system_driver_set_psram_clk_sel(bool value);
uint32_t system_driver_get_psram_clk_sel();

void system_driver_set_qspi0_clk_div(uint32_t value);
uint32_t system_driver_get_qspi0_clk_div();

void system_driver_set_sdio_clk_div(uint32_t value);
uint32_t system_driver_get_sdio_clk_div();
void system_driver_set_sdio_clk_sel(bool value);
uint32_t system_driver_get_sdio_clk_sel();

void system_driver_set_auxs_clk_div(uint32_t value);
uint32_t system_driver_get_auxs_clk_div();
void system_driver_set_auxs_clk_sel(bool value);
uint32_t system_driver_get_auxs_clk_sel();

void system_driver_set_flash_clk_sel(bool value);
uint32_t system_driver_get_flash_clk_sel();
void system_driver_set_flash_clk_div(uint32_t value);
uint32_t system_driver_get_flash_clk_div();

void system_driver_set_i2s0_clk_div(uint32_t value);
uint32_t system_driver_get_i2s0_clk_div();

//0x0a
void system_driver_set_26m_clk_div(uint32_t value);
uint32_t system_driver_get_26m_clk_div();
void system_driver_set_wdt_clk_div(bool value);
uint32_t system_driver_get_wdt_clk_div();

//0x0b
void system_driver_set_anaspi_freq(uint32_t value);
uint32_t system_driver_get_anaspi_freq();

//0x0c
void system_driver_set_i2c0_clk_en(bool value);
uint32_t system_driver_get_i2c0_clk_en();

void system_driver_set_spi0_clk_en(bool value);
uint32_t system_driver_get_spi0_clk_en();

void system_driver_set_uart0_clk_en(bool value);
uint32_t system_driver_get_uart0_clk_en();

void system_driver_set_pwm0_clk_en(bool value);
uint32_t system_driver_get_pwm0_clk_en();

void system_driver_set_timer0_clk_en(bool value);
uint32_t system_driver_get_timer0_clk_en();

void system_driver_set_sadc_clk_en(bool value);
uint32_t system_driver_get_sadc_clk_en();

void system_driver_set_irda_clk_en(bool value);
uint32_t system_driver_get_irda_clk_en();

void system_driver_set_efuse_clk_en(bool value);
uint32_t system_driver_get_efuse_clk_en();

void system_driver_set_i2c1_clk_en(bool value);
uint32_t system_driver_get_i2c1_clk_en();

void system_driver_set_spi1_clk_en(bool value);
uint32_t system_driver_get_spi1_clk_en();

void system_driver_set_uart1_clk_en(bool value);
uint32_t system_driver_get_uart1_clk_en();

void system_driver_set_uart2_clk_en(bool value);
uint32_t system_driver_get_uart2_clk_en();

void system_driver_set_pwm1_clk_en(bool value);
uint32_t system_driver_get_pwm1_clk_en();

void system_driver_set_timer1_clk_en(bool value);
uint32_t system_driver_get_timer1_clk_en();

void system_driver_set_timer2_clk_en(bool value);
uint32_t system_driver_get_timer2_clk_en();

void system_driver_set_otp_clk_en(bool value);
uint32_t system_driver_get_otp_clk_en();

void system_driver_set_i2s_clk_en(bool value);
uint32_t system_driver_get_i2s_clk_en();

void system_driver_set_usb_clk_en(bool value);
uint32_t system_driver_get_usb_clk_en();

void system_driver_set_can_clk_en(bool value);
uint32_t system_driver_get_can_clk_en();

void system_driver_set_psram_clk_en(bool value);
uint32_t system_driver_get_psram_clk_en();

void system_driver_set_qspi0_clk_en(bool value);
uint32_t system_driver_get_qspi0_clk_en();

void system_driver_set_qspi1_clk_en(bool value);
uint32_t system_driver_get_qspi1_clk_en();

void system_driver_set_sdio_clk_en(bool value);
uint32_t system_driver_get_sdio_clk_en();

void system_driver_set_auxs_clk_en(bool value);
uint32_t system_driver_get_auxs_clk_en();

void system_driver_set_btdm_clk_en(bool value);
uint32_t system_driver_get_btdm_clk_en();

void system_driver_set_xvr_clk_en(bool value);
uint32_t system_driver_get_xvr_clk_en();

void system_driver_set_mac_clk_en(bool value);
uint32_t system_driver_get_mac_clk_en();

void system_driver_set_phy_clk_en(bool value);
uint32_t system_driver_get_phy_clk_en();

void system_driver_set_jpeg_clk_en(bool value);
uint32_t system_driver_get_jpeg_clk_en();

void system_driver_set_disp_clk_en(bool value);
uint32_t system_driver_get_disp_clk_en();

void system_driver_set_aud_clk_en(bool value);
uint32_t system_driver_get_aud_clk_en();

void system_driver_set_wdt_clk_en(bool value);
uint32_t system_driver_get_wdt_clk_en();


//0x0e
void system_driver_set_aon_disckg(bool value);
uint32_t system_driver_get_aon_disckg();

void system_driver_set_sys_disckg(bool value);
uint32_t system_driver_get_sys_disckg();

void system_driver_set_dma_disckg(bool value);
uint32_t system_driver_get_dma_disckg();

void system_driver_set_flash_disckg(bool value);
uint32_t system_driver_get_flash_disckg();

void system_driver_set_wdt_disckg(bool value);
uint32_t system_driver_get_wdt_disckg();

void system_driver_set_timer0_disckg(bool value);
uint32_t system_driver_get_timer0_disckg();

void system_driver_set_uart0_disckg(bool value);
uint32_t system_driver_get_uart0_disckg();

void system_driver_set_pwm0_disckg(bool value);
uint32_t system_driver_get_pwm0_disckg();

void system_driver_set_i2c0_disckg(bool value);
uint32_t system_driver_get_i2c0_disckg();

void system_driver_set_spi0_disckg(bool value);
uint32_t system_driver_get_spi0_disckg();

void system_driver_set_sadc_disckg(bool value);
uint32_t system_driver_get_sadc_disckg();

void system_driver_set_efuse_disckg(bool value);
uint32_t system_driver_get_efuse_disckg();

void system_driver_set_irda_disckg(bool value);
uint32_t system_driver_get_irda_disckg();

void system_driver_set_trng_disckg(bool value);
uint32_t system_driver_get_trng_disckg();

void system_driver_set_sdio_disckg(bool value);
uint32_t system_driver_get_sdio_disckg();

void system_driver_set_la_disckg(bool value);
uint32_t system_driver_get_la_disckg();

void system_driver_set_timer1_disckg(bool value);
uint32_t system_driver_get_timer1_disckg();

void system_driver_set_uart1_disckg(bool value);
uint32_t system_driver_get_uart1_disckg();
void system_driver_set_uart2_disckg(bool value);
uint32_t system_driver_get_uart2_disckg();

void system_driver_set_pwm1_disckg(bool value);
uint32_t system_driver_get_pwm1_disckg();

void system_driver_set_i2c1_disckg(bool value);
uint32_t system_driver_get_i2c1_disckg();
void system_driver_set_spi1_disckg(bool value);
uint32_t system_driver_get_spi1_disckg();

void system_driver_set_usb_disckg(bool value);
uint32_t system_driver_get_usb_disckg();
void system_driver_set_can_disckg(bool value);
uint32_t system_driver_get_can_disckg();

void system_driver_set_qspi0_disckg(bool value);
uint32_t system_driver_get_qspi0_disckg();
void system_driver_set_psram_disckg(bool value);
uint32_t system_driver_get_psram_disckg();
void system_driver_set_fft_disckg(bool value);
uint32_t system_driver_get_fft_disckg();
void system_driver_set_sbc_disckg(bool value);
uint32_t system_driver_get_sbc_disckg();
void system_driver_set_aud_disckg(bool value);
uint32_t system_driver_get_aud_disckg();
void system_driver_set_i2s_disckg(bool value);
uint32_t system_driver_get_i2s_disckg();

void system_driver_set_jpeg_disckg(bool value);
uint32_t system_driver_get_jpeg_disckg();
void system_driver_set_jpeg_dec_disckg(bool value);
uint32_t system_driver_get_jpeg_dec_disckg();

//0xff
void system_driver_set_disp_disckg(bool value);
uint32_t system_driver_get_disp_disckg();

void system_driver_set_dma2d_disckg(bool value);
uint32_t system_driver_get_dma2d_disckg();

void system_driver_set_btdm_disckg(bool value);
uint32_t system_driver_get_btdm_disckg();

void system_driver_set_xver_disckg(bool value);
uint32_t system_driver_get_xver_disckg();

void system_driver_set_btdm_bps_disckg(bool value);
uint32_t system_driver_get_btdm_bps_disckg();

//0x10
void system_driver_set_power_on_mem1(bool value);
uint32_t system_driver_get_power_on_mem1();
void system_driver_set_power_on_mem2(bool value);
uint32_t system_driver_get_power_on_mem2();
void system_driver_set_power_on_mem3(bool value);
uint32_t system_driver_get_power_on_mem3();

void system_driver_set_power_on_encp(bool value);
uint32_t system_driver_get_power_on_encp();
void system_driver_set_power_on_bakp(bool value);
uint32_t system_driver_get_power_on_bakp();
void system_driver_set_power_on_ahbp(bool value);
uint32_t system_driver_get_power_on_ahbp();
void system_driver_set_power_on_audp(bool value);
uint32_t system_driver_get_power_on_audp();

void system_driver_set_power_on_vidp(bool value);
uint32_t system_driver_get_power_on_vidp();
void system_driver_set_power_on_btsp(bool value);
uint32_t system_driver_get_power_on_btsp();
void system_driver_set_power_on_wifi_mac(bool value);
uint32_t system_driver_get_power_on_wifi_mac();
void system_driver_set_power_on_wifi_phy(bool value);
uint32_t system_driver_get_power_on_wifi_phy();

void system_driver_set_sleep_en_need_flash_idle(bool value);
uint32_t system_driver_get_sleep_en_need_flash_idle();
void system_driver_set_sleep_en_need_cpu1_wifi(bool value);
uint32_t system_driver_get_sleep_en_need_cpu1_wifi();
void system_driver_set_sleep_en_need_cpu0_wifi(bool value);
uint32_t system_driver_get_sleep_en_need_cpu0_wifi();
void system_driver_set_sleep_en_need_global(bool value);
uint32_t system_driver_get_sleep_en_need_global();

void system_driver_set_wifi_wakeup_platform_en(bool value);
uint32_t system_driver_get_wifi_wakeup_platform_en();
void system_driver_set_bts_wakeup_platform_en(bool value);
uint32_t system_driver_get_bts_wakeup_platform_en();
void system_driver_set_bts_sleep_exit_req(bool value);
uint32_t system_driver_get_bts_sleep_exit_req();

//0x20
void system_driver_set_cpu0_bmc32_int_en(bool value);
uint32_t system_driver_get_cpu0_bmc32_int_en();

void system_driver_set_cpu0_host_0_irq_en(bool value);
uint32_t system_driver_get_cpu0_host_0_irq_en();

void system_driver_set_cpu0_host_0_sec_irq_en(bool value);
uint32_t system_driver_get_cpu0_host_0_sec_irq_en();

void system_driver_set_cpu0_timer_int_en(bool value);
uint32_t system_driver_get_cpu0_timer_int_en();

void system_driver_set_cpu0_uart_int_en(bool value);
uint32_t system_driver_get_cpu0_uart_int_en();

void system_driver_set_cpu0_pwm_int_en(bool value);
uint32_t system_driver_get_cpu0_pwm_int_en();

void system_driver_set_cpu0_i2c_int_en(bool value);
uint32_t system_driver_get_cpu0_i2c_int_en();

void system_driver_set_cpu0_spi_int_en(bool value);
uint32_t system_driver_get_cpu0_spi_int_en();

void system_driver_set_cpu0_sadc_int_en(bool value);
uint32_t system_driver_get_cpu0_sadc_int_en();

void system_driver_set_cpu0_irda_int_en(bool value);
uint32_t system_driver_get_cpu0_irda_int_en();

void system_driver_set_cpu0_sdio_int_en(bool value);
uint32_t system_driver_get_cpu0_sdio_int_en();

void system_driver_set_cpu0_gdma_int_en(bool value);
uint32_t system_driver_get_cpu0_gdma_int_en();

void system_driver_set_cpu0_la_int_en(bool value);
uint32_t system_driver_get_cpu0_la_int_en();

void system_driver_set_cpu0_timer1_int_en(bool value);
uint32_t system_driver_get_cpu0_timer1_int_en();

void system_driver_set_cpu0_i2c1_int_en(bool value);
uint32_t system_driver_get_cpu0_i2c1_int_en();

void system_driver_set_cpu0_uart1_int_en(bool value);
uint32_t system_driver_get_cpu0_uart1_int_en();

void system_driver_set_cpu0_uart2_int_en(bool value);
uint32_t system_driver_get_cpu0_uart2_int_en();

void system_driver_set_cpu0_spi1_int_en(bool value);
uint32_t system_driver_get_cpu0_spi1_int_en();

void system_driver_set_cpu0_can_int_en(bool value);
uint32_t system_driver_get_cpu0_can_int_en();

void system_driver_set_cpu0_usb_int_en(bool value);
uint32_t system_driver_get_cpu0_usb_int_en();

void system_driver_set_cpu0_qspi_int_en(bool value);
uint32_t system_driver_get_cpu0_qspi_int_en();

void system_driver_set_cpu0_fft_int_en(bool value);
uint32_t system_driver_get_cpu0_fft_int_en();

void system_driver_set_cpu0_sbc_int_en(bool value);
uint32_t system_driver_get_cpu0_sbc_int_en();

void system_driver_set_cpu0_aud_int_en(bool value);
uint32_t system_driver_get_cpu0_aud_int_en();

void system_driver_set_cpu0_i2s_int_en(bool value);
uint32_t system_driver_get_cpu0_i2s_int_en();

void system_driver_set_cpu0_jpegenc_int_en(bool value);
uint32_t system_driver_get_cpu0_jpegenc_int_en();

void system_driver_set_cpu0_jpegdec_int_en(bool value);
uint32_t system_driver_get_cpu0_jpegdec_int_en();

void system_driver_set_cpu0_lcd_int_en(bool value);
uint32_t system_driver_get_cpu0_lcd_int_en();

void system_driver_set_cpu0_wifi_int_phy_en(bool value);
uint32_t system_driver_get_cpu0_wifi_int_phy_en();

void system_driver_set_cpu0_wifi_mac_int_timer_en(bool value);
uint32_t system_driver_get_cpu0_wifi_mac_int_timer_en();

void system_driver_set_cpu0_wifi_mac_int_misc_en(bool value);
uint32_t system_driver_get_cpu0_wifi_mac_int_misc_en();

//0x21
void system_driver_set_cpu0_wifi_mac_int_rx_trigger_en(bool value);
uint32_t system_driver_get_cpu0_wifi_mac_int_rx_trigger_en();

void system_driver_set_cpu0_wifi_mac_int_tx_trigger_en(bool value);
uint32_t system_driver_get_cpu0_wifi_mac_int_tx_trigger_en();

void system_driver_set_cpu0_wifi_mac_port_trigger_en(bool value);
uint32_t system_driver_get_cpu0_wifi_mac_port_trigger_en();

void system_driver_set_cpu0_wifi_mac_int_gen_en(bool value);
uint32_t system_driver_get_cpu0_wifi_mac_int_gen_en();

void system_driver_set_cpu0_wifi_hsu_irq_en(bool value);
uint32_t system_driver_get_cpu0_wifi_hsu_irq_en();

void system_driver_set_cpu0_wifi_int_mac_wakeup_en(bool value);
uint32_t system_driver_get_cpu0_wifi_int_mac_wakeup_en();

void system_driver_set_cpu0_dm_irq_en(bool value);
uint32_t system_driver_get_cpu0_dm_irq_en();

void system_driver_set_cpu0_ble_irq_en(bool value);
uint32_t system_driver_get_cpu0_ble_irq_en();

void system_driver_set_cpu0_bt_irq_en(bool value);
uint32_t system_driver_get_cpu0_bt_irq_en();

void system_driver_set_cpu0_mbox0_int_en(bool value);
uint32_t system_driver_get_cpu0_mbox0_int_en();

void system_driver_set_cpu0_mbox1_int_en(bool value);
uint32_t system_driver_get_cpu0_mbox1_int_en();

void system_driver_set_cpu0_bmc64_int_en(bool value);
uint32_t system_driver_get_cpu0_bmc64_int_en();

void system_driver_set_cpu0_touched_int_en(bool value);
uint32_t system_driver_get_cpu0_touched_int_en();

void system_driver_set_cpu0_usbplug_int_en(bool value);
uint32_t system_driver_get_cpu0_usbplug_int_en();

void system_driver_set_cpu0_rtc_int_en(bool value);
uint32_t system_driver_get_cpu0_rtc_int_en();
void system_driver_set_cpu0_gpio_int_en(bool value);
uint32_t system_driver_get_cpu0_gpio_int_en();

//0x22
void system_driver_set_cpu1_bmc32_int_en(bool value);
uint32_t system_driver_get_cpu1_bmc32_int_en();
void system_driver_set_cpu1_host_0_irq_en(bool value);
uint32_t system_driver_get_cpu1_host_0_irq_en();
void system_driver_set_cpu1_host_0_sec_irq_en(bool value);
uint32_t system_driver_get_cpu1_host_0_sec_irq_en();
void system_driver_set_cpu1_timer_int_en(bool value);
uint32_t system_driver_get_cpu1_timer_int_en();
void system_driver_set_cpu1_uart_int_en(bool value);
uint32_t system_driver_get_cpu1_uart_int_en();
void system_driver_set_cpu1_pwm_int_en(bool value);
uint32_t system_driver_get_cpu1_pwm_int_en();
void system_driver_set_cpu1_i2c_int_en(bool value);
uint32_t system_driver_get_cpu1_i2c_int_en();
void system_driver_set_cpu1_spi_int_en(bool value);
uint32_t system_driver_get_cpu1_spi_int_en();
void system_driver_set_cpu1_sadc_int_en(bool value);
uint32_t system_driver_get_cpu1_sadc_int_en();
void system_driver_set_cpu1_irda_int_en(bool value);
uint32_t system_driver_get_cpu1_irda_int_en();
void system_driver_set_cpu1_sdio_int_en(bool value);
uint32_t system_driver_get_cpu1_sdio_int_en();
void system_driver_set_cpu1_gdma_int_en(bool value);
uint32_t system_driver_get_cpu1_gdma_int_en();
void system_driver_set_cpu1_la_int_en(bool value);
uint32_t system_driver_get_cpu1_la_int_en();
void system_driver_set_cpu1_timer1_int_en(bool value);
uint32_t system_driver_get_cpu1_timer1_int_en();
void system_driver_set_cpu1_i2c1_int_en(bool value);
uint32_t system_driver_get_cpu1_i2c1_int_en();
void system_driver_set_cpu1_uart1_int_en(bool value);
uint32_t system_driver_get_cpu1_uart1_int_en();
void system_driver_set_cpu1_uart2_int_en(bool value);
uint32_t system_driver_get_cpu1_uart2_int_en();
void system_driver_set_cpu1_spi1_int_en(bool value);
uint32_t system_driver_get_cpu1_spi1_int_en();
void system_driver_set_cpu1_can_int_en(bool value);
uint32_t system_driver_get_cpu1_can_int_en();
void system_driver_set_cpu1_usb_int_en(bool value);
uint32_t system_driver_get_cpu1_usb_int_en();
void system_driver_set_cpu1_qspi_int_en(bool value);
uint32_t system_driver_get_cpu1_qspi_int_en();
void system_driver_set_cpu1_fft_int_en(bool value);
uint32_t system_driver_get_cpu1_fft_int_en();
void system_driver_set_cpu1_sbc_int_en(bool value);
uint32_t system_driver_get_cpu1_sbc_int_en();
void system_driver_set_cpu1_aud_int_en(bool value);
uint32_t system_driver_get_cpu1_aud_int_en();
void system_driver_set_cpu1_i2s_int_en(bool value);
uint32_t system_driver_get_cpu1_i2s_int_en();
void system_driver_set_cpu1_jpegenc_int_en(bool value);
uint32_t system_driver_get_cpu1_jpegenc_int_en();
void system_driver_set_cpu1_jpegdec_int_en(bool value);
uint32_t system_driver_get_cpu1_jpegdec_int_en();
void system_driver_set_cpu1_lcd_int_en(bool value);
uint32_t system_driver_get_cpu1_lcd_int_en();
void system_driver_set_cpu1_wifi_int_phy_en(bool value);
uint32_t system_driver_get_cpu1_wifi_int_phy_en();
void system_driver_set_cpu1_wifi_mac_int_timer_en(bool value);
uint32_t system_driver_get_cpu1_wifi_mac_int_timer_en();
void system_driver_set_cpu1_wifi_mac_int_misc_en(bool value);
uint32_t system_driver_get_cpu1_wifi_mac_int_misc_en();

//0x23
void system_driver_set_cpu1_wifi_mac_int_rx_trigger_en(bool value);
uint32_t system_driver_get_cpu1_wifi_mac_int_rx_trigger_en();
void system_driver_set_cpu1_wifi_mac_int_tx_trigger_en(bool value);
uint32_t system_driver_get_cpu1_wifi_mac_int_tx_trigger_en();
void system_driver_set_cpu1_wifi_mac_port_trigger_en(bool value);
uint32_t system_driver_get_cpu1_wifi_mac_port_trigger_en();
void system_driver_set_cpu1_wifi_mac_int_gen_en(bool value);
uint32_t system_driver_get_cpu1_wifi_mac_int_gen_en();
void system_driver_set_cpu1_wifi_hsu_irq_en(bool value);
uint32_t system_driver_get_cpu1_wifi_hsu_irq_en();
void system_driver_set_cpu1_wifi_int_mac_wakeup_en(bool value);
uint32_t system_driver_get_cpu1_wifi_int_mac_wakeup_en();
void system_driver_set_cpu1_dm_irq_en(bool value);
uint32_t system_driver_get_cpu1_dm_irq_en();
void system_driver_set_cpu1_ble_irq_en(bool value);
uint32_t system_driver_get_cpu1_ble_irq_en();
void system_driver_set_cpu1_bt_irq_en(bool value);
uint32_t system_driver_get_cpu1_bt_irq_en();
void system_driver_set_cpu1_mbox0_int_en(bool value);
uint32_t system_driver_get_cpu1_mbox0_int_en();
void system_driver_set_cpu1_mbox1_int_en(bool value);
uint32_t system_driver_get_cpu1_mbox1_int_en();
void system_driver_set_cpu1_bmc64_int_en(bool value);
uint32_t system_driver_get_cpu1_bmc64_int_en();
void system_driver_set_cpu1_touched_int_en(bool value);
uint32_t system_driver_get_cpu1_touched_int_en();
void system_driver_set_cpu1_usbplug_int_en(bool value);
uint32_t system_driver_get_cpu1_usbplug_int_en();
void system_driver_set_cpu1_rtc_int_en(bool value);
uint32_t system_driver_get_cpu1_rtc_int_en();
void system_driver_set_cpu1_gpio_int_en(bool value);
uint32_t system_driver_get_cpu1_gpio_int_en();

//0x28
uint32_t system_driver_get_cpu0_bmc32_int_st();
uint32_t system_driver_get_cpu0_host_0_irq_st();
uint32_t system_driver_get_cpu0_host_0_sec_irq_st();
uint32_t system_driver_get_cpu0_timer_int_st();
uint32_t system_driver_get_cpu0_uart_int_st();
uint32_t system_driver_get_cpu0_pwm_int_st();
uint32_t system_driver_get_cpu0_i2c_int_st();
uint32_t system_driver_get_cpu0_spi_int_st();
uint32_t system_driver_get_cpu0_sadc_int_st();
uint32_t system_driver_get_cpu0_irda_int_st();
uint32_t system_driver_get_cpu0_sdio_int_st();
uint32_t system_driver_get_cpu0_gdma_int_st();
uint32_t system_driver_get_cpu0_la_int_st();
uint32_t system_driver_get_cpu0_timer1_int_st();
uint32_t system_driver_get_cpu0_i2c1_int_st();
uint32_t system_driver_get_cpu0_uart1_int_st();
uint32_t system_driver_get_cpu0_uart2_int_st();
uint32_t system_driver_get_cpu0_spi1_int_st();
uint32_t system_driver_get_cpu0_can_int_st();
uint32_t system_driver_get_cpu0_usb_int_st();
uint32_t system_driver_get_cpu0_qspi_int_st();
uint32_t system_driver_get_cpu0_fft_int_st();
uint32_t system_driver_get_cpu0_sbc_int_st();
uint32_t system_driver_get_cpu0_aud_int_st();
uint32_t system_driver_get_cpu0_i2s_int_st();
uint32_t system_driver_get_cpu0_jpegenc_int_st();
uint32_t system_driver_get_cpu0_jpegdec_int_st();
uint32_t system_driver_get_cpu0_lcd_int_st();
uint32_t system_driver_get_cpu0_wifi_int_phy_st();
uint32_t system_driver_get_cpu0_wifi_mac_int_timer_st();
uint32_t system_driver_get_cpu0_wifi_mac_int_misc_st();

//0x29
uint32_t system_driver_get_cpu0_wifi_mac_int_rx_trigger_st();
uint32_t system_driver_get_cpu0_wifi_mac_int_tx_trigger_st();
uint32_t system_driver_get_cpu0_wifi_mac_port_trigger_st();
uint32_t system_driver_get_cpu0_wifi_mac_int_gen_st();
uint32_t system_driver_get_cpu0_wifi_hsu_irq_st();
uint32_t system_driver_get_cpu0_wifi_int_mac_wakeup_st();
uint32_t system_driver_get_cpu0_dm_irq_st();
uint32_t system_driver_get_cpu0_ble_irq_st();
uint32_t system_driver_get_cpu0_bt_irq_st();
uint32_t system_driver_get_cpu0_mbox0_int_st();
uint32_t system_driver_get_cpu0_mbox1_int_st();
uint32_t system_driver_get_cpu0_bmc64_int_st();
uint32_t system_driver_get_cpu0_touched_int_st();
uint32_t system_driver_get_cpu0_usbplug_int_st();
uint32_t system_driver_get_cpu0_rtc_int_st();
uint32_t system_driver_get_cpu0_gpio_int_st();

//0x2a
uint32_t system_driver_get_cpu1_bmc32_int_st();
uint32_t system_driver_get_cpu1_host_0_irq_st();
uint32_t system_driver_get_cpu1_host_0_sec_irq_st();
uint32_t system_driver_get_cpu1_timer_int_st();
uint32_t system_driver_get_cpu1_uart_int_st();
uint32_t system_driver_get_cpu1_pwm_int_st();
uint32_t system_driver_get_cpu1_i2c_int_st();
uint32_t system_driver_get_cpu1_spi_int_st();
uint32_t system_driver_get_cpu1_sadc_int_st();
uint32_t system_driver_get_cpu1_irda_int_st();
uint32_t system_driver_get_cpu1_sdio_int_st();
uint32_t system_driver_get_cpu1_gdma_int_st();
uint32_t system_driver_get_cpu1_la_int_st();
uint32_t system_driver_get_cpu1_timer1_int_st();
uint32_t system_driver_get_cpu1_i2c1_int_st();
uint32_t system_driver_get_cpu1_uart1_int_st();
uint32_t system_driver_get_cpu1_uart2_int_st();
uint32_t system_driver_get_cpu1_spi1_int_st();
uint32_t system_driver_get_cpu1_can_int_st();
uint32_t system_driver_get_cpu1_usb_int_st();
uint32_t system_driver_get_cpu1_qspi_int_st();
uint32_t system_driver_get_cpu1_fft_int_st();
uint32_t system_driver_get_cpu1_sbc_int_st();
uint32_t system_driver_get_cpu1_aud_int_st();
uint32_t system_driver_get_cpu1_i2s_int_st();
uint32_t system_driver_get_cpu1_jpegenc_int_st();
uint32_t system_driver_get_cpu1_jpegdec_int_st();
uint32_t system_driver_get_cpu1_lcd_int_st();
uint32_t system_driver_get_cpu1_wifi_int_phy_st();
uint32_t system_driver_get_cpu1_wifi_mac_int_timer_st();
uint32_t system_driver_get_cpu1_wifi_mac_int_misc_st();

//0x2b
uint32_t system_driver_get_cpu1_wifi_mac_int_rx_trigger_st();
uint32_t system_driver_get_cpu1_wifi_mac_int_tx_trigger_st();
uint32_t system_driver_get_cpu1_wifi_mac_port_trigger_st();
uint32_t system_driver_get_cpu1_wifi_mac_int_gen_st();
uint32_t system_driver_get_cpu1_wifi_hsu_irq_st();
uint32_t system_driver_get_cpu1_wifi_int_mac_wakeup_st();
uint32_t system_driver_get_cpu1_dm_irq_st();
uint32_t system_driver_get_cpu1_ble_irq_st();
uint32_t system_driver_get_cpu1_bt_irq_st();
uint32_t system_driver_get_cpu1_mbox0_int_st();
uint32_t system_driver_get_cpu1_mbox1_int_st();
uint32_t system_driver_get_cpu1_bmc64_int_st();
uint32_t system_driver_get_cpu1_touched_int_st();
uint32_t system_driver_get_cpu1_usbplug_int_st();
uint32_t system_driver_get_cpu1_rtc_int_st();
uint32_t system_driver_get_cpu1_gpio_int_st();

//REG_0x30 0x31 0x32 0x33 0x34 0x35
void system_driver_set_gpio0_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio0_perial_mode(uint32_t group);
void system_driver_set_gpio1_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio1_perial_mode(uint32_t group);
void system_driver_set_gpio2_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio2_perial_mode(uint32_t group);
void system_driver_set_gpio3_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio3_perial_mode(uint32_t group);
void system_driver_set_gpio4_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio4_perial_mode(uint32_t group);
void system_driver_set_gpio5_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio5_perial_mode(uint32_t group);
void system_driver_set_gpio6_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio6_perial_mode(uint32_t group);
void system_driver_set_gpio7_perial_mode(uint32_t group, bool value);
uint32_t system_driver_get_gpio7_perial_mode(uint32_t group);

//0x38
void system_driver_set_dbug_cfg0(uint32_t group, bool value);
uint32_t system_driver_get_dbug_cfg0(uint32_t group);

//0x39
void system_driver_set_dbug_cfg1(uint32_t group, bool value);
uint32_t system_driver_get_dbug_cfg1(uint32_t group);

#ifdef __cplusplus
}
#endif

