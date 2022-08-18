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
#include <components/log.h>

#include "media_core.h"
#include "media_evt.h"
#include "camera_act.h"
#include "mailbox_act.h"
#include "aud_act.h"
#include "comm_act.h"
#include "lcd_act.h"
#include "transfer_act.h"
#include "storage_act.h"
#include "mailbox_channel.h"
#include "frame_buffer.h"
#include "media_app.h"
#include "mlist.h"


#define MJ_TAG "media0"

#define LOGI(...) BK_LOGI(MJ_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(MJ_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(MJ_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(MJ_TAG, ##__VA_ARGS__)

static beken_thread_t media_major_th_hd = NULL;
static beken_queue_t media_major_msg_queue = NULL;

#if 0
static mlist_t *mailbox_free_list = NULL;
static mlist_t *mailbox_msg_list = NULL;
bool mailbox_busy = false;
#define MAX_MAILBOX_QUEUE = 10;
#endif

extern uint32_t  platform_is_in_interrupt_context(void);

#ifdef CONFIG_DUAL_CORE
bk_err_t media_mailbox_send_msg(uint32_t cmd, uint32_t param1, uint32_t param2)
{
	mb_chnl_cmd_t mb_cmd;

	mb_cmd.hdr.cmd = 1;
	mb_cmd.param1 = cmd;
	mb_cmd.param2 = param1;
	mb_cmd.param3 = param2;
	return mb_chnl_write(MB_CHNL_MEDIA, &mb_cmd);
}

static void media_major_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
	LOGD("%s, %08X\n", __func__, cmd_buf->param1);

	switch (cmd_buf->param1 >> MEDIA_EVT_BIT)
	{
#ifdef CONFIG_CAMERA
		case MAILBOX_EVT:
		{
			media_msg_t msg;
			msg.event = cmd_buf->param1;
			msg.param = cmd_buf->param2;

			if (media_major_msg_queue)
			{
				bk_err_t ret;

				ret = rtos_push_to_queue(&media_major_msg_queue, &msg, BEKEN_NO_WAIT);

				if (BK_OK != ret)
				{
					LOGE("%s failed\n", __func__);
				}
			}

		}
		break;
#endif
		default:
			break;
	}
}

static void media_major_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);

}

static void media_major_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
	LOGD("%s\n", __func__);

}
#endif

bk_err_t media_send_msg(media_msg_t *msg)
{
	bk_err_t ret = BK_OK;

#ifdef CONFIG_DUAL_CORE

	if (msg->event >> MEDIA_EVT_BIT == MAILBOX_EVT)
	{
		LOGE("%s failed, error event\n", __func__);
		return BK_FAIL;
	}

	if (msg->event >> MEDIA_EVT_BIT == MAILBOX_CMD)
	{
#if 0
		if (platform_is_in_interrupt_context())
		{
			//LOGW("%s should not call form isr\n", __func__);
		}
#endif

		ret = media_mailbox_send_msg(msg->event, msg->param, 0);
		return ret;
	}
#endif

	if (media_major_msg_queue)
	{
		ret = rtos_push_to_queue(&media_major_msg_queue, msg, BEKEN_NO_WAIT);

		if (BK_OK != ret)
		{
			LOGE("%s failed\n", __func__);
			return BK_FAIL;
		}

		return ret;
	}
	return ret;
}



