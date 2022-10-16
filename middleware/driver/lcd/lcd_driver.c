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
#include <math.h>
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
#include <driver/dma2d.h>
#include <./lcd/lcd_devices.h>
#include <driver/jpeg_dec.h>
#include <driver/jpeg_dec_types.h>
#include <driver/timer.h>

#if CONFIG_PWM
#include <driver/pwm.h>
#endif
#include <modules/pm.h>
#include "lcd_devices.h"

#ifdef CONFIG_MASTER_CORE
#include "mailbox_channel.h"
#endif

#if (CONFIG_JPEG_DECODE)
#include <components/jpeg_decode.h>
#endif


#define TAG "lcd_drv"
#define timer_control_pfs   0
#define IO_FUNCTION_ENABLE(pin, func)   \
	do {                                \
		gpio_dev_unmap(pin);            \
		gpio_dev_map(pin, func);        \
	} while (0)
#define IO_FUNCTION_DISLAY(pin) 	\
	do {								\
		gpio_dev_unmap(pin);			\
	} while (0)

#define LCD_RETURN_ON_NOT_INIT() do {\
		if (!s_lcd_driver_is_init) {\
			return BK_ERR_LCD_NOT_INIT;\
		}\
	} while(0)
static bool s_lcd_driver_is_init = false;

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

//#define LCD_DIAG_DEBUG

#ifdef LCD_DIAG_DEBUG

#define LCD_DIAG_DEBUG_INIT()                   \
	do {                                        \
		gpio_dev_unmap(GPIO_2);                 \
		bk_gpio_disable_pull(GPIO_2);           \
		bk_gpio_enable_output(GPIO_2);          \
		bk_gpio_set_output_low(GPIO_2);         \
		\
		gpio_dev_unmap(GPIO_3);                 \
		bk_gpio_disable_pull(GPIO_3);           \
		bk_gpio_enable_output(GPIO_3);          \
		bk_gpio_set_output_low(GPIO_3);         \
		\
		gpio_dev_unmap(GPIO_4);                 \
		bk_gpio_disable_pull(GPIO_4);           \
		bk_gpio_enable_output(GPIO_4);          \
		bk_gpio_set_output_low(GPIO_4);         \
		\
		gpio_dev_unmap(GPIO_5);                 \
		bk_gpio_disable_pull(GPIO_5);           \
		bk_gpio_enable_output(GPIO_5);          \
		bk_gpio_set_output_low(GPIO_5);         \
		\
	} while (0)

#define LCD_DECODER_START()                 bk_gpio_set_output_high(GPIO_2)
#define LCD_DECODER_END()                   bk_gpio_set_output_low(GPIO_2)

#define LCD_ROTATE_START()                  bk_gpio_set_output_high(GPIO_3)
#define LCD_ROTATE_END()                    bk_gpio_set_output_low(GPIO_3)

#define LCD_DISPLAY_START()                 bk_gpio_set_output_high(GPIO_4)
#define LCD_DISPLAY_END()                   bk_gpio_set_output_low(GPIO_4)

#define LCD_DISPLAY_ISR_ENTRY()             bk_gpio_set_output_high(GPIO_5)
#define LCD_DISPLAY_ISR_OUT()               bk_gpio_set_output_low(GPIO_5)

#else

#define LCD_DIAG_DEBUG_INIT()

#define LCD_DECODER_START()
#define LCD_DECODER_END()

#define LCD_ROTATE_START()
#define LCD_ROTATE_END()

#define LCD_DISPLAY_START()
#define LCD_DISPLAY_END()

#define LCD_DISPLAY_ISR_ENTRY()
#define LCD_DISPLAY_ISR_OUT()

#endif



extern media_debug_t *media_debug;
extern u64 riscv_get_mtimer(void);

typedef struct
{
#if (USE_LCD_REGISTER_CALLBACKS == 1)  //register callback
	lcd_isr_t lcd_8080_frame_start_handler;
	lcd_isr_t lcd_8080_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_start_handler;
#endif

	beken_semaphore_t display;
	lcd_config_t config;
	uint16_t width;
	uint16_t height;
	pixel_format_t fmt;
	uint8_t enable : 1;
	uint8_t decoder_en : 1;
	uint8_t rotate_en : 1;
	uint8_t display_en : 1;

	frame_buffer_t *decoder_frame;
	frame_buffer_t *rotate_frame;
	frame_buffer_t *display_frame;
	frame_buffer_t *pingpong_frame;

	beken_semaphore_t dec_sem;
	beken_semaphore_t rot_sem;
	beken_semaphore_t disp_sem;

	beken_mutex_t dec_lock;
	beken_mutex_t rot_lock;
	beken_mutex_t disp_lock;

} lcd_driver_t;


extern bk_err_t bk_lcd_set_yuv_mode(pixel_format_t input_data_format);

const lcd_device_t *lcd_devices[] =
{
	&lcd_device_st7282,
	&lcd_device_hx8282,
	&lcd_device_st7796s,
	&lcd_device_gc9503v,
	&lcd_device_nt35512
};

static lcd_driver_t s_lcd = {0};
static uint8_t sw_decode_en = 0;

const lcd_device_t * get_lcd_device_by_name(char * name)
{
	uint32_t i, size = sizeof(lcd_devices) / sizeof(lcd_device_t *);

	for (i = 0; i < size; i++)
	{
		if (os_memcmp(lcd_devices[i]->name, name, sizeof(name)) == 0)
		{
			return lcd_devices[i];
		}
	}
	return NULL;
}

const lcd_device_t *get_lcd_device_by_ppi(media_ppi_t ppi)
{
	uint32_t i, size = sizeof(lcd_devices) / sizeof(lcd_device_t *);

	for (i = 0; i < size; i++)
	{
		if (lcd_devices[i]->ppi == ppi)
		{
			return lcd_devices[i];
		}
	}

	return NULL;
}

const lcd_device_t *get_lcd_device_by_id(lcd_device_id_t id)
{
	uint32_t i, size = sizeof(lcd_devices) / sizeof(lcd_device_t *);

	for (i = 0; i < size; i++)
	{
		if (lcd_devices[i]->id == id)
		{
			return lcd_devices[i];
		}
	}

	return NULL;
}

#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
#else
bk_err_t lcd_mcu_gpio_init(void)
{
	LOGI("%s\n", __func__);

	IO_FUNCTION_ENABLE(LCD_MCU_D0_PIN, LCD_MCU_D0_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D1_PIN, LCD_MCU_D1_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D2_PIN, LCD_MCU_D2_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D3_PIN, LCD_MCU_D3_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D4_PIN, LCD_MCU_D4_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D5_PIN, LCD_MCU_D5_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D6_PIN, LCD_MCU_D6_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_D7_PIN, LCD_MCU_D7_FUNC);

	IO_FUNCTION_ENABLE(LCD_MCU_RDX_PIN, LCD_MCU_RDX_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_WRX_PIN, LCD_MCU_WRX_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_RSX_PIN, LCD_MCU_RSX_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_RESET_PIN, LCD_MCU_RESET_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_CSX_PIN, LCD_MCU_CSX_FUNC);

	return BK_OK;
}
#endif

