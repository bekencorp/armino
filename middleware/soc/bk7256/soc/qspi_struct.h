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
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_a_l;

	/* REG_0x1 */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_a_h;

	/* REG_0x2 */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_a_cfg1;

	/* REG_0x3 */
	union {
		struct {
			uint32_t reserved:     14;   /**< bit[ 0:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved1:      1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved2:      3;   /**< bit[27:29] */
			uint32_t cmd_mode:      2;   /**< bit[30:31] */
		};
		uint32_t v;
	} cmd_a_cfg2;

	/* REG_0x4 */
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_b_l;

	/* REG_0x5 */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_b_h;

	/* REG_0x6 */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_b_cfg1;

	/* REG_0x7 */
	union {
		struct {
			uint32_t reserved:     14;   /**< bit[ 0:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved1:      1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved2:      3;   /**< bit[27:29] */
			uint32_t cmd_mode:      2;   /**< bit[30:31] */
		};
		uint32_t v;
	} cmd_b_cfg2;

	/* REG_0x8 */
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_c_l;

	/* REG_0x9 */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_c_h;

	/* REG_0xa */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_c_cfg1;

	/* REG_0xb */
	union {
		struct {
			uint32_t cmd_start:     1;   /**< bit[0] */
			uint32_t reserved:      1;   /**< bit[1] */
			uint32_t data_len:     10;   /**< bit[ 2:11] */
			uint32_t reserved1:      2;   /**< bit[12:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved2:      1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved3:      5;   /**< bit[27:31] */
		};
		uint32_t v;
	} cmd_c_cfg2;


	/* REG_0xc */
	union {
		struct {
			uint32_t cmd5:    8; /**< bit[ 0: 7] */
			uint32_t cmd6:    8; /**< bit[ 8:15] */
			uint32_t cmd7:    8; /**< bit[16:23] */
			uint32_t cmd8:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_d_l;

	/* REG_0xd */
	union {
		struct {
			uint32_t cmd1:    8; /**< bit[ 0: 7] */
			uint32_t cmd2:    8; /**< bit[ 8:15] */
			uint32_t cmd3:    8; /**< bit[16:23] */
			uint32_t cmd4:    8; /**< bit[24:31] */
		};
		uint32_t v;
	} cmd_d_h;

	/* REG_0xe */
	union {
		struct {
			uint32_t cmd1_line:    2;   /**< bit[ 0: 1] */
			uint32_t cmd2_line:    2;   /**< bit[ 2: 3] */
			uint32_t cmd3_line:    2;   /**< bit[ 4: 5] */
			uint32_t cmd4_line:    2;   /**< bit[ 6: 7] */
			uint32_t cmd5_line:    2;   /**< bit[ 8: 9] */
			uint32_t cmd6_line:    2;   /**< bit[10:11] */
			uint32_t cmd7_line:    2;   /**< bit[12:13] */
			uint32_t cmd8_line:    2;   /**< bit[14:15] */
			uint32_t reserved:     16;  /**< bit[16:31] */
		};
		uint32_t v;
	} cmd_d_cfg1;

	/* REG_0xf */
	union {
		struct {
			uint32_t cmd_start:     1;   /**< bit[0] */
			uint32_t reserved:      1;   /**< bit[1] */
			uint32_t data_len:     10;   /**< bit[ 2:11] */
			uint32_t reserved1:      2;   /**< bit[12:13] */
			uint32_t data_line:     2;   /**< bit[14:15] */
			uint32_t dummy_clock:   7;   /**< bit[16:22] */
			uint32_t reserved2:      1;   /**< bit[23]    */
			uint32_t dummy_mode:    3;   /**< bit[24:26] */
			uint32_t reserved3:      5;   /**< bit[27:31] */
		};
		uint32_t v;
	} cmd_d_cfg2;

	/* REG_0x10 ~ REG_0x1b */
	uint32_t reserved0[12];

	/* REG_0x1c */
	union {
		struct {
			uint32_t qspi_en:           1;   /**< bit[0] */
			uint32_t cpol:              1;   /**< bit[1] */
			uint32_t cpha:              1;   /**< bit[2] */
			uint32_t io2_io3_mode:      1;   /**< bit[3] */
			uint32_t io2:               1;   /**< bit[4] */
			uint32_t io3:               1;   /**< bit[5] */
			uint32_t reserved:          1;   /**< bit[6] */
			uint32_t nss_h_gen_sck:     1;   /**< bit[7] */
			uint32_t clk_rate:          8;   /**< bit[ 8:15] */
			uint32_t reserved1:          2;   /**< bit[16:17] */
			uint32_t first_bit_mode:    1;   /**< bit[18]    */
			uint32_t reserved2:          5;   /**< bit[19:23] */
			uint32_t qspi_cs_h_wait:    8;   /**< bit[24:31] */
		};
		uint32_t v;
	} config;

	/* REG_0x1d ~ REG_0x1e */
	uint32_t reserved1[2];

	/* REG_0x1f */
	union {
		struct {
			uint32_t reserved:          1;   /**< bit[0]    */
			uint32_t sw_rst_fifo:       1;   /**< bit[1]    */
			uint32_t reserved1:         30;   /**< bit[2:31] */
		};
		uint32_t v;
	} rst_cfg;

	/* REG_0x20 */
	union {
		struct {
			uint32_t int_rx_done:          1;   /**< bit[0]    */
			uint32_t int_tx_done:          1;   /**< bit[1]    */
			uint32_t int_cmd_start_done:   1;   /**< bit[2]    */
			uint32_t int_qspi_done:        1;   /**< bit[3]    */
			uint32_t int_cmd_start_fail:   1;   /**< bit[4]    */
			uint32_t reserved:            27;   /**< bit[5:31] */
		};
		uint32_t v;
	} int_en;

	/* REG_0x21 */
	union {
		struct {
			uint32_t clr_rx_done:          1;   /**< bit[0]    */
			uint32_t clr_tx_done:          1;   /**< bit[1]    */
			uint32_t clr_cmd_start_done:   1;   /**< bit[2]    */
			uint32_t clr_qspi_done:        1;   /**< bit[3]    */
			uint32_t clr_cmd_start_fail:   1;   /**< bit[4]    */
			uint32_t reserved:            27;   /**< bit[5:31] */
		};
		uint32_t v;
	} status_clr;

	/* REG_0x22 */
	union {
		struct {
			uint32_t rx_done:          1;   /**< bit[0]    */
			uint32_t tx_done:          1;   /**< bit[1]    */
			uint32_t cmd_start_done:   1;   /**< bit[2]    */
			uint32_t qspi_done:        1;   /**< bit[3]    */
			uint32_t cmd_start_fail:   1;   /**< bit[4]    */
			uint32_t reserved:         7;   /**< bit[ 5:11]*/
			uint32_t qspi_cs:          1;   /**< bit[12]   */
			uint32_t rx_busy:          1;   /**< bit[13]   */
			uint32_t tx_busy:          1;   /**< bit[14]   */
			uint32_t reserved1:        1;   /**< bit[15]   */
			uint32_t fifo_empty:       1;   /**< bit[16]   */
			uint32_t reserved2:        15;  /**< bit[17:31]*/
		};
		uint32_t v;
	} status;

	/* REG_0x23 */
	union {
		struct {
			uint32_t led_wr_continue_cmd:  24;   /**< bit[ 0:23]*/
			uint32_t reserved:             8;   /**< bit[24:31] */
		};
		uint32_t v;
	} led_cfg;

	/* REG_0x24 ~ REG_0x3f */
	uint32_t reserved2[28];

	/* REG_0x40 ~ REG_0x7f */
	uint32_t fifo_data[64];
} qspi_hw_t;

#ifdef __cplusplus
}
#endif

