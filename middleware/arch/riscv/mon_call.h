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

#define OTP_USER_ID_START	32
#define OTP_SEG_ID_MAX		0xEF
#define OTP_SEG_MAX_BYTE	1000
#define OTP_SEG_MAX_WORD	(OTP_SEG_MAX_BYTE / 4)

#define OTP_R_ON				0x01		/* read enable. */
#define OTP_W_ON				0x02		/* write enable. */
#define OTP_PERM_MASK			0x03
#define OTP_PERM_BITS			0x02 		/* 2-bits for every app. */

enum
{
	OTP_OK = 0,
	OTP_ID_ERROR,               /* ID not in the range of 0 ~ OTP_SEG_ID_MAX. */
	OTP_LEN_ERROR,              /* request size to be read/written is not the same with OTP data. */
	OTP_ALIGN_ERROR,            /* buffer address is not aligned with 4 bytes. */
	OTP_PERMISSION_ERROR,       /* no permission to read or write. */
	OTP_EXISTED_ERROR,          /* the OTP data specified by id is exited when try to create */
	OTP_NOT_EXISTED_ERROR,      /* the OTP data specified by id is not exited when try to read/write. */
	OTP_NO_SPACE_ERROR,         /* no free space when try to create new OTP data. */
	OTP_WRITE_ERROR,            /* try to write 1 to OTP bit which was in 0 state. */
};

enum
{
	ENCLV_ID_BOOTLDR = 0,
	ENCLV_ID_APP,
};

#define PMP_X_ON                    4
#define PMP_W_ON                    2
#define PMP_R_ON                    1
#define PMP_XWR_ON                  (PMP_X_ON | PMP_W_ON | PMP_R_ON)
#define PMP_XWR_MASK                (PMP_X_ON | PMP_W_ON | PMP_R_ON)

/**
 * @brief	  FreeRTOS yield porting API
 *
 * This API is used for FreRTOS task switch by issue the ecall instruction:
 *
 * @param 
 *   - void
 *
 * @attention   Used for FreeRTOS porting only.
 *
 * @return
 *  - void
 */
void mon_task_yield(void);

/**
 * @brief	  clear U-Timer interrupt.
 *
 * This API is used to clear U mode Timer interrupt.
 *
 * @param 
 *   - void
 *
 * @attention 
 *
 * @return
 *  - void
 */
void mon_timer_int_clear(void);

/**
 * @brief	  Start/Stop CPU1
 *
 * This API is used for FreRTOS task switch by issue the ecall instruction:
 *	 - Uses CPU poll wait to do delay, so it blocks CPU.
 *	 - The caller should confirm the specifies GPIO is not used by other APP.
 *
 * @param 
 * - enable:  0: stop CPU1, otherwise start CPU1.
 * @param 
 * - start_addr: start addess where CPU1 will run from when it is to start CPU1,
 *   it is not care if it is to stop CPU1.
 *
 * @attention 
 *   - this function will damage the last 2 bytes of the Shared Memory 
 *     to save the start_addr temporarily.
 *
 * @return
 *  - void
 */
void mon_reset_cpu1(u32 enable, u32 start_addr);

/**
 * @brief	  Flush all data cache
 *
 * This API will flush the WHOLE data cache if the memory specified by 'addr' is in the cacheable region.
 *
 * @param addr    the memory address intended to be flush.
 *
 * @attention   it is not efficient to call this API if just want to flush one or two cache lines.
 *              it is 32 byte per cache line in BK7256/BK7235.
 *
 * @return
 *  - void
 */
void mon_flush_dcache(u32 addr);

/**
 * @brief	  get chache line size
 *
 * @param 
 *  - void 
 *
 * @return
 *  - bytes per cache line.
 */
u32 mon_get_dcache_linesize(void);

// extern void 		mon_set_stack_limit(u32 statc_max_addr, u32 stack_min_addr, u32 sp);

/**
 * @brief	  switched from bootloader to Application.
 *
 * This API is used by bootloader to jump to Application code.
 *
 * @param *entry  entry addres of Application code.
 *
 * @attention Application can NOT call this API.
 *
 * @return
 *  - void
 */
void mon_bl_2_app(void *entry);

