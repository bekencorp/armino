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
#include <components/log.h>
#include <components/video_transfer.h>
#include "media_core.h"
#include <os/mem.h>
#include <driver/media_types.h>

#include "transfer_act.h"
#include "media_evt.h"
#include "frame_buffer.h"

#define TAG "usb_major"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static beken_queue_t usb_major_msg_que = NULL;
static beken_thread_t usb_major_task = NULL;
static media_mailbox_msg_t *usb_major_node = NULL;

static bool usb_major_task_running = false;
static usb_trs_info_t usb_trs_info;

extern media_debug_t *media_debug;

static bk_err_t usb_major_task_send_msg(uint8_t type, uint32_t data)
{
	bk_err_t ret;
	trs_task_msg_t msg;

	if (usb_major_msg_que)
	{
		msg.type = type;
		msg.data = data;

		ret = rtos_push_to_queue(&usb_major_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			LOGE("usb_major_msg_que failed, type:%d\r\n", type);
			return kNoResourcesErr;
		}

		return ret;
	}
	return kGeneralErr;
}

static void usb_major_task_transfer_start_handle(uint32_t param)
{
	LOGI("%s, %d\r\n", __func__, param);

	usb_trs_info.param = param;
	frame_buffer_fb_register(MODULE_WIFI, usb_trs_info.param);
	usb_major_task_send_msg(TRS_USB_DATA, 0);
}

static void usb_major_task_transfer_data_handle(uint32_t param)
{
	frame_buffer_t *encode_frame = NULL;

	usb_major_task_running = true;

	while (usb_major_task_running)
	{
		if (usb_trs_info.param == FB_INDEX_JPEG)
		{
			encode_frame = frame_buffer_fb_read(MODULE_WIFI);
		}
		else
		{
			encode_frame = frame_buffer_fb_h264_pop_wait();
		}

		if (encode_frame == NULL)
		{
			LOGE("read frame NULL\n");
			continue;
		}
		else
		{
			// uint8_t *a = encode_frame->frame;
			// os_printf("%02x %02x %02x %02x %02x %02x %02x %02x \r\n",*a,*(a+1),*(a+2),*(a+3),*(a+4),*(a+5),*(a+6),*(a+7));
            // os_printf("len %d \r\n", encode_frame->length);
		}

		media_debug->wifi_read++;
		media_debug->fps_wifi++;

		// send msg to cpu0
		usb_major_node->event = EVENT_USB_DATA_NOTIFY;
		usb_major_node->param = (uint32_t)encode_frame;
		msg_send_req_to_media_major_mailbox_sync(usb_major_node, APP_MODULE);

		if (usb_trs_info.param == FB_INDEX_JPEG)
		{
			frame_buffer_fb_free(encode_frame, MODULE_WIFI);
		}
		else
		{
			frame_buffer_fb_h264_free(encode_frame);
		}
	}
}

static void usb_major_task_exit_handle(uint32_t param)
{
	media_mailbox_msg_t *msg = (media_mailbox_msg_t *)param;

	if (usb_major_node)
	{
		if(usb_major_node->sem)
		{
			rtos_deinit_semaphore(&usb_major_node->sem);
			usb_major_node->sem = NULL;
		}

		os_free(usb_major_node);
		usb_major_node = NULL;
	}

	frame_buffer_fb_deregister(MODULE_WIFI);

	set_transfer_state(TRS_STATE_DISABLED);

	msg_send_rsp_to_media_major_mailbox(msg, BK_OK, APP_MODULE);
}

static void usb_major_task_entry(beken_thread_arg_t data)
{
	int ret = kNoErr;

	media_debug->wifi_read = 0;
	media_debug->transfer_timer_us = 50000;

	while (1)
	{
		trs_task_msg_t msg;
		ret = rtos_pop_from_queue(&usb_major_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			switch (msg.type)
			{
				case TRS_USB_START:
					usb_major_task_transfer_start_handle(msg.data);
					break;

				case TRS_USB_DATA:
					usb_major_task_transfer_data_handle(msg.data);
					break;

				case TRS_USB_EXIT:
					usb_major_task_exit_handle(msg.data);
					goto exit;
					break;

				default:
					break;
			}
		}
	};

exit:

	LOGI("usb_major_task task exit\n");

	rtos_deinit_queue(&usb_major_msg_que);
	usb_major_msg_que = NULL;

	usb_major_task = NULL;
	rtos_delete_thread(NULL);
}

