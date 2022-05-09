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
#include "system_ll.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	system_hw_t *hw;
} system_hal_t;

void system_hal_init();

//0x0
#define system_hal_get_device_id(hal)                                  system_ll_get_device_id((hal)->hw)

//0x01
#define system_hal_get_version_id(hal)                                 system_ll_get_version_id((hal)->hw)

//0x02
#define system_hal_get_core0_halted_st(hal)                            system_ll_get_core0_halted_st((hal)->hw)
#define system_hal_get_core1_halted_st(hal)                            system_ll_get_core1_halted_st((hal)->hw)
#define system_hal_get_cpu0_sw_reset_st(hal)                           system_ll_get_cpu0_sw_reset_st((hal)->hw)
#define system_hal_get_cpu1_sw_reset_st(hal)                           system_ll_get_cpu1_sw_reset_st((hal)->hw)
#define system_hal_get_cpu0_pwr_dw_st(hal)                             system_ll_get_cpu0_pwr_dw_st((hal)->hw)
#define system_hal_get_cpu1_pwr_dw_st(hal)                             system_ll_get_cpu1_pwr_dw_st((hal)->hw)

//0x03
#define system_hal_set_boot_rom_mode(hal)                              system_ll_set_boot_rom_mode((hal)->hw)
#define system_hal_set_boot_flash_mode(hal)                            system_ll_set_boot_flash_mode((hal)->hw)
#define system_hal_get_boot_mode(hal)                                  system_ll_get_boot_mode((hal)->hw)

#define system_hal_set_jtag_connect_core0(hal)                         system_ll_set_jtag_connect_core0((hal)->hw)
#define system_hal_set_jtag_connect_core1(hal)                         system_ll_set_jtag_connect_core1((hal)->hw)
#define system_hal_get_jtag_connect_core(hal)                          system_ll_get_jtag_connect_core((hal)->hw)

#define system_hal_set_flash_select_normal_op(hal)                     system_ll_set_flash_select_normal_op((hal)->hw)
#define system_hal_set_flash_select_spi_op(hal)                        system_ll_set_flash_select_spi_op((hal)->hw)
#define system_hal_get_flash_select_op(hal)                            system_ll_get_flash_select_op((hal)->hw)

//0x04
#define system_hal_set_cpu0_int_mask(hal, value)                       system_ll_set_cpu0_int_mask((hal)->hw, value)
#define system_hal_get_cpu0_int_mask(hal)                              system_ll_get_cpu0_int_mask((hal)->hw)

#define system_hal_set_core0_halt_indicate(hal, value)                 system_ll_set_core0_halt_indicate((hal)->hw, value)
#define system_hal_get_core0_halt_indicate(hal)                        system_ll_get_core0_halt_indicate((hal)->hw)

#define system_hal_set_cpu0_clk_div(hal, value)                        system_ll_set_cpu0_clk_div((hal)->hw, value)
#define system_hal_get_cpu0_clk_div(hal)                               system_ll_get_cpu0_clk_div((hal)->hw)


//0x05
#define system_hal_set_cpu1_int_mask(hal, value)                       system_ll_set_cpu1_int_mask((hal)->hw, value)
#define system_hal_get_cpu1_int_mask(hal)                              system_ll_get_cpu1_int_mask((hal)->hw)

#define system_hal_set_core1_halt_indicate(hal, value)                 system_ll_set_core1_halt_indicate((hal)->hw, value)
#define system_hal_get_core1_halt_indicate(hal)                        system_ll_get_core1_halt_indicate((hal)->hw)

#define system_hal_set_cpu1_clk_div(hal, value)                        system_ll_set_cpu1_clk_div((hal)->hw, value)
#define system_hal_get_cpu1_clk_div(hal)                               system_ll_get_cpu1_clk_div((hal)->hw)


//0x08
#define system_hal_set_core_clk_div(hal, value)                        system_ll_set_core_clk_div((hal)->hw, value)
#define system_hal_get_core_clk_div(hal)                               system_ll_get_core_clk_div((hal)->hw)

#define system_hal_set_core_clk_sel(hal, value)                        system_ll_set_core_clk_sel((hal)->hw, value)
#define system_hal_get_core_clk_sel(hal)                               system_ll_get_core_clk_sel((hal)->hw)

#define system_hal_set_bus_clk_div(hal, value)                         system_ll_set_bus_clk_div((hal)->hw, value)
#define system_hal_get_bus_clk_div(hal)                                system_ll_get_bus_clk_div((hal)->hw)

#define system_hal_set_uart0_clk_div(hal, value)                       system_ll_set_uart0_clk_div((hal)->hw, value)
#define system_hal_get_uart0_clk_div(hal)                              system_ll_get_uart0_clk_div((hal)->hw)
#define system_hal_set_uart0_clk_sel(hal, value)                       system_ll_set_uart0_clk_sel((hal)->hw, value)
#define system_hal_get_uart0_clk_sel(hal)                              system_ll_get_uart0_clk_sel((hal)->hw)

#define system_hal_set_uart1_clk_div(hal, value)                       system_ll_set_uart1_clk_div((hal)->hw, value)
#define system_hal_get_uart1_clk_div(hal)                              system_ll_get_uart1_clk_div((hal)->hw)
#define system_hal_set_uart1_clk_sel(hal, value)                       system_ll_set_uart1_clk_sel((hal)->hw, value)
#define system_hal_get_uart1_clk_sel(hal)                              system_ll_get_uart1_clk_sel((hal)->hw)

#define system_hal_set_uart2_clk_div(hal, value)                       system_ll_set_uart2_clk_div((hal)->hw, value)
#define system_hal_get_uart2_clk_div(hal)                              system_ll_get_uart2_clk_div((hal)->hw)
#define system_hal_set_uart2_clk_sel(hal, value)                       system_ll_set_uart2_clk_sel((hal)->hw, value)
#define system_hal_get_uart2_clk_sel(hal)                              system_ll_get_uart2_clk_sel((hal)->hw)

#define system_hal_set_sadc_clk_sel(hal, value)                        system_ll_set_sadc_clk_sel((hal)->hw, value)
#define system_hal_get_sadc_clk_sel(hal)                               system_ll_get_sadc_clk_sel((hal)->hw)

#define system_hal_set_pwm0_clk_sel(hal, value)                        system_ll_set_pwm0_clk_sel((hal)->hw, value)
#define system_hal_get_pwm0_clk_sel(hal)                               system_ll_get_pwm0_clk_sel((hal)->hw)

#define system_hal_set_pwm1_clk_sel(hal, value)                        system_ll_set_pwm1_clk_sel((hal)->hw, value)
#define system_hal_get_pwm1_clk_sel(hal)                               system_ll_get_pwm1_clk_sel((hal)->hw)

#define system_hal_set_timer0_clk_sel(hal, value)                      system_ll_set_timer0_clk_sel((hal)->hw, value)
#define system_hal_get_timer0_clk_sel(hal)                             system_ll_get_timer0_clk_sel((hal)->hw)

#define system_hal_set_timer1_clk_sel(hal, value)                      system_ll_set_timer1_clk_sel((hal)->hw, value)
#define system_hal_get_timer1_clk_sel(hal)                             system_ll_get_timer1_clk_sel((hal)->hw)

#define system_hal_set_timer2_clk_sel(hal, value)                      system_ll_set_timer2_clk_sel((hal)->hw, value)
#define system_hal_get_timer2_clk_sel(hal)                             system_ll_get_timer2_clk_sel((hal)->hw)

#define system_hal_set_i2s_clk_sel(hal, value)                         system_ll_set_i2s_clk_sel((hal)->hw, value)
#define system_hal_get_i2s_clk_sel(hal)                                system_ll_get_i2s_clk_sel((hal)->hw)

#define system_hal_set_aud_clk_sel(hal, value)                         system_ll_set_aud_clk_sel((hal)->hw, value)
#define system_hal_get_aud_clk_sel(hal)                                system_ll_get_aud_clk_sel((hal)->hw)

#define system_hal_set_jpeg_clk_div(hal, value)                        system_ll_set_jpeg_clk_div((hal)->hw, value)
#define system_hal_get_jpeg_clk_div(hal)                               system_ll_get_jpeg_clk_div((hal)->hw)
#define system_hal_set_jpeg_clk_sel(hal, value)                        system_ll_set_jpeg_clk_sel((hal)->hw, value)
#define system_hal_get_jpeg_clk_sel(hal)                               system_ll_get_jpeg_clk_sel((hal)->hw)


#define system_hal_set_disp_l_clk_div(hal, value)                      system_ll_set_disp_l_clk_div((hal)->hw, value)
#define system_hal_get_disp_l_clk_div(hal)                             system_ll_get_disp_l_clk_div((hal)->hw)

//0x09
#define system_hal_set_disp_h_clk_div(hal, value)                      system_ll_set_disp_h_clk_div((hal)->hw, value)
#define system_hal_get_disp_h_clk_div(hal)                             system_ll_get_disp_h_clk_div((hal)->hw)
#define system_hal_set_disp_clk_sel(hal, value)                        system_ll_set_disp_clk_sel((hal)->hw, value)
#define system_hal_get_disp_clk_sel(hal)                               system_ll_get_disp_clk_sel((hal)->hw)


#define system_hal_set_psram_clk_div(hal, value)                       system_ll_set_psram_clk_div((hal)->hw, value)
#define system_hal_get_psram_clk_div(hal)                              system_ll_get_psram_clk_div((hal)->hw)
#define system_hal_set_psram_clk_sel(hal, value)                       system_ll_set_psram_clk_sel((hal)->hw, value)
#define system_hal_get_psram_clk_sel(hal)                              system_ll_get_psram_clk_sel((hal)->hw)

