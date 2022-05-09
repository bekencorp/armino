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

    .equ    OS_PSR_THUMB,       0x20
    .equ    OS_PSR_INT_DIS,     0xC0
    .equ    OS_PSR_FIQ_DIS,     0x40
    .equ    OS_PSR_IRQ_DIS,     0x80
    .equ    OS_PSR_MODE_MASK,   0x1F
    .equ    OS_PSR_MODE_USR,    0x10
    .equ    OS_PSR_MODE_FIQ,    0x11
    .equ    OS_PSR_MODE_IRQ,    0x12
    .equ    OS_PSR_MODE_SVC,    0x13
    .equ    OS_PSR_MODE_ABT,    0x17
    .equ    OS_PSR_MODE_UND,    0x1B
    .equ    OS_PSR_MODE_SYS,    0x1F

    .global HalStartToRun
    .global OsTaskEntryArm
    .global OsTaskEntryThumb
    .global HalExceptSwiHdl
    .global HalExceptFiqHdl
    .global HalExceptIrqHdl

    .extern OsTaskEntry
    .extern OsSchedTaskSwitch
    .extern HalInterrupt
    .extern g_losTask

    .code 32
    .text

.macro SAVE_CONTEXT
    STMFD   SP!, {R0}

    MRS     R0, SPSR
    AND     R0, R0, #OS_PSR_MODE_SYS
    CMP     R0, #OS_PSR_MODE_SYS
    BNE     1f

    STMFD   SP!, {SP}^
    LDMFD   SP!, {R0}

    STMFD   R0!, {LR}
    MOV     LR, R0
    LDMFD   SP!, {R0}

    STMFD   LR, {R0-R14}^

    SUB     LR, LR, #60
    MRS     R0, SPSR
    STMFD   LR!, {R0}

    LDR     R1, =g_losTask
    LDR     R1, [R1]
    STR     LR, [R1]
    B       2f

1:
    LDMFD   SP!, {R0}
    STMFD   SP!, {R0-R12, LR}
    MRS     R0, SPSR
    STMFD   SP!, {R0}

2:
.endm

.macro RETSORE_CONTEXT
    MRS     R0, SPSR
    AND     R0, R0, #OS_PSR_MODE_SYS
    CMP     R0, #OS_PSR_MODE_SYS
    BNE     3f

    LDR     R1, =g_losTask
    LDR     R1, [R1]
    LDR     LR, [R1]

    LDMFD   LR!, {R0}
    MSR     SPSR_cxsf, R0
    LDMFD   LR, {R0-R14}^
    ADD     LR, LR, #60
    LDMFD   LR!, {PC}^
3:
    LDMFD   SP!, {R0}
    MSR     SPSR_cxsf, R0
    LDMFD   SP!, {R0-R12, LR}
    MOVS    PC, LR
.endm

.macro TASK_SWITCH
    MRS     R0, CPSR
    ORR     R0, R0, #OS_PSR_INT_DIS
    MSR     CPSR, R0
    BLX     OsSchedTaskSwitch
    CMP     R0, #0
    BEQ     4f

    LDR     R0, =g_losTask
    LDR     R1, [R0, #4]
    STR     R1, [R0]

4:
.endm

HalStartToRun:
    LDR     R1, =g_losTask
    LDR     R0, [R1, #4]
    LDR     LR, [R0]

    LDMFD   LR!, {R0}
    MSR     SPSR_cxsf, R0

    LDMFD   LR, {R0-R14}^
    ADD     LR, LR, #60
    LDMFD   LR!, {PC}^

HalExceptSwiHdl:
    SAVE_CONTEXT

    TASK_SWITCH

    RETSORE_CONTEXT

HalExceptFiqHdl:
    SUB     LR, LR, #4
    SAVE_CONTEXT

    BLX     HalInterrupt

    RETSORE_CONTEXT

HalExceptIrqHdl:
    SUB     LR, LR, #4

    SAVE_CONTEXT

    BLX     HalInterrupt

    RETSORE_CONTEXT

OsTaskEntryArm:
    STMFD   SP!, {LR}
    BL      OsTaskEntry
    LDMFD   SP!, {LR}
    BX      LR

    .code 16
    .text
OsTaskEntryThumb:
    PUSH    {LR}
    BL      OsTaskEntry
    POP     {R0}
    MOV     LR, R0
    BX      LR

