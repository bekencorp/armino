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
			uint32_t sw_cmd_en:  1;  /**< bit[0] */
			uint32_t sw_cmd_bw:  1;  /**< bit[1] */
			uint32_t sw_cmd:     8;  /**< bit[2:9] */
			uint32_t sw_cmd_cnt: 8;  /**< bit[10:17] */
			uint32_t reserved:   14; /**< bit[18:31] */
		};
		uint32_t v;
	} sw_cmd;

	/* REG_0x1 */
	union {
		struct {
			uint32_t sw_addr_en:  1;  /**< bit[0] */
			uint32_t sw_addr_bw:  1;  /**< bit[1] */
			uint32_t sw_addr:     24; /**< bit[2:25] */
			uint32_t sw_addr_cnt: 5;  /**< bit[26:30] */
			uint32_t reserved:    1;  /**< bit[31] */
		};
		uint32_t v;
	} sw_addr;

	/* REG_0x2 */
	union {
		struct {
			uint32_t sw_dum_en:  1;  /**< bit[0] */
			uint32_t sw_dum_bw:  1;  /**< bit[1] */
			uint32_t sw_dum_cnt: 4;  /**< bit[2:5] */
			uint32_t reserved:   26; /**< bit[6:31] */
		};
		uint32_t v;
	} sw_dum;

	/* REG_0x3 */
	union {
		struct {
			uint32_t sw_dat_en:  1;  /**< bit[0] */
			uint32_t sw_dat_bw:  1;  /**< bit[1] */
			uint32_t sw_dat_cnt: 12; /**< bit[2:13] */
			uint32_t sw_dat_dir: 1;  /**< bit[14] */
			uint32_t reserved:   17; /**< bit[15:31] */
		};
		uint32_t v;
	} sw_dat;

	/* REG_0x4 */
	uint32_t reserved0;

	/* REG_0x5 */
	union {
		struct {
			uint32_t addr_vid_ini: 24; /**< bit[0:23] */
			uint32_t reserved:     8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_vid_ini;

	/* REG_0x6 */
	union {
		struct {
			uint32_t addr_aud_ini: 24; /**< bit[0:23] */
			uint32_t reserved:     8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_aud_ini;

	/* REG_0x7 */
	union {
		struct {
			uint32_t addr_vid_bon: 24; /**< bit[0:23] */
			uint32_t reserved:     8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_vid_bon;

	/* REG_0x8 */
	union {
		struct {
			uint32_t addr_aud_bon: 24; /**< bit[0:23] */
			uint32_t reserved:     8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_aud_bon;

	/* REG_0x9 */
	union {
		struct {
			uint32_t sw_op_type:    4;  /**< bit[0:3] */
			uint32_t sw_start:      1;  /**< bit[4] */
			uint32_t addr_ini_set:  1;  /**< bit[5] */
			uint32_t int_sw_op_clr: 1;  /**< bit[6] */
			uint32_t int_mask:      21; /**< bit[7:27] */
			uint32_t bp_empty_aud:  1;  /**< bit[28] */
			uint32_t bp_full_aud:   1;  /**< bit[29] */
			uint32_t bp_empty_vid:  1;  /**< bit[30] */
			uint32_t bp_full_vid:   1;  /**< bit[31] */
		};
		uint32_t v;
	} sw_op;

	/* REG_0xA */
	uint32_t vid_trxfifo_data;
	/* REG_0xB */
	uint32_t aud_trxfifo_data;
	/* REG_0xC */
	uint32_t ge0_trxfifo_data;
	/* REG_0xD */
	uint32_t ge1_trxfifo_data;
	/* REG_0xE */
	uint32_t fls_trxfifo_data;
	/* REG_0xF */
	uint32_t reserved1;

	/* REG_0x10 */
	union {
		struct {
			uint32_t vidwr_th: 11; /**< bit[0:10] */
			uint32_t vidrd_th: 11; /**< bit[11:21] */
			uint32_t reserved: 10; /**< bit[22:31] */
		};
		uint32_t v;
	} vid_th;

	/* REG_0x11 */
	union {
		struct {
			uint32_t audwr_th: 11; /**< bit[0:10] */
			uint32_t audrd_th: 11; /**< bit[11:21] */
			uint32_t reserved: 10; /**< bit[22:31] */
		};
		uint32_t v;
	} aud_th;

	/* REG_0x12 */
	/* address_mapping is reserved, but used for ge0_th in old spi driver */
	union {
		struct {
			uint32_t ge0wr_th: 11; /**< bit[0:10] */
			uint32_t ge0rd_th: 11; /**< bit[11:21] */
			uint32_t reserved: 10; /**< bit[22:31] */
		};
		uint32_t v;
	} ge0_th;

	/* REG_0x13 */
	/* address_mapping is reserved, but used for ge1_th in old spi driver */
	union {
		struct {
			uint32_t ge1wr_th: 11; /**< bit[0:10] */
			uint32_t ge1rd_th: 11; /**< bit[11:21] */
			uint32_t reserved: 10; /**< bit[22:31] */
		};
		uint32_t v;
	} ge1_th;

	/* REG_0x14 */
	uint32_t reserved2;

	/* REG_0x15 */
	union {
		struct {
			uint32_t vidwr_dep: 11; /**< bit[0:10] */
			uint32_t vidrd_dep: 11; /**< bit[11:21] */
			uint32_t reserved:  10; /**< bit[22:31] */
		};
		uint32_t v;
	} vid_dep;

	/* REG_0x16 */
	union {
		struct {
			uint32_t audwr_dep: 11; /**< bit[0:10] */
			uint32_t audrd_dep: 11; /**< bit[11:21] */
			uint32_t reserved:  10; /**< bit[22:31] */
		};
		uint32_t v;
	} aud_dep;

	/* REG_0x17 */
	union {
		struct {
			uint32_t ge0wr_dep: 11; /**< bit[0:10] */
			uint32_t ge0rd_dep: 11; /**< bit[11:21] */
			uint32_t reserved:  10; /**< bit[22:31] */
		};
		uint32_t v;
	} ge0_dep;

	/* REG_0x18 */
	union {
		struct {
			uint32_t ge1wr_dep: 11; /**< bit[0:10] */
			uint32_t ge1rd_dep: 11; /**< bit[11:21] */
			uint32_t reserved:  10; /**< bit[22:31] */
		};
		uint32_t v;
	} ge1_dep;

	/* REG_0x19 */
	union {
		struct {
			uint32_t flswr_dep: 11; /**< bit[0:10] */
			uint32_t flsrd_dep: 11; /**< bit[11:21] */
			uint32_t reserved:  10; /**< bit[22:31] */
		};
		uint32_t v;
	} fls_dep;

	/* REG_0x1A */
	union {
		struct {
			uint32_t fls_rx_enable: 1;  /**< bit[0] */
			uint32_t fls_tx_enable: 1;  /**< bit[1] */
			uint32_t ge1_rx_enable: 1;  /**< bit[2] */
			uint32_t ge1_tx_enable: 1;  /**< bit[3] */
			uint32_t ge0_rx_enable: 1;  /**< bit[4] */
			uint32_t ge0_tx_enable: 1;  /**< bit[5] */
			uint32_t aud_rx_enable: 1;  /**< bit[6] */
			uint32_t aud_tx_enable: 1;  /**< bit[7] */
			uint32_t vid_rx_enable: 1;  /**< bit[8] */
			uint32_t vid_tx_enable: 1;  /**< bit[9] */
			uint32_t reserved:      22; /**< bit[10:31] */
		};
		uint32_t v;
	} enable;

	/* REG_0x1B */
	union {
		struct {
			uint32_t en_dly_cnt: 16; /**< bit[0:15] */
			uint32_t reserved:   16; /**< bit[16:31] */
		};
		uint32_t v;
	} en_dly_cnt;

	/* REG_0x1C */
	union {
		struct {
			uint32_t m_value:      8;  /**< bit[0:7] */
			uint32_t clk_div:      3;  /**< bit[8:10] */
			uint32_t flsrd_sel:    1;  /**< bit[11] */
			uint32_t sclk_fls_com: 1;  /**< bit[12] */
			uint32_t reserved:     19; /**< bit[13:31] */
		};
		uint32_t v;
	} ctrl;

	/* REG_0x1D */
	union {
		struct {
			uint32_t vidwr_cnt: 9;  /**< bit[0:8] */
			uint32_t vidrd_cnt: 9;  /**< bit[9:17] */
			uint32_t reserved:  14; /**< bit[18:31] */
		};
		uint32_t v;
	} vid_cnt;

	/* REG_0x1E */
	union {
		struct {
			uint32_t audwr_cnt: 9;  /**< bit[0:8] */
			uint32_t audrd_cnt: 9;  /**< bit[9:17] */
			uint32_t reserved:  14; /**< bit[18:31] */
		};
		uint32_t v;
	} aud_cnt;

	/* REG_0x1F */
	uint32_t reserved3;

	/* REG_0x20 */
	union {
		struct {
			uint32_t vid_psram_full_th: 24; /**< bit[0:23] */
			uint32_t reserved:          8;  /**< bit[24:31] */
		};
		uint32_t v;
	} vid_psram_full_th;

	/* REG_0x21 */
	union {
		struct {
			uint32_t vid_psram_empty_th: 24; /**< bit[0:23] */
			uint32_t reserved:           8;  /**< bit[24:31] */
		};
		uint32_t v;
	} vid_psram_empty_th;

	/* REG_0x22 */
	union {
		struct {
			uint32_t aud_psram_full_th: 24; /**< bit[0:23] */
			uint32_t reserved:          8;  /**< bit[24:31] */
		};
		uint32_t v;
	} aud_psram_full_th;

	/* REG_0x23 */
	union {
		struct {
			uint32_t aud_psram_empty_th: 24; /**< bit[0:23] */
			uint32_t reserved:           8;  /**< bit[24:31] */
		};
		uint32_t v;
	} aud_psram_empty_th;

	/* REG_0x24 */
	union {
		struct {
			uint32_t dcard_cmd_en:  1;  /**< bit[0] */
			uint32_t dcard_cmd_bw:  1;  /**< bit[1] */
			uint32_t dcard_cmd:     8;  /**< bit[2:9] */
			uint32_t dcard_cmd_cnt: 8;  /**< bit[10:17] */
			uint32_t reserved:      14; /**< bit[18:31] */
		};
		uint32_t v;
	} dcard_cmd;

	/* REG_0x25 */
	union {
		struct {
			uint32_t dcard_addr_en:  1;  /**< bit[0] */
			uint32_t dcard_addr_bw:  1;  /**< bit[1] */
			uint32_t reserved0:      24; /**< bit[2:25] */
			uint32_t dcard_addr_cnt: 5;  /**< bit[26:30] */
			uint32_t reserved1:      1;  /**< bit[31] */
		};
		uint32_t v;
	} dcard_addr;

	/* REG_0x26 */
	union {
		struct {
			uint32_t dcard_dum_en:  1;  /**< bit[0] */
			uint32_t dcard_dum_bw:  1;  /**< bit[1] */
			uint32_t dcard_dum_cnt: 4;  /**< bit[2:5] */
			uint32_t reserved:      26; /**< bit[6:31] */
		};
		uint32_t v;
	} dcard_dum;

	/* REG_0x27 */
	union {
		struct {
			uint32_t dcard_dat_en:  1;  /**< bit[0] */
			uint32_t dcard_dat_bw:  1;  /**< bit[1] */
			uint32_t dcard_dat_cnt: 12; /**< bit[2:13] */
			uint32_t dcard_dat_dir: 1;  /**< bit[14] */
			uint32_t reserved:      17; /**< bit[15:31] */
		};
		uint32_t v;
	} dcard_dat;

	/* REG_0x28 */
	union {
		struct {
			uint32_t dcawr_cmd_en:  1;  /**< bit[0] */
			uint32_t dcawr_cmd_bw:  1;  /**< bit[1] */
			uint32_t dcawr_cmd:     8;  /**< bit[2:9] */
			uint32_t dcawr_cmd_cnt: 8;  /**< bit[10:17] */
			uint32_t reserved:      14; /**< bit[18:31] */
		};
		uint32_t v;
	} dcawr_cmd;

	/* REG_0x29 */
	union {
		struct {
			uint32_t dcawr_addr_en:  1;  /**< bit[0] */
			uint32_t dcawr_addr_bw:  1;  /**< bit[1] */
			uint32_t reserved0:      24; /**< bit[2:25] */
			uint32_t dcawr_addr_cnt: 5;  /**< bit[26:30] */
			uint32_t reserved1:      1;  /**< bit[31] */
		};
		uint32_t v;
	} dcawr_addr;

	/* REG_0x2A */
	union {
		struct {
			uint32_t dcawr_dum_en:  1;  /**< bit[0] */
			uint32_t dcawr_dum_bw:  1;  /**< bit[1] */
			uint32_t dcawr_dum_cnt: 4;  /**< bit[2:5] */
			uint32_t reserved:      26; /**< bit[6:31] */
		};
		uint32_t v;
	} dcawr_dum;

	/* REG_0x2B */
	union {
		struct {
			uint32_t dcawr_dat_en:  1;  /**< bit[0] */
			uint32_t dcawr_dat_bw:  1;  /**< bit[1] */
			uint32_t dcawr_dat_cnt: 12; /**< bit[2:13] */
			uint32_t dcawr_dat_dir: 1;  /**< bit[14] */
			uint32_t reserved:      17; /**< bit[15:31] */
		};
		uint32_t v;
	} dcawr_dat;

	/* REG_0x2C */
	union {
		struct {
			uint32_t rw_req_msk: 1;  /**< bit[0] */
			uint32_t reserved:   31; /**< bit[1:31] */
		};
		uint32_t v;
	} rw_req_msk;

	/* REG_0x2D ~ REG_0x34 */
	uint32_t reserved4[8];

	/* REG_0x35 */
	uint32_t fifo_status;

	/* REG_0x36 */
	union {
		struct {
			uint32_t int_status:     16; /**< bit[0:15] */
			uint32_t int_sw_op:      1;  /**< bit[16] */
			uint32_t near_empty_aud: 1;  /**< bit[17] */
			uint32_t near_full_aud:  1;  /**< bit[18] */
			uint32_t near_empty_vid: 1;  /**< bit[19] */
			uint32_t near_full_vid:  1;  /**< bit[20] */
			uint32_t reserved:       11; /**< bit[21:31] */
		};
		uint32_t v;
	} int_status;

	/* REG_0x37 */
	union {
		struct {
			uint32_t addr_vid_rd_reg: 24; /**< bit[0:23] */
			uint32_t reserved:        8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_vid_rd_reg;

	/* REG_0x38 */
	union {
		struct {
			uint32_t addr_vid_wr_reg: 24; /**< bit[0:23] */
			uint32_t reserved:        8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_vid_wr_reg;

	/* REG_0x39 */
	union {
		struct {
			uint32_t addr_aud_rd_reg: 24; /**< bit[0:23] */
			uint32_t reserved:        8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_aud_rd_reg;

	/* REG_0x3A */
	union {
		struct {
			uint32_t addr_aud_wr_reg: 24; /**< bit[0:23] */
			uint32_t reserved:        8;  /**< bit[24:31] */
		};
		uint32_t v;
	} addr_aud_wr_reg;
} qspi_hw_t;

#ifdef __cplusplus
}
#endif
