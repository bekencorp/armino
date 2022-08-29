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

#include <driver/int.h>
#include <os/mem.h>
#include "clock_driver.h"
#include "sys_driver.h"
#include "psram_hal.h"
#include "aon_pmu_hal.h"
#include "driver/psram_types.h"

extern int delay(INT32 num);
#define addSYSTEM_Reg0xe                                        *((volatile unsigned long *) (0x44010000+0xe*4))

#define addFLASH_Reg0x7                                         *((volatile unsigned long *) (0x44030000+0x7*4))
#define set_FLASH_Reg0x7_mode_sel(val)                          addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0x1F0)) | ((val) << 4))
#define get_FLASH_Reg0x7_mode_sel                               ((addFLASH_Reg0x7 & 0x1F0) >> 4)

#define BK_ERR_PSRAM_DRIVER_NOT_INIT       (BK_ERR_PSRAM_BASE - 1) /**< psram driver not init */
#define BK_ERR_PSRAM_SERVER_NOT_INIT       (BK_ERR_PSRAM_BASE - 2) /**< psram server not init */


static bool s_psram_driver_is_init = false;
static bool s_psram_server_is_init = false;

#define PSRAM_RETURN_ON_DRIVER_NOT_INIT() do {\
        if (!s_psram_driver_is_init) {\
            return BK_ERR_PSRAM_DRIVER_NOT_INIT;\
        }\
    } while(0)

#define PSRAM_RETURN_ON_SERVER_NOT_INIT() do {\
				if (!s_psram_server_is_init) {\
					return BK_ERR_PSRAM_SERVER_NOT_INIT;\
				}\
			} while(0)

static void psram_init_common(void)
{
	addSYSTEM_Reg0xe |= (0x1 << 25); // not set

	//setf_SYSTEM_Reg0x9_cksel_psram;//480M
	sys_drv_psram_clk_sel(0x0);// 0/1:160M/240M

	//set_SYSTEM_Reg0x9_ckdiv_psram(1);//120M
	sys_drv_psram_set_clkdiv(0x0);//0/1:div

	// when use psram 120M, need open this code
	//aon_pmu_hal_psram_iodrv_set(0x2);

	//setf_SYSTEM_Reg0xc_psram_cken;
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_UP);//psram_clk_enable bit19=1
}

bk_err_t bk_psram_driver_init(void)
{
	if (s_psram_driver_is_init)
		return BK_OK;

	sys_drv_psram_power_enable();

	//sys_drv_psram_psldo_vsel(1);

	s_psram_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_psram_driver_deinit(void)
{
	if (!s_psram_driver_is_init) {
		return BK_OK;
	}

	sys_drv_psram_ldo_enable(0); // 断电

	s_psram_driver_is_init = false;
	s_psram_server_is_init = false;

	return BK_OK;
}


bk_err_t bk_psram_set_clk(psram_clk_t clk)
{
	bk_err_t ret = BK_OK;

	switch (clk)
	{
		case PSRAM_240M:
			sys_hal_psram_clk_sel(1);    // clk sel: 0-320 1-480
			sys_hal_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_160M:
			sys_hal_psram_clk_sel(0);    // clk sel: 0-320 1-480
			sys_hal_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_120M:
			sys_hal_psram_clk_sel(1);    // clk sel: 0-320 1-480
			sys_hal_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_60M:
			sys_hal_psram_clk_sel(0);    // clk sel: 0-320 1-480
			sys_hal_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
		default:
			break;
	}

	return ret;
}

bk_err_t bk_psram_set_voltage(psram_voltage_t voltage)
{
	bk_err_t ret = BK_OK;
	
	switch (voltage)
	{
		case PSRAM_OUT_3_2V:
			sys_hal_psram_psldo_vset(0, 1);
			break;
		case PSRAM_OUT_3V:
			sys_hal_psram_psldo_vset(0, 0);
			break;
		case PSRAM_OUT_2V:
			sys_hal_psram_psldo_vset(1, 1);
			break;
		case PSRAM_OUT_1_3V:
			sys_hal_psram_psldo_vset(1, 0);
			break;
		default:
			break;
	}
	
	return ret;
}

bk_err_t bk_psram_set_transfer_mode(psram_tansfer_mode_t transfer_mode)
{
	bk_err_t ret = BK_OK;

	psram_hal_set_transfer_mode(transfer_mode);
	
	return ret;
}


bk_err_t bk_psram_init(void)
{
	PSRAM_RETURN_ON_DRIVER_NOT_INIT();

	if (s_psram_server_is_init) {
		return BK_OK;
	}

	uint32_t mode = 0xa8054043;
	uint32_t val = 0;

	psram_init_common();
	delay(1000);

	psram_hal_set_sf_reset(1);

	psram_hal_set_mode_value(mode);
	delay(1500);

	psram_hal_set_cmd_reset();
	delay(1500);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101
	delay(100);

	val = psram_hal_get_regb_value();
	//val = (val & ~(0x7 << 10)) | (0x4 << 10) | (0x3 << 8);//read latency 100 166Mhz
	val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x3 << 8);
	//	val = (val & ~(0x1f << 8)) | (0x10 << 8);//read latency 100 166Mhz dstr set 00
	psram_hal_cmd_write(0x00000000, val);
	delay(100);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	psram_hal_cmd_read(0x00000004);

	val = psram_hal_get_regb_value();
	val = (val & ~(0x7 << 13)) | (0x6 << 13);//write latency 110 166Mhz

	psram_hal_cmd_write(0x00000004, val);
	s_psram_server_is_init = true;
	delay(1000);
	return BK_OK;
}

bk_err_t bk_psram_deinit(void)
{
	PSRAM_RETURN_ON_DRIVER_NOT_INIT();

	if (!s_psram_server_is_init) {
		return BK_OK;
	}

	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_DOWN);//psram_clk_disable
	delay(1000);
	s_psram_server_is_init = false;
	return BK_OK;
}

