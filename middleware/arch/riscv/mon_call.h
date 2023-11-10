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

#ifndef  __MON_CALL_H__
#define  __MON_CALL_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <common/bk_typedef.h>

#define SYS_CALL_ENCLV_OS_TRAP			0
#define SYS_CALL_ENCLV_MTIMER			2
#define SYS_CALL_RESET_CPU_X			3
#define SYS_CALL_FLUSH_DCACHE			4
#define SYS_CALL_GET_DCACHE_LINE		5
#define SYS_CALL_SET_STACK_LIMIT		6

#define SYS_CALL_ENCLV_STARTED			12

#define SYS_CALL_READ_OTP				20
#define SYS_CALL_WRITE_OTP				21
// #define SYS_CALL_ENABLE_OTP				22
#define SYS_CALL_CREATE_OTP				23

#define SYS_CALL_PMP_CFG				30
#define SYS_CALL_GET_FREE_PMP_CFG		31

extern void			mon_task_yield(void);
extern void			mon_timer_int_clear(void);
extern void 		mon_reset_cpu1(u32 enable, u32 start_addr);
extern void 		mon_flush_dcache(u32 addr);
extern u32 			mon_get_dcache_linesize(void);
// extern void 		mon_set_stack_limit(u32 statc_max_addr, u32 stack_min_addr, u32 sp);

extern u32 			mon_app_started(void);
extern u32 			mon_read_otp(u32 *buf, u32 data_id, u32 len);
extern u32 			mon_write_otp(u32 data_id, u32 *buf, u32 len);
extern u32 			mon_create_otp(u32 data_id, u32 *buf, u32 len, u32 attr);
// extern u32 			mon_enable_otp(u32 enabled);
extern u32 			mon_pmp_cfg(u32 start_addr, u32 size, u8 attr);
extern u32 			mon_get_free_pmp_cfg(u8 *free_cnt);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __MON_CALL_H__ */

