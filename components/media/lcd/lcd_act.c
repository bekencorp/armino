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

#include <os/os.h>
#include <components/log.h>

#include "media_core.h"
#include "media_evt.h"
#include "storage_act.h"
#include "lcd_act.h"
#include "frame_buffer.h"

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>

#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>

#include <soc/mapping.h>

#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/jpeg_dec.h>
#include <driver/dma2d.h>
//#include <lcd_dma2d_config.h>
#include <driver/jpeg_dec_types.h>
#include "modules/image_scale.h"
//#include "lcd_blend_config.h"
#include <driver/uvc_camera_types.h>
#include <driver/uvc_camera.h>

#include <modules/pm.h>
#include <driver/timer.h>

#define TAG "lcd"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

uint32_t media_decoder_isr_count = 0;
uint32_t media_lcd_isr_count = 0;


lcd_info_t lcd_info = {0};

void lcd_frame_pingpong_insert(frame_buffer_t *buffer)
{
	LOGD("YUV frame: %u\n", buffer->sequence);

	if (lcd_info.ping_frame == NULL
	    && lcd_info.pong_frame == NULL)
	{
		lcd_info.ping_frame = buffer;
		lcd_info.display_frame = buffer;
		lcd_driver_set_display_base_addr((uint32_t)lcd_info.ping_frame->frame);
		lcd_driver_display_enable(true);
		LOGD("display start\n");
	}
	else
	{
		if (lcd_info.ping_frame != lcd_info.display_frame)
		{
			if (lcd_info.ping_frame != NULL)
			{
				frame_buffer_free_request(lcd_info.ping_frame, MODULE_DISPLAY);
			}
			lcd_info.ping_frame = buffer;
		}

		if (lcd_info.pong_frame != lcd_info.display_frame)
		{
			if (lcd_info.pong_frame != NULL)
			{
				frame_buffer_free_request(lcd_info.pong_frame, MODULE_DISPLAY);
			}
			lcd_info.pong_frame = buffer;
		}

	}
}


static void lcd_act_complete_callback(void)
{
	media_lcd_isr_count++;

	if (lcd_info.ping_frame == lcd_info.display_frame
	    && lcd_info.pong_frame != NULL)
	{
		frame_buffer_free_request(lcd_info.display_frame, MODULE_DISPLAY);
		lcd_info.ping_frame = NULL;
		lcd_info.display_frame = lcd_info.pong_frame;
		lcd_driver_set_display_base_addr((uint32_t)lcd_info.pong_frame->frame);
	}

	if (lcd_info.pong_frame == lcd_info.display_frame
	    && lcd_info.ping_frame != NULL)
	{
		frame_buffer_free_request(lcd_info.display_frame, MODULE_DISPLAY);
		lcd_info.display_frame = lcd_info.ping_frame;
		lcd_info.pong_frame = NULL;
		lcd_driver_set_display_base_addr((uint32_t)lcd_info.ping_frame->frame);
	}

	//LOGI("rgb %p\n", lcd_info.display_frame->frame);

	lcd_driver_display_enable(true);
}

static void jpeg_dec_eof_cb(void)
{
	media_decoder_isr_count++;

	//bk_gpio_pull_up(GPIO_8);

	LOGD("jpgeof %p\n", lcd_info.decoder_frame->frame);

	if (lcd_info.step_mode == false)
	{
		bk_timer_stop(TIMER_ID3);
	}
	else
	{
		if (lcd_info.step_trigger == true)
		{
			lcd_info.step_trigger = false;
		}
		lcd_frame_pingpong_insert(lcd_info.decoder_frame);
		return;
	}

	if (lcd_info.rotate == false)
	{
		lcd_frame_pingpong_insert(lcd_info.decoder_frame);
		lcd_info.decoder_frame = NULL;
		if (lcd_info.jpeg_frame && true == frame_buffer_get_state())
		{
			media_msg_t msg;

			LOGD("free decoder frame: %u\n", lcd_info.jpeg_frame->sequence);
			msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
			msg.param = (uint32_t)lcd_info.jpeg_frame;
			media_send_msg(&msg);
			lcd_info.jpeg_frame = NULL;
		}
		else
		{
			LOGD("fjpgeof frame: %p, %d\n", lcd_info.jpeg_frame, frame_buffer_get_state());
		}
	}
	else
	{
		lcd_info.rotate_frame = frame_buffer_alloc(FRAME_DISPLAY);
		lcd_info.rotate_frame->sequence = lcd_info.decoder_frame->sequence;

#if 1

		media_msg_t msg;
		msg.event = EVENT_LCD_ROTATE_RIGHT_CMD;
		msg.param = (uint32_t)&lcd_info;
		media_send_msg(&msg);
#else
		media_msg_t msg;
		msg.event = EVENT_LCD_FRAME_LOCAL_ROTATE_IND;
		msg.param = (uint32_t)&lcd_info;
		media_send_msg(&msg);
#endif
	}

	//bk_gpio_pull_down(GPIO_8);
}

