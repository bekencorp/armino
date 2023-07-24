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

#define TAG "app_test"


#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


//static LIST_HEADER_T mailbox_app_test_msg_queue;

static beken_thread_t mailbox_app_test_th_hd = NULL;
beken_semaphore_t mailbox_app_test_sem = NULL;

bk_err_t mailbox_app_test_send_msg(uint32_t event, void *param)
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
        node->sem = mailbox_app_test_sem;
//        LOGE("====>>>>1 %s\n", __func__);
        msg_send_to_media_app_mailbox(node);
        ret = rtos_get_semaphore(&mailbox_app_test_sem, BEKEN_WAIT_FOREVER);
        if (ret != BK_OK)
        {
            LOGE("%s, rtos_get_semaphore\n", __func__);
            ret = BK_FAIL;
        }
        else
        {
            ret = result;
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

static void mailbox_app_test_message_handle(void)
{
	bk_err_t ret = BK_OK;
    (void)(ret);
	LOGE("%s\n", __func__);
    rtos_delay_milliseconds(1000);
	while (1)
	{
	    ret = mailbox_app_test_send_msg(EVENT_LCD_OPEN_REQ, NULL);

        LOGE("%s EVENT_LCD_OPEN_REQ %d\n", __func__, ret);
        rtos_delay_milliseconds(1000);
	}

    goto exit;
exit:

    if (mailbox_app_test_sem)
    {
        rtos_deinit_semaphore(&mailbox_app_test_sem);
    }

	/* delate task */
	rtos_delete_thread(NULL);

	mailbox_app_test_th_hd = NULL;

	LOGE("delete task complete\n");
}

bk_err_t media_app_test_init(void)
{
	bk_err_t ret = BK_OK;

    if (mailbox_app_test_sem != NULL)
    {
        LOGE("%s, mailbox app semaphore already init, exit!\n");
		goto exit;
    }

	ret = rtos_init_semaphore(&mailbox_app_test_sem, 1);

	if (ret != BK_OK)
	{
		LOGE("%s, create mailbox app test semaphore failed\n");
		goto error;
	}

	if (mailbox_app_test_th_hd != NULL)
	{
		ret = BK_OK;
		LOGE("%s, mailbox_app_test_th_hd already init, exit!\n");
		goto error;
	}

	ret = rtos_create_thread(&mailbox_app_test_th_hd,
	                         6,
	                         "mailbox_app_test_thread",
	                         (beken_thread_function_t)mailbox_app_test_message_handle,
	                         2048,
	                         NULL);

	if (ret != BK_OK)
	{
		LOGE("create mailbox app test thread fail\n");
		goto error;
	}

	LOGI("====>>>>mailbox app test thread startup complete\n");

	return BK_OK;

error:
    if (mailbox_app_test_sem)
    {
        rtos_deinit_semaphore(&mailbox_app_test_sem);
    }

exit:

	return ret;
}

