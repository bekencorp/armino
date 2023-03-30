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
    LREG  t0, 17 * REGBYTES(sp)
    csrw  ustatus, t0
    LREG  t0, 16 * REGBYTES(sp)
    csrw  uepc, t0
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

.macro PUSH_ALL_F_REG
	addi sp, sp, -(FPU_CONTEXT_SIZE * REGBYTES) /* Make room for the FPU registers. */

	#ifdef __riscv_dsp
	csrr t0, ucode
	SREG t0, 0 * REGBYTES( sp )
	#endif

	#ifdef __riscv_flen
	frcsr t0
	SREG t0, 1 * REGBYTES( sp )
	F_SREG f0, ( 2 * REGBYTES + 0 * FREGBYTES )( sp )
	F_SREG f1, ( 2 * REGBYTES + 1 * FREGBYTES )( sp )
	F_SREG f2, ( 2 * REGBYTES + 2 * FREGBYTES )( sp )
	F_SREG f3, ( 2 * REGBYTES + 3 * FREGBYTES )( sp )
	F_SREG f4, ( 2 * REGBYTES + 4 * FREGBYTES )( sp )
	F_SREG f5, ( 2 * REGBYTES + 5 * FREGBYTES )( sp )
	F_SREG f6, ( 2 * REGBYTES + 6 * FREGBYTES )( sp )
	F_SREG f7, ( 2 * REGBYTES + 7 * FREGBYTES )( sp )
	F_SREG f8, ( 2 * REGBYTES + 8 * FREGBYTES )( sp )
	F_SREG f9, ( 2 * REGBYTES + 9 * FREGBYTES )( sp )
	F_SREG f10, ( 2 * REGBYTES + 10 * FREGBYTES )( sp )
	F_SREG f11, ( 2 * REGBYTES + 11 * FREGBYTES )( sp )
	F_SREG f12, ( 2 * REGBYTES + 12 * FREGBYTES )( sp )
	F_SREG f13, ( 2 * REGBYTES + 13 * FREGBYTES )( sp )
	F_SREG f14, ( 2 * REGBYTES + 14 * FREGBYTES )( sp )
	F_SREG f15, ( 2 * REGBYTES + 15 * FREGBYTES )( sp )
	F_SREG f16, ( 2 * REGBYTES + 16 * FREGBYTES )( sp )
	F_SREG f17, ( 2 * REGBYTES + 17 * FREGBYTES )( sp )
	F_SREG f18, ( 2 * REGBYTES + 18 * FREGBYTES )( sp )
	F_SREG f19, ( 2 * REGBYTES + 19 * FREGBYTES )( sp )
	F_SREG f20, ( 2 * REGBYTES + 20 * FREGBYTES )( sp )
	F_SREG f21, ( 2 * REGBYTES + 21 * FREGBYTES )( sp )
	F_SREG f22, ( 2 * REGBYTES + 22 * FREGBYTES )( sp )
	F_SREG f23, ( 2 * REGBYTES + 23 * FREGBYTES )( sp )
	F_SREG f24, ( 2 * REGBYTES + 24 * FREGBYTES )( sp )
	F_SREG f25, ( 2 * REGBYTES + 25 * FREGBYTES )( sp )
	F_SREG f26, ( 2 * REGBYTES + 26 * FREGBYTES )( sp )
	F_SREG f27, ( 2 * REGBYTES + 27 * FREGBYTES )( sp )
	F_SREG f28, ( 2 * REGBYTES + 28 * FREGBYTES )( sp )
	F_SREG f29, ( 2 * REGBYTES + 29 * FREGBYTES )( sp )
	F_SREG f30, ( 2 * REGBYTES + 30 * FREGBYTES )( sp )
	F_SREG f31, ( 2 * REGBYTES + 31 * FREGBYTES )( sp )
	#endif
.endm