static void lcd_act_decoder_timeout(timer_id_t timer_id)
{
	LOGI("decoder timeout\n");

	frame_buffer_free_request(lcd_info.decoder_frame, MODULE_DISPLAY);
	lcd_info.decoder_frame = NULL;


	if (lcd_info.jpeg_frame && true == frame_buffer_get_state())
	{
		media_msg_t msg;

		LOGI("free decoder frame: %u\n", lcd_info.jpeg_frame->sequence);
		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)lcd_info.jpeg_frame;
		media_send_msg(&msg);
		lcd_info.jpeg_frame = NULL;
	}
}

void lcd_act_rotate_complete(frame_buffer_t *frame)
{
	frame_buffer_free_request(lcd_info.decoder_frame, MODULE_DISPLAY);

	if (frame != lcd_info.rotate_frame)
	{
		LOGE("frame not match\n");
	}

	lcd_frame_pingpong_insert(lcd_info.rotate_frame);

	if (lcd_info.jpeg_frame && true == frame_buffer_get_state())
	{
		media_msg_t msg;

		LOGD("free decoder frame: %u\n", lcd_info.jpeg_frame->sequence);
		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)lcd_info.jpeg_frame;
		media_send_msg(&msg);
		lcd_info.jpeg_frame = NULL;
	}
}


int lcd_set_partical(uint16_t src_x, uint16_t src_y, uint16_t dst_x, uint16_t dst_y)
{
	uint16_t start_x, end_x, start_y, end_y;

	start_x = (src_x - dst_x) / 2 + 1;
	end_x = start_x + dst_x - 1;

	start_y = (src_y - dst_y) / 2 + 1;
	end_y = start_y + dst_y - 1;

	LOGI("%s, offset %d, %d, %d, %d\n", __func__, start_x, end_x, start_y, end_y);

	bk_lcd_set_partical_display(start_x, end_x, start_y, end_y);
	return 0;
}

