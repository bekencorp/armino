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

/* REG_0x09:cpu_clk_div_mode2->ckdiv_sdio:0x9[16:14],0:/1  1:/2  2:/4  3:/8  4:/16  5:/32  6:/64  7:/256,0,R/W*/
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

/* REG_0x40:ana_reg0->dpll_tsten:0x40[0],dpll test enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_dpll_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_DPLL_TSTEN_POS) & SYS_ANA_REG0_DPLL_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_dpll_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_DPLL_TSTEN_MASK << SYS_ANA_REG0_DPLL_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG0_DPLL_TSTEN_MASK) << SYS_ANA_REG0_DPLL_TSTEN_POS);
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

/* REG_0x40:ana_reg0->LoopRzctrl:0x40[13:10],Rz ctrl:2K to 17K,1K step,2,R/W*/
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

/* REG_0x40:ana_reg0->openloop_en:0x40[16],open loop test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_openloop_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_OPENLOOP_EN_POS) & SYS_ANA_REG0_OPENLOOP_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_openloop_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_OPENLOOP_EN_MASK << SYS_ANA_REG0_OPENLOOP_EN_POS);
    reg_value |= ((value & SYS_ANA_REG0_OPENLOOP_EN_MASK) << SYS_ANA_REG0_OPENLOOP_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->cksel:0x40[18:17],00:26M;01:40M;,0,R/W*/
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

/* REG_0x40:ana_reg0->spitrig:0x40[19],SPI band selection trigger signal,1,R/W*/
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

/* REG_0x40:ana_reg0->band0:0x40[20],enable the output of 480M clock and the output of 160M clock of calibration,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_band0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_BAND0_POS) & SYS_ANA_REG0_BAND0_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_band0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_BAND0_MASK << SYS_ANA_REG0_BAND0_POS);
    reg_value |= ((value & SYS_ANA_REG0_BAND0_MASK) << SYS_ANA_REG0_BAND0_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->band1:0x40[21],enable the output of 320M clock,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_band1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_BAND1_POS) & SYS_ANA_REG0_BAND1_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_band1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_BAND1_MASK << SYS_ANA_REG0_BAND1_POS);
    reg_value |= ((value & SYS_ANA_REG0_BAND1_MASK) << SYS_ANA_REG0_BAND1_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x40:ana_reg0->band:0x40[24:22],band manual value,2,R/W*/
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

/* REG_0x40:ana_reg0->vctrl_dpllldo:0x40[31],0 :0.9V/ 1: 1V,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg0_vctrl_dpllldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG0_VCTRL_DPLLLDO_POS) & SYS_ANA_REG0_VCTRL_DPLLLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg0_vctrl_dpllldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG0_ADDR);
    reg_value &= ~(SYS_ANA_REG0_VCTRL_DPLLLDO_MASK << SYS_ANA_REG0_VCTRL_DPLLLDO_POS);
    reg_value |= ((value & SYS_ANA_REG0_VCTRL_DPLLLDO_MASK) << SYS_ANA_REG0_VCTRL_DPLLLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG0_ADDR,reg_value);
}

/* REG_0x41 //REG ADDR :0x44010104 */
static inline void sys_ll_set_ana_reg1_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,value);
}

/* REG_0x41:ana_reg1->msw:0x41[10:2],set the frequency of DCO manual,70,W*/
static inline void sys_ll_set_ana_reg1_msw(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_MSW_MASK << SYS_ANA_REG1_MSW_POS);
    reg_value |= ((value & SYS_ANA_REG1_MSW_MASK) << SYS_ANA_REG1_MSW_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->ictrl:0x41[13:11],controlling the bias cuttent of DCO core,3,W*/
static inline void sys_ll_set_ana_reg1_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_ICTRL_MASK << SYS_ANA_REG1_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG1_ICTRL_MASK) << SYS_ANA_REG1_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->osc_trig:0x41[14],reset the DCO core by spi to make it oscillate again,0,W*/
static inline void sys_ll_set_ana_reg1_osc_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_OSC_TRIG_MASK << SYS_ANA_REG1_OSC_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG1_OSC_TRIG_MASK) << SYS_ANA_REG1_OSC_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->osccal_trig:0x41[15],trigger the action of callibration in the DCO,0,W*/
static inline void sys_ll_set_ana_reg1_osccal_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_OSCCAL_TRIG_MASK << SYS_ANA_REG1_OSCCAL_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG1_OSCCAL_TRIG_MASK) << SYS_ANA_REG1_OSCCAL_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->cnti:0x41[24:16],set the controlling work of calibration in the DCO block to get the different frequency,180,W*/
static inline void sys_ll_set_ana_reg1_cnti(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_CNTI_MASK << SYS_ANA_REG1_CNTI_POS);
    reg_value |= ((value & SYS_ANA_REG1_CNTI_MASK) << SYS_ANA_REG1_CNTI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->spi_rst:0x41[25],reset the calibration block of DCO by spi,0,W*/
static inline void sys_ll_set_ana_reg1_spi_rst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_SPI_RST_MASK << SYS_ANA_REG1_SPI_RST_POS);
    reg_value |= ((value & SYS_ANA_REG1_SPI_RST_MASK) << SYS_ANA_REG1_SPI_RST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->amsel:0x41[26],disable the calibration function of the DCO,set the frequency of DCO manual,1,W*/
static inline void sys_ll_set_ana_reg1_amsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_AMSEL_MASK << SYS_ANA_REG1_AMSEL_POS);
    reg_value |= ((value & SYS_ANA_REG1_AMSEL_MASK) << SYS_ANA_REG1_AMSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG1_ADDR,reg_value);
}

/* REG_0x41:ana_reg1->divctrl:0x41[29:27],controlling the value of divider in the DCO to get the different frequency,3,W*/
static inline void sys_ll_set_ana_reg1_divctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG1_ADDR);
    reg_value &= ~(SYS_ANA_REG1_DIVCTRL_MASK << SYS_ANA_REG1_DIVCTRL_POS);
    reg_value |= ((value & SYS_ANA_REG1_DIVCTRL_MASK) << SYS_ANA_REG1_DIVCTRL_POS);
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

/* REG_0x42:ana_reg2->xtalh_ctune:0x42[7:0],xtalh load cap tuning,30,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_xtalh_ctune(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_XTALH_CTUNE_POS) & SYS_ANA_REG2_XTALH_CTUNE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_xtalh_ctune(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_XTALH_CTUNE_MASK << SYS_ANA_REG2_XTALH_CTUNE_POS);
    reg_value |= ((value & SYS_ANA_REG2_XTALH_CTUNE_MASK) << SYS_ANA_REG2_XTALH_CTUNE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->force_26mpll:0x42[8],26M PLL force enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_force_26mpll(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_FORCE_26MPLL_POS) & SYS_ANA_REG2_FORCE_26MPLL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_force_26mpll(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_FORCE_26MPLL_MASK << SYS_ANA_REG2_FORCE_26MPLL_POS);
    reg_value |= ((value & SYS_ANA_REG2_FORCE_26MPLL_MASK) << SYS_ANA_REG2_FORCE_26MPLL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->gadc_cmp_ictrl:0x42[10:9],gadc comparaor current select ,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_gadc_cmp_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_GADC_CMP_ICTRL_POS) & SYS_ANA_REG2_GADC_CMP_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_gadc_cmp_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_GADC_CMP_ICTRL_MASK << SYS_ANA_REG2_GADC_CMP_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG2_GADC_CMP_ICTRL_MASK) << SYS_ANA_REG2_GADC_CMP_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->gadc_inbuf_ictrl:0x42[12:11],gadc input buffer current select ,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_gadc_inbuf_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_GADC_INBUF_ICTRL_POS) & SYS_ANA_REG2_GADC_INBUF_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_gadc_inbuf_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_GADC_INBUF_ICTRL_MASK << SYS_ANA_REG2_GADC_INBUF_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG2_GADC_INBUF_ICTRL_MASK) << SYS_ANA_REG2_GADC_INBUF_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->gadc_refbuf_ictrl:0x42[14:13],gadc reference buffer current select ,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_gadc_refbuf_ictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_GADC_REFBUF_ICTRL_POS) & SYS_ANA_REG2_GADC_REFBUF_ICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_gadc_refbuf_ictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_GADC_REFBUF_ICTRL_MASK << SYS_ANA_REG2_GADC_REFBUF_ICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG2_GADC_REFBUF_ICTRL_MASK) << SYS_ANA_REG2_GADC_REFBUF_ICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->gadc_nobuf_enable:0x42[15],gadc no buffer mode enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_gadc_nobuf_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_GADC_NOBUF_ENABLE_POS) & SYS_ANA_REG2_GADC_NOBUF_ENABLE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_gadc_nobuf_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_GADC_NOBUF_ENABLE_MASK << SYS_ANA_REG2_GADC_NOBUF_ENABLE_POS);
    reg_value |= ((value & SYS_ANA_REG2_GADC_NOBUF_ENABLE_MASK) << SYS_ANA_REG2_GADC_NOBUF_ENABLE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->vref_scale:0x42[16],gadc reference voltage scale enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_vref_scale(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_VREF_SCALE_POS) & SYS_ANA_REG2_VREF_SCALE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_vref_scale(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_VREF_SCALE_MASK << SYS_ANA_REG2_VREF_SCALE_POS);
    reg_value |= ((value & SYS_ANA_REG2_VREF_SCALE_MASK) << SYS_ANA_REG2_VREF_SCALE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->scal_en:0x42[17],gadc reference scale enable, 0:normal mode,1: scale mode ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_scal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_SCAL_EN_POS) & SYS_ANA_REG2_SCAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_scal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_SCAL_EN_MASK << SYS_ANA_REG2_SCAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG2_SCAL_EN_MASK) << SYS_ANA_REG2_SCAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->gadc_capcal_en:0x42[18],gadc cap cali enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_gadc_capcal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_GADC_CAPCAL_EN_POS) & SYS_ANA_REG2_GADC_CAPCAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_gadc_capcal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_GADC_CAPCAL_EN_MASK << SYS_ANA_REG2_GADC_CAPCAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG2_GADC_CAPCAL_EN_MASK) << SYS_ANA_REG2_GADC_CAPCAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->gadc_capcal:0x42[24:19],gadc cap cali value,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_gadc_capcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_GADC_CAPCAL_POS) & SYS_ANA_REG2_GADC_CAPCAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_gadc_capcal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_GADC_CAPCAL_MASK << SYS_ANA_REG2_GADC_CAPCAL_POS);
    reg_value |= ((value & SYS_ANA_REG2_GADC_CAPCAL_MASK) << SYS_ANA_REG2_GADC_CAPCAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x42:ana_reg2->sp_nt_ctrl:0x42[31:25],gadc timing control,3F,R/W*/
static inline uint32_t sys_ll_get_ana_reg2_sp_nt_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG2_SP_NT_CTRL_POS) & SYS_ANA_REG2_SP_NT_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg2_sp_nt_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG2_ADDR);
    reg_value &= ~(SYS_ANA_REG2_SP_NT_CTRL_MASK << SYS_ANA_REG2_SP_NT_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG2_SP_NT_CTRL_MASK) << SYS_ANA_REG2_SP_NT_CTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG2_ADDR,reg_value);
}

/* REG_0x43 //REG ADDR :0x4401010c */
static inline void sys_ll_set_ana_reg3_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,value);
}

/* REG_0x43:ana_reg3->usbpen:0x43[3:0],usb dp driver capability control,8,W*/
static inline void sys_ll_set_ana_reg3_usbpen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_USBPEN_MASK << SYS_ANA_REG3_USBPEN_POS);
    reg_value |= ((value & SYS_ANA_REG3_USBPEN_MASK) << SYS_ANA_REG3_USBPEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->usbnen:0x43[7:4],usb dn driver capability control,8,W*/
static inline void sys_ll_set_ana_reg3_usbnen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_USBNEN_MASK << SYS_ANA_REG3_USBNEN_POS);
    reg_value |= ((value & SYS_ANA_REG3_USBNEN_MASK) << SYS_ANA_REG3_USBNEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->hpssren:0x43[8],xtal core high psrr enable.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_hpssren(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_HPSSREN_POS) & SYS_ANA_REG3_HPSSREN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_hpssren(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_HPSSREN_MASK << SYS_ANA_REG3_HPSSREN_POS);
    reg_value |= ((value & SYS_ANA_REG3_HPSSREN_MASK) << SYS_ANA_REG3_HPSSREN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->ck_sel:0x43[9],xtal digital buffer selection. 0 : high hystersis digital buffer.; 1. high gain digital buffer.,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_ck_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_CK_SEL_POS) & SYS_ANA_REG3_CK_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_ck_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_CK_SEL_MASK << SYS_ANA_REG3_CK_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG3_CK_SEL_MASK) << SYS_ANA_REG3_CK_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->anabuf_sel_rx:0x43[10],xtal analog buffer selection. 0: high power analog buffer; 1: high psrr analog buffer for rx mode,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_anabuf_sel_rx(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ANABUF_SEL_RX_POS) & SYS_ANA_REG3_ANABUF_SEL_RX_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_anabuf_sel_rx(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ANABUF_SEL_RX_MASK << SYS_ANA_REG3_ANABUF_SEL_RX_POS);
    reg_value |= ((value & SYS_ANA_REG3_ANABUF_SEL_RX_MASK) << SYS_ANA_REG3_ANABUF_SEL_RX_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->pwd_xtalldo:0x43[11],xtal ldo pwd,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_pwd_xtalldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_PWD_XTALLDO_POS) & SYS_ANA_REG3_PWD_XTALLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_pwd_xtalldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_PWD_XTALLDO_MASK << SYS_ANA_REG3_PWD_XTALLDO_POS);
    reg_value |= ((value & SYS_ANA_REG3_PWD_XTALLDO_MASK) << SYS_ANA_REG3_PWD_XTALLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->iamp:0x43[12],bias current control of xtal amplitude control.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_iamp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_IAMP_POS) & SYS_ANA_REG3_IAMP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_iamp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_IAMP_MASK << SYS_ANA_REG3_IAMP_POS);
    reg_value |= ((value & SYS_ANA_REG3_IAMP_MASK) << SYS_ANA_REG3_IAMP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->vddren:0x43[13],RC enable on the power supply of  xtal digital buffer.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_vddren(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_VDDREN_POS) & SYS_ANA_REG3_VDDREN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_vddren(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_VDDREN_MASK << SYS_ANA_REG3_VDDREN_POS);
    reg_value |= ((value & SYS_ANA_REG3_VDDREN_MASK) << SYS_ANA_REG3_VDDREN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->xamp:0x43[19:14],xtal amplitude control,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_xamp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_XAMP_POS) & SYS_ANA_REG3_XAMP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_xamp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_XAMP_MASK << SYS_ANA_REG3_XAMP_POS);
    reg_value |= ((value & SYS_ANA_REG3_XAMP_MASK) << SYS_ANA_REG3_XAMP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->vosel:0x43[24:20],xtal ldo output voltage control.,5,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_vosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_VOSEL_POS) & SYS_ANA_REG3_VOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_vosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_VOSEL_MASK << SYS_ANA_REG3_VOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG3_VOSEL_MASK) << SYS_ANA_REG3_VOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->en_xtalh_sleep:0x43[25],xtalh sleep enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_en_xtalh_sleep(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_EN_XTALH_SLEEP_POS) & SYS_ANA_REG3_EN_XTALH_SLEEP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_en_xtalh_sleep(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_EN_XTALH_SLEEP_MASK << SYS_ANA_REG3_EN_XTALH_SLEEP_POS);
    reg_value |= ((value & SYS_ANA_REG3_EN_XTALH_SLEEP_MASK) << SYS_ANA_REG3_EN_XTALH_SLEEP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->xtal40_en:0x43[26],40M to 32K div enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_xtal40_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_XTAL40_EN_POS) & SYS_ANA_REG3_XTAL40_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_xtal40_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_XTAL40_EN_MASK << SYS_ANA_REG3_XTAL40_EN_POS);
    reg_value |= ((value & SYS_ANA_REG3_XTAL40_EN_MASK) << SYS_ANA_REG3_XTAL40_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->bufictrl:0x43[27],xtal analog buffer current control. ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_bufictrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_BUFICTRL_POS) & SYS_ANA_REG3_BUFICTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_bufictrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_BUFICTRL_MASK << SYS_ANA_REG3_BUFICTRL_POS);
    reg_value |= ((value & SYS_ANA_REG3_BUFICTRL_MASK) << SYS_ANA_REG3_BUFICTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->ibias_ctrl:0x43[29:28],xtal core bias current control. 0->3 current increased.,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_ibias_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_IBIAS_CTRL_POS) & SYS_ANA_REG3_IBIAS_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_ibias_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_IBIAS_CTRL_MASK << SYS_ANA_REG3_IBIAS_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG3_IBIAS_CTRL_MASK) << SYS_ANA_REG3_IBIAS_CTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG3_ADDR,reg_value);
}

