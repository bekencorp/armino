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
#include <os/str.h>
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
#include <soc/mapping.h>
#include <driver/lcd_spi.h>

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
#include <driver/dma2d.h>
#include "modules/image_scale.h"
#include <driver/dma2d_types.h>

#if  CONFIG_LCD_FONT_BLEND
#include "modules/lcd_font.h"
#endif

#if CONFIG_LCD_QSPI
#include <driver/lcd_qspi.h>
#include <driver/lcd_qspi_types.h>
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
uint8_t *yuv_data = NULL;
uint8_t *rgb565_data = NULL;
extern uint8_t g_wifi_current_level;

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

#if (CONFIG_LCD_QSPI && CONFIG_LVGL)
extern lcd_qspi_device_t *s_device_config;
#endif

extern media_debug_t *media_debug;
extern u64 riscv_get_mtimer(void);
extern uint8_t get_decode_mode(void);
typedef enum
{
	SOFTWARE_DECODING_CPU0,
	SOFTWARE_DECODING_CPU1,
	HARDWARE_DECODING,
} decode_mode_t;

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
	uint8_t dma2d_blend : 1;
    uint8_t result;

	frame_buffer_t *decoder_frame;
	frame_buffer_t *rotate_frame;
	frame_buffer_t *display_frame;
	frame_buffer_t *lvgl_frame;
	frame_buffer_t *pingpong_frame;

	beken_semaphore_t dec_sem;
	beken_semaphore_t rot_sem;
	beken_semaphore_t disp_sem;
#if (USE_DMA2D_BLEND_ISR_CALLBACKS == 1)
	beken_semaphore_t dma2d_complete_sem;
	beken_semaphore_t dma2d_err_sem;
#endif
	beken_mutex_t dec_lock;
	beken_mutex_t rot_lock;
	beken_mutex_t disp_lock;

} lcd_driver_t;


extern bk_err_t bk_lcd_set_yuv_mode(pixel_format_t input_data_format);

const lcd_device_t *lcd_devices[] =
{
#if CONFIG_LCD_ST7282
	&lcd_device_st7282,
#endif
#if CONFIG_LCD_HX8282
	&lcd_device_hx8282,
#endif
#if CONFIG_LCD_ST7796S
	&lcd_device_st7796s,
#endif
#if CONFIG_LCD_GC9503V
	&lcd_device_gc9503v,
#endif
#if CONFIG_LCD_NT35512
	&lcd_device_nt35512,
#endif
#if CONFIG_LCD_NT35510
	&lcd_device_nt35510,
#endif
#if CONFIG_LCD_NT35510_MCU
	&lcd_device_nt35510_mcu,
#endif
#if CONFIG_LCD_H050IWV
	&lcd_device_h050iwv,
#endif
#if CONFIG_LCD_MD0430R
	&lcd_device_md0430r,
#endif
#if CONFIG_LCD_MD0700R
	&lcd_device_md0700r,
#endif

#if CONFIG_LCD_ST7710S
	&lcd_device_st7710s,
#endif

#if CONFIG_LCD_ST7701S
	&lcd_device_st7701s,
#endif

#if CONFIG_LCD_SN5ST7701S
	&lcd_device_sn5st7701s,
#endif

#if CONFIG_LCD_ST7701S_LY
	&lcd_device_st7701s_ly,
#endif

};

static lcd_driver_t s_lcd = {0};

const lcd_device_t * get_lcd_device_by_name(char * name)
{
	uint32_t i, size = sizeof(lcd_devices) / sizeof(lcd_device_t *);

	for (i = 0; i < size; i++)
	{
		if (os_memcmp(lcd_devices[i]->name, name, os_strlen(name)) == 0)
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

	if (cmd_buf->hdr.cmd == EVENT_LCD_ROTATE_MBRSP)
	{
		ret = rtos_set_semaphore(&s_lcd.rot_sem);

		if (ret != BK_OK)
		{
			LOGE("%s semaphore get failed: %d\n", __func__, ret);
		}
	}

	if (cmd_buf->hdr.cmd == EVENT_LCD_DEC_SW_MBRSP)
	{
		ret = rtos_set_semaphore(&s_lcd.dec_sem);

		if (ret != BK_OK)
		{
			LOGE("%s semaphore get failed: %d\n", __func__, ret);
		}
        s_lcd.result = (uint8_t)cmd_buf->param3;
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
__attribute__((section(".itcm_sec_code"))) void lcd_isr();

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

__attribute__((section(".itcm_sec_code"))) void lcd_isr()
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
		LOGE("%s already init. \n", __func__);
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
		case LCD_30M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_1, DISP_INT_EN, DSIP_DISCLK_ALWAYS_ON);
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
		case PIXEL_FMT_BGR565:
			lcd_hal_display_yuv_sel(0);
			lcd_hal_set_pixel_reverse(1);
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
		LOGE("%s, lcd already deinit. \n", __func__);
		return BK_OK;
	}

	bk_int_isr_unregister(INT_SRC_LCD);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);
	if (sys_drv_lcd_close() != 0)
	{
		LOGE("lcd system deinit reg config error \r\n");
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
	lcd_hal_set_sync_low(rgb->hsync_pulse_width, rgb->vsync_pulse_width);
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
		if(s_lcd.config.device->id == LCD_DEVICE_NT35510_MCU)
		{
			lcd_hal_8080_start_transfer(0);
			if(s_lcd.config.device->mcu->set_display_area)
				s_lcd.config.device->mcu->set_display_area(0, 479, 0, 799);

			if(s_lcd.config.device->mcu->start_transform)
				s_lcd.config.device->mcu->start_transform();
		}

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
		if(s_lcd.config.device->id != LCD_DEVICE_NT35510_MCU)
		{
			lcd_hal_8080_write_cmd(0x3c);
		}
		else
		{
			lcd_hal_8080_start_transfer(0);

			if(s_lcd.config.device->mcu->continue_transform)
				s_lcd.config.device->mcu->continue_transform();

			lcd_hal_8080_start_transfer(1);
			lcd_hal_8080_cmd_param_count(1);
			lcd_hal_8080_write_cmd(0x3c);
		}
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
	uint16_t start_x = 1;
	uint16_t start_y = 1;
	uint16_t end_x = x;
	uint16_t end_y = y;

	if (x < width || y < height)
	{
		if (x < width)
		{
			start_x = (width - x) / 2 + 1;
			end_x = start_x + x - 1;
		}
		if (y < height)
		{
			start_y = (height - y) / 2 + 1;
			end_y = start_y + y - 1;
		}

		LOGD("%s, offset %d, %d, %d, %d\n", __func__, start_x, end_x, start_y, end_y);
	}
	
	bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
}


