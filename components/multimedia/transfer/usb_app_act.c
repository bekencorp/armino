// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
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
#include "frame_buffer.h"
#include "media_evt.h"
#include "transfer_act.h"
#include "media_mailbox_list_util.h"

#if (CONFIG_CACHE_ENABLE)
#include "cache.h"
#endif

#define TAG "usb_app_transfer"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define MAX_TX_SIZE (1472)
#define MAX_COPY_SIZE (1472 - sizeof(usb_data_t))
#define MAX_RETRY (10000)
#define RETRANSMITS_TIME (5)

typedef struct {
	uint8_t id;
	uint8_t eof;
	uint8_t cnt;
	uint8_t size;
	uint8_t data[];
} usb_data_t;

static beken_semaphore_t usb_app_sem;
static beken_thread_t usb_app_task = NULL;
static beken_queue_t usb_app_msg_que = NULL;
static usb_data_t *usb_data = NULL;
static bool usb_app_task_running = false;
static frame_buffer_t *current_frame = NULL;
static media_mailbox_msg_t *usb_app_node = NULL;

extern void usbd_video_open(uint8_t intf);
extern void usbd_video_send_common_frame(uint8_t *data, uint32_t size);
extern void os_memcpy_word_reverse(uint32_t *dst, uint32_t *src, uint32_t size);

static void video_frame_handle(frame_buffer_t *buffer)
{
	//LOGI("seq: %u, length: %u, size: %u\n", buffer->sequence, buffer->length, buffer->size);
	if (buffer->fmt == PIXEL_FMT_DVP_H264)
	{
		os_memcpy_word_reverse((uint32_t *)buffer->frame, (uint32_t *)buffer->frame, buffer->length);
		usbd_video_send_common_frame(buffer->frame, buffer->length);
		/* send h264 I frame again, otherwise pc decode failed */
		if (buffer->sequence == 1)
		{
			usbd_video_send_common_frame(buffer->frame, buffer->length);
		}
	}
	else if (buffer->fmt == PIXEL_FMT_DVP_JPEG)
	{
		usbd_video_send_common_frame(buffer->frame, buffer->length);
	}
}

static void usb_app_task_send_handle(frame_buffer_t *frame)
{
	if(current_frame)
	{
#if (CONFIG_CACHE_ENABLE)
		flush_dcache(current_frame->frame, current_frame->size);
#endif
		video_frame_handle(current_frame);
		current_frame = NULL;

		msg_send_rsp_to_media_app_mailbox(usb_app_node, BK_OK);
	}
}

static void usb_app_task_entry(beken_thread_arg_t data)
{
	rtos_set_semaphore(&usb_app_sem);
	usbd_video_open(0);

	while (1) {
		trs_task_msg_t msg;
		int ret = rtos_pop_from_queue(&usb_app_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			switch (msg.type)
			{
				case TRS_USB_DATA:
					usb_app_task_send_handle((frame_buffer_t *)msg.data);
					break;

				case TRS_USB_EXIT:
					goto exit;
					break;

				default:
					break;
			}
		}
	}

exit:

	LOGI("usb transfer task exit\n");

	if(usb_data)
	{
		os_free(usb_data);
		usb_data = NULL;
	}

	current_frame = NULL;
	usb_app_node = NULL;

	usb_app_task_running = false;

	rtos_deinit_queue(&usb_app_msg_que);
	usb_app_msg_que = NULL;

	usb_app_task = NULL;
	rtos_delete_thread(NULL);
}

bk_err_t usb_app_task_init(video_setup_t *config)
{
	bk_err_t ret = 0;

	if (usb_data == NULL) {
		usb_data = (usb_data_t *)os_malloc(sizeof(usb_data_t) + MAX_COPY_SIZE);
		if (usb_data == NULL) {
			LOGE("%s usb_data malloc failed\n", __func__);
			return BK_ERR_NO_MEM;
		}
	}

	ret = rtos_init_semaphore_ex(&usb_app_sem, 1, 0);
	if (BK_OK != ret) {
		LOGE("%s semaphore init failed\n", __func__);
		return ret;
	}

	if((!usb_app_task) && (!usb_app_msg_que)) {
		usb_app_task_running = true;

		ret = rtos_init_queue(&usb_app_msg_que,
								"usb_app_msg_que",
								sizeof(trs_task_msg_t),
								20);
		if (BK_OK != ret)
		{
			LOGE("%s usb_app_task init failed\n");
			ret = BK_ERR_NO_MEM;
			goto error;
		}

		ret = rtos_create_thread(&usb_app_task,
								4,
								"usb_app_task",
								(beken_thread_function_t)usb_app_task_entry,
								4 * 1024,
								NULL);

		if (BK_OK != ret) {
			LOGE("%s usb_app_task init failed\n");
			return ret;
		}

		ret = rtos_get_semaphore(&usb_app_sem, BEKEN_NEVER_TIMEOUT);

		if (BK_OK != ret) {
			LOGE("%s usb_app_sem get failed\n", __func__);
		}
	}

	return BK_OK;

error:

	if (usb_app_msg_que)
	{
		rtos_deinit_queue(&usb_app_msg_que);
		usb_app_msg_que = NULL;
	}

	if (usb_app_task)
	{
		usb_app_task = NULL;
	}

	return ret;
}

static bk_err_t usb_app_task_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret = kNoErr;
	trs_task_msg_t msg;

	if (usb_app_msg_que)
	{
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&usb_app_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			LOGE("usb_app_task_send_msg failed, type:%d\r\n", type);
			return kNoResourcesErr;
		}

		return ret;
	}
	return kGeneralErr;
}

static bk_err_t usb_app_task_start_handle(media_mailbox_msg_t *mailbox_msg)
{
	if (usb_app_task_running)
	{
		usb_app_node = mailbox_msg;
		current_frame = (frame_buffer_t *)usb_app_node->param;
		usb_app_task_send_msg(TRS_USB_DATA, mailbox_msg->param);
	}
	else
	{
		LOGI("%s usb_app_task not start\r\n", __func__);
		msg_send_rsp_to_media_app_mailbox(mailbox_msg, BK_OK);
	}

	return kNoErr;
}

static bk_err_t usb_app_task_close_handle(media_mailbox_msg_t *mailbox_msg)
{
	if (usb_app_task_running)
	{
		usb_app_task_running = false;
		usb_app_task_send_msg(TRS_USB_EXIT, mailbox_msg->param);
	}
	else
	{
		LOGI("%s usb_app_task have closed\r\n", __func__);
	}

	return kNoErr;
}

bk_err_t usb_app_event_handle(media_mailbox_msg_t *msg)
{
	int ret = kNoErr;

	switch (msg->event)
	{
		case EVENT_USB_DATA_NOTIFY:
			ret = usb_app_task_start_handle(msg);
			break;

		case EVENT_TRANSFER_USB_CLOSE_IND:
			ret = usb_app_task_close_handle(msg);
			break;

		default:
			break;
	}

	return ret;
}