/* REG_0x43:ana_reg3->icore_ctrl:0x43[31:30],xtal core current control. 0->3 current increased.,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg3_icore_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG3_ICORE_CTRL_POS) & SYS_ANA_REG3_ICORE_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg3_icore_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG3_ADDR);
    reg_value &= ~(SYS_ANA_REG3_ICORE_CTRL_MASK << SYS_ANA_REG3_ICORE_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG3_ICORE_CTRL_MASK) << SYS_ANA_REG3_ICORE_CTRL_POS);
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

/* REG_0x44:ana_reg4->anabuf_sel_tx:0x44[0],xtal analog buffer selection. 0: high power analog buffer; 1: high psrr analog buffer for tx mode,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_anabuf_sel_tx(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ANABUF_SEL_TX_POS) & SYS_ANA_REG4_ANABUF_SEL_TX_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_anabuf_sel_tx(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ANABUF_SEL_TX_MASK << SYS_ANA_REG4_ANABUF_SEL_TX_POS);
    reg_value |= ((value & SYS_ANA_REG4_ANABUF_SEL_TX_MASK) << SYS_ANA_REG4_ANABUF_SEL_TX_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->trng_tsten:0x44[1],TRNG setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_trng_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_TRNG_TSTEN_POS) & SYS_ANA_REG4_TRNG_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_trng_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_TRNG_TSTEN_MASK << SYS_ANA_REG4_TRNG_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG4_TRNG_TSTEN_MASK) << SYS_ANA_REG4_TRNG_TSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->itune_ref:0x44[4:2],TRNG setting,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_itune_ref(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ITUNE_REF_POS) & SYS_ANA_REG4_ITUNE_REF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_itune_ref(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ITUNE_REF_MASK << SYS_ANA_REG4_ITUNE_REF_POS);
    reg_value |= ((value & SYS_ANA_REG4_ITUNE_REF_MASK) << SYS_ANA_REG4_ITUNE_REF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->itune_opa:0x44[7:5],TRNG setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_itune_opa(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ITUNE_OPA_POS) & SYS_ANA_REG4_ITUNE_OPA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_itune_opa(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ITUNE_OPA_MASK << SYS_ANA_REG4_ITUNE_OPA_POS);
    reg_value |= ((value & SYS_ANA_REG4_ITUNE_OPA_MASK) << SYS_ANA_REG4_ITUNE_OPA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->itune_cmp:0x44[10:8],TRNG setting,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_itune_cmp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_ITUNE_CMP_POS) & SYS_ANA_REG4_ITUNE_CMP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_itune_cmp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_ITUNE_CMP_MASK << SYS_ANA_REG4_ITUNE_CMP_POS);
    reg_value |= ((value & SYS_ANA_REG4_ITUNE_CMP_MASK) << SYS_ANA_REG4_ITUNE_CMP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->Rnooise_sel:0x44[11],TRNG setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_rnooise_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_RNOOISE_SEL_POS) & SYS_ANA_REG4_RNOOISE_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_rnooise_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_RNOOISE_SEL_MASK << SYS_ANA_REG4_RNOOISE_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG4_RNOOISE_SEL_MASK) << SYS_ANA_REG4_RNOOISE_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->Fslow_sel:0x44[14:12],TRNG setting,2,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_fslow_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_FSLOW_SEL_POS) & SYS_ANA_REG4_FSLOW_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_fslow_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_FSLOW_SEL_MASK << SYS_ANA_REG4_FSLOW_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG4_FSLOW_SEL_MASK) << SYS_ANA_REG4_FSLOW_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->Ffast_sel:0x44[18:15],TRNG setting,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_ffast_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_FFAST_SEL_POS) & SYS_ANA_REG4_FFAST_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_ffast_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_FFAST_SEL_MASK << SYS_ANA_REG4_FFAST_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG4_FFAST_SEL_MASK) << SYS_ANA_REG4_FFAST_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->trng_tstck_sel:0x44[19],TRNG setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_trng_tstck_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_TRNG_TSTCK_SEL_POS) & SYS_ANA_REG4_TRNG_TSTCK_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_trng_tstck_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_TRNG_TSTCK_SEL_MASK << SYS_ANA_REG4_TRNG_TSTCK_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG4_TRNG_TSTCK_SEL_MASK) << SYS_ANA_REG4_TRNG_TSTCK_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->cktst_sel:0x44[21:20],clock test signal selection rosc/xtall/dco/dpll,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_cktst_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_CKTST_SEL_POS) & SYS_ANA_REG4_CKTST_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_cktst_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_CKTST_SEL_MASK << SYS_ANA_REG4_CKTST_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG4_CKTST_SEL_MASK) << SYS_ANA_REG4_CKTST_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->ck_tst_enbale:0x44[22],system clock test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_ck_tst_enbale(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_CK_TST_ENBALE_POS) & SYS_ANA_REG4_CK_TST_ENBALE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_ck_tst_enbale(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_CK_TST_ENBALE_MASK << SYS_ANA_REG4_CK_TST_ENBALE_POS);
    reg_value |= ((value & SYS_ANA_REG4_CK_TST_ENBALE_MASK) << SYS_ANA_REG4_CK_TST_ENBALE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->sw_bias:0x44[23],LCD_SW_BIAS,0X1=Short VL3 BIAS ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_sw_bias(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_SW_BIAS_POS) & SYS_ANA_REG4_SW_BIAS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_sw_bias(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_SW_BIAS_MASK << SYS_ANA_REG4_SW_BIAS_POS);
    reg_value |= ((value & SYS_ANA_REG4_SW_BIAS_MASK) << SYS_ANA_REG4_SW_BIAS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->crb:0x44[27:24],LCD_CR<3:0> , 0xF=MAX(vcc) 0X0=min(vcc/2),0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_crb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_CRB_POS) & SYS_ANA_REG4_CRB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_crb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_CRB_MASK << SYS_ANA_REG4_CRB_POS);
    reg_value |= ((value & SYS_ANA_REG4_CRB_MASK) << SYS_ANA_REG4_CRB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG4_ADDR,reg_value);
}

/* REG_0x44:ana_reg4->port_enablel:0x44[31:28],LCD_GPIO<3:0>_ENABLE ,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg4_port_enablel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG4_PORT_ENABLEL_POS) & SYS_ANA_REG4_PORT_ENABLEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg4_port_enablel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG4_ADDR);
    reg_value &= ~(SYS_ANA_REG4_PORT_ENABLEL_MASK << SYS_ANA_REG4_PORT_ENABLEL_POS);
    reg_value |= ((value & SYS_ANA_REG4_PORT_ENABLEL_MASK) << SYS_ANA_REG4_PORT_ENABLEL_POS);
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

/* REG_0x45:ana_reg5->en_usb:0x45[0],usb enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_usb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_USB_POS) & SYS_ANA_REG5_EN_USB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_usb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_USB_MASK << SYS_ANA_REG5_EN_USB_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_USB_MASK) << SYS_ANA_REG5_EN_USB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_xtall:0x45[1],xtall enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_xtall(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_XTALL_POS) & SYS_ANA_REG5_EN_XTALL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_xtall(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_XTALL_MASK << SYS_ANA_REG5_EN_XTALL_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_XTALL_MASK) << SYS_ANA_REG5_EN_XTALL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_dco:0x45[2],dco enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_dco(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_DCO_POS) & SYS_ANA_REG5_EN_DCO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_dco(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_DCO_MASK << SYS_ANA_REG5_EN_DCO_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_DCO_MASK) << SYS_ANA_REG5_EN_DCO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_ram:0x45[3],psram enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_ram(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_RAM_POS) & SYS_ANA_REG5_EN_RAM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_ram(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_RAM_MASK << SYS_ANA_REG5_EN_RAM_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_RAM_MASK) << SYS_ANA_REG5_EN_RAM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_temp:0x45[4],temp_det enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_temp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_TEMP_POS) & SYS_ANA_REG5_EN_TEMP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_temp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_TEMP_MASK << SYS_ANA_REG5_EN_TEMP_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_TEMP_MASK) << SYS_ANA_REG5_EN_TEMP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_dpll:0x45[5],dpll enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_dpll(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_DPLL_POS) & SYS_ANA_REG5_EN_DPLL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_dpll(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_DPLL_MASK << SYS_ANA_REG5_EN_DPLL_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_DPLL_MASK) << SYS_ANA_REG5_EN_DPLL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_cb:0x45[6],CB enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_cb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_CB_POS) & SYS_ANA_REG5_EN_CB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_cb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_CB_MASK << SYS_ANA_REG5_EN_CB_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_CB_MASK) << SYS_ANA_REG5_EN_CB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->en_lcd:0x45[7],LCD enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_en_lcd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_EN_LCD_POS) & SYS_ANA_REG5_EN_LCD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_en_lcd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_EN_LCD_MASK << SYS_ANA_REG5_EN_LCD_POS);
    reg_value |= ((value & SYS_ANA_REG5_EN_LCD_MASK) << SYS_ANA_REG5_EN_LCD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->trxspi_ctrl:0x45[9:8],trx spi voltage selection,3,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_trxspi_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_TRXSPI_CTRL_POS) & SYS_ANA_REG5_TRXSPI_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_trxspi_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_TRXSPI_CTRL_MASK << SYS_ANA_REG5_TRXSPI_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG5_TRXSPI_CTRL_MASK) << SYS_ANA_REG5_TRXSPI_CTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->adc_div:0x45[11:10],gadc input divider selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_adc_div(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_ADC_DIV_POS) & SYS_ANA_REG5_ADC_DIV_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_adc_div(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_ADC_DIV_MASK << SYS_ANA_REG5_ADC_DIV_POS);
    reg_value |= ((value & SYS_ANA_REG5_ADC_DIV_MASK) << SYS_ANA_REG5_ADC_DIV_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->usb_speed:0x45[12],usb speed selection,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_usb_speed(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_USB_SPEED_POS) & SYS_ANA_REG5_USB_SPEED_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_usb_speed(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_USB_SPEED_MASK << SYS_ANA_REG5_USB_SPEED_POS);
    reg_value |= ((value & SYS_ANA_REG5_USB_SPEED_MASK) << SYS_ANA_REG5_USB_SPEED_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->spiDeepSleep:0x45[13],usb deepsleep enable,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_spideepsleep(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_SPIDEEPSLEEP_POS) & SYS_ANA_REG5_SPIDEEPSLEEP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_spideepsleep(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_SPIDEEPSLEEP_MASK << SYS_ANA_REG5_SPIDEEPSLEEP_POS);
    reg_value |= ((value & SYS_ANA_REG5_SPIDEEPSLEEP_MASK) << SYS_ANA_REG5_SPIDEEPSLEEP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->vsel:0x45[14],psram ldo mode output voltage setting,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_VSEL_POS) & SYS_ANA_REG5_VSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_vsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_VSEL_MASK << SYS_ANA_REG5_VSEL_POS);
    reg_value |= ((value & SYS_ANA_REG5_VSEL_MASK) << SYS_ANA_REG5_VSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->swb:0x45[15],psram ldo mode ,1,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_swb(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_SWB_POS) & SYS_ANA_REG5_SWB_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_swb(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_SWB_MASK << SYS_ANA_REG5_SWB_POS);
    reg_value |= ((value & SYS_ANA_REG5_SWB_MASK) << SYS_ANA_REG5_SWB_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->itune_xtall:0x45[19:16],xtall bias current selection,7,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_itune_xtall(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_ITUNE_XTALL_POS) & SYS_ANA_REG5_ITUNE_XTALL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_itune_xtall(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_ITUNE_XTALL_MASK << SYS_ANA_REG5_ITUNE_XTALL_POS);
    reg_value |= ((value & SYS_ANA_REG5_ITUNE_XTALL_MASK) << SYS_ANA_REG5_ITUNE_XTALL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->xtall_ten:0x45[20],xtall test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_xtall_ten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_XTALL_TEN_POS) & SYS_ANA_REG5_XTALL_TEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_xtall_ten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_XTALL_TEN_MASK << SYS_ANA_REG5_XTALL_TEN_POS);
    reg_value |= ((value & SYS_ANA_REG5_XTALL_TEN_MASK) << SYS_ANA_REG5_XTALL_TEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->rosc_tsten:0x45[21],rosc test enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_rosc_tsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_ROSC_TSTEN_POS) & SYS_ANA_REG5_ROSC_TSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_rosc_tsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_ROSC_TSTEN_MASK << SYS_ANA_REG5_ROSC_TSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG5_ROSC_TSTEN_MASK) << SYS_ANA_REG5_ROSC_TSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bcal_start:0x45[22],bias cal start,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bcal_start(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BCAL_START_POS) & SYS_ANA_REG5_BCAL_START_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bcal_start(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BCAL_START_MASK << SYS_ANA_REG5_BCAL_START_POS);
    reg_value |= ((value & SYS_ANA_REG5_BCAL_START_MASK) << SYS_ANA_REG5_BCAL_START_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bcal_en:0x45[23],bias cal enable,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bcal_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BCAL_EN_POS) & SYS_ANA_REG5_BCAL_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bcal_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BCAL_EN_MASK << SYS_ANA_REG5_BCAL_EN_POS);
    reg_value |= ((value & SYS_ANA_REG5_BCAL_EN_MASK) << SYS_ANA_REG5_BCAL_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->bcal_sel:0x45[26:24],bias cal voltage setting,4,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_bcal_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_BCAL_SEL_POS) & SYS_ANA_REG5_BCAL_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_bcal_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_BCAL_SEL_MASK << SYS_ANA_REG5_BCAL_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG5_BCAL_SEL_MASK) << SYS_ANA_REG5_BCAL_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG5_ADDR,reg_value);
}

/* REG_0x45:ana_reg5->vbias:0x45[31:27],bias voltage setting at manul mode ,10,R/W*/
static inline uint32_t sys_ll_get_ana_reg5_vbias(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG5_VBIAS_POS) & SYS_ANA_REG5_VBIAS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg5_vbias(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG5_ADDR);
    reg_value &= ~(SYS_ANA_REG5_VBIAS_MASK << SYS_ANA_REG5_VBIAS_POS);
    reg_value |= ((value & SYS_ANA_REG5_VBIAS_MASK) << SYS_ANA_REG5_VBIAS_POS);
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