#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
#else
static bk_err_t lcd_rgb_gpio_init(void)
{
	LOGI("%s\n", __func__);

	IO_FUNCTION_ENABLE(LCD_RGB_R0_PIN, LCD_RGB_R0_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R1_PIN, LCD_RGB_R1_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R2_PIN, LCD_RGB_R2_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R3_PIN, LCD_RGB_R3_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R4_PIN, LCD_RGB_R4_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G0_PIN, LCD_RGB_G0_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G1_PIN, LCD_RGB_G1_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G2_PIN, LCD_RGB_G2_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G3_PIN, LCD_RGB_G3_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G4_PIN, LCD_RGB_G4_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G5_PIN, LCD_RGB_G5_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B0_PIN, LCD_RGB_B0_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B1_PIN, LCD_RGB_B1_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B2_PIN, LCD_RGB_B2_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B3_PIN, LCD_RGB_B3_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B4_PIN, LCD_RGB_B4_FUNC);

	IO_FUNCTION_ENABLE(LCD_RGB_CLK_PIN, LCD_RGB_CLK_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_DISP_PIN, LCD_RGB_DISP_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_HSYNC_PIN, LCD_RGB_HSYNC_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_VSYNC_PIN, LCD_RGB_VSYNC_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_DE_PIN, LCD_RGB_DE_FUNC);

	return BK_OK;
}
#endif

static bk_err_t lcd_mcu_gpio_reinit(void)
{
	//LOGI("%s\n", __func__);
//	bk_gpio_enable_output(GPIO_19);
	bk_gpio_set_output_high(GPIO_19);
	bk_gpio_set_output_high(GPIO_20);
	bk_gpio_set_output_high(GPIO_21);
	bk_gpio_set_output_high(GPIO_22);
	bk_gpio_set_output_high(GPIO_23);

//	addAON_GPIO_Reg0x13 = 0x2;
//	addAON_GPIO_Reg0x14 = 0x2;
//	addAON_GPIO_Reg0x15 = 0x2;
//	addAON_GPIO_Reg0x16 = 0x2;
//	addAON_GPIO_Reg0x17 = 0x2;

	IO_FUNCTION_DISLAY(LCD_MCU_RDX_PIN);
	IO_FUNCTION_DISLAY(LCD_MCU_WRX_PIN);
	IO_FUNCTION_DISLAY(LCD_MCU_RSX_PIN);
	IO_FUNCTION_DISLAY(LCD_MCU_RESET_PIN);
	IO_FUNCTION_DISLAY(LCD_MCU_CSX_PIN);

	lcd_hal_soft_reset(1);
	rtos_delay_milliseconds(1);
	lcd_hal_soft_reset(0);

	IO_FUNCTION_ENABLE(LCD_MCU_RDX_PIN, LCD_MCU_RDX_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_WRX_PIN, LCD_MCU_WRX_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_RSX_PIN, LCD_MCU_RSX_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_RESET_PIN, LCD_MCU_RESET_FUNC);
	IO_FUNCTION_ENABLE(LCD_MCU_CSX_PIN, LCD_MCU_CSX_FUNC);
	return BK_OK;
}

#ifdef CONFIG_MASTER_CORE
static void lcd_driver_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
	bk_err_t ret;

	//LOGI("%s, %08X\n", __func__, cmd_buf->param1);

	if (cmd_buf->hdr.cmd == 0x19)
	{
		ret = rtos_set_semaphore(&s_lcd.rot_sem);

		if (ret != BK_OK)
		{
			LOGE("%s semaphore get failed: %d\n", __func__, ret);
		}
	}


}

static void lcd_driver_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);

}

static void lcd_driver_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
	LOGD("%s\n", __func__);
}
#endif


#if (USE_LCD_REGISTER_CALLBACKS == 1)
static void lcd_isr(void);

bk_err_t  bk_lcd_isr_register(lcd_int_type_t int_type, lcd_isr_t isr)
{
	if (int_type == I8080_OUTPUT_SOF)
	{
		s_lcd.lcd_8080_frame_start_handler = isr;
	}
	if (int_type == I8080_OUTPUT_EOF)
	{
		s_lcd.lcd_8080_frame_end_handler = isr;
	}
	if (int_type == RGB_OUTPUT_SOF)
	{
		s_lcd.lcd_rgb_frame_start_handler = isr;
	}
	if (int_type == RGB_OUTPUT_EOF)
	{
		s_lcd.lcd_rgb_frame_end_handler = isr;
	}
	return BK_OK;
}

static void lcd_isr(void)
{
	uint32_t int_status = lcd_hal_int_status_get();

	if (int_status & RGB_OUTPUT_SOF)
	{
		if (s_lcd.lcd_rgb_frame_start_handler)
		{
			s_lcd.lcd_rgb_frame_start_handler();
		}
		lcd_hal_rgb_sof_int_status_clear();
	}
	if (int_status & RGB_OUTPUT_EOF)
	{
		if (s_lcd.lcd_rgb_frame_end_handler)
		{
			s_lcd.lcd_rgb_frame_end_handler();
		}
		lcd_hal_rgb_eof_int_status_clear();
	}
	if (int_status & I8080_OUTPUT_SOF)
	{
		if (s_lcd.lcd_8080_frame_start_handler)
		{
			s_lcd.lcd_8080_frame_start_handler();
		}
		lcd_hal_eof_int_status_clear();
	}

	if (int_status & I8080_OUTPUT_EOF)
	{
		if (s_lcd.lcd_8080_frame_end_handler)
		{
			s_lcd.lcd_8080_frame_end_handler();
		}
		lcd_hal_eof_int_status_clear();
	}
}
#else
bk_err_t bk_lcd_isr_register(lcd_isr_t lcd_isr)
{
	bk_int_isr_register(INT_SRC_LCD, lcd_isr, NULL);
	return BK_OK;
}


uint32_t bk_lcd_int_status_get(void)
{
	return reg_DISP_INT_CONFIG;
}


bk_err_t bk_lcd_int_status_clear(lcd_int_type_t int_type)
{
	switch (int_type)
	{
		case RGB_OUTPUT_SOF:
			lcd_hal_rgb_sof_int_status_clear();
			break;
		case RGB_OUTPUT_EOF:
			lcd_hal_rgb_eof_int_status_clear();
			break;
		case I8080_OUTPUT_SOF:
			lcd_hal_eof_int_status_clear();
			break;
		case I8080_OUTPUT_EOF:
			lcd_hal_eof_int_status_clear();
			break;
		default:
			reg_DISP_INT_CONFIG = 0;
			break;
	}
	return BK_OK;
}
#endif

bk_err_t bk_lcd_8080_int_enable(bool is_sof_en, bool is_eof_en)
{
	lcd_hal_8080_int_enable(is_sof_en, is_eof_en);
	return BK_OK;
}

bk_err_t  bk_lcd_rgb_int_enable(bool is_sof_en, bool is_eof_en)
{
	lcd_hal_rgb_int_enable(is_sof_en, is_eof_en);
	return BK_OK;
}


bk_err_t bk_lcd_driver_init(lcd_clk_t clk)
{
	bk_err_t ret = BK_OK;
	if (s_lcd_driver_is_init) {
		LOGI("%s already init. \n", __func__);
		return BK_OK;
	}
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_ON);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_UP);
	switch (clk)
	{
		case LCD_320M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_0, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_160M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_0, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_120M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_0, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
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

#if (USE_LCD_REGISTER_CALLBACKS == 1)
	bk_int_isr_register(INT_SRC_LCD, lcd_isr, NULL);
#endif
	s_lcd_driver_is_init = true;
	return ret;
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
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_pixel_config(x_pixel, y_pixel);
	return BK_OK;
}


bk_err_t bk_lcd_8080_send_cmd(uint8_t param_count, uint32_t command, uint32_t *param)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_8080_cmd_send(param_count, command, param);
	return BK_OK;
}


