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
#include <os/mem.h>
#include <components/log.h>
#include <components/video_types.h>

#include "media_core.h"
#include "camera_act.h"
#include "frame_buffer.h"
#include <driver/net_camera_types.h>
#include <driver/dma.h>
#include "bk_general_dma.h"

#define TAG "net_camera"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

typedef struct {
	uint8_t last_id;
	uint8_t recv_state;
} net_camera_param_t;

net_camera_buffer_t *net_camera_buf = NULL;
net_camera_param_t net_camera_param = {0};

const net_camera_config_t net_camera_config =
{
	.fb_jpeg_init = frame_buffer_fb_jpeg_init,
	.fb_jpeg_deinit = frame_buffer_fb_jpeg_deinit,
	.fb_jpeg_complete = frame_buffer_fb_push,
	.fb_jpeg_malloc = frame_buffer_fb_jpeg_malloc,
	.fb_jpeg_free = frame_buffer_fb_jpeg_free,
	.receive_jpeg = NULL,
};


bk_err_t bk_net_camera_open(void)
{
	//step 1: init lcd, shou do it after calling this api
	if (net_camera_buf == NULL)
	{
		net_camera_buf = (net_camera_buffer_t *)os_malloc(sizeof(net_camera_buffer_t));
		if (net_camera_buf == NULL)
		{
			LOGE("malloc net_camera_buf failed\r\n");
			return BK_FAIL;
		}

		net_camera_buf->start_buf = BUF_STA_INIT;
	}

	os_memset(net_camera_buf, 0, sizeof(net_camera_buffer_t));

	net_camera_config.fb_jpeg_init((uint32_t)(PPI_640X480));

	net_camera_buf->frame = net_camera_config.fb_jpeg_malloc();
	if (net_camera_buf->frame == NULL)
	{
		return BK_FAIL;
	}

	net_camera_buf->frame->fmt = PIXEL_FMT_UVC_JPEG; //all set uvc_jpeg, because jpeg need jepg decode

	net_camera_buf->buf_ptr = net_camera_buf->frame->frame;
	net_camera_buf->frame_pkt_cnt = 0;

	net_camera_buf->dma_id = bk_dma_alloc(DMA_DEV_JPEG);
	if ((net_camera_buf->dma_id < DMA_ID_0) || (net_camera_buf->dma_id >= DMA_ID_MAX))
	{
		LOGE("malloc net_camera_buf->dma_id fail \r\n");
		net_camera_buf->dma_id = DMA_ID_MAX;
	}

	LOGI("net_camera_buf->dma_id:%d\r\n", net_camera_buf->dma_id);

	net_camera_param.recv_state = BUF_STA_INIT;

	return BK_OK;
}

bk_err_t bk_net_camera_close(void)
{
	if (net_camera_param.recv_state == BUF_STA_COPY)
	{
		net_camera_param.recv_state = BUF_STA_DEINIT;
	}

	rtos_delay_milliseconds(200);

	if (net_camera_buf)
	{
		net_camera_config.fb_jpeg_free(net_camera_buf->frame);

		if (net_camera_buf->dma_id != DMA_ID_MAX)
		{
			bk_dma_stop(net_camera_buf->dma_id);
			bk_dma_deinit(net_camera_buf->dma_id);
			bk_dma_free(DMA_DEV_JPEG, net_camera_buf->dma_id);
		}

		net_camera_buf->frame = NULL;
		net_camera_buf->buf_ptr = NULL;
		os_free(net_camera_buf);
		net_camera_buf = NULL;
	}

	LOGI("%s complete\r\n", __func__);

	return BK_OK;
}

uint32_t bk_net_camera_receive_data(uint8_t *data, uint32_t length)
{
	uint32_t fack_len = 0;


	if ((net_camera_param.recv_state == BUF_STA_INIT || net_camera_param.recv_state == BUF_STA_COPY) && length > 4)
	{
		video_header_t *hdr = (video_header_t *)data;
		uint32_t org_len;
//		GLOBAL_INT_DECLARATION();

		org_len = length - sizeof(video_header_t);

		if (org_len & 0x3)
		{
			fack_len = ((org_len >> 2) + 1) << 2;
		}

		data = data + sizeof(video_header_t);

		//LOGI("hdr->id:%d, last_id:%d, recv_state:%d\r\n", hdr->id, net_camera_param.last_id, net_camera_param.recv_state);

		if (hdr->id != net_camera_param.last_id)
		{
			if (net_camera_param.recv_state == BUF_STA_COPY)
			{
				net_camera_config.fb_jpeg_complete(net_camera_buf->frame);

				net_camera_buf->frame = net_camera_config.fb_jpeg_malloc();
				if (net_camera_buf->frame == NULL)
				{
					LOGE("frame buffer malloc failed\r\n");
					return BK_FAIL;
				}

				net_camera_buf->frame->fmt = PIXEL_FMT_UVC_JPEG; //all set uvc_jpeg, because jpeg need jepg decode
				net_camera_buf->buf_ptr = net_camera_buf->frame->frame;
			}
			else
			{
				net_camera_param.recv_state = BUF_STA_COPY;
			}
			
		}

		if (net_camera_buf->dma_id != DMA_ID_MAX)
		{
			dma_memcpy_by_chnl(net_camera_buf->buf_ptr, data, fack_len ? fack_len : org_len, net_camera_buf->dma_id);
		}
		else
		{
			os_memcpy(net_camera_buf->buf_ptr, data, fack_len ? fack_len : org_len);
		}

		net_camera_buf->frame->length += org_len;
		net_camera_buf->buf_ptr += org_len;

		net_camera_param.last_id = hdr->id;
	}

	return length;
}

