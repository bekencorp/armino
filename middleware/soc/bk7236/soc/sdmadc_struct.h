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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef volatile union {
	struct {
		uint32_t deviceid                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x0_t;


typedef volatile union {
	struct {
		uint32_t versionid                : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x1_t;


typedef volatile union {
	struct {
		uint32_t soft_rst                 :  1; /**<bit[0 : 0] */
		uint32_t bypass_ckg               :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_31        : 30; /**<bit[2 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x2_t;


typedef volatile union {
	struct {
		uint32_t global_status            : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x3_t;


typedef volatile union {
	struct {
		uint32_t sample_enable            :  1; /**<bit[0 : 0] */
		uint32_t reserved_bit_1_31        : 31; /**<bit[1 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x4_t;


typedef volatile union {
	struct {
		uint32_t sample_mode              :  1; /**<bit[0 : 0] */
		uint32_t sample_numb              :  2; /**<bit[1 : 2] */
		uint32_t sample_chsel             :  4; /**<bit[3 : 6] */
		uint32_t cic2_bypass              :  1; /**<bit[7 : 7] */
		uint32_t comp_bypass              :  1; /**<bit[8 : 8] */
		uint32_t cic2_gains               :  6; /**<bit[9 : 14] */
		uint32_t intr_enable              :  5; /**<bit[15 : 19] */
		uint32_t reserved_bit_20_31       : 12; /**<bit[20 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x5_t;


typedef volatile union {
	struct {
		uint32_t cali_offset              : 16; /**<bit[0 : 15] */
		uint32_t cali_gain                : 13; /**<bit[16 : 28] */
		uint32_t reserved_bit_29_31       :  3; /**<bit[29 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x6_t;


typedef volatile union {
	struct {
		uint32_t sadc_status              :  5; /**<bit[0 : 4] */
		uint32_t reserved_bit_5_31        : 27; /**<bit[5 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x7_t;


typedef volatile union {
	struct {
		uint32_t fifo_data                : 16; /**<bit[0 : 15] */
		uint32_t reserved_bit_16_31       : 16; /**<bit[16 : 31] */
	};
	uint32_t v;
} sdmadc_REG0x8_t;

typedef volatile struct {
	volatile sdmadc_REG0x0_t REG0x0;
	volatile sdmadc_REG0x1_t REG0x1;
	volatile sdmadc_REG0x2_t REG0x2;
	volatile sdmadc_REG0x3_t REG0x3;
	volatile sdmadc_REG0x4_t REG0x4;
	volatile sdmadc_REG0x5_t REG0x5;
	volatile sdmadc_REG0x6_t REG0x6;
	volatile sdmadc_REG0x7_t REG0x7;
	volatile sdmadc_REG0x8_t REG0x8;
} sdmadc_hw_t;

#ifdef __cplusplus
}
#endif
