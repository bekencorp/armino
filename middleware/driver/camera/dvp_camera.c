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

#include <driver/int.h>
#include <os/mem.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>

#include <driver/dma.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <driver/jpeg_enc_types.h>

#include <driver/media_types.h>
#include <driver/dvp_camera.h>
#include <driver/dvp_camera_types.h>
#include "dvp_sensor_devices.h"

#include <driver/h264.h>
#include "bk_general_dma.h"

#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <gpio_map.h>

#include "gpio_driver.h"
#include <driver/gpio.h>
#include <modules/pm.h>

#if (CONFIG_PSRAM)
#include <driver/psram.h>
#endif

#if (CONFIG_YUV_BUF)
#include <driver/yuv_buf.h>
#endif

#include <soc/mapping.h>

#include "bk_misc.h"

#define TAG "dvp_drv"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define FRAME_BUFFER_DMA_TH (1024 * 10)
#define JPEG_CRC_SIZE (5)
#define DROP_FRAME_COUNT (0)

#define FRAME_BUFFER_CACHE (1024 * 10)
//#define JPEG_DROP_SENSOR_FRAME

#define DVP_STRIP

//#define DVP_DIAG_DEBUG

#define YUV_PARTIAL_DEBUG 0

#ifdef DVP_DIAG_DEBUG

#define DVP_DIAG_DEBUG_INIT()                   \
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
		gpio_dev_unmap(GPIO_12);                \
		bk_gpio_disable_pull(GPIO_12);          \
		bk_gpio_enable_output(GPIO_12);         \
		bk_gpio_set_output_low(GPIO_12);        \
		\
		gpio_dev_unmap(GPIO_13);                \
		bk_gpio_disable_pull(GPIO_13);          \
		bk_gpio_enable_output(GPIO_13);         \
		bk_gpio_set_output_low(GPIO_13);        \
		\
	} while (0)

#define DVP_JPEG_VSYNC_ENTRY()          bk_gpio_set_output_high(GPIO_2)
#define DVP_JPEG_VSYNC_OUT()            bk_gpio_set_output_low(GPIO_2)

#define DVP_JPEG_EOF_ENTRY()            bk_gpio_set_output_high(GPIO_3)
#define DVP_JPEG_EOF_OUT()              bk_gpio_set_output_low(GPIO_3)

#define DVP_YUV_EOF_ENTRY()             bk_gpio_set_output_high(GPIO_4)
#define DVP_YUV_EOF_OUT()               bk_gpio_set_output_low(GPIO_4)

#define DVP_JPEG_START_ENTRY()          bk_gpio_set_output_high(GPIO_5)
#define DVP_JPEG_START_OUT()            bk_gpio_set_output_low(GPIO_5)

#define DVP_JPEG_HEAD_ENTRY()           bk_gpio_set_output_high(GPIO_12)
#define DVP_JPEG_HEAD_OUT()             bk_gpio_set_output_low(GPIO_12)

#define DVP_PPI_ERROR_ENTRY()           DVP_YUV_EOF_ENTRY()
#define DVP_PPI_ERROR_OUT()             DVP_YUV_EOF_OUT()

#define DVP_H264_EOF_ENTRY()            DVP_JPEG_EOF_ENTRY()
#define DVP_H264_EOF_OUT()              DVP_JPEG_EOF_OUT()

#define DVP_JPEG_SDMA_ENTRY()           bk_gpio_set_output_high(GPIO_13)
#define DVP_JPEG_SDMA_OUT()             bk_gpio_set_output_low(GPIO_13)

#define DVP_DEBUG_IO()                      \
	do {                                    \
		bk_gpio_set_output_high(GPIO_6);    \
		bk_gpio_set_output_low(GPIO_6);     \
	} while (0)

#else
#define DVP_DIAG_DEBUG_INIT()

#define DVP_JPEG_EOF_ENTRY()
#define DVP_JPEG_EOF_OUT()

#define DVP_YUV_EOF_ENTRY()
#define DVP_YUV_EOF_OUT()

#define DVP_JPEG_START_ENTRY()
#define DVP_JPEG_START_OUT()

#define DVP_JPEG_HEAD_ENTRY()
#define DVP_JPEG_HEAD_OUT()

#define DVP_PPI_ERROR_ENTRY()
#define DVP_PPI_ERROR_OUT()

#define DVP_H264_EOF_ENTRY()
#define DVP_H264_EOF_OUT()

#define DVP_JPEG_SDMA_ENTRY()
#define DVP_JPEG_SDMA_OUT()

#define DVP_JPEG_VSYNC_ENTRY()
#define DVP_JPEG_VSYNC_OUT()

#endif

#define H264_TYPE_CONDITION(type)			\
	type == MEDIA_DVP_H264_WIFI_TRANSFER ||	\
	type == MEDIA_DVP_H264_USB_TRANSFER ||	\
	type == MEDIA_DVP_H264_LOCAL ||			\
	type == MEDIA_DVP_H264_ENC_LCD

#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
extern void usbd_video_h264_init();
extern bk_err_t demo_h264_usb_init(void);
#endif

#define IO_FUNCTION_ENABLE(pin, func)   \
	do {                                \
		gpio_dev_unmap(pin);            \
		gpio_dev_map(pin, func);        \
	} while (0)


typedef struct
{
	uint8 index : 1;
	uint8 cached : 1;
	uint8 eof : 1;
	uint8 error : 1;
	uint8 psram_dma;
	uint8 psram_dma_busy;
	int16 psram_dma_left;
	uint8 *buffer;
	uint8 drop_count;
	frame_buffer_t *frame;
} dvp_camera_drv_t;

typedef struct
{
	uint32_t yuv_em_addr;
	uint32_t yuv_pingpong_length;
	uint32_t yuv_data_offset;
	uint8_t dma_collect_yuv;
}h264_enc_lcd_config_t;

#define DVP_BUFFER_SIZE 1024
#define DVP_BUFFER_COUNT 4

extern media_debug_t *media_debug;

static beken_semaphore_t dvp_sema = NULL;
dvp_camera_drv_t *dvp_camera_drv = NULL;
h264_enc_lcd_config_t *h264_enc_lcd_config = NULL;
dvp_camera_device_t *dvp_camera_device = NULL;
static uint8_t *dvp_camera_encode = NULL;
static const dvp_sensor_config_t *current_sensor = NULL;
frame_buffer_t *curr_encode_frame = NULL;
frame_buffer_t *curr_yuv_frame = NULL;

static dvp_camera_config_t dvp_camera_config = {0};
static jpeg_config_t jpeg_config = {0};

static volatile media_state_t dvp_state = MSTATE_TURN_OFF;

uint32_t sequence = 0;
static uint8_t dvp_camera_dma_channel = DMA_ID_MAX;
#ifdef JPEG_DROP_SENSOR_FRAME
uint8_t g_vsync_index = 0;
#endif

static bool camera_read_flag = false;

bk_err_t dvp_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls);

/*
static void jpeg_dump(char *tag, uint8_t *src, uint32_t size)
{
    uint32_t i;

    LOGE("%s: ", tag);

    for (i = 0; i < size; i++)
    {
        os_printf("%02X ", src[i]);
    }
    os_printf("\n");
}
*/
#if (CONFIG_ENCODE_BUF_DYNAMIC)
static bk_err_t dvp_camera_yuv_base_addr_init(media_ppi_t ppi, yuv_buf_work_mode_t mode)
{
	if (dvp_camera_encode == NULL)
	{
		if (ppi > PPI_640X480)
		{
			if (mode == YUV_BUF_MODE_JPEG)
				dvp_camera_encode = (uint8_t *)os_malloc(40 * 1024);
			else
				dvp_camera_encode = (uint8_t *)os_malloc(80 * 1024);
		}
		else
		{
			if (mode == YUV_BUF_MODE_JPEG)
				dvp_camera_encode = (uint8_t *)os_malloc(20 * 1024);
			else
				dvp_camera_encode = (uint8_t *)os_malloc(40 * 1024);
		}

		if (dvp_camera_encode == NULL)
		{
			LOGE("yuv_buf base addr init failed!\n");
			return BK_FAIL;
		}
	}

	return BK_OK;
}
#endif

