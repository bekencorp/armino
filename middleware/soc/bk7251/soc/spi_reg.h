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

#define SPI_R_BASE(_id)          (SOC_SPI_REG_BASE)

#define SPI_R_CTRL(_id)          (SPI_R_BASE(_id) + 4 * 0x00)

#define SPI_F_CLK_DIV            (BIT(8))
#define SPI_F_CLK_DIV_M          (0xFF)
#define SPI_F_CLK_DIV_V          (0xFF)
#define SPI_F_CLK_DIV_S          (8)

#define SPI_R_STATUS(_id)        (SPI_R_BASE(_id) + 4 * 0x01)

#define SPI_R_DATA(_id)          (SPI_R_BASE(_id) + 4 * 0x02)

#define SPI_R_SLAVE_CTRL(_id)    (SPI_R_BASE(_id) + 4 * 0x03)

#ifdef __cplusplus
}
#endif