bk_err_t bk_lcd_8080_ram_write(uint32_t command)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_8080_cmd_param_count(1);
	lcd_hal_8080_write_cmd(command);
	return BK_OK;
}

bk_err_t bk_lcd_set_yuv_mode(pixel_format_t input_data_format)
{
	switch (input_data_format)
	{
		case PIXEL_FMT_RGB565:
			lcd_hal_display_yuv_sel(0);
			lcd_hal_set_pixel_reverse(0);
			break;
		case PIXEL_FMT_YUYV:
			lcd_hal_display_yuv_sel(1);
			lcd_hal_set_pixel_reverse(0);
			break;
		case PIXEL_FMT_UYVY:
			lcd_hal_display_yuv_sel(2);
			lcd_hal_set_pixel_reverse(0);
			break;
		case PIXEL_FMT_YYUV:
			lcd_hal_display_yuv_sel(3);
			lcd_hal_set_pixel_reverse(0);
			break;
		case PIXEL_FMT_UVYY:
			lcd_hal_display_yuv_sel(4);
			lcd_hal_set_pixel_reverse(0);
			break;
		case PIXEL_FMT_VUYY:
			lcd_hal_display_yuv_sel(5);
			lcd_hal_set_pixel_reverse(0);
			break;
		case PIXEL_FMT_YVYU:
			lcd_hal_display_yuv_sel(1);
			lcd_hal_set_pixel_reverse(1);
			break;
		case PIXEL_FMT_VYUY:
			lcd_hal_display_yuv_sel(2);
			lcd_hal_set_pixel_reverse(1);
			break;
		case PIXEL_FMT_YYVU:
			lcd_hal_display_yuv_sel(5);
			lcd_hal_set_pixel_reverse(1);
			break;
		default:
			break;
	}
	return BK_OK;
}

pixel_format_t bk_lcd_get_yuv_mode()
{
	uint32_t yuv_mode = lcd_hal_get_display_yuv_sel();
	uint32_t reverse = lcd_hal_get_pixel_reverse();
	pixel_format_t output_data_format = PIXEL_FMT_UNKNOW;

	if (reverse == 0)
	{
		if (yuv_mode == 0)
		{
			output_data_format = PIXEL_FMT_RGB565;
		}
		else if (yuv_mode == 1)
		{
			output_data_format = PIXEL_FMT_YUYV;
		}
		else if (yuv_mode == 2)
		{
			output_data_format = PIXEL_FMT_UYVY;
		}
		else if (yuv_mode == 3)
		{
			output_data_format = PIXEL_FMT_YYUV;
		}
		else if (yuv_mode == 4)
		{
			output_data_format = PIXEL_FMT_UVYY;
		}
		else if (yuv_mode == 5)
		{
			output_data_format = PIXEL_FMT_VUYY;
		}
	}
	else if (reverse == 1)
	{
		if (yuv_mode == 1)
		{
			output_data_format = PIXEL_FMT_YVYU;
		}
		else if (yuv_mode == 2)
		{
			output_data_format = PIXEL_FMT_VYUY;
		}
		else if (yuv_mode == 5)
		{
			output_data_format = PIXEL_FMT_YYVU;
		}
	}

	return output_data_format;
}

bk_err_t bk_lcd_rgb_init(lcd_device_id_t id, uint16_t x_pixel, uint16_t y_pixel, pixel_format_t input_data_format)
{

	LCD_RETURN_ON_NOT_INIT();

#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
	/*
	 * GPIO info is setted in GPIO_DEFAULT_DEV_CONFIG and
	 * inited in bk_gpio_driver_init->gpio_hal_default_map_init.
	 * If needs to re-config GPIO, can deal it here.
	 */
#else
	lcd_rgb_gpio_init();
#endif
	lcd_hal_rgb_int_enable(0, 1);
	lcd_hal_rgb_display_sel(1);  //RGB display enable, and select rgb module
	lcd_hal_rgb_display_en(0);
	lcd_hal_set_sync_low(HSYNC_BACK_LOW, VSYNC_BACK_LOW);
	switch (id)
	{
		case LCD_DEVICE_ST7282:
			lcd_hal_rgb_sync_config(RGB_HSYNC_BACK_PORCH, RGB_HSYNC_FRONT_PORCH, RGB_VSYNC_BACK_PORCH, RGB_VSYNC_FRONT_PORCH);
			lcd_hal_set_rgb_clk_rev_edge(POSEDGE_OUTPUT);
			break;
		case LCD_DEVICE_HX8282:
			lcd_hal_rgb_sync_config(RGB_720P_HSYNC_BACK_PORCH, RGB_720P_HSYNC_FRONT_PORCH, RGB_720P_VSYNC_BACK_PORCH, RGB_720P_VSYNC_FRONT_PORCH);
			lcd_hal_set_rgb_clk_rev_edge(POSEDGE_OUTPUT);
			break;
		case LCD_DEVICE_GC9503V:
			lcd_hal_rgb_sync_config(RGB_HSYNC_BACK_PORCH, RGB_HSYNC_FRONT_PORCH, RGB_VSYNC_BACK_PORCH, RGB_VSYNC_FRONT_PORCH);
			lcd_hal_set_rgb_clk_rev_edge(NEGEDGE_OUTPUT);  //output data is in clk doen edge or up adge
			break;
		default:
			lcd_hal_rgb_sync_config(RGB_HSYNC_BACK_PORCH, RGB_HSYNC_FRONT_PORCH, RGB_VSYNC_BACK_PORCH, RGB_VSYNC_FRONT_PORCH);
			lcd_hal_set_rgb_clk_rev_edge(POSEDGE_OUTPUT);
			break;
	}

	lcd_hal_disconti_mode(DISCONTINUE_MODE);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	bk_lcd_set_yuv_mode(input_data_format);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	return BK_OK;
}



bk_err_t bk_lcd_8080_init(uint16_t x_pixel, uint16_t y_pixel, pixel_format_t input_data_format)
{
	LCD_RETURN_ON_NOT_INIT();

#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
	/*
	 * GPIO info is setted in GPIO_DEFAULT_DEV_CONFIG and
	 * inited in bk_gpio_driver_init->gpio_hal_default_map_init.
	 * If needs to re-config GPIO, can deal it here.
	 */
#else
	lcd_mcu_gpio_init();
#endif
	lcd_hal_rgb_display_sel(0); //25bit - rgb_on = 0 select 8080 mode
	lcd_hal_rgb_display_en(0);
	lcd_hal_disconti_mode(DISCONTINUE_MODE);
	lcd_hal_8080_verify_1ms_count(VERIFY_1MS_COUNT);
	lcd_hal_8080_set_tik(TIK_CNT);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	lcd_hal_8080_set_fifo_data_thrd(CMD_FIFO_WR_THRD, CMD_FIFO_RD_THRD);
	bk_lcd_pixel_config(x_pixel, y_pixel);
	lcd_hal_8080_display_enable(1);
	lcd_hal_8080_int_enable(0, 1); //set eof int enable
	bk_lcd_set_yuv_mode(input_data_format);
	lcd_hal_8080_sleep_in(1);
	lcd_hal_8080_start_transfer(0);
	//delay(7017857); //reset need 131ms.
	return BK_OK;
}

static bk_err_t bk_lcd_driver_deinit(void)
{
	if (!s_lcd_driver_is_init) {
		LOGI("%s, lcd already deinit. \n", __func__);
		return BK_OK;
	}

	bk_int_isr_unregister(INT_SRC_LCD);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);
	if (sys_drv_lcd_close() != 0)
	{
		os_printf("lcd system deinit reg config error \r\n");
		return BK_FAIL;
	}
	s_lcd_driver_is_init = false;
	return BK_OK;

}

