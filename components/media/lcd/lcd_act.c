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

#include <driver/timer.h>
//#include <driver/media_types.h>
#include <driver/psram.h>

#include "modules/lvgl/lcd_task.h"

#include "driver/flash.h"
#include "driver/flash_partition.h"
#include "beken_image.h"
#include <components/jpeg_decode.h>

#define TAG "lcd"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

extern media_debug_t *media_debug;

lcd_info_t lcd_info = {0};

beken_semaphore_t step_sem;

/**< camera jpeg display */
beken_semaphore_t camera_display_sem;
bool camera_display_task_running = false;
static beken_thread_t camera_display_task = NULL;

/**< lcd jpeg display */
#define DISPLAY_PIPELINE_TASK
#ifdef DISPLAY_PIPELINE_TASK
beken_semaphore_t jpeg_display_sem;
bool jpeg_display_task_running = false;
static beken_thread_t jpeg_display_task = NULL;
#endif

static frame_buffer_t *dbg_jpeg_frame = NULL;
static frame_buffer_t *dbg_display_frame = NULL;
lcd_open_t lcd_transfer_info = {0};

char *jpeg_name = NULL;
static int g_lcd_with_gui = 0;
static int g_lcd_display_logo_first = 0;

#ifdef DISPLAY_PIPELINE_TASK
static void jpeg_display_task_entry(beken_thread_arg_t data)
{
	frame_buffer_t *frame = NULL;
	bool rotate = (bool)data;

	LOGI("%s, rotate: %d\n", __func__, rotate);

	rtos_set_semaphore(&jpeg_display_sem);

	while (jpeg_display_task_running)
	{
		frame = frame_buffer_fb_display_pop_wait();

		if (frame == NULL)
		{
			LOGE("read display frame NULL\n");
			continue;
		}

		if (rotate == true)
		{
			frame = lcd_driver_rotate_frame(frame);

			if (frame == NULL)
			{
				LOGE("rotate frame NULL\n");
				continue;
			}
		}

		lcd_driver_display_frame(frame);
	}

	LOGI("jpeg display task exit\n");

	jpeg_display_task = NULL;
	rtos_set_semaphore(&jpeg_display_sem);
	rtos_delete_thread(NULL);
}


void jpeg_display_task_start(bool rotate)
{
	bk_err_t ret;

	if (jpeg_display_task != NULL)
	{
		LOGE("%s jpeg_display_thread already running\n", __func__);
		return;
	}

	frame_buffer_fb_register(MODULE_LCD, FB_INDEX_DISPLAY);

	ret = rtos_init_semaphore_ex(&jpeg_display_sem, 1, 0);

	if (BK_OK != ret)
	{
		LOGE("%s semaphore init failed\n", __func__);
		return;
	}

	jpeg_display_task_running = true;

	ret = rtos_create_thread(&jpeg_display_task,
	                         4,
	                         "jpeg_display_thread",
	                         (beken_thread_function_t)jpeg_display_task_entry,
	                         4 * 1024,
	                         (beken_thread_arg_t)rotate);

	if (BK_OK != ret)
	{
		LOGE("%s jpeg_display_thread init failed\n");
		return;
	}

	ret = rtos_get_semaphore(&jpeg_display_sem, BEKEN_NEVER_TIMEOUT);

	if (BK_OK != ret)
	{
		LOGE("%s decoder_sem get failed\n", __func__);
	}
}

void jpeg_display_task_stop(void)
{
	bk_err_t ret;

	if (jpeg_display_task_running == false)
	{
		LOGI("%s already stop\n", __func__);
		return;
	}

	jpeg_display_task_running = false;

	frame_buffer_fb_deregister(MODULE_LCD);

	ret = rtos_get_semaphore(&jpeg_display_sem, BEKEN_NEVER_TIMEOUT);

	if (BK_OK != ret)
	{
		LOGE("%s jpeg_display_sem get failed\n");
	}

	LOGI("%s complete\n", __func__);

	ret = rtos_deinit_semaphore(&jpeg_display_sem);

	if (BK_OK != ret)
	{
		LOGE("%s jpeg_display_sem deinit failed\n");
	}
}

