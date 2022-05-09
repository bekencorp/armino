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

#include <stdlib.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include "arch_interrupt.h"
#include "lcd_disp_ll_macro_def.h"
#include "lcd_disp_hal.h"
#include <driver/lcd.h>
#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "gpio_map.h"
#include <driver/int.h>
#include "dma_hal.h"
#include <driver/dma.h>
#include "dma_driver.h"
#include "sys_driver.h"

typedef struct {
	lcd_isr_t lcd_8080_frame_start_handler;
	lcd_isr_t lcd_8080_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_start_handler;
} lcd_driver_t;

static lcd_driver_t s_lcd = {0};
static void lcd_isr(void);

static bk_err_t bk_lcd_8080_gpio_init(void)
{
	bk_err_t ret = BK_OK;
	lcd_gpio_map_t lcd_gpio_map_table[] = GPIO_LCD_8080_GPIO_MAP;

	for (uint32_t i = 0; i < GPIO_LCD_8080_USED_GPIO_NUM; i++) {
		ret = gpio_dev_unmap(lcd_gpio_map_table[i].gpio_id);
		if (ret != BK_OK) {
			os_printf("lcd gpio unmap error \r\n");
			return BK_FAIL;
		}
		ret = gpio_dev_map(lcd_gpio_map_table[i].gpio_id, lcd_gpio_map_table[i].dev);
		if (ret != BK_OK) {
			os_printf("lcd gpio map error \r\n");
			return BK_FAIL;
		}
	}
	return BK_OK;
}

static bk_err_t bk_lcd_rgb_gpio_init(void)
{
	bk_err_t ret = BK_OK;

	lcd_gpio_map_t lcd_gpio_map_table[] = GPIO_LCD_RGB_GPIO_MAP;
	for (uint32_t i = 0; i < GPIO_LCD_RGB_GPIO_NUM; i++) {
		ret = gpio_dev_unmap(lcd_gpio_map_table[i].gpio_id);
		if (ret != BK_OK) {
			os_printf("lcd gpio map error \r\n");
			return BK_FAIL;
		}
		ret = gpio_dev_map(lcd_gpio_map_table[i].gpio_id, lcd_gpio_map_table[i].dev);
		if (ret != BK_OK) {
			os_printf("lcd gpio map error \r\n");
			return BK_FAIL;
		}
	}
	return BK_OK;
}


bk_err_t bk_lcd_driver_init(lcd_clk_t clk)
{
	bk_err_t ret = BK_OK;

	/* 0:clk_320M  1:clk_480M */
	/*  Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2)*/
	//sys_drv_lcd_set(clk_src_sel, clk_div_l, clk_div_h, 1, 0);
	if (clk == LCD_20M) {
		ret = sys_drv_lcd_set(0, 1, 7, 1, 0);
	}else if (clk == LCD_30M) {
		ret = sys_drv_lcd_set(1, 1, 7, 1, 0);
	} else if(clk == LCD_40M) {
		ret = sys_drv_lcd_set(1, 1, 5, 1, 0);
	} else if(clk == LCD_48M) {
		ret = sys_drv_lcd_set(1, 1, 4, 1, 0);
	} else if(clk == LCD_60M) {
		ret = sys_drv_lcd_set(1, 1, 3, 1, 0);
	} else if(clk == LCD_80M) {
		ret = sys_drv_lcd_set(1, 1, 2, 1, 0);
	} else if(clk == LCD_96M) {
		ret = sys_drv_lcd_set(1, 0, 2, 1, 0);
	} else if(clk == LCD_160M) {
		ret = sys_drv_lcd_set(1, 0, 1, 1, 0);
	} else if (clk == LCD_240M) {
		ret = sys_drv_lcd_set(1, 1, 0, 1, 0);
	} else {
		ret = sys_drv_lcd_set(1, 0, 2, 1, 0);
	}
	if (ret != BK_OK) {
		os_printf("lcd system reg config error \r\n");
		return BK_FAIL;
	}

	os_memset(&s_lcd, 0, sizeof(s_lcd));
	bk_int_isr_register(INT_SRC_LCD, lcd_isr, NULL);
	return BK_OK;
}

static void bk_lcd_8080_reset(void)
{
	hal_lcd_8080_reset();
	hal_lcd_8080_unreset();
}

/**
 * @brief This API config lcd display x size and y size
 
 * @param
 *     - width lcd display width
 *     - height lcd display height
 *
 * attention 1. int the next version, the width and height deside the transfer number of lcd display.
 *              will config with another two register x offset and y offset
 *
 * attention 2. in this sdk version width/height only set once in 8080_init,if you want set twice,should 
                set bk_lcd_8080_display_enable(0)
 */
bk_err_t bk_lcd_pixel_config(uint16_t x_pixel, uint16_t y_pixel)
{
	lcd_hal_pixel_config(x_pixel, y_pixel);
	return BK_OK;
}



bk_err_t bk_lcd_8080_init(display_pixel_format_t x_pixel, display_pixel_format_t y_pixel)
{
#if(0)
	bk_lcd_8080_gpio_init();
	
	lcd_hal_mem_clr();
	lcd_hal_disconti_mode(1);
	lcd_hal_8080_set_fifo_data_thrd(96,96);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	lcd_hal_8080_display_enable(1);
	lcd_hal_8080_display_enable(0);
	bk_lcd_8080_reset();
#else
	lcd_hal_8080_display_enable(1);
	bk_lcd_8080_reset();
	bk_lcd_8080_gpio_init();
	lcd_hal_mem_clr();
	lcd_hal_disconti_mode(1);
	lcd_hal_8080_set_fifo_data_thrd(96,96);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	lcd_hal_8080_int_enable(0, 1);
	return BK_OK;
#endif
}

