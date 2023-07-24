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
#include "media_mailbox_list_util.h"

#define TAG "mailbox_major_test"


#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static LIST_HEADER_T mailbox_major_test_msg_queue_req;


static beken_thread_t mailbox_major_test_th_hd = NULL;
beken_semaphore_t mailbox_major_test_sem = NULL;
media_mailbox_msg_t *msg_major_test_send = NULL;

extern bk_err_t mailbox_major_send_app_msg(media_mailbox_msg_t *msg);

void msg_send_to_major_test_mailbox(media_mailbox_msg_t *msg)
{

	switch (msg->event >> MEDIA_EVT_BIT)
	{
		case MAILBOX_REQ:
        case MAILBOX_RSP:
        case MAILBOX_NOTIFY:
		{
            media_mailbox_list_push(msg, &mailbox_major_test_msg_queue_req);
		}
		break;

		default:
			break;
	}
}

bk_err_t mailbox_major_test_send_to_app_msg(uint32_t event, uint32_t result, media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;
    msg->event = event;
    msg->result = result;
//    LOGE("====>>>>7 %s %p\n", __func__, msg->sem);

    msg_send_to_media_major_mailbox(msg);
    return ret;
}

bk_err_t mailbox_major_test_send_to_minor_msg(uint32_t event, void *param)
{
	bk_err_t ret = BK_OK;
	uint32_t result = BK_OK;

	media_mailbox_msg_t *node = NULL;
    node = os_malloc(sizeof(media_mailbox_msg_t));
    if (node != NULL)
    {
        node->event = event;
        node->dest = 0;
        node->src = 1;
        node->param = (uint32_t)param;
        node->result = result;
        node->sem = mailbox_major_test_sem;
        msg_send_to_media_major_mailbox(node);
        ret = rtos_get_semaphore(&mailbox_major_test_sem, BEKEN_WAIT_FOREVER);
        if (ret != BK_OK)
        {
            LOGE("%s, rtos_get_semaphore failed\n", __func__);
            ret = BK_FAIL;
        }
        else
        {
            ret = node->result;
        }
    }
    else
    {
        LOGE("%s, no mem\n", __func__);
        ret = BK_ERR_NO_MEM;
    }

    if (node)
    {
        os_free(node);
    }
    return ret;
}


void mailbox_major_test_req_handle(media_mailbox_msg_t *msg)
{
//    LOGE("====>>>>6 %s\n", __func__);

	bk_err_t result = BK_OK;
	switch (msg->event)
	{
	    case EVENT_LCD_OPEN_REQ:
            // TODO
            mailbox_major_test_send_to_app_msg(EVENT_LCD_OPEN_RSP, result, msg);
            break;
	}
}

void mailbox_major_test_rsp_handle(media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;
	switch (msg->event)
	{
		case EVENT_LCD_DEC_SW_RSP:
            ret = rtos_set_semaphore(&msg->sem);
            
            if (ret != BK_OK)
            {
                LOGE("%s semaphore get failed: %d\n", __func__, ret);
            }

            if (msg->result != BK_OK)
            {
                LOGE("%s LCD OPEN error %d\n", __func__, msg->result);
            }
            else
            {
                LOGI("%s LCD OPEN successed\n", __func__);
            }

	}
}

static void mailbox_major_test_message_handle(void)
{
	media_mailbox_msg_t *node = NULL;

	while (1)
	{
        rtos_delay_milliseconds(10);
        node = media_mailbox_list_pop(&mailbox_major_test_msg_queue_req);

        if (node != NULL)
		{
			switch (node->event >> MEDIA_EVT_BIT)
			{
				case MAILBOX_REQ:
					mailbox_major_test_req_handle(node);
					break;

                case MAILBOX_RSP:
                    mailbox_major_test_rsp_handle(node);
                    break;

                case MAILBOX_NOTIFY:
                    mailbox_major_send_app_msg(node);
                    break;

				case EXIT_EVENT:
					goto exit;
					break;

				default:
					break;
			}
		}

        //TODO

	}

exit:

    if (mailbox_major_test_sem)
    {
        rtos_deinit_semaphore(&mailbox_major_test_sem);
        mailbox_major_test_sem = NULL;
    }

	/* delate task */
	rtos_delete_thread(NULL);

	mailbox_major_test_th_hd = NULL;

	LOGE("delete task complete\n");
}

bk_err_t mailbox_major_test_init(void)
{
	bk_err_t ret = BK_OK;

    if (mailbox_major_test_sem != NULL)
    {
//        LOGE("mailbox_major_test_sem already init, exit!\n");
		goto exit;
    }

	ret = rtos_init_semaphore(&mailbox_major_test_sem, 1);

	if (ret != BK_OK)
	{
		LOGE("create mailbox major semaphore failed\n");
		goto exit;
	}

	if (mailbox_major_test_th_hd != NULL)
	{
		ret = BK_OK;
		LOGE("media_major_th_hd already init, exit!\n");
		goto error;
	}

    INIT_LIST_HEAD(&mailbox_major_test_msg_queue_req);

	ret = rtos_create_thread(&mailbox_major_test_th_hd,
	                         6,
	                         "mailbox_major_test_thread",
	                         (beken_thread_function_t)mailbox_major_test_message_handle,
	                         1024,
	                         NULL);

	if (ret != BK_OK)
	{
		LOGE("create mailbox major test thread fail\n");
		goto error;
	}

	LOGI("mailbox major test thread startup complete\n");

	return BK_OK;

error:

    if (mailbox_major_test_sem)
    {
        rtos_deinit_semaphore(&mailbox_major_test_sem);
        mailbox_major_test_sem = NULL;
    }
exit:

	return ret;
}

