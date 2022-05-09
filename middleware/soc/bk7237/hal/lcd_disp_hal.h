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
#include <common/bk_include.h>
#include <soc/soc.h>
#include <soc/soc_port.h>
#include "hal_config.h"
#include <driver/hal/hal_gpio_types.h>

#include "lcd_disp_ll_macro_def.h"
#include "driver/lcd_disp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t dev;
} lcd_gpio_map_t;



#define lcd_hal_int_status_get()        lcd_disp_ll_get_display_int_value()

/* 0~0x1ff */
#define lcd_hal_8080_set_tik(value)      lcd_disp_ll_set_i8080_config_tik_cnt(value)
#define lcd_hal_8080_get_tik()     lcd_disp_ll_get_i8080_config_tik_cnt()
#define lcd_hal_8080_verify_1ms_count(cnt)   lcd_disp_ll_set_i8080_config_i8080_1ms_count(cnt)
#define lcd_hal_8080_get_1ms_verify_count()    lcd_disp_ll_get_i8080_config_i8080_1ms_count()
#define lcd_hal_disconti_mode(en) lcd_disp_ll_set_display_int_disconti_mode(en)
#define lcd_hal_8080_fifo_mode(mode)   lcd_disp_ll_set_i8080_config_i8080_fifo_mode(mode)

/* 0~0x7ff */
#define lcd_hal_rgb_clk_div(clk_div) lcd_disp_ll_set_status_rgb_clk_div(clk_div)
#define lcd_hal_set_rgb_clk_div(clk_div) lcd_disp_ll_set_status_rgb_clk_div(clk_div)
#define lcd_hal_set_rgb_clk_rev_edge(edge)  lcd_disp_ll_set_status_dclk_rev(edge)

#define lcd_hal_get_hsync_back_porch()   lcd_disp_ll_get_hsync_vsync_cfg_hsync_back_porch()
#define lcd_hal_get_hsync_front_porch()  lcd_disp_ll_get_hsync_vsync_cfg_hsync_front_porch()
#define lcd_hal_get_vsync_back_porch()  lcd_disp_ll_get_hsync_vsync_cfg_vsync_back_porch()
#define lcd_hal_get_vsync_front_porch()  lcd_disp_ll_get_hsync_vsync_cfg_vsync_front_porch()
#define lcd_hal_rgb_yuv_sel(data_format) lcd_disp_ll_set_hsync_vsync_cfg_yuv_sel(data_format)

/* 0:disable; 1 eanble*/
#define lcd_hal_8080_display_enable(value)     lcd_disp_ll_set_i8080_config_i8080_disp_en(value)
/* 1 display on */
#define lcd_hal_8080_start_transfer(value)     lcd_disp_ll_set_i8080_config_i8080_dat_on(value)
/* cmd bit[15:0] */
#define lcd_hal_8080_write_cmd(value)          lcd_disp_ll_set_i8080_cmd_fifo_value(value)

#define lcd_hal_8080_write_data(value)         lcd_disp_ll_set_i8080_dat_fifo_value(value)

/* 1: rgb display enable  */
#define lcd_hal_rgb_display_en(value)          lcd_disp_ll_set_status_rgb_disp_on(value)

/* 1: rgb output, 0 no rgb output  */
#define lcd_hal_rgb_io_enable(en)              lcd_disp_ll_set_status_rgb_on(en)

void lcd_hal_8080_set_fifo_data_thrd(uint16_t wr_threshold_val, uint16_t rd_threshold_val);
void lcd_hal_pixel_config(uint16_t x_pixel, uint16_t y_pixel);
void lcd_hal_8080_int_enable(bool is_sof_en, bool is_eof_en);


void lcd_hal_rgb_hsync(uint16_t rgb_hsync_back_porch, uint16_t rgb_hsync_front_porch);
void lcd_hal_rgb_vsync(uint16_t rgb_vsync_back_porch, uint16_t rgb_vsync_front_porch);
void lcd_hal_rgb_sync_config(uint16_t rgb_hsync_back_porch, uint16_t rgb_hsync_front_porch, uint16_t rgb_vsync_back_porch, uint16_t rgb_vsync_front_porch);
void lcd_hal_rgb_int_enable(bool is_sof_en, bool is_eof_en);
void lcd_hal_rgb_set_thrd(uint16_t wr_threshold_val, uint16_t rd_threshold_val);
void hal_lcd_8080_reset();
void hal_lcd_8080_unreset();
void lcd_hal_reg_deinit(void);
void lcd_hal_int_status_clear(uint32_t int_type);
void lcd_hal_eof_int_status_clear(void);
void lcd_hal_sof_int_status_clear(void);
void lcd_hal_rgb_eof_int_status_clear(void);
void lcd_hal_rgb_sof_int_status_clear(void);
void lcd_hal_mem_clr(void);
void lcd_hal_rgb_display_sel(void);

#define hal_lcd_8080_get_reset_value()   lcd_disp_ll_get_i8080_config_reset_sleep_in()


#if CFG_HAL_DEBUG_LCD_DISP
void lcd_disp_struct_dump(void);
#else
#define lcd_disp_struct_dump(void)
#endif

#ifdef __cplusplus
}
#endif


