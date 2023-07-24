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
#include <string.h>
#include <stdlib.h>

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
#include <driver/jpeg_dec_types.h>
#include "modules/image_scale.h"
#include <driver/uvc_camera_types.h>
#include <driver/uvc_camera.h>

#include <driver/timer.h>
#include <driver/psram.h>
#include "lv_vendor.h"

#include "driver/flash.h"
#include "driver/flash_partition.h"
#include "beken_image.h"
#include <components/jpeg_decode.h>
#include <os/str.h>
#include <blend_logo.h>
#include "modules/image_scale.h"
#include <driver/dma2d.h>
#include "modules/lcd_font.h"

#ifdef CONFIG_LVGL
#include "lvgl.h"
#endif
#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif


#define TAG "lcd_act"

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

///char *g_blend_name = NULL;
#if CONFIG_LCD_DMA2D_BLEND || CONFIG_LCD_FONT_BLEND
static lcd_blend_data_t g_blend_data = {0};
static uint8_t *yuv_blend_addr = NULL;
static uint8_t *rgb_blend_addr = NULL;
uint8_t g_wifi_current_level = WIFI_LEVEL_MAX;

#endif


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
		case PIXEL_FMT_UVC_H264:
			return "_uvc.h264";
		case PIXEL_FMT_RGB565_LE:
			return ".rgb565le";
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
		default:
			break;
	}

	return ".unknow";
}

#ifdef DISPLAY_PIPELINE_TASK
static void jpeg_display_task_entry(beken_thread_arg_t data)
{
	frame_buffer_t *frame = NULL;
	media_rotate_t rotate = (media_rotate_t)data;

	LOGI("%s, rotate: %d\n", __func__, rotate);

	rtos_set_semaphore(&jpeg_display_sem);

	while (jpeg_display_task_running)
	{
		rotate = lcd_info.rotate;
		
		frame = frame_buffer_fb_display_pop_wait();

		if (frame == NULL)
		{
			LOGD("read display frame NULL\n");
			continue;
		}

		if (lcd_info.resize)
		{
			LOGD("%d\n", lcd_info.resize_ppi);
			frame = lcd_driver_resize_frame(frame, lcd_info.resize_ppi);

			if (frame == NULL)
			{
				LOGD("resize frame NULL\n");
				continue;
			}
		}
		if (lcd_transfer_info.yuv2rgb == YUV2RGB565)
		{
			frame = lcd_driver_rotate_frame(frame, YUV2RGB565);
			if (frame == NULL)
			{
				LOGD("rotate frame NULL\n");
				continue;
			}
		}
		if (rotate != ROTATE_NONE)
		{
			frame = lcd_driver_rotate_frame(frame, lcd_info.rotate);
			if (frame == NULL)
			{
				LOGD("rotate frame NULL\n");
				continue;
			}
		}

#if CONFIG_LCD_FONT_BLEND
		lcd_font_handle(frame);
#endif
#if CONFIG_LCD_DMA2D_BLEND
		lcd_dma2d_handle(frame);
#endif
		lcd_driver_display_frame(frame);
	}

	LOGI("jpeg display task exit\n");

	jpeg_display_task = NULL;
	rtos_set_semaphore(&jpeg_display_sem);
	rtos_delete_thread(NULL);
}


void jpeg_display_task_start(media_rotate_t rotate)
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
	                         6,
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
	media_rotate_t rotate = (media_rotate_t)data;

	rtos_set_semaphore(&camera_display_sem);

	while (camera_display_task_running)
	{
		if (lcd_info.step_mode == false)
		{
			/* Normal display workflow */
			jpeg_frame = frame_buffer_fb_read(MODULE_DECODER);

			if (jpeg_frame == NULL)
			{
				LOGD("%s read jpeg frame NULL\n", __func__);
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
			rotate = lcd_info.rotate;
			if (lcd_info.resize)
			{
				dec_frame = lcd_driver_resize_frame(dec_frame, lcd_info.resize_ppi);

				if (dec_frame == NULL)
				{
					LOGD("resize frame NULL\n");
					continue;
				}
			}

			if (lcd_transfer_info.yuv2rgb == YUV2RGB565)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame, YUV2RGB565);
				if (dec_frame == NULL)
				{
					LOGD("rotate frame NULL\n");
					continue;
				}
			}
			if (rotate != ROTATE_NONE)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame, lcd_info.rotate);
				if (dec_frame == NULL)
				{
					LOGD("rotate frame NULL\n");
					continue;
				}
			}
			
#if CONFIG_LCD_FONT_BLEND
			lcd_font_handle(dec_frame);
#endif
#if CONFIG_LCD_DMA2D_BLEND
			lcd_dma2d_handle(dec_frame);
