/*
 * Copyright (c) 2013-2020, Huawei Technologies Co., Ltd. All rights reserved.
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

#include "los_compiler.h"
#include "los_context.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_arch_context
 * The initialization value of stack space.
 */
#define EMPTY_STACK                 0xCACA

#define TP_INIT_VALUE  0x02020202L
#define SP_INIT_VALUE  0x03030303L
#define S11_INIT_VALUE 0x04040404L
#define S10_INIT_VALUE 0x05050505L
#define S9_INIT_VALUE  0x06060606L
#define S8_INIT_VALUE  0x07070707L
#define S7_INIT_VALUE  0x08080808L
#define S6_INIT_VALUE  0x09090909L
#define S5_INIT_VALUE  0x10101010L
#define S4_INIT_VALUE  0x11111111L
#define S3_INIT_VALUE  0x12121212L
#define S2_INIT_VALUE  0x13131313L
#define S1_INIT_VALUE  0x14141414L
#define FP_INIT_VALUE  0x15151515L
#define T6_INIT_VALUE  0x16161616L
#define T5_INIT_VALUE  0x17171717L
#define T4_INIT_VALUE  0x18181818L
#define T3_INIT_VALUE  0x19191919L
#define A7_INIT_VALUE  0x20202020L
#define A6_INIT_VALUE  0x21212121L
#define A5_INIT_VALUE  0x22222222L
#define A4_INIT_VALUE  0x23232323L
#define A3_INIT_VALUE  0x24242424L
#define A2_INIT_VALUE  0x25252525L
#define A1_INIT_VALUE  0x26262626L
#define T2_INIT_VALUE  0x28282828L
#define T1_INIT_VALUE  0x29292929L
#define T0_INIT_VALUE  0x30303030L

/**
 * @ingroup los_arch_context
 * Define the type of a task context control block.
 */
typedef struct {
    UINT32 sp;
    UINT32 tp;
    UINT32 t6;
    UINT32 t5;
    UINT32 t4;
    UINT32 t3;
    UINT32 t2;
    UINT32 t1;
    UINT32 t0;
    UINT32 s11;
    UINT32 s10;
    UINT32 s9;
    UINT32 s8;
    UINT32 s7;
    UINT32 s6;
    UINT32 s5;
    UINT32 mstatus;
    UINT32 mepc;
    UINT32 a7;
    UINT32 a6;
    UINT32 a5;
    UINT32 a4;
    UINT32 a3;
    UINT32 a2;
    UINT32 a1;
    UINT32 a0;
    UINT32 s4;
    UINT32 s3;
    UINT32 s2;
    UINT32 s1;
    UINT32 s0;
    UINT32 ra;
} TaskContext;

STATIC INLINE UINTPTR GetSP(VOID)
{
    UINTPTR spSave;
    __asm__ __volatile__("mv %0, sp" : "=r"(spSave));
    return spSave;
}

STATIC INLINE UINTPTR GetFp(VOID)
{
    UINTPTR fpSave = 0;
    __asm__ __volatile__("mv %0, s0" : "=r"(fpSave));
    return fpSave;
}

extern VOID HalStartToRun(VOID);

extern VOID HalTaskContextSwitch(UINT32 intSave);

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
extern VOID mb(VOID);

/**
 * @ingroup los_arch_context
 * @brief: mem fence function.
 *
 * @par Description:
 * This API is same as mb, it just for adaptation.
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

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ARCH_CONTEXT_H */
