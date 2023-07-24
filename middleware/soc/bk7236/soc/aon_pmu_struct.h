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
		uint32_t memchk_bps               :  1; /**<bit[0 : 0] */
		uint32_t fast_boot                :  1; /**<bit[1 : 1] */
		uint32_t reserved_bit_2_30        : 29; /**<bit[2 : 30] */
		uint32_t gpio_sleep               :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} aon_pmu_r0_t;


typedef volatile union {
	struct {
		uint32_t touch_select             :  4; /**<bit[0 : 3] */
		uint32_t touch_int_en             : 16; /**<bit[4 : 19] */
		uint32_t usbplug_int_en           :  1; /**<bit[20 : 20] */
		uint32_t clk_mod                  :  1; /**<bit[21 : 21] */
		uint32_t reserved_bit_22_31       : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} aon_pmu_r1_t;


typedef volatile union {
	struct {
		uint32_t wdt_rst_ana              :  1; /**<bit[0 : 0] */
		uint32_t wdt_rst_top              :  1; /**<bit[1 : 1] */
		uint32_t wdt_rst_aon              :  1; /**<bit[2 : 2] */
		uint32_t wdt_rst_awt              :  1; /**<bit[3 : 3] */
		uint32_t wdt_rst_gpio             :  1; /**<bit[4 : 4] */
		uint32_t wdt_rst_rtc              :  1; /**<bit[5 : 5] */
		uint32_t wdt_rst_wdt              :  1; /**<bit[6 : 6] */
		uint32_t wdt_rst_pmu              :  1; /**<bit[7 : 7] */
		uint32_t wdt_rst_phy              :  1; /**<bit[8 : 8] */
		uint32_t reserved_bit_9_31        : 23; /**<bit[9 : 31] */
	};
	uint32_t v;
} aon_pmu_r2_t;


typedef volatile union {
	struct {
		uint32_t reserved_bit_0_30        : 31; /**<bit[0 : 30] */
		uint32_t sd_en                    :  1; /**<bit[31 : 31]>*/
	};
	uint32_t v;
} aon_pmu_r3_t;


typedef volatile union {
	struct {
		uint32_t rsv                      : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} aon_pmu_r25_t;


typedef volatile union {
	struct {
		uint32_t wake1_delay              :  4; /**<bit[0 : 3] */
		uint32_t wake2_delay              :  4; /**<bit[4 : 7] */
		uint32_t wake3_delay              :  4; /**<bit[8 : 11] */
		uint32_t halt1_delay              :  4; /**<bit[12 : 15] */
		uint32_t halt2_delay              :  4; /**<bit[16 : 19] */
		uint32_t halt3_delay              :  4; /**<bit[20 : 23] */
		uint32_t halt_volt                :  1; /**<bit[24 : 24] */
		uint32_t halt_xtal                :  1; /**<bit[25 : 25] */
		uint32_t halt_core                :  1; /**<bit[26 : 26] */
		uint32_t halt_flash               :  1; /**<bit[27 : 27] */
		uint32_t halt_rosc                :  1; /**<bit[28 : 28] */
		uint32_t halt_resten              :  1; /**<bit[29 : 29] */
		uint32_t halt_isolat              :  1; /**<bit[30 : 30] */
		uint32_t halt_clkena              :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} aon_pmu_r40_t;


typedef volatile union {
	struct {
		uint32_t lpo_config               :  2; /**<bit[0 : 1] */
		uint32_t flshsck_iocap            :  2; /**<bit[2 : 3] */
		uint32_t wakeup_ena               :  6; /**<bit[4 : 9] */
		uint32_t io_drv                   :  2; /**<bit[10 : 11] */
		uint32_t reserved_bit_12_13       :  2; /**<bit[12 : 13] */
		uint32_t xtal_sel                 :  1; /**<bit[14 : 14] */
		uint32_t reserved_bit_15_23       :  9; /**<bit[15 : 23] */
		uint32_t halt_lpo                 :  1; /**<bit[24 : 24] */
		uint32_t halt_busrst              :  1; /**<bit[25 : 25] */
		uint32_t halt_busiso              :  1; /**<bit[26 : 26] */
		uint32_t halt_buspwd              :  1; /**<bit[27 : 27] */
		uint32_t halt_blpiso              :  1; /**<bit[28 : 28] */
		uint32_t halt_blppwd              :  1; /**<bit[29 : 29] */
		uint32_t halt_wlpiso              :  1; /**<bit[30 : 30] */
		uint32_t halt_wlppwd              :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} aon_pmu_r41_t;


typedef volatile union {
	struct {
		uint32_t clr_int_touched          : 16; /**<bit[0 : 15] */
		uint32_t clr_int_usbplug          :  1; /**<bit[16 : 16] */
		uint32_t clr_wakeup               :  1; /**<bit[17 : 17] */
		uint32_t reserved_bit_18_31       : 14; /**<bit[18 : 31] */
	};
	uint32_t v;
} aon_pmu_r43_t;


typedef volatile union {
	struct {
		uint32_t int_touched              : 16; /**<bit[0 : 15] */
		uint32_t int_usbplug              :  1; /**<bit[16 : 16] */
		uint32_t reserved_bit_17_31       : 15; /**<bit[17 : 31] */
	};
	uint32_t v;
} aon_pmu_r70_t;


typedef volatile union {
	struct {
		uint32_t touch_state              : 16; /**<bit[0 : 15] */
		uint32_t usbplug_state            :  1; /**<bit[16 : 16] */
		uint32_t reserved_bit_17_19       :  3; /**<bit[17 : 19] */
		uint32_t wakeup_source            :  5; /**<bit[20 : 24] */
		uint32_t reserved_bit_25_31       :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} aon_pmu_r71_t;

typedef volatile union {
	struct {
		uint32_t vdw_soft1v               : 16; /**<bit[0 : 15]*/
		uint32_t vup_soft1v               : 16; /**<bit[16 :31]*/
	};
	uint32_t v;
} aon_pmu_r72_t;

typedef volatile union {
	struct {
		uint32_t mul_touch_int            : 16; /**<bit[0 : 15]*/
		uint32_t wake_up_soft             :  1; /**<bit[16 : 16]*/
		uint32_t cap_cal                  :  9; /**<bit[17 : 25]*/
		uint32_t cal_done                 :  1; /**<bit[26 : 26]*/
		uint32_t nc_27_31                 :  5; /**<bit[27 : 31]*/
	};
	uint32_t v;
} aon_pmu_r73_t;

typedef volatile union {
	struct {
		uint32_t id                       : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} aon_pmu_r7c_t;


typedef volatile union {
	struct {
		uint32_t lcal                     :  8; /**<bit[0 : 7] */
		uint32_t l                        :  1; /**<bit[8 : 8] */
		uint32_t adc_cal                  :  6; /**<bit[9 : 14] */
		uint32_t bgcal                    :  6; /**<bit[15 : 20] */
		uint32_t pll_26m_unlock           :  1; /**<bit[21 : 21] */
		uint32_t dpll_unlock              :  1; /**<bit[22 : 22] */
		uint32_t h1                       :  9; /**<bit[23 : 31] */
	};
	uint32_t v;
} aon_pmu_r7d_t;


typedef volatile union {
	struct {
		uint32_t cbcal                    :  5; /**<bit[0 : 4] */
		uint32_t ad_state                 :  4; /**<bit[5 : 8] */
		uint32_t td_states1               : 13; /**<bit[9 : 21] */
		uint32_t h2                       : 10; /**<bit[22 : 31] */
	};
	uint32_t v;
} aon_pmu_r7e_t;

typedef volatile struct {
	volatile aon_pmu_r0_t r0;
	volatile aon_pmu_r1_t r1;
	volatile aon_pmu_r2_t r2;
	volatile aon_pmu_r3_t r3;
	volatile uint32_t rsv_4_3f[60];
	volatile aon_pmu_r40_t r40;
	volatile aon_pmu_r41_t r41;
	volatile uint32_t rsv_42_42[1];
	volatile aon_pmu_r43_t r43;
	volatile uint32_t rsv_44_6f[44];
	volatile aon_pmu_r70_t r70;
	volatile aon_pmu_r71_t r71;
	volatile aon_pmu_r72_t r72;
	volatile aon_pmu_r73_t r73;
	volatile uint32_t rsv_74_7b[8];
	volatile aon_pmu_r7c_t r7c;
	volatile aon_pmu_r7d_t r7d;
	volatile aon_pmu_r7e_t r7e;
} aon_pmu_hw_t;

#ifdef __cplusplus
}
#endif