#endif

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
				LOGD("read jpeg frame NULL\n");
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

			if (lcd_info.resize == true)
			{
				dec_frame = lcd_driver_resize_frame(dec_frame, lcd_info.resize_ppi);

				if (dec_frame == NULL)
				{
					LOGD("resize frame NULL\n");
					continue;
				}
			}

			if (rotate == true)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame, lcd_info.rotate);

				if (dec_frame == NULL)
				{
					LOGD("rotate frame NULL\n");
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

void camera_display_task_start(media_rotate_t rotate)
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
	                         6,
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
	frame_buffer_t *jpeg_frame = NULL;
	frame_buffer_t *dec_frame = NULL;
	sw_jpeg_dec_res_t result;
	uint32_t * img_addr = NULL;
	uint32_t img_len = 0;

#if CONFIG_LCD_FLASH_JPEG_DISPLAY
		/// use define addr and len
		img_addr= (uint32_t *)(beken_800_480_jpg);
		img_len = sizeof(beken_800_480_jpg);
#else
		LOGE("flash img display macro is not enable \n");
		return BK_FAIL;
#endif

	ret = frame_buffer_fb_jpeg_init(PPI_800X600);
	frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);
	jpeg_frame = frame_buffer_fb_jpeg_malloc();

	if (jpeg_frame == NULL)
	{
		LOGE("jpeg frame malloc failed\r\n");
		return BK_FAIL;
	}

	os_memcpy_word((uint32_t *)jpeg_frame->frame, img_addr, img_len);
	ret = bk_jpeg_dec_sw_init();
	if (ret != kNoErr) {
		LOGE("init sw jpeg decoder failed\r\n");
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		return BK_FAIL;
	}

	dec_frame = frame_buffer_fb_display_malloc();
	if (dec_frame == NULL)
	{
		LOGE("src_frame malloc failed\r\n");
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		return BK_FAIL;
	}

	ret = bk_jpeg_dec_sw_start(img_len, jpeg_frame->frame, dec_frame->frame, &result);
	bk_jpeg_dec_sw_deinit();

	if (ret != kNoErr) {
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		frame_buffer_fb_display_free(dec_frame);
		LOGE("sw jpeg decoder failed\r\n");
		return BK_FAIL;
	}

	LOGI("sw jpeg decode (pixel_x pixel_y) : (%x, %x) \r\n", result.pixel_x, result.pixel_y);
	dec_frame->width = result.pixel_x;
	dec_frame->height = result.pixel_y;
	dec_frame->fmt = PIXEL_FMT_YUYV;

	frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free

	frame_buffer_fb_display_push(dec_frame);

	return 	BK_OK;
}

bk_err_t lcd_open_handle(media_mailbox_msg_t *msg)
{
    int ret = BK_OK;

    lcd_open_t *lcd_open = (lcd_open_t *)msg->param;
	lcd_info.rotate = lcd_open->rotate;
	lcd_transfer_info.yuv2rgb = lcd_open->yuv2rgb;

    if (LCD_STATE_DISPLAY == get_lcd_state())
    {
        camera_display_task_start(lcd_info.rotate); 
        set_lcd_state(LCD_STATE_ENABLED);
        LOGI("%s: open camera_display_task \n", __func__);
    }

    if (LCD_STATE_ENABLED == get_lcd_state())
    {
        LOGW("%s already open\n", __func__);
        goto out;
    }

    lcd_config_t lcd_config = {0};

#if CONFIG_LCD_DMA2D_BLEND  || CONFIG_LCD_FONT_BLEND

#if CONFIG_CAMERA
    if (bk_dvp_camera_get_device() == NULL)
    {
        yuv_blend_addr = (uint8_t *)LCD_BLEND_JPEGSRAM_ADDR_1;
        rgb_blend_addr = (uint8_t *)LCD_BLEND_JPEGSRAM_ADDR_2;
    }
    else
    {
        if (yuv_blend_addr == NULL)
        {
            yuv_blend_addr = (uint8_t *)os_malloc(LCD_BLEND_MALLOC_SIZE);
        }

        if (rgb_blend_addr == NULL)
        {
            rgb_blend_addr = (uint8_t *)os_malloc(LCD_BLEND_MALLOC_SIZE);
        }
    }
#else
    yuv_blend_addr = (uint8_t *)LCD_BLEND_JPEGSRAM_ADDR_1;
    rgb_blend_addr = (uint8_t *)LCD_BLEND_JPEGSRAM_ADDR_2;
#endif

    if (yuv_blend_addr == NULL || rgb_blend_addr == NULL)
    {
        LOGE("lcd blen malloc error\r\n");
        if (yuv_blend_addr)
        {
            os_free(yuv_blend_addr);
            yuv_blend_addr = NULL;
        }

        if (rgb_blend_addr)
        {
            os_free(rgb_blend_addr);
            rgb_blend_addr = NULL;
        }

        goto out;
    }

    lcd_config.yuv_addr = yuv_blend_addr;
    lcd_config.rgb565_addr = rgb_blend_addr;
#endif

    os_memcpy(&lcd_transfer_info, lcd_open, sizeof(lcd_open_t));

    if (lcd_transfer_info.device_name != NULL)
        lcd_config.device = get_lcd_device_by_name(lcd_transfer_info.device_name);

    if (lcd_config.device == NULL)
    {
        lcd_config.device = get_lcd_device_by_ppi(lcd_transfer_info.device_ppi);
    }
    else
    {
        lcd_transfer_info.device_ppi = lcd_config.device->ppi;
    }

    if (lcd_config.device == NULL)
    {
        LOGE("%s lcd device not found\n", __func__);
        goto out;
    }
    else
    {
        lcd_transfer_info.device_name = lcd_config.device->name;
    }

    LOGI("%s, lcd ppi: %dX%d %s\n", __func__, lcd_config.device->ppi >> 16, lcd_config.device->ppi & 0xFFFF, lcd_config.device->name);

    lcd_config.fb_display_init  = frame_buffer_fb_display_init;
    lcd_config.fb_display_deinit  = frame_buffer_fb_display_deinit;
    lcd_config.fb_free  = frame_buffer_fb_display_free;
    lcd_config.fb_malloc = frame_buffer_fb_display_malloc_wait;
	if (bk_dvp_camera_get_device() != NULL)
	{
		dvp_camera_device_t *dvp_device = bk_dvp_camera_get_device();
		frame_buffer_fb_display_init(dvp_device->ppi);
		LOGI("%s, dvp camera ppi: %dX%d\n", __func__, dvp_device->ppi >> 16, dvp_device->ppi & 0xFFFF);
	}
	else if(bk_uvc_camera_get_device() != NULL)
	{
		uvc_camera_device_t *uvc_device = bk_uvc_camera_get_device();
		frame_buffer_fb_display_init(uvc_device->width << 16 | uvc_device->height);
		LOGI("%s, uvc camera ppi: %dX%d\n", __func__, uvc_device->width, uvc_device->height);
	}
	else
		frame_buffer_fb_display_init(lcd_config.device->ppi);

    lcd_driver_init(&lcd_config);

    camera_display_task_start(lcd_transfer_info.rotate);
#ifdef DISPLAY_PIPELINE_TASK
    jpeg_display_task_start(lcd_transfer_info.rotate);
#endif
    set_lcd_state(LCD_STATE_ENABLED);

#if CONFIG_LCD_FLASH_JPEG_DISPLAY
	lcd_set_logo_on(1);

    if (lcd_info.start_logo == 1)
    {
        ret= display_logo();
		lcd_set_logo_on(0);
        if(ret != BK_OK)
        {
            LOGE("%s display logo frist err \n", __func__);
        }
    }
#endif
    LOGI("%s complete\n", __func__);
    lcd_info.lcd_width = ppi_to_pixel_x(lcd_config.device->ppi);
    lcd_info.lcd_height = ppi_to_pixel_y(lcd_config.device->ppi);

out:
	return ret;
}


