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

#define TAG "dvp_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define FRAME_BUFFER_DMA_TH (1024 * 10)
#define JPEG_CRC_SIZE (5)
#define DROP_FRAME_COUNT (0)

#define FRAME_BUFFER_CACHE (1024 * 10)

#define DVP_I2C_TIMEOUT (2000)

//#define DVP_STRIP

//#define DVP_DIAG_DEBUG

#ifdef DVP_DIAG_DEBUG

#define DVP_DIAG_DEBUG_INIT()                   \
	do {                                        \
		gpio_dev_unmap(GPIO_5);                 \
		bk_gpio_disable_pull(GPIO_5);           \
		bk_gpio_enable_output(GPIO_5);          \
		bk_gpio_set_output_low(GPIO_5);         \
		\
		gpio_dev_unmap(GPIO_6);                 \
		bk_gpio_disable_pull(GPIO_6);           \
		bk_gpio_enable_output(GPIO_6);          \
		bk_gpio_set_output_low(GPIO_6);         \
		\
		gpio_dev_unmap(GPIO_8);                 \
		bk_gpio_disable_pull(GPIO_8);           \
		bk_gpio_enable_output(GPIO_8);          \
		bk_gpio_set_output_low(GPIO_8);         \
		\
		gpio_dev_unmap(GPIO_9);                 \
		bk_gpio_disable_pull(GPIO_9);           \
		bk_gpio_enable_output(GPIO_9);          \
		bk_gpio_set_output_low(GPIO_9);         \
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

#define DVP_JPEG_VSYNC_ENTRY()            bk_gpio_set_output_high(GPIO_5)
#define DVP_JPEG_VSYNC_OUT()              bk_gpio_set_output_low(GPIO_5)

#define DVP_JPEG_EOF_ENTRY()            bk_gpio_set_output_high(GPIO_6)
#define DVP_JPEG_EOF_OUT()              bk_gpio_set_output_low(GPIO_6)

#define DVP_YUV_EOF_ENTRY()             bk_gpio_set_output_high(GPIO_8)
#define DVP_YUV_EOF_OUT()               bk_gpio_set_output_low(GPIO_8)

#define DVP_JPEG_START_ENTRY()          bk_gpio_set_output_high(GPIO_9)
#define DVP_JPEG_START_OUT()            bk_gpio_set_output_low(GPIO_9)

#define DVP_JPEG_HEAD_ENTRY()           bk_gpio_set_output_high(GPIO_12)
#define DVP_JPEG_HEAD_OUT()             bk_gpio_set_output_low(GPIO_12)

#define DVP_JPEG_SDMA_ENTRY()           bk_gpio_set_output_high(GPIO_13)
#define DVP_JPEG_SDMA_OUT()             bk_gpio_set_output_low(GPIO_13)

#define DVP_DEBUG_IO()                      \
	do {                                    \
		bk_gpio_set_output_high(GPIO_4);    \
		bk_gpio_set_output_low(GPIO_4);     \
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

#define DVP_JPEG_SDMA_ENTRY()
#define DVP_JPEG_SDMA_OUT()

#define DVP_JPEG_VSYNC_ENTRY()
#define DVP_JPEG_VSYNC_OUT()

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
	uint8 psram_dma;
	uint8 psram_dma_busy;
	uint16 psram_dma_left;
	uint8 *buffer;
	uint8 drop_count;
	frame_buffer_t *frame;
} dvp_camera_drv_t;

typedef struct
{
	uint8_t  enable_set;
	uint8_t  auto_encode;
	uint32_t up;
	uint32_t low;
} dvp_camera_encode_t;

dvp_camera_drv_t *dvp_camera_drv = NULL;


const dvp_sensor_config_t *dvp_sensor_configs[] =
{
	&dvp_sensor_gc0328c,
	&dvp_sensor_hm1055,
	&dvp_sensor_gc2145,
	&dvp_sensor_ov2640,
};

static const dvp_camera_i2c_callback_t dvp_camera_i2c_cb =
{
	dvp_camera_i2c_read_uint8,
	dvp_camera_i2c_read_uint16,
	dvp_camera_i2c_write_uint8,
	dvp_camera_i2c_write_uint16,
};

static const dvp_sensor_config_t *current_sensor = NULL;
static dvp_camera_config_t dvp_camera_config = {0};

static uint8_t dvp_camera_dma_channel = 0;

static volatile media_state_t dvp_state = MSTATE_TURN_OFF;
static bool camera_read_flag = false;

#define DVP_BUFFER_SIZE 1024
#define DVP_BUFFER_COUNT 4

uint32_t sequence = 0;
dvp_camera_encode_t dvp_camera_encode = {0, 0, 50 * 1024, 30 * 1024};
static beken_semaphore_t dvp_sema = NULL;

extern media_debug_t *media_debug;

frame_buffer_t *curr_jpeg_frame = NULL;
frame_buffer_t *curr_yuv_frame = NULL;

dvp_camera_device_t *dvp_camera_device = NULL;

bk_err_t dvp_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls);

