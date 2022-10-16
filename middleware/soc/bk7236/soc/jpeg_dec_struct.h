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
			uint32_t clk_gate_bypass: 1; /**< bit[1] bypass uart clock gate */
			uint32_t reserved:       30; /**< bit[2:31] reserved */
		};
		uint32_t v;
	} global_ctrl;

	/* REG_0x03 */
	uint32_t dev_status;

	/* REG_0x04 */
	uint32_t jd_mask_cur;

	/* REG_0x05 */
	union {
		struct {
			uint32_t mcu_x:    16; /**< bit[0:15] */
			uint32_t reserved: 16; /**< bit[16:31] */
		};
		uint32_t v;
	} mcu_x;

	/* REG_0x06 */
	union {
		struct {
			uint32_t mcu_y:    16; /**< bit[0:15] */
			uint32_t reserved: 16; /**< bit[16:31] */
		};
		uint32_t v;
	} mcu_y;

	/* REG_0x7 */
	union {
		struct {
			uint32_t v_vld_num: 4; /**< bit[0:3] */
			uint32_t reserved: 28; /**< bit[4:31] */
		};
		uint32_t v;
	} v_vld_num;

	/* REG_0x8 */
	union {
		struct {
			uint32_t dec_cmd:   4; /**< bit[0:3] */
			uint32_t reserved: 28; /**< bit[4:31] */
		};
		uint32_t v;
	} dec_cmd;

	/* REG_0x9 */
	uint32_t cur_run;

	/* REG_0xa */
	union {
		struct {
			uint32_t x_pixel:  16; /**< bit[0:15] */
			uint32_t reserved: 16; /**< bit[16:31] */
		};
		uint32_t v;
	} x_pixel;

	/* REG_0xb */
	uint32_t huf_data;

	/* REG_0xc */
	uint32_t cur_nbit;

	/* REG_0xd */
	union {
		struct {
			uint32_t state_dec_busy:    1; /**< bit[0] */
			uint32_t state_rrload:      1; /**< bit[1] */
			uint32_t state_rload:       1; /**< bit[2] */
			uint32_t state_search:      1; /**< bit[3] */
			uint32_t state_ext_rload:   1; /**< bit[4] */
			uint32_t state_ext_bits:    1; /**< bit[5] */
			uint32_t state_idct:        1; /**< bit[6] */
			uint32_t state_tmp_wr0:     1; /**< bit[7] */
			uint32_t state_dec_busy2:   1; /**< bit[8] */
			uint32_t state_ahbmst_idle: 1; /**< bit[9] */
			uint32_t reserved:         22; /**< bit[10:31] */
		};
		uint32_t v;
	} state;

	/* REG_0xe */
	uint32_t ext_data;

	/* REG_0xf */
	union {
		struct {
			uint32_t mcu_blk:   8; /**< bit[0:7] */
			uint32_t reserved: 24; /**< bit[8:31] */
		};
		uint32_t v;
	} mcu_blk;

	/* REG_0x10 - REG_0x1f */
	uint32_t hufdc_len_y[0xf];

	/* REG_0x20 - REG_0x2f */
	uint32_t hufdc_len_uv[0xf];

	/* REG_0x30 - REG_0x3f */
	uint32_t hufac_len_y[0xf];

	/* REG_0x40 - REG_0x4f */
	uint32_t hufac_len_uv[0xf];

	/* REG_0x50 */
	union {
		struct {
			uint32_t jpeg_dec_en:  1; /**< bit[0] */
			uint32_t reserved:    28; /**< bit[1:28] */
			uint32_t dri_bps:      1; /**< bit[29] */
			uint32_t yy_vld_bps:   1; /**< bit[30] */
			uint32_t sharemem_bps: 1; /**< bit[31] */
		};
		uint32_t v;
	} jpeg_dec_en;

	/* REG_0x51 */
	union {
		struct {
			uint32_t mcu_index: 16; /**< bit[0:15] */
			uint32_t mcu_y_num: 10; /**< bit[16:25] */
			uint32_t reserved:   6; /**< bit[26:31] */
		};
		uint32_t v;
	} mcu_index;

	/* REG_0x52 */
	union {
		struct {
			uint32_t jpeg_dec_auto:  1; /**< bit[0] */
			uint32_t jpeg_dec_linen: 1; /**< bit[1] */
			uint32_t reserved0:      6; /**< bit[2:7] */
			uint32_t jpeg_line_num: 13; /**< bit[8:20] */
			uint32_t reserved1:     11; /**< bit[21:31] */
		};
		uint32_t v;
	} jpeg_dec_auto;

	/* REG_0x53 */
	union {
		struct {
			uint32_t uv_vld:    1; /**< bit[0] */
			uint32_t reserved: 31; /**< bit[1:31] */
		};
		uint32_t v;
	} uv_vld;

	uint32_t reserved[0x57 - 0x53 - 1];

	/* REG_0x57 */
	uint32_t master_wr_cnt;

	/* REG_0x58 */
	uint32_t base_raddr;

	/* REG_0x59 */
	uint32_t base_waddr;

	/* REG_0x5a */
	uint32_t base_rd_len;

	/* REG_0x5b */
	uint32_t base_wr_len;

	/* REG_0x5c */
	uint32_t base_ffda;

	/* REG_0x5d */
	uint32_t master_rd_cnt;

	/* REG_0x5e */
	union {
		struct {
			uint32_t dec_busy_int:       1; /**< bit[0] */
			uint32_t dec_rrload_int:     1; /**< bit[1] */
			uint32_t dec_rload_int:      1; /**< bit[2] */
			uint32_t dec_search_int:     1; /**< bit[3] */
			uint32_t dec_ext_reload_int: 1; /**< bit[4] */
			uint32_t dec_ext_bit_int:    1; /**< bit[5] */
			uint32_t dec_total_busy_int: 1; /**< bit[6] */
			uint32_t dec_mcu_finish_int: 1; /**< bit[7] */
			uint32_t dec_frame_int:      1; /**< bit[8] */
			uint32_t reserved:          23; /**< bit[9:31] */
		};
		uint32_t v;
	} int_en;
} jpeg_dec_hw_t;

#ifdef __cplusplus
}
#endif