#ifdef DVP_STRIP
static uint32_t dvp_frame_sram_strip(uint8_t *src, uint32_t size)
{
	uint32_t i, first_zero = 0;
	uint32_t tail = size - 1;

	if (src[tail] != 0xD9
		&& src[tail - 1] != 0xFF)
	{
		if (dvp_camera_drv->index)
		{
			src = (dvp_camera_drv->buffer);
		}
		else
		{
			src = (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE);
		}
		if (src[tail] != 0xD9
		&& src[tail - 1] != 0xFF)
		{
			LOGE("strip tail error %u %u %u\n", curr_encode_frame->length, size, curr_encode_frame->sequence);
			return size;
		}
		else
		{
			tail -= 2;
		}
	}
	else
	{
		tail -= 2;
	}

	//jpeg_dump("before", src + size - 10, 10);

	for (i = tail; i > 0; i--)
	{
		if (src[i] == 0xFF)
		{
			tail--;
		}
		else if(src[i] == 0x00)
		{
			if (first_zero == 0)
			{
				if (src[i-1] == 0xFF)
				{
					tail--;
					first_zero ++;
				}
				else if (src[i-1] & 0x01)
				{
					tail--;
				}
				else
				{
					tail++;
					break;
				}
			}
			else
			{
				tail++;
				break;
			}
		}
		else
		{
			tail++;
			break;
		}
	}

	src[tail++] = 0xFF;
	src[tail++] = 0xD9;
	//jpeg_dump("after", src + tail - 10, 10);

	return tail;
}
#endif

#if CONFIG_YUV_BUF
static void dvp_camera_sensor_ppi_err_handler(yuv_buf_unit_t id, void *param)
{
	DVP_PPI_ERROR_ENTRY();

	if (!dvp_camera_drv->error)
	{
		dvp_camera_drv->error = true;
	}

	DVP_PPI_ERROR_OUT();
}

static void dvp_camera_reset_hardware_modules_handler(void)
{
	bk_yuv_buf_soft_reset();

	if (dvp_camera_dma_channel < DMA_ID_MAX)
	{
		bk_dma_stop(dvp_camera_dma_channel);
		if (curr_encode_frame)
		{
			curr_encode_frame->length = 0;
		}

		if (H264_TYPE_CONDITION(dvp_camera_device->type))
		{
			bk_yuv_buf_stop(YUV_BUF_MODE_H264);
			bk_yuv_buf_start(YUV_BUF_MODE_H264);
		}

		if (dvp_camera_device->type == MEDIA_DVP_MJPEG)
		{
			bk_jpeg_enc_soft_reset();
		}

		bk_dma_start(dvp_camera_dma_channel);
	}
}
#endif


static void dvp_camera_vsync_negedge_handler(jpeg_unit_t id, void *param)
{
	DVP_JPEG_VSYNC_ENTRY();

	if (dvp_state == MSTATE_TURNING_OFF)
	{
		bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);
		bk_jpeg_enc_set_enable(0, JPEG_YUV_MODE);
#if (CONFIG_YUV_BUF)
		bk_yuv_buf_stop(YUV_BUF_MODE_YUV);
		bk_yuv_buf_stop(YUV_BUF_MODE_JPEG);
		bk_yuv_buf_stop(YUV_BUF_MODE_H264);
#endif

		if (dvp_sema != NULL)
		{
			rtos_set_semaphore(&dvp_sema);
		}
	}

	if (dvp_camera_drv->error)
	{
		dvp_camera_drv->error = false;
#if (CONFIG_YUV_BUF)
		dvp_camera_reset_hardware_modules_handler();
		LOGI("reset OK \r\n");
#endif
		DVP_JPEG_VSYNC_OUT();
		return;
	}

#ifdef JPEG_DROP_SENSOR_FRAME

	if (g_vsync_index == 0)
	{
		g_vsync_index = 1;
		dvp_camera_config.drop_frame(1);
	}
	else
	{
		g_vsync_index = 0;
		dvp_camera_config.drop_frame(0);
	}
#endif

	DVP_JPEG_VSYNC_OUT();
}

#ifdef DVP_DIAG_DEBUG
void dvp_camera_dvp_start_handler(jpeg_unit_t id, void *param)
{
	DVP_JPEG_START_ENTRY();

	DVP_JPEG_START_OUT();
}

void dvp_camera_dvp_head_handler(jpeg_unit_t id, void *param)
{
	DVP_JPEG_HEAD_ENTRY();

	DVP_JPEG_HEAD_OUT();
}
#endif


#if (CONFIG_H264)
static void dvp_h264_eof_handler(void *param)
{
	uint32_t real_length = bk_h264_get_encode_count() * 4;
	uint32_t remain_length = 0;

	DVP_H264_EOF_ENTRY();

	if (dvp_camera_drv->error)
	{
		curr_encode_frame->length = 0;
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_start(dvp_camera_dma_channel);
		DVP_H264_EOF_OUT();
		return;
	}

	LOGD("real_length:%d, remain_length:%d\r\n", real_length, remain_length);
	media_debug->isr_jpeg++;

	if (dvp_camera_drv->drop_count)
	{
		dvp_camera_drv->drop_count--;
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_start(dvp_camera_dma_channel);
		DVP_H264_EOF_OUT();
		return;
	}

	if (dvp_camera_drv->cached)
	{
		if (curr_encode_frame == NULL
		    || curr_encode_frame->frame == NULL)
		{
			LOGE("curr_encode_frame NULL error\n");
			DVP_H264_EOF_OUT();
			goto error;
		}

		remain_length = FRAME_BUFFER_CACHE - bk_dma_get_remain_len(dvp_camera_dma_channel);

		if (curr_encode_frame->length + remain_length != real_length)
		{
			LOGW("size no match: %u:%u\n", curr_encode_frame->length + remain_length, real_length);
		}

		bk_dma_stop(dvp_camera_dma_channel);

		if (dvp_camera_drv->psram_dma_busy == true)
		{
			dvp_camera_drv->frame = curr_encode_frame;
			dvp_camera_drv->psram_dma_left = remain_length;
			dvp_camera_drv->eof = true;
		}
		else
		{
			dvp_camera_drv->frame = curr_encode_frame;
			dvp_camera_drv->psram_dma_left = 0;
			dvp_camera_drv->eof = true;

			dvp_memcpy_by_chnl(curr_encode_frame->frame + curr_encode_frame->length,
			                   dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer,
			                   remain_length, dvp_camera_drv->psram_dma);
			curr_encode_frame->length += remain_length;
			curr_encode_frame->sequence = ++sequence;
		}
	}
	else
	{
		remain_length = FRAME_BUFFER_DMA_TH - bk_dma_get_remain_len(dvp_camera_dma_channel);

		if (curr_encode_frame->length + remain_length != real_length)
		{
			LOGW("size no match: %u:%u\n", curr_encode_frame->length + remain_length, real_length);
		}

		curr_encode_frame->length += remain_length;
		curr_encode_frame->sequence = ++sequence;
		dvp_camera_config.fb_h264_complete(curr_encode_frame);
		curr_encode_frame = dvp_camera_config.fb_h264_malloc();
		curr_encode_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
		curr_encode_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
		curr_encode_frame->fmt = PIXEL_FMT_DVP_H264;

		if (curr_encode_frame == NULL
		    || curr_encode_frame->frame == NULL)
		{
			LOGE("alloc frame error\n");
			DVP_H264_EOF_OUT();
			return;
		}
		bk_dma_flush_src_buffer(dvp_camera_dma_channel);
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_set_dest_addr(dvp_camera_dma_channel, (uint32_t)curr_encode_frame->frame, (uint32_t)(curr_encode_frame->frame + curr_encode_frame->size));
		bk_dma_start(dvp_camera_dma_channel);

		if (dvp_camera_device->type == MEDIA_DVP_H264_ENC_LCD)
		{
			h264_enc_lcd_config->yuv_data_offset = 0;
			bk_dma_flush_src_buffer(h264_enc_lcd_config->dma_collect_yuv);
			dvp_camera_config.fb_display_complete(curr_yuv_frame);
			LOGD("push ! \r\n");
			curr_yuv_frame = dvp_camera_config.fb_display_malloc();
			if (curr_yuv_frame != NULL)
			{
				curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
				curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
				curr_yuv_frame->fmt = PIXEL_FMT_YUYV;
			}
			else
			{
				LOGE("%s malloc frame failed\n", __func__);
			}
		}
	}

	DVP_H264_EOF_OUT();

	return;

error:
	dvp_camera_drv->index = 0;
	bk_dma_stop(dvp_camera_dma_channel);
	bk_yuv_buf_stop(YUV_BUF_MODE_H264);

}

