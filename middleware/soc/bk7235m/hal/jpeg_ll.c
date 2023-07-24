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

#include "jpeg_ll.h"

#define JPEG_QUANT_TABLE_LEN    32

static const uint32_t jpeg_quant_table[JPEG_QUANT_TABLE_LEN] = {
	0x07060608, 0x07080506, 0x09090707, 0x140c0a08,
	0x0b0b0c0d, 0x1312190c, 0x1a1d140f, 0x1a1d1e1f,
	0x24201c1c, 0x2220272e, 0x1c1c232c, 0x2c293728,
	0x34343130, 0x39271f34, 0x3c32383d, 0x3234332e,
	0x0c090909, 0x0d180c0b, 0x2132180d, 0x3232211c,
	0x32323232, 0x32323232, 0x32323232, 0x32323232,
	0x32323232, 0x32323232, 0x32323232, 0x32323232,
	0x32323232, 0x32323232, 0x32323232, 0x32323232
};

void jpeg_ll_init_quant_table(jpeg_hw_t *hw)
{
	uint32_t reg_addr;

	for (uint32_t i = 0; i < JPEG_QUANT_TABLE_LEN; i++) {
		reg_addr = JPEG_R_QUANT_TABLE + i * 4;
		REG_WRITE(reg_addr, (uint32_t)jpeg_quant_table[i]);
	}
}

