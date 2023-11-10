// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <os/os.h>
#include <os/str.h>
#include <os/mem.h>
#include <components/log.h>
#include <common/bk_include.h>
#include <driver/int.h>

#include "lcd_act.h"

#include "bk_list.h"
#include "media_core.h"
#include "media_evt.h"
#include "mailbox_channel.h"
#include "media_mailbox_list_util.h"


#include "lcd_act.h"
#include "transfer_act.h"
#include "storage_act.h"
#include "camera_act.h"
#include "frame_buffer.h"
#include <modules/pm.h>

#include "aud_tras_drv.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>


#define TAG "media_major_mailbox"

//#define CONFIG_MINOR

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static LIST_HEADER_T media_major_mailbox_msg_queue_req;
static LIST_HEADER_T media_major_mailbox_msg_queue_rsp;
static beken_mutex_t media_major_mailbox_msg_queue_req_lock;
static beken_mutex_t media_major_mailbox_msg_queue_rsp_lock;

static beken_queue_t media_major_mailbox_msg_queue;


static beken_thread_t media_major_mailbox_th_hd = NULL;
beken_semaphore_t media_major_mailbox_sem = NULL;
beken_semaphore_t media_major_mailbox_rsp_sem = NULL;
beken_semaphore_t media_major_mailbox_send_sem = NULL;

media_mailbox_msg_t *msg_major_send = NULL;


static void msg_send_to_media_major_mailbox_list(media_mailbox_msg_t *msg)
{
	media_mailbox_list_push(msg, &media_major_mailbox_msg_queue_req, media_major_mailbox_msg_queue_req_lock);

	rtos_set_semaphore(&media_major_mailbox_sem);
}

bk_err_t msg_send_to_media_major_mailbox(media_mailbox_msg_t *msg, uint32_t result, uint32_t dest)
{
	bk_err_t ret = BK_OK;
	msg->src = MAJOR_MODULE;
	msg->dest = dest;
	msg->result = result;
	LOGD("====>>>>7 %s %p %x\n", __func__, msg->sem, msg->result);

	msg_send_to_media_major_mailbox_list(msg);
	return ret;
}

static bk_err_t msg_send_back_to_media_major_mailbox(media_mailbox_msg_t *msg, uint32_t result)
{
	bk_err_t ret = BK_OK;
	msg->src = MAJOR_MODULE;
	msg->dest = MAJOR_MODULE;
	msg->result = result;
	LOGD("====>>>>7 %s %p %x\n", __func__, msg->sem, msg->result);

	msg_send_to_media_major_mailbox_list(msg);
	return ret;
}

static bk_err_t media_major_mailbox_send_msg_to_media_app_mailbox(media_mailbox_msg_t *msg)
{
	mb_chnl_cmd_t mb_cmd;

	mb_cmd.hdr.cmd = 1;
	mb_cmd.param1 = msg->event;
	mb_cmd.param2 = (uint32_t)msg;
	mb_cmd.param3 = (uint32_t)msg->sem;
	return mb_chnl_write(MB_CHNL_MEDIA, &mb_cmd);
}

static bk_err_t media_major_mailbox_send_msg_to_media_minor_mailbox(media_mailbox_msg_t *msg)
{
	mb_chnl_cmd_t mb_cmd;

	mb_cmd.hdr.cmd = 1;
	mb_cmd.param1 = msg->event;
	mb_cmd.param2 = (uint32_t)msg;
	mb_cmd.param3 = (uint32_t)msg->sem;
	return mb_chnl_write(MB_CHNL_VID, &mb_cmd);
}

static bk_err_t media_send_msg_to_queue(media_mailbox_msg_t *param, uint8_t ori)
{
	bk_err_t ret = BK_OK;
	media_msg_t msg;
	msg.event = ori;
	msg.param = (uint32_t)param;

	if (media_major_mailbox_msg_queue)
	{
		ret = rtos_push_to_queue(&media_major_mailbox_msg_queue, &msg, BEKEN_NO_WAIT);

		if (BK_OK != ret)
		{
			LOGE("%s failed\n", __func__);
			return BK_FAIL;
		}

		return ret;
	}
	return ret;
}

static void media_major_mailbox_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
	media_mailbox_msg_t *msg;
	LOGD("====>>>>3 %s\n", __func__);

	msg = (media_mailbox_msg_t *)cmd_buf->param2;
//	media_mailbox_list_push(msg, &media_major_mailbox_msg_queue_req, media_major_mailbox_msg_queue_req_lock);
	media_send_msg_to_queue(msg, 0);

	rtos_set_semaphore(&media_major_mailbox_sem);
}


static void media_major_mailbox_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);
}

static void media_major_mailbox_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
	LOGD("====>>>>10 %s %p %p\n", __func__, msg_major_send, media_major_mailbox_rsp_sem);

