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

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-01-17 15:35:03                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#include <soc/soc.h>          
#include "aon_pmu_hw.h"        
#include "aon_pmu_ll_macro_def.h"         

#ifdef __cplusplus          
extern "C" {              
#endif                      

#if 0

/* REG_0x00 */

static inline uint32_t aon_pmu_ll_get_reg0_value(aon_pmu_hw_t *hw)
{
    return hw->reg0.v;
}

static inline void aon_pmu_ll_set_reg0_value(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg0.v = value;
}

/* REG_0x00:reg0->memchk_bps:0x0[0],memcheck bypass,1,R/W*/
static inline uint32_t aon_pmu_ll_get_reg0_memchk_bps(aon_pmu_hw_t *hw)
{
    return hw->reg0.memchk_bps;
}

static inline void aon_pmu_ll_set_reg0_memchk_bps(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg0.memchk_bps = value;
}

/* REG_0x01 */

static inline uint32_t aon_pmu_ll_get_reg1_value(aon_pmu_hw_t *hw)
{
    return hw->reg1.v;
}

/* REG_0x01:reg1->touch_select:0x1[3:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg1_touch_select(aon_pmu_hw_t *hw)
{
    return hw->reg1.touch_select;
}

/* REG_0x01:reg1->touch_int_en:0x1[19:4], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg1_touch_int_en(aon_pmu_hw_t *hw)
{
    return hw->reg1.touch_int_en;
}

static inline void aon_pmu_ll_set_reg1_touch_int_en(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg1.touch_int_en = value;
}

/* REG_0x01:reg1->usbplug_int_en:0x1[20], ,None,R/W*/
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

/* REG_0x02:reg2->wdt_rst_ana:0x2[0],wdt rst of ana,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_ana(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_ana;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_ana(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_ana = value;
}

/* REG_0x02:reg2->wdt_rst_top:0x2[1],wdt rst of top,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_top(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_top;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_top(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_top = value;
}

/* REG_0x02:reg2->wdt_rst_aon:0x2[2],wdt rst of aon,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_aon(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_aon;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_aon(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_aon = value;
}

/* REG_0x02:reg2->wdt_rst_awt:0x2[3],wdt rst of awt,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_awt(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_awt;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_awt(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_awt = value;
}

/* REG_0x02:reg2->wdt_rst_gpio:0x2[4],wdt rst of gpio,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_gpio(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_gpio;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_gpio(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_gpio = value;
}

/* REG_0x02:reg2->wdt_rst_rtc:0x2[5],wdt rst of rtc,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_rtc(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_rtc;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_rtc(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_rtc = value;
}

/* REG_0x02:reg2->wdt_rst_wdt:0x2[6],wdt rst of wdt,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_wdt(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_wdt;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_wdt(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_wdt = value;
}

/* REG_0x02:reg2->wdt_rst_pmu:0x2[7],wdt rst of pmu,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_pmu(aon_pmu_hw_t *hw)
{
    return hw->reg2.wdt_rst_pmu;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_pmu(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg2.wdt_rst_pmu = value;
}

/* REG_0x02:reg2->wdt_rst_phy:0x2[8],wdt rst of phy,None,R/W*/
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

/* REG_0x03:reg3->aon_reg3:0x3[31:0],aon_reg3,None,R/W*/
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

/* REG_0x40:reg40->wake1_delay:0x40[3:0],wake delay of Xtal,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_wake1_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.wake1_delay;
}

static inline void aon_pmu_ll_set_reg40_wake1_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.wake1_delay = value;
}

/* REG_0x40:reg40->wake2_delay:0x40[7:4],wake delay of Voltage,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_wake2_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.wake2_delay;
}

static inline void aon_pmu_ll_set_reg40_wake2_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.wake2_delay = value;
}

/* REG_0x40:reg40->wake3_delay:0x40[11:8], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_wake3_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.wake3_delay;
}

static inline void aon_pmu_ll_set_reg40_wake3_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.wake3_delay = value;
}

/* REG_0x40:reg40->halt1_delay:0x40[15:12],halt delay of Voltage ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt1_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt1_delay;
}

static inline void aon_pmu_ll_set_reg40_halt1_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt1_delay = value;
}

/* REG_0x40:reg40->halt2_delay:0x40[19:16],halt delay of Core and flash ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt2_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt2_delay;
}

static inline void aon_pmu_ll_set_reg40_halt2_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt2_delay = value;
}

/* REG_0x40:reg40->halt3_delay:0x40[23:20],halt delay of Xtal,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt3_delay(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt3_delay;
}

static inline void aon_pmu_ll_set_reg40_halt3_delay(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt3_delay = value;
}

/* REG_0x40:reg40->halt_volt:0x40[24],1:volt   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_volt(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_volt;
}

static inline void aon_pmu_ll_set_reg40_halt_volt(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_volt = value;
}

/* REG_0x40:reg40->halt_xtal:0x40[25],1:xtal   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_xtal(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_xtal;
}

static inline void aon_pmu_ll_set_reg40_halt_xtal(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_xtal = value;
}

/* REG_0x40:reg40->halt_core:0x40[26],1:core   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_core(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_core;
}

static inline void aon_pmu_ll_set_reg40_halt_core(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_core = value;
}

/* REG_0x40:reg40->halt_flash:0x40[27],1:flash  halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_flash(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_flash;
}

static inline void aon_pmu_ll_set_reg40_halt_flash(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_flash = value;
}

/* REG_0x40:reg40->halt_rosc:0x40[28],1:rosc   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_rosc(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_rosc;
}

static inline void aon_pmu_ll_set_reg40_halt_rosc(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_rosc = value;
}

/* REG_0x40:reg40->halt_resten:0x40[29],1:resten halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_resten(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_resten;
}

static inline void aon_pmu_ll_set_reg40_halt_resten(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_resten = value;
}

/* REG_0x40:reg40->halt_isolat:0x40[30],1:isolat halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_isolat(aon_pmu_hw_t *hw)
{
    return hw->reg40.halt_isolat;
}

static inline void aon_pmu_ll_set_reg40_halt_isolat(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg40.halt_isolat = value;
}

/* REG_0x40:reg40->halt_clkena:0x40[31],0:reg config clk halt ,None,R/W*/
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

/* REG_0x41:reg41->lpo_config:0x41[1:0],lpo_src : 1x : clk_ROSC ; 1:clk_X32k ;     0:clk_DIVD,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg41_lpo_config(aon_pmu_hw_t *hw)
{
    return hw->reg41.lpo_config;
}

static inline void aon_pmu_ll_set_reg41_lpo_config(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg41.lpo_config = value;
}

/* REG_0x41:reg41->flshsck_iocap:0x41[3:2],spiflash sck driver selection,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg41_flshsck_iocap(aon_pmu_hw_t *hw)
{
    return hw->reg41.flshsck_iocap;
}

static inline void aon_pmu_ll_set_reg41_flshsck_iocap(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg41.flshsck_iocap = value;
}

/* REG_0x41:reg41->wakeup_ena:0x41[8:4],[8]:int_touched                                                                 [7]:int_usbplug                                                                    [6]:system_wake                                                                   [5]:int_rtc                                                                                 [4]:int_gpio,None,R/W*/
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

/* REG_0x42:reg42->reg42:0x42[31:0], ,None,R/W*/
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

/* REG_0x43:reg43->clr_int_touched:0x43[15:0],0:touch reset enable,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg43_clr_int_touched(aon_pmu_hw_t *hw)
{
    return hw->reg43.clr_int_touched;
}

static inline void aon_pmu_ll_set_reg43_clr_int_touched(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg43.clr_int_touched = value;
}

/* REG_0x43:reg43->clr_int_usbplug:0x43[16],0:usb reset enable,None,R/W*/
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

/* REG_0x70:reg70->int_touched:0x70[15:0],touch int state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg70_int_touched(aon_pmu_hw_t *hw)
{
    return hw->reg70.int_touched;
}

/* REG_0x70:reg70->int_usbplug:0x70[16],usb int state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg70_int_usbplug(aon_pmu_hw_t *hw)
{
    return hw->reg70.int_usbplug;
}

/* REG_0x71 */

static inline uint32_t aon_pmu_ll_get_reg71_value(aon_pmu_hw_t *hw)
{
    return hw->reg71.v;
}

/* REG_0x71:reg71->touch_state:0x71[15:0],touch_state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg71_touch_state(aon_pmu_hw_t *hw)
{
    return hw->reg71.touch_state;
}

/* REG_0x71:reg71->usbplug_state:0x71[16],usbplug_state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg71_usbplug_state(aon_pmu_hw_t *hw)
{
    return hw->reg71.usbplug_state;
}

/* REG_0x71:reg71->wakeup_source:0x71[24:20],wakeup_source,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg71_wakeup_source(aon_pmu_hw_t *hw)
{
    return hw->reg71.wakeup_source;
}

/* REG_0x7c */

static inline uint32_t aon_pmu_ll_get_reg7c_value(aon_pmu_hw_t *hw)
{
    return hw->reg7c.v;
}

/* REG_0x7c:reg7c->ID:0x7c[31:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7c_id(aon_pmu_hw_t *hw)
{
    return hw->reg7c.v;
}

/* REG_0x7d */

static inline uint32_t aon_pmu_ll_get_reg7d_value(aon_pmu_hw_t *hw)
{
    return hw->reg7d.v;
}

/* REG_0x7d:reg7d->lcal:0x7d[7:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_lcal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.lcal;
}

/* REG_0x7d:reg7d->rcal:0x7d[15:8], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_rcal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.rcal;
}

/* REG_0x7d:reg7d->adc_cal:0x7d[22:16], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_adc_cal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.adc_cal;
}

/* REG_0x7d:reg7d->bgcal:0x7d[38:23], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_bgcal(aon_pmu_hw_t *hw)
{
    return hw->reg7d.bgcal;
}

/* REG_0x7d:reg7d->ltemp:0x7d[29], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_ltemp(aon_pmu_hw_t *hw)
{
    return hw->reg7d.ltemp;
}

/* REG_0x7d:reg7d->htemp:0x7d[30], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_htemp(aon_pmu_hw_t *hw)
{
    return hw->reg7d.htemp;
}

/* REG_0x7d:reg7d->pll26M_unlock:0x7d[31], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_pll26m_unlock(aon_pmu_hw_t *hw)
{
    return hw->reg7d.pll26m_unlock;
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

/* REG_0x7e:reg7e->capcal:0x7e[8:0], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg7e_capcal(aon_pmu_hw_t *hw)
{
    return hw->reg7e.capcal;
}

static inline void aon_pmu_ll_set_reg7e_capcal(aon_pmu_hw_t *hw, uint32_t value)
{
    hw->reg7e.capcal = value;
}

/* REG_0x7e:reg7e->H:0x7e[31:9], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7e_h(aon_pmu_hw_t *hw)
{
    return hw->reg7e.h;
}

/* REG_0x7f */

static inline uint32_t aon_pmu_ll_get_reg7f_value(aon_pmu_hw_t *hw)
{
    return hw->reg7f.v;
}

/* REG_0x7f:reg7f->H:0x7f[31:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7f_h(aon_pmu_hw_t *hw)
{
    return hw->reg7f.v;
}

#endif 

#ifdef __cplusplus 
}                  
#endif             