#if (CONFIG_IMAGE_STORAGE && !CONFIG_SLAVE_CORE)
extern storage_flash_t storge_flash;
#endif

void lcd_display_handle(param_pak_t *param)
{
	int ret = BK_OK;
	lcd_display_t *lcd_display = (lcd_display_t *)param->param;
	LOGI("display img addr: %p , length %d \n", lcd_display->image_addr, lcd_display->img_length);
	if(lcd_display==NULL||lcd_display->image_addr==0||lcd_display->img_length<=0)
	{
		goto out;
	}
	
	camera_display_task_stop();
	set_lcd_state(LCD_STATE_DISPLAY);
//	rtos_delay_milliseconds(10);

	if (lcd_transfer_info.device_ppi < PPI_1024X600)
		ret = frame_buffer_fb_jpeg_init(PPI_800X600);
	else
		ret = frame_buffer_fb_jpeg_init(PPI_1280X720);
	frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);
	frame_buffer_t *jpeg_frame = frame_buffer_fb_jpeg_malloc();
	if (jpeg_frame == NULL)
	{
		LOGE("src_frame malloc failed\r\n");
		goto out;
	}
	
	jpeg_frame->length = lcd_display->img_length;
//		extern void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);
//		uint8_t read_ptr[10] = {0};
//		bk_flash_read_bytes(storge_flash.flash_image_addr, (uint8_t*)read_ptr, 10);
//		bk_mem_dump_ex("befor flash data", read_ptr, 10);
	ret = bk_flash_read_word(lcd_display->image_addr, (uint32_t *)jpeg_frame->frame, jpeg_frame->length);
//		bk_mem_dump_ex("befor psram data",(uint8_t *)jpeg_frame->frame, 10);
//		bk_mem_dump_ex("befor psram data",(uint8_t *) (jpeg_frame->frame + storge_flash.flasg_img_length - 3), 4);
	if (BK_OK != ret)
	{
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		LOGE("%s, sd/flash read file failed\n", __func__);
		goto out;
	}

	jpeg_frame->fmt = PIXEL_FMT_UVC_JPEG;
	frame_buffer_t *dec_frame = lcd_driver_decoder_frame(jpeg_frame);

	if (dec_frame == NULL)
	{
		frame_buffer_fb_jpeg_free(jpeg_frame);	   //frame_buffer_fb_free
		LOGE("display handler jpeg decoder frame error\n");
		goto out;
	}

	frame_buffer_fb_jpeg_free(jpeg_frame);	   //frame_buffer_fb_free
	frame_buffer_fb_display_push(dec_frame);
out:

	MEDIA_EVT_RETURN(param, ret);
}


bk_err_t lcd_display_file_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
	
	camera_display_task_stop();
	set_lcd_state(LCD_STATE_DISPLAY);

	if (lcd_info.picture_echo == true)
	{
		LOGE("%s display echo is on going\r\n", __func__);
		msg->result = BK_FAIL;
		return BK_FAIL;
	}

	if (lcd_transfer_info.device_ppi < PPI_1024X600)
		ret = frame_buffer_fb_jpeg_init(PPI_800X600);
	else
		ret = frame_buffer_fb_jpeg_init(PPI_1280X720);

	frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);
	frame_buffer_t *frame = frame_buffer_fb_jpeg_malloc();
	
	if (frame == NULL)
	{
		LOGE("%s, read jpeg frame NULL\n", __func__);
		
		msg->result = BK_FAIL;
		return BK_FAIL;
	}
	
	lcd_info.picture_echo = true;
	media_mailbox_msg_t *picture_echo_node = os_malloc(sizeof(media_mailbox_msg_t));
	
	if (picture_echo_node != NULL)
	{
		ret = rtos_init_semaphore_ex(&picture_echo_node->sem, 1, 0);
	}
	picture_echo_node->event = EVENT_LCD_PICTURE_ECHO_NOTIFY;
	picture_echo_node->param = (uint32_t)frame;
	picture_echo_node->result = ret;
	msg_send_to_media_major_mailbox(picture_echo_node, ret, APP_MODULE);

	ret = rtos_get_semaphore(&picture_echo_node->sem, BEKEN_WAIT_FOREVER);
	if (BK_OK != ret)
	{
		LOGE("%s semaphore get failed: %d\n", __func__, ret);
	}
	msg->result = picture_echo_node->result;
	ret = msg->result;
	os_free(picture_echo_node);
	return ret;
	
}

