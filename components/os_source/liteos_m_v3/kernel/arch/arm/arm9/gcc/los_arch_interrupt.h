/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LOS_ARCH_INTERRUPT_H
#define _LOS_ARCH_INTERRUPT_H

#include "los_config.h"
#include "los_compiler.h"
#include "los_interrupt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* *
 * @ingroup los_arch_interrupt
 * Maximum number of used hardware interrupts.
 */
#ifndef OS_HWI_MAX_NUM
#define OS_HWI_MAX_NUM                        LOSCFG_PLATFORM_HWI_LIMIT
#endif

/* *
 * @ingroup los_arch_interrupt
 * Highest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_HIGHEST
#define OS_HWI_PRIO_HIGHEST                   0
#endif

/* *
 * @ingroup los_arch_interrupt
 * Lowest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_LOWEST
#define OS_HWI_PRIO_LOWEST                    7
#endif


/* *
 * @ingroup  los_arch_interrupt
 * Define the type of a hardware interrupt vector table function.
 */
typedef VOID (**HWI_VECTOR_FUNC)(void);

/* *
 * @ingroup los_arch_interrupt
 * Count of interrupts.
 */
extern UINT32 g_intCount;

/* *
 * @ingroup los_arch_interrupt
 * Count of arm9 system interrupt vector.
 */
#define OS_SYS_VECTOR_CNT                     0

/* *
 * @ingroup los_arch_interrupt
 * Count of arm9 interrupt vector.
 */
#define OS_VECTOR_CNT                         (OS_SYS_VECTOR_CNT + OS_HWI_MAX_NUM)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: Invalid interrupt number.
 *
 * Value: 0x02000900
 *
 * Solution: Ensure that the interrupt number is valid. The value range of the interrupt number applicable for a Cortex-A7 platform is [OS_USER_HWI_MIN,OS_USER_HWI_MAX].
 */
#define OS_ERRNO_HWI_NUM_INVALID              LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x00)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: Null hardware interrupt handling function.
 *
 * Value: 0x02000901
 *
 * Solution: Pass in a valid non-null hardware interrupt handling function.
 */
#define OS_ERRNO_HWI_PROC_FUNC_NULL           LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x01)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: Insufficient interrupt resources for hardware interrupt creation.
 *
 * Value: 0x02000902
 *
 * Solution: Increase the configured maximum number of supported hardware interrupts.
 */
#define OS_ERRNO_HWI_CB_UNAVAILABLE           LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x02)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: Insufficient memory for hardware interrupt initialization.
 *
 * Value: 0x02000903
 *
 * Solution: Expand the configured memory.
 */
#define OS_ERRNO_HWI_NO_MEMORY                LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x03)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: The interrupt has already been created.
 *
 * Value: 0x02000904
 *
 * Solution: Check whether the interrupt specified by the passed-in interrupt number has already been created.
 */
#define OS_ERRNO_HWI_ALREADY_CREATED          LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x04)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: Invalid interrupt priority.
 *
 * Value: 0x02000905
 *
 * Solution: Ensure that the interrupt priority is valid. The value range of the interrupt priority applicable for a Cortex-A7 platform is [0,15].
 */
#define OS_ERRNO_HWI_PRIO_INVALID             LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x05)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: Incorrect interrupt creation mode.
 *
 * Value: 0x02000906
 *
 * Solution: The interrupt creation mode can be only set to OS_HWI_MODE_COMM or OS_HWI_MODE_FAST of which the value can be 0 or 1.
 */
#define OS_ERRNO_HWI_MODE_INVALID             LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x06)

/* *
 * @ingroup los_arch_interrupt
 * Hardware interrupt error code: The interrupt has already been created as a fast interrupt.
 *
 * Value: 0x02000907
 *
 * Solution: Check whether the interrupt specified by the passed-in interrupt number has already been created.
 */
#define OS_ERRNO_HWI_FASTMODE_ALREADY_CREATED LOS_ERRNO_OS_ERROR(LOS_MOD_HWI, 0x07)

#if (OS_HWI_WITH_ARG == 1)
/* *
 * @ingroup los_hwi
 * Set interrupt vector table.
 */
extern VOID OsSetVector(UINT32 num, HWI_PROC_FUNC vector, VOID *arg);
#else
/* *
 * @ingroup los_hwi
 * Set interrupt vector table.
 */
