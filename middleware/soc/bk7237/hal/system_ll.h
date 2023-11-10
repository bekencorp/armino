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
#include "system_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYSTEM_LL_REG_BASE      (SOC_SYSTEM_REG_BASE)

//reg0x00
static inline uint32 system_ll_get_device_id(system_hw_t *hw)
{
	return hw->device_id.v;
}

//reg0x01
static inline uint32 system_ll_get_version_id(system_hw_t *hw)
{
	return hw->version_id.v;
}

//reg0x02
static inline uint32 system_ll_get_core0_halted_st(system_hw_t *hw)
{
	return hw->cpu_current_run_status.core0_halted;
}

static inline uint32 system_ll_get_core1_halted_st(system_hw_t *hw)
{
	return hw->cpu_current_run_status.core1_halted;
}

static inline uint32 system_ll_get_cpu0_sw_reset_st(system_hw_t *hw)
{
	return hw->cpu_current_run_status.cpu0_sw_reset;
}

static inline uint32 system_ll_get_cpu1_sw_reset_st(system_hw_t *hw)
{
	return hw->cpu_current_run_status.cpu1_sw_reset;
}

static inline uint32 system_ll_get_cpu0_pwr_dw_st(system_hw_t *hw)
{
	return hw->cpu_current_run_status.cpu0_pwr_dw_state;
}

static inline uint32 system_ll_get_cpu1_pwr_dw_st(system_hw_t *hw)
{
	return hw->cpu_current_run_status.cpu1_pwr_dw_state;
}

//reg0x03 boot mode:  0:ROM boot 1:FLASH boot
static inline void system_ll_set_boot_rom_mode(system_hw_t *hw)
{
	hw->cpu_storage_connect_op_select.boot_mode = 0;
}

static inline void system_ll_set_boot_flash_mode(system_hw_t *hw)
{
	hw->cpu_storage_connect_op_select.boot_mode = 1;
}

static inline uint32_t system_ll_get_boot_mode(system_hw_t *hw)
{
	return hw->cpu_storage_connect_op_select.boot_mode;
}

//reg0x03 jtag connect: 0: jtag connect core0, 1:jtag connect core1
static inline void system_ll_set_jtag_connect_core0(system_hw_t *hw)
{
	hw->cpu_storage_connect_op_select.jtag_core_sel = 0;
}

static inline void system_ll_set_jtag_connect_core1(system_hw_t *hw)
{
	hw->cpu_storage_connect_op_select.jtag_core_sel = 1;
}

static inline uint32_t system_ll_get_jtag_connect_core(system_hw_t *hw)
{
	return hw->cpu_storage_connect_op_select.jtag_core_sel;
}

//reg0x03 flash select operation: 0: normal flash operation 1:flash download by spi
static inline void system_ll_set_flash_select_normal_op(system_hw_t *hw)
{
	hw->cpu_storage_connect_op_select.flash_sel = 0;
}

static inline void system_ll_set_flash_select_spi_op(system_hw_t *hw)
{
	hw->cpu_storage_connect_op_select.flash_sel = 1;
}

static inline uint32_t system_ll_get_flash_select_op(system_hw_t *hw)
{
	return hw->cpu_storage_connect_op_select.flash_sel;
}

//reg0x04 cpu0 int mask
static inline void system_ll_set_cpu0_int_mask(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_halt_clk_op.cpu0_int_mask = value;
}
static inline uint32_t system_ll_get_cpu0_int_mask(system_hw_t *hw)
{
	return hw->cpu0_int_halt_clk_op.cpu0_int_mask;
}

//reg0x04 core0 halt indicate
static inline void system_ll_set_core0_halt_indicate(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_halt_clk_op.cpu0_halt = value;
}
static inline uint32_t system_ll_get_core0_halt_indicate(system_hw_t *hw)
{
	return hw->cpu0_int_halt_clk_op.cpu0_halt;
}

//reg0x04 [4:7] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_cpu0_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_halt_clk_op.cpu0_clk_div = value;
}
static inline uint32_t system_ll_get_cpu0_clk_div(system_hw_t *hw)
{
	return hw->cpu0_int_halt_clk_op.cpu0_clk_div;
}

//reg0x05 cpu1 int mask
static inline void system_ll_set_cpu1_int_mask(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_halt_clk_op.cpu1_int_mask = value;
}
static inline uint32_t system_ll_get_cpu1_int_mask(system_hw_t *hw)
{
	return hw->cpu1_int_halt_clk_op.cpu1_int_mask;
}

//reg0x05 core1 halt indicate
static inline void system_ll_set_core1_halt_indicate(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_halt_clk_op.cpu1_halt = value;
}
static inline uint32_t system_ll_get_core1_halt_indicate(system_hw_t *hw)
{
	return hw->cpu1_int_halt_clk_op.cpu1_halt;
}

//reg0x05 [4:7] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_cpu1_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_halt_clk_op.cpu1_clk_div = value;
}
static inline uint32_t system_ll_get_cpu1_clk_div(system_hw_t *hw)
{
	return hw->cpu1_int_halt_clk_op.cpu1_clk_div;
}

//reg0x08 [0:3] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_core_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_core = value;
}
static inline uint32_t system_ll_get_core_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_core;
}

//reg0x08 [4:5]  0: clk_DCO      1 : XTAL
static inline void system_ll_set_core_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_core = value;
}
static inline uint32_t system_ll_get_core_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_core;
}

//reg0x08 [6] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_bus_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_bus = value;
}
static inline uint32_t system_ll_get_bus_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_bus;
}

//reg0x08 [8:9] Frequency division :    0:/1  1:/2  2:/4  3:/8
static inline void system_ll_set_uart0_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_uart0 = value;
}
static inline uint32_t system_ll_get_uart0_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_uart0;
}

//reg0x08 uart0 0:XTAL              1:APLL
static inline void system_ll_set_uart0_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_uart0 = value;
}
static inline uint32_t system_ll_get_uart0_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_uart0;
}

//reg0x08 [11:12] Frequency division :  0:/1  1:/2  2:/4  3:/8
static inline void system_ll_set_uart1_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_uart1 = value;
}
static inline uint32_t system_ll_get_uart1_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_uart1;
}

//reg0x08 uart1 0:XTAL			  1:APLL
static inline void system_ll_set_uart1_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_uart1 = value;
}
static inline uint32_t system_ll_get_uart1_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_uart1;
}

