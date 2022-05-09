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
#include "system_hal.h"

static system_hal_t s_system;

extern bk_err_t system_hal_init(system_hal_t *hal);

void system_driver_init()
{
	system_hal_init(&s_system);
}

//0x0
uint32_t system_driver_get_device_id()
{
	return system_hal_get_device_id(&s_system);
}

//0x01
uint32_t system_driver_get_version_id()
{
	return system_hal_get_version_id(&s_system);
}

//0x02
uint32_t system_driver_get_core0_halted_st()
{
	return system_hal_get_core0_halted_st(&s_system);
}
uint32_t system_driver_get_core1_halted_st()
{
	return system_hal_get_core1_halted_st(&s_system);
}
uint32_t system_driver_get_cpu0_sw_reset_st()
{
	return system_hal_get_core1_halted_st(&s_system);
}
uint32_t systesm_driver_get_cpu1_sw_reset_st()
{
	return system_hal_get_cpu1_sw_reset_st(&s_system);
}
uint32_t system_driver_get_cpu0_pwr_dw_st()
{
	return system_hal_get_cpu0_pwr_dw_st(&s_system);
}
uint32_t system_driver_get_cpu1_pwr_dw_st()
{
	return system_hal_get_cpu1_pwr_dw_st(&s_system);
}

//0x03
void system_driver_set_boot_rom_mode()
{
	system_hal_set_boot_rom_mode(&s_system);

}
void system_driver_set_boot_flash_mode()
{
	system_hal_set_boot_flash_mode(&s_system);
}
uint32_t system_driver_get_boot_mode()
{
	return system_hal_get_boot_mode(&s_system);
}

void system_driver_set_jtag_connect_core0()
{
	system_hal_set_jtag_connect_core0(&s_system);
}
void system_driver_set_jtag_connect_core1()
{
	system_hal_set_jtag_connect_core1(&s_system);
}
uint32_t system_driver_get_jtag_connect_core()
{
	return system_hal_get_jtag_connect_core(&s_system);
}

void system_driver_set_flash_select_normal_op()
{
	system_hal_set_flash_select_normal_op(&s_system);
}
void system_driver_set_flash_select_spi_op()
{
	system_hal_set_flash_select_spi_op(&s_system);
}
uint32_t system_driver_get_flash_select_op()
{
	return system_hal_get_flash_select_op(&s_system);
}

//0x04
void system_driver_set_cpu0_int_mask(bool value)
{
	if(value)
		system_hal_set_cpu0_int_mask(&s_system, 1);
	else
		system_hal_set_cpu0_int_mask(&s_system, 0);

}
uint32_t system_driver_get_cpu0_int_mask()
{
	return system_hal_get_cpu0_int_mask(&s_system);
}

void system_driver_set_core0_halt_indicate(bool value)
{
	if(value)
		system_hal_set_core0_halt_indicate(&s_system, 1);
	else
		system_hal_set_core0_halt_indicate(&s_system, 0);
}
uint32_t system_driver_get_core0_halt_indicate()
{
	return system_hal_get_core0_halt_indicate(&s_system);
}

void system_driver_set_cpu0_clk_div(bool value)
{
	if(value)
		system_hal_set_cpu0_clk_div(&s_system, 1);
	else
		system_hal_set_cpu0_clk_div(&s_system, 0);

}
uint32_t system_driver_get_cpu0_clk_div()
{
	return system_hal_get_cpu0_clk_div(&s_system);
}


//0x05
void system_driver_set_cpu1_int_mask(bool value)
{
	if(value)
		system_hal_set_cpu1_int_mask(&s_system, 1);
	else
		system_hal_set_cpu1_int_mask(&s_system, 0);

}
uint32_t system_driver_get_cpu1_int_mask()
{
	return system_hal_get_cpu1_int_mask(&s_system);
}

void system_driver_set_core1_halt_indicate(bool value)
{
	if(value)
		system_hal_set_core1_halt_indicate(&s_system, 1);
	else
		system_hal_set_core1_halt_indicate(&s_system, 0);
}
uint32_t system_driver_get_core1_halt_indicate()
{
	return system_hal_get_cpu1_clk_div(&s_system);
}

void system_driver_set_cpu1_clk_div(uint32_t value)
{
	system_hal_set_core_clk_div(&s_system, value);
}
uint32_t system_driver_get_cpu1_clk_div()
{
	return system_hal_get_core_clk_div(&s_system);
}


//0x08
void system_driver_set_core_clk_div(bool value)
{
	system_hal_set_core_clk_sel(&s_system, value);
}
uint32_t system_driver_get_core_clk_div()
{
	return system_hal_get_core_clk_sel(&s_system);
}

void system_driver_set_core_clk_sel(bool value)
{
	if(value)
		system_hal_set_bus_clk_div(&s_system, 1);
	else
		system_hal_set_bus_clk_div(&s_system, 0);
}
uint32_t system_driver_get_core_clk_sel()
{
	return system_hal_get_core_clk_sel(&s_system);
}

void system_driver_set_bus_clk_div(bool value)
{
	if(value)
		system_hal_set_bus_clk_div(&s_system, 1);
	else
		system_hal_set_bus_clk_div(&s_system, 0);
}
uint32_t system_driver_get_bus_clk_div()
{
	return system_hal_get_bus_clk_div(&s_system);
}

void system_driver_set_uart0_clk_div(uint32_t value)
{
	if(value)
		system_hal_set_uart0_clk_div(&s_system, value);
}
uint32_t system_driver_get_uart0_clk_div()
{
	return system_hal_get_uart0_clk_div(&s_system);
}
void system_driver_set_uart0_clk_sel(bool value)
{
	if(value)
		system_hal_set_uart0_clk_sel(&s_system, 1);
	else
		system_hal_set_uart0_clk_sel(&s_system, 0);

}
uint32_t system_driver_get_uart0_clk_sel()
{
	return system_hal_get_uart0_clk_sel(&s_system);
}

