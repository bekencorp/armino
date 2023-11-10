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

#include "bk_list.h"
#include "media_core.h"
#include "media_evt.h"
#include "mailbox_channel.h"
#include "transfer_act.h"
#include "media_mailbox_list_util.h"
#include <modules/pm.h>

#include "aud_tras_drv.h"
#include "aud_tras.h"
#include "storage_act.h"

#define TAG "media_app_mailbox"


#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static LIST_HEADER_T media_app_mailbox_msg_queue_req;
static LIST_HEADER_T media_app_mailbox_msg_queue_rsp;
static beken_mutex_t media_app_mailbox_msg_queue_req_lock;
static beken_mutex_t media_app_mailbox_msg_queue_rsp_lock;

static beken_queue_t media_app_mailbox_msg_queue;

static beken_thread_t media_app_mailbox_th_hd = NULL;
beken_semaphore_t media_app_mailbox_sem = NULL;
beken_semaphore_t media_app_mailbox_rsp_sem = NULL;
media_mailbox_msg_t *msg_app_send = NULL;


static void msg_send_to_media_app_mailbox_list(media_mailbox_msg_t *msg)
{
	//LOCK
	media_mailbox_list_push(msg, &media_app_mailbox_msg_queue_req, media_app_mailbox_msg_queue_req_lock);
	rtos_set_semaphore(&media_app_mailbox_sem);
}

bk_err_t msg_send_to_media_app_mailbox(media_mailbox_msg_t *msg, uint32_t result)
{
	bk_err_t ret = BK_OK;
	msg->src = APP_MODULE;
	msg->dest = MAJOR_MODULE;
	msg->result = result;

	msg_send_to_media_app_mailbox_list(msg);
	return ret;
}

static bk_err_t msg_send_back_to_media_app_mailbox(media_mailbox_msg_t *msg, uint32_t result)
{
	bk_err_t ret = BK_OK;
	msg->src = APP_MODULE;
	msg->dest = APP_MODULE;
	msg->result = result;

	msg_send_to_media_app_mailbox_list(msg);
	return ret;
}

static bk_err_t media_app_mailbox_send_msg_to_media_major_mailbox(media_mailbox_msg_t *msg)
{
	mb_chnl_cmd_t mb_cmd;

	mb_cmd.hdr.cmd = 1;
	mb_cmd.param1 = msg->event;
	mb_cmd.param2 = (uint32_t)msg;
	mb_cmd.param3 = (uint32_t)msg->sem;
	return mb_chnl_write(MB_CHNL_MEDIA, &mb_cmd);
}

static bk_err_t media_send_msg_to_queue(media_mailbox_msg_t *param, uint8_t ori)
{
	bk_err_t ret = BK_OK;
	media_msg_t msg;
	msg.event = ori;
	msg.param = (uint32_t)param;

	if (media_app_mailbox_msg_queue)
	{
		ret = rtos_push_to_queue(&media_app_mailbox_msg_queue, &msg, BEKEN_NO_WAIT);

		if (BK_OK != ret)
		{
			LOGE("%s failed\n", __func__);
			return BK_FAIL;
		}

		return ret;
	}
	return ret;
}

static void media_app_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
	media_mailbox_msg_t *msg;

	msg = (media_mailbox_msg_t *)cmd_buf->param2;
	LOGD("====>>>>9 %X %X %x\n", msg->src, msg->dest, msg->result);

//	media_mailbox_list_push(msg, &media_app_mailbox_msg_queue_req, media_app_mailbox_msg_queue_req_lock);
	media_send_msg_to_queue(msg, 0);
	rtos_set_semaphore(&media_app_mailbox_sem);
}


static void media_app_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);
}

static void media_app_mailbox_tx_cmpl_isr(beken_semaphore_t msg, mb_chnl_ack_t *ack_buf)
{
	LOGD("====>>>>4 %p %p\n", msg_app_send, media_app_mailbox_rsp_sem);

	if (msg_app_send)
	{
		//media_mailbox_list_push(msg_app_send, &media_app_mailbox_msg_queue_rsp, media_app_mailbox_msg_queue_rsp_lock);
		media_send_msg_to_queue(msg_app_send, 1);
		msg_app_send = NULL;
	}

	if (media_app_mailbox_rsp_sem)
	{
		rtos_set_semaphore(media_app_mailbox_rsp_sem);
	}
}

