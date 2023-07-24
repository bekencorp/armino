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

#include <soc/soc.h>

#define REG_JPEG_DEC_BASE_ADDR              SOC_JPEG_DEC_REG_BASE

#define REG_JPEG_BITS_DC00               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x10 * 4)))
#define REG_JPEG_BITS_DC01               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x11 * 4)))
#define REG_JPEG_BITS_DC02               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x12 * 4)))
#define REG_JPEG_BITS_DC03               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x13 * 4)))
#define REG_JPEG_BITS_DC04               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x14 * 4)))
#define REG_JPEG_BITS_DC05               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x15 * 4)))
#define REG_JPEG_BITS_DC06               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x16 * 4)))
#define REG_JPEG_BITS_DC07               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x17 * 4)))
#define REG_JPEG_BITS_DC08               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x18 * 4)))
#define REG_JPEG_BITS_DC09               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x19 * 4)))
#define REG_JPEG_BITS_DC0A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1a * 4)))
#define REG_JPEG_BITS_DC0B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1b * 4)))
#define REG_JPEG_BITS_DC0C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1c * 4)))
#define REG_JPEG_BITS_DC0D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1d * 4)))
#define REG_JPEG_BITS_DC0E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1e * 4)))
#define REG_JPEG_BITS_DC0F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x1f * 4)))

#define REG_JPEG_BITS_DC10               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x20 * 4)))
#define REG_JPEG_BITS_DC11               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x21 * 4)))
#define REG_JPEG_BITS_DC12               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x22 * 4)))
#define REG_JPEG_BITS_DC13               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x23 * 4)))
#define REG_JPEG_BITS_DC14               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x24 * 4)))
#define REG_JPEG_BITS_DC15               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x25 * 4)))
#define REG_JPEG_BITS_DC16               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x26 * 4)))
#define REG_JPEG_BITS_DC17               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x27 * 4)))
#define REG_JPEG_BITS_DC18               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x28 * 4)))
#define REG_JPEG_BITS_DC19               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x29 * 4)))
#define REG_JPEG_BITS_DC1A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2a * 4)))
#define REG_JPEG_BITS_DC1B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2b * 4)))
#define REG_JPEG_BITS_DC1C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2c * 4)))
#define REG_JPEG_BITS_DC1D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2d * 4)))
#define REG_JPEG_BITS_DC1E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2e * 4)))
#define REG_JPEG_BITS_DC1F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x2f * 4)))


#define REG_JPEG_BITS_AC00               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x30 * 4)))
#define REG_JPEG_BITS_AC01               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x31 * 4)))
#define REG_JPEG_BITS_AC02               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x32 * 4)))
#define REG_JPEG_BITS_AC03               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x33 * 4)))
#define REG_JPEG_BITS_AC04               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x34 * 4)))
#define REG_JPEG_BITS_AC05               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x35 * 4)))
#define REG_JPEG_BITS_AC06               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x36 * 4)))
#define REG_JPEG_BITS_AC07               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x37 * 4)))
#define REG_JPEG_BITS_AC08               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x38 * 4)))
#define REG_JPEG_BITS_AC09               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x39 * 4)))
#define REG_JPEG_BITS_AC0A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3a * 4)))
#define REG_JPEG_BITS_AC0B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3b * 4)))
#define REG_JPEG_BITS_AC0C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3c * 4)))
#define REG_JPEG_BITS_AC0D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3d * 4)))
#define REG_JPEG_BITS_AC0E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3e * 4)))
#define REG_JPEG_BITS_AC0F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3f * 4)))

#define REG_JPEG_BITS_AC10               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x40 * 4)))
#define REG_JPEG_BITS_AC11               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x41 * 4)))
#define REG_JPEG_BITS_AC12               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x42 * 4)))
#define REG_JPEG_BITS_AC13               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x43 * 4)))
#define REG_JPEG_BITS_AC14               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x44 * 4)))
#define REG_JPEG_BITS_AC15               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x45 * 4)))
#define REG_JPEG_BITS_AC16               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x46 * 4)))
#define REG_JPEG_BITS_AC17               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x47 * 4)))
#define REG_JPEG_BITS_AC18               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x48 * 4)))
#define REG_JPEG_BITS_AC19               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x49 * 4)))
#define REG_JPEG_BITS_AC1A               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4a * 4)))
#define REG_JPEG_BITS_AC1B               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4b * 4)))
#define REG_JPEG_BITS_AC1C               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4c * 4)))
#define REG_JPEG_BITS_AC1D               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4d * 4)))
#define REG_JPEG_BITS_AC1E               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4e * 4)))
#define REG_JPEG_BITS_AC1F               (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x4f * 4)))

#define BASE_FFDA                        (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x5c * 4)))

#define REG_JPEG_HUF_TABLE00             (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x80 * 4)))
#define REG_JPEG_HUF_TABLE10             (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0xc0 * 4)))
#define REG_JPEG_HUF_TABLE01             (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x100 * 4)))
#define REG_JPEG_HUF_TABLE11             (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x200 * 4)))
#define REG_JPEG_Zig                     (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x300 * 4)))
#define REG_JPEG_TMP0                    (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x340 * 4)))
#define REG_JPEG_DQT_TABLE0              (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x380 * 4)))
#define REG_JPEG_DQT_TABLE1              (*((volatile unsigned long *)(REG_JPEG_DEC_BASE_ADDR + 0x3c0 * 4)))

#ifdef __cplusplus
}
#endif

