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
	.vsync_pulse_width = 2,
	.hsync_back_porch = 40,  //3
	.hsync_front_porch = 5,  //2
	.vsync_back_porch = 8,   //1
	.vsync_front_porch = 8,  //1
};

static void lcd_st7710s_config(void)
{
	bk_gpio_set_output_low(LCD_SPI_RST);
	delay(800);
	bk_gpio_set_output_high(LCD_SPI_RST);
	delay(800);

	lcd_spi_write_cmd (0x11);
	rtos_delay_milliseconds(120);
/*-------------------------------Bank0 Setting--------------------------------*/
/*--------------------------Display Control setting---------------------------*/
	lcd_spi_write_cmd (0xFF);
	lcd_spi_write_data (0x77);
	lcd_spi_write_data (0x01);
	lcd_spi_write_data (0x00);
	lcd_spi_write_data (0x00);
	lcd_spi_write_data (0x10);
	lcd_spi_write_cmd (0xC0);
	lcd_spi_write_data (0x63);
	lcd_spi_write_data (0x00);
	lcd_spi_write_cmd (0xC1);
	lcd_spi_write_data (0x0A);
	lcd_spi_write_data (0x02);
	lcd_spi_write_cmd (0xC2);
	lcd_spi_write_data (0x31);
	lcd_spi_write_data (0x08);
	lcd_spi_write_cmd (0xCC);
	lcd_spi_write_data (0x10);
/*---------------------------Gamma Cluster Setting----------------------------*/
	lcd_spi_write_cmd (0xB0);
	lcd_spi_write_data (0x00);
	lcd_spi_write_data (0x11);
	lcd_spi_write_data (0x19);
	lcd_spi_write_data (0x0C);
	lcd_spi_write_data (0x10);
	lcd_spi_write_data (0x06);
	lcd_spi_write_data (0x07);
	lcd_spi_write_data (0x0A);
	lcd_spi_write_data (0x09);
	lcd_spi_write_data (0x22);
	lcd_spi_write_data (0x04);
	lcd_spi_write_data (0x10);
	lcd_spi_write_data (0x0E);
	lcd_spi_write_data (0x28);
	lcd_spi_write_data (0x30);
	lcd_spi_write_data (0x1C);
	lcd_spi_write_cmd (0xB1);
	lcd_spi_write_data (0x00);
	lcd_spi_write_data (0x12);
	lcd_spi_write_data (0x19);
	lcd_spi_write_data (0x0D);
	lcd_spi_write_data (0x10);
	lcd_spi_write_data (0x04);
	lcd_spi_write_data (0x06);
	lcd_spi_write_data (0x07);
	lcd_spi_write_data (0x08);
	lcd_spi_write_data (0x23);
	lcd_spi_write_data (0x04);
	lcd_spi_write_data (0x12);
	lcd_spi_write_data (0x11);
	lcd_spi_write_data (0x28);
	lcd_spi_write_data (0x30);
	lcd_spi_write_data (0x1C);
/*-----------------------------End Gamma Setting------------------------------*/
/*------------------------End Display Control setting-------------------------*/
/*-----------------------------Bank0 Setting  End-----------------------------*/
/*-------------------------------Bank1 Setting--------------------------------*/
/*--------------------- Power Control Registers Initial ----------------------*/
	lcd_spi_write_cmd (0xFF);
	lcd_spi_write_data (0x77);
	lcd_spi_write_data (0x01);
	lcd_spi_write_data (0x00);
	lcd_spi_write_data (0x00);
	lcd_spi_write_data (0x11);
	lcd_spi_write_cmd (0xB0);
	lcd_spi_write_data (0x4D);
/*--------------------------------Vcom Setting--------------------------------*/
	lcd_spi_write_cmd (0xB1);
	lcd_spi_write_data (0x3E);
/*------------------------------End Vcom Setting------------------------------*/
	lcd_spi_write_cmd(0xB2);
	lcd_spi_write_data(0x07);
	lcd_spi_write_cmd(0xB3);
	lcd_spi_write_data(0x80);
	lcd_spi_write_cmd(0xB5);
	lcd_spi_write_data(0x47);
	lcd_spi_write_cmd(0xB7);
	lcd_spi_write_data(0x85);
	lcd_spi_write_cmd(0xB8);
	lcd_spi_write_data(0x21);
	lcd_spi_write_cmd(0xB9);
	lcd_spi_write_data(0x10);
	lcd_spi_write_cmd(0xC1);
	lcd_spi_write_data(0x78);
	lcd_spi_write_cmd(0xC2);
	lcd_spi_write_data(0x78);
	lcd_spi_write_cmd(0xD0);
	lcd_spi_write_data(0x88);
/*--------------------End Power Control Registers Initial --------------------*/
	rtos_delay_milliseconds (10);
/*--------------------------------GIP Setting---------------------------------*/
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
	lcd_spi_write_data(0x34);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x34);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0xAA);
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
	lcd_spi_write_data(0x34);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x34);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0xAA);
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
/*------------------------------End GIP Setting-------------------------------*/
/*-------------------- Power Control Registers Initial End--------------------*/
/*-------------------------------Bank1 Setting--------------------------------*/
	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0x29);

}

static void lcd_st7710s_backlight_io_init(void)
{
	gpio_dev_unmap(LCD_BACKLIGHT_CTRL_GPIO);
	bk_gpio_set_capacity(LCD_BACKLIGHT_CTRL_GPIO, 0);
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_BACKLIGHT_CTRL_GPIO));
	BK_LOG_ON_ERR(bk_gpio_pull_down(LCD_BACKLIGHT_CTRL_GPIO));
}

static void lcd_st7710s_init(void)
{
	lcd_st7710s_backlight_io_init();
	lcd_spi_init_gpio();
	lcd_st7710s_config();
}

static void st7710s_lcd_backlight_open(void)
{
	BK_LOG_ON_ERR(bk_gpio_pull_up(LCD_BACKLIGHT_CTRL_GPIO));
	// pull up gpio, enable lcd backlight control
	bk_gpio_set_output_high(LCD_BACKLIGHT_CTRL_GPIO);
}

static void st7710s_lcd_backlight_close(void)
{
	bk_gpio_set_output_low(LCD_BACKLIGHT_CTRL_GPIO);
}

const lcd_device_t lcd_device_st7710s =
{
	.id = LCD_DEVIDE_ST7710S,
	.name = "st7710s",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X800,
	.rgb = &lcd_rgb,
	.backlight_open = st7710s_lcd_backlight_open,
	.backlight_set = NULL,
	.init = lcd_st7710s_init,
	.backlight_close = st7710s_lcd_backlight_close,
	.lcd_off = NULL,
};