bk_err_t lcd_display_echo_event_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
	LOGI("%s \n", __func__);

	frame_buffer_t *jpeg_frame = (frame_buffer_t *)msg->param;
	jpeg_frame->fmt = PIXEL_FMT_UVC_JPEG;
	if(msg->result != BK_OK)
	{
		frame_buffer_fb_jpeg_free(jpeg_frame);
		ret = BK_FAIL;
		goto error;
	}

	if (jpeg_frame == NULL)
	{
		LOGW("%s:jpeg_frame == NULL\n", __func__);
		ret = BK_FAIL;
		goto error;
	}

	frame_buffer_t *dec_frame = lcd_driver_decoder_frame(jpeg_frame);
	lcd_info.picture_echo = false;

	if (dec_frame == NULL)
	{
		frame_buffer_fb_jpeg_free(jpeg_frame);
		LOGE("display jpeg decoder error\n");
		ret = BK_FAIL;
		goto error;
	}

	frame_buffer_fb_jpeg_free(jpeg_frame);
	frame_buffer_fb_display_push(dec_frame);
	
	LOGI("%s complete\n", __func__);
	msg->result = ret;
	ret = rtos_set_semaphore(&msg->sem);
	if (ret != BK_OK)
	{
		LOGE("%s semaphore set failed: %d\n", __func__, ret);
	}
	return ret;

error:
	lcd_info.picture_echo = false;
	camera_display_task_start(lcd_info.rotate); 
	set_lcd_state(LCD_STATE_ENABLED);

	msg->result = ret;
	LOGI("%s: open camera_display_task \n", __func__);

	ret = rtos_set_semaphore(&msg->sem);
	if (ret != BK_OK)
	{
		LOGE("%s semaphore set failed: %d\n", __func__, ret);
	}
	return BK_FAIL;
}

void lcd_display_beken_logo_handle(param_pak_t *param)
{
	bk_err_t ret = BK_OK;
	frame_buffer_t *jpeg_frame = NULL;
	frame_buffer_t *dec_frame = NULL;
	sw_jpeg_dec_res_t result;

	lcd_display_t *logo_info = (lcd_display_t *)param->param;

#if CONFIG_LCD_FLASH_JPEG_DISPLAY
	if (logo_info->image_addr == 1)
	{
		/// use define addr and len
		logo_info->image_addr = (uint32_t)(beken_800_480_jpg);
		logo_info->img_length = sizeof(beken_800_480_jpg);
	}
	// else
	// {
	// 	logo_info->image_addr = (uint32_t)(QR_Code);
	// 	logo_info->img_length = sizeof(QR_Code);
	// }
#else
	LOGE("flash img display macro is not enable \n");
	goto out;
#endif

	camera_display_task_stop();  //USB Camera
//	rtos_delay_milliseconds(10);
	set_lcd_state(LCD_STATE_DISPLAY);

	ret = frame_buffer_fb_jpeg_init(PPI_800X600);
	frame_buffer_fb_register(MODULE_DECODER, FB_INDEX_JPEG);
	jpeg_frame = frame_buffer_fb_jpeg_malloc();

	if (jpeg_frame == NULL)
	{
		LOGE("jpeg frame malloc failed\r\n");
		goto out;
	}

	os_memcpy_word((uint32_t *)jpeg_frame->frame, (uint32_t *)logo_info->image_addr, logo_info->img_length);
	ret = bk_jpeg_dec_sw_init();
	if (ret != kNoErr) {
		LOGE("init sw jpeg decoder failed\r\n");
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		goto out;
	}

	dec_frame = frame_buffer_fb_display_malloc();
	if (dec_frame == NULL)
	{
		LOGE("src_frame malloc failed\r\n");
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		goto out;
	}

	ret = bk_jpeg_dec_sw_start(logo_info->img_length, jpeg_frame->frame, dec_frame->frame, &result);
	bk_jpeg_dec_sw_deinit();

	if (ret != kNoErr) {
		frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
		frame_buffer_fb_display_free(dec_frame);
		LOGE("sw jpeg decoder failed\r\n");
		goto out;
	}

	LOGI("sw jpeg decode (pixel_x pixel_y) : (%x, %x) \r\n", result.pixel_x, result.pixel_y);
	dec_frame->width = result.pixel_x;
	dec_frame->height = result.pixel_y;
	dec_frame->fmt = PIXEL_FMT_YUYV;

	frame_buffer_fb_jpeg_free(jpeg_frame); //frame_buffer_fb_free
	frame_buffer_fb_deregister(MODULE_DECODER);

	frame_buffer_fb_display_push(dec_frame);

out:

	MEDIA_EVT_RETURN(param, ret);
}