/* REG_0x46:ana_reg6->calib_interval:0x46[9:0],MS,100,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_calib_interval(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_CALIB_INTERVAL_POS) & SYS_ANA_REG6_CALIB_INTERVAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_calib_interval(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_CALIB_INTERVAL_MASK << SYS_ANA_REG6_CALIB_INTERVAL_POS);
    reg_value |= ((value & SYS_ANA_REG6_CALIB_INTERVAL_MASK) << SYS_ANA_REG6_CALIB_INTERVAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->modify_interval:0x46[15:10],MS,40,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_modify_interval(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_MODIFY_INTERVAL_POS) & SYS_ANA_REG6_MODIFY_INTERVAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_modify_interval(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_MODIFY_INTERVAL_MASK << SYS_ANA_REG6_MODIFY_INTERVAL_POS);
    reg_value |= ((value & SYS_ANA_REG6_MODIFY_INTERVAL_MASK) << SYS_ANA_REG6_MODIFY_INTERVAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->xtal_wakeup_time:0x46[19:16],MS,8,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_xtal_wakeup_time(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_XTAL_WAKEUP_TIME_POS) & SYS_ANA_REG6_XTAL_WAKEUP_TIME_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_xtal_wakeup_time(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_XTAL_WAKEUP_TIME_MASK << SYS_ANA_REG6_XTAL_WAKEUP_TIME_POS);
    reg_value |= ((value & SYS_ANA_REG6_XTAL_WAKEUP_TIME_MASK) << SYS_ANA_REG6_XTAL_WAKEUP_TIME_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->spi_trig:0x46[20],MS,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_spi_trig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_SPI_TRIG_POS) & SYS_ANA_REG6_SPI_TRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_spi_trig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_SPI_TRIG_MASK << SYS_ANA_REG6_SPI_TRIG_POS);
    reg_value |= ((value & SYS_ANA_REG6_SPI_TRIG_MASK) << SYS_ANA_REG6_SPI_TRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->modifi_auto:0x46[21],MS,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_modifi_auto(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_MODIFI_AUTO_POS) & SYS_ANA_REG6_MODIFI_AUTO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_modifi_auto(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_MODIFI_AUTO_MASK << SYS_ANA_REG6_MODIFI_AUTO_POS);
    reg_value |= ((value & SYS_ANA_REG6_MODIFI_AUTO_MASK) << SYS_ANA_REG6_MODIFI_AUTO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->calib_auto:0x46[22],MS,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_calib_auto(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_CALIB_AUTO_POS) & SYS_ANA_REG6_CALIB_AUTO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_calib_auto(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_CALIB_AUTO_MASK << SYS_ANA_REG6_CALIB_AUTO_POS);
    reg_value |= ((value & SYS_ANA_REG6_CALIB_AUTO_MASK) << SYS_ANA_REG6_CALIB_AUTO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->cal_mode:0x46[23],MS,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_cal_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_CAL_MODE_POS) & SYS_ANA_REG6_CAL_MODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_cal_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_CAL_MODE_MASK << SYS_ANA_REG6_CAL_MODE_POS);
    reg_value |= ((value & SYS_ANA_REG6_CAL_MODE_MASK) << SYS_ANA_REG6_CAL_MODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->manu_ena:0x46[24],MS,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_manu_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_MANU_ENA_POS) & SYS_ANA_REG6_MANU_ENA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_manu_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_MANU_ENA_MASK << SYS_ANA_REG6_MANU_ENA_POS);
    reg_value |= ((value & SYS_ANA_REG6_MANU_ENA_MASK) << SYS_ANA_REG6_MANU_ENA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG6_ADDR,reg_value);
}

/* REG_0x46:ana_reg6->manu_cin:0x46[31:25],MS,40,R/W*/
static inline uint32_t sys_ll_get_ana_reg6_manu_cin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG6_MANU_CIN_POS) & SYS_ANA_REG6_MANU_CIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg6_manu_cin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG6_ADDR);
    reg_value &= ~(SYS_ANA_REG6_MANU_CIN_MASK << SYS_ANA_REG6_MANU_CIN_POS);
    reg_value |= ((value & SYS_ANA_REG6_MANU_CIN_MASK) << SYS_ANA_REG6_MANU_CIN_POS);
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

/* REG_0x47:ana_reg7->port_enablel:0x47[31:0],LCD_GPIO<35:4>_ENABLE ,0x0000,R/W*/
static inline uint32_t sys_ll_get_ana_reg7_port_enablel(void)
{
    return REG_READ(SYS_ANA_REG7_ADDR);
}

static inline void sys_ll_set_ana_reg7_port_enablel(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG7_ADDR,value);
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

