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
                            
#include <soc/soc.h>          
#include "aon_pmu_hw.h" 
#if (CONFIG_SOC_BK7256XX)
#include "aon_pmu_ll_macro_def.h"       
#endif

#ifdef __cplusplus          
extern "C" {              
#endif                      
/* REG_0x00 */

static inline uint32_t aon_pmu_ll_get_reg0_value(aon_pmu_hw_t *hw)
{
    return hw->reg0.v;
}

static inline void aon_pmu_ll_set_reg0_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg0.v = value;
}

/* REG_0x00:reg0->memchk_bps:memcheck bypass,R/W,0x0[0]*/
static inline uint32_t aon_pmu_ll_get_reg0_memchk_bps(aon_pmu_hw_t *hw)
{
    return hw->reg0.memchk_bps;
}

static inline void aon_pmu_ll_set_reg0_memchk_bps(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg0.memchk_bps = value;
}

/* REG_0x01 */

/* REG_0x01 */

static inline uint32_t aon_pmu_ll_get_reg1_value(aon_pmu_hw_t *hw)
{
    return hw->reg1.v;
}

/* REG_0x01:reg1->touch_select: ,R,0x1[3:0]*/
static inline uint32_t aon_pmu_ll_get_reg1_touch_select(aon_pmu_hw_t *hw)
{
    return hw->reg1.touch_select;
}

/* REG_0x01:reg1->touch_int_en: ,R/W,0x1[19:4]*/
static inline uint32_t aon_pmu_ll_get_reg1_touch_int_en(aon_pmu_hw_t *hw)
{
    return hw->reg1.touch_int_en;
}

static inline void aon_pmu_ll_set_reg1_touch_int_en(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg1.touch_int_en = value;
}

/* REG_0x01:reg1->usbplug_int_en: ,R/W,0x1[20]*/
static inline uint32_t aon_pmu_ll_get_reg1_usbplug_int_en(aon_pmu_hw_t *hw)
{
    return hw->reg1.usbplug_int_en;
}

static inline void aon_pmu_ll_set_reg1_usbplug_int_en(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg1.usbplug_int_en = value;
}

/* REG_0x02 */

static inline uint32_t aon_pmu_ll_get_reg2_value(aon_pmu_hw_t *hw)
{
    return hw->reg2.v;
}

static inline void aon_pmu_ll_set_reg2_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.v = value;
}

/* REG_0x02:reg2->wdt_rst_ana:wdt rst of ana,R/W,0x2[0]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_ana(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_ana;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_ana(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_ana = value;
}

/* REG_0x02:reg2->wdt_rst_top:wdt rst of top,R/W,0x2[1]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_top(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_top;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_top(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_top = value;
}

/* REG_0x02:reg2->wdt_rst_aon:wdt rst of aon,R/W,0x2[2]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_aon(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_aon;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_aon(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_aon = value;
}

/* REG_0x02:reg2->wdt_rst_awt:wdt rst of awt,R/W,0x2[3]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_awt(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_awt;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_awt(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_awt = value;
}

/* REG_0x02:reg2->wdt_rst_gpio:wdt rst of gpio,R/W,0x2[4]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_gpio(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_gpio;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_gpio(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_gpio = value;
}

/* REG_0x02:reg2->wdt_rst_rtc:wdt rst of rtc,R/W,0x2[5]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_rtc(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_rtc;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_rtc(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_rtc = value;
}

/* REG_0x02:reg2->wdt_rst_wdt:wdt rst of wdt,R/W,0x2[6]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_wdt(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_wdt;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_wdt(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_wdt = value;
}

/* REG_0x02:reg2->wdt_rst_pmu:wdt rst of pmu,R/W,0x2[7]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_pmu(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_pmu;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_pmu(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_pmu = value;
}

/* REG_0x02:reg2->wdt_rst_phy:wdt rst of phy,R/W,0x2[8]*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_phy(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_phy;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_phy(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_phy = value;
}

/* REG_0x03 */

static inline uint32_t aon_pmu_ll_get_reg3_value(aon_pmu_hw_t *hw)
{
    return hw->reg3.v;
}

static inline void aon_pmu_ll_set_reg3_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg3.v = value;
}

/* REG_0x03:reg3->aon_reg3:aon_reg3,R/W,0x3[31:0]*/
static inline uint32_t aon_pmu_ll_get_reg3_aon_reg3(aon_pmu_hw_t *hw)
{
    return hw->reg3.v;
}

