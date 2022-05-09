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

    .equ OS_PSR_INT_DIS,    0xC0
    .equ OS_PSR_FIQ_DIS,    0x40
    .equ OS_PSR_IRQ_DIS,    0x80
    .equ OS_PSR_MODE_MASK,  0x1F
    .equ OS_PSR_MODE_USR,   0x10
    .equ OS_PSR_MODE_FIQ,   0x11
    .equ OS_PSR_MODE_IRQ,   0x12
    .equ OS_PSR_MODE_SVC,   0x13
    .equ OS_PSR_MODE_ABT,   0x17
    .equ OS_PSR_MODE_UND,   0x1B
    .equ OS_PSR_MODE_SYS,   0x1F

    .equ OS_EXCEPT_RESET,           0x00
    .equ OS_EXCEPT_UNDEF_INSTR,     0x01
    .equ OS_EXCEPT_SWI,             0x02
    .equ OS_EXCEPT_PREFETCH_ABORT,  0x03
    .equ OS_EXCEPT_DATA_ABORT,      0x04
    .equ OS_EXCEPT_FIQ,             0x05
    .equ OS_EXCEPT_ADDR_ABORT,      0x06
    .equ OS_EXCEPT_IRQ,             0x07

    .global HalExceptAddrAbortHdl
    .global HalExceptDataAbortHdl
    .global HalExceptPrefetchAbortHdl
    .global HalExceptUndefInstrHdl

    .extern HalExcHandleEntry
    .extern __exc_stack_top

    .code 32
    .text

HalExceptUndefInstrHdl:
    STMFD   SP!, {R0-R5}
    MOV     R0, #OS_EXCEPT_UNDEF_INSTR
    B       _osExceptDispatch

HalExceptPrefetchAbortHdl:
    SUB     LR, LR, #4
    STMFD   SP!, {R0-R5}

    MOV     R0, #OS_EXCEPT_PREFETCH_ABORT

    B       _osExceptDispatch

HalExceptDataAbortHdl:
    SUB     LR, LR, #4
    STMFD   SP!, {R0-R5}

    MOV     R0, #OS_EXCEPT_DATA_ABORT

    B       _osExceptDispatch

HalExceptAddrAbortHdl:
    SUB     LR, LR, #8
    STMFD   SP!, {R0-R5}

    MOV     R0, #OS_EXCEPT_ADDR_ABORT

    B       _osExceptDispatch

_osExceptDispatch:
    MRS     R1, SPSR
    MOV     R2, LR
    MOV     R4, SP
    ADD     SP, SP, #(6 * 4)

    MSR     CPSR_c, #(OS_PSR_INT_DIS | OS_PSR_MODE_SVC)
    MOV     R3, SP
    LDR     SP, =__exc_stack_top

    STMFD   SP!, {R2}
    STMFD   SP!, {LR}
    STMFD   SP!, {R3}
    STMFD   SP!, {R6-R12}
    LDMFD   R4!, {R6-R12}
    STMFD   SP!, {R6-R11}
    STMFD   SP!, {R1}
    MOV     R3, SP

_osExceptionGetSP:
    STMFD   SP!, {R1}
    LDR     R2, =HalExcHandleEntry

    MOV     LR, PC
    BX      R2

    LDMFD   SP!, {R1}
    MOV     SP, R1

    LDMFD   SP!, {R1}
    MSR     CPSR, R1
    LDMFD   SP!, {R0-R12}
    ADD     SP, SP, #(4 * 2)
    LDMFD   SP!, {LR}
    SUB     SP, SP, #(4 * 3)
    LDMFD   SP, {SP}
    ADD     LR, LR, #4
    MOV     PC, LR

    .end