#endif

static void camera_display_task_entry(beken_thread_arg_t data)
{
	frame_buffer_t *jpeg_frame = NULL;
	frame_buffer_t *dec_frame = NULL;
	bool rotate = (bool)data;

	rtos_set_semaphore(&camera_display_sem);

	while (camera_display_task_running)
	{
		if (lcd_info.step_mode == false)
		{
			/* Normal display workflow */

			jpeg_frame = frame_buffer_fb_read(MODULE_DECODER);

			if (jpeg_frame == NULL)
			{
				LOGE("%s read jpeg frame NULL\n", __func__);
				continue;
			}

			dec_frame = lcd_driver_decoder_frame(jpeg_frame);

			frame_buffer_fb_free(jpeg_frame, MODULE_DECODER);

			if (dec_frame == NULL)
			{
				LOGD("jpeg decoder frame NULL\n");
				continue;
			}

#ifdef  DISPLAY_PIPELINE_TASK
			frame_buffer_fb_display_push(dec_frame);
#else

			if (rotate == true)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame);

				if (dec_frame == NULL)
				{
					LOGE("rotate frame NULL\n");
					continue;
				}
			}

			lcd_driver_display_frame(dec_frame);
#endif
		}
		else
		{
			/* Debug display workflow */
			if (rtos_get_semaphore(&step_sem, BEKEN_NEVER_TIMEOUT))
			{
				LOGE("%s step_sem get failed\n", __func__);
			}

			if (jpeg_frame)
			{
				LOGI("free frame: %u\n", jpeg_frame->sequence);
				frame_buffer_fb_free(jpeg_frame, MODULE_DECODER);
				jpeg_frame = NULL;
			}

			jpeg_frame = frame_buffer_fb_read(MODULE_DECODER);

			if (jpeg_frame == NULL)
			{
				LOGE("read jpeg frame NULL\n");
				continue;
			}

			dbg_jpeg_frame = jpeg_frame;

			LOGI("Got jpeg frame seq: %u, ptr: %p, length: %u, fmt: %u\n",
			     dbg_jpeg_frame->sequence, dbg_jpeg_frame->frame, dbg_jpeg_frame->length, dbg_jpeg_frame->fmt);

			dec_frame = lcd_driver_decoder_frame(jpeg_frame);

			if (dec_frame == NULL)
			{
				LOGD("jpeg decoder frame NULL\n");
				continue;
			}

			if (rotate == true)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame);

				if (dec_frame == NULL)
				{
					LOGE("rotate frame NULL\n");
					continue;
				}
			}


			dbg_display_frame = dec_frame;

			LOGI("Got display frame seq: %u, ptr: %p, length: %u, fmt: %u\n",
			     dbg_display_frame->sequence, dbg_display_frame->frame, dbg_display_frame->length, dbg_display_frame->fmt);

			lcd_driver_display_frame(dec_frame);

		}
	}
	LOGI("camera display task exit\n");

	camera_display_task = NULL;
	rtos_set_semaphore(&camera_display_sem);

	rtos_delete_thread(NULL);
}

void camera_display_task_start(bool rotate)
{
	bk_err_t ret;

	if (camera_display_task != NULL)
	{
		LOGE("%s camera_display_thread already running\n", __func__);
		return;
	}

	frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);

	ret = rtos_init_semaphore_ex(&camera_display_sem, 1, 0);

	if (BK_OK != ret)
	{
		LOGE("%s semaphore init failed\n", __func__);
		return;
	}

	camera_display_task_running = true;

	ret = rtos_create_thread(&camera_display_task,
	                         4,
	                         "camera_display_thread",
	                         (beken_thread_function_t)camera_display_task_entry,
	                         4 * 1024,
	                         (beken_thread_arg_t)rotate);

	if (BK_OK != ret)
	{
		LOGE("%s camera_display_task init failed\n");
		return;
	}

	ret = rtos_get_semaphore(&camera_display_sem, BEKEN_NEVER_TIMEOUT);

	if (BK_OK != ret)
	{
		LOGE("%s camera_display_sem get failed\n", __func__);
	}
}

