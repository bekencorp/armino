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

#define I2C_R_BASE(_id)          (SOC_I2C0_REG_BASE + _id * 0x40)

#define I2C_R_CONFIG(_id)        (I2C_R_BASE(_id) + 4 * 0x0)

#define I2C_F_FREQ_DIV           (BIT(6))
#define I2C_F_FREQ_DIV_M         0x3FF
#define I2C_F_FREQ_DIV_V         0x3FF
#define I2C_F_FREQ_DIV_S         6

#define I2C_F_SLAVE_ADDR         (BIT(16))
#define I2C_F_SLAVE_ADDR_M       0x3FF
#define I2C_F_SLAVE_ADDR_V       0x3FF
#define I2C_F_SLAVE_ADDR_S       16

#define I2C1_R_INT_STAUS(_id)    (I2C_R_BASE(_id) + 4 * 0x1)

#define I2C1_F_SM_INT            (BIT(0))
#define I2C1_F_SM_INT_M          0x1
#define I2C1_F_SM_INT_V          0x1
#define I2C1_F_SM_INT_S          0

#define I2C1_F_INT_MODE          (BIT(6))
#define I2C1_F_INT_MODE_M        0x3
#define I2C1_F_INT_MODE_V        (0x3 << 6)
#define I2C1_F_INT_MODE_S        6

#define I2C1_F_ACK               (BIT(8))
#define I2C1_F_ACK_M             0x1
#define I2C1_F_ACK_V             0x1
#define I2C1_F_ACK_S             8

#define I2C1_F_STOP              (BIT(9))
#define I2C1_F_STOP_M            0x1
#define I2C1_F_STOP_V            0x1
#define I2C1_F_STOP_S            9

#define I2C1_F_START             (BIT(10))
#define I2C1_F_START_M           0x1
#define I2C1_F_START_V           0x1
#define I2C1_F_START_S           10

#define I2C0_R_DATA              (I2C_R_BASE(0) + 4 * 0x1)
#define I2C1_R_DATA              (I2C_R_BASE(1) + 4 * 0x2)

#define I2C_F_DATA               (BIT(0))
#define I2C_F_DATA_M             0xFF
#define I2C_F_DATA_V             0xFF
#define I2C_F_DATA_S             0

#ifdef __cplusplus
}
#endif