/* REG_0x48:ana_reg8->ioldo_lp:0x48[0],0: ioldo high power mode; 1: ioldo low power mode,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_ioldo_lp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_IOLDO_LP_POS) & SYS_ANA_REG8_IOLDO_LP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_ioldo_lp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_IOLDO_LP_MASK << SYS_ANA_REG8_IOLDO_LP_POS);
    reg_value |= ((value & SYS_ANA_REG8_IOLDO_LP_MASK) << SYS_ANA_REG8_IOLDO_LP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->coreldo_hp:0x48[1],0:coreldo low power mode; 1:coreldo high power mode,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_coreldo_hp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_CORELDO_HP_POS) & SYS_ANA_REG8_CORELDO_HP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_coreldo_hp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_CORELDO_HP_MASK << SYS_ANA_REG8_CORELDO_HP_POS);
    reg_value |= ((value & SYS_ANA_REG8_CORELDO_HP_MASK) << SYS_ANA_REG8_CORELDO_HP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->dldohp:0x48[2],0:digldo low power mode; 1:digldo high power mode,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_dldohp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_DLDOHP_POS) & SYS_ANA_REG8_DLDOHP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_dldohp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_DLDOHP_MASK << SYS_ANA_REG8_DLDOHP_POS);
    reg_value |= ((value & SYS_ANA_REG8_DLDOHP_MASK) << SYS_ANA_REG8_DLDOHP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->t_vanaldosel:0x48[5:3],000:1.25V；001:1.3V；…111:1.6V     +50mV for bucka,0x4,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_t_vanaldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_T_VANALDOSEL_POS) & SYS_ANA_REG8_T_VANALDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_t_vanaldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_T_VANALDOSEL_MASK << SYS_ANA_REG8_T_VANALDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG8_T_VANALDOSEL_MASK) << SYS_ANA_REG8_T_VANALDOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->r_vanaldosel:0x48[8:6],000:1.25V；001:1.3V；…111:1.6V     +50mV for bucka,0x4,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_r_vanaldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_R_VANALDOSEL_POS) & SYS_ANA_REG8_R_VANALDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_r_vanaldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_R_VANALDOSEL_MASK << SYS_ANA_REG8_R_VANALDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG8_R_VANALDOSEL_MASK) << SYS_ANA_REG8_R_VANALDOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->en_trsw:0x48[9],0: analdo trsw control disable; 1: analdo trsw control enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_en_trsw(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_EN_TRSW_POS) & SYS_ANA_REG8_EN_TRSW_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_en_trsw(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_EN_TRSW_MASK << SYS_ANA_REG8_EN_TRSW_POS);
    reg_value |= ((value & SYS_ANA_REG8_EN_TRSW_MASK) << SYS_ANA_REG8_EN_TRSW_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->aldohp:0x48[10],0: analdo lowpower mode; 1: analdo highpower mode,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_aldohp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_ALDOHP_POS) & SYS_ANA_REG8_ALDOHP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_aldohp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_ALDOHP_MASK << SYS_ANA_REG8_ALDOHP_POS);
    reg_value |= ((value & SYS_ANA_REG8_ALDOHP_MASK) << SYS_ANA_REG8_ALDOHP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->anacurlim:0x48[11],0: analdo current limit disable; 1: analdo current limit enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_anacurlim(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_ANACURLIM_POS) & SYS_ANA_REG8_ANACURLIM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_anacurlim(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_ANACURLIM_MASK << SYS_ANA_REG8_ANACURLIM_POS);
    reg_value |= ((value & SYS_ANA_REG8_ANACURLIM_MASK) << SYS_ANA_REG8_ANACURLIM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->violdosel:0x48[14:12],000:2.9V；001:3.0V；…111:3.6V,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_violdosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_VIOLDOSEL_POS) & SYS_ANA_REG8_VIOLDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_violdosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_VIOLDOSEL_MASK << SYS_ANA_REG8_VIOLDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG8_VIOLDOSEL_MASK) << SYS_ANA_REG8_VIOLDOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->iocurlim:0x48[15],0: ioldo current limit disable; 1: ioldo current limit enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_iocurlim(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_IOCURLIM_POS) & SYS_ANA_REG8_IOCURLIM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_iocurlim(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_IOCURLIM_MASK << SYS_ANA_REG8_IOCURLIM_POS);
    reg_value |= ((value & SYS_ANA_REG8_IOCURLIM_MASK) << SYS_ANA_REG8_IOCURLIM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->valoldosel:0x48[18:16],0:0.6V；1:0.65V；2:0.7V；3:0.75V; 4:0.8V；5:0.85V；6:0.9V；7:0.95V,0x6,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_valoldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_VALOLDOSEL_POS) & SYS_ANA_REG8_VALOLDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_valoldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_VALOLDOSEL_MASK << SYS_ANA_REG8_VALOLDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG8_VALOLDOSEL_MASK) << SYS_ANA_REG8_VALOLDOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->alopowsel:0x48[19],0: aloldo power select vddio; 1: aloldo power select vddana,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_alopowsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_ALOPOWSEL_POS) & SYS_ANA_REG8_ALOPOWSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_alopowsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_ALOPOWSEL_MASK << SYS_ANA_REG8_ALOPOWSEL_POS);
    reg_value |= ((value & SYS_ANA_REG8_ALOPOWSEL_MASK) << SYS_ANA_REG8_ALOPOWSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->en_fast_aloldo:0x48[20],0: aloldo fast response disable; 1: aloldo fast response enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_en_fast_aloldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_EN_FAST_ALOLDO_POS) & SYS_ANA_REG8_EN_FAST_ALOLDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_en_fast_aloldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_EN_FAST_ALOLDO_MASK << SYS_ANA_REG8_EN_FAST_ALOLDO_POS);
    reg_value |= ((value & SYS_ANA_REG8_EN_FAST_ALOLDO_MASK) << SYS_ANA_REG8_EN_FAST_ALOLDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->aloldohp:0x48[21],0: aloldo lowpower mode; 1: aloldo highpower mode,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_aloldohp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_ALOLDOHP_POS) & SYS_ANA_REG8_ALOLDOHP_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_aloldohp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_ALOLDOHP_MASK << SYS_ANA_REG8_ALOLDOHP_POS);
    reg_value |= ((value & SYS_ANA_REG8_ALOLDOHP_MASK) << SYS_ANA_REG8_ALOLDOHP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->bgcal:0x48[27:22],vbg voltage manual control,0x1F,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_bgcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_BGCAL_POS) & SYS_ANA_REG8_BGCAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_bgcal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_BGCAL_MASK << SYS_ANA_REG8_BGCAL_POS);
    reg_value |= ((value & SYS_ANA_REG8_BGCAL_MASK) << SYS_ANA_REG8_BGCAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->vbgcalmode:0x48[28],0: vbg calibration ; 1: vbg calivration,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_vbgcalmode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_VBGCALMODE_POS) & SYS_ANA_REG8_VBGCALMODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_vbgcalmode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_VBGCALMODE_MASK << SYS_ANA_REG8_VBGCALMODE_POS);
    reg_value |= ((value & SYS_ANA_REG8_VBGCALMODE_MASK) << SYS_ANA_REG8_VBGCALMODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->vbgcalstart:0x48[29],vbg calibration trigger signal,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_vbgcalstart(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_VBGCALSTART_POS) & SYS_ANA_REG8_VBGCALSTART_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_vbgcalstart(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_VBGCALSTART_MASK << SYS_ANA_REG8_VBGCALSTART_POS);
    reg_value |= ((value & SYS_ANA_REG8_VBGCALSTART_MASK) << SYS_ANA_REG8_VBGCALSTART_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->pwd_bgcal:0x48[30],0: vbg calibration power on; 1: vbg calivration power down,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_pwd_bgcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_PWD_BGCAL_POS) & SYS_ANA_REG8_PWD_BGCAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_pwd_bgcal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_PWD_BGCAL_MASK << SYS_ANA_REG8_PWD_BGCAL_POS);
    reg_value |= ((value & SYS_ANA_REG8_PWD_BGCAL_MASK) << SYS_ANA_REG8_PWD_BGCAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG8_ADDR,reg_value);
}

/* REG_0x48:ana_reg8->spi_envbg:0x48[31],shunt down mode force bandgap enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg8_spi_envbg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG8_SPI_ENVBG_POS) & SYS_ANA_REG8_SPI_ENVBG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg8_spi_envbg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG8_ADDR);
    reg_value &= ~(SYS_ANA_REG8_SPI_ENVBG_MASK << SYS_ANA_REG8_SPI_ENVBG_POS);
    reg_value |= ((value & SYS_ANA_REG8_SPI_ENVBG_MASK) << SYS_ANA_REG8_SPI_ENVBG_POS);
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

/* REG_0x49:ana_reg9->wkgpiosel1:0x49[3:0],shuntdown wakeup GPIO select;,0x4,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_wkgpiosel1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_WKGPIOSEL1_POS) & SYS_ANA_REG9_WKGPIOSEL1_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_wkgpiosel1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_WKGPIOSEL1_MASK << SYS_ANA_REG9_WKGPIOSEL1_POS);
    reg_value |= ((value & SYS_ANA_REG9_WKGPIOSEL1_MASK) << SYS_ANA_REG9_WKGPIOSEL1_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->rst_wks1v:0x49[4],rest source flag;,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_rst_wks1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_RST_WKS1V_POS) & SYS_ANA_REG9_RST_WKS1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_rst_wks1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_RST_WKS1V_MASK << SYS_ANA_REG9_RST_WKS1V_POS);
    reg_value |= ((value & SYS_ANA_REG9_RST_WKS1V_MASK) << SYS_ANA_REG9_RST_WKS1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->wkgpiosel2:0x49[8:5],shuntdown wakeup GPIO select;,0x5,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_wkgpiosel2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_WKGPIOSEL2_POS) & SYS_ANA_REG9_WKGPIOSEL2_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_wkgpiosel2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_WKGPIOSEL2_MASK << SYS_ANA_REG9_WKGPIOSEL2_POS);
    reg_value |= ((value & SYS_ANA_REG9_WKGPIOSEL2_MASK) << SYS_ANA_REG9_WKGPIOSEL2_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->spi_latch1v:0x49[9],spi latch control,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_spi_latch1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_SPI_LATCH1V_POS) & SYS_ANA_REG9_SPI_LATCH1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_spi_latch1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_SPI_LATCH1V_MASK << SYS_ANA_REG9_SPI_LATCH1V_POS);
    reg_value |= ((value & SYS_ANA_REG9_SPI_LATCH1V_MASK) << SYS_ANA_REG9_SPI_LATCH1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->digcurlim:0x49[10],Rset source flag clear,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_digcurlim(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_DIGCURLIM_POS) & SYS_ANA_REG9_DIGCURLIM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_digcurlim(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_DIGCURLIM_MASK << SYS_ANA_REG9_DIGCURLIM_POS);
    reg_value |= ((value & SYS_ANA_REG9_DIGCURLIM_MASK) << SYS_ANA_REG9_DIGCURLIM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->pupres_enb1v:0x49[11],GPIO pull up resistor control;,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_pupres_enb1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_PUPRES_ENB1V_POS) & SYS_ANA_REG9_PUPRES_ENB1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_pupres_enb1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_PUPRES_ENB1V_MASK << SYS_ANA_REG9_PUPRES_ENB1V_POS);
    reg_value |= ((value & SYS_ANA_REG9_PUPRES_ENB1V_MASK) << SYS_ANA_REG9_PUPRES_ENB1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->pdnres_en1v:0x49[12],GPIO pull down resistor control;,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_pdnres_en1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_PDNRES_EN1V_POS) & SYS_ANA_REG9_PDNRES_EN1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_pdnres_en1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_PDNRES_EN1V_MASK << SYS_ANA_REG9_PDNRES_EN1V_POS);
    reg_value |= ((value & SYS_ANA_REG9_PDNRES_EN1V_MASK) << SYS_ANA_REG9_PDNRES_EN1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->d_veasel1v:0x49[14:13],buckd EA bias setting;,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_d_veasel1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_D_VEASEL1V_POS) & SYS_ANA_REG9_D_VEASEL1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_d_veasel1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_D_VEASEL1V_MASK << SYS_ANA_REG9_D_VEASEL1V_POS);
    reg_value |= ((value & SYS_ANA_REG9_D_VEASEL1V_MASK) << SYS_ANA_REG9_D_VEASEL1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->ensfsdd:0x49[15],0:digldo softstart disable; 1:digldo softstart enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_ensfsdd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_ENSFSDD_POS) & SYS_ANA_REG9_ENSFSDD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_ensfsdd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_ENSFSDD_MASK << SYS_ANA_REG9_ENSFSDD_POS);
    reg_value |= ((value & SYS_ANA_REG9_ENSFSDD_MASK) << SYS_ANA_REG9_ENSFSDD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vcorehsel:0x49[19:16],0:0.6V；...F:0.975V； step:25mV,0xB,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_vcorehsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VCOREHSEL_POS) & SYS_ANA_REG9_VCOREHSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vcorehsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VCOREHSEL_MASK << SYS_ANA_REG9_VCOREHSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_VCOREHSEL_MASK) << SYS_ANA_REG9_VCOREHSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vcorelsel:0x49[22:20],0:0.45V；...3:0.625V； step:25mV,0x7,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_vcorelsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VCORELSEL_POS) & SYS_ANA_REG9_VCORELSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vcorelsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VCORELSEL_MASK << SYS_ANA_REG9_VCORELSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_VCORELSEL_MASK) << SYS_ANA_REG9_VCORELSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vlden:0x49[23],0: coreldo low voltage disable; 1: coreldo low voltage enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_vlden(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VLDEN_POS) & SYS_ANA_REG9_VLDEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vlden(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VLDEN_MASK << SYS_ANA_REG9_VLDEN_POS);
    reg_value |= ((value & SYS_ANA_REG9_VLDEN_MASK) << SYS_ANA_REG9_VLDEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->en_fast_coreldo:0x49[24],0: digldo fast response disable; 1: digldo fast response enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_en_fast_coreldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_EN_FAST_CORELDO_POS) & SYS_ANA_REG9_EN_FAST_CORELDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_en_fast_coreldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_EN_FAST_CORELDO_MASK << SYS_ANA_REG9_EN_FAST_CORELDO_POS);
    reg_value |= ((value & SYS_ANA_REG9_EN_FAST_CORELDO_MASK) << SYS_ANA_REG9_EN_FAST_CORELDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->pwdcoreldo:0x49[25],0: coreldo poweron; 1: coreldo powerdown,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_pwdcoreldo(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_PWDCORELDO_POS) & SYS_ANA_REG9_PWDCORELDO_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_pwdcoreldo(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_PWDCORELDO_MASK << SYS_ANA_REG9_PWDCORELDO_POS);
    reg_value |= ((value & SYS_ANA_REG9_PWDCORELDO_MASK) << SYS_ANA_REG9_PWDCORELDO_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vdighsel:0x49[28:26],0:0.7V；1:0.75V；2:0.8V；3:0.85V      +50mV for buckd; 4:0.9V；5:0.95V；6:1.0V；7:1.05V,0x6,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_vdighsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VDIGHSEL_POS) & SYS_ANA_REG9_VDIGHSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vdighsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VDIGHSEL_MASK << SYS_ANA_REG9_VDIGHSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_VDIGHSEL_MASK) << SYS_ANA_REG9_VDIGHSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vdigsel:0x49[30:29],00:0.45V；01:0.5V；10:0.55V；11:0.6V    +50mV for buckd,0x3,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_vdigsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VDIGSEL_POS) & SYS_ANA_REG9_VDIGSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vdigsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VDIGSEL_MASK << SYS_ANA_REG9_VDIGSEL_POS);
    reg_value |= ((value & SYS_ANA_REG9_VDIGSEL_MASK) << SYS_ANA_REG9_VDIGSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG9_ADDR,reg_value);
}

/* REG_0x49:ana_reg9->vdd12lden:0x49[31],0: digldo low voltage disable; 1: digldo low voltage enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg9_vdd12lden(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG9_VDD12LDEN_POS) & SYS_ANA_REG9_VDD12LDEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg9_vdd12lden(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG9_ADDR);
    reg_value &= ~(SYS_ANA_REG9_VDD12LDEN_MASK << SYS_ANA_REG9_VDD12LDEN_POS);
    reg_value |= ((value & SYS_ANA_REG9_VDD12LDEN_MASK) << SYS_ANA_REG9_VDD12LDEN_POS);
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

/* REG_0x4a:ana_reg10->pasoft_st:0x4a[3:0],buckpa softstart setting,0x7,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_pasoft_st(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_PASOFT_ST_POS) & SYS_ANA_REG10_PASOFT_ST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_pasoft_st(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_PASOFT_ST_MASK << SYS_ANA_REG10_PASOFT_ST_POS);
    reg_value |= ((value & SYS_ANA_REG10_PASOFT_ST_MASK) << SYS_ANA_REG10_PASOFT_ST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->azcdcnt_manu:0x4a[6:4],bucka zcd cont setting,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_azcdcnt_manu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_AZCDCNT_MANU_POS) & SYS_ANA_REG10_AZCDCNT_MANU_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_azcdcnt_manu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_AZCDCNT_MANU_MASK << SYS_ANA_REG10_AZCDCNT_MANU_POS);
    reg_value |= ((value & SYS_ANA_REG10_AZCDCNT_MANU_MASK) << SYS_ANA_REG10_AZCDCNT_MANU_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->timer_sel:0x4a[10:7],timer wakeup setting,0x7,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_timer_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_TIMER_SEL_POS) & SYS_ANA_REG10_TIMER_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_timer_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_TIMER_SEL_MASK << SYS_ANA_REG10_TIMER_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG10_TIMER_SEL_MASK) << SYS_ANA_REG10_TIMER_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->vpabucksel:0x4a[14:11],buckpa vout setting,0x8,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_vpabucksel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_VPABUCKSEL_POS) & SYS_ANA_REG10_VPABUCKSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_vpabucksel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_VPABUCKSEL_MASK << SYS_ANA_REG10_VPABUCKSEL_POS);
    reg_value |= ((value & SYS_ANA_REG10_VPABUCKSEL_MASK) << SYS_ANA_REG10_VPABUCKSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->spi_timerwken:0x4a[15],timer wakeup enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_spi_timerwken(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_SPI_TIMERWKEN_POS) & SYS_ANA_REG10_SPI_TIMERWKEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_spi_timerwken(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_SPI_TIMERWKEN_MASK << SYS_ANA_REG10_SPI_TIMERWKEN_POS);
    reg_value |= ((value & SYS_ANA_REG10_SPI_TIMERWKEN_MASK) << SYS_ANA_REG10_SPI_TIMERWKEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->sd:0x4a[17],shunt down,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_sd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_SD_POS) & SYS_ANA_REG10_SD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_sd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_SD_MASK << SYS_ANA_REG10_SD_POS);
    reg_value |= ((value & SYS_ANA_REG10_SD_MASK) << SYS_ANA_REG10_SD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->ioldosel:0x4a[18],0: ioldo disable/buckpa enable; 1: ioldo enable/buckpa disable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_ioldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_IOLDOSEL_POS) & SYS_ANA_REG10_IOLDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_ioldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_IOLDOSEL_MASK << SYS_ANA_REG10_IOLDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG10_IOLDOSEL_MASK) << SYS_ANA_REG10_IOLDOSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->iobyapssen:0x4a[19],ioldo bypass enable;,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_iobyapssen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_IOBYAPSSEN_POS) & SYS_ANA_REG10_IOBYAPSSEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_iobyapssen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_IOBYAPSSEN_MASK << SYS_ANA_REG10_IOBYAPSSEN_POS);
    reg_value |= ((value & SYS_ANA_REG10_IOBYAPSSEN_MASK) << SYS_ANA_REG10_IOBYAPSSEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->ckfs:0x4a[21:20],buck output clock freq. select   0--500k 1---1M  2--2M  3--4M,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_ckfs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_CKFS_POS) & SYS_ANA_REG10_CKFS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_ckfs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_CKFS_MASK << SYS_ANA_REG10_CKFS_POS);
    reg_value |= ((value & SYS_ANA_REG10_CKFS_MASK) << SYS_ANA_REG10_CKFS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->ckintsel:0x4a[22],buck clock source select  1-- ring oscillator   0--divider,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_ckintsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_CKINTSEL_POS) & SYS_ANA_REG10_CKINTSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_ckintsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_CKINTSEL_MASK << SYS_ANA_REG10_CKINTSEL_POS);
    reg_value |= ((value & SYS_ANA_REG10_CKINTSEL_MASK) << SYS_ANA_REG10_CKINTSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->osccaltrig:0x4a[23],buck oscillator auto cali. triggle( 0--> 1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_osccaltrig(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_OSCCALTRIG_POS) & SYS_ANA_REG10_OSCCALTRIG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_osccaltrig(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_OSCCALTRIG_MASK << SYS_ANA_REG10_OSCCALTRIG_POS);
    reg_value |= ((value & SYS_ANA_REG10_OSCCALTRIG_MASK) << SYS_ANA_REG10_OSCCALTRIG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->mroscsel:0x4a[24],buck oscillator manual cali. enable(=1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_mroscsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_MROSCSEL_POS) & SYS_ANA_REG10_MROSCSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_mroscsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_MROSCSEL_MASK << SYS_ANA_REG10_MROSCSEL_POS);
    reg_value |= ((value & SYS_ANA_REG10_MROSCSEL_MASK) << SYS_ANA_REG10_MROSCSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->mrosci_cal:0x4a[27:25],buck oscillator manual fsel  0x0---500k 0x1--1M 0x2---2M,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_mrosci_cal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_MROSCI_CAL_POS) & SYS_ANA_REG10_MROSCI_CAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_mrosci_cal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_MROSCI_CAL_MASK << SYS_ANA_REG10_MROSCI_CAL_POS);
    reg_value |= ((value & SYS_ANA_REG10_MROSCI_CAL_MASK) << SYS_ANA_REG10_MROSCI_CAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG10_ADDR,reg_value);
}

/* REG_0x4a:ana_reg10->mrosccap_cal:0x4a[31:28],buck oscillator manual cap_cal  0xA---500k 0xB--1M 0x9---2M,0x6,W/R*/
static inline uint32_t sys_ll_get_ana_reg10_mrosccap_cal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG10_MROSCCAP_CAL_POS) & SYS_ANA_REG10_MROSCCAP_CAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg10_mrosccap_cal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG10_ADDR);
    reg_value &= ~(SYS_ANA_REG10_MROSCCAP_CAL_MASK << SYS_ANA_REG10_MROSCCAP_CAL_POS);
    reg_value |= ((value & SYS_ANA_REG10_MROSCCAP_CAL_MASK) << SYS_ANA_REG10_MROSCCAP_CAL_POS);
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

