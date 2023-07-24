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
#include <driver/media_co_list.h>
#include "bk_general_dma.h"

#define TAG "net_camera"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define NET_CAMERA_POOL_LEN             (1472 * 35)
#define NET_CAMERA_RXNODE_SIZE          1472

typedef struct {
	struct co_list_hdr hdr;
	void *buf_start;
	uint32_t buf_len;
} net_camera_elem_t;

typedef struct {
	uint8_t *pool;
	net_camera_elem_t elem[NET_CAMERA_POOL_LEN / NET_CAMERA_RXNODE_SIZE];
	struct co_list free;
	struct co_list ready;
} net_camera_pool_t;

typedef struct {
	media_ppi_t ppi;
	pixel_format_t fmt;
	video_send_type_t send_type;
} net_camera_param_t;

beken_thread_t net_camera_task = NULL;
bool net_camera_task_running = false;

net_camera_buffer_t *net_camera_buf = NULL;
net_camera_param_t net_camera_param = {0};
net_camera_pool_t net_camera_pool;
static beken_semaphore_t s_recv_video_data_sem;

const net_camera_config_t net_camera_config =
{
	.fb_jpeg_init = frame_buffer_fb_jpeg_init,
	.fb_jpeg_deinit = frame_buffer_fb_jpeg_deinit,
	.fb_jpeg_complete = frame_buffer_fb_push,
	.fb_jpeg_malloc = frame_buffer_fb_jpeg_malloc,
	.fb_jpeg_free = frame_buffer_fb_jpeg_free,
	.receive_jpeg = NULL,
};

void printf_receive_data_head(uint8_t *data, uint32_t length)
{
	if (length >= 6)
		LOGI("%02x %02x %02x %02x %02x %02x %d\r\n", data[0], data[1], data[2], data[3], data[4], data[5], length);
}

uint32_t bk_net_tcp_checkout_eof(uint8_t *data, uint32_t length)
{
	uint32_t i =0;
	for (i = 0; i < length - 1; i++)
	{
		if (data[i] == 0xFF && data[i + 1] == 0xD9)
		{
			break;
		}
	}

	return (i + 2);
}

static bk_err_t net_camera_free_memory(void)
{
	if (net_camera_buf)
	{
		if (net_camera_buf->dma_id != DMA_ID_MAX)
		{
			bk_dma_stop(net_camera_buf->dma_id);
			bk_dma_deinit(net_camera_buf->dma_id);
			bk_dma_free(DMA_DEV_JPEG, net_camera_buf->dma_id);
		}

		if (net_camera_buf->dma_psram != DMA_ID_MAX)
		{
			bk_dma_stop(net_camera_buf->dma_psram);
			bk_dma_deinit(net_camera_buf->dma_psram);
			bk_dma_free(DMA_DEV_DTCM, net_camera_buf->dma_psram);
		}

		if (net_camera_buf->frame)
		{
			net_camera_config.fb_jpeg_free(net_camera_buf->frame);
		}

		net_camera_buf->frame = NULL;
		net_camera_buf->buf_ptr = NULL;
		os_free(net_camera_buf);
		net_camera_buf = NULL;
	}

	if (net_camera_pool.pool)
	{
		os_free(net_camera_pool.pool);
		net_camera_pool.pool = NULL;
	}

	return BK_OK;
}

static bk_err_t net_camera_init_pool(void)
{
	if (net_camera_pool.pool == NULL)
	{
		net_camera_pool.pool = os_malloc(NET_CAMERA_POOL_LEN);
		if (net_camera_pool.pool == NULL)
		{
			LOGE("tvideo_pool alloc failed\r\n");
			return kNoMemoryErr;
		}
	}

	os_memset(&net_camera_pool.pool[0], 0, NET_CAMERA_POOL_LEN);

	co_list_init(&net_camera_pool.free);
	co_list_init(&net_camera_pool.ready);

	for (uint8_t i = 0; i < (NET_CAMERA_POOL_LEN / NET_CAMERA_RXNODE_SIZE); i++) {
		net_camera_pool.elem[i].buf_start =
			(void *)&net_camera_pool.pool[i * NET_CAMERA_RXNODE_SIZE];
		net_camera_pool.elem[i].buf_len = 0;

		co_list_push_back(&net_camera_pool.free,
			(struct co_list_hdr *)&net_camera_pool.elem[i].hdr);
	}

	return BK_OK;
}

