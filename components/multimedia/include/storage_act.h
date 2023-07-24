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

#include "media_mailbox_list_util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	STORAGE_STATE_DISABLED,
	STORAGE_STATE_ENABLED,
} storage_state_t;


typedef struct
{
	storage_state_t state;
	storage_state_t capture_state;
	frame_buffer_t *frame;
	uint32_t param;
} storage_info_t;

typedef struct
{
	uint32_t flash_image_addr;
	uint32_t flasg_img_length;
} storage_flash_t;

typedef struct
{
	uint8_t type;
	uint32_t data;
} storages_task_msg_t;


typedef enum
{
	STORAGE_TASK_CAPTURE,
	STORAGE_TASK_SAVE,
	STORAGE_TASK_SAVE_STOP,
	STORAGE_TASK_EXIT,
} storage_task_evt_t;

bk_err_t storage_app_set_frame_name(char *name);
storage_state_t get_storage_state(void);
void set_storage_state(storage_state_t state);
void storage_init(void);
void storage_frame_buffer_dump(frame_buffer_t *frame, char *name);
bk_err_t storage_app_event_handle(media_mailbox_msg_t *msg);
bk_err_t storage_major_event_handle(media_mailbox_msg_t *msg);
bk_err_t sdcard_read_to_mem(char *filename, uint32_t *paddr, uint32_t *total_len);
bk_err_t storage_mem_to_sdcard(char *filename, uint8_t *paddr, uint32_t total_len);
bk_err_t sdcard_read_filelen(char *filename);
bk_err_t read_storage_file_to_mem_handle(media_mailbox_msg_t *msg);


#ifdef __cplusplus
}
#endif
