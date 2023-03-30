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

#include "hal_config.h"
#include "sdmadc_hw.h"
#include "sdmadc_hal.h"

typedef void (*sdmadc_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	sdmadc_dump_fn_t fn;
} sdmadc_reg_fn_map_t;

static void sdmadc_dump_REG0x0(void)
{
	SOC_LOGI("REG0x0: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x0 << 2)));
}

static void sdmadc_dump_REG0x1(void)
{
	SOC_LOGI("REG0x1: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x1 << 2)));
}

static void sdmadc_dump_REG0x2(void)
{
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t *)(SOC_SDMADC_REG_BASE + (0x2 << 2));

	SOC_LOGI("REG0x2: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x2 << 2)));
	SOC_LOGI("	soft_rst: %8x\r\n", r->soft_rst);
	SOC_LOGI("	bypass_ckg: %8x\r\n", r->bypass_ckg);
	SOC_LOGI("	reserved_bit_2_31: %8x\r\n", r->reserved_bit_2_31);
}

static void sdmadc_dump_REG0x3(void)
{
	SOC_LOGI("REG0x3: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x3 << 2)));
}

static void sdmadc_dump_REG0x4(void)
{
	sdmadc_REG0x4_t *r = (sdmadc_REG0x4_t *)(SOC_SDMADC_REG_BASE + (0x4 << 2));

	SOC_LOGI("REG0x4: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x4 << 2)));
	SOC_LOGI("	sample_enable: %8x\r\n", r->sample_enable);
	SOC_LOGI("	reserved_bit_1_31: %8x\r\n", r->reserved_bit_1_31);
}

static void sdmadc_dump_REG0x5(void)
{
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t *)(SOC_SDMADC_REG_BASE + (0x5 << 2));

	SOC_LOGI("REG0x5: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x5 << 2)));
	SOC_LOGI("	sample_mode: %8x\r\n", r->sample_mode);
	SOC_LOGI("	sample_numb: %8x\r\n", r->sample_numb);
	SOC_LOGI("	sample_chsel: %8x\r\n", r->sample_chsel);
	SOC_LOGI("	cic2_bypass: %8x\r\n", r->cic2_bypass);
	SOC_LOGI("	comp_bypass: %8x\r\n", r->comp_bypass);
	SOC_LOGI("	cic2_gains: %8x\r\n", r->cic2_gains);
	SOC_LOGI("	intr_enable: %8x\r\n", r->intr_enable);
	SOC_LOGI("	reserved_bit_20_31: %8x\r\n", r->reserved_bit_20_31);
}

static void sdmadc_dump_REG0x6(void)
{
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t *)(SOC_SDMADC_REG_BASE + (0x6 << 2));

	SOC_LOGI("REG0x6: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x6 << 2)));
	SOC_LOGI("	cali_offset: %8x\r\n", r->cali_offset);
	SOC_LOGI("	cali_gain: %8x\r\n", r->cali_gain);
	SOC_LOGI("	reserved_bit_29_31: %8x\r\n", r->reserved_bit_29_31);
}

static void sdmadc_dump_REG0x7(void)
{
	sdmadc_REG0x7_t *r = (sdmadc_REG0x7_t *)(SOC_SDMADC_REG_BASE + (0x7 << 2));

	SOC_LOGI("REG0x7: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x7 << 2)));
	SOC_LOGI("	sadc_status: %8x\r\n", r->sadc_status);
	SOC_LOGI("	reserved_bit_5_31: %8x\r\n", r->reserved_bit_5_31);
}

static void sdmadc_dump_REG0x8(void)
{
	sdmadc_REG0x8_t *r = (sdmadc_REG0x8_t *)(SOC_SDMADC_REG_BASE + (0x8 << 2));

	SOC_LOGI("REG0x8: %8x\r\n", REG_READ(SOC_SDMADC_REG_BASE + (0x8 << 2)));
	SOC_LOGI("	fifo_data: %8x\r\n", r->fifo_data);
	SOC_LOGI("	reserved_bit_16_31: %8x\r\n", r->reserved_bit_16_31);
}

static sdmadc_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, sdmadc_dump_REG0x0},
	{0x1, 0x1, sdmadc_dump_REG0x1},
	{0x2, 0x2, sdmadc_dump_REG0x2},
	{0x3, 0x3, sdmadc_dump_REG0x3},
	{0x4, 0x4, sdmadc_dump_REG0x4},
	{0x5, 0x5, sdmadc_dump_REG0x5},
	{0x6, 0x6, sdmadc_dump_REG0x6},
	{0x7, 0x7, sdmadc_dump_REG0x7},
	{0x8, 0x8, sdmadc_dump_REG0x8},
	{-1, -1, 0}
};

void sdmadc_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}