bk_err_t bk_lcd_8080_deinit(void)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_8080_sleep_in(1);
	lcd_hal_8080_int_enable(0, 0);
	lcd_hal_8080_display_enable(0);
	lcd_hal_8080_start_transfer(0);
	lcd_hal_soft_reset(1);
	rtos_delay_milliseconds(1);
	lcd_hal_soft_reset(0);
	bk_lcd_driver_deinit();

	return BK_OK;
}


bk_err_t bk_lcd_rgb_display_en(bool en)
{
	lcd_hal_rgb_display_en(en);
	return BK_OK;
}


bk_err_t bk_lcd_rgb_deinit(void)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_rgb_int_enable(0, 0);
	lcd_hal_rgb_display_en(0);
	lcd_hal_rgb_display_sel(0);

	lcd_hal_soft_reset(1);
	rtos_delay_milliseconds(1);
	lcd_hal_soft_reset(0);
	bk_lcd_driver_deinit();
	return BK_OK;
}

bk_err_t bk_lcd_8080_start_transfer(bool start)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_8080_start_transfer(start);
	return BK_OK;
}

bk_err_t lcd_driver_rgb_init(const lcd_config_t *config)
{
	const lcd_rgb_t *rgb = config->device->rgb;
	uint16_t x = ppi_to_pixel_x(config->device->ppi);  //lcd size x
	uint16_t y = ppi_to_pixel_y(config->device->ppi);  //lcd size y

	LOGI("%s\n", __func__);

	lcd_hal_set_rgb_clk_rev_edge(0);  //output data is in clk doen edge or up adge
	lcd_hal_rgb_display_sel(1);  //RGB display enable, and select rgb module
	lcd_hal_set_sync_low(HSYNC_BACK_LOW, VSYNC_BACK_LOW);
	lcd_hal_rgb_int_enable(0, 1);

	lcd_hal_rgb_sync_config(rgb->hsync_back_porch,
	                        rgb->hsync_front_porch,
	                        rgb->vsync_back_porch,
	                        rgb->vsync_front_porch);

	lcd_hal_set_rgb_clk_rev_edge(rgb->data_out_clk_edge);

	lcd_hal_disconti_mode(DISCONTINUE_MODE);

	bk_lcd_pixel_config(x, y); //image xpixel ypixel

	bk_lcd_set_yuv_mode(config->fmt);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);

	return BK_OK;
}

bk_err_t bk_lcd_set_partical_display(bool en, uint16_t partial_clum_l, uint16_t partial_clum_r, uint16_t partial_line_l, uint16_t partial_line_r)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_set_partical_display(en, partial_clum_l, partial_clum_r, partial_line_l, partial_line_r);
	return BK_OK;
}


bk_err_t lcd_driver_mcu_init(const lcd_config_t *config)
{
	uint16_t x = ppi_to_pixel_x(config->device->ppi);
	uint16_t y = ppi_to_pixel_y(config->device->ppi);

	LOGI("%s\n", __func__);

	lcd_hal_rgb_display_sel(0); //25bit - rgb_on = 0 select 8080 mode
	lcd_hal_disconti_mode(DISCONTINUE_MODE);
	lcd_hal_8080_verify_1ms_count(VERIFY_1MS_COUNT);
	lcd_hal_8080_set_tik(TIK_CNT);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	lcd_hal_8080_set_fifo_data_thrd(CMD_FIFO_WR_THRD, CMD_FIFO_RD_THRD);
	lcd_hal_pixel_config(x, y);
	lcd_hal_8080_display_enable(1);
	lcd_hal_8080_int_enable(0, 1); //set eof int enable
	bk_lcd_set_yuv_mode(config->fmt);
	lcd_hal_8080_sleep_in(1);
	return BK_OK;
}

#if CONFIG_PWM
bk_err_t lcd_driver_backlight_init(uint8_t pwm_id, uint8_t percent)
{
	pwm_init_config_t config = {0};
	BK_LOG_ON_ERR(bk_pwm_driver_init());

	if (percent > 100)
	{
		percent  = 100;
	}

	config.period_cycle = 100;
	config.duty_cycle = percent;
	BK_LOG_ON_ERR(bk_pwm_init(pwm_id, &config));
	BK_LOG_ON_ERR(bk_pwm_start(pwm_id));

	return BK_OK;
}

bk_err_t lcd_driver_backlight_deinit(uint8_t pwm_id)
{
	BK_LOG_ON_ERR(bk_pwm_stop(pwm_id));
	BK_LOG_ON_ERR(bk_pwm_deinit(pwm_id));
	return BK_OK;
}

#endif

bk_err_t lcd_driver_set_backlight(uint8_t percent)
{
	if (s_lcd.config.device->backlight_set)
	{
		s_lcd.config.device->backlight_set(percent);
	}
	return BK_OK;
}

bk_err_t lcd_driver_display_enable(void)
{
	lcd_type_t type;

	type = s_lcd.config.device->type;

	if (type == LCD_TYPE_RGB565)
	{
		lcd_hal_rgb_display_en(1);
	}
	else if (type == LCD_TYPE_MCU8080)
	{
		lcd_hal_8080_start_transfer(1);
		lcd_hal_8080_cmd_param_count(1);
		lcd_hal_8080_write_cmd(0x2c);
	}

	return BK_OK;
}
bk_err_t lcd_driver_display_continue(void)
{
	lcd_type_t type;
	type = s_lcd.config.device->type;
	if (type == LCD_TYPE_RGB565)
	{
	}

	if (type == LCD_TYPE_MCU8080)
	{
		lcd_hal_8080_write_cmd(0x3c);
	}
	return BK_OK;
}

bk_err_t lcd_driver_set_display_base_addr(uint32_t disp_base_addr)
{
	lcd_hal_set_display_read_base_addr(disp_base_addr);

	return BK_OK;
}
uint32_t bk_lcd_get_display_base_addr(void)
{
	return lcd_disp_ll_get_mater_rd_base_addr();
}

#if timer_control_pfs
static void lcd_fps_control_isr(timer_id_t timer_id)
{
	bk_timer_stop(TIMER_ID4);

	bk_lcd_rgb_display_en(1);
}
#endif

void lcd_driver_ppi_set(uint16_t width, uint16_t height)
{
	uint16_t x = ppi_to_pixel_x(s_lcd.config.device->ppi);
	uint16_t y = ppi_to_pixel_y(s_lcd.config.device->ppi);

	lcd_hal_pixel_config(width, height);

	if (x < width || y < height)
	{
		uint16_t start_x, end_x, start_y, end_y;

		start_x = (width - x) / 2 + 1;
		end_x = start_x + x - 1;

		start_y = (height - y) / 2 + 1;
		end_y = start_y + y - 1;

		LOGD("%s, offset %d, %d, %d, %d\n", __func__, start_x, end_x, start_y, end_y);

		bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
	}
}

