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
#include <driver/lcd.h>


#if CONFIG_PWM
#include <driver/pwm.h>
#endif


static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_26M,
	.data_out_clk_edge = NEGEDGE_OUTPUT,

	.hsync_pulse_width = 2,
	.vsync_pulse_width = 2,
	.hsync_back_porch = 10, //40,  //3
	.hsync_front_porch = 10, //5,  //2
	.vsync_back_porch = 10, //8,   //1
	.vsync_front_porch = 10, //8,  //1
};

static void lcd_st7701s_config(void)
{
#if 0
	//--------------------------------------ST7701 Reset Sequence---------------------------------------//
	// bk_gpio_set_output_high(LCD_SPI_RST);
	// delay_ms(1);
	// bk_gpio_set_output_low(LCD_SPI_RST);
	// delay_ms(1);
	// bk_gpio_set_output_high(LCD_SPI_RST);
	rtos_delay_milliseconds(120);
	lcd_spi_write_cmd(0x11);
	rtos_delay_milliseconds(20);
	//---------------------------------------Bank0 Setting-------------------------------------------------//
	//------------------------------------Display Control setting----------------------------------------------//
	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x10);
	lcd_spi_write_cmd(0xC0);
	lcd_spi_write_data(0x63);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0xC1);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x02);
	lcd_spi_write_cmd(0xC2);
	lcd_spi_write_data(0x31);
	lcd_spi_write_data(0x08);
	lcd_spi_write_cmd(0xCC);
	lcd_spi_write_data(0x10);
	//-------------------------------------Gamma Cluster Setting-------------------------------------------//
	lcd_spi_write_cmd(0xB0);

	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x19);
	lcd_spi_write_data(0x0C);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x0A);
	lcd_spi_write_data(0x09);
	lcd_spi_write_data(0x22);
	lcd_spi_write_data(0x04);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x0E);
	lcd_spi_write_data(0x28);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x1C);
	lcd_spi_write_cmd(0xB1);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x12);
	lcd_spi_write_data(0x19);
	lcd_spi_write_data(0x0D);
	lcd_spi_write_data(0x10);
	lcd_spi_write_data(0x04);
	lcd_spi_write_data(0x06);
	lcd_spi_write_data(0x07);
	lcd_spi_write_data(0x08);
	lcd_spi_write_data(0x23);
	lcd_spi_write_data(0x04);
	lcd_spi_write_data(0x12);
	lcd_spi_write_data(0x11);
	lcd_spi_write_data(0x28);
	lcd_spi_write_data(0x30);
	lcd_spi_write_data(0x1C);
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
	lcd_spi_write_data(0x4D);
	//-------------------------------------------Vcom Setting---------------------------------------------------//
	lcd_spi_write_cmd(0xB1);
	lcd_spi_write_data(0x3E);
	//-----------------------------------------End Vcom Setting-----------------------------------------------//
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
	//--------------------------------------------End GIP Setting-----------------------------------------------//
	//------------------------------ Power Control Registers Initial End-----------------------------------//
	//------------------------------------------Bank1 Setting----------------------------------------------------//
	lcd_spi_write_cmd(0xFF);
	lcd_spi_write_data(0x77);
	lcd_spi_write_data(0x01);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_data(0x00);
	lcd_spi_write_cmd(0x29);