/* REG_0x4b:ana_reg11->sfsr:0x4b[3:0],softstart time setting,0:max；F:min,0xA,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_sfsr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_SFSR_POS) & SYS_ANA_REG11_SFSR_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_sfsr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_SFSR_MASK << SYS_ANA_REG11_SFSR_POS);
    reg_value |= ((value & SYS_ANA_REG11_SFSR_MASK) << SYS_ANA_REG11_SFSR_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->ensfsaa:0x4b[4],0:analdo softstart disable; 1:analdo softstart enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_ensfsaa(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ENSFSAA_POS) & SYS_ANA_REG11_ENSFSAA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ensfsaa(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ENSFSAA_MASK << SYS_ANA_REG11_ENSFSAA_POS);
    reg_value |= ((value & SYS_ANA_REG11_ENSFSAA_MASK) << SYS_ANA_REG11_ENSFSAA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->apfms:0x4b[9:5],bucka pfm-pwm mode shift control threshold,  higher value, higher mode shift loading needed,0x7,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_apfms(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_APFMS_POS) & SYS_ANA_REG11_APFMS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_apfms(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_APFMS_MASK << SYS_ANA_REG11_APFMS_POS);
    reg_value |= ((value & SYS_ANA_REG11_APFMS_MASK) << SYS_ANA_REG11_APFMS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->atmpo_sel:0x4b[11:10],bucka mpo pulse width control 0--shortest   3---longest,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_atmpo_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ATMPO_SEL_POS) & SYS_ANA_REG11_ATMPO_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_atmpo_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ATMPO_SEL_MASK << SYS_ANA_REG11_ATMPO_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG11_ATMPO_SEL_MASK) << SYS_ANA_REG11_ATMPO_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->ampoen:0x4b[12],bucka mpo mode enable( =1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_ampoen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_AMPOEN_POS) & SYS_ANA_REG11_AMPOEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_ampoen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_AMPOEN_MASK << SYS_ANA_REG11_AMPOEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_AMPOEN_MASK) << SYS_ANA_REG11_AMPOEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->enpowa:0x4b[13],bucka EA fast transient enable(=1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_enpowa(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ENPOWA_POS) & SYS_ANA_REG11_ENPOWA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_enpowa(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ENPOWA_MASK << SYS_ANA_REG11_ENPOWA_POS);
    reg_value |= ((value & SYS_ANA_REG11_ENPOWA_MASK) << SYS_ANA_REG11_ENPOWA_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->avea_sel:0x4b[15:14],bucka EA bias setting,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_avea_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_AVEA_SEL_POS) & SYS_ANA_REG11_AVEA_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_avea_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_AVEA_SEL_MASK << SYS_ANA_REG11_AVEA_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG11_AVEA_SEL_MASK) << SYS_ANA_REG11_AVEA_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->aforcepfm:0x4b[16],bucka force pfm mode(=1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_aforcepfm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_AFORCEPFM_POS) & SYS_ANA_REG11_AFORCEPFM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_aforcepfm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_AFORCEPFM_MASK << SYS_ANA_REG11_AFORCEPFM_POS);
    reg_value |= ((value & SYS_ANA_REG11_AFORCEPFM_MASK) << SYS_ANA_REG11_AFORCEPFM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->acls:0x4b[19:17],bucka current limit setting,0x5,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_acls(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ACLS_POS) & SYS_ANA_REG11_ACLS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_acls(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ACLS_MASK << SYS_ANA_REG11_ACLS_POS);
    reg_value |= ((value & SYS_ANA_REG11_ACLS_MASK) << SYS_ANA_REG11_ACLS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->aswrsten:0x4b[20],bucka freewheeling damping enable(=1) ,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_aswrsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ASWRSTEN_POS) & SYS_ANA_REG11_ASWRSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_aswrsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ASWRSTEN_MASK << SYS_ANA_REG11_ASWRSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_ASWRSTEN_MASK) << SYS_ANA_REG11_ASWRSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->aripc:0x4b[23:21],bucka pfm mode voltage ripple control setting,0x3,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_aripc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ARIPC_POS) & SYS_ANA_REG11_ARIPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_aripc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ARIPC_MASK << SYS_ANA_REG11_ARIPC_POS);
    reg_value |= ((value & SYS_ANA_REG11_ARIPC_MASK) << SYS_ANA_REG11_ARIPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->arampc:0x4b[27:24],bucka ramping compensation setting,0x4,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_arampc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ARAMPC_POS) & SYS_ANA_REG11_ARAMPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_arampc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ARAMPC_MASK << SYS_ANA_REG11_ARAMPC_POS);
    reg_value |= ((value & SYS_ANA_REG11_ARAMPC_MASK) << SYS_ANA_REG11_ARAMPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->arampcen:0x4b[28],bucka ramping compensation enable(=1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_arampcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ARAMPCEN_POS) & SYS_ANA_REG11_ARAMPCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_arampcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ARAMPCEN_MASK << SYS_ANA_REG11_ARAMPCEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_ARAMPCEN_MASK) << SYS_ANA_REG11_ARAMPCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->aenburst:0x4b[29],bucka burst mode enable(=1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_aenburst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_AENBURST_POS) & SYS_ANA_REG11_AENBURST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_aenburst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_AENBURST_MASK << SYS_ANA_REG11_AENBURST_POS);
    reg_value |= ((value & SYS_ANA_REG11_AENBURST_MASK) << SYS_ANA_REG11_AENBURST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->apfmen:0x4b[30],bucka pfm mode enable(=1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_apfmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_APFMEN_POS) & SYS_ANA_REG11_APFMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_apfmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_APFMEN_MASK << SYS_ANA_REG11_APFMEN_POS);
    reg_value |= ((value & SYS_ANA_REG11_APFMEN_MASK) << SYS_ANA_REG11_APFMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG11_ADDR,reg_value);
}

/* REG_0x4b:ana_reg11->aldosel:0x4b[31],0: analdo disable/buckA enable; 1: analdo enable/buckA disable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg11_aldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG11_ALDOSEL_POS) & SYS_ANA_REG11_ALDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg11_aldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG11_ADDR);
    reg_value &= ~(SYS_ANA_REG11_ALDOSEL_MASK << SYS_ANA_REG11_ALDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG11_ALDOSEL_MASK) << SYS_ANA_REG11_ALDOSEL_POS);
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

/* REG_0x4c:ana_reg12->buckd_softst:0x4c[3:0],buckd softstart setting,0x0A,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_buckd_softst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_BUCKD_SOFTST_POS) & SYS_ANA_REG12_BUCKD_SOFTST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_buckd_softst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_BUCKD_SOFTST_MASK << SYS_ANA_REG12_BUCKD_SOFTST_POS);
    reg_value |= ((value & SYS_ANA_REG12_BUCKD_SOFTST_MASK) << SYS_ANA_REG12_BUCKD_SOFTST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dzcdcnt_manu:0x4c[6:4],buckd zcd cont setting,0x6,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dzcdcnt_manu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DZCDCNT_MANU_POS) & SYS_ANA_REG12_DZCDCNT_MANU_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dzcdcnt_manu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DZCDCNT_MANU_MASK << SYS_ANA_REG12_DZCDCNT_MANU_POS);
    reg_value |= ((value & SYS_ANA_REG12_DZCDCNT_MANU_MASK) << SYS_ANA_REG12_DZCDCNT_MANU_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->clk_sel:0x4c[7],timer clk select,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_clk_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_CLK_SEL_POS) & SYS_ANA_REG12_CLK_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_clk_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_CLK_SEL_MASK << SYS_ANA_REG12_CLK_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG12_CLK_SEL_MASK) << SYS_ANA_REG12_CLK_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dpfms:0x4c[12:8],buckd pfm-pwm mode shift control threshold,  higher value, higher mode shift loading needed,0x3,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dpfms(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DPFMS_POS) & SYS_ANA_REG12_DPFMS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dpfms(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DPFMS_MASK << SYS_ANA_REG12_DPFMS_POS);
    reg_value |= ((value & SYS_ANA_REG12_DPFMS_MASK) << SYS_ANA_REG12_DPFMS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dtmpo_sel:0x4c[14:13],buckd mpo pulse width control 0--shortest   3---longest,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dtmpo_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DTMPO_SEL_POS) & SYS_ANA_REG12_DTMPO_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dtmpo_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DTMPO_SEL_MASK << SYS_ANA_REG12_DTMPO_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG12_DTMPO_SEL_MASK) << SYS_ANA_REG12_DTMPO_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dmpoen:0x4c[15],buckd mpo mode enable( =1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dmpoen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DMPOEN_POS) & SYS_ANA_REG12_DMPOEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dmpoen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DMPOEN_MASK << SYS_ANA_REG12_DMPOEN_POS);
    reg_value |= ((value & SYS_ANA_REG12_DMPOEN_MASK) << SYS_ANA_REG12_DMPOEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dforcepfm:0x4c[16],buckd force pfm mode(=1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dforcepfm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DFORCEPFM_POS) & SYS_ANA_REG12_DFORCEPFM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dforcepfm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DFORCEPFM_MASK << SYS_ANA_REG12_DFORCEPFM_POS);
    reg_value |= ((value & SYS_ANA_REG12_DFORCEPFM_MASK) << SYS_ANA_REG12_DFORCEPFM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dcls:0x4c[19:17],buckd current limit setting,0x5,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dcls(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DCLS_POS) & SYS_ANA_REG12_DCLS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dcls(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DCLS_MASK << SYS_ANA_REG12_DCLS_POS);
    reg_value |= ((value & SYS_ANA_REG12_DCLS_MASK) << SYS_ANA_REG12_DCLS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dswrsten:0x4c[20],buckd freewheeling damping enable(=1) ,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dswrsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DSWRSTEN_POS) & SYS_ANA_REG12_DSWRSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dswrsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DSWRSTEN_MASK << SYS_ANA_REG12_DSWRSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG12_DSWRSTEN_MASK) << SYS_ANA_REG12_DSWRSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dripc:0x4c[23:21],buckd pfm mode voltage ripple control setting,0x3,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dripc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DRIPC_POS) & SYS_ANA_REG12_DRIPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dripc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DRIPC_MASK << SYS_ANA_REG12_DRIPC_POS);
    reg_value |= ((value & SYS_ANA_REG12_DRIPC_MASK) << SYS_ANA_REG12_DRIPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->drampc:0x4c[27:24],buckd ramping compensation setting,0x4,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_drampc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DRAMPC_POS) & SYS_ANA_REG12_DRAMPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_drampc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DRAMPC_MASK << SYS_ANA_REG12_DRAMPC_POS);
    reg_value |= ((value & SYS_ANA_REG12_DRAMPC_MASK) << SYS_ANA_REG12_DRAMPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->drampcen:0x4c[28],buckd ramping compensation enable(=1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_drampcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DRAMPCEN_POS) & SYS_ANA_REG12_DRAMPCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_drampcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DRAMPCEN_MASK << SYS_ANA_REG12_DRAMPCEN_POS);
    reg_value |= ((value & SYS_ANA_REG12_DRAMPCEN_MASK) << SYS_ANA_REG12_DRAMPCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->denburst:0x4c[29],buckd burst mode enable(=1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_denburst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DENBURST_POS) & SYS_ANA_REG12_DENBURST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_denburst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DENBURST_MASK << SYS_ANA_REG12_DENBURST_POS);
    reg_value |= ((value & SYS_ANA_REG12_DENBURST_MASK) << SYS_ANA_REG12_DENBURST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dpfmen:0x4c[30],buckd pfm mode enable(=1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dpfmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DPFMEN_POS) & SYS_ANA_REG12_DPFMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dpfmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DPFMEN_MASK << SYS_ANA_REG12_DPFMEN_POS);
    reg_value |= ((value & SYS_ANA_REG12_DPFMEN_MASK) << SYS_ANA_REG12_DPFMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG12_ADDR,reg_value);
}

/* REG_0x4c:ana_reg12->dldosel:0x4c[31],0: digldo disable/buckd enable; 1: digldo enable/buckd disable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg12_dldosel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG12_DLDOSEL_POS) & SYS_ANA_REG12_DLDOSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg12_dldosel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG12_ADDR);
    reg_value &= ~(SYS_ANA_REG12_DLDOSEL_MASK << SYS_ANA_REG12_DLDOSEL_POS);
    reg_value |= ((value & SYS_ANA_REG12_DLDOSEL_MASK) << SYS_ANA_REG12_DLDOSEL_POS);
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

/* REG_0x4d:ana_reg13->pwdovp1v:0x4d[0],ovp powerdown control,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_pwdovp1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_PWDOVP1V_POS) & SYS_ANA_REG13_PWDOVP1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_pwdovp1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_PWDOVP1V_MASK << SYS_ANA_REG13_PWDOVP1V_POS);
    reg_value |= ((value & SYS_ANA_REG13_PWDOVP1V_MASK) << SYS_ANA_REG13_PWDOVP1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->asoft_stc:0x4d[4:1],bucka softstart setting,0x7,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_asoft_stc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_ASOFT_STC_POS) & SYS_ANA_REG13_ASOFT_STC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_asoft_stc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_ASOFT_STC_MASK << SYS_ANA_REG13_ASOFT_STC_POS);
    reg_value |= ((value & SYS_ANA_REG13_ASOFT_STC_MASK) << SYS_ANA_REG13_ASOFT_STC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->volen:0x4d[5],buckpa low voltage enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_volen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_VOLEN_POS) & SYS_ANA_REG13_VOLEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_volen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_VOLEN_MASK << SYS_ANA_REG13_VOLEN_POS);
    reg_value |= ((value & SYS_ANA_REG13_VOLEN_MASK) << SYS_ANA_REG13_VOLEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dpfms:0x4d[10:6],buckpa pfm-pwm mode shift control threshold,  higher value, higher mode shift loading needed,0x3,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dpfms(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DPFMS_POS) & SYS_ANA_REG13_DPFMS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dpfms(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DPFMS_MASK << SYS_ANA_REG13_DPFMS_POS);
    reg_value |= ((value & SYS_ANA_REG13_DPFMS_MASK) << SYS_ANA_REG13_DPFMS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dtmpo_sel:0x4d[12:11],buckpa mpo pulse width control 0--shortest   3--longest,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dtmpo_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DTMPO_SEL_POS) & SYS_ANA_REG13_DTMPO_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dtmpo_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DTMPO_SEL_MASK << SYS_ANA_REG13_DTMPO_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG13_DTMPO_SEL_MASK) << SYS_ANA_REG13_DTMPO_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dmpoen:0x4d[13],buckpa mpo mode enable( =1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dmpoen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DMPOEN_POS) & SYS_ANA_REG13_DMPOEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dmpoen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DMPOEN_MASK << SYS_ANA_REG13_DMPOEN_POS);
    reg_value |= ((value & SYS_ANA_REG13_DMPOEN_MASK) << SYS_ANA_REG13_DMPOEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->pavea_sel:0x4d[15:14],buckpa EA bias setting,0x2,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_pavea_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_PAVEA_SEL_POS) & SYS_ANA_REG13_PAVEA_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_pavea_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_PAVEA_SEL_MASK << SYS_ANA_REG13_PAVEA_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG13_PAVEA_SEL_MASK) << SYS_ANA_REG13_PAVEA_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dforcepfm:0x4d[16],buckpa force pfm mode(=1),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dforcepfm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DFORCEPFM_POS) & SYS_ANA_REG13_DFORCEPFM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dforcepfm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DFORCEPFM_MASK << SYS_ANA_REG13_DFORCEPFM_POS);
    reg_value |= ((value & SYS_ANA_REG13_DFORCEPFM_MASK) << SYS_ANA_REG13_DFORCEPFM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dcls:0x4d[19:17],buckpa current limit setting,0x5,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dcls(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DCLS_POS) & SYS_ANA_REG13_DCLS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dcls(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DCLS_MASK << SYS_ANA_REG13_DCLS_POS);
    reg_value |= ((value & SYS_ANA_REG13_DCLS_MASK) << SYS_ANA_REG13_DCLS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dswrsten:0x4d[20],buckpa freewheeling damping enable(=1) ,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dswrsten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DSWRSTEN_POS) & SYS_ANA_REG13_DSWRSTEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dswrsten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DSWRSTEN_MASK << SYS_ANA_REG13_DSWRSTEN_POS);
    reg_value |= ((value & SYS_ANA_REG13_DSWRSTEN_MASK) << SYS_ANA_REG13_DSWRSTEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->dripc:0x4d[23:21],buckpa pfm mode voltage ripple control setting,0x3,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_dripc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DRIPC_POS) & SYS_ANA_REG13_DRIPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_dripc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DRIPC_MASK << SYS_ANA_REG13_DRIPC_POS);
    reg_value |= ((value & SYS_ANA_REG13_DRIPC_MASK) << SYS_ANA_REG13_DRIPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->drampc:0x4d[27:24],buckpa ramping compensation setting,0x4,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_drampc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DRAMPC_POS) & SYS_ANA_REG13_DRAMPC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_drampc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DRAMPC_MASK << SYS_ANA_REG13_DRAMPC_POS);
    reg_value |= ((value & SYS_ANA_REG13_DRAMPC_MASK) << SYS_ANA_REG13_DRAMPC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->drampcen:0x4d[28],buckpa ramping compensation enable(=1),0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_drampcen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_DRAMPCEN_POS) & SYS_ANA_REG13_DRAMPCEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_drampcen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_DRAMPCEN_MASK << SYS_ANA_REG13_DRAMPCEN_POS);
    reg_value |= ((value & SYS_ANA_REG13_DRAMPCEN_MASK) << SYS_ANA_REG13_DRAMPCEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->paenburst:0x4d[29],buckpa burst mode enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_paenburst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_PAENBURST_POS) & SYS_ANA_REG13_PAENBURST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_paenburst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_PAENBURST_MASK << SYS_ANA_REG13_PAENBURST_POS);
    reg_value |= ((value & SYS_ANA_REG13_PAENBURST_MASK) << SYS_ANA_REG13_PAENBURST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->papfmen:0x4d[30],buckpa pfm mode enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_papfmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_PAPFMEN_POS) & SYS_ANA_REG13_PAPFMEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_papfmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_PAPFMEN_MASK << SYS_ANA_REG13_PAPFMEN_POS);
    reg_value |= ((value & SYS_ANA_REG13_PAPFMEN_MASK) << SYS_ANA_REG13_PAPFMEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG13_ADDR,reg_value);
}

/* REG_0x4d:ana_reg13->enbuckpa:0x4d[31],buckpa enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg13_enbuckpa(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG13_ENBUCKPA_POS) & SYS_ANA_REG13_ENBUCKPA_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg13_enbuckpa(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG13_ADDR);
    reg_value &= ~(SYS_ANA_REG13_ENBUCKPA_MASK << SYS_ANA_REG13_ENBUCKPA_POS);
    reg_value |= ((value & SYS_ANA_REG13_ENBUCKPA_MASK) << SYS_ANA_REG13_ENBUCKPA_POS);
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

/* REG_0x4e:ana_reg14->chs:0x4e[15:0],scan扫描模式通道选择，选择哪个通道，哪一位=1。,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg14_chs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG14_CHS_POS) & SYS_ANA_REG14_CHS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg14_chs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_CHS_MASK << SYS_ANA_REG14_CHS_POS);
    reg_value |= ((value & SYS_ANA_REG14_CHS_MASK) << SYS_ANA_REG14_CHS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->en_lpmod:0x4e[16],使能Low power 模式。,0,W*/