//reg0x08 uart2 Frequency division :  0:/1  1:/2  2:/4  3:/8
static inline void system_ll_set_uart2_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_uart2 = value;
}
static inline uint32_t system_ll_get_uart2_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_uart2;
}

//reg0x08 uart2 0:XTAL			  1:APLL
static inline void system_ll_set_uart2_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_uart2 = value;
}
static inline uint32_t system_ll_get_uart2_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_uart2;
}

//reg0x08 sadc 0:XTAL			  1:APLL
static inline void system_ll_set_sadc_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_sadc = value;
}
static inline uint32_t system_ll_get_sadc_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_sadc;
}

//reg0x08 pwm0 0:clk32              1:XTAL
static inline void system_ll_set_pwm0_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_pwm0 = value;
}
static inline uint32_t system_ll_get_pwm0_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_pwm0;
}

//reg0x08 pwm1 0:clk32              1:XTAL
static inline void system_ll_set_pwm1_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_pwm1 = value;
}
static inline uint32_t system_ll_get_pwm1_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_pwm1;
}

//reg0x08 timer0 0:clk32				1:XTAL
static inline void system_ll_set_timer0_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_timer0 = value;
}
static inline uint32_t system_ll_get_timer0_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_timer0;
}

//reg0x08 timer1 0:clk32				1:XTAL
static inline void system_ll_set_timer1_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_timer1 = value;
}
static inline uint32_t system_ll_get_timer1_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_timer1;
}

//reg0x08 timer2 0:clk32				1:XTAL
static inline void system_ll_set_timer2_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_timer2 = value;
}
static inline uint32_t system_ll_get_timer2_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_timer2;
}

//reg0x08 i2s 0:XTAL			  1:APLL
static inline void system_ll_set_i2s_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_i2s = value;
}
static inline uint32_t system_ll_get_i2s_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_i2s;
}

//reg0x08 aud 0:XTAL			  1:APLL
static inline void system_ll_set_aud_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_aud = value;
}
static inline uint32_t system_ll_get_aud_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_aud;
}

//reg0x08 [26:29] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_jpeg_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_jpeg = value;
}
static inline uint32_t system_ll_get_jpeg_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_jpeg;
}

//reg0x08 jpeg 0:clk_320M      1:clk_480M
static inline void system_ll_set_jpeg_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.cksel_jpeg = value;
}
static inline uint32_t system_ll_get_jpeg_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.cksel_jpeg;
}

//reg0x08 disp: Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2)
static inline void system_ll_set_disp_l_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode1.clkdiv_disp_l = value;
}
static inline uint32_t system_ll_get_disp_l_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode1.clkdiv_disp_l;
}

//reg0x09 [0:2] Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2)
static inline void system_ll_set_disp_h_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.clkdiv_disp_h = value;
}
static inline uint32_t system_ll_get_disp_h_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.clkdiv_disp_h;
}

//reg0x09 disp 0:clk_320M	   1:clk_480M
static inline void system_ll_set_disp_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.cksel_disp = value;
}
static inline uint32_t system_ll_get_disp_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.cksel_disp;
}

//reg0x09 psram Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_psram_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.ckdiv_psram = value;
}
static inline uint32_t system_ll_get_psram_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.ckdiv_psram;
}

//reg0x09 psram 0:clk_320M	   1:clk_480M
static inline void system_ll_set_psram_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.cksel_psram = value;
}
static inline uint32_t system_ll_get_psram_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.cksel_psram;
}

//reg0x09 qspi0 [6:9] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_qspi0_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.ckdiv_qspi0 = value;
}
static inline uint32_t system_ll_get_qspi0_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.ckdiv_qspi0;
}

//reg0x09 sdio [14:16]  0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256
static inline void system_ll_set_sdio_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.ckdiv_sdio = value;
}
static inline uint32_t system_ll_get_sdio_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.ckdiv_sdio;
}

//reg0x09 sdio 0：XTAL          1：320M
static inline void system_ll_set_sdio_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.cksel_sdio = value;
}
static inline uint32_t system_ll_get_sdio_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.cksel_sdio;
}

//reg0x09 [18:21] Frequency division : F/(1+N), N is the data of the reg value
static inline void system_ll_set_auxs_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.ckdiv_auxs = value;
}
static inline uint32_t system_ll_get_auxs_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.ckdiv_auxs;
}

//reg0x09 pwm1 [22:23]  0:DCO     1:APLL      2:320M       3:480M
static inline void system_ll_set_auxs_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.cksel_auxs = value;
}
static inline uint32_t system_ll_get_auxs_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.cksel_auxs;
}

//reg0x09 flash [24:25]  0:XTAL      1:APLL      1x :clk_120M
static inline void system_ll_set_flash_clk_sel(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.cksel_flash = value;
}
static inline uint32_t system_ll_get_flash_clk_sel(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.cksel_flash;
}

//reg0x09 flash [26:27]  0:/1  1:/2  2:/4  3:/8
static inline void system_ll_set_flash_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.ckdiv_flash = value;
}
static inline uint32_t system_ll_get_flash_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.ckdiv_flash;
}

//reg0x09 i2s0 [28:30]  0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256
static inline void system_ll_set_i2s0_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_clk_div_mode2.ckdiv_i2s0 = value;
}
static inline uint32_t system_ll_get_i2s0_clk_div(system_hw_t *hw)
{
	return hw->cpu_clk_div_mode2.ckdiv_i2s0;
}

//reg0x0a 26m [1:0] 	 0:/1  1:/2  2:/4  3:/8
static inline void system_ll_set_26m_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_26m_wdt_clk_div.ckdiv_26m = value;
}
static inline uint32_t system_ll_get_26m_clk_div(system_hw_t *hw)
{
	return hw->cpu_26m_wdt_clk_div.ckdiv_26m;
}

//reg0x0a wdt [1:0] 	 0:/1  1:/2  2:/4  3:/8
static inline void system_ll_set_wdt_clk_div(system_hw_t *hw, uint32_t value)
{
	hw->cpu_26m_wdt_clk_div.ckdiv_wdt = value;
}
static inline uint32_t system_ll_get_wdt_clk_div(system_hw_t *hw)
{
	return hw->cpu_26m_wdt_clk_div.ckdiv_wdt;
}

//reg0x0b anaspi_freq [15:0]
static inline void system_ll_set_anaspi_freq(system_hw_t *hw, uint32_t value)
{
	hw->cpu_anaspi_freq.anaspi_freq = value;
}
static inline uint32_t system_ll_get_anaspi_freq(system_hw_t *hw)
{
	return hw->cpu_anaspi_freq.anaspi_freq;
}

