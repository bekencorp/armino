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

#include <driver/dma_types.h>
#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_JPEG_NOT_INIT    (BK_ERR_JPEG_BASE - 1) /**< JPEG driver not init */

typedef uint8_t jpeg_unit_t; /**< jpeg uint id */

typedef struct {
	uint8_t *rx_buf;      /**< the address of receiving jpeg data */
	uint8_t  dma_channel;/**< jpeg encode work mode for transfer data */
	uint8_t  yuv_mode;    /**< jpeg module work mode:0/1:jpegencoder/yuv */
	uint32_t sys_clk_div; /**< div system clock for jpeg module, div = 1/(1+sys_clk_div) */
	uint32_t mclk_div;    /**< div jpeg module clock for MCLK and PCLK, mclk_div:0x00:1/4, 0x01:1/2, 0x10:1/6, 0x11:1/3 */
	uint32_t rx_buf_len;  /**< the length for receiving jpeg data */
	uint32_t node_len;    /**< dma transfer length */
	uint32_t x_pixel;     /**< jpeg encode image resolution for width */
	uint32_t y_pixel;     /**< jpeg encode image resolution for height */
	dma_isr_t dma_rx_finish_handler; /**< dma transfer finish callback */
} jpeg_config_t;

#ifdef __cplusplus
}
#endif