static void media_app_mailbox_msg_send(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;

	LOGD("====>>>>2 %p %08x\n", msg->sem, msg->event);
	if((msg->event >> MEDIA_EVT_BIT) == MAILBOX_NOTIFY)
	{
		ret = media_app_mailbox_send_msg_to_media_major_mailbox(msg);
		if (ret != BK_OK)
		{
			LOGE("%s FAILED 1\n", __func__);
		}

		ret = rtos_get_semaphore(&media_app_mailbox_rsp_sem, 2000);
		if(ret != BK_OK)
		{
			LOGE("%s send msg error\n", __func__);
		}
	}
	else
	{
		msg_app_send = msg;
		ret = media_app_mailbox_send_msg_to_media_major_mailbox(msg);
		if (ret != BK_OK)
		{
			if (msg_app_send)
			{
				msg_app_send = NULL;
			}
			msg_send_back_to_media_app_mailbox(msg, BK_FAIL);
			LOGE("%s FAILED 2\n", __func__);
		}
		
		ret = rtos_get_semaphore(&media_app_mailbox_rsp_sem, 2000);
		if(ret != BK_OK)
		{
			LOGE("%s send msg error\n", __func__);
		}
	}
}

static void media_app_mailbox_msg_handle(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;
	LOGD("====>>>>11 %p %x\n", msg->sem, msg->result);

	if ((msg->event >> MEDIA_EVT_BIT) == MAILBOX_NOTIFY)
	{
		switch (msg->event)
		{
			case EVENT_AUD_MIC_DATA_NOTIFY:
			case EVENT_AUD_SPK_DATA_NOTIFY:
			{
				GPIO_UP(4);

				msg->src = APP_MODULE;
				msg->dest = MAJOR_MODULE;
				msg->result = 160;
				msg_send_to_media_app_mailbox(msg, 160);
#if 0
				//TODO  modify after
				aud_tras_drv_mic_notify_t *mic_notify = (aud_tras_drv_mic_notify_t *)msg->param;
				extern int demo_doorbell_udp_voice_send_packet(unsigned char *data, unsigned int len);
				int size = demo_doorbell_udp_voice_send_packet((unsigned char *)(mic_notify->ptr_data), mic_notify->length);
				if (size < 0) {
					LOGE("%s send audio data fail\n", __func__, size);
				} else {
					LOGD("%s send audio data complete \n", __func__, size);
				}
#else
				aud_tras_send_msg(AUD_TRAS_TX, (void *)msg->param);
#endif
				GPIO_DOWN(4);
			}
			break;
			case EVENT_MEDIA_DATA_NOTIFY:
				transfer_app_event_handle(msg);
				break;

			case EVENT_VID_CAPTURE_NOTIFY:
				storage_app_event_handle(msg);
				break;
				
			default:
				break;
		}
	}
	else
	{

		if (msg->sem)
		{
			media_mailbox_list_del_node(msg->sem, &media_app_mailbox_msg_queue_rsp, media_app_mailbox_msg_queue_rsp_lock);
			ret = rtos_set_semaphore(&msg->sem);
			
			if (ret != BK_OK)
			{
				LOGE("%s semaphore set failed: %d\n", __func__, ret);
			}
		}
		else
		{
			media_mailbox_list_del_node_by_event(msg->event, &media_app_mailbox_msg_queue_rsp, media_app_mailbox_msg_queue_rsp_lock);
		}
	}
}

