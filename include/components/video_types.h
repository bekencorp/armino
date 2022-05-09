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

typedef enum {
	QVGA_320_240    = 0,
	VGA_640_480,
	VGA_800_600,
	VGA_1280_720,
	VGA_480_272,
	PPI_MAX
} ppi_type_t; // Pixel per inch

typedef enum {
	TYPE_5FPS     = 0,
	TYPE_10FPS,
	TYPE_15FPS,
	TYPE_20FPS,
	TYPE_25FPS,
	TYPE_30FPS,
	FPS_MAX
} fps_type_t; // frame per second

#define PPI_POSI        0
#define PPI_MASK        0xFF
#define FPS_POSI        8
#define FPS_MASK        0xFF

#define CMPARAM_SET_PPI(p, x)   (p = ((p & ~(PPI_MASK << PPI_POSI)) | ((x & PPI_MASK) << PPI_POSI)))
#define CMPARAM_GET_PPI(p)      ((p >> PPI_POSI) & PPI_MASK)

#define CMPARAM_SET_FPS(p, x)   (p = ((p & ~(FPS_MASK << FPS_POSI)) | ((x & FPS_MASK) << FPS_POSI)))
#define CMPARAM_GET_FPS(p)      ((p >> FPS_POSI) & FPS_MASK)

typedef enum {
	TVIDEO_OPEN_NONE         = 0,
	TVIDEO_OPEN_SCCB,
	TVIDEO_OPEN_SPIDMA,
} video_open_type_t;

typedef enum {
	TVIDEO_SND_NONE         = 0,
	TVIDEO_SND_UDP,
	TVIDEO_SND_TCP,
	TVIDEO_SND_INTF,
	TVIDEO_SND_BUFFER,
} video_send_type_t;

typedef struct {
	uint8_t *rxbuf;

	void (*node_full_handler)(void *curptr, uint32_t newlen, uint32_t is_eof, uint32_t frame_len);
	void (*data_end_handler)(void);

	uint16_t rxbuf_len;
	uint16_t rx_read_len;
	uint32_t node_len;
	uint32_t sener_cfg;
} video_config_t;

typedef struct {
	uint8_t *ptk_ptr;
	uint32_t ptklen;
	uint32_t frame_id;
	uint32_t is_eof;
	uint32_t frame_len;
} video_packet_t;

typedef void (*tvideo_add_pkt_header)(video_packet_t *param);
typedef int (*video_transfer_send_func)(uint8_t *data, uint32_t len);
typedef void (*video_transfer_start_cb)(void);
typedef void (*video_transfer_end_cb)(void);

typedef struct {
	uint16_t open_type;
	uint16_t send_type;
	video_transfer_send_func send_func;
	video_transfer_start_cb start_cb;
	video_transfer_start_cb end_cb;

	uint32_t pkt_header_size;
	tvideo_add_pkt_header add_pkt_header;
} video_setup_t;

#ifdef __cplusplus
}
#endif