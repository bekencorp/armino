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


static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_26M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,

	.hsync_back_porch = 40,  //3
	.hsync_front_porch = 5,  //2
	.vsync_back_porch = 8,   //1
	.vsync_front_porch = 8,  //1
};

static void lcd_gc9503v_config(void)
{
	lcd_spi_write_cmd(0xF0);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0xAA);
	lcd_spi_write_data(0x52);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x00);

	lcd_spi_write_cmd(0xF6);
	lcd_spi_write_data(0x5A);
	lcd_spi_write_data(0x87);

	lcd_spi_write_cmd(0xC1);
	lcd_spi_write_data(0x3F);

	lcd_spi_write_cmd(0xC2);
	lcd_spi_write_data(0x0E);

	lcd_spi_write_cmd(0xC6);
	lcd_spi_write_data(0xF8);

	lcd_spi_write_cmd(0xC9);
	lcd_spi_write_data(0x10);

	lcd_spi_write_cmd(0xCD);
	lcd_spi_write_data(0x25);

	lcd_spi_write_cmd(0xF8);
	lcd_spi_write_data(0x8A);
	lcd_spi_write_data(0xAC);
	lcd_spi_write_data(0x65);

	lcd_spi_write_cmd(0xA0);
	lcd_spi_write_data(0xBB);
	lcd_spi_write_cmd(0xA7);
	lcd_spi_write_data(0x47);

	lcd_spi_write_cmd(0xFA);
	lcd_spi_write_data(0x0F);
	lcd_spi_write_data(0x0F);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x04);

	lcd_spi_write_cmd(0xA3);
	lcd_spi_write_data(0xEE);

	lcd_spi_write_cmd(0xFD);
	lcd_spi_write_data(0x28);
	lcd_spi_write_data(0x28);
	lcd_spi_write_data(0x00);

	lcd_spi_write_cmd(0x71);
	lcd_spi_write_data(0x48);

	lcd_spi_write_cmd(0x72);
	lcd_spi_write_data(0x48);

	lcd_spi_write_cmd(0x73);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x44);

	lcd_spi_write_cmd(0x97);
	lcd_spi_write_data(0xEE);

	lcd_spi_write_cmd(0x83);
	lcd_spi_write_data(0x93);

	lcd_spi_write_cmd(0x9A);
	lcd_spi_write_data(0x90);

	lcd_spi_write_cmd(0x9B);
	lcd_spi_write_data(0xC4);

	lcd_spi_write_cmd(0x82);
	lcd_spi_write_data(0x73);
	lcd_spi_write_data(0x73);

	lcd_spi_write_cmd(0xB1);
	lcd_spi_write_data(0x90);// 0x90

	lcd_spi_write_cmd(0x6D);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1F);
	lcd_spi_write_data(0x19);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x0C);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x0E);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x1E);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x0D);
	lcd_spi_write_data(0x0F);
	lcd_spi_write_data(0x09);
	lcd_spi_write_data(0x0B);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x19);
	lcd_spi_write_data(0x1F);
	lcd_spi_write_data(0x00);

	lcd_spi_write_cmd(0x64);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x21);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x65);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x22);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x23);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x66);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x04);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x24);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x25);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x67);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x26);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x27);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x68);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x1B);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x1B);
	lcd_spi_write_data(0x60);
	lcd_spi_write_data(0x70);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x1B);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x1B);
	lcd_spi_write_data(0x60);
	lcd_spi_write_data(0x70);

	lcd_spi_write_cmd(0x60);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x0C);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x0B);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x61);
	lcd_spi_write_data(0x18);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x18);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x62);
	lcd_spi_write_data(0x18);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x18);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x7A);
	
	lcd_spi_write_cmd(0x63);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x7A);
	lcd_spi_write_data(0x38);
	lcd_spi_write_data(0x09);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x7A);

	lcd_spi_write_cmd(0x69);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x08);

	lcd_spi_write_cmd(0x6B);
	lcd_spi_write_data(0x07);
	
	lcd_spi_write_cmd(0x7A);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x13);

	lcd_spi_write_cmd(0x7B);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x13);

	lcd_spi_write_cmd(0xD1);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x68);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x86);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0xBA);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x2E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8D);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0xE5);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x46);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x72);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x98);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xCA);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xE0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xF5);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFD);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFF);

	lcd_spi_write_cmd(0xD2);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x68);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x86);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0xBA);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x2E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8D);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0xE5);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x46);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x72);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x98);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xCA);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xE0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xF5);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFD);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFF);

	lcd_spi_write_cmd(0xD3);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x68);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x86);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0xBA);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x2E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8D);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0xE5);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x46);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x72);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x98);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xCA);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xE0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xF5);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFD);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFF);

	lcd_spi_write_cmd(0xD4);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x68);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x86);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0xBA);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x2E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8D);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0xE5);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x46);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x72);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x98);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xCA);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xE0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xF5);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFD);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFF);

	lcd_spi_write_cmd(0xD5);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x68);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x86);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0xBA);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x2E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8D);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0xE5);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x46);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x72);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x98);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xCA);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xE0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xF5);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFD);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFF);

	lcd_spi_write_cmd(0xD6);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x05);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x1A);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x20);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x3B);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x55);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x68);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x86);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0xBA);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x2E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8D);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x8E);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0xE5);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0x46);
	lcd_spi_write_data(0x02);
	lcd_spi_write_data(0xA0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x72);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0x98);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xCA);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xE0);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xF5);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFD);
	lcd_spi_write_data(0x03);
	lcd_spi_write_data(0xFF);

	lcd_spi_write_cmd(0x3A);
	lcd_spi_write_data(0x55);

	lcd_spi_write_cmd(0x11);
	delay_ms(20);
	lcd_spi_write_cmd(0x29);
	delay_ms(10);
}

static void lcd_gc9503v_reset(void)
{
	BK_LOG_ON_ERR(bk_gpio_disable_input(34));
	BK_LOG_ON_ERR(bk_gpio_enable_output(34));
	// pull up gpio34, enable lcd backlight control
	bk_gpio_set_output_low(34);
	delay_ms(15);
	bk_gpio_set_output_high(34);

	BK_LOG_ON_ERR(bk_gpio_enable_output(8));
	bk_gpio_set_output_high(8);
	delay_ms(15);
	bk_gpio_set_output_low(8);

	BK_LOG_ON_ERR(bk_gpio_enable_output(6));
	bk_gpio_set_output_low(6);
	delay_ms(15);
	bk_gpio_set_output_high(6);
	delay_ms(150);
}



void lcd_gc9503v_init(void)
{
	lcd_gc9503v_reset();
	lcd_spi_init_gpio();
	lcd_gc9503v_config();
}
const lcd_device_t lcd_device_gc9503v =
{
	.id = LCD_DEVICE_GC9503V,
	.name = "gc9503v",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X800,
	.rgb = &lcd_rgb,
	.init = lcd_gc9503v_init,
};

