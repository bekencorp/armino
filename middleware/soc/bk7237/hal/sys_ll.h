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
* CHIP ID:BK7256,GENARATE TIME:2022-03-17 20:29:39                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#include <soc/soc.h>          
#include "system_hw.h"        
#include "sys_ll_macro_def.h"         
#include "sys_ll_op_if.h"         

#ifdef __cplusplus          
extern "C" {              
#endif                      

#if 0

/* REG_0x00 */

static inline uint32_t sys_ll_get_device_id_value(sys_hw_t *hw)
{
    return hw->device_id.v;
}

/* REG_0x00:device_id->DeviceID:0x0[31:0], ,0x53434647,RO*/
static inline uint32_t sys_ll_get_device_id_deviceid(sys_hw_t *hw)
{
    return hw->device_id.v;
}

/* REG_0x01 */

static inline uint32_t sys_ll_get_version_id_value(sys_hw_t *hw)
{
    return hw->version_id.v;
}

/* REG_0x01:version_id->VersionID:0x1[31:0], ,0x72560001,RO*/
static inline uint32_t sys_ll_get_version_id_versionid(sys_hw_t *hw)
{
    return hw->version_id.v;
}

/* REG_0x02 */

static inline uint32_t sys_ll_get_cpu_current_run_status_value(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.v;
}

/* REG_0x02:cpu_current_run_status->core0_halted:0x2[0],core0 halt indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_core0_halted(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.core0_halted;
}

/* REG_0x02:cpu_current_run_status->core1_halted:0x2[1],core1 halt indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_core1_halted(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.core1_halted;
}

/* REG_0x02:cpu_current_run_status->cpu0_sw_reset:0x2[4],cpu0_sw_reset indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_sw_reset(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.cpu0_sw_reset;
}

/* REG_0x02:cpu_current_run_status->cpu1_sw_reset:0x2[5],cpu1_sw_reset indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu1_sw_reset(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.cpu1_sw_reset;
}

/* REG_0x02:cpu_current_run_status->cpu0_pwr_dw_state:0x2[8],cpu0_pwr_dw_state,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.cpu0_pwr_dw_state;
}

/* REG_0x02:cpu_current_run_status->cpu1_pwr_dw_state:0x2[9],cpu1_pwr_dw_state,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state(sys_hw_t *hw)
{
    return hw->cpu_current_run_status.cpu1_pwr_dw_state;
}

/* REG_0x03 */

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_value(sys_hw_t *hw)
{
    return hw->cpu_storage_connect_op_select.v;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_storage_connect_op_select.v = value;
}

/* REG_0x03:cpu_storage_connect_op_select->boot_mode:0x3[0],0:ROM boot 1:FLASH boot,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_boot_mode(sys_hw_t *hw)
{
    return hw->cpu_storage_connect_op_select.boot_mode;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_boot_mode(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_storage_connect_op_select.boot_mode = value;
}

/* REG_0x03:cpu_storage_connect_op_select->rf_switch_en:0x3[4],0: rf switch by PTA; 1: rf switch by SW,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_rf_switch_en(sys_hw_t *hw)
{
    return hw->cpu_storage_connect_op_select.rf_switch_en;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_rf_switch_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_storage_connect_op_select.rf_switch_en = value;
}

/* REG_0x03:cpu_storage_connect_op_select->rf_for_wifiorbt:0x3[5],0: rf for wifi;  1: rf for bt,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_rf_for_wifiorbt(sys_hw_t *hw)
{
    return hw->cpu_storage_connect_op_select.rf_for_wifiorbt;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_rf_for_wifiorbt(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_storage_connect_op_select.rf_for_wifiorbt = value;
}

/* REG_0x03:cpu_storage_connect_op_select->jtag_core_sel:0x3[8],0:jtag connect core0, 1:jtag connect core1,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel(sys_hw_t *hw)
{
    return hw->cpu_storage_connect_op_select.jtag_core_sel;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_storage_connect_op_select.jtag_core_sel = value;
}

/* REG_0x03:cpu_storage_connect_op_select->flash_sel:0x3[9],0: normal flash operation 1:flash download by spi,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_flash_sel(sys_hw_t *hw)
{
    return hw->cpu_storage_connect_op_select.flash_sel;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_flash_sel(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_storage_connect_op_select.flash_sel = value;
}

/* REG_0x04 */

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_value(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.v;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_halt_clk_op.v = value;
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_sw_rst:0x4[0],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.cpu0_sw_rst;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_halt_clk_op.cpu0_sw_rst = value;
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_pwr_dw:0x4[1],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.cpu0_pwr_dw;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_halt_clk_op.cpu0_pwr_dw = value;
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_int_mask:0x4[2],cpu0 int mask,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.cpu0_int_mask;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_halt_clk_op.cpu0_int_mask = value;
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_halt:0x4[3],core0 halt indicate,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.cpu0_halt;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_halt_clk_op.cpu0_halt = value;
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_clk_div:0x4[7:4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_clk_div(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.cpu0_clk_div;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_clk_div(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_halt_clk_op.cpu0_clk_div = value;
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_offset:0x4[31:8],reserved,0x0,RO*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(sys_hw_t *hw)
{
    return hw->cpu0_int_halt_clk_op.cpu0_offset;
}

/* REG_0x05 */

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_value(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.v;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.v = value;
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_sw_rst:0x5[0],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_sw_rst(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.cpu1_sw_rst;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.cpu1_sw_rst = value;
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_pwr_dw:0x5[1],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.cpu1_pwr_dw;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.cpu1_pwr_dw = value;
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_int_mask:0x5[2],cpu1 int mask,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_int_mask(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.cpu1_int_mask;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.cpu1_int_mask = value;
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_halt:0x5[3],core1 halt indicate,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_halt(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.cpu1_halt;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.cpu1_halt = value;
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_clk_div:0x5[7:4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_clk_div(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.cpu1_clk_div;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_clk_div(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.cpu1_clk_div = value;
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_offset:0x5[31:8],reserved,0x0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_offset(sys_hw_t *hw)
{
    return hw->cpu1_int_halt_clk_op.cpu1_offset;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_halt_clk_op.cpu1_offset = value;
}

/* REG_0x06 */

/* REG_0x08 */

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_value(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.v;
}

static inline void sys_ll_set_cpu_clk_div_mode1_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.v = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_core:0x8[3:0],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_core(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_core;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_core(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_core = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_core:0x8[5:4],0:XTAL       1 : clk_DCO      2 : 320M      3 : 480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_core(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_core;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_core(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_core = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_bus:0x8[6],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_bus(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_bus;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_bus = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart0:0x8[9:8],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_uart0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_uart0 = value;
}

/* REG_0x08:cpu_clk_div_mode1->clksel_uart0:0x8[10],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clksel_uart0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clksel_uart0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clksel_uart0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clksel_uart0 = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart1:0x8[12:11],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart1(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_uart1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart1(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_uart1 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_uart1:0x8[13],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart1(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_uart1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart1(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_uart1 = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart2:0x8[15:14],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart2(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_uart2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart2(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_uart2 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_uart2:0x8[16],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart2(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_uart2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart2(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_uart2 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_sadc:0x8[17],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_sadc(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_sadc;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_sadc(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_sadc = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm0:0x8[18],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_pwm0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_pwm0 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm1:0x8[19],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm1(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_pwm1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_pwm1 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer0:0x8[20],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_timer0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_timer0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_timer0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_timer0 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer1:0x8[21],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_timer1(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_timer1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_timer1(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_timer1 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer2:0x8[22],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_timer2(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_timer2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_timer2(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_timer2 = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_can:0x8[23],0:XTAL              1:80M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_can(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_can;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_can(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_can = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_i2s:0x8[24],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2s(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_i2s;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_i2s(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_i2s = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_aud:0x8[25],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_aud(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_aud;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_aud(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_aud = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_jpeg:0x8[29:26],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_jpeg(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_jpeg;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_jpeg = value;
}

/* REG_0x08:cpu_clk_div_mode1->cksel_jpeg:0x8[30],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_jpeg(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.cksel_jpeg;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.cksel_jpeg = value;
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_disp_l:0x8[31],Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode1.clkdiv_disp_l;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode1.clkdiv_disp_l = value;
}

/* REG_0x09 */

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_value(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.v;
}

static inline void sys_ll_set_cpu_clk_div_mode2_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.v = value;
}

/* REG_0x09:cpu_clk_div_mode2->clkdiv_disp_h:0x9[2:0],Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.clkdiv_disp_h;
}

static inline void sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.clkdiv_disp_h = value;
}

/* REG_0x09:cpu_clk_div_mode2->cksel_disp:0x9[3],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_disp(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.cksel_disp;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_disp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.cksel_disp = value;
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_psram:0x9[4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.ckdiv_psram;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.ckdiv_psram = value;
}

/* REG_0x09:cpu_clk_div_mode2->cksel_psram:0x9[5],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_psram(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.cksel_psram;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_psram(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.cksel_psram = value;
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_qspi0:0x9[9:6],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_qspi0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.ckdiv_qspi0;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.ckdiv_qspi0 = value;
}

/* REG_0x09:cpu_clk_div_mode2->cksel_qspi0:0x9[10],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_qspi0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.cksel_qspi0;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_qspi0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.cksel_qspi0 = value;
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_sdio:0x9[16:14],0:/2  1:/4  2:/6  3:/8  4:/10  5:/12  6:/16  7:/256,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.ckdiv_sdio;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.ckdiv_sdio = value;
}

/* REG_0x09:cpu_clk_div_mode2->cksel_sdio:0x9[17],0：XTAL          1：320M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_sdio(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.cksel_sdio;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_sdio(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.cksel_sdio = value;
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_auxs:0x9[21:18],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.ckdiv_auxs;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.ckdiv_auxs = value;
}

/* REG_0x09:cpu_clk_div_mode2->cksel_auxs:0x9[23:22],0:DCO              1:APLL                2:320M                     4:480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_auxs(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.cksel_auxs;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_auxs(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.cksel_auxs = value;
}

/* REG_0x09:cpu_clk_div_mode2->cksel_flash:0x9[25:24],0:XTAL              1:APLL               1x :clk_120M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_flash(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.cksel_flash;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_flash(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.cksel_flash = value;
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_flash:0x9[27:26],0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.ckdiv_flash;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.ckdiv_flash = value;
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_i2s0:0x9[30:28],0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_i2s0(sys_hw_t *hw)
{
    return hw->cpu_clk_div_mode2.ckdiv_i2s0;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_i2s0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_clk_div_mode2.ckdiv_i2s0 = value;
}

/* REG_0x0a */

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_value(sys_hw_t *hw)
{
    return hw->cpu_26m_wdt_clk_div.v;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_26m_wdt_clk_div.v = value;
}

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_26m:0xa[1:0],0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_26m(sys_hw_t *hw)
{
    return hw->cpu_26m_wdt_clk_div.ckdiv_26m;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_26m(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_26m_wdt_clk_div.ckdiv_26m = value;
}

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_wdt:0xa[3:2],0:/2 1:/4 2:/8 3:/16,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(sys_hw_t *hw)
{
    return hw->cpu_26m_wdt_clk_div.ckdiv_wdt;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_26m_wdt_clk_div.ckdiv_wdt = value;
}

/* REG_0x0a:cpu_26m_wdt_clk_div->clksel_spi0:0xa[4],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi0(sys_hw_t *hw)
{
    return hw->cpu_26m_wdt_clk_div.clksel_spi0;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_26m_wdt_clk_div.clksel_spi0 = value;
}

/* REG_0x0a:cpu_26m_wdt_clk_div->clksel_spi1:0xa[5],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi1(sys_hw_t *hw)
{
    return hw->cpu_26m_wdt_clk_div.clksel_spi1;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_26m_wdt_clk_div.clksel_spi1 = value;
}

/* REG_0x0b */

static inline uint32_t sys_ll_get_cpu_anaspi_freq_value(sys_hw_t *hw)
{
    return hw->cpu_anaspi_freq.v;
}

static inline void sys_ll_set_cpu_anaspi_freq_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_anaspi_freq.v = value;
}

/* REG_0x0b:cpu_anaspi_freq->anaspi_freq:0xb[5:0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_anaspi_freq_anaspi_freq(sys_hw_t *hw)
{
    return hw->cpu_anaspi_freq.anaspi_freq;
}

static inline void sys_ll_set_cpu_anaspi_freq_anaspi_freq(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_anaspi_freq.anaspi_freq = value;
}

/* REG_0x0b:cpu_anaspi_freq->anareg_state:0xb[27:8],analog register state:0x0: register write is idle;0x1: register write is busy; ,0,R*/
static inline uint32_t sys_ll_get_cpu_anaspi_freq_anareg_state(sys_hw_t *hw)
{
    return hw->cpu_anaspi_freq.anareg_state;
}

/* REG_0x0c */

static inline uint32_t sys_ll_get_cpu_device_clk_enable_value(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.v;
}

static inline void sys_ll_set_cpu_device_clk_enable_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.v = value;
}

/* REG_0x0c:cpu_device_clk_enable->i2c0_cken:0xc[0],1:i2c0_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c0_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.i2c0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c0_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.i2c0_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->spi0_cken:0xc[1],1:spi0_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi0_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.spi0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi0_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.spi0_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->uart0_cken:0xc[2],1:uart0_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart0_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.uart0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart0_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.uart0_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->pwm0_cken:0xc[3],1:pwm0_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm0_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.pwm0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm0_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.pwm0_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->tim0_cken:0xc[4],1:tim0_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim0_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.tim0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim0_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.tim0_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->sadc_cken:0xc[5],1:sadc_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_sadc_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.sadc_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_sadc_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.sadc_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->irda_cken:0xc[6],1:irda_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_irda_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.irda_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_irda_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.irda_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->efuse_cken:0xc[7],1:efuse_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_efuse_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.efuse_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_efuse_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.efuse_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->i2c1_cken:0xc[8],1:i2c1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c1_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.i2c1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c1_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.i2c1_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->spi1_cken:0xc[9],1:spi1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi1_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.spi1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi1_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.spi1_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->uart1_cken:0xc[10],1:uart1_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart1_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.uart1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart1_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.uart1_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->uart2_cken:0xc[11],1:uart2_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart2_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.uart2_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart2_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.uart2_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->pwm1_cken:0xc[12],1:pwm1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm1_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.pwm1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm1_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.pwm1_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->tim1_cken:0xc[13],1:tim1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim1_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.tim1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim1_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.tim1_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->tim2_cken:0xc[14],1:tim2_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim2_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.tim2_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim2_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.tim2_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->otp_cken:0xc[15],1:otp_clk enable  ,1,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_otp_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.otp_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_otp_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.otp_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->i2s_cken:0xc[16],1:i2s_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2s_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.i2s_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2s_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.i2s_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->usb_cken:0xc[17],1:usb_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_usb_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.usb_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_usb_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.usb_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->can_cken:0xc[18],1:can_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_can_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.can_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_can_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.can_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->psram_cken:0xc[19],1:psram_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_psram_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.psram_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_psram_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.psram_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->qspi0_cken:0xc[20],1:qspi0_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi0_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.qspi0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi0_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.qspi0_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->qspi1_cken:0xc[21],1:qspi1_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi1_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.qspi1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi1_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.qspi1_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->sdio_cken:0xc[22],1:sdio_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_sdio_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.sdio_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_sdio_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.sdio_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->auxs_cken:0xc[23],1:auxs_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_auxs_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.auxs_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_auxs_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.auxs_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->btdm_cken:0xc[24],1:btdm_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_btdm_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.btdm_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_btdm_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.btdm_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->xvr_cken:0xc[25],1:xvr_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_xvr_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.xvr_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_xvr_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.xvr_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->mac_cken:0xc[26],1:mac_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.mac_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_mac_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.mac_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->phy_cken:0xc[27],1:phy_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.phy_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_phy_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.phy_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->jpeg_cken:0xc[28],1:jpeg_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_jpeg_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.jpeg_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_jpeg_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.jpeg_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->disp_cken:0xc[29],1:disp_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_disp_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.disp_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_disp_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.disp_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->aud_cken:0xc[30],1:aud_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_aud_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.aud_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_aud_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.aud_cken = value;
}

/* REG_0x0c:cpu_device_clk_enable->wdt_cken:0xc[31],1:wdt_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_wdt_cken(sys_hw_t *hw)
{
    return hw->cpu_device_clk_enable.wdt_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_wdt_cken(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_device_clk_enable.wdt_cken = value;
}

/* REG_0x0d */

/* REG_0x0e */

static inline uint32_t sys_ll_get_cpu_mode_disckg1_value(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.v;
}

static inline void sys_ll_set_cpu_mode_disckg1_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.v = value;
}

/* REG_0x0e:cpu_mode_disckg1->aon_disckg:0xe[0],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_aon_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.aon_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_aon_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.aon_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->sys_disckg:0xe[1],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sys_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.sys_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_sys_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.sys_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->dma_disckg:0xe[2],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_dma_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.dma_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_dma_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.dma_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->flash_disckg:0xe[3],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_flash_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.flash_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_flash_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.flash_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->wdt_disckg:0xe[4],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_wdt_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.wdt_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_wdt_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.wdt_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->tim_disckg:0xe[5],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_tim_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.tim_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_tim_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.tim_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->urt_disckg:0xe[6],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_urt_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.urt_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_urt_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.urt_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->pwm_disckg:0xe[7],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_pwm_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.pwm_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_pwm_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.pwm_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->i2c_disckg:0xe[8],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_i2c_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.i2c_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_i2c_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.i2c_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->spi_disckg:0xe[9],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_spi_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.spi_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_spi_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.spi_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->sadc_disckg:0xe[10],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sadc_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.sadc_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_sadc_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.sadc_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->efs_disckg:0xe[11],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_efs_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.efs_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_efs_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.efs_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->irda_disckg:0xe[12],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_irda_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.irda_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_irda_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.irda_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->trng_disckg:0xe[13],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_trng_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.trng_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_trng_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.trng_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->sdio_disckg:0xe[14],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sdio_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.sdio_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_sdio_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.sdio_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->LA_disckg:0xe[15],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_la_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.la_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_la_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.la_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->tim1_disckg:0xe[16],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_tim1_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.tim1_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_tim1_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.tim1_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->urt1_disckg:0xe[17],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_urt1_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.urt1_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_urt1_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.urt1_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->urt2_disckg:0xe[18],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_urt2_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.urt2_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_urt2_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.urt2_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->pwm1_disckg:0xe[19],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_pwm1_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.pwm1_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_pwm1_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.pwm1_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->i2c1_disckg:0xe[20],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_i2c1_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.i2c1_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_i2c1_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.i2c1_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->spi1_disckg:0xe[21],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_spi1_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.spi1_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_spi1_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.spi1_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->usb_disckg:0xe[22],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_usb_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.usb_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_usb_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.usb_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->can_disckg:0xe[23],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_can_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.can_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_can_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.can_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->qspi0_disckg:0xe[24],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_qspi0_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.qspi0_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_qspi0_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.qspi0_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->psram_disckg:0xe[25],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_psram_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.psram_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_psram_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.psram_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->fft_disckg:0xe[26],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_fft_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.fft_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_fft_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.fft_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->sbc_disckg:0xe[27],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sbc_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.sbc_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_sbc_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.sbc_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->aud_disckg:0xe[28],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_aud_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.aud_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_aud_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.aud_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->i2s_disckg:0xe[29],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_i2s_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.i2s_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_i2s_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.i2s_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->jpeg_disckg:0xe[30],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_jpeg_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.jpeg_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_jpeg_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.jpeg_disckg = value;
}

/* REG_0x0e:cpu_mode_disckg1->jpeg_dec_disckg:0xe[31],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_jpeg_dec_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg1.jpeg_dec_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg1_jpeg_dec_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg1.jpeg_dec_disckg = value;
}

/* REG_0x0f */

static inline uint32_t sys_ll_get_cpu_mode_disckg2_value(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg2.v;
}

static inline void sys_ll_set_cpu_mode_disckg2_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg2.v = value;
}

/* REG_0x0f:cpu_mode_disckg2->disp_disckg:0xf[0],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_disp_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg2.disp_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg2_disp_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg2.disp_disckg = value;
}

/* REG_0x0f:cpu_mode_disckg2->dma2d_disckg:0xf[1],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_dma2d_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg2.dma2d_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg2_dma2d_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg2.dma2d_disckg = value;
}

/* REG_0x0f:cpu_mode_disckg2->btdm_disckg:0xf[3],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_btdm_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg2.btdm_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg2_btdm_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg2.btdm_disckg = value;
}

/* REG_0x0f:cpu_mode_disckg2->xver_disckg:0xf[4],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_xver_disckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg2.xver_disckg;
}

static inline void sys_ll_set_cpu_mode_disckg2_xver_disckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg2.xver_disckg = value;
}

/* REG_0x0f:cpu_mode_disckg2->btdm_bps_ckg:0xf[8:5],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_btdm_bps_ckg(sys_hw_t *hw)
{
    return hw->cpu_mode_disckg2.btdm_bps_ckg;
}

static inline void sys_ll_set_cpu_mode_disckg2_btdm_bps_ckg(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_mode_disckg2.btdm_bps_ckg = value;
}

/* REG_0x10 */

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_value(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.v;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.v = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem1:0x10[0],0:power on of mem1      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_mem1;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_mem1 = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem2:0x10[1],0:power on of mem2      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_mem2;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_mem2 = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem3:0x10[2],0:power on of mem3      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_mem3;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_mem3 = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_encp:0x10[3],0:power on of encp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_encp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_encp = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_bakp:0x10[4],0:power on of bakp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_bakp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_bakp = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_ahbp:0x10[5],0:power on of ahbp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ahbp(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_ahbp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_ahbp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_ahbp = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_audp:0x10[6],0:power on of audp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_audp(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_audp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_audp = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_vidp:0x10[7],0:power on of vidp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_vidp(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_vidp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_vidp = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_btsp:0x10[8],0:power on of btsp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_btsp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_btsp = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_mac:0x10[9],0:power on of wifp_mac  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_wifp_mac;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_wifp_mac = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_phy:0x10[10],0:power on of wifp_phy  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_wifp_phy;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_wifp_phy = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem0:0x10[11] ,0:power on of mem0,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem0(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.pwd_mem0;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem0(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.pwd_mem0 = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_flash_idle:0x10[16],0:sleep_en of flash_idle,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.sleep_en_need_flash_idle;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.sleep_en_need_flash_idle = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu1_wfi:0x10[17],0:sleep_en of cpu1_wfi  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.sleep_en_need_cpu1_wfi;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.sleep_en_need_cpu1_wfi = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu0_wfi:0x10[18],0:sleep_en of cpu0_wfi  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.sleep_en_need_cpu0_wfi;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.sleep_en_need_cpu0_wfi = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_global:0x10[19],0:sleep_en of global    ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.sleep_en_global;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.sleep_en_global = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->wifi_wakeup_platform_en:0x10[20],0:wifi_wakeup_en        ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.wifi_wakeup_platform_en;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.wifi_wakeup_platform_en = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->bts_wakeup_platform_en:0x10[21],0:bts_wakeup_en         ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.bts_wakeup_platform_en;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.bts_wakeup_platform_en = value;
}

/* REG_0x10:cpu_power_sleep_wakeup->bts_sleep_exit_req:0x10[22],0:bt sleep exit request ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_sleep_exit_req(sys_hw_t *hw)
{
    return hw->cpu_power_sleep_wakeup.bts_sleep_exit_req;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(sys_hw_t *hw, uint32_t value)
{
    hw->cpu_power_sleep_wakeup.bts_sleep_exit_req = value;
}

/* REG_0x11 */

/* REG_0x20 */

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_value(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.v;
}

static inline void sys_ll_set_cpu0_int_0_31_en_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.v = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_bmc32_int_en:0x20[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_bmc32_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_bmc32_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_bmc32_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_bmc32_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_irq_en:0x20[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_host_0_irq_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_host_0_irq_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_host_0_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_host_0_irq_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_sec_irq_en:0x20[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_host_0_sec_irq_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_host_0_sec_irq_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer_int_en:0x20[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_timer_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_timer_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart_int_en:0x20[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_uart_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_uart_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_pwm_int_en:0x20[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_pwm_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_pwm_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_pwm_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_pwm_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c_int_en:0x20[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_i2c_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_i2c_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi_int_en:0x20[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_spi_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_spi_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sadc_int_en:0x20[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sadc_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_sadc_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sadc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_sadc_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_irda_int_en:0x20[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_irda_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_irda_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_irda_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_irda_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sdio_int_en:0x20[10], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_sdio_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_sdio_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_gdma_int_en:0x20[11], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_gdma_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_gdma_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_la_int_en:0x20[12], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_la_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_la_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer1_int_en:0x20[13], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_timer1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_timer1_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c1_int_en:0x20[14], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_i2c1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_i2c1_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart1_int_en:0x20[15], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_uart1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_uart1_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart2_int_en:0x20[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_uart2_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_uart2_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi1_int_en:0x20[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_spi1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_spi1_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_can_int_en:0x20[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_can_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_can_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_can_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_can_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_usb_int_en:0x20[19], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_usb_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_usb_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_usb_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_usb_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_qspi_int_en:0x20[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_qspi_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_qspi_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_qspi_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_qspi_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_fft_int_en:0x20[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_fft_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_fft_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_fft_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sbc_int_en:0x20[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sbc_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_sbc_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sbc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_sbc_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_aud_int_en:0x20[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_aud_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_aud_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_aud_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2s_int_en:0x20[24], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2s_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_i2s_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2s_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_i2s_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegenc_int_en:0x20[25], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_jpegenc_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_jpegenc_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegdec_int_en:0x20[26], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_jpegdec_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_jpegdec_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_lcd_int_en:0x20[27], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_lcd_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_lcd_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_dma2d_int_en:0x20[28], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma2d_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_dma2d_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_dma2d_int_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_int_phy_mpb_en:0x20[29], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_wifi_int_phy_mpb_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_wifi_int_phy_mpb_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_int_phy_riu_en:0x20[30], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_wifi_int_phy_riu_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_wifi_int_phy_riu_en = value;
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_mac_int_tx_rx_timer_en:0x20[31], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_en.cpu0_wifi_mac_int_tx_rx_timer_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_0_31_en.cpu0_wifi_mac_int_tx_rx_timer_en = value;
}

/* REG_0x21 */

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_value(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.v;
}

static inline void sys_ll_set_cpu0_int_32_63_en_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.v = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_tx_rx_misc_en:0x21[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_tx_rx_misc_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_tx_rx_misc_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_rx_trigger_en:0x21[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_rx_trigger_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_rx_trigger_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_tx_trigger_en:0x21[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_tx_trigger_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_tx_trigger_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_prot_trigger_en:0x21[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_prot_trigger_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_prot_trigger_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_gen_en:0x21[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_gen_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_mac_int_gen_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_hsu_irq_en:0x21[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_hsu_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_hsu_irq_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_int_mac_wakeup_en:0x21[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_wifi_int_mac_wakeup_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_wifi_int_mac_wakeup_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_dm_irq_en:0x21[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_dm_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_dm_irq_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_ble_irq_en:0x21[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_ble_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_ble_irq_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_bt_irq_en:0x21[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_bt_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_bt_irq_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox0_int_en:0x21[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_mbox0_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_mbox0_int_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox1_int_en:0x21[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_mbox1_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_mbox1_int_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_bmc64_int_en:0x21[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_bmc64_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_bmc64_int_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_touched_int_en:0x21[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_touched_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_touched_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_touched_int_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_usbplug_int_en:0x21[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_usbplug_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_usbplug_int_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_rtc_int_en:0x21[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rtc_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_rtc_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_rtc_int_en = value;
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_gpio_int_en:0x21[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_int_en(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_en.cpu0_gpio_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu0_int_32_63_en.cpu0_gpio_int_en = value;
}

/* REG_0x22 */

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_value(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.v;
}

static inline void sys_ll_set_cpu1_int_0_31_en_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.v = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_bmc32_int_en:0x22[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_bmc32_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_bmc32_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_bmc32_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_bmc32_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_irq_en:0x22[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_host_0_irq_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_host_0_irq_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_host_0_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_host_0_irq_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_sec_irq_en:0x22[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_host_0_sec_irq_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_host_0_sec_irq_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer_int_en:0x22[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_timer_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_timer_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_timer_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart_int_en:0x22[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_uart_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_uart_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_pwm_int_en:0x22[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_pwm_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_pwm_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_pwm_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_pwm_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c_int_en:0x22[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_i2c_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_i2c_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi_int_en:0x22[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_spi_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_spi_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_spi_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sadc_int_en:0x22[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sadc_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_sadc_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sadc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_sadc_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_irda_int_en:0x22[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_irda_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_irda_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_irda_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_irda_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sdio_int_en:0x22[10], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sdio_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_sdio_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_sdio_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_gdma_int_en:0x22[11], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_gdma_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_gdma_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_gdma_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_gdma_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_la_int_en:0x22[12], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_la_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_la_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_la_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_la_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer1_int_en:0x22[13], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer1_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_timer1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_timer1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_timer1_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c1_int_en:0x22[14], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_i2c1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_i2c1_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart1_int_en:0x22[15], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart1_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_uart1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_uart1_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart2_int_en:0x22[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart2_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_uart2_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart2_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_uart2_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi1_int_en:0x22[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi1_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_spi1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_spi1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_spi1_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_can_int_en:0x22[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_can_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_can_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_can_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_can_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_usb_int_en:0x22[19], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_usb_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_usb_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_usb_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_usb_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_qspi_int_en:0x22[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_qspi_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_qspi_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_qspi_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_qspi_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_fft_int_en:0x22[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_fft_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_fft_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_fft_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_fft_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sbc_int_en:0x22[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sbc_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_sbc_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sbc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_sbc_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_aud_int_en:0x22[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_aud_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_aud_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_aud_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_aud_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2s_int_en:0x22[24], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2s_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_i2s_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2s_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_i2s_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegenc_int_en:0x22[25], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_jpegenc_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_jpegenc_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegdec_int_en:0x22[26], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_jpegdec_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_jpegdec_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_lcd_int_en:0x22[27], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_lcd_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_lcd_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_lcd_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_dma2d_int_en:0x22[28], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma2d_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_dma2d_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_dma2d_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_dma2d_int_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_int_phy_mpb_en:0x22[29], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_wifi_int_phy_mpb_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_wifi_int_phy_mpb_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_int_phy_riu_en:0x22[30], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_wifi_int_phy_riu_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_wifi_int_phy_riu_en = value;
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_mac_int_tx_rx_timer_en:0x22[31], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_en.cpu1_wifi_mac_int_tx_rx_timer_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_0_31_en.cpu1_wifi_mac_int_tx_rx_timer_en = value;
}

/* REG_0x23 */

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_value(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.v;
}

static inline void sys_ll_set_cpu1_int_32_63_en_value(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.v = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_tx_rx_misc_en:0x23[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_tx_rx_misc_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_tx_rx_misc_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_rx_trigger_en:0x23[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_rx_trigger_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_rx_trigger_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_tx_trigger_en:0x23[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_tx_trigger_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_tx_trigger_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_prot_trigger_en:0x23[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_prot_trigger_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_prot_trigger_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_gen_en:0x23[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_gen_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_mac_int_gen_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_hsu_irq_en:0x23[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_hsu_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_hsu_irq_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_int_mac_wakeup_en:0x23[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_wifi_int_mac_wakeup_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_wifi_int_mac_wakeup_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_dm_irq_en:0x23[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dm_irq_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_dm_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_dm_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_dm_irq_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_ble_irq_en:0x23[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_ble_irq_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_ble_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_ble_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_ble_irq_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_bt_irq_en:0x23[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bt_irq_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_bt_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_bt_irq_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_bt_irq_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox0_int_en:0x23[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_mbox0_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_mbox0_int_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox1_int_en:0x23[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_mbox1_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_mbox1_int_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_bmc64_int_en:0x23[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_bmc64_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_bmc64_int_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_touched_int_en:0x23[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_touched_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_touched_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_touched_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_touched_int_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_usbplug_int_en:0x23[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_usbplug_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_usbplug_int_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_rtc_int_en:0x23[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rtc_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_rtc_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_rtc_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_rtc_int_en = value;
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_gpio_int_en:0x23[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_gpio_int_en(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_en.cpu1_gpio_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_gpio_int_en(sys_hw_t *hw, uint32_t value)
{
    hw->cpu1_int_32_63_en.cpu1_gpio_int_en = value;
}

/* REG_0x28 */

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_value(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.v;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_bmc32_int_st:0x28[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_bmc32_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_bmc32_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_irq_st:0x28[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_host_0_irq_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_host_0_irq_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_sec_irq_st:0x28[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_host_0_sec_irq_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_host_0_sec_irq_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer_int_st:0x28[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_timer_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart_int_st:0x28[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_uart_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_pwm_int_st:0x28[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_pwm_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_pwm_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c_int_st:0x28[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_i2c_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi_int_st:0x28[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_spi_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sadc_int_st:0x28[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_sadc_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_irda_int_st:0x28[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_irda_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_irda_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sdio_int_st:0x28[10], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_sdio_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_gdma_int_st:0x28[11], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_gdma_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_la_int_st:0x28[12], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_la_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer1_int_st:0x28[13], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_timer1_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c1_int_st:0x28[14], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_i2c1_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart1_int_st:0x28[15], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_uart1_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart2_int_st:0x28[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_uart2_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi1_int_st:0x28[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_spi1_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_can_int_st:0x28[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_can_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_can_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_usb_int_st:0x28[19], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_usb_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_usb_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_qspi_int_st:0x28[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_qspi_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_qspi_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_fft_int_st:0x28[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_fft_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_fft_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sbc_int_st:0x28[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sbc_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_sbc_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_aud_int_st:0x28[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_aud_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_aud_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2s_int_st:0x28[24], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2s_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_i2s_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegenc_int_st:0x28[25], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_jpegenc_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegdec_int_st:0x28[26], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_jpegdec_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_lcd_int_st:0x28[27], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_lcd_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_lcd_int_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_int_phy_mpb_st:0x28[29], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_mpb_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_wifi_int_phy_mpb_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_int_phy_riu_st:0x28[30], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_riu_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_wifi_int_phy_riu_st;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_mac_int_tx_rx_timer_st:0x28[31], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(sys_hw_t *hw)
{
    return hw->cpu0_int_0_31_status.cpu0_wifi_mac_int_tx_rx_timer_st;
}

/* REG_0x29 */

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_value(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.v;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_tx_rx_misc_st:0x29[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_rx_misc_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_tx_rx_misc_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_rx_trigger_st:0x29[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_rx_trigger_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_tx_trigger_st:0x29[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_tx_trigger_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_prot_trigger_st:0x29[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_prot_trigger_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_prot_trigger_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_gen_st:0x29[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_mac_int_gen_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_hsu_irq_st:0x29[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_hsu_irq_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_int_mac_wakeup_st:0x29[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_wifi_int_mac_wakeup_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_dm_irq_st:0x29[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_dm_irq_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_ble_irq_st:0x29[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_ble_irq_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_bt_irq_st:0x29[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_bt_irq_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox0_int_st:0x29[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_mbox0_int_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox1_int_st:0x29[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_mbox1_int_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_bmc64_int_st:0x29[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_bmc64_int_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_touched_int_st:0x29[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_touched_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_touched_int_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_usbplug_int_st:0x29[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_usbplug_int_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_rtc_int_st:0x29[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rtc_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_rtc_int_st;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st:0x29[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st(sys_hw_t *hw)
{
    return hw->cpu0_int_32_63_status.cpu0_gpio_int_st;
}

/* REG_0x2a */

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_value(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.v;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_bmc32_int_st:0x2a[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_bmc32_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_bmc32_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_irq_st:0x2a[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_host_0_irq_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_host_0_irq_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_sec_irq_st:0x2a[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_host_0_sec_irq_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_host_0_sec_irq_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer_int_st:0x2a[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_timer_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart_int_st:0x2a[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_uart_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_pwm_int_st:0x2a[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_pwm_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_pwm_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c_int_st:0x2a[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_i2c_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi_int_st:0x2a[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_spi_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sadc_int_st:0x2a[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sadc_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_sadc_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_irda_int_st:0x2a[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_irda_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_irda_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sdio_int_st:0x2a[10], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sdio_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_sdio_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_gdma_int_st:0x2a[11], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_gdma_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_gdma_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_la_int_st:0x2a[12], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_la_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_la_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer1_int_st:0x2a[13], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer1_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_timer1_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c1_int_st:0x2a[14], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_i2c1_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart1_int_st:0x2a[15], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart1_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_uart1_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart2_int_st:0x2a[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart2_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_uart2_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi1_int_st:0x2a[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi1_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_spi1_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_can_int_st:0x2a[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_can_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_can_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_usb_int_st:0x2a[19], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_usb_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_usb_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_qspi_int_st:0x2a[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_qspi_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_qspi_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_fft_int_st:0x2a[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_fft_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_fft_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sbc_int_st:0x2a[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sbc_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_sbc_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_aud_int_st:0x2a[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_aud_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_aud_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2s_int_st:0x2a[24], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2s_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_i2s_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegenc_int_st:0x2a[25], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_jpegenc_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegdec_int_st:0x2a[26], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_jpegdec_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_lcd_int_st:0x2a[27], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_lcd_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_lcd_int_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_int_phy_mpb_st:0x2a[29], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_mpb_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_wifi_int_phy_mpb_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_int_phy_riu_st:0x2a[30], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_riu_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_wifi_int_phy_riu_st;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_mac_int_tx_rx_timer_st:0x2a[31], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(sys_hw_t *hw)
{
    return hw->cpu1_int_0_31_status.cpu1_wifi_mac_int_tx_rx_timer_st;
}

/* REG_0x2b */

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_value(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.v;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_tx_rx_misc_st:0x2b[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_rx_misc_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_tx_rx_misc_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_rx_trigger_st:0x2b[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_rx_trigger_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_tx_trigger_st:0x2b[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_tx_trigger_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_prot_trigger_st:0x2b[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_prot_trigger_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_prot_trigger_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_gen_st:0x2b[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_mac_int_gen_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_hsu_irq_st:0x2b[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_hsu_irq_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_int_mac_wakeup_st:0x2b[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_wifi_int_mac_wakeup_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_dm_irq_st:0x2b[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dm_irq_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_dm_irq_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_ble_irq_st:0x2b[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_ble_irq_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_ble_irq_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bt_irq_st:0x2b[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bt_irq_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_bt_irq_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox0_int_st:0x2b[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_mbox0_int_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox1_int_st:0x2b[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_mbox1_int_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bmc64_int_st:0x2b[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_bmc64_int_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_touched_int_st:0x2b[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_touched_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_touched_int_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_usbplug_int_st:0x2b[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_usbplug_int_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_rtc_int_st:0x2b[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rtc_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_rtc_int_st;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_gpio_int_st:0x2b[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_gpio_int_st(sys_hw_t *hw)
{
    return hw->cpu1_int_32_63_status.cpu1_gpio_int_st;
}

/* REG_0x30 */

static inline uint32_t sys_ll_get_gpio_config0_value(sys_hw_t *hw)
{
    return hw->gpio_config0.v;
}

static inline void sys_ll_set_gpio_config0_value(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.v = value;
}

/* REG_0x30:gpio_config0->sys_gpio0:0x30[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio0(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio0;
}

static inline void sys_ll_set_gpio_config0_sys_gpio0(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio0 = value;
}

/* REG_0x30:gpio_config0->sys_gpio1:0x30[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio1(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio1;
}

static inline void sys_ll_set_gpio_config0_sys_gpio1(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio1 = value;
}

/* REG_0x30:gpio_config0->sys_gpio2:0x30[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio2(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio2;
}

static inline void sys_ll_set_gpio_config0_sys_gpio2(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio2 = value;
}

/* REG_0x30:gpio_config0->sys_gpio3:0x30[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio3(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio3;
}

static inline void sys_ll_set_gpio_config0_sys_gpio3(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio3 = value;
}

/* REG_0x30:gpio_config0->sys_gpio4:0x30[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio4(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio4;
}

static inline void sys_ll_set_gpio_config0_sys_gpio4(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio4 = value;
}

/* REG_0x30:gpio_config0->sys_gpio5:0x30[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio5(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio5;
}

static inline void sys_ll_set_gpio_config0_sys_gpio5(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio5 = value;
}

/* REG_0x30:gpio_config0->sys_gpio6:0x30[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio6(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio6;
}

static inline void sys_ll_set_gpio_config0_sys_gpio6(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio6 = value;
}

/* REG_0x30:gpio_config0->sys_gpio7:0x30[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio7(sys_hw_t *hw)
{
    return hw->gpio_config0.sys_gpio7;
}

static inline void sys_ll_set_gpio_config0_sys_gpio7(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config0.sys_gpio7 = value;
}

/* REG_0x31 */

static inline uint32_t sys_ll_get_gpio_config1_value(sys_hw_t *hw)
{
    return hw->gpio_config1.v;
}

static inline void sys_ll_set_gpio_config1_value(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.v = value;
}

/* REG_0x31:gpio_config1->sys_gpio8:0x31[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio8(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio8;
}

static inline void sys_ll_set_gpio_config1_sys_gpio8(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio8 = value;
}

/* REG_0x31:gpio_config1->sys_gpio9:0x31[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio9(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio9;
}

static inline void sys_ll_set_gpio_config1_sys_gpio9(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio9 = value;
}

/* REG_0x31:gpio_config1->sys_gpio10:0x31[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio10(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio10;
}

static inline void sys_ll_set_gpio_config1_sys_gpio10(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio10 = value;
}

/* REG_0x31:gpio_config1->sys_gpio11:0x31[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio11(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio11;
}

static inline void sys_ll_set_gpio_config1_sys_gpio11(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio11 = value;
}

/* REG_0x31:gpio_config1->sys_gpio12:0x31[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio12(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio12;
}

static inline void sys_ll_set_gpio_config1_sys_gpio12(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio12 = value;
}

/* REG_0x31:gpio_config1->sys_gpio13:0x31[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio13(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio13;
}

static inline void sys_ll_set_gpio_config1_sys_gpio13(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio13 = value;
}

/* REG_0x31:gpio_config1->sys_gpio14:0x31[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio14(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio14;
}

static inline void sys_ll_set_gpio_config1_sys_gpio14(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio14 = value;
}

/* REG_0x31:gpio_config1->sys_gpio15:0x31[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio15(sys_hw_t *hw)
{
    return hw->gpio_config1.sys_gpio15;
}

static inline void sys_ll_set_gpio_config1_sys_gpio15(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config1.sys_gpio15 = value;
}

/* REG_0x32 */

static inline uint32_t sys_ll_get_gpio_config2_value(sys_hw_t *hw)
{
    return hw->gpio_config2.v;
}

static inline void sys_ll_set_gpio_config2_value(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.v = value;
}

/* REG_0x32:gpio_config2->sys_gpio16:0x32[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio16(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio16;
}

static inline void sys_ll_set_gpio_config2_sys_gpio16(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio16 = value;
}

/* REG_0x32:gpio_config2->sys_gpio17:0x32[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio17(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio17;
}

static inline void sys_ll_set_gpio_config2_sys_gpio17(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio17 = value;
}

/* REG_0x32:gpio_config2->sys_gpio18:0x32[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio18(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio18;
}

static inline void sys_ll_set_gpio_config2_sys_gpio18(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio18 = value;
}

/* REG_0x32:gpio_config2->sys_gpio19:0x32[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio19(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio19;
}

static inline void sys_ll_set_gpio_config2_sys_gpio19(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio19 = value;
}

/* REG_0x32:gpio_config2->sys_gpio20:0x32[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio20(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio20;
}

static inline void sys_ll_set_gpio_config2_sys_gpio20(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio20 = value;
}

/* REG_0x32:gpio_config2->sys_gpio21:0x32[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio21(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio21;
}

static inline void sys_ll_set_gpio_config2_sys_gpio21(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio21 = value;
}

/* REG_0x32:gpio_config2->sys_gpio22:0x32[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio22(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio22;
}

static inline void sys_ll_set_gpio_config2_sys_gpio22(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio22 = value;
}

/* REG_0x32:gpio_config2->sys_gpio23:0x32[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio23(sys_hw_t *hw)
{
    return hw->gpio_config2.sys_gpio23;
}

static inline void sys_ll_set_gpio_config2_sys_gpio23(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config2.sys_gpio23 = value;
}

/* REG_0x33 */

static inline uint32_t sys_ll_get_gpio_config3_value(sys_hw_t *hw)
{
    return hw->gpio_config3.v;
}

static inline void sys_ll_set_gpio_config3_value(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.v = value;
}

/* REG_0x33:gpio_config3->sys_gpio24:0x33[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio24(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio24;
}

static inline void sys_ll_set_gpio_config3_sys_gpio24(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio24 = value;
}

/* REG_0x33:gpio_config3->sys_gpio25:0x33[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio25(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio25;
}

static inline void sys_ll_set_gpio_config3_sys_gpio25(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio25 = value;
}

/* REG_0x33:gpio_config3->sys_gpio26:0x33[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio26(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio26;
}

static inline void sys_ll_set_gpio_config3_sys_gpio26(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio26 = value;
}

/* REG_0x33:gpio_config3->sys_gpio27:0x33[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio27(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio27;
}

static inline void sys_ll_set_gpio_config3_sys_gpio27(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio27 = value;
}

/* REG_0x33:gpio_config3->sys_gpio28:0x33[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio28(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio28;
}

static inline void sys_ll_set_gpio_config3_sys_gpio28(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio28 = value;
}

/* REG_0x33:gpio_config3->sys_gpio29:0x33[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio29(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio29;
}

static inline void sys_ll_set_gpio_config3_sys_gpio29(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio29 = value;
}

/* REG_0x33:gpio_config3->sys_gpio30:0x33[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio30(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio30;
}

static inline void sys_ll_set_gpio_config3_sys_gpio30(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio30 = value;
}

/* REG_0x33:gpio_config3->sys_gpio31:0x33[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio31(sys_hw_t *hw)
{
    return hw->gpio_config3.sys_gpio31;
}

static inline void sys_ll_set_gpio_config3_sys_gpio31(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config3.sys_gpio31 = value;
}

/* REG_0x34 */

static inline uint32_t sys_ll_get_gpio_config4_value(sys_hw_t *hw)
{
    return hw->gpio_config4.v;
}

static inline void sys_ll_set_gpio_config4_value(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.v = value;
}

/* REG_0x34:gpio_config4->sys_gpio32:0x34[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio32(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio32;
}

static inline void sys_ll_set_gpio_config4_sys_gpio32(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio32 = value;
}

/* REG_0x34:gpio_config4->sys_gpio33:0x34[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio33(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio33;
}

static inline void sys_ll_set_gpio_config4_sys_gpio33(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio33 = value;
}

/* REG_0x34:gpio_config4->sys_gpio34:0x34[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio34(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio34;
}

static inline void sys_ll_set_gpio_config4_sys_gpio34(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio34 = value;
}

/* REG_0x34:gpio_config4->sys_gpio35:0x34[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio35(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio35;
}

static inline void sys_ll_set_gpio_config4_sys_gpio35(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio35 = value;
}

/* REG_0x34:gpio_config4->sys_gpio36:0x34[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio36(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio36;
}

static inline void sys_ll_set_gpio_config4_sys_gpio36(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio36 = value;
}

/* REG_0x34:gpio_config4->sys_gpio37:0x34[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio37(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio37;
}

static inline void sys_ll_set_gpio_config4_sys_gpio37(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio37 = value;
}

/* REG_0x34:gpio_config4->sys_gpio38:0x34[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio38(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio38;
}

static inline void sys_ll_set_gpio_config4_sys_gpio38(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio38 = value;
}

/* REG_0x34:gpio_config4->sys_gpio39:0x34[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio39(sys_hw_t *hw)
{
    return hw->gpio_config4.sys_gpio39;
}

static inline void sys_ll_set_gpio_config4_sys_gpio39(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config4.sys_gpio39 = value;
}

/* REG_0x35 */

static inline uint32_t sys_ll_get_gpio_config5_value(sys_hw_t *hw)
{
    return hw->gpio_config5.v;
}

static inline void sys_ll_set_gpio_config5_value(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.v = value;
}

/* REG_0x35:gpio_config5->sys_gpio40:0x35[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio40(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio40;
}

static inline void sys_ll_set_gpio_config5_sys_gpio40(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio40 = value;
}

/* REG_0x35:gpio_config5->sys_gpio41:0x35[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio41(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio41;
}

static inline void sys_ll_set_gpio_config5_sys_gpio41(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio41 = value;
}

/* REG_0x35:gpio_config5->sys_gpio42:0x35[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio42(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio42;
}

static inline void sys_ll_set_gpio_config5_sys_gpio42(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio42 = value;
}

/* REG_0x35:gpio_config5->sys_gpio43:0x35[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio43(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio43;
}

static inline void sys_ll_set_gpio_config5_sys_gpio43(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio43 = value;
}

/* REG_0x35:gpio_config5->sys_gpio44:0x35[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio44(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio44;
}

static inline void sys_ll_set_gpio_config5_sys_gpio44(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio44 = value;
}

/* REG_0x35:gpio_config5->sys_gpio45:0x35[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio45(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio45;
}

static inline void sys_ll_set_gpio_config5_sys_gpio45(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio45 = value;
}

/* REG_0x35:gpio_config5->sys_gpio46:0x35[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio46(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio46;
}

static inline void sys_ll_set_gpio_config5_sys_gpio46(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio46 = value;
}

/* REG_0x35:gpio_config5->sys_gpio47:0x35[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio47(sys_hw_t *hw)
{
    return hw->gpio_config5.sys_gpio47;
}

static inline void sys_ll_set_gpio_config5_sys_gpio47(sys_hw_t *hw, uint32_t value)
{
    hw->gpio_config5.sys_gpio47 = value;
}

/* REG_0x38 */

static inline uint32_t sys_ll_get_sys_debug_config0_value(sys_hw_t *hw)
{
    return hw->sys_debug_config0.v;
}

static inline void sys_ll_set_sys_debug_config0_value(sys_hw_t *hw, uint32_t value)
{
    hw->sys_debug_config0.v = value;
}

/* REG_0x38:sys_debug_config0->dbug_config0:0x38[31:0], ,0,R/W*/
static inline uint32_t sys_ll_get_sys_debug_config0_dbug_config0(sys_hw_t *hw)
{
    return hw->sys_debug_config0.v;
}

static inline void sys_ll_set_sys_debug_config0_dbug_config0(sys_hw_t *hw, uint32_t value)
{
    hw->sys_debug_config0.v = value;
}

/* REG_0x39 */

static inline uint32_t sys_ll_get_sys_debug_config1_value(sys_hw_t *hw)
{
    return hw->sys_debug_config1.v;
}

static inline void sys_ll_set_sys_debug_config1_value(sys_hw_t *hw, uint32_t value)
{
    hw->sys_debug_config1.v = value;
}

/* REG_0x39:sys_debug_config1->dbug_config1:0x39[31:0],0: btsp_debug[0:32]        1: btsp_debug[32+:32]           2: btsp_debug[64+:32]  4:btsp_debug[96+:6]       5:wifip_mac_dbg[31:0]           6: wifip_phy_dbg[31:0]                            default:  dbug_config0                   ,0,R/W*/
static inline uint32_t sys_ll_get_sys_debug_config1_dbug_config1(sys_hw_t *hw)
{
    return hw->sys_debug_config1.v;
}

static inline void sys_ll_set_sys_debug_config1_dbug_config1(sys_hw_t *hw, uint32_t value)
{
    hw->sys_debug_config1.v = value;
}

/* REG_0x40 */

static inline uint32_t sys_ll_get_ana_reg0_value(sys_hw_t *hw)
{
    return hw->ana_reg0.v;
}

static inline void sys_ll_set_ana_reg0_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.v = value;
}

/* REG_0x40:ana_reg0->ck2652sel:0x40[0],1:26MHz/0:52MHz,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_ck2652sel(sys_hw_t *hw)
{
    return hw->ana_reg0.ck2652sel;
}

static inline void sys_ll_set_ana_reg0_ck2652sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.ck2652sel = value;
}

/* REG_0x40:ana_reg0->cp:0x40[3:1],cp curent control 0to 350uA 50uA step,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_cp(sys_hw_t *hw)
{
    return hw->ana_reg0.cp;
}

static inline void sys_ll_set_ana_reg0_cp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.cp = value;
}

/* REG_0x40:ana_reg0->spideten:0x40[4],unlock detect enable fron spi 1:enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_spideten(sys_hw_t *hw)
{
    return hw->ana_reg0.spideten;
}

static inline void sys_ll_set_ana_reg0_spideten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.spideten = value;
}

/* REG_0x40:ana_reg0->hvref:0x40[6:5],high vth control for unlock detect 00:0.85V;01:0.9V;10:0.95V;11:1.05V,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_hvref(sys_hw_t *hw)
{
    return hw->ana_reg0.hvref;
}

static inline void sys_ll_set_ana_reg0_hvref(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.hvref = value;
}

/* REG_0x40:ana_reg0->lvref:0x40[8:7],low vth control for unlock detect 00:0.2V;01:0.3V;10:0.35V;11:0.4V,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_lvref(sys_hw_t *hw)
{
    return hw->ana_reg0.lvref;
}

static inline void sys_ll_set_ana_reg0_lvref(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.lvref = value;
}

/* REG_0x40:ana_reg0->Rzctrl26M:0x40[9],Rz ctrl in 26M mode:1:normal;0:add 14K,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_rzctrl26m(sys_hw_t *hw)
{
    return hw->ana_reg0.rzctrl26m;
}

static inline void sys_ll_set_ana_reg0_rzctrl26m(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.rzctrl26m = value;
}

/* REG_0x40:ana_reg0->LoopRzctrl:0x40[13:10],Rz ctrl:2K to 17K,1K step,9,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_looprzctrl(sys_hw_t *hw)
{
    return hw->ana_reg0.looprzctrl;
}

static inline void sys_ll_set_ana_reg0_looprzctrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.looprzctrl = value;
}

/* REG_0x40:ana_reg0->rpc:0x40[15:14],second pole Rp ctrl:00:30K;01:10K;10:22K;11:2K,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_rpc(sys_hw_t *hw)
{
    return hw->ana_reg0.rpc;
}

static inline void sys_ll_set_ana_reg0_rpc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.rpc = value;
}

/* REG_0x40:ana_reg0->nsyn:0x40[16],N divider rst,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_nsyn(sys_hw_t *hw)
{
    return hw->ana_reg0.nsyn;
}

static inline void sys_ll_set_ana_reg0_nsyn(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.nsyn = value;
}

/* REG_0x40:ana_reg0->cksel:0x40[18:17],0:26M;1:40M;2:24M;3:19.2M,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_cksel(sys_hw_t *hw)
{
    return hw->ana_reg0.cksel;
}

static inline void sys_ll_set_ana_reg0_cksel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.cksel = value;
}

/* REG_0x40:ana_reg0->spitrig:0x40[19],SPI band selection trigger signal,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_spitrig(sys_hw_t *hw)
{
    return hw->ana_reg0.spitrig;
}

static inline void sys_ll_set_ana_reg0_spitrig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.spitrig = value;
}

/* REG_0x40:ana_reg0->band:0x40[24:20],band manual value/band[0] ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_band(sys_hw_t *hw)
{
    return hw->ana_reg0.band;
}

static inline void sys_ll_set_ana_reg0_band(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.band = value;
}

/* REG_0x40:ana_reg0->bandmanual:0x40[25],1:band manual;0:band auto,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_bandmanual(sys_hw_t *hw)
{
    return hw->ana_reg0.bandmanual;
}

static inline void sys_ll_set_ana_reg0_bandmanual(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.bandmanual = value;
}

/* REG_0x40:ana_reg0->dsptrig:0x40[26],band selection trigger signal,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_dsptrig(sys_hw_t *hw)
{
    return hw->ana_reg0.dsptrig;
}

static inline void sys_ll_set_ana_reg0_dsptrig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.dsptrig = value;
}

/* REG_0x40:ana_reg0->lpen_dpll:0x40[27],dpll low power mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_lpen_dpll(sys_hw_t *hw)
{
    return hw->ana_reg0.lpen_dpll;
}

static inline void sys_ll_set_ana_reg0_lpen_dpll(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.lpen_dpll = value;
}

/* REG_0x40:ana_reg0->xamp:0x40[31:28],xtal OSC amp control/xamp<0> shared with pll_cktst_en,0xF,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_xamp(sys_hw_t *hw)
{
    return hw->ana_reg0.xamp;
}

static inline void sys_ll_set_ana_reg0_xamp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg0.xamp = value;
}

/* REG_0x41 */

static inline uint32_t sys_ll_get_ana_reg1_value(sys_hw_t *hw)
{
    return hw->ana_reg1.v;
}

static inline void sys_ll_set_ana_reg1_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.v = value;
}

/* REG_0x41:ana_reg1->dpll_vrefsel:0x41[1],dpll ldo reference voltage selection  0:vbg_aon/1:vbg_cal,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_dpll_vrefsel(sys_hw_t *hw)
{
    return hw->ana_reg1.dpll_vrefsel;
}

static inline void sys_ll_set_ana_reg1_dpll_vrefsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.dpll_vrefsel = value;
}

/* REG_0x41:ana_reg1->msw:0x41[10:2],set the frequency of DCO manual,70,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_msw(sys_hw_t *hw)
{
    return hw->ana_reg1.msw;
}

static inline void sys_ll_set_ana_reg1_msw(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.msw = value;
}

/* REG_0x41:ana_reg1->ictrl:0x41[13:11],controlling the bias cuttent of DCO core,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_ictrl(sys_hw_t *hw)
{
    return hw->ana_reg1.ictrl;
}

static inline void sys_ll_set_ana_reg1_ictrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.ictrl = value;
}

/* REG_0x41:ana_reg1->osc_trig:0x41[14],reset the DCO core by spi to make it oscillate again,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_osc_trig(sys_hw_t *hw)
{
    return hw->ana_reg1.osc_trig;
}

static inline void sys_ll_set_ana_reg1_osc_trig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.osc_trig = value;
}

/* REG_0x41:ana_reg1->osccal_trig:0x41[15],trigger the action of callibration in the DCO,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_osccal_trig(sys_hw_t *hw)
{
    return hw->ana_reg1.osccal_trig;
}

static inline void sys_ll_set_ana_reg1_osccal_trig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.osccal_trig = value;
}

/* REG_0x41:ana_reg1->cnti:0x41[24:16],set the controlling work of calibration in the DCO block to get the different frequency,C0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_cnti(sys_hw_t *hw)
{
    return hw->ana_reg1.cnti;
}

static inline void sys_ll_set_ana_reg1_cnti(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.cnti = value;
}

/* REG_0x41:ana_reg1->spi_rst:0x41[25],reset the calibration block of DCO by spi,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_spi_rst(sys_hw_t *hw)
{
    return hw->ana_reg1.spi_rst;
}

static inline void sys_ll_set_ana_reg1_spi_rst(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.spi_rst = value;
}

/* REG_0x41:ana_reg1->amsel:0x41[26],disable the calibration function of the DCO,set the frequency of DCO manual,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_amsel(sys_hw_t *hw)
{
    return hw->ana_reg1.amsel;
}

static inline void sys_ll_set_ana_reg1_amsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.amsel = value;
}

/* REG_0x41:ana_reg1->divctrl:0x41[29:27],controlling the value of divider in the DCO to get the different frequency,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_divctrl(sys_hw_t *hw)
{
    return hw->ana_reg1.divctrl;
}

static inline void sys_ll_set_ana_reg1_divctrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.divctrl = value;
}

/* REG_0x41:ana_reg1->dco_tsten:0x41[30],dco test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_dco_tsten(sys_hw_t *hw)
{
    return hw->ana_reg1.dco_tsten;
}

static inline void sys_ll_set_ana_reg1_dco_tsten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.dco_tsten = value;
}

/* REG_0x41:ana_reg1->rosc_tsten:0x41[31],rosc test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_rosc_tsten(sys_hw_t *hw)
{
    return hw->ana_reg1.rosc_tsten;
}

static inline void sys_ll_set_ana_reg1_rosc_tsten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg1.rosc_tsten = value;
}

/* REG_0x42 */

static inline uint32_t sys_ll_get_ana_reg2_value(sys_hw_t *hw)
{
    return hw->ana_reg2.v;
}

static inline void sys_ll_set_ana_reg2_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.v = value;
}

/* REG_0x42:ana_reg2->pwmscmen:0x42[0],buck nmos disable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_pwmscmen(sys_hw_t *hw)
{
    return hw->ana_reg2.pwmscmen;
}

static inline void sys_ll_set_ana_reg2_pwmscmen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.pwmscmen = value;
}

/* REG_0x42:ana_reg2->buck_fasten:0x42[1],buck EA fast transient enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_buck_fasten(sys_hw_t *hw)
{
    return hw->ana_reg2.buck_fasten;
}

static inline void sys_ll_set_ana_reg2_buck_fasten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.buck_fasten = value;
}

/* REG_0x42:ana_reg2->cls:0x42[4:2],buck current limit setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_cls(sys_hw_t *hw)
{
    return hw->ana_reg2.cls;
}

static inline void sys_ll_set_ana_reg2_cls(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.cls = value;
}

/* REG_0x42:ana_reg2->pfms:0x42[9:5],buck freewheeling damping enable(=1) ,13,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_pfms(sys_hw_t *hw)
{
    return hw->ana_reg2.pfms;
}

static inline void sys_ll_set_ana_reg2_pfms(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.pfms = value;
}

/* REG_0x42:ana_reg2->ripc:0x42[12:10],buck pfm mode voltage ripple control setting,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_ripc(sys_hw_t *hw)
{
    return hw->ana_reg2.ripc;
}

static inline void sys_ll_set_ana_reg2_ripc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.ripc = value;
}

/* REG_0x42:ana_reg2->rampc:0x42[16:13],buck ramping compensation setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_rampc(sys_hw_t *hw)
{
    return hw->ana_reg2.rampc;
}

static inline void sys_ll_set_ana_reg2_rampc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.rampc = value;
}

/* REG_0x42:ana_reg2->rampcen:0x42[17],buck ramping compensation enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_rampcen(sys_hw_t *hw)
{
    return hw->ana_reg2.rampcen;
}

static inline void sys_ll_set_ana_reg2_rampcen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.rampcen = value;
}

/* REG_0x42:ana_reg2->dpfmen:0x42[18],buck pfm mode current reduce enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_dpfmen(sys_hw_t *hw)
{
    return hw->ana_reg2.dpfmen;
}

static inline void sys_ll_set_ana_reg2_dpfmen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.dpfmen = value;
}

/* REG_0x42:ana_reg2->pfmen:0x42[19],buck pfm mode enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_pfmen(sys_hw_t *hw)
{
    return hw->ana_reg2.pfmen;
}

static inline void sys_ll_set_ana_reg2_pfmen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.pfmen = value;
}

/* REG_0x42:ana_reg2->forcepfm:0x42[20],buck force pfm mode(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_forcepfm(sys_hw_t *hw)
{
    return hw->ana_reg2.forcepfm;
}

static inline void sys_ll_set_ana_reg2_forcepfm(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.forcepfm = value;
}

/* REG_0x42:ana_reg2->swrsten:0x42[21],buck freewheeling damping enable(=1) ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_swrsten(sys_hw_t *hw)
{
    return hw->ana_reg2.swrsten;
}

static inline void sys_ll_set_ana_reg2_swrsten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.swrsten = value;
}

/* REG_0x42:ana_reg2->tmposel:0x42[23:22],buck mpo pulse width control 0--shortest   3---longest,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_tmposel(sys_hw_t *hw)
{
    return hw->ana_reg2.tmposel;
}

static inline void sys_ll_set_ana_reg2_tmposel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.tmposel = value;
}

/* REG_0x42:ana_reg2->mpoen:0x42[24],buck mpo mode enable( =1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_mpoen(sys_hw_t *hw)
{
    return hw->ana_reg2.mpoen;
}

static inline void sys_ll_set_ana_reg2_mpoen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.mpoen = value;
}

/* REG_0x42:ana_reg2->spi_latchb:0x42[25],spi latch disable 0:latch;1:no latch,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_spi_latchb(sys_hw_t *hw)
{
    return hw->ana_reg2.spi_latchb;
}

static inline void sys_ll_set_ana_reg2_spi_latchb(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.spi_latchb = value;
}

/* REG_0x42:ana_reg2->ldosel:0x42[26],ldo/buck select, 0:buck;1:LDO,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_ldosel(sys_hw_t *hw)
{
    return hw->ana_reg2.ldosel;
}

static inline void sys_ll_set_ana_reg2_ldosel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.ldosel = value;
}

/* REG_0x42:ana_reg2->iovoc:0x42[29:27],ioldo output voltage select 0:2.9V,….7:3.6V,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_iovoc(sys_hw_t *hw)
{
    return hw->ana_reg2.iovoc;
}

static inline void sys_ll_set_ana_reg2_iovoc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.iovoc = value;
}

/* REG_0x42:ana_reg2->vbpbuf_hp:0x42[30],vbspbuffer high power enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_vbpbuf_hp(sys_hw_t *hw)
{
    return hw->ana_reg2.vbpbuf_hp;
}

static inline void sys_ll_set_ana_reg2_vbpbuf_hp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.vbpbuf_hp = value;
}

/* REG_0x42:ana_reg2->bypassen:0x42[31],ioldo bypass enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_bypassen(sys_hw_t *hw)
{
    return hw->ana_reg2.bypassen;
}

static inline void sys_ll_set_ana_reg2_bypassen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg2.bypassen = value;
}

/* REG_0x43 */

static inline uint32_t sys_ll_get_ana_reg3_value(sys_hw_t *hw)
{
    return hw->ana_reg3.v;
}

static inline void sys_ll_set_ana_reg3_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.v = value;
}

/* REG_0x43:ana_reg3->zcdta:0x43[4:0],buck zcd delay tune setting,1F,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdta(sys_hw_t *hw)
{
    return hw->ana_reg3.zcdta;
}

static inline void sys_ll_set_ana_reg3_zcdta(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.zcdta = value;
}

/* REG_0x43:ana_reg3->zcdcala:0x43[10:5],buck zcd offset cali setting,E,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdcala(sys_hw_t *hw)
{
    return hw->ana_reg3.zcdcala;
}

static inline void sys_ll_set_ana_reg3_zcdcala(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.zcdcala = value;
}

/* REG_0x43:ana_reg3->zcdmen:0x43[11],buck zcd manual cali enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdmen(sys_hw_t *hw)
{
    return hw->ana_reg3.zcdmen;
}

static inline void sys_ll_set_ana_reg3_zcdmen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.zcdmen = value;
}

/* REG_0x43:ana_reg3->zcdcalen:0x43[12],buck zcd calibration enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdcalen(sys_hw_t *hw)
{
    return hw->ana_reg3.zcdcalen;
}

static inline void sys_ll_set_ana_reg3_zcdcalen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.zcdcalen = value;
}

/* REG_0x43:ana_reg3->zcdcal_tri:0x43[13],buck zcd auto cali triggle(0-->1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdcal_tri(sys_hw_t *hw)
{
    return hw->ana_reg3.zcdcal_tri;
}

static inline void sys_ll_set_ana_reg3_zcdcal_tri(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.zcdcal_tri = value;
}

/* REG_0x43:ana_reg3->mroscsel:0x43[14],buck oscillator manual cali. enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_mroscsel(sys_hw_t *hw)
{
    return hw->ana_reg3.mroscsel;
}

static inline void sys_ll_set_ana_reg3_mroscsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.mroscsel = value;
}

/* REG_0x43:ana_reg3->mfsel:0x43[17:15],buck oscillator manual fsel  ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_mfsel(sys_hw_t *hw)
{
    return hw->ana_reg3.mfsel;
}

static inline void sys_ll_set_ana_reg3_mfsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.mfsel = value;
}

/* REG_0x43:ana_reg3->mroscbcal:0x43[21:18],buck oscillator manual cap_cal  0xA---500k 0xB--1M 0x9---2M,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_mroscbcal(sys_hw_t *hw)
{
    return hw->ana_reg3.mroscbcal;
}

static inline void sys_ll_set_ana_reg3_mroscbcal(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.mroscbcal = value;
}

/* REG_0x43:ana_reg3->osccaltrig:0x43[22],buck oscillator manual cali. enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_osccaltrig(sys_hw_t *hw)
{
    return hw->ana_reg3.osccaltrig;
}

static inline void sys_ll_set_ana_reg3_osccaltrig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.osccaltrig = value;
}

/* REG_0x43:ana_reg3->ckintsel:0x43[23],buck clock source select  1-- ring oscillator   0--divider,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_ckintsel(sys_hw_t *hw)
{
    return hw->ana_reg3.ckintsel;
}

static inline void sys_ll_set_ana_reg3_ckintsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.ckintsel = value;
}

/* REG_0x43:ana_reg3->ckfs:0x43[25:24],buck output clock freq. select   0--500k 1---1M  2--2M  3--4M,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_ckfs(sys_hw_t *hw)
{
    return hw->ana_reg3.ckfs;
}

static inline void sys_ll_set_ana_reg3_ckfs(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.ckfs = value;
}

/* REG_0x43:ana_reg3->vlsel_ldodig:0x43[28:26],digldo output voltage select(low power)  0:0.6V,…..7:1.4V,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_vlsel_ldodig(sys_hw_t *hw)
{
    return hw->ana_reg3.vlsel_ldodig;
}

static inline void sys_ll_set_ana_reg3_vlsel_ldodig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.vlsel_ldodig = value;
}

/* REG_0x43:ana_reg3->vhsel_ldodig:0x43[31:29],digldo output voltage select(high power)  0:0.6V,…..7:1.4V,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_vhsel_ldodig(sys_hw_t *hw)
{
    return hw->ana_reg3.vhsel_ldodig;
}

static inline void sys_ll_set_ana_reg3_vhsel_ldodig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg3.vhsel_ldodig = value;
}

/* REG_0x44 */

static inline uint32_t sys_ll_get_ana_reg4_value(sys_hw_t *hw)
{
    return hw->ana_reg4.v;
}

static inline void sys_ll_set_ana_reg4_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.v = value;
}

/* REG_0x44:ana_reg4->cb_manu_val:0x44[9:5],CB Calibration Manual Value,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cb_manu_val(sys_hw_t *hw)
{
    return hw->ana_reg4.cb_manu_val;
}

static inline void sys_ll_set_ana_reg4_cb_manu_val(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.cb_manu_val = value;
}

/* REG_0x44:ana_reg4->cb_cal_trig:0x44[10],CB Calibration Trigger,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cb_cal_trig(sys_hw_t *hw)
{
    return hw->ana_reg4.cb_cal_trig;
}

static inline void sys_ll_set_ana_reg4_cb_cal_trig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.cb_cal_trig = value;
}

/* REG_0x44:ana_reg4->cb_cal_manu:0x44[11],CB Calibration Manual Mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cb_cal_manu(sys_hw_t *hw)
{
    return hw->ana_reg4.cb_cal_manu;
}

static inline void sys_ll_set_ana_reg4_cb_cal_manu(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.cb_cal_manu = value;
}

/* REG_0x44:ana_reg4->rosc_cal_intval:0x44[14:12],Rosc Calibration Interlval 0.25s~2s,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_intval(sys_hw_t *hw)
{
    return hw->ana_reg4.rosc_cal_intval;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_intval(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.rosc_cal_intval = value;
}

/* REG_0x44:ana_reg4->manu_cin:0x44[21:15],Rosc Calibration Manual Cin,40,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_manu_cin(sys_hw_t *hw)
{
    return hw->ana_reg4.manu_cin;
}

static inline void sys_ll_set_ana_reg4_manu_cin(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.manu_cin = value;
}

/* REG_0x44:ana_reg4->manu_fin:0x44[26:22],Rosc Calibration Manual Fin,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_manu_fin(sys_hw_t *hw)
{
    return hw->ana_reg4.manu_fin;
}

static inline void sys_ll_set_ana_reg4_manu_fin(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.manu_fin = value;
}

/* REG_0x44:ana_reg4->rosc_cal_mode:0x44[27],Rosc Calibration Mode:; 0x1: 32K; 0x0: 31.25K,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_mode(sys_hw_t *hw)
{
    return hw->ana_reg4.rosc_cal_mode;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_mode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.rosc_cal_mode = value;
}

/* REG_0x44:ana_reg4->rosc_cal_trig:0x44[28],Rosc Calibration Trigger,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_trig(sys_hw_t *hw)
{
    return hw->ana_reg4.rosc_cal_trig;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_trig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.rosc_cal_trig = value;
}

/* REG_0x44:ana_reg4->rosc_cal_en:0x44[29],Rosc Calibration Enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_en(sys_hw_t *hw)
{
    return hw->ana_reg4.rosc_cal_en;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.rosc_cal_en = value;
}

/* REG_0x44:ana_reg4->rosc_manu_en:0x44[30],Rosc Calibration Manual Mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_manu_en(sys_hw_t *hw)
{
    return hw->ana_reg4.rosc_manu_en;
}

static inline void sys_ll_set_ana_reg4_rosc_manu_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.rosc_manu_en = value;
}

/* REG_0x44:ana_reg4->rosc_tsten:0x44[31],Rosc test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_tsten(sys_hw_t *hw)
{
    return hw->ana_reg4.rosc_tsten;
}

static inline void sys_ll_set_ana_reg4_rosc_tsten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg4.rosc_tsten = value;
}

/* REG_0x45 */

static inline uint32_t sys_ll_get_ana_reg5_value(sys_hw_t *hw)
{
    return hw->ana_reg5.v;
}

static inline void sys_ll_set_ana_reg5_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.v = value;
}

/* REG_0x45:ana_reg5->vref_scale:0x45[0],gadc reference voltage scale enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vref_scale(sys_hw_t *hw)
{
    return hw->ana_reg5.vref_scale;
}

static inline void sys_ll_set_ana_reg5_vref_scale(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.vref_scale = value;
}

/* REG_0x45:ana_reg5->dccal_en:0x45[1],gadc DC calibration enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_dccal_en(sys_hw_t *hw)
{
    return hw->ana_reg5.dccal_en;
}

static inline void sys_ll_set_ana_reg5_dccal_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.dccal_en = value;
}

/* REG_0x45:ana_reg5->xtalh_ctune:0x45[8:2],xtalh load cap tuning,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_xtalh_ctune(sys_hw_t *hw)
{
    return hw->ana_reg5.xtalh_ctune;
}

static inline void sys_ll_set_ana_reg5_xtalh_ctune(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.xtalh_ctune = value;
}

/* REG_0x45:ana_reg5->cktst_sel:0x45[10:9],clock test signal selection rosc/xtall/dco/dpll,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_cktst_sel(sys_hw_t *hw)
{
    return hw->ana_reg5.cktst_sel;
}

static inline void sys_ll_set_ana_reg5_cktst_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.cktst_sel = value;
}

/* REG_0x45:ana_reg5->ck_tst_enbale:0x45[11],system clock test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_ck_tst_enbale(sys_hw_t *hw)
{
    return hw->ana_reg5.ck_tst_enbale;
}

static inline void sys_ll_set_ana_reg5_ck_tst_enbale(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.ck_tst_enbale = value;
}

/* REG_0x45:ana_reg5->trxt_tst_enable:0x45[12],wifi trx test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_trxt_tst_enable(sys_hw_t *hw)
{
    return hw->ana_reg5.trxt_tst_enable;
}

static inline void sys_ll_set_ana_reg5_trxt_tst_enable(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.trxt_tst_enable = value;
}

/* REG_0x45:ana_reg5->encb:0x45[13],global central bias enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_encb(sys_hw_t *hw)
{
    return hw->ana_reg5.encb;
}

static inline void sys_ll_set_ana_reg5_encb(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.encb = value;
}

/* REG_0x45:ana_reg5->vctrl_dpllldo:0x45[15:14],dpll ldo output selection,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vctrl_dpllldo(sys_hw_t *hw)
{
    return hw->ana_reg5.vctrl_dpllldo;
}

static inline void sys_ll_set_ana_reg5_vctrl_dpllldo(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.vctrl_dpllldo = value;
}

/* REG_0x45:ana_reg5->vctrl_sysldo:0x45[17:16],sys ldo output selection,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vctrl_sysldo(sys_hw_t *hw)
{
    return hw->ana_reg5.vctrl_sysldo;
}

static inline void sys_ll_set_ana_reg5_vctrl_sysldo(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.vctrl_sysldo = value;
}

/* REG_0x45:ana_reg5->temptst_en:0x45[18],tempdet test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_temptst_en(sys_hw_t *hw)
{
    return hw->ana_reg5.temptst_en;
}

static inline void sys_ll_set_ana_reg5_temptst_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.temptst_en = value;
}

/* REG_0x45:ana_reg5->gadc_inbuf_ictrl:0x45[20:19],gadc input buf Ibias selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_gadc_inbuf_ictrl(sys_hw_t *hw)
{
    return hw->ana_reg5.gadc_inbuf_ictrl;
}

static inline void sys_ll_set_ana_reg5_gadc_inbuf_ictrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.gadc_inbuf_ictrl = value;
}

/* REG_0x45:ana_reg5->xtalh_ictrl:0x45[22],xtalh current control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_xtalh_ictrl(sys_hw_t *hw)
{
    return hw->ana_reg5.xtalh_ictrl;
}

static inline void sys_ll_set_ana_reg5_xtalh_ictrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.xtalh_ictrl = value;
}

/* REG_0x45:ana_reg5->bgcalm:0x45[28:23],bandgap calibration manual setting,20,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcalm(sys_hw_t *hw)
{
    return hw->ana_reg5.bgcalm;
}

static inline void sys_ll_set_ana_reg5_bgcalm(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.bgcalm = value;
}

/* REG_0x45:ana_reg5->bgcal_trig:0x45[29],bandgap calibrarion trig,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcal_trig(sys_hw_t *hw)
{
    return hw->ana_reg5.bgcal_trig;
}

static inline void sys_ll_set_ana_reg5_bgcal_trig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.bgcal_trig = value;
}

/* REG_0x45:ana_reg5->bgcal_manu:0x45[30],bandgap calibration manual mode enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcal_manu(sys_hw_t *hw)
{
    return hw->ana_reg5.bgcal_manu;
}

static inline void sys_ll_set_ana_reg5_bgcal_manu(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.bgcal_manu = value;
}

/* REG_0x45:ana_reg5->bgcal_en:0x45[31],bandgap calibration enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcal_en(sys_hw_t *hw)
{
    return hw->ana_reg5.bgcal_en;
}

static inline void sys_ll_set_ana_reg5_bgcal_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg5.bgcal_en = value;
}

/* REG_0x46 */

static inline uint32_t sys_ll_get_ana_reg6_value(sys_hw_t *hw)
{
    return hw->ana_reg6.v;
}

static inline void sys_ll_set_ana_reg6_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.v = value;
}

/* REG_0x46:ana_reg6->itune_xtall:0x46[3:0],xtall core current control,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_itune_xtall(sys_hw_t *hw)
{
    return hw->ana_reg6.itune_xtall;
}

static inline void sys_ll_set_ana_reg6_itune_xtall(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.itune_xtall = value;
}

/* REG_0x46:ana_reg6->xtall_ten:0x46[4],xtall test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_xtall_ten(sys_hw_t *hw)
{
    return hw->ana_reg6.xtall_ten;
}

static inline void sys_ll_set_ana_reg6_xtall_ten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.xtall_ten = value;
}

/* REG_0x46:ana_reg6->psldo_vsel:0x46[5],ps ldo output voltage selection,0:VIO /1:1.8V,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_psldo_vsel(sys_hw_t *hw)
{
    return hw->ana_reg6.psldo_vsel;
}

static inline void sys_ll_set_ana_reg6_psldo_vsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.psldo_vsel = value;
}

/* REG_0x46:ana_reg6->en_usb:0x46[6],usb phy enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_usb(sys_hw_t *hw)
{
    return hw->ana_reg6.en_usb;
}

static inline void sys_ll_set_ana_reg6_en_usb(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_usb = value;
}

/* REG_0x46:ana_reg6->en_xtall:0x46[7],xtall oscillator enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_xtall(sys_hw_t *hw)
{
    return hw->ana_reg6.en_xtall;
}

static inline void sys_ll_set_ana_reg6_en_xtall(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_xtall = value;
}

/* REG_0x46:ana_reg6->en_dco:0x46[8],dco enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_dco(sys_hw_t *hw)
{
    return hw->ana_reg6.en_dco;
}

static inline void sys_ll_set_ana_reg6_en_dco(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_dco = value;
}

/* REG_0x46:ana_reg6->en_psram_ldo:0x46[9],psram ldo enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_psram_ldo(sys_hw_t *hw)
{
    return hw->ana_reg6.en_psram_ldo;
}

static inline void sys_ll_set_ana_reg6_en_psram_ldo(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_psram_ldo = value;
}

/* REG_0x46:ana_reg6->en_tempdet:0x46[10],tempreture det enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_tempdet(sys_hw_t *hw)
{
    return hw->ana_reg6.en_tempdet;
}

static inline void sys_ll_set_ana_reg6_en_tempdet(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_tempdet = value;
}

/* REG_0x46:ana_reg6->en_audpll:0x46[11],audio pll enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_audpll(sys_hw_t *hw)
{
    return hw->ana_reg6.en_audpll;
}

static inline void sys_ll_set_ana_reg6_en_audpll(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_audpll = value;
}

/* REG_0x46:ana_reg6->en_dpll:0x46[12],dpll enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_dpll(sys_hw_t *hw)
{
    return hw->ana_reg6.en_dpll;
}

static inline void sys_ll_set_ana_reg6_en_dpll(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_dpll = value;
}

/* REG_0x46:ana_reg6->en_sysldo:0x46[13],sysldo enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_sysldo(sys_hw_t *hw)
{
    return hw->ana_reg6.en_sysldo;
}

static inline void sys_ll_set_ana_reg6_en_sysldo(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_sysldo = value;
}

/* REG_0x46:ana_reg6->en_aud:0x46[14],audio ldo enable,0,W*/
static inline uint32_t sys_ll_get_ana_reg6_en_aud(sys_hw_t *hw)
{
    return hw->ana_reg6.en_aud;
}

static inline void sys_ll_set_ana_reg6_en_aud(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_aud = value;
}

/* REG_0x46:ana_reg6->pwd_gadc_buf:0x46[15],gadc input buffer pwd,0,W*/
static inline uint32_t sys_ll_get_ana_reg6_pwd_gadc_buf(sys_hw_t *hw)
{
    return hw->ana_reg6.pwd_gadc_buf;
}

static inline void sys_ll_set_ana_reg6_pwd_gadc_buf(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.pwd_gadc_buf = value;
}

/* REG_0x46:ana_reg6->vaon_sel:0x46[17],0:vddaon drop enable,1,W*/
static inline uint32_t sys_ll_get_ana_reg6_vaon_sel(sys_hw_t *hw)
{
    return hw->ana_reg6.vaon_sel;
}

static inline void sys_ll_set_ana_reg6_vaon_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.vaon_sel = value;
}

/* REG_0x46:ana_reg6->xtal_hpsrr_en:0x46[18],xtal high psrr buffer enable,1,W*/
static inline uint32_t sys_ll_get_ana_reg6_xtal_hpsrr_en(sys_hw_t *hw)
{
    return hw->ana_reg6.xtal_hpsrr_en;
}

static inline void sys_ll_set_ana_reg6_xtal_hpsrr_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.xtal_hpsrr_en = value;
}

/* REG_0x46:ana_reg6->en_xtal2rf:0x46[19],xtal clock to rfpll gate enable ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_xtal2rf(sys_hw_t *hw)
{
    return hw->ana_reg6.en_xtal2rf;
}

static inline void sys_ll_set_ana_reg6_en_xtal2rf(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_xtal2rf = value;
}

/* REG_0x46:ana_reg6->en_sleep:0x46[20],xtal sleep enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_sleep(sys_hw_t *hw)
{
    return hw->ana_reg6.en_sleep;
}

static inline void sys_ll_set_ana_reg6_en_sleep(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.en_sleep = value;
}

/* REG_0x46:ana_reg6->clkbuf_hd:0x46[21],xtal lpsrr clock buffer high power mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_clkbuf_hd(sys_hw_t *hw)
{
    return hw->ana_reg6.clkbuf_hd;
}

static inline void sys_ll_set_ana_reg6_clkbuf_hd(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.clkbuf_hd = value;
}

/* REG_0x46:ana_reg6->clkbuf_dsel_manu:0x46[22],xtal lpsrr clock buffer power mode selection 0: auto /1:manu ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_clkbuf_dsel_manu(sys_hw_t *hw)
{
    return hw->ana_reg6.clkbuf_dsel_manu;
}

static inline void sys_ll_set_ana_reg6_clkbuf_dsel_manu(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.clkbuf_dsel_manu = value;
}

/* REG_0x46:ana_reg6->xtal_lpmode_ctrl:0x46[23],xtal core low power mode enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_xtal_lpmode_ctrl(sys_hw_t *hw)
{
    return hw->ana_reg6.xtal_lpmode_ctrl;
}

static inline void sys_ll_set_ana_reg6_xtal_lpmode_ctrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.xtal_lpmode_ctrl = value;
}

/* REG_0x46:ana_reg6->rxtal_lp:0x46[27:24],xtal bias current setting at low power mode ,F,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_rxtal_lp(sys_hw_t *hw)
{
    return hw->ana_reg6.rxtal_lp;
}

static inline void sys_ll_set_ana_reg6_rxtal_lp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.rxtal_lp = value;
}

/* REG_0x46:ana_reg6->rxtal_hp:0x46[31:28],xtal26m bias current setting at high power mode ,F,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_rxtal_hp(sys_hw_t *hw)
{
    return hw->ana_reg6.rxtal_hp;
}

static inline void sys_ll_set_ana_reg6_rxtal_hp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg6.rxtal_hp = value;
}

/* REG_0x47 */

static inline uint32_t sys_ll_get_ana_reg7_value(sys_hw_t *hw)
{
    return hw->ana_reg7.v;
}

static inline void sys_ll_set_ana_reg7_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.v = value;
}

/* REG_0x47:ana_reg7->rng_tstck_sel:0x47[0],trng setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_rng_tstck_sel(sys_hw_t *hw)
{
    return hw->ana_reg7.rng_tstck_sel;
}

static inline void sys_ll_set_ana_reg7_rng_tstck_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.rng_tstck_sel = value;
}

/* REG_0x47:ana_reg7->rng_tsten:0x47[1],trng setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_rng_tsten(sys_hw_t *hw)
{
    return hw->ana_reg7.rng_tsten;
}

static inline void sys_ll_set_ana_reg7_rng_tsten(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.rng_tsten = value;
}

/* REG_0x47:ana_reg7->itune_ref:0x47[4:2],trng setting,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_itune_ref(sys_hw_t *hw)
{
    return hw->ana_reg7.itune_ref;
}

static inline void sys_ll_set_ana_reg7_itune_ref(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.itune_ref = value;
}

/* REG_0x47:ana_reg7->itune_opa:0x47[7:5],trng setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_itune_opa(sys_hw_t *hw)
{
    return hw->ana_reg7.itune_opa;
}

static inline void sys_ll_set_ana_reg7_itune_opa(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.itune_opa = value;
}

/* REG_0x47:ana_reg7->itune_cmp:0x47[10:8],trng setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_itune_cmp(sys_hw_t *hw)
{
    return hw->ana_reg7.itune_cmp;
}

static inline void sys_ll_set_ana_reg7_itune_cmp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.itune_cmp = value;
}

/* REG_0x47:ana_reg7->Rnooise_sel:0x47[11],trng setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_rnooise_sel(sys_hw_t *hw)
{
    return hw->ana_reg7.rnooise_sel;
}

static inline void sys_ll_set_ana_reg7_rnooise_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.rnooise_sel = value;
}

/* REG_0x47:ana_reg7->Fslow_sel:0x47[14:12],trng setting,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_fslow_sel(sys_hw_t *hw)
{
    return hw->ana_reg7.fslow_sel;
}

static inline void sys_ll_set_ana_reg7_fslow_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.fslow_sel = value;
}

/* REG_0x47:ana_reg7->Ffast_sel:0x47[18:15],trng setting,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_ffast_sel(sys_hw_t *hw)
{
    return hw->ana_reg7.ffast_sel;
}

static inline void sys_ll_set_ana_reg7_ffast_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.ffast_sel = value;
}

/* REG_0x47:ana_reg7->gadc_cal_sel:0x47[20:19],gadc calibration mode selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_cal_sel(sys_hw_t *hw)
{
    return hw->ana_reg7.gadc_cal_sel;
}

static inline void sys_ll_set_ana_reg7_gadc_cal_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.gadc_cal_sel = value;
}

/* REG_0x47:ana_reg7->gadc_cmp_ictrl:0x47[25:22],gadc comparaor current select ,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_cmp_ictrl(sys_hw_t *hw)
{
    return hw->ana_reg7.gadc_cmp_ictrl;
}

static inline void sys_ll_set_ana_reg7_gadc_cmp_ictrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.gadc_cmp_ictrl = value;
}

/* REG_0x47:ana_reg7->gadc_buf_ictrl:0x47[29:26],gadc buffer current select ,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_buf_ictrl(sys_hw_t *hw)
{
    return hw->ana_reg7.gadc_buf_ictrl;
}

static inline void sys_ll_set_ana_reg7_gadc_buf_ictrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.gadc_buf_ictrl = value;
}

/* REG_0x47:ana_reg7->vref_sel:0x47[30],gadc input reference select, 0：bandgap signal 1:GPIO voltage divided,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_vref_sel(sys_hw_t *hw)
{
    return hw->ana_reg7.vref_sel;
}

static inline void sys_ll_set_ana_reg7_vref_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.vref_sel = value;
}

/* REG_0x47:ana_reg7->scal_en:0x47[31],gadc reference scale enable, 0:normal mode,1: scale mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_scal_en(sys_hw_t *hw)
{
    return hw->ana_reg7.scal_en;
}

static inline void sys_ll_set_ana_reg7_scal_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg7.scal_en = value;
}

/* REG_0x48 */

static inline uint32_t sys_ll_get_ana_reg8_value(sys_hw_t *hw)
{
    return hw->ana_reg8.v;
}

static inline void sys_ll_set_ana_reg8_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.v = value;
}

/* REG_0x48:ana_reg8->cap_calspi:0x48[8:0],manul mode ,input cap calibretion value,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_cap_calspi(sys_hw_t *hw)
{
    return hw->ana_reg8.cap_calspi;
}

static inline void sys_ll_set_ana_reg8_cap_calspi(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.cap_calspi = value;
}

/* REG_0x48:ana_reg8->gain_s:0x48[10:9],Sensitivity level selection,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_gain_s(sys_hw_t *hw)
{
    return hw->ana_reg8.gain_s;
}

static inline void sys_ll_set_ana_reg8_gain_s(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.gain_s = value;
}

/* REG_0x48:ana_reg8->pwd_td:0x48[11],power down touch module,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_pwd_td(sys_hw_t *hw)
{
    return hw->ana_reg8.pwd_td;
}

static inline void sys_ll_set_ana_reg8_pwd_td(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.pwd_td = value;
}

/* REG_0x48:ana_reg8->en_fsr:0x48[12],low power mode ,enable fast response,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_fsr(sys_hw_t *hw)
{
    return hw->ana_reg8.en_fsr;
}

static inline void sys_ll_set_ana_reg8_en_fsr(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.en_fsr = value;
}

/* REG_0x48:ana_reg8->en_scm:0x48[13],scan mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_scm(sys_hw_t *hw)
{
    return hw->ana_reg8.en_scm;
}

static inline void sys_ll_set_ana_reg8_en_scm(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.en_scm = value;
}

/* REG_0x48:ana_reg8->en_adcmode:0x48[14],adc mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_adcmode(sys_hw_t *hw)
{
    return hw->ana_reg8.en_adcmode;
}

static inline void sys_ll_set_ana_reg8_en_adcmode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.en_adcmode = value;
}

/* REG_0x48:ana_reg8->en_lpmode:0x48[15],low power mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_lpmode(sys_hw_t *hw)
{
    return hw->ana_reg8.en_lpmode;
}

static inline void sys_ll_set_ana_reg8_en_lpmode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.en_lpmode = value;
}

/* REG_0x48:ana_reg8->chs_scan:0x48[31:16],scan mode chan selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_chs_scan(sys_hw_t *hw)
{
    return hw->ana_reg8.chs_scan;
}

static inline void sys_ll_set_ana_reg8_chs_scan(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg8.chs_scan = value;
}

/* REG_0x49 */

static inline uint32_t sys_ll_get_ana_reg9_value(sys_hw_t *hw)
{
    return hw->ana_reg9.v;
}

static inline void sys_ll_set_ana_reg9_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.v = value;
}

/* REG_0x49:ana_reg9->en_otp_spi:0x49[0],otp ldo spi enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_en_otp_spi(sys_hw_t *hw)
{
    return hw->ana_reg9.en_otp_spi;
}

static inline void sys_ll_set_ana_reg9_en_otp_spi(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.en_otp_spi = value;
}

/* REG_0x49:ana_reg9->entemp2:0x49[1],dummy,0,W*/
static inline uint32_t sys_ll_get_ana_reg9_entemp2(sys_hw_t *hw)
{
    return hw->ana_reg9.entemp2;
}
static inline void sys_ll_set_ana_reg9_entemp2(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.entemp2 = value;
}

/* REG_0x49:ana_reg9->vtempsel:0x49[3:2],00:nc  01:vtemp  10:vbe   11:vbg1p3,0,W*/
static inline uint32_t sys_ll_get_ana_reg9_vtempsel(sys_hw_t *hw)
{
    return hw->ana_reg9.vtempsel;
}
static inline void sys_ll_set_ana_reg9_vtempsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.vtempsel = value;
}

/* REG_0x49:ana_reg9->vtsel:0x49[4],dummy,1,W*/
static inline uint32_t sys_ll_get_ana_reg9_vtsel(sys_hw_t *hw)
{
    return hw->ana_reg9.vtsel;
}
static inline void sys_ll_set_ana_reg9_vtsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.vtsel = value;
}

/* REG_0x49:ana_reg9->en_bias_5u:0x49[5],Ibias 5u enable,1,W*/
static inline uint32_t sys_ll_get_ana_reg9_en_bias_5u(sys_hw_t *hw)
{
    return hw->ana_reg9.en_bias_5u;
}
static inline void sys_ll_set_ana_reg9_en_bias_5u(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.en_bias_5u = value;
}

/* REG_0x49:ana_reg9->dummy2:0x49[6],5uA channel on(for PLL & DCO),1,W*/
static inline uint32_t sys_ll_get_ana_reg9_dummy2(sys_hw_t *hw)
{
    return hw->ana_reg9.dummy2;
}

static inline void sys_ll_set_ana_reg9_dummy2(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.dummy2 = value;
}

/* REG_0x49:ana_reg9->touch_serial_cap:0x49[7],1:touch serial capacitors 6pF,1,W*/
static inline uint32_t sys_ll_get_ana_reg9_touch_serial_cap(sys_hw_t *hw)
{
    return hw->ana_reg9.touch_serial_cap;
}
static inline void sys_ll_set_ana_reg9_touch_serial_cap(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.touch_serial_cap = value;
}

/* REG_0x49:ana_reg9->buckfb_czenb:0x49[8],buck EA feedback cz selection,1,W*/
static inline uint32_t sys_ll_get_ana_reg9_buckfb_czenb(sys_hw_t *hw)
{
    return hw->ana_reg9.buckfb_czenb;
}

static inline void sys_ll_set_ana_reg9_buckfb_czenb(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.buckfb_czenb = value;
}

/* REG_0x49:ana_reg9->buckea_cur_ctrl:0x49[10:9],buck EA ibias selection,3,W*/
static inline uint32_t sys_ll_get_ana_reg9_buckea_cur_ctrl(sys_hw_t *hw)
{
    return hw->ana_reg9.buckea_cur_ctrl;
}

static inline void sys_ll_set_ana_reg9_buckea_cur_ctrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.buckea_cur_ctrl = value;
}

/* REG_0x49:ana_reg9->cbtst_en:0x49[11],CB test enable,0,W*/
static inline uint32_t sys_ll_get_ana_reg9_cbtst_en(sys_hw_t *hw)
{
    return hw->ana_reg9.cbtst_en;
}

static inline void sys_ll_set_ana_reg9_cbtst_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.cbtst_en = value;
}

/* REG_0x49:ana_reg9->psldo_vsel:0x49[12],psldo voltage selsection,0,W*/
static inline uint32_t sys_ll_get_ana_reg9_psldo_vsel(sys_hw_t *hw)
{
    return hw->ana_reg9.psldo_vsel;
}

static inline void sys_ll_set_ana_reg9_psldo_vsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.psldo_vsel = value;
}

/* REG_0x49:ana_reg9->ovr_l:0x49[13],ovr low enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_ovr_l(sys_hw_t *hw)
{
    return hw->ana_reg9.ovr_l;
}

static inline void sys_ll_set_ana_reg9_ovr_l(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.ovr_l = value;
}

/* REG_0x49:ana_reg9->usbpen:0x49[17:14],usb dp driver capability control,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usbpen(sys_hw_t *hw)
{
    return hw->ana_reg9.usbpen;
}

static inline void sys_ll_set_ana_reg9_usbpen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.usbpen = value;
}

/* REG_0x49:ana_reg9->usbnen:0x49[21:18],usb dn driver capability control,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usbnen(sys_hw_t *hw)
{
    return hw->ana_reg9.usbnen;
}

static inline void sys_ll_set_ana_reg9_usbnen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.usbnen = value;
}

/* REG_0x49:ana_reg9->usb_speed:0x49[22],usb speed selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usb_speed(sys_hw_t *hw)
{
    return hw->ana_reg9.usb_speed;
}

static inline void sys_ll_set_ana_reg9_usb_speed(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.usb_speed = value;
}

/* REG_0x49:ana_reg9->usb_deepsleep:0x49[23],usb deepsleep mode enable by spi,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usb_deepsleep(sys_hw_t *hw)
{
    return hw->ana_reg9.usb_deepsleep;
}

static inline void sys_ll_set_ana_reg9_usb_deepsleep(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.usb_deepsleep = value;
}

/* REG_0x49:ana_reg9->man_mode:0x49[24],manul mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_man_mode(sys_hw_t *hw)
{
    return hw->ana_reg9.man_mode;
}

static inline void sys_ll_set_ana_reg9_man_mode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.man_mode = value;
}

/* REG_0x49:ana_reg9->crg:0x49[26:25],detect range selection :8pF/12pF/19pF/27pF,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_crg(sys_hw_t *hw)
{
    return hw->ana_reg9.crg;
}

static inline void sys_ll_set_ana_reg9_crg(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.crg = value;
}

/* REG_0x49:ana_reg9->vrefs:0x49[29:27],detect threshold selection ,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_vrefs(sys_hw_t *hw)
{
    return hw->ana_reg9.vrefs;
}

static inline void sys_ll_set_ana_reg9_vrefs(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.vrefs = value;
}

/* REG_0x49:ana_reg9->en_cal:0x49[31],calibretion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_en_cal(sys_hw_t *hw)
{
    return hw->ana_reg9.en_cal;
}

static inline void sys_ll_set_ana_reg9_en_cal(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg9.en_cal = value;
}

/* REG_0x4a */

static inline uint32_t sys_ll_get_ana_reg10_value(sys_hw_t *hw)
{
    return hw->ana_reg10.v;
}

static inline void sys_ll_set_ana_reg10_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg10.v = value;
}

/* REG_0x4a:ana_reg10->sdm_val:0x4a[29:0],audio pll sdm value,0F1FAA45,R/W*/
static inline uint32_t sys_ll_get_ana_reg10_sdm_val(sys_hw_t *hw)
{
    return hw->ana_reg10.sdm_val;
}

static inline void sys_ll_set_ana_reg10_sdm_val(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg10.sdm_val = value;
}

/* REG_0x4a:ana_reg10->vco_hfreq_enb:0x4a[30],audio pll vco high frequency enb,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg10_vco_hfreq_enb(sys_hw_t *hw)
{
    return hw->ana_reg10.vco_hfreq_enb;
}

static inline void sys_ll_set_ana_reg10_vco_hfreq_enb(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg10.vco_hfreq_enb = value;
}

/* REG_0x4a:ana_reg10->cal_refen:0x4a[31],cal_ref enable of audio pll,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg10_cal_refen(sys_hw_t *hw)
{
    return hw->ana_reg10.cal_refen;
}

static inline void sys_ll_set_ana_reg10_cal_refen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg10.cal_refen = value;
}

/* REG_0x4b */

static inline uint32_t sys_ll_get_ana_reg11_value(sys_hw_t *hw)
{
    return hw->ana_reg11.v;
}

static inline void sys_ll_set_ana_reg11_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.v = value;
}

/* REG_0x4b:ana_reg11->int_mod:0x4b[0],DPLL integer mode enable; 0: fractional mode; 1: integer mode,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_int_mod(sys_hw_t *hw)
{
    return hw->ana_reg11.int_mod;
}

static inline void sys_ll_set_ana_reg11_int_mod(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.int_mod = value;
}

/* REG_0x4b:ana_reg11->Nsyn:0x4b[1],DPLL Ncoutner reset ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_nsyn(sys_hw_t *hw)
{
    return hw->ana_reg11.nsyn;
}

static inline void sys_ll_set_ana_reg11_nsyn(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.nsyn = value;
}

/* REG_0x4b:ana_reg11->open_enb:0x4b[2], ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_open_enb(sys_hw_t *hw)
{
    return hw->ana_reg11.open_enb;
}

static inline void sys_ll_set_ana_reg11_open_enb(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.open_enb = value;
}

/* REG_0x4b:ana_reg11->reset:0x4b[3],DPLL reset,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_reset(sys_hw_t *hw)
{
    return hw->ana_reg11.reset;
}

static inline void sys_ll_set_ana_reg11_reset(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.reset = value;
}

/* REG_0x4b:ana_reg11->Ioffset:0x4b[6:4],DPLL  charge pump offset current control,5,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ioffset(sys_hw_t *hw)
{
    return hw->ana_reg11.ioffset;
}

static inline void sys_ll_set_ana_reg11_ioffset(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.ioffset = value;
}

/* REG_0x4b:ana_reg11->LPFRz:0x4b[10:7],DPLL Rz control of LPF,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_lpfrz(sys_hw_t *hw)
{
    return hw->ana_reg11.lpfrz;
}

static inline void sys_ll_set_ana_reg11_lpfrz(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.lpfrz = value;
}

/* REG_0x4b:ana_reg11->vsel:0x4b[13:11],DPLL vtrl selection during VCO band calibration,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_vsel(sys_hw_t *hw)
{
    return hw->ana_reg11.vsel;
}

static inline void sys_ll_set_ana_reg11_vsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.vsel = value;
}

/* REG_0x4b:ana_reg11->vsel_cal:0x4b[14], selection during VCO band calibration,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_vsel_cal(sys_hw_t *hw)
{
    return hw->ana_reg11.vsel_cal;
}

static inline void sys_ll_set_ana_reg11_vsel_cal(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.vsel_cal = value;
}

/* REG_0x4b:ana_reg11->pwd_lockdet:0x4b[15], ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_pwd_lockdet(sys_hw_t *hw)
{
    return hw->ana_reg11.pwd_lockdet;
}

static inline void sys_ll_set_ana_reg11_pwd_lockdet(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.pwd_lockdet = value;
}

/* REG_0x4b:ana_reg11->lockdet_bypass:0x4b[16], ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_lockdet_bypass(sys_hw_t *hw)
{
    return hw->ana_reg11.lockdet_bypass;
}

static inline void sys_ll_set_ana_reg11_lockdet_bypass(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.lockdet_bypass = value;
}

/* REG_0x4b:ana_reg11->ckref_loop_sel:0x4b[17],polarity selection of referenc clock  to SDM,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ckref_loop_sel(sys_hw_t *hw)
{
    return hw->ana_reg11.ckref_loop_sel;
}

static inline void sys_ll_set_ana_reg11_ckref_loop_sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.ckref_loop_sel = value;
}

/* REG_0x4b:ana_reg11->spi_trigger:0x4b[18],DPLL band calibration spi trigger,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_spi_trigger(sys_hw_t *hw)
{
    return hw->ana_reg11.spi_trigger;
}

static inline void sys_ll_set_ana_reg11_spi_trigger(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.spi_trigger = value;
}

/* REG_0x4b:ana_reg11->manual:0x4b[19],DPLL VCO band manual enable; 0: auto mode; 1: manual mode,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_manual(sys_hw_t *hw)
{
    return hw->ana_reg11.manual;
}

static inline void sys_ll_set_ana_reg11_manual(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.manual = value;
}

/* REG_0x4b:ana_reg11->test_en:0x4b[20],test enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_test_en(sys_hw_t *hw)
{
    return hw->ana_reg11.test_en;
}

static inline void sys_ll_set_ana_reg11_test_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.test_en = value;
}

/* REG_0x4b:ana_reg11->Icp:0x4b[23:22],DPLL charge pump current control; ,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_icp(sys_hw_t *hw)
{
    return hw->ana_reg11.icp;
}

static inline void sys_ll_set_ana_reg11_icp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.icp = value;
}

/* REG_0x4b:ana_reg11->ck26Men:0x4b[24],xtal26M clock for audio enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ck26men(sys_hw_t *hw)
{
    return hw->ana_reg11.ck26men;
}

static inline void sys_ll_set_ana_reg11_ck26men(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.ck26men = value;
}

/* REG_0x4b:ana_reg11->ckaudio_outen:0x4b[25],DPLL clock output to PAD enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ckaudio_outen(sys_hw_t *hw)
{
    return hw->ana_reg11.ckaudio_outen;
}

static inline void sys_ll_set_ana_reg11_ckaudio_outen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.ckaudio_outen = value;
}

/* REG_0x4b:ana_reg11->divctrl:0x4b[28:26],DPLL divider control; 000: div1; 001: div2; 010: div4; 011: div8; 1xx: div16,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_divctrl(sys_hw_t *hw)
{
    return hw->ana_reg11.divctrl;
}

static inline void sys_ll_set_ana_reg11_divctrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.divctrl = value;
}

/* REG_0x4b:ana_reg11->cksel:0x4b[29],DPLL divider control; 0: div3; 1: div4,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_cksel(sys_hw_t *hw)
{
    return hw->ana_reg11.cksel;
}

static inline void sys_ll_set_ana_reg11_cksel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.cksel = value;
}

/* REG_0x4b:ana_reg11->ck2mcu:0x4b[30],DPLL clock for mcu enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ck2mcu(sys_hw_t *hw)
{
    return hw->ana_reg11.ck2mcu;
}

static inline void sys_ll_set_ana_reg11_ck2mcu(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.ck2mcu = value;
}

/* REG_0x4b:ana_reg11->audioen:0x4b[31],DPLL clock for audio enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_audioen(sys_hw_t *hw)
{
    return hw->ana_reg11.audioen;
}

static inline void sys_ll_set_ana_reg11_audioen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg11.audioen = value;
}

/* REG_0x4c */

static inline uint32_t sys_ll_get_ana_reg12_value(sys_hw_t *hw)
{
    return hw->ana_reg12.v;
}

static inline void sys_ll_set_ana_reg12_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.v = value;
}

/* REG_0x4c:ana_reg12->digmic_ckinv:0x4c[2],digmic clock inversion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_digmic_ckinv(sys_hw_t *hw)
{
    return hw->ana_reg12.digmic_ckinv;
}

static inline void sys_ll_set_ana_reg12_digmic_ckinv(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.digmic_ckinv = value;
}

/* REG_0x4c:ana_reg12->enmicdig:0x4c[3],digmic enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enmicdig(sys_hw_t *hw)
{
    return hw->ana_reg12.enmicdig;
}

static inline void sys_ll_set_ana_reg12_enmicdig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.enmicdig = value;
}

/* REG_0x4c:ana_reg12->audck_rlcen:0x4c[4],audio clock re-latch enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_audck_rlcen(sys_hw_t *hw)
{
    return hw->ana_reg12.audck_rlcen;
}

static inline void sys_ll_set_ana_reg12_audck_rlcen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.audck_rlcen = value;
}

/* REG_0x4c:ana_reg12->lchckinven:0x4c[5],audio clock re-latch clock inversion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_lchckinven(sys_hw_t *hw)
{
    return hw->ana_reg12.lchckinven;
}

static inline void sys_ll_set_ana_reg12_lchckinven(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.lchckinven = value;
}

/* REG_0x4c:ana_reg12->ldo1v_vsel1v:0x4c[8:6],audio 1.0V LDO selection, 000=0.8, 1X1=1.0,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_ldo1v_vsel1v(sys_hw_t *hw)
{
    return hw->ana_reg12.ldo1v_vsel1v;
}

static inline void sys_ll_set_ana_reg12_ldo1v_vsel1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.ldo1v_vsel1v = value;
}

/* REG_0x4c:ana_reg12->ldo1v_adj:0x4c[13:9],audio 1.0V LDO output trimming, 00000=min, 11111=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_ldo1v_adj(sys_hw_t *hw)
{
    return hw->ana_reg12.ldo1v_adj;
}

static inline void sys_ll_set_ana_reg12_ldo1v_adj(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.ldo1v_adj = value;
}

/* REG_0x4c:ana_reg12->audvdd_trm1v:0x4c[15:14],audio 1.5V LDO selection, 00=min, 11=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_audvdd_trm1v(sys_hw_t *hw)
{
    return hw->ana_reg12.audvdd_trm1v;
}

static inline void sys_ll_set_ana_reg12_audvdd_trm1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.audvdd_trm1v = value;
}

/* REG_0x4c:ana_reg12->audvdd_voc1v:0x4c[20:16],audio 1.5V LDO output trimming, 00000=min, 11111=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_audvdd_voc1v(sys_hw_t *hw)
{
    return hw->ana_reg12.audvdd_voc1v;
}

static inline void sys_ll_set_ana_reg12_audvdd_voc1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.audvdd_voc1v = value;
}

/* REG_0x4c:ana_reg12->enaudvdd1v:0x4c[21],audio 1.0V LDO enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enaudvdd1v(sys_hw_t *hw)
{
    return hw->ana_reg12.enaudvdd1v;
}

static inline void sys_ll_set_ana_reg12_enaudvdd1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.enaudvdd1v = value;
}

/* REG_0x4c:ana_reg12->loadhp:0x4c[22],audio 1.5V LDO, 1=good stability with small loading,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_loadhp(sys_hw_t *hw)
{
    return hw->ana_reg12.loadhp;
}

static inline void sys_ll_set_ana_reg12_loadhp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.loadhp = value;
}

/* REG_0x4c:ana_reg12->enaudvdd1v5:0x4c[23],audio 1.5V LDO enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enaudvdd1v5(sys_hw_t *hw)
{
    return hw->ana_reg12.enaudvdd1v5;
}

static inline void sys_ll_set_ana_reg12_enaudvdd1v5(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.enaudvdd1v5 = value;
}

/* REG_0x4c:ana_reg12->enmicbias1v:0x4c[24],micbias enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enmicbias1v(sys_hw_t *hw)
{
    return hw->ana_reg12.enmicbias1v;
}

static inline void sys_ll_set_ana_reg12_enmicbias1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.enmicbias1v = value;
}

/* REG_0x4c:ana_reg12->micbias_trim:0x4c[26:25],micbias output selection, 00=min, 11=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_micbias_trim(sys_hw_t *hw)
{
    return hw->ana_reg12.micbias_trim;
}

static inline void sys_ll_set_ana_reg12_micbias_trim(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.micbias_trim = value;
}

/* REG_0x4c:ana_reg12->micbias_voc1v:0x4c[31:27],micbias output trimming, 00000=min, 11111=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_micbias_voc1v(sys_hw_t *hw)
{
    return hw->ana_reg12.micbias_voc1v;
}

static inline void sys_ll_set_ana_reg12_micbias_voc1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg12.micbias_voc1v = value;
}

/* REG_0x4d */

static inline uint32_t sys_ll_get_ana_reg13_value(sys_hw_t *hw)
{
    return hw->ana_reg13.v;
}

static inline void sys_ll_set_ana_reg13_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.v = value;
}

/* REG_0x4d:ana_reg13->byp_dwaadc:0x4d[8],adc dwa pass enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_byp_dwaadc(sys_hw_t *hw)
{
    return hw->ana_reg13.byp_dwaadc;
}

static inline void sys_ll_set_ana_reg13_byp_dwaadc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.byp_dwaadc = value;
}

/* REG_0x4d:ana_reg13->rst:0x4d[9],rst,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_rst(sys_hw_t *hw)
{
    return hw->ana_reg13.rst;
}

static inline void sys_ll_set_ana_reg13_rst(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.rst = value;
}

/* REG_0x4d:ana_reg13->adcdwa_mode:0x4d[10],adc dwa model sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_adcdwa_mode(sys_hw_t *hw)
{
    return hw->ana_reg13.adcdwa_mode;
}

static inline void sys_ll_set_ana_reg13_adcdwa_mode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.adcdwa_mode = value;
}

/* REG_0x4d:ana_reg13->vodadjspi:0x4d[15:11],adc reference manual spi control,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_vodadjspi(sys_hw_t *hw)
{
    return hw->ana_reg13.vodadjspi;
}

static inline void sys_ll_set_ana_reg13_vodadjspi(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.vodadjspi = value;
}

/* REG_0x4d:ana_reg13->refvsel:0x4d[21],0= high reference; 1=small reference,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_refvsel(sys_hw_t *hw)
{
    return hw->ana_reg13.refvsel;
}

static inline void sys_ll_set_ana_reg13_refvsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.refvsel = value;
}

/* REG_0x4d:ana_reg13->capsw1v:0x4d[27:23],munual value for cap trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_capsw1v(sys_hw_t *hw)
{
    return hw->ana_reg13.capsw1v;
}

static inline void sys_ll_set_ana_reg13_capsw1v(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.capsw1v = value;
}

/* REG_0x4d:ana_reg13->adcckinven:0x4d[30],audio adc clock inversion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_adcckinven(sys_hw_t *hw)
{
    return hw->ana_reg13.adcckinven;
}

static inline void sys_ll_set_ana_reg13_adcckinven(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg13.adcckinven = value;
}

/* REG_0x4e */

static inline uint32_t sys_ll_get_ana_reg14_value(sys_hw_t *hw)
{
    return hw->ana_reg14.v;
}

static inline void sys_ll_set_ana_reg14_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.v = value;
}

/* REG_0x4e:ana_reg14->isel:0x4e[1:0],adc bias trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_isel(sys_hw_t *hw)
{
    return hw->ana_reg14.isel;
}

static inline void sys_ll_set_ana_reg14_isel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.isel = value;
}

/* REG_0x4e:ana_reg14->micdcocdin:0x4e[9:2],adc micmode dcoc din,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocdin(sys_hw_t *hw)
{
    return hw->ana_reg14.micdcocdin;
}

static inline void sys_ll_set_ana_reg14_micdcocdin(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdcocdin = value;
}

/* REG_0x4e:ana_reg14->micdcocvc:0x4e[11:10],adc micmode dcoc control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocvc(sys_hw_t *hw)
{
    return hw->ana_reg14.micdcocvc;
}

static inline void sys_ll_set_ana_reg14_micdcocvc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdcocvc = value;
}

/* REG_0x4e:ana_reg14->micdcocen_n:0x4e[12],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocen_n(sys_hw_t *hw)
{
    return hw->ana_reg14.micdcocen_n;
}

static inline void sys_ll_set_ana_reg14_micdcocen_n(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdcocen_n = value;
}

/* REG_0x4e:ana_reg14->micdcocen_p:0x4e[13],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocen_p(sys_hw_t *hw)
{
    return hw->ana_reg14.micdcocen_p;
}

static inline void sys_ll_set_ana_reg14_micdcocen_p(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdcocen_p = value;
}

/* REG_0x4e:ana_reg14->micsingleEn:0x4e[14],adc micmode, single_end enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micsingleen(sys_hw_t *hw)
{
    return hw->ana_reg14.micsingleen;
}

static inline void sys_ll_set_ana_reg14_micsingleen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micsingleen = value;
}

/* REG_0x4e:ana_reg14->micGain:0x4e[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micgain(sys_hw_t *hw)
{
    return hw->ana_reg14.micgain;
}

static inline void sys_ll_set_ana_reg14_micgain(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micgain = value;
}

/* REG_0x4e:ana_reg14->micdacen:0x4e[19],adc micmode micdac enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdacen(sys_hw_t *hw)
{
    return hw->ana_reg14.micdacen;
}

static inline void sys_ll_set_ana_reg14_micdacen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdacen = value;
}

/* REG_0x4e:ana_reg14->micdaciH:0x4e[27:20],adc micmode, micdac input ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdacih(sys_hw_t *hw)
{
    return hw->ana_reg14.micdacih;
}

static inline void sys_ll_set_ana_reg14_micdacih(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdacih = value;
}

/* REG_0x4e:ana_reg14->micdacit:0x4e[29:28],adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdacit(sys_hw_t *hw)
{
    return hw->ana_reg14.micdacit;
}

static inline void sys_ll_set_ana_reg14_micdacit(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micdacit = value;
}

/* REG_0x4e:ana_reg14->hcen:0x4e[30],adc 1stg op current trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_hcen(sys_hw_t *hw)
{
    return hw->ana_reg14.hcen;
}

static inline void sys_ll_set_ana_reg14_hcen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.hcen = value;
}

/* REG_0x4e:ana_reg14->micEn:0x4e[31],mic1 mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micen(sys_hw_t *hw)
{
    return hw->ana_reg14.micen;
}

static inline void sys_ll_set_ana_reg14_micen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg14.micen = value;
}

/* REG_0x4f */

static inline uint32_t sys_ll_get_ana_reg15_value(sys_hw_t *hw)
{
    return hw->ana_reg15.v;
}

static inline void sys_ll_set_ana_reg15_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.v = value;
}

/* REG_0x4f:ana_reg15->isel:0x4f[1:0],adc bias trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_isel(sys_hw_t *hw)
{
    return hw->ana_reg15.isel;
}

static inline void sys_ll_set_ana_reg15_isel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.isel = value;
}

/* REG_0x4f:ana_reg15->micdcocdin:0x4f[9:2],adc micmode dcoc din,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocdin(sys_hw_t *hw)
{
    return hw->ana_reg15.micdcocdin;
}

static inline void sys_ll_set_ana_reg15_micdcocdin(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdcocdin = value;
}

/* REG_0x4f:ana_reg15->micdcocvc:0x4f[11:10],adc micmode dcoc control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocvc(sys_hw_t *hw)
{
    return hw->ana_reg15.micdcocvc;
}

static inline void sys_ll_set_ana_reg15_micdcocvc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdcocvc = value;
}

/* REG_0x4f:ana_reg15->micdcocen_n:0x4f[12],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocen_n(sys_hw_t *hw)
{
    return hw->ana_reg15.micdcocen_n;
}

static inline void sys_ll_set_ana_reg15_micdcocen_n(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdcocen_n = value;
}

/* REG_0x4f:ana_reg15->micdcocen_p:0x4f[13],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocen_p(sys_hw_t *hw)
{
    return hw->ana_reg15.micdcocen_p;
}

static inline void sys_ll_set_ana_reg15_micdcocen_p(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdcocen_p = value;
}

/* REG_0x4f:ana_reg15->micsingleEn:0x4f[14],adc micmode, single_end enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micsingleen(sys_hw_t *hw)
{
    return hw->ana_reg15.micsingleen;
}

static inline void sys_ll_set_ana_reg15_micsingleen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micsingleen = value;
}

/* REG_0x4f:ana_reg15->micGain:0x4f[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micgain(sys_hw_t *hw)
{
    return hw->ana_reg15.micgain;
}

static inline void sys_ll_set_ana_reg15_micgain(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micgain = value;
}

/* REG_0x4f:ana_reg15->micdacen:0x4f[19],adc micmode micdac enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdacen(sys_hw_t *hw)
{
    return hw->ana_reg15.micdacen;
}

static inline void sys_ll_set_ana_reg15_micdacen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdacen = value;
}

/* REG_0x4f:ana_reg15->micdaciH:0x4f[27:20],adc micmode, micdac input ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdacih(sys_hw_t *hw)
{
    return hw->ana_reg15.micdacih;
}

static inline void sys_ll_set_ana_reg15_micdacih(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdacih = value;
}

/* REG_0x4f:ana_reg15->micdacit:0x4f[29:28],adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdacit(sys_hw_t *hw)
{
    return hw->ana_reg15.micdacit;
}

static inline void sys_ll_set_ana_reg15_micdacit(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micdacit = value;
}

/* REG_0x4f:ana_reg15->hcen:0x4f[30],adc 1stg op current trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_hcen(sys_hw_t *hw)
{
    return hw->ana_reg15.hcen;
}

static inline void sys_ll_set_ana_reg15_hcen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.hcen = value;
}

/* REG_0x4f:ana_reg15->micEn:0x4f[31],mic2 mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micen(sys_hw_t *hw)
{
    return hw->ana_reg15.micen;
}

static inline void sys_ll_set_ana_reg15_micen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg15.micen = value;
}

/* REG_0x50 */

static inline uint32_t sys_ll_get_ana_reg16_value(sys_hw_t *hw)
{
    return hw->ana_reg16.v;
}

static inline void sys_ll_set_ana_reg16_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.v = value;
}

/* REG_0x50:ana_reg16->hpdac:0x50[0],class ab driver high current mode. "1" high current. ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_hpdac(sys_hw_t *hw)
{
    return hw->ana_reg16.hpdac;
}

static inline void sys_ll_set_ana_reg16_hpdac(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.hpdac = value;
}

/* REG_0x50:ana_reg16->vcmsdac:0x50[1],1stg OP input common model voltage selection. "1" low common mode voltage,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_vcmsdac(sys_hw_t *hw)
{
    return hw->ana_reg16.vcmsdac;
}

static inline void sys_ll_set_ana_reg16_vcmsdac(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.vcmsdac = value;
}

/* REG_0x50:ana_reg16->oscdac:0x50[3:2],threshold current setting for over current protection . "3" maximum current. "0" minimum current,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_oscdac(sys_hw_t *hw)
{
    return hw->ana_reg16.oscdac;
}

static inline void sys_ll_set_ana_reg16_oscdac(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.oscdac = value;
}

/* REG_0x50:ana_reg16->ocendac:0x50[4],over current protection enable. "1" enable.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_ocendac(sys_hw_t *hw)
{
    return hw->ana_reg16.ocendac;
}

static inline void sys_ll_set_ana_reg16_ocendac(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.ocendac = value;
}

/* REG_0x50:ana_reg16->isel_idac:0x50[5],idac current sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_isel_idac(sys_hw_t *hw)
{
    return hw->ana_reg16.isel_idac;
}

static inline void sys_ll_set_ana_reg16_isel_idac(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.isel_idac = value;
}

/* REG_0x50:ana_reg16->adjdacref:0x50[10:6],audio dac reference voltage adjust.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_adjdacref(sys_hw_t *hw)
{
    return hw->ana_reg16.adjdacref;
}

static inline void sys_ll_set_ana_reg16_adjdacref(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.adjdacref = value;
}

/* REG_0x50:ana_reg16->dcochg:0x50[12],dcoc high gain selection. "1" high gain,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dcochg(sys_hw_t *hw)
{
    return hw->ana_reg16.dcochg;
}

static inline void sys_ll_set_ana_reg16_dcochg(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.dcochg = value;
}

/* REG_0x50:ana_reg16->diffen:0x50[13],enable differential mode. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_diffen(sys_hw_t *hw)
{
    return hw->ana_reg16.diffen;
}

static inline void sys_ll_set_ana_reg16_diffen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.diffen = value;
}

/* REG_0x50:ana_reg16->endaccal:0x50[14],enable offset calibration process. "1" enable.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_endaccal(sys_hw_t *hw)
{
    return hw->ana_reg16.endaccal;
}

static inline void sys_ll_set_ana_reg16_endaccal(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.endaccal = value;
}

/* REG_0x50:ana_reg16->rendcoc:0x50[15],R-channel dcoc dac enablel. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_rendcoc(sys_hw_t *hw)
{
    return hw->ana_reg16.rendcoc;
}

static inline void sys_ll_set_ana_reg16_rendcoc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.rendcoc = value;
}

/* REG_0x50:ana_reg16->lendcoc:0x50[16],L-channel Dcoc dac enable. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_lendcoc(sys_hw_t *hw)
{
    return hw->ana_reg16.lendcoc;
}

static inline void sys_ll_set_ana_reg16_lendcoc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.lendcoc = value;
}

/* REG_0x50:ana_reg16->renvcmd:0x50[17],R-channel common mode output buffer enable."1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_renvcmd(sys_hw_t *hw)
{
    return hw->ana_reg16.renvcmd;
}

static inline void sys_ll_set_ana_reg16_renvcmd(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.renvcmd = value;
}

/* REG_0x50:ana_reg16->lenvcmd:0x50[18],L-channel common mode output buffer enable. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_lenvcmd(sys_hw_t *hw)
{
    return hw->ana_reg16.lenvcmd;
}

static inline void sys_ll_set_ana_reg16_lenvcmd(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.lenvcmd = value;
}

/* REG_0x50:ana_reg16->dacdrven:0x50[19],dac output driver enable."1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacdrven(sys_hw_t *hw)
{
    return hw->ana_reg16.dacdrven;
}

static inline void sys_ll_set_ana_reg16_dacdrven(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.dacdrven = value;
}

/* REG_0x50:ana_reg16->dacRen:0x50[20],dac R-channel enable. "1"  enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacren(sys_hw_t *hw)
{
    return hw->ana_reg16.dacren;
}

static inline void sys_ll_set_ana_reg16_dacren(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.dacren = value;
}

/* REG_0x50:ana_reg16->dacLen:0x50[21],dac L-channel enable. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_daclen(sys_hw_t *hw)
{
    return hw->ana_reg16.daclen;
}

static inline void sys_ll_set_ana_reg16_daclen(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.daclen = value;
}

/* REG_0x50:ana_reg16->dacG:0x50[24:22],dac gain setting: 000=0dB, 111=8dB,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacg(sys_hw_t *hw)
{
    return hw->ana_reg16.dacg;
}

static inline void sys_ll_set_ana_reg16_dacg(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.dacg = value;
}

/* REG_0x50:ana_reg16->ck4xsel:0x50[25],dac clock sel ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_ck4xsel(sys_hw_t *hw)
{
    return hw->ana_reg16.ck4xsel;
}

static inline void sys_ll_set_ana_reg16_ck4xsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.ck4xsel = value;
}

/* REG_0x50:ana_reg16->dacmute:0x50[26],dac mute enable. "1" mute enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacmute(sys_hw_t *hw)
{
    return hw->ana_reg16.dacmute;
}

static inline void sys_ll_set_ana_reg16_dacmute(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.dacmute = value;
}

/* REG_0x50:ana_reg16->dwamode:0x50[27],dac dwa mode sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dwamode(sys_hw_t *hw)
{
    return hw->ana_reg16.dwamode;
}

static inline void sys_ll_set_ana_reg16_dwamode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.dwamode = value;
}

/* REG_0x50:ana_reg16->ckposel:0x50[28],dac sample clock edge selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_ckposel(sys_hw_t *hw)
{
    return hw->ana_reg16.ckposel;
}

static inline void sys_ll_set_ana_reg16_ckposel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.ckposel = value;
}

/* REG_0x50:ana_reg16->byldo:0x50[31],bypass 1v8 LDO,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_byldo(sys_hw_t *hw)
{
    return hw->ana_reg16.byldo;
}

static inline void sys_ll_set_ana_reg16_byldo(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg16.byldo = value;
}

/* REG_0x51 */

static inline uint32_t sys_ll_get_ana_reg17_value(sys_hw_t *hw)
{
    return hw->ana_reg17.v;
}

static inline void sys_ll_set_ana_reg17_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.v = value;
}

/* REG_0x51:ana_reg17->lmdcin:0x51[7:0],l-cnannel offset cancel dac maumual input.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_lmdcin(sys_hw_t *hw)
{
    return hw->ana_reg17.lmdcin;
}

static inline void sys_ll_set_ana_reg17_lmdcin(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.lmdcin = value;
}

/* REG_0x51:ana_reg17->rmdcin:0x51[15:8],r-channel offset cancel dac manmual input ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_rmdcin(sys_hw_t *hw)
{
    return hw->ana_reg17.rmdcin;
}

static inline void sys_ll_set_ana_reg17_rmdcin(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.rmdcin = value;
}

/* REG_0x51:ana_reg17->spirst_ovc:0x51[16],ovc rst,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_spirst_ovc(sys_hw_t *hw)
{
    return hw->ana_reg17.spirst_ovc;
}

static inline void sys_ll_set_ana_reg17_spirst_ovc(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.spirst_ovc = value;
}

/* REG_0x51:ana_reg17->hc2s0v9:0x51[20],0=current is half,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_hc2s0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.hc2s0v9;
}

static inline void sys_ll_set_ana_reg17_hc2s0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.hc2s0v9 = value;
}

/* REG_0x51:ana_reg17->lvcmsel:0x51[21],low vcm sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_lvcmsel(sys_hw_t *hw)
{
    return hw->ana_reg17.lvcmsel;
}

static inline void sys_ll_set_ana_reg17_lvcmsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.lvcmsel = value;
}

/* REG_0x51:ana_reg17->loop2sel:0x51[22],2rd loop sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_loop2sel(sys_hw_t *hw)
{
    return hw->ana_reg17.loop2sel;
}

static inline void sys_ll_set_ana_reg17_loop2sel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.loop2sel = value;
}

/* REG_0x51:ana_reg17->enbias:0x51[23],dac bias enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_enbias(sys_hw_t *hw)
{
    return hw->ana_reg17.enbias;
}

static inline void sys_ll_set_ana_reg17_enbias(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.enbias = value;
}

/* REG_0x51:ana_reg17->calck_sel0v9:0x51[24],offset calibration clock selection. "1" high clock.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_calck_sel0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.calck_sel0v9;
}

static inline void sys_ll_set_ana_reg17_calck_sel0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.calck_sel0v9 = value;
}

/* REG_0x51:ana_reg17->bpdwa0v9:0x51[25],bypss audio dac dwa. "1" bypass.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_bpdwa0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.bpdwa0v9;
}

static inline void sys_ll_set_ana_reg17_bpdwa0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.bpdwa0v9 = value;
}

/* REG_0x51:ana_reg17->looprst0v9:0x51[26],audio dac integrator capacitor reset. "1" reset.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_looprst0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.looprst0v9;
}

static inline void sys_ll_set_ana_reg17_looprst0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.looprst0v9 = value;
}

/* REG_0x51:ana_reg17->oct0v9:0x51[28:27],over current delay time setting."11" maximum time. "00" minimum current.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_oct0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.oct0v9;
}

static inline void sys_ll_set_ana_reg17_oct0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.oct0v9 = value;
}

/* REG_0x51:ana_reg17->sout0v9:0x51[29],short output with 600ohm resistor. "1" short output.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_sout0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.sout0v9;
}

static inline void sys_ll_set_ana_reg17_sout0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.sout0v9 = value;
}

/* REG_0x51:ana_reg17->hc0v9:0x51[31:30],dac current trimming, 00=minimum current, 11=maximum current,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_hc0v9(sys_hw_t *hw)
{
    return hw->ana_reg17.hc0v9;
}

static inline void sys_ll_set_ana_reg17_hc0v9(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg17.hc0v9 = value;
}

/* REG_0x52 */

static inline void sys_ll_set_ana_reg18_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.v = value;
}

/* REG_0x52:ana_reg18->ictrl_dsppll:0x52[3:0],26M PLL setting,7,W*/
static inline void sys_ll_set_ana_reg18_ictrl_dsppll(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.ictrl_dsppll = value;
}

/* REG_0x52:ana_reg18->FBdivN:0x52[13:4],26M PLL setting,0B6,W*/
static inline void sys_ll_set_ana_reg18_fbdivn(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.fbdivn = value;
}

/* REG_0x52:ana_reg18->N_mcudsp:0x52[18:14],26M PLL setting,0E,W*/
static inline void sys_ll_set_ana_reg18_n_mcudsp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.n_mcudsp = value;
}

/* REG_0x52:ana_reg18->mode:0x52[19],26M PLL setting,1,W*/
static inline void sys_ll_set_ana_reg18_mode(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.mode = value;
}

/* REG_0x52:ana_reg18->iamsel:0x52[20],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg18_iamsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.iamsel = value;
}

/* REG_0x52:ana_reg18->hvref:0x52[22:21],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg18_hvref(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.hvref = value;
}

/* REG_0x52:ana_reg18->lvref:0x52[24:23],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg18_lvref(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg18.lvref = value;
}

/* REG_0x53 */

static inline void sys_ll_set_ana_reg19_value(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.v = value;
}

/* REG_0x53:ana_reg19->amsel:0x53[0],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg19_amsel(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.amsel = value;
}

/* REG_0x53:ana_reg19->msw:0x53[9:1],26M PLL setting,101,W*/
static inline void sys_ll_set_ana_reg19_msw(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.msw = value;
}

/* REG_0x53:ana_reg19->tstcken_dpll:0x53[10],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg19_tstcken_dpll(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.tstcken_dpll = value;
}

/* REG_0x53:ana_reg19->osccal_trig:0x53[11],26M PLL setting,1,W*/
static inline void sys_ll_set_ana_reg19_osccal_trig(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.osccal_trig = value;
}

/* REG_0x53:ana_reg19->cnti:0x53[20:12],26M PLL setting,100,W*/
static inline void sys_ll_set_ana_reg19_cnti(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.cnti = value;
}

/* REG_0x53:ana_reg19->spi_rst:0x53[22],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg19_spi_rst(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.spi_rst = value;
}

/* REG_0x53:ana_reg19->closeloop_en:0x53[23],26M PLL setting,1,W*/
static inline void sys_ll_set_ana_reg19_closeloop_en(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.closeloop_en = value;
}

/* REG_0x53:ana_reg19->caltime:0x53[24],26M PLL setting,1,W*/
static inline void sys_ll_set_ana_reg19_caltime(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.caltime = value;
}

/* REG_0x53:ana_reg19->LPFRz:0x53[26:25],26M PLL setting,2,W*/
static inline void sys_ll_set_ana_reg19_lpfrz(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.lpfrz = value;
}

/* REG_0x53:ana_reg19->ICP:0x53[30:27],26M PLL setting,8,W*/
static inline void sys_ll_set_ana_reg19_icp(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.icp = value;
}

/* REG_0x53:ana_reg19->CP2ctrl:0x53[31],26M PLL setting,0,W*/
static inline void sys_ll_set_ana_reg19_cp2ctrl(sys_hw_t *hw, uint32_t value)
{
    hw->ana_reg19.cp2ctrl = value;
}

#endif 

#ifdef __cplusplus 
}                  
#endif             
