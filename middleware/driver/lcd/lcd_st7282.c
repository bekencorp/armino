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

#include <driver/gpio.h>
#include <driver/media_types.h>
#include <driver/lcd_types.h>

#include "lcd_devices.h"

static const lcd_rgb_t lcd_rgb =
{
	.clk = LCD_8M,
	.data_out_clk_edge = POSEDGE_OUTPUT,

	.hsync_back_porch = 40,
	.hsync_front_porch = 5,
	.vsync_back_porch = 8,
	.vsync_front_porch = 8,
};

const lcd_device_t lcd_device_st7282 =
{
	.id = LCD_DEVICE_ST7282,
	.name = "st7282",
	.type = LCD_TYPE_RGB565,
	.ppi = PPI_480X272,
	.rgb = &lcd_rgb,
	.init = NULL,
};

