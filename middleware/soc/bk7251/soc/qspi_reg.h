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

#define QSPI_R_BASE          (SOC_QSPI_REG_BASE)

#define QSPI_R_SW_CMD        (QSPI_R_BASE + 4 * 0x0)

#define QSPI_F_SW_CMD        (BIT(2))
#define QSPI_F_SW_CMD_M      0xFF
#define QSPI_F_SW_CMD_V      0xFF
#define QSPI_F_SW_CMD_S      0x2

#define QSPI_R_SW_ADDR       (QSPI_R_BASE + 4 * 0x1)

#define QSPI_F_SW_ADDR       (BIT(2))
#define QSPI_F_SW_ADDR_M     0xFFFFFF
#define QSPI_F_SW_ADDR_V     0xFFFFFF
#define QSPI_F_SW_ADDR_S     0x2

#define QSPI_R_SW_DUMMY      (QSPI_R_BASE + 4 * 0x1)

#define QSPI_F_SW_DUMMY      (BIT(2))
#define QSPI_F_SW_DUMMY_M    0xF
#define QSPI_F_SW_DUMMY_V    0xF
#define QSPI_F_SW_DUMMY_S    0x2

#define QSPI_R_CTRL          (QSPI_R_BASE + 4 * 0x1C)

#define QSPI_F_CLK_DIV       (BIT(8))
#define QSPI_F_CLK_DIV_M     0x7
#define QSPI_F_CLK_DIV_V     0x7
#define QSPI_F_CLK_DIV_S     0x8

#define QSPI_R_INT_STATUS    (QSPI_R_BASE + 4 * 0x36)

#define QSPI_F_INT_STATUS    (BIT(0))
#define QSPI_F_INT_STATUS_M  0xFFFF
#define QSPI_F_INT_STATUS_V  0xFFFF
#define QSPI_F_INT_STATUS_S  0

#define QSPI_F_INT_SW_OP     (BIT(16))
#define QSPI_F_INT_SW_OP_M   0x1
#define QSPI_F_INT_SW_OP_V   0x1
#define QSPI_F_INT_SW_OP_S   16

#define QSPI_DCACHE_BASE_ADDR    (0x03000000)

typedef enum {
	QSPI_GE0_RD = 5,
	QSPI_GE0_WR,
	QSPI_GE1_RD,
	QSPI_GE1_WR,
	QSPI_FLS_RD,
	QSPI_FLS_WR,
} qspi_sw_op_type_t;

typedef enum {
	QSPI_VOL_1_8V = 0,
	QSPI_VOL_2_5V,
	QSPI_VOL_3_3V,
} qspi_voltage_level_t;

#ifdef __cplusplus
}
#endif