bk_err_t lcd_font_handle(frame_buffer_t *frame)
{
#if CONFIG_LCD_FONT_BLEND

	lcd_font_config_t lcd_font_config = {0};

	uint16_t start_x = 0;
	uint16_t start_y = 0;
	uint32_t frame_addr_offset = 0;

	if(g_blend_data.lcd_blend_type == 0)
	{
		return BK_OK;
	}
	
	if ((lcd_info.lcd_width < frame->width)  || (lcd_info.lcd_height < frame->height) ) //for lcd size is small then frame image size
	{
		if (lcd_info.lcd_width < frame->width)
			start_x = (frame->width - lcd_info.lcd_width) / 2;
		if (lcd_info.lcd_height < frame->height)
			start_y = (frame->height - lcd_info.lcd_height) / 2;
	}

	if ((g_blend_data.lcd_blend_type & LCD_BLEND_TIME) != 0)         /// start display lcd (0,0)
	{
#if CONFIG_CAMERA
		if (bk_dvp_camera_get_device() == NULL)
		{
			LOGD("lcd time blend =%s \n", g_blend_data.time_data);
			frame_addr_offset = (start_y * frame->width + start_x) * 2;

			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - CLOCK_LOGO_W;
			lcd_font_config.xsize = CLOCK_LOGO_W;
			lcd_font_config.ysize = CLOCK_LOGO_H;
			lcd_font_config.str_num = 1;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;
			lcd_font_config.str[0] = (font_str_t){(const char *)g_blend_data.time_data, FONT_WHITE, font_digit_Roboto53, 0, 0}; 
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
		}
		else
		{
			frame_addr_offset = (start_y * frame->width + start_x) * 2;

			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DVP_LOGO_W;
			lcd_font_config.xsize = DVP_LOGO_W;
			lcd_font_config.ysize = DVP_LOGO_H;
			lcd_font_config.str_num = 1;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;

			lcd_font_config.str[0] = (font_str_t){(const char *)g_blend_data.time_data, FONT_WHITE, font_digit_black24, 0,0}; 
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
		}
#else
		LOGD("lcd time blend =%s \n", g_blend_data.time_data);
		frame_addr_offset = (start_y * frame->width + start_x) * 2;

		lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
		lcd_font_config.bg_offline = frame->width - CLOCK_LOGO_W;
		lcd_font_config.xsize = CLOCK_LOGO_W;
		lcd_font_config.ysize = CLOCK_LOGO_H;
		lcd_font_config.str_num = 1;
		if (frame->fmt == PIXEL_FMT_VUYY)
			lcd_font_config.font_format = FONT_VUYY;
		else if (frame->fmt == PIXEL_FMT_YUYV)
			lcd_font_config.font_format = FONT_YUYV;
		else
			lcd_font_config.font_format = FONT_RGB565;

		lcd_font_config.str[0] = (font_str_t){(const char *)g_blend_data.time_data, FONT_WHITE, font_digit_Roboto53, 0, 0}; 
		lcd_font_config.bg_data_format = frame->fmt;
		lcd_font_config.bg_width = frame->width;
		lcd_font_config.bg_height = frame->height;
		lcd_driver_font_blend(&lcd_font_config);
#endif
	}
#if (!CONFIG_SOC_BK7258)
	lcd_blend_t lcd_blend = {0};

	if ((g_blend_data.lcd_blend_type & LCD_BLEND_WIFI) != 0)      /// start display lcd (lcd_width,0)
	{
		LOGD("lcd wifi blend level =%d \n", g_blend_data.wifi_data);
		frame_addr_offset = (start_y * frame->width + start_x + (lcd_info.lcd_width - WIFI_LOGO_W)) * 2;

		lcd_blend.pfg_addr = (uint8_t *)wifi_logo[g_blend_data.wifi_data];
		lcd_blend.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
		lcd_blend.fg_offline = 0;
		lcd_blend.bg_offline = frame->width - WIFI_LOGO_W;
		lcd_blend.xsize = WIFI_LOGO_W;
		lcd_blend.ysize = WIFI_LOGO_H;
		lcd_blend.fg_alpha_value = FG_ALPHA;
		lcd_blend.fg_data_format = ARGB8888;
		lcd_blend.bg_data_format = frame->fmt;
		lcd_blend.bg_width = frame->width;
		lcd_blend.bg_height = frame->height;
		if (g_blend_data.wifi_data != g_wifi_current_level)
		{
			g_wifi_current_level = g_blend_data.wifi_data;
			lcd_blend.flag = 1;
		}
		else
		{
			lcd_blend.flag = 0;
		}
		lcd_driver_blend(&lcd_blend);
	}
#endif
	if ((g_blend_data.lcd_blend_type & LCD_BLEND_DATA) != 0)   /// tart display lcd (DATA_POSTION_X,DATA_POSTION_Y)
	{
#if CONFIG_CAMERA
		if (bk_dvp_camera_get_device() == NULL)
		{
			if ((DATA_POSTION_X + DATA_LOGO_W) > lcd_info.lcd_width)
				frame_addr_offset = ((start_y + DATA_POSTION_Y + lcd_info.lcd_height - DATA_LOGO_H) * frame->width + start_x) * 2;
			else
				frame_addr_offset = ((start_y + DATA_POSTION_Y) * frame->width + start_x + DATA_POSTION_X) * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DATA_LOGO_W;
			lcd_font_config.xsize = DATA_LOGO_W;
			lcd_font_config.ysize = DATA_LOGO_H;
			lcd_font_config.str_num = 2;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;

			lcd_font_config.str[0] = (font_str_t){(const char *)("晴转多云, 27℃"), FONT_WHITE, font_digit_black24, 0, 2};
			lcd_font_config.str[1] = (font_str_t){(const char *)("2022-12-12 星期三"), FONT_WHITE, font_digit_black24, 0, 26};

			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
		}
		else
		{
			frame_addr_offset = ((start_y + DATA_POSTION_Y + lcd_info.lcd_height - DVP_LOGO_H) * frame->width + start_x) * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DVP_LOGO_W;
			lcd_font_config.xsize = DVP_LOGO_W;
			lcd_font_config.ysize = DVP_LOGO_H;
			lcd_font_config.str_num = 1;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;
			lcd_font_config.str[0] = (font_str_t){(const char *)("2023-"), FONT_WHITE, font_digit_black24, 0, 0};
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);

			frame_addr_offset = frame_addr_offset + 60 * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DVP_LOGO_W;
			lcd_font_config.xsize = DVP_LOGO_W;
			lcd_font_config.ysize = DVP_LOGO_H;
			lcd_font_config.str_num = 1;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;

			lcd_font_config.str[0] = (font_str_t){(const char *)("01-14"), FONT_WHITE,font_digit_black24, 0, 0};
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
		}
#else
			if ((DATA_POSTION_X + DATA_LOGO_W) > lcd_info.lcd_width)
				frame_addr_offset = ((start_y + DATA_POSTION_Y + lcd_info.lcd_height - DATA_LOGO_H) * frame->width + start_x) * 2;
			else
				frame_addr_offset = ((start_y + DATA_POSTION_Y) * frame->width + start_x + DATA_POSTION_X) * 2;

//			frame_addr_offset = ((start_y + DATA_POSTION_Y) * frame->width + start_x + DATA_POSTION_X) * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DATA_LOGO_W;
			lcd_font_config.xsize = DATA_LOGO_W;
			lcd_font_config.ysize = DATA_LOGO_H;
			lcd_font_config.str_num = 2;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;

			lcd_font_config.str[0] = (font_str_t){(const char *)("晴转多云, 27℃"), FONT_WHITE, font_digit_black24, 0, 2};
			lcd_font_config.str[1] = (font_str_t){(const char *)("2022-12-12 星期三"), FONT_WHITE, font_digit_black24, 0, 26};
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
#endif
	}
	
	if ((g_blend_data.lcd_blend_type & LCD_BLEND_VERSION) != 0) /// start display lcd (VERSION_POSTION_X,VERSION_POSTION_Y)
	{
		LOGD("lcd blend version =%s \n", g_blend_data.ver_data);
#if CONFIG_CAMERA
		char half_ver_temp[10] = {0};
		char *half_ver = half_ver_temp;
		uint32_t x_pos = VERSION_POSTION_X;
		
		uint8_t ver_len = strlen((char*)g_blend_data.ver_data);
		os_memcpy(half_ver, (const char *)g_blend_data.ver_data, 7);
		for (int i = 0; i < 2; i++)
		{
			if (lcd_info.lcd_height > PIXEL_480)
			{
				frame_addr_offset = ((start_y + VERSION_POSTION_Y + 300) * frame->width + start_x + x_pos) * 2;
			}
			else
			{
				frame_addr_offset = ((start_y + VERSION_POSTION_Y) * frame->width + start_x + x_pos) * 2;
			}
			lcd_font_config.bg_offline = frame->width - DVP_LOGO_W;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.xsize = DVP_LOGO_W;
			lcd_font_config.ysize = DVP_LOGO_H;
			lcd_font_config.str_num = 1;
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else if (frame->fmt == PIXEL_FMT_YUYV)
				lcd_font_config.font_format = FONT_YUYV;
			else
				lcd_font_config.font_format = FONT_RGB565;

			lcd_font_config.str[0] = (font_str_t){(const char *)half_ver, FONT_WHITE, font_digit_black24, 0, 0};
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
			x_pos += 84;  //ver num * ver width = 7*12
			os_memcpy(half_ver, (const char *)g_blend_data.ver_data + 7, (ver_len - 6));
		}
#else
		///for "VL4"
		lcd_font_config.bg_offline = frame->width - VERSION_LOGO_W;
		if (lcd_info.lcd_height > PIXEL_480)
		{
			frame_addr_offset = ((start_y + VERSION_POSTION_Y + 300) * frame->width + start_x + VERSION_POSTION_X) * 2;
		}
		else
		{
			frame_addr_offset = ((start_y + VERSION_POSTION_Y) * frame->width + start_x + VERSION_POSTION_X) * 2;
		}
		lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
		lcd_font_config.xsize = VERSION_LOGO_W;
		lcd_font_config.ysize = VERSION_LOGO_H;
		lcd_font_config.str_num = 1;
		#if 1  ///font yuv data to bg yuv image
		if (frame->fmt == PIXEL_FMT_VUYY)
			lcd_font_config.font_format = FONT_VUYY;
		else
			lcd_font_config.font_format = FONT_YUYV;
		#else  ///font rgb data to bg yuv image
		lcd_font_config.font_format = FONT_RGB565;
		#endif
		lcd_font_config.str[0] = (font_str_t){(const char *)g_blend_data.ver_data, FONT_WHITE, font_digit_black48, 0, 0};
		lcd_font_config.bg_data_format = frame->fmt;
		lcd_font_config.bg_width = frame->width;
		lcd_font_config.bg_height = frame->height;
		lcd_driver_font_blend(&lcd_font_config);
#endif
	}
