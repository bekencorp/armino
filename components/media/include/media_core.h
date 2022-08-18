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

#pragma once

#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEDIA_MAJOR_MSG_QUEUE_SIZE (60)
#define MEDIA_MINOR_MSG_QUEUE_SIZE (60)

#define MEDIA_EVT_BIT (16)

typedef enum
{
	MONO_CPU = 0,
	MAJOR_CPU,
	MINOR_CPU,
} media_cpu_t;


bk_err_t media_major_init(void);
bk_err_t media_minor_init(void);
bk_err_t media_app_init(void);
bk_err_t media_mailbox_send_msg(uint32_t cmd, uint32_t param1, uint32_t param2);

#ifdef CONFIG_DUAL_CORE
#define GET_CPU_ID() get_cpu_id()
#else
#define GET_CPU_ID() (MONO_CPU)
#endif
media_cpu_t get_cpu_id(void);


#ifdef __cplusplus
}
#endif