//reg0x0c
static inline void system_ll_set_i2c0_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.i2c0_cken = value;
}
static inline uint32_t system_ll_get_i2c0_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.i2c0_cken;
}

static inline void system_ll_set_spi0_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.spi0_cken = value;
}
static inline uint32_t system_ll_get_spi0_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.spi0_cken;
}

static inline void system_ll_set_uart0_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.uart0_cken = value;
}
static inline uint32_t system_ll_get_uart0_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.uart0_cken;
}

static inline void system_ll_set_pwm0_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.pwm0_cken = value;
}
static inline uint32_t system_ll_get_pwm0_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.pwm0_cken;
}

static inline void system_ll_set_timer0_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.tim0_cken = value;
}
static inline uint32_t system_ll_get_timer0_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.tim0_cken;
}

static inline void system_ll_set_sadc_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.sadc_cken = value;
}
static inline uint32_t system_ll_get_sadc_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.sadc_cken;
}

static inline void system_ll_set_irda_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.irda_cken = value;
}
static inline uint32_t system_ll_get_irda_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.irda_cken;
}

static inline void system_ll_set_efuse_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.efuse_cken = value;
}
static inline uint32_t system_ll_get_efuse_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.efuse_cken;
}

static inline void system_ll_set_i2c1_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.i2c1_cken = value;
}
static inline uint32_t system_ll_get_i2c1_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.i2c1_cken;
}

static inline void system_ll_set_spi1_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.spi1_cken = value;
}
static inline uint32_t system_ll_get_spi1_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.spi1_cken;
}

static inline void system_ll_set_uart1_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.uart1_cken = value;
}
static inline uint32_t system_ll_get_uart1_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.uart1_cken;
}

static inline void system_ll_set_uart2_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.uart2_cken = value;
}
static inline uint32_t system_ll_get_uart2_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.uart2_cken;
}

static inline void system_ll_set_pwm1_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.pwm1_cken = value;
}
static inline uint32_t system_ll_get_pwm1_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.pwm1_cken;
}

static inline void system_ll_set_timer1_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.tim1_cken = value;
}
static inline uint32_t system_ll_get_timer1_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.tim1_cken;
}

static inline void system_ll_set_timer2_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.tim2_cken = value;
}
static inline uint32_t system_ll_get_timer2_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.tim2_cken;
}

static inline void system_ll_set_otp_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.otp_cken = value;
}
static inline uint32_t system_ll_get_otp_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.otp_cken;
}

static inline void system_ll_set_i2s_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.i2s_cken = value;
}
static inline uint32_t system_ll_get_i2s_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.i2s_cken;
}

static inline void system_ll_set_usb_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.usb_cken = value;
}
static inline uint32_t system_ll_get_usb_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.usb_cken;
}

static inline void system_ll_set_can_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.can_cken = value;
}
static inline uint32_t system_ll_get_can_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.can_cken;
}

static inline void system_ll_set_psram_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.psram_cken = value;
}
static inline uint32_t system_ll_get_psram_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.psram_cken;
}

static inline void system_ll_set_qspi0_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.qspi0_cken = value;
}
static inline uint32_t system_ll_get_qspi0_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.qspi0_cken;
}

static inline void system_ll_set_qspi1_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.qspi1_cken = value;
}
static inline uint32_t system_ll_get_qspi1_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.qspi1_cken;
}

static inline void system_ll_set_sdio_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.sdio_cken = value;
}
static inline uint32_t system_ll_get_sdio_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.sdio_cken;
}

static inline void system_ll_set_auxs_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.auxs_cken = value;
}
static inline uint32_t system_ll_get_auxs_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.auxs_cken;
}

static inline void system_ll_set_btdm_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.btdm_cken = value;
}
static inline uint32_t system_ll_get_btdm_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.btdm_cken;
}

static inline void system_ll_set_xvr_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.xvr_cken = value;
}
static inline uint32_t system_ll_get_xvr_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.xvr_cken;
}

static inline void system_ll_set_mac_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.mac_cken = value;
}
static inline uint32_t system_ll_get_mac_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.mac_cken;
}

static inline void system_ll_set_phy_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.phy_cken = value;
}
static inline uint32_t system_ll_get_phy_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.phy_cken;
}

static inline void system_ll_set_jpeg_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.jpeg_cken = value;
}
static inline uint32_t system_ll_get_jpeg_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.jpeg_cken;
}

static inline void system_ll_set_disp_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.disp_cken = value;
}
static inline uint32_t system_ll_get_disp_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.disp_cken;
}

static inline void system_ll_set_aud_clk_en(system_hw_t *hw, uint32_t value)
		{
		hw->cpu_device_clk_enable.aud_cken = value;
		}
static inline uint32_t system_ll_get_aud_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.aud_cken;
}

static inline void system_ll_set_wdt_clk_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_device_clk_enable.wdt_cken = value;
}
static inline uint32_t system_ll_get_wdt_clk_en(system_hw_t *hw)
{
	return hw->cpu_device_clk_enable.wdt_cken;
}

//reg0x0e
static inline void system_ll_set_aon_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.aon_disckg = value;
}
static inline uint32_t system_ll_get_aon_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.aon_disckg;
}

static inline void system_ll_set_sys_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.sys_disckg = value;
}
static inline uint32_t system_ll_get_sys_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.sys_disckg;
}

static inline void system_ll_set_dma_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.dma_disckg = value;
}
static inline uint32_t system_ll_get_dma_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.dma_disckg;
}

static inline void system_ll_set_flash_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.flash_disckg = value;
}
static inline uint32_t system_ll_get_flash_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.flash_disckg;
}

static inline void system_ll_set_wdt_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.wdt_disckg = value;
}
static inline uint32_t system_ll_get_wdt_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.wdt_disckg;
}

static inline void system_ll_set_timer0_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.tim_disckg = value;
}
static inline uint32_t system_ll_get_timer0_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.tim_disckg;
}

static inline void system_ll_set_uart0_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.urt_disckg = value;
}
static inline uint32_t system_ll_get_uart0_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.urt_disckg;
}

static inline void system_ll_set_pwm0_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.pwm_disckg = value;
}
static inline uint32_t system_ll_get_pwm0_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.pwm_disckg;
}

static inline void system_ll_set_i2c0_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.i2c_disckg = value;
}
static inline uint32_t system_ll_get_i2c0_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.i2c_disckg;
}

