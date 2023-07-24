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
#include <driver/media_types.h>
#include "frame_buffer.h"
#include "media_mailbox_list_util.h"
#include <components/video_types.h>

#include "media_app.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	media_trs_state_t state;
	uint8_t pause : 1;
	uint8_t debug : 1;
	uint32_t param;
} transfer_info_t;

typedef struct
{
	media_trs_state_t state;
	uint8_t pause : 1;
	uint8_t debug : 1;
	uint32_t param;
} usb_trs_info_t;

typedef struct
{
	uint8_t id;
	uint8_t eof;
	uint8_t cnt;
	uint8_t size;
	uint8_t data[];
} transfer_data_t;

typedef struct
{
	uint8_t type;
	uint32_t data;
} trs_task_msg_t;

typedef enum
{
	TRS_TRANSFER_START,
	TRS_TRANSFER_DATA,
	TRS_TRANSFER_EXIT,
	TRS_USB_START,
	TRS_USB_DATA,
	TRS_USB_EXIT,
} trs_task_msg_type_t;

void set_transfer_state(media_trs_state_t state);
media_trs_state_t get_transfer_state(void);
#ifdef CONFIG_INTEGRATION_DOORBELL
bk_err_t transfer_app_task_init(const media_transfer_cb_t *cb);
#else
bk_err_t transfer_app_task_init(video_setup_t *config);
#endif
void transfer_init(void);
bk_err_t transfer_app_event_handle(media_mailbox_msg_t *msg);
bk_err_t transfer_major_event_handle(media_mailbox_msg_t *msg);

void set_usb_trs_state(media_trs_state_t state);
media_trs_state_t get_usb_trs_state(void);
bk_err_t usb_app_task_init(video_setup_t *config);
void usb_transfer_init(void);
bk_err_t usb_app_event_handle(media_mailbox_msg_t *msg);
bk_err_t usb_major_event_handle(media_mailbox_msg_t *msg);

#ifdef __cplusplus
}
#endif