#define LCD_DRIVER_FRAME_FREE(frame)			\
	do {										\
		if (frame								\
			&& s_lcd.lvgl_frame != frame)		\
		{										\
			s_lcd.config.fb_free(frame);		\
			frame = NULL;						\
		}										\
	} while (0)

__attribute__((section(".itcm_sec_code")))  void flash_busy_lcd_callback(void)
{
	uint32_t int_level = rtos_disable_int();

	if (lcd_disp_ll_get_display_int_rgb_eof())
	{
		lcd_disp_ll_set_display_int_rgb_eof(1);
	}
	
	rtos_enable_int(int_level);
}

__attribute__((section(".itcm_sec_code"))) static void lcd_driver_display_rgb_isr(void)
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
		lcd_driver_ppi_set(s_lcd.pingpong_frame->width, s_lcd.pingpong_frame->height);
		bk_lcd_set_yuv_mode(s_lcd.pingpong_frame->fmt);

		if (s_lcd.display_frame != NULL)
		{
			media_debug->fps_lcd++;

			LCD_DRIVER_FRAME_FREE(s_lcd.display_frame);

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
	//lcd_driver_display_continue();
#endif

	LCD_DISPLAY_ISR_OUT();

}

static void lcd_driver_display_mcu_isr(void)
{
	LCD_DISPLAY_ISR_ENTRY();

	media_debug->isr_lcd++;

	if (s_lcd.enable == false)
	{
		return;
	}

	if (s_lcd.pingpong_frame != NULL)
	{
		media_debug->fps_lcd++;
		LCD_DRIVER_FRAME_FREE(s_lcd.display_frame);

		s_lcd.display_frame = s_lcd.pingpong_frame;
		s_lcd.pingpong_frame = NULL;
		bk_lcd_8080_start_transfer(0);

		rtos_set_semaphore(&s_lcd.disp_sem);
	}

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
	if (s_lcd.decoder_frame)
	{
		s_lcd.decoder_frame->height = result->pixel_y;
		s_lcd.decoder_frame->width = result->pixel_x ;
		s_lcd.decoder_frame->length = result->pixel_y * result->pixel_x *2;
	}

	if (result->ok == false)
	{
		if (s_lcd.decoder_frame)
		{
			LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
		}

		media_debug->isr_decoder--;
		LOGD("%s decoder error\n", __func__);
	}

	ret = rtos_set_semaphore(&s_lcd.dec_sem);

	if (ret != BK_OK)
	{
		LOGE("%s semaphore set failed: %d\n", __func__, ret);
	}
}


#if (USE_DMA2D_BLEND_ISR_CALLBACKS == 1)
static void dma2d_config_error(void)
{
	LOGE("%s \n", __func__);
	rtos_set_semaphore(&s_lcd.dma2d_err_sem);
}

static void dma2d_transfer_error(void)
{
	LOGE("%s \n", __func__);
	rtos_set_semaphore(&s_lcd.dma2d_err_sem);
}

static void dma2d_transfer_complete(void)
{
	LOGD("%s \n", __func__);
	rtos_set_semaphore(&s_lcd.dma2d_complete_sem);
}
#endif

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
		LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
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

#ifdef CONFIG_MASTER_CORE
	 mb_chnl_cmd_t mb_cmd;
#endif
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