static inline void system_ll_set_spi0_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.spi_disckg = value;
}
static inline uint32_t system_ll_get_spi0_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.spi_disckg;
}

static inline void system_ll_set_sadc_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.sadc_disckg = value;
}
static inline uint32_t system_ll_get_sadc_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.sadc_disckg;
}

static inline void system_ll_set_efuse_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.efs_disckg = value;
}
static inline uint32_t system_ll_get_efuse_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.efs_disckg;
}

static inline void system_ll_set_irda_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.irda_disckg = value;
}
static inline uint32_t system_ll_get_irda_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.irda_disckg;
}

static inline void system_ll_set_trng_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.trng_disckg = value;
}
static inline uint32_t system_ll_get_trng_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.trng_disckg;
}

static inline void system_ll_set_sdio_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.sdio_disckg = value;
}
static inline uint32_t system_ll_get_sdio_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.sdio_disckg;
}

static inline void system_ll_set_la_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.la_disckg = value;
}
static inline uint32_t system_ll_get_la_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.la_disckg;
}

static inline void system_ll_set_timer1_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.tim1_disckg = value;
}
static inline uint32_t system_ll_get_timer1_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.tim1_disckg;
}

static inline void system_ll_set_uart1_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.urt1_disckg = value;
}
static inline uint32_t system_ll_get_uart1_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.urt1_disckg;
}

static inline void system_ll_set_uart2_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.urt2_disckg = value;
}
static inline uint32_t system_ll_get_uart2_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.urt2_disckg;
}

static inline void system_ll_set_pwm1_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.pwm1_disckg = value;
}
static inline uint32_t system_ll_get_pwm1_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.pwm1_disckg;
}

static inline void system_ll_set_i2c1_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.i2c1_disckg = value;
}
static inline uint32_t system_ll_get_i2c1_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.i2c1_disckg;
}

static inline void system_ll_set_spi1_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.spi1_disckg = value;
}
static inline uint32_t system_ll_get_spi1_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.spi1_disckg;
}

static inline void system_ll_set_usb_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.usb_disckg = value;
}
static inline uint32_t system_ll_get_usb_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.usb_disckg;
}

static inline void system_ll_set_can_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.can_disckg = value;
}
static inline uint32_t system_ll_get_can_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.can_disckg;
}

static inline void system_ll_set_qspi0_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.qspi0_disckg = value;
}
static inline uint32_t system_ll_get_qspi0_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.qspi0_disckg;
}

static inline void system_ll_set_psram_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.psram_disckg = value;
}
static inline uint32_t system_ll_get_psram_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.psram_disckg;
}

static inline void system_ll_set_fft_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.fft_disckg = value;
}
static inline uint32_t system_ll_get_fft_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.fft_disckg;
}

static inline void system_ll_set_sbc_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.sbc_disckg = value;
}
static inline uint32_t system_ll_get_sbc_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.sbc_disckg;
}

static inline void system_ll_set_aud_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.aud_disckg = value;
}
static inline uint32_t system_ll_get_aud_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.aud_disckg;
}

static inline void system_ll_set_i2s_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.i2s_disckg = value;
}
static inline uint32_t system_ll_get_i2s_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.i2s_disckg;
}

static inline void system_ll_set_jpeg_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.jpeg_disckg = value;
}
static inline uint32_t system_ll_get_jpeg_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.jpeg_disckg;
}

static inline void system_ll_set_jpeg_dec_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg1.jpeg_dec_disckg = value;
}
static inline uint32_t system_ll_get_jpeg_dec_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg1.jpeg_dec_disckg;
}

//reg0x0f
static inline void system_ll_set_disp_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg2.disp_disckg = value;
}
static inline uint32_t system_ll_get_disp_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg2.disp_disckg;
}

static inline void system_ll_set_dma2d_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg2.dma2d_disckg = value;
}
static inline uint32_t system_ll_get_dma2d_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg2.dma2d_disckg;
}

static inline void system_ll_set_btdm_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg2.btdm_disckg = value;
}
static inline uint32_t system_ll_get_btdm_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg2.btdm_disckg;
}

static inline void system_ll_set_xver_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg2.xver_disckg = value;
}
static inline uint32_t system_ll_get_xver_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg2.xver_disckg;
}

//0x0f[8:5]   BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open
static inline void system_ll_set_btdm_bps_disckg(system_hw_t *hw, uint32_t value)
{
	hw->cpu_mode_disckg2.btdm_bps_disckg = value;
}
static inline uint32_t system_ll_get_btdm_bps_disckg(system_hw_t *hw)
{
	return hw->cpu_mode_disckg2.btdm_bps_disckg;
}

//0x10
static inline void system_ll_set_power_on_mem1(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_mem1 = value;
}
static inline uint32_t system_ll_get_power_on_mem1(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_mem1;
}

static inline void system_ll_set_power_on_mem2(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_mem2 = value;
}
static inline uint32_t system_ll_get_power_on_mem2(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_mem2;
}

static inline void system_ll_set_power_on_mem3(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_mem3 = value;
}
static inline uint32_t system_ll_get_power_on_mem3(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_mem3;
}

static inline void system_ll_set_power_on_encp(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_encp = value;
}
static inline uint32_t system_ll_get_power_on_encp(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_encp;
}

static inline void system_ll_set_power_on_bakp(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_bakp = value;
}
static inline uint32_t system_ll_get_power_on_bakp(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_bakp;
}

static inline void system_ll_set_power_on_ahbp(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_ahbp = value;
}
static inline uint32_t system_ll_get_power_on_ahbp(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_ahbp;
}

static inline void system_ll_set_power_on_audp(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_audp = value;
}
static inline uint32_t system_ll_get_power_on_audp(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_audp;
}

static inline void system_ll_set_power_on_vidp(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_vidp = value;
}
static inline uint32_t system_ll_get_power_on_vidp(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_vidp;
}

static inline void system_ll_set_power_on_btsp(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_btsp = value;
}
static inline uint32_t system_ll_get_power_on_btsp(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_btsp;
}

static inline void system_ll_set_power_on_wifi_mac(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_wifp_mac = value;
}
static inline uint32_t system_ll_get_power_on_wifi_mac(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_wifp_mac;
}

static inline void system_ll_set_power_on_wifi_phy(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.pwd_wifp_phy = value;
}
static inline uint32_t system_ll_get_power_on_wifi_phy(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.pwd_wifp_phy;
}

static inline void system_ll_set_sleep_en_need_flash_idle(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.sleep_en_need_flash_idle = value;
}
static inline uint32_t system_ll_get_sleep_en_need_flash_idle(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.sleep_en_need_flash_idle;
}