#define system_hal_set_qspi0_clk_div(hal, value)                       system_ll_set_qspi0_clk_div((hal)->hw, value)
#define system_hal_get_qspi0_clk_div(hal)                              system_ll_get_qspi0_clk_div((hal)->hw)

#define system_hal_set_sdio_clk_div(hal, value)                        system_ll_set_sdio_clk_div((hal)->hw, value)
#define system_hal_get_sdio_clk_div(hal)                               system_ll_get_sdio_clk_div((hal)->hw)
#define system_hal_set_sdio_clk_sel(hal, value)                        system_ll_set_sdio_clk_sel((hal)->hw, value)
#define system_hal_get_sdio_clk_sel(hal)                               system_ll_get_sdio_clk_sel((hal)->hw)

#define system_hal_set_auxs_clk_div(hal, value)                        system_ll_set_auxs_clk_div((hal)->hw, value)
#define system_hal_get_auxs_clk_div(hal)                               system_ll_get_auxs_clk_div((hal)->hw)
#define system_hal_set_auxs_clk_sel(hal, value)                        system_ll_set_auxs_clk_sel((hal)->hw, value)
#define system_hal_get_auxs_clk_sel(hal)                               system_ll_get_auxs_clk_sel((hal)->hw)

#define system_hal_set_flash_clk_sel(hal, value)                       system_ll_set_flash_clk_sel((hal)->hw, value)
#define system_hal_get_flash_clk_sel(hal)                              system_ll_get_flash_clk_sel((hal)->hw)
#define system_hal_set_flash_clk_div(hal, value)                       system_ll_set_flash_clk_div((hal)->hw, value)
#define system_hal_get_flash_clk_div(hal)                              system_ll_get_flash_clk_div((hal)->hw)

#define system_hal_set_i2s0_clk_div(hal, value)                        system_ll_set_i2s0_clk_div((hal)->hw, value)
#define system_hal_get_i2s0_clk_div(hal)                               system_ll_get_i2s0_clk_div((hal)->hw)

//0x0a
#define system_hal_set_26m_clk_div(hal, value)                         system_ll_set_26m_clk_div((hal)->hw, value)
#define system_hal_get_26m_clk_div(hal)                                system_ll_get_26m_clk_div((hal)->hw)
#define system_hal_set_wdt_clk_div(hal, value)                         system_ll_set_wdt_clk_div((hal)->hw, value)
#define system_hal_get_wdt_clk_div(hal)                                system_ll_get_wdt_clk_div((hal)->hw)

//0x0b
#define system_hal_set_anaspi_freq(hal, value)                         system_ll_set_anaspi_freq((hal)->hw, value)
#define system_hal_get_anaspi_freq(hal)                                system_ll_get_anaspi_freq((hal)->hw)

//0x0c
#define system_hal_set_i2c0_clk_en(hal, value)                         system_ll_set_i2c0_clk_en((hal)->hw, value)
#define system_hal_get_i2c0_clk_en(hal)                                system_ll_get_i2c0_clk_en((hal)->hw)

#define system_hal_set_spi0_clk_en(hal, value)                         system_ll_set_spi0_clk_en((hal)->hw, value)
#define system_hal_get_spi0_clk_en(hal)                                system_ll_get_spi0_clk_en((hal)->hw)

#define system_hal_set_uart0_clk_en(hal, value)                        system_ll_set_uart0_clk_en((hal)->hw, value)
#define system_hal_get_uart0_clk_en(hal)                               system_ll_get_uart0_clk_en((hal)->hw)

#define system_hal_set_pwm0_clk_en(hal, value)                         system_ll_set_pwm0_clk_en((hal)->hw, value)
#define system_hal_get_pwm0_clk_en(hal)                                system_ll_get_pwm0_clk_en((hal)->hw)

#define system_hal_set_timer0_clk_en(hal, value)                       system_ll_set_timer0_clk_en((hal)->hw, value)
#define system_hal_get_timer0_clk_en(hal)                              system_ll_get_timer0_clk_en((hal)->hw)

#define system_hal_set_sadc_clk_en(hal, value)                         system_ll_set_sadc_clk_en((hal)->hw, value)
#define system_hal_get_sadc_clk_en(hal)                                system_ll_get_sadc_clk_en((hal)->hw)

#define system_hal_set_irda_clk_en(hal, value)                         system_ll_set_irda_clk_en((hal)->hw, value)
#define system_hal_get_irda_clk_en(hal)                                system_ll_get_irda_clk_en((hal)->hw)

#define system_hal_set_efuse_clk_en(hal, value)                        system_ll_set_efuse_clk_en((hal)->hw, value)
#define system_hal_get_efuse_clk_en(hal)                               system_ll_get_efuse_clk_en((hal)->hw)

#define system_hal_set_i2c1_clk_en(hal, value)                         system_ll_set_i2c1_clk_en((hal)->hw, value)
#define system_hal_get_i2c1_clk_en(hal)                                system_ll_get_i2c1_clk_en((hal)->hw)

#define system_hal_set_spi1_clk_en(hal, value)                         system_ll_set_spi1_clk_en((hal)->hw, value)
#define system_hal_get_spi1_clk_en(hal)                                system_ll_get_spi1_clk_en((hal)->hw)

#define system_hal_set_uart1_clk_en(hal, value)                        system_ll_set_uart1_clk_en((hal)->hw, value)
#define system_hal_get_uart1_clk_en(hal)                               system_ll_get_uart1_clk_en((hal)->hw)

#define system_hal_set_uart2_clk_en(hal, value)                        system_ll_set_uart2_clk_en((hal)->hw, value)
#define system_hal_get_uart2_clk_en(hal)                               system_ll_get_uart2_clk_en((hal)->hw)

#define system_hal_set_pwm1_clk_en(hal, value)                         system_ll_set_pwm1_clk_en((hal)->hw, value)
#define system_hal_get_pwm1_clk_en(hal)                                system_ll_get_pwm1_clk_en((hal)->hw)

#define system_hal_set_timer1_clk_en(hal, value)                       system_ll_set_timer1_clk_en((hal)->hw, value)
#define system_hal_get_timer1_clk_en(hal)                              system_ll_get_timer1_clk_en((hal)->hw)

#define system_hal_set_timer2_clk_en(hal, value)                       system_ll_set_timer2_clk_en((hal)->hw, value)
#define system_hal_get_timer2_clk_en(hal)                              system_ll_get_timer2_clk_en((hal)->hw)

#define system_hal_set_otp_clk_en(hal, value)                          system_ll_set_otp_clk_en((hal)->hw, value)
#define system_hal_get_otp_clk_en(hal)                                 system_ll_get_otp_clk_en((hal)->hw)

#define system_hal_set_i2s_clk_en(hal, value)                          system_ll_set_i2s_clk_en((hal)->hw, value)
#define system_hal_get_i2s_clk_en(hal)                                 system_ll_get_i2s_clk_en((hal)->hw)

#define system_hal_set_usb_clk_en(hal, value)                          system_ll_set_usb_clk_en((hal)->hw, value)
#define system_hal_get_usb_clk_en(hal)                                 system_ll_get_usb_clk_en((hal)->hw)

#define system_hal_set_can_clk_en(hal, value)                          system_ll_set_can_clk_en((hal)->hw, value)
#define system_hal_get_can_clk_en(hal)                                 system_ll_get_can_clk_en((hal)->hw)

#define system_hal_set_psram_clk_en(hal, value)                        system_ll_set_psram_clk_en((hal)->hw, value)
#define system_hal_get_psram_clk_en(hal)                               system_ll_get_psram_clk_en((hal)->hw)

#define system_hal_set_qspi0_clk_en(hal, value)                        system_ll_set_qspi0_clk_en((hal)->hw, value)
#define system_hal_get_qspi0_clk_en(hal)                               system_ll_get_qspi0_clk_en((hal)->hw)

#define system_hal_set_qspi1_clk_en(hal, value)                        system_ll_set_qspi1_clk_en((hal)->hw, value)
#define system_hal_get_qspi1_clk_en(hal)                               system_ll_get_qspi1_clk_en((hal)->hw)

#define system_hal_set_sdio_clk_en(hal, value)                         system_ll_set_sdio_clk_en((hal)->hw, value)
#define system_hal_get_sdio_clk_en(hal)                                system_ll_get_sdio_clk_en((hal)->hw)

#define system_hal_set_auxs_clk_en(hal, value)                         system_ll_set_auxs_clk_en((hal)->hw, value)
#define system_hal_get_auxs_clk_en(hal)                                system_ll_get_auxs_clk_en((hal)->hw)

#define system_hal_set_btdm_clk_en(hal, value)                         system_ll_set_btdm_clk_en((hal)->hw, value)
#define system_hal_get_btdm_clk_en(hal)                                system_ll_get_btdm_clk_en((hal)->hw)

#define system_hal_set_xvr_clk_en(hal, value)                          system_ll_set_xvr_clk_en((hal)->hw, value)
#define system_hal_get_xvr_clk_en(hal)                                 system_ll_get_xvr_clk_en((hal)->hw)

#define system_hal_set_mac_clk_en(hal, value)                          system_ll_set_mac_clk_en((hal)->hw, value)
#define system_hal_get_mac_clk_en(hal)                                 system_ll_get_mac_clk_en((hal)->hw)

