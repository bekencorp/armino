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
			uint32_t start:        1;  /**< bit[0] */
			uint32_t rsp_en:       1;  /**< bit[1] 1: need response, 0: not need response */
			uint32_t long_rsp_en:  1;  /**< bit[2] 1: need long response, 0: need short response */
			uint32_t crc_check_en: 1;  /**< bit[3] */
			uint32_t cmd_index:    6;  /**< bit[4:9] */
			uint32_t reserved:     22; /**< bit[10:31] */
		};
		uint32_t v;
	} sd_cmd_ctrl;

	/* REG_0x1 */
	uint32_t cmd_argument;

	/* REG_0x2 */
	uint32_t sd_cmd_timer;

	/* REG_0x3 */
	union {
		struct {
			uint32_t sd_data_en:      1;  /**< bit[0] */
			uint32_t sd_data_stop_en: 1;  /**< bit[1] */
			uint32_t sd_data_bus:     1;  /**< bit[2] */
			uint32_t sd_data_mul_blk: 1;  /**< bit[3] */
			uint32_t sd_data_blk:     12; /**< bit[4:15] */
			uint32_t sd_start_wr_en:  1;  /**< bit[16] */
			uint32_t sd_byte_sel:     1;  /**< bit[17] */
			uint32_t reserved:        14; /**< bit[18:31] */
		};
		uint32_t v;
	} sd_data_ctrl;

	/* REG_0x4 */
	uint32_t sd_data_timer;

	/* REG_0x5 */
	uint32_t sd_rsp_agument_0;

	/* REG_0x6 */
	uint32_t sd_rsp_agument_1;

	/* REG_0x7 */
	uint32_t sd_rsp_agument_2;

	/* REG_0x8 */
	uint32_t sd_rsp_agument_3;

	/* REG_0x9 */
	union {
		struct {
			uint32_t sd_cmd_send_no_rsp_end_int:  1; /**< bit[0] */
			uint32_t sd_cmd_send_rsp_end_int:     1; /**< bit[1] */
			uint32_t sd_cmd_send_rsp_timeout_int: 1; /**< bit[2] */
			uint32_t sd_data_rec_end_int:         1; /**< bit[3] */
			uint32_t sd_data_wr_end_int:          1; /**< bit[4] */
			uint32_t sd_data_time_out_int:        1; /**< bit[5] */
			uint32_t rx_fifo_need_read:           1; /**< bit[6] */
			uint32_t tx_fifo_need_write:          1; /**< bit[7] */
			uint32_t rx_overflow_int:             1; /**< bit[8] */
			uint32_t tx_fifo_empty_int:           1; /**< bit[9] */
			uint32_t sd_rsp_cmd_crc_ok:           1; /**< bit[10] */
			uint32_t sd_rsp_cmd_crc_fail:         1; /**< bit[11] */
			uint32_t sd_data_crc_ok:              1; /**< bit[12] */
			uint32_t sd_data_crc_fail:            1; /**< bit[13] */
			uint32_t sd_rsp_index:                6; /**< bit[14:19] */
			uint32_t wr_status:                   3; /**< bit[20:22] */
			uint32_t data_busy:                   1; /**< bit[23] */
			uint32_t reserved:                    8; /**< bit[24:31] */
		};
		uint32_t v;
	} sd_cmd_rsp_int_sel;

	/* REG_0xa */
	union {
		struct {
			uint32_t sd_cmd_send_no_rsp_end_mask:  1;  /**< bit[0] */
			uint32_t sd_cmd_send_rsp_end_mask:     1;  /**< bit[1] */
			uint32_t sd_cmd_send_rsp_timeout_mask: 1;  /**< bit[2] */
			uint32_t sd_data_rec_end_mask:         1;  /**< bit[3] */
			uint32_t sd_data_wr_end_mask:          1;  /**< bit[4] */
			uint32_t sd_data_time_out_mask:        1;  /**< bit[5] */
			uint32_t rx_fifo_need_read_mask:       1;  /**< bit[6] */
			uint32_t tx_fifo_need_write_mask:      1;  /**< bit[7] */
			uint32_t rx_overflow_mask:             1;  /**< bit[8] */
			uint32_t tx_fifo_empty_mask:           1;  /**< bit[9] */
			uint32_t reserved:                     22; /**< bit[10:31] */
		};
		uint32_t v;
	} sd_cmd_rsp_int_mask;

	/* REG_0xb */
	uint32_t tx_fifo_din;

	/* REG_0xc */
	uint32_t rx_fifo_dout;

	/* REG_0xd */
	union {
		struct {
			uint32_t rx_fifo_threshold: 8; /**< bit[0:7] */
			uint32_t tx_fifo_threshold: 8; /**< bit[8:15] */
			uint32_t rx_fifo_reset:     1; /**< bit[16] */
			uint32_t tx_fifo_reset:     1; /**< bit[17] */
			uint32_t rx_fifo_rd_ready:  1; /**< bit[18] */
			uint32_t tx_fifo_wr_ready:  1; /**< bit[19] */
			uint32_t sd_start_reset:    1; /**< bit[20] */
			uint32_t sd_rate_mux:       2; /**< bit[21:22] */
			uint32_t reserved:          9; /**< bit[23:31] */
		};
		uint32_t v;
	} sd_fifo_threshold;
} sdio_hw_t;

#ifdef __cplusplus
}
#endif

