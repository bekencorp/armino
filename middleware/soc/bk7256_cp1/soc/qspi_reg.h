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

#define QSPI_R_BASE(_id)                  (SOC_QSPI_REG_BASE)

#define QSPI_DCACHE_BASE_ADDR             (SOC_QSPI_DATA_REG_BASE)

#define QSPI_R_CMD_A_L(_id)               (QSPI_R_BASE(_id) + 4 * 0x0)
#define QSPI_R_CMD_A_H(_id)               (QSPI_R_BASE(_id) + 4 * 0x1)
#define QSPI_R_CMD_A_CFG1(_id)            (QSPI_R_BASE(_id) + 4 * 0x2)
#define QSPI_R_CMD_A_CFG2(_id)            (QSPI_R_BASE(_id) + 4 * 0x3)

#define QSPI_R_CMD_B_L(_id)               (QSPI_R_BASE(_id) + 4 * 0x4)
#define QSPI_R_CMD_B_H(_id)               (QSPI_R_BASE(_id) + 4 * 0x5)
#define QSPI_R_CMD_B_CFG1(_id)            (QSPI_R_BASE(_id) + 4 * 0x6)
#define QSPI_R_CMD_B_CFG2(_id)            (QSPI_R_BASE(_id) + 4 * 0x7)

#define QSPI_R_CMD_C_L(_id)               (QSPI_R_BASE(_id) + 4 * 0x8)
#define QSPI_R_CMD_C_H(_id)               (QSPI_R_BASE(_id) + 4 * 0x9)
#define QSPI_R_CMD_C_CFG1(_id)            (QSPI_R_BASE(_id) + 4 * 0xa)
#define QSPI_R_CMD_C_CFG2(_id)            (QSPI_R_BASE(_id) + 4 * 0xb)

#define QSPI_R_CMD_D_L(_id)               (QSPI_R_BASE(_id) + 4 * 0xc)
#define QSPI_R_CMD_D_H(_id)               (QSPI_R_BASE(_id) + 4 * 0xd)
#define QSPI_R_CMD_D_CFG1(_id)            (QSPI_R_BASE(_id) + 4 * 0xe)
#define QSPI_R_CMD_D_CFG2(_id)            (QSPI_R_BASE(_id) + 4 * 0xf)

#define QSPI_F_CMD1                       (BIT(0))
#define QSPI_F_CMD1_M                     (0xFF)
#define QSPI_F_CMD1_S                     (0)

#define QSPI_F_CMD2                       (BIT(8))
#define QSPI_F_CMD2_M                     (0xFF)
#define QSPI_F_CMD2_S                     (8)

#define QSPI_F_CMD3                       (BIT(16))
#define QSPI_F_CMD3_M                     (0xFF)
#define QSPI_F_CMD3_S                     (16)

#define QSPI_F_CMD4                       (BIT(24))
#define QSPI_F_CMD4_M                     (0xFF)
#define QSPI_F_CMD4_S                     (24)

#define QSPI_F_DUMMY_CLK                  (BIT(16))
#define QSPI_F_DUMMY_CLK_M                (0x7F)
#define QSPI_F_DUMMY_CLK_V                (0x7F)
#define QSPI_F_DUMMY_CLK_S                (16)

#define QSPI_R_CONFIG(_id)                (QSPI_R_BASE(_id) + 4 * 0x1c)

#define QSPI_F_CLK_DIV                    (BIT(8))
#define QSPI_F_CLK_DIV_M                  (0xFF)
#define QSPI_F_CLK_DIV_V                  (0xFF)
#define QSPI_F_CLK_DIV_S                  (8)

#define QSPI_R_RST_FIFO_CFG(_id)          (QSPI_R_BASE(_id) + 4 * 0x1f)

#define QSPI_R_INT_EN(_id)                (QSPI_R_BASE(_id) + 4 * 0x20)

#define QSPI_R_STATUS_CLR(_id)            (QSPI_R_BASE(_id) + 4 * 0x21)

#define QSPI_R_STATUS(_id)                (QSPI_R_BASE(_id) + 4 * 0x22)

#define QSPI_R_LED_WR_CONT_CMD(_id)       (QSPI_R_BASE(_id) + 4 * 0x23)

#define QSPI_R_DATA(_id)                  (QSPI_R_BASE(_id) + 4 * 0x40)

#ifdef __cplusplus
}
#endif

