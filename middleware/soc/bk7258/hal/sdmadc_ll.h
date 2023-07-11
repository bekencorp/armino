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

#include <soc/soc.h>
#include "hal_port.h"
#include "sdmadc_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SDMADC_LL_REG_BASE   SOC_SDMADC_REG_BASE

//reg REG0x0:

static inline void sdmadc_ll_set_REG0x0_value(uint32_t v) {
	sdmadc_REG0x0_t *r = (sdmadc_REG0x0_t*)(SOC_SDMADC_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x0_value(void) {
	sdmadc_REG0x0_t *r = (sdmadc_REG0x0_t*)(SOC_SDMADC_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline uint32_t sdmadc_ll_get_REG0x0_deviceid(void) {
	sdmadc_REG0x0_t *r = (sdmadc_REG0x0_t*)(SOC_SDMADC_REG_BASE + (0x0 << 2));
	return r->deviceid;
}

//reg REG0x1:

static inline void sdmadc_ll_set_REG0x1_value(uint32_t v) {
	sdmadc_REG0x1_t *r = (sdmadc_REG0x1_t*)(SOC_SDMADC_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x1_value(void) {
	sdmadc_REG0x1_t *r = (sdmadc_REG0x1_t*)(SOC_SDMADC_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline uint32_t sdmadc_ll_get_REG0x1_versionid(void) {
	sdmadc_REG0x1_t *r = (sdmadc_REG0x1_t*)(SOC_SDMADC_REG_BASE + (0x1 << 2));
	return r->versionid;
}

//reg REG0x2:

static inline void sdmadc_ll_set_REG0x2_value(uint32_t v) {
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t*)(SOC_SDMADC_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x2_value(void) {
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t*)(SOC_SDMADC_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void sdmadc_ll_set_REG0x2_soft_rst(uint32_t v) {
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t*)(SOC_SDMADC_REG_BASE + (0x2 << 2));
	r->soft_rst = v;
}

static inline uint32_t sdmadc_ll_get_REG0x2_soft_rst(void) {
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t*)(SOC_SDMADC_REG_BASE + (0x2 << 2));
	return r->soft_rst;
}

static inline void sdmadc_ll_set_REG0x2_bypass_ckg(uint32_t v) {
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t*)(SOC_SDMADC_REG_BASE + (0x2 << 2));
	r->bypass_ckg = v;
}

static inline uint32_t sdmadc_ll_get_REG0x2_bypass_ckg(void) {
	sdmadc_REG0x2_t *r = (sdmadc_REG0x2_t*)(SOC_SDMADC_REG_BASE + (0x2 << 2));
	return r->bypass_ckg;
}

//reg REG0x3:

static inline void sdmadc_ll_set_REG0x3_value(uint32_t v) {
	sdmadc_REG0x3_t *r = (sdmadc_REG0x3_t*)(SOC_SDMADC_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x3_value(void) {
	sdmadc_REG0x3_t *r = (sdmadc_REG0x3_t*)(SOC_SDMADC_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline uint32_t sdmadc_ll_get_REG0x3_global_status(void) {
	sdmadc_REG0x3_t *r = (sdmadc_REG0x3_t*)(SOC_SDMADC_REG_BASE + (0x3 << 2));
	return r->global_status;
}

//reg REG0x4:

static inline void sdmadc_ll_set_REG0x4_value(uint32_t v) {
	sdmadc_REG0x4_t *r = (sdmadc_REG0x4_t*)(SOC_SDMADC_REG_BASE + (0x4 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x4_value(void) {
	sdmadc_REG0x4_t *r = (sdmadc_REG0x4_t*)(SOC_SDMADC_REG_BASE + (0x4 << 2));
	return r->v;
}

static inline void sdmadc_ll_set_REG0x4_sample_enable(uint32_t v) {
	sdmadc_REG0x4_t *r = (sdmadc_REG0x4_t*)(SOC_SDMADC_REG_BASE + (0x4 << 2));
	r->sample_enable = v;
}

static inline uint32_t sdmadc_ll_get_REG0x4_sample_enable(void) {
	sdmadc_REG0x4_t *r = (sdmadc_REG0x4_t*)(SOC_SDMADC_REG_BASE + (0x4 << 2));
	return r->sample_enable;
}

//reg REG0x5:

static inline void sdmadc_ll_set_REG0x5_value(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_value(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->v;
}

static inline void sdmadc_ll_set_REG0x5_sample_mode(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->sample_mode = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_sample_mode(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->sample_mode;
}

static inline void sdmadc_ll_set_REG0x5_sample_numb(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->sample_numb = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_sample_numb(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->sample_numb;
}

static inline void sdmadc_ll_set_REG0x5_sample_chsel(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->sample_chsel = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_sample_chsel(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->sample_chsel;
}

static inline void sdmadc_ll_set_REG0x5_cic2_bypass(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->cic2_bypass = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_cic2_bypass(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->cic2_bypass;
}

static inline void sdmadc_ll_set_REG0x5_comp_bypass(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->comp_bypass = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_comp_bypass(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->comp_bypass;
}

static inline void sdmadc_ll_set_REG0x5_cic2_gains(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->cic2_gains = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_cic2_gains(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->cic2_gains;
}

static inline void sdmadc_ll_set_REG0x5_intr_enable(uint32_t v) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	r->intr_enable = v;
}

static inline uint32_t sdmadc_ll_get_REG0x5_intr_enable(void) {
	sdmadc_REG0x5_t *r = (sdmadc_REG0x5_t*)(SOC_SDMADC_REG_BASE + (0x5 << 2));
	return r->intr_enable;
}

//reg REG0x6:

static inline void sdmadc_ll_set_REG0x6_value(uint32_t v) {
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t*)(SOC_SDMADC_REG_BASE + (0x6 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x6_value(void) {
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t*)(SOC_SDMADC_REG_BASE + (0x6 << 2));
	return r->v;
}

static inline void sdmadc_ll_set_REG0x6_cali_offset(uint32_t v) {
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t*)(SOC_SDMADC_REG_BASE + (0x6 << 2));
	r->cali_offset = v;
}

static inline uint32_t sdmadc_ll_get_REG0x6_cali_offset(void) {
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t*)(SOC_SDMADC_REG_BASE + (0x6 << 2));
	return r->cali_offset;
}

static inline void sdmadc_ll_set_REG0x6_cali_gain(uint32_t v) {
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t*)(SOC_SDMADC_REG_BASE + (0x6 << 2));
	r->cali_gain = v;
}

static inline uint32_t sdmadc_ll_get_REG0x6_cali_gain(void) {
	sdmadc_REG0x6_t *r = (sdmadc_REG0x6_t*)(SOC_SDMADC_REG_BASE + (0x6 << 2));
	return r->cali_gain;
}

//reg REG0x7:

static inline void sdmadc_ll_set_REG0x7_value(uint32_t v) {
	sdmadc_REG0x7_t *r = (sdmadc_REG0x7_t*)(SOC_SDMADC_REG_BASE + (0x7 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x7_value(void) {
	sdmadc_REG0x7_t *r = (sdmadc_REG0x7_t*)(SOC_SDMADC_REG_BASE + (0x7 << 2));
	return r->v;
}

static inline uint32_t sdmadc_ll_get_REG0x7_sadc_status(void) {
	//sdmadc_REG0x7_t *r = (sdmadc_REG0x7_t*)(SOC_SDMADC_REG_BASE + (0x7 << 2));
	//return r->sadc_status;

	return (REG_READ(SOC_SDMADC_REG_BASE + (0x7 << 2))) & 0x1f;
}

//reg REG0x8:

static inline void sdmadc_ll_set_REG0x8_value(uint32_t v) {
	sdmadc_REG0x8_t *r = (sdmadc_REG0x8_t*)(SOC_SDMADC_REG_BASE + (0x8 << 2));
	r->v = v;
}

static inline uint32_t sdmadc_ll_get_REG0x8_value(void) {
	sdmadc_REG0x8_t *r = (sdmadc_REG0x8_t*)(SOC_SDMADC_REG_BASE + (0x8 << 2));
	return r->v;
}

static inline uint32_t sdmadc_ll_get_REG0x8_fifo_data(void) {
	sdmadc_REG0x8_t *r = (sdmadc_REG0x8_t*)(SOC_SDMADC_REG_BASE + (0x8 << 2));
	return r->fifo_data;
}
#ifdef __cplusplus
}
#endif