jpeg_config_t jpeg_config = {0};

const dvp_sensor_config_t *get_sensor_config_interface_by_id(sensor_id_t id)
{
	uint32_t i, size = sizeof(dvp_sensor_configs) / sizeof(dvp_sensor_config_t *);

	for (i = 0; i < size; i++)
	{
		if (dvp_sensor_configs[i]->id == id)
		{
			return dvp_sensor_configs[i];
		}
	}

	return NULL;
}

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

#ifdef DVP_STRIP
static uint32_t dvp_frame_sram_strip(uint8_t *src, uint32_t size)
{
	uint32_t i;
	uint32_t tail = size - 1;

	if (src[tail] != 0xD9
	    && src[tail - 1 ] != 0xFF)
	{
		LOGE("strip tail error\n");
		return size;
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

static void dvp_camera_vsync_negedge_handler(jpeg_unit_t id, void *param)
{
	DVP_JPEG_VSYNC_ENTRY();

	if (dvp_state == MSTATE_TURNING_OFF)
	{

#if (CONFIG_SIM_I2C)
		if (current_sensor->power_down)
		{
			current_sensor->power_down(&dvp_camera_i2c_cb);
		}
#endif
		bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);
		bk_jpeg_enc_set_enable(0, JPEG_YUV_MODE);

		if (dvp_sema != NULL)
		{
			rtos_set_semaphore(&dvp_sema);
		}

	}

	DVP_JPEG_VSYNC_OUT();

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
			dvp_camera_config.fb_jpeg_complete(frame);
			dvp_camera_drv->index = 0;
			dvp_camera_drv->frame = NULL;
			dvp_camera_drv->eof = false;

			if (dvp_state == MSTATE_TURN_ON)
			{
				if (dvp_camera_device->mode == DVP_MODE_JPG)
				{
					curr_jpeg_frame = dvp_camera_config.fb_jpeg_malloc();

					if (curr_jpeg_frame == NULL
					    || curr_jpeg_frame->frame == NULL)
					{
						LOGE("alloc frame error\n");
						return;
					}

					curr_jpeg_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
					curr_jpeg_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
					curr_jpeg_frame->fmt = PIXEL_FMT_DVP_JPEG;

					bk_dma_start(dvp_camera_dma_channel);
				}
			}
			else
			{
				curr_jpeg_frame = NULL;
			}
		}
	}

}

