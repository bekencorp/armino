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

#include <driver/gpio.h>
#include <driver/media_types.h>
#include <driver/lcd_types.h>
#include <driver/lcd_spi.h>
#include "bk_misc.h"
#include "lcd_devices.h"
#include "gpio_driver.h"

static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_20M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,
	.hsync_pulse_width = 2,
	.hsync_back_porch = 17,
	.hsync_front_porch = 5,
	.vsync_pulse_width =3,
	.vsync_back_porch = 12,
	.vsync_front_porch = 5,
};

static void lcd_st7701s_config(void)
{
	bk_gpio_set_output_high(LCD_SPI_RST);
	rtos_delay_milliseconds(1);
	bk_gpio_set_output_low(LCD_SPI_RST);
	rtos_delay_milliseconds(1);
	bk_gpio_set_output_high(LCD_SPI_RST);
	rtos_delay_milliseconds(120);

	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x13);
	lcd_spi_write_cmd(0xEF);
	lcd_spi_write_data(0x08);
	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_cmd(0xC0);
	lcd_spi_write_data(0xE9);
	lcd_spi_write_data(0x03);
	lcd_spi_write_cmd(0xC1);
	lcd_spi_write_data(0x0C);
	lcd_spi_write_data(0x02);
	lcd_spi_write_cmd(0xC2);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x06);
	lcd_spi_write_cmd(0xCC);
	lcd_spi_write_data(0x38);
	//-------------------------------------Gamma Cluster Setting-------------------------------------------//
	lcd_spi_write_cmd(0xB0);
	lcd_spi_write_data(0x40);
	lcd_spi_write_data(0x0B);
	lcd_spi_write_data(0x58);
	lcd_spi_write_data(0x0C);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x0B);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x09);
	lcd_spi_write_data(0x26);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x16);
	lcd_spi_write_data(0x18);
	lcd_spi_write_data(0x6A);
	lcd_spi_write_data(0x6E);
	lcd_spi_write_data(0x4F);
	lcd_spi_write_cmd(0xB1);
	lcd_spi_write_data(0x40);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x57);
	lcd_spi_write_data(0x0D);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x0B);
	lcd_spi_write_data(0x09);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x26);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0xD3);
	lcd_spi_write_data(0x0D);
	lcd_spi_write_data(0x6B);
	lcd_spi_write_data(0x6E);
	lcd_spi_write_data(0x4F);
	//---------------------------------------End Gamma Setting----------------------------------------------//
	//------------------------------------End Display Control setting----------------------------------------//
	//-----------------------------------------Bank0 Setting End---------------------------------------------//
	//-------------------------------------------Bank1 Setting---------------------------------------------------//
	//-------------------------------- Power Control Registers Initial --------------------------------------//
	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x11);
	lcd_spi_write_cmd(0xB0);
	lcd_spi_write_data(0x56);
	//-------------------------------------------Vcom Setting---------------------------------------------------//
	lcd_spi_write_cmd(0xB1);
	lcd_spi_write_data(0x3A);
	//-----------------------------------------End Vcom Setting-----------------------------------------------//
	lcd_spi_write_cmd(0xB2);
	lcd_spi_write_data(0x87);
	lcd_spi_write_cmd(0xB3);
	lcd_spi_write_data(0x80);
	lcd_spi_write_cmd(0xB5);
	lcd_spi_write_data(0x4D);
	lcd_spi_write_cmd(0xB7);
	lcd_spi_write_data(0x85);
	lcd_spi_write_cmd(0xB8);
	lcd_spi_write_data(0x10);
	lcd_spi_write_cmd(0xB9);
	lcd_spi_write_data(0x10);
	lcd_spi_write_cmd(0xBC);
	lcd_spi_write_data(0x03);
	lcd_spi_write_cmd(0xC0);
	lcd_spi_write_data(0x89);
	lcd_spi_write_cmd(0xC1);
	lcd_spi_write_data(0x78);
	lcd_spi_write_cmd(0xC2);
	lcd_spi_write_data(0x78);
	lcd_spi_write_cmd(0xD0);
	lcd_spi_write_data(0x88);
	//---------------------------------End Power Control Registers Initial -------------------------------//
	rtos_delay_milliseconds(100);
	//---------------------------------------------GIP Setting----------------------------------------------------//
	lcd_spi_write_cmd(0xE0);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x02);
	lcd_spi_write_cmd(0xE1);
	lcd_spi_write_data(0x04);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x20);
	lcd_spi_write_cmd(0xE2);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xE3);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x33);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xE4);
	lcd_spi_write_data(0x22);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xE5);
	lcd_spi_write_data(0x04);
	lcd_spi_write_data(0x5C);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x5C);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xE6);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x33);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xE7);
	lcd_spi_write_data(0x22);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xE8);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x5C);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x5C);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xEB);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x40);
	lcd_spi_write_data(0x40);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xEC);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xED);
	lcd_spi_write_data(0xFA);
	lcd_spi_write_data(0x45);
	lcd_spi_write_data(0x0B);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xFF);
	lcd_spi_write_data(0xB0);
	lcd_spi_write_data(0x54);
	lcd_spi_write_data(0xAF);
	lcd_spi_write_cmd(0xEF);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x45);
	lcd_spi_write_data(0x3F);
	lcd_spi_write_data(0x54);
	//--------------------------------------------End GIP Setting-----------------------------------------------//
	//------------------------------ Power Control Registers Initial End-----------------------------------//
	//------------------------------------------Bank1 Setting----------------------------------------------------//
	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0x3a);
	lcd_spi_write_data(0x50);
	lcd_spi_write_cmd(0x11);
	rtos_delay_milliseconds(120);
	lcd_spi_write_cmd(0x29);

}

static void lcd_st7701s_backlight_io_init(void)
{
	gpio_dev_unmap(34);
	bk_gpio_set_capacity(34, 0);
	BK_LOG_ON_ERR(bk_gpio_enable_output(34));
	BK_LOG_ON_ERR(bk_gpio_pull_down(34));
}

static void lcd_st7701s_init(void)
{
	lcd_st7701s_backlight_io_init();
	lcd_spi_init_gpio();
	lcd_st7701s_config();
}

static void st7701s_lcd_backlight_open(void)
{
	BK_LOG_ON_ERR(bk_gpio_pull_up(34));
	// pull up gpio34, enable lcd backlight control
	bk_gpio_set_output_high(34);
}

static void st7701s_lcd_backlight_close(void)
{
	bk_gpio_set_output_low(34);
}

const lcd_device_t lcd_device_sn5st7701s =
{
	.id = LCD_DEVICE_SN5ST7701S,
	.name = "sn5st7701s",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X854,
	.rgb = &lcd_rgb,
	.backlight_open =  st7701s_lcd_backlight_open,///st7701s_lcd_backlight_open,
	.backlight_set = NULL,
	.init = lcd_st7701s_init,
	.backlight_close =  st7701s_lcd_backlight_close,//st7701s_lcd_backlight_close,
	.lcd_off = NULL,
};


