#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <driver/spi.h>
#include <driver/gpio.h>
#include "bk_misc.h"
#include <driver/hal/hal_gpio_types.h>

#define LCD_SPI_ID        1
#define LCD_SPI_CLK_GPIO  GPIO_2
#define LCD_SPI_CSX_GPIO  GPIO_3
#define LCD_SPI_SDA_GPIO  GPIO_4
#define LCD_SPI_DELAY     10


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

		delay_us(LCD_SPI_DELAY);
		data <<= 1;

		bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
		delay_us(LCD_SPI_DELAY);
		bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
		delay_us(LCD_SPI_DELAY);
		
	}
	GLOBAL_INT_RESTORE();
}

void lcd_spi_write_cmd(uint8_t cmd)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_low(LCD_SPI_SDA_GPIO);
	delay_us(LCD_SPI_DELAY);

	bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);

	SPI_SendData(cmd);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
}

void lcd_spi_write_data(uint8_t data)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_high(LCD_SPI_SDA_GPIO);
	delay_us(LCD_SPI_DELAY);

	bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);

	SPI_SendData(data);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
}


void lcd_spi_init_gpio(void)
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