static void media_major_message_handle(void)
{
	bk_err_t ret = BK_OK;
	media_msg_t msg;

	frame_buffer_init();

#ifdef CONFIG_CAMERA
	dvp_camera_init();
#endif

#ifdef CONFIG_WIFI_TRANSFER
	trs_video_transfer_init();
#endif

#ifdef CONFIG_LCD
	lcd_init();
#endif

#ifdef CONFIG_CAMERA
	storage_init();
#endif

#ifdef CONFIG_USB_UVC
	uvc_camera_init();
#endif

	while (1)
	{

		ret = rtos_pop_from_queue(&media_major_msg_queue, &msg, BEKEN_WAIT_FOREVER);

		if (kNoErr == ret)
		{
			switch (msg.event >> MEDIA_EVT_BIT)
			{
				case COM_EVENT:
					comm_event_handle(msg.event, msg.param);
					break;

#ifdef CONFIG_CAMERA
				case DVP_EVENT:
					dvp_camera_event_handle(msg.event, msg.param);
					break;
#endif

#ifdef CONFIG_AUDIO
				case AUD_EVENT:
					audio_event_handle(msg.event, msg.param);
					break;
#endif

#ifdef CONFIG_LCD
				case LCD_EVENT:
					lcd_event_handle(msg.event, msg.param);
					break;
#endif

#ifdef CONFIG_WIFI_TRANSFER
				case TRS_EVENT:
					wifi_transfer_event_handle(msg.event, msg.param);
					break;
#endif

#ifdef CONFIG_CAMERA
				case STORAGE_EVENT:
					storage_event_handle(msg.event, msg.param);
					break;
#endif

#if CONFIG_USB_UVC
				case UVC_EVENT:
					uvc_camera_event_handle(msg.event, msg.param);
					break;
#endif

#ifdef CONFIG_DUAL_CORE
				case MAILBOX_EVT:
					mailbox_evt_handle(msg.event, msg.param);
					break;
#endif
				case QUEUE_EVENT:

					break;

				case EXIT_EVENT:
					goto exit;
					break;

				default:
					break;
			}
		}
	}

exit:

	frame_buffer_deinit();

	/* delate msg queue */
	ret = rtos_deinit_queue(&media_major_msg_queue);

	if (ret != kNoErr)
	{
		LOGE("delate message queue fail\n");
	}

	media_major_msg_queue = NULL;

	LOGE("delate message queue complete\n");

	/* delate task */
	rtos_delete_thread(NULL);

	media_major_th_hd = NULL;

	LOGE("delate task complete\n");
}


bk_err_t media_major_init(void)
{
	bk_err_t ret = BK_OK;

	if (media_major_msg_queue != NULL)
	{
		ret = kNoErr;
		LOGE("%s, media_major_msg_queue allready init, exit!\n");
		goto error;
	}

	if (media_major_th_hd != NULL)
	{
		ret = kNoErr;
		LOGE("%s, media_major_th_hd allready init, exit!\n");
		goto error;
	}

	ret = rtos_init_queue(&media_major_msg_queue,
	                      "media_major_queue",
	                      sizeof(media_msg_t),
	                      MEDIA_MAJOR_MSG_QUEUE_SIZE);

	if (ret != kNoErr)
	{
		LOGE("%s, ceate media major message queue failed\n");
		goto error;
	}

	ret = rtos_create_thread(&media_major_th_hd,
	                         BEKEN_DEFAULT_WORKER_PRIORITY,
	                         "media_major_thread",
	                         (beken_thread_function_t)media_major_message_handle,
	                         4096,
	                         NULL);

	if (ret != kNoErr)
	{
		LOGE("create media major thread fail\n");
		goto error;
	}

	media_app_init();

#ifdef CONFIG_DUAL_CORE

#if 0
	if (mailbox_free_list)
	{
		mailbox_free_list = mlist_new(NULL);

		for (int i = 0; i < MAX_MAILBOX_QUEUE; i ++)
		{
			media_msg_t *data = (media_msg_t *)osi_malloc(sizeof(media_msg_t));

			mlist_append(mailbox_free_list, data);
		}
	}

	if (mailbox_msg_list)
	{
		mailbox_msg_list = mlist_new(NULL);
	}
#endif

	mb_chnl_open(MB_CHNL_MEDIA, NULL);
	mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_RX_ISR, media_major_mailbox_rx_isr);
	mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_TX_ISR, media_major_mailbox_tx_isr);
	mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_TX_CMPL_ISR, media_major_mailbox_tx_cmpl_isr);
#endif

	LOGI("media major thread startup complete\n");

	return kNoErr;
error:

	if (media_major_msg_queue)
	{
		rtos_deinit_queue(&media_major_msg_queue);
		media_major_msg_queue = NULL;
	}

	return ret;
}

media_cpu_t get_cpu_id(void)
{
	return MAJOR_CPU;
}
