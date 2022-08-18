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
* CHIP ID:BK7256,GENARATE TIME:2022-03-17 20:29:40                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          
#include "sys_ll_macro_def.h"         
#include "bk_misc.h"          

#define SYS_ANALOG_REG_SPI_STATE_REG (SYS_CPU_ANASPI_FREQ_ADDR) // need complete macro manually
#define SYS_ANALOG_REG_SPI_STATE_POS(idx)  (idx + SYS_CPU_ANASPI_FREQ_ANAREG_STATE_POS) // need complete macro manually
#define GET_SYS_ANALOG_REG_IDX(addr)  ((addr - SYS_ANA_REG0_ADDR) >> 2) // need complete macro manually

static inline uint32_t sys_ll_get_analog_reg_value(uint32_t addr)
{
    return REG_READ(addr);
}

static inline void sys_ll_set_analog_reg_value(uint32_t addr, uint32_t value)
{
    uint32_t idx;
    idx = GET_SYS_ANALOG_REG_IDX(addr);

    REG_WRITE(addr, value);

    delay_10us(1);
    while(REG_READ(SYS_ANALOG_REG_SPI_STATE_REG) & (1 << SYS_ANALOG_REG_SPI_STATE_POS(idx)));
}

static inline void sys_ll_set_analog_reg_bits(uint32_t addr, uint32_t value, uint32_t pos, uint32_t mask)
{
    uint32_t reg_value;
    reg_value = REG_READ(addr);
    reg_value &= ~(mask << pos);
    reg_value |= ((value & mask) << pos);

    sys_ll_set_analog_reg_value(addr, reg_value);
}

/* REG_0x00 //REG ADDR :0x44010000 */
static inline uint32_t sys_ll_get_device_id_value(void)
{
    return REG_READ(SYS_DEVICE_ID_ADDR);
}

/* REG_0x00:device_id->DeviceID:0x0[31:0], ,0x53434647,RO*/
static inline uint32_t sys_ll_get_device_id_deviceid(void)
{
    return REG_READ(SYS_DEVICE_ID_ADDR);
}

/* REG_0x01 //REG ADDR :0x44010004 */
static inline uint32_t sys_ll_get_version_id_value(void)
{
    return REG_READ(SYS_VERSION_ID_ADDR);
}

/* REG_0x01:version_id->VersionID:0x1[31:0], ,0x72560001,RO*/
static inline uint32_t sys_ll_get_version_id_versionid(void)
{
    return REG_READ(SYS_VERSION_ID_ADDR);
}

/* REG_0x02 //REG ADDR :0x44010008 */
static inline uint32_t sys_ll_get_cpu_current_run_status_value(void)
{
    return REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
}

/* REG_0x02:cpu_current_run_status->core0_halted:0x2[0],core0 halt indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_core0_halted(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CURRENT_RUN_STATUS_CORE0_HALTED_POS)&SYS_CPU_CURRENT_RUN_STATUS_CORE0_HALTED_MASK);
    return reg_value;
}

/* REG_0x02:cpu_current_run_status->core1_halted:0x2[1],core1 halt indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_core1_halted(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CURRENT_RUN_STATUS_CORE1_HALTED_POS)&SYS_CPU_CURRENT_RUN_STATUS_CORE1_HALTED_MASK);
    return reg_value;
}

/* REG_0x02:cpu_current_run_status->cpu0_sw_reset:0x2[4],cpu0_sw_reset indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_sw_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CURRENT_RUN_STATUS_CPU0_SW_RESET_POS)&SYS_CPU_CURRENT_RUN_STATUS_CPU0_SW_RESET_MASK);
    return reg_value;
}

/* REG_0x02:cpu_current_run_status->cpu1_sw_reset:0x2[5],cpu1_sw_reset indicate,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu1_sw_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CURRENT_RUN_STATUS_CPU1_SW_RESET_POS)&SYS_CPU_CURRENT_RUN_STATUS_CPU1_SW_RESET_MASK);
    return reg_value;
}

/* REG_0x02:cpu_current_run_status->cpu0_pwr_dw_state:0x2[8],cpu0_pwr_dw_state,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CURRENT_RUN_STATUS_CPU0_PWR_DW_STATE_POS)&SYS_CPU_CURRENT_RUN_STATUS_CPU0_PWR_DW_STATE_MASK);
    return reg_value;
}

/* REG_0x02:cpu_current_run_status->cpu1_pwr_dw_state:0x2[9],cpu1_pwr_dw_state,0,RO*/
static inline uint32_t sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CURRENT_RUN_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CURRENT_RUN_STATUS_CPU1_PWR_DW_STATE_POS)&SYS_CPU_CURRENT_RUN_STATUS_CPU1_PWR_DW_STATE_MASK);
    return reg_value;
}

/* REG_0x03 //REG ADDR :0x4401000c */
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_value(void)
{
    return REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
}

static inline void sys_ll_set_cpu_storage_connect_op_select_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR,value);
}

/* REG_0x03:cpu_storage_connect_op_select->boot_mode:0x3[0],0:ROM boot 1:FLASH boot,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_boot_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value = ((reg_value >> SYS_CPU_STORAGE_CONNECT_OP_SELECT_BOOT_MODE_POS) & SYS_CPU_STORAGE_CONNECT_OP_SELECT_BOOT_MODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_boot_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value &= ~(SYS_CPU_STORAGE_CONNECT_OP_SELECT_BOOT_MODE_MASK << SYS_CPU_STORAGE_CONNECT_OP_SELECT_BOOT_MODE_POS);
    reg_value |= ((value & SYS_CPU_STORAGE_CONNECT_OP_SELECT_BOOT_MODE_MASK) << SYS_CPU_STORAGE_CONNECT_OP_SELECT_BOOT_MODE_POS);
    REG_WRITE(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR,reg_value);
}

/* REG_0x03:cpu_storage_connect_op_select->rf_switch_en:0x3[4],0: rf switch by PTA; 1: rf switch by SW,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_rf_switch_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value = ((reg_value >> SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_SWITCH_EN_POS) & SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_SWITCH_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_rf_switch_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value &= ~(SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_SWITCH_EN_MASK << SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_SWITCH_EN_POS);
    reg_value |= ((value & SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_SWITCH_EN_MASK) << SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_SWITCH_EN_POS);
    REG_WRITE(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR,reg_value);
}

/* REG_0x03:cpu_storage_connect_op_select->rf_for_wifiorbt:0x3[5],0: rf for wifi;  1: rf for bt,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_rf_for_wifiorbt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value = ((reg_value >> SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_FOR_WIFIORBT_POS) & SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_FOR_WIFIORBT_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_rf_for_wifiorbt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value &= ~(SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_FOR_WIFIORBT_MASK << SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_FOR_WIFIORBT_POS);
    reg_value |= ((value & SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_FOR_WIFIORBT_MASK) << SYS_CPU_STORAGE_CONNECT_OP_SELECT_RF_FOR_WIFIORBT_POS);
    REG_WRITE(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR,reg_value);
}

/* REG_0x03:cpu_storage_connect_op_select->jtag_core_sel:0x3[8],0:jtag connect core0, 1:jtag connect core1,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value = ((reg_value >> SYS_CPU_STORAGE_CONNECT_OP_SELECT_JTAG_CORE_SEL_POS) & SYS_CPU_STORAGE_CONNECT_OP_SELECT_JTAG_CORE_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value &= ~(SYS_CPU_STORAGE_CONNECT_OP_SELECT_JTAG_CORE_SEL_MASK << SYS_CPU_STORAGE_CONNECT_OP_SELECT_JTAG_CORE_SEL_POS);
    reg_value |= ((value & SYS_CPU_STORAGE_CONNECT_OP_SELECT_JTAG_CORE_SEL_MASK) << SYS_CPU_STORAGE_CONNECT_OP_SELECT_JTAG_CORE_SEL_POS);
    REG_WRITE(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR,reg_value);
}

/* REG_0x03:cpu_storage_connect_op_select->flash_sel:0x3[9],0: normal flash operation 1:flash download by spi,0,R/W*/
static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_flash_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value = ((reg_value >> SYS_CPU_STORAGE_CONNECT_OP_SELECT_FLASH_SEL_POS) & SYS_CPU_STORAGE_CONNECT_OP_SELECT_FLASH_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_flash_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR);
    reg_value &= ~(SYS_CPU_STORAGE_CONNECT_OP_SELECT_FLASH_SEL_MASK << SYS_CPU_STORAGE_CONNECT_OP_SELECT_FLASH_SEL_POS);
    reg_value |= ((value & SYS_CPU_STORAGE_CONNECT_OP_SELECT_FLASH_SEL_MASK) << SYS_CPU_STORAGE_CONNECT_OP_SELECT_FLASH_SEL_POS);
    REG_WRITE(SYS_CPU_STORAGE_CONNECT_OP_SELECT_ADDR,reg_value);
}

/* REG_0x04 //REG ADDR :0x44010010 */
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_value(void)
{
    return REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_value(uint32_t value)
{
    REG_WRITE(SYS_CPU0_INT_HALT_CLK_OP_ADDR,value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_sw_rst:0x4[0],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_HALT_CLK_OP_CPU0_SW_RST_POS) & SYS_CPU0_INT_HALT_CLK_OP_CPU0_SW_RST_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU0_INT_HALT_CLK_OP_CPU0_SW_RST_MASK << SYS_CPU0_INT_HALT_CLK_OP_CPU0_SW_RST_POS);
    reg_value |= ((value & SYS_CPU0_INT_HALT_CLK_OP_CPU0_SW_RST_MASK) << SYS_CPU0_INT_HALT_CLK_OP_CPU0_SW_RST_POS);
    REG_WRITE(SYS_CPU0_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_pwr_dw:0x4[1],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_HALT_CLK_OP_CPU0_PWR_DW_POS) & SYS_CPU0_INT_HALT_CLK_OP_CPU0_PWR_DW_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU0_INT_HALT_CLK_OP_CPU0_PWR_DW_MASK << SYS_CPU0_INT_HALT_CLK_OP_CPU0_PWR_DW_POS);
    reg_value |= ((value & SYS_CPU0_INT_HALT_CLK_OP_CPU0_PWR_DW_MASK) << SYS_CPU0_INT_HALT_CLK_OP_CPU0_PWR_DW_POS);
    REG_WRITE(SYS_CPU0_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_int_mask:0x4[2],cpu0 int mask,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_HALT_CLK_OP_CPU0_INT_MASK_POS) & SYS_CPU0_INT_HALT_CLK_OP_CPU0_INT_MASK_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU0_INT_HALT_CLK_OP_CPU0_INT_MASK_MASK << SYS_CPU0_INT_HALT_CLK_OP_CPU0_INT_MASK_POS);
    reg_value |= ((value & SYS_CPU0_INT_HALT_CLK_OP_CPU0_INT_MASK_MASK) << SYS_CPU0_INT_HALT_CLK_OP_CPU0_INT_MASK_POS);
    REG_WRITE(SYS_CPU0_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_halt:0x4[3],core0 halt indicate,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_HALT_CLK_OP_CPU0_HALT_POS) & SYS_CPU0_INT_HALT_CLK_OP_CPU0_HALT_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU0_INT_HALT_CLK_OP_CPU0_HALT_MASK << SYS_CPU0_INT_HALT_CLK_OP_CPU0_HALT_POS);
    reg_value |= ((value & SYS_CPU0_INT_HALT_CLK_OP_CPU0_HALT_MASK) << SYS_CPU0_INT_HALT_CLK_OP_CPU0_HALT_POS);
    REG_WRITE(SYS_CPU0_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_clk_div:0x4[7:4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_clk_div(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_HALT_CLK_OP_CPU0_CLK_DIV_POS) & SYS_CPU0_INT_HALT_CLK_OP_CPU0_CLK_DIV_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_clk_div(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU0_INT_HALT_CLK_OP_CPU0_CLK_DIV_MASK << SYS_CPU0_INT_HALT_CLK_OP_CPU0_CLK_DIV_POS);
    reg_value |= ((value & SYS_CPU0_INT_HALT_CLK_OP_CPU0_CLK_DIV_MASK) << SYS_CPU0_INT_HALT_CLK_OP_CPU0_CLK_DIV_POS);
    REG_WRITE(SYS_CPU0_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x04:cpu0_int_halt_clk_op->cpu0_offset:0x4[31:8],reserved,0x0,RO*/
static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_HALT_CLK_OP_CPU0_OFFSET_POS)&SYS_CPU0_INT_HALT_CLK_OP_CPU0_OFFSET_MASK);
    return reg_value;
}

/* REG_0x05 //REG ADDR :0x44010014 */
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_value(void)
{
    return REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_value(uint32_t value)
{
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_sw_rst:0x5[0],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_sw_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_HALT_CLK_OP_CPU1_SW_RST_POS) & SYS_CPU1_INT_HALT_CLK_OP_CPU1_SW_RST_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU1_INT_HALT_CLK_OP_CPU1_SW_RST_MASK << SYS_CPU1_INT_HALT_CLK_OP_CPU1_SW_RST_POS);
    reg_value |= ((value & SYS_CPU1_INT_HALT_CLK_OP_CPU1_SW_RST_MASK) << SYS_CPU1_INT_HALT_CLK_OP_CPU1_SW_RST_POS);
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_pwr_dw:0x5[1],reserved,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_HALT_CLK_OP_CPU1_PWR_DW_POS) & SYS_CPU1_INT_HALT_CLK_OP_CPU1_PWR_DW_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU1_INT_HALT_CLK_OP_CPU1_PWR_DW_MASK << SYS_CPU1_INT_HALT_CLK_OP_CPU1_PWR_DW_POS);
    reg_value |= ((value & SYS_CPU1_INT_HALT_CLK_OP_CPU1_PWR_DW_MASK) << SYS_CPU1_INT_HALT_CLK_OP_CPU1_PWR_DW_POS);
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_int_mask:0x5[2],cpu1 int mask,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_int_mask(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_HALT_CLK_OP_CPU1_INT_MASK_POS) & SYS_CPU1_INT_HALT_CLK_OP_CPU1_INT_MASK_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU1_INT_HALT_CLK_OP_CPU1_INT_MASK_MASK << SYS_CPU1_INT_HALT_CLK_OP_CPU1_INT_MASK_POS);
    reg_value |= ((value & SYS_CPU1_INT_HALT_CLK_OP_CPU1_INT_MASK_MASK) << SYS_CPU1_INT_HALT_CLK_OP_CPU1_INT_MASK_POS);
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_halt:0x5[3],core1 halt indicate,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_halt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_HALT_CLK_OP_CPU1_HALT_POS) & SYS_CPU1_INT_HALT_CLK_OP_CPU1_HALT_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU1_INT_HALT_CLK_OP_CPU1_HALT_MASK << SYS_CPU1_INT_HALT_CLK_OP_CPU1_HALT_POS);
    reg_value |= ((value & SYS_CPU1_INT_HALT_CLK_OP_CPU1_HALT_MASK) << SYS_CPU1_INT_HALT_CLK_OP_CPU1_HALT_POS);
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_clk_div:0x5[7:4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_clk_div(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_HALT_CLK_OP_CPU1_CLK_DIV_POS) & SYS_CPU1_INT_HALT_CLK_OP_CPU1_CLK_DIV_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_clk_div(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU1_INT_HALT_CLK_OP_CPU1_CLK_DIV_MASK << SYS_CPU1_INT_HALT_CLK_OP_CPU1_CLK_DIV_POS);
    reg_value |= ((value & SYS_CPU1_INT_HALT_CLK_OP_CPU1_CLK_DIV_MASK) << SYS_CPU1_INT_HALT_CLK_OP_CPU1_CLK_DIV_POS);
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x05:cpu1_int_halt_clk_op->cpu1_offset:0x5[31:8],reserved,0x0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_offset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_HALT_CLK_OP_CPU1_OFFSET_POS) & SYS_CPU1_INT_HALT_CLK_OP_CPU1_OFFSET_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_HALT_CLK_OP_ADDR);
    reg_value &= ~(SYS_CPU1_INT_HALT_CLK_OP_CPU1_OFFSET_MASK << SYS_CPU1_INT_HALT_CLK_OP_CPU1_OFFSET_POS);
    reg_value |= ((value & SYS_CPU1_INT_HALT_CLK_OP_CPU1_OFFSET_MASK) << SYS_CPU1_INT_HALT_CLK_OP_CPU1_OFFSET_POS);
    REG_WRITE(SYS_CPU1_INT_HALT_CLK_OP_ADDR,reg_value);
}

/* REG_0x06 //REG ADDR :0x44010018 */
/* REG_0x08 //REG ADDR :0x44010020 */
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_value(void)
{
    return REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
}

static inline void sys_ll_set_cpu_clk_div_mode1_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_core:0x8[3:0],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_core(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_core(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_CORE_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_core:0x8[5:4],0:XTAL       1 : clk_DCO      2 : 320M      3 : 480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_core(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_core(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_CORE_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_bus:0x8[6],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_bus(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_BUS_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart0:0x8[9:8],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART0_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART0_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART0_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clksel_uart0:0x8[10],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clksel_uart0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKSEL_UART0_POS) & SYS_CPU_CLK_DIV_MODE1_CLKSEL_UART0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clksel_uart0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKSEL_UART0_MASK << SYS_CPU_CLK_DIV_MODE1_CLKSEL_UART0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKSEL_UART0_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKSEL_UART0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart1:0x8[12:11],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART1_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART1_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART1_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART1_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART1_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART1_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_uart1:0x8[13],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_UART1_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_UART1_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_UART1_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_UART1_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_UART1_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_UART1_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_uart2:0x8[15:14],Frequency division :    0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART2_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART2_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART2_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART2_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART2_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_UART2_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_uart2:0x8[16],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_UART2_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_UART2_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_UART2_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_UART2_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_UART2_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_UART2_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_sadc:0x8[17],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_sadc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_SADC_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_SADC_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_sadc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_SADC_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_SADC_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_SADC_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_SADC_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm0:0x8[18],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM0_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM0_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM0_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_pwm1:0x8[19],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM1_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM1_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM1_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM1_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM1_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_PWM1_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer0:0x8[20],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_timer0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER0_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_timer0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER0_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER0_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer1:0x8[21],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_timer1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER1_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER1_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_timer1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER1_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER1_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER1_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER1_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_timer2:0x8[22],0:clk32              1:XTAL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_timer2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER2_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER2_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_timer2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER2_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER2_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER2_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_TIMER2_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_can:0x8[23],0:XTAL              1:80M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_can(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_CAN_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_CAN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_can(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_CAN_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_CAN_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_CAN_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_CAN_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_i2s:0x8[24],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2s(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_I2S_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_I2S_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_i2s(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_I2S_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_I2S_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_I2S_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_I2S_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_aud:0x8[25],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_aud(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_AUD_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_AUD_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_aud(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_AUD_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_AUD_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_AUD_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_AUD_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_jpeg:0x8[29:26],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_jpeg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_JPEG_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_JPEG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_JPEG_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_JPEG_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_JPEG_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_JPEG_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->cksel_jpeg:0x8[30],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_jpeg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CKSEL_JPEG_POS) & SYS_CPU_CLK_DIV_MODE1_CKSEL_JPEG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CKSEL_JPEG_MASK << SYS_CPU_CLK_DIV_MODE1_CKSEL_JPEG_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CKSEL_JPEG_MASK) << SYS_CPU_CLK_DIV_MODE1_CKSEL_JPEG_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x08:cpu_clk_div_mode1->clkdiv_disp_l:0x8[31],Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE1_CLKDIV_DISP_L_POS) & SYS_CPU_CLK_DIV_MODE1_CLKDIV_DISP_L_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE1_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE1_CLKDIV_DISP_L_MASK << SYS_CPU_CLK_DIV_MODE1_CLKDIV_DISP_L_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE1_CLKDIV_DISP_L_MASK) << SYS_CPU_CLK_DIV_MODE1_CLKDIV_DISP_L_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE1_ADDR,reg_value);
}

/* REG_0x09 //REG ADDR :0x44010024 */
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_value(void)
{
    return REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
}

static inline void sys_ll_set_cpu_clk_div_mode2_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,value);
}

/* REG_0x09:cpu_clk_div_mode2->clkdiv_disp_h:0x9[2:0],Frequency division : F/(1+clkdiv_disp_l+clkdiv_disp_h*2),0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CLKDIV_DISP_H_POS) & SYS_CPU_CLK_DIV_MODE2_CLKDIV_DISP_H_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CLKDIV_DISP_H_MASK << SYS_CPU_CLK_DIV_MODE2_CLKDIV_DISP_H_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CLKDIV_DISP_H_MASK) << SYS_CPU_CLK_DIV_MODE2_CLKDIV_DISP_H_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_disp:0x9[3],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_disp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKSEL_DISP_POS) & SYS_CPU_CLK_DIV_MODE2_CKSEL_DISP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_disp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_DISP_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_DISP_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKSEL_DISP_MASK) << SYS_CPU_CLK_DIV_MODE2_CKSEL_DISP_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_psram:0x9[4],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKDIV_PSRAM_POS) & SYS_CPU_CLK_DIV_MODE2_CKDIV_PSRAM_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_PSRAM_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_PSRAM_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKDIV_PSRAM_MASK) << SYS_CPU_CLK_DIV_MODE2_CKDIV_PSRAM_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_psram:0x9[5],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_psram(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKSEL_PSRAM_POS) & SYS_CPU_CLK_DIV_MODE2_CKSEL_PSRAM_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_psram(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_PSRAM_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_PSRAM_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKSEL_PSRAM_MASK) << SYS_CPU_CLK_DIV_MODE2_CKSEL_PSRAM_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_qspi0:0x9[9:6],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_qspi0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKDIV_QSPI0_POS) & SYS_CPU_CLK_DIV_MODE2_CKDIV_QSPI0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_QSPI0_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_QSPI0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKDIV_QSPI0_MASK) << SYS_CPU_CLK_DIV_MODE2_CKDIV_QSPI0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_qspi0:0x9[10],0:clk_320M      1:clk_480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_qspi0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKSEL_QSPI0_POS) & SYS_CPU_CLK_DIV_MODE2_CKSEL_QSPI0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_qspi0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_QSPI0_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_QSPI0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKSEL_QSPI0_MASK) << SYS_CPU_CLK_DIV_MODE2_CKSEL_QSPI0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_sdio:0x9[16:14],0:/2  1:/4  2:/6  3:/8  4:/10  5:/12  6:/16  7:/256,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKDIV_SDIO_POS) & SYS_CPU_CLK_DIV_MODE2_CKDIV_SDIO_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_SDIO_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_SDIO_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKDIV_SDIO_MASK) << SYS_CPU_CLK_DIV_MODE2_CKDIV_SDIO_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_sdio:0x9[17],0：XTAL          1：320M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_sdio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKSEL_SDIO_POS) & SYS_CPU_CLK_DIV_MODE2_CKSEL_SDIO_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_sdio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_SDIO_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_SDIO_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKSEL_SDIO_MASK) << SYS_CPU_CLK_DIV_MODE2_CKSEL_SDIO_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_auxs:0x9[21:18],Frequency division : F/(1+N), N is the data of the reg value,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKDIV_AUXS_POS) & SYS_CPU_CLK_DIV_MODE2_CKDIV_AUXS_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_AUXS_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_AUXS_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKDIV_AUXS_MASK) << SYS_CPU_CLK_DIV_MODE2_CKDIV_AUXS_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_auxs:0x9[23:22],0:DCO              1:APLL                2:320M                     4:480M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_auxs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKSEL_AUXS_POS) & SYS_CPU_CLK_DIV_MODE2_CKSEL_AUXS_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_auxs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_AUXS_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_AUXS_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKSEL_AUXS_MASK) << SYS_CPU_CLK_DIV_MODE2_CKSEL_AUXS_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->cksel_flash:0x9[25:24],0:XTAL              1:APLL               1x :clk_120M,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_flash(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_POS) & SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_flash(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_MASK << SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_MASK) << SYS_CPU_CLK_DIV_MODE2_CKSEL_FLASH_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_flash:0x9[27:26],0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_POS) & SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_MASK) << SYS_CPU_CLK_DIV_MODE2_CKDIV_FLASH_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x09:cpu_clk_div_mode2->ckdiv_i2s0:0x9[30:28],0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,0,R/W*/
static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_i2s0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_CLK_DIV_MODE2_CKDIV_I2S0_POS) & SYS_CPU_CLK_DIV_MODE2_CKDIV_I2S0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_i2s0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_CLK_DIV_MODE2_ADDR);
    reg_value &= ~(SYS_CPU_CLK_DIV_MODE2_CKDIV_I2S0_MASK << SYS_CPU_CLK_DIV_MODE2_CKDIV_I2S0_POS);
    reg_value |= ((value & SYS_CPU_CLK_DIV_MODE2_CKDIV_I2S0_MASK) << SYS_CPU_CLK_DIV_MODE2_CKDIV_I2S0_POS);
    REG_WRITE(SYS_CPU_CLK_DIV_MODE2_ADDR,reg_value);
}

