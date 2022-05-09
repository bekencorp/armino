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

typedef volatile struct {
	/* REG_0x0 */
	union {
		struct {
			uint32_t reserved0: 2;  /**< bit[0:1] */
			uint32_t int_en:    2;  /**< bit[2:3] */
			uint32_t mclk_div:  2;  /**< bit[4:5] */
			uint32_t reserved1: 26; /**< bit[6:31] */
		};
		uint32_t v;
	} int_en;

	/* REG_0x1 */
	union {
		struct {
			uint32_t reserved0:          1; /**< bit[0] */
			uint32_t video_byte_reverse: 1; /**< bit[1] */
			uint32_t yuv_fmt_sel:        2; /**< bit[2:3] */
			uint32_t jpeg_enc_en:        1; /**< bit[4] */
			uint32_t reserved1:          3; /**< bit[5:7] */
			uint32_t x_pixel:            8; /**< bit[8:15] */
			uint32_t jpeg_enc_size:      1; /**< bit[16] */
			uint32_t bitrate_ctrl:       1; /**< bit[17] */
			uint32_t bitrate_step:       3; /**< bit[18:20] */
			uint32_t hsync_rev:          1; /**< bit[21] */
			uint32_t vsync_rev:          1; /**< bit[22] */
			uint32_t reserved2:          1; /**< bit[23] */
			uint32_t y_pixel:            8; /**< bit[24:31] */
		};
		uint32_t v;
	} cfg;

	/* REG_0x2 */
	uint32_t target_byte_h;

	/* REG_0x3 */
	uint32_t target_byte_l;

	/* REG_0x4 */
	uint32_t reserved;

	/* REG_0x5 */
	uint32_t rx_fifo_data;

	/* REG_0x6 */
	union {
		struct {
			uint32_t int_status: 2;  /**< bit[0:1] */
			uint32_t reserved:   30; /**< bit[2:31] */
		};
		uint32_t v;
	} int_status;

	/* REG_0x7 */
	uint32_t byte_count_pfrm;
} jpeg_hw_t;

#ifdef __cplusplus
}
#endif