extern VOID OsSetVector(UINT32 num, HWI_PROC_FUNC vector);
#endif

/* *
 * @ingroup  los_arch_interrupt
 * @brief: Hardware interrupt entry function.
 *
 * @par Description:
 * This API is used as all hardware interrupt handling function entry.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param:None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_arch_interrupt.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
extern VOID HalInterrupt(VOID);

/* *
 * @ingroup  los_arch_interrupt
 * @brief: Get an interrupt number.
 *
 * @par Description:
 * This API is used to get the current interrupt number.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param: None.
 *
 * @retval: Interrupt Indexes number.
 * @par Dependency:
 * <ul><li>los_arch_interrupt.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
extern UINT32 HalIntNumGet(VOID);

/* *
 * @ingroup  los_arch_interrupt
 * @brief: Default vector handling function.
 *
 * @par Description:
 * This API is used to configure interrupt for null function.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param:None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_arch_interrupt.h: the header file that contains the API declaration.</li
></ul>
 * @see None.
 */
extern VOID HalHwiDefaultHandler(VOID);

#define OS_EXC_IN_INIT                      0
#define OS_EXC_IN_TASK                      1
#define OS_EXC_IN_HWI                       2

#define OS_EXC_FLAG_FAULTADDR_VALID         0x01
#define OS_EXC_FLAG_IN_HWI                  0x02

#define OS_EXC_IMPRECISE_ACCESS_ADDR        0xABABABAB

/**
 * @ingroup los_exc
 * the struct of register files
 *
 * description: the register files that saved when exception triggered
 *
 * notes:the following register with prefix 'uw'  correspond to the registers in the cpu  data sheet.
 */
typedef struct TagExcContext {
    UINT32 spsr;
    UINT32 r0;
    UINT32 r1;
    UINT32 r2;
    UINT32 r3;
    UINT32 r4;
    UINT32 r5;
    UINT32 r6;
    UINT32 r7;
    UINT32 r8;
    UINT32 r9;
    UINT32 r10;
    UINT32 r11;
    UINT32 r12;
    UINT32 sp;
    UINT32 lr;
    UINT32 pc;
} EXC_CONTEXT_S;

typedef VOID (*EXC_PROC_FUNC)(UINT32, EXC_CONTEXT_S *);
VOID HalExcHandleEntry(UINT32 excType, UINT32 faultAddr, UINT32 pid, EXC_CONTEXT_S *excBufAddr);
VOID HalHwiInit();

/**
 * @ingroup los_exc
 * exception types: undefined instruction exception.
 */
#define OS_EXCEPT_UNDEF_INSTR           1

/**
 * @ingroup los_exc
 * exception types: software interrupt.
 */
#define OS_EXCEPT_SWI                   2

/**
 * @ingroup los_exc
 * exception types: prefetch abort exception.
 */
#define OS_EXCEPT_PREFETCH_ABORT        3

/**
 * @ingroup los_exc
 * exception types: data abort exception.
 */
#define OS_EXCEPT_DATA_ABORT            4

/**
 * @ingroup los_exc
 * exception types: FIQ exception.
 */
#define OS_EXCEPT_FIQ                   5

/**
 * @ingroup los_exc
 * Exception information structure
 *
 * Description: Exception information saved when an exception is triggered on the Cortex-M4 platform.
 *
 */
typedef struct TagExcInfo {
    /**< Exception occurrence phase: 0 means that an exception occurs in initialization, 1 means that an exception occurs in a task, and 2 means that an exception occurs in an interrupt */
    UINT16 phase;
    /**< Exception type. When exceptions occur, check the numbers 1 - 19 listed above */
    UINT16 type;
    /**< If the exact address access error indicates the wrong access address when the exception occurred */
    UINT32 faultAddr;
    /**< An exception occurs in an interrupt, indicating the interrupt number. An exception occurs in the task, indicating the task ID, or 0xFFFFFFFF if it occurs during initialization */
    UINT32 thrdPid;
    /**< Number of nested exceptions. Currently only registered hook functions are supported when an exception is entered for the first time */
    UINT16 nestCnt;
    /**< reserve */
    UINT16 reserved;
    /**< Hardware context at the time an exception to the automatic stack floating-point register occurs */
    EXC_CONTEXT_S *context;
} ExcInfo;

extern UINT32 g_intCount;
extern ExcInfo g_excInfo;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ARCH_INTERRUPT_H */
