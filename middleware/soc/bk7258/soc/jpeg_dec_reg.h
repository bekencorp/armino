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

#define JPEG_DEC_R_BASE           (SOC_JPEG_DEC_REG_BASE)

#define JPEG_DEC_R_HUF_TABLE00    (JPEG_R_BASE + 4 * 0x80)
#define JPEG_DEC_R_HUF_TABLE10    (JPEG_R_BASE + 4 * 0xc0)
#define JPEG_DEC_R_HUF_TABLE01    (JPEG_R_BASE + 4 * 0x100)
#define JPEG_DEC_R_HUF_TABLE11    (JPEG_R_BASE + 4 * 0x200)
#define JPEG_DEC_R_ZIG            (JPEG_R_BASE + 4 * 0x300)
#define JPEG_DEC_R_TMP0           (JPEG_R_BASE + 4 * 0x340)
#define JPEG_DEC_R_DQT_TABLE0     (JPEG_R_BASE + 4 * 0x380)
#define JPEG_DEC_R_DQT_TABLE1     (JPEG_R_BASE + 4 * 0x3c0)

#ifdef __cplusplus
}
#endif


