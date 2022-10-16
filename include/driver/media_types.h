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


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief define all image resolution, unit pixel
 * @{
 */

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

typedef enum {
	MSTATE_TURN_OFF,
	MSTATE_TURNING_OFF,
	MSTATE_TURNING_ON,
	MSTATE_TURN_ON,
} media_state_t;


typedef enum
{
	PPI_DEFAULT     = 0,
	PPI_320X240     = (PIXEL_320 << 16) | PIXEL_240,
	PPI_320X480     = (PIXEL_320 << 16) | PIXEL_480,
	PPI_480X272     = (PIXEL_480 << 16) | PIXEL_272,
	PPI_480X320     = (PIXEL_480 << 16) | PIXEL_320,
	PPI_640X480     = (PIXEL_640 << 16) | PIXEL_480,
	PPI_480X800 	= (PIXEL_480 << 16) | PIXEL_800,
	PPI_800X480 	= (PIXEL_800 << 16) | PIXEL_480,
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
	PPI_CAP_480X800     = (1 << 5), /**< 480 * 800 */
	PPI_CAP_800X480     = (1 << 6), /**< 800 * 480 */
	PPI_CAP_800X600     = (1 << 7), /**< 800 * 600 */
	PPI_CAP_1024X600    = (1 << 8), /**< 1024 * 600 */
	PPI_CAP_1280X720    = (1 << 9), /**< 1280 * 720 */
	PPI_CAP_1600X1200   = (1 << 10), /**< 1600 * 1200 */
} media_ppi_cap_t;

/** rgb lcd input data format, data save in mem is little endian, like VUYY format is [bit31-bit0] is [V U Y Y]*/
typedef enum {
	PIXEL_FMT_UNKNOW,
	PIXEL_FMT_DVP_JPEG,
	PIXEL_FMT_UVC_JPEG,
	PIXEL_FMT_RGB565,        /**< input data format is rgb565*/
	PIXEL_FMT_YUYV,    /**< input data is yuyv format, diaplay module internal may convert to rgb565 data*/
	PIXEL_FMT_UYVY,
	PIXEL_FMT_YYUV,            /**< input data is yyuv format */
	PIXEL_FMT_UVYY,            /**< input data is uvyy format*/
	PIXEL_FMT_VUYY,            /**< input data is vuyy format */
	PIXEL_FMT_YVYU,
	PIXEL_FMT_VYUY,
	PIXEL_FMT_YYVU
} pixel_format_t;

/**
 * @brief define frame buffer
 * @{
 */
typedef struct
{
	pixel_format_t fmt;
	uint8_t mix : 1;
	uint8_t *frame;
	uint16_t width;
	uint16_t height;
	uint32_t length;
	uint32_t size;
	uint32_t sequence;
} frame_buffer_t;

/**
 * @brief define camera type
 * @{
 */
typedef enum
{
	UNKNOW_CAMERA,
	DVP_CAMERA,    /**< dvp camera */
	UVC_CAMERA,    /**< uvc camera */
} camera_type_t;

/**
 * @brief define struct for debug
 * @{
 */
typedef struct
{
	uint16_t isr_jpeg;    /**< jpeg frame count */
	uint16_t isr_decoder; /**< jpeg decode frame count */
	uint16_t err_dec;     /**< jpeg decode error count */
	uint16_t isr_lcd;     /**< lcd display count */
	uint16_t fps_lcd;     /**< lcd display fps */
	uint16_t fps_wifi;    /**< wifi transfer fps */
	uint16_t wifi_read;   /**< wifi transfer read cnt */
} media_debug_t;

/**
 * @brief get camera width
 * @{
 */
static inline uint16_t ppi_to_pixel_x(media_ppi_t ppi)
{
	return ppi >> 16;
}

/**
 * @brief get camera height
 * @{
 */
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

/**
 * @brief get camera support ppi compare with user set
 * @{
 */
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

		case PPI_800X480:
			cap = PPI_CAP_800X480;
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

/**
 * @brief get yuv422 image size
 * @{
 */
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
