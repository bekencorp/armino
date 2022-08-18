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

#define SDIO_HOST_R_BASE                    (SOC_SDIO_HOST_REG_BASE)

#define SDIO_HOST_R_DATA_CTRL               (SDIO_HOST_R_BASE + 4 * 0x3)

#define SDIO_HOST_F_DATA_BLOCK_SIZE         (BIT(4))
#define SDIO_HOST_F_DATA_BLOCK_SIZE_M       (0xfff)
#define SDIO_HOST_F_DATA_BLOCK_SIZE_V       (0xfff)
#define SDIO_HOST_F_DATA_BLOCK_SIZE_S       (4)

#define SDIO_HOST_R_CMD_RSP_INT_SEL         (SDIO_HOST_R_BASE + 4 * 0x9)

#define SDIO_HOST_F_CMD_NO_RSP_END_INT      (BIT(0))
#define SDIO_HOST_F_CMD_NO_RSP_END_INT_M    (0x1)
#define SDIO_HOST_F_CMD_NO_RSP_END_INT_V    (0x1)
#define SDIO_HOST_F_CMD_NO_RSP_END_INT_S    (0)

#define SDIO_HOST_F_CMD_RSP_END_INT         (BIT(1))
#define SDIO_HOST_F_CMD_RSP_END_INT_M       (0x1)
#define SDIO_HOST_F_CMD_RSP_END_INT_V       (0x1)
#define SDIO_HOST_F_CMD_RSP_END_INT_S       (1)

#define SDIO_HOST_F_CMD_RSP_TIMEOUT_INT     (BIT(2))
#define SDIO_HOST_F_CMD_RSP_TIMEOUT_INT_M   (0x1)
#define SDIO_HOST_F_CMD_RSP_TIMEOUT_INT_V   (0x1)
#define SDIO_HOST_F_CMD_RSP_TIMEOUT_INT_S   (2)

#define SDIO_HOST_F_DATA_RECV_END_INT       (BIT(3))
#define SDIO_HOST_F_DATA_RECV_END_INT_M     (0x1)
#define SDIO_HOST_F_DATA_RECV_END_INT_V     (0x1)
#define SDIO_HOST_F_DATA_RECV_END_INT_S     (3)

#define SDIO_HOST_F_DATA_WR_END_INT         (BIT(4))
#define SDIO_HOST_F_DATA_WR_END_INT_M       (0x1)
#define SDIO_HOST_F_DATA_WR_END_INT_V       (0x1)
#define SDIO_HOST_F_DATA_WR_END_INT_S       (4)

#define SDIO_HOST_F_DATA_TIMEOUT_INT        (BIT(5))
#define SDIO_HOST_F_DATA_TIMEOUT_INT_M      (0x1)
#define SDIO_HOST_F_DATA_TIMEOUT_INT_V      (0x1)
#define SDIO_HOST_F_DATA_TIMEOUT_INT_S      (5)

#define SDIO_HOST_F_TX_FIFO_NEED_WRITE      (BIT(7))
#define SDIO_HOST_F_TX_FIFO_NEED_WRITE_M    (0x1)
#define SDIO_HOST_F_TX_FIFO_NEED_WRITE_V    (0x1)
#define SDIO_HOST_F_TX_FIFO_NEED_WRITE_S    (7)

#define SDIO_HOST_F_FIFO_EMPTY_INT          (BIT(9))
#define SDIO_HOST_F_FIFO_EMPTY_INT_M        (0x1)
#define SDIO_HOST_F_FIFO_EMPTY_INT_V        (0x1)
#define SDIO_HOST_F_FIFO_EMPTY_INT_S        (9)

#define SDIO_HOST_F_CMD_RSP_CRC_FAIL        (BIT(11))
#define SDIO_HOST_F_CMD_RSP_CRC_FAIL_M      (0x1)
#define SDIO_HOST_F_CMD_RSP_CRC_FAIL_V      (0x1)
#define SDIO_HOST_F_CMD_RSP_CRC_FAIL_S      (11)

#define SDIO_HOST_F_DATA_CRC_FAIL           (BIT(13))
#define SDIO_HOST_F_DATA_CRC_FAIL_M         (0x1)
#define SDIO_HOST_F_DATA_CRC_FAIL_V         (0x1)
#define SDIO_HOST_F_DATA_CRC_FAIL_S         (13)

#define SDIO_HOST_R_CMD_RSP_INT_MASK        (SDIO_HOST_R_BASE + 4 * 0xa)

#ifdef __cplusplus
}
#endif

