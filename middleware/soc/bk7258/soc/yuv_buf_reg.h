// Copyright 2022-2023 Beken
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

#define YUV_BUF_R_BASE                 (SOC_YUV_BUF_REG_BASE)

#define YUV_BUF_R_CTRL                 (SOC_YUV_BUF_REG_BASE + 0x4 * 4)

#define YUV_BUF_F_MCLK_DIV             (BIT(10))
#define YUV_BUF_F_MCLK_DIV_M           0x3
#define YUV_BUF_F_MCLK_DIV_V           0x3
#define YUV_BUF_F_MCLK_DIV_S           10

#define YUV_BUF_R_INT_STATUS           (SOC_YUV_BUF_REG_BASE + 0xa * 4)

#ifdef __cplusplus
}
#endif

