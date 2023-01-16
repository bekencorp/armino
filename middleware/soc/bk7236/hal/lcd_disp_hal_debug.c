// Copyright 2021-2022 Beken
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

#include "hal_config.h"

#include "system_hw.h"
#include "lcd_disp_ll_macro_def.h"


#if CFG_HAL_DEBUG_LCD_DISP

void lcd_disp_struct_dump(void)
{
	SOC_LOGI (" cpu cksel=0x%x \r\n", sys_ll_get_cpu_clk_div_mode1_cksel_core());
	SOC_LOGI (" cpu cksel=0x%x \r\n", sys_ll_get_cpu_clk_div_mode1_clkdiv_core());
	SOC_LOGI (" lcd cksel=0x%x \n", sys_ll_get_cpu_clk_div_mode2_cksel_disp());
	SOC_LOGI (" clkdiv_disp_l=0x%x \n", sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l());
	SOC_LOGI (" clkdiv_disp_h=0x%x \n", sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h());
	SOC_LOGI (" cpu0_lcd_int_en=0x%x \n", sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_display_int_en());
	SOC_LOGI (" disp_cken=0x%x \n", sys_ll_get_cpu_device_clk_enable_disp_cken());
	//SOC_LOGI (" disp_disckg=0x%x \n", sys_ll_get_cpu_mode_disckg2_disp_disckg());

	SOC_LOGI("lcd base=%x\r\n", (uint32_t)SOC_LCD_DISP_REG_BASE);

	SOC_LOGI("   soft_reset:  %x\r\n", lcd_disp_ll_get_module_control_soft_reset());
	SOC_LOGI("  display_int=0x%x value=0x%x\n", LCD_DISP_DISPLAY_INT_ADDR, lcd_disp_ll_get_display_int_value());
	SOC_LOGI("    rgb_int_en: %x\n", lcd_disp_ll_get_display_int_rgb_int_en());
	SOC_LOGI("    i8080_int_en:    %x\n",lcd_disp_ll_get_display_int_i8080_int_en());
	SOC_LOGI("    rgb_sof:    %x\n", lcd_disp_ll_get_display_int_rgb_sof());
	SOC_LOGI("    rgb_eof:   %x\n", lcd_disp_ll_get_display_int_rgb_eof());
	SOC_LOGI("    i8080_sof:   %x\n", lcd_disp_ll_get_display_int_i8080_eof());
	SOC_LOGI("    i8080_eof: %x\n", lcd_disp_ll_get_display_int_i8080_eof());
	SOC_LOGI("    8080 disconti_mode: %x\n", lcd_disp_ll_get_display_int_disconti_mode());

	SOC_LOGI("  rgb_cfg=0x%x value=0x%x\n", LCD_DISP_RGB_CFG_ADDR, lcd_disp_ll_get_rgb_cfg_value());
	SOC_LOGI("   x_pixel: %x\n", lcd_disp_ll_get_rgb_cfg_x_pixel());
	SOC_LOGI("    dclk_rev:    %x\n", lcd_disp_ll_get_rgb_cfg_dclk_rev());
	SOC_LOGI("   y_pixel:    %x\n", lcd_disp_ll_get_rgb_cfg_y_pixel());
	SOC_LOGI("    rgb_disp_on:   %x\n", lcd_disp_ll_get_rgb_cfg_rgb_disp_on());
	SOC_LOGI("    rgb mode io sel:   %x\n", lcd_disp_ll_get_rgb_cfg_rgb_on());
	SOC_LOGI("    lcd_display_on: %x\n",lcd_disp_ll_get_rgb_cfg_lcd_display_on());

	SOC_LOGI("  sync_cfg=0x%x value=0x%x\n", LCD_DISP_SYNC_CFG_ADDR, lcd_disp_ll_get_sync_cfg_value());
	SOC_LOGI("    hsync_back_porch:    %x\n", lcd_disp_ll_get_sync_cfg_hsync_back_porch());
	SOC_LOGI("   hsync_front_porch:   %x\n", lcd_disp_ll_get_sync_cfg_hsync_front_porch());
	SOC_LOGI("   vsync_back_porch:   %x\n", lcd_disp_ll_get_sync_cfg_vsync_back_porch());
	SOC_LOGI("    vsync_front_porch: %x\n", lcd_disp_ll_get_sync_cfg_vsync_front_porch());
	SOC_LOGI("    yuv_sel: %x\n", lcd_disp_ll_get_sync_cfg_yuv_sel());

	SOC_LOGI("  i8080_config=0x%x value=0x%x\n",LCD_DISP_I8080_CONFIG_ADDR, lcd_disp_ll_get_i8080_config_value());
	SOC_LOGI("    i8080_disp_en:    %x\n", lcd_disp_ll_get_i8080_config_i8080_disp_en());
	SOC_LOGI("    i8080_dat_on:   %x\n", lcd_disp_ll_get_i8080_config_i8080_dat_on());
	SOC_LOGI("    i8080_fifo_mode:   %x\n", lcd_disp_ll_get_i8080_config_i8080_fifo_mode());
	SOC_LOGI("    reset_sleep_in:    %x\n", lcd_disp_ll_get_i8080_config_reset_sleep_in());
	SOC_LOGI("    i8080_ena:      %x\n", lcd_disp_ll_get_i8080_config_i8080_ena());
	SOC_LOGI("    tik_cnt:   %x\n", lcd_disp_ll_get_i8080_config_tik_cnt());
	SOC_LOGI("    i8080_1ms_count:   %x\n", lcd_disp_ll_get_i8080_config_i8080_1ms_count());

	SOC_LOGI("  i8080_cmd_fifo=0x%x value=0x%x\n", LCD_DISP_I8080_CMD_FIFO_ADDR, lcd_disp_ll_get_i8080_cmd_fifo_value());
	SOC_LOGI("    8080_cmd_fifo:    %x\n", lcd_disp_ll_get_i8080_cmd_fifo_i8080_cmd_fifo());

	SOC_LOGI("  i8080_dat_fifo=0x%x value=0x%x\n",LCD_DISP_I8080_DAT_FIFO_ADDR, lcd_disp_ll_get_i8080_dat_fifo_value());

	SOC_LOGI("  i8080_thrd=0x%x value=0x%x\n",LCD_DISP_I8080_THRD_ADDR, lcd_disp_ll_get_i8080_thrd_value());
	SOC_LOGI("    cmd_wr_thrd:    %x\n", lcd_disp_ll_get_i8080_thrd_cmd_wr_thrd());
	SOC_LOGI("    cmd_rd_thrd:   %x\n", lcd_disp_ll_get_i8080_thrd_cmd_rd_thrd());

	SOC_LOGI("  disp_status=0x%x value=0x%x\n",LCD_DISP_DISP_STATUS_ADDR, lcd_disp_ll_get_disp_status_value());
	SOC_LOGI("    rgb_ver_cnt:    %x\n", lcd_disp_ll_get_disp_status_rgb_ver_cnt());
	SOC_LOGI("    i8080_cmd_cfg_done:   %x\n", lcd_disp_ll_get_disp_status_i8080_cmd_cfg_done());
	SOC_LOGI("    i8080_ver_cnt:   %x\n", lcd_disp_ll_get_disp_status_i8080_ver_cnt());
	SOC_LOGI("    disp_fifo_empty:    %x\n", lcd_disp_ll_get_disp_status_disp_fifo_empty());
	SOC_LOGI("    disp_fifo_near_full:   %x\n", lcd_disp_ll_get_disp_status_disp_fifo_near_full());

	SOC_LOGI("  rgb_sync_low=0x%x value=0x%x\n",LCD_DISP_RGB_SYNC_LOW_ADDR, lcd_disp_ll_get_rgb_sync_low_value());
	SOC_LOGI("    hsync_back_low:    %x\n", lcd_disp_ll_get_rgb_sync_low_hsync_back_low());
	SOC_LOGI("    vsync_back_low:   %x\n", lcd_disp_ll_get_rgb_sync_low_vsync_back_low());
	SOC_LOGI("    pfc_pixel_reve:   %x\n", lcd_disp_ll_get_rgb_sync_low_pfc_pixel_reve());

	SOC_LOGI("  rgb_clum_offset=0x%x value=0x%x\n", LCD_DISP_RGB_CLUM_OFFSET_ADDR, lcd_disp_ll_get_rgb_clum_offset_value());
	SOC_LOGI("    partial_offset_clum_l:    %x\n", lcd_disp_ll_get_rgb_clum_offset_partial_offset_clum_l());
	SOC_LOGI("   partial_offset_clum_r:   %x\n", lcd_disp_ll_get_rgb_clum_offset_partial_offset_clum_r());
	SOC_LOGI("   partial_area_ena:   %x\n", lcd_disp_ll_get_rgb_clum_offset_partial_area_ena());

	SOC_LOGI("  rgb_line_offset=0x%x value=0x%x\n", LCD_DISP_RGB_LINE_OFFSET_ADDR, lcd_disp_ll_get_rgb_line_offset_value());
	SOC_LOGI("    partial_offset_clum_l:    %x\n", lcd_disp_ll_get_rgb_line_offset_partial_offset_clum_l());
	SOC_LOGI("   partial_offset_clum_r:   %x\n", lcd_disp_ll_get_rgb_line_offset_partial_offset_clum_r());

	SOC_LOGI("  dat_fifo_thrd=0x%x value=0x%x\n", LCD_DISP_DAT_FIFO_THRD_ADDR, lcd_disp_ll_get_dat_fifo_thrd_value());

	SOC_LOGI("   dat_wr_thrd:   %x\n", lcd_disp_ll_get_dat_fifo_thrd_dat_wr_thrd());
	SOC_LOGI("   dat_rd_thrd:   %x\n", lcd_disp_ll_get_dat_fifo_thrd_dat_rd_thrd());

	SOC_LOGI("  mater_rd_base_addr=0x%x value=0x%x\n", LCD_DISP_MATER_RD_BASE_ADDR_ADDR, lcd_disp_ll_get_mater_rd_base_addr());
	SOC_LOGI("    i8080_cmd_para_count:    %x\n", lcd_disp_ll_get_cmd_count_i8080_cmd_para_count());

}

#endif