#define system_hal_set_phy_clk_en(hal, value)                          system_ll_set_phy_clk_en((hal)->hw, value)
#define system_hal_get_phy_clk_en(hal)                                 system_ll_get_phy_clk_en((hal)->hw)

#define system_hal_set_jpeg_clk_en(hal, value)                         system_ll_set_jpeg_clk_en((hal)->hw, value)
#define system_hal_get_jpeg_clk_en(hal)                                system_ll_get_jpeg_clk_en((hal)->hw)

#define system_hal_set_disp_clk_en(hal, value)                         system_ll_set_disp_clk_en((hal)->hw, value)
#define system_hal_get_disp_clk_en(hal)                                system_ll_get_disp_clk_en((hal)->hw)

#define system_hal_set_aud_clk_en(hal, value)                          system_ll_set_aud_clk_en((hal)->hw, value)
#define system_hal_get_aud_clk_en(hal)                                 system_ll_get_aud_clk_en((hal)->hw)

#define system_hal_set_wdt_clk_en(hal, value)                          system_ll_set_wdt_clk_en((hal)->hw, value)
#define system_hal_get_wdt_clk_en(hal)                                 system_ll_get_wdt_clk_en((hal)->hw)


//0x0e
#define system_hal_set_aon_disckg(hal, value)                          system_ll_set_aon_disckg((hal)->hw, value)
#define system_hal_get_aon_disckg(hal)                                 system_ll_get_aon_disckg((hal)->hw)

#define system_hal_set_sys_disckg(hal, value)                          system_ll_set_sys_disckg((hal)->hw, value)
#define system_hal_get_sys_disckg(hal)                                 system_ll_get_sys_disckg((hal)->hw)

#define system_hal_set_dma_disckg(hal, value)                          system_ll_set_dma_disckg((hal)->hw, value)
#define system_hal_get_dma_disckg(hal)                                 system_ll_get_dma_disckg((hal)->hw)

#define system_hal_set_flash_disckg(hal, value)                        system_ll_set_flash_disckg((hal)->hw, value)
#define system_hal_get_flash_disckg(hal)                               system_ll_get_flash_disckg((hal)->hw)

#define system_hal_set_wdt_disckg(hal, value)                          system_ll_set_wdt_disckg((hal)->hw, value)
#define system_hal_get_wdt_disckg(hal)                                 system_ll_get_wdt_disckg((hal)->hw)

#define system_hal_set_timer0_disckg(hal, value)                       system_ll_set_timer0_disckg((hal)->hw, value)
#define system_hal_get_timer0_disckg(hal)                              system_ll_get_timer0_disckg((hal)->hw)

#define system_hal_set_uart0_disckg(hal, value)                        system_ll_set_uart0_disckg((hal)->hw, value)
#define system_hal_get_uart0_disckg(hal)                               system_ll_get_uart0_disckg((hal)->hw)

#define system_hal_set_pwm0_disckg(hal, value)                         system_ll_set_pwm0_disckg((hal)->hw, value)
#define system_hal_get_pwm0_disckg(hal)                                system_ll_get_pwm0_disckg((hal)->hw)

#define system_hal_set_i2c0_disckg(hal, value)                         system_ll_set_i2c0_disckg((hal)->hw, value)
#define system_hal_get_i2c0_disckg(hal)                                system_ll_get_i2c0_disckg((hal)->hw)

#define system_hal_set_spi0_disckg(hal, value)                         system_ll_set_spi0_disckg((hal)->hw, value)
#define system_hal_get_spi0_disckg(hal)                                system_ll_get_spi0_disckg((hal)->hw)

#define system_hal_set_sadc_disckg(hal, value)                         system_ll_set_sadc_disckg((hal)->hw, value)
#define system_hal_get_sadc_disckg(hal)                                system_ll_get_sadc_disckg((hal)->hw)

#define system_hal_set_efuse_disckg(hal, value)                        system_ll_set_efuse_disckg((hal)->hw, value)
#define system_hal_get_efuse_disckg(hal)                               system_ll_get_efuse_disckg((hal)->hw)

#define system_hal_set_irda_disckg(hal, value)                         system_ll_set_irda_disckg((hal)->hw, value)
#define system_hal_get_irda_disckg(hal)                                system_ll_get_irda_disckg((hal)->hw)

#define system_hal_set_trng_disckg(hal, value)                         system_ll_set_trng_disckg((hal)->hw, value)
#define system_hal_get_trng_disckg(hal)                                system_ll_get_trng_disckg((hal)->hw)

#define system_hal_set_sdio_disckg(hal, value)                         system_ll_set_sdio_disckg((hal)->hw, value)
#define system_hal_get_sdio_disckg(hal)                                system_ll_get_sdio_disckg((hal)->hw)

#define system_hal_set_la_disckg(hal, value)                           system_ll_set_la_disckg((hal)->hw, value)
#define system_hal_get_la_disckg(hal)                                  system_ll_get_la_disckg((hal)->hw)

#define system_hal_set_timer1_disckg(hal, value)                       system_ll_set_timer1_disckg((hal)->hw, value)
#define system_hal_get_timer1_disckg(hal)                              system_ll_get_timer1_disckg((hal)->hw)

#define system_hal_set_uart1_disckg(hal, value)                        system_ll_set_uart1_disckg((hal)->hw, value)
#define system_hal_get_uart1_disckg(hal)                               system_ll_get_uart1_disckg((hal)->hw)
#define system_hal_set_uart2_disckg(hal, value)                        system_ll_set_uart2_disckg((hal)->hw, value)
#define system_hal_get_uart2_disckg(hal)                               system_ll_get_uart2_disckg((hal)->hw)

#define system_hal_set_pwm1_disckg(hal, value)                         system_ll_set_pwm1_disckg((hal)->hw, value)
#define system_hal_get_pwm1_disckg(hal)                                system_ll_get_pwm1_disckg((hal)->hw)

#define system_hal_set_i2c1_disckg(hal, value)                         system_ll_set_i2c1_disckg((hal)->hw, value)
#define system_hal_get_i2c1_disckg(hal)                                system_ll_get_i2c1_disckg((hal)->hw)
#define system_hal_set_spi1_disckg(hal, value)                         system_ll_set_spi1_disckg((hal)->hw, value)
#define system_hal_get_spi1_disckg(hal)                                system_ll_get_spi1_disckg((hal)->hw)

#define system_hal_set_usb_disckg(hal, value)                          system_ll_set_usb_disckg((hal)->hw, value)
#define system_hal_get_usb_disckg(hal)                                 system_ll_get_usb_disckg((hal)->hw)
#define system_hal_set_can_disckg(hal, value)                          system_ll_set_can_disckg((hal)->hw, value)
#define system_hal_get_can_disckg(hal)                                 system_ll_get_can_disckg((hal)->hw)

#define system_hal_set_qspi0_disckg(hal, value)                        system_ll_set_qspi0_disckg((hal)->hw, value)
#define system_hal_get_qspi0_disckg(hal)                               system_ll_get_qspi0_disckg((hal)->hw)
#define system_hal_set_psram_disckg(hal, value)                        system_ll_set_psram_disckg((hal)->hw, value)
#define system_hal_get_psram_disckg(hal)                               system_ll_get_psram_disckg((hal)->hw)
#define system_hal_set_fft_disckg(hal, value)                          system_ll_set_fft_disckg((hal)->hw, value)
#define system_hal_get_fft_disckg(hal)                                 system_ll_get_fft_disckg((hal)->hw)
#define system_hal_set_sbc_disckg(hal, value)                          system_ll_set_sbc_disckg((hal)->hw, value)
#define system_hal_get_sbc_disckg(hal)                                 system_ll_get_sbc_disckg((hal)->hw)
#define system_hal_set_aud_disckg(hal, value)                          system_ll_set_aud_disckg((hal)->hw, value)
#define system_hal_get_aud_disckg(hal)                                 system_ll_get_aud_disckg((hal)->hw)
#define system_hal_set_i2s_disckg(hal, value)                          system_ll_set_i2s_disckg((hal)->hw, value)
#define system_hal_get_i2s_disckg(hal)                                 system_ll_get_i2s_disckg((hal)->hw)

#define system_hal_set_jpeg_disckg(hal, value)                         system_ll_set_jpeg_disckg((hal)->hw, value)
#define system_hal_get_jpeg_disckg(hal)                                system_ll_get_jpeg_disckg((hal)->hw)
#define system_hal_set_jpeg_dec_disckg(hal, value)                     system_ll_set_jpeg_dec_disckg((hal)->hw, value)
#define system_hal_get_jpeg_dec_disckg(hal)                            system_ll_get_jpeg_dec_disckg((hal)->hw)

//0xff
#define system_hal_set_disp_disckg(hal, value)                         system_ll_set_disp_disckg((hal)->hw, value)
#define system_hal_get_disp_disckg(hal)                                system_ll_get_disp_disckg((hal)->hw)

#define system_hal_set_dma2d_disckg(hal, value)                        system_ll_set_dma2d_disckg((hal)->hw, value)
#define system_hal_get_dma2d_disckg(hal)                               system_ll_get_dma2d_disckg((hal)->hw)

#define system_hal_set_btdm_disckg(hal, value)                         system_ll_set_btdm_disckg((hal)->hw, value)
#define system_hal_get_btdm_disckg(hal)                                system_ll_get_btdm_disckg((hal)->hw)

#define system_hal_set_xver_disckg(hal, value)                         system_ll_set_xver_disckg((hal)->hw, value)
#define system_hal_get_xver_disckg(hal)                                system_ll_get_xver_disckg((hal)->hw)