bk_err_t dvp_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls)
{
	dma_config_t dma_config;

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

	BK_LOG_ON_ERR(bk_dma_register_isr(cpy_chnls, NULL, dvp_memcpy_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(cpy_chnls));
	BK_LOG_ON_ERR(bk_dma_start(cpy_chnls));
	//BK_WHILE (bk_dma_get_enable_status(cpy_chnl));


	return BK_OK;

}

static void dvp_camera_eof_handler(jpeg_unit_t id, void *param)
{
	uint32_t real_length = bk_jpeg_enc_get_frame_size();
	uint32_t remain_length = FRAME_BUFFER_CACHE - bk_dma_get_remain_len(dvp_camera_dma_channel);
	uint32_t left_length = remain_length - JPEG_CRC_SIZE;

	DVP_JPEG_EOF_ENTRY();

	//LOGI("JPEG EOF\n");
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

		if (dvp_camera_encode.enable_set)
		{
			if (dvp_camera_encode.auto_encode)
			{
				bk_jpeg_enc_enable_encode_auto_ctrl(dvp_camera_encode.auto_encode);
				bk_jpeg_enc_set_target_size(dvp_camera_encode.up, dvp_camera_encode.low);
			}
			else
			{
				bk_jpeg_enc_enable_encode_auto_ctrl(dvp_camera_encode.auto_encode);
			}

			dvp_camera_encode.enable_set = 0;
		}

		if (curr_jpeg_frame == NULL
		    || curr_jpeg_frame->frame == NULL)
		{
			LOGE("curr_jpeg_frame NULL error\n");
			goto error;
		}

		if (curr_jpeg_frame->length + left_length != real_length)
		{
			LOGW("size no match: %u:%u:%u\n", curr_jpeg_frame->length + left_length, real_length, remain_length);
		}

#ifdef DVP_STRIP
		left_length = dvp_frame_sram_strip(dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer, left_length);
#endif
		bk_dma_stop(dvp_camera_dma_channel);

		if ((dvp_camera_device->mode == DVP_MODE_MIX)
		    && (dvp_state == MSTATE_TURN_ON))
		{
			curr_jpeg_frame->mix = true;

			bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);

			curr_yuv_frame = dvp_camera_config.fb_display_malloc();

			if (curr_yuv_frame == NULL)
			{
				LOGE("%s malloc curr_yuv_frame failed\n", __func__);
				goto error;
			}

			curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
			curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
			curr_yuv_frame->fmt = PIXEL_FMT_YUYV;

			LOGD("start yuv\n");
			jpeg_config.yuv_mode = 1;
			bk_jpeg_enc_dvp_switch(&jpeg_config);
			bk_jpeg_set_em_base_addr(curr_yuv_frame->frame);
		}

		if (dvp_camera_drv->psram_dma_busy == true)
		{
			dvp_camera_drv->frame = curr_jpeg_frame;
			dvp_camera_drv->psram_dma_left = left_length;
			dvp_camera_drv->eof = true;
		}
		else
		{
			dvp_camera_drv->frame = curr_jpeg_frame;
			dvp_camera_drv->psram_dma_left = 0;
			dvp_camera_drv->eof = true;

			dvp_memcpy_by_chnl(curr_jpeg_frame->frame + curr_jpeg_frame->length,
			                   dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer,
			                   left_length, dvp_camera_drv->psram_dma);
			curr_jpeg_frame->length += left_length;
			curr_jpeg_frame->sequence = ++sequence;
		}
	}
	else
	{
		remain_length = FRAME_BUFFER_DMA_TH - bk_dma_get_remain_len(dvp_camera_dma_channel);
		left_length = remain_length - JPEG_CRC_SIZE;

		if (curr_jpeg_frame->length + left_length != real_length)
		{
			LOGW("size no match: %u:%u:%u\n", curr_jpeg_frame->length + left_length, real_length, remain_length);
			LOGW("size: %u:%u:%u\n", bk_dma_get_remain_len(dvp_camera_dma_channel), bk_jpeg_enc_get_frame_size(), curr_jpeg_frame->size);
		}

		curr_jpeg_frame->length += left_length;
		dvp_camera_config.fb_jpeg_complete(curr_jpeg_frame);
		curr_jpeg_frame = dvp_camera_config.fb_jpeg_malloc();
		curr_jpeg_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
		curr_jpeg_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
		curr_jpeg_frame->fmt = PIXEL_FMT_DVP_JPEG;

		if (curr_jpeg_frame == NULL
		    || curr_jpeg_frame->frame == NULL)
		{
			LOGE("alloc frame error\n");
			return;
		}

		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_set_dest_addr(dvp_camera_dma_channel, (uint32_t)curr_jpeg_frame->frame, (uint32_t)(curr_jpeg_frame->frame + curr_jpeg_frame->size));
		bk_dma_start(dvp_camera_dma_channel);

	}

	DVP_JPEG_EOF_OUT();

	return;