static void lcd_driver_display_isr(void)
{
	LCD_DISPLAY_ISR_ENTRY();

	media_debug->isr_lcd++;
#if timer_control_pfs
	bk_timer_start(TIMER_ID4, 40, lcd_fps_control_isr);
	bk_lcd_rgb_display_en(0);
#endif
	if (s_lcd.enable == false)
	{
		return;
	}

	if (s_lcd.pingpong_frame != NULL)
	{
		if (bk_lcd_get_yuv_mode() != s_lcd.pingpong_frame->fmt)
		{
			lcd_driver_ppi_set(s_lcd.pingpong_frame->width, s_lcd.pingpong_frame->height);
			bk_lcd_set_yuv_mode(s_lcd.pingpong_frame->fmt);
		}

		if (s_lcd.display_frame != NULL)
		{
			media_debug->fps_lcd++;

			s_lcd.config.fb_free(s_lcd.display_frame);
			s_lcd.display_frame = s_lcd.pingpong_frame;
			s_lcd.pingpong_frame = NULL;
			lcd_driver_set_display_base_addr((uint32_t)s_lcd.display_frame->frame);
			rtos_set_semaphore(&s_lcd.disp_sem);
		}
		else
		{
			s_lcd.display_frame = s_lcd.pingpong_frame;
			s_lcd.pingpong_frame = NULL;
			rtos_set_semaphore(&s_lcd.disp_sem);
		}
	}
#if !timer_control_pfs // not timer control pfs
	lcd_driver_display_continue();
#endif

	LCD_DISPLAY_ISR_OUT();

}

static void jpeg_dec_eof_cb(jpeg_dec_res_t *result)
{
	bk_err_t ret = BK_FAIL;

	LCD_DECODER_END();

	media_debug->isr_decoder++;

	bk_timer_stop(TIMER_ID3);

	if (s_lcd.enable == false)
	{
		return;
	}
	s_lcd.decoder_frame->height = result->pixel_y;
	s_lcd.decoder_frame->width = result->pixel_x ;
	s_lcd.decoder_frame->size = result->size;

	if (result->ok == false)
	{
		if (s_lcd.decoder_frame)
		{
			s_lcd.config.fb_free(s_lcd.decoder_frame);
			s_lcd.decoder_frame = NULL;
		}

		LOGD("%s decoder error\n", __func__);
	}

	ret = rtos_set_semaphore(&s_lcd.dec_sem);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore set failed: %d\n", __func__, ret);
	}
}

static void lcd_driver_decoder_timeout(timer_id_t timer_id)
{
	bk_err_t ret = BK_FAIL;

	bk_timer_stop(TIMER_ID3);

	if (s_lcd.enable == false)
	{
		return;
	}

	LOGD("decoder timeout\n");
	bk_jpeg_dec_stop();

	if (s_lcd.decoder_frame)
	{
		s_lcd.config.fb_free(s_lcd.decoder_frame);
		s_lcd.decoder_frame = NULL;
	}

	ret = rtos_set_semaphore(&s_lcd.dec_sem);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore set failed: %d\n", __func__, ret);
	}
}


frame_buffer_t *lcd_driver_decoder_frame(frame_buffer_t *frame)
{
	bk_err_t ret = BK_FAIL;
	frame_buffer_t *dec_frame = NULL;
	uint64_t before, after;

	if (s_lcd.enable == false)
	{
		return dec_frame;
	}

	rtos_lock_mutex(&s_lcd.dec_lock);

	if (s_lcd.decoder_en == false)
	{
		rtos_unlock_mutex(&s_lcd.dec_lock);
		return dec_frame;
	}

	before = riscv_get_mtimer();

	s_lcd.decoder_frame = s_lcd.config.fb_malloc();

	if (s_lcd.decoder_frame == NULL)
	{
		LOGE("malloc decoder NULL\n");
		goto out;
	}

	s_lcd.decoder_frame->sequence = frame->sequence;

	if (!sw_decode_en)
	{
		s_lcd.decoder_frame->fmt = PIXEL_FMT_VUYY;

		LCD_DECODER_START();
		if (frame->fmt == PIXEL_FMT_UVC_JPEG)
		{
			ret = bk_jpeg_dec_dma_start(frame->length, frame->frame, s_lcd.decoder_frame->frame);
		}
		else
		{
			ret = bk_jpeg_dec_hw_start(frame->length, frame->frame, s_lcd.decoder_frame->frame);
		}

		if (ret != BK_OK)
		{
			LOGE("%s hw decoder error\n", __func__);
			s_lcd.config.fb_free(s_lcd.decoder_frame);
			s_lcd.decoder_frame = NULL;
			goto out;
		}

		bk_timer_start(TIMER_ID3, 200, lcd_driver_decoder_timeout);

		ret = rtos_get_semaphore(&s_lcd.dec_sem, BEKEN_NEVER_TIMEOUT);

		if (ret != BK_OK)
		{
			LOGE("%s semaphore get failed: %d\n", __func__, ret);
		}
	}
	else
	{
		s_lcd.decoder_frame->fmt = PIXEL_FMT_YUYV;
#if CONFIG_JPEG_DECODE
		ret = bk_jpeg_dec_sw_start(frame->length, frame->frame, s_lcd.decoder_frame->frame, NULL);
#endif
		s_lcd.decoder_frame->height = frame->height;
		s_lcd.decoder_frame->width = frame->width ;

		if (ret != BK_OK)
		{
			LOGE("%s sw decoder error\n", __func__);
			s_lcd.config.fb_free(s_lcd.decoder_frame);
			s_lcd.decoder_frame = NULL;
			goto out;
		}
	}
		if (s_lcd.decoder_frame == NULL)
		{
			media_debug->err_dec++;

			LOGD("%s decoder failed\n", __func__);
			ret = BK_FAIL;
			goto out;
		}

		dec_frame = s_lcd.decoder_frame;
		s_lcd.decoder_frame = NULL;

out:

	rtos_unlock_mutex(&s_lcd.dec_lock);

	after = riscv_get_mtimer();

	LOGD("decoder time: %lu\n", (after - before) / 26000);

	return dec_frame;
}


frame_buffer_t *lcd_driver_rotate_frame(frame_buffer_t *frame)
{
	frame_buffer_t *rot_frame = NULL;
	uint64_t before, after;

#ifdef CONFIG_MASTER_CORE
	bk_err_t ret = BK_FAIL;
	mb_chnl_cmd_t mb_cmd;
#endif

	LCD_ROTATE_START();

	if (s_lcd.enable == false)
	{
		s_lcd.config.fb_free(frame);
		return rot_frame;
	}

	rtos_lock_mutex(&s_lcd.rot_lock);

	if (s_lcd.rotate_en == false)
	{
		rtos_unlock_mutex(&s_lcd.rot_lock);
		s_lcd.config.fb_free(frame);
		return rot_frame;
	}

	before = riscv_get_mtimer();

	if (s_lcd.rotate_frame == NULL)
	{
		s_lcd.rotate_frame = s_lcd.config.fb_malloc();
	}

	s_lcd.rotate_frame->height = frame->width;
	s_lcd.rotate_frame->width = frame->height;
	s_lcd.rotate_frame->sequence = frame->sequence;
	s_lcd.rotate_frame->length = frame->width * frame->height * 2;
	s_lcd.rotate_frame->fmt = frame->fmt;


#ifdef CONFIG_MASTER_CORE

	mb_cmd.hdr.cmd = 0x18;
	mb_cmd.param1 = (uint32_t)frame;
	mb_cmd.param2 = (uint32_t)s_lcd.rotate_frame;
	mb_cmd.param3 = 0;

	//LOGI("%s start rotate\n", __func__);
	ret = mb_chnl_write(MB_CHNL_VID, &mb_cmd);

	if (ret != BK_OK)
	{
		LOGE("%s mb_chnl_write failed: %d\n", __func__, ret);
		goto error;
	}

	//LOGI("%s wait rotate\n", __func__);
	ret = rtos_get_semaphore(&s_lcd.rot_sem, BEKEN_NEVER_TIMEOUT);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore get failed: %d\n", __func__, ret);
		goto error;
	}

	s_lcd.rotate_frame->fmt = PIXEL_FMT_YUYV;