#define system_hal_set_btdm_bps_disckg(hal, value)                     system_ll_set_btdm_bps_disckg((hal)->hw, value)
#define system_hal_get_btdm_bps_disckg(hal)                            system_ll_get_btdm_bps_disckg((hal)->hw)

//0x10
#define system_hal_set_power_on_mem1(hal, value)                       system_ll_set_power_on_mem1((hal)->hw, value)
#define system_hal_get_power_on_mem1(hal)                              system_ll_get_power_on_mem1((hal)->hw)
#define system_hal_set_power_on_mem2(hal, value)                       system_ll_set_power_on_mem2((hal)->hw, value)
#define system_hal_get_power_on_mem2(hal)                              system_ll_get_power_on_mem2((hal)->hw)
#define system_hal_set_power_on_mem3(hal, value)                       system_ll_set_power_on_mem3((hal)->hw, value)
#define system_hal_get_power_on_mem3(hal)                              system_ll_get_power_on_mem3((hal)->hw)

#define system_hal_set_power_on_encp(hal, value)                       system_ll_set_power_on_encp((hal)->hw, value)
#define system_hal_get_power_on_encp(hal)                              system_ll_get_power_on_encp((hal)->hw)
#define system_hal_set_power_on_bakp(hal, value)                       system_ll_set_power_on_bakp((hal)->hw, value)
#define system_hal_get_power_on_bakp(hal)                              system_ll_get_power_on_bakp((hal)->hw)
#define system_hal_set_power_on_ahbp(hal, value)                       system_ll_set_power_on_ahbp((hal)->hw, value)
#define system_hal_get_power_on_ahbp(hal)                              system_ll_get_power_on_ahbp((hal)->hw)
#define system_hal_set_power_on_audp(hal, value)                       system_ll_set_power_on_audp((hal)->hw, value)
#define system_hal_get_power_on_audp(hal)                              system_ll_get_power_on_audp((hal)->hw)

#define system_hal_set_power_on_vidp(hal, value)                       system_ll_set_power_on_vidp((hal)->hw, value)
#define system_hal_get_power_on_vidp(hal)                              system_ll_get_power_on_vidp((hal)->hw)
#define system_hal_set_power_on_btsp(hal, value)                       system_ll_set_power_on_btsp((hal)->hw, value)
#define system_hal_get_power_on_btsp(hal)                              system_ll_get_power_on_btsp((hal)->hw)
#define system_hal_set_power_on_wifi_mac(hal, value)                   system_ll_set_power_on_wifi_mac((hal)->hw, value)
#define system_hal_get_power_on_wifi_mac(hal)                          system_ll_get_power_on_wifi_mac((hal)->hw)
#define system_hal_set_power_on_wifi_phy(hal, value)                   system_ll_set_power_on_wifi_phy((hal)->hw, value)
#define system_hal_get_power_on_wifi_phy(hal)                          system_ll_get_power_on_wifi_phy((hal)->hw)

#define system_hal_set_sleep_en_need_flash_idle(hal, value)            system_ll_set_sleep_en_need_flash_idle((hal)->hw, value)
#define system_hal_get_sleep_en_need_flash_idle(hal)                   system_ll_get_sleep_en_need_flash_idle((hal)->hw)
#define system_hal_set_sleep_en_need_cpu1_wifi(hal, value)             system_ll_set_sleep_en_need_cpu1_wifi((hal)->hw, value)
#define system_hal_get_sleep_en_need_cpu1_wifi(hal)                    system_ll_get_sleep_en_need_cpu1_wifi((hal)->hw)
#define system_hal_set_sleep_en_need_cpu0_wifi(hal, value)             system_ll_set_sleep_en_need_cpu0_wifi((hal)->hw, value)
#define system_hal_get_sleep_en_need_cpu0_wifi(hal)                    system_ll_get_sleep_en_need_cpu0_wifi((hal)->hw)
#define system_hal_set_sleep_en_need_global(hal, value)                system_ll_set_sleep_en_need_global((hal)->hw, value)
#define system_hal_get_sleep_en_need_global(hal)                       system_ll_get_sleep_en_need_global((hal)->hw)

#define system_hal_set_wifi_wakeup_platform_en(hal, value)             system_ll_set_wifi_wakeup_platform_en((hal)->hw, value)
#define system_hal_get_wifi_wakeup_platform_en(hal)                    system_ll_get_wifi_wakeup_platform_en((hal)->hw)
#define system_hal_set_bts_wakeup_platform_en(hal, value)              system_ll_set_bts_wakeup_platform_en((hal)->hw, value)
#define system_hal_get_bts_wakeup_platform_en(hal)                     system_ll_get_bts_wakeup_platform_en((hal)->hw)
#define system_hal_set_bts_sleep_exit_req(hal, value)                  system_ll_set_bts_sleep_exit_req((hal)->hw, value)
#define system_hal_get_bts_sleep_exit_req(hal)                         system_ll_get_bts_sleep_exit_req((hal)->hw)

//0x20
#define system_hal_set_cpu0_bmc32_int_en(hal, value)                   system_ll_set_cpu0_bmc32_int_en((hal)->hw, value)
#define system_hal_get_cpu0_bmc32_int_en(hal)                          system_ll_get_cpu0_bmc32_int_en((hal)->hw)

#define system_hal_set_cpu0_host_0_irq_en(hal, value)                  system_ll_set_cpu0_host_0_irq_en((hal)->hw, value)
#define system_hal_get_cpu0_host_0_irq_en(hal)                         system_ll_get_cpu0_host_0_irq_en((hal)->hw)

#define system_hal_set_cpu0_host_0_sec_irq_en(hal, value)              system_ll_set_cpu0_host_0_sec_irq_en((hal)->hw, value)
#define system_hal_get_cpu0_host_0_sec_irq_en(hal)                     system_ll_get_cpu0_host_0_sec_irq_en((hal)->hw)

#define system_hal_set_cpu0_timer_int_en(hal, value)                   system_ll_set_cpu0_timer_int_en((hal)->hw, value)
#define system_hal_get_cpu0_timer_int_en(hal)                          system_ll_get_cpu0_timer_int_en((hal)->hw)

#define system_hal_set_cpu0_uart_int_en(hal, value)                    system_ll_set_cpu0_uart_int_en((hal)->hw, value)
#define system_hal_get_cpu0_uart_int_en(hal)                           system_ll_get_cpu0_uart_int_en((hal)->hw)

#define system_hal_set_cpu0_pwm_int_en(hal, value)                     system_ll_set_cpu0_pwm_int_en((hal)->hw, value)
#define system_hal_get_cpu0_pwm_int_en(hal)                            system_ll_get_cpu0_pwm_int_en((hal)->hw)

#define system_hal_set_cpu0_i2c_int_en(hal, value)                     system_ll_set_cpu0_i2c_int_en((hal)->hw, value)
#define system_hal_get_cpu0_i2c_int_en(hal)                            system_ll_get_cpu0_i2c_int_en((hal)->hw)

#define system_hal_set_cpu0_spi_int_en(hal, value)                     system_ll_set_cpu0_spi_int_en((hal)->hw, value)
#define system_hal_get_cpu0_spi_int_en(hal)                            system_ll_get_cpu0_spi_int_en((hal)->hw)

#define system_hal_set_cpu0_sadc_int_en(hal, value)                    system_ll_set_cpu0_sadc_int_en((hal)->hw, value)
#define system_hal_get_cpu0_sadc_int_en(hal)                           system_ll_get_cpu0_sadc_int_en((hal)->hw)

#define system_hal_set_cpu0_irda_int_en(hal, value)                    system_ll_set_cpu0_irda_int_en((hal)->hw, value)
#define system_hal_get_cpu0_irda_int_en(hal)                           system_ll_get_cpu0_irda_int_en((hal)->hw)

#define system_hal_set_cpu0_sdio_int_en(hal, value)                    system_ll_set_cpu0_sdio_int_en((hal)->hw, value)
#define system_hal_get_cpu0_sdio_int_en(hal)                           system_ll_get_cpu0_sdio_int_en((hal)->hw)

#define system_hal_set_cpu0_gdma_int_en(hal, value)                    system_ll_set_cpu0_gdma_int_en((hal)->hw, value)
#define system_hal_get_cpu0_gdma_int_en(hal)                           system_ll_get_cpu0_gdma_int_en((hal)->hw)

#define system_hal_set_cpu0_la_int_en(hal, value)                      system_ll_set_cpu0_la_int_en((hal)->hw, value)
#define system_hal_get_cpu0_la_int_en(hal)                             system_ll_get_cpu0_la_int_en((hal)->hw)

#define system_hal_set_cpu0_timer1_int_en(hal, value)                  system_ll_set_cpu0_timer1_int_en((hal)->hw, value)
#define system_hal_get_cpu0_timer1_int_en(hal)                         system_ll_get_cpu0_timer1_int_en((hal)->hw)

#define system_hal_set_cpu0_i2c1_int_en(hal, value)                    system_ll_set_cpu0_i2c1_int_en((hal)->hw, value)
#define system_hal_get_cpu0_i2c1_int_en(hal)                           system_ll_get_cpu0_i2c1_int_en((hal)->hw)

#define system_hal_set_cpu0_uart1_int_en(hal, value)                   system_ll_set_cpu0_uart1_int_en((hal)->hw, value)
#define system_hal_get_cpu0_uart1_int_en(hal)                          system_ll_get_cpu0_uart1_int_en((hal)->hw)