error:
	dvp_camera_drv->index = 0;
	bk_dma_stop(dvp_camera_dma_channel);
	bk_jpeg_enc_set_enable(0, JPEG_ENC_MODE);
}

static void dvp_camera_dma_finish_callback(dma_id_t id)
{
	DVP_JPEG_SDMA_ENTRY();

	if (dvp_camera_drv->cached == true)
	{
		if (curr_jpeg_frame == NULL
		    || curr_jpeg_frame->frame == NULL)
		{
			LOGE("%s curr_jpeg_frame NULL\n");
			return;
		}

		if (dvp_camera_drv->drop_count == 0)
		{
			dvp_memcpy_by_chnl(curr_jpeg_frame->frame + curr_jpeg_frame->length,
			                   dvp_camera_drv->index ? (dvp_camera_drv->buffer + FRAME_BUFFER_CACHE) : dvp_camera_drv->buffer,
			                   FRAME_BUFFER_CACHE, dvp_camera_drv->psram_dma);
			curr_jpeg_frame->length += FRAME_BUFFER_CACHE;
		}
	}
	else
	{
		curr_jpeg_frame->length += FRAME_BUFFER_DMA_TH;
	}

	DVP_JPEG_SDMA_OUT();
}

static bk_err_t dvp_camera_dma_config(void)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config = {0};
	uint32_t jpeg_fifo_addr;

	curr_jpeg_frame = dvp_camera_config.fb_jpeg_malloc();

	if (curr_jpeg_frame == NULL)
	{
		LOGE("malloc frame fail \r\n");
		ret = BK_FAIL;
		return ret;
	}

	curr_jpeg_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
	curr_jpeg_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
	curr_jpeg_frame->fmt = PIXEL_FMT_DVP_JPEG;

	dvp_camera_dma_channel = bk_dma_alloc(DMA_DEV_JPEG);
	if ((dvp_camera_dma_channel < DMA_ID_0) || (dvp_camera_dma_channel >= DMA_ID_MAX))
	{
		LOGE("malloc dma fail \r\n");
		ret = BK_FAIL;
		return ret;
	}
	LOGI("dvp_dma id:%d \r\n", dvp_camera_dma_channel);

	bk_jpeg_enc_get_fifo_addr(&jpeg_fifo_addr);

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_JPEG;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = jpeg_fifo_addr;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;

	if (dvp_camera_drv->cached)
	{
		dma_config.dst.start_addr = (uint32_t)dvp_camera_drv->buffer;
		dma_config.dst.end_addr = (uint32_t)(dvp_camera_drv->buffer + FRAME_BUFFER_CACHE * 2);
	}
	else
	{
		dma_config.dst.start_addr = (uint32_t)curr_jpeg_frame->frame;
		dma_config.dst.end_addr = (uint32_t)(curr_jpeg_frame->frame + curr_jpeg_frame->size);
	}

	//os_printf("dst_start_addr:%08x, dst_end_addr:%08x\r\n", (uint32_t)info.buffer_addr, dma_config.dst.end_addr);

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
	BK_LOG_ON_ERR(bk_dma_start(dvp_camera_dma_channel));

	return ret;
}