static inline void system_ll_set_sleep_en_need_cpu1_wifi(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.sleep_en_need_cpu1_wifi = value;
}
static inline uint32_t system_ll_get_sleep_en_need_cpu1_wifi(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.sleep_en_need_cpu1_wifi;
}

static inline void system_ll_set_sleep_en_need_cpu0_wifi(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.sleep_en_need_cpu0_wfi = value;
}
static inline uint32_t system_ll_get_sleep_en_need_cpu0_wifi(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.sleep_en_need_cpu0_wfi;
}

static inline void system_ll_set_sleep_en_need_global(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.sleep_en_global = value;
}
static inline uint32_t system_ll_get_sleep_en_need_global(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.sleep_en_global;
}

static inline void system_ll_set_wifi_wakeup_platform_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.wifi_wakeup_platform_en = value;
}
static inline uint32_t system_ll_get_wifi_wakeup_platform_en(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.wifi_wakeup_platform_en;
}

static inline void system_ll_set_bts_wakeup_platform_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.bts_wakeup_platform_en = value;
}
static inline uint32_t system_ll_get_bts_wakeup_platform_en(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.bts_wakeup_platform_en;
}

static inline void system_ll_set_bts_sleep_exit_req(system_hw_t *hw, uint32_t value)
{
	hw->cpu_power_sleep_wakeup.bts_sleep_exit_req = value;
}
static inline uint32_t system_ll_get_bts_sleep_exit_req(system_hw_t *hw)
{
	return hw->cpu_power_sleep_wakeup.bts_sleep_exit_req;
}

//reg0x20 cpu0 int 0:31
static inline void system_ll_set_cpu0_bmc32_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_bmc32_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_bmc32_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_bmc32_int_en;
}

static inline void system_ll_set_cpu0_host_0_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_host_0_irq_en = value;
}
static inline uint32_t system_ll_get_cpu0_host_0_irq_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_host_0_irq_en;
}

static inline void system_ll_set_cpu0_host_0_sec_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_host_0_sec_irq_en = value;
}
static inline uint32_t system_ll_get_cpu0_host_0_sec_irq_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_host_0_sec_irq_en;
}

static inline void system_ll_set_cpu0_timer_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_timer_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_timer_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_timer_int_en;
}

static inline void system_ll_set_cpu0_uart_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_uart_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_uart_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_uart_int_en;
}

static inline void system_ll_set_cpu0_pwm_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_pwm_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_pwm_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_pwm_int_en;
}

static inline void system_ll_set_cpu0_i2c_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_i2c_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_i2c_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_i2c_int_en;
}

static inline void system_ll_set_cpu0_spi_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_spi_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_spi_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_spi_int_en;
}

static inline void system_ll_set_cpu0_sadc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_sadc_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_sadc_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_sadc_int_en;
}

static inline void system_ll_set_cpu0_irda_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_irda_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_irda_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_irda_int_en;
}

static inline void system_ll_set_cpu0_sdio_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_sdio_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_sdio_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_sdio_int_en;
}

static inline void system_ll_set_cpu0_gdma_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_gdma_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_gdma_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_gdma_int_en;
}

static inline void system_ll_set_cpu0_la_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_la_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_la_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_la_int_en;
}

static inline void system_ll_set_cpu0_timer1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_timer1_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_timer1_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_timer1_int_en;
}

static inline void system_ll_set_cpu0_i2c1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_i2c1_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_i2c1_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_i2c1_int_en;
}

static inline void system_ll_set_cpu0_uart1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_uart1_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_uart1_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_uart1_int_en;
}

static inline void system_ll_set_cpu0_uart2_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_uart2_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_uart2_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_uart2_int_en;
}

static inline void system_ll_set_cpu0_spi1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_spi1_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_spi1_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_spi1_int_en;
}

static inline void system_ll_set_cpu0_can_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_can_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_can_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_can_int_en;
}

static inline void system_ll_set_cpu0_usb_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_usb_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_usb_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_usb_int_en;
}

static inline void system_ll_set_cpu0_qspi_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_qspi_int_en = value;
}
static inline uint32_t system_ll_cpu0_qspi_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_qspi_int_en;
}

static inline void system_ll_set_cpu0_fft_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_fft_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_fft_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_fft_int_en;
}

static inline void system_ll_set_cpu0_sbc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_sbc_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_sbc_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_sbc_int_en;
}

static inline void system_ll_set_cpu0_aud_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_aud_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_aud_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_aud_int_en;
}

static inline void system_ll_set_cpu0_i2s_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_i2s_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_i2s_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_i2s_int_en;
}

static inline void system_ll_set_cpu0_jpegenc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_jpegenc_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_jpegenc_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_jpegenc_int_en;
}

static inline void system_ll_set_cpu0_jpegdec_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_jpegdec_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_jpegdec_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_jpegdec_int_en;
}

static inline void system_ll_set_cpu0_lcd_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_lcd_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_lcd_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_lcd_int_en;
}

static inline void system_ll_set_cpu0_wifi_int_phy_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_wifi_int_phy_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_int_phy_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_wifi_int_phy_en;
}

static inline void system_ll_set_cpu0_wifi_mac_int_timer_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_0_31_en.cpu0_wifi_mac_int_tx_rx_timer_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_timer_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_wifi_mac_int_tx_rx_timer_en;
}

static inline void system_ll_set_cpu0_wifi_mac_int_misc_en(system_hw_t *hw, uint32_t value)
{
hw->cpu0_int_0_31_en.cpu0_wifi_mac_int_tx_rx_misc_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_misc_en(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_en.cpu0_wifi_mac_int_tx_rx_misc_en;
}

//0x21 int 32:63
static inline void system_ll_set_cpu0_wifi_mac_int_rx_trigger_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_rx_trigger_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_rx_trigger_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_rx_trigger_en;
}

static inline void system_ll_set_cpu0_wifi_mac_int_tx_trigger_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_tx_trigger_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_tx_trigger_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_tx_trigger_en;
}

static inline void system_ll_set_cpu0_wifi_mac_port_trigger_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_wifi_mac_port_trigger_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_port_trigger_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_wifi_mac_port_trigger_en;
}
static inline void system_ll_set_cpu0_wifi_mac_int_gen_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_gen_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_gen_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_gen_en;
}

static inline void system_ll_set_cpu0_wifi_hsu_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_wifi_hsu_irq_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_hsu_irq_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_wifi_hsu_irq_en;
}