static void media_app_mailbox_message_handle(void)
{
	bk_err_t ret = BK_OK;
	(void)(ret);
	media_mailbox_msg_t *node = NULL;
	media_mailbox_msg_t *tmp = NULL;
	media_msg_t msg;
	LOGI("%s\n", __func__);


	while (1)
	{
		if (msg_app_send == NULL)
		{
			node = media_mailbox_list_pop(&media_app_mailbox_msg_queue_req, media_app_mailbox_msg_queue_req_lock);
		}
		else
		{
			node = msg_app_send;
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
					media_app_mailbox_msg_send(node);
					break;
				case APP_MODULE:
					media_app_mailbox_msg_handle(node);
					break;

				default:
					break;
			}
		}
		else
		{
			if (media_app_mailbox_sem)
			{
				ret = rtos_get_semaphore(&media_app_mailbox_sem, BEKEN_WAIT_FOREVER);
				while(1)
				{
					ret = rtos_pop_from_queue(&media_app_mailbox_msg_queue, &msg, 0);
					if (kNoErr == ret)
					{
						tmp = (media_mailbox_msg_t *)msg.param;
						if (msg.event == 0)
						{
							media_mailbox_list_push(tmp, &media_app_mailbox_msg_queue_req, media_app_mailbox_msg_queue_req_lock);
						}
						else
						{
							media_mailbox_list_push(tmp, &media_app_mailbox_msg_queue_rsp, media_app_mailbox_msg_queue_rsp_lock);
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

	if (media_app_mailbox_msg_queue)
	{
		rtos_deinit_queue(&media_app_mailbox_msg_queue);
		media_app_mailbox_msg_queue = NULL;
	}

	if (media_app_mailbox_rsp_sem)
	{
		rtos_deinit_semaphore(&media_app_mailbox_rsp_sem);
		media_app_mailbox_rsp_sem = NULL;
	}
	if (media_app_mailbox_sem)
	{
		rtos_deinit_semaphore(&media_app_mailbox_sem);
		media_app_mailbox_sem = NULL;
	}

	/* delate task */
	rtos_delete_thread(NULL);

	media_app_mailbox_th_hd = NULL;

	LOGE("delete task complete\n");
}

bk_err_t media_app_mailbox_init(void)
{
	bk_err_t ret = BK_OK;

	if (media_app_mailbox_sem != NULL)
	{
		LOGE("mailbox app semaphore already init, exit!\n");
		goto exit;
	}

	ret = rtos_init_semaphore(&media_app_mailbox_sem, 1);

	if (ret != BK_OK)
	{
		LOGE("create mailbox app semaphore failed\n");
		goto error1;
	}

	if (media_app_mailbox_rsp_sem == NULL)
	{
		ret = rtos_init_semaphore(&media_app_mailbox_rsp_sem, 1);
		if (ret != BK_OK)
		{
			LOGE("create mailbox app semaphore failed\n");
			goto error1;
		}
	}

	if (media_app_mailbox_th_hd != NULL)
	{
		ret = BK_OK;
		LOGE("media_app_mailbox_th_hd already init, exit!\n");
		goto error1;
	}

	INIT_LIST_HEAD(&media_app_mailbox_msg_queue_req);
	INIT_LIST_HEAD(&media_app_mailbox_msg_queue_rsp);
	rtos_init_mutex(&media_app_mailbox_msg_queue_req_lock);
	rtos_init_mutex(&media_app_mailbox_msg_queue_rsp_lock);

	mb_chnl_open(MB_CHNL_MEDIA, NULL);
	mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_RX_ISR, media_app_mailbox_rx_isr);
	mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_TX_ISR, media_app_mailbox_tx_isr);
	mb_chnl_ctrl(MB_CHNL_MEDIA, MB_CHNL_SET_TX_CMPL_ISR, media_app_mailbox_tx_cmpl_isr);

	if (media_app_mailbox_msg_queue != NULL)
	{
		ret = kNoErr;
		LOGE("media_major_msg_queue allready init, exit!\n");
		goto error2;
	}

	ret = rtos_init_queue(&media_app_mailbox_msg_queue,
							"media_app_mailbox_msg_queue",
							sizeof(media_msg_t),
							20);

	ret = rtos_create_thread(&media_app_mailbox_th_hd,
							 6,
							 "media_app_mailbox_thread",
							 (beken_thread_function_t)media_app_mailbox_message_handle,
							 2048,
							 NULL);

	if (ret != BK_OK)
	{
		LOGE("create mailbox app thread fail\n");
		goto error3;
	}


	LOGI("mailbox app thread startup complete\n");

	return BK_OK;

error3:
	if (media_app_mailbox_msg_queue)
	{
		rtos_deinit_queue(&media_app_mailbox_msg_queue);
		media_app_mailbox_msg_queue = NULL;
	}

error2:
	if (media_app_mailbox_rsp_sem)
	{
		rtos_deinit_semaphore(&media_app_mailbox_rsp_sem);
		media_app_mailbox_rsp_sem = NULL;
	}

error1:
	if (media_app_mailbox_sem)
	{
		rtos_deinit_semaphore(&media_app_mailbox_sem);
		media_app_mailbox_sem = NULL;
	}

exit:

	return ret;
}


