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
#include "media_mailbox_list_util.h"

#define TAG "media_mailbox_list_util"


#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


media_mailbox_msg_t *media_mailbox_list_get_node(beken_semaphore_t msg, LIST_HEADER_T *list, beken_mutex_t lock)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	rtos_lock_mutex(&lock);
	LIST_HEADER_T *pos, *n;
	media_mailbox_msg_t *node = NULL;
	media_mailbox_list_t *tmp = NULL;
	list_for_each_safe(pos, n, list)
	{
		tmp = list_entry(pos, media_mailbox_list_t, list);
		if (tmp != NULL)
		{
			if(tmp->msg->sem == msg)
			{
				node = tmp->msg;
				break;
			}
		}
	}
	rtos_unlock_mutex(&lock);
	GLOBAL_INT_RESTORE();
	return node;
}

media_mailbox_msg_t *media_mailbox_list_del_node_by_event(uint32_t event, LIST_HEADER_T *list, beken_mutex_t lock)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	rtos_lock_mutex(&lock);
	LIST_HEADER_T *pos, *n;
	media_mailbox_msg_t *node = NULL;
	media_mailbox_list_t *tmp = NULL;
	list_for_each_safe(pos, n, list)
	{
		tmp = list_entry(pos, media_mailbox_list_t, list);
		if (tmp != NULL)
		{
			if(tmp->msg->event == event)
			{
				node = tmp->msg;
				list_del(pos);
				os_free(tmp);
				break;
			}
		}
	}
	rtos_unlock_mutex(&lock);
	GLOBAL_INT_RESTORE();
	return node;
}

media_mailbox_msg_t *media_mailbox_list_del_node(beken_semaphore_t msg, LIST_HEADER_T *list, beken_mutex_t lock)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	rtos_lock_mutex(&lock);
	LIST_HEADER_T *pos, *n;
	media_mailbox_msg_t *node = NULL;
	media_mailbox_list_t *tmp = NULL;
	list_for_each_safe(pos, n, list)
	{
		tmp = list_entry(pos, media_mailbox_list_t, list);
		if (tmp != NULL)
		{
			if(tmp->msg->sem == msg)
			{
				node = tmp->msg;
				list_del(pos);
				os_free(tmp);
				break;
			}
		}
	}
	rtos_unlock_mutex(&lock);
	GLOBAL_INT_RESTORE();
	return node;
}

media_mailbox_msg_t *media_mailbox_list_pop(LIST_HEADER_T *list, beken_mutex_t lock)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	rtos_lock_mutex(&lock);
	LIST_HEADER_T *pos, *n;
	media_mailbox_msg_t *node = NULL;
	media_mailbox_list_t *tmp = NULL;
	list_for_each_safe(pos, n, list)
	{
		tmp = list_entry(pos, media_mailbox_list_t, list);
		if (tmp != NULL)
		{
			node = tmp->msg;
			list_del(pos);
			os_free(tmp);
			break;
		}
	}
	rtos_unlock_mutex(&lock);
	GLOBAL_INT_RESTORE();
	return node;
}

void media_mailbox_list_push(media_mailbox_msg_t *tmp, LIST_HEADER_T *list, beken_mutex_t lock)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	rtos_lock_mutex(&lock);
	media_mailbox_list_t *mailbox_list = os_malloc(sizeof(media_mailbox_list_t));
	mailbox_list->msg = tmp;
	list_add_tail(&mailbox_list->list, list);
	rtos_unlock_mutex(&lock);
	GLOBAL_INT_RESTORE();
}