void camera_display_task_stop(void)
{
	bk_err_t ret;
	if (camera_display_task_running == false)
	{
		LOGI("%s already stop\n", __func__);
		return;
	}
	camera_display_task_running = false;

	frame_buffer_fb_deregister(MODULE_DECODER);
	ret = rtos_get_semaphore(&camera_display_sem, BEKEN_NEVER_TIMEOUT);

	if (BK_OK != ret)
	{
		LOGE("%s camera_display_sem get failed\n");
	}

	ret = rtos_deinit_semaphore(&camera_display_sem);

	if (BK_OK != ret)
	{
		LOGE("%s decoder_sem deinit failed\n");
	}
	
	LOGI("%s complete\n", __func__);
}


static bk_err_t display_logo(void)
{
	bk_err_t ret = BK_OK;
	unsigned char *jpeg_psram = (unsigned char *)JPEG_SRAM_ADDRESS;
	frame_buffer_t *src_frame = NULL;
	uint32_t img_len = sizeof(beken_800_480_jpg);
	sw_jpeg_dec_res_t result;
	
	src_frame = frame_buffer_fb_display_malloc();
	if (src_frame == NULL)
	{
		frame_buffer_fb_display_free(src_frame);
		LOGE("src_frame malloc failed\r\n");
		return BK_FAIL;
	}
	os_memcpy(jpeg_psram, (uint8_t *)beken_800_480_jpg, img_len);
	ret = bk_jpeg_dec_sw_init();
	if (ret != kNoErr) {
		LOGE("init sw jpeg decoder failed\r\n");
		return BK_FAIL;
	}

	ret = bk_jpeg_dec_sw_start(img_len, jpeg_psram, src_frame->frame, &result);
	bk_jpeg_dec_sw_deinit();

	if (ret != kNoErr) {
		LOGE("sw jpeg decoder failed\r\n");
		frame_buffer_fb_display_free(src_frame);
		return BK_FAIL;
	}

	LOGI("sw jpeg decode (pixel_x pixel_y) : (%x, %x) \r\n", result.pixel_x, result.pixel_y);
	src_frame->width = result.pixel_x;
	src_frame->height = result.pixel_y;
	src_frame->fmt = PIXEL_FMT_YUYV;

	frame_buffer_fb_display_push(src_frame);

	return BK_OK;
}