#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif

	s_lcd.decoder_frame = s_lcd.config.fb_malloc();

	if (s_lcd.decoder_frame == NULL)
	{
		LOGE("malloc decoder NULL\n");
		goto out;
	}

	s_lcd.decoder_frame->sequence = frame->sequence;

	if (get_decode_mode() == HARDWARE_DECODING)
	{
		s_lcd.decoder_frame->fmt = PIXEL_FMT_VUYY;

		LCD_DECODER_START();
		if (frame->fmt == PIXEL_FMT_UVC_JPEG)
		{
			if (frame->length <= 128 * 1024)
			{
				ret = bk_jpeg_dec_dma_start(frame->length, frame->frame, s_lcd.decoder_frame->frame);
			}
			else
			{
				LOGE("uvc output image too large!\n");
				ret = BK_FAIL;
			}
		}
		else
		{
			ret = bk_jpeg_dec_hw_start(frame->length, frame->frame, s_lcd.decoder_frame->frame);
		}

		if (ret != BK_OK)
		{
			LOGE("%s hw decoder error\n", __func__);
			LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
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
#ifdef CONFIG_MASTER_CORE
		if (get_decode_mode() == SOFTWARE_DECODING_CPU1)
		{
			mb_cmd.hdr.cmd = EVENT_LCD_DEC_SW_MBCMD;
			mb_cmd.param1 = (uint32_t)frame;
			mb_cmd.param2 = (uint32_t)s_lcd.decoder_frame;
			mb_cmd.param3 = 0;
			s_lcd.result = BK_FAIL;

			ret = mb_chnl_write(MB_CHNL_VID, &mb_cmd);

			if (ret != BK_OK)
			{
				LOGE("%s mb_chnl_write failed: %d\n", __func__, ret);
				LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
				goto out;
			}

			//LOGI("%s wait rotate\n", __func__);
			ret = rtos_get_semaphore(&s_lcd.dec_sem, BEKEN_NEVER_TIMEOUT);

			if (ret != BK_OK)
			{
				LOGE("%s semaphore get failed: %d\n", __func__, ret);
				LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
				goto out;
			}

			if (s_lcd.result != BK_OK)
			{
				LOGE("%s sw decoder error\n", __func__);
				LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
				goto out;
			}
		}
		else if (get_decode_mode() == SOFTWARE_DECODING_CPU0)
		{
			sw_jpeg_dec_res_t result;
			ret = bk_jpeg_dec_sw_start(frame->length, frame->frame, s_lcd.decoder_frame->frame, &result);
#if (CONFIG_TASK_WDT)
			extern void bk_task_wdt_feed(void);
			bk_task_wdt_feed();
#endif
			if (ret != BK_OK)
			{
				LOGE("%s sw decoder error\n", __func__);
				LCD_DRIVER_FRAME_FREE(s_lcd.decoder_frame);
				goto out;
			}
			else
			{
				s_lcd.decoder_frame->height = result.pixel_y;
				s_lcd.decoder_frame->width = result.pixel_x ;
			}
		}
#endif
#endif
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
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	LOGD("decoder time: %lu\n", (after - before) / 26000);

	return dec_frame;
}


frame_buffer_t *lcd_driver_rodegree_frame(frame_buffer_t *frame, media_rotate_t rotate)
{
	frame_buffer_t *rot_frame = NULL;
	uint64_t before, after;

#ifdef CONFIG_MASTER_CORE
	bk_err_t ret = BK_FAIL;
	mb_chnl_cmd_t mb_cmd;
#endif

	if (rotate == ROTATE_NONE
		|| rotate == ROTATE_180)
	{
		LOGE("%s no supported paramters\n", __func__);
		return frame;
	}

	LCD_ROTATE_START();

	if (s_lcd.enable == false)
	{
		LCD_DRIVER_FRAME_FREE(frame);
		return rot_frame;
	}

	rtos_lock_mutex(&s_lcd.rot_lock);

	if (s_lcd.rotate_en == false)
	{
		rtos_unlock_mutex(&s_lcd.rot_lock);
		LCD_DRIVER_FRAME_FREE(frame);
		return rot_frame;
	}
#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif
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

	mb_cmd.hdr.cmd = EVENT_LCD_ROTATE_MBCMD;
	mb_cmd.param1 = (uint32_t)frame;
	mb_cmd.param2 = (uint32_t)s_lcd.rotate_frame;
	mb_cmd.param3 = rotate;

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

#else
	//TODO
#endif
	rot_frame = s_lcd.rotate_frame;
	s_lcd.rotate_frame = frame;

	rtos_unlock_mutex(&s_lcd.rot_lock);
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	LOGD("rotate time: %lu\n", (after - before) / 26000);

	LCD_ROTATE_END();

	return rot_frame;

#ifdef CONFIG_MASTER_CORE
error:

	LCD_DRIVER_FRAME_FREE(frame);

	if (s_lcd.rotate_frame)
	{
		LCD_DRIVER_FRAME_FREE(s_lcd.rotate_frame);
	}
#endif

	rtos_unlock_mutex(&s_lcd.rot_lock);

	return NULL;
}

frame_buffer_t *lcd_driver_rotate_frame(frame_buffer_t *frame)
{
	return lcd_driver_rodegree_frame(frame, ROTATE_90);
}


frame_buffer_t *lcd_driver_rotate_frame_ppi(frame_buffer_t *frame, media_ppi_t ppi)
{
	frame_buffer_t *rot_frame = NULL;
	uint64_t before, after;
	uint16_t dest_width, dest_height;

#ifdef CONFIG_MASTER_CORE
	bk_err_t ret = BK_FAIL;
	mb_chnl_cmd_t mb_cmd;
#endif

	dest_width = ppi >> 16;
	dest_height = ppi & 0xFFFF;

	if (frame->width < dest_height || frame->height < dest_width)
	{
		s_lcd.config.fb_free(frame);
		return rot_frame;
	}

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
#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif
	if (s_lcd.rotate_frame == NULL)
	{
		s_lcd.rotate_frame = s_lcd.config.fb_malloc();
	}

	s_lcd.rotate_frame->height = dest_height;
	s_lcd.rotate_frame->width = dest_width;
	s_lcd.rotate_frame->sequence = frame->sequence;
	s_lcd.rotate_frame->length = dest_width * dest_height * 2;
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

#else
	//TODO
#endif
	rot_frame = s_lcd.rotate_frame;
	s_lcd.rotate_frame = frame;

	rtos_unlock_mutex(&s_lcd.rot_lock);
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
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

/**
  * @brief  dma2d_memcpy for src and dst addr is in psram
  * @param  Psrc.   memcpy or partical copy start addr, in other word, the lcd frame addr add offset addr is the start copy addr.
  * @param  Pdst.   memcpy or partical copy dst addr, in other word, the dst frame addr add dst offset addr is the end copy addr.
  * @param  xsize   the memcpy or partical copy width
  * @param  ysize   the memcpy or partical copy hight
  * @param  src_offline. for partical copy this is to calculate start copy addr based frame addr, uint by pixel
  * @param  dest_offline for partical copy this is to calculate copy to dst addr based frame addr, uint by pixel

  * @return none
  */
void dma2d_memcpy_psram(void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline)
{
	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_ARGB8888; /**< Output color mode is ARGB4444 : 16 bpp */
	dma2d_config.init.output_offset = dest_offline/2;                   /**< No offset on output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888; /**< Input color is ARGB4444 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offline/2;                     /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;      /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

	bk_dma2d_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, (uint32_t)Pdst, xsize/2, ysize);
	while (bk_dma2d_is_transfer_busy()) {}
}

void dma2d_memcpy_psram_wait_last_transform(void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline)
{
	dma2d_config_t dma2d_config = {0};
	static u8 flag = 0;

	if(1 == flag)
	{
		while (bk_dma2d_is_transfer_busy()) {}
		flag = 0;
	}

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_ARGB8888; /**< Output color mode is ARGB4444 : 16 bpp */
	dma2d_config.init.output_offset = dest_offline/2;                   /**< No offset on output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888; /**< Input color is ARGB4444 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offline/2;                     /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;      /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

	bk_dma2d_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, (uint32_t)Pdst, xsize/2, ysize);
	flag = 1;
}

extern void lcd_storage_capture_save(char * capture_name, uint8_t *addr, uint32_t len);
//extern void bk_mem_dump_ex(const char * title, unsigned char * data, uint32_t data_len);

bk_err_t lcd_driver_blend(lcd_blend_t *lcd_blend)
{
#if CONFIG_BLEND_USE_GUI
#define BLEND_DATA_MAX_ROW   2
    uint8_t *tmp_yuv_data = NULL;
    uint8_t *tmp_rgb565_data = NULL;
    uint8_t *tmp_blend_data = NULL;
    int tmp_blend_data_size = 0;
    int y = 0;
    int blend_y_size = 0;
    dma2d_blend_t dma2d_config;
    int addr_offset = 0;

    void lv_get_gui_blend_buff(u8 **yuv_data, u8 **rgb565_data, u8 **gui_bak_data);
    lv_get_gui_blend_buff(&tmp_yuv_data, &tmp_rgb565_data, NULL);

    pixel_format_t format = lcd_blend->bg_data_format;

    //STEP 1 : bg img yuyv copy to sram and pixel convert to rgb565
    dma2d_memcpy_psram(lcd_blend->pbg_addr, tmp_yuv_data, lcd_blend->xsize, lcd_blend->ysize, lcd_blend->bg_offline, 0);

    if (PIXEL_FMT_VUYY == format)
    {
        vuyy_to_rgb565_convert((unsigned char *)tmp_yuv_data, (unsigned char *)tmp_rgb565_data, lcd_blend->xsize, lcd_blend->ysize);
    }
    else
    {
        yuyv_to_rgb565_convert((unsigned char *)tmp_yuv_data, (unsigned char *)tmp_rgb565_data, lcd_blend->xsize, lcd_blend->ysize);
    }

    tmp_blend_data_size = lcd_blend->xsize * BLEND_DATA_MAX_ROW * 2;
    tmp_blend_data = (uint8_t *)os_malloc(tmp_blend_data_size);
    if(!tmp_blend_data)
    {
        bk_printf("[%s][%d] malloc fail\r\n", __FUNCTION__, __LINE__);
        return BK_FAIL;
    }

    //STEP 2 : bg img rgb565 and fg img argb8888 blend , the resulr output bg mem
    for(y=0; y<lcd_blend->ysize; y+=BLEND_DATA_MAX_ROW)
    {
        if((y+BLEND_DATA_MAX_ROW) <= lcd_blend->ysize)
            blend_y_size = BLEND_DATA_MAX_ROW;
        else
            blend_y_size = lcd_blend->ysize - y;

        dma2d_memcpy_psram(tmp_rgb565_data + addr_offset, tmp_blend_data, lcd_blend->xsize, blend_y_size, 0, 0);
        //os_memcpy(tmp_blend_data, tmp_rgb565_data + addr_offset, lcd_blend->xsize * blend_y_size * 2);

#if CONFIG_BLEND_GUI_OUTPUT_888
        dma2d_config.pfg_addr = lcd_blend->pfg_addr + addr_offset*2;
#else
        dma2d_config.pfg_addr = lcd_blend->pfg_addr + addr_offset;
#endif
        dma2d_config.pbg_addr = tmp_blend_data;
        dma2d_config.pdst_addr = tmp_blend_data;
#if CONFIG_BLEND_GUI_OUTPUT_888
        dma2d_config.fg_color_mode = DMA2D_INPUT_ARGB8888;
        dma2d_config.red_bule_swap = DMA2D_RB_SWAP;
#else
        dma2d_config.fg_color_mode = DMA2D_INPUT_RGB565;
        dma2d_config.red_bule_swap = DMA2D_RB_REGULAR;
#endif
        dma2d_config.bg_color_mode = DMA2D_INPUT_RGB565;
        dma2d_config.dst_color_mode = DMA2D_OUTPUT_RGB565;
        dma2d_config.fg_offline = 0;
        dma2d_config.bg_offline = 0;
        dma2d_config.dest_offline = 0;
        dma2d_config.xsize = lcd_blend->xsize;
        dma2d_config.ysize = blend_y_size;
        dma2d_config.fg_alpha_value = lcd_blend->fg_alpha_value;
        dma2d_config.bg_alpha_value = lcd_blend->bg_alpha_value;
        bk_dma2d_blend(&dma2d_config);
		while (bk_dma2d_is_transfer_busy()) {}

        dma2d_memcpy_psram(tmp_blend_data, tmp_yuv_data + addr_offset, lcd_blend->xsize, blend_y_size, 0, 0);
        //os_memcpy_word((uint32_t *)(tmp_yuv_data + addr_offset), (const uint32_t *)tmp_blend_data, lcd_blend->xsize * blend_y_size * 2);
        addr_offset += lcd_blend->xsize * blend_y_size * 2;
    }

    os_free(tmp_blend_data);
    tmp_blend_data = NULL;

    if (PIXEL_FMT_VUYY == format)
    {
        rgb565_to_vuyy_convert((uint16_t *)tmp_yuv_data, (uint16_t *)tmp_rgb565_data, lcd_blend->xsize, lcd_blend->ysize);
    }
    else
    {
        rgb565_to_yuyv_convert((uint16_t *)tmp_yuv_data, (uint16_t *)tmp_rgb565_data, lcd_blend->xsize, lcd_blend->ysize);
    }
    dma2d_memcpy_psram(tmp_rgb565_data, lcd_blend->pbg_addr, lcd_blend->xsize, lcd_blend->ysize, 0, lcd_blend->bg_offline);
#elif CONFIG_LCD_DMA2D_BLEND_FLASH_IMG || CONFIG_LCD_FONT_BLEND
	if (s_lcd.dma2d_blend == true)
	{
#if 0 //blend yuyv not use pixel convert and dma2d
		pixel_format_t format = lcd_blend->bg_data_format;

		//STEP 1 : bg img yuyv copy to sram and pixel convert to rgb565
#if 0
		dma2d_memcpy_psram(lcd_blend->pbg_addr, yuv_data, lcd_blend->xsize, lcd_blend->ysize, lcd_blend->bg_offline, 0);
#else
		register uint32_t i =0;
		uint32_t * p_yuv_src = (uint32_t *)lcd_blend->pbg_addr;
		uint32_t * p_yuv_dst = (uint32_t *)yuv_data;
		for(i = 0; i < lcd_blend->ysize; i++)
		{
			os_memcpy_word(p_yuv_dst, p_yuv_src, lcd_blend->xsize*2);
			p_yuv_dst += (lcd_blend->xsize/2);
			p_yuv_src += (lcd_blend->bg_width/2);
		}
#endif
	//	lcd_storage_capture_save("yuv_data.yuv", yuv_data, len);

		if (PIXEL_FMT_VUYY == format)
		{
			vuyy_to_rgb565_convert((unsigned char *)yuv_data, (unsigned char *)rgb565_data, lcd_blend->xsize, lcd_blend->ysize);
		}
		else
		{
			yuyv_to_rgb565_convert((unsigned char *)yuv_data, (unsigned char *)rgb565_data, lcd_blend->xsize, lcd_blend->ysize);
		}
	//	lcd_storage_capture_save("yuv_to_rgb565.rgb", rgb565_data, len);

		//STEP 2 : bg img rgb565 and fg img argb8888 blend , the resulr output bg mem
		dma2d_blend_t dma2d_config;
		dma2d_config.pfg_addr = lcd_blend->pfg_addr;
		dma2d_config.pbg_addr = rgb565_data;
		dma2d_config.pdst_addr = rgb565_data;
		if (lcd_blend->fg_data_format == ARGB8888)
		{
			dma2d_config.fg_color_mode = DMA2D_INPUT_ARGB8888;
			dma2d_config.red_bule_swap = DMA2D_RB_SWAP;
		}
		if (lcd_blend->fg_data_format == RGB565)
		{
			dma2d_config.fg_color_mode = DMA2D_INPUT_RGB565;
			dma2d_config.red_bule_swap = DMA2D_RB_REGULAR;
		}
		dma2d_config.bg_color_mode = DMA2D_INPUT_RGB565;
		dma2d_config.dst_color_mode = DMA2D_OUTPUT_RGB565;
		dma2d_config.fg_offline = 0;
		dma2d_config.bg_offline = 0;
		dma2d_config.dest_offline = 0;
		dma2d_config.xsize = lcd_blend->xsize;
		dma2d_config.ysize = lcd_blend->ysize;
		dma2d_config.fg_alpha_value = lcd_blend->fg_alpha_value;
		dma2d_config.bg_alpha_value = lcd_blend->bg_alpha_value;
		bk_dma2d_blend(&dma2d_config);
	//	lcd_storage_capture_save("blend.rgb", rgb565_data, len);
	
#if (USE_DMA2D_BLEND_ISR_CALLBACKS == 1)
		if (rtos_get_semaphore(&s_lcd.dma2d_complete_sem, BEKEN_NEVER_TIMEOUT) != BK_OK)
		{
			LOGE("%s, dma2d_complete_sem get failed: %d\n", __func__);
		}
#else
		while (bk_dma2d_is_transfer_busy()) {}
#endif
		if (PIXEL_FMT_VUYY == format)
		{
			rgb565_to_vuyy_convert((uint16_t *)rgb565_data, (uint16_t *)yuv_data, lcd_blend->xsize, lcd_blend->ysize);
		}
		else
		{
			rgb565_to_yuyv_convert((uint16_t *)rgb565_data, (uint16_t *)yuv_data, lcd_blend->xsize, lcd_blend->ysize);
		}
	//	lcd_storage_capture_save("blend_to_yuv.yuv", yuv_data, len);
#if 0
			dma2d_memcpy_psram(yuv_data, lcd_blend->pbg_addr, lcd_blend->xsize, lcd_blend->ysize, 0, lcd_blend->bg_offline);
#else
			p_yuv_src = (uint32_t *)yuv_data;
			p_yuv_dst = lcd_blend->pbg_addr;
			for(i = 0; i < lcd_blend->ysize; i++)
			{
				os_memcpy_word(p_yuv_dst, p_yuv_src, lcd_blend->xsize*2);
				p_yuv_src += (lcd_blend->xsize/2);
				p_yuv_dst += (lcd_blend->bg_width/2);
			}
#endif
	//	lcd_storage_capture_save("pbg_addr.yuv", (uint8_t *)0x60000000, (640*480*2));
#else
	int	i = 0;
	uint8_t * p_fg_yuv = (uint8_t *)lcd_blend->pfg_addr;
	uint8_t * p_yuv_src = (uint8_t *)lcd_blend->pbg_addr;
	uint8_t * p_yuv_dst = (uint8_t *)yuv_data;
	uint8_t * p_logo_addr = rgb565_data;
	//STEP 1 COPY BG YUV DATA

	if (lcd_blend->flag == 1)
	{
		os_memcpy_word((uint32_t *)(p_logo_addr), (uint32_t *)p_fg_yuv, lcd_blend->xsize * lcd_blend->ysize *4);
	}
	for(i = 0; i < lcd_blend->ysize; i++)
	{
		os_memcpy_word((uint32_t *)p_yuv_dst,(uint32_t *) p_yuv_src, lcd_blend->xsize*2);
		p_yuv_dst += (lcd_blend->xsize*2);
		p_yuv_src += (lcd_blend->bg_width*2);
	}
	p_yuv_dst = (uint8_t *)yuv_data;
	//STEP 2 check alpha=0 logo pixel,and copy alpha!=0 pixel to bg yuv data
	if (lcd_blend->bg_data_format == PIXEL_FMT_VUYY)
	{
		argb8888_to_vuyy_blend(p_logo_addr, p_yuv_dst, lcd_blend->xsize, lcd_blend->ysize);
	}
	else
	{
		argb8888_to_yuyv_blend(p_logo_addr, p_yuv_dst, lcd_blend->xsize, lcd_blend->ysize);
	}
	//STEP 3 copy return bg image
	p_yuv_src = (uint8_t *)lcd_blend->pbg_addr;
	for(i = 0; i < lcd_blend->ysize; i++)
	{
		os_memcpy_word((uint32_t *)p_yuv_src, (uint32_t *)p_yuv_dst, lcd_blend->xsize*2);
		p_yuv_dst += (lcd_blend->xsize*2);
		p_yuv_src += (lcd_blend->bg_width*2);
	}
#endif
	}
	else
	{
		LOGE("%s s_lcd.dma2d_blend == false \n", __func__);
	}
#endif
	return BK_OK;
}

#if CONFIG_LCD_FONT_BLEND
bk_err_t lcd_driver_font_blend(lcd_font_config_t *lcd_font)
{
	int ret = BK_OK;
	if (s_lcd.dma2d_blend == true)
	{
		#if 0
		dma2d_memcpy_psram(lcd_font->pbg_addr, yuv_data, lcd_font->xsize, lcd_font->ysize, lcd_font->bg_offline, 0);
		#else
		register uint32_t i =0;
		uint32_t * p_yuv_src = (uint32_t *)lcd_font->pbg_addr;
		uint32_t * p_yuv_dst = (uint32_t *)yuv_data;
		for(i = 0; i < lcd_font->ysize; i++)
		{
			os_memcpy_word(p_yuv_dst, p_yuv_src, lcd_font->xsize*2);
			p_yuv_dst += (lcd_font->xsize/2);
			p_yuv_src += (lcd_font->bg_width/2);
		}
		#endif
		//	lcd_storage_capture_save("yuv_data.yuv", yuv_data, len);
		if(lcd_font->font_format == FONT_RGB565)  //font rgb565 data to yuv bg image
		{
			if (PIXEL_FMT_VUYY == lcd_font->bg_data_format)
			{
				vuyy_to_rgb565_convert((unsigned char *)yuv_data, (unsigned char *)rgb565_data, lcd_font->xsize, lcd_font->ysize);
			}
			else
			{
				yuyv_to_rgb565_convert((unsigned char *)yuv_data, (unsigned char *)rgb565_data, lcd_font->xsize, lcd_font->ysize);
			}
			//	lcd_storage_capture_save("yuv_to_rgb565.rgb", rgb565_data, len);

			font_t font;
			font.info = (ui_display_info_struct){rgb565_data,0,lcd_font->ysize,0,{0}};
			font.width = lcd_font->xsize;
			font.height = lcd_font->ysize;
			font.font_fmt = lcd_font->font_format;
			for(int i = 0; i < lcd_font->str_num; i++)
			{
				font.digit_info = lcd_font->str[i].font_digit_type;
				font.s = lcd_font->str[i].str;
				font.font_color = lcd_font->str[i].font_color;
				font.x_pos = lcd_font->str[i].x_pos;
				font.y_pos = lcd_font->str[i].y_pos;
				lcd_draw_font(&font);
			}
			if (PIXEL_FMT_VUYY == lcd_font->bg_data_format)
			{
				rgb565_to_vuyy_convert((uint16_t *)rgb565_data, (uint16_t *)yuv_data, lcd_font->xsize, lcd_font->ysize);
			}
			else
			{
				rgb565_to_yuyv_convert((uint16_t *)rgb565_data, (uint16_t *)yuv_data, lcd_font->xsize, lcd_font->ysize);
			}
		}
		else  //font yuv data to yuv bg image
		{
			font_t font;
			font.info = (ui_display_info_struct){yuv_data,0,lcd_font->ysize,0,{0}};
			font.width = lcd_font->xsize;
			font.height = lcd_font->ysize;
			font.font_fmt = lcd_font->font_format;
			for(int i = 0; i < lcd_font->str_num; i++)
			{
				font.digit_info = lcd_font->str[i].font_digit_type;
				font.s = lcd_font->str[i].str;
				font.font_color = lcd_font->str[i].font_color;
				font.x_pos = lcd_font->str[i].x_pos;
				font.y_pos = lcd_font->str[i].y_pos;
				lcd_draw_font(&font);
			}
		}
			//	lcd_storage_capture_save("blend_to_yuv.yuv", yuv_data, len);
		#if 0
			dma2d_memcpy_psram(yuv_data, lcd_font->pbg_addr, lcd_font->xsize, lcd_font->ysize, 0, lcd_font->bg_offline);
		#else
				p_yuv_src = (uint32_t *)yuv_data;
				p_yuv_dst = lcd_font->pbg_addr;
				for(i = 0; i < lcd_font->ysize; i++)
				{
					os_memcpy_word(p_yuv_dst, p_yuv_src, lcd_font->xsize*2);
					p_yuv_src += (lcd_font->xsize/2);
					p_yuv_dst += (lcd_font->bg_width/2);
				}
		#endif
	}
	else
	{
		LOGE("%s s_lcd.dma2d_blend == false \n", __func__);
	}
	return ret;
}
#endif

bk_err_t lcd_driver_display_frame_wait(uint32_t timeout_ms)
{
	bk_err_t ret = rtos_get_semaphore(&s_lcd.disp_sem, timeout_ms);
	
	if (ret != BK_OK)
	{
		LOGE("%s semaphore get failed: %d\n", __func__, ret);
	}

	return ret;
}

bk_err_t lcd_driver_display_frame_sync(frame_buffer_t *frame, bool wait)
{
	bk_err_t ret = BK_FAIL;
	uint64_t before, after;

	LOGD("lcd_driver_display_frame_sync\n");

#if timer_control_pfs
	bk_timer_stop(TIMER_ID4);
#endif

	if (s_lcd.enable == false)
	{
		LCD_DRIVER_FRAME_FREE(frame);
		return ret;
	}

	LCD_DISPLAY_START();
#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif
	rtos_lock_mutex(&s_lcd.disp_lock);

	if (s_lcd.display_en == false)
	{
		LCD_DRIVER_FRAME_FREE(frame);
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
		LOGI("display start\n");
	}
	else
	{
		if (s_lcd.pingpong_frame != NULL)
		{
			LCD_DRIVER_FRAME_FREE(s_lcd.pingpong_frame);
		}

		s_lcd.pingpong_frame = frame;
#if timer_control_pfs
		lcd_driver_set_display_base_addr((uint32_t)frame->frame);
		lcd_driver_display_enable();
#endif

		LOGD("display start continue\n");

		if (s_lcd.config.device->type == LCD_TYPE_MCU8080)
		{
			lcd_driver_ppi_set(frame->width, frame->height);
			bk_lcd_set_yuv_mode(frame->fmt);
			lcd_driver_set_display_base_addr((uint32_t)frame->frame);
			bk_lcd_8080_start_transfer(1);
			lcd_driver_display_continue();
		}
	}

	GLOBAL_INT_RESTORE();

	if(wait == BK_TRUE)
	{
		ret = lcd_driver_display_frame_wait(BEKEN_NEVER_TIMEOUT);
	}

	rtos_unlock_mutex(&s_lcd.disp_lock);
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	LOGD("display time: %lu\n", (after - before) / 26000);

	LCD_DISPLAY_END();

	return ret;
}


bk_err_t lcd_driver_display_frame(frame_buffer_t *frame)
{
	return lcd_driver_display_frame_sync(frame, BK_TRUE);
}


#if (CONFIG_LCD_QSPI && CONFIG_LVGL)
#else
static u8 g_gui_need_to_wait = BK_FALSE;
#endif

void lcd_driver_display_frame_with_gui(void *buffer, int width, int height)
{
#if (CONFIG_LCD_QSPI && CONFIG_LVGL)
	bk_lcd_qspi_send_data(s_device_config, (uint32_t *)buffer, width * height * 4);
#else

    if(g_gui_need_to_wait)
	{
		lcd_driver_display_frame_wait(2000);

		g_gui_need_to_wait = BK_FALSE;
	}

    if(s_lcd.lvgl_frame){
        s_lcd.lvgl_frame->fmt = PIXEL_FMT_BGR565;
        s_lcd.lvgl_frame->frame = buffer;
        s_lcd.lvgl_frame->width = width;
        s_lcd.lvgl_frame->height = height;

        if (s_lcd.config.device->type == LCD_TYPE_RGB565)
        {
            lcd_driver_display_frame_sync(s_lcd.lvgl_frame, BK_FALSE);
            g_gui_need_to_wait = BK_TRUE;
        }
        else
        {
            lcd_driver_display_frame_sync(s_lcd.lvgl_frame, BK_TRUE);
            g_gui_need_to_wait = BK_FALSE;
        }
    }
    else{
        LOGI("[%s][%d] fb malloc fail\r\n", __FUNCTION__, __LINE__);
    }
#endif
}


bk_err_t lcd_ldo_power_enable(uint8_t enable)
{
#if (CONFIG_LCD_POWER_GPIO_CTRL)

	if (enable)
	{

#if (LCD_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_HIGH);
#else
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_LOW);
#endif
	}
	else
	{

#if (LCD_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_LOW);
#else
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_HIGH);
#endif
	}

#endif

	return BK_OK;
}

bk_err_t lcd_backlight_control_enable(uint8_t enable)
{
#if (CONFIG_LCD_BACKLIGHT_GPIO_CTRL)

	if (enable)
	{
		gpio_dev_unmap(LCD_BACKLIGHT_CTRL_GPIO);
		bk_gpio_set_capacity(LCD_BACKLIGHT_CTRL_GPIO, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_BACKLIGHT_CTRL_GPIO));
		BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_BACKLIGHT_CTRL_GPIO));

#if (LCD_BACKLIGHT_CTRL_ACTIVE_LEVEL)
		bk_gpio_set_output_high(LCD_BACKLIGHT_CTRL_GPIO);
#else
		bk_gpio_set_output_low(LCD_BACKLIGHT_CTRL_GPIO);
#endif
		rtos_delay_milliseconds(5);
	}
	else
	{

#if (LCD_BACKLIGHT_CTRL_ACTIVE_LEVEL)
		bk_gpio_set_output_low(LCD_BACKLIGHT_CTRL_GPIO);
#else
		bk_gpio_set_output_high(LCD_BACKLIGHT_CTRL_GPIO);
#endif
	}

#endif

	return BK_OK;
}


bk_err_t lcd_driver_init(const lcd_config_t *config)
{
	int ret = BK_FAIL;
	lcd_clk_t clk = 0;
	const lcd_device_t *device = config->device;

	LCD_DIAG_DEBUG_INIT();

	lcd_ldo_power_enable(1);
#if !CONFIG_SOC_BK7236
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);
#endif
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

	s_lcd.lvgl_frame = os_malloc(sizeof(frame_buffer_t));
	os_memset(s_lcd.lvgl_frame, 0, sizeof(frame_buffer_t));

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

	if (get_decode_mode() == HARDWARE_DECODING)
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

	LOGD("%s, LCD clk set: %d\n", __func__, clk);

	ret = bk_lcd_driver_init(clk);

	if (ret != BK_OK)
	{
		LOGE("%s, device clk set error\n", __func__);
		goto error;
	}

	if (device->type == LCD_TYPE_RGB565)
	{
		LOGD("%s, rgb eof register\n", __func__);
		s_lcd.lcd_rgb_frame_end_handler = lcd_driver_display_rgb_isr;
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
		//bk_flash_register_wait_cb(NULL);   //flash_busy_lcd_callback
	}
	else if (device->type == LCD_TYPE_MCU8080)
	{
		LOGD("%s, mcu eof register\n", __func__);
		s_lcd.lcd_8080_frame_end_handler = lcd_driver_display_mcu_isr;
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
#if CONFIG_ARCH_RISCV
	before = riscv_get_mtimer();
#else
	before = 0;
#endif

	if (device->init)
	{
		device->init();
	}
#if CONFIG_ARCH_RISCV
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	LOGI("lcd init time: %lu\n", (after - before) / 26000);

	if (device->backlight_open)
	{
		device->backlight_open();
	}

#if CONFIG_LCD_DMA2D_BLEND_FLASH_IMG || CONFIG_LCD_FONT_BLEND
	bk_dma2d_driver_init();
	yuv_data = config->yuv_addr;
	rgb565_data = config->rgb565_addr;
	s_lcd.dma2d_blend = true;
	/*if (ret != BK_OK)
	{
		bk_dma2d_driver_deinit();
		s_lcd.dma2d_blend = false;
	}*/
#if (USE_DMA2D_BLEND_ISR_CALLBACKS == 1)
	ret = rtos_init_semaphore_ex(&s_lcd.dma2d_complete_sem, 1, 0);
	if (ret != BK_OK)
	{
		LOGE("%s dma2d_sem init failed: %d\n", __func__, ret);
		return ret;
	}
	ret = rtos_init_semaphore_ex(&s_lcd.dma2d_err_sem, 1, 0);
	if (ret != BK_OK)
	{
		LOGE("%s dma2d_err_sem init failed: %d\n", __func__, ret);
		return ret;
	}
	bk_dma2d_int_enable(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE,1);
	bk_dma2d_register_int_callback_isr(DMA2D_CFG_ERROR_ISR, dma2d_config_error);
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, dma2d_transfer_error);
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);
#endif 

#endif
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
	if (get_decode_mode() == HARDWARE_DECODING)
	{
		bk_jpeg_dec_driver_deinit();
	}
	else
	{
#if (CONFIG_JPEG_DECODE)
		bk_jpeg_dec_sw_deinit();
#endif
	}
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
#if CONFIG_LCD_DMA2D_BLEND_FLASH_IMG  || CONFIG_LCD_FONT_BLEND
	if (s_lcd.dma2d_blend == true)
	{
		bk_dma2d_driver_deinit();
		yuv_data = NULL;
		rgb565_data = NULL;


#if USE_DMA2D_BLEND_ISR_CALLBACKS
		ret = rtos_deinit_semaphore(&s_lcd.dma2d_complete_sem);
		if (ret != BK_OK)
		{
			LOGE("%s dma2d_complete_sem deinit failed: %d\n", __func__, ret);
			return ret;
		}
		ret = rtos_deinit_semaphore(&s_lcd.dma2d_err_sem);
		if (ret != BK_OK)
		{
			LOGE("%s dma2d_err_sem deinit failed: %d\n", __func__, ret);
			return ret;
		}
#endif
	}
#endif

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
		if (s_lcd.display_frame != s_lcd.lvgl_frame)
		{
			s_lcd.config.fb_free(s_lcd.display_frame);
		}
		s_lcd.display_frame = NULL;
	}

	if (s_lcd.lvgl_frame)
	{
		os_free(s_lcd.lvgl_frame);
		s_lcd.lvgl_frame = NULL;
	}