static void yuv_sm0_line_done(void *param)
{
	BK_WHILE(bk_dma_get_enable_status(h264_enc_lcd_config->dma_collect_yuv));
	bk_dma_stop(h264_enc_lcd_config->dma_collect_yuv);
	bk_dma_set_src_start_addr(h264_enc_lcd_config->dma_collect_yuv,
							  (uint32_t)h264_enc_lcd_config->yuv_em_addr);
	bk_dma_set_dest_start_addr(h264_enc_lcd_config->dma_collect_yuv,
							   (uint32_t)(curr_yuv_frame->frame + h264_enc_lcd_config->yuv_data_offset));
	bk_dma_start(h264_enc_lcd_config->dma_collect_yuv);
	h264_enc_lcd_config->yuv_data_offset += h264_enc_lcd_config->yuv_pingpong_length;
}

static void yuv_sm1_line_done(void *param)
{
	BK_WHILE(bk_dma_get_enable_status(h264_enc_lcd_config->dma_collect_yuv));
	bk_dma_stop(h264_enc_lcd_config->dma_collect_yuv);
	bk_dma_set_src_start_addr(h264_enc_lcd_config->dma_collect_yuv,
							  (uint32_t)h264_enc_lcd_config->yuv_em_addr + h264_enc_lcd_config->yuv_pingpong_length);
	bk_dma_set_dest_start_addr(h264_enc_lcd_config->dma_collect_yuv,
							   (uint32_t)(curr_yuv_frame->frame + h264_enc_lcd_config->yuv_data_offset));
	bk_dma_start(h264_enc_lcd_config->dma_collect_yuv);
	h264_enc_lcd_config->yuv_data_offset += h264_enc_lcd_config->yuv_pingpong_length;
}
#endif
static void dvp_camera_eof_handler(jpeg_unit_t id, void *param)
{
	uint32_t real_length = bk_jpeg_enc_get_frame_size();
	int32_t remain_length = FRAME_BUFFER_CACHE - bk_dma_get_remain_len(dvp_camera_dma_channel);
	int32_t left_length = remain_length - JPEG_CRC_SIZE;

	DVP_JPEG_EOF_ENTRY();

	if (dvp_camera_drv->error)
	{
		curr_encode_frame->length = 0;
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_start(dvp_camera_dma_channel);
		DVP_JPEG_EOF_OUT();
		return;
	}

	//LOGI("JPEG EOF\n");
	LOGD("real_length:%d, remain_length:%d, left_length:%d\r\n", real_length, remain_length, left_length);
	media_debug->isr_jpeg++;

	if (dvp_camera_drv->drop_count)
	{
		dvp_camera_drv->drop_count--;
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_start(dvp_camera_dma_channel);
		return;
	}

	if (dvp_camera_drv->cached)
	{
		if (curr_encode_frame == NULL
			|| curr_encode_frame->frame == NULL)
		{
			LOGE("curr_encode_frame NULL error\n");
			goto error;
		}

		if (curr_encode_frame->length + left_length != real_length)
		{
			LOGW("size no match: %u:%u:%u\n", curr_encode_frame->length + left_length, real_length, remain_length);
		}

#ifdef DVP_STRIP
		if (left_length > 0)
		{
			left_length = dvp_frame_sram_strip(dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer, left_length);
		}
		else
		{
			dvp_camera_drv->index = !dvp_camera_drv->index;
			left_length = dvp_frame_sram_strip(dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer, (curr_encode_frame->length + left_length)%FRAME_BUFFER_CACHE);
		}
#endif
		bk_dma_stop(dvp_camera_dma_channel);

		if ((dvp_camera_device->type == MEDIA_DVP_MIX)
			&& (dvp_state == MSTATE_TURN_ON))
		{
			curr_encode_frame->mix = true;

			bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);
#if (CONFIG_YUV_BUF)
			bk_yuv_buf_stop(YUV_BUF_MODE_JPEG);
#endif
			curr_yuv_frame = dvp_camera_config.fb_display_malloc();

			if (curr_yuv_frame == NULL)
			{
				LOGE("%s malloc curr_yuv_frame failed\n", __func__);
				goto error;
			}

			curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
			curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
			curr_yuv_frame->fmt = current_sensor->fmt;//PIXEL_FMT_YUYV

			LOGD("start yuv\n");
			jpeg_config.mode = JPEG_YUV_MODE;
			bk_jpeg_enc_mode_switch(&jpeg_config);
			bk_jpeg_set_em_base_addr(curr_yuv_frame->frame);
#if (CONFIG_YUV_BUF)
			bk_yuv_buf_set_em_base_addr((uint32_t)curr_yuv_frame->frame);
#endif
		}

		if (dvp_camera_drv->psram_dma_busy == true)
		{
			dvp_camera_drv->frame = curr_encode_frame;
			dvp_camera_drv->eof = true;
			if ((remain_length - JPEG_CRC_SIZE) > 0)
			{
				dvp_camera_drv->psram_dma_left = left_length;
			}
			else
			{
				if (curr_encode_frame->length >= FRAME_BUFFER_CACHE)
				{
					curr_encode_frame->length -= FRAME_BUFFER_CACHE;
				}
				dvp_camera_drv->psram_dma_left = left_length;
			}
		}
		else
		{
			dvp_camera_drv->frame = curr_encode_frame;
			dvp_camera_drv->psram_dma_left = 0;
			dvp_camera_drv->eof = true;

			dvp_memcpy_by_chnl(curr_encode_frame->frame + curr_encode_frame->length,
							   dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer,
							   left_length, dvp_camera_drv->psram_dma);
			curr_encode_frame->length += left_length;
			curr_encode_frame->sequence = ++sequence;
		}
	}
	else
	{
		remain_length = FRAME_BUFFER_DMA_TH - bk_dma_get_remain_len(dvp_camera_dma_channel);
		left_length = remain_length - JPEG_CRC_SIZE;

		if (curr_encode_frame->length + left_length != real_length)
		{
			LOGW("size no match: %u:%u:%u\n", curr_encode_frame->length + left_length, real_length, remain_length);
			LOGW("size: %u:%u:%u\n", bk_dma_get_remain_len(dvp_camera_dma_channel), bk_jpeg_enc_get_frame_size(), curr_encode_frame->size);
		}

		curr_encode_frame->length += left_length;
		dvp_camera_config.fb_jpeg_complete(curr_encode_frame);
		curr_encode_frame = dvp_camera_config.fb_jpeg_malloc();
		curr_encode_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
		curr_encode_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
		curr_encode_frame->fmt = PIXEL_FMT_DVP_JPEG;

		if (curr_encode_frame == NULL
			|| curr_encode_frame->frame == NULL)
		{
			LOGE("alloc frame error\n");
			return;
		}

		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_set_dest_addr(dvp_camera_dma_channel, (uint32_t)curr_encode_frame->frame, (uint32_t)(curr_encode_frame->frame + curr_encode_frame->size));
		bk_dma_start(dvp_camera_dma_channel);

	}

	DVP_JPEG_EOF_OUT();

	return;

error:
	dvp_camera_drv->index = 0;
	bk_dma_stop(dvp_camera_dma_channel);
	bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);
#if (CONFIG_YUV_BUF)
	bk_yuv_buf_stop(YUV_BUF_MODE_JPEG);