/* REG_0x0A //REG ADDR :0x44010028 */
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_value(void)
{
    return REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_26M_WDT_CLK_DIV_ADDR,value);
}

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_26m:0xa[1:0],0:/1  1:/2  2:/4  3:/8,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_26m(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value = ((reg_value >> SYS_CPU_26M_WDT_CLK_DIV_CKDIV_26M_POS) & SYS_CPU_26M_WDT_CLK_DIV_CKDIV_26M_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_26m(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value &= ~(SYS_CPU_26M_WDT_CLK_DIV_CKDIV_26M_MASK << SYS_CPU_26M_WDT_CLK_DIV_CKDIV_26M_POS);
    reg_value |= ((value & SYS_CPU_26M_WDT_CLK_DIV_CKDIV_26M_MASK) << SYS_CPU_26M_WDT_CLK_DIV_CKDIV_26M_POS);
    REG_WRITE(SYS_CPU_26M_WDT_CLK_DIV_ADDR,reg_value);
}

/* REG_0x0a:cpu_26m_wdt_clk_div->ckdiv_wdt:0xa[3:2],0:/2 1:/4 2:/8 3:/16,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value = ((reg_value >> SYS_CPU_26M_WDT_CLK_DIV_CKDIV_WDT_POS) & SYS_CPU_26M_WDT_CLK_DIV_CKDIV_WDT_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value &= ~(SYS_CPU_26M_WDT_CLK_DIV_CKDIV_WDT_MASK << SYS_CPU_26M_WDT_CLK_DIV_CKDIV_WDT_POS);
    reg_value |= ((value & SYS_CPU_26M_WDT_CLK_DIV_CKDIV_WDT_MASK) << SYS_CPU_26M_WDT_CLK_DIV_CKDIV_WDT_POS);
    REG_WRITE(SYS_CPU_26M_WDT_CLK_DIV_ADDR,reg_value);
}

/* REG_0x0a:cpu_26m_wdt_clk_div->clksel_spi0:0xa[4],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value = ((reg_value >> SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI0_POS) & SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value &= ~(SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI0_MASK << SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI0_POS);
    reg_value |= ((value & SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI0_MASK) << SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI0_POS);
    REG_WRITE(SYS_CPU_26M_WDT_CLK_DIV_ADDR,reg_value);
}

/* REG_0x0a:cpu_26m_wdt_clk_div->clksel_spi1:0xa[5],0:XTAL              1:APLL,0,R/W*/
static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value = ((reg_value >> SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI1_POS) & SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI1_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_26M_WDT_CLK_DIV_ADDR);
    reg_value &= ~(SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI1_MASK << SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI1_POS);
    reg_value |= ((value & SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI1_MASK) << SYS_CPU_26M_WDT_CLK_DIV_CLKSEL_SPI1_POS);
    REG_WRITE(SYS_CPU_26M_WDT_CLK_DIV_ADDR,reg_value);
}

/* REG_0x0B //REG ADDR :0x4401002c */
static inline uint32_t sys_ll_get_cpu_anaspi_freq_value(void)
{
    return REG_READ(SYS_CPU_ANASPI_FREQ_ADDR);
}

static inline void sys_ll_set_cpu_anaspi_freq_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_ANASPI_FREQ_ADDR,value);
}

/* REG_0x0b:cpu_anaspi_freq->anaspi_freq:0xb[5:0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_anaspi_freq_anaspi_freq(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_ANASPI_FREQ_ADDR);
    reg_value = ((reg_value >> SYS_CPU_ANASPI_FREQ_ANASPI_FREQ_POS) & SYS_CPU_ANASPI_FREQ_ANASPI_FREQ_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_anaspi_freq_anaspi_freq(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_ANASPI_FREQ_ADDR);
    reg_value &= ~(SYS_CPU_ANASPI_FREQ_ANASPI_FREQ_MASK << SYS_CPU_ANASPI_FREQ_ANASPI_FREQ_POS);
    reg_value |= ((value & SYS_CPU_ANASPI_FREQ_ANASPI_FREQ_MASK) << SYS_CPU_ANASPI_FREQ_ANASPI_FREQ_POS);
    REG_WRITE(SYS_CPU_ANASPI_FREQ_ADDR,reg_value);
}

/* REG_0x0b:cpu_anaspi_freq->anareg_state:0xb[27:8],analog register state:0x0: register write is idle;0x1: register write is busy; ,0,R*/
static inline uint32_t sys_ll_get_cpu_anaspi_freq_anareg_state(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_ANASPI_FREQ_ADDR);
    reg_value = ((reg_value >> SYS_CPU_ANASPI_FREQ_ANAREG_STATE_POS)&SYS_CPU_ANASPI_FREQ_ANAREG_STATE_MASK);
    return reg_value;
}

/* REG_0x0C //REG ADDR :0x44010030 */
static inline uint32_t sys_ll_get_cpu_device_clk_enable_value(void)
{
    return REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
}

static inline void sys_ll_set_cpu_device_clk_enable_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,value);
}

/* REG_0x0c:cpu_device_clk_enable->i2c0_cken:0xc[0],1:i2c0_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c0_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_I2C0_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_I2C0_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c0_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_I2C0_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_I2C0_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_I2C0_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_I2C0_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->spi0_cken:0xc[1],1:spi0_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi0_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_SPI0_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_SPI0_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi0_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_SPI0_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_SPI0_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_SPI0_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_SPI0_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->uart0_cken:0xc[2],1:uart0_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart0_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_UART0_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_UART0_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart0_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_UART0_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_UART0_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_UART0_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_UART0_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->pwm0_cken:0xc[3],1:pwm0_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm0_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_PWM0_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_PWM0_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm0_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_PWM0_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_PWM0_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_PWM0_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_PWM0_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->tim0_cken:0xc[4],1:tim0_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim0_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_TIM0_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_TIM0_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim0_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_TIM0_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_TIM0_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_TIM0_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_TIM0_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->sadc_cken:0xc[5],1:sadc_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_sadc_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_SADC_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_SADC_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_sadc_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_SADC_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_SADC_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_SADC_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_SADC_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->irda_cken:0xc[6],1:irda_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_irda_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_IRDA_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_IRDA_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_irda_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_IRDA_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_IRDA_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_IRDA_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_IRDA_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->efuse_cken:0xc[7],1:efuse_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_efuse_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_EFUSE_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_EFUSE_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_efuse_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_EFUSE_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_EFUSE_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_EFUSE_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_EFUSE_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->i2c1_cken:0xc[8],1:i2c1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c1_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_I2C1_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_I2C1_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c1_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_I2C1_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_I2C1_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_I2C1_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_I2C1_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->spi1_cken:0xc[9],1:spi1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi1_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_SPI1_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_SPI1_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi1_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_SPI1_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_SPI1_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_SPI1_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_SPI1_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->uart1_cken:0xc[10],1:uart1_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart1_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_UART1_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_UART1_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart1_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_UART1_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_UART1_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_UART1_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_UART1_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->uart2_cken:0xc[11],1:uart2_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart2_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_UART2_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_UART2_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart2_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_UART2_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_UART2_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_UART2_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_UART2_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->pwm1_cken:0xc[12],1:pwm1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm1_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_PWM1_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_PWM1_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm1_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_PWM1_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_PWM1_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_PWM1_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_PWM1_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->tim1_cken:0xc[13],1:tim1_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim1_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_TIM1_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_TIM1_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim1_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_TIM1_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_TIM1_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_TIM1_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_TIM1_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->tim2_cken:0xc[14],1:tim2_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim2_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_TIM2_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_TIM2_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim2_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_TIM2_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_TIM2_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_TIM2_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_TIM2_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->otp_cken:0xc[15],1:otp_clk enable  ,1,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_otp_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_OTP_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_OTP_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_otp_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_OTP_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_OTP_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_OTP_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_OTP_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->i2s_cken:0xc[16],1:i2s_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2s_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_I2S_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_I2S_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2s_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_I2S_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_I2S_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_I2S_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_I2S_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->usb_cken:0xc[17],1:usb_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_usb_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_USB_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_USB_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_usb_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_USB_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_USB_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_USB_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_USB_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->can_cken:0xc[18],1:can_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_can_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_CAN_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_CAN_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_can_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_CAN_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_CAN_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_CAN_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_CAN_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->psram_cken:0xc[19],1:psram_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_psram_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_PSRAM_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_PSRAM_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_psram_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_PSRAM_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_PSRAM_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_PSRAM_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_PSRAM_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->qspi0_cken:0xc[20],1:qspi0_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi0_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_QSPI0_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_QSPI0_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi0_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_QSPI0_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_QSPI0_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_QSPI0_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_QSPI0_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->qspi1_cken:0xc[21],1:qspi1_clk enable,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi1_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_QSPI1_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_QSPI1_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi1_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_QSPI1_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_QSPI1_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_QSPI1_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_QSPI1_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->sdio_cken:0xc[22],1:sdio_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_sdio_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_SDIO_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_SDIO_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_sdio_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_SDIO_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_SDIO_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_SDIO_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_SDIO_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->auxs_cken:0xc[23],1:auxs_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_auxs_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_AUXS_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_AUXS_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_auxs_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_AUXS_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_AUXS_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_AUXS_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_AUXS_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->btdm_cken:0xc[24],1:btdm_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_btdm_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_BTDM_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_BTDM_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_btdm_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_BTDM_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_BTDM_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_BTDM_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_BTDM_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->xvr_cken:0xc[25],1:xvr_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_xvr_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_XVR_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_XVR_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_xvr_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_XVR_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_XVR_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_XVR_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_XVR_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->mac_cken:0xc[26],1:mac_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_MAC_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_MAC_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_mac_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_MAC_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_MAC_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_MAC_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_MAC_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->phy_cken:0xc[27],1:phy_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_PHY_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_PHY_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_phy_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_PHY_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_PHY_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_PHY_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_PHY_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->jpeg_cken:0xc[28],1:jpeg_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_jpeg_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_JPEG_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_JPEG_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_jpeg_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_JPEG_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_JPEG_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_JPEG_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_JPEG_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->disp_cken:0xc[29],1:disp_clk enable ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_disp_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_DISP_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_DISP_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_disp_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_DISP_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_DISP_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_DISP_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_DISP_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->aud_cken:0xc[30],1:aud_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_aud_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_AUD_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_AUD_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_aud_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_AUD_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_AUD_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_AUD_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_AUD_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0c:cpu_device_clk_enable->wdt_cken:0xc[31],1:wdt_clk enable  ,0,R/W*/
static inline uint32_t sys_ll_get_cpu_device_clk_enable_wdt_cken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value = ((reg_value >> SYS_CPU_DEVICE_CLK_ENABLE_WDT_CKEN_POS) & SYS_CPU_DEVICE_CLK_ENABLE_WDT_CKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_device_clk_enable_wdt_cken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_WDT_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_WDT_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_WDT_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_WDT_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK_ENABLE_ADDR,reg_value);
}

/* REG_0x0D //REG ADDR :0x44010034 */
/* REG_0x0E //REG ADDR :0x44010038 */
static inline uint32_t sys_ll_get_cpu_mode_disckg1_value(void)
{
    return REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
}

static inline void sys_ll_set_cpu_mode_disckg1_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,value);
}

/* REG_0x0e:cpu_mode_disckg1->aon_disckg:0xe[0],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_aon_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_AON_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_AON_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_aon_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_AON_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_AON_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_AON_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_AON_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->sys_disckg:0xe[1],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sys_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_SYS_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_SYS_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_sys_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_SYS_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_SYS_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_SYS_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_SYS_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->dma_disckg:0xe[2],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_dma_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_DMA_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_DMA_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_dma_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_DMA_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_DMA_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_DMA_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_DMA_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->flash_disckg:0xe[3],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_flash_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_FLASH_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_FLASH_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_flash_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_FLASH_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_FLASH_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_FLASH_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_FLASH_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->wdt_disckg:0xe[4],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_wdt_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_WDT_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_WDT_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_wdt_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_WDT_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_WDT_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_WDT_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_WDT_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->tim_disckg:0xe[5],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_tim_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_TIM_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_TIM_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_tim_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_TIM_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_TIM_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_TIM_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_TIM_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->urt_disckg:0xe[6],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_urt_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_URT_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_URT_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_urt_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_URT_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_URT_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_URT_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_URT_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->pwm_disckg:0xe[7],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_pwm_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_PWM_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_PWM_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_pwm_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_PWM_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_PWM_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_PWM_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_PWM_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->i2c_disckg:0xe[8],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_i2c_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_I2C_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_I2C_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_i2c_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_I2C_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_I2C_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_I2C_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_I2C_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->spi_disckg:0xe[9],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_spi_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_SPI_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_SPI_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_spi_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_SPI_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_SPI_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_SPI_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_SPI_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->sadc_disckg:0xe[10],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sadc_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_SADC_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_SADC_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_sadc_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_SADC_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_SADC_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_SADC_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_SADC_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->efs_disckg:0xe[11],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_efs_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_EFS_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_EFS_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_efs_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_EFS_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_EFS_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_EFS_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_EFS_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->irda_disckg:0xe[12],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_irda_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_IRDA_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_IRDA_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_irda_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_IRDA_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_IRDA_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_IRDA_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_IRDA_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->trng_disckg:0xe[13],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_trng_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_TRNG_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_TRNG_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_trng_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_TRNG_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_TRNG_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_TRNG_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_TRNG_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->sdio_disckg:0xe[14],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sdio_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_SDIO_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_SDIO_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_sdio_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_SDIO_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_SDIO_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_SDIO_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_SDIO_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->LA_disckg:0xe[15],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_la_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_LA_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_LA_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_la_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_LA_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_LA_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_LA_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_LA_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->tim1_disckg:0xe[16],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_tim1_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_TIM1_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_TIM1_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_tim1_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_TIM1_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_TIM1_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_TIM1_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_TIM1_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->urt1_disckg:0xe[17],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_urt1_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_URT1_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_URT1_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_urt1_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_URT1_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_URT1_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_URT1_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_URT1_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->urt2_disckg:0xe[18],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_urt2_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_URT2_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_URT2_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_urt2_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_URT2_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_URT2_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_URT2_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_URT2_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->pwm1_disckg:0xe[19],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_pwm1_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_PWM1_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_PWM1_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_pwm1_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_PWM1_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_PWM1_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_PWM1_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_PWM1_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->i2c1_disckg:0xe[20],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_i2c1_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_I2C1_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_I2C1_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_i2c1_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_I2C1_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_I2C1_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_I2C1_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_I2C1_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->spi1_disckg:0xe[21],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_spi1_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_SPI1_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_SPI1_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_spi1_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_SPI1_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_SPI1_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_SPI1_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_SPI1_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->usb_disckg:0xe[22],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_usb_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_USB_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_USB_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_usb_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_USB_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_USB_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_USB_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_USB_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->can_disckg:0xe[23],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_can_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_CAN_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_CAN_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_can_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_CAN_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_CAN_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_CAN_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_CAN_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->qspi0_disckg:0xe[24],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_qspi0_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_QSPI0_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_QSPI0_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_qspi0_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_QSPI0_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_QSPI0_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_QSPI0_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_QSPI0_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->psram_disckg:0xe[25],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_psram_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_PSRAM_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_PSRAM_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_psram_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_PSRAM_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_PSRAM_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_PSRAM_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_PSRAM_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->fft_disckg:0xe[26],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_fft_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_FFT_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_FFT_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_fft_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_FFT_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_FFT_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_FFT_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_FFT_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->sbc_disckg:0xe[27],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_sbc_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_SBC_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_SBC_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_sbc_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_SBC_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_SBC_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_SBC_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_SBC_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->aud_disckg:0xe[28],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_aud_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_AUD_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_AUD_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_aud_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_AUD_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_AUD_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_AUD_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_AUD_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->i2s_disckg:0xe[29],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_i2s_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_I2S_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_I2S_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_i2s_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_I2S_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_I2S_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_I2S_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_I2S_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->jpeg_disckg:0xe[30],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_jpeg_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_JPEG_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_JPEG_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_jpeg_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_JPEG_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_JPEG_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_JPEG_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_JPEG_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0e:cpu_mode_disckg1->jpeg_dec_disckg:0xe[31],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg1_jpeg_dec_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG1_JPEG_DEC_DISCKG_POS) & SYS_CPU_MODE_DISCKG1_JPEG_DEC_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg1_jpeg_dec_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG1_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG1_JPEG_DEC_DISCKG_MASK << SYS_CPU_MODE_DISCKG1_JPEG_DEC_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG1_JPEG_DEC_DISCKG_MASK) << SYS_CPU_MODE_DISCKG1_JPEG_DEC_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG1_ADDR,reg_value);
}

/* REG_0x0F //REG ADDR :0x4401003c */
static inline uint32_t sys_ll_get_cpu_mode_disckg2_value(void)
{
    return REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
}

static inline void sys_ll_set_cpu_mode_disckg2_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_MODE_DISCKG2_ADDR,value);
}

/* REG_0x0f:cpu_mode_disckg2->disp_disckg:0xf[0],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_disp_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG2_DISP_DISCKG_POS) & SYS_CPU_MODE_DISCKG2_DISP_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg2_disp_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG2_DISP_DISCKG_MASK << SYS_CPU_MODE_DISCKG2_DISP_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG2_DISP_DISCKG_MASK) << SYS_CPU_MODE_DISCKG2_DISP_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG2_ADDR,reg_value);
}

/* REG_0x0f:cpu_mode_disckg2->dma2d_disckg:0xf[1],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_dma2d_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG2_DMA2D_DISCKG_POS) & SYS_CPU_MODE_DISCKG2_DMA2D_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg2_dma2d_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG2_DMA2D_DISCKG_MASK << SYS_CPU_MODE_DISCKG2_DMA2D_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG2_DMA2D_DISCKG_MASK) << SYS_CPU_MODE_DISCKG2_DMA2D_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG2_ADDR,reg_value);
}

/* REG_0x0f:cpu_mode_disckg2->btdm_disckg:0xf[3],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_btdm_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG2_BTDM_DISCKG_POS) & SYS_CPU_MODE_DISCKG2_BTDM_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg2_btdm_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG2_BTDM_DISCKG_MASK << SYS_CPU_MODE_DISCKG2_BTDM_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG2_BTDM_DISCKG_MASK) << SYS_CPU_MODE_DISCKG2_BTDM_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG2_ADDR,reg_value);
}

/* REG_0x0f:cpu_mode_disckg2->xver_disckg:0xf[4],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_xver_disckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG2_XVER_DISCKG_POS) & SYS_CPU_MODE_DISCKG2_XVER_DISCKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg2_xver_disckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG2_XVER_DISCKG_MASK << SYS_CPU_MODE_DISCKG2_XVER_DISCKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG2_XVER_DISCKG_MASK) << SYS_CPU_MODE_DISCKG2_XVER_DISCKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG2_ADDR,reg_value);
}

/* REG_0x0f:cpu_mode_disckg2->btdm_bps_ckg:0xf[8:5],BUS_CLK  ENABLE,0: bus clock open when module is select,1:bus clock always open,0,R/W*/
static inline uint32_t sys_ll_get_cpu_mode_disckg2_btdm_bps_ckg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value = ((reg_value >> SYS_CPU_MODE_DISCKG2_BTDM_BPS_CKG_POS) & SYS_CPU_MODE_DISCKG2_BTDM_BPS_CKG_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_mode_disckg2_btdm_bps_ckg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_MODE_DISCKG2_ADDR);
    reg_value &= ~(SYS_CPU_MODE_DISCKG2_BTDM_BPS_CKG_MASK << SYS_CPU_MODE_DISCKG2_BTDM_BPS_CKG_POS);
    reg_value |= ((value & SYS_CPU_MODE_DISCKG2_BTDM_BPS_CKG_MASK) << SYS_CPU_MODE_DISCKG2_BTDM_BPS_CKG_POS);
    REG_WRITE(SYS_CPU_MODE_DISCKG2_ADDR,reg_value);
}