#define system_hal_set_cpu0_uart2_int_en(hal, value)                   system_ll_set_cpu0_uart2_int_en((hal)->hw, value)
#define system_hal_get_cpu0_uart2_int_en(hal)                          system_ll_get_cpu0_uart2_int_en((hal)->hw)

#define system_hal_set_cpu0_spi1_int_en(hal, value)                    system_ll_set_cpu0_spi1_int_en((hal)->hw, value)
#define system_hal_get_cpu0_spi1_int_en(hal)                           system_ll_get_cpu0_spi1_int_en((hal)->hw)

#define system_hal_set_cpu0_can_int_en(hal, value)                     system_ll_set_cpu0_can_int_en((hal)->hw, value)
#define system_hal_get_cpu0_can_int_en(hal)                            system_ll_get_cpu0_can_int_en((hal)->hw)

#define system_hal_set_cpu0_usb_int_en(hal, value)                     system_ll_set_cpu0_usb_int_en((hal)->hw, value)
#define system_hal_get_cpu0_usb_int_en(hal)                            system_ll_get_cpu0_usb_int_en((hal)->hw)

#define system_hal_set_cpu0_qspi_int_en(hal, value)                    system_ll_set_cpu0_qspi_int_en((hal)->hw, value)
#define system_hal_cpu0_qspi_int_en(hal)                               system_ll_cpu0_qspi_int_en((hal)->hw)

#define system_hal_set_cpu0_fft_int_en(hal, value)                     system_ll_set_cpu0_fft_int_en((hal)->hw, value)
#define system_hal_get_cpu0_fft_int_en(hal)                            system_ll_get_cpu0_fft_int_en((hal)->hw)

#define system_hal_set_cpu0_sbc_int_en(hal, value)                     system_ll_set_cpu0_sbc_int_en((hal)->hw, value)
#define system_hal_get_cpu0_sbc_int_en(hal)                            system_ll_get_cpu0_sbc_int_en((hal)->hw)

#define system_hal_set_cpu0_aud_int_en(hal, value)                     system_ll_set_cpu0_aud_int_en((hal)->hw, value)
#define system_hal_get_cpu0_aud_int_en(hal)                            system_ll_get_cpu0_aud_int_en((hal)->hw)

#define system_hal_set_cpu0_i2s_int_en(hal, value)                     system_ll_set_cpu0_i2s_int_en((hal)->hw, value)
#define system_hal_get_cpu0_i2s_int_en(hal)                            system_ll_get_cpu0_i2s_int_en((hal)->hw)

#define system_hal_set_cpu0_jpegenc_int_en(hal, value)                 system_ll_set_cpu0_jpegenc_int_en((hal)->hw, value)
#define system_hal_get_cpu0_jpegenc_int_en(hal)                        system_ll_get_cpu0_jpegenc_int_en((hal)->hw)

#define system_hal_set_cpu0_jpegdec_int_en(hal, value)                 system_ll_set_cpu0_jpegdec_int_en((hal)->hw, value)
#define system_hal_get_cpu0_jpegdec_int_en(hal)                        system_ll_get_cpu0_jpegdec_int_en((hal)->hw)

#define system_hal_set_cpu0_lcd_int_en(hal, value)                     system_ll_set_cpu0_lcd_int_en((hal)->hw, value)
#define system_hal_get_cpu0_lcd_int_en(hal)                            system_ll_get_cpu0_lcd_int_en((hal)->hw)

#define system_hal_set_cpu0_wifi_int_phy_en(hal, value)                system_ll_set_cpu0_wifi_int_phy_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_int_phy_en(hal)                       system_ll_get_cpu0_wifi_int_phy_en((hal)->hw)

#define system_hal_set_cpu0_wifi_mac_int_timer_en(hal, value)          system_ll_set_cpu0_wifi_mac_int_timer_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_mac_int_timer_en(hal)                 system_ll_get_cpu0_wifi_mac_int_timer_en((hal)->hw)

#define system_hal_set_cpu0_wifi_mac_int_misc_en(hal, value)           system_ll_set_cpu0_wifi_mac_int_misc_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_mac_int_misc_en(hal)                  system_ll_get_cpu0_wifi_mac_int_misc_en((hal)->hw)

//0x21
#define system_hal_set_cpu0_wifi_mac_int_rx_trigger_en(hal, value)     system_ll_set_cpu0_wifi_mac_int_rx_trigger_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_mac_int_rx_trigger_en(hal)            system_ll_get_cpu0_wifi_mac_int_rx_trigger_en((hal)->hw)

#define system_hal_set_cpu0_wifi_mac_int_tx_trigger_en(hal, value)     system_ll_set_cpu0_wifi_mac_int_tx_trigger_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_mac_int_tx_trigger_en(hal)            system_ll_get_cpu0_wifi_mac_int_tx_trigger_en((hal)->hw)

#define system_hal_set_cpu0_wifi_mac_port_trigger_en(hal, value)       system_ll_set_cpu0_wifi_mac_port_trigger_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_mac_port_trigger_en(hal)              system_ll_get_cpu0_wifi_mac_port_trigger_en((hal)->hw)

#define system_hal_set_cpu0_wifi_mac_int_gen_en(hal, value)            system_ll_set_cpu0_wifi_mac_int_gen_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_mac_int_gen_en(hal)                   system_ll_get_cpu0_wifi_mac_int_gen_en((hal)->hw)

#define system_hal_set_cpu0_wifi_hsu_irq_en(hal, value)                system_ll_set_cpu0_wifi_hsu_irq_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_hsu_irq_en(hal)                       system_ll_get_cpu0_wifi_hsu_irq_en((hal)->hw)

#define system_hal_set_cpu0_wifi_int_mac_wakeup_en(hal, value)         system_ll_set_cpu0_wifi_int_mac_wakeup_en((hal)->hw, value)
#define system_hal_get_cpu0_wifi_int_mac_wakeup_en(hal)                system_ll_get_cpu0_wifi_int_mac_wakeup_en((hal)->hw)

#define system_hal_set_cpu0_dm_irq_en(hal, value)                      system_ll_set_cpu0_dm_irq_en((hal)->hw, value)
#define system_hal_get_cpu0_dm_irq_en(hal)                             system_ll_get_cpu0_dm_irq_en((hal)->hw)

#define system_hal_set_cpu0_ble_irq_en(hal, value)                     system_ll_set_cpu0_ble_irq_en((hal)->hw, value)
#define system_hal_get_cpu0_ble_irq_en(hal)                            system_ll_get_cpu0_ble_irq_en((hal)->hw)

#define system_hal_set_cpu0_bt_irq_en(hal, value)                      system_ll_set_cpu0_bt_irq_en((hal)->hw, value)
#define system_hal_get_cpu0_bt_irq_en(hal)                             system_ll_get_cpu0_bt_irq_en((hal)->hw)

#define system_hal_set_cpu0_mbox0_int_en(hal, value)                   system_ll_set_cpu0_mbox0_int_en((hal)->hw, value)
#define system_hal_get_cpu0_mbox0_int_en(hal)                          system_ll_get_cpu0_mbox0_int_en((hal)->hw)

#define system_hal_set_cpu0_mbox1_int_en(hal, value)                   system_ll_set_cpu0_mbox1_int_en((hal)->hw, value)
#define system_hal_get_cpu0_mbox1_int_en(hal)                          system_ll_get_cpu0_mbox1_int_en((hal)->hw)

#define system_hal_set_cpu0_bmc64_int_en(hal, value)                   system_ll_set_cpu0_bmc64_int_en((hal)->hw, value)
#define system_hal_get_cpu0_bmc64_int_en(hal)                          system_ll_get_cpu0_bmc64_int_en((hal)->hw)

#define system_hal_set_cpu0_touched_int_en(hal, value)                 system_ll_set_cpu0_touched_int_en((hal)->hw, value)
#define system_hal_get_cpu0_touched_int_en(hal)                        system_ll_get_cpu0_touched_int_en((hal)->hw)

#define system_hal_set_cpu0_usbplug_int_en(hal, value)                 system_ll_set_cpu0_usbplug_int_en((hal)->hw, value)
#define system_hal_get_cpu0_usbplug_int_en(hal)                        system_ll_get_cpu0_usbplug_int_en((hal)->hw)

#define system_hal_set_cpu0_rtc_int_en(hal, value)                     system_ll_set_cpu0_rtc_int_en((hal)->hw, value)
#define system_hal_get_cpu0_rtc_int_en(hal)                            system_ll_get_cpu0_rtc_int_en((hal)->hw)
#define system_hal_set_cpu0_gpio_int_en(hal, value)                    system_ll_set_cpu0_gpio_int_en((hal)->hw, value)
#define system_hal_get_cpu0_gpio_int_en(hal)                           system_ll_get_cpu0_gpio_int_en((hal)->hw)

