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
#include <driver/dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t *rxbuf;
	uint8_t mem_status;
	uint8_t capture_enable;

	void (*node_full_handler)(void *curptr, uint32_t newlen, uint8_t is_eof, uint32_t frame_len);
	void (*data_end_handler)(void);

	uint32_t rxbuf_len;
	uint32_t rx_read_len;
	uint32_t node_len;
	uint32_t frame_id;
	uint32_t frame_len;

	void (*end_frame_handler)(uint32_t frame_len);
	void (*frame_capture_handler)(uint32_t frame_len);
#if CONFIG_GENERAL_DMA
	dma_isr_t dma_rx_handler;
	uint8_t  dma_channel;
#endif
} uvc_desc_t;

typedef struct {
	uint8_t  type;
	uint32_t data;
}uvc_msg_t;

typedef enum {
	UVC_MEM_IDLE,
	UVC_MEM_BUSY,
	UVC_STATUS_MAX,
} uvc_memory_status_t;

typedef enum {
	UVC_SET_PARAM,
	UVC_START,
	UVC_STOP,
	UVC_EOF,
	UVC_CAPTURE,
	UVC_EXIT,
}uvc_msg_type_t;


#ifdef __cplusplus
}
#endif