#else
	//TODO
#endif
	rot_frame = s_lcd.rotate_frame;
	s_lcd.rotate_frame = frame;

	rtos_unlock_mutex(&s_lcd.rot_lock);

	after = riscv_get_mtimer();
	LOGD("rotate time: %lu\n", (after - before) / 26000);

	LCD_ROTATE_END();

	return rot_frame;

#ifdef CONFIG_MASTER_CORE
error:

	s_lcd.config.fb_free(frame);

	if (s_lcd.rotate_frame)
	{
		s_lcd.config.fb_free(s_lcd.rotate_frame);
		s_lcd.rotate_frame = NULL;
	}
#endif

	rtos_unlock_mutex(&s_lcd.rot_lock);

	return NULL;
}

void lcd_driver_display_frame_with_gui(void *buffer, int width, int height)
{
    frame_buffer_t *frame = s_lcd.config.fb_malloc();

    if(frame){
        frame->fmt = PIXEL_FMT_RGB565;
        frame->frame = buffer;
        frame->width = width;
        frame->height = height;
        lcd_driver_display_frame(frame);
    }
    else{
        bk_printf("[%s][%d] fb malloc fail\r\n", __FUNCTION__, __LINE__);
    }
}

bk_err_t lcd_driver_display_frame(frame_buffer_t *frame)
{
	bk_err_t ret = BK_FAIL;
	uint64_t before, after;

#if timer_control_pfs
	bk_timer_stop(TIMER_ID4);
#endif

	if (s_lcd.enable == false)
	{
		s_lcd.config.fb_free(frame);
		return ret;
	}

	LCD_DISPLAY_START();

	before = riscv_get_mtimer();

	rtos_lock_mutex(&s_lcd.disp_lock);

	if (s_lcd.display_en == false)
	{
		s_lcd.config.fb_free(frame);
		rtos_unlock_mutex(&s_lcd.disp_lock);
		return ret;
	}

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();

	if (s_lcd.display_frame == NULL)
	{
		lcd_driver_ppi_set(frame->width, frame->height);
		bk_lcd_set_yuv_mode(frame->fmt);
		s_lcd.pingpong_frame = frame;

		lcd_driver_set_display_base_addr((uint32_t)frame->frame);
		lcd_driver_display_enable();
		LOGD("display start\n");
	}
	else
	{
		if (s_lcd.pingpong_frame != NULL)
		{
			s_lcd.config.fb_free(s_lcd.pingpong_frame);
		}

		s_lcd.pingpong_frame = frame;
#if timer_control_pfs
		lcd_driver_set_display_base_addr((uint32_t)frame->frame);
		lcd_driver_display_enable();
		LOGD("display start\n");
#endif
	}

	GLOBAL_INT_RESTORE();

	ret = rtos_get_semaphore(&s_lcd.disp_sem, BEKEN_NEVER_TIMEOUT);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore get failed: %d\n", __func__, ret);
	}

	rtos_unlock_mutex(&s_lcd.disp_lock);

	after = riscv_get_mtimer();

	LOGD("display time: %lu\n", (after - before) / 26000);

	LCD_DISPLAY_END();

	return ret;
}


bk_err_t lcd_driver_init(const lcd_config_t *config)
{
	int ret = BK_FAIL;
	lcd_clk_t clk = 0;
	const lcd_device_t *device = config->device;

	LCD_DIAG_DEBUG_INIT();


	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);

	if (device == NULL)
	{
		LOGE("%s, device need to be set\n", __func__);
		goto error;
	}

	media_debug->err_dec = 0;
	media_debug->fps_lcd = 0;
	media_debug->isr_decoder = 0;
	media_debug->isr_lcd = 0;

	os_memset(&s_lcd, 0, sizeof(s_lcd));
	os_memcpy(&s_lcd.config, config, sizeof(lcd_config_t));

	uint16_t x = ppi_to_pixel_x(config->device->ppi);  //lcd size x
	uint16_t y = ppi_to_pixel_y(config->device->ppi);  //lcd size y
	config->fb_display_init(x << 16 | y);

#ifdef CONFIG_MASTER_CORE
	/* Mailbox */
	mb_chnl_open(MB_CHNL_VID, NULL);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_RX_ISR, lcd_driver_mailbox_rx_isr);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_ISR, lcd_driver_mailbox_tx_isr);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_CMPL_ISR, lcd_driver_mailbox_tx_cmpl_isr);
#endif

	rtos_init_mutex(&s_lcd.dec_lock);
	rtos_init_mutex(&s_lcd.rot_lock);
	rtos_init_mutex(&s_lcd.disp_lock);

	s_lcd.decoder_en = true;
	s_lcd.rotate_en = true;
	s_lcd.display_en = true;

	ret = rtos_init_semaphore_ex(&s_lcd.dec_sem, 1, 0);

	if (ret != BK_OK)
	{
		LOGE("%s dec_sem init failed: %d\n", __func__, ret);
		return ret;
	}

	ret = rtos_init_semaphore_ex(&s_lcd.rot_sem, 1, 0);

	if (ret != BK_OK)
	{
		LOGE("%s rot_sem init failed: %d\n", __func__, ret);
		return ret;
	}

	ret = rtos_init_semaphore_ex(&s_lcd.disp_sem, 1, 0);

	if (ret != BK_OK)
	{
		LOGE("%s disp_sem init failed: %d\n", __func__, ret);
		return ret;
	}
	if (!sw_decode_en)
	{
		ret = bk_jpeg_dec_driver_init();
#if(1)  //enable jpeg complete int isr
			bk_jpeg_dec_isr_register(DEC_END_OF_FRAME, jpeg_dec_eof_cb);
#else   //enable uvc ppi 640X480 jpeg 24 line decode complete int isr
			bk_jpeg_dec_isr_register(DEC_END_OF_LINE_NUM, jpeg_dec_eof_cb);
#endif
	}
	else
	{
#if CONFIG_JPEG_DECODE
		ret = bk_jpeg_dec_sw_init();
#endif
	}


	if (ret != BK_OK)
	{
		LOGE("bk_jpeg_dec_driver_init failed\n");
		return ret;
	}
	if (device->type == LCD_TYPE_RGB565)
	{
		clk = device->rgb->clk;
	}
	else if (device->type == LCD_TYPE_MCU8080)
	{
		clk = device->mcu->clk;
	}

	LOGI("%s, LCD clk set: %d\n", __func__, clk);

	ret = bk_lcd_driver_init(clk);

	if (ret != BK_OK)
	{
		LOGE("%s, device clk set error\n", __func__);
		goto error;
	}

	if (device->type == LCD_TYPE_RGB565)
	{
		LOGI("%s, rgb eof register\n", __func__);
		s_lcd.lcd_rgb_frame_end_handler = lcd_driver_display_isr;
#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
		/*
		 * GPIO info is setted in GPIO_DEFAULT_DEV_CONFIG and
		 * inited in bk_gpio_driver_init->gpio_hal_default_map_init.
		 * If needs to re-config GPIO, can deal it here.
		 */
#else
		lcd_rgb_gpio_init();
#endif
		lcd_driver_rgb_init(config);
	}
	else if (device->type == LCD_TYPE_MCU8080)
	{
		LOGI("%s, mcu eof register\n", __func__);
		s_lcd.lcd_8080_frame_end_handler = lcd_driver_display_isr;
#if CONFIG_GPIO_DEFAULT_SET_SUPPORT
		/*
		 * GPIO info is setted in GPIO_DEFAULT_DEV_CONFIG and
		 * inited in bk_gpio_driver_init->gpio_hal_default_map_init.
		 * If needs to re-config GPIO, can deal it here.
		 */
#else
		lcd_mcu_gpio_init();
#endif
		lcd_driver_mcu_init(config);
	}

	uint64_t before, after;
	before = riscv_get_mtimer();

	if (device->init)
	{
		device->init();
	}
	
	after = riscv_get_mtimer();
	
	LOGI("lcd init time: %lu\n", (after - before) / 26000);

	if (device->backlight_open)
	{
		device->backlight_open();
	}

	s_lcd.enable = true;

	return ret;

