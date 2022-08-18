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

#include <common/bk_include.h>
#include <os/os.h>
#include <stdio.h>
#include <driver/media_types.h>
#include <driver/uvc_camera.h>
#include <os/mem.h>
#include "bk_usb.h"
#include <modules/usb.h>
#include <common/bk_err.h>
#include <driver/dma.h>
#include "bk_general_dma.h"

#include <driver/psram.h>

extern void delay(INT32 num);

#define TAG "uvc_drv"

#define UVC_LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define UVC_LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define UVC_LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define UVC_LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define USB_UVC_FIFO_ADDR           (0x46002024)
#define UVC_DATA_LENGTH             512
#define USB_UVC_HEAD_LEN            12

void uvc_camera_memcpy_finish_callback(dma_id_t id);


beken_thread_t uvc_thread_drv_handle = NULL;
beken_queue_t uvc_drv_msg_que = NULL;
static const uvc_camera_config_t *uvc_camera_config = NULL;
static frame_buffer_t *curr_frame_buffer = NULL;
static uint8_t *uvc_rx_vstream_buffptr = NULL;
static uint8_t  g_uvc_start = 0;
uint32_t uvc_frame_id = 0;

#define FRAME_BUFFER_UVC         (1024 * 5)

typedef struct
{
	uint8_t  index;
	uint8_t  eof;
	uint8_t  uvc_dma;
	uint8_t  psram_dma;
	uint8_t  psram_dma_busy;
	uint16_t psram_dma_left;
	uint32_t rxbuf_len;
	uint32_t rx_read_len;
	uint32_t uvc_transfer_len;
	uint8_t *buffer;
	frame_buffer_t *frame;
	void (*node_full_handler)(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len);
} uvc_camera_drv_t;

uvc_camera_drv_t *uvc_camera_drv = NULL;

