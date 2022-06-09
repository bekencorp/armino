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
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	/// the frame id
	uint8_t id;
	/// the flag of end frame, 1 for end
	uint8_t is_eof;
	/// the packet count of one frame
	uint8_t pkt_cnt;
	/// the packet header's count of one frame
	uint8_t pkt_seq;
}vbuf_header_t;

typedef struct {
	/// the video data receive complete
	beken_semaphore_t aready_semaphore;
	/// the receive video data, malloc by user
	uint8_t *buf_base;  // handler in usr thread
	/// video buff length, malloc by user
	uint32_t buf_len;
	/// frame id
	uint32_t frame_id;
	/// the packet count of one frame
	uint32_t frame_pkt_cnt;
	/// recoder the buff ptr of every time receive video packte
	uint8_t *buf_ptr;
	/// the length of receive one frame
	uint32_t frame_len;
	/// video buff receive state
	uint32_t receive_state;
}video_buff_t;

enum video_buff_state {
	/// video buff init
	VIDEO_BUFF_IDLE = 0,
	/// video buff begin copy
	VIDEO_BUFF_COPY,
	/// video buff full
	VIDEO_BUFF_FULL,
};

#ifdef __cplusplus
}
#endif