static inline void system_ll_set_cpu0_wifi_int_mac_wakeup_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_wifi_int_mac_wakeup_en = value;
}
static inline uint32_t system_ll_get_cpu0_wifi_int_mac_wakeup_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_wifi_int_mac_wakeup_en;
}

static inline void system_ll_set_cpu0_dm_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_dm_irq_en = value;
}
static inline uint32_t system_ll_get_cpu0_dm_irq_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_dm_irq_en;
}

static inline void system_ll_set_cpu0_ble_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_ble_irq_en = value;
}
static inline uint32_t system_ll_get_cpu0_ble_irq_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_ble_irq_en;
}

static inline void system_ll_set_cpu0_bt_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_bt_irq_en = value;
}
static inline uint32_t system_ll_get_cpu0_bt_irq_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_bt_irq_en;
}

static inline void system_ll_set_cpu0_mbox0_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_mbox0_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_mbox0_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_mbox0_int_en;
}

static inline void system_ll_set_cpu0_mbox1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_mbox1_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_mbox1_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_mbox1_int_en;
}

static inline void system_ll_set_cpu0_bmc64_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_bmc64_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_bmc64_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_bmc64_int_en;
}

static inline void system_ll_set_cpu0_touched_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_touched_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_touched_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_touched_int_en;
}

static inline void system_ll_set_cpu0_usbplug_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_usbplug_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_usbplug_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_usbplug_int_en;
}

static inline void system_ll_set_cpu0_rtc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_rtc_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_rtc_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_rtc_int_en;
}

static inline void system_ll_set_cpu0_gpio_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu0_int_32_63_en.cpu0_gpio_int_en = value;
}
static inline uint32_t system_ll_get_cpu0_gpio_int_en(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_en.cpu0_gpio_int_en;
}

//reg0x22 cpu1 int 0:31
static inline void system_ll_set_cpu1_bmc32_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_bmc32_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_bmc32_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_bmc32_int_en;
}

static inline void system_ll_set_cpu1_host_0_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_host_0_irq_en = value;
}
static inline uint32_t system_ll_get_cpu1_host_0_irq_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_host_0_irq_en;
}

static inline void system_ll_set_cpu1_host_0_sec_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_host_0_sec_irq_en = value;
}
static inline uint32_t system_ll_get_cpu1_host_0_sec_irq_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_host_0_sec_irq_en;
}

static inline void system_ll_set_cpu1_timer_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_timer_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_timer_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_timer_int_en;
}

static inline void system_ll_set_cpu1_uart_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_uart_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_uart_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_uart_int_en;
}

static inline void system_ll_set_cpu1_pwm_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_pwm_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_pwm_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_pwm_int_en;
}

static inline void system_ll_set_cpu1_i2c_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_i2c_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_i2c_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_i2c_int_en;
}

static inline void system_ll_set_cpu1_spi_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_spi_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_spi_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_spi_int_en;
}

static inline void system_ll_set_cpu1_sadc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_sadc_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_sadc_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_sadc_int_en;
}

static inline void system_ll_set_cpu1_irda_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_irda_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_irda_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_irda_int_en;
}

static inline void system_ll_set_cpu1_sdio_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_sdio_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_sdio_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_sdio_int_en;
}

static inline void system_ll_set_cpu1_gdma_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_gdma_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_gdma_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_gdma_int_en;
}

static inline void system_ll_set_cpu1_la_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_la_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_la_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_la_int_en;
}

static inline void system_ll_set_cpu1_timer1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_timer1_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_timer1_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_timer1_int_en;
}

static inline void system_ll_set_cpu1_i2c1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_i2c1_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_i2c1_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_i2c1_int_en;
}

static inline void system_ll_set_cpu1_uart1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_uart1_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_uart1_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_uart1_int_en;
}

static inline void system_ll_set_cpu1_uart2_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_uart2_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_uart2_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_uart2_int_en;
}

static inline void system_ll_set_cpu1_spi1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_spi1_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_spi1_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_spi1_int_en;
}

static inline void system_ll_set_cpu1_can_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_can_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_can_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_can_int_en;
}

static inline void system_ll_set_cpu1_usb_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_usb_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_usb_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_usb_int_en;
}

static inline void system_ll_set_cpu1_qspi_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_qspi_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_qspi_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_qspi_int_en;
}

static inline void system_ll_set_cpu1_fft_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_fft_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_fft_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_fft_int_en;
}

static inline void system_ll_set_cpu1_sbc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_sbc_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_sbc_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_sbc_int_en;
}

static inline void system_ll_set_cpu1_aud_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_aud_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_aud_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_aud_int_en;
}

static inline void system_ll_set_cpu1_i2s_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_i2s_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_i2s_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_i2s_int_en;
}

static inline void system_ll_set_cpu1_jpegenc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_jpegenc_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_jpegenc_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_jpegenc_int_en;
}

static inline void system_ll_set_cpu1_jpegdec_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_jpegdec_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_jpegdec_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_jpegdec_int_en;
}

static inline void system_ll_set_cpu1_lcd_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_lcd_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_lcd_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_lcd_int_en;
}

static inline void system_ll_set_cpu1_wifi_int_phy_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_wifi_int_phy_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_int_phy_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_wifi_int_phy_en;
}

static inline void system_ll_set_cpu1_wifi_mac_int_timer_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_0_31_en.cpu1_wifi_mac_int_tx_rx_timer_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_timer_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_wifi_mac_int_tx_rx_timer_en;
}

static inline void system_ll_set_cpu1_wifi_mac_int_misc_en(system_hw_t *hw, uint32_t value)
{
hw->cpu1_int_0_31_en.cpu1_wifi_mac_int_tx_rx_misc_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_misc_en(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_en.cpu1_wifi_mac_int_tx_rx_misc_en;
}

//0x23 cpu1 int 32:63
static inline void system_ll_set_cpu1_wifi_mac_int_rx_trigger_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_rx_trigger_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_rx_trigger_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_rx_trigger_en;
}

static inline void system_ll_set_cpu1_wifi_mac_int_tx_trigger_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_tx_trigger_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_tx_trigger_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_tx_trigger_en;
}

static inline void system_ll_set_cpu1_wifi_mac_port_trigger_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_wifi_mac_port_trigger_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_port_trigger_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_wifi_mac_port_trigger_en;
}
static inline void system_ll_set_cpu1_wifi_mac_int_gen_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_gen_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_gen_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_gen_en;
}

