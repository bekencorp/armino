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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	media_storage_state_t state;
	media_storage_state_t capture_state;
	frame_buffer_t *frame;
} storage_info_t;

typedef struct
{
	uint32_t flash_image_addr;
	uint32_t flasg_img_length;
} storage_flash_t;


media_storage_state_t get_storage_state(void);
void set_storage_state(media_storage_state_t state);
void storage_init(void);
void storage_frame_buffer_dump(frame_buffer_t *frame, char *name);


void storage_capture_frame_notify(frame_buffer_t *frame);

void storage_event_handle(uint32_t event, uint32_t param);
bk_err_t sdcard_read_to_mem(char *filename, uint32_t *paddr, uint32_t *total_len);
bk_err_t storage_mem_to_sdcard(char *filename, uint8_t *paddr, uint32_t total_len);
bk_err_t sdcard_read_filelen(char *filename);


#ifdef __cplusplus
}
#endif