#endif
}

static void dvp_memcpy_finish_callback(dma_id_t id)
{
	dvp_camera_drv->psram_dma_busy = false;
	dvp_camera_drv->index = !dvp_camera_drv->index;

	if (dvp_camera_drv->eof == true)
	{
		frame_buffer_t *frame = dvp_camera_drv->frame;

		if (dvp_camera_drv->psram_dma_left != 0)
		{
			dvp_memcpy_by_chnl(frame->frame + frame->length,
			                   dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer,
			                   dvp_camera_drv->psram_dma_left, dvp_camera_drv->psram_dma);
			frame->length += dvp_camera_drv->psram_dma_left;
			frame->sequence = ++sequence;
			dvp_camera_drv->psram_dma_left = 0;
		}
		else
		{
			if(dvp_state == MSTATE_TURNING_OFF)
			{
				return;
			}

			if (H264_TYPE_CONDITION(dvp_camera_device->type))
				dvp_camera_config.fb_h264_complete(frame);
			else // JPEG || MIX
				dvp_camera_config.fb_jpeg_complete(frame);

			dvp_camera_drv->index = 0;
			dvp_camera_drv->frame = NULL;
			dvp_camera_drv->eof = false;

			if (dvp_state == MSTATE_TURN_ON)
			{
				if (H264_TYPE_CONDITION(dvp_camera_device->type))
				{
					curr_encode_frame = dvp_camera_config.fb_h264_malloc();
				}
				else if (dvp_camera_device->type == MEDIA_DVP_MJPEG)// JPEG
				{
					curr_encode_frame = dvp_camera_config.fb_jpeg_malloc();
				}

				if (curr_encode_frame == NULL
					    || curr_encode_frame->frame == NULL)
				{
					LOGE("alloc frame error\n");
					return;
				}

				curr_encode_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
				curr_encode_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);

				if (H264_TYPE_CONDITION(dvp_camera_device->type))
				{
					curr_encode_frame->fmt = PIXEL_FMT_DVP_H264;
				}
				else if (dvp_camera_device->type == MEDIA_DVP_MJPEG)// JPEG
				{
					curr_encode_frame->fmt = PIXEL_FMT_DVP_JPEG;
				}

				bk_dma_start(dvp_camera_dma_channel);
			}
			else
			{
				curr_encode_frame = NULL;
			}
		}
	}
}

bk_err_t dvp_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls)
{
	dma_config_t dma_config = {0};

	/* fix for psram 4bytes alignment */
	if (len % 4)
	{
		len = (len / 4 + 1) * 4;
	}

	os_memset(&dma_config, 0, sizeof(dma_config_t));

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 1;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.start_addr = (uint32_t)in;
	dma_config.src.end_addr = (uint32_t)(in + len);

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)out;
	dma_config.dst.end_addr = (uint32_t)(out + len);

	dvp_camera_drv->psram_dma_busy = true;

	BK_LOG_ON_ERR(bk_dma_init(cpy_chnls, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(cpy_chnls, len));
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(cpy_chnls, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(cpy_chnls, DMA_ATTR_SEC));
#endif
	BK_LOG_ON_ERR(bk_dma_register_isr(cpy_chnls, NULL, dvp_memcpy_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(cpy_chnls));
	BK_LOG_ON_ERR(bk_dma_start(cpy_chnls));
	//BK_WHILE (bk_dma_get_enable_status(cpy_chnl));

	return BK_OK;
}


static void dvp_camera_dma_finish_callback(dma_id_t id)
{
	DVP_JPEG_SDMA_ENTRY();

	if (dvp_camera_drv->cached == true)
	{
		if (curr_encode_frame == NULL
		    || curr_encode_frame->frame == NULL)
		{
			LOGE("%s curr_encode_frame NULL\n");
			return;
		}

		if (dvp_camera_drv->drop_count == 0)
		{
			dvp_memcpy_by_chnl(curr_encode_frame->frame + curr_encode_frame->length,
			                   dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer,
			                   FRAME_BUFFER_CACHE, dvp_camera_drv->psram_dma);
			curr_encode_frame->length += FRAME_BUFFER_CACHE;
		}
	}
	else
	{
		curr_encode_frame->length += FRAME_BUFFER_DMA_TH;
	}

	DVP_JPEG_SDMA_OUT();
}

static bk_err_t dvp_camera_dma_config(void)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config = {0};
	uint32_t encode_fifo_addr;

	if (H264_TYPE_CONDITION(dvp_camera_device->type))
	{
		curr_encode_frame = dvp_camera_config.fb_h264_malloc();
	}
	else // MJPEG || MIX
	{
		curr_encode_frame = dvp_camera_config.fb_jpeg_malloc();
	}

	if (curr_encode_frame == NULL)
	{
		LOGE("malloc frame fail \r\n");
		ret = BK_FAIL;
		return ret;
	}
	curr_encode_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
	curr_encode_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);

	if (H264_TYPE_CONDITION(dvp_camera_device->type))
	{
#if (CONFIG_H264)
		bk_h264_get_fifo_addr(&encode_fifo_addr);
		curr_encode_frame->fmt = PIXEL_FMT_DVP_H264;
		dvp_camera_dma_channel = bk_dma_alloc(DMA_DEV_H264);
#endif
	}
	else // JPEG || MIX
	{
		bk_jpeg_enc_get_fifo_addr(&encode_fifo_addr);
		curr_encode_frame->fmt = PIXEL_FMT_DVP_JPEG;
		dvp_camera_dma_channel = bk_dma_alloc(DMA_DEV_JPEG);
	}

	if ((dvp_camera_dma_channel < DMA_ID_0) || (dvp_camera_dma_channel >= DMA_ID_MAX))
	{
		LOGE("malloc dma fail \r\n");
		ret = BK_FAIL;
		return ret;
	}
	LOGI("dvp_dma id:%d \r\n", dvp_camera_dma_channel);

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 0;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = encode_fifo_addr;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;

	if (H264_TYPE_CONDITION(dvp_camera_device->type))
	{
#ifdef CONFIG_H264
		dma_config.src.dev = DMA_DEV_H264;
		BK_LOG_ON_ERR(bk_dma_set_src_burst_len(dvp_camera_dma_channel, 0));
		BK_LOG_ON_ERR(bk_dma_set_dest_burst_len(dvp_camera_dma_channel, 3));
#endif
	}
	else // JPEG || MIX
	{
		dma_config.src.dev = DMA_DEV_JPEG;
	}

	if (dvp_camera_drv->cached)
	{
		dma_config.dst.start_addr = (uint32_t)dvp_camera_drv->buffer;
		dma_config.dst.end_addr = (uint32_t)(dvp_camera_drv->buffer + FRAME_BUFFER_CACHE * 2);
	}
	else
	{
		dma_config.dst.start_addr = (uint32_t)curr_encode_frame->frame;
		dma_config.dst.end_addr = (uint32_t)(curr_encode_frame->frame + curr_encode_frame->size);
	}

	BK_LOG_ON_ERR(bk_dma_init(dvp_camera_dma_channel, &dma_config));

	if (dvp_camera_drv->cached)
	{
		BK_LOG_ON_ERR(bk_dma_set_transfer_len(dvp_camera_dma_channel, FRAME_BUFFER_CACHE));
	}
	else
	{
		BK_LOG_ON_ERR(bk_dma_set_transfer_len(dvp_camera_dma_channel, FRAME_BUFFER_DMA_TH));
	}

	BK_LOG_ON_ERR(bk_dma_register_isr(dvp_camera_dma_channel, NULL, dvp_camera_dma_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dvp_camera_dma_channel));
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(dvp_camera_dma_channel, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(dvp_camera_dma_channel, DMA_ATTR_SEC));
#endif
	BK_LOG_ON_ERR(bk_dma_start(dvp_camera_dma_channel));

	return ret;
}

static void dvp_camera_yuv_eof_handler(jpeg_unit_t id, void *param)
{
	/*there have a asic bug, work in yuv mode use vsync negedge int as yuv eof, need reset all video modules*/
	if (dvp_camera_drv->error)
	{
		return;
	}

	frame_buffer_t *frame = NULL;

	DVP_YUV_EOF_ENTRY();

#if (CONFIG_YUV_BUF)
	if (dvp_state == MSTATE_TURNING_OFF)
	{
		bk_yuv_buf_stop(YUV_BUF_MODE_YUV);
		bk_yuv_buf_stop(YUV_BUF_MODE_JPEG);
		bk_yuv_buf_stop(YUV_BUF_MODE_H264);

		if (dvp_sema != NULL)
		{
			rtos_set_semaphore(&dvp_sema);
		}

		return;
	}
#endif

	media_debug->isr_jpeg++;

	curr_yuv_frame->sequence = ++sequence;

	if (dvp_camera_device)
	{
		curr_yuv_frame->length = dvp_camera_device->pixel_size;
	}

#if YUV_PARTIAL_DEBUG
	curr_yuv_frame->width = 640;
	curr_yuv_frame->height = 480;
#endif

	dvp_camera_config.fb_display_complete(curr_yuv_frame);
	curr_yuv_frame = NULL;

	if (dvp_state != MSTATE_TURNING_OFF)
	{
		/*bk_jpeg_enc_set_enable(0, JPEG_YUV_MODE);
		bk_jpeg_enc_disable_int(JPEG_END_YUV_INT);

		if (dvp_sema != NULL)
		{
		    rtos_set_semaphore(&dvp_sema);
		}

		curr_yuv_frame = NULL;
		}
		else
		{*/
		if (dvp_camera_device->type == MEDIA_DVP_MIX)
		{
			bk_jpeg_enc_set_enable(0, JPEG_YUV_MODE);
#if (CONFIG_YUV_BUF)
			bk_yuv_buf_stop(YUV_BUF_MODE_YUV);
#endif
			curr_encode_frame = dvp_camera_config.fb_jpeg_malloc();

			if (curr_encode_frame == NULL
			    || curr_encode_frame->frame == NULL)
			{
				LOGE("alloc frame error\n");
				return;
			}

			curr_encode_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
			curr_encode_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
			curr_encode_frame->fmt = PIXEL_FMT_DVP_JPEG;

			bk_dma_start(dvp_camera_dma_channel);

			LOGD("start jpeg\n");

			jpeg_config.mode = JPEG_ENC_MODE;
			bk_jpeg_enc_mode_switch(&jpeg_config);
		}
		else
		{
			frame = dvp_camera_config.fb_display_malloc();

			if (frame != NULL)
			{
				curr_yuv_frame = frame;
				curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
				curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
				curr_yuv_frame->fmt = current_sensor->fmt;
				bk_jpeg_set_em_base_addr(curr_yuv_frame->frame);
#if (CONFIG_YUV_BUF)
				bk_yuv_buf_set_em_base_addr((uint32_t)curr_yuv_frame->frame);
#endif
			}
			else
			{
				LOGE("%s malloc frame failed\n", __func__);
			}
		}
	}

	DVP_YUV_EOF_OUT();
}

bk_err_t bk_dvp_camera_read_register_enable(bool enable)
{
	camera_read_flag = enable;

	return kNoErr;
}


bk_err_t bk_dvp_camera_gpio_init(const dvp_camera_config_t *config, uint8_t mode)
{
	if (mode == 1)
	{
		IO_FUNCTION_ENABLE(CAMERA_DVP_MCLK_PIN, CAMERA_DVP_MCLK_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PCLK_PIN, CAMERA_DVP_PCLK_FUNC);
		bk_gpio_set_capacity(CAMERA_DVP_MCLK_PIN, 3);
		gpio_dev_unmap(CAMERA_DVP_HSYNC_PIN);
		gpio_dev_unmap(CAMERA_DVP_VSYNC_PIN);
	}

	if (mode == 2)
	{
		IO_FUNCTION_ENABLE(CAMERA_DVP_HSYNC_PIN, CAMERA_DVP_HSYNC_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_VSYNC_PIN, CAMERA_DVP_VSYNC_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA0_PIN, CAMERA_DVP_PXDATA0_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA1_PIN, CAMERA_DVP_PXDATA1_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA2_PIN, CAMERA_DVP_PXDATA2_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA3_PIN, CAMERA_DVP_PXDATA3_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA4_PIN, CAMERA_DVP_PXDATA4_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA5_PIN, CAMERA_DVP_PXDATA5_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA6_PIN, CAMERA_DVP_PXDATA6_FUNC);
		IO_FUNCTION_ENABLE(CAMERA_DVP_PXDATA7_PIN, CAMERA_DVP_PXDATA7_FUNC);
	}

	return 0;
}

bk_err_t bk_dvp_camera_power_enable(uint8_t enable)
{
#if (CONFIG_CAMERA_POWER_GPIO_CTRL)

	if (enable)
	{
		gpio_dev_unmap(CAMERA_LDO_CTRL_GPIO);
		bk_gpio_set_capacity(CAMERA_LDO_CTRL_GPIO, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(CAMERA_LDO_CTRL_GPIO));
		BK_LOG_ON_ERR(bk_gpio_enable_output(CAMERA_LDO_CTRL_GPIO));

#if (CAMERA_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_set_output_high(CAMERA_LDO_CTRL_GPIO);
#else
		bk_gpio_set_output_low(CAMERA_LDO_CTRL_GPIO);
#endif
	}
	else
	{
#if (CAMERA_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_set_output_low(CAMERA_LDO_CTRL_GPIO);
#else
		bk_gpio_set_output_high(CAMERA_LDO_CTRL_GPIO);
#endif
	}

	delay_ms(5);

#endif

	return BK_OK;
}

static bk_err_t dvp_camera_enable_mclk(media_camera_type_t type)
{
	int ret = 0;
#if (CONFIG_YUV_BUF)
	uint32_t video_work_mode = 0;
	switch (type) {
	case MEDIA_DVP_MJPEG:
		 video_work_mode = YUV_BUF_MODE_JPEG;
		 break;
	case MEDIA_DVP_H264_WIFI_TRANSFER:
	case MEDIA_DVP_H264_LOCAL:
	case MEDIA_DVP_H264_USB_TRANSFER:
	case MEDIA_DVP_H264_ENC_LCD:
		 video_work_mode = YUV_BUF_MODE_H264;
		 break;
	default:
		 video_work_mode = YUV_BUF_MODE_YUV;
		 break;
	}
	ret = bk_yuv_buf_enable_mclk(video_work_mode);
#else
	bk_jpeg_enc_mclk_enable();
#endif
	return ret;
}

static bk_err_t dvp_camera_init_device(dvp_camera_config_t *config)
{
	int ret = 0;

 	if (dvp_camera_device == NULL) {
		dvp_camera_device = (dvp_camera_device_t *)os_malloc(sizeof(dvp_camera_device_t));
		if (dvp_camera_device == NULL) {
			LOGE("dvp_camera_device malloc failed\n");
			return BK_ERR_NO_MEM;
		}
	}
	os_memset(dvp_camera_device, 0, sizeof(dvp_camera_device_t));
	dvp_camera_device->type = config->type;
	dvp_camera_device->fps = current_sensor->def_fps;
	dvp_camera_device->name = current_sensor->name;
	dvp_camera_device->id = current_sensor->id;
	dvp_camera_device->fps_cap = current_sensor->fps_cap;
	dvp_camera_device->ppi_cap = current_sensor->ppi_cap;
	if ((config->ppi != dvp_camera_device->ppi) &&
		(pixel_ppi_to_cap(config->ppi) & (dvp_camera_device->ppi_cap))) {
		dvp_camera_device->ppi = config->ppi;
		LOGI("%s switch ppi to %dX%d\n", __func__, config->ppi >> 16, config->ppi & 0xFFFF);
	} else {
		dvp_camera_device->ppi = current_sensor->def_ppi;
	}
	dvp_camera_device->pixel_size = get_ppi_size(dvp_camera_device->ppi);
	LOGI("pixel size: %d\n", dvp_camera_device->pixel_size);
	LOGI("x_pixel:%d, y_pixel:%d\r\n", ppi_to_pixel_x(dvp_camera_device->ppi), ppi_to_pixel_y(dvp_camera_device->ppi));

	return ret;
}

static bk_err_t dvp_camera_init(dvp_camera_config_t *config)
{
	i2c_config_t i2c_config = {0};

#if CONFIG_EXTERN_32K
	pm_clk_32k_source_switch(PM_LPO_SRC_DIVD);
#endif
	/* enbale dvp power */
	bk_dvp_camera_power_enable(1);
	/* enable mclk for i2c communicate with dvp */
	BK_RETURN_ON_ERR(dvp_camera_enable_mclk(config->type));
	/* map gpio as MCLK, PCLK for i2c communicate with dvp */
	bk_dvp_camera_gpio_init(&dvp_camera_config, 1);

	/* init i2c */
	i2c_config.baud_rate = CAMERA_DVP_I2C_BAUD_RATE;
	i2c_config.addr_mode = CAMERA_DVP_I2C_MODE;
	BK_RETURN_ON_ERR(bk_i2c_init(CAMERA_DVP_I2C_ID, &i2c_config));
	/* auto detect sensor */
	current_sensor = bk_dvp_get_sensor_auto_detect();
	if (current_sensor == NULL) {
		LOGE("%s no dvp camera found\n", __func__);
		return BK_ERR_NOT_FOUND;
	}
	LOGI("auto detect success, dvp camera name:%s\r\n", current_sensor->name);
	LOGI("sensor_clk:%d\r\n", current_sensor->clk);

#ifdef DVP_DIAG_DEBUG
	//bk_dvp_camera_read_register_enable(true);
#endif
	current_sensor->read_register(camera_read_flag);

	/* set current used camera config */
	BK_RETURN_ON_ERR(dvp_camera_init_device(config));

	return BK_OK;
}

bk_err_t bk_dvp_camera_driver_init(dvp_camera_config_t *config)
{
	int ret = BK_OK;

#ifdef JPEG_DROP_SENSOR_FRAME
	g_vsync_index = 0;
#endif

#if (CONFIG_PSRAM && (!CONFIG_SLAVE_CORE))
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_320M);
	bk_psram_init();
#endif

	os_memcpy(&dvp_camera_config, config, sizeof(dvp_camera_config_t));

