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
#include "dvp_act.h"
#include "mailbox_channel.h"

static struct list_head media_mb_list;


void mailbox_queue_init(void)
{
	INIT_LIST_HEAD(&media_mb_list);
}

void mailbox_queue_enqueue(void *param)
{
	list_add_tail(param, &media_mb_list);
}

void mailbox_queue_dequeue(void *param)
{
	list_add_tail(param, &media_mb_list);
}