static inline void system_ll_set_cpu1_wifi_hsu_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_wifi_hsu_irq_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_hsu_irq_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_wifi_hsu_irq_en;
}

static inline void system_ll_set_cpu1_wifi_int_mac_wakeup_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_wifi_int_mac_wakeup_en = value;
}
static inline uint32_t system_ll_get_cpu1_wifi_int_mac_wakeup_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_wifi_int_mac_wakeup_en;
}

static inline void system_ll_set_cpu1_dm_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_dm_irq_en = value;
}
static inline uint32_t system_ll_get_cpu1_dm_irq_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_dm_irq_en;
}

static inline void system_ll_set_cpu1_ble_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_ble_irq_en = value;
}
static inline uint32_t system_ll_get_cpu1_ble_irq_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_ble_irq_en;
}

static inline void system_ll_set_cpu1_bt_irq_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_bt_irq_en = value;
}
static inline uint32_t system_ll_get_cpu1_bt_irq_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_bt_irq_en;
}

static inline void system_ll_set_cpu1_mbox0_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_mbox0_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_mbox0_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_mbox0_int_en;
}

static inline void system_ll_set_cpu1_mbox1_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_mbox1_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_mbox1_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_mbox1_int_en;
}

static inline void system_ll_set_cpu1_bmc64_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_bmc64_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_bmc64_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_bmc64_int_en;
}

static inline void system_ll_set_cpu1_touched_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_touched_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_touched_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_touched_int_en;
}

static inline void system_ll_set_cpu1_usbplug_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_usbplug_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_usbplug_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_usbplug_int_en;
}

static inline void system_ll_set_cpu1_rtc_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_rtc_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_rtc_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_rtc_int_en;
}

static inline void system_ll_set_cpu1_gpio_int_en(system_hw_t *hw, uint32_t value)
{
	hw->cpu1_int_32_63_en.cpu1_gpio_int_en = value;
}
static inline uint32_t system_ll_get_cpu1_gpio_int_en(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_en.cpu1_gpio_int_en;
}

//reg0x28 cpu0 int 0:31
static inline uint32_t system_ll_get_cpu0_bmc32_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_bmc32_int_st;
}

static inline uint32_t system_ll_get_cpu0_host_0_irq_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_host_0_irq_st;
}

static inline uint32_t system_ll_get_cpu0_host_0_sec_irq_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_host_0_sec_irq_st;
}

static inline uint32_t system_ll_get_cpu0_timer_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_timer_int_st;
}

static inline uint32_t system_ll_get_cpu0_uart_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_uart_int_st;
}

static inline uint32_t system_ll_get_cpu0_pwm_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_pwm_int_st;
}

static inline uint32_t system_ll_get_cpu0_i2c_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_i2c_int_st;
}

static inline uint32_t system_ll_get_cpu0_spi_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_spi_int_st;
}

static inline uint32_t system_ll_get_cpu0_sadc_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_sadc_int_st;
}

static inline uint32_t system_ll_get_cpu0_irda_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_irda_int_st;
}

static inline uint32_t system_ll_get_cpu0_sdio_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_sdio_int_st;
}

static inline uint32_t system_ll_get_cpu0_gdma_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_gdma_int_st;
}

static inline uint32_t system_ll_get_cpu0_la_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_la_int_st;
}

static inline uint32_t system_ll_get_cpu0_timer1_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_timer1_int_st;
}

static inline uint32_t system_ll_get_cpu0_i2c1_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_i2c1_int_st;
}

static inline uint32_t system_ll_get_cpu0_uart1_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_uart1_int_st;
}

static inline uint32_t system_ll_get_cpu0_uart2_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_uart2_int_st;
}

static inline uint32_t system_ll_get_cpu0_spi1_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_spi1_int_st;
}

static inline uint32_t system_ll_get_cpu0_can_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_can_int_st;
}

static inline uint32_t system_ll_get_cpu0_usb_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_usb_int_st;
}

static inline uint32_t system_ll_get_cpu0_qspi_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_qspi_int_st;
}

static inline uint32_t system_ll_get_cpu0_fft_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_fft_int_st;
}

static inline uint32_t system_ll_get_cpu0_sbc_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_sbc_int_st;
}

static inline uint32_t system_ll_get_cpu0_aud_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_aud_int_st;
}

static inline uint32_t system_ll_get_cpu0_i2s_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_i2s_int_st;
}

static inline uint32_t system_ll_get_cpu0_jpegenc_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_jpegenc_int_st;
}

static inline uint32_t system_ll_get_cpu0_jpegdec_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_jpegdec_int_st;
}

static inline uint32_t system_ll_get_cpu0_lcd_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_lcd_int_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_int_phy_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_wifi_int_phy_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_mac_int_timer_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_wifi_mac_int_tx_rx_timer_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_mac_int_misc_st(system_hw_t *hw)
{
	return hw->cpu0_int_0_31_status.cpu0_wifi_mac_int_tx_rx_misc_st;
}

//0x29 cpu0 int 32:63
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_rx_trigger_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_rx_trigger_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_mac_int_tx_trigger_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_tx_trigger_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_mac_port_trigger_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_wifi_mac_port_trigger_st;
}
static inline uint32_t system_ll_get_cpu0_wifi_mac_int_gen_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_gen_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_hsu_irq_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_wifi_hsu_irq_st;
}

static inline uint32_t system_ll_get_cpu0_wifi_int_mac_wakeup_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_wifi_int_mac_wakeup_st;
}

static inline uint32_t system_ll_get_cpu0_dm_irq_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_dm_irq_st;
}

static inline uint32_t system_ll_get_cpu0_ble_irq_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_ble_irq_st;
}

static inline uint32_t system_ll_get_cpu0_bt_irq_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_bt_irq_st;
}

static inline uint32_t system_ll_get_cpu0_mbox0_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_mbox0_int_st;
}

static inline uint32_t system_ll_get_cpu0_mbox1_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_mbox1_int_st;
}

static inline uint32_t system_ll_get_cpu0_bmc64_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_bmc64_int_st;
}

static inline uint32_t system_ll_get_cpu0_touched_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_touched_int_st;
}

static inline uint32_t system_ll_get_cpu0_usbplug_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_usbplug_int_st;
}

static inline uint32_t system_ll_get_cpu0_rtc_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_rtc_int_st;
}

static inline uint32_t system_ll_get_cpu0_gpio_int_st(system_hw_t *hw)
{
	return hw->cpu0_int_32_63_status.cpu0_gpio_int_st;
}

