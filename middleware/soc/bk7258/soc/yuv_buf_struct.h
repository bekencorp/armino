// Copyright 2022-2023 Beken
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
	/* REG_0x00 */
	uint32_t dev_id;

	/* REG_0x01 */
	uint32_t dev_version;

	/* REG_0x02 */
	union {
		struct {
			uint32_t soft_reset:      1; /**< bit[0] soft reset */
			uint32_t clk_gate_bypass: 1; /**< bit[1] bypass clock gate */
			uint32_t reserved:       30; /**< bit[2:31] reserved */
		};
		uint32_t v;
	} global_ctrl;

	/* REG_0x03 */
	uint32_t dev_status;

	/* REG_0x04 */
	union {
		struct {
			uint32_t yuv_mode:           1; /**< bit[0] */
			uint32_t jpeg_word_reverse:  1; /**< bit[1] */
			uint32_t yuv_fmt_sel:        2; /**< bit[2:3] */
			uint32_t reserved0:          1; /**< bit[4] */
			uint32_t vck_edge:           1; /**< bit[5] */
			uint32_t hsync_rev:          1; /**< bit[6] */
			uint32_t vsync_rev:          1; /**< bit[7] */
			uint32_t partial_display_en: 1; /**< bit[8] */
			uint32_t sync_edge_dect_en:  1; /**< bit[9] */
			uint32_t mclk_div:           2; /**< bit[10:11] */
			uint32_t h264_mode:          1; /**< bit[12] */
			uint32_t bps_cis:            1; /**< bit[13] */
			uint32_t memrev:          1; /**< bit[14] */
			uint32_t mclk_hold_en:       1; /**< bit[15] */
			uint32_t sen_mem_clr:        1; /**< bit[16] */
			uint32_t sen_mem_en_hd:      1; /**< bit[17] */
			uint32_t bus_dat_byte_reve:  1; /**< bit[18] */
			uint32_t soi_hsync:          1; /**< bit[19] */
			uint32_t reserved2:         12; /**< bit[20:31] */
		};
		uint32_t v;
	} ctrl;

	/* REG_0x05 */
	union {
		struct {
			uint32_t x_pixel:       8; /**< bit[0:7] */
			uint32_t y_pixel:       8; /**< bit[8:15] */
			uint32_t frame_blk:    16; /**< bit[16:31] */
		};
		uint32_t v;
	} pixel;

	/* REG_0x06 */
	union {
		struct {
			uint32_t x_partial_offset_l: 11; /**< bit[0:10] */
			uint32_t reserved0:           5; /**< bit[11:15] */
			uint32_t x_partial_offset_r: 11; /**< bit[16:26] */
			uint32_t reserved1:           5; /**< bit[27:31] */
		};
		uint32_t v;
	} x_partial_offset;

	/* REG_0x07 */
	union {
		struct {
			uint32_t y_partial_offset_l: 11; /**< bit[0:10] */
			uint32_t reserved0:           5; /**< bit[11:15] */
			uint32_t y_partial_offset_r: 11; /**< bit[16:26] */
			uint32_t reserved1:           5; /**< bit[27:31] */
		};
		uint32_t v;
	} y_partial_offset;

	/* REG_0x08 */
	uint32_t em_base_addr;

	/* REG_0x09 */
	union {
		struct {
			uint32_t vsync_nege_int_en:     1; /**< bit[0] */
			uint32_t yuv_arv_int_en:        1; /**< bit[1] */
			uint32_t sm0_wr_int_en:         1; /**< bit[2] */
			uint32_t sm1_wr_int_en:         1; /**< bit[3] */
			uint32_t sen_full_int_en:       1; /**< bit[4] */
			uint32_t enc_line_int_en:       1; /**< bit[5] */
			uint32_t sen_resl_int_en:       1; /**< bit[6] */
			uint32_t h264_err_int_en:       1; /**< bit[7] */
			uint32_t enc_slow_int_en:       1; /**< bit[8] */
			uint32_t reserved:             23; /**< bit[9:31] */
		};
		uint32_t v;
	} int_en;

	/* REG_0xa */
	union {
		struct {
			uint32_t vsync_nege_int:     1; /**< bit[0] */
			uint32_t yuv_arv_int:        1; /**< bit[1] */
			uint32_t sm0_wr_int:         1; /**< bit[2] */
			uint32_t sm1_wr_int:         1; /**< bit[3] */
			uint32_t sen_full_int:       1; /**< bit[4] */
			uint32_t enc_line_int:       1; /**< bit[5] */
			uint32_t sen_resl_int:       1; /**< bit[6] */
			uint32_t h264_err_int:       1; /**< bit[7] */
			uint32_t enc_slow_int:       1; /**< bit[8] */
			uint32_t reserved:          23; /**< bit[9:31] */
		};
		uint32_t v;
	} int_status;

	uint32_t reserved[0xf - 0xa - 1];

	/* REG_0xf */
	union {
		struct {
			uint32_t renc_start: 1; /**< bit[0] */
			uint32_t reserved:  31; /**< bit[1:31] */
		};
		uint32_t v;
	} renc_start;
} yuv_buf_hw_t;

#ifdef __cplusplus
}
#endif