//	if (msg_major_send && msg_major_send->sem)
//	{
//		rtos_lock_mutex(&media_major_mailbox_msg_queue_rsp_lock);
//		media_mailbox_list_del_node(msg_major_send->sem, &media_major_mailbox_msg_queue_rsp);
//		rtos_unlock_mutex(&media_major_mailbox_msg_queue_rsp_lock);
//	}

	if (msg_major_send)
	{
		//media_mailbox_list_push(msg_major_send, &media_major_mailbox_msg_queue_rsp, media_major_mailbox_msg_queue_rsp_lock);
		media_send_msg_to_queue(msg_major_send, 1);
		msg_major_send = NULL;
	}

	if (media_major_mailbox_send_sem)
	{
		rtos_set_semaphore(media_major_mailbox_send_sem);
		media_major_mailbox_send_sem = NULL;
	}
}

void media_major_mailbox_rsp_handle_ext(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;

	//media_mailbox_list_del_node(msg->sem, &media_major_mailbox_msg_queue_rsp, media_major_mailbox_msg_queue_rsp_lock);

	ret = rtos_set_semaphore(&msg->sem);
	
	if (ret != BK_OK)
	{
		LOGE("%s semaphore set failed: %d\n", __func__, ret);
	}

}

void media_major_mailbox_msg_handle(media_mailbox_msg_t *msg)
{
	LOGD("====>>>>50 %s %x %x %x\n", __func__, msg->src, msg->event, LCD_EVENT);

	media_msg_t media_msg;

	if (msg->src == APP_MODULE)
	{
		switch (msg->event >> MEDIA_EVT_BIT)
		{
			case LCD_EVENT:
				media_msg.event = msg->event;
				media_msg.param = (uint32_t)msg;
				media_send_msg(&media_msg);
				break;

			case CAM_EVENT:
				media_msg.event = msg->event;
				media_msg.param = (uint32_t)msg;
				media_send_msg(&media_msg);
				break;

			case TRS_EVENT:
				media_msg.event = msg->event;
				media_msg.param = (uint32_t)msg;
				media_send_msg(&media_msg);
				break;

			case STORAGE_EVENT:
				media_msg.event = msg->event;
				media_msg.param = (uint32_t)msg;
				media_send_msg(&media_msg);
				break;

			case AUD_EVENT:
			case AUD_NOTIFY:
				audio_event_handle(msg);
				break;

			case MAILBOX_NOTIFY:
				switch (msg->event)
				{
					case EVENT_MEDIA_DATA_NOTIFY:
					case EVENT_VID_CAPTURE_NOTIFY:
					case EVENT_VID_SAVE_ALL_NOTIFY:
						media_major_mailbox_rsp_handle_ext(msg);
						break;

					case EVENT_AUD_MIC_DATA_NOTIFY:
					case EVENT_AUD_SPK_DATA_NOTIFY:
						audio_event_handle(msg);
						break;

					default:
						break;
				}
				break;
			default:
				break;

		}
	}
	else if (msg->src == MINOR_MODULE)
	{
	}
	else
	{
	}
//	LOGE("====>>>>53 %s %x %x %x\n", __func__, msg->src, msg->event, LCD_EVENT);
}

void media_major_mailbox_msg_send(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;

	LOGD("====>>>>8 %s %x %x\n", __func__, msg->src, msg->result);
	media_major_mailbox_send_sem = media_major_mailbox_rsp_sem;
	ret = media_major_mailbox_send_msg_to_media_app_mailbox(msg);

	ret = rtos_get_semaphore(&media_major_mailbox_rsp_sem, 500);
	if(ret != BK_OK)
	{
		LOGE("%s send msg error\n", __func__);
	}
}