#else
	#define Delay rtos_delay_milliseconds
	#define SPI_WriteComm lcd_spi_write_cmd
	#define SPI_WriteData lcd_spi_write_data

	Delay(1);
	bk_gpio_set_output_low(LCD_SPI_RST);
	Delay(1);
	bk_gpio_set_output_high(LCD_SPI_RST);
	Delay(1);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);
	SPI_WriteComm(0xEF);
	SPI_WriteData(0x08);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteComm(0xC0);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteComm(0xC1);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x02);
	SPI_WriteComm(0xC2);
	SPI_WriteData(0x21);
	SPI_WriteData(0x08);

	SPI_WriteComm(0xCD);
	SPI_WriteData(0x08);//18-bit/pixel: MDT=0:D[21:16]=R,D[13:8]=G,D[5:0]=B(CDH=00) ;

					//              MDT=1:D[17:12]=R,D[11:6]=G,D[5:0]=B(CDH=08) ;


	SPI_WriteComm(0xB0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteData(0x18);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x11);
	SPI_WriteData(0x06);
	SPI_WriteData(0x07);
	SPI_WriteData(0x08);
	SPI_WriteData(0x07);
	SPI_WriteData(0x22);
	SPI_WriteData(0x04);
	SPI_WriteData(0x12);
	SPI_WriteData(0x0F);
	SPI_WriteData(0xAA);
	SPI_WriteData(0x31);
	SPI_WriteData(0x18);
	SPI_WriteComm(0xB1);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteData(0x19);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x12);
	SPI_WriteData(0x07);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x22);
	SPI_WriteData(0x04);
	SPI_WriteData(0x11);
	SPI_WriteData(0x11);
	SPI_WriteData(0xA9);
	SPI_WriteData(0x32);
	SPI_WriteData(0x18);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteComm(0xB0);
	SPI_WriteData(0x60);
	SPI_WriteComm(0xB1);
	SPI_WriteData(0x30);
	SPI_WriteComm(0xB2);
	SPI_WriteData(0x87);
	SPI_WriteComm(0xB3);
	SPI_WriteData(0x80);
	SPI_WriteComm(0xB5);
	SPI_WriteData(0x49);
	SPI_WriteComm(0xB7);
	SPI_WriteData(0x85);
	SPI_WriteComm(0xB8);
	SPI_WriteData(0x21);
	SPI_WriteComm(0xC1);
	SPI_WriteData(0x78);
	SPI_WriteComm(0xC2);
	SPI_WriteData(0x78);
	Delay(20);
	SPI_WriteComm(0xE0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x02);
	SPI_WriteComm(0xE1);
	SPI_WriteData(0x08);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x07);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);
	SPI_WriteComm(0xE2);
	SPI_WriteData(0x11);
	SPI_WriteData(0x11);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);
	SPI_WriteData(0xED);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0xEC);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteComm(0xE3);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteData(0x11);
	SPI_WriteComm(0xE4);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);
	SPI_WriteComm(0xE5);
	SPI_WriteData(0x0A);
	SPI_WriteData(0xE9);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0C);
	SPI_WriteData(0xEB);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0E);
	SPI_WriteData(0xED);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x10);
	SPI_WriteData(0xEF);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteComm(0xE6);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);
	SPI_WriteData(0x11);
	SPI_WriteComm(0xE7);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);
	SPI_WriteComm(0xE8);
	SPI_WriteData(0x09);
	SPI_WriteData(0xE8);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0B);
	SPI_WriteData(0xEA);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0D);
	SPI_WriteData(0xEC);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x0F);
	SPI_WriteData(0xEE);
	SPI_WriteData(0xD8);
	SPI_WriteData(0xA0);
	SPI_WriteComm(0xEB);
	SPI_WriteData(0x02);
	SPI_WriteData(0x00);
	SPI_WriteData(0xE4);
	SPI_WriteData(0xE4);
	SPI_WriteData(0x88);
	SPI_WriteData(0x00);
	SPI_WriteData(0x40);
	SPI_WriteComm(0xEC);
	SPI_WriteData(0x3C);
	SPI_WriteData(0x00);
	SPI_WriteComm(0xED);
	SPI_WriteData(0xAB);
	SPI_WriteData(0x89);
	SPI_WriteData(0x76);
	SPI_WriteData(0x54);
	SPI_WriteData(0x02);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0x20);
	SPI_WriteData(0x45);
	SPI_WriteData(0x67);
	SPI_WriteData(0x98);
	SPI_WriteData(0xBA);
	SPI_WriteComm(0xEF);
	SPI_WriteData(0x10);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x04);
	SPI_WriteData(0x08);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x1F);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x3A);
	SPI_WriteData(0x66);//55/50=16bit(RGB565);66=18bit(RGB666);77或默认不写3AH是=24bit(RGB888)

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);
	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0E);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteComm(0x11);
	Delay(120);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);
	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0C);
	Delay(10);
	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x36);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x21);

	SPI_WriteComm(0x29);
	Delay(20);
#endif
}

static void lcd_st7701s_init(void)
{
#if 1
    os_printf("lcd_st7701s: init.\r\n");
#endif
	lcd_spi_init_gpio();
	lcd_st7701s_config();
}

static void st7701s_lcd_backlight_open(void)
{
	os_printf("lcd_st7701s: backlight open.\r\n");
#if CONFIG_PWM
	lcd_driver_backlight_init(LCD_RGB_PWM_BACKLIGHT, 100);
#endif
}

static void lcd_set_backlight(uint8_t percent)
{
#if CONFIG_PWM
	pwm_period_duty_config_t config = {0};

	if (percent > 100)
	{
		percent  = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;

	bk_pwm_set_period_duty(LCD_RGB_PWM_BACKLIGHT, &config);
#endif
}

static void st7701s_lcd_backlight_close(void)
{
	os_printf("lcd_st7701s: backlight close.\r\n");
#if CONFIG_PWM
	lcd_driver_backlight_deinit(LCD_RGB_PWM_BACKLIGHT);
#endif
}

const lcd_device_t lcd_device_st7701s =
{
	.id = LCD_DEVICE_ST7701S,
	.name = "st7701s",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X480,
	.rgb = &lcd_rgb,
	.backlight_open = st7701s_lcd_backlight_open,
	.backlight_set = lcd_set_backlight,
	.init = lcd_st7701s_init,
	.backlight_close = st7701s_lcd_backlight_close,
	.lcd_off = NULL,
};