static bk_err_t usb_major_task_init(void)
{
	bk_err_t ret = BK_FAIL;

	if (usb_major_node == NULL)
	{
		usb_major_node = (media_mailbox_msg_t *)os_malloc(sizeof(media_mailbox_msg_t));
		if (usb_major_node != NULL)
		{
			ret = rtos_init_semaphore_ex(&usb_major_node->sem, 1, 0);

			if (ret != kNoErr)
			{
				LOGE("%s init semaphore failed\n", __func__);
				goto error;
			}
		}
	}

	if ((!usb_major_task) && (!usb_major_msg_que))
	{
		ret = rtos_init_queue(&usb_major_msg_que,
								"usb_major_msg_que",
								sizeof(trs_task_msg_t),
								10);
		if (kNoErr != ret)
		{
			LOGE("usb_major_msg_que init failed\n");
			ret = kNoMemoryErr;
			goto error;
		}

		ret = rtos_create_thread(&usb_major_task,
								BEKEN_DEFAULT_WORKER_PRIORITY,
								"usb_major_task",
								(beken_thread_function_t)usb_major_task_entry,
								1024,
								NULL);

		if (BK_OK != ret)
		{
			LOGE("usb_major_task init failed\n");
			ret = kNoMemoryErr;
			goto error;
		}
	}

	return ret;

error:

	if (usb_major_node)
	{
		if(usb_major_node->sem)
		{
			rtos_deinit_semaphore(&usb_major_node->sem);
			usb_major_node->sem = NULL;
		}

		os_free(usb_major_node);
		usb_major_node = NULL;
	}

	if (usb_major_msg_que)
	{
		rtos_deinit_queue(&usb_major_msg_que);
		usb_major_msg_que = NULL;
	}

	if (usb_major_task)
	{
		usb_major_task = NULL;
	}

	return ret;
}

static bk_err_t usb_major_task_open_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;
	if (get_usb_trs_state() == TRS_STATE_ENABLED)
	{
		LOGI("usb_major_task have been opened!\r\n");
		goto end;
	}

	ret = usb_major_task_init();
	if (ret == BK_OK)
	{
		usb_major_task_send_msg(TRS_USB_START, msg->param);

		set_usb_trs_state(TRS_STATE_ENABLED);
	}

end:

	msg_send_rsp_to_media_major_mailbox(msg, BK_OK, APP_MODULE);

	LOGI("%s complete\n", __func__);

	return ret;
}

static bk_err_t usb_major_task_close_handle(media_mailbox_msg_t *msg)
{
	LOGI("%s\n", __func__);

	if (usb_major_task_running == false || get_transfer_state() == TRS_STATE_DISABLED)
	{
		LOGI("transfer_major_task have closed!\r\n");
		msg_send_rsp_to_media_major_mailbox(msg, BK_OK, APP_MODULE);
	}

	usb_major_task_running = false;

	usb_major_task_send_msg(TRS_USB_EXIT, (uint32_t)msg);

	return BK_OK;
}


bk_err_t usb_major_event_handle(media_mailbox_msg_t *msg)
{
	int ret = BK_OK;

	switch (msg->event)
	{
		case EVENT_TRANSFER_USB_OPEN_IND:
			ret = usb_major_task_open_handle(msg);
			break;
		case EVENT_TRANSFER_USB_CLOSE_IND:
			ret = usb_major_task_close_handle(msg);
			break;

		default:
			break;
	}

	return ret;
}

media_trs_state_t get_usb_trs_state(void)
{
	return usb_trs_info.state;
}

void set_usb_trs_state(media_trs_state_t state)
{
	usb_trs_info.state = state;
}

void usb_transfer_init(void)
{
	usb_trs_info.state = TRS_STATE_DISABLED;
	usb_trs_info.debug = false;
}
