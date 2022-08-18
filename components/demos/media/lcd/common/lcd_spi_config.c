#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <driver/spi.h>
#include <driver/gpio.h>
#include "bk_misc.h"
#include <driver/gpio.h>

#define LCD_SPI_ID        1
#define LCD_SPI_CLK_GPIO  2
#define LCD_SPI_CSX_GPIO  3
#define LCD_SPI_SDA_GPIO  4

static uint32_t delay_time = 10;


static void SPI_SendData(uint8_t data)
{
	uint8_t n;

	//in while loop, to avoid disable IRQ too much time, release it if finish one byte.
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	for(n = 0; n < 8; n++) {
		if (data & 0x80)
			bk_gpio_set_output_high(LCD_SPI_SDA_GPIO);
		else
			bk_gpio_set_output_low(LCD_SPI_SDA_GPIO);

		delay_us(delay_time);
		data <<= 1;

		bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
		delay_us(delay_time);
		bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
		delay_us(delay_time);
		
	}
	GLOBAL_INT_RESTORE();
}

static void SPI_WriteComm(uint8_t data)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(delay_time);
	bk_gpio_set_output_low(LCD_SPI_SDA_GPIO);
	delay_us(delay_time);

	bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
	delay_us(delay_time);
	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	delay_us(delay_time);

	SPI_SendData(data);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(delay_time);
}

static void SPI_WriteData(uint8_t data)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(delay_time);
	bk_gpio_set_output_high(LCD_SPI_SDA_GPIO);
	delay_us(delay_time);

	bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
	delay_us(delay_time);
	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	delay_us(delay_time);

	SPI_SendData(data);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(delay_time);
}

void lcd_set_spi_delay_time(uint32_t time)
{
	delay_time = time;
}

void lcd_init_gpio(void)
{
//	BK_LOG_ON_ERR(bk_gpio_disable_input(34));
//	BK_LOG_ON_ERR(bk_gpio_enable_output(34));
//	// pull up gpio34, enable lcd backlight control
//	bk_gpio_set_output_low(34);
//	delay_ms(15);
//	bk_gpio_set_output_high(34);

	BK_LOG_ON_ERR(bk_gpio_disable_input(37));
	BK_LOG_ON_ERR(bk_gpio_enable_output(37));
	// pull up gpio37, enable uvc camera vol
	bk_gpio_set_output_low(37);
	delay_ms(15);
	bk_gpio_set_output_high(37);

	BK_LOG_ON_ERR(bk_gpio_disable_input(8));
	BK_LOG_ON_ERR(bk_gpio_enable_output(8));
	// pull up gpio8, enbale pa and TF card vol
	bk_gpio_set_output_high(8);
	delay_ms(15);
	bk_gpio_set_output_low(8);

//	BK_LOG_ON_ERR(bk_gpio_disable_input(39));
//	BK_LOG_ON_ERR(bk_gpio_enable_output(39));
//	// pull up gpio39, enable audio pa vol
//	bk_gpio_set_output_low(39);
//	delay_ms(15);
//	bk_gpio_set_output_high(39);

	BK_LOG_ON_ERR(bk_gpio_disable_input(6));
	BK_LOG_ON_ERR(bk_gpio_enable_output(6));
	bk_gpio_set_output_low(6);
	bk_gpio_set_output_high(6);
	bk_gpio_set_output_low(6);
	delay_ms(15);
	bk_gpio_set_output_high(6);
	delay_ms(150);
}

void spi_init_gpio(void)
{
	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_CLK_GPIO));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_CLK_GPIO));

	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_CSX_GPIO));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_CSX_GPIO));

	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_SDA_GPIO));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_SDA_GPIO));

	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(200);
}