static inline void aon_pmu_ll_set_reg3_aon_reg3(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg3.v = value;
}

/* REG_0x40 */

static inline uint32_t aon_pmu_ll_get_reg40_value(aon_pmu_hw_t *hw)
{
    return hw->reg40.v;
}

static inline void aon_pmu_ll_set_reg40_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.v = value;
}

/* REG_0x40:reg40->wake1_delay:wake delay of Xtal,R/W,0x40[3:0]*/
static inline uint32_t aon_pmu_ll_get_reg40_wake1_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.wake1_delay;
}

static inline void aon_pmu_ll_set_reg40_wake1_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.wake1_delay = value;
}

/* REG_0x40:reg40->wake2_delay:wake delay of Voltage,R/W,0x40[7:4]*/
static inline uint32_t aon_pmu_ll_get_reg40_wake2_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.wake2_delay;
}

static inline void aon_pmu_ll_set_reg40_wake2_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.wake2_delay = value;
}

/* REG_0x40:reg40->wake3_delay: ,R/W,0x40[11:8]*/
static inline uint32_t aon_pmu_ll_get_reg40_wake3_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.wake3_delay;
}

static inline void aon_pmu_ll_set_reg40_wake3_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.wake3_delay = value;
}

/* REG_0x40:reg40->halt1_delay:halt delay of Voltage ,R/W,0x40[15:12]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt1_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt1_delay;
}

static inline void aon_pmu_ll_set_reg40_halt1_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt1_delay = value;
}

/* REG_0x40:reg40->halt2_delay:halt delay of Core and flash ,R/W,0x40[19:16]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt2_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt2_delay;
}

static inline void aon_pmu_ll_set_reg40_halt2_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt2_delay = value;
}

/* REG_0x40:reg40->halt3_delay:halt delay of Xtal,R/W,0x40[23:20]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt3_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt3_delay;
}

static inline void aon_pmu_ll_set_reg40_halt3_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt3_delay = value;
}

/* REG_0x40:reg40->halt_volt:volt   when halt,R/W,0x40[24]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_volt(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_volt;
}

static inline void aon_pmu_ll_set_reg40_halt_volt(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_volt = value;
}

/* REG_0x40:reg40->halt_xtal:xtal   when halt,R/W,0x40[25]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_xtal(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_xtal;
}

static inline void aon_pmu_ll_set_reg40_halt_xtal(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_xtal = value;
}

/* REG_0x40:reg40->halt_core:core   when halt,R/W,0x40[26]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_core(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_core;
}

static inline void aon_pmu_ll_set_reg40_halt_core(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_core = value;
}

/* REG_0x40:reg40->halt_flash:flash  when halt,R/W,0x40[27]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_flash(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_flash;
}

static inline void aon_pmu_ll_set_reg40_halt_flash(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_flash = value;
}

/* REG_0x40:reg40->halt_rosc:rosc   when halt,R/W,0x40[28]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_rosc(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_rosc;
}

static inline void aon_pmu_ll_set_reg40_halt_rosc(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_rosc = value;
}

/* REG_0x40:reg40->halt_resten:resten when halt,R/W,0x40[29]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_resten(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_resten;
}

static inline void aon_pmu_ll_set_reg40_halt_resten(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_resten = value;
}

/* REG_0x40:reg40->halt_isolat:isolat when halt,R/W,0x40[30]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_isolat(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_isolat;
}

static inline void aon_pmu_ll_set_reg40_halt_isolat(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_isolat = value;
}

/* REG_0x40:reg40->halt_clkena:clkena when halt,R/W,0x40[31]*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_clkena(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_clkena;
}

static inline void aon_pmu_ll_set_reg40_halt_clkena(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_clkena = value;
}

/* REG_0x41 */

static inline uint32_t aon_pmu_ll_get_reg41_value(aon_pmu_hw_t *hw)
{
    return hw->reg41.v;
}

static inline void aon_pmu_ll_set_reg41_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg41.v = value;
}

/* REG_0x41:reg41->lpo_config:lpo_src : 1x : clk_ROSC ; 1:clk_X32k ;     0:clk_DIVD,R/W,0x41[1:0]*/
static inline uint32_t aon_pmu_ll_get_reg41_lpo_config(aon_pmu_hw_t *hw)
{
    return hw->reg41.lpo_config;
}