#if (CONFIG_LCD_QSPI && CONFIG_LVGL)
#else
	g_gui_need_to_wait = BK_FALSE;
#endif


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

	lcd_ldo_power_enable(0);

	return ret;
}

static void dma2d_lcd_fill(uint32_t frameaddr, uint16_t width, uint16_t height, uint32_t color)
{
	//uint32_t color_temp = (color << 16) | color;
	//  os_printf("displat color :%x\r\n", color_temp);
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M;                      /**< Mode Register to Memory */
	dma2d_config.init.color_mode       = DMA2D_OUTPUT_ARGB8888;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;     /**< No alpha inversion for the output image */
	bk_dma2d_init(&dma2d_config);

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
	bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)frameaddr, width, height);
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
	#if CONFIG_LCD_ST7796S
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
		#endif
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
#if CONFIG_LCD_ST7796S
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
#endif
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
#if CONFIG_LCD_ST7796S
		case LCD_DEVICE_ST7796S:
			if (x >= 320 || y >= 480)
			{
				x = 320 - 1;
				y = 480 - 1;
			}
			lcd_st7796s_set_display_mem_area(x, x + 1, y, y + 1);
			break;
#endif
		default:
			break;
	}
	lcd_hal_8080_cmd_send(2, 0x2c, param_clumn);
	return BK_OK;
}