error:
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_DEFAULT);
	s_lcd.enable = false;
	return ret;
}

bk_err_t lcd_driver_deinit(void)
{
	lcd_type_t type;
	bk_err_t ret = BK_OK;

	GLOBAL_INT_DECLARATION();

	if (s_lcd.config.device->backlight_close)
	{
		s_lcd.config.device->backlight_close();
	}
	if (s_lcd.config.device->lcd_off)
	{
		s_lcd.config.device->lcd_off();
	}

	GLOBAL_INT_DISABLE();
	s_lcd.enable = false;
	GLOBAL_INT_RESTORE();

	rtos_lock_mutex(&s_lcd.dec_lock);
	s_lcd.decoder_en = false;
	rtos_unlock_mutex(&s_lcd.dec_lock);

	rtos_lock_mutex(&s_lcd.rot_lock);
	s_lcd.rotate_en = false;
	rtos_unlock_mutex(&s_lcd.rot_lock);

	rtos_lock_mutex(&s_lcd.disp_lock);
	s_lcd.display_en = false;
	rtos_unlock_mutex(&s_lcd.disp_lock);

	type = s_lcd.config.device->type;

	bk_timer_stop(TIMER_ID3);
	bk_jpeg_dec_driver_deinit();

#ifdef CONFIG_MASTER_CORE
	/* Mailbox */
	mb_chnl_close(MB_CHNL_VID);
#endif

	/* LCD controller */
	if (type == LCD_TYPE_RGB565)
	{
		ret = bk_lcd_rgb_deinit();
		if (ret != 0)
		{
			LOGE("lcd system deinit reg config error \r\n");
			ret = BK_OK;
			goto out;
		}
	}
	else if (type == LCD_TYPE_MCU8080)
	{
		ret = bk_lcd_8080_deinit();
		if (ret != BK_OK)
		{
			LOGE("lcd system deinit reg config error \r\n");
			ret = BK_FAIL;
			goto out;
		}
	}

	if (s_lcd.rotate_frame)
	{
		s_lcd.config.fb_free(s_lcd.rotate_frame);
		s_lcd.rotate_frame = NULL;
	}

	if (s_lcd.decoder_frame)
	{
		s_lcd.config.fb_free(s_lcd.decoder_frame);
		s_lcd.decoder_frame = NULL;
	}

	if (s_lcd.pingpong_frame)
	{
		s_lcd.config.fb_free(s_lcd.pingpong_frame);
		s_lcd.pingpong_frame = NULL;
	}

	if (s_lcd.display_frame)
	{
		s_lcd.config.fb_free(s_lcd.display_frame);
		s_lcd.display_frame = NULL;
	}

	ret = rtos_deinit_semaphore(&s_lcd.dec_sem);

	if (ret != BK_OK)
	{
		LOGE("%s dec_sem deinit failed: %d\n", __func__, ret);
		return ret;
	}

	ret = rtos_deinit_semaphore(&s_lcd.rot_sem);

	if (ret != BK_OK)
	{
		LOGE("%s rot_sem deinit failed: %d\n", __func__, ret);
		return ret;
	}

	ret = rtos_deinit_semaphore(&s_lcd.disp_sem);

	if (ret != BK_OK)
	{
		LOGE("%s disp_sem deinit failed: %d\n", __func__, ret);
		return ret;
	}

	rtos_deinit_mutex(&s_lcd.dec_lock);
	rtos_deinit_mutex(&s_lcd.rot_lock);
	rtos_deinit_mutex(&s_lcd.disp_lock);

//	s_lcd.config.fb_display_deinit();

out:

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_DEFAULT);

	return ret;
}

static void dma2d_lcd_fill(uint32_t frameaddr, uint16_t width, uint16_t height, uint32_t color)
{
	uint32_t color_temp = (color << 16) | color;
	//  os_printf("displat color :%x\r\n", color_temp);
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M;                      /**< Mode Register to Memory */
	dma2d_config.init.color_mode       = DMA2D_OUTPUT_ARGB8888;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;     /**< No alpha inversion for the output image */
	bk_dma2d_driver_init(&dma2d_config);

	if (width == 0 && height == 0)
	{
	}
	else if (width == 0)
	{
		height = height / 2;
	}
	else if (height == 0)
	{
		width = width / 2;
	}
	else
	{
		width = width / 2;
	}
	bk_dma2d_start_transfer(&dma2d_config, color_temp, (uint32_t)frameaddr, width, height);
	while (bk_dma2d_is_transfer_busy())
	{
	}
}

static bk_err_t lcd_partical_display_config(uint16_t lcd_size_x, uint16_t lcd_size_y, uint16_t image_x,  uint16_t image_y)
{
	uint16_t start_x = 0;
	uint16_t end_x = 0;
	uint16_t start_y = 0;
	uint16_t end_y = 0;

	if (image_x <= lcd_size_x && image_y <= lcd_size_y)
	{
		bk_lcd_set_partical_display(0, start_x, end_x, start_y, end_y);
		LOGD("image less then lcd size, not set partical display \n");
	}
	else if (image_x > lcd_size_x && image_y > lcd_size_y)
	{
		start_x = (image_x - lcd_size_x) / 2 + 1;
		end_x = start_x + lcd_size_x - 1;
		start_y = (image_y - lcd_size_y) / 2 + 1;
		end_y = start_y + lcd_size_y - 1;
		bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
		LOGD("image exceed then lcd size, offset %d, %d, %d, %d\n", start_x, end_x, start_y, end_y);
	}
	else if (image_x > lcd_size_x && image_y <= lcd_size_y)
	{
		start_x = (image_x - lcd_size_x) / 2 + 1;
		end_x = start_x + lcd_size_x - 1;
		start_y = 1;
		end_y = image_y;
		bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
		LOGD("image h is less then lcd h size, offset %d, %d, %d, %d\n", start_x, end_x, start_y, end_y);
	}
	else if (image_x <= lcd_size_x && image_y > lcd_size_y)
	{
		start_x = 1;
		end_x = image_x;
		start_y = (image_y - lcd_size_y) / 2 + 1;
		end_y = start_y + lcd_size_y - 1;
		bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
		LOGD("image x is less then lcd x size, offset %d, %d, %d, %d\n", start_x, end_x, start_y, end_y);
	}
	else
	{
		LOGD("no lcd_partical_display_config\n");
	}

	return BK_OK;
}