static inline void aon_pmu_ll_set_reg41_lpo_config(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg41.lpo_config = value;
}

/* REG_0x41:reg41->flshsck_iocap:spiflash sck driver selection,R/W,0x41[3:2]*/
static inline uint32_t aon_pmu_ll_get_reg41_flshsck_iocap(aon_pmu_hw_t *hw)
{
    return hw->reg41.flshsck_iocap;
}

static inline void aon_pmu_ll_set_reg41_flshsck_iocap(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg41.flshsck_iocap = value;
}

/* REG_0x41:reg41->wakeup_ena:[8]:int_touched                                                                 [7]:int_usbplug                                                                    [6]:system_wake                                                                   [5]:int_rtc                                                                                 [4]:int_gpio,R/W,0x41[8:4]*/
static inline uint32_t aon_pmu_ll_get_reg41_wakeup_ena(aon_pmu_hw_t *hw)
{
    return hw->reg41.wakeup_ena;
}

static inline void aon_pmu_ll_set_reg41_wakeup_ena(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg41.wakeup_ena = value;
}

/* REG_0x42 */

static inline uint32_t aon_pmu_ll_get_reg42_value(aon_pmu_hw_t *hw)
{
    return hw->reg42.v;
}

static inline void aon_pmu_ll_set_reg42_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg42.v = value;
}

/* REG_0x42:reg42->reg42: ,R/W,0x42[31:0]*/
static inline uint32_t aon_pmu_ll_get_reg42_reg42(aon_pmu_hw_t *hw)
{
    return hw->reg42.v;
}

static inline void aon_pmu_ll_set_reg42_reg42(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg42.v = value;
}

/* REG_0x43 */

static inline uint32_t aon_pmu_ll_get_reg43_value(aon_pmu_hw_t *hw)
{
    return hw->reg43.v;
}

static inline void aon_pmu_ll_set_reg43_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg43.v = value;
}

/* REG_0x43:reg43->clr_int_touched: ,R/W,0x43[15:0]*/
static inline uint32_t aon_pmu_ll_get_reg43_clr_int_touched(aon_pmu_hw_t *hw)
{
    return hw->reg43.clr_int_touched;
}

static inline void aon_pmu_ll_set_reg43_clr_int_touched(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg43.clr_int_touched = value;
}

/* REG_0x43:reg43->clr_int_usbplug: ,R/W,0x43[16]*/
static inline uint32_t aon_pmu_ll_get_reg43_clr_int_usbplug(aon_pmu_hw_t *hw)
{
    return hw->reg43.clr_int_usbplug;
}

static inline void aon_pmu_ll_set_reg43_clr_int_usbplug(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg43.clr_int_usbplug = value;
}

/* REG_0x70 */

static inline uint32_t aon_pmu_ll_get_reg70_value(aon_pmu_hw_t *hw)
{
    return hw->reg70.v;
}

static inline void aon_pmu_ll_set_reg70_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg70.v = value;
}

/* REG_0x70:reg70->int_touched: ,R/W,0x70[15:0]*/
static inline uint32_t aon_pmu_ll_get_reg70_int_touched(aon_pmu_hw_t *hw)
{
    return hw->reg70.int_touched;
}

static inline void aon_pmu_ll_set_reg70_int_touched(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg70.int_touched = value;
}

/* REG_0x70:reg70->int_usbplug: ,R/W,0x70[16]*/
static inline uint32_t aon_pmu_ll_get_reg70_int_usbplug(aon_pmu_hw_t *hw)
{
    return hw->reg70.int_usbplug;
}

static inline void aon_pmu_ll_set_reg70_int_usbplug(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg70.int_usbplug = value;
}

/* REG_0x71 */

static inline uint32_t aon_pmu_ll_get_reg71_value(aon_pmu_hw_t *hw)
{
    return hw->reg71.v;
}

static inline void aon_pmu_ll_set_reg71_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg71.v = value;
}

/* REG_0x71:reg71->touch_state: ,R/W,0x71[15:0]*/
static inline uint32_t aon_pmu_ll_get_reg71_touch_state(aon_pmu_hw_t *hw)
{
    return hw->reg71.touch_state;
}

