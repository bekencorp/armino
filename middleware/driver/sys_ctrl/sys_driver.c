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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"

/**  Platform Start **/
//Platform

/** Platform Misc Start **/
void sys_drv_init()
{
	uint32_t int_level = rtos_disable_int();

	sys_amp_res_init();
	sys_hal_init();

	rtos_enable_int(int_level);
}

/** Platform Misc End **/

uint32 sys_drv_get_chip_id(void)
{
	uint32 reg = 0;
	uint32_t int_level = rtos_disable_int();

	reg = sys_hal_get_chip_id();

	rtos_enable_int(int_level);

	return reg;
}

// Replace sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL)
uint32 sys_drv_get_device_id(void)
{
	uint32 reg = 0;
	uint32_t int_level = rtos_disable_int();

	reg = sys_hal_get_device_id();

	rtos_enable_int(int_level);

	return reg;
}

int32 sys_drv_set_jtag_mode(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_jtag_mode(param);
	rtos_enable_int(int_level);

	return ret;
}

uint32 sys_drv_get_jtag_mode(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_jtag_mode();
	rtos_enable_int(int_level);

	return ret;
}

void sys_drv_en_tempdet(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_en_tempdet(value);
    rtos_enable_int(int_level);
}
/**  Platform End **/

uint32_t sys_drv_get_cpu_storage_connect_op_select_flash_sel(void)
{
	return 	sys_hal_get_cpu_storage_connect_op_select_flash_sel();
}

void sys_drv_set_cpu_storage_connect_op_select_flash_sel(uint32_t value)
{
	sys_hal_set_cpu_storage_connect_op_select_flash_sel(value);
}

/**  Misc Start **/
//Misc
/**  Misc End **/

#if 1
void system_driver_set_bts_wakeup_platform_en(bool value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	if(value)
		sys_hal_set_bts_wakeup_platform_en(1);
	else
		sys_hal_set_bts_wakeup_platform_en(0);

	if(!ret)
		ret = sys_amp_res_release();

}
uint32_t system_driver_get_bts_wakeup_platform_en()
{
	return sys_hal_get_bts_wakeup_platform_en();
}

void system_driver_set_bts_sleep_exit_req(bool value)
{
	if(value)
		sys_hal_set_bts_sleep_exit_req(1);
	else
		sys_hal_set_bts_sleep_exit_req(0);
}
uint32_t system_driver_get_bts_sleep_exit_req()
{
	return sys_hal_get_bts_sleep_exit_req();
}
#endif

void sys_drv_set_ana_trxt_tst_enable(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_trxt_tst_enable(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_scal_en(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_scal_en(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_gadc_buf_ictrl(uint32_t value)
{
   uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_gadc_buf_ictrl(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_gadc_cmp_ictrl(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_gadc_cmp_ictrl(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_pwd_gadc_buf(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_pwd_gadc_buf(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_vref_sel(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_vref_sel(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_cb_cal_manu(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_cb_cal_manu(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_cb_cal_trig(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_cb_cal_trig(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_cb_cal_manu_val(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_cb_cal_manu_val(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_vlsel_ldodig(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vlsel_ldodig(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_vhsel_ldodig(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vhsel_ldodig(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_vctrl_sysldo(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vctrl_sysldo(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_vtempsel(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vtempsel(value);
    rtos_enable_int(int_level);
}