/**
 * @brief	  notify Monitor that U-mode code starts ok.
 *
 * This API is used by U-mode code to notify Monitor that it can handle interrupts,
 * so the interrupts will be sent to U-mode after this API returns .
 *
 * @param 
 * - void 
 *
 * @attention  U-mode code must call this API as soon as it startup (can handle interrupts by U-mode itself).
 *
 * @return
 *  - 0: succeed
 *  - this API always returns 0.
 */
u32 mon_app_started(void);

/**
 * @brief	  read OTP data specified by data id.
 *
 * This API is to read OTP data specified by the id.
 *
 * @param *buf   buffer to receive OTP data, its size MUST be >= the OPT data size.
 * @param data_id  0 ~ OTP_SEG_ID_MAX.
 * @param len    bytes request to read, it ** MUST ** be the data size already created in the OTP.
 *
 * @attention   The buf must be aligned with 4 bytes, 
 *              otherwise the error code OTP_ALIGN_ERROR will be returned.
 *
 * @return
 *  - OTP_OK: succeed
 *  - others: error code.
 */
u32 mon_read_otp(u32 *buf, u32 data_id, u32 len);

/**
 * @brief	  write OTP data specified by data id. data segment must have the 'write' attribute.
 *
 * This API is to write OTP data specified by the id.
 *
 * @param data_id  0 ~ OTP_SEG_ID_MAX.
 * @param *buf   buffer contained OTP data
 * @param len    bytes need be written, it is the size of the OTP data created before.
 *
 * @attention   The buf must be aligned with 4 bytes, 
 *              otherwise the error code OTP_ALIGN_ERROR will be returned.
 *
 * @return
 *  - OTP_OK: succeed
 *  - others: error code.
 */
u32 mon_write_otp(u32 data_id, u32 *buf, u32 len);

/**
 * @brief	 Create OTP data segment
 *
 * This API is to create an OTP data segment with the specified id.
 *
 * @param data_id  OTP_USER_ID_START ~ OTP_SEG_ID_MAX.
 * @param *buf   buffer contained OTP data
 * @param len    bytes need be written, it is the size of the OTP data to be created. ( 1 ~ OTP_SEG_MAX_BYTE inclusive)
 * @param attr   attributes of the data segment. it sets the R/W attributes for every ENCLAVE (bootloader / application)
 *               bit 0 is the Read permission for bootloader, bit 1 is the Write permission for bootloader.
 *               bit 2 is the Read permission for Application, bit 3 is the Write permission for Application.
 *               bit 4~7 are reserved, must be set to 0.
 *
 * @attention   The buf must be aligned with 4 bytes, 
 *              otherwise the error code OTP_ALIGN_ERROR will be returned.
 *              data_id must be in range from OTP_USER_ID_START to OTP_SEG_ID_MAX (inclusive) for this API.
 *
 * @return
 *  - OTP_OK: succeed
 *  - others: error code.
 */
u32 mon_create_otp(u32 data_id, u32 *buf, u32 len, u32 attr);

/**
 * @brief	  set PMP protection setting
 *
 * This API is used to set the memory protection for the specified range (start_addr & size) with the attributes.
 *
 * @param start_addr  start address of the protection range, MUST be aligned with 8 bytes..
 * @param size    size of the protection range, MUST be >= 8 bytes.
 * @param attr   execution (PMP_X_ON), write (PMP_W_ON), read (PMP_R_ON). can be any combinations of the 3 attributes.
 *
 * @attention this API may use 1~2 entries for one setting, relies on the alignment of the start address and the size of the range.
 *
 * @return
 *  - 0: succeed
 *  - 1: no free config entry for the setting.
 */
u32 mon_pmp_cfg(u32 start_addr, u32 size, u8 attr);

/**
 * @brief	  get numbers of free memory protection config entry.
 *
 * This API returns number of unused PMP config entries.
 *   U-mode code can use the free cfg entries to protect range of the addres space. 
 *	 - The caller should confirm the specifies GPIO is not used by other APP.
 *
 * @param *free_cnt  receives the free config entries.
 *
 * @return
 *  - 0: succeed, this API always return 0.
 */
u32 mon_get_free_pmp_cfg(u8 *free_cnt);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __MON_CALL_H__ */