static bk_err_t uvc_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	uvc_msg_t msg;

	if (uvc_drv_msg_que)
	{
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&uvc_drv_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			os_printf("uvc_send_msg failed, type:%d\r\n", type);
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t uvc_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnls)
{
	dma_config_t dma_config = {0};

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

	uvc_camera_drv->psram_dma_busy = true;

	BK_LOG_ON_ERR(bk_dma_init(cpy_chnls, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(cpy_chnls, len));

	BK_LOG_ON_ERR(bk_dma_register_isr(cpy_chnls, NULL, uvc_camera_memcpy_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(cpy_chnls));
	BK_LOG_ON_ERR(bk_dma_start(cpy_chnls));

	return BK_OK;
}

static void uvc_process_data_packet(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len)
{
	uint8_t *data;
	uint8_t bmhead_info;
	uint32_t fack_len = 0;

	if (curr_frame_buffer == NULL
	    || curr_frame_buffer->frame == NULL)
	{
		UVC_LOGE("%s curr_frame_buffer NULL\n");
		return;
	}

	bmhead_info = *((uint8_t *)curptr + 1);
	data = (uint8_t *)curptr + USB_UVC_HEAD_LEN;

	if (bmhead_info & 0x40)  // bit6 = 1, payload error
	{
		return;
	}

	if (bmhead_info & 0x02)   // bit1 = 1, end frame
	{
		/*uint8_t *eof;
		eof = (uint8_t *)curptr + newlen - 2;
		os_printf("%s, %02x, %02x\r\n", __func__, eof[0], eof[1]);*/
		uvc_camera_drv->eof = true;
		uvc_camera_drv->frame = curr_frame_buffer;
		curr_frame_buffer->sequence = ++uvc_frame_id;
		if (frame_len % 4)
		{
			fack_len = (frame_len / 4 + 1) * 4;
			//os_printf("eof frame_len:%d\r\n", frame_len);
		}
	}
	else
	{
		/*if ((data[0] == 0xff) && (data[1] == 0xd8)) { // strat frame
		    os_printf("uvc start, %02x, %02x\r\n", data[0], data[1]);
		}*/
	}

	uvc_memcpy_by_chnl(curr_frame_buffer->frame + curr_frame_buffer->length, data, fack_len ? fack_len : frame_len, uvc_camera_drv->psram_dma);
	curr_frame_buffer->length += frame_len;
}

void uvc_camera_memcpy_finish_callback(dma_id_t id)
{
	uvc_camera_drv->psram_dma_busy = false;
	uvc_camera_drv->index = !uvc_camera_drv->index;

	if (uvc_camera_drv->eof == true)
	{
		frame_buffer_t *frame = uvc_camera_drv->frame;

		if (uvc_camera_drv->psram_dma_left != 0)
		{
			uvc_memcpy_by_chnl(frame->frame + frame->length,
			                   uvc_camera_drv->index ? (uvc_camera_drv->buffer + FRAME_BUFFER_UVC) : uvc_camera_drv->buffer,
			                   uvc_camera_drv->psram_dma_left, uvc_camera_drv->psram_dma);
			frame->length += uvc_camera_drv->psram_dma_left;
			frame->sequence = ++uvc_frame_id;
			uvc_camera_drv->psram_dma_left = 0;
		}
		else
		{
			uvc_camera_config->frame_complete(frame);
			uvc_camera_drv->index = 0;
			uvc_camera_drv->frame = NULL;
			uvc_camera_drv->eof = false;

			curr_frame_buffer = uvc_camera_config->frame_alloc();

			if (curr_frame_buffer == NULL
			    || curr_frame_buffer->frame == NULL)
			{
				UVC_LOGE("alloc frame error\n");
				return;
			}

			curr_frame_buffer->length = 0;
		}
	}
}

static void uvc_camera_dma_finish_callback(dma_id_t id)
{

	uint32_t already_len = uvc_camera_drv->rx_read_len;
	uint32_t copy_len = uvc_camera_drv->uvc_transfer_len;
	uint32_t frame_len = uvc_camera_drv->uvc_transfer_len - USB_UVC_HEAD_LEN;
	if (uvc_camera_drv->node_full_handler != NULL)
	{
		uvc_camera_drv->node_full_handler(uvc_camera_drv->buffer + already_len, copy_len, 0, frame_len);
	}

	already_len += copy_len;

	if (already_len >= uvc_camera_drv->rxbuf_len)
	{
		already_len = 0;
	}

	uvc_camera_drv->rx_read_len = already_len;
}

static bk_err_t uvc_dma_config(void)
{
	bk_err_t ret = kNoErr;
	curr_frame_buffer = uvc_camera_config->frame_alloc();

	if (curr_frame_buffer == NULL)
	{
		UVC_LOGE("malloc frame fail \r\n");
		ret = kNoResourcesErr;
		return ret;
	}

	curr_frame_buffer->length = 0;

	uvc_camera_drv->uvc_dma = bk_dma_alloc(DMA_DEV_USB);
	if ((uvc_camera_drv->uvc_dma < DMA_ID_0) || (uvc_camera_drv->uvc_dma >= DMA_ID_MAX))
	{
		UVC_LOGE("malloc dma fail \r\n");
		ret = kNoResourcesErr;
		return ret;
	}

	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;//DMA_DEV_USB;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = USB_UVC_FIFO_ADDR;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)uvc_camera_drv->buffer;
	dma_config.dst.end_addr = (uint32_t)(uvc_camera_drv->buffer + FRAME_BUFFER_UVC * 2);

	BK_LOG_ON_ERR(bk_dma_init(uvc_camera_drv->uvc_dma, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(uvc_camera_drv->uvc_dma, FRAME_BUFFER_UVC));
	BK_LOG_ON_ERR(bk_dma_register_isr(uvc_camera_drv->uvc_dma, NULL, uvc_camera_dma_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(uvc_camera_drv->uvc_dma));

	return ret;
}

static void uvc_notify_uvc_configed_callback(void)
{}

static void uvc_fiddle_rx_vs_callback(void)
{
	if (g_uvc_start)
	{
		bk_uvc_receive_video_stream();
	}
	else
	{
		bk_uvc_stop();
	}
}

static void uvc_get_packet_rx_vs_callback(uint8_t *arg, uint32_t count)
{
	uint32_t left_len = 0;

	bk_dma_set_src_start_addr(uvc_camera_drv->uvc_dma, (uint32_t)arg);
	uvc_camera_drv->uvc_transfer_len = count;
	left_len = uvc_camera_drv->rxbuf_len - uvc_camera_drv->rx_read_len;
	if (left_len < uvc_camera_drv->uvc_transfer_len)
	{
		uvc_camera_drv->rx_read_len = 0;
	}

	uint32_t dest_start_addr = (uint32_t)(uvc_camera_drv->buffer + uvc_camera_drv->rx_read_len);
	bk_dma_set_dest_start_addr(uvc_camera_drv->uvc_dma, dest_start_addr);
	bk_dma_set_transfer_len(uvc_camera_drv->uvc_dma, uvc_camera_drv->uvc_transfer_len);
	bk_dma_start(uvc_camera_drv->uvc_dma);
}

static uint32_t uvc_process_ppi_fps(const uvc_camera_device_t *config)
{
	uint32_t param = 0;
	uint32_t resolution_id = UVC_FRAME_640_480;

	UVC_LOGI("width:%d, height:%d, fps:%d\r\n", config->width, config->height, config->fps);
	if ((config->width == 160) && (config->height == 120))
	{
		resolution_id = UVC_FRAME_160_120;
	}
	else if ((config->width == 176) && (config->height == 144))
	{
		resolution_id = UVC_FRAME_176_144;
	}
	else if ((config->width == 320) && (config->height == 240))
	{
		resolution_id = UVC_FRAME_320_240;
	}
	else if ((config->width == 352) && (config->height == 288))
	{
		resolution_id = UVC_FRAME_352_288;
	}
	else if ((config->width == 480) && (config->height == 320))
	{
		resolution_id = UVC_FRAME_480_320;
	}
	else if ((config->width == 480) && (config->height == 800))
	{
		resolution_id = UVC_FRAME_480_800;
	}
	else if ((config->width == 640) && (config->height == 320))
	{
		resolution_id = UVC_FRAME_640_320;
	}
	else if ((config->width == 640) && (config->height == 360))
	{
		resolution_id = UVC_FRAME_640_360;
	}
	else if ((config->width == 640) && (config->height == 480))
	{
		resolution_id = UVC_FRAME_640_480;
	}
	else if ((config->width == 800) && (config->height == 400))
	{
		resolution_id = UVC_FRAME_800_400;
	}
	else if ((config->width == 800) && (config->height == 480))
	{
		resolution_id = UVC_FRAME_800_480;
	}
	else if ((config->width == 800) && (config->height == 600))
	{
		resolution_id = UVC_FRAME_800_600;
	}
	else if ((config->width == 960) && (config->height == 540))
	{
		resolution_id = UVC_FRAME_960_540;
	}
	else if ((config->width == 1280) && (config->height == 720))
	{
		resolution_id = UVC_FRAME_1280_720;
	}
	else
	{
		UVC_LOGE("Not find this resolution, use default\r\n");
		return 0;
	}

	if (config->fps < FPS_5 || config->fps > FPS_30)
	{
		UVC_LOGE("Set FPS unknow: %d\r\n", config->fps);
		return 0;
	}

	param |= (resolution_id << 16);//bit[31-16]:ppi
	param |= config->fps; // bit[15-0]:fps

	return param;
}

static void uvc_set_ppi_fps(uint32_t data)
{
	uint32_t resolution_id;
	uint32_t fps;
	bk_err_t status;
	resolution_id = data >> 16;
	fps = data & 0xFFFF;
	status = bk_uvc_set_parameter(resolution_id, fps);
	if (status != kNoErr)
	{
		os_printf("Set uvc param0 error!\r\n");
		status = kOptionErr;
	}

	if (status != kNoErr)
	{
		uvc_send_msg(UVC_EXIT, 0);
	}
}

static void uvc_set_start(uint32_t data)
{
	bk_err_t status = BK_OK;
	g_uvc_start = 1;
	status = bk_uvc_start();
	if (status != BK_OK)
	{
		os_printf("start uvc error!\r\n");
		uvc_send_msg(UVC_EXIT, 0);
	}
}

static void uvc_set_stop(uint32_t data)
{
	g_uvc_start = 0;
	uvc_camera_drv->rx_read_len = 0;
}

static bk_err_t uvc_camera_init(const uvc_camera_config_t *config)
{
	uint32_t param;
	void *parameter;
	bk_err_t err = BK_OK;

	uvc_camera_config->frame_set_ppi((uint32_t)(uvc_camera_config->device->width) << 16 | uvc_camera_config->device->height);

	// step 1: init dma, fifo to sharemem
	err = uvc_dma_config();
	if (err != BK_OK)
	{
		goto init_error;
	}

	// step 2: init dma, sharemem to frame_buffer
	uvc_camera_drv->psram_dma = bk_dma_alloc(DMA_DEV_DTCM);
	if ((uvc_camera_drv->psram_dma < DMA_ID_0) || (uvc_camera_drv->psram_dma >= DMA_ID_MAX))
	{
		UVC_LOGE("malloc uvc_camera_drv->psram_dma fail \r\n");
		err = kNoResourcesErr;
		goto init_error;
	}

	parameter = (void *)uvc_notify_uvc_configed_callback;
	err = bk_uvc_register_config_callback(parameter);
	if (err != BK_OK)
	{
		os_printf("register uvc config callback error!\r\n");
		goto init_error;
	}

	parameter = (void *)uvc_fiddle_rx_vs_callback;
	err = bk_uvc_register_VSrxed_callback(parameter);
	if (err != BK_OK)
	{
		os_printf("register uvc rx video stream callback error!\r\n");
		goto init_error;
	}

	parameter = (void *)uvc_get_packet_rx_vs_callback;
	err = bk_uvc_register_VSrxed_packet_callback(parameter);
	if (err != BK_OK)
	{
		os_printf("register uvc rx every packet callback error!\r\n");
		goto init_error;
	}

	uvc_rx_vstream_buffptr = (uint8_t *)os_malloc(128);
	if (!uvc_rx_vstream_buffptr)
	{
		os_printf("malloc rx video stream buf error!\r\n");
		goto init_error;
	}

	parameter = (void *)uvc_rx_vstream_buffptr;
	err = bk_uvc_register_rx_vstream_buffptr(parameter);
	if (err != BK_OK)
	{
		os_printf("uvc set rx video stream buf addr error!\r\n");
		goto init_error;
	}

	param = 128;
	err = bk_uvc_register_rx_vstream_bufflen(param);
	if (err != BK_OK)
	{
		os_printf("uvc set rx video stream buf length error!\r\n");
		goto init_error;
	}

	bk_uvc_register_link(0);
	param = uvc_process_ppi_fps(uvc_camera_config->device);

	err = bk_uvc_set_parameter((param >> 16), (param & 0xFFFF));
	if (err != BK_OK)
	{
		UVC_LOGE("uvc set fps and ppi error!\r\n");
	}

init_error:

	return err;
}

static bk_err_t uvc_camera_deinit(void)
{
	// uvc deinit
	bk_err_t status = 0;
	g_uvc_start = 0;
	rtos_delay_milliseconds(500);

	if (uvc_camera_drv)
	{
		bk_dma_stop(uvc_camera_drv->uvc_dma);
		bk_dma_deinit(uvc_camera_drv->uvc_dma);
		bk_dma_free(DMA_DEV_USB, uvc_camera_drv->uvc_dma);

		bk_dma_stop(uvc_camera_drv->psram_dma);
		bk_dma_deinit(uvc_camera_drv->psram_dma);
		bk_dma_free(DMA_DEV_DTCM, uvc_camera_drv->psram_dma);

		if (uvc_camera_drv->buffer)
		{
			os_free(uvc_camera_drv->buffer);
			uvc_camera_drv->buffer = NULL;
		}

		os_free(uvc_camera_drv);
		uvc_camera_drv = NULL;
	}

	uvc_frame_id = 0;
	uvc_camera_config = NULL;
	curr_frame_buffer = NULL;
	if (uvc_rx_vstream_buffptr)
	{
		os_free(uvc_rx_vstream_buffptr);
		uvc_rx_vstream_buffptr = NULL;
	}

	return status;
}

static void uvc_process_main(void)
{
	bk_err_t err;

	uvc_send_msg(UVC_START, 0);

	while (1)
	{
		uvc_msg_t msg;
		err = rtos_pop_from_queue(&uvc_drv_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err)
		{
			switch (msg.type)
			{
				case UVC_SET_PARAM:
					uvc_set_ppi_fps(msg.data);
					break;
				case UVC_START:
					uvc_set_start(msg.data);
					break;
				case UVC_STOP:
					uvc_set_stop(msg.data);
					break;
				case UVC_EXIT:
					goto uvc_exit;
					break;
				default:
					break;
			}
		}
	}

uvc_exit:

	uvc_camera_deinit();

	rtos_deinit_queue(&uvc_drv_msg_que);
	uvc_drv_msg_que = NULL;

	uvc_thread_drv_handle = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t bk_uvc_camera_get_config(uvc_camera_device_t *param, uint16_t count)
{
	if (param == NULL)
	{
		return BK_FAIL;
	}
	bk_uvc_get_resolution_framerate((void *)param, count);

	return BK_OK;
}

bk_err_t bk_uvc_camera_set_config(uvc_camera_device_t *config)
{
	int status = kNoErr;
	uint32_t param = 0;
	param = uvc_process_ppi_fps(config);

	status = uvc_send_msg(UVC_SET_PARAM, param);

	return status;
}

bk_err_t bk_uvc_camera_driver_start(void)
{
	uvc_send_msg(UVC_START, 0);

	return BK_OK;
}

bk_err_t bk_uvc_camera_driver_stop(void)
{
	uvc_send_msg(UVC_STOP, 0);

	return BK_OK;
}

uvc_camera_device_t *bk_uvc_camera_get_device(void)
{
	return uvc_camera_config->device;
}


bk_err_t bk_uvc_camera_driver_init(const uvc_camera_config_t *config)
{
	int ret = kNoErr;

	BK_ASSERT(uvc_camera_config->frame_alloc != NULL);
	BK_ASSERT(uvc_camera_config->frame_complete != NULL);

#if (CONFIG_PSRAM)
	bk_psram_init();
#endif
	if (uvc_camera_drv == NULL)
	{
		uvc_camera_drv = (uvc_camera_drv_t *)os_malloc(sizeof(uvc_camera_drv_t));

		if (uvc_camera_drv == NULL)
		{
			UVC_LOGE("uvc_camera_drv malloc failed\n");
			ret = kNoMemoryErr;
			goto error;
		}

		os_memset(uvc_camera_drv, 0, sizeof(uvc_camera_drv));

		uvc_camera_drv->buffer = (uint8 *)os_malloc(FRAME_BUFFER_UVC * 2);

		if (uvc_camera_drv->buffer == NULL)
		{
			UVC_LOGE("uvc_camera_drv malloc failed\n");
			if (uvc_camera_drv)
			{
				os_free(uvc_camera_drv);
				uvc_camera_drv = NULL;
			}

			ret = kNoMemoryErr;
			goto error;
		}
	}

	uvc_camera_drv->rxbuf_len = FRAME_BUFFER_UVC * 2;
	uvc_camera_drv->node_full_handler = uvc_process_data_packet;

	uvc_camera_config = config;

	if (uvc_camera_init(uvc_camera_config) != BK_OK)
	{
		ret = kGeneralErr;
		goto error;
	}

	if ((!uvc_thread_drv_handle) && (!uvc_drv_msg_que))
	{

		ret = rtos_init_queue(&uvc_drv_msg_que,
		                      "uvc_queue",
		                      sizeof(uvc_msg_t),
		                      30);
		if (kNoErr != ret)
		{
			goto error;
		}

		ret = rtos_create_thread(&uvc_thread_drv_handle,
		                         3,
		                         "uvc_init",
		                         (beken_thread_function_t)uvc_process_main,
		                         4 * 1024,
		                         (beken_thread_arg_t)0);

		if (ret != kNoErr)
		{

			goto error;
		}

		return kNoErr;
	}

error:

	if (uvc_camera_drv)
	{
		if (uvc_camera_drv->buffer)
		{
			os_free(uvc_camera_drv->buffer);
			uvc_camera_drv->buffer = NULL;
		}

		os_free(uvc_camera_drv);
		uvc_camera_drv = NULL;
	}

	uvc_camera_config = NULL;

	uvc_camera_deinit();

	if (uvc_drv_msg_que)
	{
		rtos_deinit_queue(&uvc_drv_msg_que);
		uvc_drv_msg_que = NULL;
	}

	if (uvc_thread_drv_handle)
	{
		uvc_thread_drv_handle = NULL;
	}

	return ret;
}

bk_err_t bk_uvc_camera_driver_deinit(void)
{
	if (!uvc_thread_drv_handle)
	{
		return BK_OK;
	}
	uvc_send_msg(UVC_EXIT, 0);

	while (uvc_thread_drv_handle)
	{
		rtos_delay_milliseconds(10);
	}

	return BK_OK;
}


