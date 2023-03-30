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

#ifdef __cplusplus
extern "C" {
#endif

#define  USE_ROTT_REGISTER_CALLBACKS  0
#define BK_ERR_ROTT_NOT_INIT    (BK_ERR_LCD_BASE - 2) /**< LCD driver not init */
typedef void (*rott_isr_t)(void);

typedef enum {
	ROTATE_COMPLETE_INT =   1 << 0,    /**< rotate finish int enable */
	ROTATE_WARTERMARK_INT = 1 << 1,      /**<rotate watermark int enable. */
	ROTATE_CFG_ERR_INT =    1 << 2,         /**< rotate config error int enable */
} rott_int_type_t;

typedef enum {
	NO_ROTATE_ONLY_YUV2RGB565 = 0, /**< bypass rotating, only use yuv2rgb565 pixel formart convert function */
	CLOKEWISE_ROTATE90_YUV2RGB565,
	ANTICLOCK_ROTATE90_YUV2RGB565,
}rott_mode_t;


typedef enum
{
	INPUT_NORMAL,
	INPUT_REVESE_BYTE_BY_BYTE,            /**< convert input data revese byte by byte. */
	INPUT_REVESE_HALFWORD_BY_HALFWORD,   /**< convert input data revese halfword by halfword */
} rott_input_data_flow_t;

typedef enum
{
	OUTPUT_NORMAL,
	OUTPUT_REVESE_HALFWORD_BY_HALFWORD,  /**< convert output data reverse halfword by halfword*/
} rott_output_data_flow_t;


/*
 * @}
 */

#ifdef __cplusplus
}
#endif