void lcd_open_handle(param_pak_t *param)
{
	int ret = BK_OK;

	LOGI("%s \n", __func__);
	if (LCD_STATE_DISPLAY == get_lcd_state())
	{
		camera_display_task_start(lcd_info.rotate); 
		set_lcd_state(LCD_STATE_ENABLED);
	}

	if (LCD_STATE_ENABLED == get_lcd_state())
	{
		LOGW("%s already open\n", __func__);
		goto out;
	}

	lcd_open_t *lcd_open = (lcd_open_t *)param->param;
	lcd_config_t lcd_config;

	os_memcpy(&lcd_transfer_info, lcd_open, sizeof(lcd_open_t));

	lcd_config.device = get_lcd_device_by_name(lcd_transfer_info.device_name);

	if (lcd_config.device == NULL)
	{
		lcd_config.device = get_lcd_device_by_ppi(lcd_transfer_info.device_ppi);
	}

	if (lcd_config.device == NULL)
	{
		LOGE("%s lcd device not found\n", __func__);
		goto out;
	}
	LOGI("%s, ppi: %dX%d %s\n", __func__, lcd_config.device->ppi >> 16, lcd_config.device->ppi & 0xFFFF, lcd_config.device->name);

	lcd_config.fb_display_init  = frame_buffer_fb_display_init;
	lcd_config.fb_display_deinit  = frame_buffer_fb_display_deinit;
	lcd_config.fb_free  = frame_buffer_fb_display_free;
	lcd_config.fb_malloc = frame_buffer_fb_display_malloc_wait;
	lcd_config.use_gui = g_lcd_with_gui;

	lcd_driver_init(&lcd_config);

	if (!g_lcd_with_gui) {
		camera_display_task_start(lcd_info.rotate);
#ifdef DISPLAY_PIPELINE_TASK
		jpeg_display_task_start(lcd_info.rotate);
#endif
		if(g_lcd_display_logo_first == 1)
		{
			ret= display_logo();
			if(ret != BK_OK)
			{
				LOGE("%s yikang_test display logo frist err \n", __func__);
				goto out;
			}
		}
		
	}
	set_lcd_state(LCD_STATE_ENABLED);

	LOGI("%s complete\n", __func__);

	#if (CONFIG_LVGL)
	if(g_lcd_with_gui){
		uint16_t hor_size = ppi_to_pixel_x(lcd_config.device->ppi);  //lcd size x
		uint16_t ver_size = ppi_to_pixel_y(lcd_config.device->ppi);  //lcd size y

		bk_err_t bk_psram_init(void);
		bk_psram_init();
		bk_gui_disp_task_init(hor_size, ver_size);
	}
	#endif
	
out:
		MEDIA_EVT_RETURN(param, ret);

}

char * display_name = NULL;
extern storage_flash_t storge_flash;

void lcd_display_handle(param_pak_t *param)
{
	int ret = BK_OK;

	char *ptr = (char *)param->param;
    bk_psram_init();

	if(display_name != NULL)
	{
		os_free(display_name);
		display_name = NULL;
	}
	display_name = ptr;

	camera_display_task_stop();
	set_lcd_state(LCD_STATE_DISPLAY);
	rtos_delay_milliseconds(10);
	LOGI("display_name: %s , length %d \n", display_name,  storge_flash.flasg_img_length);

	frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);
	frame_buffer_t *jpeg_frame = frame_buffer_fb_jpeg_malloc();
	if (jpeg_frame == NULL)
	{
		frame_buffer_fb_jpeg_free(jpeg_frame);	   //frame_buffer_fb_free
		LOGE("src_frame malloc failed\r\n");
		goto out;
	}
#if CONFIG_SDCARD_HOST
		ret = sdcard_read_to_mem((char *)display_name, (uint32_t *)jpeg_frame->frame, &jpeg_frame->length );
#else   //display flash data
		jpeg_frame->length =  storge_flash.flasg_img_length;

//		extern void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);
//		uint8_t read_ptr[10] = {0};
//		bk_flash_read_bytes(storge_flash.flash_image_addr, (uint8_t*)read_ptr, 10);
//		bk_mem_dump_ex("befor flash data", read_ptr, 10);
		ret = bk_flash_read_word(storge_flash.flash_image_addr, (uint32_t *)jpeg_frame->frame, storge_flash.flasg_img_length);
//		bk_mem_dump_ex("befor psram data",(uint8_t *)jpeg_frame->frame, 10);
//		bk_mem_dump_ex("befor psram data",(uint8_t *) (jpeg_frame->frame + storge_flash.flasg_img_length - 3), 4);
#endif
		if (BK_OK != ret)
		{
			frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
			camera_display_task_start(lcd_info.rotate);
			set_lcd_state(LCD_STATE_ENABLED);
			LOGE("%s, sd/flash read file failed\n", __func__);
			goto out;
		}

		jpeg_frame->fmt = PIXEL_FMT_UVC_JPEG;
		frame_buffer_t *dec_frame = lcd_driver_decoder_frame(jpeg_frame);

		if (dec_frame == NULL)
		{
			LOGD("display handler jpeg decoder frame error\n");
			goto out;
		}

		frame_buffer_fb_jpeg_free(jpeg_frame);	   //frame_buffer_fb_free
		frame_buffer_fb_display_push(dec_frame);

