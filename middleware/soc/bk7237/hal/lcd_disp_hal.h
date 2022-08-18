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

#include "lcd_disp_ll_macro_def_mp2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BK7256_DISP_BASE_ADDR             0x48060000
#define reg_DISP_INT_CONFIG               (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+0*4)))
#define reg_DISP_RGB_CONFIG               (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+1*4)))
#define reg_DISP_RGB_FIFO                 (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+2*4)))
#define reg_DISP_SYNC_CONFIG              (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+3*4)))
#define reg_DISP_I8080_CONFIG             (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+4*4)))
#define reg_DISP_CMD_FIFO                 (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+5*4)))
#define reg_DISP_DAT_FIFO                 (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+6*4)))
#define reg_DISP_I8080_THRD               (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+7*4)))
#define reg_DISP_STATUS                   (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+8*4)))
#define reg_DISP_SYNC_LOW                 (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+9*4)))
#define reg_DISP_PARTIAL1                 (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+10*4)))
#define reg_DISP_PARTIAL2                 (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+11*4)))
#define reg_DISP_THRD                     (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+12*4)))
#define reg_DISP_BASE_ADDR                (*((volatile unsigned long *)   (BK7256_DISP_BASE_ADDR+13*4)))

//common
#define lcd_hal_soft_reset                 lcd_disp_ll_set_display_int_soft_rset
#define lcd_hal_display_yuv_sel            lcd_disp_ll_set_hsync_vsync_yuv_sel
#define lcd_hal_set_rgb_data_revert        lcd_disp_ll_set_rgb_sync_low_pfc_pixel_reve  //rgb display input data two byte revert
#define lcd_hal_set_display_read_base_addr lcd_disp_ll_set_mater_rd_base_addr
#define lcd_hal_set_partial_display_en     lcd_disp_ll_set_rgb_clum_offset_partial_area_ena
#define lcd_hal_set_pixel_reverse          lcd_disp_ll_set_rgb_sync_low_pfc_pixel_reve

void lcd_hal_set_partical_display(uint8_t partial_en, uint16_t partial_clum_l, uint16_t partial_clum_r, uint16_t partial_line_l, uint16_t partial_line_r);
void lcd_hal_set_data_fifo_thrd(uint16_t wr_threshold_val, uint16_t rd_threshold_val);

//rgb cfg
#define lcd_hal_set_rgb_clk_rev_edge      lcd_disp_ll_set_rgb_cfg_dclk_rev
#define lcd_hal_get_status_ver_cnt_status lcd_disp_ll_get_disp_status_rgb_ver_cnt
#define lcd_hal_rgb_display_en            lcd_disp_ll_set_rgb_cfg_rgb_disp_on  /* bit24: rgb display enable  */
#define lcd_hal_rgb_io_enable             lcd_disp_ll_set_rgb_cfg_rgb_on /* bit25:  1: rgb module output, 0 8080 module output  */
#define lcd_hal_rgb_start_transfer        lcd_disp_ll_set_rgb_cfg_lcd_display_on /*BIT26 : lcd_display_on*/
void lcd_hal_set_sync_low(uint8_t hsync_back_low, uint16_t vsync_back_low);

//8080cfg
#define lcd_hal_get_status_i8080_ver_cnt        lcd_disp_ll_get_disp_status_i8080_ver_cnt
#define lcd_hal_8080_cmd_param_count            lcd_disp_ll_set_dat_fifo_thrd_i8080_cmd_para_count
#define lcd_hal_get_8080_cmd_cfg_done_status    lcd_disp_ll_get_disp_status_i8080_cmd_cfg_done
#define lcd_hal_get_8080_fifo_empty_status      lcd_disp_ll_get_disp_status_disp_fifo_empty
#define lcd_hal_get_8080_fifo_near_full_status  lcd_disp_ll_get_disp_status_disp_fifo_near_full
void lcd_hal_8080_cmd_send(uint8_t param_count, uint32_t command, uint32_t *param);

//common
#define lcd_hal_int_status_get          lcd_disp_ll_get_display_int_value
#define lcd_hal_disconti_mode           lcd_disp_ll_set_display_int_disconti_mode
void lcd_hal_pixel_config(uint16_t x_pixel, uint16_t y_pixel);

//rgb cfg
void lcd_hal_rgb_int_enable(bool is_sof_en, bool is_eof_en);
void lcd_hal_rgb_sync_config(uint16_t rgb_hsync_back_porch, uint16_t rgb_hsync_front_porch, uint16_t rgb_vsync_back_porch, uint16_t rgb_vsync_front_porch);
void lcd_hal_rgb_eof_int_status_clear(void);
void lcd_hal_rgb_sof_int_status_clear(void);
void lcd_hal_rgb_display_sel(bool en);

//8080 cfg
#define lcd_hal_8080_set_tik               lcd_disp_ll_set_i8080_config_tik_cnt
#define lcd_hal_8080_get_tik               lcd_disp_ll_get_i8080_config_tik_cnt
#define lcd_hal_8080_verify_1ms_count      lcd_disp_ll_set_i8080_config_i8080_1ms_count
#define lcd_hal_8080_get_1ms_verify_count  lcd_disp_ll_get_i8080_config_i8080_1ms_count
#define lcd_hal_8080_display_enable        lcd_disp_ll_set_i8080_config_i8080_disp_en /*8080 module  0:disable; 1 enable*/
#define lcd_hal_8080_start_transfer        lcd_disp_ll_set_i8080_config_i8080_dat_on /* 1 display on */
#define lcd_hal_8080_write_cmd             lcd_disp_ll_set_i8080_cmd_fifo_value
#define lcd_hal_8080_write_data            lcd_disp_ll_set_i8080_dat_fifo_value
#define hal_lcd_8080_get_reset_value       lcd_disp_ll_get_i8080_config_reset_sleep_in
#define lcd_hal_8080_fifo_mode             lcd_disp_ll_set_i8080_config_i8080_fifo_mode
#define lcd_hal_8080_sleep_in              lcd_disp_ll_set_i8080_config_reset_sleep_in

void lcd_hal_8080_set_fifo_data_thrd(uint16_t wr_threshold_val, uint16_t rd_threshold_val);
void lcd_hal_8080_int_enable(bool is_sof_en, bool is_eof_en);
void lcd_hal_eof_int_status_clear(void);
void lcd_hal_sof_int_status_clear(void);


#if CFG_HAL_DEBUG_LCD_DISP
void lcd_disp_struct_dump(void);
#else
#define lcd_disp_struct_dump(void)
#endif

#ifdef __cplusplus
}
#endif


