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

#include <common/bk_err.h>
#include <bk_list.h>


#ifdef __cplusplus
extern "C" {
#endif

#define PIXEL_240 	(240)
#define PIXEL_272 	(272)
#define PIXEL_320 	(320)
#define PIXEL_480 	(480)
#define PIXEL_600 	(600)
#define PIXEL_640 	(640)
#define PIXEL_720 	(720)
#define PIXEL_800 	(800)
#define PIXEL_1024 (1024)
#define PIXEL_1200 (1200)
#define PIXEL_1280 (1280)
#define PIXEL_1600 (1600)


typedef enum
{
	PPI_DEFAULT     = 0,
	PPI_320X240     = (PIXEL_320 << 16) | PIXEL_240,
	PPI_320X480     = (PIXEL_320 << 16) | PIXEL_480,
	PPI_480X272     = (PIXEL_480 << 16) | PIXEL_272,
	PPI_480X320     = (PIXEL_480 << 16) | PIXEL_320,
	PPI_640X480     = (PIXEL_640 << 16) | PIXEL_480,
	PPI_480X800 	= (PIXEL_480 << 16) | PIXEL_800,
	PPI_800X600     = (PIXEL_800 << 16) | PIXEL_640,
	PPI_1024X600    = (PIXEL_1024 << 16) | PIXEL_600,
	PPI_1280X720    = (PIXEL_1280 << 16) | PIXEL_720,
	PPI_1600X1200   = (PIXEL_1600 << 16) | PIXEL_1200,
} media_ppi_t;

typedef enum
{
	PPI_CAP_UNKNOW 		= 0,
	PPI_CAP_320X240     = (1 << 0), /**< 320 * 240 */
	PPI_CAP_320X480     = (1 << 1), /**< 320 * 480 */
	PPI_CAP_480X272     = (1 << 2), /**< 480 * 272 */
	PPI_CAP_480X320     = (1 << 3), /**< 480 * 320 */
	PPI_CAP_640X480     = (1 << 4), /**< 640 * 480 */
	PPI_CAP_480X800     = (1 << 5), /**< 480 *800 */
	PPI_CAP_800X600     = (1 << 6), /**< 800 * 600 */
	PPI_CAP_1024X600    = (1 << 7), /**< 1024 * 600 */
	PPI_CAP_1280X720    = (1 << 8), /**< 1280 * 720 */
	PPI_CAP_1600X1200   = (1 << 9), /**< 1600 * 1200 */
} media_ppi_cap_t;


typedef enum
{
	STATE_INVALID = 0,
	STATE_ALLOCED,
	STATE_FRAMED,
} frame_state_t;

typedef enum
{
	FRAME_DISPLAY,
	FRAME_JPEG,
} frame_type_t;

typedef struct
{
	LIST_HEADER_T list;
	frame_state_t state;
	frame_type_t type;
	uint8_t id;
	uint8_t lock;
	uint8_t *frame;
	uint32_t length;
	uint32_t size;
	uint32_t sequence;
} frame_buffer_t;


static inline uint16_t ppi_to_pixel_x(media_ppi_t ppi)
{
	return ppi >> 16;
}

static inline uint16_t ppi_to_pixel_y(media_ppi_t ppi)
{
	return ppi & 0xFFFF;
}

static inline uint16_t ppi_to_pixel_x_block(media_ppi_t ppi)
{
	return (ppi >> 16) / 8;
}

static inline uint16_t ppi_to_pixel_y_block(media_ppi_t ppi)
{
	return (ppi & 0xFFFF) / 8;
}


static inline media_ppi_cap_t pixel_ppi_to_cap(media_ppi_t ppi)
{
	media_ppi_cap_t cap = PPI_CAP_UNKNOW;

	switch (ppi)
	{
		case PPI_320X240:
			cap = PPI_CAP_320X240;
			break;

		case PPI_320X480:
			cap = PPI_CAP_320X480;
			break;

		case PPI_480X272:
			cap = PPI_CAP_480X272;
			break;

		case PPI_480X320:
			cap = PPI_CAP_480X320;
			break;

		case PPI_640X480:
			cap = PPI_CAP_640X480;
			break;

		case PPI_480X800:
			cap = PPI_CAP_480X800;
			break;

		case PPI_800X600:
			cap = PPI_CAP_800X600;
			break;

		case PPI_1024X600:
			cap = PPI_CAP_1024X600;
			break;

		case PPI_1280X720:
			cap = PPI_CAP_1280X720;
			break;

		case PPI_1600X1200:
			cap = PPI_CAP_1600X1200;
			break;

		case PPI_DEFAULT:
		default:
			break;
	}

	return cap;
}

static inline uint32_t get_ppi_size(media_ppi_t ppi)
{
	return (ppi >> 16) * (ppi & 0xFFFF) * 2;
}


/*
 * @}
 */

#ifdef __cplusplus
}
#endif
