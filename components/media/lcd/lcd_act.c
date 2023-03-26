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
//#include <lcd_dma2d_config.h>
#include <driver/jpeg_dec_types.h>
#include "modules/image_scale.h"
//#include "lcd_blend_config.h"
#include <driver/uvc_camera_types.h>
#include <driver/uvc_camera.h>

#include <driver/timer.h>
//#include <driver/media_types.h>
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
#if CONFIG_LCD_DMA2D_BLEND_FLASH_IMG || CONFIG_LCD_FONT_BLEND
static lcd_blend_data_t g_blend_data = {0};
static uint8_t *yuv_blend_addr = NULL;
static uint8_t *rgb_blend_addr = NULL;
uint8_t g_wifi_current_level = WIFI_LEVEL_MAX;

#endif
static int g_lcd_display_logo_first = 0;
static short g_lcd_width = 0;
static short g_lcd_height = 0;
#if CONFIG_BLEND_USE_GUI
static short g_cammer_width = 0;
static short g_blend_x_size = 0;
static short g_blend_y_size = 0;
static short g_blend_hor_align = 0;   //0£ºË®Æ½×ó¶ÔÆë  1£ºË®Æ½¾ÓÖÐ   2£ºË®Æ½ÓÒ¶ÔÆë
static short g_blend_ver_offset = 0;  //´¹Ö±Æ«ÒÆ
static beken_mutex_t g_disp_mutex_blend;
static int g_gui_blend_switch = BK_FALSE;
lcd_blend_t g_lcd_blend_data;
#endif

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
			LOGD("read display frame NULL\n");
			continue;
		}

		if (rotate == true)
		{
			if (lcd_transfer_info.device_ppi == PPI_320X480)
			{
				frame = lcd_driver_rotate_frame_ppi(frame, PPI_320X480);
			}
			else
			{
				frame = lcd_driver_rotate_frame(frame);
			}

			if (frame == NULL)
			{
				LOGD("rotate frame NULL\n");
				continue;
			}
		}

		lcd_blend_handle(frame);

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
	                         8 * 1024,
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

			if (rotate == true)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame);

				if (dec_frame == NULL)
				{
					LOGD("rotate frame NULL\n");
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

			if (rotate == true)
			{
				dec_frame = lcd_driver_rotate_frame(dec_frame);

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

void lcd_open_handle(param_pak_t *param)
{
	int ret = BK_OK;

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

	lcd_open_t *lcd_open = (lcd_open_t *)param->param;
	lcd_config_t lcd_config = {0};

#if CONFIG_LCD_DMA2D_BLEND_FLASH_IMG  || CONFIG_LCD_FONT_BLEND

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

	LOGI("%s, ppi: %dX%d %s\n", __func__, lcd_config.device->ppi >> 16, lcd_config.device->ppi & 0xFFFF, lcd_config.device->name);

	lcd_config.fb_display_init  = frame_buffer_fb_display_init;
	lcd_config.fb_display_deinit  = frame_buffer_fb_display_deinit;
	lcd_config.fb_free  = frame_buffer_fb_display_free;
	lcd_config.fb_malloc = frame_buffer_fb_display_malloc_wait;

	lcd_driver_init(&lcd_config);

	camera_display_task_start(lcd_info.rotate);
#ifdef DISPLAY_PIPELINE_TASK
	jpeg_display_task_start(lcd_info.rotate);
#endif
	set_lcd_state(LCD_STATE_ENABLED);

	if (g_lcd_display_logo_first == 1)
	{
		ret= display_logo();
		g_lcd_display_logo_first=0;
		if(ret != BK_OK)
		{
			LOGE("%s display logo frist err \n", __func__);
		}
	}


	LOGI("%s complete\n", __func__);
	g_lcd_width = ppi_to_pixel_x(lcd_config.device->ppi);
	g_lcd_height = ppi_to_pixel_y(lcd_config.device->ppi);

out:
		MEDIA_EVT_RETURN(param, ret);

}

#if CONFIG_LVGL && CONFIG_BLEND_USE_GUI
bk_err_t media_app_lcd_set_pos(int hor_pos, int ver_offset)
{
    if(hor_pos >= 0 && hor_pos <= 2)
        g_blend_hor_align = hor_pos;

    if(ver_offset < (g_lcd_height - g_blend_y_size))
        g_blend_ver_offset = ver_offset;

    bk_printf("g_blend_hor_align:%d, g_blend_ver_offset:%d\r\n", g_blend_hor_align, g_blend_ver_offset);
    return kNoErr;
}

bk_err_t media_app_lcd_gui_blend_open(int blend_x_size, int blend_y_size)
{
    //if(g_lcd_with_gui)
    {
        bk_printf("[%s][%d] you should close gui first: media lcd close\r\n", __FUNCTION__, __LINE__);
        return kGeneralErr;
    }

    if(g_lcd_width> 0 && blend_x_size > g_lcd_width)
    {
        bk_printf("[%s][%d] blend_x_size %d is two big, should <= lcd width \r\n", 
                                                            __FUNCTION__, __LINE__, blend_x_size);
        return kGeneralErr;
    }

    rtos_init_mutex(&g_disp_mutex_blend);
    g_blend_x_size = blend_x_size;
    g_blend_y_size = blend_y_size;
    bk_err_t bk_psram_init(void);
    bk_psram_init();
    bk_gui_disp_task_init(blend_x_size, blend_y_size, BK_TRUE);
    g_gui_blend_switch = BK_TRUE;
    
    return kNoErr;
}

bk_err_t media_app_lcd_gui_blend_close(void)
{
    g_gui_blend_switch = BK_FALSE;

    bk_gui_disp_task_deinit();
    rtos_deinit_mutex(&g_disp_mutex_blend);

    os_memset(&g_lcd_blend_data, 0, sizeof(g_lcd_blend_data));

    return kNoErr;
}
#endif

#if (CONFIG_CAMERA || CONFIG_USB_UVC)
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


void lcd_display_file_handle(param_pak_t *param)
{
	int ret = BK_OK;
	LOGI("%s, %s\n", __func__, (char *)param->param);
	char *file_name = NULL;
	
	file_name = (char *)os_malloc(32);
	os_memcpy(file_name, (char *)param->param, 31);
	file_name[31] = 0;
	LOGI("display sd card file name: %s \n", file_name);

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

#if CONFIG_FATFS
		ret = sdcard_read_to_mem((char *)file_name, (uint32_t *)jpeg_frame->frame, &jpeg_frame->length );
#endif

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

	frame_buffer_fb_jpeg_free(jpeg_frame);    //frame_buffer_fb_free
	frame_buffer_fb_display_push(dec_frame);
out:
	os_free(file_name);

	MEDIA_EVT_RETURN(param, ret);
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

void lcd_driver_set_blend_data(uint8_t *gui_addr, uint32 xsize, uint32 ysize, uint32 color_size)
{
#if CONFIG_BLEND_USE_GUI
    u8 *gui_bak_addr = NULL;

    if(g_gui_blend_switch == BK_TRUE)
    {
        rtos_lock_mutex(&g_disp_mutex_blend);
        void lv_get_gui_blend_buff(u8 **yuv_data, u8 **rgb565_data, u8 **gui_bak_data);
        void dma2d_memcpy_psram(void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline);

        lv_get_gui_blend_buff(NULL, NULL, &gui_bak_addr);
        if(g_cammer_width < xsize)   //eg:lcd:800*480 camera:640*480  blend:700*480
        {
            if(color_size == 2)
                dma2d_memcpy_psram(gui_addr, gui_bak_addr, g_cammer_width, ysize, (xsize-g_cammer_width), 0);
            else if(color_size == 4)
                dma2d_memcpy_psram(gui_addr, gui_bak_addr, g_cammer_width*2, ysize, (xsize-g_cammer_width)*2, 0);
            //os_memcpy_word((uint32_t*)gui_bak_addr, (const uint32_t *)gui_addr, xsize*ysize*color_size);
            g_lcd_blend_data.xsize = g_cammer_width;
        }
        else
        {
            if(color_size == 2)
                dma2d_memcpy_psram(gui_addr, gui_bak_addr, xsize, ysize, 0, 0);
            else
                dma2d_memcpy_psram(gui_addr, gui_bak_addr, xsize*2, ysize, 0, 0);
            //os_memcpy_word((uint32_t*)gui_bak_addr, (const uint32_t *)gui_addr, xsize*ysize*color_size*2);
            g_lcd_blend_data.xsize = xsize;
        }
        
        g_lcd_blend_data.pfg_addr = (uint8_t *)gui_bak_addr;
        
        g_lcd_blend_data.fg_offline = 0;
        g_lcd_blend_data.ysize = ysize;
        g_lcd_blend_data.fg_alpha_value = 255;
#if !CONFIG_BLEND_GUI_OUTPUT_888
        g_lcd_blend_data.fg_data_format = RGB565;
#else
        g_lcd_blend_data.fg_data_format = ARGB8888;
#endif
        rtos_unlock_mutex(&g_disp_mutex_blend);
    }
#endif
}

bk_err_t lcd_blend_handle(frame_buffer_t *frame)
{
#if CONFIG_BLEND_USE_GUI
    int x_offset = 0;
    int y_offset = 0;

    if(g_gui_blend_switch == BK_TRUE)
    {
        if(0 == g_cammer_width)
            g_cammer_width = frame->width;
        
        rtos_lock_mutex(&g_disp_mutex_blend);
        if(g_lcd_blend_data.xsize > 0)
        {
            if(0 == g_blend_hor_align)
            {
                if(frame->width >= g_lcd_width)
                    x_offset = (frame->width - g_lcd_width)/2;
                else
                    x_offset = 0;
            }
            else if(1 == g_blend_hor_align)
            {
                x_offset = (frame->width - g_blend_x_size)/2;
            }
            else if(2 == g_blend_hor_align)
            {
                if(frame->width >= g_lcd_width)
                    x_offset = (frame->width - g_lcd_width)/2 + (g_lcd_width - g_blend_x_size);
                else
                    x_offset = frame->width - g_blend_x_size;
            }

            if(frame->height >= g_lcd_height)
                y_offset = (frame->height - g_lcd_height)/2 + g_blend_ver_offset;
            else
                y_offset = 0 + g_blend_ver_offset;
            
            g_lcd_blend_data.pbg_addr = (uint8_t *)(frame->frame + y_offset*frame->width*2 + x_offset*2);
            g_lcd_blend_data.bg_offline = frame->width - g_lcd_blend_data.xsize;
            lcd_driver_blend(&g_lcd_blend_data);
        }
        rtos_unlock_mutex(&g_disp_mutex_blend);
    }
#endif

#if CONFIG_LCD_FONT_BLEND
	lcd_blend_t lcd_blend = {0};
	lcd_font_config_t lcd_font_config = {0};

	uint16_t start_x = 0;
	uint16_t start_y = 0;
	uint32_t frame_addr_offset = 0;

	if(g_blend_data.lcd_blend_type == 0)
	{
		return BK_OK;
	}
	if ((g_lcd_width < frame->width)  || (g_lcd_height < frame->height) ) //for lcd size is small then frame image size
	{
		if (g_lcd_width < frame->width)
			start_x = (frame->width - g_lcd_width) / 2;
		if (g_lcd_height < frame->height)
			start_y = (frame->height - g_lcd_height) / 2;
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
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
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
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
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
		#if 1  ///font yuv data to bg yuv image
		if (frame->fmt == PIXEL_FMT_VUYY)
			lcd_font_config.font_format = FONT_VUYY;
		else
			lcd_font_config.font_format = FONT_YUYV;
		#else  ///font rgb data to bg yuv image
		lcd_font_config.font_format = FONT_RGB565;
		#endif
		lcd_font_config.str[0] = (font_str_t){(const char *)g_blend_data.time_data, FONT_WHITE, font_digit_Roboto53, 0, 0}; 
		lcd_font_config.bg_data_format = frame->fmt;
		lcd_font_config.bg_width = frame->width;
		lcd_font_config.bg_height = frame->height;
		lcd_driver_font_blend(&lcd_font_config);
#endif
	}
	if ((g_blend_data.lcd_blend_type & LCD_BLEND_WIFI) != 0)      /// start display lcd (lcd_width,0)
	{
		LOGD("lcd wifi blend level =%d \n", g_blend_data.wifi_data);
		frame_addr_offset = (start_y * frame->width + start_x + (g_lcd_width - WIFI_LOGO_W)) * 2;

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
	if ((g_blend_data.lcd_blend_type & LCD_BLEND_DATA) != 0)   /// tart display lcd (DATA_POSTION_X,DATA_POSTION_Y)
	{
#if CONFIG_CAMERA
		if (bk_dvp_camera_get_device() == NULL)
		{
			if ((DATA_POSTION_X + DATA_LOGO_W) > g_lcd_width)
				frame_addr_offset = ((start_y + DATA_POSTION_Y + g_lcd_height - DATA_LOGO_H) * frame->width + start_x) * 2;
			else
				frame_addr_offset = ((start_y + DATA_POSTION_Y) * frame->width + start_x + DATA_POSTION_X) * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DATA_LOGO_W;
			lcd_font_config.xsize = DATA_LOGO_W;
			lcd_font_config.ysize = DATA_LOGO_H;
			lcd_font_config.str_num = 2;
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
			lcd_font_config.str[0] = (font_str_t){(const char *)("æ™´è½¬å¤šäº‘, 27â„ƒ"), FONT_WHITE, font_digit_black24, 0, 2};
			lcd_font_config.str[1] = (font_str_t){(const char *)("2022-12-12 æ˜ŸæœŸä¸‰"), FONT_WHITE, font_digit_black24, 0, 26};

			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
		}
		else
		{
			frame_addr_offset = ((start_y + DATA_POSTION_Y + g_lcd_height - DVP_LOGO_H) * frame->width + start_x) * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DVP_LOGO_W;
			lcd_font_config.xsize = DVP_LOGO_W;
			lcd_font_config.ysize = DVP_LOGO_H;
			lcd_font_config.str_num = 1;
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
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
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
			lcd_font_config.str[0] = (font_str_t){(const char *)("01-14"), FONT_WHITE,font_digit_black24, 0, 0};
			lcd_font_config.bg_data_format = frame->fmt;
			lcd_font_config.bg_width = frame->width;
			lcd_font_config.bg_height = frame->height;
			lcd_driver_font_blend(&lcd_font_config);
		}
#else
			if ((DATA_POSTION_X + DATA_LOGO_W) > g_lcd_width)
				frame_addr_offset = ((start_y + DATA_POSTION_Y + g_lcd_height - DATA_LOGO_H) * frame->width + start_x) * 2;
			else
				frame_addr_offset = ((start_y + DATA_POSTION_Y) * frame->width + start_x + DATA_POSTION_X) * 2;

//			frame_addr_offset = ((start_y + DATA_POSTION_Y) * frame->width + start_x + DATA_POSTION_X) * 2;
			lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset);
			lcd_font_config.bg_offline = frame->width - DATA_LOGO_W;
			lcd_font_config.xsize = DATA_LOGO_W;
			lcd_font_config.ysize = DATA_LOGO_H;
			lcd_font_config.str_num = 2;
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
			lcd_font_config.str[0] = (font_str_t){(const char *)("æ™´è½¬å¤šäº‘, 27â„ƒ"), FONT_WHITE, font_digit_black24, 0, 2};
			lcd_font_config.str[1] = (font_str_t){(const char *)("2022-12-12 æ˜ŸæœŸä¸‰"), FONT_WHITE, font_digit_black24, 0, 26};
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
			if (g_lcd_height > PIXEL_480)
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
			#if 1  ///font yuv data to bg yuv image
			if (frame->fmt == PIXEL_FMT_VUYY)
				lcd_font_config.font_format = FONT_VUYY;
			else
				lcd_font_config.font_format = FONT_YUYV;
			#else  ///font rgb data to bg yuv image
			lcd_font_config.font_format = FONT_RGB565;
			#endif
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
		if (g_lcd_height > PIXEL_480)
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
#endif
	return BK_OK;
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

	camera_display_task_stop();
	jpeg_display_task_stop();

	lcd_driver_deinit();

#if CONFIG_LCD_DMA2D_BLEND_FLASH_IMG || CONFIG_LCD_FONT_BLEND
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
		case PIXEL_FMT_UVC_H264:
			return "_uvc.h264";
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
		default:
			break;
	}

	return ".unknow";
}

#ifdef CONFIG_FATFS
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
#endif


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

#ifdef CONFIG_FATFS
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
#endif

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
#if CONFIG_LCD_DMA2D_BLEND_FLASH_IMG || CONFIG_LCD_FONT_BLEND
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
void lcd_set_logo_on(int status)
{
	g_lcd_display_logo_first = status;
}




