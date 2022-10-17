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


//#include "lcd_disp_ll.h"
#include "lcd_disp_ll_macro_def.h"
#include "lcd_disp_hal.h"


/* wr_threshold_val: 0-0xff rd_threshold_val：0-0xff */
void lcd_hal_8080_set_fifo_data_thrd(uint16_t wr_threshold_val, uint16_t rd_threshold_val)
{
	lcd_disp_ll_set_i8080thrd_dat_wr_thrd(wr_threshold_val);
	lcd_disp_ll_set_i8080thrd_dat_rd_thrd(rd_threshold_val);
}


/* wr_threshold_val: 0-0x3ff rd_threshold_val：0-0xfff */
void lcd_hal_rgb_set_thrd(uint16_t wr_threshold_val, uint16_t rd_threshold_val)
{
	lcd_disp_ll_set_display_int_wr_thrd(wr_threshold_val);
	lcd_disp_ll_set_display_int_rd_thrd(rd_threshold_val);
}


/* is_sof_en :1 enable,0:disable ;  is_eof_en:1 enable,0:disable   */
void lcd_hal_8080_int_enable(bool is_sof_en, bool is_eof_en)
{
	lcd_disp_ll_set_display_int_i8080_int_en(is_sof_en| (is_eof_en << 1));
}

/* is_sof_en :1 enable,0:disable ;  is_eof_en:1 enable,0:disable   */
void lcd_hal_rgb_int_enable(bool         is_sof_en, bool is_eof_en)
{
	lcd_disp_ll_set_display_int_rgb_int_en(is_sof_en | (is_eof_en << 1));
}

void lcd_hal_pixel_config(uint16_t x_pixel, uint16_t y_pixel)
{
	lcd_disp_ll_set_status_x_pixel(x_pixel);
	lcd_disp_ll_set_status_y_pixel(y_pixel);
}

void lcd_hal_rgb_hsync(uint16_t rgb_hsync_back_porch, uint16_t rgb_hsync_front_porch)
{
	lcd_disp_ll_set_hsync_vsync_cfg_hsync_back_porch(rgb_hsync_back_porch);
	lcd_disp_ll_set_hsync_vsync_cfg_hsync_front_porch(rgb_hsync_front_porch);
}

void lcd_hal_rgb_vsync(uint16_t rgb_vsync_back_porch, uint16_t rgb_vsync_front_porch)
{
	lcd_disp_ll_set_hsync_vsync_cfg_vsync_front_porch(rgb_vsync_front_porch);
	lcd_disp_ll_set_hsync_vsync_cfg_vsync_back_porch(rgb_vsync_back_porch);
}

void lcd_hal_rgb_sync_config(uint16_t rgb_hsync_back_porch, uint16_t rgb_hsync_front_porch, uint16_t rgb_vsync_back_porch, uint16_t rgb_vsync_front_porch)
{
	uint32_t sync = rgb_hsync_back_porch + (rgb_hsync_front_porch << 8) + (rgb_vsync_back_porch << 15) + (rgb_vsync_front_porch << 20);
	lcd_disp_ll_set_hsync_vsync_cfg_value(sync);
}
void hal_lcd_8080_reset()
{
	lcd_disp_ll_set_i8080_config_reset_sleep_in(1);
	//reg_value |= (0x1 << 5);
	//reg_value |= 0xffffffdf;
	//bk_printf("reset value= %x \r\n", lcd_disp_ll_get_i8080_config_value());
}
void hal_lcd_8080_unreset()
{
	lcd_disp_ll_set_i8080_config_reset_sleep_in(0);
	//reg_value |= 0xffffffdf;
	//bk_printf("unreset value= %x \r\n", lcd_disp_ll_get_i8080_config_value());
}

void lcd_hal_int_status_clear(uint32_t int_type)
{
	if (I8080_OUTPUT_EOF | int_type) { 
		lcd_disp_ll_set_display_int_i8080_eof(1);
	}
	if (I8080_OUTPUT_SOF | int_type) {
		lcd_disp_ll_set_display_int_i8080_sof(1);
	}
}


void lcd_hal_eof_int_status_clear(void)
{
	lcd_disp_ll_set_display_int_i8080_eof(1);
}
void lcd_hal_sof_int_status_clear(void)
{
	lcd_disp_ll_set_display_int_i8080_sof(1);
}

void lcd_hal_rgb_eof_int_status_clear(void)
{
	lcd_disp_ll_set_display_int_rgb_eof(1);
}

void lcd_hal_rgb_sof_int_status_clear(void)
{
	lcd_disp_ll_set_display_int_rgb_sof(1);
}

void lcd_hal_reg_deinit(void)
{
	lcd_disp_ll_set_display_int_value(0);
	lcd_disp_ll_set_status_value(0);
	lcd_disp_ll_set_hsync_vsync_cfg_value(0);
	//lcd_disp_ll_set_i8080_config_value(0);
	lcd_disp_ll_set_display_int_value(0);
}

void lcd_hal_mem_clr(void)
{
	lcd_disp_ll_set_i8080_config_i8080_fifo_clr(1);
	lcd_disp_ll_set_i8080_config_i8080_cmdfifo_clr(1);
	lcd_disp_ll_set_status_str_fifo_clr(1);
}

void lcd_hal_rgb_display_sel(bool en)
{
	lcd_disp_ll_set_status_rgb_on(en);
	lcd_disp_ll_set_status_lcd_display_on(en);
}

