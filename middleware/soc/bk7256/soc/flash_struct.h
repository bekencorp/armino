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
			uint32_t addr_sw_reg: 24; /**< bit[0:23] */
			uint32_t op_type_sw:  5;  /**< bit[24:28] */
			uint32_t op_sw:       1;  /**< bit[29] */
			uint32_t wp_value:    1;  /**< bit[30] */
			uint32_t busy_sw:     1;  /**< bit[31] */
		};
		uint32_t v;
	} op_sw;

	/* REG_0x1 */
	uint32_t data_sw_flash; /**< data to be written from software to flash */

	/* REG_0x2 */
	uint32_t data_flash_sw; /**< data read from flash to software */

	/* REG_0x3 */
	uint32_t reserved0;

	/* REG_0x4 */
	uint32_t rd_flash_id; /**< read flash id data */

	/* REG_0x5 */
	union {
		struct {
			uint32_t status_reg:       8;  /**< bit[0:7] */
			uint32_t crc_err_num: 8;  /**< bit[8:15] */
			uint32_t byte_sel_wr: 3;  /**< bit[16:18] */
			uint32_t byte_sel_rd: 3;  /**< bit[19:21] */
			uint32_t reserved:    10; /**< bit[22:31] */
		};
		uint32_t v;
	} state;

	/* REG_0x6 */
	uint32_t reserved1;

	/* REG_0x7 */
	union {
		struct {
			uint32_t clk_cfg:        4;  /**< bit[0:3] */
			uint32_t mode_sel:       5;  /**< bit[4:8] */
			uint32_t cpu_data_wr_en: 1;  /**< bit[9] */
			uint32_t wrsr_data:      16; /**< bit[10:25] */
			uint32_t crc_en:         1;  /**< bit[26] */
			uint32_t reserved:       5;  /**< bit[27:31] */
		};
		uint32_t v;
	} config;
} flash_hw_t;

#ifdef __cplusplus
}
#endif

