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

typedef enum
{
	MODULE_DECODER,
	MODULE_WIFI,
	MODULE_RECODER,
	MODULE_CAPTURE,
	MODULE_DISPLAY,
} frame_module_t;

typedef struct
{
	uint8_t enable : 1;
	uint8_t wifi_register : 1;
	uint8_t wifi_lock : 1;
	uint8_t decoder_register : 1;
	uint8_t decoder_lock : 1;
	uint8_t recoder_register : 1;
	uint8_t recoder_lock : 1;
	uint8_t capture_register : 1;
	uint8_t capture_lock : 1;
	uint8_t display_register : 1;
	uint8_t display_lock : 4;
	uint8_t minimal_layout : 1;
	void (*wifi_comp_cb)(frame_buffer_t *buffer);
	void (*decoder_comp_cb)(frame_buffer_t *buffer);
	void (*recoder_comp_cb)(frame_buffer_t *buffer);
	void (*capture_comp_cb)(frame_buffer_t *buffer);
	uint8_t (*display_comp_cb)(frame_buffer_t *buffer);
} frame_buffer_info_t;

void frame_buffer_generate_complete(frame_buffer_t *buffer, frame_type_t type);
frame_buffer_t *frame_buffer_alloc(frame_type_t type);
frame_buffer_t *frame_buffer_get_available_frame(frame_type_t type);
void frame_buffer_free_request(frame_buffer_t *buffer, frame_module_t module);
void frame_buffer_frame_register(frame_module_t module, void *callback);
void frame_buffer_frame_deregister(frame_module_t module);
void frame_buffer_init(void);
void frame_buffer_deinit(void);
bool frame_buffer_get_state(void);
void frame_buffer_enable(bool enable);
int frame_buffer_set_ppi(media_ppi_t ppi);
void frame_buffer_push_frame(frame_buffer_t *buffer);
void frame_buffer_lock_frame(frame_buffer_t *frame);
void frame_buffer_free_frame(frame_buffer_t *buffer);
int frame_buffer_display_frame_init(void);


#ifdef __cplusplus
}
#endif
