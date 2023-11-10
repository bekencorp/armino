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

#define SYS_CALL_ENCLV_OS_TRAP			0
#define SYS_CALL_ENCLV_MTIMER			2
#define SYS_CALL_RESET_CPU_X			3
#define SYS_CALL_FLUSH_DCACHE			4
#define SYS_CALL_GET_DCACHE_LINE		5
#define SYS_CALL_SET_STACK_LIMIT		6

#define SYS_CALL_BL_2_APP				11
#define SYS_CALL_ENCLV_STARTED			12

#define SYS_CALL_READ_OTP				20
#define SYS_CALL_WRITE_OTP				21
#define SYS_CALL_CREATE_OTP				23

#define SYS_CALL_PMP_CFG				30
#define SYS_CALL_GET_FREE_PMP_CFG		31


#define GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define SYS_CALL(...) GET_MACRO(__VA_ARGS__, \
        riscv_sys_call4, riscv_sys_call3, riscv_sys_call2, riscv_sys_call1)(__VA_ARGS__)

extern u32 riscv_sys_call1(u32 id);
extern u32 riscv_sys_call2(u32 id, u32 param1);
extern u32 riscv_sys_call3(u32 id, u32 param1, u32 param2);
extern u32 riscv_sys_call4(u32 id, u32 param1, u32 param2, u32 param3);

void mon_task_yield(void)
{
	SYS_CALL(SYS_CALL_ENCLV_OS_TRAP);
}

void mon_timer_int_clear(void)
{
	SYS_CALL(SYS_CALL_ENCLV_MTIMER);
}

void mon_reset_cpu1(u32 enable, u32 start_addr)
{
	SYS_CALL(SYS_CALL_RESET_CPU_X, enable, start_addr);
}

void mon_flush_dcache(u32 addr)
{
	SYS_CALL(SYS_CALL_FLUSH_DCACHE, addr);
}

u32 mon_get_dcache_linesize(void)
{
	return SYS_CALL(SYS_CALL_GET_DCACHE_LINE);
}

#if 0
void mon_set_stack_limit(u32 statc_max_addr, u32 statc_min_addr, u32 sp)
{
	SYS_CALL(SYS_CALL_SET_STACK_LIMIT, statc_max_addr, statc_min_addr, sp);
}
#endif

void mon_bl_2_app(void *entry)
{
	if(SYS_CALL(SYS_CALL_BL_2_APP, (u32)entry) != 0)
	{
		while(1);
	}
}

u32 mon_app_started(void)
{
	return SYS_CALL(SYS_CALL_ENCLV_STARTED);
}

u32 mon_read_otp(u32 *buf, u32 data_id, u32 len)
{
	return SYS_CALL(SYS_CALL_READ_OTP, (u32)buf, data_id, len);
}

u32 mon_write_otp(u32 data_id, u32 *buf, u32 len)
{
	return SYS_CALL(SYS_CALL_WRITE_OTP, data_id, (u32)buf, len);
}

u32 mon_create_otp(u32 data_id, u32 *buf, u32 len, u32 attr)
{
	return SYS_CALL(SYS_CALL_CREATE_OTP, data_id, (u32)buf, (len & 0xFFFF) + (attr << 16));
}

u32 mon_pmp_cfg(u32 start_addr, u32 size, u8 attr)
{
	return SYS_CALL(SYS_CALL_PMP_CFG, start_addr, size, attr);
}

u32 mon_get_free_pmp_cfg(u8 *free_cnt)
{
	return SYS_CALL(SYS_CALL_GET_FREE_PMP_CFG, (u32)free_cnt);
}