int lcd_act_driver_init(uint32_t lcd_ppi)
{
	int ret = BK_FAIL;
	dvp_camera_device_t *dvp_device = NULL;
	bool yuv_mode = false;

	LOGI("%s, ppi: %dX%d\n", __func__, lcd_ppi >> 16, lcd_ppi & 0xFFFF);

	if (lcd_info.state == LCD_STATE_ENABLED)
	{
		LOGE("alread enable\n", __func__);
		return ret;
	}

	dvp_device = bk_dvp_camera_get_device();

	if (dvp_device == NULL || dvp_device->id == ID_UNKNOW)
	{
		LOGE("dvp camera was not init\n");
#if (CONFIG_USB_UVC)
		uvc_camera_device_t *uvc_device = NULL;

		uvc_device = bk_uvc_camera_get_device();

		if (uvc_device == NULL)
		{
			LOGE("uvc camera was not init\n");
			return BK_FAIL;
		}

		lcd_info.src_pixel_x = uvc_device->width;
		lcd_info.src_pixel_y = uvc_device->height;
#else
		return BK_FAIL;
#endif
	}
	else
	{
		lcd_info.src_pixel_x = ppi_to_pixel_x(dvp_device->ppi);
		lcd_info.src_pixel_y = ppi_to_pixel_y(dvp_device->ppi);

		if (dvp_device->mode == DVP_MODE_YUV)
		{
			yuv_mode = true;
		}
	}

	ret = bk_jpeg_dec_driver_init();

	media_decoder_isr_count = 0;
	media_lcd_isr_count = 0;

	if (ret != BK_OK)
	{
		LOGE("bk_jpeg_dec_driver_init failed\n");
		return ret;
	}

	lcd_info.lcd_pixel_x = ppi_to_pixel_x(lcd_ppi);
	lcd_info.lcd_pixel_y = ppi_to_pixel_y(lcd_ppi);

	lcd_info.ping_frame = NULL;
	lcd_info.pong_frame = NULL;
	lcd_info.jpeg_frame = NULL;
	lcd_info.decoder_frame = NULL;
	lcd_info.step_mode = false;
	lcd_info.step_trigger = false;

	LOGI("%s Camera PPI: %dX%d\n", __func__, lcd_info.src_pixel_x, lcd_info.src_pixel_y);

	lcd_config_t lcd_config;

	if (lcd_ppi == PPI_1024X600)
	{
		lcd_config.device = get_lcd_device_by_id(LCD_DEVICE_HX8282);
	}
	else if (lcd_ppi == PPI_320X480)
	{
		lcd_config.device = get_lcd_device_by_id(LCD_DEVICE_ST7796S);
	}
	else if (lcd_ppi == PPI_480X800)
	{
		lcd_config.device = get_lcd_device_by_id(LCD_DEVICE_GC9503V);
	}
	else
	{
		lcd_config.device = get_lcd_device_by_id(LCD_DEVICE_ST7282);
	}

	lcd_config.complete_callback = lcd_act_complete_callback;

	if (lcd_info.rotate == false)
	{
		lcd_config.pixel_x = lcd_info.src_pixel_x;
		lcd_config.pixel_y = lcd_info.src_pixel_y;
	}
	else
	{
		lcd_config.pixel_x = ppi_to_pixel_x(lcd_config.device->ppi);
		lcd_config.pixel_y = ppi_to_pixel_y(lcd_config.device->ppi);
	}

	lcd_info.pixel_size =  get_ppi_size(lcd_config.device->ppi);

	if (yuv_mode)
	{
		lcd_config.fmt = ORGINAL_YUYV_DATA;
	}
	else
	{
		if (lcd_info.rotate)
		{
			lcd_config.fmt = ORGINAL_YUYV_DATA;
		}
		else
		{
			lcd_config.fmt = VUYY_DATA;
		}
		bk_jpeg_dec_isr_register(DEC_END_OF_FRAME, jpeg_dec_eof_cb);
	}

	lcd_driver_init(&lcd_config);

#if CONFIG_PWM
	lcd_driver_set_backlight(100);
#endif

	lcd_info.state = LCD_STATE_ENABLED;
	LOGI("%s successful\n", __func__);
	return ret;
}

uint8_t lcd_frame_complete_callback(frame_buffer_t *buffer)
{
	bool lock = false;

	if (buffer->type == FRAME_JPEG)
	{
		media_msg_t msg;

		msg.event = EVENT_LCD_FRAME_COMPLETE_IND;
		msg.param = (uint32_t)buffer;

		media_send_msg(&msg);
	}
	else if (buffer->type == FRAME_DISPLAY)
	{
		lcd_frame_pingpong_insert(buffer);
	}

	return lock;
}

void lcd_open_handle(param_pak_t *param)
{
	int ret = BK_OK;
	//media_msg_t msg;

	LOGI("%s\n", __func__);

	if (LCD_STATE_ENABLED == get_lcd_state())
	{
		LOGW("%s already open\n", __func__);
		goto out;
	}

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);
	frame_buffer_display_frame_init();

	ret = lcd_act_driver_init(param->param);

	frame_buffer_frame_register(MODULE_DECODER, lcd_frame_complete_callback);
	frame_buffer_frame_register(MODULE_DISPLAY, lcd_frame_complete_callback);

	set_lcd_state(LCD_STATE_ENABLED);

out:

	MEDIA_EVT_RETURN(param, ret);
}