static void dvp_camera_yuv_eof_handler(jpeg_unit_t id, void *param)
{
	frame_buffer_t *frame = NULL;

	DVP_YUV_EOF_ENTRY();

	media_debug->isr_jpeg++;

	curr_yuv_frame->sequence = ++sequence;

	if (dvp_camera_device)
	{
		curr_yuv_frame->length = dvp_camera_device->pixel_size;
	}

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
		if (dvp_camera_device->mode == DVP_MODE_MIX)
		{
			bk_jpeg_enc_set_enable(0, JPEG_YUV_MODE);

			curr_jpeg_frame = dvp_camera_config.fb_jpeg_malloc();

			if (curr_jpeg_frame == NULL
			    || curr_jpeg_frame->frame == NULL)
			{
				LOGE("alloc frame error\n");
				return;
			}

			curr_jpeg_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
			curr_jpeg_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
			curr_jpeg_frame->fmt = PIXEL_FMT_DVP_JPEG;

			bk_dma_start(dvp_camera_dma_channel);

			LOGD("start jpeg\n");

			jpeg_config.yuv_mode = 0;
			bk_jpeg_enc_dvp_switch(&jpeg_config);
		}
		else
		{
			frame = dvp_camera_config.fb_display_malloc();

			if (frame != NULL)
			{
				curr_yuv_frame = frame;
				curr_yuv_frame->width = ppi_to_pixel_x(dvp_camera_device->ppi);
				curr_yuv_frame->height = ppi_to_pixel_y(dvp_camera_device->ppi);
				curr_yuv_frame->fmt = PIXEL_FMT_YUYV;
				bk_jpeg_set_em_base_addr(curr_yuv_frame->frame);
			}
			else
			{
				LOGE("%s malloc frame failed\n", __func__);
			}
		}
	}

	DVP_YUV_EOF_OUT();
}

const dvp_sensor_config_t *get_sensor_auto_detect(const dvp_camera_i2c_callback_t *cb)
{
	uint32_t i, size = sizeof(dvp_sensor_configs) / sizeof(const dvp_sensor_config_t *);

	for (i = 0; i < size; i++)
	{
		if (dvp_sensor_configs[i]->detect(cb) == true)
		{
			return dvp_sensor_configs[i];
		}
	}

	return NULL;
}

int dvp_camera_i2c_read_uint8(uint8_t addr, uint8_t reg, uint8_t *value)
{
	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = value;

	return bk_i2c_memory_read(CAMERA_DVP_I2C_ID, &mem_param);
}

int dvp_camera_i2c_read_uint16(uint8_t addr, uint16_t reg, uint8_t *value)
{
	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_16BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = value;

	return bk_i2c_memory_read(CAMERA_DVP_I2C_ID, &mem_param);
}

int dvp_camera_i2c_write_uint8(uint8_t addr, uint8_t reg, uint8_t value)
{
	i2c_mem_param_t mem_param = {0};
	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = (uint8_t *)(&value);

	return bk_i2c_memory_write(CAMERA_DVP_I2C_ID, &mem_param);
}

