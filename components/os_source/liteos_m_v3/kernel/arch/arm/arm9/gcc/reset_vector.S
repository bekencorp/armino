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

    .equ CPSR_IRQ_DISABLE,  0x80
    .equ CPSR_FIQ_DISABLE,  0x40
    .equ CPSR_THUMB_ENABLE, 0x20
    .equ CPSR_USER_MODE,    0x10
    .equ CPSR_FIQ_MODE,     0x11
    .equ CPSR_IRQ_MODE,     0x12
    .equ CPSR_SVC_MODE,     0x13
    .equ CPSR_ABT_MODE,     0x17
    .equ CPSR_UNDEF_MODE,   0x1B

    .global __exc_stack_top
    .global __irq_stack_top
    .global __fiq_stack_top
    .global __svc_stack_top
    .global __abt_stack_top
    .global __undef_stack_top
    .global __exc_stack
    .global __irq_stack
    .global __fiq_stack
    .global __svc_stack
    .global __abt_stack
    .global __undef_stack
    .global main

    .extern HalExceptFiqHdl
    .extern HalExceptAddrAbortHdl
    .extern HalExceptDataAbortHdl
    .extern HalExceptPrefetchAbortHdl
    .extern HalExceptSwiHdl
    .extern HalExceptUndefInstrHdl
    .extern HalExceptIrqHdl
    .extern _bss_start
    .extern _bss_end

    .code 32
    .text

    .section ".vectors", "ax"
    .global _vector_start

_vector_start:
    B     HalResetVector
    B     HalExceptUndefInstrHdl
    B     HalExceptSwiHdl
    B     HalExceptPrefetchAbortHdl
    B     HalExceptDataAbortHdl
    B     HalExceptAddrAbortHdl
    B     HalExceptIrqHdl
    B     HalExceptFiqHdl

    .globl  HalResetVector
    .section ".boot", "ax"

HalResetVector:
    MOV    R0, #(CPSR_IRQ_DISABLE | CPSR_FIQ_DISABLE | CPSR_IRQ_MODE)
    MSR    CPSR, R0
    LDR    SP, =__irq_stack_top

    MOV    R0, #(CPSR_IRQ_DISABLE | CPSR_FIQ_DISABLE | CPSR_UNDEF_MODE)
    MSR    CPSR, R0
    LDR    SP, =__undef_stack_top

    MOV    R0, #(CPSR_IRQ_DISABLE | CPSR_FIQ_DISABLE | CPSR_ABT_MODE)
    MSR    CPSR, R0
    LDR    SP, =__abt_stack_top

    MOV    R0, #(CPSR_IRQ_DISABLE | CPSR_FIQ_DISABLE | CPSR_FIQ_MODE)
    MSR    CPSR, R0
    LDR    SP, =__fiq_stack_top

    MOV    R0, #(CPSR_IRQ_DISABLE | CPSR_FIQ_DISABLE | CPSR_SVC_MODE)
    MSR    CPSR, R0
    MSR    SPSR, R0
    LDR    SP, =__svc_stack_top

    BL     OsBssInit

    B      main
    B      .

OsBssInit:
    LDR     R0, =_bss_start
    LDR     R1, =_bss_end

    MOV     R3, R1
    MOV     R4, R0
    MOV     R2, #0
1:  CMP     R4, R3
    STRLO   R2, [R4], #4
    BLO     1b
    BX      LR

    .section ".bss", "wa", %nobits
    .align 3
__undef_stack:
    .space  32
__undef_stack_top:

__abt_stack:
    .space  32
__abt_stack_top:

__irq_stack:
    .space  1024
__irq_stack_top:

__fiq_stack:
    .space  1024
__fiq_stack_top:

__svc_stack:
    .space  1024
__svc_stack_top:

__exc_stack:
    .space  512
__exc_stack_top:

