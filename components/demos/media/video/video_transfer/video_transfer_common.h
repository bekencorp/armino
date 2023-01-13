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
#include "video_co_list.h"
#include <components/video_types.h>

#ifdef __cplusplus
extern "C" {
#endif


#define TVIDEO_DROP_DATA_NONODE     0
#define TVIDEO_USE_HDR              1

#define TVIDEO_RXNODE_SIZE_UDP      1472
#define TVIDEO_RXNODE_SIZE_TCP      1460
#ifndef TVIDEO_RXNODE_SIZE
#define TVIDEO_RXNODE_SIZE          TVIDEO_RXNODE_SIZE_UDP
#endif

#define TVIDEO_DROP_DATA_FLAG       0x01

#if TVIDEO_DROP_DATA_NONODE
#define TVIDEO_POOL_LEN             (TVIDEO_RXNODE_SIZE * 38)  // 54KB
#else
#define TVIDEO_POOL_LEN             (TVIDEO_RXNODE_SIZE * 25)  // 7KB
#endif

#define TVIDEO_RXBUF_LEN            (TVIDEO_RXNODE_SIZE_UDP * 4)


typedef struct {
	uint8_t  buffer_id;
	uint8_t  buffer_state;
	uint32_t buffer_addr;
	uint32_t buffer_len;
	uint32_t frame_id;
	uint32_t already_len;
	uint32_t frame_len;
} frame_information_t;

typedef enum {
	VIDEO_SEND,
	VIDEO_EOF_CHECK,
	VIDEO_EXIT,
} video_msg_type_t;

typedef struct {
	video_msg_type_t type;
	uint32_t data;
}video_msg_t;

typedef struct {
	struct co_list_hdr hdr;
	void *buf_start;
	uint32_t buf_len;
	uint32_t frame_id;
} video_elem_t;

typedef struct {
	uint8_t *pool;
	video_elem_t elem[TVIDEO_POOL_LEN / TVIDEO_RXNODE_SIZE];
	struct co_list free;
	struct co_list ready;

#if TVIDEO_DROP_DATA_NONODE
	struct co_list receiving;
	uint32_t drop_pkt_flag;
#endif

	uint16_t open_type;
	uint16_t send_type;
	video_transfer_send_func send_func;
	video_transfer_start_cb start_cb;
	video_transfer_end_cb end_cb;

#if(TVIDEO_USE_HDR)
	uint16_t frame_id;
	uint16_t pkt_header_size;
	tvideo_add_pkt_header add_pkt_header;
#endif
} video_pool_t;

#ifdef __cplusplus
}
#endif