int dvp_camera_i2c_write_uint16(uint8_t addr, uint16_t reg, uint8_t value)
{
	i2c_mem_param_t mem_param = {0};
	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_16BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = (uint8_t *)(&value);

	return bk_i2c_memory_write(CAMERA_DVP_I2C_ID, &mem_param);
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

bk_err_t bk_dvp_camera_driver_init(dvp_camera_config_t *config)
{
	int ret = kNoErr;
	i2c_config_t i2c_config = {0};

#if (CONFIG_PSRAM)
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_PSRAM, PM_CPU_FRQ_320M);
	bk_psram_init();
#endif

	if (dvp_camera_device == NULL)
	{
		dvp_camera_device = (dvp_camera_device_t *)os_malloc(sizeof(dvp_camera_device_t));

		if (dvp_camera_device == NULL)
		{
			LOGE("dvp_camera_device malloc failed\n");
			goto error;
		}

		os_memset(dvp_camera_device, 0, sizeof(dvp_camera_device_t));
	}

	dvp_camera_device->mode = config->mode;

	os_memcpy(&dvp_camera_config, config, sizeof(dvp_camera_config_t));

	ret = bk_jpeg_enc_driver_init();


	if (ret != BK_OK)
	{
		LOGE("jpeg encode driver init failed\n");
		goto error;
	}

	bk_dvp_camera_gpio_init(&dvp_camera_config, 1);
	//bk_dvp_camera_gpio_init(&dvp_camera_config, 2);

	bk_jpeg_enc_mclk_enable();

	rtos_delay_milliseconds(5);

	i2c_config.baud_rate = CAMERA_DVP_I2C_BAUD_RATE;
	i2c_config.addr_mode = CAMERA_DVP_I2C_MODE;
	bk_i2c_init(CAMERA_DVP_I2C_ID, &i2c_config);

	current_sensor = get_sensor_auto_detect(&dvp_camera_i2c_cb);

	if (current_sensor == NULL)
	{
		LOGE("%s no dvp camera found\n", __func__);
		ret = kGenericErrorBase;
		goto error;
	}

	current_sensor->read_register(camera_read_flag);
	dvp_camera_device->ppi = current_sensor->def_ppi;
	dvp_camera_device->fps = current_sensor->def_fps;
	dvp_camera_device->name = current_sensor->name;
	dvp_camera_device->id = current_sensor->id;
	dvp_camera_device->fps_cap = current_sensor->fps_cap;
	dvp_camera_device->ppi_cap = current_sensor->ppi_cap;

	if ((config->ppi != dvp_camera_device->ppi)
	    && (pixel_ppi_to_cap(config->ppi) & (dvp_camera_device->ppi_cap)))
	{
		dvp_camera_device->ppi = config->ppi;
		LOGI("%s switch ppi to %dX%d\n", __func__, config->ppi >> 16, config->ppi & 0xFFFF);
	}

	dvp_camera_device->pixel_size = get_ppi_size(dvp_camera_device->ppi);

	LOGI("Pixel size: %d\n", dvp_camera_device->pixel_size);

	jpeg_config.x_pixel = ppi_to_pixel_x(dvp_camera_device->ppi) / 8;
	jpeg_config.y_pixel = ppi_to_pixel_y(dvp_camera_device->ppi) / 8;

	switch (current_sensor->clk)
	{
		case JPEG_96M_MCLK_16M:
			jpeg_config.sys_clk_div = 4;
			jpeg_config.mclk_div = 1;
			break;

		case JPEG_96M_MCLK_24M:
			jpeg_config.sys_clk_div = 4;
			jpeg_config.mclk_div = 0;
			break;

		case JPEG_120M_MCLK_20M:
			jpeg_config.sys_clk_div = 3;
			jpeg_config.mclk_div = 1;
			break;

		case JPEG_120M_MCLK_30M:
			jpeg_config.sys_clk_div = 3;
			jpeg_config.mclk_div = 0;
			break;

		default:
			break;
	}

	if (config->mode == DVP_MODE_JPG)
	{
		jpeg_config.yuv_mode = 0;

		config->fb_jpeg_init(dvp_camera_device->ppi);

		if (dvp_camera_drv == NULL)
		{
			dvp_camera_drv = (dvp_camera_drv_t *)os_malloc(sizeof(dvp_camera_drv_t));

			if (dvp_camera_drv == NULL)
			{
				LOGE("dvp_camera_drv malloc failed\n");
				goto error;
			}

			os_memset(dvp_camera_drv, 0, sizeof(dvp_camera_drv_t));

			dvp_camera_drv->cached = true;
		}

		if (dvp_camera_drv->cached)
		{
			dvp_camera_drv->buffer = (uint8 *)os_malloc(FRAME_BUFFER_CACHE * 2);
			os_memset(dvp_camera_drv->buffer, 0, FRAME_BUFFER_CACHE * 2);

			if (dvp_camera_drv->buffer == NULL)
			{
				LOGE("dvp_camera_drv malloc failed\n");
				goto error;
			}

			dvp_camera_drv->psram_dma = bk_dma_alloc(DMA_DEV_JPEG);
			if ((dvp_camera_drv->psram_dma < DMA_ID_0) || (dvp_camera_drv->psram_dma >= DMA_ID_MAX))
			{
				LOGE("malloc dvp_camera_drv->psram_dma fail \r\n");
				ret = BK_FAIL;
				goto error;
			}

			LOGI("dvp_camera_drv->psram_dma:%d \r\n", dvp_camera_drv->psram_dma);

		}

		ret = dvp_camera_dma_config();

		if (ret != BK_OK)
		{
			LOGE("dma init failed\n");
			goto error;
		}

		ret = bk_jpeg_enc_dvp_init(&jpeg_config);
		if (ret != BK_OK)
		{
			LOGE("jpeg init error\n");
			goto error;
		}

		bk_jpeg_enc_register_isr(END_OF_FRAME, dvp_camera_eof_handler, NULL);
	}
	else if (config->mode == DVP_MODE_YUV)
	{
		jpeg_config.yuv_mode = 1;

		ret = bk_jpeg_enc_dvp_init(&jpeg_config);
		if (ret != BK_OK)
		{
			LOGE("jpeg init error\n");
			goto error;
		}

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
		curr_yuv_frame->fmt = PIXEL_FMT_YUYV;
		bk_jpeg_enc_register_isr(END_OF_YUV, dvp_camera_yuv_eof_handler, NULL);
		bk_jpeg_set_em_base_addr(curr_yuv_frame->frame);
	}
	else if (config->mode == DVP_MODE_MIX)
	{
		config->fb_jpeg_init(dvp_camera_device->ppi);
		config->fb_display_init(dvp_camera_device->ppi);

		if (dvp_camera_drv == NULL)
		{
			dvp_camera_drv = (dvp_camera_drv_t *)os_malloc(sizeof(dvp_camera_drv_t));

			if (dvp_camera_drv == NULL)
			{
				LOGE("dvp_camera_drv malloc failed\n");
				goto error;
			}

			os_memset(dvp_camera_drv, 0, sizeof(dvp_camera_drv_t));

			dvp_camera_drv->cached = true;
		}

		if (dvp_camera_drv->cached)
		{
			dvp_camera_drv->buffer = (uint8 *)os_malloc(FRAME_BUFFER_CACHE * 2);
			os_memset(dvp_camera_drv->buffer, 0, FRAME_BUFFER_CACHE * 2);

			if (dvp_camera_drv->buffer == NULL)
			{
				LOGE("dvp_camera_drv malloc failed\n");
				goto error;
			}

			dvp_camera_drv->psram_dma = bk_dma_alloc(DMA_DEV_JPEG);
			if ((dvp_camera_drv->psram_dma < DMA_ID_0) || (dvp_camera_drv->psram_dma >= DMA_ID_MAX))
			{
				LOGE("malloc dvp_camera_drv->psram_dma fail \r\n");
				ret = BK_FAIL;
				goto error;
			}

			LOGI("dvp_camera_drv->psram_dma:%d \r\n", dvp_camera_drv->psram_dma);

		}

		ret = dvp_camera_dma_config();

		if (ret != BK_OK)
		{
			LOGE("dma init failed\n");
			goto error;
		}

		jpeg_config.yuv_mode = 0;

		bk_jpeg_enc_dvp_init(&jpeg_config);

		bk_jpeg_enc_register_isr(END_OF_FRAME, dvp_camera_eof_handler, NULL);
		bk_jpeg_enc_register_isr(END_OF_YUV, dvp_camera_yuv_eof_handler, NULL);

#ifdef DVP_DIAG_DEBUG

		bk_jpeg_enc_register_isr(START_OF_FRAME, dvp_camera_dvp_start_handler, NULL);
		bk_jpeg_enc_register_isr(HEAD_OUTPUT, dvp_camera_dvp_head_handler, NULL);

#endif
	}

	bk_jpeg_enc_register_isr(VSYNC_NEGEDGE, dvp_camera_vsync_negedge_handler, NULL);
	dvp_state = MSTATE_TURN_ON;

	current_sensor->init(&dvp_camera_i2c_cb);
	bk_dvp_camera_gpio_init(&dvp_camera_config, 2);

	current_sensor->set_ppi(&dvp_camera_i2c_cb, dvp_camera_device->ppi);

	current_sensor->set_fps(&dvp_camera_i2c_cb, dvp_camera_device->fps);

	media_debug->isr_jpeg = 0;

	DVP_DIAG_DEBUG_INIT();

	LOGI("dvp camera init complete with mode: %d\n", config->mode);

	return ret;

error:

	if (dvp_camera_dma_channel < DMA_ID_MAX)
	{
		bk_dma_stop(dvp_camera_dma_channel);
		bk_dma_deinit(dvp_camera_dma_channel);
		bk_dma_free(DMA_DEV_JPEG, dvp_camera_dma_channel);
	}

	if (dvp_camera_drv != NULL)
	{
		if (dvp_camera_drv->buffer != NULL)
		{
			os_free(dvp_camera_drv->buffer);
			dvp_camera_drv->buffer = NULL;
		}

		if (dvp_camera_drv->psram_dma < DMA_ID_MAX)
		{
			bk_dma_stop(dvp_camera_drv->psram_dma);
			bk_dma_deinit(dvp_camera_drv->psram_dma);
			bk_dma_free(DMA_DEV_JPEG, dvp_camera_drv->psram_dma);
		}

		os_free(dvp_camera_drv);
		dvp_camera_drv = NULL;
	}

	if (dvp_camera_device)
	{
		os_free(dvp_camera_device);
		dvp_camera_device = NULL;
	}

	bk_jpeg_enc_deinit();

	bk_jpeg_enc_driver_deinit();

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
		LOGI("Not wait jpeg eof!\r\n");
	}

	bk_dma_stop(dvp_camera_dma_channel);
	bk_dma_deinit(dvp_camera_dma_channel);
	bk_dma_free(DMA_DEV_JPEG, dvp_camera_dma_channel);

	bk_jpeg_enc_dvp_deinit();

	if (dvp_camera_drv)
	{
		bk_dma_stop(dvp_camera_drv->psram_dma);
		bk_dma_deinit(dvp_camera_drv->psram_dma);
		bk_dma_free(DMA_DEV_JPEG, dvp_camera_drv->psram_dma);
		if (dvp_camera_drv->buffer)
		{
			os_free(dvp_camera_drv->buffer);
			dvp_camera_drv->buffer = NULL;
		}

		os_free(dvp_camera_drv);
		dvp_camera_drv = NULL;
	}

	bk_i2c_deinit(CAMERA_DVP_I2C_ID);