out:

	MEDIA_EVT_RETURN(param, ret);
}

void lcd_display_beken_logo_handle(param_pak_t *param)
{
	bk_err_t ret = BK_OK;
	unsigned char *jpeg_psram = (unsigned char *)JPEG_SRAM_ADDRESS;
	frame_buffer_t *src_frame = NULL;
	uint32_t img_len = sizeof(beken_800_480_jpg);
	sw_jpeg_dec_res_t result;

	camera_display_task_stop();  //USB Camera
	
	rtos_delay_milliseconds(50);
	set_lcd_state(LCD_STATE_DISPLAY);

	src_frame = frame_buffer_fb_display_malloc();
	if (src_frame == NULL)
	{
		frame_buffer_fb_display_free(src_frame);
		LOGE("src_frame malloc failed\r\n");
		goto out;
	}
	os_memcpy(jpeg_psram, (uint8_t *)beken_800_480_jpg, img_len);
	ret = bk_jpeg_dec_sw_init();
	if (ret != kNoErr) {
		LOGE("init sw jpeg decoder failed\r\n");
		goto out;
	}

	ret = bk_jpeg_dec_sw_start(img_len, jpeg_psram, src_frame->frame, &result);
	bk_jpeg_dec_sw_deinit();

	if (ret != kNoErr) {
		LOGE("sw jpeg decoder failed\r\n");
		goto out;
	}

	LOGI("sw jpeg decode (pixel_x pixel_y) : (%x, %x) \r\n", result.pixel_x, result.pixel_y);
	src_frame->width = result.pixel_x;
	src_frame->height = result.pixel_y;
	src_frame->fmt = PIXEL_FMT_YUYV;

	frame_buffer_fb_display_push(src_frame);

out:

	MEDIA_EVT_RETURN(param, ret);

}

void lcd_close_handle(param_pak_t *param)
{
	int ret = BK_OK;

	LOGI("%s\n", __func__);
   
    if (step_sem != NULL)
    {
        if (rtos_set_semaphore(&step_sem))
        {
            LOGE("%s step_sem set failed\n", __func__);
        }

        if (rtos_deinit_semaphore(&step_sem))
        {
            LOGE("%s step_sem deinit failed\n");
        }
        step_sem = NULL;
    }

	if (LCD_STATE_DISABLED == get_lcd_state())
	{
		LOGW("%s already close\n", __func__);
		goto out;
	}

	if(!g_lcd_with_gui){
		camera_display_task_stop();
		jpeg_display_task_stop();
	}
#if (CONFIG_LVGL)
	else
		bk_gui_disp_task_deinit();
#endif

	lcd_driver_deinit();

	lcd_info.rotate = false;

	set_lcd_state(LCD_STATE_DISABLED);

	LOGI("%s complete\n", __func__);

out:

	MEDIA_EVT_RETURN(param, ret);
}

void lcd_set_backligth_handle(param_pak_t *param)
{
	int ret = BK_OK;

	LOGI("%s, levle: %d\n", __func__, param->param);
	lcd_driver_set_backlight(param->param);

	MEDIA_EVT_RETURN(param, ret);
}