static inline void sys_ll_set_ana_reg14_en_lpmod(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_EN_LPMOD_MASK << SYS_ANA_REG14_EN_LPMOD_POS);
    reg_value |= ((value & SYS_ANA_REG14_EN_LPMOD_MASK) << SYS_ANA_REG14_EN_LPMOD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->cal_vth:0x4e[19:17],设置校正阈值大小，0，最容易触发。数值越大，越不容易触发。,3,W*/
static inline void sys_ll_set_ana_reg14_cal_vth(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_CAL_VTH_MASK << SYS_ANA_REG14_CAL_VTH_POS);
    reg_value |= ((value & SYS_ANA_REG14_CAL_VTH_MASK) << SYS_ANA_REG14_CAL_VTH_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->crg:0x4e[21:20],设置校正量程范围。Crg=0，量程最小。,0,W*/
static inline void sys_ll_set_ana_reg14_crg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_CRG_MASK << SYS_ANA_REG14_CRG_POS);
    reg_value |= ((value & SYS_ANA_REG14_CRG_MASK) << SYS_ANA_REG14_CRG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->vrefs:0x4e[25:22],设置touch触发阈值。0最容易触发，数值越大，越不容易触发。,6,W*/
static inline void sys_ll_set_ana_reg14_vrefs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_VREFS_MASK << SYS_ANA_REG14_VREFS_POS);
    reg_value |= ((value & SYS_ANA_REG14_VREFS_MASK) << SYS_ANA_REG14_VREFS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->gain_s:0x4e[29:26],设置touch灵敏度。0最高灵敏度，数值越大，越不灵敏。,3,W*/
static inline void sys_ll_set_ana_reg14_gain_s(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_GAIN_S_MASK << SYS_ANA_REG14_GAIN_S_POS);
    reg_value |= ((value & SYS_ANA_REG14_GAIN_S_MASK) << SYS_ANA_REG14_GAIN_S_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->TD_latch1v:0x4e[30], 0，SPI控制数值被内部锁定，不能写入。1，可以写入。,1,W*/
static inline void sys_ll_set_ana_reg14_td_latch1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_TD_LATCH1V_MASK << SYS_ANA_REG14_TD_LATCH1V_POS);
    reg_value |= ((value & SYS_ANA_REG14_TD_LATCH1V_MASK) << SYS_ANA_REG14_TD_LATCH1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG14_ADDR,reg_value);
}

/* REG_0x4e:ana_reg14->pwd_td:0x4e[31],打开touch模块,1,W*/
static inline void sys_ll_set_ana_reg14_pwd_td(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG14_ADDR);
    reg_value &= ~(SYS_ANA_REG14_PWD_TD_MASK << SYS_ANA_REG14_PWD_TD_POS);
    reg_value |= ((value & SYS_ANA_REG14_PWD_TD_MASK) << SYS_ANA_REG14_PWD_TD_POS);
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

/* REG_0x4f:ana_reg15->cal_number1v:0x4f[3:0],设置校正模式检测环境变化信号个数，多少个输出一个有效信号。,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg15_cal_number1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG15_CAL_NUMBER1V_POS) & SYS_ANA_REG15_CAL_NUMBER1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg15_cal_number1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_CAL_NUMBER1V_MASK << SYS_ANA_REG15_CAL_NUMBER1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_CAL_NUMBER1V_MASK) << SYS_ANA_REG15_CAL_NUMBER1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->cal_period1v:0x4f[12:4],设置校正模式间隔周期大小,0,W*/
static inline void sys_ll_set_ana_reg15_cal_period1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_CAL_PERIOD1V_MASK << SYS_ANA_REG15_CAL_PERIOD1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_CAL_PERIOD1V_MASK) << SYS_ANA_REG15_CAL_PERIOD1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->test_number1v:0x4f[16:13],设置测试模式检测触摸信号个数，多少个输出一个有效信号。,0,W*/
static inline void sys_ll_set_ana_reg15_test_number1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_TEST_NUMBER1V_MASK << SYS_ANA_REG15_TEST_NUMBER1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_TEST_NUMBER1V_MASK) << SYS_ANA_REG15_TEST_NUMBER1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->test_period1v:0x4f[20:17],设置测试模式间隔周期大小,0,W*/
static inline void sys_ll_set_ana_reg15_test_period1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_TEST_PERIOD1V_MASK << SYS_ANA_REG15_TEST_PERIOD1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_TEST_PERIOD1V_MASK) << SYS_ANA_REG15_TEST_PERIOD1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->chs_sel_cal1v:0x4f[25:22],校正模式，通道选择，一次只能选择一个通道校正。,0,W*/
static inline void sys_ll_set_ana_reg15_chs_sel_cal1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_CHS_SEL_CAL1V_MASK << SYS_ANA_REG15_CHS_SEL_CAL1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_CHS_SEL_CAL1V_MASK) << SYS_ANA_REG15_CHS_SEL_CAL1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->cal_done_clr1v:0x4f[26],校正结束标志位清零。,0,W*/
static inline void sys_ll_set_ana_reg15_cal_done_clr1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_CAL_DONE_CLR1V_MASK << SYS_ANA_REG15_CAL_DONE_CLR1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_CAL_DONE_CLR1V_MASK) << SYS_ANA_REG15_CAL_DONE_CLR1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->en_cal_force1v:0x4f[27],使能强制校正模式。,0,W*/
static inline void sys_ll_set_ana_reg15_en_cal_force1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_EN_CAL_FORCE1V_MASK << SYS_ANA_REG15_EN_CAL_FORCE1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_EN_CAL_FORCE1V_MASK) << SYS_ANA_REG15_EN_CAL_FORCE1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->en_cal_auto1v:0x4f[28],使能自动校正模式。,0,W*/
static inline void sys_ll_set_ana_reg15_en_cal_auto1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_EN_CAL_AUTO1V_MASK << SYS_ANA_REG15_EN_CAL_AUTO1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_EN_CAL_AUTO1V_MASK) << SYS_ANA_REG15_EN_CAL_AUTO1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->en_scm:0x4f[29],使能scan扫描模式。,0,W*/
static inline void sys_ll_set_ana_reg15_en_scm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_EN_SCM_MASK << SYS_ANA_REG15_EN_SCM_POS);
    reg_value |= ((value & SYS_ANA_REG15_EN_SCM_MASK) << SYS_ANA_REG15_EN_SCM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->en_adcmod:0x4f[30],使能adc工作模式，需要配合SAR ADC一起使用。,0,W*/
static inline void sys_ll_set_ana_reg15_en_adcmod(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_EN_ADCMOD_MASK << SYS_ANA_REG15_EN_ADCMOD_POS);
    reg_value |= ((value & SYS_ANA_REG15_EN_ADCMOD_MASK) << SYS_ANA_REG15_EN_ADCMOD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG15_ADDR,reg_value);
}

/* REG_0x4f:ana_reg15->enfsr1v:0x4f[31],NA，废弃的位。,0,W*/
static inline void sys_ll_set_ana_reg15_enfsr1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG15_ADDR);
    reg_value &= ~(SYS_ANA_REG15_ENFSR1V_MASK << SYS_ANA_REG15_ENFSR1V_POS);
    reg_value |= ((value & SYS_ANA_REG15_ENFSR1V_MASK) << SYS_ANA_REG15_ENFSR1V_POS);
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

/* REG_0x50:ana_reg16->int_en:0x50[9:0], mult_touch_int_enable，对应[9:0]通道,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg16_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG16_INT_EN_POS) & SYS_ANA_REG16_INT_EN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg16_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_INT_EN_MASK << SYS_ANA_REG16_INT_EN_POS);
    reg_value |= ((value & SYS_ANA_REG16_INT_EN_MASK) << SYS_ANA_REG16_INT_EN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->int_en16:0x50[10], wake_up_soft enable ,0,W*/
static inline void sys_ll_set_ana_reg16_int_en16(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_INT_EN16_MASK << SYS_ANA_REG16_INT_EN16_POS);
    reg_value |= ((value & SYS_ANA_REG16_INT_EN16_MASK) << SYS_ANA_REG16_INT_EN16_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->ckadc_sel:0x50[16],touch工作时钟来源选择。0，来自saradc；1，来自于sigmadelta adc。,0,W*/
static inline void sys_ll_set_ana_reg16_ckadc_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_CKADC_SEL_MASK << SYS_ANA_REG16_CKADC_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG16_CKADC_SEL_MASK) << SYS_ANA_REG16_CKADC_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->int_clr_sel1v:0x50[17],int clear 选择,0,W*/
static inline void sys_ll_set_ana_reg16_int_clr_sel1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_INT_CLR_SEL1V_MASK << SYS_ANA_REG16_INT_CLR_SEL1V_POS);
    reg_value |= ((value & SYS_ANA_REG16_INT_CLR_SEL1V_MASK) << SYS_ANA_REG16_INT_CLR_SEL1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->ctrl_ck2d:0x50[18],在ctrl_seri_cap=0，本控制位无用；在ctrl_seri_cap=1，ctrl_ck2d=1,正常工作模式；在ctrl_seri_cap=1，ctrl_ck2d=0,串联电容开关同步ck2d。,0,W*/
static inline void sys_ll_set_ana_reg16_ctrl_ck2d(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_CTRL_CK2D_MASK << SYS_ANA_REG16_CTRL_CK2D_POS);
    reg_value |= ((value & SYS_ANA_REG16_CTRL_CK2D_MASK) << SYS_ANA_REG16_CTRL_CK2D_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->ctrl_seri_cap:0x50[19],0，内部串联6pF电容模式；1，正常不串联电容模式，需要配合ctrl_ck2d一起使用。,0,W*/
static inline void sys_ll_set_ana_reg16_ctrl_seri_cap(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_CTRL_SERI_CAP_MASK << SYS_ANA_REG16_CTRL_SERI_CAP_POS);
    reg_value |= ((value & SYS_ANA_REG16_CTRL_SERI_CAP_MASK) << SYS_ANA_REG16_CTRL_SERI_CAP_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->en_testcmp1v:0x50[20],使能比较器测试信号输出,0,W*/
static inline void sys_ll_set_ana_reg16_en_testcmp1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_EN_TESTCMP1V_MASK << SYS_ANA_REG16_EN_TESTCMP1V_POS);
    reg_value |= ((value & SYS_ANA_REG16_EN_TESTCMP1V_MASK) << SYS_ANA_REG16_EN_TESTCMP1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->en_man_wr1v:0x50[21],手动模式写入命令。,0,W*/
static inline void sys_ll_set_ana_reg16_en_man_wr1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_EN_MAN_WR1V_MASK << SYS_ANA_REG16_EN_MAN_WR1V_POS);
    reg_value |= ((value & SYS_ANA_REG16_EN_MAN_WR1V_MASK) << SYS_ANA_REG16_EN_MAN_WR1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->en_manmod1v:0x50[22],使能手动模式,0,W*/
static inline void sys_ll_set_ana_reg16_en_manmod1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_EN_MANMOD1V_MASK << SYS_ANA_REG16_EN_MANMOD1V_POS);
    reg_value |= ((value & SYS_ANA_REG16_EN_MANMOD1V_MASK) << SYS_ANA_REG16_EN_MANMOD1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG16_ADDR,reg_value);
}

/* REG_0x50:ana_reg16->cap_calspi1v:0x50[31:23],手动模式设置校正值大小,0,W*/
static inline void sys_ll_set_ana_reg16_cap_calspi1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG16_ADDR);
    reg_value &= ~(SYS_ANA_REG16_CAP_CALSPI1V_MASK << SYS_ANA_REG16_CAP_CALSPI1V_POS);
    reg_value |= ((value & SYS_ANA_REG16_CAP_CALSPI1V_MASK) << SYS_ANA_REG16_CAP_CALSPI1V_POS);
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

