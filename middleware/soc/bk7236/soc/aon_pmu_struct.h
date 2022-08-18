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

typedef volatile struct {
	/* REG_0x00 */
	union {
		struct {
			uint32_t memchk_bps                     : 1;  //0x0[0],memcheck bypass,1,R/W
			uint32_t reserved                       :31;  //0x0[31:1],reserved,0,R
		};
		uint32_t v;
	} reg0;

	/* REG_0x01 */
	union {
		struct {
			uint32_t touch_select                   : 4;  //0x1[3:0], ,None,R
			uint32_t touch_int_en                   :16;  //0x1[19:4], ,None,R/W
			uint32_t usbplug_int_en                 : 1;  //0x1[20], ,None,R/W
			uint32_t reserved                       :11;  //0x1[31:21],reserved,0,R
		};
		uint32_t v;
	} reg1;

	/* REG_0x02 */
	union {
		struct {
			uint32_t wdt_rst_ana                    : 1;  //0x2[0],wdt rst of ana,None,R/W
			uint32_t wdt_rst_top                    : 1;  //0x2[1],wdt rst of top,None,R/W
			uint32_t wdt_rst_aon                    : 1;  //0x2[2],wdt rst of aon,None,R/W
			uint32_t wdt_rst_awt                    : 1;  //0x2[3],wdt rst of awt,None,R/W
			uint32_t wdt_rst_gpio                   : 1;  //0x2[4],wdt rst of gpio,None,R/W
			uint32_t wdt_rst_rtc                    : 1;  //0x2[5],wdt rst of rtc,None,R/W
			uint32_t wdt_rst_wdt                    : 1;  //0x2[6],wdt rst of wdt,None,R/W
			uint32_t wdt_rst_pmu                    : 1;  //0x2[7],wdt rst of pmu,None,R/W
			uint32_t wdt_rst_phy                    : 1;  //0x2[8],wdt rst of phy,None,R/W
			uint32_t reserved                       :23;  //0x2[31:9],reserved,0,R
		};
		uint32_t v;
	} reg2;

	/* REG_0x03 */
	union {
		struct {
			uint32_t fast_boot                      : 1;  //0x3[0], fast boot of bootrom
			uint32_t aon_reg                        :31;  //0x3[31:1],aon_reg3,None,R/W
		};
		uint32_t v;
	} reg3;

	uint32_t reserved0[0x40 - 0x3 - 1];

	/* REG_0x40 */
	union {
		struct {
			uint32_t wake1_delay                    : 4;  //0x40[3:0],wake delay of Xtal,None,R/W
			uint32_t wake2_delay                    : 4;  //0x40[7:4],wake delay of Voltage,None,R/W
			uint32_t wake3_delay                    : 4;  //0x40[11:8], ,None,R/W
			uint32_t halt1_delay                    : 4;  //0x40[15:12],halt delay of Voltage ,None,R/W
			uint32_t halt2_delay                    : 4;  //0x40[19:16],halt delay of Core and flash ,None,R/W
			uint32_t halt3_delay                    : 4;  //0x40[23:20],halt delay of Xtal,None,R/W
			uint32_t halt_volt                      : 1;  //0x40[24],volt   when halt,None,R/W
			uint32_t halt_xtal                      : 1;  //0x40[25],xtal   when halt,None,R/W
			uint32_t halt_core                      : 1;  //0x40[26],core   when halt,None,R/W
			uint32_t halt_flash                     : 1;  //0x40[27],flash  when halt,None,R/W
			uint32_t halt_rosc                      : 1;  //0x40[28],rosc   when halt,None,R/W
			uint32_t halt_resten                    : 1;  //0x40[29],resten when halt,None,R/W
			uint32_t halt_isolat                    : 1;  //0x40[30],isolat when halt,None,R/W
			uint32_t halt_clkena                    : 1;  //0x40[31],clkena when halt,None,R/W
		};
		uint32_t v;
	} reg40;

	/* REG_0x41 */
	union {
		struct {
			uint32_t lpo_config                     : 2;  //0x41[1:0],lpo_src : 1x : clk_ROSC ; 1:clk_X32k ;     0:clk_DIVD,None,R/W
			uint32_t flshsck_iocap                  : 2;  //0x41[3:2],spiflash sck driver selection,None,R/W
			uint32_t wakeup_ena                     : 5;  //0x41[8:4],[8]:int_touched, [7]:int_usbplug, [6]:system_wake, [5]:int_rtc, [4]:int_gpio,None,R/W
			uint32_t reserved0                      :15;  //0x41[23:9],reserved,0,R
			uint32_t halt_lpo                       : 1;  //0x41[24]
			uint32_t reserved1                      : 7;  //0x41[25:31],reserved,0,R
		};
		uint32_t v;
	} reg41;

	/* REG_0x42 */
	uint32_t reserved1;

	/* REG_0x43 */
	union {
		struct {
			uint32_t clr_int_touched                :16;  //0x43[15:0], ,None,R/W
			uint32_t clr_int_usbplug                : 1;  //0x43[16], ,None,R/W
			uint32_t reserved                       :15;  //0x43[31:17],reserved,None,R
		};
		uint32_t v;
	} reg43;

	uint32_t reserved2[0x70 - 0x43 - 1];

	/* REG_0x70 */
	union {
		struct {
			uint32_t int_touched                    :16;  //0x70[15:0], ,None,R/W
			uint32_t int_usbplug                    : 1;  //0x70[16], ,None,R/W
			uint32_t reserved                       :15;  //0x70[31:17],reserved,None,R
		};
		uint32_t v;
	} reg70;

	/* REG_0x71 */
	union {
		struct {
			uint32_t touch_state                    :16;  //0x71[15:0], ,None,R/W
			uint32_t usbplug_state                  : 1;  //0x71[16], ,None,R/W
			uint32_t reserved0                      : 3;  //0x71[19:17],reserved,None,R
			uint32_t wakeup_source                  : 5;  //0x71[24:20], ,None,R/W
			uint32_t reserved1                      : 7;  //0x71[31:25],reserved,None,R
		};
		uint32_t v;
	} reg71;

	uint32_t reserved3[0x7c - 0x71 - 1];

	/* REG_0x7c */
	union {
		struct {
			uint32_t id                             :32;  //0x7c[31:0], ,None,R
		};
		uint32_t v;
	} reg7c;

	/* REG_0x7d */
	union {
		struct {
			uint32_t lcal                           : 8;  //0x7d[7:0], ,None,R
			uint32_t rcal                           : 8;  //0x7d[15:8], ,None,R
			uint32_t adc_cal                        : 7;  //0x7d[22:16], ,None,R
			uint32_t bgcal                          :16;  //0x7d[38:23], ,None,R
			uint32_t ltemp                          : 1;  //0x7d[29], ,None,R
			uint32_t htemp                          : 1;  //0x7d[30], ,None,R
			uint32_t mpll_unlock                    : 1;  //0x7d[31], ,None,R
		};
		uint32_t v;
	} reg7d;

	/* REG_0x7e */
	union {
		struct {
			uint32_t capcal                         : 9;  //0x7e[8:0], ,None,R/W
			uint32_t h                              :23;  //0x7e[31:9], ,None,R
		};
		uint32_t v;
	} reg7e;

	/* REG_0x7f */
	union {
		struct {
			uint32_t  h                              :32;  //0x7f[31:0], ,None,R
		};
		uint32_t v;
	} reg7f;
} aon_pmu_hw_t;

#ifdef __cplusplus
}
#endif