/* REG_0x10 //REG ADDR :0x44010040 */
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_value(void)
{
    return REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_value(uint32_t value)
{
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem1:0x10[0],0:power on of mem1      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM1_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM1_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM1_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM1_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM1_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM1_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem2:0x10[1],0:power on of mem2      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM2_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM2_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM2_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM2_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM2_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM2_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem3:0x10[2],0:power on of mem3      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM3_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM3_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM3_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM3_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM3_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM3_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_encp:0x10[3],0:power on of encp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_ENCP_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_ENCP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_ENCP_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_ENCP_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_ENCP_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_ENCP_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_bakp:0x10[4],0:power on of bakp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BAKP_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BAKP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BAKP_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BAKP_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BAKP_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BAKP_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_ahbp:0x10[5],0:power on of ahbp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ahbp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AHBP_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AHBP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_ahbp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AHBP_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AHBP_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AHBP_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AHBP_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_audp:0x10[6],0:power on of audp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_audp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AUDP_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AUDP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AUDP_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AUDP_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AUDP_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_AUDP_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_vidp:0x10[7],0:power on of vidp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_vidp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_VIDP_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_VIDP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_VIDP_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_VIDP_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_VIDP_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_VIDP_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_btsp:0x10[8],0:power on of btsp      ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BTSP_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BTSP_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BTSP_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BTSP_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BTSP_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_BTSP_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_mac:0x10[9],0:power on of wifp_mac  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_MAC_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_MAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_MAC_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_MAC_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_MAC_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_MAC_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_wifp_phy:0x10[10],0:power on of wifp_phy  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_PHY_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_PHY_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_PHY_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_PHY_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_PHY_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_WIFP_PHY_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->pwd_mem0:0x10[11] ,0:power on of mem0,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM0_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM0_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM0_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM0_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM0_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_PWD_MEM0_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_flash_idle:0x10[16],0:sleep_en of flash_idle,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_FLASH_IDLE_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_FLASH_IDLE_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_FLASH_IDLE_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_FLASH_IDLE_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_FLASH_IDLE_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_FLASH_IDLE_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu1_wfi:0x10[17],0:sleep_en of cpu1_wfi  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU1_WFI_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU1_WFI_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU1_WFI_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU1_WFI_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU1_WFI_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU1_WFI_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_need_cpu0_wfi:0x10[18],0:sleep_en of cpu0_wfi  ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU0_WFI_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU0_WFI_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU0_WFI_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU0_WFI_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU0_WFI_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_NEED_CPU0_WFI_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->sleep_en_global:0x10[19],0:sleep_en of global    ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_GLOBAL_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_GLOBAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_GLOBAL_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_GLOBAL_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_GLOBAL_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_SLEEP_EN_GLOBAL_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->wifi_wakeup_platform_en:0x10[20],0:wifi_wakeup_en        ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_WIFI_WAKEUP_PLATFORM_EN_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_WIFI_WAKEUP_PLATFORM_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_wifi_wakeup_platform_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_WIFI_WAKEUP_PLATFORM_EN_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_WIFI_WAKEUP_PLATFORM_EN_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_WIFI_WAKEUP_PLATFORM_EN_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_WIFI_WAKEUP_PLATFORM_EN_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->bts_wakeup_platform_en:0x10[21],0:bts_wakeup_en         ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_BTS_WAKEUP_PLATFORM_EN_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_BTS_WAKEUP_PLATFORM_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_BTS_WAKEUP_PLATFORM_EN_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_BTS_WAKEUP_PLATFORM_EN_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_BTS_WAKEUP_PLATFORM_EN_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_BTS_WAKEUP_PLATFORM_EN_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x10:cpu_power_sleep_wakeup->bts_sleep_exit_req:0x10[22],0:bt sleep exit request ,0,RW*/
static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_sleep_exit_req(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value = ((reg_value >> SYS_CPU_POWER_SLEEP_WAKEUP_BTS_SLEEP_EXIT_REQ_POS) & SYS_CPU_POWER_SLEEP_WAKEUP_BTS_SLEEP_EXIT_REQ_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_sleep_exit_req(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR);
    reg_value &= ~(SYS_CPU_POWER_SLEEP_WAKEUP_BTS_SLEEP_EXIT_REQ_MASK << SYS_CPU_POWER_SLEEP_WAKEUP_BTS_SLEEP_EXIT_REQ_POS);
    reg_value |= ((value & SYS_CPU_POWER_SLEEP_WAKEUP_BTS_SLEEP_EXIT_REQ_MASK) << SYS_CPU_POWER_SLEEP_WAKEUP_BTS_SLEEP_EXIT_REQ_POS);
    REG_WRITE(SYS_CPU_POWER_SLEEP_WAKEUP_ADDR,reg_value);
}

/* REG_0x11 //REG ADDR :0x44010044 */
/* REG_0x20 //REG ADDR :0x44010080 */
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_value(void)
{
    return REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
}

static inline void sys_ll_set_cpu0_int_0_31_en_value(uint32_t value)
{
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_bmc32_int_en:0x20[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_bmc32_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_bmc32_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_BMC32_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_irq_en:0x20[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_host_0_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_host_0_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_IRQ_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_host_0_sec_irq_en:0x20[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_host_0_sec_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_HOST_0_SEC_IRQ_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer_int_en:0x20[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_TIMER_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart_int_en:0x20[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_UART_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_pwm_int_en:0x20[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_pwm_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_pwm_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_PWM_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c_int_en:0x20[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_I2C_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi_int_en:0x20[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_SPI_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sadc_int_en:0x20[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sadc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sadc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_SADC_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_irda_int_en:0x20[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_irda_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_irda_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_IRDA_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sdio_int_en:0x20[10], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_SDIO_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_gdma_int_en:0x20[11], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_GDMA_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_la_int_en:0x20[12], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_LA_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_timer1_int_en:0x20[13], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_TIMER1_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2c1_int_en:0x20[14], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_I2C1_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart1_int_en:0x20[15], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_UART1_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_uart2_int_en:0x20[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_UART2_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_spi1_int_en:0x20[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_SPI1_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_can_int_en:0x20[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_can_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_can_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_CAN_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_usb_int_en:0x20[19], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_usb_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_usb_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_USB_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_qspi_int_en:0x20[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_qspi_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_qspi_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_QSPI_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_fft_int_en:0x20[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_fft_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_FFT_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_sbc_int_en:0x20[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sbc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sbc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_SBC_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_aud_int_en:0x20[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_aud_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_AUD_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_i2s_int_en:0x20[24], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2s_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2s_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_I2S_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegenc_int_en:0x20[25], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_JPEGENC_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_jpegdec_int_en:0x20[26], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_JPEGDEC_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_lcd_int_en:0x20[27], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_LCD_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_dma2d_int_en:0x20[28], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma2d_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_DMA2D_INT_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_DMA2D_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_DMA2D_INT_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_DMA2D_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_DMA2D_INT_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_DMA2D_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_int_phy_mpb_en:0x20[29], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_MPB_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_int_phy_riu_en:0x20[30], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_INT_PHY_RIU_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x20:cpu0_int_0_31_en->cpu0_wifi_mac_int_tx_rx_timer_en:0x20[31], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_POS) & SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_MASK << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_MASK) << SYS_CPU0_INT_0_31_EN_CPU0_WIFI_MAC_INT_TX_RX_TIMER_EN_POS);
    REG_WRITE(SYS_CPU0_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x21 //REG ADDR :0x44010084 */
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_value(void)
{
    return REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
}

static inline void sys_ll_set_cpu0_int_32_63_en_value(uint32_t value)
{
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_tx_rx_misc_en:0x21[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_RX_MISC_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_rx_trigger_en:0x21[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_RX_TRIGGER_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_tx_trigger_en:0x21[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_TX_TRIGGER_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_prot_trigger_en:0x21[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_PROT_TRIGGER_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_PROT_TRIGGER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_PROT_TRIGGER_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_PROT_TRIGGER_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_PROT_TRIGGER_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_PROT_TRIGGER_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_mac_int_gen_en:0x21[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_MAC_INT_GEN_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_hsu_irq_en:0x21[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_HSU_IRQ_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_wifi_int_mac_wakeup_en:0x21[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_WIFI_INT_MAC_WAKEUP_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_dm_irq_en:0x21[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_DM_IRQ_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_ble_irq_en:0x21[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_BLE_IRQ_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_bt_irq_en:0x21[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_BT_IRQ_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox0_int_en:0x21[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_MBOX0_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_mbox1_int_en:0x21[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_MBOX1_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_bmc64_int_en:0x21[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_BMC64_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_touched_int_en:0x21[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_touched_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_TOUCHED_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_usbplug_int_en:0x21[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_USBPLUG_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_rtc_int_en:0x21[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rtc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_RTC_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x21:cpu0_int_32_63_en->cpu0_gpio_int_en:0x21[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_POS) & SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_MASK << SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_POS);
    reg_value |= ((value & SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_MASK) << SYS_CPU0_INT_32_63_EN_CPU0_GPIO_INT_EN_POS);
    REG_WRITE(SYS_CPU0_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x22 //REG ADDR :0x44010088 */
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_value(void)
{
    return REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
}

static inline void sys_ll_set_cpu1_int_0_31_en_value(uint32_t value)
{
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_bmc32_int_en:0x22[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_bmc32_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_BMC32_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_BMC32_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_bmc32_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_BMC32_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_BMC32_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_BMC32_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_BMC32_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_irq_en:0x22[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_host_0_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_IRQ_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_host_0_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_IRQ_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_IRQ_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_IRQ_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_host_0_sec_irq_en:0x22[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_SEC_IRQ_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_SEC_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_host_0_sec_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_SEC_IRQ_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_SEC_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_SEC_IRQ_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_HOST_0_SEC_IRQ_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer_int_en:0x22[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_TIMER_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_TIMER_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_timer_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_TIMER_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_TIMER_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_TIMER_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_TIMER_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart_int_en:0x22[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_UART_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_UART_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_UART_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_UART_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_UART_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_UART_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_pwm_int_en:0x22[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_pwm_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_PWM_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_PWM_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_pwm_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_PWM_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_PWM_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_PWM_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_PWM_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c_int_en:0x22[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_I2C_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_I2C_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_I2C_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_I2C_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_I2C_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_I2C_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi_int_en:0x22[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_SPI_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_SPI_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_spi_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_SPI_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_SPI_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_SPI_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_SPI_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sadc_int_en:0x22[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sadc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_SADC_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_SADC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sadc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_SADC_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_SADC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_SADC_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_SADC_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_irda_int_en:0x22[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_irda_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_IRDA_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_IRDA_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_irda_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_IRDA_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_IRDA_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_IRDA_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_IRDA_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sdio_int_en:0x22[10], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sdio_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_SDIO_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_SDIO_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_SDIO_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_SDIO_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_SDIO_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_SDIO_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_gdma_int_en:0x22[11], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_gdma_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_GDMA_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_GDMA_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_gdma_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_GDMA_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_GDMA_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_GDMA_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_GDMA_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_la_int_en:0x22[12], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_la_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_LA_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_LA_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_la_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_LA_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_LA_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_LA_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_LA_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_timer1_int_en:0x22[13], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_TIMER1_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_TIMER1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_timer1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_TIMER1_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_TIMER1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_TIMER1_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_TIMER1_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2c1_int_en:0x22[14], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_I2C1_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_I2C1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_I2C1_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_I2C1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_I2C1_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_I2C1_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart1_int_en:0x22[15], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_UART1_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_UART1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_UART1_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_UART1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_UART1_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_UART1_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_uart2_int_en:0x22[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart2_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_UART2_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_UART2_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart2_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_UART2_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_UART2_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_UART2_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_UART2_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_spi1_int_en:0x22[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_SPI1_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_SPI1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_spi1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_SPI1_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_SPI1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_SPI1_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_SPI1_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_can_int_en:0x22[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_can_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_CAN_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_CAN_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_can_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_CAN_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_CAN_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_CAN_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_CAN_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_usb_int_en:0x22[19], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_usb_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_USB_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_USB_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_usb_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_USB_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_USB_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_USB_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_USB_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_qspi_int_en:0x22[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_qspi_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_QSPI_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_QSPI_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_qspi_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_QSPI_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_QSPI_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_QSPI_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_QSPI_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_fft_int_en:0x22[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_fft_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_FFT_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_FFT_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_fft_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_FFT_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_FFT_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_FFT_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_FFT_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_sbc_int_en:0x22[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sbc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_SBC_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_SBC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sbc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_SBC_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_SBC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_SBC_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_SBC_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_aud_int_en:0x22[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_aud_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_AUD_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_AUD_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_aud_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_AUD_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_AUD_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_AUD_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_AUD_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_i2s_int_en:0x22[24], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2s_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_I2S_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_I2S_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2s_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_I2S_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_I2S_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_I2S_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_I2S_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegenc_int_en:0x22[25], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_JPEGENC_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_JPEGENC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_JPEGENC_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_JPEGENC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_JPEGENC_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_JPEGENC_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_jpegdec_int_en:0x22[26], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_JPEGDEC_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_JPEGDEC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_JPEGDEC_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_JPEGDEC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_JPEGDEC_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_JPEGDEC_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_lcd_int_en:0x22[27], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_lcd_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_LCD_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_LCD_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_LCD_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_LCD_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_LCD_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_LCD_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_dma2d_int_en:0x22[28], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma2d_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_DMA2D_INT_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_DMA2D_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_dma2d_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_DMA2D_INT_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_DMA2D_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_DMA2D_INT_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_DMA2D_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_int_phy_mpb_en:0x22[29], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_MPB_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_MPB_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_MPB_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_MPB_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_MPB_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_MPB_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_int_phy_riu_en:0x22[30], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_RIU_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_RIU_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_RIU_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_RIU_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_RIU_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_WIFI_INT_PHY_RIU_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x22:cpu1_int_0_31_en->cpu1_wifi_mac_int_tx_rx_timer_en:0x22[31], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_EN_CPU1_WIFI_MAC_INT_TX_RX_TIMER_EN_POS) & SYS_CPU1_INT_0_31_EN_CPU1_WIFI_MAC_INT_TX_RX_TIMER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_0_31_EN_CPU1_WIFI_MAC_INT_TX_RX_TIMER_EN_MASK << SYS_CPU1_INT_0_31_EN_CPU1_WIFI_MAC_INT_TX_RX_TIMER_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_0_31_EN_CPU1_WIFI_MAC_INT_TX_RX_TIMER_EN_MASK) << SYS_CPU1_INT_0_31_EN_CPU1_WIFI_MAC_INT_TX_RX_TIMER_EN_POS);
    REG_WRITE(SYS_CPU1_INT_0_31_EN_ADDR,reg_value);
}

/* REG_0x23 //REG ADDR :0x4401008c */
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_value(void)
{
    return REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
}

static inline void sys_ll_set_cpu1_int_32_63_en_value(uint32_t value)
{
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_tx_rx_misc_en:0x23[0], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_RX_MISC_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_RX_MISC_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_RX_MISC_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_RX_MISC_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_RX_MISC_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_RX_MISC_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_rx_trigger_en:0x23[1], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_RX_TRIGGER_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_RX_TRIGGER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_RX_TRIGGER_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_RX_TRIGGER_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_RX_TRIGGER_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_RX_TRIGGER_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_tx_trigger_en:0x23[2], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_TRIGGER_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_TRIGGER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_TRIGGER_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_TRIGGER_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_TRIGGER_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_TX_TRIGGER_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_prot_trigger_en:0x23[3], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_PROT_TRIGGER_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_PROT_TRIGGER_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_PROT_TRIGGER_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_PROT_TRIGGER_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_PROT_TRIGGER_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_PROT_TRIGGER_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_mac_int_gen_en:0x23[4], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_GEN_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_GEN_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_GEN_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_GEN_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_GEN_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_MAC_INT_GEN_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_hsu_irq_en:0x23[5], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_HSU_IRQ_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_HSU_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_HSU_IRQ_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_HSU_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_HSU_IRQ_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_HSU_IRQ_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_wifi_int_mac_wakeup_en:0x23[6], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_WIFI_INT_MAC_WAKEUP_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_INT_MAC_WAKEUP_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_WIFI_INT_MAC_WAKEUP_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_INT_MAC_WAKEUP_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_WIFI_INT_MAC_WAKEUP_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_WIFI_INT_MAC_WAKEUP_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_dm_irq_en:0x23[7], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dm_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_DM_IRQ_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_DM_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_dm_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_DM_IRQ_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_DM_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_DM_IRQ_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_DM_IRQ_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_ble_irq_en:0x23[8], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_ble_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_BLE_IRQ_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_BLE_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_ble_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_BLE_IRQ_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_BLE_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_BLE_IRQ_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_BLE_IRQ_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_bt_irq_en:0x23[9], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bt_irq_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_BT_IRQ_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_BT_IRQ_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_bt_irq_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_BT_IRQ_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_BT_IRQ_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_BT_IRQ_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_BT_IRQ_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox0_int_en:0x23[16], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_MBOX0_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_MBOX0_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_MBOX0_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_MBOX0_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_MBOX0_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_MBOX0_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_mbox1_int_en:0x23[17], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_MBOX1_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_MBOX1_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_MBOX1_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_MBOX1_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_MBOX1_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_MBOX1_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_bmc64_int_en:0x23[18], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_BMC64_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_BMC64_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_BMC64_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_BMC64_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_BMC64_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_BMC64_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_touched_int_en:0x23[20], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_touched_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_TOUCHED_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_TOUCHED_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_touched_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_TOUCHED_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_TOUCHED_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_TOUCHED_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_TOUCHED_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_usbplug_int_en:0x23[21], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_USBPLUG_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_USBPLUG_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_USBPLUG_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_USBPLUG_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_USBPLUG_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_USBPLUG_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_rtc_int_en:0x23[22], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rtc_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_RTC_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_RTC_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_rtc_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_RTC_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_RTC_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_RTC_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_RTC_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x23:cpu1_int_32_63_en->cpu1_gpio_int_en:0x23[23], ,0,R/W*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_gpio_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_EN_CPU1_GPIO_INT_EN_POS) & SYS_CPU1_INT_32_63_EN_CPU1_GPIO_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_gpio_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_EN_ADDR);
    reg_value &= ~(SYS_CPU1_INT_32_63_EN_CPU1_GPIO_INT_EN_MASK << SYS_CPU1_INT_32_63_EN_CPU1_GPIO_INT_EN_POS);
    reg_value |= ((value & SYS_CPU1_INT_32_63_EN_CPU1_GPIO_INT_EN_MASK) << SYS_CPU1_INT_32_63_EN_CPU1_GPIO_INT_EN_POS);
    REG_WRITE(SYS_CPU1_INT_32_63_EN_ADDR,reg_value);
}

/* REG_0x28 //REG ADDR :0x440100a0 */
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_value(void)
{
    return REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_bmc32_int_st:0x28[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_bmc32_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_BMC32_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_BMC32_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_irq_st:0x28[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_host_0_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_HOST_0_IRQ_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_HOST_0_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_host_0_sec_irq_st:0x28[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_host_0_sec_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_HOST_0_SEC_IRQ_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_HOST_0_SEC_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer_int_st:0x28[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_TIMER_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_TIMER_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart_int_st:0x28[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_UART_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_UART_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_pwm_int_st:0x28[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_pwm_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_PWM_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_PWM_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c_int_st:0x28[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_I2C_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_I2C_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi_int_st:0x28[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_SPI_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_SPI_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sadc_int_st:0x28[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_SADC_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_SADC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_irda_int_st:0x28[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_irda_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_IRDA_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_IRDA_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sdio_int_st:0x28[10], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_SDIO_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_SDIO_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_gdma_int_st:0x28[11], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_GDMA_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_GDMA_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_la_int_st:0x28[12], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_LA_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_LA_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_timer1_int_st:0x28[13], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_TIMER1_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_TIMER1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2c1_int_st:0x28[14], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_I2C1_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_I2C1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart1_int_st:0x28[15], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_UART1_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_UART1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_uart2_int_st:0x28[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_UART2_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_UART2_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_spi1_int_st:0x28[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_SPI1_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_SPI1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_can_int_st:0x28[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_can_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_CAN_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_CAN_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_usb_int_st:0x28[19], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_usb_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_USB_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_USB_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_qspi_int_st:0x28[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_qspi_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_QSPI_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_QSPI_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_fft_int_st:0x28[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_fft_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_FFT_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_FFT_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_sbc_int_st:0x28[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sbc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_SBC_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_SBC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_aud_int_st:0x28[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_aud_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_AUD_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_AUD_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_i2s_int_st:0x28[24], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2s_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_I2S_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_I2S_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegenc_int_st:0x28[25], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_JPEGENC_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_JPEGENC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_jpegdec_int_st:0x28[26], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_JPEGDEC_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_JPEGDEC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_lcd_int_st:0x28[27], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_lcd_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_LCD_INT_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_LCD_INT_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_int_phy_mpb_st:0x28[29], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_mpb_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_WIFI_INT_PHY_MPB_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_WIFI_INT_PHY_MPB_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_int_phy_riu_st:0x28[30], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_riu_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_WIFI_INT_PHY_RIU_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_WIFI_INT_PHY_RIU_ST_MASK);
    return reg_value;
}

/* REG_0x28:cpu0_int_0_31_status->cpu0_wifi_mac_int_tx_rx_timer_st:0x28[31], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_0_31_STATUS_CPU0_WIFI_MAC_INT_TX_RX_TIMER_ST_POS)&SYS_CPU0_INT_0_31_STATUS_CPU0_WIFI_MAC_INT_TX_RX_TIMER_ST_MASK);
    return reg_value;
}

/* REG_0x29 //REG ADDR :0x440100a4 */
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_value(void)
{
    return REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_tx_rx_misc_st:0x29[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_rx_misc_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_TX_RX_MISC_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_TX_RX_MISC_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_rx_trigger_st:0x29[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_RX_TRIGGER_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_RX_TRIGGER_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_tx_trigger_st:0x29[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_TX_TRIGGER_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_TX_TRIGGER_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_prot_trigger_st:0x29[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_prot_trigger_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_PROT_TRIGGER_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_PROT_TRIGGER_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_mac_int_gen_st:0x29[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_GEN_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_MAC_INT_GEN_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_hsu_irq_st:0x29[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_HSU_IRQ_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_HSU_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_wifi_int_mac_wakeup_st:0x29[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_INT_MAC_WAKEUP_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_WIFI_INT_MAC_WAKEUP_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_dm_irq_st:0x29[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_DM_IRQ_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_DM_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_ble_irq_st:0x29[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_BLE_IRQ_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_BLE_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_bt_irq_st:0x29[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_BT_IRQ_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_BT_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox0_int_st:0x29[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_MBOX0_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_MBOX0_INT_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_mbox1_int_st:0x29[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_MBOX1_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_MBOX1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_bmc64_int_st:0x29[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_BMC64_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_BMC64_INT_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_touched_int_st:0x29[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_touched_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_TOUCHED_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_TOUCHED_INT_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_usbplug_int_st:0x29[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_USBPLUG_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_USBPLUG_INT_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_rtc_int_st:0x29[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rtc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_RTC_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_RTC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x29:cpu0_int_32_63_status->cpu0_gpio_int_st:0x29[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU0_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU0_INT_32_63_STATUS_CPU0_GPIO_INT_ST_POS)&SYS_CPU0_INT_32_63_STATUS_CPU0_GPIO_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2A //REG ADDR :0x440100a8 */
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_value(void)
{
    return REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_bmc32_int_st:0x2a[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_bmc32_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_BMC32_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_BMC32_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_irq_st:0x2a[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_host_0_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_HOST_0_IRQ_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_HOST_0_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_host_0_sec_irq_st:0x2a[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_host_0_sec_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_HOST_0_SEC_IRQ_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_HOST_0_SEC_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer_int_st:0x2a[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_TIMER_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_TIMER_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart_int_st:0x2a[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_UART_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_UART_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_pwm_int_st:0x2a[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_pwm_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_PWM_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_PWM_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c_int_st:0x2a[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_I2C_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_I2C_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi_int_st:0x2a[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_SPI_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_SPI_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sadc_int_st:0x2a[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sadc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_SADC_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_SADC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_irda_int_st:0x2a[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_irda_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_IRDA_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_IRDA_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sdio_int_st:0x2a[10], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sdio_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_SDIO_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_SDIO_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_gdma_int_st:0x2a[11], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_gdma_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_GDMA_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_GDMA_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_la_int_st:0x2a[12], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_la_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_LA_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_LA_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_timer1_int_st:0x2a[13], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_TIMER1_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_TIMER1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2c1_int_st:0x2a[14], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_I2C1_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_I2C1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart1_int_st:0x2a[15], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_UART1_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_UART1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_uart2_int_st:0x2a[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart2_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_UART2_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_UART2_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_spi1_int_st:0x2a[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_SPI1_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_SPI1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_can_int_st:0x2a[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_can_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_CAN_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_CAN_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_usb_int_st:0x2a[19], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_usb_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_USB_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_USB_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_qspi_int_st:0x2a[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_qspi_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_QSPI_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_QSPI_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_fft_int_st:0x2a[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_fft_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_FFT_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_FFT_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_sbc_int_st:0x2a[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sbc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_SBC_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_SBC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_aud_int_st:0x2a[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_aud_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_AUD_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_AUD_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_i2s_int_st:0x2a[24], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2s_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_I2S_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_I2S_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegenc_int_st:0x2a[25], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_JPEGENC_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_JPEGENC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_jpegdec_int_st:0x2a[26], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_JPEGDEC_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_JPEGDEC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_lcd_int_st:0x2a[27], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_lcd_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_LCD_INT_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_LCD_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_int_phy_mpb_st:0x2a[29], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_mpb_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_WIFI_INT_PHY_MPB_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_WIFI_INT_PHY_MPB_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_int_phy_riu_st:0x2a[30], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_riu_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_WIFI_INT_PHY_RIU_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_WIFI_INT_PHY_RIU_ST_MASK);
    return reg_value;
}

/* REG_0x2a:cpu1_int_0_31_status->cpu1_wifi_mac_int_tx_rx_timer_st:0x2a[31], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_0_31_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_0_31_STATUS_CPU1_WIFI_MAC_INT_TX_RX_TIMER_ST_POS)&SYS_CPU1_INT_0_31_STATUS_CPU1_WIFI_MAC_INT_TX_RX_TIMER_ST_MASK);
    return reg_value;
}

/* REG_0x2B //REG ADDR :0x440100ac */
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_value(void)
{
    return REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_tx_rx_misc_st:0x2b[0], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_rx_misc_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_TX_RX_MISC_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_TX_RX_MISC_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_rx_trigger_st:0x2b[1], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_RX_TRIGGER_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_RX_TRIGGER_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_tx_trigger_st:0x2b[2], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_TX_TRIGGER_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_TX_TRIGGER_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_prot_trigger_st:0x2b[3], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_prot_trigger_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_PROT_TRIGGER_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_PROT_TRIGGER_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_mac_int_gen_st:0x2b[4], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_GEN_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_MAC_INT_GEN_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_hsu_irq_st:0x2b[5], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_HSU_IRQ_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_HSU_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_wifi_int_mac_wakeup_st:0x2b[6], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_INT_MAC_WAKEUP_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_WIFI_INT_MAC_WAKEUP_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_dm_irq_st:0x2b[7], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dm_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_DM_IRQ_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_DM_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_ble_irq_st:0x2b[8], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_ble_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_BLE_IRQ_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_BLE_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bt_irq_st:0x2b[9], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bt_irq_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_BT_IRQ_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_BT_IRQ_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox0_int_st:0x2b[16], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_MBOX0_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_MBOX0_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_mbox1_int_st:0x2b[17], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_MBOX1_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_MBOX1_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_bmc64_int_st:0x2b[18], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_BMC64_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_BMC64_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_touched_int_st:0x2b[20], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_touched_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_TOUCHED_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_TOUCHED_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_usbplug_int_st:0x2b[21], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_USBPLUG_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_USBPLUG_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_rtc_int_st:0x2b[22], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rtc_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_RTC_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_RTC_INT_ST_MASK);
    return reg_value;
}

/* REG_0x2b:cpu1_int_32_63_status->cpu1_gpio_int_st:0x2b[23], ,0,R*/
static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_gpio_int_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU1_INT_32_63_STATUS_ADDR);
    reg_value = ((reg_value >> SYS_CPU1_INT_32_63_STATUS_CPU1_GPIO_INT_ST_POS)&SYS_CPU1_INT_32_63_STATUS_CPU1_GPIO_INT_ST_MASK);
    return reg_value;
}

/* REG_0x30 //REG ADDR :0x440100c0 */
static inline uint32_t sys_ll_get_gpio_config0_value(void)
{
    return REG_READ(SYS_GPIO_CONFIG0_ADDR);
}

static inline void sys_ll_set_gpio_config0_value(uint32_t value)
{
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,value);
}

/* REG_0x30:gpio_config0->sys_gpio0:0x30[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO0_POS) & SYS_GPIO_CONFIG0_SYS_GPIO0_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO0_MASK << SYS_GPIO_CONFIG0_SYS_GPIO0_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO0_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO0_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio1:0x30[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO1_POS) & SYS_GPIO_CONFIG0_SYS_GPIO1_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO1_MASK << SYS_GPIO_CONFIG0_SYS_GPIO1_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO1_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO1_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio2:0x30[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO2_POS) & SYS_GPIO_CONFIG0_SYS_GPIO2_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO2_MASK << SYS_GPIO_CONFIG0_SYS_GPIO2_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO2_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO2_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio3:0x30[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio3(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO3_POS) & SYS_GPIO_CONFIG0_SYS_GPIO3_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio3(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO3_MASK << SYS_GPIO_CONFIG0_SYS_GPIO3_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO3_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO3_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio4:0x30[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio4(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO4_POS) & SYS_GPIO_CONFIG0_SYS_GPIO4_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio4(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO4_MASK << SYS_GPIO_CONFIG0_SYS_GPIO4_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO4_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO4_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio5:0x30[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio5(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO5_POS) & SYS_GPIO_CONFIG0_SYS_GPIO5_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio5(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO5_MASK << SYS_GPIO_CONFIG0_SYS_GPIO5_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO5_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO5_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio6:0x30[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio6(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO6_POS) & SYS_GPIO_CONFIG0_SYS_GPIO6_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio6(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO6_MASK << SYS_GPIO_CONFIG0_SYS_GPIO6_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO6_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO6_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x30:gpio_config0->sys_gpio7:0x30[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config0_sys_gpio7(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG0_SYS_GPIO7_POS) & SYS_GPIO_CONFIG0_SYS_GPIO7_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config0_sys_gpio7(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG0_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG0_SYS_GPIO7_MASK << SYS_GPIO_CONFIG0_SYS_GPIO7_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG0_SYS_GPIO7_MASK) << SYS_GPIO_CONFIG0_SYS_GPIO7_POS);
    REG_WRITE(SYS_GPIO_CONFIG0_ADDR,reg_value);
}

/* REG_0x31 //REG ADDR :0x440100c4 */
static inline uint32_t sys_ll_get_gpio_config1_value(void)
{
    return REG_READ(SYS_GPIO_CONFIG1_ADDR);
}

static inline void sys_ll_set_gpio_config1_value(uint32_t value)
{
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,value);
}

/* REG_0x31:gpio_config1->sys_gpio8:0x31[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio8(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO8_POS) & SYS_GPIO_CONFIG1_SYS_GPIO8_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio8(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO8_MASK << SYS_GPIO_CONFIG1_SYS_GPIO8_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO8_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO8_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio9:0x31[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO9_POS) & SYS_GPIO_CONFIG1_SYS_GPIO9_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO9_MASK << SYS_GPIO_CONFIG1_SYS_GPIO9_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO9_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO9_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio10:0x31[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio10(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO10_POS) & SYS_GPIO_CONFIG1_SYS_GPIO10_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio10(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO10_MASK << SYS_GPIO_CONFIG1_SYS_GPIO10_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO10_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO10_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio11:0x31[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio11(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO11_POS) & SYS_GPIO_CONFIG1_SYS_GPIO11_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio11(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO11_MASK << SYS_GPIO_CONFIG1_SYS_GPIO11_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO11_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO11_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio12:0x31[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio12(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO12_POS) & SYS_GPIO_CONFIG1_SYS_GPIO12_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio12(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO12_MASK << SYS_GPIO_CONFIG1_SYS_GPIO12_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO12_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO12_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio13:0x31[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio13(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO13_POS) & SYS_GPIO_CONFIG1_SYS_GPIO13_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio13(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO13_MASK << SYS_GPIO_CONFIG1_SYS_GPIO13_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO13_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO13_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio14:0x31[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio14(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO14_POS) & SYS_GPIO_CONFIG1_SYS_GPIO14_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio14(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO14_MASK << SYS_GPIO_CONFIG1_SYS_GPIO14_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO14_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO14_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x31:gpio_config1->sys_gpio15:0x31[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config1_sys_gpio15(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG1_SYS_GPIO15_POS) & SYS_GPIO_CONFIG1_SYS_GPIO15_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config1_sys_gpio15(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG1_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG1_SYS_GPIO15_MASK << SYS_GPIO_CONFIG1_SYS_GPIO15_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG1_SYS_GPIO15_MASK) << SYS_GPIO_CONFIG1_SYS_GPIO15_POS);
    REG_WRITE(SYS_GPIO_CONFIG1_ADDR,reg_value);
}

/* REG_0x32 //REG ADDR :0x440100c8 */
static inline uint32_t sys_ll_get_gpio_config2_value(void)
{
    return REG_READ(SYS_GPIO_CONFIG2_ADDR);
}

static inline void sys_ll_set_gpio_config2_value(uint32_t value)
{
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,value);
}

/* REG_0x32:gpio_config2->sys_gpio16:0x32[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio16(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO16_POS) & SYS_GPIO_CONFIG2_SYS_GPIO16_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio16(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO16_MASK << SYS_GPIO_CONFIG2_SYS_GPIO16_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO16_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO16_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio17:0x32[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio17(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO17_POS) & SYS_GPIO_CONFIG2_SYS_GPIO17_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio17(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO17_MASK << SYS_GPIO_CONFIG2_SYS_GPIO17_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO17_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO17_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio18:0x32[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio18(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO18_POS) & SYS_GPIO_CONFIG2_SYS_GPIO18_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio18(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO18_MASK << SYS_GPIO_CONFIG2_SYS_GPIO18_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO18_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO18_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio19:0x32[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio19(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO19_POS) & SYS_GPIO_CONFIG2_SYS_GPIO19_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio19(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO19_MASK << SYS_GPIO_CONFIG2_SYS_GPIO19_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO19_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO19_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio20:0x32[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio20(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO20_POS) & SYS_GPIO_CONFIG2_SYS_GPIO20_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio20(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO20_MASK << SYS_GPIO_CONFIG2_SYS_GPIO20_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO20_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO20_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio21:0x32[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio21(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO21_POS) & SYS_GPIO_CONFIG2_SYS_GPIO21_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio21(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO21_MASK << SYS_GPIO_CONFIG2_SYS_GPIO21_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO21_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO21_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio22:0x32[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio22(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO22_POS) & SYS_GPIO_CONFIG2_SYS_GPIO22_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio22(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO22_MASK << SYS_GPIO_CONFIG2_SYS_GPIO22_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO22_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO22_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x32:gpio_config2->sys_gpio23:0x32[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config2_sys_gpio23(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG2_SYS_GPIO23_POS) & SYS_GPIO_CONFIG2_SYS_GPIO23_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config2_sys_gpio23(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG2_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG2_SYS_GPIO23_MASK << SYS_GPIO_CONFIG2_SYS_GPIO23_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG2_SYS_GPIO23_MASK) << SYS_GPIO_CONFIG2_SYS_GPIO23_POS);
    REG_WRITE(SYS_GPIO_CONFIG2_ADDR,reg_value);
}

/* REG_0x33 //REG ADDR :0x440100cc */
static inline uint32_t sys_ll_get_gpio_config3_value(void)
{
    return REG_READ(SYS_GPIO_CONFIG3_ADDR);
}

static inline void sys_ll_set_gpio_config3_value(uint32_t value)
{
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,value);
}

/* REG_0x33:gpio_config3->sys_gpio24:0x33[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio24(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO24_POS) & SYS_GPIO_CONFIG3_SYS_GPIO24_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio24(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO24_MASK << SYS_GPIO_CONFIG3_SYS_GPIO24_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO24_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO24_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio25:0x33[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio25(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO25_POS) & SYS_GPIO_CONFIG3_SYS_GPIO25_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio25(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO25_MASK << SYS_GPIO_CONFIG3_SYS_GPIO25_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO25_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO25_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio26:0x33[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio26(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO26_POS) & SYS_GPIO_CONFIG3_SYS_GPIO26_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio26(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO26_MASK << SYS_GPIO_CONFIG3_SYS_GPIO26_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO26_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO26_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio27:0x33[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio27(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO27_POS) & SYS_GPIO_CONFIG3_SYS_GPIO27_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio27(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO27_MASK << SYS_GPIO_CONFIG3_SYS_GPIO27_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO27_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO27_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio28:0x33[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio28(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO28_POS) & SYS_GPIO_CONFIG3_SYS_GPIO28_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio28(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO28_MASK << SYS_GPIO_CONFIG3_SYS_GPIO28_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO28_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO28_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio29:0x33[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio29(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO29_POS) & SYS_GPIO_CONFIG3_SYS_GPIO29_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio29(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO29_MASK << SYS_GPIO_CONFIG3_SYS_GPIO29_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO29_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO29_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio30:0x33[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio30(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO30_POS) & SYS_GPIO_CONFIG3_SYS_GPIO30_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio30(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO30_MASK << SYS_GPIO_CONFIG3_SYS_GPIO30_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO30_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO30_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x33:gpio_config3->sys_gpio31:0x33[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config3_sys_gpio31(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG3_SYS_GPIO31_POS) & SYS_GPIO_CONFIG3_SYS_GPIO31_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config3_sys_gpio31(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG3_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG3_SYS_GPIO31_MASK << SYS_GPIO_CONFIG3_SYS_GPIO31_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG3_SYS_GPIO31_MASK) << SYS_GPIO_CONFIG3_SYS_GPIO31_POS);
    REG_WRITE(SYS_GPIO_CONFIG3_ADDR,reg_value);
}

/* REG_0x34 //REG ADDR :0x440100d0 */
static inline uint32_t sys_ll_get_gpio_config4_value(void)
{
    return REG_READ(SYS_GPIO_CONFIG4_ADDR);
}

static inline void sys_ll_set_gpio_config4_value(uint32_t value)
{
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,value);
}

/* REG_0x34:gpio_config4->sys_gpio32:0x34[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio32(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO32_POS) & SYS_GPIO_CONFIG4_SYS_GPIO32_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio32(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO32_MASK << SYS_GPIO_CONFIG4_SYS_GPIO32_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO32_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO32_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio33:0x34[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio33(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO33_POS) & SYS_GPIO_CONFIG4_SYS_GPIO33_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio33(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO33_MASK << SYS_GPIO_CONFIG4_SYS_GPIO33_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO33_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO33_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio34:0x34[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio34(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO34_POS) & SYS_GPIO_CONFIG4_SYS_GPIO34_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio34(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO34_MASK << SYS_GPIO_CONFIG4_SYS_GPIO34_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO34_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO34_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio35:0x34[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio35(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO35_POS) & SYS_GPIO_CONFIG4_SYS_GPIO35_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio35(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO35_MASK << SYS_GPIO_CONFIG4_SYS_GPIO35_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO35_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO35_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio36:0x34[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio36(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO36_POS) & SYS_GPIO_CONFIG4_SYS_GPIO36_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio36(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO36_MASK << SYS_GPIO_CONFIG4_SYS_GPIO36_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO36_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO36_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio37:0x34[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio37(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO37_POS) & SYS_GPIO_CONFIG4_SYS_GPIO37_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio37(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO37_MASK << SYS_GPIO_CONFIG4_SYS_GPIO37_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO37_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO37_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio38:0x34[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio38(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO38_POS) & SYS_GPIO_CONFIG4_SYS_GPIO38_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio38(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO38_MASK << SYS_GPIO_CONFIG4_SYS_GPIO38_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO38_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO38_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x34:gpio_config4->sys_gpio39:0x34[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config4_sys_gpio39(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG4_SYS_GPIO39_POS) & SYS_GPIO_CONFIG4_SYS_GPIO39_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config4_sys_gpio39(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG4_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG4_SYS_GPIO39_MASK << SYS_GPIO_CONFIG4_SYS_GPIO39_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG4_SYS_GPIO39_MASK) << SYS_GPIO_CONFIG4_SYS_GPIO39_POS);
    REG_WRITE(SYS_GPIO_CONFIG4_ADDR,reg_value);
}

/* REG_0x35 //REG ADDR :0x440100d4 */
static inline uint32_t sys_ll_get_gpio_config5_value(void)
{
    return REG_READ(SYS_GPIO_CONFIG5_ADDR);
}

static inline void sys_ll_set_gpio_config5_value(uint32_t value)
{
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,value);
}

/* REG_0x35:gpio_config5->sys_gpio40:0x35[3:0],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio40(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO40_POS) & SYS_GPIO_CONFIG5_SYS_GPIO40_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio40(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO40_MASK << SYS_GPIO_CONFIG5_SYS_GPIO40_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO40_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO40_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio41:0x35[7:4],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio41(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO41_POS) & SYS_GPIO_CONFIG5_SYS_GPIO41_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio41(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO41_MASK << SYS_GPIO_CONFIG5_SYS_GPIO41_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO41_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO41_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio42:0x35[11:8],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio42(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO42_POS) & SYS_GPIO_CONFIG5_SYS_GPIO42_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio42(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO42_MASK << SYS_GPIO_CONFIG5_SYS_GPIO42_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO42_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO42_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio43:0x35[15:12],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio43(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO43_POS) & SYS_GPIO_CONFIG5_SYS_GPIO43_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio43(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO43_MASK << SYS_GPIO_CONFIG5_SYS_GPIO43_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO43_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO43_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio44:0x35[19:16],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio44(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO44_POS) & SYS_GPIO_CONFIG5_SYS_GPIO44_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio44(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO44_MASK << SYS_GPIO_CONFIG5_SYS_GPIO44_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO44_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO44_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio45:0x35[23:20],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio45(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO45_POS) & SYS_GPIO_CONFIG5_SYS_GPIO45_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio45(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO45_MASK << SYS_GPIO_CONFIG5_SYS_GPIO45_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO45_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO45_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio46:0x35[27:24],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio46(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO46_POS) & SYS_GPIO_CONFIG5_SYS_GPIO46_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio46(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO46_MASK << SYS_GPIO_CONFIG5_SYS_GPIO46_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO46_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO46_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x35:gpio_config5->sys_gpio47:0x35[31:28],0:mode1; 1:mode2; 2:mode3; 3:mode4 4:mode5,0,R/W*/
static inline uint32_t sys_ll_get_gpio_config5_sys_gpio47(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value = ((reg_value >> SYS_GPIO_CONFIG5_SYS_GPIO47_POS) & SYS_GPIO_CONFIG5_SYS_GPIO47_MASK);
    return reg_value;
}

static inline void sys_ll_set_gpio_config5_sys_gpio47(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_GPIO_CONFIG5_ADDR);
    reg_value &= ~(SYS_GPIO_CONFIG5_SYS_GPIO47_MASK << SYS_GPIO_CONFIG5_SYS_GPIO47_POS);
    reg_value |= ((value & SYS_GPIO_CONFIG5_SYS_GPIO47_MASK) << SYS_GPIO_CONFIG5_SYS_GPIO47_POS);
    REG_WRITE(SYS_GPIO_CONFIG5_ADDR,reg_value);
}

/* REG_0x38 //REG ADDR :0x440100e0 */
static inline uint32_t sys_ll_get_sys_debug_config0_value(void)
{
    return REG_READ(SYS_SYS_DEBUG_CONFIG0_ADDR);
}

static inline void sys_ll_set_sys_debug_config0_value(uint32_t value)
{
    REG_WRITE(SYS_SYS_DEBUG_CONFIG0_ADDR,value);
}

/* REG_0x38:sys_debug_config0->dbug_config0:0x38[31:0], ,0,R/W*/
static inline uint32_t sys_ll_get_sys_debug_config0_dbug_config0(void)
{
    return REG_READ(SYS_SYS_DEBUG_CONFIG0_ADDR);
}

static inline void sys_ll_set_sys_debug_config0_dbug_config0(uint32_t value)
{
    REG_WRITE(SYS_SYS_DEBUG_CONFIG0_ADDR,value);
}

/* REG_0x39 //REG ADDR :0x440100e4 */
static inline uint32_t sys_ll_get_sys_debug_config1_value(void)
{
    return REG_READ(SYS_SYS_DEBUG_CONFIG1_ADDR);
}

static inline void sys_ll_set_sys_debug_config1_value(uint32_t value)
{
    REG_WRITE(SYS_SYS_DEBUG_CONFIG1_ADDR,value);
}

/* REG_0x39:sys_debug_config1->dbug_config1:0x39[31:0],0: btsp_debug[0:32]        1: btsp_debug[32+:32]           2: btsp_debug[64+:32]  4:btsp_debug[96+:6]       5:wifip_mac_dbg[31:0]           6: wifip_phy_dbg[31:0]                            default:  dbug_config0                   ,0,R/W*/
static inline uint32_t sys_ll_get_sys_debug_config1_dbug_config1(void)
{
    return REG_READ(SYS_SYS_DEBUG_CONFIG1_ADDR);
}

static inline void sys_ll_set_sys_debug_config1_dbug_config1(uint32_t value)
{
    REG_WRITE(SYS_SYS_DEBUG_CONFIG1_ADDR,value);
}

/* REG_0x40 //REG ADDR :0x44010100 */
static inline uint32_t sys_ll_get_ana_reg0_value(void)
{
    return REG_READ(SYS_ANA_REG0_ADDR);
}

static inline void sys_ll_set_ana_reg0_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,value);
}

/* REG_0x40:ana_reg0->ck2652sel:0x40[0],1:26MHz/0:52MHz,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_ck2652sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_CK2652SEL_POS) & SYS_ANA_REG0_CK2652SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_ck2652sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_CK2652SEL_MASK << SYS_ANA_REG0_CK2652SEL_POS);
    reg_value |= ((value & SYS_ANA_REG0_CK2652SEL_MASK) << SYS_ANA_REG0_CK2652SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->cp:0x40[3:1],cp curent control 0to 350uA 50uA step,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_cp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_CP_POS) & SYS_ANA_REG0_CP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_cp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_CP_MASK << SYS_ANA_REG0_CP_POS);
    reg_value |= ((value & SYS_ANA_REG0_CP_MASK) << SYS_ANA_REG0_CP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->spideten:0x40[4],unlock detect enable fron spi 1:enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_spideten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_SPIDETEN_POS) & SYS_ANA_REG0_SPIDETEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_spideten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_SPIDETEN_MASK << SYS_ANA_REG0_SPIDETEN_POS);
    reg_value |= ((value & SYS_ANA_REG0_SPIDETEN_MASK) << SYS_ANA_REG0_SPIDETEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->hvref:0x40[6:5],high vth control for unlock detect 00:0.85V;01:0.9V;10:0.95V;11:1.05V,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_hvref(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_HVREF_POS) & SYS_ANA_REG0_HVREF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_hvref(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_HVREF_MASK << SYS_ANA_REG0_HVREF_POS);
    reg_value |= ((value & SYS_ANA_REG0_HVREF_MASK) << SYS_ANA_REG0_HVREF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->lvref:0x40[8:7],low vth control for unlock detect 00:0.2V;01:0.3V;10:0.35V;11:0.4V,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_lvref(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_LVREF_POS) & SYS_ANA_REG0_LVREF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_lvref(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_LVREF_MASK << SYS_ANA_REG0_LVREF_POS);
    reg_value |= ((value & SYS_ANA_REG0_LVREF_MASK) << SYS_ANA_REG0_LVREF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->Rzctrl26M:0x40[9],Rz ctrl in 26M mode:1:normal;0:add 14K,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_rzctrl26m(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_RZCTRL26M_POS) & SYS_ANA_REG0_RZCTRL26M_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_rzctrl26m(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_RZCTRL26M_MASK << SYS_ANA_REG0_RZCTRL26M_POS);
    reg_value |= ((value & SYS_ANA_REG0_RZCTRL26M_MASK) << SYS_ANA_REG0_RZCTRL26M_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->LoopRzctrl:0x40[13:10],Rz ctrl:2K to 17K,1K step,9,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_looprzctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_LOOPRZCTRL_POS) & SYS_ANA_REG0_LOOPRZCTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_looprzctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_LOOPRZCTRL_MASK << SYS_ANA_REG0_LOOPRZCTRL_POS);
    reg_value |= ((value & SYS_ANA_REG0_LOOPRZCTRL_MASK) << SYS_ANA_REG0_LOOPRZCTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->rpc:0x40[15:14],second pole Rp ctrl:00:30K;01:10K;10:22K;11:2K,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_rpc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_RPC_POS) & SYS_ANA_REG0_RPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_rpc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_RPC_MASK << SYS_ANA_REG0_RPC_POS);
    reg_value |= ((value & SYS_ANA_REG0_RPC_MASK) << SYS_ANA_REG0_RPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->nsyn:0x40[16],N divider rst,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_nsyn(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_NSYN_POS) & SYS_ANA_REG0_NSYN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_nsyn(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_NSYN_MASK << SYS_ANA_REG0_NSYN_POS);
    reg_value |= ((value & SYS_ANA_REG0_NSYN_MASK) << SYS_ANA_REG0_NSYN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->cksel:0x40[18:17],0:26M;1:40M;2:24M;3:19.2M,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_cksel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_CKSEL_POS) & SYS_ANA_REG0_CKSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_cksel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_CKSEL_MASK << SYS_ANA_REG0_CKSEL_POS);
    reg_value |= ((value & SYS_ANA_REG0_CKSEL_MASK) << SYS_ANA_REG0_CKSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->spitrig:0x40[19],SPI band selection trigger signal,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_spitrig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_SPITRIG_POS) & SYS_ANA_REG0_SPITRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_spitrig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_SPITRIG_MASK << SYS_ANA_REG0_SPITRIG_POS);
    reg_value |= ((value & SYS_ANA_REG0_SPITRIG_MASK) << SYS_ANA_REG0_SPITRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->band:0x40[24:20],band manual value/band[0] ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_band(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_BAND_POS) & SYS_ANA_REG0_BAND_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_band(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_BAND_MASK << SYS_ANA_REG0_BAND_POS);
    reg_value |= ((value & SYS_ANA_REG0_BAND_MASK) << SYS_ANA_REG0_BAND_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->bandmanual:0x40[25],1:band manual;0:band auto,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_bandmanual(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_BANDMANUAL_POS) & SYS_ANA_REG0_BANDMANUAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_bandmanual(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_BANDMANUAL_MASK << SYS_ANA_REG0_BANDMANUAL_POS);
    reg_value |= ((value & SYS_ANA_REG0_BANDMANUAL_MASK) << SYS_ANA_REG0_BANDMANUAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->dsptrig:0x40[26],band selection trigger signal,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_dsptrig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_DSPTRIG_POS) & SYS_ANA_REG0_DSPTRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_dsptrig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_DSPTRIG_MASK << SYS_ANA_REG0_DSPTRIG_POS);
    reg_value |= ((value & SYS_ANA_REG0_DSPTRIG_MASK) << SYS_ANA_REG0_DSPTRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->lpen_dpll:0x40[27],dpll low power mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_lpen_dpll(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_LPEN_DPLL_POS) & SYS_ANA_REG0_LPEN_DPLL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_lpen_dpll(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_LPEN_DPLL_MASK << SYS_ANA_REG0_LPEN_DPLL_POS);
    reg_value |= ((value & SYS_ANA_REG0_LPEN_DPLL_MASK) << SYS_ANA_REG0_LPEN_DPLL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->xamp:0x40[31:28],xtal OSC amp control/xamp<0> shared with pll_cktst_en,0xF,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_xamp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_XAMP_POS) & SYS_ANA_REG0_XAMP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_xamp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_XAMP_MASK << SYS_ANA_REG0_XAMP_POS);
    reg_value |= ((value & SYS_ANA_REG0_XAMP_MASK) << SYS_ANA_REG0_XAMP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x41 //REG ADDR :0x44010104 */
static inline uint32_t sys_ll_get_ana_reg1_value(void)
{
    return REG_READ(SYS_ANA_REG1_ADDR);
}

static inline void sys_ll_set_ana_reg1_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,value);
}

/* REG_0x41:ana_reg1->dpll_vrefsel:0x41[1],dpll ldo reference voltage selection  0:vbg_aon/1:vbg_cal,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_dpll_vrefsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_DPLL_VREFSEL_POS) & SYS_ANA_REG1_DPLL_VREFSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_dpll_vrefsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_DPLL_VREFSEL_MASK << SYS_ANA_REG1_DPLL_VREFSEL_POS);
    reg_value |= ((value & SYS_ANA_REG1_DPLL_VREFSEL_MASK) << SYS_ANA_REG1_DPLL_VREFSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->msw:0x41[10:2],set the frequency of DCO manual,70,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_msw(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_MSW_POS) & SYS_ANA_REG1_MSW_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_msw(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_MSW_MASK << SYS_ANA_REG1_MSW_POS);
    reg_value |= ((value & SYS_ANA_REG1_MSW_MASK) << SYS_ANA_REG1_MSW_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->ictrl:0x41[13:11],controlling the bias cuttent of DCO core,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_ICTRL_POS) & SYS_ANA_REG1_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_ICTRL_MASK << SYS_ANA_REG1_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG1_ICTRL_MASK) << SYS_ANA_REG1_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->osc_trig:0x41[14],reset the DCO core by spi to make it oscillate again,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_osc_trig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_OSC_TRIG_POS) & SYS_ANA_REG1_OSC_TRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_osc_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_OSC_TRIG_MASK << SYS_ANA_REG1_OSC_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG1_OSC_TRIG_MASK) << SYS_ANA_REG1_OSC_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->osccal_trig:0x41[15],trigger the action of callibration in the DCO,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_osccal_trig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_OSCCAL_TRIG_POS) & SYS_ANA_REG1_OSCCAL_TRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_osccal_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_OSCCAL_TRIG_MASK << SYS_ANA_REG1_OSCCAL_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG1_OSCCAL_TRIG_MASK) << SYS_ANA_REG1_OSCCAL_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->cnti:0x41[24:16],set the controlling work of calibration in the DCO block to get the different frequency,C0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_cnti(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_CNTI_POS) & SYS_ANA_REG1_CNTI_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_cnti(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_CNTI_MASK << SYS_ANA_REG1_CNTI_POS);
    reg_value |= ((value & SYS_ANA_REG1_CNTI_MASK) << SYS_ANA_REG1_CNTI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->spi_rst:0x41[25],reset the calibration block of DCO by spi,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_spi_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_SPI_RST_POS) & SYS_ANA_REG1_SPI_RST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_spi_rst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_SPI_RST_MASK << SYS_ANA_REG1_SPI_RST_POS);
    reg_value |= ((value & SYS_ANA_REG1_SPI_RST_MASK) << SYS_ANA_REG1_SPI_RST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->amsel:0x41[26],disable the calibration function of the DCO,set the frequency of DCO manual,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_amsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_AMSEL_POS) & SYS_ANA_REG1_AMSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_amsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_AMSEL_MASK << SYS_ANA_REG1_AMSEL_POS);
    reg_value |= ((value & SYS_ANA_REG1_AMSEL_MASK) << SYS_ANA_REG1_AMSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->divctrl:0x41[29:27],controlling the value of divider in the DCO to get the different frequency,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_divctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_DIVCTRL_POS) & SYS_ANA_REG1_DIVCTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_divctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_DIVCTRL_MASK << SYS_ANA_REG1_DIVCTRL_POS);
    reg_value |= ((value & SYS_ANA_REG1_DIVCTRL_MASK) << SYS_ANA_REG1_DIVCTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->dco_tsten:0x41[30],dco test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_dco_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_DCO_TSTEN_POS) & SYS_ANA_REG1_DCO_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_dco_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_DCO_TSTEN_MASK << SYS_ANA_REG1_DCO_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG1_DCO_TSTEN_MASK) << SYS_ANA_REG1_DCO_TSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->rosc_tsten:0x41[31],rosc test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg1_rosc_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG1_ROSC_TSTEN_POS) & SYS_ANA_REG1_ROSC_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg1_rosc_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_ROSC_TSTEN_MASK << SYS_ANA_REG1_ROSC_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG1_ROSC_TSTEN_MASK) << SYS_ANA_REG1_ROSC_TSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x42 //REG ADDR :0x44010108 */
static inline uint32_t sys_ll_get_ana_reg2_value(void)
{
    return REG_READ(SYS_ANA_REG2_ADDR);
}

static inline void sys_ll_set_ana_reg2_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,value);
}

/* REG_0x42:ana_reg2->pwmscmen:0x42[0],buck nmos disable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_pwmscmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_PWMSCMEN_POS) & SYS_ANA_REG2_PWMSCMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_pwmscmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_PWMSCMEN_MASK << SYS_ANA_REG2_PWMSCMEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_PWMSCMEN_MASK) << SYS_ANA_REG2_PWMSCMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->buck_fasten:0x42[1],buck EA fast transient enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_buck_fasten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_BUCK_FASTEN_POS) & SYS_ANA_REG2_BUCK_FASTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_buck_fasten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_BUCK_FASTEN_MASK << SYS_ANA_REG2_BUCK_FASTEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_BUCK_FASTEN_MASK) << SYS_ANA_REG2_BUCK_FASTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->cls:0x42[4:2],buck current limit setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_cls(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_CLS_POS) & SYS_ANA_REG2_CLS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_cls(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_CLS_MASK << SYS_ANA_REG2_CLS_POS);
    reg_value |= ((value & SYS_ANA_REG2_CLS_MASK) << SYS_ANA_REG2_CLS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->pfms:0x42[9:5],buck freewheeling damping enable(=1) ,13,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_pfms(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_PFMS_POS) & SYS_ANA_REG2_PFMS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_pfms(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_PFMS_MASK << SYS_ANA_REG2_PFMS_POS);
    reg_value |= ((value & SYS_ANA_REG2_PFMS_MASK) << SYS_ANA_REG2_PFMS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->ripc:0x42[12:10],buck pfm mode voltage ripple control setting,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_ripc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_RIPC_POS) & SYS_ANA_REG2_RIPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_ripc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_RIPC_MASK << SYS_ANA_REG2_RIPC_POS);
    reg_value |= ((value & SYS_ANA_REG2_RIPC_MASK) << SYS_ANA_REG2_RIPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->rampc:0x42[16:13],buck ramping compensation setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_rampc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_RAMPC_POS) & SYS_ANA_REG2_RAMPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_rampc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_RAMPC_MASK << SYS_ANA_REG2_RAMPC_POS);
    reg_value |= ((value & SYS_ANA_REG2_RAMPC_MASK) << SYS_ANA_REG2_RAMPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->rampcen:0x42[17],buck ramping compensation enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_rampcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_RAMPCEN_POS) & SYS_ANA_REG2_RAMPCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_rampcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_RAMPCEN_MASK << SYS_ANA_REG2_RAMPCEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_RAMPCEN_MASK) << SYS_ANA_REG2_RAMPCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->dpfmen:0x42[18],buck pfm mode current reduce enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_dpfmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_DPFMEN_POS) & SYS_ANA_REG2_DPFMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_dpfmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_DPFMEN_MASK << SYS_ANA_REG2_DPFMEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_DPFMEN_MASK) << SYS_ANA_REG2_DPFMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->pfmen:0x42[19],buck pfm mode enable(=1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_pfmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_PFMEN_POS) & SYS_ANA_REG2_PFMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_pfmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_PFMEN_MASK << SYS_ANA_REG2_PFMEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_PFMEN_MASK) << SYS_ANA_REG2_PFMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->forcepfm:0x42[20],buck force pfm mode(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_forcepfm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_FORCEPFM_POS) & SYS_ANA_REG2_FORCEPFM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_forcepfm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_FORCEPFM_MASK << SYS_ANA_REG2_FORCEPFM_POS);
    reg_value |= ((value & SYS_ANA_REG2_FORCEPFM_MASK) << SYS_ANA_REG2_FORCEPFM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->swrsten:0x42[21],buck freewheeling damping enable(=1) ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_swrsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_SWRSTEN_POS) & SYS_ANA_REG2_SWRSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_swrsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_SWRSTEN_MASK << SYS_ANA_REG2_SWRSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_SWRSTEN_MASK) << SYS_ANA_REG2_SWRSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->tmposel:0x42[23:22],buck mpo pulse width control 0--shortest   3---longest,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_tmposel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_TMPOSEL_POS) & SYS_ANA_REG2_TMPOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_tmposel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_TMPOSEL_MASK << SYS_ANA_REG2_TMPOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG2_TMPOSEL_MASK) << SYS_ANA_REG2_TMPOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->mpoen:0x42[24],buck mpo mode enable( =1),1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_mpoen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_MPOEN_POS) & SYS_ANA_REG2_MPOEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_mpoen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_MPOEN_MASK << SYS_ANA_REG2_MPOEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_MPOEN_MASK) << SYS_ANA_REG2_MPOEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->spi_latchb:0x42[25],spi latch disable 0:latch;1:no latch,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_spi_latchb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_SPI_LATCHB_POS) & SYS_ANA_REG2_SPI_LATCHB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_spi_latchb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_SPI_LATCHB_MASK << SYS_ANA_REG2_SPI_LATCHB_POS);
    reg_value |= ((value & SYS_ANA_REG2_SPI_LATCHB_MASK) << SYS_ANA_REG2_SPI_LATCHB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->ldosel:0x42[26],ldo/buck select, 0:buck;1:LDO,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_ldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_LDOSEL_POS) & SYS_ANA_REG2_LDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_ldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_LDOSEL_MASK << SYS_ANA_REG2_LDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG2_LDOSEL_MASK) << SYS_ANA_REG2_LDOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->iovoc:0x42[29:27],ioldo output voltage select 0:2.9V,….7:3.6V,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_iovoc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_IOVOC_POS) & SYS_ANA_REG2_IOVOC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_iovoc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_IOVOC_MASK << SYS_ANA_REG2_IOVOC_POS);
    reg_value |= ((value & SYS_ANA_REG2_IOVOC_MASK) << SYS_ANA_REG2_IOVOC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->vbpbuf_hp:0x42[30],vbspbuffer high power enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_vbpbuf_hp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_VBPBUF_HP_POS) & SYS_ANA_REG2_VBPBUF_HP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_vbpbuf_hp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_VBPBUF_HP_MASK << SYS_ANA_REG2_VBPBUF_HP_POS);
    reg_value |= ((value & SYS_ANA_REG2_VBPBUF_HP_MASK) << SYS_ANA_REG2_VBPBUF_HP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->bypassen:0x42[31],ioldo bypass enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_bypassen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_BYPASSEN_POS) & SYS_ANA_REG2_BYPASSEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_bypassen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_BYPASSEN_MASK << SYS_ANA_REG2_BYPASSEN_POS);
    reg_value |= ((value & SYS_ANA_REG2_BYPASSEN_MASK) << SYS_ANA_REG2_BYPASSEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x43 //REG ADDR :0x4401010c */
static inline uint32_t sys_ll_get_ana_reg3_value(void)
{
    return REG_READ(SYS_ANA_REG3_ADDR);
}

static inline void sys_ll_set_ana_reg3_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,value);
}

/* REG_0x43:ana_reg3->zcdta:0x43[4:0],buck zcd delay tune setting,1F,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdta(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ZCDTA_POS) & SYS_ANA_REG3_ZCDTA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_zcdta(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ZCDTA_MASK << SYS_ANA_REG3_ZCDTA_POS);
    reg_value |= ((value & SYS_ANA_REG3_ZCDTA_MASK) << SYS_ANA_REG3_ZCDTA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->zcdcala:0x43[10:5],buck zcd offset cali setting,E,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdcala(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ZCDCALA_POS) & SYS_ANA_REG3_ZCDCALA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_zcdcala(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ZCDCALA_MASK << SYS_ANA_REG3_ZCDCALA_POS);
    reg_value |= ((value & SYS_ANA_REG3_ZCDCALA_MASK) << SYS_ANA_REG3_ZCDCALA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->zcdmen:0x43[11],buck zcd manual cali enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ZCDMEN_POS) & SYS_ANA_REG3_ZCDMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_zcdmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ZCDMEN_MASK << SYS_ANA_REG3_ZCDMEN_POS);
    reg_value |= ((value & SYS_ANA_REG3_ZCDMEN_MASK) << SYS_ANA_REG3_ZCDMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->zcdcalen:0x43[12],buck zcd calibration enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdcalen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ZCDCALEN_POS) & SYS_ANA_REG3_ZCDCALEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_zcdcalen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ZCDCALEN_MASK << SYS_ANA_REG3_ZCDCALEN_POS);
    reg_value |= ((value & SYS_ANA_REG3_ZCDCALEN_MASK) << SYS_ANA_REG3_ZCDCALEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->zcdcal_tri:0x43[13],buck zcd auto cali triggle(0-->1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_zcdcal_tri(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ZCDCAL_TRI_POS) & SYS_ANA_REG3_ZCDCAL_TRI_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_zcdcal_tri(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ZCDCAL_TRI_MASK << SYS_ANA_REG3_ZCDCAL_TRI_POS);
    reg_value |= ((value & SYS_ANA_REG3_ZCDCAL_TRI_MASK) << SYS_ANA_REG3_ZCDCAL_TRI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->mroscsel:0x43[14],buck oscillator manual cali. enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_mroscsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_MROSCSEL_POS) & SYS_ANA_REG3_MROSCSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_mroscsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_MROSCSEL_MASK << SYS_ANA_REG3_MROSCSEL_POS);
    reg_value |= ((value & SYS_ANA_REG3_MROSCSEL_MASK) << SYS_ANA_REG3_MROSCSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->mfsel:0x43[17:15],buck oscillator manual fsel  ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_mfsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_MFSEL_POS) & SYS_ANA_REG3_MFSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_mfsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_MFSEL_MASK << SYS_ANA_REG3_MFSEL_POS);
    reg_value |= ((value & SYS_ANA_REG3_MFSEL_MASK) << SYS_ANA_REG3_MFSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->mroscbcal:0x43[21:18],buck oscillator manual cap_cal  0xA---500k 0xB--1M 0x9---2M,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_mroscbcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_MROSCBCAL_POS) & SYS_ANA_REG3_MROSCBCAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_mroscbcal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_MROSCBCAL_MASK << SYS_ANA_REG3_MROSCBCAL_POS);
    reg_value |= ((value & SYS_ANA_REG3_MROSCBCAL_MASK) << SYS_ANA_REG3_MROSCBCAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->osccaltrig:0x43[22],buck oscillator manual cali. enable(=1),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_osccaltrig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_OSCCALTRIG_POS) & SYS_ANA_REG3_OSCCALTRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_osccaltrig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_OSCCALTRIG_MASK << SYS_ANA_REG3_OSCCALTRIG_POS);
    reg_value |= ((value & SYS_ANA_REG3_OSCCALTRIG_MASK) << SYS_ANA_REG3_OSCCALTRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->ckintsel:0x43[23],buck clock source select  1-- ring oscillator   0--divider,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_ckintsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_CKINTSEL_POS) & SYS_ANA_REG3_CKINTSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_ckintsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_CKINTSEL_MASK << SYS_ANA_REG3_CKINTSEL_POS);
    reg_value |= ((value & SYS_ANA_REG3_CKINTSEL_MASK) << SYS_ANA_REG3_CKINTSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->ckfs:0x43[25:24],buck output clock freq. select   0--500k 1---1M  2--2M  3--4M,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_ckfs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_CKFS_POS) & SYS_ANA_REG3_CKFS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_ckfs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_CKFS_MASK << SYS_ANA_REG3_CKFS_POS);
    reg_value |= ((value & SYS_ANA_REG3_CKFS_MASK) << SYS_ANA_REG3_CKFS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->vlsel_ldodig:0x43[28:26],digldo output voltage select(low power)  0:0.6V,…..7:1.4V,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_vlsel_ldodig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_VLSEL_LDODIG_POS) & SYS_ANA_REG3_VLSEL_LDODIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_vlsel_ldodig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_VLSEL_LDODIG_MASK << SYS_ANA_REG3_VLSEL_LDODIG_POS);
    reg_value |= ((value & SYS_ANA_REG3_VLSEL_LDODIG_MASK) << SYS_ANA_REG3_VLSEL_LDODIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->vhsel_ldodig:0x43[31:29],digldo output voltage select(high power)  0:0.6V,…..7:1.4V,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_vhsel_ldodig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_VHSEL_LDODIG_POS) & SYS_ANA_REG3_VHSEL_LDODIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_vhsel_ldodig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_VHSEL_LDODIG_MASK << SYS_ANA_REG3_VHSEL_LDODIG_POS);
    reg_value |= ((value & SYS_ANA_REG3_VHSEL_LDODIG_MASK) << SYS_ANA_REG3_VHSEL_LDODIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x44 //REG ADDR :0x44010110 */
static inline uint32_t sys_ll_get_ana_reg4_value(void)
{
    return REG_READ(SYS_ANA_REG4_ADDR);
}

static inline void sys_ll_set_ana_reg4_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,value);
}

/* REG_0x44:ana_reg4->cb_manu_val:0x44[9:5],CB Calibration Manual Value,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cb_manu_val(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_CB_MANU_VAL_POS) & SYS_ANA_REG4_CB_MANU_VAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_cb_manu_val(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_CB_MANU_VAL_MASK << SYS_ANA_REG4_CB_MANU_VAL_POS);
    reg_value |= ((value & SYS_ANA_REG4_CB_MANU_VAL_MASK) << SYS_ANA_REG4_CB_MANU_VAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->cb_cal_trig:0x44[10],CB Calibration Trigger,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cb_cal_trig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_CB_CAL_TRIG_POS) & SYS_ANA_REG4_CB_CAL_TRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_cb_cal_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_CB_CAL_TRIG_MASK << SYS_ANA_REG4_CB_CAL_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG4_CB_CAL_TRIG_MASK) << SYS_ANA_REG4_CB_CAL_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->cb_cal_manu:0x44[11],CB Calibration Manual Mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cb_cal_manu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_CB_CAL_MANU_POS) & SYS_ANA_REG4_CB_CAL_MANU_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_cb_cal_manu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_CB_CAL_MANU_MASK << SYS_ANA_REG4_CB_CAL_MANU_POS);
    reg_value |= ((value & SYS_ANA_REG4_CB_CAL_MANU_MASK) << SYS_ANA_REG4_CB_CAL_MANU_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->rosc_cal_intval:0x44[14:12],Rosc Calibration Interlval 0.25s~2s,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_intval(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ROSC_CAL_INTVAL_POS) & SYS_ANA_REG4_ROSC_CAL_INTVAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_intval(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ROSC_CAL_INTVAL_MASK << SYS_ANA_REG4_ROSC_CAL_INTVAL_POS);
    reg_value |= ((value & SYS_ANA_REG4_ROSC_CAL_INTVAL_MASK) << SYS_ANA_REG4_ROSC_CAL_INTVAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->manu_cin:0x44[21:15],Rosc Calibration Manual Cin,40,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_manu_cin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_MANU_CIN_POS) & SYS_ANA_REG4_MANU_CIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_manu_cin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_MANU_CIN_MASK << SYS_ANA_REG4_MANU_CIN_POS);
    reg_value |= ((value & SYS_ANA_REG4_MANU_CIN_MASK) << SYS_ANA_REG4_MANU_CIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->manu_fin:0x44[26:22],Rosc Calibration Manual Fin,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_manu_fin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_MANU_FIN_POS) & SYS_ANA_REG4_MANU_FIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_manu_fin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_MANU_FIN_MASK << SYS_ANA_REG4_MANU_FIN_POS);
    reg_value |= ((value & SYS_ANA_REG4_MANU_FIN_MASK) << SYS_ANA_REG4_MANU_FIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->rosc_cal_mode:0x44[27],Rosc Calibration Mode:; 0x1: 32K; 0x0: 31.25K,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ROSC_CAL_MODE_POS) & SYS_ANA_REG4_ROSC_CAL_MODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ROSC_CAL_MODE_MASK << SYS_ANA_REG4_ROSC_CAL_MODE_POS);
    reg_value |= ((value & SYS_ANA_REG4_ROSC_CAL_MODE_MASK) << SYS_ANA_REG4_ROSC_CAL_MODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->rosc_cal_trig:0x44[28],Rosc Calibration Trigger,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_trig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ROSC_CAL_TRIG_POS) & SYS_ANA_REG4_ROSC_CAL_TRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ROSC_CAL_TRIG_MASK << SYS_ANA_REG4_ROSC_CAL_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG4_ROSC_CAL_TRIG_MASK) << SYS_ANA_REG4_ROSC_CAL_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->rosc_cal_en:0x44[29],Rosc Calibration Enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_cal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ROSC_CAL_EN_POS) & SYS_ANA_REG4_ROSC_CAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rosc_cal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ROSC_CAL_EN_MASK << SYS_ANA_REG4_ROSC_CAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG4_ROSC_CAL_EN_MASK) << SYS_ANA_REG4_ROSC_CAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->rosc_manu_en:0x44[30],Rosc Calibration Manual Mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_manu_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ROSC_MANU_EN_POS) & SYS_ANA_REG4_ROSC_MANU_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rosc_manu_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ROSC_MANU_EN_MASK << SYS_ANA_REG4_ROSC_MANU_EN_POS);
    reg_value |= ((value & SYS_ANA_REG4_ROSC_MANU_EN_MASK) << SYS_ANA_REG4_ROSC_MANU_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->rosc_tsten:0x44[31],Rosc test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rosc_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ROSC_TSTEN_POS) & SYS_ANA_REG4_ROSC_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rosc_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ROSC_TSTEN_MASK << SYS_ANA_REG4_ROSC_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG4_ROSC_TSTEN_MASK) << SYS_ANA_REG4_ROSC_TSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x45 //REG ADDR :0x44010114 */
static inline uint32_t sys_ll_get_ana_reg5_value(void)
{
    return REG_READ(SYS_ANA_REG5_ADDR);
}

static inline void sys_ll_set_ana_reg5_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,value);
}

/* REG_0x45:ana_reg5->vref_scale:0x45[0],gadc reference voltage scale enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vref_scale(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_VREF_SCALE_POS) & SYS_ANA_REG5_VREF_SCALE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_vref_scale(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_VREF_SCALE_MASK << SYS_ANA_REG5_VREF_SCALE_POS);
    reg_value |= ((value & SYS_ANA_REG5_VREF_SCALE_MASK) << SYS_ANA_REG5_VREF_SCALE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->dccal_en:0x45[1],gadc DC calibration enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_dccal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_DCCAL_EN_POS) & SYS_ANA_REG5_DCCAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_dccal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_DCCAL_EN_MASK << SYS_ANA_REG5_DCCAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG5_DCCAL_EN_MASK) << SYS_ANA_REG5_DCCAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->xtalh_ctune:0x45[8:2],xtalh load cap tuning,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_xtalh_ctune(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_XTALH_CTUNE_POS) & SYS_ANA_REG5_XTALH_CTUNE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_xtalh_ctune(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_XTALH_CTUNE_MASK << SYS_ANA_REG5_XTALH_CTUNE_POS);
    reg_value |= ((value & SYS_ANA_REG5_XTALH_CTUNE_MASK) << SYS_ANA_REG5_XTALH_CTUNE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->cktst_sel:0x45[10:9],clock test signal selection rosc/xtall/dco/dpll,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_cktst_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_CKTST_SEL_POS) & SYS_ANA_REG5_CKTST_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_cktst_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_CKTST_SEL_MASK << SYS_ANA_REG5_CKTST_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG5_CKTST_SEL_MASK) << SYS_ANA_REG5_CKTST_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->ck_tst_enbale:0x45[11],system clock test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_ck_tst_enbale(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_CK_TST_ENBALE_POS) & SYS_ANA_REG5_CK_TST_ENBALE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_ck_tst_enbale(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_CK_TST_ENBALE_MASK << SYS_ANA_REG5_CK_TST_ENBALE_POS);
    reg_value |= ((value & SYS_ANA_REG5_CK_TST_ENBALE_MASK) << SYS_ANA_REG5_CK_TST_ENBALE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->trxt_tst_enable:0x45[12],wifi trx test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_trxt_tst_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_TRXT_TST_ENABLE_POS) & SYS_ANA_REG5_TRXT_TST_ENABLE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_trxt_tst_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_TRXT_TST_ENABLE_MASK << SYS_ANA_REG5_TRXT_TST_ENABLE_POS);
    reg_value |= ((value & SYS_ANA_REG5_TRXT_TST_ENABLE_MASK) << SYS_ANA_REG5_TRXT_TST_ENABLE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->encb:0x45[13],global central bias enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_encb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_ENCB_POS) & SYS_ANA_REG5_ENCB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_encb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_ENCB_MASK << SYS_ANA_REG5_ENCB_POS);
    reg_value |= ((value & SYS_ANA_REG5_ENCB_MASK) << SYS_ANA_REG5_ENCB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->vctrl_dpllldo:0x45[15:14],dpll ldo output selection,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vctrl_dpllldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_VCTRL_DPLLLDO_POS) & SYS_ANA_REG5_VCTRL_DPLLLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_vctrl_dpllldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_VCTRL_DPLLLDO_MASK << SYS_ANA_REG5_VCTRL_DPLLLDO_POS);
    reg_value |= ((value & SYS_ANA_REG5_VCTRL_DPLLLDO_MASK) << SYS_ANA_REG5_VCTRL_DPLLLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->vctrl_sysldo:0x45[17:16],sys ldo output selection,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vctrl_sysldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_VCTRL_SYSLDO_POS) & SYS_ANA_REG5_VCTRL_SYSLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_vctrl_sysldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_VCTRL_SYSLDO_MASK << SYS_ANA_REG5_VCTRL_SYSLDO_POS);
    reg_value |= ((value & SYS_ANA_REG5_VCTRL_SYSLDO_MASK) << SYS_ANA_REG5_VCTRL_SYSLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->temptst_en:0x45[18],tempdet test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_temptst_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_TEMPTST_EN_POS) & SYS_ANA_REG5_TEMPTST_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_temptst_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_TEMPTST_EN_MASK << SYS_ANA_REG5_TEMPTST_EN_POS);
    reg_value |= ((value & SYS_ANA_REG5_TEMPTST_EN_MASK) << SYS_ANA_REG5_TEMPTST_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->gadc_inbuf_ictrl:0x45[20:19],gadc input buf Ibias selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_gadc_inbuf_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_GADC_INBUF_ICTRL_POS) & SYS_ANA_REG5_GADC_INBUF_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_gadc_inbuf_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_GADC_INBUF_ICTRL_MASK << SYS_ANA_REG5_GADC_INBUF_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG5_GADC_INBUF_ICTRL_MASK) << SYS_ANA_REG5_GADC_INBUF_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->xtalh_ictrl:0x45[22],xtalh current control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_xtalh_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_XTALH_ICTRL_POS) & SYS_ANA_REG5_XTALH_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_xtalh_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_XTALH_ICTRL_MASK << SYS_ANA_REG5_XTALH_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG5_XTALH_ICTRL_MASK) << SYS_ANA_REG5_XTALH_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bgcalm:0x45[28:23],bandgap calibration manual setting,20,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcalm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BGCALM_POS) & SYS_ANA_REG5_BGCALM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bgcalm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BGCALM_MASK << SYS_ANA_REG5_BGCALM_POS);
    reg_value |= ((value & SYS_ANA_REG5_BGCALM_MASK) << SYS_ANA_REG5_BGCALM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bgcal_trig:0x45[29],bandgap calibrarion trig,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcal_trig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BGCAL_TRIG_POS) & SYS_ANA_REG5_BGCAL_TRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bgcal_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BGCAL_TRIG_MASK << SYS_ANA_REG5_BGCAL_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG5_BGCAL_TRIG_MASK) << SYS_ANA_REG5_BGCAL_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bgcal_manu:0x45[30],bandgap calibration manual mode enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcal_manu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BGCAL_MANU_POS) & SYS_ANA_REG5_BGCAL_MANU_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bgcal_manu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BGCAL_MANU_MASK << SYS_ANA_REG5_BGCAL_MANU_POS);
    reg_value |= ((value & SYS_ANA_REG5_BGCAL_MANU_MASK) << SYS_ANA_REG5_BGCAL_MANU_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bgcal_en:0x45[31],bandgap calibration enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bgcal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BGCAL_EN_POS) & SYS_ANA_REG5_BGCAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bgcal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BGCAL_EN_MASK << SYS_ANA_REG5_BGCAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG5_BGCAL_EN_MASK) << SYS_ANA_REG5_BGCAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x46 //REG ADDR :0x44010118 */
static inline uint32_t sys_ll_get_ana_reg6_value(void)
{
    return REG_READ(SYS_ANA_REG6_ADDR);
}

static inline void sys_ll_set_ana_reg6_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,value);
}

/* REG_0x46:ana_reg6->itune_xtall:0x46[3:0],xtall core current control,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_itune_xtall(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_ITUNE_XTALL_POS) & SYS_ANA_REG6_ITUNE_XTALL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_itune_xtall(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_ITUNE_XTALL_MASK << SYS_ANA_REG6_ITUNE_XTALL_POS);
    reg_value |= ((value & SYS_ANA_REG6_ITUNE_XTALL_MASK) << SYS_ANA_REG6_ITUNE_XTALL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->xtall_ten:0x46[4],xtall test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_xtall_ten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_XTALL_TEN_POS) & SYS_ANA_REG6_XTALL_TEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_xtall_ten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_XTALL_TEN_MASK << SYS_ANA_REG6_XTALL_TEN_POS);
    reg_value |= ((value & SYS_ANA_REG6_XTALL_TEN_MASK) << SYS_ANA_REG6_XTALL_TEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->psldo_vsel:0x46[5],ps ldo output voltage selection,0:VIO /1:1.8V,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_psldo_vsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_PSLDO_VSEL_POS) & SYS_ANA_REG6_PSLDO_VSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_psldo_vsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_PSLDO_VSEL_MASK << SYS_ANA_REG6_PSLDO_VSEL_POS);
    reg_value |= ((value & SYS_ANA_REG6_PSLDO_VSEL_MASK) << SYS_ANA_REG6_PSLDO_VSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_usb:0x46[6],usb phy enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_usb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_USB_POS) & SYS_ANA_REG6_EN_USB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_usb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_USB_MASK << SYS_ANA_REG6_EN_USB_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_USB_MASK) << SYS_ANA_REG6_EN_USB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_xtall:0x46[7],xtall oscillator enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_xtall(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_XTALL_POS) & SYS_ANA_REG6_EN_XTALL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_xtall(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_XTALL_MASK << SYS_ANA_REG6_EN_XTALL_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_XTALL_MASK) << SYS_ANA_REG6_EN_XTALL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_dco:0x46[8],dco enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_dco(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_DCO_POS) & SYS_ANA_REG6_EN_DCO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_dco(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_DCO_MASK << SYS_ANA_REG6_EN_DCO_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_DCO_MASK) << SYS_ANA_REG6_EN_DCO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_psram_ldo:0x46[9],psram ldo enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_psram_ldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_PSRAM_LDO_POS) & SYS_ANA_REG6_EN_PSRAM_LDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_psram_ldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_PSRAM_LDO_MASK << SYS_ANA_REG6_EN_PSRAM_LDO_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_PSRAM_LDO_MASK) << SYS_ANA_REG6_EN_PSRAM_LDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_tempdet:0x46[10],tempreture det enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_tempdet(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_TEMPDET_POS) & SYS_ANA_REG6_EN_TEMPDET_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_tempdet(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_TEMPDET_MASK << SYS_ANA_REG6_EN_TEMPDET_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_TEMPDET_MASK) << SYS_ANA_REG6_EN_TEMPDET_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_audpll:0x46[11],audio pll enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_audpll(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_AUDPLL_POS) & SYS_ANA_REG6_EN_AUDPLL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_audpll(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_AUDPLL_MASK << SYS_ANA_REG6_EN_AUDPLL_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_AUDPLL_MASK) << SYS_ANA_REG6_EN_AUDPLL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_dpll:0x46[12],dpll enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_dpll(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_DPLL_POS) & SYS_ANA_REG6_EN_DPLL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_dpll(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_DPLL_MASK << SYS_ANA_REG6_EN_DPLL_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_DPLL_MASK) << SYS_ANA_REG6_EN_DPLL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_sysldo:0x46[13],sysldo enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_sysldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_SYSLDO_POS) & SYS_ANA_REG6_EN_SYSLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_sysldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_SYSLDO_MASK << SYS_ANA_REG6_EN_SYSLDO_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_SYSLDO_MASK) << SYS_ANA_REG6_EN_SYSLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_aud:0x46[14],en_aud pwd,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_aud(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_AUD_POS) & SYS_ANA_REG6_EN_AUD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_aud(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_AUD_MASK << SYS_ANA_REG6_EN_AUD_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_AUD_MASK) << SYS_ANA_REG6_EN_AUD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->pwd_gadc_buf:0x46[15],gadc input buffer pwd,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_pwd_gadc_buf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_PWD_GADC_BUF_POS) & SYS_ANA_REG6_PWD_GADC_BUF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_pwd_gadc_buf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_PWD_GADC_BUF_MASK << SYS_ANA_REG6_PWD_GADC_BUF_POS);
    reg_value |= ((value & SYS_ANA_REG6_PWD_GADC_BUF_MASK) << SYS_ANA_REG6_PWD_GADC_BUF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->vaon_sel:0x46[17],0:vddaon drop enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_vaon_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_VAON_SEL_POS) & SYS_ANA_REG6_VAON_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_vaon_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_VAON_SEL_MASK << SYS_ANA_REG6_VAON_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG6_VAON_SEL_MASK) << SYS_ANA_REG6_VAON_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->xtal_hpsrr_en:0x46[18],xtal high psrr buffer enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_xtal_hpsrr_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_XTAL_HPSRR_EN_POS) & SYS_ANA_REG6_XTAL_HPSRR_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_xtal_hpsrr_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_XTAL_HPSRR_EN_MASK << SYS_ANA_REG6_XTAL_HPSRR_EN_POS);
    reg_value |= ((value & SYS_ANA_REG6_XTAL_HPSRR_EN_MASK) << SYS_ANA_REG6_XTAL_HPSRR_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_xtal2rf:0x46[19],xtal clock to rfpll gate enable ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_xtal2rf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_XTAL2RF_POS) & SYS_ANA_REG6_EN_XTAL2RF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_xtal2rf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_XTAL2RF_MASK << SYS_ANA_REG6_EN_XTAL2RF_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_XTAL2RF_MASK) << SYS_ANA_REG6_EN_XTAL2RF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->en_sleep:0x46[20],xtal sleep enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_en_sleep(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_EN_SLEEP_POS) & SYS_ANA_REG6_EN_SLEEP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_en_sleep(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_EN_SLEEP_MASK << SYS_ANA_REG6_EN_SLEEP_POS);
    reg_value |= ((value & SYS_ANA_REG6_EN_SLEEP_MASK) << SYS_ANA_REG6_EN_SLEEP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->clkbuf_hd:0x46[21],xtal lpsrr clock buffer high power mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_clkbuf_hd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_CLKBUF_HD_POS) & SYS_ANA_REG6_CLKBUF_HD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_clkbuf_hd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_CLKBUF_HD_MASK << SYS_ANA_REG6_CLKBUF_HD_POS);
    reg_value |= ((value & SYS_ANA_REG6_CLKBUF_HD_MASK) << SYS_ANA_REG6_CLKBUF_HD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->clkbuf_dsel_manu:0x46[22],xtal lpsrr clock buffer power mode selection 0: auto /1:manu ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_clkbuf_dsel_manu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_CLKBUF_DSEL_MANU_POS) & SYS_ANA_REG6_CLKBUF_DSEL_MANU_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_clkbuf_dsel_manu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_CLKBUF_DSEL_MANU_MASK << SYS_ANA_REG6_CLKBUF_DSEL_MANU_POS);
    reg_value |= ((value & SYS_ANA_REG6_CLKBUF_DSEL_MANU_MASK) << SYS_ANA_REG6_CLKBUF_DSEL_MANU_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->xtal_lpmode_ctrl:0x46[23],xtal core low power mode enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_xtal_lpmode_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS) & SYS_ANA_REG6_XTAL_LPMODE_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_xtal_lpmode_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_XTAL_LPMODE_CTRL_MASK << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG6_XTAL_LPMODE_CTRL_MASK) << SYS_ANA_REG6_XTAL_LPMODE_CTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->rxtal_lp:0x46[27:24],xtal bias current setting at low power mode ,F,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_rxtal_lp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_RXTAL_LP_POS) & SYS_ANA_REG6_RXTAL_LP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_rxtal_lp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_RXTAL_LP_MASK << SYS_ANA_REG6_RXTAL_LP_POS);
    reg_value |= ((value & SYS_ANA_REG6_RXTAL_LP_MASK) << SYS_ANA_REG6_RXTAL_LP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->rxtal_hp:0x46[31:28],xtal26m bias current setting at high power mode ,F,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_rxtal_hp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_RXTAL_HP_POS) & SYS_ANA_REG6_RXTAL_HP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_rxtal_hp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_RXTAL_HP_MASK << SYS_ANA_REG6_RXTAL_HP_POS);
    reg_value |= ((value & SYS_ANA_REG6_RXTAL_HP_MASK) << SYS_ANA_REG6_RXTAL_HP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x47 //REG ADDR :0x4401011c */
static inline uint32_t sys_ll_get_ana_reg7_value(void)
{
    return REG_READ(SYS_ANA_REG7_ADDR);
}

static inline void sys_ll_set_ana_reg7_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,value);
}

/* REG_0x47:ana_reg7->rng_tstck_sel:0x47[0],trng setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_rng_tstck_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_RNG_TSTCK_SEL_POS) & SYS_ANA_REG7_RNG_TSTCK_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_rng_tstck_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_RNG_TSTCK_SEL_MASK << SYS_ANA_REG7_RNG_TSTCK_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG7_RNG_TSTCK_SEL_MASK) << SYS_ANA_REG7_RNG_TSTCK_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->rng_tsten:0x47[1],trng setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_rng_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_RNG_TSTEN_POS) & SYS_ANA_REG7_RNG_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_rng_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_RNG_TSTEN_MASK << SYS_ANA_REG7_RNG_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG7_RNG_TSTEN_MASK) << SYS_ANA_REG7_RNG_TSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->itune_ref:0x47[4:2],trng setting,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_itune_ref(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_ITUNE_REF_POS) & SYS_ANA_REG7_ITUNE_REF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_itune_ref(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_ITUNE_REF_MASK << SYS_ANA_REG7_ITUNE_REF_POS);
    reg_value |= ((value & SYS_ANA_REG7_ITUNE_REF_MASK) << SYS_ANA_REG7_ITUNE_REF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->itune_opa:0x47[7:5],trng setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_itune_opa(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_ITUNE_OPA_POS) & SYS_ANA_REG7_ITUNE_OPA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_itune_opa(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_ITUNE_OPA_MASK << SYS_ANA_REG7_ITUNE_OPA_POS);
    reg_value |= ((value & SYS_ANA_REG7_ITUNE_OPA_MASK) << SYS_ANA_REG7_ITUNE_OPA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->itune_cmp:0x47[10:8],trng setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_itune_cmp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_ITUNE_CMP_POS) & SYS_ANA_REG7_ITUNE_CMP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_itune_cmp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_ITUNE_CMP_MASK << SYS_ANA_REG7_ITUNE_CMP_POS);
    reg_value |= ((value & SYS_ANA_REG7_ITUNE_CMP_MASK) << SYS_ANA_REG7_ITUNE_CMP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->Rnooise_sel:0x47[11],trng setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_rnooise_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_RNOOISE_SEL_POS) & SYS_ANA_REG7_RNOOISE_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_rnooise_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_RNOOISE_SEL_MASK << SYS_ANA_REG7_RNOOISE_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG7_RNOOISE_SEL_MASK) << SYS_ANA_REG7_RNOOISE_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->Fslow_sel:0x47[14:12],trng setting,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_fslow_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_FSLOW_SEL_POS) & SYS_ANA_REG7_FSLOW_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_fslow_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_FSLOW_SEL_MASK << SYS_ANA_REG7_FSLOW_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG7_FSLOW_SEL_MASK) << SYS_ANA_REG7_FSLOW_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->Ffast_sel:0x47[18:15],trng setting,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_ffast_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_FFAST_SEL_POS) & SYS_ANA_REG7_FFAST_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_ffast_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_FFAST_SEL_MASK << SYS_ANA_REG7_FFAST_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG7_FFAST_SEL_MASK) << SYS_ANA_REG7_FFAST_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->gadc_cal_sel:0x47[20:19],gadc calibration mode selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_cal_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_GADC_CAL_SEL_POS) & SYS_ANA_REG7_GADC_CAL_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_gadc_cal_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_GADC_CAL_SEL_MASK << SYS_ANA_REG7_GADC_CAL_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG7_GADC_CAL_SEL_MASK) << SYS_ANA_REG7_GADC_CAL_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->gadc_ten:0x47[21],gadc test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_ten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_GADC_TEN_POS) & SYS_ANA_REG7_GADC_TEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_gadc_ten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_GADC_TEN_MASK << SYS_ANA_REG7_GADC_TEN_POS);
    reg_value |= ((value & SYS_ANA_REG7_GADC_TEN_MASK) << SYS_ANA_REG7_GADC_TEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->gadc_cmp_ictrl:0x47[25:22],gadc comparaor current select ,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_cmp_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_GADC_CMP_ICTRL_POS) & SYS_ANA_REG7_GADC_CMP_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_gadc_cmp_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_GADC_CMP_ICTRL_MASK << SYS_ANA_REG7_GADC_CMP_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG7_GADC_CMP_ICTRL_MASK) << SYS_ANA_REG7_GADC_CMP_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->gadc_buf_ictrl:0x47[29:26],gadc buffer current select ,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_gadc_buf_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_GADC_BUF_ICTRL_POS) & SYS_ANA_REG7_GADC_BUF_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_gadc_buf_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_GADC_BUF_ICTRL_MASK << SYS_ANA_REG7_GADC_BUF_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG7_GADC_BUF_ICTRL_MASK) << SYS_ANA_REG7_GADC_BUF_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->vref_sel:0x47[30],gadc input reference select, 0：bandgap signal 1:GPIO voltage divided,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_vref_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_VREF_SEL_POS) & SYS_ANA_REG7_VREF_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_vref_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_VREF_SEL_MASK << SYS_ANA_REG7_VREF_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG7_VREF_SEL_MASK) << SYS_ANA_REG7_VREF_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x47:ana_reg7->scal_en:0x47[31],gadc reference scale enable, 0:normal mode,1: scale mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_scal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG7_SCAL_EN_POS) & SYS_ANA_REG7_SCAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg7_scal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG7_ADDR);
    reg_value &= ~(SYS_ANA_REG7_SCAL_EN_MASK << SYS_ANA_REG7_SCAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG7_SCAL_EN_MASK) << SYS_ANA_REG7_SCAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,reg_value);
}

/* REG_0x48 //REG ADDR :0x44010120 */
static inline uint32_t sys_ll_get_ana_reg8_value(void)
{
    return REG_READ(SYS_ANA_REG8_ADDR);
}

static inline void sys_ll_set_ana_reg8_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,value);
}

/* REG_0x48:ana_reg8->cap_calspi:0x48[8:0],manul mode ,input cap calibretion value,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_cap_calspi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_CAP_CALSPI_POS) & SYS_ANA_REG8_CAP_CALSPI_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_cap_calspi(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_CAP_CALSPI_MASK << SYS_ANA_REG8_CAP_CALSPI_POS);
    reg_value |= ((value & SYS_ANA_REG8_CAP_CALSPI_MASK) << SYS_ANA_REG8_CAP_CALSPI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->gain_s:0x48[10:9],Sensitivity level selection,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_gain_s(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_GAIN_S_POS) & SYS_ANA_REG8_GAIN_S_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_gain_s(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_GAIN_S_MASK << SYS_ANA_REG8_GAIN_S_POS);
    reg_value |= ((value & SYS_ANA_REG8_GAIN_S_MASK) << SYS_ANA_REG8_GAIN_S_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->pwd_td:0x48[11],power down touch module,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_pwd_td(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_PWD_TD_POS) & SYS_ANA_REG8_PWD_TD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_pwd_td(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_PWD_TD_MASK << SYS_ANA_REG8_PWD_TD_POS);
    reg_value |= ((value & SYS_ANA_REG8_PWD_TD_MASK) << SYS_ANA_REG8_PWD_TD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->en_fsr:0x48[12],low power mode ,enable fast response,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_fsr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_EN_FSR_POS) & SYS_ANA_REG8_EN_FSR_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_en_fsr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_EN_FSR_MASK << SYS_ANA_REG8_EN_FSR_POS);
    reg_value |= ((value & SYS_ANA_REG8_EN_FSR_MASK) << SYS_ANA_REG8_EN_FSR_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->en_scm:0x48[13],scan mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_scm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_EN_SCM_POS) & SYS_ANA_REG8_EN_SCM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_en_scm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_EN_SCM_MASK << SYS_ANA_REG8_EN_SCM_POS);
    reg_value |= ((value & SYS_ANA_REG8_EN_SCM_MASK) << SYS_ANA_REG8_EN_SCM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->en_adcmode:0x48[14],adc mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_adcmode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_EN_ADCMODE_POS) & SYS_ANA_REG8_EN_ADCMODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_en_adcmode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_EN_ADCMODE_MASK << SYS_ANA_REG8_EN_ADCMODE_POS);
    reg_value |= ((value & SYS_ANA_REG8_EN_ADCMODE_MASK) << SYS_ANA_REG8_EN_ADCMODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->en_lpmode:0x48[15],low power mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_en_lpmode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_EN_LPMODE_POS) & SYS_ANA_REG8_EN_LPMODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_en_lpmode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_EN_LPMODE_MASK << SYS_ANA_REG8_EN_LPMODE_POS);
    reg_value |= ((value & SYS_ANA_REG8_EN_LPMODE_MASK) << SYS_ANA_REG8_EN_LPMODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->chs_scan:0x48[31:16],scan mode chan selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg8_chs_scan(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_CHS_SCAN_POS) & SYS_ANA_REG8_CHS_SCAN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_chs_scan(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_CHS_SCAN_MASK << SYS_ANA_REG8_CHS_SCAN_POS);
    reg_value |= ((value & SYS_ANA_REG8_CHS_SCAN_MASK) << SYS_ANA_REG8_CHS_SCAN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x49 //REG ADDR :0x44010124 */
static inline uint32_t sys_ll_get_ana_reg9_value(void)
{
    return REG_READ(SYS_ANA_REG9_ADDR);
}

static inline void sys_ll_set_ana_reg9_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,value);
}

/* REG_0x49:ana_reg9->en_otp_spi:0x49[0],otp ldo spi enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_en_otp_spi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_EN_OTP_SPI_POS) & SYS_ANA_REG9_EN_OTP_SPI_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_en_otp_spi(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_EN_OTP_SPI_MASK << SYS_ANA_REG9_EN_OTP_SPI_POS);
    reg_value |= ((value & SYS_ANA_REG9_EN_OTP_SPI_MASK) << SYS_ANA_REG9_EN_OTP_SPI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

static inline void sys_ll_set_ana_reg9_vtempsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VTEMPSEL_MASK << SYS_ANA_REG9_VTEMPSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_VTEMPSEL_MASK) << SYS_ANA_REG9_VTEMPSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->entemp2:0x49[1],dummy,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_entemp2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_ENTEMP2_POS) & SYS_ANA_REG9_ENTEMP2_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_entemp2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_ENTEMP2_MASK << SYS_ANA_REG9_ENTEMP2_POS);
    reg_value |= ((value & SYS_ANA_REG9_ENTEMP2_MASK) << SYS_ANA_REG9_ENTEMP2_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vtempsel:0x49[3:2],00:nc  01:vtemp  10:vbe   11:vbg1p3,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_vtempsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VTEMPSEL_POS) & SYS_ANA_REG9_VTEMPSEL_MASK);
    return reg_value;
}

/* REG_0x49:ana_reg9->vtsel:0x49[4],dummy,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_vtsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VTSEL_POS) & SYS_ANA_REG9_VTSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vtsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VTSEL_MASK << SYS_ANA_REG9_VTSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_VTSEL_MASK) << SYS_ANA_REG9_VTSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->en_bias_5u:0x49[5],Ibias 5u enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_en_bias_5u(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_EN_BIAS_5U_POS) & SYS_ANA_REG9_EN_BIAS_5U_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_en_bias_5u(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_EN_BIAS_5U_MASK << SYS_ANA_REG9_EN_BIAS_5U_POS);
    reg_value |= ((value & SYS_ANA_REG9_EN_BIAS_5U_MASK) << SYS_ANA_REG9_EN_BIAS_5U_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->dummy2:0x49[6],5uA channel on(for PLL & DCO),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_dummy2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_DUMMY2_POS) & SYS_ANA_REG9_DUMMY2_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_dummy2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_DUMMY2_MASK << SYS_ANA_REG9_DUMMY2_POS);
    reg_value |= ((value & SYS_ANA_REG9_DUMMY2_MASK) << SYS_ANA_REG9_DUMMY2_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->touch_serial_cap:0x49[7],5uA channel on(for PLL & DCO),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_touch_serial_cap(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_TOUCH_SERIAL_CAP_POS) & SYS_ANA_REG9_TOUCH_SERIAL_CAP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_touch_serial_cap(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_TOUCH_SERIAL_CAP_MASK << SYS_ANA_REG9_TOUCH_SERIAL_CAP_POS);
    reg_value |= ((value & SYS_ANA_REG9_TOUCH_SERIAL_CAP_MASK) << SYS_ANA_REG9_TOUCH_SERIAL_CAP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->buckfb_czenb:0x49[8],buck EA feedback cz selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_buckfb_czenb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_BUCKFB_CZENB_POS) & SYS_ANA_REG9_BUCKFB_CZENB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_buckfb_czenb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_BUCKFB_CZENB_MASK << SYS_ANA_REG9_BUCKFB_CZENB_POS);
    reg_value |= ((value & SYS_ANA_REG9_BUCKFB_CZENB_MASK) << SYS_ANA_REG9_BUCKFB_CZENB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->buckea_cur_ctrl:0x49[10:9],buck EA ibias selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_buckea_cur_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_BUCKEA_CUR_CTRL_POS) & SYS_ANA_REG9_BUCKEA_CUR_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_buckea_cur_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_BUCKEA_CUR_CTRL_MASK << SYS_ANA_REG9_BUCKEA_CUR_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG9_BUCKEA_CUR_CTRL_MASK) << SYS_ANA_REG9_BUCKEA_CUR_CTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->cbtst_en:0x49[11],CB test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_cbtst_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_CBTST_EN_POS) & SYS_ANA_REG9_CBTST_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_cbtst_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_CBTST_EN_MASK << SYS_ANA_REG9_CBTST_EN_POS);
    reg_value |= ((value & SYS_ANA_REG9_CBTST_EN_MASK) << SYS_ANA_REG9_CBTST_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->psldo_vsel:0x49[12],psldo voltage selsection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_psldo_vsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_PSLDO_VSEL_POS) & SYS_ANA_REG9_PSLDO_VSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_psldo_vsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_PSLDO_VSEL_MASK << SYS_ANA_REG9_PSLDO_VSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_PSLDO_VSEL_MASK) << SYS_ANA_REG9_PSLDO_VSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->ovr_l:0x49[13],ovr low enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_ovr_l(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_OVR_L_POS) & SYS_ANA_REG9_OVR_L_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_ovr_l(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_OVR_L_MASK << SYS_ANA_REG9_OVR_L_POS);
    reg_value |= ((value & SYS_ANA_REG9_OVR_L_MASK) << SYS_ANA_REG9_OVR_L_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->usbpen:0x49[17:14],usb dp driver capability control,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usbpen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_USBPEN_POS) & SYS_ANA_REG9_USBPEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_usbpen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_USBPEN_MASK << SYS_ANA_REG9_USBPEN_POS);
    reg_value |= ((value & SYS_ANA_REG9_USBPEN_MASK) << SYS_ANA_REG9_USBPEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->usbnen:0x49[21:18],usb dn driver capability control,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usbnen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_USBNEN_POS) & SYS_ANA_REG9_USBNEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_usbnen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_USBNEN_MASK << SYS_ANA_REG9_USBNEN_POS);
    reg_value |= ((value & SYS_ANA_REG9_USBNEN_MASK) << SYS_ANA_REG9_USBNEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->usb_speed:0x49[22],usb speed selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usb_speed(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_USB_SPEED_POS) & SYS_ANA_REG9_USB_SPEED_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_usb_speed(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_USB_SPEED_MASK << SYS_ANA_REG9_USB_SPEED_POS);
    reg_value |= ((value & SYS_ANA_REG9_USB_SPEED_MASK) << SYS_ANA_REG9_USB_SPEED_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->usb_deepsleep:0x49[23],usb deepsleep mode enable by spi,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_usb_deepsleep(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_USB_DEEPSLEEP_POS) & SYS_ANA_REG9_USB_DEEPSLEEP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_usb_deepsleep(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_USB_DEEPSLEEP_MASK << SYS_ANA_REG9_USB_DEEPSLEEP_POS);
    reg_value |= ((value & SYS_ANA_REG9_USB_DEEPSLEEP_MASK) << SYS_ANA_REG9_USB_DEEPSLEEP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->man_mode:0x49[24],manul mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_man_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_MAN_MODE_POS) & SYS_ANA_REG9_MAN_MODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_man_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_MAN_MODE_MASK << SYS_ANA_REG9_MAN_MODE_POS);
    reg_value |= ((value & SYS_ANA_REG9_MAN_MODE_MASK) << SYS_ANA_REG9_MAN_MODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->crg:0x49[26:25],detect range selection :8pF/12pF/19pF/27pF,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_crg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_CRG_POS) & SYS_ANA_REG9_CRG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_crg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_CRG_MASK << SYS_ANA_REG9_CRG_POS);
    reg_value |= ((value & SYS_ANA_REG9_CRG_MASK) << SYS_ANA_REG9_CRG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vrefs:0x49[29:27],detect threshold selection ,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_vrefs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VREFS_POS) & SYS_ANA_REG9_VREFS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vrefs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VREFS_MASK << SYS_ANA_REG9_VREFS_POS);
    reg_value |= ((value & SYS_ANA_REG9_VREFS_MASK) << SYS_ANA_REG9_VREFS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->en_cal:0x49[31],calibretion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg9_en_cal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_EN_CAL_POS) & SYS_ANA_REG9_EN_CAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_en_cal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_EN_CAL_MASK << SYS_ANA_REG9_EN_CAL_POS);
    reg_value |= ((value & SYS_ANA_REG9_EN_CAL_MASK) << SYS_ANA_REG9_EN_CAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x4A //REG ADDR :0x44010128 */
static inline uint32_t sys_ll_get_ana_reg10_value(void)
{
    return REG_READ(SYS_ANA_REG10_ADDR);
}

static inline void sys_ll_set_ana_reg10_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,value);
}

/* REG_0x4a:ana_reg10->sdm_val:0x4a[29:0],audio pll sdm value,0F1FAA45,R/W*/
static inline uint32_t sys_ll_get_ana_reg10_sdm_val(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_SDM_VAL_POS) & SYS_ANA_REG10_SDM_VAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_sdm_val(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_SDM_VAL_MASK << SYS_ANA_REG10_SDM_VAL_POS);
    reg_value |= ((value & SYS_ANA_REG10_SDM_VAL_MASK) << SYS_ANA_REG10_SDM_VAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->vco_hfreq_enb:0x4a[30],audio pll vco high frequency enb,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg10_vco_hfreq_enb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_VCO_HFREQ_ENB_POS) & SYS_ANA_REG10_VCO_HFREQ_ENB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_vco_hfreq_enb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_VCO_HFREQ_ENB_MASK << SYS_ANA_REG10_VCO_HFREQ_ENB_POS);
    reg_value |= ((value & SYS_ANA_REG10_VCO_HFREQ_ENB_MASK) << SYS_ANA_REG10_VCO_HFREQ_ENB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->cal_refen:0x4a[31],cal_ref enable of audio pll,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg10_cal_refen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_CAL_REFEN_POS) & SYS_ANA_REG10_CAL_REFEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_cal_refen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_CAL_REFEN_MASK << SYS_ANA_REG10_CAL_REFEN_POS);
    reg_value |= ((value & SYS_ANA_REG10_CAL_REFEN_MASK) << SYS_ANA_REG10_CAL_REFEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4B //REG ADDR :0x4401012c */
static inline uint32_t sys_ll_get_ana_reg11_value(void)
{
    return REG_READ(SYS_ANA_REG11_ADDR);
}

static inline void sys_ll_set_ana_reg11_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,value);
}

/* REG_0x4b:ana_reg11->int_mod:0x4b[0],DPLL integer mode enable; 0: fractional mode; 1: integer mode,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_int_mod(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_INT_MOD_POS) & SYS_ANA_REG11_INT_MOD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_int_mod(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_INT_MOD_MASK << SYS_ANA_REG11_INT_MOD_POS);
    reg_value |= ((value & SYS_ANA_REG11_INT_MOD_MASK) << SYS_ANA_REG11_INT_MOD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->Nsyn:0x4b[1],DPLL Ncoutner reset ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_nsyn(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_NSYN_POS) & SYS_ANA_REG11_NSYN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_nsyn(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_NSYN_MASK << SYS_ANA_REG11_NSYN_POS);
    reg_value |= ((value & SYS_ANA_REG11_NSYN_MASK) << SYS_ANA_REG11_NSYN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->open_enb:0x4b[2], ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_open_enb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_OPEN_ENB_POS) & SYS_ANA_REG11_OPEN_ENB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_open_enb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_OPEN_ENB_MASK << SYS_ANA_REG11_OPEN_ENB_POS);
    reg_value |= ((value & SYS_ANA_REG11_OPEN_ENB_MASK) << SYS_ANA_REG11_OPEN_ENB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->reset:0x4b[3],DPLL reset,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_RESET_POS) & SYS_ANA_REG11_RESET_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_reset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_RESET_MASK << SYS_ANA_REG11_RESET_POS);
    reg_value |= ((value & SYS_ANA_REG11_RESET_MASK) << SYS_ANA_REG11_RESET_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->Ioffset:0x4b[6:4],DPLL  charge pump offset current control,5,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ioffset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_IOFFSET_POS) & SYS_ANA_REG11_IOFFSET_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ioffset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_IOFFSET_MASK << SYS_ANA_REG11_IOFFSET_POS);
    reg_value |= ((value & SYS_ANA_REG11_IOFFSET_MASK) << SYS_ANA_REG11_IOFFSET_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->LPFRz:0x4b[10:7],DPLL Rz control of LPF,6,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_lpfrz(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_LPFRZ_POS) & SYS_ANA_REG11_LPFRZ_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_lpfrz(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_LPFRZ_MASK << SYS_ANA_REG11_LPFRZ_POS);
    reg_value |= ((value & SYS_ANA_REG11_LPFRZ_MASK) << SYS_ANA_REG11_LPFRZ_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->vsel:0x4b[13:11],DPLL vtrl selection during VCO band calibration,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_vsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_VSEL_POS) & SYS_ANA_REG11_VSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_vsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_VSEL_MASK << SYS_ANA_REG11_VSEL_POS);
    reg_value |= ((value & SYS_ANA_REG11_VSEL_MASK) << SYS_ANA_REG11_VSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->vsel_cal:0x4b[14], selection during VCO band calibration,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_vsel_cal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_VSEL_CAL_POS) & SYS_ANA_REG11_VSEL_CAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_vsel_cal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_VSEL_CAL_MASK << SYS_ANA_REG11_VSEL_CAL_POS);
    reg_value |= ((value & SYS_ANA_REG11_VSEL_CAL_MASK) << SYS_ANA_REG11_VSEL_CAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->pwd_lockdet:0x4b[15], ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_pwd_lockdet(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_PWD_LOCKDET_POS) & SYS_ANA_REG11_PWD_LOCKDET_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_pwd_lockdet(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_PWD_LOCKDET_MASK << SYS_ANA_REG11_PWD_LOCKDET_POS);
    reg_value |= ((value & SYS_ANA_REG11_PWD_LOCKDET_MASK) << SYS_ANA_REG11_PWD_LOCKDET_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->lockdet_bypass:0x4b[16], ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_lockdet_bypass(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_LOCKDET_BYPASS_POS) & SYS_ANA_REG11_LOCKDET_BYPASS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_lockdet_bypass(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_LOCKDET_BYPASS_MASK << SYS_ANA_REG11_LOCKDET_BYPASS_POS);
    reg_value |= ((value & SYS_ANA_REG11_LOCKDET_BYPASS_MASK) << SYS_ANA_REG11_LOCKDET_BYPASS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->ckref_loop_sel:0x4b[17],polarity selection of referenc clock  to SDM,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ckref_loop_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_CKREF_LOOP_SEL_POS) & SYS_ANA_REG11_CKREF_LOOP_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ckref_loop_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_CKREF_LOOP_SEL_MASK << SYS_ANA_REG11_CKREF_LOOP_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG11_CKREF_LOOP_SEL_MASK) << SYS_ANA_REG11_CKREF_LOOP_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->spi_trigger:0x4b[18],DPLL band calibration spi trigger,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_spi_trigger(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_SPI_TRIGGER_POS) & SYS_ANA_REG11_SPI_TRIGGER_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_spi_trigger(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_SPI_TRIGGER_MASK << SYS_ANA_REG11_SPI_TRIGGER_POS);
    reg_value |= ((value & SYS_ANA_REG11_SPI_TRIGGER_MASK) << SYS_ANA_REG11_SPI_TRIGGER_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->manual:0x4b[19],DPLL VCO band manual enable; 0: auto mode; 1: manual mode,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_manual(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_MANUAL_POS) & SYS_ANA_REG11_MANUAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_manual(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_MANUAL_MASK << SYS_ANA_REG11_MANUAL_POS);
    reg_value |= ((value & SYS_ANA_REG11_MANUAL_MASK) << SYS_ANA_REG11_MANUAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->test_en:0x4b[20],test enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_test_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_TEST_EN_POS) & SYS_ANA_REG11_TEST_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_test_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_TEST_EN_MASK << SYS_ANA_REG11_TEST_EN_POS);
    reg_value |= ((value & SYS_ANA_REG11_TEST_EN_MASK) << SYS_ANA_REG11_TEST_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->Icp:0x4b[23:22],DPLL charge pump current control; ,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_icp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ICP_POS) & SYS_ANA_REG11_ICP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_icp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ICP_MASK << SYS_ANA_REG11_ICP_POS);
    reg_value |= ((value & SYS_ANA_REG11_ICP_MASK) << SYS_ANA_REG11_ICP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->ck26Men:0x4b[24],xtal26M clock for audio enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ck26men(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_CK26MEN_POS) & SYS_ANA_REG11_CK26MEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ck26men(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_CK26MEN_MASK << SYS_ANA_REG11_CK26MEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_CK26MEN_MASK) << SYS_ANA_REG11_CK26MEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->ckaudio_outen:0x4b[25],DPLL clock output to PAD enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ckaudio_outen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_CKAUDIO_OUTEN_POS) & SYS_ANA_REG11_CKAUDIO_OUTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ckaudio_outen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_CKAUDIO_OUTEN_MASK << SYS_ANA_REG11_CKAUDIO_OUTEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_CKAUDIO_OUTEN_MASK) << SYS_ANA_REG11_CKAUDIO_OUTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->divctrl:0x4b[28:26],DPLL divider control; 000: div1; 001: div2; 010: div4; 011: div8; 1xx: div16,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_divctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_DIVCTRL_POS) & SYS_ANA_REG11_DIVCTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_divctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_DIVCTRL_MASK << SYS_ANA_REG11_DIVCTRL_POS);
    reg_value |= ((value & SYS_ANA_REG11_DIVCTRL_MASK) << SYS_ANA_REG11_DIVCTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->cksel:0x4b[29],DPLL divider control; 0: div3; 1: div4,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_cksel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_CKSEL_POS) & SYS_ANA_REG11_CKSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_cksel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_CKSEL_MASK << SYS_ANA_REG11_CKSEL_POS);
    reg_value |= ((value & SYS_ANA_REG11_CKSEL_MASK) << SYS_ANA_REG11_CKSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->ck2mcu:0x4b[30],DPLL clock for mcu enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_ck2mcu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_CK2MCU_POS) & SYS_ANA_REG11_CK2MCU_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ck2mcu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_CK2MCU_MASK << SYS_ANA_REG11_CK2MCU_POS);
    reg_value |= ((value & SYS_ANA_REG11_CK2MCU_MASK) << SYS_ANA_REG11_CK2MCU_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->audioen:0x4b[31],DPLL clock for audio enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg11_audioen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_AUDIOEN_POS) & SYS_ANA_REG11_AUDIOEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_audioen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_AUDIOEN_MASK << SYS_ANA_REG11_AUDIOEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_AUDIOEN_MASK) << SYS_ANA_REG11_AUDIOEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4C //REG ADDR :0x44010130 */
static inline uint32_t sys_ll_get_ana_reg12_value(void)
{
    return REG_READ(SYS_ANA_REG12_ADDR);
}

static inline void sys_ll_set_ana_reg12_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,value);
}

/* REG_0x4c:ana_reg12->digmic_ckinv:0x4c[2],digmic clock inversion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_digmic_ckinv(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DIGMIC_CKINV_POS) & SYS_ANA_REG12_DIGMIC_CKINV_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_digmic_ckinv(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DIGMIC_CKINV_MASK << SYS_ANA_REG12_DIGMIC_CKINV_POS);
    reg_value |= ((value & SYS_ANA_REG12_DIGMIC_CKINV_MASK) << SYS_ANA_REG12_DIGMIC_CKINV_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->enmicdig:0x4c[3],digmic enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enmicdig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_ENMICDIG_POS) & SYS_ANA_REG12_ENMICDIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_enmicdig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_ENMICDIG_MASK << SYS_ANA_REG12_ENMICDIG_POS);
    reg_value |= ((value & SYS_ANA_REG12_ENMICDIG_MASK) << SYS_ANA_REG12_ENMICDIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->audck_rlcen:0x4c[4],audio clock re-latch enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_audck_rlcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_AUDCK_RLCEN_POS) & SYS_ANA_REG12_AUDCK_RLCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_audck_rlcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_AUDCK_RLCEN_MASK << SYS_ANA_REG12_AUDCK_RLCEN_POS);
    reg_value |= ((value & SYS_ANA_REG12_AUDCK_RLCEN_MASK) << SYS_ANA_REG12_AUDCK_RLCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->lchckinven:0x4c[5],audio clock re-latch clock inversion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_lchckinven(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_LCHCKINVEN_POS) & SYS_ANA_REG12_LCHCKINVEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_lchckinven(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_LCHCKINVEN_MASK << SYS_ANA_REG12_LCHCKINVEN_POS);
    reg_value |= ((value & SYS_ANA_REG12_LCHCKINVEN_MASK) << SYS_ANA_REG12_LCHCKINVEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->ldo1v_vsel1v:0x4c[8:6],audio 1.0V LDO selection, 000=0.8, 1X1=1.0,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_ldo1v_vsel1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_LDO1V_VSEL1V_POS) & SYS_ANA_REG12_LDO1V_VSEL1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_ldo1v_vsel1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_LDO1V_VSEL1V_MASK << SYS_ANA_REG12_LDO1V_VSEL1V_POS);
    reg_value |= ((value & SYS_ANA_REG12_LDO1V_VSEL1V_MASK) << SYS_ANA_REG12_LDO1V_VSEL1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->ldo1v_adj:0x4c[13:9],audio 1.0V LDO output trimming, 00000=min, 11111=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_ldo1v_adj(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_LDO1V_ADJ_POS) & SYS_ANA_REG12_LDO1V_ADJ_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_ldo1v_adj(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_LDO1V_ADJ_MASK << SYS_ANA_REG12_LDO1V_ADJ_POS);
    reg_value |= ((value & SYS_ANA_REG12_LDO1V_ADJ_MASK) << SYS_ANA_REG12_LDO1V_ADJ_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->audvdd_trm1v:0x4c[15:14],audio 1.5V LDO selection, 00=min, 11=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_audvdd_trm1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_AUDVDD_TRM1V_POS) & SYS_ANA_REG12_AUDVDD_TRM1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_audvdd_trm1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_AUDVDD_TRM1V_MASK << SYS_ANA_REG12_AUDVDD_TRM1V_POS);
    reg_value |= ((value & SYS_ANA_REG12_AUDVDD_TRM1V_MASK) << SYS_ANA_REG12_AUDVDD_TRM1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->audvdd_voc1v:0x4c[20:16],audio 1.5V LDO output trimming, 00000=min, 11111=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_audvdd_voc1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_AUDVDD_VOC1V_POS) & SYS_ANA_REG12_AUDVDD_VOC1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_audvdd_voc1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_AUDVDD_VOC1V_MASK << SYS_ANA_REG12_AUDVDD_VOC1V_POS);
    reg_value |= ((value & SYS_ANA_REG12_AUDVDD_VOC1V_MASK) << SYS_ANA_REG12_AUDVDD_VOC1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->enaudvdd1v:0x4c[21],audio 1.0V LDO enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enaudvdd1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_ENAUDVDD1V_POS) & SYS_ANA_REG12_ENAUDVDD1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_enaudvdd1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_ENAUDVDD1V_MASK << SYS_ANA_REG12_ENAUDVDD1V_POS);
    reg_value |= ((value & SYS_ANA_REG12_ENAUDVDD1V_MASK) << SYS_ANA_REG12_ENAUDVDD1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->loadhp:0x4c[22],audio 1.5V LDO, 1=good stability with small loading,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_loadhp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_LOADHP_POS) & SYS_ANA_REG12_LOADHP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_loadhp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_LOADHP_MASK << SYS_ANA_REG12_LOADHP_POS);
    reg_value |= ((value & SYS_ANA_REG12_LOADHP_MASK) << SYS_ANA_REG12_LOADHP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->enaudvdd1v5:0x4c[23],audio 1.5V LDO enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enaudvdd1v5(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_ENAUDVDD1V5_POS) & SYS_ANA_REG12_ENAUDVDD1V5_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_enaudvdd1v5(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_ENAUDVDD1V5_MASK << SYS_ANA_REG12_ENAUDVDD1V5_POS);
    reg_value |= ((value & SYS_ANA_REG12_ENAUDVDD1V5_MASK) << SYS_ANA_REG12_ENAUDVDD1V5_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->enmicbias1v:0x4c[24],micbias enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_enmicbias1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_ENMICBIAS1V_POS) & SYS_ANA_REG12_ENMICBIAS1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_enmicbias1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_ENMICBIAS1V_MASK << SYS_ANA_REG12_ENMICBIAS1V_POS);
    reg_value |= ((value & SYS_ANA_REG12_ENMICBIAS1V_MASK) << SYS_ANA_REG12_ENMICBIAS1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->micbias_trim:0x4c[26:25],micbias output selection, 00=min, 11=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_micbias_trim(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_MICBIAS_TRIM_POS) & SYS_ANA_REG12_MICBIAS_TRIM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_micbias_trim(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_MICBIAS_TRIM_MASK << SYS_ANA_REG12_MICBIAS_TRIM_POS);
    reg_value |= ((value & SYS_ANA_REG12_MICBIAS_TRIM_MASK) << SYS_ANA_REG12_MICBIAS_TRIM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->micbias_voc1v:0x4c[31:27],micbias output trimming, 00000=min, 11111=max,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg12_micbias_voc1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_MICBIAS_VOC1V_POS) & SYS_ANA_REG12_MICBIAS_VOC1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_micbias_voc1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_MICBIAS_VOC1V_MASK << SYS_ANA_REG12_MICBIAS_VOC1V_POS);
    reg_value |= ((value & SYS_ANA_REG12_MICBIAS_VOC1V_MASK) << SYS_ANA_REG12_MICBIAS_VOC1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4D //REG ADDR :0x44010134 */
static inline uint32_t sys_ll_get_ana_reg13_value(void)
{
    return REG_READ(SYS_ANA_REG13_ADDR);
}

static inline void sys_ll_set_ana_reg13_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,value);
}

/* REG_0x4d:ana_reg13->byp_dwaadc:0x4d[8],adc dwa pass enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_byp_dwaadc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_BYP_DWAADC_POS) & SYS_ANA_REG13_BYP_DWAADC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_byp_dwaadc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_BYP_DWAADC_MASK << SYS_ANA_REG13_BYP_DWAADC_POS);
    reg_value |= ((value & SYS_ANA_REG13_BYP_DWAADC_MASK) << SYS_ANA_REG13_BYP_DWAADC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->rst:0x4d[9],rst,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_RST_POS) & SYS_ANA_REG13_RST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_rst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_RST_MASK << SYS_ANA_REG13_RST_POS);
    reg_value |= ((value & SYS_ANA_REG13_RST_MASK) << SYS_ANA_REG13_RST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->adcdwa_mode:0x4d[10],adc dwa model sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_adcdwa_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_ADCDWA_MODE_POS) & SYS_ANA_REG13_ADCDWA_MODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_adcdwa_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_ADCDWA_MODE_MASK << SYS_ANA_REG13_ADCDWA_MODE_POS);
    reg_value |= ((value & SYS_ANA_REG13_ADCDWA_MODE_MASK) << SYS_ANA_REG13_ADCDWA_MODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->vodadjspi:0x4d[15:11],adc reference manual spi control,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_vodadjspi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_VODADJSPI_POS) & SYS_ANA_REG13_VODADJSPI_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_vodadjspi(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_VODADJSPI_MASK << SYS_ANA_REG13_VODADJSPI_POS);
    reg_value |= ((value & SYS_ANA_REG13_VODADJSPI_MASK) << SYS_ANA_REG13_VODADJSPI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->refvsel:0x4d[21],0= high reference; 1=small reference,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_refvsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_REFVSEL_POS) & SYS_ANA_REG13_REFVSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_refvsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_REFVSEL_MASK << SYS_ANA_REG13_REFVSEL_POS);
    reg_value |= ((value & SYS_ANA_REG13_REFVSEL_MASK) << SYS_ANA_REG13_REFVSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->capsw1v:0x4d[27:23],munual value for cap trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_capsw1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_CAPSW1V_POS) & SYS_ANA_REG13_CAPSW1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_capsw1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_CAPSW1V_MASK << SYS_ANA_REG13_CAPSW1V_POS);
    reg_value |= ((value & SYS_ANA_REG13_CAPSW1V_MASK) << SYS_ANA_REG13_CAPSW1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->adcckinven:0x4d[30],audio adc clock inversion enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg13_adcckinven(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_ADCCKINVEN_POS) & SYS_ANA_REG13_ADCCKINVEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_adcckinven(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_ADCCKINVEN_MASK << SYS_ANA_REG13_ADCCKINVEN_POS);
    reg_value |= ((value & SYS_ANA_REG13_ADCCKINVEN_MASK) << SYS_ANA_REG13_ADCCKINVEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4E //REG ADDR :0x44010138 */
static inline uint32_t sys_ll_get_ana_reg14_value(void)
{
    return REG_READ(SYS_ANA_REG14_ADDR);
}

static inline void sys_ll_set_ana_reg14_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,value);
}

/* REG_0x4e:ana_reg14->isel:0x4e[1:0],adc bias trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_isel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_ISEL_POS) & SYS_ANA_REG14_ISEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_isel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_ISEL_MASK << SYS_ANA_REG14_ISEL_POS);
    reg_value |= ((value & SYS_ANA_REG14_ISEL_MASK) << SYS_ANA_REG14_ISEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdcocdin:0x4e[9:2],adc micmode dcoc din,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocdin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDCOCDIN_POS) & SYS_ANA_REG14_MICDCOCDIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdcocdin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDCOCDIN_MASK << SYS_ANA_REG14_MICDCOCDIN_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDCOCDIN_MASK) << SYS_ANA_REG14_MICDCOCDIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdcocvc:0x4e[11:10],adc micmode dcoc control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocvc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDCOCVC_POS) & SYS_ANA_REG14_MICDCOCVC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdcocvc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDCOCVC_MASK << SYS_ANA_REG14_MICDCOCVC_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDCOCVC_MASK) << SYS_ANA_REG14_MICDCOCVC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdcocen_n:0x4e[12],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocen_n(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDCOCEN_N_POS) & SYS_ANA_REG14_MICDCOCEN_N_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdcocen_n(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDCOCEN_N_MASK << SYS_ANA_REG14_MICDCOCEN_N_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDCOCEN_N_MASK) << SYS_ANA_REG14_MICDCOCEN_N_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdcocen_p:0x4e[13],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdcocen_p(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDCOCEN_P_POS) & SYS_ANA_REG14_MICDCOCEN_P_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdcocen_p(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDCOCEN_P_MASK << SYS_ANA_REG14_MICDCOCEN_P_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDCOCEN_P_MASK) << SYS_ANA_REG14_MICDCOCEN_P_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micsingleEn:0x4e[14],adc micmode, single_end enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micsingleen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICSINGLEEN_POS) & SYS_ANA_REG14_MICSINGLEEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micsingleen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICSINGLEEN_MASK << SYS_ANA_REG14_MICSINGLEEN_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICSINGLEEN_MASK) << SYS_ANA_REG14_MICSINGLEEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micGain:0x4e[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micgain(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICGAIN_POS) & SYS_ANA_REG14_MICGAIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micgain(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICGAIN_MASK << SYS_ANA_REG14_MICGAIN_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICGAIN_MASK) << SYS_ANA_REG14_MICGAIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdacen:0x4e[19],adc micmode micdac enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdacen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDACEN_POS) & SYS_ANA_REG14_MICDACEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdacen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDACEN_MASK << SYS_ANA_REG14_MICDACEN_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDACEN_MASK) << SYS_ANA_REG14_MICDACEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdaciH:0x4e[27:20],adc micmode, micdac input ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdacih(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDACIH_POS) & SYS_ANA_REG14_MICDACIH_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdacih(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDACIH_MASK << SYS_ANA_REG14_MICDACIH_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDACIH_MASK) << SYS_ANA_REG14_MICDACIH_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micdacit:0x4e[29:28],adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micdacit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICDACIT_POS) & SYS_ANA_REG14_MICDACIT_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micdacit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICDACIT_MASK << SYS_ANA_REG14_MICDACIT_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICDACIT_MASK) << SYS_ANA_REG14_MICDACIT_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->hcen:0x4e[30],adc 1stg op current trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_hcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_HCEN_POS) & SYS_ANA_REG14_HCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_hcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_HCEN_MASK << SYS_ANA_REG14_HCEN_POS);
    reg_value |= ((value & SYS_ANA_REG14_HCEN_MASK) << SYS_ANA_REG14_HCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->micEn:0x4e[31],mic1 mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_micen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_MICEN_POS) & SYS_ANA_REG14_MICEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_micen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_MICEN_MASK << SYS_ANA_REG14_MICEN_POS);
    reg_value |= ((value & SYS_ANA_REG14_MICEN_MASK) << SYS_ANA_REG14_MICEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4F //REG ADDR :0x4401013c */
static inline uint32_t sys_ll_get_ana_reg15_value(void)
{
    return REG_READ(SYS_ANA_REG15_ADDR);
}

static inline void sys_ll_set_ana_reg15_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,value);
}

/* REG_0x4f:ana_reg15->isel:0x4f[1:0],adc bias trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_isel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_ISEL_POS) & SYS_ANA_REG15_ISEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_isel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_ISEL_MASK << SYS_ANA_REG15_ISEL_POS);
    reg_value |= ((value & SYS_ANA_REG15_ISEL_MASK) << SYS_ANA_REG15_ISEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdcocdin:0x4f[9:2],adc micmode dcoc din,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocdin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDCOCDIN_POS) & SYS_ANA_REG15_MICDCOCDIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdcocdin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDCOCDIN_MASK << SYS_ANA_REG15_MICDCOCDIN_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDCOCDIN_MASK) << SYS_ANA_REG15_MICDCOCDIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdcocvc:0x4f[11:10],adc micmode dcoc control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocvc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDCOCVC_POS) & SYS_ANA_REG15_MICDCOCVC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdcocvc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDCOCVC_MASK << SYS_ANA_REG15_MICDCOCVC_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDCOCVC_MASK) << SYS_ANA_REG15_MICDCOCVC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdcocen_n:0x4f[12],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocen_n(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDCOCEN_N_POS) & SYS_ANA_REG15_MICDCOCEN_N_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdcocen_n(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDCOCEN_N_MASK << SYS_ANA_REG15_MICDCOCEN_N_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDCOCEN_N_MASK) << SYS_ANA_REG15_MICDCOCEN_N_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdcocen_p:0x4f[13],adc micmode dcoc enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdcocen_p(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDCOCEN_P_POS) & SYS_ANA_REG15_MICDCOCEN_P_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdcocen_p(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDCOCEN_P_MASK << SYS_ANA_REG15_MICDCOCEN_P_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDCOCEN_P_MASK) << SYS_ANA_REG15_MICDCOCEN_P_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micsingleEn:0x4f[14],adc micmode, single_end enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micsingleen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICSINGLEEN_POS) & SYS_ANA_REG15_MICSINGLEEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micsingleen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICSINGLEEN_MASK << SYS_ANA_REG15_MICSINGLEEN_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICSINGLEEN_MASK) << SYS_ANA_REG15_MICSINGLEEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micGain:0x4f[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micgain(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICGAIN_POS) & SYS_ANA_REG15_MICGAIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micgain(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICGAIN_MASK << SYS_ANA_REG15_MICGAIN_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICGAIN_MASK) << SYS_ANA_REG15_MICGAIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdacen:0x4f[19],adc micmode micdac enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdacen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDACEN_POS) & SYS_ANA_REG15_MICDACEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdacen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDACEN_MASK << SYS_ANA_REG15_MICDACEN_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDACEN_MASK) << SYS_ANA_REG15_MICDACEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdaciH:0x4f[27:20],adc micmode, micdac input ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdacih(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDACIH_POS) & SYS_ANA_REG15_MICDACIH_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdacih(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDACIH_MASK << SYS_ANA_REG15_MICDACIH_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDACIH_MASK) << SYS_ANA_REG15_MICDACIH_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micdacit:0x4f[29:28],adc micmode, mic_dac Iout Full-range, 00=280uA, 01=320uA, 1X=447uA,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micdacit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICDACIT_POS) & SYS_ANA_REG15_MICDACIT_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micdacit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICDACIT_MASK << SYS_ANA_REG15_MICDACIT_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICDACIT_MASK) << SYS_ANA_REG15_MICDACIT_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->hcen:0x4f[30],adc 1stg op current trimming,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_hcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_HCEN_POS) & SYS_ANA_REG15_HCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_hcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_HCEN_MASK << SYS_ANA_REG15_HCEN_POS);
    reg_value |= ((value & SYS_ANA_REG15_HCEN_MASK) << SYS_ANA_REG15_HCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->micEn:0x4f[31],mic2 mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_micen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_MICEN_POS) & SYS_ANA_REG15_MICEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_micen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_MICEN_MASK << SYS_ANA_REG15_MICEN_POS);
    reg_value |= ((value & SYS_ANA_REG15_MICEN_MASK) << SYS_ANA_REG15_MICEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x50 //REG ADDR :0x44010140 */
static inline uint32_t sys_ll_get_ana_reg16_value(void)
{
    return REG_READ(SYS_ANA_REG16_ADDR);
}

static inline void sys_ll_set_ana_reg16_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,value);
}

/* REG_0x50:ana_reg16->hpdac:0x50[0],class ab driver high current mode. "1" high current. ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_hpdac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_HPDAC_POS) & SYS_ANA_REG16_HPDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_hpdac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_HPDAC_MASK << SYS_ANA_REG16_HPDAC_POS);
    reg_value |= ((value & SYS_ANA_REG16_HPDAC_MASK) << SYS_ANA_REG16_HPDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->vcmsdac:0x50[1],1stg OP input common model voltage selection. "1" low common mode voltage,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_vcmsdac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_VCMSDAC_POS) & SYS_ANA_REG16_VCMSDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_vcmsdac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_VCMSDAC_MASK << SYS_ANA_REG16_VCMSDAC_POS);
    reg_value |= ((value & SYS_ANA_REG16_VCMSDAC_MASK) << SYS_ANA_REG16_VCMSDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->oscdac:0x50[3:2],threshold current setting for over current protection . "3" maximum current. "0" minimum current,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_oscdac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_OSCDAC_POS) & SYS_ANA_REG16_OSCDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_oscdac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_OSCDAC_MASK << SYS_ANA_REG16_OSCDAC_POS);
    reg_value |= ((value & SYS_ANA_REG16_OSCDAC_MASK) << SYS_ANA_REG16_OSCDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->ocendac:0x50[4],over current protection enable. "1" enable.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_ocendac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_OCENDAC_POS) & SYS_ANA_REG16_OCENDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_ocendac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_OCENDAC_MASK << SYS_ANA_REG16_OCENDAC_POS);
    reg_value |= ((value & SYS_ANA_REG16_OCENDAC_MASK) << SYS_ANA_REG16_OCENDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->isel_idac:0x50[5],idac current sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_isel_idac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_ISEL_IDAC_POS) & SYS_ANA_REG16_ISEL_IDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_isel_idac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_ISEL_IDAC_MASK << SYS_ANA_REG16_ISEL_IDAC_POS);
    reg_value |= ((value & SYS_ANA_REG16_ISEL_IDAC_MASK) << SYS_ANA_REG16_ISEL_IDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->adjdacref:0x50[10:6],audio dac reference voltage adjust.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_adjdacref(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_ADJDACREF_POS) & SYS_ANA_REG16_ADJDACREF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_adjdacref(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_ADJDACREF_MASK << SYS_ANA_REG16_ADJDACREF_POS);
    reg_value |= ((value & SYS_ANA_REG16_ADJDACREF_MASK) << SYS_ANA_REG16_ADJDACREF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dcochg:0x50[12],dcoc high gain selection. "1" high gain,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dcochg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DCOCHG_POS) & SYS_ANA_REG16_DCOCHG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_dcochg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DCOCHG_MASK << SYS_ANA_REG16_DCOCHG_POS);
    reg_value |= ((value & SYS_ANA_REG16_DCOCHG_MASK) << SYS_ANA_REG16_DCOCHG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->diffen:0x50[13],enable differential mode. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_diffen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DIFFEN_POS) & SYS_ANA_REG16_DIFFEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_diffen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DIFFEN_MASK << SYS_ANA_REG16_DIFFEN_POS);
    reg_value |= ((value & SYS_ANA_REG16_DIFFEN_MASK) << SYS_ANA_REG16_DIFFEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->endaccal:0x50[14],enable offset calibration process. "1" enable.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_endaccal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_ENDACCAL_POS) & SYS_ANA_REG16_ENDACCAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_endaccal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_ENDACCAL_MASK << SYS_ANA_REG16_ENDACCAL_POS);
    reg_value |= ((value & SYS_ANA_REG16_ENDACCAL_MASK) << SYS_ANA_REG16_ENDACCAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->rendcoc:0x50[15],R-channel dcoc dac enablel. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_rendcoc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_RENDCOC_POS) & SYS_ANA_REG16_RENDCOC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_rendcoc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_RENDCOC_MASK << SYS_ANA_REG16_RENDCOC_POS);
    reg_value |= ((value & SYS_ANA_REG16_RENDCOC_MASK) << SYS_ANA_REG16_RENDCOC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->lendcoc:0x50[16],L-channel Dcoc dac enable. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_lendcoc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_LENDCOC_POS) & SYS_ANA_REG16_LENDCOC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_lendcoc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_LENDCOC_MASK << SYS_ANA_REG16_LENDCOC_POS);
    reg_value |= ((value & SYS_ANA_REG16_LENDCOC_MASK) << SYS_ANA_REG16_LENDCOC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->renvcmd:0x50[17],R-channel common mode output buffer enable."1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_renvcmd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_RENVCMD_POS) & SYS_ANA_REG16_RENVCMD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_renvcmd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_RENVCMD_MASK << SYS_ANA_REG16_RENVCMD_POS);
    reg_value |= ((value & SYS_ANA_REG16_RENVCMD_MASK) << SYS_ANA_REG16_RENVCMD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->lenvcmd:0x50[18],L-channel common mode output buffer enable. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_lenvcmd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_LENVCMD_POS) & SYS_ANA_REG16_LENVCMD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_lenvcmd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_LENVCMD_MASK << SYS_ANA_REG16_LENVCMD_POS);
    reg_value |= ((value & SYS_ANA_REG16_LENVCMD_MASK) << SYS_ANA_REG16_LENVCMD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dacdrven:0x50[19],dac output driver enable."1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacdrven(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DACDRVEN_POS) & SYS_ANA_REG16_DACDRVEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_dacdrven(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DACDRVEN_MASK << SYS_ANA_REG16_DACDRVEN_POS);
    reg_value |= ((value & SYS_ANA_REG16_DACDRVEN_MASK) << SYS_ANA_REG16_DACDRVEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dacRen:0x50[20],dac R-channel enable. "1"  enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacren(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DACREN_POS) & SYS_ANA_REG16_DACREN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_dacren(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DACREN_MASK << SYS_ANA_REG16_DACREN_POS);
    reg_value |= ((value & SYS_ANA_REG16_DACREN_MASK) << SYS_ANA_REG16_DACREN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dacLen:0x50[21],dac L-channel enable. "1" enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_daclen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DACLEN_POS) & SYS_ANA_REG16_DACLEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_daclen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DACLEN_MASK << SYS_ANA_REG16_DACLEN_POS);
    reg_value |= ((value & SYS_ANA_REG16_DACLEN_MASK) << SYS_ANA_REG16_DACLEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dacG:0x50[24:22],dac gain setting: 000=0dB, 111=8dB,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DACG_POS) & SYS_ANA_REG16_DACG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_dacg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DACG_MASK << SYS_ANA_REG16_DACG_POS);
    reg_value |= ((value & SYS_ANA_REG16_DACG_MASK) << SYS_ANA_REG16_DACG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->ck4xsel:0x50[25],dac clock sel ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_ck4xsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_CK4XSEL_POS) & SYS_ANA_REG16_CK4XSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_ck4xsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_CK4XSEL_MASK << SYS_ANA_REG16_CK4XSEL_POS);
    reg_value |= ((value & SYS_ANA_REG16_CK4XSEL_MASK) << SYS_ANA_REG16_CK4XSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dacmute:0x50[26],dac mute enable. "1" mute enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dacmute(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DACMUTE_POS) & SYS_ANA_REG16_DACMUTE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_dacmute(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DACMUTE_MASK << SYS_ANA_REG16_DACMUTE_POS);
    reg_value |= ((value & SYS_ANA_REG16_DACMUTE_MASK) << SYS_ANA_REG16_DACMUTE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->dwamode:0x50[27],dac dwa mode sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_dwamode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_DWAMODE_POS) & SYS_ANA_REG16_DWAMODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_dwamode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_DWAMODE_MASK << SYS_ANA_REG16_DWAMODE_POS);
    reg_value |= ((value & SYS_ANA_REG16_DWAMODE_MASK) << SYS_ANA_REG16_DWAMODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->ckposel:0x50[28],dac sample clock edge selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_ckposel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_CKPOSEL_POS) & SYS_ANA_REG16_CKPOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_ckposel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_CKPOSEL_MASK << SYS_ANA_REG16_CKPOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG16_CKPOSEL_MASK) << SYS_ANA_REG16_CKPOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->byldo:0x50[31],bypass 1v8 LDO,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_byldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_BYLDO_POS) & SYS_ANA_REG16_BYLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_byldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_BYLDO_MASK << SYS_ANA_REG16_BYLDO_POS);
    reg_value |= ((value & SYS_ANA_REG16_BYLDO_MASK) << SYS_ANA_REG16_BYLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x51 //REG ADDR :0x44010144 */
static inline uint32_t sys_ll_get_ana_reg17_value(void)
{
    return REG_READ(SYS_ANA_REG17_ADDR);
}

static inline void sys_ll_set_ana_reg17_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,value);
}

/* REG_0x51:ana_reg17->lmdcin:0x51[7:0],l-cnannel offset cancel dac maumual input.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_lmdcin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_LMDCIN_POS) & SYS_ANA_REG17_LMDCIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_lmdcin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_LMDCIN_MASK << SYS_ANA_REG17_LMDCIN_POS);
    reg_value |= ((value & SYS_ANA_REG17_LMDCIN_MASK) << SYS_ANA_REG17_LMDCIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->rmdcin:0x51[15:8],r-channel offset cancel dac manmual input ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_rmdcin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_RMDCIN_POS) & SYS_ANA_REG17_RMDCIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_rmdcin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_RMDCIN_MASK << SYS_ANA_REG17_RMDCIN_POS);
    reg_value |= ((value & SYS_ANA_REG17_RMDCIN_MASK) << SYS_ANA_REG17_RMDCIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->spirst_ovc:0x51[16],ovc rst,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_spirst_ovc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_SPIRST_OVC_POS) & SYS_ANA_REG17_SPIRST_OVC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_spirst_ovc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_SPIRST_OVC_MASK << SYS_ANA_REG17_SPIRST_OVC_POS);
    reg_value |= ((value & SYS_ANA_REG17_SPIRST_OVC_MASK) << SYS_ANA_REG17_SPIRST_OVC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->hc2s0v9:0x51[20],0=current is half,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_hc2s0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_HC2S0V9_POS) & SYS_ANA_REG17_HC2S0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_hc2s0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_HC2S0V9_MASK << SYS_ANA_REG17_HC2S0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_HC2S0V9_MASK) << SYS_ANA_REG17_HC2S0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->lvcmsel:0x51[21],low vcm sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_lvcmsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_LVCMSEL_POS) & SYS_ANA_REG17_LVCMSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_lvcmsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_LVCMSEL_MASK << SYS_ANA_REG17_LVCMSEL_POS);
    reg_value |= ((value & SYS_ANA_REG17_LVCMSEL_MASK) << SYS_ANA_REG17_LVCMSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->loop2sel:0x51[22],2rd loop sel,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_loop2sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_LOOP2SEL_POS) & SYS_ANA_REG17_LOOP2SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_loop2sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_LOOP2SEL_MASK << SYS_ANA_REG17_LOOP2SEL_POS);
    reg_value |= ((value & SYS_ANA_REG17_LOOP2SEL_MASK) << SYS_ANA_REG17_LOOP2SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->enbias:0x51[23],dac bias enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_enbias(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_ENBIAS_POS) & SYS_ANA_REG17_ENBIAS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_enbias(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_ENBIAS_MASK << SYS_ANA_REG17_ENBIAS_POS);
    reg_value |= ((value & SYS_ANA_REG17_ENBIAS_MASK) << SYS_ANA_REG17_ENBIAS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->calck_sel0v9:0x51[24],offset calibration clock selection. "1" high clock.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_calck_sel0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_CALCK_SEL0V9_POS) & SYS_ANA_REG17_CALCK_SEL0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_calck_sel0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_CALCK_SEL0V9_MASK << SYS_ANA_REG17_CALCK_SEL0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_CALCK_SEL0V9_MASK) << SYS_ANA_REG17_CALCK_SEL0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->bpdwa0v9:0x51[25],bypss audio dac dwa. "1" bypass.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_bpdwa0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_BPDWA0V9_POS) & SYS_ANA_REG17_BPDWA0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_bpdwa0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_BPDWA0V9_MASK << SYS_ANA_REG17_BPDWA0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_BPDWA0V9_MASK) << SYS_ANA_REG17_BPDWA0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->looprst0v9:0x51[26],audio dac integrator capacitor reset. "1" reset.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_looprst0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_LOOPRST0V9_POS) & SYS_ANA_REG17_LOOPRST0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_looprst0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_LOOPRST0V9_MASK << SYS_ANA_REG17_LOOPRST0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_LOOPRST0V9_MASK) << SYS_ANA_REG17_LOOPRST0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->oct0v9:0x51[28:27],over current delay time setting."11" maximum time. "00" minimum current.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_oct0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_OCT0V9_POS) & SYS_ANA_REG17_OCT0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_oct0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_OCT0V9_MASK << SYS_ANA_REG17_OCT0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_OCT0V9_MASK) << SYS_ANA_REG17_OCT0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->sout0v9:0x51[29],short output with 600ohm resistor. "1" short output.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_sout0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_SOUT0V9_POS) & SYS_ANA_REG17_SOUT0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_sout0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_SOUT0V9_MASK << SYS_ANA_REG17_SOUT0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_SOUT0V9_MASK) << SYS_ANA_REG17_SOUT0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->hc0v9:0x51[31:30],dac current trimming, 00=minimum current, 11=maximum current,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_hc0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_HC0V9_POS) & SYS_ANA_REG17_HC0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_hc0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_HC0V9_MASK << SYS_ANA_REG17_HC0V9_POS);
    reg_value |= ((value & SYS_ANA_REG17_HC0V9_MASK) << SYS_ANA_REG17_HC0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x52 //REG ADDR :0x44010148 */
/*write only reg:ana_reg18:default value:0xb8b67*/
static inline void sys_ll_set_ana_reg18_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,value);
}

/* REG_0x53 //REG ADDR :0x4401014c */
/*write only reg:ana_reg19:default value:0x45900a02*/
static inline void sys_ll_set_ana_reg19_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,value);
}

#ifdef __cplusplus 
}                  
#endif             