static char *frame_suffix(pixel_format_t fmt)
{
	switch (fmt)
	{
		case PIXEL_FMT_UNKNOW:
			return ".unknow";
		case PIXEL_FMT_DVP_JPEG:
			return "_dvp.jpg";
		case PIXEL_FMT_UVC_JPEG:
			return "_uvc.jpg";
		case PIXEL_FMT_RGB565:
			return ".rgb565";
		case PIXEL_FMT_YUYV:
			return ".yuyv";
		case PIXEL_FMT_UYVY:
			return ".uyvy";
		case PIXEL_FMT_YYUV:
			return ".yyuv";
		case PIXEL_FMT_UVYY:
			return ".uvyy";
		case PIXEL_FMT_VUYY:
			return ".vuyy";
		case PIXEL_FMT_YVYU:
			return ".yvyu";
		case PIXEL_FMT_VYUY:
			return ".vyuy";
		case PIXEL_FMT_YYVU:
			return ".yyvu";
	}

	return ".unknow";
}

void lcd_act_dump_decoder_frame(void)
{
	//storage_frame_buffer_dump(lcd_info.decoder_frame, "decoder_vuyy.yuv");
}

void lcd_act_dump_jpeg_frame(void)
{
	if (dbg_jpeg_frame == NULL)
	{
		LOGE("dbg_jpeg_frame was NULL\n");
		return;
	}

	LOGI("Dump jpeg frame seq: %u, ptr: %p, length: %u, fmt: %u\n",
	     dbg_jpeg_frame->sequence, dbg_jpeg_frame->frame, dbg_jpeg_frame->length, dbg_jpeg_frame->fmt);

	storage_frame_buffer_dump(dbg_jpeg_frame, frame_suffix(dbg_jpeg_frame->fmt));
}

void lcd_act_dump_display_frame(void)
{
	if (dbg_display_frame == NULL)
	{
		LOGE("dbg_display_frame was NULL\n");
		return;
	}

	LOGI("Dump display frame seq: %u, ptr: %p, length: %u, fmt: %u\n",
	     dbg_display_frame->sequence, dbg_display_frame->frame, dbg_display_frame->length, dbg_display_frame->fmt);

	storage_frame_buffer_dump(dbg_display_frame, frame_suffix(dbg_display_frame->fmt));
}



void lcd_event_handle(uint32_t event, uint32_t param)
{
	param_pak_t *param_pak = NULL;

	switch (event)
	{
		case EVENT_LCD_OPEN_IND:
			g_lcd_with_gui = 0;
			g_lcd_display_logo_first = 1;
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
			break;

		case EVENT_LCD_FRAME_LOCAL_ROTATE_IND:
#if 0
			lcd_act_rotate_degree90(param);
			lcd_act_rotate_complete(lcd_info.rotate_frame);
#endif
			break;

		case EVENT_LCD_CLOSE_IND:
			lcd_close_handle((param_pak_t *)param);
			g_lcd_with_gui = 0;
			break;

		case EVENT_LCD_OPEN_WITH_GUI:
			g_lcd_with_gui = 1;
			lcd_open_handle((param_pak_t *)param);
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
				rtos_init_semaphore_ex(&step_sem, 1, 0);
			}
			else
			{
				LOGI("step mode disable");
				lcd_info.step_mode = false;

				if (rtos_set_semaphore(&step_sem))
				{
					LOGE("%s step_sem set failed\n", __func__);
				}

				if (rtos_deinit_semaphore(&step_sem))
				{
					LOGE("%s step_sem deinit failed\n");
				}

			}
			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;

		case EVENT_LCD_STEP_TRIGGER_IND:
			param_pak = (param_pak_t *)param;

			LOGI("step trigger start");
			lcd_info.step_trigger = true;

			if (rtos_set_semaphore(&step_sem))
			{
				LOGE("%s step_sem set failed\n", __func__);
			}

			MEDIA_EVT_RETURN(param_pak, BK_OK);
			break;

		case EVENT_LCD_DISPLAY_IND:
			lcd_display_handle((param_pak_t *)param);
			break;

		case EVENT_LCD_BEKEN_LOGO_DISPLAY:
			lcd_display_beken_logo_handle((param_pak_t *)param);
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
