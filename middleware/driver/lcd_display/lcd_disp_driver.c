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
#include "lcd_disp_hal.h"
#include <driver/lcd.h>
#include "gpio_map.h"
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "gpio_map.h"
#include <driver/int.h>
#include "sys_driver.h"
#include <modules/pm.h>
#include <driver/hal/hal_gpio_types.h>
#include <driver/hal/hal_lcd_types.h>
#include "BK7256_RegList.h"
#include "lcd_disp_ll_macro_def_mp2.h"


extern void delay(INT32 num);
typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t dev;
} lcd_gpio_map_t;

typedef struct {
	lcd_isr_t lcd_8080_frame_start_handler;
	lcd_isr_t lcd_8080_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_start_handler;
} lcd_driver_t;

static lcd_driver_t s_lcd = {0};
static void lcd_isr(void);

bk_err_t bk_lcd_8080_gpio_init(void)
{

#if(0)
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
#else
	uint32_t fun_sel = 4;

	addAON_GPIO_Reg0x13 = 0x40;
	addAON_GPIO_Reg0x14 = 0x40;
	addAON_GPIO_Reg0x15 = 0x40;
	addAON_GPIO_Reg0x16 = 0x40;
	addAON_GPIO_Reg0x17 = 0x40;
	addAON_GPIO_Reg0x28 = 0x40;
	addAON_GPIO_Reg0x29 = 0x40;
	addAON_GPIO_Reg0x2a = 0x40;
	addAON_GPIO_Reg0x2b = 0x40;
	addAON_GPIO_Reg0x2c = 0x40;
	addAON_GPIO_Reg0x2d = 0x40;
	addAON_GPIO_Reg0x2e = 0x40;
	addAON_GPIO_Reg0x2f = 0x40;
	
	addSYSTEM_Reg0x32 = ((fun_sel << 12) | (fun_sel << 16) | (fun_sel << 20) | (fun_sel << 24) | (fun_sel << 28));
	addSYSTEM_Reg0x35 = ((fun_sel) | (fun_sel << 4) | (fun_sel << 8) | (fun_sel << 12) | (fun_sel << 16) | (fun_sel << 20) | (fun_sel << 24) | (fun_sel << 28));
#endif

	return BK_OK;
}

static bk_err_t bk_lcd_rgb_gpio_init(void)
{
#if(0)
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
#else 
	uint32_t fun_sel = 4;

	addAON_GPIO_Reg0xe = 0x40;	//14~26
	addAON_GPIO_Reg0xf = 0x40;
	addAON_GPIO_Reg0x10 = 0x40;
	addAON_GPIO_Reg0x11 = 0x40;
	addAON_GPIO_Reg0x12 = 0x40;
	addAON_GPIO_Reg0x13 = 0x40;
	addAON_GPIO_Reg0x14 = 0x40;
	addAON_GPIO_Reg0x15 = 0x40;
	addAON_GPIO_Reg0x16 = 0x40;
	addAON_GPIO_Reg0x17 = 0x40;
	addAON_GPIO_Reg0x18 = 0x40;
	addAON_GPIO_Reg0x19 = 0x40;
	addAON_GPIO_Reg0x1a = 0x40;
	addAON_GPIO_Reg0x28 = 0x40; //uart3
	addAON_GPIO_Reg0x29 = 0x40; //uart3
	addAON_GPIO_Reg0x2a = 0x40;
	addAON_GPIO_Reg0x2b = 0x40;
	addAON_GPIO_Reg0x2c = 0x40;
	addAON_GPIO_Reg0x2d = 0x40;
	addAON_GPIO_Reg0x2e = 0x40;
	addAON_GPIO_Reg0x2f = 0x40;

	addSYSTEM_Reg0x31 = ((fun_sel << 24) | (fun_sel << 28));//14~15
	addSYSTEM_Reg0x32 = ((fun_sel) | (fun_sel << 4) | (fun_sel << 8) | (fun_sel << 12) | (fun_sel << 16) | (fun_sel << 20) | (fun_sel << 24) | (fun_sel << 28));
	addSYSTEM_Reg0x33 = ((fun_sel) | (fun_sel << 4) | (fun_sel << 8));//24~26
	addSYSTEM_Reg0x35 = ((fun_sel) | (fun_sel << 4) | (fun_sel << 8) | (fun_sel << 12) | (fun_sel << 16) | (fun_sel << 20) | (fun_sel << 24) | (fun_sel << 28));


	os_printf("rgb_io_init ok. \r\n");
#endif
	return BK_OK;
}

bk_err_t bk_lcd_unmap_uart3_io_to_lcd_func(void)
{
	addAON_GPIO_Reg0x28 = 0x40; //uart3
	addAON_GPIO_Reg0x29 = 0x40; //uart3
	addSYSTEM_Reg0x35 =0x44444444;
	return BK_OK;
}


bk_err_t bk_lcd_soft_reset(void)
{
	lcd_hal_8080_display_enable(0); //disable 8080 display en
	lcd_hal_rgb_display_en(0);  //disable 8080 display en
	lcd_hal_soft_reset(1);

	delay(200);	//delay 200ns
	lcd_hal_soft_reset(0);
	
	return BK_OK;
}

bk_err_t bk_lcd_set_display_base_addr(uint32_t disp_base_addr)
{
	lcd_hal_set_display_read_base_addr(disp_base_addr);
	
	return BK_OK;
}


uint32_t bk_lcd_get_display_base_addr(void)
{
	return lcd_disp_ll_get_mater_rd_base_addr();
}


