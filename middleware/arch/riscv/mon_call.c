// Copyright 2020-2022 Beken
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

#include <common/bk_typedef.h>
#include "mon_call.h"
#include <components/log.h>

//extern u32 riscv_sys_call(void);

extern u32 riscv_sys_call1(u32 id);
extern u32 riscv_sys_call2(u32 id, u32 param1);
extern u32 riscv_sys_call3(u32 id, u32 param1, u32 param2);
extern u32 riscv_sys_call4(u32 id, u32 param1, u32 param2, u32 param3);

void mon_task_yield(void)
{
	riscv_sys_call1(SYS_CALL_ENCLV_OS_TRAP);
}

void mon_timer_int_clear(void)
{
	riscv_sys_call1(SYS_CALL_ENCLV_MTIMER);
}

void mon_reset_cpu1(u32 enable, u32 start_addr)
{
	riscv_sys_call3(SYS_CALL_RESET_CPU_X, enable, start_addr);
}

u32 mon_set_app_entry(void *entry)
{
	return riscv_sys_call2(SYS_CALL_APP_ENTRY_SET, (u32)entry);
}

void mon_bl_2_app(void)
{
	if(riscv_sys_call1(SYS_CALL_BL_2_APP) != 0)
	{
		bk_printf("\r\nStart App failed.\r\n");
		while(1);
	}
}

u32 mon_app_started(void)
{
	return riscv_sys_call1(SYS_CALL_ENCLV_STARTED);
}

u32 mon_read_otp(u32 *buf, u32 otp_addr, u32 len)
{
	return riscv_sys_call4(SYS_CALL_READ_OTP, (u32)buf, otp_addr, len);
}

u32 mon_write_otp(u32 otp_addr, u32 *buf, u32 len)
{
	return riscv_sys_call4(SYS_CALL_WRITE_OTP, otp_addr, (u32)buf, len);
}

u32 mon_enable_otp(u32 enabled)
{
	return riscv_sys_call2(SYS_CALL_ENABLE_OTP, enabled);
}

u32 mon_pmp_cfg(u32 start_addr, u32 size, u8 attr)
{
	return riscv_sys_call4(SYS_CALL_PMP_CFG, start_addr, size, attr);
}

u32 mon_get_free_pmp_cfg(u8 *free_cnt)
{
	return riscv_sys_call2(SYS_CALL_GET_FREE_PMP_CFG, (u32)free_cnt);
}


