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

#include <soc/soc.h>
#include "hal_port.h"
#include "lcd_disp_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_LL_REG_BASE    (SOC_LCD_DISP_REG_BASE)

#define reg0  (LCD_LL_REG_BASE  + 0x0*4) //REG ADDR :0x48060000
#define reg1  (LCD_LL_REG_BASE  + 0x1*4) //REG ADDR :0x48060004
#define reg2  (LCD_LL_REG_BASE  + 0x2*4) //REG ADDR :0x47800000
#define reg3  (LCD_LL_REG_BASE  + 0x3*4) //REG ADDR :0x47800000
#define reg4  (LCD_LL_REG_BASE  + 0x4*4) //REG ADDR :0x47800000
#define reg5  (LCD_LL_REG_BASE  + 0x5*4) //REG ADDR :0x47800000
#define reg6  (LCD_LL_REG_BASE  + 0x6*4) //REG ADDR :0x47800000
#define reg7  (LCD_LL_REG_BASE  + 0x7*4) //REG ADDR :0x47800000


static inline uint32_t lcd_disp_reg1_get_value(void)
{
    return REG_READ(reg1);
}

static inline void lcd_disp_reg0_config_value(uint32_t value)
{
    REG_WRITE(reg0, value);
}

static inline void lcd_disp_reg1_config_value(uint32_t value)
{
    REG_WRITE(reg1, value);
}
static inline void lcd_disp_reg2_config_value(uint32_t value)
{
    REG_WRITE(reg2, value);
}
static inline void lcd_disp_reg3_config_value(uint32_t value)
{
    REG_WRITE(reg3, value);
}
static inline void lcd_disp_reg4_config_value(uint32_t value)
{
    REG_WRITE(reg4, value);
}
static inline void lcd_disp_reg5_config_value(uint32_t value)
{
    REG_WRITE(reg5, value);
}
static inline void lcd_disp_reg6_config_value(uint32_t value)
{
    REG_WRITE(reg6, value);
}
static inline void lcd_disp_reg7_config_value(uint32_t value)
{
    REG_WRITE(reg7, value);
}


/**-------------------------------------------RGB/I8080 common  API----------------------------------------------------*/
/**-------------------------------------------RGB/I8080 common  API----------------------------------------------------*/
/**-------------------------------------------RGB/I8080 common  API----------------------------------------------------*/

static inline void lcd_display_fifo_clr(lcd_disp_hw_t *hw)
{
	hw->i8080_config.i8080_fifo_clr = 1;
	hw->i8080_config.i8080_cmdfifo_clr = 1;
	hw->status.str_fifo_clr = 1;
}

static inline void lcd_display_ll_int_en(lcd_disp_hw_t *hw, LCD_INT_TYPE int_type, bool en)
{
	if (I8080_OUTPUT_EOF | int_type) {
		hw->display_int.i8080_int_en |= (en << 1);
	}

	if (I8080_OUTPUT_SOF | int_type) {
		hw->display_int.i8080_int_en |= (en << 0);
	}

	if (RGB_OUTPUT_EOF | int_type) {
		hw->display_int.rgb_int_en |= (en << 1);
	}

	if (RGB_OUTPUT_SOF | int_type) {
		hw->display_int.rgb_int_en |= (en << 0);
	}
}

static inline uint32_t lcd_display_ll_int_status_get(lcd_disp_hw_t *hw)
{
	return hw->display_int.v;
}

static inline void lcd_display_ll_clear_int_status(lcd_disp_hw_t *hw, LCD_INT_TYPE int_type)
{
	if (I8080_OUTPUT_EOF | int_type) { 
		hw->display_int.i8080_eof = 1;
	}
	if (I8080_OUTPUT_SOF | int_type) {
		hw->display_int.i8080_sof = 1;
	}
	if (RGB_OUTPUT_EOF | int_type) {
		hw->display_int.rgb_eof = 1;
	}
	if (RGB_OUTPUT_SOF | int_type) {
		hw->display_int.rgb_sof = 1;
	}
}

static inline void lcd_display_ll_pixel_config(lcd_disp_hw_t *hw, uint16_t x_pixel, uint16_t y_pixel )
{
	hw->status.x_pixel = x_pixel;
	hw->status.y_pixel = y_pixel;
}


static inline uint32_t lcd_display_ll_status_get(lcd_disp_hw_t *hw)
{
	return hw->status.v & 0xff;
}

/**-------------------------------------------I8080  API----------------------------------------------------*/
/**-------------------------------------------I8080  API----------------------------------------------------*/
/**-------------------------------------------I8080  API----------------------------------------------------*/
static inline void lcd_display_ll_8080_display_enable(lcd_disp_hw_t *hw, uint8_t disp_en)
{
	hw->i8080_config.i8080_disp_en = disp_en;
}


