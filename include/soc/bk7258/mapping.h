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

#include "reg_base.h"

#define JPEG_SRAM_ADDRESS (SOC_SRAM0_DATA_BASE)


#define LCD_BLEND_FRAME_COUNT     (2)
#define LCD_BLEND_MALLOC_SIZE      (100 * 44 * 2)

#define LCD_BLEND_JPEGSRAM_SIZE    (1024 * 24)
#define LCD_BLEND_JPEGSRAM_ADDR_1  (SOC_SRAM0_DATA_BASE + (1024 * 80))
#define LCD_BLEND_JPEGSRAM_ADDR_2  (LCD_BLEND_JPEGSRAM_ADDR_1 + LCD_BLEND_JPEGSRAM_SIZE)