bk_err_t bk_lcd_8080_deinit(void)
{

	bk_int_isr_unregister(INT_SRC_LCD);
	bk_lcd_8080_reset();
	lcd_hal_8080_int_enable(0, 0);
	lcd_hal_8080_display_enable(0);
	lcd_hal_8080_start_transfer(0);
	lcd_hal_mem_clr();
	if(sys_drv_lcd_close() != 0) {
		os_printf("lcd system deinit reg config error \r\n");
		return BK_FAIL;
	}
	return BK_OK;
}
bk_err_t bk_lcd_rgb_init(uint32_t rgb_clk_div, display_pixel_format_t x_pixel, display_pixel_format_t y_pixel, rgb_input_data_format_t input_data_format) 
{

//	uint8_t vsync_front_porch = 8; 
//	uint8_t vsync_back_porch = 8;
//	uint8_t hsync_front_porch = 5;
//	uint8_t hsync_back_porch = 40;

	bk_lcd_rgb_gpio_init();
	lcd_hal_mem_clr();
	lcd_hal_disconti_mode(1);
	lcd_hal_set_rgb_clk_div(rgb_clk_div);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	lcd_hal_rgb_display_sel(1);
	lcd_hal_rgb_set_thrd(0x60, 0x60);
	//lcd_hal_rgb_sync_config(hsync_back_porch, hsync_front_porch, vsync_back_porch, vsync_front_porch);
	lcd_hal_rgb_sync_config(40, 5, 8, 8);
	lcd_hal_rgb_yuv_sel(input_data_format);
	lcd_hal_rgb_int_enable(0, 1);
	return BK_OK;
}

bk_err_t bk_lcd_rgb_display_en(bool en)
{
	lcd_hal_rgb_display_en(en);
	return BK_OK;
}

bk_err_t bk_lcd_rgb_deinit(void)
{
	lcd_hal_rgb_int_enable(0, 0);
	lcd_hal_rgb_display_en(0);
	lcd_hal_rgb_display_sel(0);
	lcd_hal_mem_clr();
	bk_int_isr_unregister(INT_SRC_LCD);
	if(sys_drv_lcd_close() != 0) {
		os_printf("lcd system deinit reg config error \r\n");
		return BK_FAIL;
	}
	return BK_OK;
}

bk_err_t bk_lcd_power_on_ctrl(bool is_lcd_power_on)
{
	if(is_lcd_power_on) {
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_ON);
		//sys_drv_dev_clk_pwr_up(CLK_PWR_ID_DISP, CLK_PWR_CTRL_PWR_UP);
	} else {
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP, POWER_MODULE_STATE_OFF);
		//sys_drv_dev_clk_pwr_up(CLK_PWR_ID_DISP, CLK_PWR_CTRL_PWR_DOWN);
	}
	return BK_OK;
}



 bk_err_t  bk_lcd_8080_write_cmd(uint32_t cmd)
{
	lcd_hal_8080_write_cmd(cmd);
	return BK_OK;
}

bk_err_t  bk_lcd_8080_write_data(uint32_t data)
{
	lcd_hal_8080_write_data(data);
	return BK_OK;
}


bk_err_t bk_lcd_8080_int_enable(bool is_sof_en, bool is_eof_en) 
{
	lcd_hal_8080_int_enable(is_sof_en, is_eof_en);
	return BK_OK;
}

bk_err_t  bk_lcd_rgb_int_enable(bool       is_sof_en, bool is_eof_en)
{
	lcd_hal_rgb_int_enable(is_sof_en, is_eof_en);
	return BK_OK;
}

bk_err_t bk_lcd_8080_start_transfer(bool start)
{
	lcd_hal_8080_start_transfer(start);
	return BK_OK;
}

bk_err_t bk_lcd_8080_display_enable(bool en )
{
	lcd_hal_8080_display_enable(en);
	return BK_OK;
}

bk_err_t  bk_lcd_isr_register(lcd_int_type_t int_type, lcd_isr_t isr)
{
	if(int_type == I8080_OUTPUT_SOF) {
		s_lcd.lcd_8080_frame_start_handler = isr;
	}
	if(int_type == I8080_OUTPUT_EOF) {
		s_lcd.lcd_8080_frame_end_handler = isr;
	}
	if(int_type == RGB_OUTPUT_SOF) {
		s_lcd.lcd_rgb_frame_start_handler= isr;
	}
	if(int_type == RGB_OUTPUT_EOF) {
		s_lcd.lcd_rgb_frame_end_handler= isr;
	}
	return BK_OK;
}

static void lcd_isr(void)
{
	uint32_t int_status = lcd_hal_int_status_get();

	if (int_status & RGB_OUTPUT_SOF) {
		lcd_hal_rgb_sof_int_status_clear(); 
		if (s_lcd.lcd_rgb_frame_start_handler) {
			s_lcd.lcd_rgb_frame_start_handler();
		}
	}
	if (int_status & RGB_OUTPUT_EOF) {
		lcd_hal_rgb_eof_int_status_clear(); 
		if (s_lcd.lcd_rgb_frame_end_handler) {
			s_lcd.lcd_rgb_frame_end_handler();
		}
	}
	if (int_status & I8080_OUTPUT_SOF) {
		lcd_hal_eof_int_status_clear(); 
		if (s_lcd.lcd_8080_frame_start_handler) {
			s_lcd.lcd_8080_frame_start_handler();
		}
	}

	if (int_status & I8080_OUTPUT_EOF) {
		lcd_hal_eof_int_status_clear(); 
		if (s_lcd.lcd_8080_frame_end_handler) {
			s_lcd.lcd_8080_frame_end_handler();
		}
	}
}