static void media_major_mailbox_message_handle(void)
{
	bk_err_t ret = BK_OK;
	(void)(ret);
	media_mailbox_msg_t *node = NULL;
	media_mailbox_msg_t *tmp = NULL;
	media_msg_t msg;
	LOGI("%s\n", __func__);
	media_init();

	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 0, 0);

	while (1)
	{
		if (msg_major_send == NULL)
		{
			node = media_mailbox_list_pop(&media_major_mailbox_msg_queue_req, media_major_mailbox_msg_queue_req_lock);
		}
		else
		{
			node = msg_major_send;
		}
		if (node != NULL)
		{
			if ((node->event >> MEDIA_EVT_BIT) == EXIT_EVENT)
			{
				goto exit;
			}
			switch (node->dest)
			{
				case MAJOR_MODULE:
					media_major_mailbox_msg_handle(node);
					break;

				case APP_MODULE:
//					msg_major_send = node;
					media_major_mailbox_msg_send(node);
					break;

				case MINOR_MODULE:
//					msg_major_send = node;
//					media_major_mailbox_rsp_handle(node);
					break;

				default:
					break;
			}
		}
		else
		{
			if (media_major_mailbox_sem)
			{
				ret = rtos_get_semaphore(&media_major_mailbox_sem, BEKEN_WAIT_FOREVER);
				while(1)
				{
					ret = rtos_pop_from_queue(&media_major_mailbox_msg_queue, &msg, 0);
					if (kNoErr == ret)
					{
						tmp = (media_mailbox_msg_t *)msg.param;
						if (msg.event == 0)
						{
							media_mailbox_list_push(tmp, &media_major_mailbox_msg_queue_req, media_major_mailbox_msg_queue_req_lock);
						}
						else
						{
							media_mailbox_list_push(tmp, &media_major_mailbox_msg_queue_rsp, media_major_mailbox_msg_queue_rsp_lock);
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

exit:

	media_deinit();

	if (media_major_mailbox_rsp_sem)
	{
		rtos_deinit_semaphore(&media_major_mailbox_rsp_sem);
		media_major_mailbox_rsp_sem = NULL;
	}

	if (media_major_mailbox_sem)
	{
		rtos_deinit_semaphore(&media_major_mailbox_sem);
		media_major_mailbox_sem = NULL;
	}

	/* delate task */
	rtos_delete_thread(NULL);

	media_major_mailbox_th_hd = NULL;

	LOGE("delete task complete\n");
}

bk_err_t media_major_mailbox_init(void)
{
	bk_err_t ret = BK_OK;
	LOGE("%s\n", __func__);

	if (media_major_mailbox_sem != NULL)
	{
		LOGE("media_major_mailbox_sem already init, exit!\n");
		goto exit;
	}

	ret = rtos_init_semaphore(&media_major_mailbox_sem, 1);

	if (ret != BK_OK)
	{
		LOGE("create media_major_mailbox_sem failed\n");
		goto error1;
	}

	if (media_major_mailbox_rsp_sem == NULL)
	{
		ret = rtos_init_semaphore(&media_major_mailbox_rsp_sem, 1);
		if (ret != BK_OK)
		{
			LOGE("create media_major_mailbox_rsp_sem failed\n");
			goto error1;
		}
	}

	if (media_major_mailbox_th_hd != NULL)
	{
		ret = BK_OK;
		LOGE("media_major_mailbox_th_hd already init, exit!\n");
		goto error2;
	}

	INIT_LIST_HEAD(&media_major_mailbox_msg_queue_req);
	INIT_LIST_HEAD(&media_major_mailbox_msg_queue_rsp);
	rtos_init_mutex(&media_major_mailbox_msg_queue_req_lock);
	rtos_init_mutex(&media_major_mailbox_msg_queue_rsp_lock);

	 mb_chnl_open(MB_CHNL_MEDIA, NULL);
	 mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_RX_ISR, media_major_mailbox_mailbox_rx_isr);
	 mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_TX_ISR, media_major_mailbox_mailbox_tx_isr);
	 mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_TX_CMPL_ISR, media_major_mailbox_mailbox_tx_cmpl_isr);
 
#ifdef CONFIG_MINOR
	 mb_chnl_open(MB_CHNL_VID, NULL);
	 mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_RX_ISR, media_major_mailbox_mailbox_rx_isr);
	 mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_ISR, media_major_mailbox_mailbox_tx_isr);
	 mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_CMPL_ISR, media_major_mailbox_mailbox_tx_cmpl_isr);
#endif

	if (media_major_mailbox_msg_queue != NULL)
	{
		ret = kNoErr;
		LOGE("media_major_mailbox_msg_queue already init, exit!\n");
		goto error2;
	}

	ret = rtos_init_queue(&media_major_mailbox_msg_queue,
							"media_major_mailbox_msg_queue",
							sizeof(media_msg_t),
							20);

	ret = rtos_create_thread(&media_major_mailbox_th_hd,
							 4,
							 "media_major_mailbox_thread",
							 (beken_thread_function_t)media_major_mailbox_message_handle,
							 2048,
							 NULL);

	if (ret != BK_OK)
	{
		LOGE("create mailbox major thread fail\n");
		goto error3;
	}

	LOGI("mailboxs major thread startup complete\n");

	return BK_OK;

	error3:
		if (media_major_mailbox_msg_queue)
		{
			rtos_deinit_queue(&media_major_mailbox_msg_queue);
			media_major_mailbox_msg_queue = NULL;
		}

error2:
	if (media_major_mailbox_rsp_sem)
	{
		rtos_deinit_semaphore(&media_major_mailbox_rsp_sem);
		media_major_mailbox_rsp_sem = NULL;
	}

error1:
	if (media_major_mailbox_sem)
	{
		rtos_deinit_semaphore(&media_major_mailbox_sem);
		media_major_mailbox_sem = NULL;
	}

exit:


	return ret;
}