/* REG_0x51:ana_reg17->int_clr:0x51[9:0], mult_touch_int_clear，对应[9:0]通道,0,R/W*/
static inline uint32_t sys_ll_get_ana_reg17_int_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG17_INT_CLR_POS) & SYS_ANA_REG17_INT_CLR_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg17_int_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_INT_CLR_MASK << SYS_ANA_REG17_INT_CLR_POS);
    reg_value |= ((value & SYS_ANA_REG17_INT_CLR_MASK) << SYS_ANA_REG17_INT_CLR_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->int_clr16:0x51[10], wake_up_soft clear ,0,W*/
static inline void sys_ll_set_ana_reg17_int_clr16(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_INT_CLR16_MASK << SYS_ANA_REG17_INT_CLR16_POS);
    reg_value |= ((value & SYS_ANA_REG17_INT_CLR16_MASK) << SYS_ANA_REG17_INT_CLR16_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->int_clr_cal:0x51[21:12], ,0,W*/
static inline void sys_ll_set_ana_reg17_int_clr_cal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_INT_CLR_CAL_MASK << SYS_ANA_REG17_INT_CLR_CAL_POS);
    reg_value |= ((value & SYS_ANA_REG17_INT_CLR_CAL_MASK) << SYS_ANA_REG17_INT_CLR_CAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x51:ana_reg17->int_en_cal:0x51[31:22],dac current trimming, 00=minimum current, 11=maximum current,0,W*/
static inline void sys_ll_set_ana_reg17_int_en_cal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG17_ADDR);
    reg_value &= ~(SYS_ANA_REG17_INT_EN_CAL_MASK << SYS_ANA_REG17_INT_EN_CAL_POS);
    reg_value |= ((value & SYS_ANA_REG17_INT_EN_CAL_MASK) << SYS_ANA_REG17_INT_EN_CAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG17_ADDR,reg_value);
}

/* REG_0x52 //REG ADDR :0x44010148 */
static inline uint32_t sys_ll_get_ana_reg18_value(void)
{
    return REG_READ(SYS_ANA_REG18_ADDR);
}

static inline void sys_ll_set_ana_reg18_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,value);
}

/* REG_0x52:ana_reg18->iselaud:0x52[0],audio ADC top bias selection, 0=half, 1=normal,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_iselaud(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_ISELAUD_POS) & SYS_ANA_REG18_ISELAUD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_iselaud(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_ISELAUD_MASK << SYS_ANA_REG18_ISELAUD_POS);
    reg_value |= ((value & SYS_ANA_REG18_ISELAUD_MASK) << SYS_ANA_REG18_ISELAUD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->audck_rlcen1v:0x52[1],audio clock re-latch enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_audck_rlcen1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_AUDCK_RLCEN1V_POS) & SYS_ANA_REG18_AUDCK_RLCEN1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_audck_rlcen1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_AUDCK_RLCEN1V_MASK << SYS_ANA_REG18_AUDCK_RLCEN1V_POS);
    reg_value |= ((value & SYS_ANA_REG18_AUDCK_RLCEN1V_MASK) << SYS_ANA_REG18_AUDCK_RLCEN1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->lchckinven1v:0x52[2],latch clock inversion enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_lchckinven1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_LCHCKINVEN1V_POS) & SYS_ANA_REG18_LCHCKINVEN1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_lchckinven1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_LCHCKINVEN1V_MASK << SYS_ANA_REG18_LCHCKINVEN1V_POS);
    reg_value |= ((value & SYS_ANA_REG18_LCHCKINVEN1V_MASK) << SYS_ANA_REG18_LCHCKINVEN1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->enaudbias:0x52[3],audio bias enable(for adc and dac both),0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_enaudbias(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_ENAUDBIAS_POS) & SYS_ANA_REG18_ENAUDBIAS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_enaudbias(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_ENAUDBIAS_MASK << SYS_ANA_REG18_ENAUDBIAS_POS);
    reg_value |= ((value & SYS_ANA_REG18_ENAUDBIAS_MASK) << SYS_ANA_REG18_ENAUDBIAS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->enadcbias:0x52[4],adc current and reference enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_enadcbias(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_ENADCBIAS_POS) & SYS_ANA_REG18_ENADCBIAS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_enadcbias(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_ENADCBIAS_MASK << SYS_ANA_REG18_ENADCBIAS_POS);
    reg_value |= ((value & SYS_ANA_REG18_ENADCBIAS_MASK) << SYS_ANA_REG18_ENADCBIAS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->enmicbias:0x52[5],micbias enable, 1=enable,0x1,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_enmicbias(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_ENMICBIAS_POS) & SYS_ANA_REG18_ENMICBIAS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_enmicbias(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_ENMICBIAS_MASK << SYS_ANA_REG18_ENMICBIAS_POS);
    reg_value |= ((value & SYS_ANA_REG18_ENMICBIAS_MASK) << SYS_ANA_REG18_ENMICBIAS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->adcckinven1v:0x52[6],audio adc clock inversion selection,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_adcckinven1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_ADCCKINVEN1V_POS) & SYS_ANA_REG18_ADCCKINVEN1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_adcckinven1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_ADCCKINVEN1V_MASK << SYS_ANA_REG18_ADCCKINVEN1V_POS);
    reg_value |= ((value & SYS_ANA_REG18_ADCCKINVEN1V_MASK) << SYS_ANA_REG18_ADCCKINVEN1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->dacfb2st0v9:0x52[7],audio dac 2stg feedback res trimming, 0=204k, 1=102k,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_dacfb2st0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_DACFB2ST0V9_POS) & SYS_ANA_REG18_DACFB2ST0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_dacfb2st0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_DACFB2ST0V9_MASK << SYS_ANA_REG18_DACFB2ST0V9_POS);
    reg_value |= ((value & SYS_ANA_REG18_DACFB2ST0V9_MASK) << SYS_ANA_REG18_DACFB2ST0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->MICBIAS_trm:0x52[10:9],micbias trimming, 00=2.4V, 11=1.8V,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_micbias_trm(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_MICBIAS_TRM_POS) & SYS_ANA_REG18_MICBIAS_TRM_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_micbias_trm(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_MICBIAS_TRM_MASK << SYS_ANA_REG18_MICBIAS_TRM_POS);
    reg_value |= ((value & SYS_ANA_REG18_MICBIAS_TRM_MASK) << SYS_ANA_REG18_MICBIAS_TRM_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->MICBIAS_VOC:0x52[15:11],micbias output trimming,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_micbias_voc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_MICBIAS_VOC_POS) & SYS_ANA_REG18_MICBIAS_VOC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_micbias_voc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_MICBIAS_VOC_MASK << SYS_ANA_REG18_MICBIAS_VOC_POS);
    reg_value |= ((value & SYS_ANA_REG18_MICBIAS_VOC_MASK) << SYS_ANA_REG18_MICBIAS_VOC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->vrefsel1v:0x52[16],mic bias compensation selection, 1=external cap; 0=internal compensation,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_vrefsel1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_VREFSEL1V_POS) & SYS_ANA_REG18_VREFSEL1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_vrefsel1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_VREFSEL1V_MASK << SYS_ANA_REG18_VREFSEL1V_POS);
    reg_value |= ((value & SYS_ANA_REG18_VREFSEL1V_MASK) << SYS_ANA_REG18_VREFSEL1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->capswspi:0x52[21:17],munual value for cap trimming,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_capswspi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_CAPSWSPI_POS) & SYS_ANA_REG18_CAPSWSPI_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_capswspi(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_CAPSWSPI_MASK << SYS_ANA_REG18_CAPSWSPI_POS);
    reg_value |= ((value & SYS_ANA_REG18_CAPSWSPI_MASK) << SYS_ANA_REG18_CAPSWSPI_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->adref_sel:0x52[23:22],ADC ref Sel 00=300mV 01=400mV 10=500mV 11=600mV,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg18_adref_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG18_ADREF_SEL_POS) & SYS_ANA_REG18_ADREF_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg18_adref_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_ADREF_SEL_MASK << SYS_ANA_REG18_ADREF_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG18_ADREF_SEL_MASK) << SYS_ANA_REG18_ADREF_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x52:ana_reg18->spi_dacckpssel:0x52[31], ,0,W*/
static inline void sys_ll_set_ana_reg18_spi_dacckpssel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG18_ADDR);
    reg_value &= ~(SYS_ANA_REG18_SPI_DACCKPSSEL_MASK << SYS_ANA_REG18_SPI_DACCKPSSEL_POS);
    reg_value |= ((value & SYS_ANA_REG18_SPI_DACCKPSSEL_MASK) << SYS_ANA_REG18_SPI_DACCKPSSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG18_ADDR,reg_value);
}

/* REG_0x53 //REG ADDR :0x4401014c */
static inline uint32_t sys_ll_get_ana_reg19_value(void)
{
    return REG_READ(SYS_ANA_REG19_ADDR);
}

static inline void sys_ll_set_ana_reg19_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,value);
}

/* REG_0x53:ana_reg19->isel:0x53[1:0],adc bias trimming, 00=1.25uA, 01=1.875uA, 10=2.5uA(default), 11=3.125uA,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_isel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_ISEL_POS) & SYS_ANA_REG19_ISEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_isel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_ISEL_MASK << SYS_ANA_REG19_ISEL_POS);
    reg_value |= ((value & SYS_ANA_REG19_ISEL_MASK) << SYS_ANA_REG19_ISEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micirsel1:0x53[2],mic model input res selection, 0=9K, 1=6K,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micirsel1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICIRSEL1_POS) & SYS_ANA_REG19_MICIRSEL1_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micirsel1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICIRSEL1_MASK << SYS_ANA_REG19_MICIRSEL1_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICIRSEL1_MASK) << SYS_ANA_REG19_MICIRSEL1_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micdacit:0x53[4:3],adc micmode, mic_dac Iout Full-range, 00=112~177uA, 01=177~241uA, 10=241~305uA, 11=305~370uA,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micdacit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICDACIT_POS) & SYS_ANA_REG19_MICDACIT_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micdacit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICDACIT_MASK << SYS_ANA_REG19_MICDACIT_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICDACIT_MASK) << SYS_ANA_REG19_MICDACIT_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micdaciH:0x53[12:5],micdac dac control bits <7:0>,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micdacih(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICDACIH_POS) & SYS_ANA_REG19_MICDACIH_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micdacih(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICDACIH_MASK << SYS_ANA_REG19_MICDACIH_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICDACIH_MASK) << SYS_ANA_REG19_MICDACIH_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micsingleEn:0x53[13],adc micmode, single_end enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micsingleen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICSINGLEEN_POS) & SYS_ANA_REG19_MICSINGLEEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micsingleen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICSINGLEEN_MASK << SYS_ANA_REG19_MICSINGLEEN_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICSINGLEEN_MASK) << SYS_ANA_REG19_MICSINGLEEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->dccompen:0x53[14],mic signle_end mode, voltage swing of input OP control,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_dccompen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_DCCOMPEN_POS) & SYS_ANA_REG19_DCCOMPEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_dccompen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_DCCOMPEN_MASK << SYS_ANA_REG19_DCCOMPEN_POS);
    reg_value |= ((value & SYS_ANA_REG19_DCCOMPEN_MASK) << SYS_ANA_REG19_DCCOMPEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micGain:0x53[18:15],adc micmode gain, 0=0dB(17.9K), F=24dB(267.6K), 2dB/step,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micgain(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICGAIN_POS) & SYS_ANA_REG19_MICGAIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micgain(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICGAIN_MASK << SYS_ANA_REG19_MICGAIN_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICGAIN_MASK) << SYS_ANA_REG19_MICGAIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micdacen:0x53[19],adc micmode micdac enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micdacen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICDACEN_POS) & SYS_ANA_REG19_MICDACEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micdacen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICDACEN_MASK << SYS_ANA_REG19_MICDACEN_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICDACEN_MASK) << SYS_ANA_REG19_MICDACEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->stg2lsen1v:0x53[20],NC,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_stg2lsen1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_STG2LSEN1V_POS) & SYS_ANA_REG19_STG2LSEN1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_stg2lsen1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_STG2LSEN1V_MASK << SYS_ANA_REG19_STG2LSEN1V_POS);
    reg_value |= ((value & SYS_ANA_REG19_STG2LSEN1V_MASK) << SYS_ANA_REG19_STG2LSEN1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->openloopCal1v:0x53[21],mic single_end openloop calibration enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_openloopcal1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_OPENLOOPCAL1V_POS) & SYS_ANA_REG19_OPENLOOPCAL1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_openloopcal1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_OPENLOOPCAL1V_MASK << SYS_ANA_REG19_OPENLOOPCAL1V_POS);
    reg_value |= ((value & SYS_ANA_REG19_OPENLOOPCAL1V_MASK) << SYS_ANA_REG19_OPENLOOPCAL1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->callatch:0x53[22],mic dcoc n_end enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_callatch(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_CALLATCH_POS) & SYS_ANA_REG19_CALLATCH_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_callatch(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_CALLATCH_MASK << SYS_ANA_REG19_CALLATCH_POS);
    reg_value |= ((value & SYS_ANA_REG19_CALLATCH_MASK) << SYS_ANA_REG19_CALLATCH_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->vcmsel:0x53[23], ,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_vcmsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_VCMSEL_POS) & SYS_ANA_REG19_VCMSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_vcmsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_VCMSEL_MASK << SYS_ANA_REG19_VCMSEL_POS);
    reg_value |= ((value & SYS_ANA_REG19_VCMSEL_MASK) << SYS_ANA_REG19_VCMSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->dwamode:0x53[24],dwa mode selection,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_dwamode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_DWAMODE_POS) & SYS_ANA_REG19_DWAMODE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_dwamode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_DWAMODE_MASK << SYS_ANA_REG19_DWAMODE_POS);
    reg_value |= ((value & SYS_ANA_REG19_DWAMODE_MASK) << SYS_ANA_REG19_DWAMODE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->r2ren:0x53[25],adc 1st stage OP rail-2-rail enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_r2ren(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_R2REN_POS) & SYS_ANA_REG19_R2REN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_r2ren(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_R2REN_MASK << SYS_ANA_REG19_R2REN_POS);
    reg_value |= ((value & SYS_ANA_REG19_R2REN_MASK) << SYS_ANA_REG19_R2REN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->micEn:0x53[28],mic1 mode enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_micen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_MICEN_POS) & SYS_ANA_REG19_MICEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_micen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_MICEN_MASK << SYS_ANA_REG19_MICEN_POS);
    reg_value |= ((value & SYS_ANA_REG19_MICEN_MASK) << SYS_ANA_REG19_MICEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->rst:0x53[29],adc reset enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_rst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_RST_POS) & SYS_ANA_REG19_RST_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_rst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_RST_MASK << SYS_ANA_REG19_RST_POS);
    reg_value |= ((value & SYS_ANA_REG19_RST_MASK) << SYS_ANA_REG19_RST_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->bpdwa1v:0x53[30],adc1 dwa: 0=enable dwa; 0=disable dwa,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_bpdwa1v(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_BPDWA1V_POS) & SYS_ANA_REG19_BPDWA1V_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_bpdwa1v(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_BPDWA1V_MASK << SYS_ANA_REG19_BPDWA1V_POS);
    reg_value |= ((value & SYS_ANA_REG19_BPDWA1V_MASK) << SYS_ANA_REG19_BPDWA1V_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x53:ana_reg19->hcen1stg:0x53[31],1stg op bw increase enable, 1=high bw,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg19_hcen1stg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG19_HCEN1STG_POS) & SYS_ANA_REG19_HCEN1STG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg19_hcen1stg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG19_ADDR);
    reg_value &= ~(SYS_ANA_REG19_HCEN1STG_MASK << SYS_ANA_REG19_HCEN1STG_POS);
    reg_value |= ((value & SYS_ANA_REG19_HCEN1STG_MASK) << SYS_ANA_REG19_HCEN1STG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG19_ADDR,reg_value);
}

/* REG_0x54 //REG ADDR :0x44010150 */
static inline uint32_t sys_ll_get_ana_reg20_value(void)
{
    return REG_READ(SYS_ANA_REG20_ADDR);
}