void lcd_gc9503v_set_config(void)
{
	SPI_WriteComm(0xF0);
	SPI_WriteData(0x55);
	SPI_WriteData(0xAA);
	SPI_WriteData(0x52);
	SPI_WriteData(0x08);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xF6);
	SPI_WriteData(0x5A);
	SPI_WriteData(0x87);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x3F);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x0E);

	SPI_WriteComm(0xC6);
	SPI_WriteData(0xF8);

	SPI_WriteComm(0xC9);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xCD);
	SPI_WriteData(0x25);

	SPI_WriteComm(0xF8);
	SPI_WriteData(0x8A);
	SPI_WriteData(0xAC);
	SPI_WriteData(0x65);

	SPI_WriteComm(0xA0);
	SPI_WriteData(0xBB);
	SPI_WriteComm(0xA7);
	SPI_WriteData(0x47);

	SPI_WriteComm(0xFA);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x00);
	SPI_WriteData(0x04);

	SPI_WriteComm(0xA3);
	SPI_WriteData(0xEE);

	SPI_WriteComm(0xFD);
	SPI_WriteData(0x28);
	SPI_WriteData(0x28);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x71);
	SPI_WriteData(0x48);

	SPI_WriteComm(0x72);
	SPI_WriteData(0x48);

	SPI_WriteComm(0x73);
	SPI_WriteData(0x00);
	SPI_WriteData(0x44);

	SPI_WriteComm(0x97);
	SPI_WriteData(0xEE);

	SPI_WriteComm(0x83);
	SPI_WriteData(0x93);

	SPI_WriteComm(0x9A);
	SPI_WriteData(0x90);

	SPI_WriteComm(0x9B);
	SPI_WriteData(0xC4);

	SPI_WriteComm(0x82);
	SPI_WriteData(0x73);
	SPI_WriteData(0x73);

	SPI_WriteComm(0xB1);
	SPI_WriteData(0x90);// 0xB0

	SPI_WriteComm(0x6D);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1F);
	SPI_WriteData(0x19);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x10);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x02);
	SPI_WriteData(0x08);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x1E);
	SPI_WriteData(0x07);
	SPI_WriteData(0x01);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x09);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x19);
	SPI_WriteData(0x1F);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x64);
	SPI_WriteData(0x38);
	SPI_WriteData(0x08);
	SPI_WriteData(0x03);
	SPI_WriteData(0x20);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x38);
	SPI_WriteData(0x07);
	SPI_WriteData(0x03);
	SPI_WriteData(0x21);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x65);
	SPI_WriteData(0x38);
	SPI_WriteData(0x06);
	SPI_WriteData(0x03);
	SPI_WriteData(0x22);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x38);
	SPI_WriteData(0x05);
	SPI_WriteData(0x03);
	SPI_WriteData(0x23);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x66);
	SPI_WriteData(0x38);
	SPI_WriteData(0x04);
	SPI_WriteData(0x03);
	SPI_WriteData(0x24);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x38);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x25);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x67);
	SPI_WriteData(0x38);
	SPI_WriteData(0x02);
	SPI_WriteData(0x03);
	SPI_WriteData(0x26);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x38);
	SPI_WriteData(0x01);
	SPI_WriteData(0x03);
	SPI_WriteData(0x27);
	SPI_WriteData(0x03);
	SPI_WriteData(0x03);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x68);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x08);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x08);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x60);
	SPI_WriteData(0x70);
	SPI_WriteData(0x08);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x08);
	SPI_WriteData(0x1B);
	SPI_WriteData(0x60);
	SPI_WriteData(0x70);

	SPI_WriteComm(0x60);
	SPI_WriteData(0x38);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x08);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x38);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x08);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x61);
	SPI_WriteData(0x18);
	SPI_WriteData(0x00);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x18);
	SPI_WriteData(0x00);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x62);
	SPI_WriteData(0x18);
	SPI_WriteData(0x00);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x18);
	SPI_WriteData(0x00);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x7A);
	
	SPI_WriteComm(0x63);
	SPI_WriteData(0x38);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x08);
	SPI_WriteData(0x7A);
	SPI_WriteData(0x38);
	SPI_WriteData(0x09);
	SPI_WriteData(0x08);
	SPI_WriteData(0x7A);

	SPI_WriteComm(0x69);
	SPI_WriteData(0x01);
	SPI_WriteData(0x11);
	SPI_WriteData(0x01);
	SPI_WriteData(0x11);
	SPI_WriteData(0x01);
	SPI_WriteData(0x11);
	SPI_WriteData(0x08);

	SPI_WriteComm(0x6B);
	SPI_WriteData(0x07);
	
	SPI_WriteComm(0x7A);
	SPI_WriteData(0x08);
	SPI_WriteData(0x13);

	SPI_WriteComm(0x7B);
	SPI_WriteData(0x08);
	SPI_WriteData(0x13);

	SPI_WriteComm(0xD1);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x00);
	SPI_WriteData(0x30);
	SPI_WriteData(0x00);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x55);
	SPI_WriteData(0x00);
	SPI_WriteData(0x68);
	SPI_WriteData(0x00);
	SPI_WriteData(0x86);
	SPI_WriteData(0x00);
	SPI_WriteData(0xBA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x2E);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8D);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8E);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE5);
	SPI_WriteData(0x02);
	SPI_WriteData(0x46);
	SPI_WriteData(0x02);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x30);
	SPI_WriteData(0x03);
	SPI_WriteData(0x72);
	SPI_WriteData(0x03);
	SPI_WriteData(0x98);
	SPI_WriteData(0x03);
	SPI_WriteData(0xCA);
	SPI_WriteData(0x03);
	SPI_WriteData(0xE0);
	SPI_WriteData(0x03);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFD);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFF);

	SPI_WriteComm(0xD2);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x00);
	SPI_WriteData(0x30);
	SPI_WriteData(0x00);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x55);
	SPI_WriteData(0x00);
	SPI_WriteData(0x68);
	SPI_WriteData(0x00);
	SPI_WriteData(0x86);
	SPI_WriteData(0x00);
	SPI_WriteData(0xBA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x2E);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8D);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8E);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE5);
	SPI_WriteData(0x02);
	SPI_WriteData(0x46);
	SPI_WriteData(0x02);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x30);
	SPI_WriteData(0x03);
	SPI_WriteData(0x72);
	SPI_WriteData(0x03);
	SPI_WriteData(0x98);
	SPI_WriteData(0x03);
	SPI_WriteData(0xCA);
	SPI_WriteData(0x03);
	SPI_WriteData(0xE0);
	SPI_WriteData(0x03);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFD);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFF);

	SPI_WriteComm(0xD3);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x00);
	SPI_WriteData(0x30);
	SPI_WriteData(0x00);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x55);
	SPI_WriteData(0x00);
	SPI_WriteData(0x68);
	SPI_WriteData(0x00);
	SPI_WriteData(0x86);
	SPI_WriteData(0x00);
	SPI_WriteData(0xBA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x2E);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8D);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8E);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE5);
	SPI_WriteData(0x02);
	SPI_WriteData(0x46);
	SPI_WriteData(0x02);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x30);
	SPI_WriteData(0x03);
	SPI_WriteData(0x72);
	SPI_WriteData(0x03);
	SPI_WriteData(0x98);
	SPI_WriteData(0x03);
	SPI_WriteData(0xCA);
	SPI_WriteData(0x03);
	SPI_WriteData(0xE0);
	SPI_WriteData(0x03);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFD);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFF);

	SPI_WriteComm(0xD4);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x00);
	SPI_WriteData(0x30);
	SPI_WriteData(0x00);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x55);
	SPI_WriteData(0x00);
	SPI_WriteData(0x68);
	SPI_WriteData(0x00);
	SPI_WriteData(0x86);
	SPI_WriteData(0x00);
	SPI_WriteData(0xBA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x2E);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8D);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8E);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE5);
	SPI_WriteData(0x02);
	SPI_WriteData(0x46);
	SPI_WriteData(0x02);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x30);
	SPI_WriteData(0x03);
	SPI_WriteData(0x72);
	SPI_WriteData(0x03);
	SPI_WriteData(0x98);
	SPI_WriteData(0x03);
	SPI_WriteData(0xCA);
	SPI_WriteData(0x03);
	SPI_WriteData(0xE0);
	SPI_WriteData(0x03);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFD);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFF);

	SPI_WriteComm(0xD5);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x00);
	SPI_WriteData(0x30);
	SPI_WriteData(0x00);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x55);
	SPI_WriteData(0x00);
	SPI_WriteData(0x68);
	SPI_WriteData(0x00);
	SPI_WriteData(0x86);
	SPI_WriteData(0x00);
	SPI_WriteData(0xBA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x2E);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8D);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8E);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE5);
	SPI_WriteData(0x02);
	SPI_WriteData(0x46);
	SPI_WriteData(0x02);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x30);
	SPI_WriteData(0x03);
	SPI_WriteData(0x72);
	SPI_WriteData(0x03);
	SPI_WriteData(0x98);
	SPI_WriteData(0x03);
	SPI_WriteData(0xCA);
	SPI_WriteData(0x03);
	SPI_WriteData(0xE0);
	SPI_WriteData(0x03);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFD);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFF);

	SPI_WriteComm(0xD6);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x05);
	SPI_WriteData(0x00);
	SPI_WriteData(0x10);
	SPI_WriteData(0x00);
	SPI_WriteData(0x1A);
	SPI_WriteData(0x00);
	SPI_WriteData(0x20);
	SPI_WriteData(0x00);
	SPI_WriteData(0x30);
	SPI_WriteData(0x00);
	SPI_WriteData(0x3B);
	SPI_WriteData(0x00);
	SPI_WriteData(0x55);
	SPI_WriteData(0x00);
	SPI_WriteData(0x68);
	SPI_WriteData(0x00);
	SPI_WriteData(0x86);
	SPI_WriteData(0x00);
	SPI_WriteData(0xBA);
	SPI_WriteData(0x01);
	SPI_WriteData(0x2E);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8D);
	SPI_WriteData(0x01);
	SPI_WriteData(0x8E);
	SPI_WriteData(0x01);
	SPI_WriteData(0xE5);
	SPI_WriteData(0x02);
	SPI_WriteData(0x46);
	SPI_WriteData(0x02);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x03);
	SPI_WriteData(0x00);
	SPI_WriteData(0x03);
	SPI_WriteData(0x30);
	SPI_WriteData(0x03);
	SPI_WriteData(0x72);
	SPI_WriteData(0x03);
	SPI_WriteData(0x98);
	SPI_WriteData(0x03);
	SPI_WriteData(0xCA);
	SPI_WriteData(0x03);
	SPI_WriteData(0xE0);
	SPI_WriteData(0x03);
	SPI_WriteData(0xF5);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFD);
	SPI_WriteData(0x03);
	SPI_WriteData(0xFF);

	SPI_WriteComm(0x3A);
	SPI_WriteData(0x55);

	SPI_WriteComm(0x11);
	delay_ms(20);
	SPI_WriteComm(0x29);
	delay_ms(10); 
}

