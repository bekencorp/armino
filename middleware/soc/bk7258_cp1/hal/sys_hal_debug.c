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

#include "hal_config.h"
#include "system_hw.h"
#include "sys_hal.h"

#if CONFIG_HAL_DEBUG_SYS

typedef void (*sys_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	sys_dump_fn_t fn;
} sys_reg_fn_map_t;

static void sys_dump_device_id(void)
{
	SOC_LOGI("device_id: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x0 << 2)));
}

static void sys_dump_version_id(void)
{
	SOC_LOGI("version_id: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x1 << 2)));
}

static void sys_dump_cpu_storage_connect_op_select(void)
{
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t *)(SOC_SYS_REG_BASE + (0x2 << 2));

	SOC_LOGI("cpu_storage_connect_op_select: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x2 << 2)));
	SOC_LOGI("	boot_mode: %8x\r\n", r->boot_mode);
	SOC_LOGI("	reserved_bit_1_7: %8x\r\n", r->reserved_bit_1_7);
	SOC_LOGI("	jtag_core_sel: %8x\r\n", r->jtag_core_sel);
	SOC_LOGI("	flash_sel: %8x\r\n", r->flash_sel);
	SOC_LOGI("	reserved_bit_10_31: %8x\r\n", r->reserved_bit_10_31);
}

static void sys_dump_cpu_current_run_status(void)
{
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t *)(SOC_SYS_REG_BASE + (0x3 << 2));

	SOC_LOGI("cpu_current_run_status: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x3 << 2)));
	SOC_LOGI("	core0_halted: %8x\r\n", r->core0_halted);
	SOC_LOGI("	core1_halted: %8x\r\n", r->core1_halted);
	SOC_LOGI("	reserved_bit_2_3: %8x\r\n", r->reserved_bit_2_3);
	SOC_LOGI("	cpu0_sw_reset: %8x\r\n", r->cpu0_sw_reset);
	SOC_LOGI("	cpu1_sw_reset: %8x\r\n", r->cpu1_sw_reset);
	SOC_LOGI("	reserved_bit_6_7: %8x\r\n", r->reserved_bit_6_7);
	SOC_LOGI("	cpu0_pwr_dw_state: %8x\r\n", r->cpu0_pwr_dw_state);
	SOC_LOGI("	cpu1_pwr_dw_state: %8x\r\n", r->cpu1_pwr_dw_state);
	SOC_LOGI("	reserved_bit_10_31: %8x\r\n", r->reserved_bit_10_31);
}

static void sys_dump_cpu0_int_halt_clk_op(void)
{
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t *)(SOC_SYS_REG_BASE + (0x4 << 2));

	SOC_LOGI("cpu0_int_halt_clk_op: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4 << 2)));
	SOC_LOGI("	cpu0_sw_rst: %8x\r\n", r->cpu0_sw_rst);
	SOC_LOGI("	cpu0_pwr_dw: %8x\r\n", r->cpu0_pwr_dw);
	SOC_LOGI("	cpu0_int_mask: %8x\r\n", r->cpu0_int_mask);
	SOC_LOGI("	cpu0_halt: %8x\r\n", r->cpu0_halt);
	SOC_LOGI("	reserved_4_7: %8x\r\n", r->reserved_4_7);
	SOC_LOGI("	cpu0_offset: %8x\r\n", r->cpu0_offset);
}

static void sys_dump_cpu1_int_halt_clk_op(void)
{
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t *)(SOC_SYS_REG_BASE + (0x5 << 2));

	SOC_LOGI("cpu1_int_halt_clk_op: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x5 << 2)));
	SOC_LOGI("	cpu1_sw_rst: %8x\r\n", r->cpu1_sw_rst);
	SOC_LOGI("	cpu1_pwr_dw: %8x\r\n", r->cpu1_pwr_dw);
	SOC_LOGI("	cpu1_int_mask: %8x\r\n", r->cpu1_int_mask);
	SOC_LOGI("	cpu1_halt: %8x\r\n", r->cpu1_halt);
	SOC_LOGI("	cpu0_bus_clk_2div: %8x\r\n", r->cpu0_bus_clk_2div);
	SOC_LOGI("	reserved_5_7: %8x\r\n", r->reserved_5_7);
	SOC_LOGI("	cpu1_offset: %8x\r\n", r->cpu1_offset);
}

static void sys_dump_reserved_reg0x6(void)
{
	SOC_LOGI("reserved_reg0x6: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x6 << 2)));
}

static void sys_dump_rsv_7_7(void)
{
	for (uint32_t idx = 0; idx < 1; idx++) {
		SOC_LOGI("rsv_7_7: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + ((0x7 + idx) << 2)));
	}
}

static void sys_dump_cpu_clk_div_mode1(void)
{
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t *)(SOC_SYS_REG_BASE + (0x8 << 2));

	SOC_LOGI("cpu_clk_div_mode1: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x8 << 2)));
	SOC_LOGI("	clkdiv_core: %8x\r\n", r->clkdiv_core);
	SOC_LOGI("	cksel_core: %8x\r\n", r->cksel_core);
	SOC_LOGI("	clkdiv_bus: %8x\r\n", r->clkdiv_bus);
	SOC_LOGI("	reserved_bit_7_7: %8x\r\n", r->reserved_bit_7_7);
	SOC_LOGI("	clkdiv_uart0: %8x\r\n", r->clkdiv_uart0);
	SOC_LOGI("	clksel_uart0: %8x\r\n", r->clksel_uart0);
	SOC_LOGI("	clkdiv_uart1: %8x\r\n", r->clkdiv_uart1);
	SOC_LOGI("	cksel_uart1: %8x\r\n", r->cksel_uart1);
	SOC_LOGI("	clkdiv_uart2: %8x\r\n", r->clkdiv_uart2);
	SOC_LOGI("	cksel_uart2: %8x\r\n", r->cksel_uart2);
	SOC_LOGI("	cksel_sadc: %8x\r\n", r->cksel_sadc);
	SOC_LOGI("	cksel_pwm0: %8x\r\n", r->cksel_pwm0);
	SOC_LOGI("	cksel_pwm1: %8x\r\n", r->cksel_pwm1);
	SOC_LOGI("	cksel_tim0: %8x\r\n", r->cksel_tim0);
	SOC_LOGI("	cksel_tim1: %8x\r\n", r->cksel_tim1);
	SOC_LOGI("	cksel_tim2: %8x\r\n", r->cksel_tim2);
	SOC_LOGI("	reserved_bit_23_23: %8x\r\n", r->reserved_bit_23_23);
	SOC_LOGI("	cksel_i2s: %8x\r\n", r->cksel_i2s);
	SOC_LOGI("	cksel_aud: %8x\r\n", r->cksel_aud);
	SOC_LOGI("	clkdiv_jpeg: %8x\r\n", r->clkdiv_jpeg);
	SOC_LOGI("	cksel_jpeg: %8x\r\n", r->cksel_jpeg);
	SOC_LOGI("	clkdiv_disp_l: %8x\r\n", r->clkdiv_disp_l);
}

static void sys_dump_cpu_clk_div_mode2(void)
{
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t *)(SOC_SYS_REG_BASE + (0x9 << 2));

	SOC_LOGI("cpu_clk_div_mode2: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x9 << 2)));
	SOC_LOGI("	clkdiv_disp_h: %8x\r\n", r->clkdiv_disp_h);
	SOC_LOGI("	cksel_disp: %8x\r\n", r->cksel_disp);
	SOC_LOGI("	ckdiv_psram: %8x\r\n", r->ckdiv_psram);
	SOC_LOGI("	cksel_psram: %8x\r\n", r->cksel_psram);
	SOC_LOGI("	ckdiv_qspi0: %8x\r\n", r->ckdiv_qspi0);
	SOC_LOGI("	cksel_qspi0: %8x\r\n", r->cksel_qspi0);
	SOC_LOGI("	reserved_11_13: %8x\r\n", r->reserved_11_13);
	SOC_LOGI("	ckdiv_sdio: %8x\r\n", r->ckdiv_sdio);
	SOC_LOGI("	cksel_sdio: %8x\r\n", r->cksel_sdio);
	SOC_LOGI("	ckdiv_auxs: %8x\r\n", r->ckdiv_auxs);
	SOC_LOGI("	cksel_auxs: %8x\r\n", r->cksel_auxs);
	SOC_LOGI("	cksel_flash: %8x\r\n", r->cksel_flash);
	SOC_LOGI("	ckdiv_flash: %8x\r\n", r->ckdiv_flash);
	SOC_LOGI("	ckdiv_i2s0: %8x\r\n", r->ckdiv_i2s0);
	SOC_LOGI("	reserved_bit_31_31: %8x\r\n", r->reserved_bit_31_31);
}

static void sys_dump_cpu_26m_wdt_clk_div(void)
{
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t *)(SOC_SYS_REG_BASE + (0xa << 2));

	SOC_LOGI("cpu_26m_wdt_clk_div: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0xa << 2)));
	SOC_LOGI("	ckdiv_26m: %8x\r\n", r->ckdiv_26m);
	SOC_LOGI("	ckdiv_wdt: %8x\r\n", r->ckdiv_wdt);
	SOC_LOGI("	clksel_spi0: %8x\r\n", r->clksel_spi0);
	SOC_LOGI("	clksel_spi1: %8x\r\n", r->clksel_spi1);
	SOC_LOGI("	ckdiv_qspi1: %8x\r\n", r->ckdiv_qspi1);
	SOC_LOGI("	cksel_qspi1: %8x\r\n", r->cksel_qspi1);
	SOC_LOGI("	reserved_bit_11_31: %8x\r\n", r->reserved_bit_11_31);
}

static void sys_dump_cpu_anaspi_freq(void)
{
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t *)(SOC_SYS_REG_BASE + (0xb << 2));

	SOC_LOGI("cpu_anaspi_freq: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0xb << 2)));
	SOC_LOGI("	anaspi_freq: %8x\r\n", r->anaspi_freq);
	SOC_LOGI("	reserved_bit_6_31: %8x\r\n", r->reserved_bit_6_31);
}

static void sys_dump_cpu_device_clk_enable(void)
{
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t *)(SOC_SYS_REG_BASE + (0xc << 2));

	SOC_LOGI("cpu_device_clk_enable: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0xc << 2)));
	SOC_LOGI("	i2c0_cken: %8x\r\n", r->i2c0_cken);
	SOC_LOGI("	spi0_cken: %8x\r\n", r->spi0_cken);
	SOC_LOGI("	uart0_cken: %8x\r\n", r->uart0_cken);
	SOC_LOGI("	pwm0_cken: %8x\r\n", r->pwm0_cken);
	SOC_LOGI("	tim0_cken: %8x\r\n", r->tim0_cken);
	SOC_LOGI("	sadc_cken: %8x\r\n", r->sadc_cken);
	SOC_LOGI("	irda_cken: %8x\r\n", r->irda_cken);
	SOC_LOGI("	efuse_cken: %8x\r\n", r->efuse_cken);
	SOC_LOGI("	i2c1_cken: %8x\r\n", r->i2c1_cken);
	SOC_LOGI("	spi1_cken: %8x\r\n", r->spi1_cken);
	SOC_LOGI("	uart1_cken: %8x\r\n", r->uart1_cken);
	SOC_LOGI("	uart2_cken: %8x\r\n", r->uart2_cken);
	SOC_LOGI("	pwm1_cken: %8x\r\n", r->pwm1_cken);
	SOC_LOGI("	tim1_cken: %8x\r\n", r->tim1_cken);
	SOC_LOGI("	tim2_cken: %8x\r\n", r->tim2_cken);
	SOC_LOGI("	otp_cken: %8x\r\n", r->otp_cken);
	SOC_LOGI("	i2s_cken: %8x\r\n", r->i2s_cken);
	SOC_LOGI("	usb_cken: %8x\r\n", r->usb_cken);
	SOC_LOGI("	can_cken: %8x\r\n", r->can_cken);
	SOC_LOGI("	psram_cken: %8x\r\n", r->psram_cken);
	SOC_LOGI("	qspi0_cken: %8x\r\n", r->qspi0_cken);
	SOC_LOGI("	qspi1_cken: %8x\r\n", r->qspi1_cken);
	SOC_LOGI("	sdio_cken: %8x\r\n", r->sdio_cken);
	SOC_LOGI("	auxs_cken: %8x\r\n", r->auxs_cken);
	SOC_LOGI("	btdm_cken: %8x\r\n", r->btdm_cken);
	SOC_LOGI("	xvr_cken: %8x\r\n", r->xvr_cken);
	SOC_LOGI("	mac_cken: %8x\r\n", r->mac_cken);
	SOC_LOGI("	phy_cken: %8x\r\n", r->phy_cken);
	SOC_LOGI("	jpeg_cken: %8x\r\n", r->jpeg_cken);
	SOC_LOGI("	disp_cken: %8x\r\n", r->disp_cken);
	SOC_LOGI("	aud_cken: %8x\r\n", r->aud_cken);
	SOC_LOGI("	wdt_cken: %8x\r\n", r->wdt_cken);
}

static void sys_dump_reserver_reg0xd(void)
{
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t *)(SOC_SYS_REG_BASE + (0xd << 2));

	SOC_LOGI("reserver_reg0xd: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0xd << 2)));
	SOC_LOGI("	h264_cken: %8x\r\n", r->h264_cken);
	SOC_LOGI("	i2s1_cken: %8x\r\n", r->i2s1_cken);
	SOC_LOGI("	i2s2_cken: %8x\r\n", r->i2s2_cken);
	SOC_LOGI("	yuv_cken: %8x\r\n", r->yuv_cken);
	SOC_LOGI("	slcd_cken: %8x\r\n", r->slcd_cken);
	SOC_LOGI("	reserved_5_31: %8x\r\n", r->reserved_5_31);
}

static void sys_dump_cpu_device_mem_ctrl1(void)
{
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t *)(SOC_SYS_REG_BASE + (0xe << 2));

	SOC_LOGI("cpu_device_mem_ctrl1: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0xe << 2)));
	SOC_LOGI("	uart1_mem_sd: %8x\r\n", r->uart1_mem_sd);
	SOC_LOGI("	uart2: %8x\r\n", r->uart2);
	SOC_LOGI("	spi1: %8x\r\n", r->spi1);
	SOC_LOGI("	sdio: %8x\r\n", r->sdio);
	SOC_LOGI("	usb: %8x\r\n", r->usb);
	SOC_LOGI("	can: %8x\r\n", r->can);
	SOC_LOGI("	qspi0: %8x\r\n", r->qspi0);
	SOC_LOGI("	qspi1: %8x\r\n", r->qspi1);
	SOC_LOGI("	pram: %8x\r\n", r->pram);
	SOC_LOGI("	fft: %8x\r\n", r->fft);
	SOC_LOGI("	abc: %8x\r\n", r->abc);
	SOC_LOGI("	aud: %8x\r\n", r->aud);
	SOC_LOGI("	i2s0: %8x\r\n", r->i2s0);
	SOC_LOGI("	i2s1: %8x\r\n", r->i2s1);
	SOC_LOGI("	i2s2: %8x\r\n", r->i2s2);
	SOC_LOGI("	jpge: %8x\r\n", r->jpge);
	SOC_LOGI("	yuv: %8x\r\n", r->yuv);
	SOC_LOGI("	jpgd: %8x\r\n", r->jpgd);
	SOC_LOGI("	disp: %8x\r\n", r->disp);
	SOC_LOGI("	dmad: %8x\r\n", r->dmad);
	SOC_LOGI("	h26f: %8x\r\n", r->h26f);
	SOC_LOGI("	mac: %8x\r\n", r->mac);
	SOC_LOGI("	phy: %8x\r\n", r->phy);
	SOC_LOGI("	xvr: %8x\r\n", r->xvr);
	SOC_LOGI("	irda: %8x\r\n", r->irda);
	SOC_LOGI("	la: %8x\r\n", r->la);
	SOC_LOGI("	flsh: %8x\r\n", r->flsh);
	SOC_LOGI("	uart: %8x\r\n", r->uart);
	SOC_LOGI("	spi0: %8x\r\n", r->spi0);
	SOC_LOGI("	enc: %8x\r\n", r->enc);
	SOC_LOGI("	dma0: %8x\r\n", r->dma0);
	SOC_LOGI("	dma1: %8x\r\n", r->dma1);
}

static void sys_dump_cpu_device_mem_ctrl2(void)
{
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t *)(SOC_SYS_REG_BASE + (0xf << 2));

	SOC_LOGI("cpu_device_mem_ctrl2: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0xf << 2)));
	SOC_LOGI("	uart1_mem_ds: %8x\r\n", r->uart1_mem_ds);
	SOC_LOGI("	uart2: %8x\r\n", r->uart2);
	SOC_LOGI("	spi1: %8x\r\n", r->spi1);
	SOC_LOGI("	sdio: %8x\r\n", r->sdio);
	SOC_LOGI("	usb: %8x\r\n", r->usb);
	SOC_LOGI("	can: %8x\r\n", r->can);
	SOC_LOGI("	qspi0: %8x\r\n", r->qspi0);
	SOC_LOGI("	qspi1: %8x\r\n", r->qspi1);
	SOC_LOGI("	pram: %8x\r\n", r->pram);
	SOC_LOGI("	fft: %8x\r\n", r->fft);
	SOC_LOGI("	abc: %8x\r\n", r->abc);
	SOC_LOGI("	aud: %8x\r\n", r->aud);
	SOC_LOGI("	i2s0: %8x\r\n", r->i2s0);
	SOC_LOGI("	i2s1: %8x\r\n", r->i2s1);
	SOC_LOGI("	i2s2: %8x\r\n", r->i2s2);
	SOC_LOGI("	jpge: %8x\r\n", r->jpge);
	SOC_LOGI("	yuv: %8x\r\n", r->yuv);
	SOC_LOGI("	jpgd: %8x\r\n", r->jpgd);
	SOC_LOGI("	disp: %8x\r\n", r->disp);
	SOC_LOGI("	dmad: %8x\r\n", r->dmad);
	SOC_LOGI("	h26f: %8x\r\n", r->h26f);
	SOC_LOGI("	mac: %8x\r\n", r->mac);
	SOC_LOGI("	phy: %8x\r\n", r->phy);
	SOC_LOGI("	xvr: %8x\r\n", r->xvr);
	SOC_LOGI("	irda: %8x\r\n", r->irda);
	SOC_LOGI("	la: %8x\r\n", r->la);
	SOC_LOGI("	flsh: %8x\r\n", r->flsh);
	SOC_LOGI("	uart: %8x\r\n", r->uart);
	SOC_LOGI("	spi0: %8x\r\n", r->spi0);
	SOC_LOGI("	enc: %8x\r\n", r->enc);
	SOC_LOGI("	dma0: %8x\r\n", r->dma0);
	SOC_LOGI("	dma1: %8x\r\n", r->dma1);
}

static void sys_dump_cpu_power_sleep_wakeup(void)
{
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t *)(SOC_SYS_REG_BASE + (0x10 << 2));

	SOC_LOGI("cpu_power_sleep_wakeup: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x10 << 2)));
	SOC_LOGI("	pwd_mem1: %8x\r\n", r->pwd_mem1);
	SOC_LOGI("	pwd_mem2: %8x\r\n", r->pwd_mem2);
	SOC_LOGI("	pwd_mem3: %8x\r\n", r->pwd_mem3);
	SOC_LOGI("	pwd_encp: %8x\r\n", r->pwd_encp);
	SOC_LOGI("	pwd_bakp: %8x\r\n", r->pwd_bakp);
	SOC_LOGI("	pwd_ahbp: %8x\r\n", r->pwd_ahbp);
	SOC_LOGI("	pwd_audp: %8x\r\n", r->pwd_audp);
	SOC_LOGI("	pwd_vidp: %8x\r\n", r->pwd_vidp);
	SOC_LOGI("	pwd_btsp: %8x\r\n", r->pwd_btsp);
	SOC_LOGI("	pwd_wifp_mac: %8x\r\n", r->pwd_wifp_mac);
	SOC_LOGI("	pwd_wifp_phy: %8x\r\n", r->pwd_wifp_phy);
	SOC_LOGI("	pwd_mem0: %8x\r\n", r->pwd_mem0);
	SOC_LOGI("	pwd_mem4: %8x\r\n", r->pwd_mem4);
	SOC_LOGI("	pwd_ofdm: %8x\r\n", r->pwd_ofdm);
	SOC_LOGI("	tcm0_pgen: %8x\r\n", r->tcm0_pgen);
	SOC_LOGI("	rom_pgen: %8x\r\n", r->rom_pgen);
	SOC_LOGI("	sleep_en_need_flash_idle: %8x\r\n", r->sleep_en_need_flash_idle);
	SOC_LOGI("	sleep_en_need_cpu1_wfi: %8x\r\n", r->sleep_en_need_cpu1_wfi);
	SOC_LOGI("	sleep_en_need_cpu0_wfi: %8x\r\n", r->sleep_en_need_cpu0_wfi);
	SOC_LOGI("	sleep_en_global: %8x\r\n", r->sleep_en_global);
	SOC_LOGI("	sleep_bus_idle_bypass: %8x\r\n", r->sleep_bus_idle_bypass);
	SOC_LOGI("	bts_wakeup_platform_en: %8x\r\n", r->bts_wakeup_platform_en);
	SOC_LOGI("	bts_soft_wakeup_req: %8x\r\n", r->bts_soft_wakeup_req);
	SOC_LOGI("	rom_rd_disable: %8x\r\n", r->rom_rd_disable);
	SOC_LOGI("	otp_rd_disable: %8x\r\n", r->otp_rd_disable);
	SOC_LOGI("	tcm1_pgen: %8x\r\n", r->tcm1_pgen);
	SOC_LOGI("	cpu0_subpwdm_en: %8x\r\n", r->cpu0_subpwdm_en);
	SOC_LOGI("	cpu1_subpwdm_en: %8x\r\n", r->cpu1_subpwdm_en);
	SOC_LOGI("	share_mem_clkgating_disable: %8x\r\n", r->share_mem_clkgating_disable);
	SOC_LOGI("	cpu0_ticktimer_32k_enable: %8x\r\n", r->cpu0_ticktimer_32k_enable);
	SOC_LOGI("	cpu1_ticktimer_32k_enable: %8x\r\n", r->cpu1_ticktimer_32k_enable);
	SOC_LOGI("	reserved_bit_31_31: %8x\r\n", r->reserved_bit_31_31);
}

static void sys_dump_reserver_reg0x11(void)
{
	SOC_LOGI("reserver_reg0x11: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x11 << 2)));
}

static void sys_dump_cpu_device_mem_ctrl3(void)
{
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t *)(SOC_SYS_REG_BASE + (0x12 << 2));

	SOC_LOGI("cpu_device_mem_ctrl3: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x12 << 2)));
	SOC_LOGI("	ram0_mem_ds_deep_sleep: %8x\r\n", r->ram0_mem_ds_deep_sleep);
	SOC_LOGI("	ram1_deep_sleep: %8x\r\n", r->ram1_deep_sleep);
	SOC_LOGI("	ram2_deep_sleep: %8x\r\n", r->ram2_deep_sleep);
	SOC_LOGI("	ram3_deep_sleep: %8x\r\n", r->ram3_deep_sleep);
	SOC_LOGI("	ram4_deep_sleep: %8x\r\n", r->ram4_deep_sleep);
	SOC_LOGI("	cpu0_icache_itag_deep_sleep: %8x\r\n", r->cpu0_icache_itag_deep_sleep);
	SOC_LOGI("	cpu0_dcache_dtag_deep_sleep: %8x\r\n", r->cpu0_dcache_dtag_deep_sleep);
	SOC_LOGI("	cpu1_icache_itag_deep_sleep: %8x\r\n", r->cpu1_icache_itag_deep_sleep);
	SOC_LOGI("	cpu1_dcache_dtag_deep_sleep: %8x\r\n", r->cpu1_dcache_dtag_deep_sleep);
	SOC_LOGI("	cpu0_itcm_deep_sleep: %8x\r\n", r->cpu0_itcm_deep_sleep);
	SOC_LOGI("	cpu0_dtcm_deep_sleep: %8x\r\n", r->cpu0_dtcm_deep_sleep);
	SOC_LOGI("	cpu1_itcm_deep_sleep: %8x\r\n", r->cpu1_itcm_deep_sleep);
	SOC_LOGI("	cpu1_dtcm_deep_sleep: %8x\r\n", r->cpu1_dtcm_deep_sleep);
	SOC_LOGI("	rott_deep_sleep: %8x\r\n", r->rott_deep_sleep);
	SOC_LOGI("	reserved0: %8x\r\n", r->reserved0);
	SOC_LOGI("	reserved1: %8x\r\n", r->reserved1);
	SOC_LOGI("	ram0_mem_sd_shutdown: %8x\r\n", r->ram0_mem_sd_shutdown);
	SOC_LOGI("	ram1_shutdown: %8x\r\n", r->ram1_shutdown);
	SOC_LOGI("	ram2_shutdown: %8x\r\n", r->ram2_shutdown);
	SOC_LOGI("	ram3_shutdown: %8x\r\n", r->ram3_shutdown);
	SOC_LOGI("	ram4_shutdown: %8x\r\n", r->ram4_shutdown);
	SOC_LOGI("	cpu0_icache_itag_shutdown: %8x\r\n", r->cpu0_icache_itag_shutdown);
	SOC_LOGI("	cpu0_dcache_dtag_shutdown: %8x\r\n", r->cpu0_dcache_dtag_shutdown);
	SOC_LOGI("	cpu1_icache_itag_shutdown: %8x\r\n", r->cpu1_icache_itag_shutdown);
	SOC_LOGI("	cpu1_dcache_dtag_shutdown: %8x\r\n", r->cpu1_dcache_dtag_shutdown);
	SOC_LOGI("	cpu0_itcm_shutdown: %8x\r\n", r->cpu0_itcm_shutdown);
	SOC_LOGI("	cpu0_dtcm_shutdown: %8x\r\n", r->cpu0_dtcm_shutdown);
	SOC_LOGI("	cpu1_itcm_shutdown: %8x\r\n", r->cpu1_itcm_shutdown);
	SOC_LOGI("	cpu1_dtcm_shutdown: %8x\r\n", r->cpu1_dtcm_shutdown);
	SOC_LOGI("	rott_shutdown: %8x\r\n", r->rott_shutdown);
	SOC_LOGI("	reserved2: %8x\r\n", r->reserved2);
	SOC_LOGI("	reserved3: %8x\r\n", r->reserved3);
}

static void sys_dump_rsv_13_1f(void)
{
	for (uint32_t idx = 0; idx < 13; idx++) {
		SOC_LOGI("rsv_13_1f: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + ((0x13 + idx) << 2)));
	}
}

static void sys_dump_cpu0_int_0_31_en(void)
{
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t *)(SOC_SYS_REG_BASE + (0x20 << 2));

	SOC_LOGI("cpu0_int_0_31_en: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x20 << 2)));
	SOC_LOGI("	cpu0_dma0_nsec_intr_en: %8x\r\n", r->cpu0_dma0_nsec_intr_en);
	SOC_LOGI("	cpu0_encp_sec_intr_en: %8x\r\n", r->cpu0_encp_sec_intr_en);
	SOC_LOGI("	cpu0_encp_nsec_intr_en: %8x\r\n", r->cpu0_encp_nsec_intr_en);
	SOC_LOGI("	cpu0_timer0_int_en: %8x\r\n", r->cpu0_timer0_int_en);
	SOC_LOGI("	cpu0_uart_int_en: %8x\r\n", r->cpu0_uart_int_en);
	SOC_LOGI("	cpu0_pwm0_int_en: %8x\r\n", r->cpu0_pwm0_int_en);
	SOC_LOGI("	cpu0_i2c0_int_en: %8x\r\n", r->cpu0_i2c0_int_en);
	SOC_LOGI("	cpu0_spi0_int_en: %8x\r\n", r->cpu0_spi0_int_en);
	SOC_LOGI("	cpu0_sadc_int_en: %8x\r\n", r->cpu0_sadc_int_en);
	SOC_LOGI("	cpu0_irda_int_en: %8x\r\n", r->cpu0_irda_int_en);
	SOC_LOGI("	cpu0_sdio_int_en: %8x\r\n", r->cpu0_sdio_int_en);
	SOC_LOGI("	cpu0_gdma_int_en: %8x\r\n", r->cpu0_gdma_int_en);
	SOC_LOGI("	cpu0_la_int_en: %8x\r\n", r->cpu0_la_int_en);
	SOC_LOGI("	cpu0_timer1_int_en: %8x\r\n", r->cpu0_timer1_int_en);
	SOC_LOGI("	cpu0_i2c1_int_en: %8x\r\n", r->cpu0_i2c1_int_en);
	SOC_LOGI("	cpu0_uart1_int_en: %8x\r\n", r->cpu0_uart1_int_en);
	SOC_LOGI("	cpu0_uart2_int_en: %8x\r\n", r->cpu0_uart2_int_en);
	SOC_LOGI("	cpu0_spi1_int_en: %8x\r\n", r->cpu0_spi1_int_en);
	SOC_LOGI("	cpu0_can_int_en: %8x\r\n", r->cpu0_can_int_en);
	SOC_LOGI("	cpu0_usb_int_en: %8x\r\n", r->cpu0_usb_int_en);
	SOC_LOGI("	cpu0_qspi0_int_en: %8x\r\n", r->cpu0_qspi0_int_en);
	SOC_LOGI("	cpu0_fft_int_en: %8x\r\n", r->cpu0_fft_int_en);
	SOC_LOGI("	cpu0_sbc_int_en: %8x\r\n", r->cpu0_sbc_int_en);
	SOC_LOGI("	cpu0_aud_int_en: %8x\r\n", r->cpu0_aud_int_en);
	SOC_LOGI("	cpu0_i2s0_int_en: %8x\r\n", r->cpu0_i2s0_int_en);
	SOC_LOGI("	cpu0_jpegenc_int_en: %8x\r\n", r->cpu0_jpegenc_int_en);
	SOC_LOGI("	cpu0_jpegdec_int_en: %8x\r\n", r->cpu0_jpegdec_int_en);
	SOC_LOGI("	cpu0_lcd_display_int_en: %8x\r\n", r->cpu0_lcd_display_int_en);
	SOC_LOGI("	cpu0_dma2d_int_en: %8x\r\n", r->cpu0_dma2d_int_en);
	SOC_LOGI("	cpu0_wifi_int_phy_mpb_en: %8x\r\n", r->cpu0_wifi_int_phy_mpb_en);
	SOC_LOGI("	cpu0_wifi_int_phy_riu_en: %8x\r\n", r->cpu0_wifi_int_phy_riu_en);
	SOC_LOGI("	cpu0_wifi_mac_int_tx_rx_timer_en: %8x\r\n", r->cpu0_wifi_mac_int_tx_rx_timer_en);
}

static void sys_dump_cpu0_int_32_63_en(void)
{
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t *)(SOC_SYS_REG_BASE + (0x21 << 2));

	SOC_LOGI("cpu0_int_32_63_en: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x21 << 2)));
	SOC_LOGI("	cpu0_wifi_mac_int_tx_rx_misc_en: %8x\r\n", r->cpu0_wifi_mac_int_tx_rx_misc_en);
	SOC_LOGI("	cpu0_wifi_mac_int_rx_trigger_en: %8x\r\n", r->cpu0_wifi_mac_int_rx_trigger_en);
	SOC_LOGI("	cpu0_wifi_mac_int_tx_trigger_en: %8x\r\n", r->cpu0_wifi_mac_int_tx_trigger_en);
	SOC_LOGI("	cpu0_wifi_mac_int_prot_trigger_en: %8x\r\n", r->cpu0_wifi_mac_int_prot_trigger_en);
	SOC_LOGI("	cpu0_wifi_mac_int_gen_en: %8x\r\n", r->cpu0_wifi_mac_int_gen_en);
	SOC_LOGI("	cpu0_wifi_hsu_irq_en: %8x\r\n", r->cpu0_wifi_hsu_irq_en);
	SOC_LOGI("	cpu0_wifi_int_mac_wakeup_en: %8x\r\n", r->cpu0_wifi_int_mac_wakeup_en);
	SOC_LOGI("	cpu0_dm_irq_en: %8x\r\n", r->cpu0_dm_irq_en);
	SOC_LOGI("	cpu0_ble_irq_en: %8x\r\n", r->cpu0_ble_irq_en);
	SOC_LOGI("	cpu0_bt_irq_en: %8x\r\n", r->cpu0_bt_irq_en);
	SOC_LOGI("	cpu0_qspi1_int_en: %8x\r\n", r->cpu0_qspi1_int_en);
	SOC_LOGI("	cpu0_pwm1_int_en: %8x\r\n", r->cpu0_pwm1_int_en);
	SOC_LOGI("	cpu0_i2s1_int_en: %8x\r\n", r->cpu0_i2s1_int_en);
	SOC_LOGI("	cpu0_i2s2_int_en: %8x\r\n", r->cpu0_i2s2_int_en);
	SOC_LOGI("	cpu0_h264_int_en: %8x\r\n", r->cpu0_h264_int_en);
	SOC_LOGI("	cpu0_sdmadc_int_en: %8x\r\n", r->cpu0_sdmadc_int_en);
	SOC_LOGI("	cpu0_mbox0_int_en: %8x\r\n", r->cpu0_mbox0_int_en);
	SOC_LOGI("	cpu0_mbox1_int_en: %8x\r\n", r->cpu0_mbox1_int_en);
	SOC_LOGI("	cpu0_bmc64_int_en: %8x\r\n", r->cpu0_bmc64_int_en);
	SOC_LOGI("	cpu0_dpll_unlock_int_en: %8x\r\n", r->cpu0_dpll_unlock_int_en);
	SOC_LOGI("	cpu0_touched_int_en: %8x\r\n", r->cpu0_touched_int_en);
	SOC_LOGI("	cpu0_usbplug_int_en: %8x\r\n", r->cpu0_usbplug_int_en);
	SOC_LOGI("	cpu0_rtc_int_en: %8x\r\n", r->cpu0_rtc_int_en);
	SOC_LOGI("	cpu0_gpio_int_en: %8x\r\n", r->cpu0_gpio_int_en);
	SOC_LOGI("	cpu0_dma1_sec_int_en: %8x\r\n", r->cpu0_dma1_sec_int_en);
	SOC_LOGI("	cpu0_dma1_nsec_int_en: %8x\r\n", r->cpu0_dma1_nsec_int_en);
	SOC_LOGI("	cpu0_yuvb_int_en: %8x\r\n", r->cpu0_yuvb_int_en);
	SOC_LOGI("	cpu0_rott_int_en: %8x\r\n", r->cpu0_rott_int_en);
	SOC_LOGI("	reserved0: %8x\r\n", r->reserved0);
}

static void sys_dump_cpu1_int_0_31_en(void)
{
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t *)(SOC_SYS_REG_BASE + (0x22 << 2));

	SOC_LOGI("cpu1_int_0_31_en: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x22 << 2)));
	SOC_LOGI("	cpu1_dma0_nsec_intr_en: %8x\r\n", r->cpu1_dma0_nsec_intr_en);
	SOC_LOGI("	cpu1_encp_sec_intr_en: %8x\r\n", r->cpu1_encp_sec_intr_en);
	SOC_LOGI("	cpu1_encp_nsec_intr_en: %8x\r\n", r->cpu1_encp_nsec_intr_en);
	SOC_LOGI("	cpu1_timer0_int_en: %8x\r\n", r->cpu1_timer0_int_en);
	SOC_LOGI("	cpu1_uart_int_en: %8x\r\n", r->cpu1_uart_int_en);
	SOC_LOGI("	cpu1_pwm0_int_en: %8x\r\n", r->cpu1_pwm0_int_en);
	SOC_LOGI("	cpu1_i2c0_int_en: %8x\r\n", r->cpu1_i2c0_int_en);
	SOC_LOGI("	cpu1_spi0_int_en: %8x\r\n", r->cpu1_spi0_int_en);
	SOC_LOGI("	cpu1_sadc_int_en: %8x\r\n", r->cpu1_sadc_int_en);
	SOC_LOGI("	cpu1_irda_int_en: %8x\r\n", r->cpu1_irda_int_en);
	SOC_LOGI("	cpu1_sdio_int_en: %8x\r\n", r->cpu1_sdio_int_en);
	SOC_LOGI("	cpu1_gdma_int_en: %8x\r\n", r->cpu1_gdma_int_en);
	SOC_LOGI("	cpu1_la_int_en: %8x\r\n", r->cpu1_la_int_en);
	SOC_LOGI("	cpu1_timer1_int_en: %8x\r\n", r->cpu1_timer1_int_en);
	SOC_LOGI("	cpu1_i2c1_int_en: %8x\r\n", r->cpu1_i2c1_int_en);
	SOC_LOGI("	cpu1_uart1_int_en: %8x\r\n", r->cpu1_uart1_int_en);
	SOC_LOGI("	cpu1_uart2_int_en: %8x\r\n", r->cpu1_uart2_int_en);
	SOC_LOGI("	cpu1_spi1_int_en: %8x\r\n", r->cpu1_spi1_int_en);
	SOC_LOGI("	cpu1_can_int_en: %8x\r\n", r->cpu1_can_int_en);
	SOC_LOGI("	cpu1_usb_int_en: %8x\r\n", r->cpu1_usb_int_en);
	SOC_LOGI("	cpu1_qspi0_int_en: %8x\r\n", r->cpu1_qspi0_int_en);
	SOC_LOGI("	cpu1_fft_int_en: %8x\r\n", r->cpu1_fft_int_en);
	SOC_LOGI("	cpu1_sbc_int_en: %8x\r\n", r->cpu1_sbc_int_en);
	SOC_LOGI("	cpu1_aud_int_en: %8x\r\n", r->cpu1_aud_int_en);
	SOC_LOGI("	cpu1_i2s0_int_en: %8x\r\n", r->cpu1_i2s0_int_en);
	SOC_LOGI("	cpu1_jpegenc_int_en: %8x\r\n", r->cpu1_jpegenc_int_en);
	SOC_LOGI("	cpu1_jpegdec_int_en: %8x\r\n", r->cpu1_jpegdec_int_en);
	SOC_LOGI("	cpu1_lcd_display_int_en: %8x\r\n", r->cpu1_lcd_display_int_en);
	SOC_LOGI("	cpu1_dma2d_int_en: %8x\r\n", r->cpu1_dma2d_int_en);
	SOC_LOGI("	cpu1_wifi_int_phy_mpb_en: %8x\r\n", r->cpu1_wifi_int_phy_mpb_en);
	SOC_LOGI("	cpu1_wifi_int_phy_riu_en: %8x\r\n", r->cpu1_wifi_int_phy_riu_en);
	SOC_LOGI("	cpu1_wifi_mac_int_tx_rx_timer_en: %8x\r\n", r->cpu1_wifi_mac_int_tx_rx_timer_en);
}

static void sys_dump_cpu1_int_32_63_en(void)
{
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t *)(SOC_SYS_REG_BASE + (0x23 << 2));

	SOC_LOGI("cpu1_int_32_63_en: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x23 << 2)));
	SOC_LOGI("	cpu1_wifi_mac_int_tx_rx_misc_en: %8x\r\n", r->cpu1_wifi_mac_int_tx_rx_misc_en);
	SOC_LOGI("	cpu1_wifi_mac_int_rx_trigger_en: %8x\r\n", r->cpu1_wifi_mac_int_rx_trigger_en);
	SOC_LOGI("	cpu1_wifi_mac_int_tx_trigger_en: %8x\r\n", r->cpu1_wifi_mac_int_tx_trigger_en);
	SOC_LOGI("	cpu1_wifi_mac_int_prot_trigger_en: %8x\r\n", r->cpu1_wifi_mac_int_prot_trigger_en);
	SOC_LOGI("	cpu1_wifi_mac_int_gen_en: %8x\r\n", r->cpu1_wifi_mac_int_gen_en);
	SOC_LOGI("	cpu1_wifi_hsu_irq_en: %8x\r\n", r->cpu1_wifi_hsu_irq_en);
	SOC_LOGI("	cpu1_wifi_int_mac_wakeup_en: %8x\r\n", r->cpu1_wifi_int_mac_wakeup_en);
	SOC_LOGI("	cpu1_dm_irq_en: %8x\r\n", r->cpu1_dm_irq_en);
	SOC_LOGI("	cpu1_ble_irq_en: %8x\r\n", r->cpu1_ble_irq_en);
	SOC_LOGI("	cpu1_bt_irq_en: %8x\r\n", r->cpu1_bt_irq_en);
	SOC_LOGI("	cpu1_qspi1_int_en: %8x\r\n", r->cpu1_qspi1_int_en);
	SOC_LOGI("	cpu1_pwm1_int_en: %8x\r\n", r->cpu1_pwm1_int_en);
	SOC_LOGI("	cpu1_i2s1_int_en: %8x\r\n", r->cpu1_i2s1_int_en);
	SOC_LOGI("	cpu1_i2s2_int_en: %8x\r\n", r->cpu1_i2s2_int_en);
	SOC_LOGI("	cpu1_h264_int_en: %8x\r\n", r->cpu1_h264_int_en);
	SOC_LOGI("	cpu1_sdmadc_int_en: %8x\r\n", r->cpu1_sdmadc_int_en);
	SOC_LOGI("	cpu1_mbox0_int_en: %8x\r\n", r->cpu1_mbox0_int_en);
	SOC_LOGI("	cpu1_mbox1_int_en: %8x\r\n", r->cpu1_mbox1_int_en);
	SOC_LOGI("	cpu1_bmc64_int_en: %8x\r\n", r->cpu1_bmc64_int_en);
	SOC_LOGI("	cpu1_dpll_unlock_int_en: %8x\r\n", r->cpu1_dpll_unlock_int_en);
	SOC_LOGI("	cpu1_touched_int_en: %8x\r\n", r->cpu1_touched_int_en);
	SOC_LOGI("	cpu1_usbplug_int_en: %8x\r\n", r->cpu1_usbplug_int_en);
	SOC_LOGI("	cpu1_rtc_int_en: %8x\r\n", r->cpu1_rtc_int_en);
	SOC_LOGI("	cpu1_gpio_int_en: %8x\r\n", r->cpu1_gpio_int_en);
	SOC_LOGI("	cpu1_dma1_sec_int_en: %8x\r\n", r->cpu1_dma1_sec_int_en);
	SOC_LOGI("	cpu1_dma1_nsec_int_en: %8x\r\n", r->cpu1_dma1_nsec_int_en);
	SOC_LOGI("	cpu1_yuvb_int_en: %8x\r\n", r->cpu1_yuvb_int_en);
	SOC_LOGI("	reserved0: %8x\r\n", r->reserved0);
}

static void sys_dump_rsv_24_27(void)
{
	for (uint32_t idx = 0; idx < 4; idx++) {
		SOC_LOGI("rsv_24_27: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + ((0x24 + idx) << 2)));
	}
}

static void sys_dump_cpu0_int_0_31_status(void)
{
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t *)(SOC_SYS_REG_BASE + (0x28 << 2));

	SOC_LOGI("cpu0_int_0_31_status: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x28 << 2)));
	SOC_LOGI("	cpu0_dma0_nsec_intr_st: %8x\r\n", r->cpu0_dma0_nsec_intr_st);
	SOC_LOGI("	cpu0_encp_sec_intr_st: %8x\r\n", r->cpu0_encp_sec_intr_st);
	SOC_LOGI("	cpu0_encp_nsec_intr_st: %8x\r\n", r->cpu0_encp_nsec_intr_st);
	SOC_LOGI("	cpu0_timer0_int_st: %8x\r\n", r->cpu0_timer0_int_st);
	SOC_LOGI("	cpu0_uart_int_st: %8x\r\n", r->cpu0_uart_int_st);
	SOC_LOGI("	cpu0_pwm0_int_st: %8x\r\n", r->cpu0_pwm0_int_st);
	SOC_LOGI("	cpu0_i2c0_int_st: %8x\r\n", r->cpu0_i2c0_int_st);
	SOC_LOGI("	cpu0_spi0_int_st: %8x\r\n", r->cpu0_spi0_int_st);
	SOC_LOGI("	cpu0_sadc_int_st: %8x\r\n", r->cpu0_sadc_int_st);
	SOC_LOGI("	cpu0_irda_int_st: %8x\r\n", r->cpu0_irda_int_st);
	SOC_LOGI("	cpu0_sdio_int_st: %8x\r\n", r->cpu0_sdio_int_st);
	SOC_LOGI("	cpu0_gdma_int_st: %8x\r\n", r->cpu0_gdma_int_st);
	SOC_LOGI("	cpu0_la_int_st: %8x\r\n", r->cpu0_la_int_st);
	SOC_LOGI("	cpu0_timer1_int_st: %8x\r\n", r->cpu0_timer1_int_st);
	SOC_LOGI("	cpu0_i2c1_int_st: %8x\r\n", r->cpu0_i2c1_int_st);
	SOC_LOGI("	cpu0_uart1_int_st: %8x\r\n", r->cpu0_uart1_int_st);
	SOC_LOGI("	cpu0_uart2_int_st: %8x\r\n", r->cpu0_uart2_int_st);
	SOC_LOGI("	cpu0_spi1_int_st: %8x\r\n", r->cpu0_spi1_int_st);
	SOC_LOGI("	cpu0_can_int_st: %8x\r\n", r->cpu0_can_int_st);
	SOC_LOGI("	cpu0_usb_int_st: %8x\r\n", r->cpu0_usb_int_st);
	SOC_LOGI("	cpu0_qspi0_int_st: %8x\r\n", r->cpu0_qspi0_int_st);
	SOC_LOGI("	cpu0_fft_int_st: %8x\r\n", r->cpu0_fft_int_st);
	SOC_LOGI("	cpu0_sbc_int_st: %8x\r\n", r->cpu0_sbc_int_st);
	SOC_LOGI("	cpu0_aud_int_st: %8x\r\n", r->cpu0_aud_int_st);
	SOC_LOGI("	cpu0_i2s0_int_st: %8x\r\n", r->cpu0_i2s0_int_st);
	SOC_LOGI("	cpu0_jpegenc_int_st: %8x\r\n", r->cpu0_jpegenc_int_st);
	SOC_LOGI("	cpu0_jpegdec_int_st: %8x\r\n", r->cpu0_jpegdec_int_st);
	SOC_LOGI("	cpu0_lcd_display_int_st: %8x\r\n", r->cpu0_lcd_display_int_st);
	SOC_LOGI("	cpu0_dma2d_int_st: %8x\r\n", r->cpu0_dma2d_int_st);
	SOC_LOGI("	cpu0_wifi_int_phy_mpb_st: %8x\r\n", r->cpu0_wifi_int_phy_mpb_st);
	SOC_LOGI("	cpu0_wifi_int_phy_riu_st: %8x\r\n", r->cpu0_wifi_int_phy_riu_st);
	SOC_LOGI("	cpu0_wifi_mac_int_tx_rx_timer_st: %8x\r\n", r->cpu0_wifi_mac_int_tx_rx_timer_st);
}

static void sys_dump_cpu0_int_32_63_status(void)
{
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t *)(SOC_SYS_REG_BASE + (0x29 << 2));

	SOC_LOGI("cpu0_int_32_63_status: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x29 << 2)));
	SOC_LOGI("	cpu0_wifi_mac_int_tx_rx_misc_st: %8x\r\n", r->cpu0_wifi_mac_int_tx_rx_misc_st);
	SOC_LOGI("	cpu0_wifi_mac_int_rx_trigger_st: %8x\r\n", r->cpu0_wifi_mac_int_rx_trigger_st);
	SOC_LOGI("	cpu0_wifi_mac_int_tx_trigger_st: %8x\r\n", r->cpu0_wifi_mac_int_tx_trigger_st);
	SOC_LOGI("	cpu0_wifi_mac_int_prot_trigger_st: %8x\r\n", r->cpu0_wifi_mac_int_prot_trigger_st);
	SOC_LOGI("	cpu0_wifi_mac_int_gen_st: %8x\r\n", r->cpu0_wifi_mac_int_gen_st);
	SOC_LOGI("	cpu0_wifi_hsu_irq_st: %8x\r\n", r->cpu0_wifi_hsu_irq_st);
	SOC_LOGI("	cpu0_wifi_int_mac_wakeup_st: %8x\r\n", r->cpu0_wifi_int_mac_wakeup_st);
	SOC_LOGI("	cpu0_dm_irq_st: %8x\r\n", r->cpu0_dm_irq_st);
	SOC_LOGI("	cpu0_ble_irq_st: %8x\r\n", r->cpu0_ble_irq_st);
	SOC_LOGI("	cpu0_bt_irq_st: %8x\r\n", r->cpu0_bt_irq_st);
	SOC_LOGI("	cpu0_qspi1_int_st: %8x\r\n", r->cpu0_qspi1_int_st);
	SOC_LOGI("	cpu0_pwm1_int_st: %8x\r\n", r->cpu0_pwm1_int_st);
	SOC_LOGI("	cpu0_i2s1_int_st: %8x\r\n", r->cpu0_i2s1_int_st);
	SOC_LOGI("	cpu0_i2s2_int_st: %8x\r\n", r->cpu0_i2s2_int_st);
	SOC_LOGI("	cpu0_h264_int_st: %8x\r\n", r->cpu0_h264_int_st);
	SOC_LOGI("	cpu0_sdmadc_int_st: %8x\r\n", r->cpu0_sdmadc_int_st);
	SOC_LOGI("	cpu0_mbox0_int_st: %8x\r\n", r->cpu0_mbox0_int_st);
	SOC_LOGI("	cpu0_mbox1_int_st: %8x\r\n", r->cpu0_mbox1_int_st);
	SOC_LOGI("	cpu0_bmc64_int_st: %8x\r\n", r->cpu0_bmc64_int_st);
	SOC_LOGI("	cpu0_dpll_unlock_int_st: %8x\r\n", r->cpu0_dpll_unlock_int_st);
	SOC_LOGI("	cpu0_touched_int_st: %8x\r\n", r->cpu0_touched_int_st);
	SOC_LOGI("	cpu0_usbplug_int_st: %8x\r\n", r->cpu0_usbplug_int_st);
	SOC_LOGI("	cpu0_rtc_int_st: %8x\r\n", r->cpu0_rtc_int_st);
	SOC_LOGI("	cpu0_gpio_int_st: %8x\r\n", r->cpu0_gpio_int_st);
	SOC_LOGI("	cpu0_dma1_sec_int_st: %8x\r\n", r->cpu0_dma1_sec_int_st);
	SOC_LOGI("	cpu0_dma1_nsec_int_st: %8x\r\n", r->cpu0_dma1_nsec_int_st);
	SOC_LOGI("	cpu0_yuvb_int_st: %8x\r\n", r->cpu0_yuvb_int_st);
	SOC_LOGI("	reserved0: %8x\r\n", r->reserved0);
}

static void sys_dump_cpu1_int_0_31_status(void)
{
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t *)(SOC_SYS_REG_BASE + (0x2a << 2));

	SOC_LOGI("cpu1_int_0_31_status: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x2a << 2)));
	SOC_LOGI("	cpu1_dma0_nsec_intr_st: %8x\r\n", r->cpu1_dma0_nsec_intr_st);
	SOC_LOGI("	cpu1_encp_sec_intr_st: %8x\r\n", r->cpu1_encp_sec_intr_st);
	SOC_LOGI("	cpu1_encp_nsec_intr_st: %8x\r\n", r->cpu1_encp_nsec_intr_st);
	SOC_LOGI("	cpu1_timer0_int_st: %8x\r\n", r->cpu1_timer0_int_st);
	SOC_LOGI("	cpu1_uart_int_st: %8x\r\n", r->cpu1_uart_int_st);
	SOC_LOGI("	cpu1_pwm0_int_st: %8x\r\n", r->cpu1_pwm0_int_st);
	SOC_LOGI("	cpu1_i2c0_int_st: %8x\r\n", r->cpu1_i2c0_int_st);
	SOC_LOGI("	cpu1_spi0_int_st: %8x\r\n", r->cpu1_spi0_int_st);
	SOC_LOGI("	cpu1_sadc_int_st: %8x\r\n", r->cpu1_sadc_int_st);
	SOC_LOGI("	cpu1_irda_int_st: %8x\r\n", r->cpu1_irda_int_st);
	SOC_LOGI("	cpu1_sdio_int_st: %8x\r\n", r->cpu1_sdio_int_st);
	SOC_LOGI("	cpu1_gdma_int_st: %8x\r\n", r->cpu1_gdma_int_st);
	SOC_LOGI("	cpu1_la_int_st: %8x\r\n", r->cpu1_la_int_st);
	SOC_LOGI("	cpu1_timer1_int_st: %8x\r\n", r->cpu1_timer1_int_st);
	SOC_LOGI("	cpu1_i2c1_int_st: %8x\r\n", r->cpu1_i2c1_int_st);
	SOC_LOGI("	cpu1_uart1_int_st: %8x\r\n", r->cpu1_uart1_int_st);
	SOC_LOGI("	cpu1_uart2_int_st: %8x\r\n", r->cpu1_uart2_int_st);
	SOC_LOGI("	cpu1_spi1_int_st: %8x\r\n", r->cpu1_spi1_int_st);
	SOC_LOGI("	cpu1_can_int_st: %8x\r\n", r->cpu1_can_int_st);
	SOC_LOGI("	cpu1_usb_int_st: %8x\r\n", r->cpu1_usb_int_st);
	SOC_LOGI("	cpu1_qspi0_int_st: %8x\r\n", r->cpu1_qspi0_int_st);
	SOC_LOGI("	cpu1_fft_int_st: %8x\r\n", r->cpu1_fft_int_st);
	SOC_LOGI("	cpu1_sbc_int_st: %8x\r\n", r->cpu1_sbc_int_st);
	SOC_LOGI("	cpu1_aud_int_st: %8x\r\n", r->cpu1_aud_int_st);
	SOC_LOGI("	cpu1_i2s0_int_st: %8x\r\n", r->cpu1_i2s0_int_st);
	SOC_LOGI("	cpu1_jpegenc_int_st: %8x\r\n", r->cpu1_jpegenc_int_st);
	SOC_LOGI("	cpu1_jpegdec_int_st: %8x\r\n", r->cpu1_jpegdec_int_st);
	SOC_LOGI("	cpu1_lcd_display_int_st: %8x\r\n", r->cpu1_lcd_display_int_st);
	SOC_LOGI("	cpu1_dma2d_int_st: %8x\r\n", r->cpu1_dma2d_int_st);
	SOC_LOGI("	cpu1_wifi_int_phy_mpb_st: %8x\r\n", r->cpu1_wifi_int_phy_mpb_st);
	SOC_LOGI("	cpu1_wifi_int_phy_riu_st: %8x\r\n", r->cpu1_wifi_int_phy_riu_st);
	SOC_LOGI("	cpu1_wifi_mac_int_tx_rx_timer_st: %8x\r\n", r->cpu1_wifi_mac_int_tx_rx_timer_st);
}

static void sys_dump_cpu1_int_32_63_status(void)
{
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t *)(SOC_SYS_REG_BASE + (0x2b << 2));

	SOC_LOGI("cpu1_int_32_63_status: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x2b << 2)));
	SOC_LOGI("	cpu1_wifi_mac_int_tx_rx_misc_st: %8x\r\n", r->cpu1_wifi_mac_int_tx_rx_misc_st);
	SOC_LOGI("	cpu1_wifi_mac_int_rx_trigger_st: %8x\r\n", r->cpu1_wifi_mac_int_rx_trigger_st);
	SOC_LOGI("	cpu1_wifi_mac_int_tx_trigger_st: %8x\r\n", r->cpu1_wifi_mac_int_tx_trigger_st);
	SOC_LOGI("	cpu1_wifi_mac_int_prot_trigger_st: %8x\r\n", r->cpu1_wifi_mac_int_prot_trigger_st);
	SOC_LOGI("	cpu1_wifi_mac_int_gen_st: %8x\r\n", r->cpu1_wifi_mac_int_gen_st);
	SOC_LOGI("	cpu1_wifi_hsu_irq_st: %8x\r\n", r->cpu1_wifi_hsu_irq_st);
	SOC_LOGI("	cpu1_wifi_int_mac_wakeup_st: %8x\r\n", r->cpu1_wifi_int_mac_wakeup_st);
	SOC_LOGI("	cpu1_dm_irq_st: %8x\r\n", r->cpu1_dm_irq_st);
	SOC_LOGI("	cpu1_ble_irq_st: %8x\r\n", r->cpu1_ble_irq_st);
	SOC_LOGI("	cpu1_bt_irq_st: %8x\r\n", r->cpu1_bt_irq_st);
	SOC_LOGI("	cpu1_qspi1_int_st: %8x\r\n", r->cpu1_qspi1_int_st);
	SOC_LOGI("	cpu1_pwm1_int_st: %8x\r\n", r->cpu1_pwm1_int_st);
	SOC_LOGI("	cpu1_i2s1_int_st: %8x\r\n", r->cpu1_i2s1_int_st);
	SOC_LOGI("	cpu1_i2s2_int_st: %8x\r\n", r->cpu1_i2s2_int_st);
	SOC_LOGI("	cpu1_h264_int_st: %8x\r\n", r->cpu1_h264_int_st);
	SOC_LOGI("	cpu1_sdmadc_int_st: %8x\r\n", r->cpu1_sdmadc_int_st);
	SOC_LOGI("	cpu1_mbox0_int_st: %8x\r\n", r->cpu1_mbox0_int_st);
	SOC_LOGI("	cpu1_mbox1_int_st: %8x\r\n", r->cpu1_mbox1_int_st);
	SOC_LOGI("	cpu1_bmc64_int_st: %8x\r\n", r->cpu1_bmc64_int_st);
	SOC_LOGI("	cpu1_dpll_unlock_int_st: %8x\r\n", r->cpu1_dpll_unlock_int_st);
	SOC_LOGI("	cpu1_touched_int_st: %8x\r\n", r->cpu1_touched_int_st);
	SOC_LOGI("	cpu1_usbplug_int_st: %8x\r\n", r->cpu1_usbplug_int_st);
	SOC_LOGI("	cpu1_rtc_int_st: %8x\r\n", r->cpu1_rtc_int_st);
	SOC_LOGI("	cpu1_gpio_int_st: %8x\r\n", r->cpu1_gpio_int_st);
	SOC_LOGI("	cpu1_dma1_sec_int_st: %8x\r\n", r->cpu1_dma1_sec_int_st);
	SOC_LOGI("	cpu1_dma1_nsec_int_st: %8x\r\n", r->cpu1_dma1_nsec_int_st);
	SOC_LOGI("	cpu1_yuvb_int_st: %8x\r\n", r->cpu1_yuvb_int_st);
	SOC_LOGI("	reserved0: %8x\r\n", r->reserved0);
}

static void sys_dump_rsv_2c_2f(void)
{
	for (uint32_t idx = 0; idx < 4; idx++) {
		SOC_LOGI("rsv_2c_2f: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + ((0x2c + idx) << 2)));
	}
}

static void sys_dump_gpio_config0(void)
{
	SOC_LOGI("gpio_config0: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x30 << 2)));
}

static void sys_dump_gpio_config1(void)
{
	SOC_LOGI("gpio_config1: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x31 << 2)));
}

static void sys_dump_gpio_config2(void)
{
	SOC_LOGI("gpio_config2: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x32 << 2)));
}

static void sys_dump_gpio_config3(void)
{
	SOC_LOGI("gpio_config3: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x33 << 2)));
}

static void sys_dump_gpio_config4(void)
{
	SOC_LOGI("gpio_config4: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x34 << 2)));
}

static void sys_dump_gpio_config5(void)
{
	SOC_LOGI("gpio_config5: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x35 << 2)));
}

static void sys_dump_rsv_36_37(void)
{
	for (uint32_t idx = 0; idx < 2; idx++) {
		SOC_LOGI("rsv_36_37: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + ((0x36 + idx) << 2)));
	}
}

static void sys_dump_sys_debug_config0(void)
{
	SOC_LOGI("sys_debug_config0: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x38 << 2)));
}

static void sys_dump_sys_debug_config1(void)
{
	SOC_LOGI("sys_debug_config1: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x39 << 2)));
}

static void sys_dump_rsv_3a_3f(void)
{
	for (uint32_t idx = 0; idx < 6; idx++) {
		SOC_LOGI("rsv_3a_3f: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + ((0x3a + idx) << 2)));
	}
}

static void sys_dump_ana_reg0(void)
{
	sys_ana_reg0_t *r = (sys_ana_reg0_t *)(SOC_SYS_REG_BASE + (0x40 << 2));

	SOC_LOGI("ana_reg0: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x40 << 2)));
	SOC_LOGI("	dpll_tsten: %8x\r\n", r->dpll_tsten);
	SOC_LOGI("	cp: %8x\r\n", r->cp);
	SOC_LOGI("	spideten: %8x\r\n", r->spideten);
	SOC_LOGI("	hvref: %8x\r\n", r->hvref);
	SOC_LOGI("	lvref: %8x\r\n", r->lvref);
	SOC_LOGI("	rzctrl26m: %8x\r\n", r->rzctrl26m);
	SOC_LOGI("	looprzctrl: %8x\r\n", r->looprzctrl);
	SOC_LOGI("	rpc: %8x\r\n", r->rpc);
	SOC_LOGI("	openloop_en: %8x\r\n", r->openloop_en);
	SOC_LOGI("	cksel: %8x\r\n", r->cksel);
	SOC_LOGI("	spitrig: %8x\r\n", r->spitrig);
	SOC_LOGI("	band0: %8x\r\n", r->band0);
	SOC_LOGI("	band1: %8x\r\n", r->band1);
	SOC_LOGI("	band: %8x\r\n", r->band);
	SOC_LOGI("	bandmanual: %8x\r\n", r->bandmanual);
	SOC_LOGI("	dsptrig: %8x\r\n", r->dsptrig);
	SOC_LOGI("	lpen_dpll: %8x\r\n", r->lpen_dpll);
	SOC_LOGI("	nc_28_30: %8x\r\n", r->nc_28_30);
	SOC_LOGI("	vctrl_dpllldo: %8x\r\n", r->vctrl_dpllldo);
}

static void sys_dump_ana_reg1(void)
{
	sys_ana_reg1_t *r = (sys_ana_reg1_t *)(SOC_SYS_REG_BASE + (0x41 << 2));

	SOC_LOGI("ana_reg1: %8x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + (0x41 << 2)));
	SOC_LOGI("	nc_0_0: %8x\r\n", r->nc_0_0);
	SOC_LOGI("	nc_1_1: %8x\r\n", r->nc_1_1);
	SOC_LOGI("	msw: %8x\r\n", r->msw);
	SOC_LOGI("	ictrl: %8x\r\n", r->ictrl);
	SOC_LOGI("	osc_trig: %8x\r\n", r->osc_trig);
	SOC_LOGI("	osccal_trig: %8x\r\n", r->osccal_trig);
	SOC_LOGI("	cnti: %8x\r\n", r->cnti);
	SOC_LOGI("	spi_rst: %8x\r\n", r->spi_rst);
	SOC_LOGI("	amsel: %8x\r\n", r->amsel);
	SOC_LOGI("	divctrl: %8x\r\n", r->divctrl);
	SOC_LOGI("	nc_30_30: %8x\r\n", r->nc_30_30);
	SOC_LOGI("	nc_31_31: %8x\r\n", r->nc_31_31);
}

static void sys_dump_ana_reg2(void)
{
	sys_ana_reg2_t *r = (sys_ana_reg2_t *)(SOC_SYS_REG_BASE + (0x42 << 2));

	SOC_LOGI("ana_reg2: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x42 << 2)));
	SOC_LOGI("	xtalh_ctune: %8x\r\n", r->xtalh_ctune);
	SOC_LOGI("	force_26mpll: %8x\r\n", r->force_26mpll);
	SOC_LOGI("	gadc_cmp_ictrl: %8x\r\n", r->gadc_cmp_ictrl);
	SOC_LOGI("	gadc_inbuf_ictrl: %8x\r\n", r->gadc_inbuf_ictrl);
	SOC_LOGI("	gadc_refbuf_ictrl: %8x\r\n", r->gadc_refbuf_ictrl);
	SOC_LOGI("	gadc_nobuf_enable: %8x\r\n", r->gadc_nobuf_enable);
	SOC_LOGI("	vref_scale: %8x\r\n", r->vref_scale);
	SOC_LOGI("	scal_en: %8x\r\n", r->scal_en);
	SOC_LOGI("	gadc_capcal_en: %8x\r\n", r->gadc_capcal_en);
	SOC_LOGI("	gadc_capcal: %8x\r\n", r->gadc_capcal);
	SOC_LOGI("	sp_nt_ctrl: %8x\r\n", r->sp_nt_ctrl);
}

static void sys_dump_ana_reg3(void)
{
	sys_ana_reg3_t *r = (sys_ana_reg3_t *)(SOC_SYS_REG_BASE + (0x43 << 2));

	SOC_LOGI("ana_reg3: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x43 << 2)));
	SOC_LOGI("	usbpen: %8x\r\n", r->usbpen);
	SOC_LOGI("	usbnen: %8x\r\n", r->usbnen);
	SOC_LOGI("	hpssren: %8x\r\n", r->hpssren);
	SOC_LOGI("	ck_sel: %8x\r\n", r->ck_sel);
	SOC_LOGI("	anabuf_sel_rx: %8x\r\n", r->anabuf_sel_rx);
	SOC_LOGI("	pwd_xtalldo: %8x\r\n", r->pwd_xtalldo);
	SOC_LOGI("	iamp: %8x\r\n", r->iamp);
	SOC_LOGI("	vddren: %8x\r\n", r->vddren);
	SOC_LOGI("	xamp: %8x\r\n", r->xamp);
	SOC_LOGI("	vosel: %8x\r\n", r->vosel);
	SOC_LOGI("	en_xtalh_sleep: %8x\r\n", r->en_xtalh_sleep);
	SOC_LOGI("	xtal40_en: %8x\r\n", r->xtal40_en);
	SOC_LOGI("	bufictrl: %8x\r\n", r->bufictrl);
	SOC_LOGI("	ibias_ctrl: %8x\r\n", r->ibias_ctrl);
	SOC_LOGI("	icore_ctrl: %8x\r\n", r->icore_ctrl);
}

static void sys_dump_ana_reg4(void)
{
	sys_ana_reg4_t *r = (sys_ana_reg4_t *)(SOC_SYS_REG_BASE + (0x44 << 2));

	SOC_LOGI("ana_reg4: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x44 << 2)));
	SOC_LOGI("	anabuf_sel_tx: %8x\r\n", r->anabuf_sel_tx);
	SOC_LOGI("	trng_tsten: %8x\r\n", r->trng_tsten);
	SOC_LOGI("	itune_ref: %8x\r\n", r->itune_ref);
	SOC_LOGI("	itune_opa: %8x\r\n", r->itune_opa);
	SOC_LOGI("	itune_cmp: %8x\r\n", r->itune_cmp);
	SOC_LOGI("	rnooise_sel: %8x\r\n", r->rnooise_sel);
	SOC_LOGI("	fslow_sel: %8x\r\n", r->fslow_sel);
	SOC_LOGI("	ffast_sel: %8x\r\n", r->ffast_sel);
	SOC_LOGI("	trng_tstck_sel: %8x\r\n", r->trng_tstck_sel);
	SOC_LOGI("	cktst_sel: %8x\r\n", r->cktst_sel);
	SOC_LOGI("	ck_tst_enbale: %8x\r\n", r->ck_tst_enbale);
	SOC_LOGI("	sw_bias: %8x\r\n", r->sw_bias);
	SOC_LOGI("	crb: %8x\r\n", r->crb);
	SOC_LOGI("	port_enablel: %8x\r\n", r->port_enablel);
}

static void sys_dump_ana_reg5(void)
{
	sys_ana_reg5_t *r = (sys_ana_reg5_t *)(SOC_SYS_REG_BASE + (0x45 << 2));

	SOC_LOGI("ana_reg5: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x45 << 2)));
	SOC_LOGI("	en_usb: %8x\r\n", r->en_usb);
	SOC_LOGI("	en_xtall: %8x\r\n", r->en_xtall);
	SOC_LOGI("	en_dco: %8x\r\n", r->en_dco);
	SOC_LOGI("	en_ram: %8x\r\n", r->en_ram);
	SOC_LOGI("	en_temp: %8x\r\n", r->en_temp);
	SOC_LOGI("	en_dpll: %8x\r\n", r->en_dpll);
	SOC_LOGI("	en_cb: %8x\r\n", r->en_cb);
	SOC_LOGI("	en_lcd: %8x\r\n", r->en_lcd);
	SOC_LOGI("	trxspi_ctrl: %8x\r\n", r->trxspi_ctrl);
	SOC_LOGI("	adc_div: %8x\r\n", r->adc_div);
	SOC_LOGI("	usb_speed: %8x\r\n", r->usb_speed);
	SOC_LOGI("	spideepsleep: %8x\r\n", r->spideepsleep);
	SOC_LOGI("	vsel: %8x\r\n", r->vsel);
	SOC_LOGI("	swb: %8x\r\n", r->swb);
	SOC_LOGI("	itune_xtall: %8x\r\n", r->itune_xtall);
	SOC_LOGI("	xtall_ten: %8x\r\n", r->xtall_ten);
	SOC_LOGI("	rosc_tsten: %8x\r\n", r->rosc_tsten);
	SOC_LOGI("	bcal_start: %8x\r\n", r->bcal_start);
	SOC_LOGI("	bcal_en: %8x\r\n", r->bcal_en);
	SOC_LOGI("	bcal_sel: %8x\r\n", r->bcal_sel);
	SOC_LOGI("	vbias: %8x\r\n", r->vbias);
}

static void sys_dump_ana_reg6(void)
{
	sys_ana_reg6_t *r = (sys_ana_reg6_t *)(SOC_SYS_REG_BASE + (0x46 << 2));

	SOC_LOGI("ana_reg6: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x46 << 2)));
	SOC_LOGI("	calib_interval: %8x\r\n", r->calib_interval);
	SOC_LOGI("	modify_interval: %8x\r\n", r->modify_interval);
	SOC_LOGI("	xtal_wakeup_time: %8x\r\n", r->xtal_wakeup_time);
	SOC_LOGI("	spi_trig: %8x\r\n", r->spi_trig);
	SOC_LOGI("	modifi_auto: %8x\r\n", r->modifi_auto);
	SOC_LOGI("	calib_auto: %8x\r\n", r->calib_auto);
	SOC_LOGI("	cal_mode: %8x\r\n", r->cal_mode);
	SOC_LOGI("	manu_ena: %8x\r\n", r->manu_ena);
	SOC_LOGI("	manu_cin: %8x\r\n", r->manu_cin);
}

static void sys_dump_ana_reg7(void)
{
	SOC_LOGI("ana_reg7: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x47 << 2)));
}

static void sys_dump_ana_reg8(void)
{
	sys_ana_reg8_t *r = (sys_ana_reg8_t *)(SOC_SYS_REG_BASE + (0x48 << 2));

	SOC_LOGI("ana_reg8: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x48 << 2)));
	SOC_LOGI("	ioldo_lp: %8x\r\n", r->ioldo_lp);
	SOC_LOGI("	coreldo_hp: %8x\r\n", r->coreldo_hp);
	SOC_LOGI("	dldohp: %8x\r\n", r->dldohp);
	SOC_LOGI("	t_vanaldosel: %8x\r\n", r->t_vanaldosel);
	SOC_LOGI("	r_vanaldosel: %8x\r\n", r->r_vanaldosel);
	SOC_LOGI("	en_trsw: %8x\r\n", r->en_trsw);
	SOC_LOGI("	aldohp: %8x\r\n", r->aldohp);
	SOC_LOGI("	anacurlim: %8x\r\n", r->anacurlim);
	SOC_LOGI("	violdosel: %8x\r\n", r->violdosel);
	SOC_LOGI("	iocurlim: %8x\r\n", r->iocurlim);
	SOC_LOGI("	valoldosel: %8x\r\n", r->valoldosel);
	SOC_LOGI("	alopowsel: %8x\r\n", r->alopowsel);
	SOC_LOGI("	en_fast_aloldo: %8x\r\n", r->en_fast_aloldo);
	SOC_LOGI("	aloldohp: %8x\r\n", r->aloldohp);
	SOC_LOGI("	bgcal: %8x\r\n", r->bgcal);
	SOC_LOGI("	vbgcalmode: %8x\r\n", r->vbgcalmode);
	SOC_LOGI("	vbgcalstart: %8x\r\n", r->vbgcalstart);
	SOC_LOGI("	pwd_bgcal: %8x\r\n", r->pwd_bgcal);
	SOC_LOGI("	spi_envbg: %8x\r\n", r->spi_envbg);
}

static void sys_dump_ana_reg9(void)
{
	sys_ana_reg9_t *r = (sys_ana_reg9_t *)(SOC_SYS_REG_BASE + (0x49 << 2));

	SOC_LOGI("ana_reg9: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x49 << 2)));
	SOC_LOGI("	wkgpiosel1: %8x\r\n", r->wkgpiosel1);
	SOC_LOGI("	rst_wks1v: %8x\r\n", r->rst_wks1v);
	SOC_LOGI("	wkgpiosel2: %8x\r\n", r->wkgpiosel2);
	SOC_LOGI("	spi_latch1v: %8x\r\n", r->spi_latch1v);
	SOC_LOGI("	digcurlim: %8x\r\n", r->digcurlim);
	SOC_LOGI("	pupres_enb1v: %8x\r\n", r->pupres_enb1v);
	SOC_LOGI("	pdnres_en1v: %8x\r\n", r->pdnres_en1v);
	SOC_LOGI("	d_veasel1v: %8x\r\n", r->d_veasel1v);
	SOC_LOGI("	ensfsdd: %8x\r\n", r->ensfsdd);
	SOC_LOGI("	vcorehsel: %8x\r\n", r->vcorehsel);
	SOC_LOGI("	vcorelsel: %8x\r\n", r->vcorelsel);
	SOC_LOGI("	vlden: %8x\r\n", r->vlden);
	SOC_LOGI("	en_fast_coreldo: %8x\r\n", r->en_fast_coreldo);
	SOC_LOGI("	pwdcoreldo: %8x\r\n", r->pwdcoreldo);
	SOC_LOGI("	vdighsel: %8x\r\n", r->vdighsel);
	SOC_LOGI("	vdigsel: %8x\r\n", r->vdigsel);
	SOC_LOGI("	vdd12lden: %8x\r\n", r->vdd12lden);
}

static void sys_dump_ana_reg10(void)
{
	sys_ana_reg10_t *r = (sys_ana_reg10_t *)(SOC_SYS_REG_BASE + (0x4a << 2));

	SOC_LOGI("ana_reg10: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4a << 2)));
	SOC_LOGI("	pasoft_st: %8x\r\n", r->pasoft_st);
	SOC_LOGI("	azcdcnt_manu: %8x\r\n", r->azcdcnt_manu);
	SOC_LOGI("	timer_sel: %8x\r\n", r->timer_sel);
	SOC_LOGI("	vpabucksel: %8x\r\n", r->vpabucksel);
	SOC_LOGI("	spi_timerwken: %8x\r\n", r->spi_timerwken);
	SOC_LOGI("	nc_16_16: %8x\r\n", r->nc_16_16);
	SOC_LOGI("	sd: %8x\r\n", r->sd);
	SOC_LOGI("	ioldosel: %8x\r\n", r->ioldosel);
	SOC_LOGI("	iobyapssen: %8x\r\n", r->iobyapssen);
	SOC_LOGI("	ckfs: %8x\r\n", r->ckfs);
	SOC_LOGI("	ckintsel: %8x\r\n", r->ckintsel);
	SOC_LOGI("	osccaltrig: %8x\r\n", r->osccaltrig);
	SOC_LOGI("	mroscsel: %8x\r\n", r->mroscsel);
	SOC_LOGI("	mrosci_cal: %8x\r\n", r->mrosci_cal);
	SOC_LOGI("	mrosccap_cal: %8x\r\n", r->mrosccap_cal);
}

static void sys_dump_ana_reg11(void)
{
	sys_ana_reg11_t *r = (sys_ana_reg11_t *)(SOC_SYS_REG_BASE + (0x4b << 2));

	SOC_LOGI("ana_reg11: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4b << 2)));
	SOC_LOGI("	sfsr: %8x\r\n", r->sfsr);
	SOC_LOGI("	ensfsaa: %8x\r\n", r->ensfsaa);
	SOC_LOGI("	apfms: %8x\r\n", r->apfms);
	SOC_LOGI("	atmpo_sel: %8x\r\n", r->atmpo_sel);
	SOC_LOGI("	ampoen: %8x\r\n", r->ampoen);
	SOC_LOGI("	enpowa: %8x\r\n", r->enpowa);
	SOC_LOGI("	avea_sel: %8x\r\n", r->avea_sel);
	SOC_LOGI("	aforcepfm: %8x\r\n", r->aforcepfm);
	SOC_LOGI("	acls: %8x\r\n", r->acls);
	SOC_LOGI("	aswrsten: %8x\r\n", r->aswrsten);
	SOC_LOGI("	aripc: %8x\r\n", r->aripc);
	SOC_LOGI("	arampc: %8x\r\n", r->arampc);
	SOC_LOGI("	arampcen: %8x\r\n", r->arampcen);
	SOC_LOGI("	aenburst: %8x\r\n", r->aenburst);
	SOC_LOGI("	apfmen: %8x\r\n", r->apfmen);
	SOC_LOGI("	aldosel: %8x\r\n", r->aldosel);
}

static void sys_dump_ana_reg12(void)
{
	sys_ana_reg12_t *r = (sys_ana_reg12_t *)(SOC_SYS_REG_BASE + (0x4c << 2));

	SOC_LOGI("ana_reg12: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4c << 2)));
	SOC_LOGI("	buckd_softst: %8x\r\n", r->buckd_softst);
	SOC_LOGI("	dzcdcnt_manu: %8x\r\n", r->dzcdcnt_manu);
	SOC_LOGI("	clk_sel: %8x\r\n", r->clk_sel);
	SOC_LOGI("	dpfms: %8x\r\n", r->dpfms);
	SOC_LOGI("	dtmpo_sel: %8x\r\n", r->dtmpo_sel);
	SOC_LOGI("	dmpoen: %8x\r\n", r->dmpoen);
	SOC_LOGI("	dforcepfm: %8x\r\n", r->dforcepfm);
	SOC_LOGI("	dcls: %8x\r\n", r->dcls);
	SOC_LOGI("	dswrsten: %8x\r\n", r->dswrsten);
	SOC_LOGI("	dripc: %8x\r\n", r->dripc);
	SOC_LOGI("	drampc: %8x\r\n", r->drampc);
	SOC_LOGI("	drampcen: %8x\r\n", r->drampcen);
	SOC_LOGI("	denburst: %8x\r\n", r->denburst);
	SOC_LOGI("	dpfmen: %8x\r\n", r->dpfmen);
	SOC_LOGI("	dldosel: %8x\r\n", r->dldosel);
}

static void sys_dump_ana_reg13(void)
{
	sys_ana_reg13_t *r = (sys_ana_reg13_t *)(SOC_SYS_REG_BASE + (0x4d << 2));

	SOC_LOGI("ana_reg13: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4d << 2)));
	SOC_LOGI("	pwdovp1v: %8x\r\n", r->pwdovp1v);
	SOC_LOGI("	asoft_stc: %8x\r\n", r->asoft_stc);
	SOC_LOGI("	volen: %8x\r\n", r->volen);
	SOC_LOGI("	dpfms: %8x\r\n", r->dpfms);
	SOC_LOGI("	dtmpo_sel: %8x\r\n", r->dtmpo_sel);
	SOC_LOGI("	dmpoen: %8x\r\n", r->dmpoen);
	SOC_LOGI("	pavea_sel: %8x\r\n", r->pavea_sel);
	SOC_LOGI("	dforcepfm: %8x\r\n", r->dforcepfm);
	SOC_LOGI("	dcls: %8x\r\n", r->dcls);
	SOC_LOGI("	dswrsten: %8x\r\n", r->dswrsten);
	SOC_LOGI("	dripc: %8x\r\n", r->dripc);
	SOC_LOGI("	drampc: %8x\r\n", r->drampc);
	SOC_LOGI("	drampcen: %8x\r\n", r->drampcen);
	SOC_LOGI("	paenburst: %8x\r\n", r->paenburst);
	SOC_LOGI("	papfmen: %8x\r\n", r->papfmen);
	SOC_LOGI("	enbuckpa: %8x\r\n", r->enbuckpa);
}

static void sys_dump_ana_reg14(void)
{
	sys_ana_reg14_t *r = (sys_ana_reg14_t *)(SOC_SYS_REG_BASE + (0x4e << 2));

	SOC_LOGI("ana_reg14: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4e << 2)));
	SOC_LOGI("	reg: %8x\r\n", r->reg);
	SOC_LOGI("	en_adcmode: %8x\r\n", r->en_adcmode);
	SOC_LOGI("	en_out_test1v: %8x\r\n", r->en_out_test1v);
	SOC_LOGI("	nc_12_12: %8x\r\n", r->nc_12_12);
	SOC_LOGI("	sel_seri_cap: %8x\r\n", r->sel_seri_cap);
	SOC_LOGI("	en_seri_cap: %8x\r\n", r->en_seri_cap);
	SOC_LOGI("	cal_ctrl: %8x\r\n", r->cal_ctrl);
	SOC_LOGI("	cal_vth: %8x\r\n", r->cal_vth);
	SOC_LOGI("	crg: %8x\r\n", r->crg);
	SOC_LOGI("	vrefs: %8x\r\n", r->vrefs);
	SOC_LOGI("	gain_s: %8x\r\n", r->gain_s);
	SOC_LOGI("	td_latch1v: %8x\r\n", r->td_latch1v);
	SOC_LOGI("	pwd_td: %8x\r\n", r->pwd_td);
}

static void sys_dump_ana_reg15(void)
{
	sys_ana_reg15_t *r = (sys_ana_reg15_t *)(SOC_SYS_REG_BASE + (0x4f << 2));

	SOC_LOGI("ana_reg15: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x4f << 2)));
	SOC_LOGI("	test_number1v: %8x\r\n", r->test_number1v);
	SOC_LOGI("	test_period1v: %8x\r\n", r->test_period1v);
	SOC_LOGI("	chs: %8x\r\n", r->chs);
	SOC_LOGI("	chs_sel_cal1v: %8x\r\n", r->chs_sel_cal1v);
	SOC_LOGI("	cal_done_clr1v: %8x\r\n", r->cal_done_clr1v);
	SOC_LOGI("	en_cal_force1v: %8x\r\n", r->en_cal_force1v);
	SOC_LOGI("	en_cal_auto1v: %8x\r\n", r->en_cal_auto1v);
	SOC_LOGI("	en_scan: %8x\r\n", r->en_scan);
}

static void sys_dump_ana_reg16(void)
{
	sys_ana_reg16_t *r = (sys_ana_reg16_t *)(SOC_SYS_REG_BASE + (0x50 << 2));

	SOC_LOGI("ana_reg16: %8x\r\n", REG_READ(SOC_SYSTEM_REG_BASE + (0x50 << 2)));
	SOC_LOGI("	int_en: %8x\r\n", r->int_en);
	SOC_LOGI("	int_en16: %8x\r\n", r->int_en16);
	SOC_LOGI("	nc_17_17: %8x\r\n", r->nc_17_17);
	SOC_LOGI("	nc_18_18: %8x\r\n", r->nc_18_18);
	SOC_LOGI("	cal_number1v: %8x\r\n", r->cal_number1v);
	SOC_LOGI("	cal_period1v: %8x\r\n", r->cal_period1v);
}

static void sys_dump_ana_reg17(void)
{
	sys_ana_reg17_t *r = (sys_ana_reg17_t *)(SOC_SYS_REG_BASE + (0x51 << 2));

	SOC_LOGI("ana_reg17: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x51 << 2)));
	SOC_LOGI("	int_clr: %8x\r\n", r->int_clr);
	SOC_LOGI("	int_clr16: %8x\r\n", r->int_clr16);
	SOC_LOGI("	ck_adc_sel: %8x\r\n", r->ck_adc_sel);
	SOC_LOGI("	int_clr_sel: %8x\r\n", r->int_clr_sel);
	SOC_LOGI("	en_lpmod: %8x\r\n", r->en_lpmod);
	SOC_LOGI("	en_testcmp1v: %8x\r\n", r->en_testcmp1v);
	SOC_LOGI("	en_man_wr1v: %8x\r\n", r->en_man_wr1v);
	SOC_LOGI("	en_manmod1v: %8x\r\n", r->en_manmod1v);
	SOC_LOGI("	cap_calspi1v: %8x\r\n", r->cap_calspi1v);
}

static void sys_dump_ana_reg18(void)
{
	sys_ana_reg18_t *r = (sys_ana_reg18_t *)(SOC_SYS_REG_BASE + (0x52 << 2));

	SOC_LOGI("ana_reg18: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x52 << 2)));
	SOC_LOGI("	iselaud: %8x\r\n", r->iselaud);
	SOC_LOGI("	audck_rlcen1v: %8x\r\n", r->audck_rlcen1v);
	SOC_LOGI("	lchckinven1v: %8x\r\n", r->lchckinven1v);
	SOC_LOGI("	enaudbias: %8x\r\n", r->enaudbias);
	SOC_LOGI("	enadcbias: %8x\r\n", r->enadcbias);
	SOC_LOGI("	enmicbias: %8x\r\n", r->enmicbias);
	SOC_LOGI("	adcckinven1v: %8x\r\n", r->adcckinven1v);
	SOC_LOGI("	dacfb2st0v9: %8x\r\n", r->dacfb2st0v9);
	SOC_LOGI("	nc_8_8: %8x\r\n", r->nc_8_8);
	SOC_LOGI("	micbias_trm: %8x\r\n", r->micbias_trm);
	SOC_LOGI("	micbias_voc: %8x\r\n", r->micbias_voc);
	SOC_LOGI("	vrefsel1v: %8x\r\n", r->vrefsel1v);
	SOC_LOGI("	capswspi: %8x\r\n", r->capswspi);
	SOC_LOGI("	adref_sel: %8x\r\n", r->adref_sel);
	SOC_LOGI("	nc_24_25: %8x\r\n", r->nc_24_25);
	SOC_LOGI("	reserved_bit_26_30: %8x\r\n", r->reserved_bit_26_30);
	SOC_LOGI("	spi_dacckpssel: %8x\r\n", r->spi_dacckpssel);
}

static void sys_dump_ana_reg19(void)
{
	sys_ana_reg19_t *r = (sys_ana_reg19_t *)(SOC_SYS_REG_BASE + (0x53 << 2));

	SOC_LOGI("ana_reg19: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x53 << 2)));
	SOC_LOGI("	isel: %8x\r\n", r->isel);
	SOC_LOGI("	micirsel1: %8x\r\n", r->micirsel1);
	SOC_LOGI("	micdacit: %8x\r\n", r->micdacit);
	SOC_LOGI("	micdacih: %8x\r\n", r->micdacih);
	SOC_LOGI("	micsingleen: %8x\r\n", r->micsingleen);
	SOC_LOGI("	dccompen: %8x\r\n", r->dccompen);
	SOC_LOGI("	micgain: %8x\r\n", r->micgain);
	SOC_LOGI("	micdacen: %8x\r\n", r->micdacen);
	SOC_LOGI("	stg2lsen1v: %8x\r\n", r->stg2lsen1v);
	SOC_LOGI("	openloopcal1v: %8x\r\n", r->openloopcal1v);
	SOC_LOGI("	callatch: %8x\r\n", r->callatch);
	SOC_LOGI("	vcmsel: %8x\r\n", r->vcmsel);
	SOC_LOGI("	dwamode: %8x\r\n", r->dwamode);
	SOC_LOGI("	r2ren: %8x\r\n", r->r2ren);
	SOC_LOGI("	nc_26_27: %8x\r\n", r->nc_26_27);
	SOC_LOGI("	micen: %8x\r\n", r->micen);
	SOC_LOGI("	rst: %8x\r\n", r->rst);
	SOC_LOGI("	bpdwa1v: %8x\r\n", r->bpdwa1v);
	SOC_LOGI("	hcen1stg: %8x\r\n", r->hcen1stg);
}

static void sys_dump_ana_reg20(void)
{
	sys_ana_reg20_t *r = (sys_ana_reg20_t *)(SOC_SYS_REG_BASE + (0x54 << 2));

	SOC_LOGI("ana_reg20: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x54 << 2)));
	SOC_LOGI("	hpdac: %8x\r\n", r->hpdac);
	SOC_LOGI("	calcon_sel: %8x\r\n", r->calcon_sel);
	SOC_LOGI("	oscdac: %8x\r\n", r->oscdac);
	SOC_LOGI("	ocendac: %8x\r\n", r->ocendac);
	SOC_LOGI("	vcmsel: %8x\r\n", r->vcmsel);
	SOC_LOGI("	adjdacref: %8x\r\n", r->adjdacref);
	SOC_LOGI("	dcochg: %8x\r\n", r->dcochg);
	SOC_LOGI("	diffen: %8x\r\n", r->diffen);
	SOC_LOGI("	endaccal: %8x\r\n", r->endaccal);
	SOC_LOGI("	nc_15_15: %8x\r\n", r->nc_15_15);
	SOC_LOGI("	lendcoc: %8x\r\n", r->lendcoc);
	SOC_LOGI("	nc_17_17: %8x\r\n", r->nc_17_17);
	SOC_LOGI("	lenvcmd: %8x\r\n", r->lenvcmd);
	SOC_LOGI("	dacdrven: %8x\r\n", r->dacdrven);
	SOC_LOGI("	nc_20_20: %8x\r\n", r->nc_20_20);
	SOC_LOGI("	daclen: %8x\r\n", r->daclen);
	SOC_LOGI("	dacg: %8x\r\n", r->dacg);
	SOC_LOGI("	dacmute: %8x\r\n", r->dacmute);
	SOC_LOGI("	dacdwamode_sel: %8x\r\n", r->dacdwamode_sel);
	SOC_LOGI("	dacsel: %8x\r\n", r->dacsel);
}

static void sys_dump_ana_reg21(void)
{
	sys_ana_reg21_t *r = (sys_ana_reg21_t *)(SOC_SYS_REG_BASE + (0x55 << 2));

	SOC_LOGI("ana_reg21: %8x\r\n", REG_READ(SOC_SYS_REG_BASE + (0x55 << 2)));
	SOC_LOGI("	lmdcin: %8x\r\n", r->lmdcin);
	SOC_LOGI("	nc_8_15: %8x\r\n", r->nc_8_15);
	SOC_LOGI("	spirst_ovc: %8x\r\n", r->spirst_ovc);
	SOC_LOGI("	nc_17_17: %8x\r\n", r->nc_17_17);
	SOC_LOGI("	enidacl: %8x\r\n", r->enidacl);
	SOC_LOGI("	dac3rdhc0v9: %8x\r\n", r->dac3rdhc0v9);
	SOC_LOGI("	hc2s: %8x\r\n", r->hc2s);
	SOC_LOGI("	rfb_ctrl: %8x\r\n", r->rfb_ctrl);
	SOC_LOGI("	vcmsel: %8x\r\n", r->vcmsel);
	SOC_LOGI("	enbs: %8x\r\n", r->enbs);
	SOC_LOGI("	calck_sel0v9: %8x\r\n", r->calck_sel0v9);
	SOC_LOGI("	bpdwa0v9: %8x\r\n", r->bpdwa0v9);
	SOC_LOGI("	looprst0v9: %8x\r\n", r->looprst0v9);
	SOC_LOGI("	oct0v9: %8x\r\n", r->oct0v9);
	SOC_LOGI("	sout0v9: %8x\r\n", r->sout0v9);
	SOC_LOGI("	hc0v9: %8x\r\n", r->hc0v9);
}

static sys_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, sys_dump_device_id},
	{0x1, 0x1, sys_dump_version_id},
	{0x2, 0x2, sys_dump_cpu_storage_connect_op_select},
	{0x3, 0x3, sys_dump_cpu_current_run_status},
	{0x4, 0x4, sys_dump_cpu0_int_halt_clk_op},
	{0x5, 0x5, sys_dump_cpu1_int_halt_clk_op},
	{0x6, 0x6, sys_dump_reserved_reg0x6},
	{0x7, 0x8, sys_dump_rsv_7_7},
	{0x8, 0x8, sys_dump_cpu_clk_div_mode1},
	{0x9, 0x9, sys_dump_cpu_clk_div_mode2},
	{0xa, 0xa, sys_dump_cpu_26m_wdt_clk_div},
	{0xb, 0xb, sys_dump_cpu_anaspi_freq},
	{0xc, 0xc, sys_dump_cpu_device_clk_enable},
	{0xd, 0xd, sys_dump_reserver_reg0xd},
	{0xe, 0xe, sys_dump_cpu_device_mem_ctrl1},
	{0xf, 0xf, sys_dump_cpu_device_mem_ctrl2},
	{0x10, 0x10, sys_dump_cpu_power_sleep_wakeup},
	{0x11, 0x11, sys_dump_reserver_reg0x11},
	{0x12, 0x12, sys_dump_cpu_device_mem_ctrl3},
	{0x13, 0x20, sys_dump_rsv_13_1f},
	{0x20, 0x20, sys_dump_cpu0_int_0_31_en},
	{0x21, 0x21, sys_dump_cpu0_int_32_63_en},
	{0x22, 0x22, sys_dump_cpu1_int_0_31_en},
	{0x23, 0x23, sys_dump_cpu1_int_32_63_en},
	{0x24, 0x28, sys_dump_rsv_24_27},
	{0x28, 0x28, sys_dump_cpu0_int_0_31_status},
	{0x29, 0x29, sys_dump_cpu0_int_32_63_status},
	{0x2a, 0x2a, sys_dump_cpu1_int_0_31_status},
	{0x2b, 0x2b, sys_dump_cpu1_int_32_63_status},
	{0x2c, 0x30, sys_dump_rsv_2c_2f},
	{0x30, 0x30, sys_dump_gpio_config0},
	{0x31, 0x31, sys_dump_gpio_config1},
	{0x32, 0x32, sys_dump_gpio_config2},
	{0x33, 0x33, sys_dump_gpio_config3},
	{0x34, 0x34, sys_dump_gpio_config4},
	{0x35, 0x35, sys_dump_gpio_config5},
	{0x36, 0x38, sys_dump_rsv_36_37},
	{0x38, 0x38, sys_dump_sys_debug_config0},
	{0x39, 0x39, sys_dump_sys_debug_config1},
	{0x3a, 0x40, sys_dump_rsv_3a_3f},
	{0x40, 0x40, sys_dump_ana_reg0},
	{0x41, 0x41, sys_dump_ana_reg1},
	{0x42, 0x42, sys_dump_ana_reg2},
	{0x43, 0x43, sys_dump_ana_reg3},
	{0x44, 0x44, sys_dump_ana_reg4},
	{0x45, 0x45, sys_dump_ana_reg5},
	{0x46, 0x46, sys_dump_ana_reg6},
	{0x47, 0x47, sys_dump_ana_reg7},
	{0x48, 0x48, sys_dump_ana_reg8},
	{0x49, 0x49, sys_dump_ana_reg9},
	{0x4a, 0x4a, sys_dump_ana_reg10},
	{0x4b, 0x4b, sys_dump_ana_reg11},
	{0x4c, 0x4c, sys_dump_ana_reg12},
	{0x4d, 0x4d, sys_dump_ana_reg13},
	{0x4e, 0x4e, sys_dump_ana_reg14},
	{0x4f, 0x4f, sys_dump_ana_reg15},
	{0x50, 0x50, sys_dump_ana_reg16},
	{0x51, 0x51, sys_dump_ana_reg17},
	{0x52, 0x52, sys_dump_ana_reg18},
	{0x53, 0x53, sys_dump_ana_reg19},
	{0x54, 0x54, sys_dump_ana_reg20},
	{0x55, 0x55, sys_dump_ana_reg21},
	{-1, -1, 0}
};

void sys_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}

#endif