bk_err_t bk_lcd_8080_send_cmd(uint8_t param_count, uint32_t command, uint32_t *param)
{
	lcd_hal_8080_cmd_send(param_count, command, param);
	return BK_OK;
}

bk_err_t bk_lcd_set_partical_display(uint16_t partial_clum_l, uint16_t partial_clum_r, uint16_t partial_line_l, uint16_t partial_line_r)
{
	lcd_hal_set_partical_display(1, partial_clum_l, partial_clum_r, partial_line_l, partial_line_r);
	return BK_OK;
}

bk_err_t bk_lcd_partical_display_dis(void)
{
	lcd_hal_set_partial_display_en(0);
	return BK_OK;
}

bk_err_t bk_lcd_8080_ram_write(uint32_t command)
{
	lcd_hal_8080_cmd_param_count(1);
	lcd_hal_8080_write_cmd(command);

	return BK_OK;
}

bk_err_t bk_lcd_set_yuv_mode(rgb_input_data_format_t input_data_format)
{
	lcd_hal_display_yuv_sel(input_data_format);
	return BK_OK;
}

bk_err_t bk_lcd_set_pixel_reverse(bool reverse_en)
{
	lcd_hal_set_pixel_reverse(reverse_en);
	return BK_OK;
}

bk_err_t bk_lcd_rgb_init(lcd_types_t lcd_type, uint16_t x_pixel, uint16_t y_pixel, rgb_input_data_format_t input_data_format) 
{
	bk_lcd_rgb_gpio_init();
	lcd_hal_set_rgb_clk_rev_edge(0);  //output data is in clk doen edge or up adge
	bk_lcd_set_pixel_reverse(0);
	lcd_hal_rgb_int_enable(0, 1);
	lcd_hal_rgb_display_sel(1);  //RGB display enable, and select rgb module
	lcd_hal_set_sync_low(HSYNC_BACK_LOW, VSYNC_BACK_LOW);
	if (lcd_type == LCD_TYPE_480_272) {
		lcd_hal_rgb_sync_config(RGB_HSYNC_BACK_PORCH, RGB_HSYNC_FRONT_PORCH, RGB_VSYNC_BACK_PORCH, RGB_VSYNC_FRONT_PORCH);
	}else if (lcd_type == LCD_TYPE_1024_600) {
		bk_lcd_sync_config(RGB_720P_HSYNC_BACK_PORCH, RGB_720P_HSYNC_FRONT_PORCH,RGB_720P_VSYNC_BACK_PORCH, RGB_720P_VSYNC_FRONT_PORCH);
	} else {
		lcd_hal_rgb_sync_config(RGB_HSYNC_BACK_PORCH, RGB_HSYNC_FRONT_PORCH, RGB_VSYNC_BACK_PORCH, RGB_VSYNC_FRONT_PORCH);
	}

	lcd_hal_disconti_mode(DISCONTINUE_MODE);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	lcd_hal_display_yuv_sel(input_data_format);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	return BK_OK;
}

bk_err_t bk_lcd_sync_config(uint16_t rgb_hsync_back_porch, uint16_t rgb_hsync_front_porch, uint16_t rgb_vsync_back_porch, uint16_t rgb_vsync_front_porch)
{
	lcd_hal_rgb_sync_config( rgb_hsync_back_porch, rgb_hsync_front_porch, rgb_vsync_back_porch, rgb_vsync_front_porch);
	return BK_OK;
}


bk_err_t bk_lcd_driver_init(lcd_clk_t clk)
{
	bk_err_t ret = BK_OK;

	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_ON);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_UP);
	switch(clk) {
		case LCD_320M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_0, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_160M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_0, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_40M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_1, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_20M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_7, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_2, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
		break;
		case LCD_60M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_0, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_80M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_1, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_54M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_2, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_32M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_4, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_12M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_4, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_10M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_5, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_26M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_5, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_8M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_7, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		default:
			break;
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
	lcd_hal_8080_sleep_in(1);
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



bk_err_t bk_lcd_8080_init(uint16_t x_pixel, uint16_t y_pixel,rgb_input_data_format_t input_data_format)
{
	bk_lcd_8080_gpio_init();
	lcd_hal_rgb_display_sel(0); //25bit - rgb_on = 0 select 8080 mode
	lcd_hal_disconti_mode(DISCONTINUE_MODE);
	lcd_hal_8080_verify_1ms_count(VERIFY_1MS_COUNT);
	lcd_hal_8080_set_tik(TIK_CNT);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	lcd_hal_8080_set_fifo_data_thrd(CMD_FIFO_WR_THRD,CMD_FIFO_RD_THRD);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	lcd_hal_8080_display_enable(1);
	lcd_hal_8080_int_enable(0, 1); //set eof int enable 
	lcd_hal_display_yuv_sel(input_data_format);
	bk_lcd_8080_reset(); 
	//delay(7017857); //reset need 131ms.
	return BK_OK;
}

bk_err_t bk_lcd_8080_deinit(void)
{
	bk_int_isr_unregister(INT_SRC_LCD);
	bk_lcd_8080_reset();
	lcd_hal_8080_int_enable(0, 0);
	lcd_hal_8080_display_enable(0);
	lcd_hal_8080_start_transfer(0);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);
	if(sys_drv_lcd_close() != 0) {
		os_printf("lcd system deinit reg config error \r\n");
		return BK_FAIL;
	}
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
	bk_int_isr_unregister(INT_SRC_LCD);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);
	if(sys_drv_lcd_close() != 0) {
		os_printf("lcd system deinit reg config error \r\n");
		return BK_FAIL;
	}
	return BK_OK;
}


bk_err_t bk_lcd_8080_write_cmd(uint32_t cmd)
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