void lcd_st7701s_set_config(void)
{
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
	SPI_WriteData(0x63);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x10);
	SPI_WriteData(0x02);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x01);
	SPI_WriteData(0x08);

	SPI_WriteComm(0xCC);
	SPI_WriteData(0x18);

	SPI_WriteComm(0xB0);
	SPI_WriteData(0x40);
	SPI_WriteData(0xC9);
	SPI_WriteData(0x8F);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x11);
	SPI_WriteData(0x07);
	SPI_WriteData(0x02);
	SPI_WriteData(0x09);
	SPI_WriteData(0x09);
	SPI_WriteData(0x1F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x50);
	SPI_WriteData(0x0F);
	SPI_WriteData(0xE4);
	SPI_WriteData(0x29);
	SPI_WriteData(0xDF);

	SPI_WriteComm(0xB1);
	SPI_WriteData(0x40);
	SPI_WriteData(0xCB);
	SPI_WriteData(0xD3);
	SPI_WriteData(0x11);
	SPI_WriteData(0x8F);
	SPI_WriteData(0x04);
	SPI_WriteData(0x00);
	SPI_WriteData(0x08);
	SPI_WriteData(0x07);
	SPI_WriteData(0x1C);
	SPI_WriteData(0x06);
	SPI_WriteData(0x53);
	SPI_WriteData(0x12);
	SPI_WriteData(0x63);
	SPI_WriteData(0xEB);
	SPI_WriteData(0xDF);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x11);

	SPI_WriteComm(0xB0);
	SPI_WriteData(0x4A);  //65

	SPI_WriteComm(0xB1);
	SPI_WriteData(0x4D);   //34

	SPI_WriteComm(0xB2);
	SPI_WriteData(0x87);

	SPI_WriteComm(0xB3);
	SPI_WriteData(0x80);

	SPI_WriteComm(0xB5);
	SPI_WriteData(0x49);

	SPI_WriteComm(0xB7);
	SPI_WriteData(0x85);

	SPI_WriteComm(0xB8);
	SPI_WriteData(0x20);

	SPI_WriteComm(0xB9);
	SPI_WriteData(0x10);

	SPI_WriteComm(0xC1);
	SPI_WriteData(0x78);

	SPI_WriteComm(0xC2);
	SPI_WriteData(0x78);

	SPI_WriteComm(0xD0);
	SPI_WriteData(0x88);
	delay_ms(10);

	SPI_WriteComm(0xE0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x19);
	SPI_WriteData(0x02);

	SPI_WriteComm(0xE1);
	SPI_WriteData(0x05);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x07);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x04);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x06);
	SPI_WriteData(0xA0);
	SPI_WriteData(0x00);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);

	SPI_WriteComm(0xE2);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xE3);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x33);

	SPI_WriteComm(0xE4);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);

	SPI_WriteComm(0xE5);
	SPI_WriteData(0x0D);
	SPI_WriteData(0x31);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);
	SPI_WriteData(0x0F);
	SPI_WriteData(0x33);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);
	SPI_WriteData(0x09);
	SPI_WriteData(0x2D);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);
	SPI_WriteData(0x0B);
	SPI_WriteData(0x2F);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);

	SPI_WriteComm(0xE6);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x33);
	SPI_WriteData(0x33);

	SPI_WriteComm(0xE7);
	SPI_WriteData(0x44);
	SPI_WriteData(0x44);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x0C);
	SPI_WriteData(0x30);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);
	SPI_WriteData(0x0E);
	SPI_WriteData(0x32);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);
	SPI_WriteData(0x08);
	SPI_WriteData(0x2C);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);
	SPI_WriteData(0x0A);
	SPI_WriteData(0x2E);
	SPI_WriteData(0xC8);
	SPI_WriteData(0xAF);

	SPI_WriteComm(0xEB);
	SPI_WriteData(0x02);
	SPI_WriteData(0x00);
	SPI_WriteData(0xE4);
	SPI_WriteData(0xE4);
	SPI_WriteData(0x44);
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
	SPI_WriteData(0x01);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0xFF);
	SPI_WriteData(0x10);
	SPI_WriteData(0x45);
	SPI_WriteData(0x67);
	SPI_WriteData(0x98);
	SPI_WriteData(0xBA);

	SPI_WriteComm(0xEF);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x08);
	SPI_WriteData(0x45);
	SPI_WriteData(0x3F);
	SPI_WriteData(0x54);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x13);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0E);

	SPI_WriteComm(0x11);
	delay_ms(20);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x0C);
	delay_ms(10);

	SPI_WriteComm(0xE8);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0xFF);
	SPI_WriteData(0x77);
	SPI_WriteData(0x01);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

	SPI_WriteComm(0x3A);
	SPI_WriteData(0x55);

	SPI_WriteComm(0x29);
	delay_ms(10);
}


