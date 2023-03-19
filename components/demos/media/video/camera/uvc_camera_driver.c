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
#include <os/mem.h>

#include <driver/dma.h>
#include <components/video_transfer.h>
#include <driver/uvc_camera.h>
#include <driver/uvc_camera_types.h>
#include "camera_driver.h"

#define TAG "uvc"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static video_config_t *uvc_video_config = NULL;
static uint32_t uvc_frame_len = 0;
static uvc_config_t uvc_config = {0};
static uint8_t uvc_dma_channel = DMA_ID_MAX;
static uint8_t uvc_drop_frame = 0;
static bool uvc_open = false;

static void uvc_camera_process_node_data(uint8_t *curptr, uint32_t len)
{
	uint8_t *data;
	uint8_t bmhead_info;

	bmhead_info = *((uint8_t *)curptr + 1);
	data = (uint8_t *)curptr + USB_UVC_HEAD_LEN;

	if (bmhead_info & 0x40)  // bit6 = 1, payload error
	{
		return;
	}

	if (bmhead_info & 0x02)   // bit1 = 1, end frame
	{
		if (uvc_drop_frame == 0)
		{
			uvc_frame_len += len - USB_UVC_HEAD_LEN;

			if (uvc_video_config && uvc_video_config->node_full_handler != NULL)
				uvc_video_config->node_full_handler(data, len - USB_UVC_HEAD_LEN, 1, uvc_frame_len);

			uvc_frame_len = 0;

			if (uvc_video_config && uvc_video_config->data_end_handler)
				uvc_video_config->data_end_handler();
		}
		else
		{
			uvc_drop_frame--;
		}
	}
	else
	{
		if (uvc_drop_frame == 0)
		{
			uvc_frame_len += len - USB_UVC_HEAD_LEN;

			if (uvc_video_config && uvc_video_config->node_full_handler != NULL)
				uvc_video_config->node_full_handler(data, len - USB_UVC_HEAD_LEN, 0, 0);
		}
	}
}

static void uvc_camera_dma_finish_callback(dma_id_t id)
{
	GLOBAL_INT_DECLARATION();
	uint32_t already_len = uvc_video_config->rx_read_len;
	uint32_t copy_len = uvc_video_config->node_len;

	if (copy_len > USB_UVC_HEAD_LEN)
		uvc_camera_process_node_data(uvc_video_config->rxbuf + already_len, copy_len);

	already_len += copy_len;

	if (already_len >= uvc_video_config->rxbuf_len)
	{
		already_len = 0;
	}

	GLOBAL_INT_DISABLE();
	uvc_video_config->rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
}


static bk_err_t uvc_dma_config(void)
{
	bk_err_t ret = kNoErr;


	uvc_dma_channel = bk_dma_alloc(DMA_DEV_USB);
	if ((uvc_dma_channel < DMA_ID_0) || (uvc_dma_channel >= DMA_ID_MAX))
	{
		LOGE("malloc dma fail \r\n");
		ret = BK_UVC_NO_RESOURCE;
		return ret;
	}
	LOGI("uvc_dma id:%d\r\n", uvc_dma_channel);

	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;//DMA_DEV_USB;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = USB_UVC_FIFO_ADDR;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.start_addr = (uint32_t)uvc_video_config->rxbuf;
	dma_config.dst.end_addr = (uint32_t)(uvc_video_config->rxbuf + uvc_video_config->rxbuf_len);

	BK_LOG_ON_ERR(bk_dma_init(uvc_dma_channel, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(uvc_dma_channel, uvc_video_config->node_len));
	BK_LOG_ON_ERR(bk_dma_register_isr(uvc_dma_channel, NULL, uvc_camera_dma_finish_callback));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(uvc_dma_channel));
#if (CONFIG_GENERAL_DMA_SEC)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(uvc_dma_channel, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(uvc_dma_channel, DMA_ATTR_SEC));
#endif

	return ret;
}

#if 0
/* if uvc disconnect sudden, try reset uvc and open uvc again*/
static void uvc_camera_intf_retry_init(void)
{
	// try reset 3 times
	uint8_t count = 3;
	int ret = BK_OK;

	do {
		count--;

		ret = _bk_uvc_camera_init(uvc_video_config);
		if (ret == BK_OK)
			break;
	} while (count > 0 && ret != BK_OK);

	LOGI("%s complete, ret:%d, count:%d\r\n", __func__, ret, count);
}
#endif

