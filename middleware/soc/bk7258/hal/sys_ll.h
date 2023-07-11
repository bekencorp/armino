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
#include "system_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_LL_REG_BASE   SOC_SYS_REG_BASE

//This way of setting ana_reg_bit value is only for sys_ctrl, other driver please implement by yourself!!

#if CONFIG_ANA_REG_WRITE_POLL_REG_B
#define SYS_ANALOG_REG_SPI_STATE_REG (SYS_CPU_ANASPI_FREQ_ADDR)
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx + 8)
#else
#define SYS_ANALOG_REG_SPI_STATE_REG (SOC_SYS_REG_BASE + (0x3a << 2))
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx)
#endif

#define GET_SYS_ANALOG_REG_IDX(addr) ((addr - SYS_ANA_REG0_ADDR) >> 2)

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

static inline void sys_set_ana_reg_bit(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)
{
	uint32_t reg_value;
	reg_value = *(volatile uint32_t *)(reg_addr);
	reg_value &= ~(mask << pos);
	reg_value |= ((value & mask) <<pos);
	sys_ll_set_analog_reg_value(reg_addr, reg_value);
}

//reg device_id:

static inline void sys_ll_set_device_id_value(uint32_t v) {
	sys_device_id_t *r = (sys_device_id_t*)(SOC_SYS_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_device_id_value(void) {
	sys_device_id_t *r = (sys_device_id_t*)(SOC_SYS_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_device_id_deviceid(void) {
	sys_device_id_t *r = (sys_device_id_t*)(SOC_SYS_REG_BASE + (0x0 << 2));
	return r->deviceid;
}

//reg version_id:

static inline void sys_ll_set_version_id_value(uint32_t v) {
	sys_version_id_t *r = (sys_version_id_t*)(SOC_SYS_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_version_id_value(void) {
	sys_version_id_t *r = (sys_version_id_t*)(SOC_SYS_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_version_id_versionid(void) {
	sys_version_id_t *r = (sys_version_id_t*)(SOC_SYS_REG_BASE + (0x1 << 2));
	return r->versionid;
}

//reg cpu_storage_connect_op_select:

static inline void sys_ll_set_cpu_storage_connect_op_select_value(uint32_t v) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_value(void) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_boot_mode(uint32_t v) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->boot_mode = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_boot_mode(void) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	return r->boot_mode;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(uint32_t v) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->jtag_core_sel = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel(void) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	return r->jtag_core_sel;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_flash_sel(uint32_t v) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->flash_sel = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_flash_sel(void) {
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	return r->flash_sel;
}

//reg cpu_current_run_status:

static inline void sys_ll_set_cpu_current_run_status_value(uint32_t v) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_value(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_core0_halted(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->core0_halted;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_core1_halted(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->core1_halted;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_sw_reset(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->cpu0_sw_reset;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_cpu1_sw_reset(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->cpu1_sw_reset;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->cpu0_pwr_dw_state;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state(void) {
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	return r->cpu1_pwr_dw_state;
}

//reg cpu0_int_halt_clk_op:

static inline void sys_ll_set_cpu0_int_halt_clk_op_value(uint32_t v) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_value(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(uint32_t v) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_sw_rst = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->cpu0_sw_rst;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(uint32_t v) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_pwr_dw = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->cpu0_pwr_dw;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(uint32_t v) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_int_mask = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->cpu0_int_mask;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(uint32_t v) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_halt = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->cpu0_halt;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_reserved_4_7(uint32_t v) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->reserved_4_7 = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_reserved_4_7(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->reserved_4_7;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(void) {
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	return r->cpu0_offset;
}

//reg cpu1_int_halt_clk_op:

static inline void sys_ll_set_cpu1_int_halt_clk_op_value(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_value(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_sw_rst = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_sw_rst(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->cpu1_sw_rst;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_pwr_dw = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->cpu1_pwr_dw;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_int_mask = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_int_mask(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->cpu1_int_mask;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_halt = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_halt(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->cpu1_halt;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu0_bus_clk_2div = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->cpu0_bus_clk_2div;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_reserved_5_7(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->reserved_5_7 = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_reserved_5_7(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->reserved_5_7;
}

static inline void sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(uint32_t v) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_offset = v;
}

static inline uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_offset(void) {
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	return r->cpu1_offset;
}

//reg reserved_reg0x6:

static inline void sys_ll_set_reserved_reg0x6_value(uint32_t v) {
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_reserved_reg0x6_value(void) {
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	return r->v;
}

static inline void sys_ll_set_reserved_reg0x6_reserved_0_31(uint32_t v) {
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	r->reserved_0_31 = v;
}

static inline uint32_t sys_ll_get_reserved_reg0x6_reserved_0_31(void) {
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	return r->reserved_0_31;
}

//reg cpu_clk_div_mode1:

static inline void sys_ll_set_cpu_clk_div_mode1_value(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_value(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_core(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_core = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_core(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_core;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_core(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_core = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_core(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_core;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_bus = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_bus(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_bus;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart0(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_uart0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart0(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_uart0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clksel_uart0(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clksel_uart0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clksel_uart0(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clksel_uart0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart1(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_uart1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart1(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_uart1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart1(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_uart1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart1(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_uart1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart2(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_uart2 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart2(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_uart2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart2(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_uart2 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart2(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_uart2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_sadc(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_sadc = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_sadc(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_sadc;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_pwm0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_pwm0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_pwm1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm1(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_pwm1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_tim0(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_tim0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim0(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_tim0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_tim1(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_tim1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim1(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_tim1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_tim2(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_tim2 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim2(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_tim2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_i2s(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_i2s = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2s(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_i2s;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_aud(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_aud = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_aud(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_aud;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_jpeg = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_jpeg(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_jpeg;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_jpeg = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_jpeg(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->cksel_jpeg;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(uint32_t v) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_disp_l = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l(void) {
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	return r->clkdiv_disp_l;
}

//reg cpu_clk_div_mode2:

static inline void sys_ll_set_cpu_clk_div_mode2_value(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_value(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->clkdiv_disp_h = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->clkdiv_disp_h;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_disp(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_disp = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_disp(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->cksel_disp;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_psram = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->ckdiv_psram;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_psram(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_psram = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_psram(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->cksel_psram;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_qspi0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_qspi0(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->ckdiv_qspi0;
}

static inline void sys_ll_set_cpu_clk_div_mode2_reserved_10_13(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->reserved_10_13 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_reserved_10_13(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->reserved_10_13;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_sdio = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->ckdiv_sdio;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_sdio(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_sdio = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_sdio(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->cksel_sdio;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_auxs = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->ckdiv_auxs;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_auxs(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_auxs = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_auxs(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->cksel_auxs;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_flash(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_flash = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_flash(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->cksel_flash;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_flash = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->ckdiv_flash;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_i2s0(uint32_t v) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_i2s0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_i2s0(void) {
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	return r->ckdiv_i2s0;
}

//reg cpu_26m_wdt_clk_div:

static inline void sys_ll_set_cpu_26m_wdt_clk_div_value(uint32_t v) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_value(void) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_26m(uint32_t v) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->ckdiv_26m = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_26m(void) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->ckdiv_26m;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(uint32_t v) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->ckdiv_wdt = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(void) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->ckdiv_wdt;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(uint32_t v) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->clksel_spi0 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi0(void) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->clksel_spi0;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(uint32_t v) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->clksel_spi1 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi1(void) {
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->clksel_spi1;
}

//reg cpu_anaspi_freq:

static inline void sys_ll_set_cpu_anaspi_freq_value(uint32_t v) {
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_anaspi_freq_value(void) {
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_anaspi_freq_anaspi_freq(uint32_t v) {
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	r->anaspi_freq = v;
}

static inline uint32_t sys_ll_get_cpu_anaspi_freq_anaspi_freq(void) {
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	return r->anaspi_freq;
}

//reg cpu_device_clk_enable:

static inline void sys_ll_set_cpu_device_clk_enable_value(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_value(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c0_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->i2c0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c0_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->i2c0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi0_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->spi0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi0_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->spi0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart0_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->uart0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart0_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->uart0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm0_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->pwm0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm0_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->pwm0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim0_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->tim0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim0_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->tim0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_sadc_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->sadc_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_sadc_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->sadc_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_irda_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->irda_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_irda_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->irda_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_efuse_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->efuse_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_efuse_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->efuse_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c1_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->i2c1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c1_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->i2c1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi1_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->spi1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi1_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->spi1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart1_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->uart1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart1_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->uart1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart2_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->uart2_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart2_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->uart2_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm1_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->pwm1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm1_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->pwm1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim1_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->tim1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim1_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->tim1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim2_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->tim2_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim2_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->tim2_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_otp_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->otp_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_otp_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->otp_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2s_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->i2s_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2s_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->i2s_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_usb_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->usb_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_usb_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->usb_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_can_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->can_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_can_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->can_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_psram_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->psram_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_psram_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->psram_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi0_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->qspi0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi0_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->qspi0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi1_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->qspi1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi1_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->qspi1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_sdio_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->sdio_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_sdio_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->sdio_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_auxs_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->auxs_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_auxs_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->auxs_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_btdm_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->btdm_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_btdm_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->btdm_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_xvr_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->xvr_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_xvr_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->xvr_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_mac_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->mac_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->mac_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_phy_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->phy_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->phy_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_jpeg_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->jpeg_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_jpeg_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->jpeg_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_disp_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->disp_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_disp_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->disp_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_aud_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->aud_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_aud_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->aud_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_wdt_cken(uint32_t v) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->wdt_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_wdt_cken(void) {
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	return r->wdt_cken;
}

//reg reserver_reg0xd:

static inline void sys_ll_set_reserver_reg0xd_value(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_value(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->v;
}

static inline void sys_ll_set_reserver_reg0xd_h264_cken(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->h264_cken = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_h264_cken(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->h264_cken;
}

static inline void sys_ll_set_reserver_reg0xd_i2s1_cken(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->i2s1_cken = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_i2s1_cken(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->i2s1_cken;
}

static inline void sys_ll_set_reserver_reg0xd_i2s2_cken(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->i2s2_cken = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_i2s2_cken(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->i2s2_cken;
}

static inline void sys_ll_set_reserver_reg0xd_yuv_cken(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->yuv_cken = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_yuv_cken(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->yuv_cken;
}

static inline void sys_ll_set_reserver_reg0xd_slcd_cken(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->slcd_cken = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_slcd_cken(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->slcd_cken;
}

static inline void sys_ll_set_reserver_reg0xd_reserved_5_31(uint32_t v) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->reserved_5_31 = v;
}

static inline uint32_t sys_ll_get_reserver_reg0xd_reserved_5_31(void) {
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	return r->reserved_5_31;
}

//reg cpu_device_mem_ctrl1:

static inline void sys_ll_set_cpu_device_mem_ctrl1_value(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_value(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_uart1_mem_sd(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->uart1_mem_sd = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart1_mem_sd(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->uart1_mem_sd;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_uart2(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->uart2 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart2(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->uart2;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_spi1(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->spi1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_spi1(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->spi1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_sdio(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->sdio = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_sdio(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->sdio;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_usb(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->usb = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_usb(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->usb;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_can(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->can = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_can(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->can;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_qspi0(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->qspi0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_qspi0(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->qspi0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_qspi1(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->qspi1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_qspi1(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->qspi1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_pram(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->pram = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_pram(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->pram;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_fft(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->fft = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_fft(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->fft;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_abc(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->abc = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_abc(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->abc;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_aud(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->aud = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_aud(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->aud;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_i2s0(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->i2s0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s0(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->i2s0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_i2s1(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->i2s1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s1(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->i2s1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_i2s2(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->i2s2 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s2(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->i2s2;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_jpge(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->jpge = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_jpge(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->jpge;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_yuv(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->yuv = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_yuv(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->yuv;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_jpgd(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->jpgd = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_jpgd(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->jpgd;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_disp(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->disp = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_disp(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->disp;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_dmad(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->dmad = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_dmad(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->dmad;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_h26f(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->h26f = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_h26f(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->h26f;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_mac(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->mac = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_mac(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->mac;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_phy(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->phy = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_phy(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->phy;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_xvr(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->xvr = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_xvr(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->xvr;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_irda(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->irda = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_irda(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->irda;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_la(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->la = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_la(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->la;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_flsh(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->flsh = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_flsh(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->flsh;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_uart(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->uart = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->uart;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_spi0(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->spi0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_spi0(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->spi0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_enc(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->enc = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_enc(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->enc;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_dma0(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->dma0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_dma0(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->dma0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl1_dma1(uint32_t v) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->dma1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl1_dma1(void) {
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	return r->dma1;
}

//reg cpu_device_mem_ctrl2:

static inline void sys_ll_set_cpu_device_mem_ctrl2_value(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_value(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart1_mem_ds(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->uart1_mem_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart1_mem_ds(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->uart1_mem_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart2(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->uart2 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart2(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->uart2;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_spi1(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->spi1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi1(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->spi1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_sdio(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->sdio = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_sdio(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->sdio;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_usb(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->usb = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_usb(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->usb;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_can(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->can = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_can(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->can;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_qspi0(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->qspi0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi0(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->qspi0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_qspi1(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->qspi1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi1(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->qspi1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_pram(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->pram = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_pram(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->pram;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_fft(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->fft = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_fft(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->fft;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_abc(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->abc = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_abc(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->abc;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_aud(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->aud = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_aud(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->aud;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_i2s0(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->i2s0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s0(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->i2s0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_i2s1(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->i2s1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s1(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->i2s1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_i2s2(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->i2s2 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s2(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->i2s2;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_jpge(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->jpge = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_jpge(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->jpge;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_yuv(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->yuv = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_yuv(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->yuv;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_jpgd(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->jpgd = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_jpgd(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->jpgd;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_disp(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->disp = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_disp(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->disp;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_dmad(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->dmad = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_dmad(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->dmad;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_h26f(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->h26f = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_h26f(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->h26f;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_mac(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->mac = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_mac(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->mac;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_phy(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->phy = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_phy(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->phy;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_xvr(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->xvr = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_xvr(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->xvr;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_irda(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->irda = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_irda(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->irda;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_la(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->la = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_la(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->la;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_flsh(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->flsh = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_flsh(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->flsh;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->uart = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->uart;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_spi0(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->spi0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi0(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->spi0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_enc(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->enc = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_enc(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->enc;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_dma0(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->dma0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma0(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->dma0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_dma1(uint32_t v) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->dma1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma1(void) {
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	return r->dma1;
}

//reg cpu_power_sleep_wakeup:

static inline void sys_ll_set_cpu_power_sleep_wakeup_value(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_value(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem1 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_mem1;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem2 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_mem2;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem3 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_mem3;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_encp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_encp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_bakp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_bakp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_ahbp(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_ahbp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ahbp(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_ahbp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_audp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_audp(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_audp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_vidp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_vidp(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_vidp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_btsp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_btsp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_wifp_mac = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_wifp_mac;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_wifp_phy = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_wifp_phy;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem0(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem0 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem0(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_mem0;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem4(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem4 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem4(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_mem4;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_ofdm = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->pwd_ofdm;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_tcm0_pgen(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->tcm0_pgen = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm0_pgen(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->tcm0_pgen;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->rom_pgen = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_pgen(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->rom_pgen;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_need_flash_idle = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->sleep_en_need_flash_idle;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_need_cpu1_wfi = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->sleep_en_need_cpu1_wfi;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_need_cpu0_wfi = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->sleep_en_need_cpu0_wfi;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_global = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->sleep_en_global;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_bus_idle_bypass = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->sleep_bus_idle_bypass;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->bts_wakeup_platform_en = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->bts_wakeup_platform_en;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->bts_soft_wakeup_req = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_soft_wakeup_req(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->bts_soft_wakeup_req;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_rom_rd_disable(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->rom_rd_disable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_rd_disable(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->rom_rd_disable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_otp_rd_disable(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->otp_rd_disable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_otp_rd_disable(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->otp_rd_disable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_tcm1_pgen(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->tcm1_pgen = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm1_pgen(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->tcm1_pgen;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu0_subpwdm_en = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_subpwdm_en(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->cpu0_subpwdm_en;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_cpu1_subpwdm_en(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu1_subpwdm_en = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu1_subpwdm_en(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->cpu1_subpwdm_en;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_share_mem_clkgating_disable(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->share_mem_clkgating_disable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_share_mem_clkgating_disable(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->share_mem_clkgating_disable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu0_ticktimer_32k_enable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->cpu0_ticktimer_32k_enable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_cpu1_ticktimer_32k_enable(uint32_t v) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu1_ticktimer_32k_enable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu1_ticktimer_32k_enable(void) {
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	return r->cpu1_ticktimer_32k_enable;
}

//reg reserver_reg0x11:

static inline void sys_ll_set_reserver_reg0x11_value(uint32_t v) {
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_reserver_reg0x11_value(void) {
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	return r->v;
}

static inline void sys_ll_set_reserver_reg0x11_reserved_0_31(uint32_t v) {
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	r->reserved_0_31 = v;
}

static inline uint32_t sys_ll_get_reserver_reg0x11_reserved_0_31(void) {
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	return r->reserved_0_31;
}

//reg cpu_device_mem_ctrl3:

static inline void sys_ll_set_cpu_device_mem_ctrl3_value(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_value(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram0_mem_ds_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram0_mem_ds_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_mem_ds_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram0_mem_ds_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram1_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram1_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram1_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram2_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram2_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram2_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram3_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram3_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram3_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram4_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram4_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram4_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_icache_itag_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_icache_itag_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_icache_itag_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_icache_itag_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dcache_dtag_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dcache_dtag_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dcache_dtag_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_dcache_dtag_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_icache_itag_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_icache_itag_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_icache_itag_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_icache_itag_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dcache_dtag_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dcache_dtag_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dcache_dtag_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_dcache_dtag_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_itcm_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_itcm_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_itcm_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_itcm_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dtcm_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dtcm_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dtcm_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_dtcm_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_itcm_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_itcm_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_itcm_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_itcm_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dtcm_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dtcm_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dtcm_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_dtcm_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_rott_deep_sleep(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->rott_deep_sleep = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_rott_deep_sleep(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->rott_deep_sleep;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_reserved0(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved0(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->reserved0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_reserved1(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved1(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->reserved1;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram0_mem_sd_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram0_mem_sd_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_mem_sd_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram0_mem_sd_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram1_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram1_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram1_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram2_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram2_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram2_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram3_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram3_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram3_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram4_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram4_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->ram4_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_icache_itag_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_icache_itag_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_icache_itag_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_icache_itag_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dcache_dtag_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dcache_dtag_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dcache_dtag_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_dcache_dtag_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_icache_itag_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_icache_itag_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_icache_itag_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_icache_itag_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dcache_dtag_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dcache_dtag_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dcache_dtag_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_dcache_dtag_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_itcm_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_itcm_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_itcm_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_itcm_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dtcm_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dtcm_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dtcm_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu0_dtcm_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_itcm_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_itcm_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_itcm_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_itcm_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dtcm_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dtcm_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dtcm_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->cpu1_dtcm_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_rott_shutdown(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->rott_shutdown = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_rott_shutdown(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->rott_shutdown;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_reserved2(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved2 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved2(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->reserved2;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_reserved3(uint32_t v) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved3 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved3(void) {
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	return r->reserved3;
}

//reg cpu0_int_0_31_en:

static inline void sys_ll_set_cpu0_int_0_31_en_value(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_value(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_dma0_nsec_intr_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_dma0_nsec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma0_nsec_intr_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_dma0_nsec_intr_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_encp_sec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_encp_sec_intr_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_encp_nsec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_encp_nsec_intr_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer0_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_timer0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer0_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_timer0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_uart_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_uart_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_pwm0_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_pwm0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_pwm0_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_pwm0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c0_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_i2c0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c0_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_i2c0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi0_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_spi0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi0_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_spi0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sadc_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_sadc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sadc_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_sadc_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_irda_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_irda_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_irda_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_irda_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_sdio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_sdio_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_gdma_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_gdma_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_la_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_la_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_timer1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_timer1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_i2c1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_i2c1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_uart1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_uart1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_uart2_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_uart2_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_spi1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_spi1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_can_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_can_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_can_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_can_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_usb_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_usb_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_usb_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_usb_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_qspi0_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_qspi0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_qspi0_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_qspi0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_fft_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_fft_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_fft_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sbc_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_sbc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sbc_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_sbc_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_aud_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_aud_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_aud_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2s0_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_i2s0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2s0_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_i2s0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_jpegenc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_jpegenc_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_jpegdec_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_jpegdec_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_display_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_lcd_display_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_display_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_lcd_display_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_dma2d_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma2d_int_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_dma2d_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_wifi_int_phy_mpb_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_wifi_int_phy_mpb_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_wifi_int_phy_riu_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_wifi_int_phy_riu_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(uint32_t v) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_wifi_mac_int_tx_rx_timer_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(void) {
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	return r->cpu0_wifi_mac_int_tx_rx_timer_en;
}

//reg cpu0_int_32_63_en:

static inline void sys_ll_set_cpu0_int_32_63_en_value(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_value(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_tx_rx_misc_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_mac_int_tx_rx_misc_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_rx_trigger_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_mac_int_rx_trigger_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_tx_trigger_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_mac_int_tx_trigger_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_prot_trigger_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_mac_int_prot_trigger_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_gen_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_mac_int_gen_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_hsu_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_hsu_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_int_mac_wakeup_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_wifi_int_mac_wakeup_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dm_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_dm_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_ble_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_ble_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_bt_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_bt_irq_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_qspi1_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_qspi1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_qspi1_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_qspi1_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_pwm1_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_pwm1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_pwm1_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_pwm1_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_i2s1_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_i2s1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_i2s1_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_i2s1_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_i2s2_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_i2s2_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_i2s2_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_i2s2_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_h264_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_h264_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_h264_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_h264_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_sdmadc_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_sdmadc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_sdmadc_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_sdmadc_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_mbox0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_mbox0_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_mbox1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_mbox1_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_bmc64_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_bmc64_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dpll_unlock_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dpll_unlock_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dpll_unlock_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_dpll_unlock_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_touched_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_touched_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_touched_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_usbplug_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_usbplug_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_rtc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rtc_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_rtc_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_gpio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_gpio_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dma1_sec_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dma1_sec_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dma1_sec_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_dma1_sec_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dma1_nsec_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dma1_nsec_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dma1_nsec_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_dma1_nsec_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_yuvb_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_yuvb_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_yuvb_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_yuvb_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_rott_int_en(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_rott_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rott_int_en(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->cpu0_rott_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_reserved0(uint32_t v) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_reserved0(void) {
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	return r->reserved0;
}

//reg cpu1_int_0_31_en:

static inline void sys_ll_set_cpu1_int_0_31_en_value(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_value(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_dma0_nsec_intr_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_dma0_nsec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma0_nsec_intr_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_dma0_nsec_intr_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_encp_sec_intr_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_encp_sec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_encp_sec_intr_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_encp_sec_intr_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_encp_nsec_intr_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_encp_nsec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_encp_nsec_intr_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_encp_nsec_intr_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_timer0_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_timer0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer0_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_timer0_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_uart_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_uart_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_pwm0_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_pwm0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_pwm0_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_pwm0_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c0_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_i2c0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c0_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_i2c0_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_spi0_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_spi0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi0_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_spi0_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sadc_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_sadc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sadc_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_sadc_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_irda_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_irda_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_irda_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_irda_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_sdio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sdio_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_sdio_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_gdma_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_gdma_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_gdma_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_gdma_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_la_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_la_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_la_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_la_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_timer1_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_timer1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer1_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_timer1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_i2c1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_i2c1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart1_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_uart1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart1_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_uart1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_uart2_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_uart2_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart2_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_uart2_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_spi1_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_spi1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi1_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_spi1_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_can_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_can_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_can_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_can_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_usb_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_usb_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_usb_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_usb_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_qspi0_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_qspi0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_qspi0_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_qspi0_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_fft_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_fft_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_fft_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_fft_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_sbc_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_sbc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sbc_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_sbc_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_aud_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_aud_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_aud_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_aud_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_i2s0_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_i2s0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2s0_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_i2s0_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_jpegenc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_jpegenc_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_jpegdec_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_jpegdec_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_display_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_lcd_display_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_lcd_display_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_lcd_display_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_dma2d_int_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_dma2d_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma2d_int_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_dma2d_int_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_wifi_int_phy_mpb_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_wifi_int_phy_mpb_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_wifi_int_phy_riu_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_wifi_int_phy_riu_en;
}

static inline void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(uint32_t v) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_wifi_mac_int_tx_rx_timer_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(void) {
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	return r->cpu1_wifi_mac_int_tx_rx_timer_en;
}

//reg cpu1_int_32_63_en:

static inline void sys_ll_set_cpu1_int_32_63_en_value(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_value(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_tx_rx_misc_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_mac_int_tx_rx_misc_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_rx_trigger_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_mac_int_rx_trigger_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_tx_trigger_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_mac_int_tx_trigger_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_prot_trigger_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_mac_int_prot_trigger_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_gen_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_mac_int_gen_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_hsu_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_hsu_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_int_mac_wakeup_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_wifi_int_mac_wakeup_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_dm_irq_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dm_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dm_irq_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_dm_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_ble_irq_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_ble_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_ble_irq_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_ble_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_bt_irq_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_bt_irq_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bt_irq_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_bt_irq_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_qspi1_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_qspi1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_qspi1_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_qspi1_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_pwm1_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_pwm1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_pwm1_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_pwm1_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_i2s1_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_i2s1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_i2s1_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_i2s1_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_i2s2_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_i2s2_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_i2s2_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_i2s2_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_h264_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_h264_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_h264_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_h264_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_sdmadc_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_sdmadc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_sdmadc_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_sdmadc_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_mbox0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_mbox0_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_mbox1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_mbox1_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_bmc64_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_bmc64_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_dpll_unlock_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dpll_unlock_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dpll_unlock_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_dpll_unlock_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_touched_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_touched_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_touched_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_touched_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_usbplug_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_usbplug_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_rtc_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_rtc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rtc_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_rtc_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_gpio_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_gpio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_gpio_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_gpio_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_dma1_sec_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dma1_sec_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dma1_sec_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_dma1_sec_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_dma1_nsec_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dma1_nsec_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dma1_nsec_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_dma1_nsec_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_yuvb_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_yuvb_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_yuvb_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_yuvb_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_cpu1_rott_int_en(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_rott_int_en = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rott_int_en(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->cpu1_rott_int_en;
}

static inline void sys_ll_set_cpu1_int_32_63_en_reserved0(uint32_t v) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_en_reserved0(void) {
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	return r->reserved0;
}

//reg cpu0_int_0_31_status:

static inline void sys_ll_set_cpu0_int_0_31_status_value(uint32_t v) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_value(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma0_nsec_intr_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_dma0_nsec_intr_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_encp_sec_intr_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_encp_sec_intr_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_encp_nsec_intr_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_encp_nsec_intr_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer0_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_timer0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_uart_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_pwm0_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_pwm0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c0_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_i2c0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi0_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_spi0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_sadc_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_irda_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_irda_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_sdio_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_gdma_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_la_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_timer1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_i2c1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_uart1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_uart2_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_spi1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_can_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_can_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_usb_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_usb_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_qspi0_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_qspi0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_fft_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_fft_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sbc_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_sbc_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_aud_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_aud_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2s0_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_i2s0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_jpegenc_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_jpegdec_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_lcd_display_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_lcd_display_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma2d_int_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_dma2d_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_mpb_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_wifi_int_phy_mpb_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_riu_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_wifi_int_phy_riu_st;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(void) {
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	return r->cpu0_wifi_mac_int_tx_rx_timer_st;
}

//reg cpu0_int_32_63_status:

static inline void sys_ll_set_cpu0_int_32_63_status_value(uint32_t v) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_value(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_rx_misc_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_mac_int_tx_rx_misc_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_mac_int_rx_trigger_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_mac_int_tx_trigger_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_prot_trigger_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_mac_int_prot_trigger_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_mac_int_gen_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_hsu_irq_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_wifi_int_mac_wakeup_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_dm_irq_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_ble_irq_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_bt_irq_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_qspi1_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_qspi1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_pwm1_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_pwm1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_i2s1_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_i2s1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_i2s2_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_i2s2_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_h264_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_h264_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_sdmadc_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_sdmadc_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_mbox0_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_mbox1_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_bmc64_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dpll_unlock_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_dpll_unlock_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_touched_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_touched_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_usbplug_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rtc_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_rtc_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_gpio_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dma1_sec_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_dma1_sec_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dma1_nsec_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_dma1_nsec_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_yuvb_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_yuvb_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rott_int_st(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->cpu0_rott_int_st;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_reserved0(void) {
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	return r->reserved0;
}

//reg cpu1_int_0_31_status:

static inline void sys_ll_set_cpu1_int_0_31_status_value(uint32_t v) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_value(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_dma0_nsec_intr_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_dma0_nsec_intr_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_encp_sec_intr_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_encp_sec_intr_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_encp_nsec_intr_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_encp_nsec_intr_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer0_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_timer0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_uart_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_pwm0_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_pwm0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c0_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_i2c0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi0_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_spi0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sadc_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_sadc_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_irda_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_irda_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sdio_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_sdio_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_gdma_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_gdma_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_la_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_la_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer1_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_timer1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_i2c1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart1_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_uart1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart2_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_uart2_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi1_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_spi1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_can_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_can_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_usb_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_usb_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_qspi0_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_qspi0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_fft_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_fft_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sbc_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_sbc_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_aud_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_aud_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2s0_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_i2s0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_jpegenc_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_jpegdec_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_lcd_display_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_lcd_display_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_dma2d_int_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_dma2d_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_mpb_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_wifi_int_phy_mpb_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_riu_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_wifi_int_phy_riu_st;
}

static inline uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(void) {
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	return r->cpu1_wifi_mac_int_tx_rx_timer_st;
}

//reg cpu1_int_32_63_status:

static inline void sys_ll_set_cpu1_int_32_63_status_value(uint32_t v) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_value(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_rx_misc_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_mac_int_tx_rx_misc_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_mac_int_rx_trigger_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_mac_int_tx_trigger_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_prot_trigger_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_mac_int_prot_trigger_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_mac_int_gen_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_hsu_irq_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_wifi_int_mac_wakeup_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dm_irq_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_dm_irq_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_ble_irq_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_ble_irq_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bt_irq_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_bt_irq_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_qspi1_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_qspi1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_pwm1_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_pwm1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_i2s1_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_i2s1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_i2s2_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_i2s2_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_h264_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_h264_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_sdmadc_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_sdmadc_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_mbox0_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_mbox1_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_bmc64_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dpll_unlock_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_dpll_unlock_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_touched_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_touched_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_usbplug_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rtc_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_rtc_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_gpio_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_gpio_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dma1_sec_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_dma1_sec_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dma1_nsec_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_dma1_nsec_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_yuvb_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_yuvb_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rott_int_st(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->cpu1_rott_int_st;
}

static inline uint32_t sys_ll_get_cpu1_int_32_63_status_reserved0(void) {
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	return r->reserved0;
}

//reg gpio_config0:

static inline void sys_ll_set_gpio_config0_value(uint32_t v) {
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config0_value(void) {
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config0_gpio_config0(uint32_t v) {
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	r->gpio_config0 = v;
}

static inline uint32_t sys_ll_get_gpio_config0_gpio_config0(void) {
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	return r->gpio_config0;
}

//reg gpio_config1:

static inline void sys_ll_set_gpio_config1_value(uint32_t v) {
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config1_value(void) {
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config1_gpio_config1(uint32_t v) {
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	r->gpio_config1 = v;
}

static inline uint32_t sys_ll_get_gpio_config1_gpio_config1(void) {
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	return r->gpio_config1;
}

//reg gpio_config2:

static inline void sys_ll_set_gpio_config2_value(uint32_t v) {
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config2_value(void) {
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config2_gpio_config2(uint32_t v) {
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	r->gpio_config2 = v;
}

static inline uint32_t sys_ll_get_gpio_config2_gpio_config2(void) {
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	return r->gpio_config2;
}

//reg gpio_config3:

static inline void sys_ll_set_gpio_config3_value(uint32_t v) {
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config3_value(void) {
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config3_gpio_config3(uint32_t v) {
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	r->gpio_config3 = v;
}

static inline uint32_t sys_ll_get_gpio_config3_gpio_config3(void) {
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	return r->gpio_config3;
}

//reg gpio_config4:

static inline void sys_ll_set_gpio_config4_value(uint32_t v) {
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config4_value(void) {
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config4_gpio_config4(uint32_t v) {
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	r->gpio_config4 = v;
}

static inline uint32_t sys_ll_get_gpio_config4_gpio_config4(void) {
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	return r->gpio_config4;
}

//reg gpio_config5:

static inline void sys_ll_set_gpio_config5_value(uint32_t v) {
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config5_value(void) {
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config5_gpio_config5(uint32_t v) {
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	r->gpio_config5 = v;
}

static inline uint32_t sys_ll_get_gpio_config5_gpio_config5(void) {
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	return r->gpio_config5;
}

//reg sys_debug_config0:

static inline void sys_ll_set_sys_debug_config0_value(uint32_t v) {
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_sys_debug_config0_value(void) {
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	return r->v;
}

static inline void sys_ll_set_sys_debug_config0_dbug_config0(uint32_t v) {
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	r->dbug_config0 = v;
}

static inline uint32_t sys_ll_get_sys_debug_config0_dbug_config0(void) {
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	return r->dbug_config0;
}

//reg sys_debug_config1:

static inline void sys_ll_set_sys_debug_config1_value(uint32_t v) {
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_sys_debug_config1_value(void) {
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	return r->v;
}

static inline void sys_ll_set_sys_debug_config1_dbug_config1(uint32_t v) {
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	r->dbug_config1 = v;
}

static inline uint32_t sys_ll_get_sys_debug_config1_dbug_config1(void) {
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	return r->dbug_config1;
}

//reg ana_reg0:

static inline void sys_ll_set_ana_reg0_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x40 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg0_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x40 << 2));
}

static inline void sys_ll_set_ana_reg0_dpll_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_dpll_tsten(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->dpll_tsten;
}

static inline void sys_ll_set_ana_reg0_cp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 1, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg0_cp(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->cp;
}

static inline void sys_ll_set_ana_reg0_spideten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_spideten(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->spideten;
}

static inline void sys_ll_set_ana_reg0_hvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 5, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_hvref(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->hvref;
}

static inline void sys_ll_set_ana_reg0_lvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 7, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_lvref(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->lvref;
}

static inline void sys_ll_set_ana_reg0_rzctrl26m(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_rzctrl26m(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->rzctrl26m;
}

static inline void sys_ll_set_ana_reg0_looprzctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 10, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg0_looprzctrl(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->looprzctrl;
}

static inline void sys_ll_set_ana_reg0_rpc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_rpc(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->rpc;
}

static inline void sys_ll_set_ana_reg0_openloop_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_openloop_en(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->openloop_en;
}

static inline void sys_ll_set_ana_reg0_cksel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 17, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_cksel(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->cksel;
}

static inline void sys_ll_set_ana_reg0_spitrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_spitrig(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->spitrig;
}

static inline void sys_ll_set_ana_reg0_band0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_band0(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->band0;
}

static inline void sys_ll_set_ana_reg0_band1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_band1(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->band1;
}

static inline void sys_ll_set_ana_reg0_band(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 22, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg0_band(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->band;
}

static inline void sys_ll_set_ana_reg0_bandmanual(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_bandmanual(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->bandmanual;
}

static inline void sys_ll_set_ana_reg0_dsptrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_dsptrig(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->dsptrig;
}

static inline void sys_ll_set_ana_reg0_lpen_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_lpen_dpll(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->lpen_dpll;
}

static inline void sys_ll_set_ana_reg0_nc_28_30(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 28, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg0_nc_28_30(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->nc_28_30;
}

static inline void sys_ll_set_ana_reg0_vctrl_dpllldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_vctrl_dpllldo(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->vctrl_dpllldo;
}

//reg ana_reg1:

static inline void sys_ll_set_ana_reg1_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x41 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg1_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x41 << 2));
}

static inline void sys_ll_set_ana_reg1_nc3(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 0, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_nc2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 1, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_msw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 2, 0x1ff, v);
}

static inline void sys_ll_set_ana_reg1_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 11, 0x7, v);
}

static inline void sys_ll_set_ana_reg1_osc_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 14, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_osccal_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 15, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_cnti(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 16, 0x1ff, v);
}

static inline void sys_ll_set_ana_reg1_spi_rst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 25, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_amsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 26, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_divctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 27, 0x7, v);
}

static inline void sys_ll_set_ana_reg1_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 30, 0x1, v);
}

static inline void sys_ll_set_ana_reg1_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 31, 0x1, v);
}

//reg ana_reg2:

static inline void sys_ll_set_ana_reg2_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x42 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg2_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x42 << 2));
}

static inline void sys_ll_set_ana_reg2_xtalh_ctune(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 0, 0xff, v);
}

static inline uint32_t sys_ll_get_ana_reg2_xtalh_ctune(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->xtalh_ctune;
}

static inline void sys_ll_set_ana_reg2_force_26mpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_force_26mpll(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->force_26mpll;
}

static inline void sys_ll_set_ana_reg2_gadc_cmp_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 9, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_cmp_ictrl(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_cmp_ictrl;
}

static inline void sys_ll_set_ana_reg2_gadc_inbuf_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 11, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_inbuf_ictrl(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_inbuf_ictrl;
}

static inline void sys_ll_set_ana_reg2_gadc_refbuf_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 13, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_refbuf_ictrl(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_refbuf_ictrl;
}

static inline void sys_ll_set_ana_reg2_gadc_nobuf_enable(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_nobuf_enable(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_nobuf_enable;
}

static inline void sys_ll_set_ana_reg2_vref_scale(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_vref_scale(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->vref_scale;
}

static inline void sys_ll_set_ana_reg2_scal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 17, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_scal_en(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->scal_en;
}

static inline void sys_ll_set_ana_reg2_gadc_capcal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 18, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_capcal_en(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_capcal_en;
}

static inline void sys_ll_set_ana_reg2_gadc_capcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 19, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_capcal(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_capcal;
}

static inline void sys_ll_set_ana_reg2_sp_nt_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 25, 0x7f, v);
}

static inline uint32_t sys_ll_get_ana_reg2_sp_nt_ctrl(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->sp_nt_ctrl;
}

//reg ana_reg3:

static inline void sys_ll_set_ana_reg3_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x43 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg3_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x43 << 2));
}

static inline void sys_ll_set_ana_reg3_usbpen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 0, 0xf, v);
}

static inline void sys_ll_set_ana_reg3_usbnen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 4, 0xf, v);
}

static inline void sys_ll_set_ana_reg3_hpssren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_hpssren(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->hpssren;
}

static inline void sys_ll_set_ana_reg3_ck_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_ck_sel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->ck_sel;
}

static inline void sys_ll_set_ana_reg3_anabuf_sel_rx(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_anabuf_sel_rx(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->anabuf_sel_rx;
}

static inline void sys_ll_set_ana_reg3_pwd_xtalldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_pwd_xtalldo(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->pwd_xtalldo;
}

static inline void sys_ll_set_ana_reg3_iamp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_iamp(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->iamp;
}

static inline void sys_ll_set_ana_reg3_vddren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_vddren(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->vddren;
}

static inline void sys_ll_set_ana_reg3_xamp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 14, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg3_xamp(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->xamp;
}

static inline void sys_ll_set_ana_reg3_vosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 20, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg3_vosel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->vosel;
}

static inline void sys_ll_set_ana_reg3_en_xtalh_sleep(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_en_xtalh_sleep(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->en_xtalh_sleep;
}

static inline void sys_ll_set_ana_reg3_xtal40_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_xtal40_en(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->xtal40_en;
}

static inline void sys_ll_set_ana_reg3_bufictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_bufictrl(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->bufictrl;
}

static inline void sys_ll_set_ana_reg3_ibias_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg3_ibias_ctrl(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->ibias_ctrl;
}

static inline void sys_ll_set_ana_reg3_icore_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 30, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg3_icore_ctrl(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->icore_ctrl;
}

//reg ana_reg4:

static inline void sys_ll_set_ana_reg4_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x44 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg4_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x44 << 2));
}

static inline void sys_ll_set_ana_reg4_anabuf_sel_tx(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_anabuf_sel_tx(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->anabuf_sel_tx;
}

static inline void sys_ll_set_ana_reg4_trng_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_trng_tsten(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->trng_tsten;
}

static inline void sys_ll_set_ana_reg4_itune_ref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 2, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg4_itune_ref(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->itune_ref;
}

static inline void sys_ll_set_ana_reg4_itune_opa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 5, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg4_itune_opa(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->itune_opa;
}

static inline void sys_ll_set_ana_reg4_itune_cmp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 8, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg4_itune_cmp(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->itune_cmp;
}

static inline void sys_ll_set_ana_reg4_rnooise_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_rnooise_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->rnooise_sel;
}

static inline void sys_ll_set_ana_reg4_fslow_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 12, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg4_fslow_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->fslow_sel;
}

static inline void sys_ll_set_ana_reg4_ffast_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 15, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg4_ffast_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->ffast_sel;
}

static inline void sys_ll_set_ana_reg4_trng_tstck_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_trng_tstck_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->trng_tstck_sel;
}

static inline void sys_ll_set_ana_reg4_cktst_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 20, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg4_cktst_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->cktst_sel;
}

static inline void sys_ll_set_ana_reg4_ck_tst_enbale(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_ck_tst_enbale(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->ck_tst_enbale;
}

static inline void sys_ll_set_ana_reg4_sw_bias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_sw_bias(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->sw_bias;
}

static inline void sys_ll_set_ana_reg4_crb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg4_crb(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->crb;
}

static inline void sys_ll_set_ana_reg4_port_enablel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg4_port_enablel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->port_enablel;
}

//reg ana_reg5:

static inline void sys_ll_set_ana_reg5_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x45 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg5_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x45 << 2));
}

static inline void sys_ll_set_ana_reg5_en_usb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_usb(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_usb;
}

static inline void sys_ll_set_ana_reg5_en_xtall(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_xtall(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_xtall;
}

static inline void sys_ll_set_ana_reg5_en_dco(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_dco(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_dco;
}

static inline void sys_ll_set_ana_reg5_en_ram(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 3, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_ram(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_ram;
}

static inline void sys_ll_set_ana_reg5_en_temp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_temp(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_temp;
}

static inline void sys_ll_set_ana_reg5_en_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_dpll(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_dpll;
}

static inline void sys_ll_set_ana_reg5_en_cb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_cb(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_cb;
}

static inline void sys_ll_set_ana_reg5_en_lcd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_lcd(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_lcd;
}

static inline void sys_ll_set_ana_reg5_trxspi_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 8, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg5_trxspi_ctrl(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->trxspi_ctrl;
}

static inline void sys_ll_set_ana_reg5_adc_div(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 10, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg5_adc_div(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->adc_div;
}

static inline void sys_ll_set_ana_reg5_usb_speed(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_usb_speed(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->usb_speed;
}

static inline void sys_ll_set_ana_reg5_spideepsleep(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_spideepsleep(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->spideepsleep;
}

static inline void sys_ll_set_ana_reg5_vsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_vsel(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->vsel;
}

static inline void sys_ll_set_ana_reg5_swb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_swb(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->swb;
}

static inline void sys_ll_set_ana_reg5_itune_xtall(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg5_itune_xtall(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->itune_xtall;
}

static inline void sys_ll_set_ana_reg5_xtall_ten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_xtall_ten(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->xtall_ten;
}

static inline void sys_ll_set_ana_reg5_rosc_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_rosc_tsten(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->rosc_tsten;
}

static inline void sys_ll_set_ana_reg5_bcal_start(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_bcal_start(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->bcal_start;
}

static inline void sys_ll_set_ana_reg5_bcal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_bcal_en(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->bcal_en;
}

static inline void sys_ll_set_ana_reg5_bcal_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 24, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg5_bcal_sel(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->bcal_sel;
}

static inline void sys_ll_set_ana_reg5_vbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 27, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg5_vbias(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->vbias;
}

//reg ana_reg6:

static inline void sys_ll_set_ana_reg6_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x46 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg6_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x46 << 2));
}

static inline void sys_ll_set_ana_reg6_calib_interval(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 0, 0x3ff, v);
}

static inline uint32_t sys_ll_get_ana_reg6_calib_interval(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->calib_interval;
}

static inline void sys_ll_set_ana_reg6_modify_interval(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 10, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg6_modify_interval(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->modify_interval;
}

static inline void sys_ll_set_ana_reg6_xtal_wakeup_time(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg6_xtal_wakeup_time(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->xtal_wakeup_time;
}

static inline void sys_ll_set_ana_reg6_spi_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_spi_trig(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->spi_trig;
}

static inline void sys_ll_set_ana_reg6_modifi_auto(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_modifi_auto(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->modifi_auto;
}

static inline void sys_ll_set_ana_reg6_calib_auto(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_calib_auto(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->calib_auto;
}

static inline void sys_ll_set_ana_reg6_cal_mode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_cal_mode(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->cal_mode;
}

static inline void sys_ll_set_ana_reg6_manu_ena(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_manu_ena(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->manu_ena;
}

static inline void sys_ll_set_ana_reg6_manu_cin(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 25, 0x7f, v);
}

static inline uint32_t sys_ll_get_ana_reg6_manu_cin(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->manu_cin;
}

//reg ana_reg7:

static inline void sys_ll_set_ana_reg7_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x47 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg7_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x47 << 2));
}

static inline void sys_ll_set_ana_reg7_port_enablel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 0, 0xffffffff, v);
}

static inline uint32_t sys_ll_get_ana_reg7_port_enablel(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->port_enablel;
}

//reg ana_reg8:

static inline void sys_ll_set_ana_reg8_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x48 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg8_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x48 << 2));
}

static inline void sys_ll_set_ana_reg8_ioldo_lp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_ioldo_lp(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->ioldo_lp;
}

static inline void sys_ll_set_ana_reg8_coreldo_hp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_coreldo_hp(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->coreldo_hp;
}

static inline void sys_ll_set_ana_reg8_dldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_dldohp(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->dldohp;
}

static inline void sys_ll_set_ana_reg8_t_vanaldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 3, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_t_vanaldosel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->t_vanaldosel;
}

static inline void sys_ll_set_ana_reg8_r_vanaldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 6, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_r_vanaldosel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->r_vanaldosel;
}

static inline void sys_ll_set_ana_reg8_en_trsw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_en_trsw(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->en_trsw;
}

static inline void sys_ll_set_ana_reg8_aldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_aldohp(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->aldohp;
}

static inline void sys_ll_set_ana_reg8_anacurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_anacurlim(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->anacurlim;
}

static inline void sys_ll_set_ana_reg8_violdosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 12, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_violdosel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->violdosel;
}

static inline void sys_ll_set_ana_reg8_iocurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_iocurlim(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->iocurlim;
}

static inline void sys_ll_set_ana_reg8_valoldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 16, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_valoldosel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->valoldosel;
}

static inline void sys_ll_set_ana_reg8_alopowsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_alopowsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->alopowsel;
}

static inline void sys_ll_set_ana_reg8_en_fast_aloldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_en_fast_aloldo(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->en_fast_aloldo;
}

static inline void sys_ll_set_ana_reg8_aloldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_aloldohp(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->aloldohp;
}

static inline void sys_ll_set_ana_reg8_bgcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 22, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg8_bgcal(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->bgcal;
}

static inline void sys_ll_set_ana_reg8_vbgcalmode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_vbgcalmode(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->vbgcalmode;
}

static inline void sys_ll_set_ana_reg8_vbgcalstart(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_vbgcalstart(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->vbgcalstart;
}

static inline void sys_ll_set_ana_reg8_pwd_bgcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_pwd_bgcal(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->pwd_bgcal;
}

static inline void sys_ll_set_ana_reg8_spi_envbg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_spi_envbg(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->spi_envbg;
}

//reg ana_reg9:

static inline void sys_ll_set_ana_reg9_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x49 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg9_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x49 << 2));
}

static inline void sys_ll_set_ana_reg9_wkgpiosel1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg9_wkgpiosel1(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->wkgpiosel1;
}

static inline void sys_ll_set_ana_reg9_rst_wks1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_rst_wks1v(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->rst_wks1v;
}

static inline void sys_ll_set_ana_reg9_wkgpiosel2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 5, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg9_wkgpiosel2(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->wkgpiosel2;
}

static inline void sys_ll_set_ana_reg9_spi_latch1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_spi_latch1v(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->spi_latch1v;
}

static inline void sys_ll_set_ana_reg9_digcurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_digcurlim(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->digcurlim;
}

static inline void sys_ll_set_ana_reg9_pupres_enb1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_pupres_enb1v(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->pupres_enb1v;
}

static inline void sys_ll_set_ana_reg9_pdnres_en1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_pdnres_en1v(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->pdnres_en1v;
}

static inline void sys_ll_set_ana_reg9_d_veasel1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 13, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg9_d_veasel1v(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->d_veasel1v;
}

static inline void sys_ll_set_ana_reg9_ensfsdd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_ensfsdd(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->ensfsdd;
}

static inline void sys_ll_set_ana_reg9_vcorehsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg9_vcorehsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->vcorehsel;
}

static inline void sys_ll_set_ana_reg9_vcorelsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg9_vcorelsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->vcorelsel;
}

static inline void sys_ll_set_ana_reg9_vlden(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_vlden(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->vlden;
}

static inline void sys_ll_set_ana_reg9_en_fast_coreldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_en_fast_coreldo(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->en_fast_coreldo;
}

static inline void sys_ll_set_ana_reg9_pwdcoreldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_pwdcoreldo(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->pwdcoreldo;
}

static inline void sys_ll_set_ana_reg9_vdighsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 26, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg9_vdighsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->vdighsel;
}

static inline void sys_ll_set_ana_reg9_vdigsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 29, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg9_vdigsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->vdigsel;
}

static inline void sys_ll_set_ana_reg9_vdd12lden(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_vdd12lden(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->vdd12lden;
}

//reg ana_reg10:

static inline void sys_ll_set_ana_reg10_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4a << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg10_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4a << 2));
}

static inline void sys_ll_set_ana_reg10_pasoft_st(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg10_pasoft_st(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->pasoft_st;
}

static inline void sys_ll_set_ana_reg10_azcdcnt_manu(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 4, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg10_azcdcnt_manu(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->azcdcnt_manu;
}

static inline void sys_ll_set_ana_reg10_timer_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 7, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg10_timer_sel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->timer_sel;
}

static inline void sys_ll_set_ana_reg10_vpabucksel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 11, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg10_vpabucksel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->vpabucksel;
}

static inline void sys_ll_set_ana_reg10_spi_timerwken(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_spi_timerwken(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->spi_timerwken;
}

static inline void sys_ll_set_ana_reg10_nc_16_16(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_nc_16_16(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->nc_16_16;
}

static inline void sys_ll_set_ana_reg10_sd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 17, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_sd(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->sd;
}

static inline void sys_ll_set_ana_reg10_ioldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 18, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_ioldosel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->ioldosel;
}

static inline void sys_ll_set_ana_reg10_iobyapssen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_iobyapssen(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->iobyapssen;
}

static inline void sys_ll_set_ana_reg10_ckfs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 20, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg10_ckfs(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->ckfs;
}

static inline void sys_ll_set_ana_reg10_ckintsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_ckintsel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->ckintsel;
}

static inline void sys_ll_set_ana_reg10_osccaltrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_osccaltrig(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->osccaltrig;
}

static inline void sys_ll_set_ana_reg10_mroscsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_mroscsel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->mroscsel;
}

static inline void sys_ll_set_ana_reg10_mrosci_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 25, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg10_mrosci_cal(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->mrosci_cal;
}

static inline void sys_ll_set_ana_reg10_mrosccap_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg10_mrosccap_cal(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->mrosccap_cal;
}

//reg ana_reg11:

static inline void sys_ll_set_ana_reg11_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4b << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg11_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4b << 2));
}

static inline void sys_ll_set_ana_reg11_sfsr(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg11_sfsr(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->sfsr;
}

static inline void sys_ll_set_ana_reg11_ensfsaa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_ensfsaa(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->ensfsaa;
}

static inline void sys_ll_set_ana_reg11_apfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 5, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg11_apfms(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->apfms;
}

static inline void sys_ll_set_ana_reg11_atmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 10, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg11_atmpo_sel(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->atmpo_sel;
}

static inline void sys_ll_set_ana_reg11_ampoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_ampoen(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->ampoen;
}

static inline void sys_ll_set_ana_reg11_enpowa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_enpowa(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->enpowa;
}

static inline void sys_ll_set_ana_reg11_avea_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg11_avea_sel(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->avea_sel;
}

static inline void sys_ll_set_ana_reg11_aforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_aforcepfm(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->aforcepfm;
}

static inline void sys_ll_set_ana_reg11_acls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg11_acls(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->acls;
}

static inline void sys_ll_set_ana_reg11_aswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_aswrsten(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->aswrsten;
}

static inline void sys_ll_set_ana_reg11_aripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 21, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg11_aripc(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->aripc;
}

static inline void sys_ll_set_ana_reg11_arampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg11_arampc(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->arampc;
}

static inline void sys_ll_set_ana_reg11_arampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_arampcen(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->arampcen;
}

static inline void sys_ll_set_ana_reg11_aenburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_aenburst(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->aenburst;
}

static inline void sys_ll_set_ana_reg11_apfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_apfmen(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->apfmen;
}

static inline void sys_ll_set_ana_reg11_aldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg11_aldosel(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->aldosel;
}

//reg ana_reg12:

static inline void sys_ll_set_ana_reg12_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4c << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg12_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4c << 2));
}

static inline void sys_ll_set_ana_reg12_buckd_softst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg12_buckd_softst(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->buckd_softst;
}

static inline void sys_ll_set_ana_reg12_dzcdcnt_manu(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 4, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dzcdcnt_manu(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dzcdcnt_manu;
}

static inline void sys_ll_set_ana_reg12_clk_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_clk_sel(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->clk_sel;
}

static inline void sys_ll_set_ana_reg12_dpfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 8, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dpfms(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dpfms;
}

static inline void sys_ll_set_ana_reg12_dtmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 13, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dtmpo_sel(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dtmpo_sel;
}

static inline void sys_ll_set_ana_reg12_dmpoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dmpoen(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dmpoen;
}

static inline void sys_ll_set_ana_reg12_dforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dforcepfm(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dforcepfm;
}

static inline void sys_ll_set_ana_reg12_dcls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dcls(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dcls;
}

static inline void sys_ll_set_ana_reg12_dswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dswrsten(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dswrsten;
}

static inline void sys_ll_set_ana_reg12_dripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 21, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dripc(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dripc;
}

static inline void sys_ll_set_ana_reg12_drampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg12_drampc(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->drampc;
}

static inline void sys_ll_set_ana_reg12_drampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_drampcen(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->drampcen;
}

static inline void sys_ll_set_ana_reg12_denburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_denburst(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->denburst;
}

static inline void sys_ll_set_ana_reg12_dpfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dpfmen(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dpfmen;
}

static inline void sys_ll_set_ana_reg12_dldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg12_dldosel(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->dldosel;
}

//reg ana_reg13:

static inline void sys_ll_set_ana_reg13_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4d << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg13_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4d << 2));
}

static inline void sys_ll_set_ana_reg13_pwdovp1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_pwdovp1v(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->pwdovp1v;
}

static inline void sys_ll_set_ana_reg13_asoft_stc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 1, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg13_asoft_stc(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->asoft_stc;
}

static inline void sys_ll_set_ana_reg13_volen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_volen(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->volen;
}

static inline void sys_ll_set_ana_reg13_dpfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 6, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dpfms(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dpfms;
}

static inline void sys_ll_set_ana_reg13_dtmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 11, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dtmpo_sel(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dtmpo_sel;
}

static inline void sys_ll_set_ana_reg13_dmpoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dmpoen(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dmpoen;
}

static inline void sys_ll_set_ana_reg13_pavea_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg13_pavea_sel(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->pavea_sel;
}

static inline void sys_ll_set_ana_reg13_dforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dforcepfm(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dforcepfm;
}

static inline void sys_ll_set_ana_reg13_dcls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dcls(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dcls;
}

static inline void sys_ll_set_ana_reg13_dswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dswrsten(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dswrsten;
}

static inline void sys_ll_set_ana_reg13_dripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 21, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg13_dripc(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->dripc;
}

static inline void sys_ll_set_ana_reg13_drampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg13_drampc(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->drampc;
}

static inline void sys_ll_set_ana_reg13_drampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_drampcen(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->drampcen;
}

static inline void sys_ll_set_ana_reg13_paenburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_paenburst(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->paenburst;
}

static inline void sys_ll_set_ana_reg13_papfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_papfmen(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->papfmen;
}

static inline void sys_ll_set_ana_reg13_enbuckpa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg13_enbuckpa(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->enbuckpa;
}

//reg ana_reg14:

static inline void sys_ll_set_ana_reg14_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4e << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg14_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4e << 2));
}

static inline void sys_ll_set_ana_reg14_chs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 0, 0xffff, v);
}

static inline void sys_ll_set_ana_reg14_en_lpmod(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 16, 0x1, v);
}

static inline void sys_ll_set_ana_reg14_cal_vth(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 17, 0x7, v);
}

static inline void sys_ll_set_ana_reg14_crg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 20, 0x3, v);
}

static inline void sys_ll_set_ana_reg14_vrefs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 22, 0xf, v);
}

static inline void sys_ll_set_ana_reg14_gain_s(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 26, 0xf, v);
}

static inline void sys_ll_set_ana_reg14_td_latch1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 30, 0x1, v);
}

static inline void sys_ll_set_ana_reg14_pwd_td(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 31, 0x1, v);
}

//reg ana_reg15:

static inline void sys_ll_set_ana_reg15_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4f << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg15_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4f << 2));
}

static inline void sys_ll_set_ana_reg15_cal_number1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 0, 0xf, v);
}

static inline void sys_ll_set_ana_reg15_cal_period1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 4, 0x1ff, v);
}

static inline void sys_ll_set_ana_reg15_test_number1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 13, 0xf, v);
}

static inline void sys_ll_set_ana_reg15_test_period1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 17, 0xf, v);
}

static inline void sys_ll_set_ana_reg15_nc_21_21(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 21, 0x1, v);
}

static inline void sys_ll_set_ana_reg15_chs_sel_cal1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 22, 0xf, v);
}

static inline void sys_ll_set_ana_reg15_cal_done_clr1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 26, 0x1, v);
}

static inline void sys_ll_set_ana_reg15_en_cal_force1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 27, 0x1, v);
}

static inline void sys_ll_set_ana_reg15_en_cal_auto1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 28, 0x1, v);
}

static inline void sys_ll_set_ana_reg15_en_scm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 29, 0x1, v);
}

static inline void sys_ll_set_ana_reg15_en_adcmod(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 30, 0x1, v);
}

static inline void sys_ll_set_ana_reg15_enfsr1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 31, 0x1, v);
}

//reg ana_reg16:

static inline void sys_ll_set_ana_reg16_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x50 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg16_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x50 << 2));
}

static inline void sys_ll_set_ana_reg16_int_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 0, 0x3ff, v);
}

static inline void sys_ll_set_ana_reg16_int_en16(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 10, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_nc_11_15(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 11, 0x1f, v);
}

static inline void sys_ll_set_ana_reg16_ckadc_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 16, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_int_clr_sel1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 17, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_ctrl_ck2d(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 18, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_ctrl_seri_cap(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 19, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_en_testcmp1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 20, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_en_man_wr1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 21, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_en_manmod1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 22, 0x1, v);
}

static inline void sys_ll_set_ana_reg16_cap_calspi1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 23, 0x1ff, v);
}

//reg ana_reg17:

static inline void sys_ll_set_ana_reg17_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x51 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg17_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x51 << 2));
}

static inline void sys_ll_set_ana_reg17_int_clr(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 0, 0x3ff, v);
}

static inline void sys_ll_set_ana_reg17_int_clr16(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 10, 0x1, v);
}

static inline void sys_ll_set_ana_reg17_nc_11_11(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 11, 0x1, v);
}

static inline void sys_ll_set_ana_reg17_int_clr_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 12, 0x3ff, v);
}

static inline void sys_ll_set_ana_reg17_int_en_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 22, 0x3ff, v);
}

//reg ana_reg18:

static inline void sys_ll_set_ana_reg18_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x52 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg18_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x52 << 2));
}

static inline void sys_ll_set_ana_reg18_iselaud(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_iselaud(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->iselaud;
}

static inline void sys_ll_set_ana_reg18_audck_rlcen1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_audck_rlcen1v(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->audck_rlcen1v;
}

static inline void sys_ll_set_ana_reg18_lchckinven1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_lchckinven1v(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->lchckinven1v;
}

static inline void sys_ll_set_ana_reg18_enaudbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 3, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_enaudbias(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->enaudbias;
}

static inline void sys_ll_set_ana_reg18_enadcbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_enadcbias(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->enadcbias;
}

static inline void sys_ll_set_ana_reg18_enmicbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_enmicbias(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->enmicbias;
}

static inline void sys_ll_set_ana_reg18_adcckinven1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_adcckinven1v(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->adcckinven1v;
}

static inline void sys_ll_set_ana_reg18_dacfb2st0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_dacfb2st0v9(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->dacfb2st0v9;
}

static inline void sys_ll_set_ana_reg18_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_nc1(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->nc1;
}

static inline void sys_ll_set_ana_reg18_micbias_trm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 9, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg18_micbias_trm(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->micbias_trm;
}

static inline void sys_ll_set_ana_reg18_micbias_voc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 11, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg18_micbias_voc(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->micbias_voc;
}

static inline void sys_ll_set_ana_reg18_vrefsel1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg18_vrefsel1v(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->vrefsel1v;
}

static inline void sys_ll_set_ana_reg18_capswspi(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 17, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg18_capswspi(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->capswspi;
}

static inline void sys_ll_set_ana_reg18_adref_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 22, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg18_adref_sel(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->adref_sel;
}

static inline void sys_ll_set_ana_reg18_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 24, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg18_nc0(void) {
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	return r->nc0;
}

static inline void sys_ll_set_ana_reg18_spi_dacckpssel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 31, 0x1, v);
}

//reg ana_reg19:

static inline void sys_ll_set_ana_reg19_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x53 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg19_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x53 << 2));
}

static inline void sys_ll_set_ana_reg19_isel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 0, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg19_isel(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->isel;
}

static inline void sys_ll_set_ana_reg19_micirsel1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micirsel1(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micirsel1;
}

static inline void sys_ll_set_ana_reg19_micdacit(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 3, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micdacit(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micdacit;
}

static inline void sys_ll_set_ana_reg19_micdacih(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 5, 0xff, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micdacih(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micdacih;
}

static inline void sys_ll_set_ana_reg19_micsingleen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micsingleen(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micsingleen;
}

static inline void sys_ll_set_ana_reg19_dccompen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_dccompen(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->dccompen;
}

static inline void sys_ll_set_ana_reg19_micgain(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 15, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micgain(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micgain;
}

static inline void sys_ll_set_ana_reg19_micdacen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micdacen(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micdacen;
}

static inline void sys_ll_set_ana_reg19_stg2lsen1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_stg2lsen1v(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->stg2lsen1v;
}

static inline void sys_ll_set_ana_reg19_openloopcal1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_openloopcal1v(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->openloopcal1v;
}

static inline void sys_ll_set_ana_reg19_callatch(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_callatch(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->callatch;
}

static inline void sys_ll_set_ana_reg19_vcmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_vcmsel(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->vcmsel;
}

static inline void sys_ll_set_ana_reg19_dwamode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_dwamode(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->dwamode;
}

static inline void sys_ll_set_ana_reg19_r2ren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_r2ren(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->r2ren;
}

static inline void sys_ll_set_ana_reg19_nc_26_27(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 26, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg19_nc_26_27(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->nc_26_27;
}

static inline void sys_ll_set_ana_reg19_micen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_micen(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->micen;
}

static inline void sys_ll_set_ana_reg19_rst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_rst(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->rst;
}

static inline void sys_ll_set_ana_reg19_bpdwa1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_bpdwa1v(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->bpdwa1v;
}

static inline void sys_ll_set_ana_reg19_hcen1stg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg19_hcen1stg(void) {
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	return r->hcen1stg;
}

//reg ana_reg20:

static inline void sys_ll_set_ana_reg20_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x54 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg20_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x54 << 2));
}

static inline void sys_ll_set_ana_reg20_hpdac(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_hpdac(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->hpdac;
}

static inline void sys_ll_set_ana_reg20_calcon_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_calcon_sel(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->calcon_sel;
}

static inline void sys_ll_set_ana_reg20_oscdac(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 2, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg20_oscdac(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->oscdac;
}

static inline void sys_ll_set_ana_reg20_ocendac(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_ocendac(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->ocendac;
}

static inline void sys_ll_set_ana_reg20_vcmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_vcmsel(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->vcmsel;
}

static inline void sys_ll_set_ana_reg20_adjdacref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 6, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg20_adjdacref(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->adjdacref;
}

static inline void sys_ll_set_ana_reg20_dcochg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 11, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg20_dcochg(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->dcochg;
}

static inline void sys_ll_set_ana_reg20_diffen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_diffen(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->diffen;
}

static inline void sys_ll_set_ana_reg20_endaccal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_endaccal(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->endaccal;
}

static inline void sys_ll_set_ana_reg20_nc2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_nc2(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->nc2;
}

static inline void sys_ll_set_ana_reg20_lendcoc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_lendcoc(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->lendcoc;
}

static inline void sys_ll_set_ana_reg20_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 17, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_nc1(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->nc1;
}

static inline void sys_ll_set_ana_reg20_lenvcmd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 18, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_lenvcmd(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->lenvcmd;
}

static inline void sys_ll_set_ana_reg20_dacdrven(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_dacdrven(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->dacdrven;
}

static inline void sys_ll_set_ana_reg20_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_nc0(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->nc0;
}

static inline void sys_ll_set_ana_reg20_daclen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_daclen(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->daclen;
}

static inline void sys_ll_set_ana_reg20_dacg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 22, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg20_dacg(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->dacg;
}

static inline void sys_ll_set_ana_reg20_dacmute(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_dacmute(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->dacmute;
}

static inline void sys_ll_set_ana_reg20_dacdwamode_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg20_dacdwamode_sel(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->dacdwamode_sel;
}

static inline void sys_ll_set_ana_reg20_dacsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg20_dacsel(void) {
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	return r->dacsel;
}

//reg ana_reg21:

static inline void sys_ll_set_ana_reg21_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x55 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg21_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x55 << 2));
}

static inline void sys_ll_set_ana_reg21_lmdcin(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 0, 0xff, v);
}

static inline uint32_t sys_ll_get_ana_reg21_lmdcin(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->lmdcin;
}

static inline void sys_ll_set_ana_reg21_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 8, 0xff, v);
}

static inline uint32_t sys_ll_get_ana_reg21_nc1(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->nc1;
}

static inline void sys_ll_set_ana_reg21_spirst_ovc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_spirst_ovc(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->spirst_ovc;
}

static inline void sys_ll_set_ana_reg21_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 17, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_nc0(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->nc0;
}

static inline void sys_ll_set_ana_reg21_enidacl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 18, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_enidacl(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->enidacl;
}

static inline void sys_ll_set_ana_reg21_dac3rdhc0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_dac3rdhc0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->dac3rdhc0v9;
}

static inline void sys_ll_set_ana_reg21_hc2s(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_hc2s(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->hc2s;
}

static inline void sys_ll_set_ana_reg21_rfb_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_rfb_ctrl(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->rfb_ctrl;
}

static inline void sys_ll_set_ana_reg21_vcmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_vcmsel(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->vcmsel;
}

static inline void sys_ll_set_ana_reg21_enbs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_enbs(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->enbs;
}

static inline void sys_ll_set_ana_reg21_calck_sel0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_calck_sel0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->calck_sel0v9;
}

static inline void sys_ll_set_ana_reg21_bpdwa0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_bpdwa0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->bpdwa0v9;
}

static inline void sys_ll_set_ana_reg21_looprst0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_looprst0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->looprst0v9;
}

static inline void sys_ll_set_ana_reg21_oct0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 27, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg21_oct0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->oct0v9;
}

static inline void sys_ll_set_ana_reg21_sout0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg21_sout0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->sout0v9;
}

static inline void sys_ll_set_ana_reg21_hc0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 30, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg21_hc0v9(void) {
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	return r->hc0v9;
}

//reg ana_reg22:

static inline void sys_ll_set_ana_reg22_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x56 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg22_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x56 << 2));
}

static inline void sys_ll_set_ana_reg22_ictrl_dsppll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg22_ictrl_dsppll(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->ictrl_dsppll;
}

static inline void sys_ll_set_ana_reg22_nc_4_18(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 4, 0x7fff, v);
}

static inline uint32_t sys_ll_get_ana_reg22_nc_4_18(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->nc_4_18;
}

static inline void sys_ll_set_ana_reg22_mode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg22_mode(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->mode;
}

static inline void sys_ll_set_ana_reg22_iamsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg22_iamsel(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->iamsel;
}

static inline void sys_ll_set_ana_reg22_hvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 21, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg22_hvref(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->hvref;
}

static inline void sys_ll_set_ana_reg22_lvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 23, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg22_lvref(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->lvref;
}

static inline void sys_ll_set_ana_reg22_nc_25_31(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 25, 0x7f, v);
}

static inline uint32_t sys_ll_get_ana_reg22_nc_25_31(void) {
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	return r->nc_25_31;
}

//reg ana_reg23:

static inline void sys_ll_set_ana_reg23_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x57 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg23_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x57 << 2));
}

static inline void sys_ll_set_ana_reg23_camsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_camsel(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->camsel;
}

static inline void sys_ll_set_ana_reg23_msw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 1, 0x1ff, v);
}

static inline uint32_t sys_ll_get_ana_reg23_msw(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->msw;
}

static inline void sys_ll_set_ana_reg23_tstcken_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_tstcken_dpll(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->tstcken_dpll;
}

static inline void sys_ll_set_ana_reg23_osccal_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_osccal_trig(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->osccal_trig;
}

static inline void sys_ll_set_ana_reg23_cnti(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 12, 0x1ff, v);
}

static inline uint32_t sys_ll_get_ana_reg23_cnti(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->cnti;
}

static inline void sys_ll_set_ana_reg23_nc_21_21(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_nc_21_21(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->nc_21_21;
}

static inline void sys_ll_set_ana_reg23_spi_rst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_spi_rst(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->spi_rst;
}

static inline void sys_ll_set_ana_reg23_closeloop_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_closeloop_en(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->closeloop_en;
}

static inline void sys_ll_set_ana_reg23_caltime(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_caltime(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->caltime;
}

static inline void sys_ll_set_ana_reg23_lpfrz(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 25, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg23_lpfrz(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->lpfrz;
}

static inline void sys_ll_set_ana_reg23_icp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 27, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg23_icp(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->icp;
}

static inline void sys_ll_set_ana_reg23_cp2ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg23_cp2ctrl(void) {
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	return r->cp2ctrl;
}

#ifdef __cplusplus
}
#endif
