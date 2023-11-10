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
#include "aon_pmu_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AON_PMU_LL_REG_BASE   SOC_AON_PMU_REG_BASE

//reg r0:

static inline void aon_pmu_ll_set_r0(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r0(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r0_memchk_bps(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->memchk_bps = v;
}

static inline uint32_t aon_pmu_ll_get_r0_memchk_bps(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->memchk_bps;
}

static inline void aon_pmu_ll_set_r0_fast_boot(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->fast_boot = v;
}

static inline uint32_t aon_pmu_ll_get_r0_fast_boot(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->fast_boot;
}

static inline void aon_pmu_ll_set_r0_gpio_sleep(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->gpio_sleep = v;
}

static inline uint32_t aon_pmu_ll_get_r0_gpio_sleep(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->gpio_sleep;
}

//reg r1:

static inline void aon_pmu_ll_set_r1(uint32_t v) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r1(void) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r1_touch_select(void) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	return r->touch_select;
}

static inline void aon_pmu_ll_set_r1_touch_int_en(uint32_t v) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	r->touch_int_en = v;
}

static inline uint32_t aon_pmu_ll_get_r1_touch_int_en(void) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	return r->touch_int_en;
}

static inline void aon_pmu_ll_set_r1_usbplug_int_en(uint32_t v) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	r->usbplug_int_en = v;
}

static inline uint32_t aon_pmu_ll_get_r1_usbplug_int_en(void) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	return r->usbplug_int_en;
}

static inline uint32_t aon_pmu_ll_get_r1_clk_mod(void) {
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	return r->clk_mod;
}

//reg r2:

static inline void aon_pmu_ll_set_r2(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r2(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_ana(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_ana = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_ana(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_ana;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_top(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_top = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_top(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_top;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_aon(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_aon = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_aon(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_aon;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_awt(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_awt = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_awt(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_awt;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_gpio(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_gpio = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_gpio(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_gpio;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_rtc(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_rtc = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_rtc(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_rtc;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_wdt(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_wdt = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_wdt(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_wdt;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_pmu(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_pmu = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_pmu(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_pmu;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_phy(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_phy = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_phy(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_phy;
}

//reg r3:

static inline void aon_pmu_ll_set_r3(uint32_t v) {
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r3(void) {
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r3_rsv(uint32_t v) {
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	r->rsv = v;
}

static inline uint32_t aon_pmu_ll_get_r3_rsv(void) {
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	return r->rsv;
}

//reg r40:

static inline void aon_pmu_ll_set_r40(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r40(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r40_wake1_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake1_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_wake1_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->wake1_delay;
}

static inline void aon_pmu_ll_set_r40_wake2_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake2_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_wake2_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->wake2_delay;
}

static inline void aon_pmu_ll_set_r40_wake3_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake3_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_wake3_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->wake3_delay;
}

static inline void aon_pmu_ll_set_r40_halt1_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt1_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt1_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt1_delay;
}

static inline void aon_pmu_ll_set_r40_halt2_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt2_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt2_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt2_delay;
}

static inline void aon_pmu_ll_set_r40_halt3_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt3_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt3_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt3_delay;
}

static inline void aon_pmu_ll_set_r40_halt_volt(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_volt = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_volt(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_volt;
}

static inline void aon_pmu_ll_set_r40_halt_xtal(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_xtal = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_xtal(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_xtal;
}

static inline void aon_pmu_ll_set_r40_halt_core(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_core = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_core(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_core;
}

static inline void aon_pmu_ll_set_r40_halt_flash(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_flash = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_flash(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_flash;
}

static inline void aon_pmu_ll_set_r40_halt_rosc(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_rosc = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_rosc(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_rosc;
}

static inline void aon_pmu_ll_set_r40_halt_resten(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_resten = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_resten(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_resten;
}

static inline void aon_pmu_ll_set_r40_halt_isolat(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_isolat = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_isolat(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_isolat;
}

static inline void aon_pmu_ll_set_r40_halt_clkena(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_clkena = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_clkena(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_clkena;
}

//reg r41:

static inline void aon_pmu_ll_set_r41(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r41(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r41_lpo_config(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->lpo_config = v;
}

static inline uint32_t aon_pmu_ll_get_r41_lpo_config(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->lpo_config;
}

static inline void aon_pmu_ll_set_r41_flshsck_iocap(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->flshsck_iocap = v;
}

static inline uint32_t aon_pmu_ll_get_r41_flshsck_iocap(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->flshsck_iocap;
}

static inline void aon_pmu_ll_set_r41_wakeup_ena(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->wakeup_ena = v;
}

static inline uint32_t aon_pmu_ll_get_r41_wakeup_ena(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->wakeup_ena;
}

static inline void aon_pmu_ll_set_r41_io_drv(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->io_drv = v;
}

static inline uint32_t aon_pmu_ll_get_r41_io_drv(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->io_drv;
}

static inline void aon_pmu_ll_set_r41_xtal_sel(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->xtal_sel = v;
}

static inline uint32_t aon_pmu_ll_get_r41_xtal_sel(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->xtal_sel;
}

static inline void aon_pmu_ll_set_r41_halt_lpo(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_lpo = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_lpo(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_lpo;
}

static inline void aon_pmu_ll_set_r41_halt_busrst(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_busrst = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_busrst(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_busrst;
}

static inline void aon_pmu_ll_set_r41_halt_busiso(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_busiso = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_busiso(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_busiso;
}

static inline void aon_pmu_ll_set_r41_halt_buspwd(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_buspwd = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_buspwd(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_buspwd;
}

static inline void aon_pmu_ll_set_r41_halt_blpiso(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_blpiso = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_blpiso(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_blpiso;
}

static inline void aon_pmu_ll_set_r41_halt_blppwd(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_blppwd = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_blppwd(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_blppwd;
}

static inline void aon_pmu_ll_set_r41_halt_wlpiso(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_wlpiso = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_wlpiso(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_wlpiso;
}

static inline void aon_pmu_ll_set_r41_halt_wlppwd(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_wlppwd = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_wlppwd(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_wlppwd;
}

//reg r43:

static inline void aon_pmu_ll_set_r43(uint32_t v) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r43(void) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r43_clr_int_touched(uint32_t v) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->clr_int_touched = v;
}

static inline uint32_t aon_pmu_ll_get_r43_clr_int_touched(void) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	return r->clr_int_touched;
}

static inline void aon_pmu_ll_set_r43_clr_int_usbplug(uint32_t v) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->clr_int_usbplug = v;
}

static inline uint32_t aon_pmu_ll_get_r43_clr_int_usbplug(void) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	return r->clr_int_usbplug;
}

static inline void aon_pmu_ll_set_r43_clr_wakeup(uint32_t v) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->clr_wakeup = v;
}

static inline uint32_t aon_pmu_ll_get_r43_clr_wakeup(void) {
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	return r->clr_wakeup;
}

//reg r70:

static inline void aon_pmu_ll_set_r70(uint32_t v) {
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r70(void) {
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r70_int_touched(void) {
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	return r->int_touched;
}

static inline uint32_t aon_pmu_ll_get_r70_int_usbplug(void) {
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	return r->int_usbplug;
}

//reg r71:

static inline void aon_pmu_ll_set_r71(uint32_t v) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r71(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r71_touch_state(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->touch_state;
}

static inline uint32_t aon_pmu_ll_get_r71_usbplug_state(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->usbplug_state;
}

static inline uint32_t aon_pmu_ll_get_r71_wakeup_source(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->wakeup_source;
}

//reg reg72:

static inline void aon_pmu_ll_set_reg72_value(uint32_t v) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_reg72_value(void) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_reg72_vdw_soft1v(void) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	return r->vdw_soft1v;
}

static inline uint32_t aon_pmu_ll_get_reg72_vup_soft1v(void) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	return r->vup_soft1v;
}

//reg reg73:

static inline void aon_pmu_ll_set_reg73_value(uint32_t v) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_reg73_value(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_reg73_mul_touch_int(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->mul_touch_int;
}

static inline uint32_t aon_pmu_ll_get_reg73_wake_up_soft(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->wake_up_soft;
}

static inline uint32_t aon_pmu_ll_get_reg73_cap_cal(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->cap_cal;
}

static inline uint32_t aon_pmu_ll_get_reg73_cal_done(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->cal_done;
}

//reg r7c:

static inline void aon_pmu_ll_set_r7c(uint32_t v) {
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r7c(void) {
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7c_id(void) {
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	return r->id;
}

//reg r7d:

static inline void aon_pmu_ll_set_r7d(uint32_t v) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r7d(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7d_lcal(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->lcal;
}

static inline uint32_t aon_pmu_ll_get_r7d_l(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->l;
}

static inline uint32_t aon_pmu_ll_get_r7d_adc_cal(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->adc_cal;
}

static inline uint32_t aon_pmu_ll_get_r7d_bgcal(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->bgcal;
}

static inline uint32_t aon_pmu_ll_get_r7d_26mpll_unlock(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->pll_26m_unlock;
}

static inline uint32_t aon_pmu_ll_get_r7d_dpll_unlock(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->dpll_unlock;
}

static inline uint32_t aon_pmu_ll_get_r7d_h1(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->h1;
}

//reg r7e:

static inline void aon_pmu_ll_set_r7e(uint32_t v) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r7e(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7e_cbcal(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->cbcal;
}

static inline uint32_t aon_pmu_ll_get_r7e_ad_state(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->ad_state;
}

static inline uint32_t aon_pmu_ll_get_r7e_td_states1(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->td_states1;
}

static inline uint32_t aon_pmu_ll_get_r7e_h2(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->h2;
}
#ifdef __cplusplus
}
#endif
