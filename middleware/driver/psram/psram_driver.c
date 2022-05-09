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

#include <driver/dma.h>
#include <driver/int.h>
#include <os/mem.h>
#include "clock_driver.h"
#include "sys_driver.h"
#include "psram_hal.h"

extern int delay(INT32 num);
#define addSYSTEM_Reg0xe                                        *((volatile unsigned long *) (0x44010000+0xe*4))

#define addFLASH_Reg0x7                                         *((volatile unsigned long *) (0x44030000+0x7*4))
#define set_FLASH_Reg0x7_mode_sel(val)                          addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0x1F0)) | ((val) << 4))
#define get_FLASH_Reg0x7_mode_sel                               ((addFLASH_Reg0x7 & 0x1F0) >> 4)


static bool s_psram_driver_is_init = false;

static void psram_init_common(void)
{
	//addSYSTEM_Reg0x46 |=  (0x1 << 9);// psram 上电
	sys_drv_psram_ldo_enable(0x1);//bit9//psram 上电

	set_FLASH_Reg0x7_mode_sel(0x1);
	//flash_set_line_mode(0x1);//sys_drv_flash_mode_sel(0x1);//set bit4-8
	//sys_drv_set_flash_mode(0x1);

	//set_SYSTEM_Reg0x9_cksel_flash(0x2);
	sys_drv_flash_cksel(0x2);//bit 24,25

	addSYSTEM_Reg0xe |= (0x1 << 25); // not set

	//setf_SYSTEM_Reg0x9_cksel_psram;//480M
	sys_drv_psram_clk_sel(0x1);//480M

	//set_SYSTEM_Reg0x9_ckdiv_psram(1);//120M
	sys_drv_psram_set_clkdiv(0x1);

	//setf_SYSTEM_Reg0xc_psram_cken;
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_UP);//psram_clk_enable bit19=1
}

bk_err_t bk_psram_driver_init(void)
{
	if (s_psram_driver_is_init)
		return BK_OK;
	//sys_drv_psram_dpll_enable(1);//bit12=1
	//sys_drv_psram_dco_enable(1);//bit8=1
	sys_drv_psram_xtall_osc_enable(1);//bit7=1
	sys_drv_psram_volstage_sel(1);//bit5=0/1:1:1.8v
	s_psram_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_psram_driver_deinit(void)
{
	if (!s_psram_driver_is_init) {
		return BK_OK;
	}

	//sys_drv_psram_dpll_enable(0);//bit12=0
	//sys_drv_psram_dco_enable(0);//bit8=1
	//sys_drv_psram_xtall_osc_enable(0);//bit7=1
	sys_drv_psram_volstage_sel(0);//bit5=0/1:1:1.8v
	s_psram_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_psram_init(uint32_t mode)
{
	uint32_t val = 0;

	psram_init_common();
	delay(1000);

	psram_hal_set_mode_value(mode);
	delay(1500);

	psram_hal_set_cmd_reset();
	delay(1500);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101
	delay(100);

	val = psram_hal_get_regb_value();
	val = (val & ~(0x7 << 10)) | (0x4 << 10);//read latency 100 166Mhz
	//	val = (val & ~(0x1f << 8)) | (0x10 << 8);//read latency 100 166Mhz dstr set 00
	psram_hal_cmd_write(0x00000000, val);
	delay(100);

	psram_hal_cmd_read(0x00000004);

	val = psram_hal_get_regb_value();
	val = (val & ~(0x7 << 13)) | (0x6 << 13);//write latency 110 166Mhz

	psram_hal_cmd_write(0x00000004, val);
	delay(1000);
	return BK_OK;
}

bk_err_t bk_psram_deinit(void)
{
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_DOWN);//psram_clk_disable
	delay(1000);
	sys_drv_psram_ldo_enable(0); // 断电
	return BK_OK;
}