static void uvc_camera_packet_node_callback(uint8_t *addr, uint32_t len)
{
	uint32_t left_len = 0;

	bk_dma_set_src_start_addr(uvc_dma_channel, (uint32_t)addr);

	uvc_video_config->node_len = len;

	if (uvc_video_config->rx_read_len & 0x3)
	{
		uvc_video_config->rx_read_len = ((uvc_video_config->rx_read_len >> 2) + 1 ) << 2;
	}

	left_len = uvc_video_config->rxbuf_len - uvc_video_config->rx_read_len;
	if (left_len < len)
	{
		uvc_video_config->rx_read_len = 0;
	}

	uint32_t dest_start_addr = (uint32_t)(uvc_video_config->rxbuf + uvc_video_config->rx_read_len);
	bk_dma_set_dest_start_addr(uvc_dma_channel, dest_start_addr);
	bk_dma_set_transfer_len(uvc_dma_channel, uvc_video_config->node_len);
	bk_dma_start(uvc_dma_channel);
}


static void uvc_camera_connect_state_change_cb(uvc_state_t state)
{
	LOGI("%s %d\r\n", __func__, state);

#if 0
	if (state == UVC_CONNECTED)
	{
		if (uvc_open)
			return;

		_bk_uvc_camera_init(uvc_video_config);
	}
	else if (state == UVC_DISCONNECT_ABNORMAL)
	{
		if (uvc_open)
		{
			_bk_uvc_camera_deinit();

			uvc_camera_intf_retry_init();
		}
	}
#else
	if (state == UVC_DISCONNECT_ABNORMAL)
	{
		uvc_open = false;
		_bk_uvc_camera_deinit();
	}
#endif
}

bk_err_t _bk_uvc_camera_init(void *data)
{
	int ret = kNoErr;

	uvc_video_config = (video_config_t *)data;

	uvc_config.type = uvc_video_config->device->camera_type;
	uvc_config.device.width = uvc_video_config->device->ppi >> 16;
	uvc_config.device.height = uvc_video_config->device->ppi & 0xFFFF;

	switch (uvc_video_config->device->fps)
	{
		case FPS5:
			uvc_config.device.fps = 5;
			break;

		case FPS10:
			uvc_config.device.fps = 10;
			break;

		case FPS15:
			uvc_config.device.fps = 15;
			break;

		case FPS20:
			uvc_config.device.fps = 20;
			break;

		case FPS25:
			uvc_config.device.fps = 25;
			break;

		case FPS30:
			uvc_config.device.fps = 30;
			break;
		default:
			uvc_config.device.fps = 20;
	}

	uvc_config.uvc_packet_rx_cb = uvc_camera_packet_node_callback;
	uvc_config.connect_state_change_cb = uvc_camera_connect_state_change_cb;

	uvc_dma_config();

	LOGI("%s, type:%d, ppi:%d-%d, fps:%d\r\n", __func__, uvc_config.type, uvc_config.device.width,
		uvc_config.device.height, uvc_config.device.fps);

	uvc_open = true;

	ret = bk_uvc_camera_init(&uvc_config);
	if (ret != BK_OK)
	{
		LOGE("init fail\r\n");
		goto error;
	}

	ret = bk_uvc_camera_start();
	if (ret != BK_OK)
	{
		LOGE("start fail\r\n");
		goto error;
	}

	uvc_open = true;

	return ret;

error:

	bk_dma_stop(uvc_dma_channel);
	bk_dma_deinit(uvc_dma_channel);
	bk_dma_free(DMA_DEV_USB, uvc_dma_channel);
	uvc_dma_channel = DMA_ID_MAX;
	uvc_open = false;

	return ret;
}

bk_err_t _bk_uvc_camera_deinit(void)
{
	int ret = BK_OK;

	if (uvc_open)
		ret = bk_uvc_camera_deinit();

	bk_dma_stop(uvc_dma_channel);
	bk_dma_deinit(uvc_dma_channel);
	bk_dma_free(DMA_DEV_USB, uvc_dma_channel);
	uvc_dma_channel = DMA_ID_MAX;

	uvc_video_config->rx_read_len = 0;

	os_memset(&uvc_config, 0, sizeof(uvc_config_t));
	uvc_frame_len = 0;
	uvc_drop_frame = 0;
	uvc_open = false;

	return ret;
}
