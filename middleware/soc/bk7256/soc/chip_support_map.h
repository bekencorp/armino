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
#include <stdio.h>
#include <soc/soc.h>

#if CONFIG_CHIP_SUPPORT

#define OTP_REGISTER_VDD  (*((volatile unsigned long *) (0x4b008000 + 0x3*4)))
#define OTP_SOFTWARE_CHIP_ID  (*((volatile unsigned long *) (0x4b004000 + 0x80)))

#define MASK_ALL_BITS    0xFFFFFFFF
#define MASK_HIGH_16BITS 0xFFFF0000
#define MASK_LOW_16BITS  0xFFFF

#define DEVICE_ID_BK7256                0x53434647
#define CHIP_ID_BK7256                  0x72560001
#define SOFTWARE_CHIP_ID                0x72560000
#define SUPPORTED_CHIPS \
{\
	{CHIP_ID_BK7256, MASK_ALL_BITS, DEVICE_ID_BK7256, MASK_ALL_BITS, SOFTWARE_CHIP_ID, MASK_ALL_BITS},\
}

#endif //CONFIG_CHIP_SUPPORT

