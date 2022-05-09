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

#include <common/sys_config.h>

//This file tracks the defs directly from WiFi Driver, please keep them exactly same as
//defs in WiFi Driver.
//TODO - write a script to check the consistence!

#if CONFIG_WIFI6

// field definitions
/// NFRAME field mask
#define MACBYP_NFRAME_MASK   ((uint32_t)0x0000FFFF)
/// NFRAME field LSB position
#define MACBYP_NFRAME_LSB    0
/// NFRAME field width
#define MACBYP_NFRAME_WIDTH  ((uint32_t)0x00000010)

/// NFRAME field reset value
#define MACBYP_NFRAME_RST    0x0

/// MODE field mask
#define MACBYP_MODE_MASK            ((uint32_t)0x00000300)
/// MODE field LSB position
#define MACBYP_MODE_LSB             8

// field definitions
/// INTERFRAME_DELAY field mask
#define MACBYP_INTERFRAME_DELAY_MASK   ((uint32_t)0x000FFFFF)
/// INTERFRAME_DELAY field LSB position
#define MACBYP_INTERFRAME_DELAY_LSB    0
/// INTERFRAME_DELAY field width
#define MACBYP_INTERFRAME_DELAY_WIDTH  ((uint32_t)0x00000014)

/// INTERFRAME_DELAY field reset value
#define MACBYP_INTERFRAME_DELAY_RST    0x0

// field definitions
/// PAYLOAD_TX field mask
#define MACBYP_PAYLOAD_TX_MASK   ((uint32_t)0x000F0000)
/// PAYLOAD_TX field LSB position
#define MACBYP_PAYLOAD_TX_LSB    16

#define MACBYP_BYPASS_BIT           ((uint32_t)0x00000001)

#define MACBYP_TXV_COUNT  16

#else //CONFIG_WIFI6

#endif //CONFIG_WIFI6