#endif  //CONFIG_LCD_FONT_BLEND
	return BK_OK;
}

bk_err_t lcd_dma2d_handle(frame_buffer_t *frame)
{
#if CONFIG_LCD_DMA2D_BLEND
	lcd_blend_t lcd_blend = {0};

	if(g_blend_data.lcd_blend_type == 0)
	{
		return BK_OK;
	}
	if ((g_blend_data.lcd_blend_type & LCD_BLEND_WIFI) != 0)      /// start display lcd (xpos,ypos)
	{
		if(g_blend_data.wifi_data > WIFI_LEVEL_MAX - 1)
			return BK_FAIL;
		lcd_blend.pfg_addr = (uint8_t *)wifi_logo[g_blend_data.wifi_data];
		lcd_blend.pbg_addr = (uint8_t *)(frame->frame);
		lcd_blend.xsize = WIFI_LOGO_W;
		lcd_blend.ysize = WIFI_LOGO_H;
		lcd_blend.xpos = lcd_info.lcd_width - WIFI_LOGO_W;
		lcd_blend.ypos = WIFI_LOGO_YPOS;
		lcd_blend.fg_alpha_value = FG_ALPHA;
		lcd_blend.fg_data_format = ARGB8888;
		lcd_blend.bg_data_format = frame->fmt;
		lcd_blend.bg_width = frame->width;
		lcd_blend.bg_height = frame->height;
		lcd_dma2d_driver_blend(&lcd_blend);
	}
#endif
return BK_OK;
}

