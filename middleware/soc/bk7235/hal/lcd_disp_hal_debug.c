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

#include "hal_config.h"

#include "sys_ll_macro_def.h"
#include "sys_ll_op_if.h"
#include "lcd_disp_ll_macro_def.h"


#if CFG_HAL_DEBUG_LCD_DISP

void lcd_disp_struct_dump(void)
{
	SOC_LOGI (" cpu cksel=0x%x \r\n", sys_ll_get_cpu_clk_div_mode1_cksel_core());
	SOC_LOGI (" cpu cksel=0x%x \r\n", sys_ll_get_cpu_clk_div_mode1_clkdiv_core());
	SOC_LOGI (" lcd cksel=0x%x \n", sys_ll_get_cpu_clk_div_mode2_cksel_disp());
	SOC_LOGI (" clkdiv_disp_l=0x%x \n", sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l());
	SOC_LOGI (" clkdiv_disp_h=0x%x \n", sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h());
	SOC_LOGI (" cpu0_lcd_int_en=0x%x \n", sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_int_en());
	SOC_LOGI (" disp_cken=0x%x \n", sys_ll_get_cpu_device_clk_enable_disp_cken());
	SOC_LOGI (" disp_disckg=0x%x \n", sys_ll_get_cpu_mode_disckg2_disp_disckg());

	SOC_LOGI("lcd base=%x\r\n", (uint32_t)SOC_LCD_DISP_REG_BASE);
	/* REG_0x00 */
	SOC_LOGI("  lcd_config=0x%x value=0x%x\n", LCD_DISP_DISPLAY_INT_ADDR, lcd_disp_ll_get_display_int_value());
	SOC_LOGI("    rgb_int_en: %x\n", lcd_disp_ll_get_display_int_rgb_int_en());
	SOC_LOGI("    i8080_int_en:    %x\n",lcd_disp_ll_get_display_int_i8080_int_en());
	SOC_LOGI("    rgb_sof:    %x\n", lcd_disp_ll_get_display_int_rgb_sof());
	SOC_LOGI("    rgb_eof:   %x\n", lcd_disp_ll_get_display_int_rgb_eof());
	SOC_LOGI("    i8080_sof:   %x\n", lcd_disp_ll_get_display_int_i8080_eof());
	SOC_LOGI("    i8080_eof: %x\n", lcd_disp_ll_get_display_int_i8080_eof());
	SOC_LOGI("   rgb fifo wr_thrd   %x\n", lcd_disp_ll_get_display_int_wr_thrd());
	SOC_LOGI("    rgb fifo rd_thrd:   %x\n", lcd_disp_ll_get_display_int_rd_thrd());
	SOC_LOGI("    8080 disconti_mode: %x\n", lcd_disp_ll_get_display_int_disconti_mode());

	SOC_LOGI("  status=0x%x value=0x%x\n", LCD_DISP_STATUS_ADDR, lcd_disp_ll_get_status_value());
	SOC_LOGI("   x_pixel: %x\n", lcd_disp_ll_get_status_x_pixel());
	SOC_LOGI("    dclk_rev:    %x\n", lcd_disp_ll_get_status_dclk_rev());
	SOC_LOGI("   y_pixel:    %x\n", lcd_disp_ll_get_status_y_pixel());
	SOC_LOGI("    rgb_disp_on:   %x\n", lcd_disp_ll_get_status_rgb_disp_on());
	SOC_LOGI("    rgb mode io sel:   %x\n", lcd_disp_ll_get_status_rgb_on());
	SOC_LOGI("    lcd_display_on: %x\n",lcd_disp_ll_get_status_lcd_display_on());
	SOC_LOGI("    rgb_clk_div: %x\n", lcd_disp_ll_get_status_rgb_clk_div());

	SOC_LOGI("  hsync_vsync_cfg=0x%x value=0x%x\n", LCD_DISP_HSYNC_VSYNC_CFG_ADDR, lcd_disp_ll_get_hsync_vsync_cfg_value());
	SOC_LOGI("    hsync_back_porch:    %x\n", lcd_disp_ll_get_hsync_vsync_cfg_hsync_back_porch());
	SOC_LOGI("   hsync_front_porch:   %x\n", lcd_disp_ll_get_hsync_vsync_cfg_hsync_front_porch());
	SOC_LOGI("   vsync_back_porch:   %x\n", lcd_disp_ll_get_hsync_vsync_cfg_vsync_back_porch());
	SOC_LOGI("    vsync_front_porch: %x\n", lcd_disp_ll_get_hsync_vsync_cfg_vsync_front_porch());
		
	SOC_LOGI("  i8080_config=0x%x value=0x%x\n",LCD_DISP_I8080_CONFIG_ADDR, lcd_disp_ll_get_i8080_config_value());
	SOC_LOGI("    i8080_disp_en:    %x\n", lcd_disp_ll_get_i8080_config_i8080_disp_en());
	SOC_LOGI("    i8080_dat_on:   %x\n", lcd_disp_ll_get_i8080_config_i8080_dat_on());
	SOC_LOGI("    i8080_fifo_mode:   %x\n", lcd_disp_ll_get_i8080_config_i8080_fifo_mode());
	SOC_LOGI("    reset_sleep_in:    %x\n", lcd_disp_ll_get_i8080_config_reset_sleep_in());
	SOC_LOGI("    tik_cnt:   %x\n", lcd_disp_ll_get_i8080_config_tik_cnt());
	SOC_LOGI("    i8080_1ms_count:   %x\n", lcd_disp_ll_get_i8080_config_i8080_1ms_count());

	SOC_LOGI("  i8080thrd=0x%x value=0x%x\n", LCD_DISP_I8080THRD_ADDR, lcd_disp_ll_get_i8080thrd_value());
	SOC_LOGI("    dat_wr_thrd:    %x\n", lcd_disp_ll_get_i8080thrd_dat_wr_thrd());
	SOC_LOGI("    dat_rd_thrd:   %x\n", lcd_disp_ll_get_i8080thrd_dat_rd_thrd());
	SOC_LOGI("    cmd_wr_thrd:   %x\n", lcd_disp_ll_get_i8080thrd_cmd_wr_thrd());
	SOC_LOGI("    cmd_rd_thrd:   %x\n", lcd_disp_ll_get_i8080thrd_cmd_rd_thrd());
}

#endif

