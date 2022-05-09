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

#ifndef _LOS_ARCH_CONTEXT_H
#define _LOS_ARCH_CONTEXT_H

#include "los_config.h"
#include "los_compiler.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PSR_T_ARM      0x00000000U
#define PSR_T_THUMB    0x00000020U
#define PSR_MODE_SVC   0x00000013U
#define PSR_MODE_SYS   0x0000001FU

#define PSR_MODE_SVC_THUMB  (PSR_MODE_SVC | PSR_T_THUMB)
#define PSR_MODE_SVC_ARM    (PSR_MODE_SVC | PSR_T_ARM)

#define PSR_MODE_SYS_THUMB  (PSR_MODE_SYS | PSR_T_THUMB)
#define PSR_MODE_SYS_ARM    (PSR_MODE_SYS | PSR_T_ARM)

VOID OsTaskEntryArm(VOID);
VOID OsTaskEntryThumb(VOID);

typedef struct TagTskContext {
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
} TaskContext;

/**
 * @ingroup  los_config
 * @brief: Task start running function.
 *
 * @par Description:
 * This API is used to start a task.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param: None.
 *
 * @retval None.
 *
 * @par Dependency:
 * <ul><li>los_config.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
extern VOID HalStartToRun(VOID);

/**
 * @ingroup los_arch_context
 * @brief Wait for interrupt.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to suspend execution until interrupt or a debug request occurs.</li>
 * </ul>
 * @attention None.
 *
 * @param None.
 *
 * @retval: None.
 *
 * @par Dependency:
 * los_arch_context.h: the header file that contains the API declaration.
 * @see None.
 */
extern VOID wfi(VOID);

/**
 * @ingroup los_arch_context
 * @brief: mem fence function.
 *
 * @par Description:
 * This API is used to fence for memory.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param: None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_arch_context.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
extern VOID dmb(VOID);

/**
 * @ingroup los_arch_context
 * @brief: mem fence function.
 *
 * @par Description:
 * This API is same as dmb, it just for adaptation.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param: None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_arch_context.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
extern VOID dsb(VOID);

/**
 * @ingroup los_arch_context
 * @brief: instruction fence function.
 *
 * @par Description:
 * This API is used to fence for instruction.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param: None.
 *
 * @retval:None.
 * @par Dependency:
 * <ul><li>los_arch_context.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 */
extern VOID isb(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ARCH_CONTEXT_H */