static inline void sys_ll_set_ana_reg20_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,value);
}

/* REG_0x54:ana_reg20->hpdac:0x54[0],class ab driver high current mode. "1" high current. ,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_hpdac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_HPDAC_POS) & SYS_ANA_REG20_HPDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_hpdac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_HPDAC_MASK << SYS_ANA_REG20_HPDAC_POS);
    reg_value |= ((value & SYS_ANA_REG20_HPDAC_MASK) << SYS_ANA_REG20_HPDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->calcon_sel:0x54[1],1=when cal enable, fb of drver is disconnected; 0=fb is connected,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_calcon_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_CALCON_SEL_POS) & SYS_ANA_REG20_CALCON_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_calcon_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_CALCON_SEL_MASK << SYS_ANA_REG20_CALCON_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG20_CALCON_SEL_MASK) << SYS_ANA_REG20_CALCON_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->oscdac:0x54[3:2],threshold current setting for over current protection . "3" maximum current. "0" minimum current,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_oscdac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_OSCDAC_POS) & SYS_ANA_REG20_OSCDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_oscdac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_OSCDAC_MASK << SYS_ANA_REG20_OSCDAC_POS);
    reg_value |= ((value & SYS_ANA_REG20_OSCDAC_MASK) << SYS_ANA_REG20_OSCDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->ocendac:0x54[4],over current protection enable. "1" enable.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_ocendac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_OCENDAC_POS) & SYS_ANA_REG20_OCENDAC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_ocendac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_OCENDAC_MASK << SYS_ANA_REG20_OCENDAC_POS);
    reg_value |= ((value & SYS_ANA_REG20_OCENDAC_MASK) << SYS_ANA_REG20_OCENDAC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->vcmsel_1:0x54[5],combind with vcmsel<0>, 00=0.6, 01=0.65, 10=0.7, 11=0.7,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_vcmsel_1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_VCMSEL_1_POS) & SYS_ANA_REG20_VCMSEL_1_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_vcmsel_1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_VCMSEL_1_MASK << SYS_ANA_REG20_VCMSEL_1_POS);
    reg_value |= ((value & SYS_ANA_REG20_VCMSEL_1_MASK) << SYS_ANA_REG20_VCMSEL_1_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->adjdacref:0x54[10:6],audio dac reference voltage adjust.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_adjdacref(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_ADJDACREF_POS) & SYS_ANA_REG20_ADJDACREF_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_adjdacref(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_ADJDACREF_MASK << SYS_ANA_REG20_ADJDACREF_POS);
    reg_value |= ((value & SYS_ANA_REG20_ADJDACREF_MASK) << SYS_ANA_REG20_ADJDACREF_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dcochg:0x54[12:11],dcoc high gain selection. "1" high gain,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_dcochg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DCOCHG_POS) & SYS_ANA_REG20_DCOCHG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_dcochg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DCOCHG_MASK << SYS_ANA_REG20_DCOCHG_POS);
    reg_value |= ((value & SYS_ANA_REG20_DCOCHG_MASK) << SYS_ANA_REG20_DCOCHG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->diffen:0x54[13],enable differential mode. "1" enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_diffen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DIFFEN_POS) & SYS_ANA_REG20_DIFFEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_diffen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DIFFEN_MASK << SYS_ANA_REG20_DIFFEN_POS);
    reg_value |= ((value & SYS_ANA_REG20_DIFFEN_MASK) << SYS_ANA_REG20_DIFFEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->endaccal:0x54[14],enable offset calibration process. "1" enable.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_endaccal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_ENDACCAL_POS) & SYS_ANA_REG20_ENDACCAL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_endaccal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_ENDACCAL_MASK << SYS_ANA_REG20_ENDACCAL_POS);
    reg_value |= ((value & SYS_ANA_REG20_ENDACCAL_MASK) << SYS_ANA_REG20_ENDACCAL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->lendcoc:0x54[16],L-channel Dcoc dac enable. "1" enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_lendcoc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_LENDCOC_POS) & SYS_ANA_REG20_LENDCOC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_lendcoc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_LENDCOC_MASK << SYS_ANA_REG20_LENDCOC_POS);
    reg_value |= ((value & SYS_ANA_REG20_LENDCOC_MASK) << SYS_ANA_REG20_LENDCOC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->lenvcmd:0x54[18],L-channel common mode output buffer enable. "1" enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_lenvcmd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_LENVCMD_POS) & SYS_ANA_REG20_LENVCMD_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_lenvcmd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_LENVCMD_MASK << SYS_ANA_REG20_LENVCMD_POS);
    reg_value |= ((value & SYS_ANA_REG20_LENVCMD_MASK) << SYS_ANA_REG20_LENVCMD_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dacdrven:0x54[19],dac output driver enable."1" enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_dacdrven(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DACDRVEN_POS) & SYS_ANA_REG20_DACDRVEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_dacdrven(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DACDRVEN_MASK << SYS_ANA_REG20_DACDRVEN_POS);
    reg_value |= ((value & SYS_ANA_REG20_DACDRVEN_MASK) << SYS_ANA_REG20_DACDRVEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dacLen:0x54[21],dac L-channel enable. "1" enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_daclen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DACLEN_POS) & SYS_ANA_REG20_DACLEN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_daclen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DACLEN_MASK << SYS_ANA_REG20_DACLEN_POS);
    reg_value |= ((value & SYS_ANA_REG20_DACLEN_MASK) << SYS_ANA_REG20_DACLEN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dacG:0x54[25:22],dac gain setting: 0000=-15dB, 111=0dB,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_dacg(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DACG_POS) & SYS_ANA_REG20_DACG_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_dacg(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DACG_MASK << SYS_ANA_REG20_DACG_POS);
    reg_value |= ((value & SYS_ANA_REG20_DACG_MASK) << SYS_ANA_REG20_DACG_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dacmute:0x54[26],dac mute enable. "1" mute enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_dacmute(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DACMUTE_POS) & SYS_ANA_REG20_DACMUTE_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_dacmute(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DACMUTE_MASK << SYS_ANA_REG20_DACMUTE_POS);
    reg_value |= ((value & SYS_ANA_REG20_DACMUTE_MASK) << SYS_ANA_REG20_DACMUTE_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dacdwamode_sel:0x54[27],dwa mode selection,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_dacdwamode_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DACDWAMODE_SEL_POS) & SYS_ANA_REG20_DACDWAMODE_SEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_dacdwamode_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DACDWAMODE_SEL_MASK << SYS_ANA_REG20_DACDWAMODE_SEL_POS);
    reg_value |= ((value & SYS_ANA_REG20_DACDWAMODE_SEL_MASK) << SYS_ANA_REG20_DACDWAMODE_SEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x54:ana_reg20->dacsel:0x54[31:28],dac ref sel 0000=0, 1111=500mV, 33.3mV/step,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg20_dacsel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG20_DACSEL_POS) & SYS_ANA_REG20_DACSEL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg20_dacsel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG20_ADDR);
    reg_value &= ~(SYS_ANA_REG20_DACSEL_MASK << SYS_ANA_REG20_DACSEL_POS);
    reg_value |= ((value & SYS_ANA_REG20_DACSEL_MASK) << SYS_ANA_REG20_DACSEL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG20_ADDR,reg_value);
}

/* REG_0x55 //REG ADDR :0x44010154 */
static inline uint32_t sys_ll_get_ana_reg21_value(void)
{
    return REG_READ(SYS_ANA_REG21_ADDR);
}

static inline void sys_ll_set_ana_reg21_value(uint32_t value)
{
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,value);
}

/* REG_0x55:ana_reg21->lmdcin:0x55[7:0],l-cnannel offset cancel dac maumual input.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_lmdcin(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_LMDCIN_POS) & SYS_ANA_REG21_LMDCIN_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_lmdcin(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_LMDCIN_MASK << SYS_ANA_REG21_LMDCIN_POS);
    reg_value |= ((value & SYS_ANA_REG21_LMDCIN_MASK) << SYS_ANA_REG21_LMDCIN_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->spirst_ovc:0x55[16],DAC ovc control rst enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_spirst_ovc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_SPIRST_OVC_POS) & SYS_ANA_REG21_SPIRST_OVC_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_spirst_ovc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_SPIRST_OVC_MASK << SYS_ANA_REG21_SPIRST_OVC_POS);
    reg_value |= ((value & SYS_ANA_REG21_SPIRST_OVC_MASK) << SYS_ANA_REG21_SPIRST_OVC_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->enidacL:0x55[18],Left channel IDAC enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_enidacl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_ENIDACL_POS) & SYS_ANA_REG21_ENIDACL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_enidacl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_ENIDACL_MASK << SYS_ANA_REG21_ENIDACL_POS);
    reg_value |= ((value & SYS_ANA_REG21_ENIDACL_MASK) << SYS_ANA_REG21_ENIDACL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->dac3rdhc0v9:0x55[19],audio DAC 3stg OP current trimming, 0=normal, 1=x2,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_dac3rdhc0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_DAC3RDHC0V9_POS) & SYS_ANA_REG21_DAC3RDHC0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_dac3rdhc0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_DAC3RDHC0V9_MASK << SYS_ANA_REG21_DAC3RDHC0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_DAC3RDHC0V9_MASK) << SYS_ANA_REG21_DAC3RDHC0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->hc2s:0x55[20],audio DAC current control, 0=half,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_hc2s(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_HC2S_POS) & SYS_ANA_REG21_HC2S_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_hc2s(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_HC2S_MASK << SYS_ANA_REG21_HC2S_POS);
    reg_value |= ((value & SYS_ANA_REG21_HC2S_MASK) << SYS_ANA_REG21_HC2S_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->rfb_ctrl:0x55[21],audio DAC feedback res half enable, 1=half,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_rfb_ctrl(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_RFB_CTRL_POS) & SYS_ANA_REG21_RFB_CTRL_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_rfb_ctrl(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_RFB_CTRL_MASK << SYS_ANA_REG21_RFB_CTRL_POS);
    reg_value |= ((value & SYS_ANA_REG21_RFB_CTRL_MASK) << SYS_ANA_REG21_RFB_CTRL_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->vcmsel_0:0x55[22],combind with vcmsel<1>, 00=0.6, 01=0.65, 10=0.7, 11=0.7,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_vcmsel_0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_VCMSEL_0_POS) & SYS_ANA_REG21_VCMSEL_0_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_vcmsel_0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_VCMSEL_0_MASK << SYS_ANA_REG21_VCMSEL_0_POS);
    reg_value |= ((value & SYS_ANA_REG21_VCMSEL_0_MASK) << SYS_ANA_REG21_VCMSEL_0_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->enbs:0x55[23],dac bias enable,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_enbs(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_ENBS_POS) & SYS_ANA_REG21_ENBS_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_enbs(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_ENBS_MASK << SYS_ANA_REG21_ENBS_POS);
    reg_value |= ((value & SYS_ANA_REG21_ENBS_MASK) << SYS_ANA_REG21_ENBS_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->calck_sel0v9:0x55[24],offset calibration clock selection. "1" high clock.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_calck_sel0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_CALCK_SEL0V9_POS) & SYS_ANA_REG21_CALCK_SEL0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_calck_sel0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_CALCK_SEL0V9_MASK << SYS_ANA_REG21_CALCK_SEL0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_CALCK_SEL0V9_MASK) << SYS_ANA_REG21_CALCK_SEL0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->bpdwa0v9:0x55[25],bypss audio dac dwa. "1" bypass.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_bpdwa0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_BPDWA0V9_POS) & SYS_ANA_REG21_BPDWA0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_bpdwa0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_BPDWA0V9_MASK << SYS_ANA_REG21_BPDWA0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_BPDWA0V9_MASK) << SYS_ANA_REG21_BPDWA0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->looprst0v9:0x55[26],audio dac integrator capacitor reset. "1" reset.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_looprst0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_LOOPRST0V9_POS) & SYS_ANA_REG21_LOOPRST0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_looprst0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_LOOPRST0V9_MASK << SYS_ANA_REG21_LOOPRST0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_LOOPRST0V9_MASK) << SYS_ANA_REG21_LOOPRST0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->oct0v9:0x55[28:27],over current delay time setting."11" maximum time. "00" minimum current.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_oct0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_OCT0V9_POS) & SYS_ANA_REG21_OCT0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_oct0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_OCT0V9_MASK << SYS_ANA_REG21_OCT0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_OCT0V9_MASK) << SYS_ANA_REG21_OCT0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->sout0v9:0x55[29],short output with 600ohm resistor. "1" short output.,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_sout0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_SOUT0V9_POS) & SYS_ANA_REG21_SOUT0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_sout0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_SOUT0V9_MASK << SYS_ANA_REG21_SOUT0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_SOUT0V9_MASK) << SYS_ANA_REG21_SOUT0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}

/* REG_0x55:ana_reg21->hc0v9:0x55[31:30],dac current trimming, 00=minimum current, 11=maximum current,0x0,W/R*/
static inline uint32_t sys_ll_get_ana_reg21_hc0v9(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value = ((reg_value >> SYS_ANA_REG21_HC0V9_POS) & SYS_ANA_REG21_HC0V9_MASK);
    return reg_value;
}

static inline void sys_ll_set_ana_reg21_hc0v9(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_ANA_REG21_ADDR);
    reg_value &= ~(SYS_ANA_REG21_HC0V9_MASK << SYS_ANA_REG21_HC0V9_POS);
    reg_value |= ((value & SYS_ANA_REG21_HC0V9_MASK) << SYS_ANA_REG21_HC0V9_POS);
    sys_ll_set_analog_reg_value(SYS_ANA_REG21_ADDR,reg_value);
}
//TODO put below functions in suitable position
static inline void sys_ll_set_yuv_buf_clock_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK1_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_YUV_BUF_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_YUV_BUF_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_YUV_BUF_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_YUV_BUF_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK1_ENABLE_ADDR,reg_value);
}

static inline void sys_ll_set_h264_clock_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(SYS_CPU_DEVICE_CLK1_ENABLE_ADDR);
    reg_value &= ~(SYS_CPU_DEVICE_CLK_ENABLE_H264_CKEN_MASK << SYS_CPU_DEVICE_CLK_ENABLE_H264_CKEN_POS);
    reg_value |= ((value & SYS_CPU_DEVICE_CLK_ENABLE_H264_CKEN_MASK) << SYS_CPU_DEVICE_CLK_ENABLE_H264_CKEN_POS);
    REG_WRITE(SYS_CPU_DEVICE_CLK1_ENABLE_ADDR,reg_value);
}

#ifdef __cplusplus
}
#endif