//reg0x2a cpu1 int 0:31
static inline uint32_t system_ll_get_cpu1_bmc32_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_bmc32_int_st;
}

static inline uint32_t system_ll_get_cpu1_host_0_irq_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_host_0_irq_st;
}

static inline uint32_t system_ll_get_cpu1_host_0_sec_irq_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_host_0_sec_irq_st;
}

static inline uint32_t system_ll_get_cpu1_timer_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_timer_int_st;
}

static inline uint32_t system_ll_get_cpu1_uart_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_uart_int_st;
}

static inline uint32_t system_ll_get_cpu1_pwm_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_pwm_int_st;
}

static inline uint32_t system_ll_get_cpu1_i2c_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_i2c_int_st;
}

static inline uint32_t system_ll_get_cpu1_spi_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_spi_int_st;
}

static inline uint32_t system_ll_get_cpu1_sadc_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_sadc_int_st;
}

static inline uint32_t system_ll_get_cpu1_irda_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_irda_int_st;
}

static inline uint32_t system_ll_get_cpu1_sdio_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_sdio_int_st;
}

static inline uint32_t system_ll_get_cpu1_gdma_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_gdma_int_st;
}

static inline uint32_t system_ll_get_cpu1_la_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_la_int_st;
}

static inline uint32_t system_ll_get_cpu1_timer1_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_timer1_int_st;
}

static inline uint32_t system_ll_get_cpu1_i2c1_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_i2c1_int_st;
}

static inline uint32_t system_ll_get_cpu1_uart1_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_uart1_int_st;
}

static inline uint32_t system_ll_get_cpu1_uart2_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_uart2_int_st;
}

static inline uint32_t system_ll_get_cpu1_spi1_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_spi1_int_st;
}

static inline uint32_t system_ll_get_cpu1_can_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_can_int_st;
}

static inline uint32_t system_ll_get_cpu1_usb_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_usb_int_st;
}

static inline uint32_t system_ll_get_cpu1_qspi_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_qspi_int_st;
}

static inline uint32_t system_ll_get_cpu1_fft_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_fft_int_st;
}

static inline uint32_t system_ll_get_cpu1_sbc_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_sbc_int_st;
}

static inline uint32_t system_ll_get_cpu1_aud_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_aud_int_st;
}

static inline uint32_t system_ll_get_cpu1_i2s_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_i2s_int_st;
}

static inline uint32_t system_ll_get_cpu1_jpegenc_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_jpegenc_int_st;
}

static inline uint32_t system_ll_get_cpu1_jpegdec_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_jpegdec_int_st;
}

static inline uint32_t system_ll_get_cpu1_lcd_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_lcd_int_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_int_phy_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_wifi_int_phy_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_mac_int_timer_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_wifi_mac_int_tx_rx_timer_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_mac_int_misc_st(system_hw_t *hw)
{
	return hw->cpu1_int_0_31_status.cpu1_wifi_mac_int_tx_rx_misc_st;
}

//0x2b cpu1 int 32:63
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_rx_trigger_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_rx_trigger_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_mac_int_tx_trigger_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_tx_trigger_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_mac_port_trigger_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_wifi_mac_port_trigger_st;
}
static inline uint32_t system_ll_get_cpu1_wifi_mac_int_gen_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_gen_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_hsu_irq_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_wifi_hsu_irq_st;
}

static inline uint32_t system_ll_get_cpu1_wifi_int_mac_wakeup_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_wifi_int_mac_wakeup_st;
}

static inline uint32_t system_ll_get_cpu1_dm_irq_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_dm_irq_st;
}

static inline uint32_t system_ll_get_cpu1_ble_irq_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_ble_irq_st;
}

static inline uint32_t system_ll_get_cpu1_bt_irq_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_bt_irq_st;
}

static inline uint32_t system_ll_get_cpu1_mbox0_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_mbox0_int_st;
}

static inline uint32_t system_ll_get_cpu1_mbox1_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_mbox1_int_st;
}

static inline uint32_t system_ll_get_cpu1_bmc64_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_bmc64_int_st;
}

static inline uint32_t system_ll_get_cpu1_touched_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_touched_int_st;
}

static inline uint32_t system_ll_get_cpu1_usbplug_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_usbplug_int_st;
}

static inline uint32_t system_ll_get_cpu1_rtc_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_rtc_int_st;
}

static inline uint32_t system_ll_get_cpu1_gpio_int_st(system_hw_t *hw)
{
	return hw->cpu1_int_32_63_status.cpu1_gpio_int_st;
}

// REG_0x30 0x31 0x32 0x33 0x34 0x35
static inline void system_ll_set_gpio0_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio0 = value;
}
static inline uint32 system_ll_get_gpio0_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio0;
}

static inline void system_ll_set_gpio1_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio1 = value;
}
static inline uint32 system_ll_get_gpio1_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio1;
}

static inline void system_ll_set_gpio2_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio2 = value;
}
static inline uint32 system_ll_get_gpio2_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio2;
}

static inline void system_ll_set_gpio3_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio3 = value;
}
static inline uint32 system_ll_get_gpio3_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio3;
}

static inline void system_ll_set_gpio4_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio4 = value;
}
static inline uint32 system_ll_get_gpio4_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio4;
}

static inline void system_ll_set_gpio5_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio5 = value;
}
static inline uint32 system_ll_get_gpio5_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio5;
}

static inline void system_ll_set_gpio6_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio6 = value;
}
static inline uint32 system_ll_get_gpio6_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio6;
}

static inline void system_ll_set_gpio7_perial_mode(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_gpio_config[group].system_gpio7 = value;
}
static inline uint32 system_ll_get_gpio7_perial_mode(system_hw_t *hw, uint32 group)
{
	return hw->system_gpio_config[group].system_gpio7;
}

//0x38
static inline void system_ll_set_dbug_cfg0(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_debug_config0.dbug_cfg0 = value;
}
static inline uint32 system_ll_get_dbug_cfg0(system_hw_t *hw, uint32 group)
{
	return hw->system_debug_config0.dbug_cfg0;
}

//0x39
static inline void system_ll_set_dbug_cfg1(system_hw_t *hw, uint32 group, uint32_t value)
{
	hw->system_debug_config1.dbug_cfg1 = value;
}
static inline uint32 system_ll_get_dbug_cfg1(system_hw_t *hw, uint32 group)
{
	return hw->system_debug_config1.dbug_cfg1;
}

#ifdef __cplusplus
}
#endif

