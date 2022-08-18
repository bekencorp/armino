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
#include <driver/media_types.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	DVP_MODE_JPG = 0,
	DVP_MODE_YUV
} dvp_mode_t;


typedef enum
{
	JPEG_96M_MCLK_16M,
	JPEG_96M_MCLK_24M,
	JPEG_120M_MCLK_30M,
} sensor_clk_t;


typedef enum
{
	ID_UNKNOW = 0,
	ID_PAS6329,
	ID_OV7670,
	ID_PAS6375,
	ID_GC0328C,
	ID_BF2013,
	ID_GC0308C,
	ID_HM1055,
	ID_GC2145,
	ID_OV2640
} sensor_id_t;


typedef enum
{
	FPS5    = (1 << 0), /**< 5fps */
	FPS10   = (1 << 1), /**< 10fps */
	FPS15   = (1 << 2), /**< 15fps */
	FPS20   = (1 << 3), /**< 20fps */
	FPS25   = (1 << 4), /**< 25fps */
	FPS30   = (1 << 5), /**< 30fps */
} sensor_fps_t;


/*
 * @}
 */

#ifdef __cplusplus
}
#endif