#if (CONFIG_SOC_BK7236XX)
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN, PM_POWER_MODULE_STATE_ON);
#endif

	ret = dvp_camera_init(config);
	if (ret) {
		LOGE("not find sensor, please check!\r\n");
		goto error;
	}

	jpeg_config.x_pixel = ppi_to_pixel_x(dvp_camera_device->ppi) / 8;
	jpeg_config.y_pixel = ppi_to_pixel_y(dvp_camera_device->ppi) / 8;
	jpeg_config.vsync = current_sensor->vsync;
	jpeg_config.hsync = current_sensor->hsync;
	jpeg_config.clk = current_sensor->clk;

	switch (current_sensor->fmt)
	{
		case PIXEL_FMT_YUYV:
			jpeg_config.sensor_fmt = JPEG_YUYV;
			break;

		case PIXEL_FMT_UYVY:
			jpeg_config.sensor_fmt = JPEG_UYVY;
			break;

		case PIXEL_FMT_YYUV:
			jpeg_config.sensor_fmt = JPEG_YYUV;
			break;

		case PIXEL_FMT_UVYY:
			jpeg_config.sensor_fmt = JPEG_UVYY;
			break;

		default:
			LOGE("JPEG MODULE not support this sensor input format\r\n");
			ret = kParamErr;
			goto error;;
	}

	if (dvp_camera_drv == NULL) {
		dvp_camera_drv = (dvp_camera_drv_t *)os_malloc(sizeof(dvp_camera_drv_t));

		if (dvp_camera_drv == NULL) {
			LOGE("dvp_camera_drv malloc failed\n");
			goto error;
		}

		os_memset(dvp_camera_drv, 0, sizeof(dvp_camera_drv_t));

		dvp_camera_drv->cached = true;
		//dvp_camera_drv->cached = false;
	}

	if (config->type != MEDIA_DVP_YUV)
	{
		if (dvp_camera_drv->cached)
		{
			dvp_camera_drv->buffer = (uint8 *)os_malloc(FRAME_BUFFER_CACHE * 2);
			os_memset(dvp_camera_drv->buffer, 0, FRAME_BUFFER_CACHE * 2);

			if (dvp_camera_drv->buffer == NULL)
			{
				LOGE("dvp_camera_drv malloc failed\n");
				goto error;
			}

			dvp_camera_drv->psram_dma = bk_dma_alloc(DMA_DEV_DTCM);
			if ((dvp_camera_drv->psram_dma < DMA_ID_0) || (dvp_camera_drv->psram_dma >= DMA_ID_MAX))
			{
				LOGE("malloc dvp_camera_drv->psram_dma fail \r\n");
				ret = BK_FAIL;
				goto error;
			}

			LOGI("dvp_camera_drv->psram_dma:%d \r\n", dvp_camera_drv->psram_dma);
		}
	}

	if (config->type == MEDIA_DVP_MJPEG)
	{
		jpeg_config.mode = JPEG_ENC_MODE;

		config->fb_jpeg_init(dvp_camera_device->ppi);

		ret = dvp_camera_dma_config();

		if (ret != BK_OK)
		{
			LOGE("dma init failed\n");
			goto error;
		}
#if CONFIG_YUV_BUF

		yuv_buf_config_t yuv_mode_config = {0};

		yuv_mode_config.work_mode = YUV_BUF_MODE_JPEG;
		yuv_mode_config.mclk_div = YUV_MCLK_DIV_3;
		yuv_mode_config.x_pixel = jpeg_config.x_pixel;
		yuv_mode_config.y_pixel = jpeg_config.y_pixel;
		yuv_mode_config.yuv_mode_cfg.yuv_format = YUV_FORMAT_YUYV;

#if (CONFIG_ENCODE_BUF_DYNAMIC)
		ret = dvp_camera_yuv_base_addr_init(dvp_camera_config.ppi, YUV_BUF_MODE_JPEG);
		if (ret != BK_OK)
		{
			goto error;
		}

		yuv_mode_config.base_addr = dvp_camera_encode;
#endif

		ret = bk_yuv_buf_init(&yuv_mode_config);
		if (ret != BK_OK) {
			LOGE("yuv_buf jpeg mode init error\n");
			goto error;
		}

		bk_yuv_buf_set_video_module_clk(current_sensor->clk);
		bk_yuv_buf_start(YUV_BUF_MODE_JPEG);
#endif

		ret = bk_jpeg_enc_init(&jpeg_config);
		if (ret != BK_OK)
		{
			LOGE("jpeg init error\n");
			goto error;
		}

		bk_jpeg_enc_register_isr(JPEG_EOF, dvp_camera_eof_handler, NULL);
#if (CONFIG_YUV_BUF)
		bk_jpeg_enc_register_isr(JPEG_FRAME_ERR, dvp_camera_sensor_ppi_err_handler, NULL);
#endif

#ifdef DVP_DIAG_DEBUG
		bk_jpeg_enc_register_isr(JPEG_SOF, dvp_camera_dvp_start_handler, NULL);
		bk_jpeg_enc_register_isr(JPEG_HEAD_OUTPUT, dvp_camera_dvp_head_handler, NULL);
#endif
	}
	else if (config->type == MEDIA_DVP_YUV)
	{
#if CONFIG_YUV_BUF
		yuv_buf_config_t yuv_mode_config = {0};

		yuv_mode_config.work_mode = YUV_BUF_MODE_YUV;
		yuv_mode_config.mclk_div = YUV_MCLK_DIV_3;
		yuv_mode_config.x_pixel = jpeg_config.x_pixel;
		yuv_mode_config.y_pixel = jpeg_config.y_pixel;
		yuv_mode_config.yuv_mode_cfg.yuv_format = YUV_FORMAT_YUYV;

		ret = bk_yuv_buf_init(&yuv_mode_config);
		if (ret != BK_OK) {
			LOGE("yuv_buf yuv mode init error\n");
			goto error;
		}
#if YUV_PARTIAL_DEBUG
		yuv_buf_partial_offset_config_t offset_config = {0};
		offset_config.x_partial_offset_l = 1;
		offset_config.x_partial_offset_r = 640;
		offset_config.y_partial_offset_l = 1;
		offset_config.y_partial_offset_r = 480;
		bk_yuv_buf_init_partial_display(&offset_config);
#endif
		bk_yuv_buf_set_video_module_clk(current_sensor->clk);
		bk_yuv_buf_start(YUV_BUF_MODE_YUV);
#else
		jpeg_config.mode = JPEG_YUV_MODE;
		ret = bk_jpeg_enc_init(&jpeg_config);
		if (ret != BK_OK)
		{
			LOGE("jpeg init error\n");
			goto error;
		}
#endif
		config->fb_display_init(dvp_camera_device->ppi);

		curr_yuv_frame = dvp_camera_config.fb_display_malloc();

		if (curr_yuv_frame == NULL)
		{
			LOGE("malloc frame fail \r\n");
			ret = BK_FAIL;
			goto error;
		}

		curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
		curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
		curr_yuv_frame->fmt = current_sensor->fmt;
#if (CONFIG_YUV_BUF)
		bk_yuv_buf_set_em_base_addr((uint32_t)curr_yuv_frame->frame);
		bk_yuv_buf_register_isr(YUV_BUF_VSYNC_NEGEDGE, dvp_camera_yuv_eof_handler, NULL);
#else
		bk_jpeg_enc_register_isr(JPEG_EOY, dvp_camera_yuv_eof_handler, NULL);
		bk_jpeg_set_em_base_addr(curr_yuv_frame->frame);
#endif
	}
	else if (config->type == MEDIA_DVP_MIX)
	{
		config->fb_jpeg_init(dvp_camera_device->ppi);
		config->fb_display_init(dvp_camera_device->ppi);

		ret = dvp_camera_dma_config();

		if (ret != BK_OK)
		{
			LOGE("dma init failed\n");
			goto error;
		}

		jpeg_config.mode = JPEG_ENC_MODE;

		bk_jpeg_enc_init(&jpeg_config);

		bk_jpeg_enc_register_isr(JPEG_EOF, dvp_camera_eof_handler, NULL);
		bk_jpeg_enc_register_isr(JPEG_EOY, dvp_camera_yuv_eof_handler, NULL);

#ifdef DVP_DIAG_DEBUG

		bk_jpeg_enc_register_isr(JPEG_SOF, dvp_camera_dvp_start_handler, NULL);
		bk_jpeg_enc_register_isr(JPEG_HEAD_OUTPUT, dvp_camera_dvp_head_handler, NULL);

#endif
	}
	else if (H264_TYPE_CONDITION(config->type))
	{
		LOGI("h264 dvp init! %d\r\n", dvp_camera_device->ppi);
#if (CONFIG_H264)

		yuv_buf_config_t yuv_mode_config = {0};

		//bk_h264_clk_check();
		bk_h264_driver_init();
		dvp_camera_drv->cached = false;
		yuv_mode_config.work_mode = YUV_BUF_MODE_H264;
		yuv_mode_config.x_pixel = jpeg_config.x_pixel;
		yuv_mode_config.y_pixel = jpeg_config.y_pixel;
		yuv_mode_config.yuv_mode_cfg.yuv_format = YUV_FORMAT_YUYV;
		LOGI("%d-%d\r\n", yuv_mode_config.x_pixel, yuv_mode_config.y_pixel);

#if (CONFIG_ENCODE_BUF_DYNAMIC)
		ret = dvp_camera_yuv_base_addr_init(dvp_camera_config.ppi, YUV_BUF_MODE_H264);
		if (ret != BK_OK)
		{
			goto error;
		}

		yuv_mode_config.base_addr = dvp_camera_encode;
#else
		if (dvp_camera_device->ppi >= PPI_1280X720)
		{
			bk_yuv_buf_set_em_base_addr(YUV_BUFFER_BASE_ADDR);
		}
#endif

		ret = bk_yuv_buf_init(&yuv_mode_config);
		if (ret != BK_OK)
		{
			LOGE("yuv_buf yuv mode init error\n");
			goto error;
		}

		bk_h264_init(dvp_camera_device->ppi);

		//bk_yuv_buf_set_video_module_clk(JPEG_80M_MCLK_20M);
		bk_yuv_buf_set_video_module_clk(JPEG_120M_MCLK_20M);

		config->fb_h264_init(dvp_camera_device->ppi);

		ret = dvp_camera_dma_config();

		if (ret != BK_OK)
		{
			LOGE("dma init failed\n");
			goto error;
		}

		if (config->type == MEDIA_DVP_H264_ENC_LCD)
		{
			os_printf("dvp h264 enc lcd !! \r\n");
			config->fb_display_init(dvp_camera_device->ppi);
			curr_yuv_frame = config->fb_display_malloc();
			curr_yuv_frame->fmt = PIXEL_FMT_YUYV;
			if(curr_yuv_frame == NULL)
			{
				LOGE("yuv_frame malloc failed!\r\n");
				goto error;
			}
			curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
			curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);

			if (h264_enc_lcd_config == NULL)
			{
				h264_enc_lcd_config = (h264_enc_lcd_config_t *)os_malloc(sizeof(h264_enc_lcd_config_t));
				if (h264_enc_lcd_config == NULL)
				{
					LOGE("h264 enc lcd malloc error! \r\n");
					goto error;
				}
			}

			h264_enc_lcd_config->yuv_em_addr = bk_yuv_buf_get_em_base_addr();
			LOGI("yuv buffer base addr:%08x\r\n", h264_enc_lcd_config->yuv_em_addr);
			// h264_enc_lcd_config->yuv_em_addr = 0x28060000;
			// bk_yuv_buf_set_em_base_addr(0x28060000);
			h264_enc_lcd_config->dma_collect_yuv = bk_dma_alloc(DMA_DEV_DTCM);
			h264_enc_lcd_config->yuv_pingpong_length = ppi_to_pixel_x(dvp_camera_device->ppi) * 32;
			h264_enc_lcd_config->yuv_data_offset = 0;
			LOGI("dma_collect_yuv id is %d \r\n", h264_enc_lcd_config->dma_collect_yuv);

			bk_h264_enc_lcd_dma_cpy(curr_yuv_frame->frame,
							   (uint32_t *)h264_enc_lcd_config->yuv_em_addr,
							   h264_enc_lcd_config->yuv_pingpong_length,
							   h264_enc_lcd_config->dma_collect_yuv);

			bk_yuv_buf_register_isr(YUV_BUF_SM0_WR, (yuv_buf_isr_t)yuv_sm0_line_done, NULL);
			bk_yuv_buf_register_isr(YUV_BUF_SM1_WR, (yuv_buf_isr_t)yuv_sm1_line_done, NULL);
		}

		if(config->type == MEDIA_DVP_H264_USB_TRANSFER)
		{
#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
			usbd_video_h264_init();
			delay_ms(1000);
#endif
		}

		bk_h264_register_isr(H264_FINAL_OUT, (h264_isr_t)dvp_h264_eof_handler, NULL);
		bk_yuv_buf_start(YUV_BUF_MODE_H264);
#endif
	}

#if CONFIG_YUV_BUF
	/* MPW2 yuv_arv_int cannot work, regard the VSYNC_NEGEDGE_INT as the YUV_ARV_INT */
	if (H264_TYPE_CONDITION(config->type) || config->type == MEDIA_DVP_MJPEG)
	{
		bk_yuv_buf_register_isr(YUV_BUF_VSYNC_NEGEDGE, dvp_camera_vsync_negedge_handler, NULL);
	}

	bk_yuv_buf_register_isr(YUV_BUF_SEN_RESL, dvp_camera_sensor_ppi_err_handler, NULL);
	bk_yuv_buf_register_isr(YUV_BUF_FULL, dvp_camera_sensor_ppi_err_handler, NULL);
	bk_yuv_buf_register_isr(YUV_BUF_H264_ERR, dvp_camera_sensor_ppi_err_handler, NULL);
	bk_yuv_buf_register_isr(YUV_BUF_ENC_SLOW, dvp_camera_sensor_ppi_err_handler, NULL);