static void net_camera_process_packet(uint8_t *data, uint32_t length)
{
	if ((net_camera_buf->start_buf == BUF_STA_INIT || net_camera_buf->start_buf == BUF_STA_COPY) && net_camera_buf->frame)
	{
		video_header_t *hdr = (video_header_t *)data;
		uint32_t org_len;
		uint32_t fack_len = 0;
		GLOBAL_INT_DECLARATION();

		org_len = length - sizeof(video_header_t);

		data = data + sizeof(video_header_t);

		//printf_receive_data_head(data, org_len);

		if ((hdr->id != net_camera_buf->frame->sequence) && (hdr->pkt_cnt == 1)) {
			// start of frame;
			GLOBAL_INT_DISABLE();
			net_camera_buf->frame->sequence = hdr->id;
			net_camera_buf->frame->length = 0;
			net_camera_buf->frame_pkt_cnt = 0;
			net_camera_buf->buf_ptr = net_camera_buf->frame->frame;
			net_camera_buf->start_buf = BUF_STA_COPY;
			GLOBAL_INT_RESTORE();
			//LOGI("sof:%d\r\n", net_camera_buf->frame->sequence);
		}
		else
		{
			if (net_camera_buf->start_buf == BUF_STA_INIT)
				net_camera_buf->start_buf = BUF_STA_COPY;
		}

		/*LOGI("hdr-id:%d-%d, frame_packet_cnt:%d-%d, state:%d\r\n", hdr->id, net_camera_buf->frame->sequence,
			(net_camera_buf->frame_pkt_cnt + 1), hdr->pkt_cnt, net_camera_buf->start_buf);*/

		if ((hdr->id == net_camera_buf->frame->sequence)
			&& ((net_camera_buf->frame_pkt_cnt + 1) == hdr->pkt_cnt)
			&& (net_camera_buf->start_buf == BUF_STA_COPY))
		{
			if (net_camera_param.send_type == TVIDEO_SND_TCP && hdr->is_eof == 1)
			{
				org_len = bk_net_tcp_checkout_eof(data, org_len);

				if (org_len & 0x3)
				{
					fack_len = ((org_len >> 2) + 1) << 2;
				}
			}

			if (fack_len == 0)
			{
				if (org_len & 0x3)
					fack_len = ((org_len >> 2) + 1) << 2;
			}

			if (net_camera_buf->dma_psram != DMA_ID_MAX)
			{
				dma_memcpy_by_chnl(net_camera_buf->buf_ptr, data, fack_len ? fack_len : org_len, net_camera_buf->dma_id);
			}
			else
			{
				os_memcpy(net_camera_buf->buf_ptr, data, fack_len ? fack_len : org_len);
			}

			GLOBAL_INT_DISABLE();
			net_camera_buf->frame->length += org_len;
			net_camera_buf->buf_ptr += org_len;
			net_camera_buf->frame_pkt_cnt += 1;
			GLOBAL_INT_RESTORE();

			if (hdr->is_eof == 1)
			{
				net_camera_config.fb_jpeg_complete(net_camera_buf->frame);

				net_camera_buf->frame = net_camera_config.fb_jpeg_malloc();
				if (net_camera_buf->frame == NULL)
				{
					LOGE("frame buffer malloc failed\r\n");
					return;
				}

				net_camera_buf->frame->width = ppi_to_pixel_x(net_camera_param.ppi);
				net_camera_buf->frame->height = ppi_to_pixel_y(net_camera_param.ppi);


				net_camera_buf->frame->fmt = net_camera_param.fmt; //all set uvc_jpeg, because jpeg need jepg decode
				net_camera_buf->buf_ptr = net_camera_buf->frame->frame;
				net_camera_buf->frame->length = 0;
			}
		}

	}
}

static void net_camera_task_entry(beken_thread_arg_t data)
{
	net_camera_elem_t *elem = NULL;
    bk_err_t err = 0;

	net_camera_buf->start_buf = BUF_STA_INIT;
	net_camera_task_running = true;

	while (net_camera_task_running)
	{
	    err = rtos_get_semaphore(&s_recv_video_data_sem, 1000);

	    if(!net_camera_task_running)
	    {
	        break;
	    }

	    if(err != 0)
	    {
	        LOGD("%s get sem timeout\n", __func__);
	        continue;
	    }

		while((elem = (net_camera_elem_t *)co_list_pick(&net_camera_pool.ready)) != NULL)
		{
            net_camera_process_packet(elem->buf_start, elem->buf_len);

            co_list_pop_front(&net_camera_pool.ready);
            co_list_push_back(&net_camera_pool.free, (struct co_list_hdr *)&elem->hdr);
		}
	};

	rtos_deinit_semaphore(&s_recv_video_data_sem);
	net_camera_task = NULL;
	rtos_delete_thread(NULL);

	net_camera_free_memory();
}