bk_err_t lcd_close_handle(media_mailbox_msg_t *msg)
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

	camera_display_task_stop();
	
#ifdef DISPLAY_PIPELINE_TASK
	jpeg_display_task_stop();
#endif
	lcd_driver_deinit();

#if CONFIG_LCD_DMA2D_BLEND || CONFIG_LCD_FONT_BLEND
	g_blend_data.lcd_blend_type = 0;

	if (yuv_blend_addr != NULL && (uint32_t)yuv_blend_addr != LCD_BLEND_JPEGSRAM_ADDR_1)
	{
		if (yuv_blend_addr)
		{
			os_free(yuv_blend_addr);
		}

		if (rgb_blend_addr)
		{
			os_free(rgb_blend_addr);
		}
	}

	yuv_blend_addr = NULL;
	rgb_blend_addr = NULL;
	g_wifi_current_level = WIFI_LEVEL_MAX;

#endif

	lcd_info.rotate = ROTATE_NONE;
	lcd_info.resize = false;
	lcd_info.resize_ppi = PPI_800X480;

	set_lcd_state(LCD_STATE_DISABLED);

	LOGI("%s complete\n", __func__);

out:
    return ret;
}

bk_err_t lcd_rotate_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
	lcd_info.rotate = msg->param;
    return ret;
}

bk_err_t lcd_blend_font_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
#if CONFIG_LCD_DMA2D_BLEND || CONFIG_LCD_FONT_BLEND
    lcd_blend_msg_t  *blend_data = (lcd_blend_msg_t *)msg->param;
	if(blend_data != NULL)
	{
		LOGD("lcd_EVENT_LCD_BLEND_IND type=%d on=%d\n", blend_data->lcd_blend_type, blend_data->blend_on);

		if ((blend_data->lcd_blend_type & LCD_BLEND_WIFI) == LCD_BLEND_WIFI)
		{
			if(blend_data->blend_on == 0)
			{
				g_wifi_current_level = WIFI_LEVEL_MAX;
				g_blend_data.lcd_blend_type &= (~LCD_BLEND_WIFI);
			}
			else
			{
				g_blend_data.lcd_blend_type |= LCD_BLEND_WIFI;
				g_blend_data.wifi_data = blend_data->data[0];

				LOGD("g_blend_data.wifi_data =%d\n", g_blend_data.wifi_data );
			}
		}
		if ((blend_data->lcd_blend_type & LCD_BLEND_TIME) == LCD_BLEND_TIME)
		{
			if(blend_data->blend_on == 0)
			{
				g_blend_data.lcd_blend_type &= (~LCD_BLEND_TIME);
			}
			else
			{
				g_blend_data.lcd_blend_type |= LCD_BLEND_TIME;
				os_memcpy(g_blend_data.time_data, blend_data->data, MAX_BLEND_NAME_LEN);
				LOGD("g_blend_data.time_data =%s\n", g_blend_data.time_data );
			}
		}
		if ((blend_data->lcd_blend_type & LCD_BLEND_DATA) == LCD_BLEND_DATA)
		{
			if(blend_data->blend_on == 0)
			{
				g_blend_data.lcd_blend_type &= (~LCD_BLEND_DATA);
			}
			else
			{
				g_blend_data.lcd_blend_type |= LCD_BLEND_DATA;
				os_memcpy(g_blend_data.year_to_data, blend_data->data, MAX_BLEND_NAME_LEN);
				LOGD("g_blend_data.chs =%s\n", g_blend_data.year_to_data);
			}
		}
		if ((blend_data->lcd_blend_type & LCD_BLEND_VERSION) == LCD_BLEND_VERSION)
		{
			if(blend_data->blend_on == 0)
			{
				g_blend_data.lcd_blend_type &= (~LCD_BLEND_VERSION);
			}
			else
			{
				g_blend_data.lcd_blend_type |= LCD_BLEND_VERSION;
				os_memcpy(g_blend_data.ver_data, blend_data->data, MAX_BLEND_NAME_LEN);
				LOGE("g_blend_data.ver_data =%s\n", g_blend_data.ver_data );
			}
		}
	 }
#endif
    return ret;
}

void lcd_set_backligth_handle(param_pak_t *param)
{
	int ret = BK_OK;

	LOGI("%s, levle: %d\n", __func__, param->param);
	lcd_driver_set_backlight(param->param);

	MEDIA_EVT_RETURN(param, ret);
}

void lcd_act_dump_decoder_frame(void)
{
#if (CONFIG_IMAGE_STORAGE && !CONFIG_SLAVE_CORE)
	//storage_frame_buffer_dump(lcd_info.decoder_frame, "decoder_vuyy.yuv");
#endif
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

#if (CONFIG_IMAGE_STORAGE && !CONFIG_SLAVE_CORE)
	storage_frame_buffer_dump(dbg_jpeg_frame, frame_suffix(dbg_jpeg_frame->fmt));
#endif
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

#if (CONFIG_IMAGE_STORAGE && !CONFIG_SLAVE_CORE)
	storage_frame_buffer_dump(dbg_display_frame, frame_suffix(dbg_display_frame->fmt));
#endif
}