void lcd_close_handle(param_pak_t *param)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);

	if (LCD_STATE_DISABLED == get_lcd_state())
	{
		LOGW("%s already close\n", __func__);
		goto out;
	}

	bk_timer_stop(TIMER_ID3);

	bk_jpeg_dec_driver_deinit();
	lcd_driver_deinit();

	lcd_info.rotate = false;

	if (lcd_info.jpeg_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)lcd_info.jpeg_frame;
		media_send_msg(&msg);
		lcd_info.jpeg_frame = NULL;
	}

	if (lcd_info.decoder_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)lcd_info.decoder_frame;
		media_send_msg(&msg);
		lcd_info.decoder_frame = NULL;
	}

	if (lcd_info.ping_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)lcd_info.ping_frame;
		media_send_msg(&msg);
		lcd_info.ping_frame = NULL;
	}


	if (lcd_info.pong_frame)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)lcd_info.pong_frame;
		media_send_msg(&msg);
		lcd_info.pong_frame = NULL;
	}

	lcd_info.decoder_frame = NULL;

	frame_buffer_frame_deregister(MODULE_DECODER);
	frame_buffer_frame_deregister(MODULE_DISPLAY);

	set_lcd_state(LCD_STATE_DISABLED);

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_DEFAULT);

out:

	MEDIA_EVT_RETURN(param, ret);
}

void lcd_set_backligth_handle(param_pak_t *param)
{
	int ret = BK_OK;

	LOGI("%s, levle: %d\n", __func__, param->param);

#if CONFIG_PWM
	lcd_driver_set_backlight(param->param);
#endif

	MEDIA_EVT_RETURN(param, ret);
}


void lcd_act_dump_decoder_frame(void)
{
	storage_frame_buffer_dump(lcd_info.decoder_frame, "decoder_vuyy.yuv");
}

void lcd_act_dump_jpeg_frame(void)
{
	storage_frame_buffer_dump(lcd_info.jpeg_frame, "jpeg.jpg");
}

void lcd_act_dump_display_frame(void)
{
	storage_frame_buffer_dump(lcd_info.display_frame, "display_vuyy.yuv");
}


bk_err_t lcd_frame_decoder(frame_buffer_t *buffer)
{
	int ret = BK_FAIL;
	frame_buffer_t *frame = NULL;

	if (buffer->frame[0] != 0xFF || buffer->frame[1] != 0xD8)
	{
		ret = BK_FAIL;
		LOGE("%s frame header error\n");
		goto error;
	}

	if (lcd_info.jpeg_frame == NULL)
	{
		lcd_info.jpeg_frame = buffer;
		frame = frame_buffer_alloc(FRAME_DISPLAY);

		if (frame == NULL)
		{
			LOGE("%s malloc decoder frame NULL\n");
			//TODO;
			goto error;
		}

		frame->length = lcd_info.src_pixel_x * lcd_info.src_pixel_y * 2;
		frame->sequence = lcd_info.jpeg_frame->sequence;

		//bk_gpio_pull_up(GPIO_5);

		LOGD("decoder frame: %u, %p, %p\n", lcd_info.jpeg_frame->sequence, lcd_info.jpeg_frame->frame, frame->frame);
		LOGD("frame: %u, %u, %u\n", lcd_info.jpeg_frame->sequence, lcd_info.jpeg_frame->length, lcd_info.jpeg_frame->length % 4);
		bk_jpeg_dec_hw_init(lcd_info.src_pixel_x, lcd_info.src_pixel_y, lcd_info.jpeg_frame->frame, frame->frame);
		ret = bk_jpeg_dec_hw_start();

		if (lcd_info.step_mode == false)
		{
			bk_timer_start(TIMER_ID3, 120, lcd_act_decoder_timeout);
		}

		//bk_gpio_pull_down(GPIO_5);

		LOGD("bk_jpeg_dec_hw_start, :%d\n", ret);
	}
	else
	{
		LOGI("decoder frame not NULL\n");
	}

	if (ret != BK_OK)
	{
		LOGE("%s frame decoder error\n", __func__);
		goto error;
	}

	lcd_info.decoder_frame = frame;

	return ret;

error:

	lcd_info.jpeg_frame = NULL;

	if (frame != NULL)
	{
		frame_buffer_free_request(frame, MODULE_DISPLAY);
	}
	return ret;
}