static inline void aon_pmu_ll_set_reg71_touch_state(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg71.touch_state = value;
}

/* REG_0x71:reg71->usbplug_state: ,R/W,0x71[16]*/
static inline uint32_t aon_pmu_ll_get_reg71_usbplug_state(aon_pmu_hw_t *hw)
{
    return hw->reg71.usbplug_state;
}

static inline void aon_pmu_ll_set_reg71_usbplug_state(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg71.usbplug_state = value;
}

/* REG_0x71:reg71->wakeup_source: ,R/W,0x71[24:20]*/
static inline uint32_t aon_pmu_ll_get_reg71_wakeup_source(aon_pmu_hw_t *hw)
{
    return hw->reg71.wakeup_source;
}

static inline void aon_pmu_ll_set_reg71_wakeup_source(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg71.wakeup_source = value;
}

/* REG_0x7c */

/* REG_0x7c */

static inline uint32_t aon_pmu_ll_get_reg7c_value(aon_pmu_hw_t *hw)
{
    return hw->reg7c.v;
}

/* REG_0x7c:reg7c->ID: ,R,0x7c[31:0]*/
static inline uint32_t aon_pmu_ll_get_reg7c_id(aon_pmu_hw_t *hw)
{
    return hw->reg7c.v;
}

/* REG_0x7d */

/* REG_0x7d */

static inline uint32_t aon_pmu_ll_get_reg7d_value(aon_pmu_hw_t *hw)
{
    return hw->reg7d.v;
}

/* REG_0x7d:reg7d->lcal: ,R,0x7d[7:0]*/
static inline uint32_t aon_pmu_ll_get_reg7d_lcal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.lcal;
}

/* REG_0x7d:reg7d->rcal: ,R,0x7d[15:8]*/
static inline uint32_t aon_pmu_ll_get_reg7d_rcal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.rcal;
}

/* REG_0x7d:reg7d->adc_cal: ,R,0x7d[22:16]*/
static inline uint32_t aon_pmu_ll_get_reg7d_adc_cal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.adc_cal;
}

/* REG_0x7d:reg7d->bgcal: ,R,0x7d[38:23]*/
static inline uint32_t aon_pmu_ll_get_reg7d_bgcal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.bgcal;
}

/* REG_0x7d:reg7d->ltemp: ,R,0x7d[29]*/
static inline uint32_t aon_pmu_ll_get_reg7d_ltemp(aon_pmu_hw_t *hw)
{
    return hw->reg7d.ltemp;
}

/* REG_0x7d:reg7d->htemp: ,R,0x7d[30]*/
static inline uint32_t aon_pmu_ll_get_reg7d_htemp(aon_pmu_hw_t *hw)
{
    return hw->reg7d.htemp;
}

/* REG_0x7d:reg7d->26Mpll_unlock: ,R,0x7d[31]*/
static inline uint32_t aon_pmu_ll_get_reg7d_26mpll_unlock(aon_pmu_hw_t *hw)
{
    return hw->reg7d.mpll_unlock;
}

/* REG_0x7e */

static inline uint32_t aon_pmu_ll_get_reg7e_value(aon_pmu_hw_t *hw)
{
    return hw->reg7e.v;
}

static inline void aon_pmu_ll_set_reg7e_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg7e.v = value;
}

/* REG_0x7e:reg7e->capcal: ,R/W,0x7e[8:0]*/
static inline uint32_t aon_pmu_ll_get_reg7e_capcal(aon_pmu_hw_t *hw)
{
    return hw->reg7e.capcal;
}

static inline void aon_pmu_ll_set_reg7e_capcal(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg7e.capcal = value;
}

/* REG_0x7e:reg7e->H: ,R,0x7e[31:9]*/
static inline uint32_t aon_pmu_ll_get_reg7e_h(aon_pmu_hw_t *hw)
{
    return hw->reg7e.h;
}

/* REG_0x7f */

/* REG_0x7f */

static inline uint32_t aon_pmu_ll_get_reg7f_value(aon_pmu_hw_t *hw)
{
    return hw->reg7f.v;
}

/* REG_0x7f:reg7f->H: ,R,0x7f[31:0]*/
static inline uint32_t aon_pmu_ll_get_reg7f_h(aon_pmu_hw_t *hw)
{
    return hw->reg7f.v;
}

#ifdef __cplusplus 
}                  
#endif             