bk_err_t bk_lcd_fill_color(lcd_device_id_t id, lcd_disp_framebuf_t *lcd_disp, uint32_t color)
{
	LCD_RETURN_ON_NOT_INIT();

	switch (id)
	{
		case LCD_DEVICE_ST7796S:
		{
			uint16_t lcd_size_x = 320;
			uint16_t lcd_size_y = 480;
			if (lcd_disp->rect.x + lcd_disp->rect.width > lcd_size_x)
			{
				lcd_disp->rect.width = abs(lcd_size_x - lcd_disp->rect.x);
			}
			if (lcd_disp->rect.y + lcd_disp->rect.height > lcd_size_y)
			{
				lcd_disp->rect.height = abs(lcd_size_y - lcd_disp->rect.y);
			}
			if(lcd_disp->rect.width < 100 || lcd_disp->rect.height < 100)
			{
				lcd_mcu_gpio_reinit();
			}

			dma2d_lcd_fill((uint32_t)lcd_disp->buffer, lcd_disp->rect.width, lcd_disp->rect.height, color);
			lcd_hal_pixel_config(lcd_disp->rect.width, lcd_disp->rect.height);

			lcd_partical_display_config(lcd_size_x, lcd_size_y, lcd_disp->rect.width, lcd_disp->rect.height);
			if (lcd_disp->rect.width <= lcd_size_x && lcd_disp->rect.height <= lcd_size_y)
			{
				LOGD("the picture w/h (%d,%d)is all less then lcd size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				LOGD(" xs,  xe,  ys, ye =  (%d,%d,%d,%d))\n", lcd_disp->rect.x, lcd_disp->rect.x + lcd_disp->rect.width, lcd_disp->rect.y, lcd_disp->rect.y + lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(lcd_disp->rect.x, lcd_disp->rect.x + lcd_disp->rect.width, lcd_disp->rect.y, lcd_disp->rect.y + lcd_disp->rect.height);
			}
			else if (lcd_disp->rect.width > lcd_size_x && lcd_disp->rect.height >= lcd_size_y)
			{
				LOGD("the picture w/h (%d,%d)is all exceed then lcd size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(0, lcd_size_x, 0, lcd_size_y);
			}
			else if (lcd_disp->rect.width > lcd_size_x && lcd_disp->rect.height < lcd_size_y)
			{
				LOGD("only picture (%d,%d) h is less then lcd h size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(0, lcd_size_x, 0, lcd_disp->rect.height);
			}
			else if (lcd_disp->rect.width < lcd_size_x && lcd_disp->rect.height > lcd_size_y)
			{
				LOGD("only picture (%d,%d) w is less then lcd x size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(0, lcd_disp->rect.width, 0, lcd_size_y);
			}
			else
			{
				LOGD("no lcd_st7796s_set_display_mem_area\n");
			}
			break;
		}
		default:
			break;
	}
	lcd_driver_set_display_base_addr((uint32_t)lcd_disp->buffer);
	lcd_hal_8080_start_transfer(1);
	return BK_OK;
}


bk_err_t bk_lcd_fill_data(lcd_device_id_t id, lcd_disp_framebuf_t *lcd_disp)
{
	LCD_RETURN_ON_NOT_INIT();

	uint16_t lcd_size_x = 480;
	uint16_t lcd_size_y = 272;

	switch (id)
	{
		case LCD_DEVICE_ST7796S:
		{
			lcd_size_x = 320;
			lcd_size_y = 480;
#if(1)
			if(lcd_disp->rect.width < 100 || lcd_disp->rect.height < 100)
			{
				lcd_mcu_gpio_reinit();
			}
			lcd_hal_pixel_config(lcd_disp->rect.width, lcd_disp->rect.height);
			lcd_partical_display_config(lcd_size_x, lcd_size_y, lcd_disp->rect.width, lcd_disp->rect.height);
			if (lcd_disp->rect.width <= lcd_size_x && lcd_disp->rect.height <= lcd_size_y)
			{
				if (lcd_disp->rect.x + lcd_disp->rect.width > lcd_size_x)
				{
					lcd_disp->rect.width = abs(lcd_size_x - lcd_disp->rect.x);
				}
				if (lcd_disp->rect.y + lcd_disp->rect.height >  lcd_size_y)
				{
					lcd_disp->rect.height = abs(lcd_size_y - lcd_disp->rect.y);
				}
				LOGD("the picture w/h (%d,%d)is all less then lcd size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				LOGD(" xs,  xe,  ys, ye =  (%d,%d,%d,%d))\n", lcd_disp->rect.x, lcd_disp->rect.x + lcd_disp->rect.width, lcd_disp->rect.y, lcd_disp->rect.y + lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(lcd_disp->rect.x, lcd_disp->rect.x + lcd_disp->rect.width, lcd_disp->rect.y, lcd_disp->rect.y + lcd_disp->rect.height);
			}
			else if (lcd_disp->rect.width > lcd_size_x && lcd_disp->rect.height >= lcd_size_y)
			{
				LOGD("the picture w/h (%d,%d)is all exceed then lcd size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(0, lcd_size_x, 0, lcd_size_y);
			}
			else if (lcd_disp->rect.width > lcd_size_x && lcd_disp->rect.height < lcd_size_y)
			{
				LOGD("only picture (%d,%d) h is less then lcd h size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(0, lcd_size_x, 0, lcd_disp->rect.height);
			}
			else if (lcd_disp->rect.width < lcd_size_x && lcd_disp->rect.height >= lcd_size_y)
			{
				LOGD("only picture (%d,%d) w is less then lcd x size\n", lcd_disp->rect.width, lcd_disp->rect.height);
				lcd_st7796s_set_display_mem_area(0, lcd_disp->rect.width, 0, lcd_size_y);
			}
			else
			{
				LOGD("no set st7796 2a 2b\n");
			}

			lcd_driver_set_display_base_addr((uint32_t)lcd_disp->buffer);
			lcd_hal_8080_start_transfer(1);
			bk_lcd_8080_ram_write(0x2c);
#else  //cpu fill lcd data fifo, max w x h is 3 x 3, for example: fill (x,y)=(0,0), (w*h)=(3x3)or(2x2)or(1*1)
			lcd_partical_display_config(lcd_size_x, lcd_size_y, lcd_disp->rect.width, lcd_disp->rect.height);
			lcd_st7796s_set_display_mem_area(lcd_disp->rect.x, lcd_disp->rect.x + lcd_disp->rect.width, lcd_disp->rect.y, lcd_disp->rect.y + lcd_disp->rect.height);
			bk_lcd_send_data(0x2c, (uint16_t *)(lcd_disp->buffer), lcd_disp->rect.height * lcd_disp->rect.width);
#endif
			break;
		}
		case LCD_DEVICE_ST7282:
			lcd_size_x = 480;
			lcd_size_y = 272;
			lcd_hal_pixel_config(lcd_disp->rect.width, lcd_disp->rect.height);
			lcd_partical_display_config(lcd_size_x, lcd_size_y, lcd_disp->rect.width, lcd_disp->rect.height);
			lcd_driver_set_display_base_addr((uint32_t)lcd_disp->buffer);
			lcd_hal_rgb_display_en(1);
			break;
		default:
			break;
	}
	return BK_OK;
}


bk_err_t bk_lcd_draw_point(lcd_device_id_t id, uint16_t x, uint16_t y, uint16_t pixel)
{
	LCD_RETURN_ON_NOT_INIT();

	uint16 pixel_l, pixel_h;

	pixel_h = pixel >> 8;
	pixel_l = pixel & 0xff;
	uint32_t param_clumn[2] = {pixel_h, pixel_l};

	switch (id)
	{
		case LCD_DEVICE_ST7796S:
			if (x >= 320 || y >= 480)
			{
				x = 320 - 1;
				y = 480 - 1;
			}
			lcd_st7796s_set_display_mem_area(x, x + 1, y, y + 1);
			break;
		default:
			break;
	}
	lcd_hal_8080_cmd_send(2, 0x2c, param_clumn);
	return BK_OK;
}


