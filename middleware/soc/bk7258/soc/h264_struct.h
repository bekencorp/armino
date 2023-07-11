// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
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
	union {
	struct {
	uint32_t enc_en:				1;  /**< bit[0] */
	uint32_t reserved1:				14; /**< bit[1:14] */
	uint32_t keep_buffer_level:		1;  /**< bit[15] */
	uint32_t reserved2:				16; /**< bit[16:31] */
	};
	uint32_t v;
	} enc_ctrl;

	/* REG_0x01 */
	uint32_t img_width;

	/* REG_0x02 */
	uint32_t img_height;

	/* REG_0x03 */
	uint32_t qp;

	/* REG_0x04 */
	uint32_t cqp_offset;

	/* REG_0x05 */
	uint32_t qp_target;

	/* REG_0x06 */
	uint32_t mb_lines;

	/* REG_0x07 */
	uint32_t num_slices;

	/* REG_0x08 */
	union {
	struct {
	uint32_t num_pframes:			10; /**< bit[0:9] */
	uint32_t reserved1:				5;  /**< bit[10:14] */
	uint32_t disable_skip_frame:	1;  /**< bit[15] */
	uint32_t reserved2:				16; /**< bit[16:31] */
	};
	uint32_t v;
	} gop_header;

	/* REG_0x09 */
	union {
	struct {
	uint32_t iframe_I_gain:			8;  /**< bit[0:7] */
	uint32_t iframe_P_gain:			8;  /**< bit[8:15] */
	uint32_t reserved:				16; /**< bit[16:31] */
	};
	uint32_t v;
	} iframe_IP_gain;

	/* REG_0x0A */
	uint32_t iframe_D_gain;

	/* REG_0x0B */
	union {
	struct {
	uint32_t iframe_min_qp:			6;  /**< bit[0:5] */
	uint32_t reserved1:				2;  /**< bit[6:7] */
	uint32_t iframe_max_qp:			6;  /**< bit[8:13] */
	uint32_t reserved2:				18; /**< bit[14:31] */
	};
	uint32_t v;
	} iframe_qp_boundary;

	/* REG_0x0C */
	uint32_t profile_idc;

	/* REG_0x0D */
	uint32_t level_idc;

	/* REG_0x0E */
	union {
	struct {
	uint32_t filter_disable:		1;  /**< bit[0] */
	uint32_t alpha_off:				4;  /**< bit[1:4] */
	uint32_t beta_off:				4;  /**< bit[5:8] */
	uint32_t reserved:				23; /**< bit[9:31] */
	};
	uint32_t v;
	} filter_ctrl;

	/* REG_0x0F */
	uint32_t rev_0x0f;

	/* REG_0x10 */
	uint32_t rev_0x10;

	/* REG_0x11 */
	uint32_t rev_0x11;

	/* REG_0x12 */
	uint32_t rev_0x12;

	/* REG_0x13 */
	uint32_t rev_0x13;

	/* REG_0x14 */
	uint32_t rev_0x14;

	/* REG_0x15 */
	uint32_t rev_0x15;

	/* REG_0x16 */
	uint32_t rev_0x16;

	/* REG_0x17 */
	union {
	struct {
	uint32_t num_imb_bits:			12; /**< bit[0;11] */
	uint32_t reserved1:				1;  /**< bit[12] */
	uint32_t nal_align:				1;  /**< bit[13] */
	uint32_t cons_ipred:			1;  /**< bit[14] */
	uint32_t reserved2:				17; /**< bit[15:31] */
	};
	uint32_t v;
	} iframe_bit_ctrl;

	/* REG_0x18 */
	uint32_t num_pmb_bits;

	/* REG_0x19 */
	uint32_t rev_0x19;

	/* REG_0x1A */
	union {
	struct {
	uint32_t pframe_I_gain:			8;  /**< bit[0:7] */
	uint32_t pframe_P_gain:			8;  /**< bit[8:15] */
	uint32_t reserved:				16; /**< bit[16:31] */
	};
	uint32_t v;
	} pframe_IP_gain;

	/*  REG_0x1B*/
	uint32_t pframe_D_gain;

	/* REG_0x1C */
	union {
	struct {
	uint32_t pframe_min_qp:			6;  /**< bit[0:5] */
	uint32_t reserved1:				2;  /**< bit[6:7] */
	uint32_t pframe_max_qp:			6;  /**< bit[7:13] */
	uint32_t reserved2:				18; /**< bit[14:31] */
	};
	uint32_t v;
	} pframe_qp_boudary;

	/* REG_0x1D */
	union {
	struct {
	uint32_t pbits_fact1:			5;  /**< bit[0:4] */
	uint32_t pbits_fact2:			5;  /**< bit[5:9] */
	uint32_t pbits_fact3:			5;  /**< bit[10:14] */
	uint32_t reserved:				17; /**< bit[15:31] */
	};
	uint32_t v;
	} pbits_fact_1to3;

	/* REG_0x1E */
	union {
	struct {
	uint32_t pbits_fact4:			5;  /**< bit[0:4] */
	uint32_t pbits_fact5:			5;  /**< bit[5:9] */
	uint32_t reserved:				22; /**< bit[10:31] */
	};
	uint32_t v;
	} pbits_fact_4to5;

	/* REG_0x1F */
	uint32_t num_ref_bits:			12; /**< bit[0:11] */
	uint32_t reserved_0x1f_1:		3;  /**< bit[12:14] */
	uint32_t reset_i_state:			1;  /**< bit[15] */
	uint32_t reserved_0x1f_2:		16; /**< bit[16:31] */

	/* REG_0x20 */
	uint32_t vui_time_scale_L;

	/* REG_0x21 */
	uint32_t vui_time_scale_H;

	/* REG_0x22 */
	uint32_t vui_num_u_tick_L;

	/* REG_0x23 */
	uint32_t vui_num_u_tick_H;

	/* REG_0x24 */
	union {
	struct {
	uint32_t bitrate_scale:			4;  /**< bit[0:3] */
	uint32_t cpb_size_scale:		4;  /**< bit[4:7] */
	uint32_t reserved:				24; /**< bit[8:31] */
	};
	uint32 v;
	} scale;

	/* REG_0x25 */
	uint32_t bitrate_value_less1;

	/* REG_0x26 */
	uint32_t cpb_size_value_less1;

	/* REG_0x27 */
	uint32_t init_cpb_removal_delay;

	/* REG_0x28 */
	uint32_t init_enc_buffer_level_L;

	/* REG_0x29 */
	uint32_t init_enc_buffer_level_H;

	/* REG_0x2A */
	uint32_t bits_per_frame_int_L;

	/* REG_0x2B */
	uint32_t bits_per_frame_int_H;

	/* REG_0x2C */
	uint32_t bits_per_frame_rem_L;

	/* REG_0x2D */
	uint32_t bits_per_frame_rem_H;

	/* REG_0x2E */
	uint32_t cpb_size_L;

	/* REG_0x2F */
	uint32_t cpb_size_H;

	/* REG_0x30 */
	uint32_t soft_reset;

	/* skip 9 reg */
	uint32_t skip_reg_0x31_to_0x39[15];

	/* REG_0x40 */
	uint32_t devid;

	/* REG_0x41 */
	uint32_t verid;

	/* REG_0x42 */

	union {
	struct {
	uint32_t soft_reset:			1;  /**< bit[0] */
	uint32_t clkg_bypass:			1;  /**< bit[1] */
	uint32_t reserved_0x42:			30; /**< bit[2:31] */
	};
	uint32 v;
	} global_ctrl;

	/* REG_0x43 */
	uint32_t devstatus;

	/* REG_0x44 */
	uint32_t rsvd1;

	/* REG_0x45 */
	uint32_t rsvd2;

	/* REG_0x46 */
	union {
	struct {
	uint32_t skip_fram:				1;  /**< bit[0] */
	uint32_t final_out:				1;  /**< bit[1] */
	uint32_t line_done:				1;  /**< bit[2] */
	uint32_t reserved_0x46:			29; /**< bit[3:31] */
	};
	uint32_t v;
	} int_stat;

	/* REG_0x47 */
	uint32_t int_en;

	/* REG_0x48 */
	uint32_t rxfifo_data;

	/* REG_0x49 */
	uint32_t r_h264_clk_en;

	/* REG_0x4A */
	uint32_t encoder_cnt;

} h264_hw_t;

#ifdef __cplusplus
}
#endif