bk_err_t bk_net_camera_open(media_ppi_t ppi, media_camera_type_t type)
{
	int ret = BK_OK;
	//step 1: init lcd, should do it after calling this api
	if (net_camera_buf == NULL)
	{
		net_camera_buf = (net_camera_buffer_t *)os_malloc(sizeof(net_camera_buffer_t));
		if (net_camera_buf == NULL)
		{
			LOGE("malloc net_camera_buf failed\r\n");
			goto error;
		}

		//net_camera_buf->start_buf = BUF_STA_INIT;
	}

	os_memset(net_camera_buf, 0, sizeof(net_camera_buffer_t));

	net_camera_buf->dma_id = bk_dma_alloc(DMA_DEV_JPEG);
	if ((net_camera_buf->dma_id < DMA_ID_0) || (net_camera_buf->dma_id >= DMA_ID_MAX))
	{
		LOGE("malloc net_camera_buf->dma_id fail \r\n");
		net_camera_buf->dma_id = DMA_ID_MAX;
	}

	net_camera_buf->dma_psram = bk_dma_alloc(DMA_DEV_DTCM);
	if ((net_camera_buf->dma_psram < DMA_ID_0) || (net_camera_buf->dma_psram >= DMA_ID_MAX))
	{
		LOGE("malloc net_camera_buf->dma_id fail \r\n");
		net_camera_buf->dma_psram = DMA_ID_MAX;
	}

	LOGI("net_camera_buf->dma_id:%d-%d\r\n", net_camera_buf->dma_id, net_camera_buf->dma_psram);

	net_camera_config.fb_jpeg_init((uint32_t)(ppi));

	net_camera_buf->frame = net_camera_config.fb_jpeg_malloc();
	if (net_camera_buf->frame == NULL)
	{
		goto error;
	}

	if (type == MEDIA_UVC_MJPEG || type == MEDIA_DVP_MJPEG)
		net_camera_buf->frame->fmt = PIXEL_FMT_UVC_JPEG; //all set uvc_jpeg, because jpeg need jpeg decode
	else if (type == MEDIA_UVC_H264)
		net_camera_buf->frame->fmt = PIXEL_FMT_UVC_H264; //all set uvc_jpeg, because jpeg need h264 decode
	else
		return BK_FAIL;

	net_camera_buf->buf_ptr = net_camera_buf->frame->frame;
	net_camera_buf->frame_pkt_cnt = 0;
	net_camera_buf->frame->width = ppi_to_pixel_x(ppi);
	net_camera_buf->frame->height = ppi_to_pixel_y(ppi);
	net_camera_buf->frame->sequence = 0;
	net_camera_buf->start_buf = BUF_STA_INIT;

	net_camera_param.ppi = ppi;
	net_camera_param.fmt = net_camera_buf->frame->fmt;
	net_camera_param.send_type = TVIDEO_SND_TCP;

	ret = net_camera_init_pool();
	if (ret != BK_OK)
	{
		goto error;
	}

	ret = rtos_init_semaphore(&s_recv_video_data_sem, 1);

    if (BK_OK != ret)
    {
        LOGE("%s semaphore init failed\n", __func__);
        goto error;
    }

	ret = rtos_create_thread(&net_camera_task,
	                         6,
	                         "net_camera_task",
	                         (beken_thread_function_t)net_camera_task_entry,
	                         4 * 1024,
	                         NULL);

	if (BK_OK != ret)
	{
		LOGE("%s transfer_task init failed\n", __func__);
		goto error;
	}

	return BK_OK;

error:

	LOGE("%s failed\n", __func__);

	net_camera_free_memory();
	return BK_FAIL;
}

bk_err_t bk_net_camera_close(void)
{
	if (net_camera_buf->start_buf == BUF_STA_COPY)
	{
		net_camera_buf->start_buf = BUF_STA_DEINIT;
	}

	net_camera_task_running = false;

	rtos_delay_milliseconds(100);

	net_camera_free_memory();

	LOGI("%s complete\r\n", __func__);

	return BK_OK;
}

uint32_t bk_net_send_data(uint8_t *data, uint32_t length, video_send_type_t type)
{
	net_camera_elem_t *elem = NULL;

	if (net_camera_param.send_type != type)
		net_camera_param.send_type = type;
	if (length <= 4)
	{
		return length;
	}

	elem = (net_camera_elem_t *)co_list_pick(&net_camera_pool.free);
	if (elem)
	{
		if (net_camera_buf->dma_id != DMA_ID_MAX)
		{
			dma_memcpy_by_chnl(elem->buf_start, data, length, net_camera_buf->dma_id);
		}
		else
		{
			os_memcpy(elem->buf_start, data, length);
		}

		elem->buf_len = length;

		co_list_pop_front(&net_camera_pool.free);
		co_list_push_back(&net_camera_pool.ready, (struct co_list_hdr *)&elem->hdr);
		rtos_set_semaphore(&s_recv_video_data_sem);
	}
	else
	{
		LOGI("list all busy\r\n");
	}

	return length;
}

