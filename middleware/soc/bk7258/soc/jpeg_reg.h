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

#define JPEG_R_BASE          (SOC_JPEG_REG_BASE)

#define JPEG_R_INT_EN        (JPEG_R_BASE + 4 * 0x0C)

#define JPEG_F_X_PIXEL       (BIT(8))
#define JPEG_F_X_PIXEL_M     0xFF
#define JPEG_F_X_PIXEL_V     0xFF
#define JPEG_F_X_PIXEL_S     8

#define JPEG_F_Y_PIXEL       (BIT(24))
#define JPEG_F_Y_PIXEL_M     0xFF
#define JPEG_F_Y_PIXEL_V     0xFF
#define JPEG_F_Y_PIXEL_S     24

#define JPEG_R_RX_FIFO       (JPEG_R_BASE + 4 * 0x5)

#define JPEG_R_INT_STATUS    (JPEG_R_BASE + 4 * 0x6)

#define JPEG_R_QUANT_TABLE   (JPEG_R_BASE + 4 * 0x20)

#ifdef __cplusplus
}
#endif

