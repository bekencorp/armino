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
			uint32_t int_en:       4;  /**< bit[0:3] */
			uint32_t mclk_div:     2;  /**< bit[4:5] */
			uint32_t vsync_int_en: 1;  /**< bit[6] */
			uint32_t reserved1:    25; /**< bit[7:31] */
		};
		uint32_t v;
	} int_en;

	/* REG_0x1 */
	union {
		struct {
			uint32_t vck_edge:           1; /**< bit[0] */
			uint32_t video_byte_reverse: 1; /**< bit[1] */
			uint32_t yuv_fmt_sel:        2; /**< bit[2:3] */
			uint32_t jpeg_enc_en:        1; /**< bit[4] */
			uint32_t yuv_word_reverse:   1; /**< bit[5] */
			uint32_t only_y:             1; /**< bit[6] */
			uint32_t yuvbuf_mode:        1; /**< bit[7] */
			uint32_t x_pixel:            8; /**< bit[8:15] */
			uint32_t jpeg_enc_size:      1; /**< bit[16] */
			uint32_t bitrate_ctrl:       1; /**< bit[17] */
			uint32_t bitrate_step:       2; /**< bit[18:19] */
			uint32_t auto_step:          1; /**< bit[20] */
			uint32_t hsync_rev:          1; /**< bit[21] */
			uint32_t vsync_rev:          1; /**< bit[22] */
			uint32_t bitrate_mode:       1; /**< bit[23] */
			uint32_t y_pixel:            8; /**< bit[24:31] */
		};
		uint32_t v;
	} cfg;

	/* REG_0x2 */
	uint32_t target_byte_h;

	/* REG_0x3 */
	uint32_t target_byte_l;

	/* REG_0x4 */
	union {
		struct {
			uint32_t em_base_addr:       32;  //bit[15:0], the highest 16bit address of sensor data saved, bit[31:16]:eof_int offset
		};
		uint32_t v;
	}em_base_addr;

	/* REG_0x5 */
	uint32_t rx_fifo_data;

	/* REG_0x6 */
	union {
		struct {
			uint32_t int_status:        5;  /**< bit[0:4] */
			uint32_t fifo_rd_finish:    1;  /**< bit[5]*/
			uint32_t reserved:          26; /**< bit[6:31] */
		};
		uint32_t v;
	} int_status;

	/* REG_0x7 */
	uint32_t byte_count_pfrm;

	/* REG_0x8 */
	union {
		struct {
			uint32_t x_partial_offset_l: 11;  /**< bit[0:10] */
			uint32_t reserved:           1;   /**< bit[11] */
			uint32_t x_partial_offset_r: 11;  /**< bit[12:22] */
			uint32_t reserved1:          1;   /**< bit[23] */
			uint32_t partial_display_en: 1;   /**< bit[24] */
			uint32_t sync_edge_dect_en:  1;   /**< bit[25] */
			uint32_t stream_fifo_empty:  1;   /**< bit[26] */
			uint32_t stream_fifo_full:   1;   /**< bit[27] */
			uint32_t is_data_set:        1;   /**< bit[28] */
			uint32_t reserved2:          3;   /**< bit[29:31] */
		};
		uint32_t v;
	}x_partial;

	/* REG_0x9 */
	union {
		struct {
			uint32_t y_partial_offset_l: 11;  /**< bit[0:10] */
			uint32_t reserved:           1;   /**< bit[11] */
			uint32_t y_partial_offset_r: 11;  /**< bit[12:22] */
			uint32_t reserved1:          1;   /**< bit[23] */
			uint32_t y_count:            8;   /**< bit[24:31] */
		};
		uint32_t v;
	}y_partial;
} jpeg_hw_t;

#ifdef __cplusplus
}
#endif