//0x22
#define system_hal_set_cpu1_bmc32_int_en(hal, value)                   system_ll_set_cpu1_bmc32_int_en((hal)->hw, value)
#define system_hal_get_cpu1_bmc32_int_en(hal)                          system_ll_get_cpu1_bmc32_int_en((hal)->hw)
#define system_hal_set_cpu1_host_0_irq_en(hal, value)                  system_ll_set_cpu1_host_0_irq_en((hal)->hw, value)
#define system_hal_get_cpu1_host_0_irq_en(hal)                         system_ll_get_cpu1_host_0_irq_en((hal)->hw)
#define system_hal_set_cpu1_host_0_sec_irq_en(hal, value)              system_ll_set_cpu1_host_0_sec_irq_en((hal)->hw, value)
#define system_hal_get_cpu1_host_0_sec_irq_en(hal)                     system_ll_get_cpu1_host_0_sec_irq_en((hal)->hw)
#define system_hal_set_cpu1_timer_int_en(hal, value)                   system_ll_set_cpu1_timer_int_en((hal)->hw, value)
#define system_hal_get_cpu1_timer_int_en(hal)                          system_ll_get_cpu1_timer_int_en((hal)->hw)
#define system_hal_set_cpu1_uart_int_en(hal, value)                    system_ll_set_cpu1_uart_int_en((hal)->hw, value)
#define system_hal_get_cpu1_uart_int_en(hal)                           system_ll_get_cpu1_uart_int_en((hal)->hw)
#define system_hal_set_cpu1_pwm_int_en(hal, value)                     system_ll_set_cpu1_pwm_int_en((hal)->hw, value)
#define system_hal_get_cpu1_pwm_int_en(hal)                            system_ll_get_cpu1_pwm_int_en((hal)->hw)
#define system_hal_set_cpu1_i2c_int_en(hal, value)                     system_ll_set_cpu1_i2c_int_en((hal)->hw, value)
#define system_hal_get_cpu1_i2c_int_en(hal)                            system_ll_get_cpu1_i2c_int_en((hal)->hw)
#define system_hal_set_cpu1_spi_int_en(hal, value)                     system_ll_set_cpu1_spi_int_en((hal)->hw, value)
#define system_hal_get_cpu1_spi_int_en(hal)                            system_ll_get_cpu1_spi_int_en((hal)->hw)
#define system_hal_set_cpu1_sadc_int_en(hal, value)                    system_ll_set_cpu1_sadc_int_en((hal)->hw, value)
#define system_hal_get_cpu1_sadc_int_en(hal)                           system_ll_get_cpu1_sadc_int_en((hal)->hw)
#define system_hal_set_cpu1_irda_int_en(hal, value)                    system_ll_set_cpu1_irda_int_en((hal)->hw, value)
#define system_hal_get_cpu1_irda_int_en(hal)                           system_ll_get_cpu1_irda_int_en((hal)->hw)
#define system_hal_set_cpu1_sdio_int_en(hal, value)                    system_ll_set_cpu1_sdio_int_en((hal)->hw, value)
#define system_hal_get_cpu1_sdio_int_en(hal)                           system_ll_get_cpu1_sdio_int_en((hal)->hw)
#define system_hal_set_cpu1_gdma_int_en(hal, value)                    system_ll_set_cpu1_gdma_int_en((hal)->hw, value)
#define system_hal_get_cpu1_gdma_int_en(hal)                           system_ll_get_cpu1_gdma_int_en((hal)->hw)
#define system_hal_set_cpu1_la_int_en(hal, value)                      system_ll_set_cpu1_la_int_en((hal)->hw, value)
#define system_hal_get_cpu1_la_int_en(hal)                             system_ll_get_cpu1_la_int_en((hal)->hw)
#define system_hal_set_cpu1_timer1_int_en(hal, value)                  system_ll_set_cpu1_timer1_int_en((hal)->hw, value)
#define system_hal_get_cpu1_timer1_int_en(hal)                         system_ll_get_cpu1_timer1_int_en((hal)->hw)
#define system_hal_set_cpu1_i2c1_int_en(hal, value)                    system_ll_set_cpu1_i2c1_int_en((hal)->hw, value)
#define system_hal_get_cpu1_i2c1_int_en(hal)                           system_ll_get_cpu1_i2c1_int_en((hal)->hw)
#define system_hal_set_cpu1_uart1_int_en(hal, value)                   system_ll_set_cpu1_uart1_int_en((hal)->hw, value)
#define system_hal_get_cpu1_uart1_int_en(hal)                          system_ll_get_cpu1_uart1_int_en((hal)->hw)
#define system_hal_set_cpu1_uart2_int_en(hal, value)                   system_ll_set_cpu1_uart2_int_en((hal)->hw, value)
#define system_hal_get_cpu1_uart2_int_en(hal)                          system_ll_get_cpu1_uart2_int_en((hal)->hw)
#define system_hal_set_cpu1_spi1_int_en(hal, value)                    system_ll_set_cpu1_spi1_int_en((hal)->hw, value)
#define system_hal_get_cpu1_spi1_int_en(hal)                           system_ll_get_cpu1_spi1_int_en((hal)->hw)
#define system_hal_set_cpu1_can_int_en(hal, value)                     system_ll_set_cpu1_can_int_en((hal)->hw, value)
#define system_hal_get_cpu1_can_int_en(hal)                            system_ll_get_cpu1_can_int_en((hal)->hw)
#define system_hal_set_cpu1_usb_int_en(hal, value)                     system_ll_set_cpu1_usb_int_en((hal)->hw, value)
#define system_hal_get_cpu1_usb_int_en(hal)                            system_ll_get_cpu1_usb_int_en((hal)->hw)
#define system_hal_set_cpu1_qspi_int_en(hal, value)                    system_ll_set_cpu1_qspi_int_en((hal)->hw, value)
#define system_hal_get_cpu1_qspi_int_en(hal)                           system_ll_get_cpu1_qspi_int_en((hal)->hw)
#define system_hal_set_cpu1_fft_int_en(hal, value)                     system_ll_set_cpu1_fft_int_en((hal)->hw, value)
#define system_hal_get_cpu1_fft_int_en(hal)                            system_ll_get_cpu1_fft_int_en((hal)->hw)
#define system_hal_set_cpu1_sbc_int_en(hal, value)                     system_ll_set_cpu1_sbc_int_en((hal)->hw, value)
#define system_hal_get_cpu1_sbc_int_en(hal)                            system_ll_get_cpu1_sbc_int_en((hal)->hw)
#define system_hal_set_cpu1_aud_int_en(hal, value)                     system_ll_set_cpu1_aud_int_en((hal)->hw, value)
#define system_hal_get_cpu1_aud_int_en(hal)                            system_ll_get_cpu1_aud_int_en((hal)->hw)
#define system_hal_set_cpu1_i2s_int_en(hal, value)                     system_ll_set_cpu1_i2s_int_en((hal)->hw, value)
#define system_hal_get_cpu1_i2s_int_en(hal)                            system_ll_get_cpu1_i2s_int_en((hal)->hw)
#define system_hal_set_cpu1_jpegenc_int_en(hal, value)                 system_ll_set_cpu1_jpegenc_int_en((hal)->hw, value)
#define system_hal_get_cpu1_jpegenc_int_en(hal)                        system_ll_get_cpu1_jpegenc_int_en((hal)->hw)
#define system_hal_set_cpu1_jpegdec_int_en(hal, value)                 system_ll_set_cpu1_jpegdec_int_en((hal)->hw, value)
#define system_hal_get_cpu1_jpegdec_int_en(hal)                        system_ll_get_cpu1_jpegdec_int_en((hal)->hw)
#define system_hal_set_cpu1_lcd_int_en(hal, value)                     system_ll_set_cpu1_lcd_int_en((hal)->hw, value)
#define system_hal_get_cpu1_lcd_int_en(hal)                            system_ll_get_cpu1_lcd_int_en((hal)->hw)
#define system_hal_set_cpu1_wifi_int_phy_en(hal, value)                system_ll_set_cpu1_wifi_int_phy_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_int_phy_en(hal)                       system_ll_get_cpu1_wifi_int_phy_en((hal)->hw)
#define system_hal_set_cpu1_wifi_mac_int_timer_en(hal, value)          system_ll_set_cpu1_wifi_mac_int_timer_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_mac_int_timer_en(hal)                 system_ll_get_cpu1_wifi_mac_int_timer_en((hal)->hw)
#define system_hal_set_cpu1_wifi_mac_int_misc_en(hal, value)           system_ll_set_cpu1_wifi_mac_int_misc_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_mac_int_misc_en(hal)                  system_ll_get_cpu1_wifi_mac_int_misc_en((hal)->hw)