int32_t lcd_driver_get_spi_gpio(LCD_SPI_GPIO_TYPE_E gpio_type)
{
    int32_t gpio_value = 0;
    switch(gpio_type)
    {
        case SPI_GPIO_CLK:
            if(s_lcd.config.device->id == LCD_DEVICE_ST7701S)
                gpio_value = GPIO_35;
            else if ((s_lcd.config.device->id == LCD_DEVIDE_ST7710S) || (s_lcd.config.device->id == LCD_DEVICE_ST7701S_LY))
                gpio_value = GPIO_9;
            else
                gpio_value = GPIO_2;
            break;

        case SPI_GPIO_CSX:
            if(s_lcd.config.device->id == LCD_DEVICE_ST7701S)
                gpio_value = GPIO_34;
            else if ((s_lcd.config.device->id == LCD_DEVIDE_ST7710S) || (s_lcd.config.device->id == LCD_DEVICE_ST7701S_LY))
                gpio_value = GPIO_5;
            else
                gpio_value = GPIO_3;
            break;

        case SPI_GPIO_SDA:
            if(s_lcd.config.device->id == LCD_DEVICE_ST7701S)
                gpio_value = GPIO_36;
            else if ((s_lcd.config.device->id == LCD_DEVIDE_ST7710S) || (s_lcd.config.device->id == LCD_DEVICE_ST7701S_LY))
                gpio_value = GPIO_8;
            else
                gpio_value = GPIO_4;
            break;

        case SPI_GPIO_RST:
            if(s_lcd.config.device->id == LCD_DEVICE_ST7701S)
                gpio_value = GPIO_15;
            else if ((s_lcd.config.device->id == LCD_DEVIDE_ST7710S) || (s_lcd.config.device->id == LCD_DEVICE_ST7701S_LY))
                gpio_value = GPIO_6;
            else
                gpio_value = GPIO_6;
            break;

        default:
            LOGE("%s can't support this gpio type:%d\r\n", __FUNCTION__, __LINE__);
            break;
    }

    return gpio_value;
}


