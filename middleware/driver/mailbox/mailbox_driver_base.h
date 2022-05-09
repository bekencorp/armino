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

#include <components/log.h>
#include "mailbox_hal.h"
#include <driver/mailbox.h>
#include <driver/mailbox_types.h>
#include "mailbox_base_map.h"
#include <driver/int.h>

#define MAILBOX_TAG "mailbox"

#if (CONFIG_SHELL_ASYNCLOG && CONFIG_SLAVE_CORE)
#define MAILBOX_LOGI(...) 
#define MAILBOX_LOGW(...) 
#define MAILBOX_LOGE(...) 
#define MAILBOX_LOGD(...) 
#else
#define MAILBOX_LOGI(...) BK_LOGI(MAILBOX_TAG, ##__VA_ARGS__)
#define MAILBOX_LOGW(...) BK_LOGW(MAILBOX_TAG, ##__VA_ARGS__)
#define MAILBOX_LOGE(...) BK_LOGE(MAILBOX_TAG, ##__VA_ARGS__)
#define MAILBOX_LOGD(...) BK_LOGD(MAILBOX_TAG, ##__VA_ARGS__)
#endif

#define MAILBOX_SEND_WAIT_COUNT    1000

typedef void (*mailbox_callback_t)(mailbox_data_t *data);
#define MAILBOX_CALLBACK_NUMBER    2


typedef enum
{
	MAILBOX0 = 0,
	MAILBOX1 = 1,
	MAILBOX2 = 2,
	MAILBOX3 = 3,
} mailbox_num_t;

typedef struct {
	mailbox_num_t id;
	icu_int_src_t icu_src;
	mailbox_endpoint_t	src;
	mailbox_endpoint_t	dst;
} mailbox_config_t;

typedef struct {
	void (*mailbox_isr)();
	void (*rx[MAILBOX_CALLBACK_NUMBER])();
}mailbox_implement_t;


typedef struct {
	mailbox_hal_t hal;
	mailbox_config_t dir;
	mailbox_implement_t imp;
}mailbox_info_t;

bk_err_t bk_mailbox_init(void);
bk_err_t bk_mailbox_deinit(void);
bk_err_t bk_mailbox_set_param(mailbox_data_t *data, uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3);
bk_err_t bk_mailbox_recv_callback_register(mailbox_endpoint_t src, mailbox_endpoint_t dst, mailbox_callback_t callback);
bk_err_t bk_mailbox_recv_callback_unregister(mailbox_endpoint_t src, mailbox_endpoint_t dst);
bk_err_t bk_mailbox_send(mailbox_data_t *data, mailbox_endpoint_t src, mailbox_endpoint_t dst, void *arg);

void mailbox_driver_init(void);
void mailbox_driver_deinit(void);