#if CONFIG_SOC_BK7256XX
	bk_jpeg_enc_driver_deinit();
#endif

	if (dvp_camera_device)
	{
		os_free(dvp_camera_device);
		dvp_camera_device = NULL;
	}

	if (curr_jpeg_frame)
	{
		dvp_camera_config.fb_jpeg_free(curr_jpeg_frame);
		curr_jpeg_frame = NULL;
	}

	if (curr_yuv_frame)
	{
		dvp_camera_config.fb_display_free(curr_yuv_frame);
		curr_yuv_frame = NULL;
	}

	current_sensor = NULL;
	os_memset(&dvp_camera_config, 0, sizeof(dvp_camera_config_t));
	dvp_camera_dma_channel = 0;

	rtos_deinit_semaphore(&dvp_sema);
	dvp_sema = NULL;

	dvp_state = MSTATE_TURN_OFF;

	LOGI("dvp camera deinit complete\n");
	return kNoErr;
}

bk_err_t bk_dvp_camera_encode_config(uint8_t auto_ctrl, uint32_t up_size, uint32_t low_size)
{
	dvp_camera_encode.enable_set = 1;

	dvp_camera_encode.auto_encode = auto_ctrl;

	if (dvp_camera_encode.auto_encode)
	{
		if (up_size > low_size)
		{
			dvp_camera_encode.up = up_size;
			dvp_camera_encode.low = low_size;
		}
	}

	return kNoErr;
}

bk_err_t bk_dvp_camera_dump_register(void)
{
	if (current_sensor == NULL || dvp_camera_device == NULL || current_sensor->dump_register)
	{
		LOGI("%s, dump failed\r\n", __func__);
	}

	current_sensor->dump_register(&dvp_camera_i2c_cb, dvp_camera_device->ppi);

	return kNoErr;
}


