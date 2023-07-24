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
#include <os/str.h>
#include <os/mem.h>
#include <components/log.h>
#include <common/bk_include.h>
#include <driver/int.h>

#include "bk_list.h"
#include "media_core.h"
#include "media_evt.h"
#include "mailbox_channel.h"
#include "media_mailbox_list_util.h"

#define TAG "media_minor_mailbox"


#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


static LIST_HEADER_T media_minor_mailbox_msg_queue_req;
static LIST_HEADER_T media_minor_mailbox_msg_queue_rsp;

static beken_thread_t media_minor_mailbox_th_hd = NULL;
beken_semaphore_t media_minor_mailbox_sem = NULL;
beken_semaphore_t media_minor_mailbox_rsp_sem = NULL;
media_mailbox_msg_t *msg_minor_send = NULL;

void msg_send_to_media_minor_mailbox(media_mailbox_msg_t *msg)
{
	LOGD("%s, %08X\n", __func__, msg->event);

    media_mailbox_list_push(msg, &media_minor_mailbox_msg_queue_req);
    rtos_set_semaphore(&media_minor_mailbox_sem);
}

bk_err_t media_minor_mailbox_send_msg(media_mailbox_msg_t *msg)
{
	mb_chnl_cmd_t mb_cmd;

	mb_cmd.hdr.cmd = 1;
	mb_cmd.param1 = msg->event;
	mb_cmd.param2 = (uint32_t)msg;
	mb_cmd.param3 = (uint32_t)msg->sem;
	return mb_chnl_write(MB_CHNL_VID, &mb_cmd);
}

static void media_minor_mailbox_mailbox_rx_isr(void *param, mb_chnl_cmd_t *cmd_buf)
{
	LOGD("%s, %08X\n", __func__, cmd_buf->param1);
    media_mailbox_msg_t *msg;

    msg = (media_mailbox_msg_t *)cmd_buf->param2;
    media_mailbox_list_push(msg, &media_minor_mailbox_msg_queue_req);
    rtos_set_semaphore(&media_minor_mailbox_sem);
}

static void media_minor_mailbox_mailbox_tx_isr(void *param)
{
	LOGD("%s\n", __func__);
}

static void media_minor_mailbox_mailbox_tx_cmpl_isr(void *param, mb_chnl_ack_t *ack_buf)
{
	LOGD("%s msg_send %p msg_del %p\n", __func__, msg_minor_send, msg_minor_send->sem);
    if (msg_minor_send && msg_minor_send->sem)
    {
        media_mailbox_msg_t *node = NULL;
        node = media_mailbox_list_get_node(msg_minor_send->sem, &media_minor_mailbox_msg_queue_rsp);
    
        if (node != NULL)
        {
            node = media_mailbox_list_del_node(msg_minor_send->sem, &media_minor_mailbox_msg_queue_rsp);
        }
    }
    if (msg_minor_send)
    {
        media_mailbox_list_push(msg_minor_send, &media_minor_mailbox_msg_queue_rsp);
        msg_minor_send = NULL;
    }
}

void media_minor_mailbox_req_handle(media_mailbox_msg_t *msg)
{
	switch (msg->event)
	{
		case EVENT_LCD_DEC_SW_REQ:
            msg_send_to_minor_test_mailbox(msg);
            break;
	}
}

void media_minor_mailbox_rsp_handle(media_mailbox_msg_t *msg)
{
	switch (msg->event)
	{
		case EVENT_LCD_DEC_SW_RSP:
            media_minor_mailbox_send_msg(msg);
            break;
	}
}

static void media_minor_mailbox_message_handle(void)
{
	bk_err_t ret = BK_OK;
    (void)(ret);
	media_mailbox_msg_t *node = NULL;

	while (1)
	{
	    if (msg_minor_send == NULL)
	    {
	        node = media_mailbox_list_pop(&media_minor_mailbox_msg_queue_req);
		}
        else
        {
            node = msg_minor_send;
        }
		if (node != NULL)
		{
			switch (node->event >> MEDIA_EVT_BIT)
			{
				case MAILBOX_REQ:
					media_minor_mailbox_req_handle(node);
					break;

                case MAILBOX_RSP:
                    media_minor_mailbox_rsp_handle(node);
                    break;

                case MAILBOX_NOTIFY:
                    ret = media_minor_mailbox_send_msg(node);
                    break;


				case EXIT_EVENT:
					goto exit;
					break;

				default:
					break;
			}
		}
        else
        {
            if (media_minor_mailbox_sem)
            {
                ret = rtos_get_semaphore(&media_minor_mailbox_sem, BEKEN_WAIT_FOREVER);
            }
        }
	}

exit:

    if (media_minor_mailbox_sem)
    {
        rtos_deinit_semaphore(&media_minor_mailbox_sem);
    }

	/* delate task */
	rtos_delete_thread(NULL);

	media_minor_mailbox_th_hd = NULL;

	LOGE("delete task complete\n");
}

bk_err_t media_minor_mailbox_init(void)
{
	bk_err_t ret = BK_OK;

    if (media_minor_mailbox_sem != NULL)
    {
        LOGE("mailbox minor semaphore already init, exit!\n");
		goto exit;
    }

	ret = rtos_init_semaphore(&media_minor_mailbox_sem, 1);

	if (ret != BK_OK)
	{
		LOGE("create mailbox minor semaphore failed\n");
		goto error;
	}

	if (media_minor_mailbox_th_hd != NULL)
	{
		ret = BK_OK;
		LOGE("media_minor_th_hd already init, exit!\n");
		goto error;
	}

    INIT_LIST_HEAD(&media_minor_mailbox_msg_queue_req);
    INIT_LIST_HEAD(&media_minor_mailbox_msg_queue_rsp);

	ret = rtos_create_thread(&media_minor_mailbox_th_hd,
	                         4,
	                         "media_minor_thread",
	                         (beken_thread_function_t)media_minor_mailbox_message_handle,
	                         4096,
	                         NULL);

	if (ret != BK_OK)
	{
		LOGE("create media minor thread fail\n");
		goto error;
	}

	mb_chnl_open(MB_CHNL_VID, NULL);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_RX_ISR, media_minor_mailbox_mailbox_rx_isr);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_ISR, media_minor_mailbox_mailbox_tx_isr);
	mb_chnl_ctrl(MB_CHNL_VID, MB_CHNL_SET_TX_CMPL_ISR, media_minor_mailbox_mailbox_tx_cmpl_isr);

	LOGI("media minor thread startup complete\n");

	return BK_OK;

error:
    if (media_minor_mailbox_sem)
    {
        rtos_deinit_semaphore(&media_minor_mailbox_sem);
    }

exit:
	return ret;
}