//0x23
#define system_hal_set_cpu1_wifi_mac_int_rx_trigger_en(hal, value)     system_ll_set_cpu1_wifi_mac_int_rx_trigger_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_mac_int_rx_trigger_en(hal)            system_ll_get_cpu1_wifi_mac_int_rx_trigger_en((hal)->hw)
#define system_hal_set_cpu1_wifi_mac_int_tx_trigger_en(hal, value)     system_ll_set_cpu1_wifi_mac_int_tx_trigger_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_mac_int_tx_trigger_en(hal)            system_ll_get_cpu1_wifi_mac_int_tx_trigger_en((hal)->hw)
#define system_hal_set_cpu1_wifi_mac_port_trigger_en(hal, value)       system_ll_set_cpu1_wifi_mac_port_trigger_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_mac_port_trigger_en(hal)              system_ll_get_cpu1_wifi_mac_port_trigger_en((hal)->hw)
#define system_hal_set_cpu1_wifi_mac_int_gen_en(hal, value)            system_ll_set_cpu1_wifi_mac_int_gen_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_mac_int_gen_en(hal)                   system_ll_get_cpu1_wifi_mac_int_gen_en((hal)->hw)
#define system_hal_set_cpu1_wifi_hsu_irq_en(hal, value)                system_ll_set_cpu1_wifi_hsu_irq_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_hsu_irq_en(hal)                       system_ll_get_cpu1_wifi_hsu_irq_en((hal)->hw)
#define system_hal_set_cpu1_wifi_int_mac_wakeup_en(hal, value)         system_ll_set_cpu1_wifi_int_mac_wakeup_en((hal)->hw, value)
#define system_hal_get_cpu1_wifi_int_mac_wakeup_en(hal)                system_ll_get_cpu1_wifi_int_mac_wakeup_en((hal)->hw)
#define system_hal_set_cpu1_dm_irq_en(hal, value)                      system_ll_set_cpu1_dm_irq_en((hal)->hw, value)
#define system_hal_get_cpu1_dm_irq_en(hal)                             system_ll_get_cpu1_dm_irq_en((hal)->hw)
#define system_hal_set_cpu1_ble_irq_en(hal, value)                     system_ll_set_cpu1_ble_irq_en((hal)->hw, value)
#define system_hal_get_cpu1_ble_irq_en(hal)                            system_ll_get_cpu1_ble_irq_en((hal)->hw)
#define system_hal_set_cpu1_bt_irq_en(hal, value)                      system_ll_set_cpu1_bt_irq_en((hal)->hw, value)
#define system_hal_get_cpu1_bt_irq_en(hal)                             system_ll_get_cpu1_bt_irq_en((hal)->hw)
#define system_hal_set_cpu1_mbox0_int_en(hal, value)                   system_ll_set_cpu1_mbox0_int_en((hal)->hw, value)
#define system_hal_get_cpu1_mbox0_int_en(hal)                          system_ll_get_cpu1_mbox0_int_en((hal)->hw)
#define system_hal_set_cpu1_mbox1_int_en(hal, value)                   system_ll_set_cpu1_mbox1_int_en((hal)->hw, value)
#define system_hal_get_cpu1_mbox1_int_en(hal)                          system_ll_get_cpu1_mbox1_int_en((hal)->hw)
#define system_hal_set_cpu1_bmc64_int_en(hal, value)                   system_ll_set_cpu1_bmc64_int_en((hal)->hw, value)
#define system_hal_get_cpu1_bmc64_int_en(hal)                          system_ll_get_cpu1_bmc64_int_en((hal)->hw)
#define system_hal_set_cpu1_touched_int_en(hal, value)                 system_ll_set_cpu1_touched_int_en((hal)->hw, value)
#define system_hal_get_cpu1_touched_int_en(hal)                        system_ll_get_cpu1_touched_int_en((hal)->hw)
#define system_hal_set_cpu1_usbplug_int_en(hal, value)                 system_ll_set_cpu1_usbplug_int_en((hal)->hw, value)
#define system_hal_get_cpu1_usbplug_int_en(hal)                        system_ll_get_cpu1_usbplug_int_en((hal)->hw)
#define system_hal_set_cpu1_rtc_int_en(hal, value)                     system_ll_set_cpu1_rtc_int_en((hal)->hw, value)
#define system_hal_get_cpu1_rtc_int_en(hal)                            system_ll_get_cpu1_rtc_int_en((hal)->hw)
#define system_hal_set_cpu1_gpio_int_en(hal, value)                    system_ll_set_cpu1_gpio_int_en((hal)->hw, value)
#define system_hal_get_cpu1_gpio_int_en(hal)                           system_ll_get_cpu1_gpio_int_en((hal)->hw)

//0x28
#define system_hal_get_cpu0_bmc32_int_st(hal)                          system_ll_get_cpu0_bmc32_int_st((hal)->hw)
#define system_hal_get_cpu0_host_0_irq_st(hal)                         system_ll_get_cpu0_host_0_irq_st((hal)->hw)
#define system_hal_get_cpu0_host_0_sec_irq_st(hal)                     system_ll_get_cpu0_host_0_sec_irq_st((hal)->hw)
#define system_hal_get_cpu0_timer_int_st(hal)                          system_ll_get_cpu0_timer_int_st((hal)->hw)
#define system_hal_get_cpu0_uart_int_st(hal)                           system_ll_get_cpu0_uart_int_st((hal)->hw)
#define system_hal_get_cpu0_pwm_int_st(hal)                            system_ll_get_cpu0_pwm_int_st((hal)->hw)
#define system_hal_get_cpu0_i2c_int_st(hal)                            system_ll_get_cpu0_i2c_int_st((hal)->hw)
#define system_hal_get_cpu0_spi_int_st(hal)                            system_ll_get_cpu0_spi_int_st((hal)->hw)
#define system_hal_get_cpu0_sadc_int_st(hal)                           system_ll_get_cpu0_sadc_int_st((hal)->hw)
#define system_hal_get_cpu0_irda_int_st(hal)                           system_ll_get_cpu0_irda_int_st((hal)->hw)
#define system_hal_get_cpu0_sdio_int_st(hal)                           system_ll_get_cpu0_sdio_int_st((hal)->hw)
#define system_hal_get_cpu0_gdma_int_st(hal)                           system_ll_get_cpu0_gdma_int_st((hal)->hw)
#define system_hal_get_cpu0_la_int_st(hal)                             system_ll_get_cpu0_la_int_st((hal)->hw)
#define system_hal_get_cpu0_timer1_int_st(hal)                         system_ll_get_cpu0_timer1_int_st((hal)->hw)
#define system_hal_get_cpu0_i2c1_int_st(hal)                           system_ll_get_cpu0_i2c1_int_st((hal)->hw)
#define system_hal_get_cpu0_uart1_int_st(hal)                          system_ll_get_cpu0_uart1_int_st((hal)->hw)
#define system_hal_get_cpu0_uart2_int_st(hal)                          system_ll_get_cpu0_uart2_int_st((hal)->hw)
#define system_hal_get_cpu0_spi1_int_st(hal)                           system_ll_get_cpu0_spi1_int_st((hal)->hw)
#define system_hal_get_cpu0_can_int_st(hal)                            system_ll_get_cpu0_can_int_st((hal)->hw)
#define system_hal_get_cpu0_usb_int_st(hal)                            system_ll_get_cpu0_usb_int_st((hal)->hw)
#define system_hal_get_cpu0_qspi_int_st(hal)                           system_ll_get_cpu0_qspi_int_st((hal)->hw)
#define system_hal_get_cpu0_fft_int_st(hal)                            system_ll_get_cpu0_fft_int_st((hal)->hw)
#define system_hal_get_cpu0_sbc_int_st(hal)                            system_ll_get_cpu0_sbc_int_st((hal)->hw)
#define system_hal_get_cpu0_aud_int_st(hal)                            system_ll_get_cpu0_aud_int_st((hal)->hw)
#define system_hal_get_cpu0_i2s_int_st(hal)                            system_ll_get_cpu0_i2s_int_st((hal)->hw)
#define system_hal_get_cpu0_jpegenc_int_st(hal)                        system_ll_get_cpu0_jpegenc_int_st((hal)->hw)
#define system_hal_get_cpu0_jpegdec_int_st(hal)                        system_ll_get_cpu0_jpegdec_int_st((hal)->hw)
#define system_hal_get_cpu0_lcd_int_st(hal)                            system_ll_get_cpu0_lcd_int_st((hal)->hw)
#define system_hal_get_cpu0_wifi_int_phy_st(hal)                       system_ll_get_cpu0_wifi_int_phy_st((hal)->hw)
#define system_hal_get_cpu0_wifi_mac_int_timer_st(hal)                 system_ll_get_cpu0_wifi_mac_int_timer_st((hal)->hw)
#define system_hal_get_cpu0_wifi_mac_int_misc_st(hal)                  system_ll_get_cpu0_wifi_mac_int_misc_st((hal)->hw)

//0x29
#define system_hal_get_cpu0_wifi_mac_int_rx_trigger_st(hal)            system_ll_get_cpu0_wifi_mac_int_rx_trigger_st((hal)->hw)
#define system_hal_get_cpu0_wifi_mac_int_tx_trigger_st(hal)            system_ll_get_cpu0_wifi_mac_int_tx_trigger_st((hal)->hw)
#define system_hal_get_cpu0_wifi_mac_port_trigger_st(hal)              system_ll_get_cpu0_wifi_mac_port_trigger_st((hal)->hw)
#define system_hal_get_cpu0_wifi_mac_int_gen_st(hal)                   system_ll_get_cpu0_wifi_mac_int_gen_st((hal)->hw)
#define system_hal_get_cpu0_wifi_hsu_irq_st(hal)                       system_ll_get_cpu0_wifi_hsu_irq_st((hal)->hw)
#define system_hal_get_cpu0_wifi_int_mac_wakeup_st(hal)                system_ll_get_cpu0_wifi_int_mac_wakeup_st((hal)->hw)
#define system_hal_get_cpu0_dm_irq_st(hal)                             system_ll_get_cpu0_dm_irq_st((hal)->hw)
#define system_hal_get_cpu0_ble_irq_st(hal)                            system_ll_get_cpu0_ble_irq_st((hal)->hw)
#define system_hal_get_cpu0_bt_irq_st(hal)                             system_ll_get_cpu0_bt_irq_st((hal)->hw)
#define system_hal_get_cpu0_mbox0_int_st(hal)                          system_ll_get_cpu0_mbox0_int_st((hal)->hw)
#define system_hal_get_cpu0_mbox1_int_st(hal)                          system_ll_get_cpu0_mbox1_int_st((hal)->hw)
#define system_hal_get_cpu0_bmc64_int_st(hal)                          system_ll_get_cpu0_bmc64_int_st((hal)->hw)
#define system_hal_get_cpu0_touched_int_st(hal)                        system_ll_get_cpu0_touched_int_st((hal)->hw)
#define system_hal_get_cpu0_usbplug_int_st(hal)                        system_ll_get_cpu0_usbplug_int_st((hal)->hw)
#define system_hal_get_cpu0_rtc_int_st(hal)                            system_ll_get_cpu0_rtc_int_st((hal)->hw)
#define system_hal_get_cpu0_gpio_int_st(hal)                           system_ll_get_cpu0_gpio_int_st((hal)->hw)