#else
	bk_jpeg_enc_register_isr(JPEG_VSYNC_NEGEDGE, dvp_camera_vsync_negedge_handler, NULL);
#endif

	dvp_state = MSTATE_TURN_ON;

	current_sensor->init();
	current_sensor->set_ppi(dvp_camera_device->ppi);
	current_sensor->set_fps(dvp_camera_device->fps);

#if CONFIG_YUV_BUF
	BK_LOG_ON_ERR(bk_gpio_enable_input(CAMERA_DVP_VSYNC_PIN));
	/* wait for camera vsync to low level, then enable gpio vsync function */
	BK_WHILE(bk_gpio_get_input(CAMERA_DVP_VSYNC_PIN));
#endif

	uint32_t int_level = rtos_disable_int();
	bk_dvp_camera_gpio_init(&dvp_camera_config, 2);

#if (CONFIG_H264)
	if (H264_TYPE_CONDITION(dvp_camera_device->type))
	{
		bk_h264_encode_enable();
	}

	if(config->type == MEDIA_DVP_H264_USB_TRANSFER)
	{
#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
		demo_h264_usb_init();
#endif
	}
#endif

	media_debug->isr_jpeg = 0;

	DVP_DIAG_DEBUG_INIT();

	LOGI("dvp camera init complete with mode: %d\n", config->type);
	rtos_enable_int(int_level);

	return ret;

error:

	if (dvp_camera_dma_channel < DMA_ID_MAX)
	{
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_deinit(dvp_camera_dma_channel);
		bk_dma_free(DMA_DEV_JPEG, dvp_camera_dma_channel);
		dvp_camera_dma_channel = DMA_ID_MAX;
	}

	if (dvp_camera_drv)
	{
		if (dvp_camera_drv->buffer)
		{
			os_free(dvp_camera_drv->buffer);
			dvp_camera_drv->buffer = NULL;
		}

		if (dvp_camera_drv->psram_dma < DMA_ID_MAX)
		{
			bk_dma_stop(dvp_camera_drv->psram_dma);
			bk_dma_deinit(dvp_camera_drv->psram_dma);
			bk_dma_free(DMA_DEV_DTCM, dvp_camera_drv->psram_dma);
		}

		os_free(dvp_camera_drv);
		dvp_camera_drv = NULL;
	}

	if (dvp_camera_device)
	{
		os_free(dvp_camera_device);
		dvp_camera_device = NULL;
	}

	bk_i2c_deinit(CAMERA_DVP_I2C_ID);

	bk_jpeg_enc_deinit();

#if (CONFIG_PSRAM && (!CONFIG_SLAVE_CORE))
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_DEFAULT);
#endif

	bk_dvp_camera_power_enable(0);

#if CONFIG_EXTERN_32K
	pm_clk_32k_source_switch(PM_LPO_SRC_X32K);
#endif

	LOGI("dvp camera init failed\n");

	dvp_state = MSTATE_TURNING_OFF;

	return ret;
}


dvp_camera_device_t *bk_dvp_camera_get_device(void)
{
	return dvp_camera_device;
}

bk_err_t bk_dvp_camera_driver_deinit(void)
{
	LOGI("dvp camera deinit\n");
	GLOBAL_INT_DECLARATION();

	int ret = rtos_init_semaphore(&dvp_sema, 1);
	if (ret != kNoErr)
	{
		LOGE("jpeg sema not init ok\r\n");
		return ret;
	}

	GLOBAL_INT_DISABLE();
	dvp_state = MSTATE_TURNING_OFF;
	GLOBAL_INT_RESTORE();

	LOGI("%s wait dvp_sema\n", __func__);
	ret = rtos_get_semaphore(&dvp_sema, 200);
	if (ret != kNoErr)
	{
		LOGI("Not wait yuv vsync negedge!\r\n");
	}

	bk_jpeg_enc_deinit();

	bk_dma_stop(dvp_camera_dma_channel);
	bk_dma_deinit(dvp_camera_dma_channel);

	if (H264_TYPE_CONDITION(dvp_camera_device->type))
	{
		bk_dma_free(DMA_DEV_H264, dvp_camera_dma_channel);
		if(dvp_camera_device->type == MEDIA_DVP_H264_ENC_LCD)
		{
			bk_dma_stop(h264_enc_lcd_config->dma_collect_yuv);
			bk_dma_deinit(h264_enc_lcd_config->dma_collect_yuv);
			bk_dma_free(DMA_DEV_DTCM, h264_enc_lcd_config->dma_collect_yuv);
		}
	}
	else //JPEG || MIX
	{
		bk_dma_free(DMA_DEV_JPEG, dvp_camera_dma_channel);
	}

#if CONFIG_YUV_BUF
	bk_yuv_buf_deinit();
#endif

	bk_i2c_deinit(CAMERA_DVP_I2C_ID);

#if (CONFIG_H264)
	bk_h264_driver_deinit();
#endif

#if (CONFIG_PSRAM && (!CONFIG_SLAVE_CORE))
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_DEFAULT);
#endif

	bk_dvp_camera_power_enable(0);

#if CONFIG_EXTERN_32K
	pm_clk_32k_source_switch(PM_LPO_SRC_X32K);
#endif

	if (curr_encode_frame)
	{
		if (H264_TYPE_CONDITION(dvp_camera_device->type))
			dvp_camera_config.fb_h264_free(curr_encode_frame);
		else // JPEG || MIX
			dvp_camera_config.fb_jpeg_free(curr_encode_frame);

		curr_encode_frame = NULL;
	}

	if (curr_yuv_frame)
	{
		dvp_camera_config.fb_display_free(curr_yuv_frame);
		curr_yuv_frame = NULL;
	}

	if (dvp_camera_device)
	{
		os_free(dvp_camera_device);
		dvp_camera_device = NULL;
	}

	if (h264_enc_lcd_config)
	{
		os_free(h264_enc_lcd_config);
		h264_enc_lcd_config = NULL;
	}

	if (dvp_camera_drv)
	{
		bk_dma_stop(dvp_camera_drv->psram_dma);
		bk_dma_deinit(dvp_camera_drv->psram_dma);
		bk_dma_free(DMA_DEV_DTCM, dvp_camera_drv->psram_dma);

		if (dvp_camera_drv->buffer)
		{
			os_free(dvp_camera_drv->buffer);
			dvp_camera_drv->buffer = NULL;
		}

		os_free(dvp_camera_drv);
		dvp_camera_drv = NULL;
	}

	current_sensor = NULL;
	os_memset(&dvp_camera_config, 0, sizeof(dvp_camera_config_t));
	dvp_camera_dma_channel = DMA_ID_MAX;

	rtos_deinit_semaphore(&dvp_sema);
	dvp_sema = NULL;

	dvp_state = MSTATE_TURN_OFF;

	LOGI("dvp camera deinit complete\n");
	return kNoErr;
}

bk_err_t bk_dvp_camera_free_encode_mem(void)
{
	if (dvp_camera_encode)
	{
		if (dvp_state == MSTATE_TURN_OFF)
		{
			os_free(dvp_camera_encode);
			dvp_camera_encode = NULL;
			return BK_OK;
		}
		else
		{
			LOGI("camera is working, can not free\r\n");
			return BK_FAIL;
		}
	}
	else
	{
		LOGI("camera not malloc encode mem, can not free\r\n");
		return BK_OK;
	}
}

bk_err_t bk_dvp_camera_dump_register(void)
{
	if (current_sensor == NULL || dvp_camera_device == NULL || current_sensor->dump_register == NULL)
	{
		LOGI("%s, dump failed\r\n", __func__);
	}

	current_sensor->dump_register(dvp_camera_device->ppi);

	return kNoErr;
}


