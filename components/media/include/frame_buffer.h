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
#include <bk_list.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	FB_INDEX_DISPLAY,
	FB_INDEX_JPEG,
	FB_INDEX_MAX,
} fb_type_t;

typedef enum
{
	FB_MEM_SHARED,
	FB_MEM_ALONE,
} fb_mem_mode_t;

typedef enum
{
	MODULE_WIFI,
	MODULE_DECODER,
	MODULE_LCD,
	MODULE_CAPTURE,
	MODULE_MAX,
} frame_module_t;


typedef struct
{
	uint32_t count;
	uint32_t size;
	uint32_t base;
} fb_mem_set_t;

typedef struct
{
	uint32_t ppi;
	fb_mem_set_t set[FB_INDEX_MAX];
} fb_layout_t;


typedef struct
{
	LIST_HEADER_T free;
	LIST_HEADER_T ready;
	beken_mutex_t lock;
	beken_semaphore_t free_sem;
	beken_semaphore_t ready_sem;
	fb_mem_mode_t mode;
	uint8_t enable : 1;
	uint8_t free_request : 1;
	uint8_t ready_request : 1;
	uint8_t count;
	media_ppi_t ppi;
} fb_mem_list_t;


#define INDEX_MASK(bit) (1U << bit)
#define INDEX_UNMASK(bit) (~(1U << bit))

typedef struct
{
	uint8_t enable : 1;
	uint8_t plugin : 1;
	fb_type_t type;
	beken_mutex_t lock;
	beken_semaphore_t sem;
} fb_mod_t;


typedef struct
{
	uint8_t enable;
	uint32_t register_mask;
	fb_mod_t modules[MODULE_MAX];
	beken_mutex_t lock;
} fb_info_t;

typedef struct
{
	LIST_HEADER_T list;
	frame_buffer_t frame;
	uint32_t read_mask;
	uint32_t free_mask;
} frame_buffer_node_t;



void frame_buffer_fb_free(frame_buffer_t *frame, frame_module_t index);
void frame_buffer_fb_push(frame_buffer_t *frame);
bk_err_t frame_buffer_fb_register(frame_module_t index, fb_type_t type);
bk_err_t frame_buffer_fb_deregister(frame_module_t index);
void frame_buffer_init(void);
void frame_buffer_deinit(void);


int frame_buffer_fb_jpeg_init(media_ppi_t max_ppi);
int frame_buffer_fb_display_init(media_ppi_t max_ppi);
int frame_buffer_fb_jpeg_deinit(void);
int frame_buffer_fb_display_deinit(void);
void frame_buffer_fb_jpeg_free(frame_buffer_t *frame);
frame_buffer_t *frame_buffer_fb_jpeg_malloc(void);
frame_buffer_t *frame_buffer_fb_display_malloc(void);
frame_buffer_t *frame_buffer_fb_display_malloc_wait(void);
frame_buffer_t *frame_buffer_fb_read(frame_module_t index);

void frame_buffer_fb_display_free(frame_buffer_t *frame);
void frame_buffer_fb_display_push(frame_buffer_t *frame);
frame_buffer_t *frame_buffer_fb_display_pop(void);
frame_buffer_t *frame_buffer_fb_display_pop_wait(void);


#ifdef __cplusplus
}
#endif