void lcd_frame_complete_handle(frame_buffer_t *buffer)
{
	int ret = BK_FAIL;

	if (lcd_info.debug == true)
	{
		bk_gpio_set_output_high(GPIO_2);
	}

	if (buffer->type == FRAME_JPEG)
	{
		ret = lcd_frame_decoder(buffer);
	}
	else if (buffer->type == FRAME_DISPLAY)
	{
		//TODO
		LOGE("%s frame error\n");
	}


	if (ret != BK_OK)
	{
		media_msg_t msg;

		msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
		msg.param = (uint32_t)buffer;
		media_send_msg(&msg);
	}


	if (lcd_info.debug == true)
	{
		bk_gpio_set_output_low(GPIO_2);
	}
}

void lcd_event_handle(uint32_t event, uint32_t param)
{
	param_pak_t *param_pak = NULL;

	switch (event)
	{
		case EVENT_LCD_OPEN_IND:
			lcd_open_handle((param_pak_t *)param);
			break;

		case EVENT_LCD_ROTATE_ENABLE_IND:
		{
			LOGI("EVENT_LCD_ROTATE_ENABLE_IND\n");

			param_pak = (param_pak_t *)param;
			lcd_info.rotate = param_pak->param;
			MEDIA_EVT_RETURN(param_pak, BK_OK);
		}
		break;

		case EVENT_LCD_FRAME_COMPLETE_IND:
			lcd_frame_complete_handle((frame_buffer_t *)param);
			break;

		case EVENT_LCD_FRAME_LOCAL_ROTATE_IND:
#if 0
			lcd_act_rotate_degree90(param);
			lcd_act_rotate_complete(lcd_info.rotate_frame);
#endif
			break;

		case EVENT_LCD_CLOSE_IND:
			lcd_close_handle((param_pak_t *)param);
			break;
		case EVENT_LCD_SET_BACKLIGHT_IND:
			lcd_set_backligth_handle((param_pak_t *)param);
			break;

		case EVENT_LCD_DUMP_DECODER_IND:
			lcd_act_dump_decoder_frame();
			param_pak = (param_pak_t *)param;
			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;

		case EVENT_LCD_DUMP_JPEG_IND:
			lcd_act_dump_jpeg_frame();
			param_pak = (param_pak_t *)param;
			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;

		case EVENT_LCD_DUMP_DISPLAY_IND:
			lcd_act_dump_display_frame();
			param_pak = (param_pak_t *)param;
			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;

		case EVENT_LCD_STEP_MODE_IND:
			param_pak = (param_pak_t *)param;

			if (param_pak->param)
			{
				LOGI("step mode enable");
				lcd_info.step_mode = true;
				bk_timer_stop(TIMER_ID3);
			}
			else
			{
				LOGI("step mode disable");
				lcd_info.step_mode = false;
			}
			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;

		case EVENT_LCD_STEP_TRIGGER_IND:
			param_pak = (param_pak_t *)param;

			LOGI("step trigger start");
			lcd_info.step_trigger = true;

			frame_buffer_free_request(lcd_info.decoder_frame, MODULE_DISPLAY);
			lcd_info.decoder_frame = NULL;


			if (lcd_info.jpeg_frame && true == frame_buffer_get_state())
			{
				media_msg_t msg;

				LOGI("free decoder frame: %u\n", lcd_info.jpeg_frame->sequence);
				msg.event = EVENT_COM_FRAME_DECODER_FREE_IND;
				msg.param = (uint32_t)lcd_info.jpeg_frame;
				media_send_msg(&msg);
				lcd_info.jpeg_frame = NULL;
			}

			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;


	}
}


lcd_state_t get_lcd_state(void)
{
	return lcd_info.state;
}

void set_lcd_state(lcd_state_t state)
{
	lcd_info.state = state;
}

void lcd_init(void)
{
	os_memset(&lcd_info, 0, sizeof(lcd_info_t));

	lcd_info.state = LCD_STATE_DISABLED;
	lcd_info.debug = false;
	lcd_info.rotate = false;
}