.macro POP_ALL_F_REG
	#ifdef __riscv_dsp
	LREG t0, 0 * REGBYTES( sp )
	csrw ucode, t0
	#endif

	#ifdef __riscv_flen
	LREG t0,  1 * REGBYTES( sp )
	fscsr t0
	F_LREG f0, ( 2 * REGBYTES + 0 * FREGBYTES )( sp )
	F_LREG f1, ( 2 * REGBYTES + 1 * FREGBYTES )( sp )
	F_LREG f2, ( 2 * REGBYTES + 2 * FREGBYTES )( sp )
	F_LREG f3, ( 2 * REGBYTES + 3 * FREGBYTES )( sp )
	F_LREG f4, ( 2 * REGBYTES + 4 * FREGBYTES )( sp )
	F_LREG f5, ( 2 * REGBYTES + 5 * FREGBYTES )( sp )
	F_LREG f6, ( 2 * REGBYTES + 6 * FREGBYTES )( sp )
	F_LREG f7, ( 2 * REGBYTES + 7 * FREGBYTES )( sp )
	F_LREG f8, ( 2 * REGBYTES + 8 * FREGBYTES )( sp )
	F_LREG f9, ( 2 * REGBYTES + 9 * FREGBYTES )( sp )
	F_LREG f10, ( 2 * REGBYTES + 10 * FREGBYTES )( sp )
	F_LREG f11, ( 2 * REGBYTES + 11 * FREGBYTES )( sp )
	F_LREG f12, ( 2 * REGBYTES + 12 * FREGBYTES )( sp )
	F_LREG f13, ( 2 * REGBYTES + 13 * FREGBYTES )( sp )
	F_LREG f14, ( 2 * REGBYTES + 14 * FREGBYTES )( sp )
	F_LREG f15, ( 2 * REGBYTES + 15 * FREGBYTES )( sp )
	F_LREG f16, ( 2 * REGBYTES + 16 * FREGBYTES )( sp )
	F_LREG f17, ( 2 * REGBYTES + 17 * FREGBYTES )( sp )
	F_LREG f18, ( 2 * REGBYTES + 18 * FREGBYTES )( sp )
	F_LREG f19, ( 2 * REGBYTES + 19 * FREGBYTES )( sp )
	F_LREG f20, ( 2 * REGBYTES + 20 * FREGBYTES )( sp )
	F_LREG f21, ( 2 * REGBYTES + 21 * FREGBYTES )( sp )
	F_LREG f22, ( 2 * REGBYTES + 22 * FREGBYTES )( sp )
	F_LREG f23, ( 2 * REGBYTES + 23 * FREGBYTES )( sp )
	F_LREG f24, ( 2 * REGBYTES + 24 * FREGBYTES )( sp )
	F_LREG f25, ( 2 * REGBYTES + 25 * FREGBYTES )( sp )
	F_LREG f26, ( 2 * REGBYTES + 26 * FREGBYTES )( sp )
	F_LREG f27, ( 2 * REGBYTES + 27 * FREGBYTES )( sp )
	F_LREG f28, ( 2 * REGBYTES + 28 * FREGBYTES )( sp )
	F_LREG f29, ( 2 * REGBYTES + 29 * FREGBYTES )( sp )
	F_LREG f30, ( 2 * REGBYTES + 30 * FREGBYTES )( sp )
	F_LREG f31, ( 2 * REGBYTES + 31 * FREGBYTES )( sp )
	#endif

	addi sp, sp, (FPU_CONTEXT_SIZE * REGBYTES) /* Remove space added for FPU registers. */
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

	// save the FPU registers
	PUSH_ALL_F_REG

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

	// retrieve the FPU registers
	POP_ALL_F_REG

#if 0
	// enable global interrupts
    lw      t0, 17 * REGBYTES(sp)
    csrw    ustatus, t0

	// retrieve the address at which exception happened
    lw      t0, 16 * REGBYTES(sp)
    csrw    uepc, t0
#endif

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