static inline void lcd_display_ll_8080_data_start_transfer(lcd_disp_hw_t *hw, uint8_t data_start_transfer)
{
	hw->i8080_config.i8080_dat_on = data_start_transfer;  //set after init
}

/**
 * date wr: mpu write data to LCD
 *date rd:  mpu read data from LCD
 */
static inline void lcd_display_ll_8080_thrd_set(lcd_disp_hw_t *hw, uint32_t thrd_type, uint8_t threshold)
{
	if (DAT_WR_THRD | thrd_type) {
		hw->i8080thrd.dat_wr_thrd = threshold;
	}
	if (CMD_WR_THRD | thrd_type) {
		hw->i8080thrd.cmd_wr_thrd = threshold;
	}
	if (DAT_RD_THRD | thrd_type) {
		hw->i8080thrd.dat_rd_thrd = threshold;
	}
	if (CMD_RD_THRD | thrd_type) {
		hw->i8080thrd.cmd_rd_thrd = threshold;
	}
}

static inline void lcd_display_ll_8080_cmd_send(lcd_disp_hw_t *hw, uint16_t cmd)
{
	hw->i8080_cmd_fifo.i8080_cmd_fifo = cmd;
}

static inline void lcd_display_ll_8080_fifo_mode(lcd_disp_hw_t *hw, uint8_t fifo_mode)
{
	hw->i8080_config.i8080_fifo_mode = fifo_mode;
}

static inline void lcd_display_ll_8080_data_send(lcd_disp_hw_t *hw, uint16_t data)
{
	hw->i8080_dat_fifo.i8080_dat_fifo = data;
}

static inline void lcd_display_ll_8080_reset_in_sleep(lcd_disp_hw_t *hw, bool reset)
{
	hw->i8080_config.reset_sleep_in = reset;
}

static inline void lcd_display_ll_8080_tik_set(lcd_disp_hw_t *hw, uint8_t tik_cnt)
{
	hw->i8080_config.tik_cnt = tik_cnt;
}


/**-------------------------------------------RGB  API----------------------------------------------------*/
/**-------------------------------------------RGB  API----------------------------------------------------*/
/**-------------------------------------------RGB  API----------------------------------------------------*/

static inline void lcd_display_ll_rgb_enable(lcd_disp_hw_t *hw, bool en)
{
	hw->status.rgb_disp_on = en; //rgb display function on
}

static inline void lcd_display_ll_rgb_display_on(lcd_disp_hw_t *hw, bool en)
{
	hw->status.rgb_on = en;  //1: enable rgb output, 0: not rgb output
	hw->status.lcd_display_on = en;
}



static inline void lcd_display_ll_rgb_thrd_set(lcd_disp_hw_t *hw, LCD_RGB_THRD_TYPE thrd_type, uint32_t threshold)
{
	if (WR_THRD | thrd_type) {
		hw->display_int.wr_thrd = threshold;
	}
	if (RD_THRD | thrd_type) {
		hw->display_int.rd_thrd = threshold;
	}
}
static inline void lcd_display_ll_rgb_dclk_rev(lcd_disp_hw_t *hw, LCD_RGB_OUTPUT_EDGE edge)
{
	hw->status.dclk_rev = edge;
}

static inline void lcd_display_ll_rgb_clk_div(lcd_disp_hw_t *hw, uint8_t clk_div)
{
	hw->status.rgb_clk_div = clk_div;
}

static inline void lcd_display_ll_disconti_mode (lcd_disp_hw_t *hw, bool en)
{
	hw->display_int.disconti_mode = en;
}

static inline void lcd_display_ll_rgb_write_fifo(lcd_disp_hw_t *hw, uint16_t rgb_data)
{
	hw->rgb_fifo.rgb_dat = rgb_data;
}

static inline void lcd_display_ll_rgb_hsync(lcd_disp_hw_t *hw, uint8_t hsync_back_porch,uint8_t hsync_front_porch )
{
	hw->hsync_vsync.hsync_back_porch = hsync_back_porch;
	hw->hsync_vsync.hsync_front_porch = hsync_front_porch;
}

static inline void lcd_display_ll_rgb_vsync(lcd_disp_hw_t *hw, uint8_t vsync_back_porch, uint8_t vsync_front_porch)
{
	hw->hsync_vsync.vsync_back_porch = vsync_back_porch;
	hw->hsync_vsync.vsync_front_porch = vsync_front_porch;
}

static inline void lcd_display_ll_rgb_yuv_sel(lcd_disp_hw_t *hw, RGB_DATA_FORMAT data_format)
{
	hw->hsync_vsync.yuv_sel = data_format;
}

#ifdef __cplusplus
}
#endif