void system_driver_set_uart1_clk_div(uint32_t value)
{
	if(value)
		system_hal_set_uart1_clk_div(&s_system, value);

}
uint32_t system_driver_get_uart1_clk_div()
{
	return system_hal_get_uart1_clk_div(&s_system);
}
void system_driver_set_uart1_clk_sel(bool value)
{
	if(value)
		system_hal_set_uart1_clk_sel(&s_system, 1);
	else
		system_hal_set_uart1_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_uart1_clk_sel()
{
	return system_hal_get_uart1_clk_sel(&s_system);
}

void system_driver_set_uart2_clk_div(bool value)
{
	if(value)
		system_hal_set_uart2_clk_div(&s_system, value);
}
uint32_t system_driver_get_uart2_clk_div()
{
	return system_hal_get_uart2_clk_div(&s_system);
}

void system_driver_set_uart2_clk_sel(bool value)
{
	if(value)
		system_hal_set_uart2_clk_sel(&s_system, 1);
	else
		system_hal_set_uart2_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_uart2_clk_sel()
{
	return system_hal_get_uart2_clk_sel(&s_system);
}

void system_driver_set_sadc_clk_sel(bool value)
{
	if(value)
		system_hal_set_sadc_clk_sel(&s_system, 1);
	else
		system_hal_set_sadc_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_sadc_clk_sel()
{
	return system_hal_get_sadc_clk_sel(&s_system);
}

void system_driver_set_pwm0_clk_sel(bool value)
{
	if(value)
		system_hal_set_pwm0_clk_sel(&s_system, 1);
	else
		system_hal_set_pwm0_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_pwm0_clk_sel()
{
	return system_hal_get_pwm0_clk_sel(&s_system);
}

void system_driver_set_pwm1_clk_sel(bool value)
{
	if(value)
		system_hal_set_pwm1_clk_sel(&s_system, 1);
	else
		system_hal_set_pwm1_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_pwm1_clk_sel()
{
	return system_hal_get_pwm1_clk_sel(&s_system);
}

void system_driver_set_timer0_clk_sel(bool value)
{
	if(value)
		system_hal_set_timer0_clk_sel(&s_system, 1);
	else
		system_hal_set_timer0_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_timer0_clk_sel()
{
	return system_hal_get_timer0_clk_sel(&s_system);
}

void system_driver_set_timer1_clk_sel(bool value)
{
	if(value)
		system_hal_set_timer1_clk_sel(&s_system, 1);
	else
		system_hal_set_timer1_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_timer1_clk_sel()
{
	return system_hal_get_timer1_clk_sel(&s_system);
}
void system_driver_set_timer2_clk_sel(bool value)
{
	if(value)
		system_hal_set_timer2_clk_sel(&s_system, 1);
	else
		system_hal_set_timer2_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_timer2_clk_sel()
{
	return system_hal_get_timer2_clk_sel(&s_system);
}

void system_driver_set_i2s_clk_sel(bool value)
{
	if(value)
		system_hal_set_i2s_clk_sel(&s_system, 1);
	else
		system_hal_set_i2s_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_i2s_clk_sel()
{
	return system_hal_get_i2s_clk_sel(&s_system);
}

void system_driver_set_aud_clk_sel(bool value)
{
	if(value)
		system_hal_set_aud_clk_sel(&s_system, 1);
	else
		system_hal_set_aud_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_aud_clk_sel()
{
	return system_hal_get_aud_clk_sel(&s_system);
}
void system_driver_set_jpeg_clk_div(uint32_t value)
{
	system_hal_set_jpeg_clk_div(&s_system, value);
}
uint32_t system_driver_get_jpeg_clk_div()
{
	return system_hal_get_jpeg_clk_div(&s_system);
}
void system_driver_set_jpeg_clk_sel(bool value)
{
	if(value)
		system_hal_set_jpeg_clk_sel(&s_system, 1);
	else
		system_hal_set_jpeg_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_jpeg_clk_sel()
{
	return system_hal_get_jpeg_clk_sel(&s_system);
}

void system_driver_set_disp_l_clk_div(uint32_t value)
{
	system_hal_set_disp_l_clk_div(&s_system, value);
}
uint32_t system_driver_get_disp_l_clk_div()
{
	return system_hal_get_disp_l_clk_div(&s_system);
}

//0x09
void system_driver_set_disp_h_clk_div(bool value)
{
	system_hal_set_disp_h_clk_div(&s_system, value);
}
uint32_t system_driver_get_disp_h_clk_div()
{
	return system_hal_get_disp_h_clk_div(&s_system);
}
void system_driver_set_disp_clk_sel(bool value)
{
	if(value)
		system_hal_set_disp_clk_sel(&s_system, 1);
	else
		system_hal_set_disp_clk_sel(&s_system, 0);
}

uint32_t system_driver_get_disp_clk_sel()
{
	return system_hal_get_disp_clk_sel(&s_system);
}

void system_driver_set_psram_clk_div(uint32_t value)
{
	system_hal_set_psram_clk_div(&s_system, value);
}
uint32_t system_driver_get_psram_clk_div()
{
	return system_hal_get_psram_clk_div(&s_system);
}
void system_driver_set_psram_clk_sel(bool value)
{
	if(value)
		system_hal_set_psram_clk_sel(&s_system, 1);
	else
		system_hal_set_psram_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_psram_clk_sel()
{
	return system_hal_get_psram_clk_sel(&s_system);
}

void system_driver_set_qspi0_clk_div(uint32_t value)
{
	system_hal_set_qspi0_clk_div(&s_system, value);
}
uint32_t system_driver_get_qspi0_clk_div()
{
	return system_hal_get_qspi0_clk_div(&s_system);
}

void system_driver_set_sdio_clk_div(uint32_t value)
{
	system_hal_set_sdio_clk_div(&s_system, value);
}
uint32_t system_driver_get_sdio_clk_div()
{
	return system_hal_get_sdio_clk_div(&s_system);
}
void system_driver_set_sdio_clk_sel(bool value)
{
	if(value)
		system_hal_set_sdio_clk_sel(&s_system, 1);
	else
		system_hal_set_sdio_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_sdio_clk_sel()
{
	return system_hal_get_sdio_clk_sel(&s_system);
}

void system_driver_set_auxs_clk_div(uint32_t value)
{
	system_hal_set_auxs_clk_div(&s_system, value);
}
uint32_t system_driver_get_auxs_clk_div()
{
	return system_hal_get_auxs_clk_div(&s_system);
}
void system_driver_set_auxs_clk_sel(bool value)
{
	if(value)
		system_hal_set_auxs_clk_sel(&s_system, 1);
	else
		system_hal_set_auxs_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_auxs_clk_sel()
{
	return system_hal_get_auxs_clk_sel(&s_system);
}

void system_driver_set_flash_clk_sel(bool value)
{
	if(value)
		system_hal_set_flash_clk_sel(&s_system, 1);
	else
		system_hal_set_flash_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_flash_clk_sel()
{
	return system_hal_get_flash_clk_sel(&s_system);
}
void system_driver_set_flash_clk_div(uint32_t value)
{
	system_hal_set_flash_clk_div(&s_system, value);
}
uint32_t system_driver_get_flash_clk_div()
{
	return system_hal_get_flash_clk_div(&s_system);
}

void system_driver_set_i2s0_clk_div(uint32_t value)
{
	system_hal_set_i2s0_clk_div(&s_system, value);
}
uint32_t system_driver_get_i2s0_clk_div()
{
	return system_hal_get_i2s0_clk_div(&s_system);
}

//0x0a
void system_driver_set_26m_clk_div(uint32_t value)
{
	system_hal_set_26m_clk_div(&s_system, value);
}
uint32_t system_driver_get_26m_clk_div()
{
	return system_hal_get_26m_clk_div(&s_system);
}
void system_driver_set_wdt_clk_div(uint32_t value)
{
	system_hal_set_wdt_clk_div(&s_system, value);
}
uint32_t system_driver_get_wdt_clk_div()
{
	return system_hal_get_wdt_clk_div(&s_system);
}

//0x0b
void system_driver_set_anaspi_freq(uint32_t value)
{
	system_hal_set_anaspi_freq(&s_system, value);
}
uint32_t system_driver_get_anaspi_freq()
{
	return system_hal_get_anaspi_freq(&s_system);
}

//0x0c
void system_driver_set_i2c0_clk_en(bool value)
{
	if(value)
		system_hal_set_i2c0_clk_en(&s_system, 1);
	else
		system_hal_set_i2c0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_i2c0_clk_en()
{
	return system_hal_get_i2c0_clk_en(&s_system);
}

void system_driver_set_spi0_clk_en(bool value)
{
	if(value)
		system_hal_set_spi0_clk_en(&s_system, 1);
	else
		system_hal_set_spi0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_spi0_clk_en()
{
	return system_hal_get_spi0_clk_en(&s_system);
}

void system_driver_set_uart0_clk_en(bool value)
{
	if(value)
		system_hal_set_uart0_clk_en(&s_system, 1);
	else
		system_hal_set_uart0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_uart0_clk_en()
{
	return system_hal_get_uart0_clk_en(&s_system);
}

void system_driver_set_pwm0_clk_en(bool value)
{
	if(value)
		system_hal_set_pwm0_clk_en(&s_system, 1);
	else
		system_hal_set_pwm0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_pwm0_clk_en()
{
	return system_hal_get_pwm0_clk_en(&s_system);
}

void system_driver_set_timer0_clk_en(bool value)
{
	if(value)
		system_hal_set_timer0_clk_en(&s_system, 1);
	else
		system_hal_set_timer0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_timer0_clk_en()
{
	return system_hal_get_timer0_clk_en(&s_system);
}

void system_driver_set_sadc_clk_en(bool value)
{
	if(value)
		system_hal_set_sadc_clk_en(&s_system, 1);
	else
		system_hal_set_sadc_clk_en(&s_system, 0);
}
uint32_t system_driver_get_sadc_clk_en()
{
	return system_hal_get_sadc_clk_en(&s_system);
}

void system_driver_set_irda_clk_en(bool value)
{
	if(value)
		system_hal_set_irda_clk_en(&s_system, 1);
	else
		system_hal_set_irda_clk_en(&s_system, 0);
}
uint32_t system_driver_get_irda_clk_en()
{
	return system_hal_get_irda_clk_en(&s_system);
}
void system_driver_set_efuse_clk_en(bool value)
{
	if(value)
		system_hal_set_efuse_clk_en(&s_system, 1);
	else
		system_hal_set_efuse_clk_en(&s_system, 0);
}
uint32_t system_driver_get_efuse_clk_en()
{
	return system_hal_get_efuse_clk_en(&s_system);
}

void system_driver_set_i2c1_clk_en(bool value)
{
	if(value)
		system_hal_set_i2c1_clk_en(&s_system, 1);
	else
		system_hal_set_i2c1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_i2c1_clk_en()
{
	return system_hal_get_i2c1_clk_en(&s_system);
}

void system_driver_set_spi1_clk_en(bool value)
{
	if(value)
		system_hal_set_spi1_clk_en(&s_system, 1);
	else
		system_hal_set_spi1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_spi1_clk_en()
{
	return system_hal_get_spi1_clk_en(&s_system);
}

void system_driver_set_uart1_clk_en(bool value)
{
	if(value)
		system_hal_set_uart1_clk_en(&s_system, 1);
	else
		system_hal_set_uart1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_uart1_clk_en()
{
	return system_hal_get_uart1_clk_en(&s_system);
}

void system_driver_set_uart2_clk_en(bool value)
{
	if(value)
		system_hal_set_uart2_clk_en(&s_system, 1);
	else
		system_hal_set_uart2_clk_en(&s_system, 0);
}
uint32_t system_driver_get_uart2_clk_en()
{
	return system_hal_get_uart2_clk_en(&s_system);
}
void system_driver_set_pwm1_clk_en(bool value)
{
	if(value)
		system_hal_set_pwm1_clk_en(&s_system, 1);
	else
		system_hal_set_pwm1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_pwm1_clk_en()
{
	return system_hal_get_pwm1_clk_en(&s_system);
}

void system_driver_set_timer1_clk_en(bool value)
{
	if(value)
		system_hal_set_timer1_clk_en(&s_system, 1);
	else
		system_hal_set_timer1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_timer1_clk_en()
{
	return system_hal_get_timer1_clk_en(&s_system);
}

void system_driver_set_timer2_clk_en(bool value)
{
	if(value)
		system_hal_set_timer2_clk_en(&s_system, 1);
	else
		system_hal_set_timer2_clk_en(&s_system, 0);
}
uint32_t system_driver_get_timer2_clk_en()
{
	return system_hal_get_timer2_clk_en(&s_system);
}

void system_driver_set_otp_clk_en(bool value)
{
	if(value)
		system_hal_set_otp_clk_en(&s_system, 1);
	else
		system_hal_set_otp_clk_en(&s_system, 0);
}
uint32_t system_driver_get_otp_clk_en()
{
	return system_hal_get_otp_clk_en(&s_system);
}

void system_driver_set_i2s_clk_en(bool value)
{
	if(value)
		system_hal_set_i2s_clk_en(&s_system, 1);
	else
		system_hal_set_i2s_clk_en(&s_system, 0);
}
uint32_t system_driver_get_i2s_clk_en()
{
	return system_hal_get_i2s_clk_en(&s_system);
}
void system_driver_set_usb_clk_en(bool value)
{
	if(value)
		system_hal_set_usb_clk_en(&s_system, 1);
	else
		system_hal_set_usb_clk_en(&s_system, 0);
}
uint32_t system_driver_get_usb_clk_en()
{
	return system_hal_get_usb_clk_en(&s_system);
}

void system_driver_set_can_clk_en(bool value)
{
	if(value)
		system_hal_set_can_clk_en(&s_system, 1);
	else
		system_hal_set_can_clk_en(&s_system, 0);
}
uint32_t system_driver_get_can_clk_en()
{
	return system_hal_get_can_clk_en(&s_system);
}

void system_driver_set_psram_clk_en(bool value)
{
	if(value)
		system_hal_set_psram_clk_en(&s_system, 1);
	else
		system_hal_set_psram_clk_en(&s_system, 0);
}
uint32_t system_driver_get_psram_clk_en()
{
	return system_hal_get_psram_clk_en(&s_system);
}

void system_driver_set_qspi0_clk_en(bool value)
{
	if(value)
		system_hal_set_qspi0_clk_en(&s_system, 1);
	else
		system_hal_set_qspi0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_qspi0_clk_en()
{
	return system_hal_get_qspi0_clk_en(&s_system);
}

void system_driver_set_qspi1_clk_en(bool value)
{
	if(value)
		system_hal_set_qspi1_clk_en(&s_system, 1);
	else
		system_hal_set_qspi1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_qspi1_clk_en()
{
	return system_hal_get_qspi1_clk_en(&s_system);
}
void system_driver_set_sdio_clk_en(bool value)
{
	if(value)
		system_hal_set_sdio_clk_en(&s_system, 1);
	else
		system_hal_set_sdio_clk_en(&s_system, 0);
}
uint32_t system_driver_get_sdio_clk_en()
{
	return system_hal_get_sdio_clk_en(&s_system);
}

void system_driver_set_auxs_clk_en(bool value)
{
	if(value)
		system_hal_set_auxs_clk_en(&s_system, 1);
	else
		system_hal_set_auxs_clk_en(&s_system, 0);
}
uint32_t system_driver_get_auxs_clk_en()
{
	return system_hal_get_auxs_clk_en(&s_system);
}

void system_driver_set_btdm_clk_en(bool value)
{
	if(value)
		system_hal_set_btdm_clk_en(&s_system, 1);
	else
		system_hal_set_btdm_clk_en(&s_system, 0);
}
uint32_t system_driver_get_btdm_clk_en()
{
	return system_hal_get_btdm_clk_en(&s_system);
}

void system_driver_set_xvr_clk_en(bool value)
{
	if(value)
		system_hal_set_xvr_clk_en(&s_system, 1);
	else
		system_hal_set_xvr_clk_en(&s_system, 0);
}
uint32_t system_driver_get_xvr_clk_en()
{
	return system_hal_get_xvr_clk_en(&s_system);
}
void system_driver_set_mac_clk_en(bool value)
{
	if(value)
		system_hal_set_mac_clk_en(&s_system, 1);
	else
		system_hal_set_mac_clk_en(&s_system, 0);
}
uint32_t system_driver_get_mac_clk_en()
{
	return system_hal_get_mac_clk_en(&s_system);
}

void system_driver_set_phy_clk_en(bool value)
{
	if(value)
		system_hal_set_phy_clk_en(&s_system, 1);
	else
		system_hal_set_phy_clk_en(&s_system, 0);
}
uint32_t system_driver_get_phy_clk_en()
{
	return system_hal_get_phy_clk_en(&s_system);
}

void system_driver_set_jpeg_clk_en(bool value)
{
	if(value)
		system_hal_set_jpeg_clk_en(&s_system, 1);
	else
		system_hal_set_jpeg_clk_en(&s_system, 0);
}
uint32_t system_driver_get_jpeg_clk_en()
{
	return system_hal_get_jpeg_clk_en(&s_system);
}

void system_driver_set_disp_clk_en(bool value)
{
	if(value)
		system_hal_set_disp_clk_en(&s_system, 1);
	else
		system_hal_set_disp_clk_en(&s_system, 0);
}
uint32_t system_driver_get_disp_clk_en()
{
	return system_hal_get_disp_clk_en(&s_system);
}

void system_driver_set_aud_clk_en(bool value)
{
	if(value)
		system_hal_set_aud_clk_en(&s_system, 1);
	else
		system_hal_set_aud_clk_en(&s_system, 0);
}
uint32_t system_driver_get_aud_clk_en()
{
	return system_hal_get_aud_clk_en(&s_system);
}

void system_driver_set_wdt_clk_en(bool value)
{
	if(value)
		system_hal_set_wdt_clk_en(&s_system, 1);
	else
		system_hal_set_wdt_clk_en(&s_system, 0);
}
uint32_t system_driver_get_wdt_clk_en()
{
	return system_hal_get_wdt_clk_en(&s_system);
}


//0x0e
void system_driver_set_aon_disckg(bool value)
{
	if(value)
		system_hal_set_aon_disckg(&s_system, 1);
	else
		system_hal_set_aon_disckg(&s_system, 0);
}
uint32_t system_driver_get_aon_disckg()
{
	return system_hal_get_aon_disckg(&s_system);
}

void system_driver_set_sys_disckg(bool value)
{
	if(value)
		system_hal_set_sys_disckg(&s_system, 1);
	else
		system_hal_set_sys_disckg(&s_system, 0);
}
uint32_t system_driver_get_sys_disckg()
{
	return system_hal_get_sys_disckg(&s_system);
}

void system_driver_set_dma_disckg(bool value)
{
	if(value)
		system_hal_set_dma_disckg(&s_system, 1);
	else
		system_hal_set_dma_disckg(&s_system, 0);
}
uint32_t system_driver_get_dma_disckg()
{
	return system_hal_get_dma_disckg(&s_system);
}

void system_driver_set_flash_disckg(bool value)
{
	if(value)
		system_hal_set_flash_disckg(&s_system, 1);
	else
		system_hal_set_flash_disckg(&s_system, 0);
}
uint32_t system_driver_get_flash_disckg()
{
	return system_hal_get_flash_disckg(&s_system);
}

void system_driver_set_wdt_disckg(bool value)
{
	if(value)
		system_hal_set_wdt_disckg(&s_system, 1);
	else
		system_hal_set_wdt_disckg(&s_system, 0);
}
uint32_t system_driver_get_wdt_disckg()
{
	return system_hal_get_wdt_disckg(&s_system);
}
void system_driver_set_timer0_disckg(bool value)
{
	if(value)
		system_hal_set_timer0_disckg(&s_system, 1);
	else
		system_hal_set_timer0_disckg(&s_system, 0);
}
uint32_t system_driver_get_timer0_disckg()
{
	return system_hal_get_timer0_disckg(&s_system);
}
void system_driver_set_uart0_disckg(bool value)
{
	if(value)
		system_hal_set_uart0_disckg(&s_system, 1);
	else
		system_hal_set_uart0_disckg(&s_system, 0);
}
uint32_t system_driver_get_uart0_disckg()
{
	return system_hal_get_uart0_disckg(&s_system);
}

void system_driver_set_pwm0_disckg(bool value)
{
	if(value)
		system_hal_set_pwm0_disckg(&s_system, 1);
	else
		system_hal_set_pwm0_disckg(&s_system, 0);
}
uint32_t system_driver_get_pwm0_disckg()
{
	return system_hal_get_pwm0_disckg(&s_system);
}

void system_driver_set_i2c0_disckg(bool value)
{
	if(value)
		system_hal_set_i2c0_disckg(&s_system, 1);
	else
		system_hal_set_i2c0_disckg(&s_system, 0);
}
uint32_t system_driver_get_i2c0_disckg()
{
	return system_hal_get_i2c0_disckg(&s_system);
}

void system_driver_set_spi0_disckg(bool value)
{
	if(value)
		system_hal_set_spi0_disckg(&s_system, 1);
	else
		system_hal_set_spi0_disckg(&s_system, 0);
}
uint32_t system_driver_get_spi0_disckg()
{
	return system_hal_get_spi0_disckg(&s_system);
}
void system_driver_set_sadc_disckg(bool value)
{
	if(value)
		system_hal_set_sadc_disckg(&s_system, 1);
	else
		system_hal_set_sadc_disckg(&s_system, 0);
}
uint32_t system_driver_get_sadc_disckg()
{
	return system_hal_get_sadc_disckg(&s_system);
}

void system_driver_set_efuse_disckg(bool value)
{
	if(value)
		system_hal_set_efuse_disckg(&s_system, 1);
	else
		system_hal_set_efuse_disckg(&s_system, 0);
}
uint32_t system_driver_get_efuse_disckg()
{
	return system_hal_get_efuse_disckg(&s_system);
}

void system_driver_set_irda_disckg(bool value)
{
	if(value)
		system_hal_set_irda_disckg(&s_system, 1);
	else
		system_hal_set_irda_disckg(&s_system, 0);
}
uint32_t system_driver_get_irda_disckg()
{
	return system_hal_get_irda_disckg(&s_system);
}

void system_driver_set_trng_disckg(bool value)
{
	if(value)
		system_hal_set_trng_disckg(&s_system, 1);
	else
		system_hal_set_trng_disckg(&s_system, 0);
}
uint32_t system_driver_get_trng_disckg()
{
	return system_hal_get_trng_disckg(&s_system);
}

void system_driver_set_sdio_disckg(bool value)
{
	if(value)
		system_hal_set_sdio_disckg(&s_system, 1);
	else
		system_hal_set_sdio_disckg(&s_system, 0);
}
uint32_t system_driver_get_sdio_disckg()
{
	return system_hal_get_sdio_disckg(&s_system);
}

void system_driver_set_la_disckg(bool value)
{
	if(value)
		system_hal_set_la_disckg(&s_system, 1);
	else
		system_hal_set_la_disckg(&s_system, 0);
}
uint32_t system_driver_get_la_disckg()
{
	return system_hal_get_la_disckg(&s_system);
}

void system_driver_set_timer1_disckg(bool value)
{
	if(value)
		system_hal_set_timer1_disckg(&s_system, 1);
	else
		system_hal_set_timer1_disckg(&s_system, 0);
}
uint32_t system_driver_get_timer1_disckg()
{
	return system_hal_get_timer1_disckg(&s_system);
}

void system_driver_set_uart1_disckg(bool value)
{
	if(value)
		system_hal_set_uart1_disckg(&s_system, 1);
	else
		system_hal_set_uart1_disckg(&s_system, 0);
}
uint32_t system_driver_get_uart1_disckg()
{
	return system_hal_get_uart1_disckg(&s_system);
}

void system_driver_set_uart2_disckg(bool value)
{
	if(value)
		system_hal_set_uart2_disckg(&s_system, 1);
	else
		system_hal_set_uart2_disckg(&s_system, 0);
}
uint32_t system_driver_get_uart2_disckg()
{
	return system_hal_get_uart2_disckg(&s_system);
}

void system_driver_set_pwm1_disckg(bool value)
{
	if(value)
		system_hal_set_pwm1_disckg(&s_system, 1);
	else
		system_hal_set_pwm1_disckg(&s_system, 0);
}
uint32_t system_driver_get_pwm1_disckg()
{
	return system_hal_get_pwm1_disckg(&s_system);
}

void system_driver_set_i2c1_disckg(bool value)
{
	if(value)
		system_hal_set_i2c1_disckg(&s_system, 1);
	else
		system_hal_set_i2c1_disckg(&s_system, 0);
}
uint32_t system_driver_get_i2c1_disckg()
{
	return system_hal_get_i2c1_disckg(&s_system);
}

void system_driver_set_spi1_disckg(bool value)
{
	if(value)
		system_hal_set_spi1_disckg(&s_system, 1);
	else
		system_hal_set_spi1_disckg(&s_system, 0);
}
uint32_t system_driver_get_spi1_disckg()
{
	return system_hal_get_spi1_disckg(&s_system);
}

void system_driver_set_usb_disckg(bool value)
{
	if(value)
		system_hal_set_usb_disckg(&s_system, 1);
	else
		system_hal_set_usb_disckg(&s_system, 0);
}
uint32_t system_driver_get_usb_disckg()
{
	return system_hal_get_usb_disckg(&s_system);
}

void system_driver_set_can_disckg(bool value)
{
	if(value)
		system_hal_set_can_disckg(&s_system, 1);
	else
		system_hal_set_can_disckg(&s_system, 0);
}
uint32_t system_driver_get_can_disckg()
{
	return system_hal_get_can_disckg(&s_system);
}

void system_driver_set_qspi0_disckg(bool value)
{
	if(value)
		system_hal_set_qspi0_disckg(&s_system, 1);
	else
		system_hal_set_qspi0_disckg(&s_system, 0);
}
uint32_t system_driver_get_qspi0_disckg()
{
	return system_hal_get_qspi0_disckg(&s_system);
}

void system_driver_set_psram_disckg(bool value)
{
	if(value)
		system_hal_set_psram_disckg(&s_system, 1);
	else
		system_hal_set_psram_disckg(&s_system, 0);
}
uint32_t system_driver_get_psram_disckg()
{
	return system_hal_get_psram_disckg(&s_system);
}

void system_driver_set_fft_disckg(bool value)
{
	if(value)
		system_hal_set_fft_disckg(&s_system, 1);
	else
		system_hal_set_fft_disckg(&s_system, 0);
}
uint32_t system_driver_get_fft_disckg()
{
	return system_hal_get_fft_disckg(&s_system);
}

void system_driver_set_sbc_disckg(bool value)
{
	if(value)
		system_hal_set_sbc_disckg(&s_system, 1);
	else
		system_hal_set_sbc_disckg(&s_system, 0);
}
uint32_t system_driver_get_sbc_disckg()
{
	return system_hal_get_sbc_disckg(&s_system);
}

void system_driver_set_aud_disckg(bool value)
{
	if(value)
		system_hal_set_aud_disckg(&s_system, 1);
	else
		system_hal_set_aud_disckg(&s_system, 0);
}
uint32_t system_driver_get_aud_disckg()
{
	return system_hal_get_aud_disckg(&s_system);
}

void system_driver_set_i2s_disckg(bool value)
{
	if(value)
		system_hal_set_i2s_disckg(&s_system, 1);
	else
		system_hal_set_i2s_disckg(&s_system, 0);
}
uint32_t system_driver_get_i2s_disckg()
{
	return system_hal_get_i2s_disckg(&s_system);
}

void system_driver_set_jpeg_disckg(bool value)
{
	if(value)
		system_hal_set_jpeg_disckg(&s_system, 1);
	else
		system_hal_set_jpeg_disckg(&s_system, 0);
}
uint32_t system_driver_get_jpeg_disckg()
{
	return system_hal_get_jpeg_disckg(&s_system);
}

void system_driver_set_jpeg_dec_disckg(bool value)
{
	if(value)
		system_hal_set_jpeg_dec_disckg(&s_system, 1);
	else
		system_hal_set_jpeg_dec_disckg(&s_system, 0);
}
uint32_t system_driver_get_jpeg_dec_disckg()
{
	return system_hal_get_jpeg_dec_disckg(&s_system);
}

//0xff
void system_driver_set_disp_disckg(bool value)
{
	if(value)
		system_hal_set_disp_disckg(&s_system, 1);
	else
		system_hal_set_disp_disckg(&s_system, 0);
}
uint32_t system_driver_get_disp_disckg()
{
	return system_hal_get_disp_disckg(&s_system);
}

void system_driver_set_dma2d_disckg(bool value)
{
	if(value)
		system_hal_set_dma2d_disckg(&s_system, 1);
	else
		system_hal_set_dma2d_disckg(&s_system, 0);
}
uint32_t system_driver_get_dma2d_disckg()
{
	return system_hal_get_dma2d_disckg(&s_system);
}

void system_driver_set_btdm_disckg(bool value)
{
	if(value)
		system_hal_set_btdm_disckg(&s_system, 1);
	else
		system_hal_set_btdm_disckg(&s_system, 0);
}
uint32_t system_driver_get_btdm_disckg()
{
	return system_hal_get_btdm_disckg(&s_system);
}

void system_driver_set_xver_disckg(bool value)
{
	if(value)
		system_hal_set_xver_disckg(&s_system, 1);
	else
		system_hal_set_xver_disckg(&s_system, 0);
}
uint32_t system_driver_get_xver_disckg()
{
	return system_hal_get_xver_disckg(&s_system);
}

void system_driver_set_btdm_bps_disckg(bool value)
{
	if(value)
		system_hal_set_btdm_bps_disckg(&s_system, 1);
	else
		system_hal_set_btdm_bps_disckg(&s_system, 0);
}
uint32_t system_driver_get_btdm_bps_disckg()
{
	return system_hal_get_btdm_bps_disckg(&s_system);
}


//0x10
void system_driver_set_power_on_mem1(bool value)
{
	if(value)
		system_hal_set_power_on_mem1(&s_system, 1);
	else
		system_hal_set_power_on_mem1(&s_system, 0);
}
uint32_t system_driver_get_power_on_mem1()
{
	return system_hal_get_power_on_mem1(&s_system);
}
void system_driver_set_power_on_mem2(bool value)
{
	if(value)
		system_hal_set_power_on_mem2(&s_system, 1);
	else
		system_hal_set_power_on_mem2(&s_system, 0);
}
uint32_t system_driver_get_power_on_mem2()
{
	return system_hal_get_power_on_mem2(&s_system);
}
void system_driver_set_power_on_mem3(bool value)
{
	if(value)
		system_hal_set_power_on_mem3(&s_system, 1);
	else
		system_hal_set_power_on_mem3(&s_system, 0);
}
uint32_t system_driver_get_power_on_mem3()
{
	return system_hal_get_power_on_mem3(&s_system);
}

void system_driver_set_power_on_encp(bool value)
{
	if(value)
		system_hal_set_power_on_encp(&s_system, 1);
	else
		system_hal_set_power_on_encp(&s_system, 0);
}
uint32_t system_driver_get_power_on_encp()
{
	return system_hal_get_power_on_encp(&s_system);
}
void system_driver_set_power_on_bakp(bool value)
{
	if(value)
		system_hal_set_power_on_bakp(&s_system, 1);
	else
		system_hal_set_power_on_bakp(&s_system, 0);
}
uint32_t system_driver_get_power_on_bakp()
{
	return system_hal_get_power_on_bakp(&s_system);
}
void system_driver_set_power_on_ahbp(bool value)
{
	if(value)
		system_hal_set_power_on_ahbp(&s_system, 1);
	else
		system_hal_set_power_on_ahbp(&s_system, 0);
}
uint32_t system_driver_get_power_on_ahbp()
{
	return system_hal_get_power_on_ahbp(&s_system);
}

void system_driver_set_power_on_audp(bool value)
{
	if(value)
		system_hal_set_power_on_audp(&s_system, 1);
	else
		system_hal_set_power_on_audp(&s_system, 0);
}
uint32_t system_driver_get_power_on_audp()
{
	return system_hal_get_power_on_audp(&s_system);
}

void system_driver_set_power_on_vidp(bool value)
{
	if(value)
		system_hal_set_power_on_vidp(&s_system, 1);
	else
		system_hal_set_power_on_vidp(&s_system, 0);
}
uint32_t system_driver_get_power_on_vidp()
{
	return system_hal_get_power_on_vidp(&s_system);
}

void system_driver_set_power_on_btsp(bool value)
{
	if(value)
		system_hal_set_power_on_btsp(&s_system, 1);
	else
		system_hal_set_power_on_btsp(&s_system, 0);
}
uint32_t system_driver_get_power_on_btsp()
{
	return system_hal_get_power_on_btsp(&s_system);
}

void system_driver_set_power_on_wifi_mac(bool value)
{
	if(value)
		system_hal_set_power_on_wifi_mac(&s_system, 1);
	else
		system_hal_set_power_on_wifi_mac(&s_system, 0);
}
uint32_t system_driver_get_power_on_wifi_mac()
{
	return system_hal_get_power_on_wifi_mac(&s_system);
}

void system_driver_set_power_on_wifi_phy(bool value)
{
	if(value)
		system_hal_set_power_on_wifi_phy(&s_system, 1);
	else
		system_hal_set_power_on_wifi_phy(&s_system, 0);
}
uint32_t system_driver_get_power_on_wifi_phy()
{
	return system_hal_get_power_on_wifi_phy(&s_system);
}

void system_driver_set_sleep_en_need_flash_idle(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_flash_idle(&s_system, 1);
	else
		system_hal_set_sleep_en_need_flash_idle(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_flash_idle()
{
	return system_hal_get_sleep_en_need_flash_idle(&s_system);
}

void system_driver_set_sleep_en_need_cpu1_wifi(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_cpu1_wifi(&s_system, 1);
	else
		system_hal_set_sleep_en_need_cpu1_wifi(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_cpu1_wifi()
{
	return system_hal_get_sleep_en_need_cpu1_wifi(&s_system);
}

void system_driver_set_sleep_en_need_cpu0_wifi(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_cpu0_wifi(&s_system, 1);
	else
		system_hal_set_sleep_en_need_cpu0_wifi(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_cpu0_wifi()
{
	return system_hal_get_sleep_en_need_cpu0_wifi(&s_system);
}

void system_driver_set_sleep_en_need_global(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_global(&s_system, 1);
	else
		system_hal_set_sleep_en_need_global(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_global()
{
	return system_hal_get_sleep_en_need_global(&s_system);
}

void system_driver_set_wifi_wakeup_platform_en(bool value)
{
	if(value)
		system_hal_set_wifi_wakeup_platform_en(&s_system, 1);
	else
		system_hal_set_wifi_wakeup_platform_en(&s_system, 0);
}
uint32_t system_driver_get_wifi_wakeup_platform_en()
{
	return system_hal_get_wifi_wakeup_platform_en(&s_system);
}
void system_driver_set_bts_wakeup_platform_en(bool value)
{
	if(value)
		system_hal_set_bts_wakeup_platform_en(&s_system, 1);
	else
		system_hal_set_bts_wakeup_platform_en(&s_system, 0);
}
uint32_t system_driver_get_bts_wakeup_platform_en()
{
	return system_hal_get_bts_wakeup_platform_en(&s_system);
}
void system_driver_set_bts_sleep_exit_req(bool value)
{
	if(value)
		system_hal_set_bts_sleep_exit_req(&s_system, 1);
	else
		system_hal_set_bts_sleep_exit_req(&s_system, 0);
}
uint32_t system_driver_get_bts_sleep_exit_req()
{
	return system_hal_get_bts_sleep_exit_req(&s_system);
}

//0x20
void system_driver_set_cpu0_bmc32_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_bmc32_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_bmc32_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_bmc32_int_en()
{
	return system_hal_get_cpu0_bmc32_int_en(&s_system);
}

void system_driver_set_cpu0_host_0_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_host_0_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_host_0_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_host_0_irq_en()
{
	return system_hal_get_cpu0_host_0_irq_en(&s_system);
}

void system_driver_set_cpu0_host_0_sec_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_host_0_sec_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_host_0_sec_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_host_0_sec_irq_en()
{
	return system_hal_get_cpu0_host_0_sec_irq_en(&s_system);
}

void system_driver_set_cpu0_timer_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_timer_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_timer_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_timer_int_en()
{
	return system_hal_get_cpu0_timer_int_en(&s_system);
}

void system_driver_set_cpu0_uart_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_uart_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_uart_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_uart_int_en()
{
	return system_hal_get_cpu0_uart_int_en(&s_system);
}

void system_driver_set_cpu0_pwm_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_pwm_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_pwm_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_pwm_int_en()
{
	return system_hal_get_cpu0_pwm_int_en(&s_system);
}

void system_driver_set_cpu0_i2c_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_i2c_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_i2c_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_i2c_int_en()
{
	return system_hal_get_cpu0_i2c_int_en(&s_system);
}

void system_driver_set_cpu0_spi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_spi_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_spi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_spi_int_en()
{
	return system_hal_get_cpu0_spi_int_en(&s_system);
}

void system_driver_set_cpu0_sadc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_sadc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_sadc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_sadc_int_en()
{
	return system_hal_get_cpu0_sadc_int_en(&s_system);
}

void system_driver_set_cpu0_irda_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_irda_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_irda_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_irda_int_en()
{
	return system_hal_get_cpu0_irda_int_en(&s_system);
}

void system_driver_set_cpu0_sdio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_sdio_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_sdio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_sdio_int_en()
{
	return system_hal_get_cpu0_sdio_int_en(&s_system);
}

void system_driver_set_cpu0_gdma_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_gdma_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_gdma_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_gdma_int_en()
{
	return system_hal_get_cpu0_gdma_int_en(&s_system);
}

void system_driver_set_cpu0_la_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_la_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_la_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_la_int_en()
{
	return system_hal_get_cpu0_la_int_en(&s_system);
}

void system_driver_set_cpu0_timer1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_timer1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_timer1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_timer1_int_en()
{
	return system_hal_get_cpu0_timer1_int_en(&s_system);
}

void system_driver_set_cpu0_i2c1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_i2c1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_i2c1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_i2c1_int_en()
{
	return system_hal_get_cpu0_i2c1_int_en(&s_system);
}

void system_driver_set_cpu0_uart1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_uart1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_uart1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_uart1_int_en()
{
	return system_hal_get_cpu0_uart1_int_en(&s_system);
}


void system_driver_set_cpu0_uart2_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_uart2_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_uart2_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_uart2_int_en()
{
	return system_hal_get_cpu0_uart2_int_en(&s_system);
}

void system_driver_set_cpu0_spi1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_spi1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_spi1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_spi1_int_en()
{
	return system_hal_get_cpu0_spi1_int_en(&s_system);
}

void system_driver_set_cpu0_can_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_can_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_can_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_can_int_en()
{
	return system_hal_get_cpu0_can_int_en(&s_system);
}

void system_driver_set_cpu0_usb_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_usb_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_usb_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_usb_int_en()
{
	return system_hal_get_cpu0_usb_int_en(&s_system);
}

void system_driver_set_cpu0_qspi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_qspi_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_qspi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_qspi_int_en()
{
	return system_hal_get_cpu0_qspi_int_en(&s_system);
}

void system_driver_set_cpu0_fft_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_fft_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_fft_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_fft_int_en()
{
	return system_hal_get_cpu0_fft_int_en(&s_system);
}

void system_driver_set_cpu0_sbc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_sbc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_sbc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_sbc_int_en()
{
	return system_hal_get_cpu0_sbc_int_en(&s_system);
}

void system_driver_set_cpu0_aud_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_aud_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_aud_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_aud_int_en()
{
	return system_hal_get_cpu0_aud_int_en(&s_system);
}

void system_driver_set_cpu0_i2s_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_i2s_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_i2s_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_i2s_int_en()
{
	return system_hal_get_cpu0_i2s_int_en(&s_system);
}

void system_driver_set_cpu0_jpegenc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_jpegenc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_jpegenc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_jpegenc_int_en()
{
	return system_hal_get_cpu0_jpegenc_int_en(&s_system);
}

void system_driver_set_cpu0_jpegdec_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_jpegdec_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_jpegdec_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_jpegdec_int_en()
{
	return system_hal_get_cpu0_jpegdec_int_en(&s_system);
}

void system_driver_set_cpu0_lcd_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_lcd_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_lcd_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_lcd_int_en()
{
	return system_hal_get_cpu0_lcd_int_en(&s_system);
}


void system_driver_set_cpu0_wifi_int_phy_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_int_phy_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_int_phy_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_int_phy_en()
{
	return system_hal_get_cpu0_wifi_int_phy_en(&s_system);
}


void system_driver_set_cpu0_wifi_mac_int_timer_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_timer_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_timer_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_timer_en()
{
	return system_hal_get_cpu0_wifi_mac_int_timer_en(&s_system);
}

void system_driver_set_cpu0_wifi_mac_int_misc_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_misc_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_misc_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_misc_en()
{
	return system_hal_get_cpu0_wifi_mac_int_misc_en(&s_system);
}


//0x21
void system_driver_set_cpu0_wifi_mac_int_rx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_rx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_rx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_rx_trigger_en()
{
	return system_hal_get_cpu0_wifi_mac_int_rx_trigger_en(&s_system);
}
void system_driver_set_cpu0_wifi_mac_int_tx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_tx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_tx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_tx_trigger_en()
{
	return system_hal_get_cpu0_wifi_mac_int_tx_trigger_en(&s_system);
}

void system_driver_set_cpu0_wifi_mac_port_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_port_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_port_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_port_trigger_en()
{
	return system_hal_get_cpu0_wifi_mac_port_trigger_en(&s_system);
}

void system_driver_set_cpu0_wifi_mac_int_gen_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_gen_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_gen_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_gen_en()
{
	return system_hal_get_cpu0_wifi_mac_int_gen_en(&s_system);
}

void system_driver_set_cpu0_wifi_hsu_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_hsu_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_hsu_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_hsu_irq_en()
{
	return system_hal_get_cpu0_wifi_hsu_irq_en(&s_system);
}
void system_driver_set_cpu0_wifi_int_mac_wakeup_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_int_mac_wakeup_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_int_mac_wakeup_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_int_mac_wakeup_en()
{
	return system_hal_get_cpu0_wifi_int_mac_wakeup_en(&s_system);
}

void system_driver_set_cpu0_dm_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_dm_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_dm_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_dm_irq_en()
{
	return system_hal_get_cpu0_dm_irq_en(&s_system);
}


void system_driver_set_cpu0_ble_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_ble_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_ble_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_ble_irq_en()
{
	return system_hal_get_cpu0_ble_irq_en(&s_system);
}

void system_driver_set_cpu0_bt_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_bt_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_bt_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_bt_irq_en()
{
	return system_hal_get_cpu0_bt_irq_en(&s_system);
}
void system_driver_set_cpu0_mbox0_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_mbox0_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_mbox0_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_mbox0_int_en()
{
	return system_hal_get_cpu0_mbox0_int_en(&s_system);
}

void system_driver_set_cpu0_mbox1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_mbox1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_mbox1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_mbox1_int_en()
{
	return system_hal_get_cpu0_mbox1_int_en(&s_system);
}
void system_driver_set_cpu0_bmc64_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_bmc64_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_bmc64_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_bmc64_int_en()
{
	return system_hal_get_cpu0_bmc64_int_en(&s_system);
}
void system_driver_set_cpu0_touched_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_touched_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_touched_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_touched_int_en()
{
	return system_hal_get_cpu0_touched_int_en(&s_system);
}

void system_driver_set_cpu0_usbplug_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_usbplug_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_usbplug_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_usbplug_int_en()
{
	return system_hal_get_cpu0_usbplug_int_en(&s_system);
}

void system_driver_set_cpu0_rtc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_rtc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_rtc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_rtc_int_en()
{
	return system_hal_get_cpu0_rtc_int_en(&s_system);
}

void system_driver_set_cpu0_gpio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_gpio_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_gpio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_gpio_int_en()
{
	return system_hal_get_cpu0_gpio_int_en(&s_system);
}


//0x22
void system_driver_set_cpu1_bmc32_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_bmc32_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_bmc32_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_bmc32_int_en()
{
	return system_hal_get_cpu1_bmc32_int_en(&s_system);
}

void system_driver_set_cpu1_host_0_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_host_0_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_host_0_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_host_0_irq_en()
{
	return system_hal_get_cpu1_host_0_irq_en(&s_system);
}

void system_driver_set_cpu1_host_0_sec_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_host_0_sec_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_host_0_sec_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_host_0_sec_irq_en()
{
	return system_hal_get_cpu1_host_0_sec_irq_en(&s_system);
}

void system_driver_set_cpu1_timer_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_timer_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_timer_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_timer_int_en()
{
	return system_hal_get_cpu1_timer_int_en(&s_system);
}

void system_driver_set_cpu1_uart_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_uart_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_uart_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_uart_int_en()
{
	return system_hal_get_cpu1_uart_int_en(&s_system);
}

void system_driver_set_cpu1_pwm_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_pwm_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_pwm_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_pwm_int_en()
{
	return system_hal_get_cpu1_pwm_int_en(&s_system);
}

void system_driver_set_cpu1_i2c_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_i2c_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_i2c_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_i2c_int_en()
{
	return system_hal_get_cpu1_i2c_int_en(&s_system);
}

void system_driver_set_cpu1_spi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_spi_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_spi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_spi_int_en()
{
	return system_hal_get_cpu1_spi_int_en(&s_system);
}

void system_driver_set_cpu1_sadc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_sadc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_sadc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_sadc_int_en()
{
	return system_hal_get_cpu1_sadc_int_en(&s_system);
}

void system_driver_set_cpu1_irda_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_irda_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_irda_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_irda_int_en()
{
	return system_hal_get_cpu1_irda_int_en(&s_system);
}

void system_driver_set_cpu1_sdio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_sdio_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_sdio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_sdio_int_en()
{
	return system_hal_get_cpu1_sdio_int_en(&s_system);
}

void system_driver_set_cpu1_gdma_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_gdma_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_gdma_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_gdma_int_en()
{
	return system_hal_get_cpu1_gdma_int_en(&s_system);
}

void system_driver_set_cpu1_la_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_la_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_la_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_la_int_en()
{
	return system_hal_get_cpu1_la_int_en(&s_system);
}

void system_driver_set_cpu1_timer1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_timer1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_timer1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_timer1_int_en()
{
	return system_hal_get_cpu1_timer1_int_en(&s_system);
}

void system_driver_set_cpu1_i2c1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_i2c1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_i2c1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_i2c1_int_en()
{
	return system_hal_get_cpu1_i2c1_int_en(&s_system);
}

void system_driver_set_cpu1_uart1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_uart1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_uart1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_uart1_int_en()
{
	return system_hal_get_cpu1_uart1_int_en(&s_system);
}


void system_driver_set_cpu1_uart2_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_uart2_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_uart2_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_uart2_int_en()
{
	return system_hal_get_cpu1_uart2_int_en(&s_system);
}

void system_driver_set_cpu1_spi1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_spi1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_spi1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_spi1_int_en()
{
	return system_hal_get_cpu1_spi1_int_en(&s_system);
}

void system_driver_set_cpu1_can_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_can_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_can_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_can_int_en()
{
	return system_hal_get_cpu1_can_int_en(&s_system);
}

void system_driver_set_cpu1_usb_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_usb_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_usb_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_usb_int_en()
{
	return system_hal_get_cpu1_usb_int_en(&s_system);
}

void system_driver_set_cpu1_qspi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_qspi_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_qspi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_qspi_int_en()
{
	return system_hal_get_cpu1_qspi_int_en(&s_system);
}

void system_driver_set_cpu1_fft_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_fft_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_fft_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_fft_int_en()
{
	return system_hal_get_cpu1_fft_int_en(&s_system);
}

void system_driver_set_cpu1_sbc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_sbc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_sbc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_sbc_int_en()
{
	return system_hal_get_cpu1_sbc_int_en(&s_system);
}

void system_driver_set_cpu1_aud_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_aud_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_aud_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_aud_int_en()
{
	return system_hal_get_cpu1_aud_int_en(&s_system);
}

void system_driver_set_cpu1_i2s_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_i2s_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_i2s_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_i2s_int_en()
{
	return system_hal_get_cpu1_i2s_int_en(&s_system);
}

void system_driver_set_cpu1_jpegenc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_jpegenc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_jpegenc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_jpegenc_int_en()
{
	return system_hal_get_cpu1_jpegenc_int_en(&s_system);
}

void system_driver_set_cpu1_jpegdec_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_jpegdec_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_jpegdec_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_jpegdec_int_en()
{
	return system_hal_get_cpu1_jpegdec_int_en(&s_system);
}

void system_driver_set_cpu1_lcd_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_lcd_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_lcd_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_lcd_int_en()
{
	return system_hal_get_cpu1_lcd_int_en(&s_system);
}


void system_driver_set_cpu1_wifi_int_phy_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_int_phy_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_int_phy_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_int_phy_en()
{
	return system_hal_get_cpu1_wifi_int_phy_en(&s_system);
}


void system_driver_set_cpu1_wifi_mac_int_timer_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_timer_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_timer_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_timer_en()
{
	return system_hal_get_cpu1_wifi_mac_int_timer_en(&s_system);
}

void system_driver_set_cpu1_wifi_mac_int_misc_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_misc_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_misc_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_misc_en()
{
	return system_hal_get_cpu1_wifi_mac_int_misc_en(&s_system);
}


//0x23
void system_driver_set_cpu1_wifi_mac_int_rx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_rx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_rx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_rx_trigger_en()
{
	return system_hal_get_cpu1_wifi_mac_int_rx_trigger_en(&s_system);
}
void system_driver_set_cpu1_wifi_mac_int_tx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_tx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_tx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_tx_trigger_en()
{
	return system_hal_get_cpu1_wifi_mac_int_tx_trigger_en(&s_system);
}

void system_driver_set_cpu1_wifi_mac_port_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_port_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_port_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_port_trigger_en()
{
	return system_hal_get_cpu1_wifi_mac_port_trigger_en(&s_system);
}

void system_driver_set_cpu1_wifi_mac_int_gen_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_gen_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_gen_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_gen_en()
{
	return system_hal_get_cpu1_wifi_mac_int_gen_en(&s_system);
}

void system_driver_set_cpu1_wifi_hsu_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_hsu_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_hsu_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_hsu_irq_en()
{
	return system_hal_get_cpu1_wifi_hsu_irq_en(&s_system);
}
void system_driver_set_cpu1_wifi_int_mac_wakeup_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_int_mac_wakeup_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_int_mac_wakeup_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_int_mac_wakeup_en()
{
	return system_hal_get_cpu1_wifi_int_mac_wakeup_en(&s_system);
}

void system_driver_set_cpu1_dm_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_dm_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_dm_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_dm_irq_en()
{
	return system_hal_get_cpu1_dm_irq_en(&s_system);
}


void system_driver_set_cpu1_ble_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_ble_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_ble_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_ble_irq_en()
{
	return system_hal_get_cpu1_ble_irq_en(&s_system);
}

void system_driver_set_cpu1_bt_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_bt_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_bt_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_bt_irq_en()
{
	return system_hal_get_cpu1_bt_irq_en(&s_system);
}
void system_driver_set_cpu1_mbox0_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_mbox0_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_mbox0_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_mbox0_int_en()
{
	return system_hal_get_cpu1_mbox0_int_en(&s_system);
}

void system_driver_set_cpu1_mbox1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_mbox1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_mbox1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_mbox1_int_en()
{
	return system_hal_get_cpu1_mbox1_int_en(&s_system);
}
void system_driver_set_cpu1_bmc64_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_bmc64_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_bmc64_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_bmc64_int_en()
{
	return system_hal_get_cpu1_bmc64_int_en(&s_system);
}
void system_driver_set_cpu1_touched_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_touched_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_touched_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_touched_int_en()
{
	return system_hal_get_cpu1_touched_int_en(&s_system);
}

void system_driver_set_cpu1_usbplug_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_usbplug_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_usbplug_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_usbplug_int_en()
{
	return system_hal_get_cpu1_usbplug_int_en(&s_system);
}

void system_driver_set_cpu1_rtc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_rtc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_rtc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_rtc_int_en()
{
	return system_hal_get_cpu1_rtc_int_en(&s_system);
}

void system_driver_set_cpu1_gpio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_gpio_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_gpio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_gpio_int_en()
{
	return system_hal_get_cpu1_gpio_int_en(&s_system);
}


//0x28
uint32_t system_driver_get_cpu0_bmc32_int_st()
{
	return system_hal_get_cpu0_bmc32_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_host_0_irq_st()
{
	return system_hal_get_cpu0_host_0_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_host_0_sec_irq_st()
{
	return system_hal_get_cpu0_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_timer_int_st()
{
	return system_hal_get_cpu0_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_uart_int_st()
{
	return system_hal_get_cpu0_uart_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_pwm_int_st()
{
	return system_hal_get_cpu0_pwm_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_i2c_int_st()
{
	return system_hal_get_cpu0_i2c_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_spi_int_st()
{
	return system_hal_get_cpu0_spi_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_sadc_int_st()
{
	return system_hal_get_cpu0_sadc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_irda_int_st()
{
	return system_hal_get_cpu0_irda_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_sdio_int_st()
{
	return system_hal_get_cpu0_sdio_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_gdma_int_st()
{
	return system_hal_get_cpu0_gdma_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_la_int_st()
{
	return system_hal_get_cpu0_la_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_timer1_int_st()
{
	return system_hal_get_cpu0_timer1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_i2c1_int_st()
{
	return system_hal_get_cpu0_i2c1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_uart1_int_st()
{
	return system_hal_get_cpu0_uart1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_uart2_int_st()
{
	return system_hal_get_cpu0_uart2_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_spi1_int_st()
{
	return system_hal_get_cpu0_spi1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_can_int_st()
{
	return system_hal_get_cpu0_can_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_usb_int_st()
{
	return system_hal_get_cpu0_usb_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_qspi_int_st()
{
	return system_hal_get_cpu0_qspi_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_fft_int_st()
{
	return system_hal_get_cpu0_fft_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_sbc_int_st()
{
	return system_hal_get_cpu0_sbc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_aud_int_st()
{
	return system_hal_get_cpu0_aud_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_i2s_int_st()
{
	return system_hal_get_cpu0_i2s_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_jpegenc_int_st()
{
	return system_hal_get_cpu0_jpegenc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_jpegdec_int_st()
{
	return system_hal_get_cpu0_jpegdec_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_lcd_int_st()
{
	return system_hal_get_cpu0_lcd_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_int_phy_st()
{
	return system_hal_get_cpu0_wifi_int_phy_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_timer_st()
{
	return system_hal_get_cpu0_wifi_mac_int_timer_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_misc_st()
{
	return system_hal_get_cpu0_wifi_mac_int_misc_st(&s_system);
}

//0x29
uint32_t system_driver_get_cpu0_wifi_mac_int_rx_trigger_st()
{
	return system_hal_get_cpu0_wifi_mac_int_rx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_tx_trigger_st()
{
	return system_hal_get_cpu0_wifi_mac_int_tx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_port_trigger_st()
{
	return system_hal_get_cpu0_wifi_mac_port_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_gen_st()
{
	return system_hal_get_cpu0_wifi_mac_int_gen_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_hsu_irq_st()
{
	return system_hal_get_cpu0_wifi_hsu_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_int_mac_wakeup_st()
{
	return system_hal_get_cpu0_wifi_int_mac_wakeup_st(&s_system);
}
uint32_t system_driver_get_cpu0_dm_irq_st()
{
	return system_hal_get_cpu0_dm_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_ble_irq_st()
{
	return system_hal_get_cpu0_ble_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_bt_irq_st()
{
	return system_hal_get_cpu0_bt_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_mbox0_int_st()
{
	return system_hal_get_cpu0_mbox0_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_mbox1_int_st()
{
	return system_hal_get_cpu0_mbox1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_bmc64_int_st()
{
	return system_hal_get_cpu0_bmc64_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_touched_int_st()
{
	return system_hal_get_cpu0_touched_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_usbplug_int_st()
{
	return system_hal_get_cpu0_usbplug_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_rtc_int_st()
{
	return system_hal_get_cpu0_rtc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_gpio_int_st()
{
	return system_hal_get_cpu0_gpio_int_st(&s_system);
}

//0x2a
uint32_t system_driver_get_cpu1_bmc32_int_st()
{
	return system_hal_get_cpu1_bmc32_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_host_0_irq_st()
{
	return system_hal_get_cpu1_host_0_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_host_0_sec_irq_st()
{
	return system_hal_get_cpu1_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_timer_int_st()
{
	return system_hal_get_cpu1_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_uart_int_st()
{
	return system_hal_get_cpu1_uart_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_pwm_int_st()
{
	return system_hal_get_cpu1_pwm_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_i2c_int_st()
{
	return system_hal_get_cpu1_i2c_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_spi_int_st()
{
	return system_hal_get_cpu1_spi_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_sadc_int_st()
{
	return system_hal_get_cpu1_sadc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_irda_int_st()
{
	return system_hal_get_cpu1_irda_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_sdio_int_st()
{
	return system_hal_get_cpu1_sdio_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_gdma_int_st()
{
	return system_hal_get_cpu1_gdma_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_la_int_st()
{
	return system_hal_get_cpu1_la_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_timer1_int_st()
{
	return system_hal_get_cpu1_timer1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_i2c1_int_st()
{
	return system_hal_get_cpu1_i2c1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_uart1_int_st()
{
	return system_hal_get_cpu1_uart1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_uart2_int_st()
{
	return system_hal_get_cpu1_uart2_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_spi1_int_st()
{
	return system_hal_get_cpu1_spi1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_can_int_st()
{
	return system_hal_get_cpu1_can_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_usb_int_st()
{
	return system_hal_get_cpu1_usb_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_qspi_int_st()
{
	return system_hal_get_cpu1_qspi_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_fft_int_st()
{
	return system_hal_get_cpu1_fft_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_sbc_int_st()
{
	return system_hal_get_cpu1_sbc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_aud_int_st()
{
	return system_hal_get_cpu1_aud_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_i2s_int_st()
{
	return system_hal_get_cpu1_i2s_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_jpegenc_int_st()
{
	return system_hal_get_cpu1_jpegenc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_jpegdec_int_st()
{
	return system_hal_get_cpu1_jpegdec_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_lcd_int_st()
{
	return system_hal_get_cpu1_lcd_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_int_phy_st()
{
	return system_hal_get_cpu1_wifi_int_phy_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_timer_st()
{
	return system_hal_get_cpu1_wifi_mac_int_timer_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_misc_st()
{
	return system_hal_get_cpu1_wifi_mac_int_misc_st(&s_system);
}

//0x29
uint32_t system_driver_get_cpu1_wifi_mac_int_rx_trigger_st()
{
	return system_hal_get_cpu1_wifi_mac_int_rx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_tx_trigger_st()
{
	return system_hal_get_cpu1_wifi_mac_int_tx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_port_trigger_st()
{
	return system_hal_get_cpu1_wifi_mac_port_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_gen_st()
{
	return system_hal_get_cpu1_wifi_mac_int_gen_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_hsu_irq_st()
{
	return system_hal_get_cpu1_wifi_hsu_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_int_mac_wakeup_st()
{
	return system_hal_get_cpu1_wifi_int_mac_wakeup_st(&s_system);
}
uint32_t system_driver_get_cpu1_dm_irq_st()
{
	return system_hal_get_cpu1_dm_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_ble_irq_st()
{
	return system_hal_get_cpu1_ble_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_bt_irq_st()
{
	return system_hal_get_cpu1_bt_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_mbox0_int_st()
{
	return system_hal_get_cpu1_mbox0_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_mbox1_int_st()
{
	return system_hal_get_cpu1_mbox1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_bmc64_int_st()
{
	return system_hal_get_cpu1_bmc64_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_touched_int_st()
{
	return system_hal_get_cpu1_touched_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_usbplug_int_st()
{
	return system_hal_get_cpu1_usbplug_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_rtc_int_st()
{
	return system_hal_get_cpu1_rtc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_gpio_int_st()
{
	return system_hal_get_cpu1_gpio_int_st(&s_system);
}


//REG_0x30 0x31 0x32 0x33 0x34 0x35
void system_driver_set_gpio0_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio0_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio0_perial_mode(uint32_t group)
{
	return system_hal_get_gpio0_perial_mode(&s_system, group);
}
void system_driver_set_gpio1_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio1_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio1_perial_mode(uint32_t group)
{
	return system_hal_get_gpio1_perial_mode(&s_system, group);
}

void system_driver_set_gpio2_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio2_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio2_perial_mode(uint32_t group)
{
	return system_hal_get_gpio2_perial_mode(&s_system, group);
}
void system_driver_set_gpio3_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio3_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio3_perial_mode(uint32_t group)
{
	return system_hal_get_gpio3_perial_mode(&s_system, group);
}
void system_driver_set_gpio4_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio4_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio4_perial_mode(uint32_t group)
{
	return system_hal_get_gpio4_perial_mode(&s_system, group);
}
void system_driver_set_gpio5_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio5_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio5_perial_mode(uint32_t group)
{
	return system_hal_get_gpio5_perial_mode(&s_system, group);
}
void system_driver_set_gpio6_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio6_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio6_perial_mode(uint32_t group)
{
	return system_hal_get_gpio6_perial_mode(&s_system, group);
}
void system_driver_set_gpio7_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio7_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio7_perial_mode(uint32_t group)
{
	return system_hal_get_gpio7_perial_mode(&s_system, group);
}

//0x38
void system_driver_set_dbug_cfg0(uint32_t group, bool value)
{
	system_hal_set_dbug_cfg0(&s_system, group, value);
}
uint32_t system_driver_get_dbug_cfg0(uint32_t group)
{
	return system_hal_get_dbug_cfg0(&s_system, group);
}

//0x39
void system_driver_set_dbug_cfg1(uint32_t group, bool value)
{
	system_hal_set_dbug_cfg1(&s_system, group, value);
}
uint32_t system_driver_get_dbug_cfg1(uint32_t group)
{
	return system_hal_get_dbug_cfg1(&s_system, group);
}

