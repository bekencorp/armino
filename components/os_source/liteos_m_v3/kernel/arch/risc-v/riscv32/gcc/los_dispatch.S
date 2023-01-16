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

#include "soc_common.h"

.global  HalIntLock
.global  HalIntRestore
.global  HalStartToRun
.global  HalTaskContextSwitch
.extern  __irq_stack_top
.section .interrupt.text

.macro PUSH_ALL_REG
    addi sp, sp, -(30 * REGBYTES)
    SREG  ra, 0 * REGBYTES(sp)
    SREG  t0, 1 * REGBYTES(sp)
    SREG  t1, 2 * REGBYTES(sp)
    SREG  t2, 3 * REGBYTES(sp)
    SREG  a0, 4 * REGBYTES(sp)
    SREG  a1, 5 * REGBYTES(sp)
    SREG  a2, 6 * REGBYTES(sp)
    SREG  a3, 7 * REGBYTES(sp)
    SREG  a4, 8 * REGBYTES(sp)
    SREG  a5, 9 * REGBYTES(sp)
    SREG  a6, 10 * REGBYTES(sp)
    SREG  a7, 11 * REGBYTES(sp)
    SREG  t3, 12 * REGBYTES(sp)
    SREG  t4, 13 * REGBYTES(sp)
    SREG  t5, 14 * REGBYTES(sp)
    SREG  t6, 15 * REGBYTES(sp)

    SREG  s0, 18 * REGBYTES(sp)
    SREG  s1, 19 * REGBYTES(sp)
    SREG  s2, 20 * REGBYTES(sp)
    SREG  s3, 21 * REGBYTES(sp)
    SREG  s4, 22 * REGBYTES(sp)
    SREG  s5, 23 * REGBYTES(sp)
    SREG  s6, 24 * REGBYTES(sp)
    SREG  s7, 25 * REGBYTES(sp)
    SREG  s8, 26 * REGBYTES(sp)
    SREG  s9, 27 * REGBYTES(sp)
    SREG  s10, 28 * REGBYTES(sp)
    SREG  s11, 29 * REGBYTES(sp)
.endm

.macro POP_ALL_REG
    LREG  ra, 0 * REGBYTES(sp)
    LREG  t0, 1 * REGBYTES(sp)
    LREG  t1, 2 * REGBYTES(sp)
    LREG  t2, 3 * REGBYTES(sp)
    LREG  a0, 4 * REGBYTES(sp)
    LREG  a1, 5 * REGBYTES(sp)
    LREG  a2, 6 * REGBYTES(sp)
    LREG  a3, 7 * REGBYTES(sp)
    LREG  a4, 8 * REGBYTES(sp)
    LREG  a5, 9 * REGBYTES(sp)
    LREG  a6, 10 * REGBYTES(sp)
    LREG  a7, 11 * REGBYTES(sp)
    LREG  t3, 12 * REGBYTES(sp)
    LREG  t4, 13 * REGBYTES(sp)
    LREG  t5, 14 * REGBYTES(sp)
    LREG  t6, 15 * REGBYTES(sp)

    LREG  s0, 18 * REGBYTES(sp)
    LREG  s1, 19 * REGBYTES(sp)
    LREG  s2, 20 * REGBYTES(sp)
    LREG  s3, 21 * REGBYTES(sp)
    LREG  s4, 22 * REGBYTES(sp)
    LREG  s5, 23 * REGBYTES(sp)
    LREG  s6, 24 * REGBYTES(sp)
    LREG  s7, 25 * REGBYTES(sp)
    LREG  s8, 26 * REGBYTES(sp)
    LREG  s9, 27 * REGBYTES(sp)
    LREG  s10, 28 * REGBYTES(sp)
    LREG  s11, 29 * REGBYTES(sp)
    addi  sp, sp, 30 * REGBYTES
.endm

HalTaskContextSwitch:
    PUSH_ALL_REG

    // get UIE
    andi a1, a0, RISCV_MSTATUS_UIE

    // clear UPIE, UIE
    andi   a2, a0, RISCV_MSTATUS_UPIE | RISCV_MSTATUS_UIE
    xor  a0, a0, a2

    slli a1, a1, 0x4
    or   a0, a0, a1     // UPIE <- UIE.

    SREG a0, 17 * REGBYTES(sp)
    SREG ra, 16 * REGBYTES(sp)

    la   a1, g_losTask
    lw   a0, 0(a1)
    sw   sp, TASK_CB_KERNEL_SP(a0)

    lw   a0, 4(a1)
    sw   a0, 0(a1)

HalStartToRun:
    la   a1, g_losTask
    lw   a0, 4(a1)

// retireve stack pointer
    lw      sp, TASK_CB_KERNEL_SP(a0)

// enable global interrupts
    lw      t0, 17 * REGBYTES(sp)
    csrw    ustatus, t0

// retrieve the address at which exception happened
    lw      t0, 16 * REGBYTES(sp)
    csrw    uepc, t0

// retrieve the registers
    POP_ALL_REG

    uret

.section .text
HalIntLock:
    csrrci    a0, ustatus, RISCV_MSTATUS_UIE           // return value
    ret

HalIntRestore:
    csrw ustatus, a0
    ret
