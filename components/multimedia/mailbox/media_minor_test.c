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

#define TAG "mailbox_minor_test"


#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


static LIST_HEADER_T mailbox_minor_test_msg_queue_req;

static beken_thread_t mailbox_minor_test_th_hd = NULL;
beken_semaphore_t mailbox_minor_test_sem = NULL;
media_mailbox_msg_t mailbox_app_rsp = {0};

void msg_send_to_minor_test_mailbox(media_mailbox_msg_t *msg)
{
	LOGD("%s, %08X\n", __func__, msg->event);

	switch (msg->event >> MEDIA_EVT_BIT)
	{
		case MAILBOX_REQ:
        case MAILBOX_RSP:
        case MAILBOX_NOTIFY:
		{
            media_mailbox_list_push(msg, &mailbox_minor_test_msg_queue_req);
		}
		break;

		default:
			break;
	}
}

bk_err_t mailbox_minor_test_send_to_major_msg(uint32_t event,media_mailbox_msg_t *msg)
{
	bk_err_t ret = BK_OK;
    msg->event = event;
    msg_send_to_media_minor_mailbox(msg);
    return ret;
}

void mailbox_minor_test_req_handle(media_mailbox_msg_t *msg)
{
	switch (msg->event)
	{
		case EVENT_LCD_DEC_SW_REQ:
            // TODO

            mailbox_minor_test_send_to_major_msg(EVENT_LCD_DEC_SW_RSP, msg);
            break;
	}
}

static void mailbox_minor_test_message_handle(void)
{
	bk_err_t ret = BK_OK;
	(void)(ret);
	media_mailbox_msg_t *node = NULL;

	while (1)
	{
	    node = media_mailbox_list_pop(&mailbox_minor_test_msg_queue_req);
		if (node != NULL)
		{
			switch (node->event >> MEDIA_EVT_BIT)
			{
				case MAILBOX_REQ:
                    mailbox_minor_test_req_handle(node);
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
            if (mailbox_minor_test_sem)
            {
                ret = rtos_get_semaphore(&mailbox_minor_test_sem, BEKEN_WAIT_FOREVER);
            }
        }
	}

exit:

    if (mailbox_minor_test_sem)
    {
        rtos_deinit_semaphore(&mailbox_minor_test_sem);
    }

	/* delate task */
	rtos_delete_thread(NULL);

	mailbox_minor_test_th_hd = NULL;

	LOGE("delete task complete\n");
}

bk_err_t mailbox_minor_test_init(void)
{
	bk_err_t ret = BK_OK;

    if (mailbox_minor_test_sem != NULL)
    {
        LOGE("mailbox app semaphore already init, exit!\n");
		goto exit;
    }

	ret = rtos_init_semaphore(&mailbox_minor_test_sem, 1);

	if (ret != BK_OK)
	{
		LOGE("create mailbox app semaphore failed\n");
		goto error;
	}

	if (mailbox_minor_test_th_hd != NULL)
	{
		ret = BK_OK;
		LOGE("media_app_th_hd already init, exit!\n");
		goto error;
	}

    INIT_LIST_HEAD(&mailbox_minor_test_msg_queue_req);

	ret = rtos_create_thread(&mailbox_minor_test_th_hd,
	                         6,
	                         "media_app_thread",
	                         (beken_thread_function_t)mailbox_minor_test_message_handle,
	                         4096,
	                         NULL);

	if (ret != BK_OK)
	{
		LOGE("create media app thread fail\n");
		goto error;
	}

	LOGI("media app thread startup complete\n");

	return BK_OK;

error:

    if (mailbox_minor_test_sem)
    {
        rtos_deinit_semaphore(&mailbox_minor_test_sem);
    }

exit:

	return ret;
}