void lcd_event_handle(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;

	switch (msg->event)
	{
		case EVENT_LCD_OPEN_IND:
			LOGI("%s EVENT_LCD_OPEN_IND \n", __func__);
			ret = lcd_open_handle(msg);
			break;
		case EVENT_LCD_CLOSE_IND:
			LOGI(" %s EVENT_LCD_CLOSE_IND \n", __func__);
			ret = lcd_close_handle(msg);
		break;

		case EVENT_LCD_ROTATE_ENABLE_IND:
		{
			LOGI("%s EVENT_LCD_ROTATE_ENABLE_IND\n", __func__);
			ret = lcd_rotate_handle(msg);
		}
		break;

		case EVENT_LCD_BLEND_IND:
		{
#if CONFIG_LCD_DMA2D_BLEND || CONFIG_LCD_FONT_BLEND
			ret = lcd_blend_font_handle(msg);
#endif
		}
		break;
		
		case EVENT_LCD_DISPLAY_FILE_IND:
		{
			LOGI("%s EVENT_LCD_DISPLAY_FILE_IND\n", __func__);
			ret = lcd_display_file_handle(msg);
		}
		break;

		default:
			break;
    }

	msg_send_to_media_major_mailbox(msg, ret, APP_MODULE);
}
#if 0
void lcd_event_handle(uint32_t event, uint32_t param)
{
	param_pak_t *param_pak = NULL;

	switch (event)
	{
		case EVENT_LCD_OPEN_IND:
            LOGE("====>>>>6 %s\n", __func__);
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

		case EVENT_LCD_RESIZE_IND:
		{
			param_pak = (param_pak_t *)param;
			if (lcd_info.resize)
			{
				lcd_info.resize = false;
			}
			else
			{
				lcd_info.resize = true;
				lcd_info.resize_ppi = (media_ppi_t)(param_pak->param);
			}

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

		case EVENT_LCD_DISPLAY_FILE_IND:
			lcd_display_file_handle((param_pak_t *)param);
			break;

		case EVENT_LCD_BEKEN_LOGO_DISPLAY:
			lcd_display_beken_logo_handle((param_pak_t *)param);
			break;

		case EVENT_LCD_DISPLAY_IND:
			lcd_display_handle((param_pak_t *)param);
			break;

		case EVENT_LCD_BLEND_IND:
		{
#if CONFIG_LCD_DMA2D_BLEND || CONFIG_LCD_FONT_BLEND
			param_pak = (param_pak_t *)param;
			lcd_blend_msg_t  *blend_data =(lcd_blend_msg_t  *)param_pak->param;
			if(blend_data != NULL)
			{
				LOGD("lcd_EVENT_LCD_BLEND_IND type=%d on=%d\n", blend_data->lcd_blend_type, blend_data->blend_on);

				if ((blend_data->lcd_blend_type & LCD_BLEND_WIFI) == LCD_BLEND_WIFI)
				{
					if(blend_data->blend_on == 0)
					{
						g_wifi_current_level = WIFI_LEVEL_MAX;
						g_blend_data.lcd_blend_type &= (~LCD_BLEND_WIFI);
					}
					else
					{
						g_blend_data.lcd_blend_type |= LCD_BLEND_WIFI;
						g_blend_data.wifi_data = blend_data->data[0];

						LOGD("g_blend_data.wifi_data =%d\n", g_blend_data.wifi_data );
					}
				}
				if ((blend_data->lcd_blend_type & LCD_BLEND_TIME) == LCD_BLEND_TIME)
				{
					if(blend_data->blend_on == 0)
					{
						g_blend_data.lcd_blend_type &= (~LCD_BLEND_TIME);
					}
					else
					{
						g_blend_data.lcd_blend_type |= LCD_BLEND_TIME;
						os_memcpy(g_blend_data.time_data, blend_data->data, MAX_BLEND_NAME_LEN);
						LOGD("g_blend_data.time_data =%s\n", g_blend_data.time_data );
					}
				}
				if ((blend_data->lcd_blend_type & LCD_BLEND_DATA) == LCD_BLEND_DATA)
				{
					if(blend_data->blend_on == 0)
					{
						g_blend_data.lcd_blend_type &= (~LCD_BLEND_DATA);
					}
					else
					{
						g_blend_data.lcd_blend_type |= LCD_BLEND_DATA;
						os_memcpy(g_blend_data.year_to_data, blend_data->data, MAX_BLEND_NAME_LEN);
						LOGD("g_blend_data.chs =%s\n", g_blend_data.year_to_data);
					}
				}
				if ((blend_data->lcd_blend_type & LCD_BLEND_VERSION) == LCD_BLEND_VERSION)
				{
					if(blend_data->blend_on == 0)
					{
						g_blend_data.lcd_blend_type &= (~LCD_BLEND_VERSION);
					}
					else
					{
						g_blend_data.lcd_blend_type |= LCD_BLEND_VERSION;
						os_memcpy(g_blend_data.ver_data, blend_data->data, MAX_BLEND_NAME_LEN);
						LOGE("g_blend_data.ver_data =%s\n", g_blend_data.ver_data );
					}
				}
			 }

			MEDIA_EVT_RETURN(param_pak, BK_OK);
#endif
		}
		break;
	}
}
#endif

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
	lcd_info.rotate = ROTATE_NONE;
	lcd_info.resize = false;
	lcd_info.resize_ppi = PPI_800X480;
	lcd_info.decode_mode = HARDWARE_DECODING;
}

uint8_t get_decode_mode(void)
{
	return lcd_info.decode_mode;
}

void set_decode_mode(uint8_t mode)
{
	lcd_info.decode_mode = mode;
}

void lcd_set_logo_on(uint8_t on)
{
	lcd_info.start_logo = on;
}