//0x2a
#define system_hal_get_cpu1_bmc32_int_st(hal)                          system_ll_get_cpu1_bmc32_int_st((hal)->hw)
#define system_hal_get_cpu1_host_0_irq_st(hal)                         system_ll_get_cpu1_host_0_irq_st((hal)->hw)
#define system_hal_get_cpu1_host_0_sec_irq_st(hal)                     system_ll_get_cpu1_host_0_sec_irq_st((hal)->hw)
#define system_hal_get_cpu1_timer_int_st(hal)                          system_ll_get_cpu1_timer_int_st((hal)->hw)
#define system_hal_get_cpu1_uart_int_st(hal)                           system_ll_get_cpu1_uart_int_st((hal)->hw)
#define system_hal_get_cpu1_pwm_int_st(hal)                            system_ll_get_cpu1_pwm_int_st((hal)->hw)
#define system_hal_get_cpu1_i2c_int_st(hal)                            system_ll_get_cpu1_i2c_int_st((hal)->hw)
#define system_hal_get_cpu1_spi_int_st(hal)                            system_ll_get_cpu1_spi_int_st((hal)->hw)
#define system_hal_get_cpu1_sadc_int_st(hal)                           system_ll_get_cpu1_sadc_int_st((hal)->hw)
#define system_hal_get_cpu1_irda_int_st(hal)                           system_ll_get_cpu1_irda_int_st((hal)->hw)
#define system_hal_get_cpu1_sdio_int_st(hal)                           system_ll_get_cpu1_sdio_int_st((hal)->hw)
#define system_hal_get_cpu1_gdma_int_st(hal)                           system_ll_get_cpu1_gdma_int_st((hal)->hw)
#define system_hal_get_cpu1_la_int_st(hal)                             system_ll_get_cpu1_la_int_st((hal)->hw)
#define system_hal_get_cpu1_timer1_int_st(hal)                         system_ll_get_cpu1_timer1_int_st((hal)->hw)
#define system_hal_get_cpu1_i2c1_int_st(hal)                           system_ll_get_cpu1_i2c1_int_st((hal)->hw)
#define system_hal_get_cpu1_uart1_int_st(hal)                          system_ll_get_cpu1_uart1_int_st((hal)->hw)
#define system_hal_get_cpu1_uart2_int_st(hal)                          system_ll_get_cpu1_uart2_int_st((hal)->hw)
#define system_hal_get_cpu1_spi1_int_st(hal)                           system_ll_get_cpu1_spi1_int_st((hal)->hw)
#define system_hal_get_cpu1_can_int_st(hal)                            system_ll_get_cpu1_can_int_st((hal)->hw)
#define system_hal_get_cpu1_usb_int_st(hal)                            system_ll_get_cpu1_usb_int_st((hal)->hw)
#define system_hal_get_cpu1_qspi_int_st(hal)                           system_ll_get_cpu1_qspi_int_st((hal)->hw)
#define system_hal_get_cpu1_fft_int_st(hal)                            system_ll_get_cpu1_fft_int_st((hal)->hw)
#define system_hal_get_cpu1_sbc_int_st(hal)                            system_ll_get_cpu1_sbc_int_st((hal)->hw)
#define system_hal_get_cpu1_aud_int_st(hal)                            system_ll_get_cpu1_aud_int_st((hal)->hw)
#define system_hal_get_cpu1_i2s_int_st(hal)                            system_ll_get_cpu1_i2s_int_st((hal)->hw)
#define system_hal_get_cpu1_jpegenc_int_st(hal)                        system_ll_get_cpu1_jpegenc_int_st((hal)->hw)
#define system_hal_get_cpu1_jpegdec_int_st(hal)                        system_ll_get_cpu1_jpegdec_int_st((hal)->hw)
#define system_hal_get_cpu1_lcd_int_st(hal)                            system_ll_get_cpu1_lcd_int_st((hal)->hw)
#define system_hal_get_cpu1_wifi_int_phy_st(hal)                       system_ll_get_cpu1_wifi_int_phy_st((hal)->hw)
#define system_hal_get_cpu1_wifi_mac_int_timer_st(hal)                 system_ll_get_cpu1_wifi_mac_int_timer_st((hal)->hw)
#define system_hal_get_cpu1_wifi_mac_int_misc_st(hal)                  system_ll_get_cpu1_wifi_mac_int_misc_st((hal)->hw)

//0x2b
#define system_hal_get_cpu1_wifi_mac_int_rx_trigger_st(hal)            system_ll_get_cpu1_wifi_mac_int_rx_trigger_st((hal)->hw)
#define system_hal_get_cpu1_wifi_mac_int_tx_trigger_st(hal)            system_ll_get_cpu1_wifi_mac_int_tx_trigger_st((hal)->hw)
#define system_hal_get_cpu1_wifi_mac_port_trigger_st(hal)              system_ll_get_cpu1_wifi_mac_port_trigger_st((hal)->hw)
#define system_hal_get_cpu1_wifi_mac_int_gen_st(hal)                   system_ll_get_cpu1_wifi_mac_int_gen_st((hal)->hw)
#define system_hal_get_cpu1_wifi_hsu_irq_st(hal)                       system_ll_get_cpu1_wifi_hsu_irq_st((hal)->hw)
#define system_hal_get_cpu1_wifi_int_mac_wakeup_st(hal)                system_ll_get_cpu1_wifi_int_mac_wakeup_st((hal)->hw)
#define system_hal_get_cpu1_dm_irq_st(hal)                             system_ll_get_cpu1_dm_irq_st((hal)->hw)
#define system_hal_get_cpu1_ble_irq_st(hal)                            system_ll_get_cpu1_ble_irq_st((hal)->hw)
#define system_hal_get_cpu1_bt_irq_st(hal)                             system_ll_get_cpu1_bt_irq_st((hal)->hw)
#define system_hal_get_cpu1_mbox0_int_st(hal)                          system_ll_get_cpu1_mbox0_int_st((hal)->hw)
#define system_hal_get_cpu1_mbox1_int_st(hal)                          system_ll_get_cpu1_mbox1_int_st((hal)->hw)
#define system_hal_get_cpu1_bmc64_int_st(hal)                          system_ll_get_cpu1_bmc64_int_st((hal)->hw)
#define system_hal_get_cpu1_touched_int_st(hal)                        system_ll_get_cpu1_touched_int_st((hal)->hw)
#define system_hal_get_cpu1_usbplug_int_st(hal)                        system_ll_get_cpu1_usbplug_int_st((hal)->hw)
#define system_hal_get_cpu1_rtc_int_st(hal)                            system_ll_get_cpu1_rtc_int_st((hal)->hw)
#define system_hal_get_cpu1_gpio_int_st(hal)                           system_ll_get_cpu1_gpio_int_st((hal)->hw)

//REG_0x30 0x31 0x32 0x33 0x34 0x35
#define system_hal_set_gpio0_perial_mode(hal, group, value)            system_ll_set_gpio0_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio0_perial_mode(hal, group)                   system_ll_get_gpio0_perial_mode((hal)->hw, group)
#define system_hal_set_gpio1_perial_mode(hal, group, value)            system_ll_set_gpio1_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio1_perial_mode(hal, group)                   system_ll_get_gpio1_perial_mode((hal)->hw, group)
#define system_hal_set_gpio2_perial_mode(hal, group, value)            system_ll_set_gpio2_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio2_perial_mode(hal, group)                   system_ll_get_gpio2_perial_mode((hal)->hw, group)
#define system_hal_set_gpio3_perial_mode(hal, group, value)            system_ll_set_gpio3_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio3_perial_mode(hal, group)                   system_ll_get_gpio3_perial_mode((hal)->hw, group)
#define system_hal_set_gpio4_perial_mode(hal, group, value)            system_ll_set_gpio4_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio4_perial_mode(hal, group)                   system_ll_get_gpio4_perial_mode((hal)->hw, group)
#define system_hal_set_gpio5_perial_mode(hal, group, value)            system_ll_set_gpio5_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio5_perial_mode(hal, group)                   system_ll_get_gpio5_perial_mode((hal)->hw, group)
#define system_hal_set_gpio6_perial_mode(hal, group, value)            system_ll_set_gpio6_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio6_perial_mode(hal, group)                   system_ll_get_gpio6_perial_mode((hal)->hw, group)
#define system_hal_set_gpio7_perial_mode(hal, group, value)            system_ll_set_gpio7_perial_mode((hal)->hw, group, value)
#define system_hal_get_gpio7_perial_mode(hal, group)                   system_ll_get_gpio7_perial_mode((hal)->hw, group)

//0x38
#define system_hal_set_dbug_cfg0(hal, group, value)                    system_ll_set_dbug_cfg0((hal)->hw, group, value)
#define system_hal_get_dbug_cfg0(hal, group)                           system_ll_get_dbug_cfg0((hal)->hw, group)

//0x39
#define system_hal_set_dbug_cfg1(hal, group, value)                    system_ll_set_dbug_cfg1((hal)->hw, group, value)
#define system_hal_get_dbug_cfg1(hal, group)                           system_ll_get_dbug_cfg1((hal)->hw, group)


#ifdef __cplusplus
}
#endif

