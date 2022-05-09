// Copyright 2020-2021 Beken
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

#include "boot.h"
#include "arm.h"
#include "boot_asm_macro.h"

.globl boot_swi
.globl irq_handler
.globl fiq_handler
.globl do_irq
.globl do_fiq
.globl do_swi
.global boot_exception_swi

.global rt_interrupt_enter
.global rt_interrupt_leave
.global rt_thread_switch_interrupt_flag
.global rt_interrupt_from_thread
.global rt_interrupt_to_thread

.align  5
boot_swi:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400008
	LDR    r0, [R1]
	BX     r0

.align  5
do_swi:
	LDMFD   SP!, {R0-R1}
	PUSH_SVC_REG
	BL boot_exception_swi
	B       .

.align	5
irq_handler:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400000
	LDR    r0, [R1]
	BX     r0

.align	5
fiq_handler:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400004
	LDR    r0, [R1]
	BX     r0

.align  5
do_irq:
	LDMFD   SP!, {R0-R1}
	STMFD   SP!, {R0-R12,LR}

	mrs r4, cpsr
	orr r1, r4, #0xC0          @; disable interrupt
	msr cpsr_c, r1

	BL      rt_interrupt_enter
	BL      rt_irq_dispatch
	BL      rt_interrupt_leave

	LDR     R0, =rt_thread_switch_interrupt_flag
	LDR     R1, [R0]
	CMP     R1, #1
	BEQ     rt_hw_context_switch_interrupt_do

	LDMFD   SP!, {R0-R12,LR}
	SUBS    PC,  LR, #4

rt_hw_context_switch_interrupt_do:
	MOV     R1,  #0
	STR     R1,  [R0]

	MOV     R1, SP
	ADD     SP, SP, #4*4
	LDMFD   SP!, {R4-R12,LR}

	MRS     R0,  SPSR
	SUB     R2,  LR, #4

	MSR     CPSR_c, #ARM_FIQ_IRQ_MASK|BOOT_ARM_MODE_SVC

	STMFD   SP!, {R2}
	STMFD   SP!, {R4-R12,LR}
	LDMFD   R1,  {R1-R4}
	STMFD   SP!, {R1-R4}
	STMFD   SP!, {R0}

	LDR     R4,  =rt_interrupt_from_thread
	LDR     R5,  [R4]
	STR     SP,  [R5]

	LDR     R6,  =rt_interrupt_to_thread
	LDR     R6,  [R6]
	LDR     SP,  [R6]

	LDMFD   SP!, {R4}
	MSR     SPSR_cxsf, R4

	LDMFD   SP!, {R0-R12,LR,PC}^

.align  5
do_fiq:
	LDMFD   SP!, {R0-R1}
	STMFD   SP!,{R0-R7,LR}

	BL      rt_interrupt_enter
	BL      rt_fiq_dispatch
	BL      rt_interrupt_leave

	MRS R3, spsr
	AND R2, R3, #0x1F
	CMP R2, #0x12            @; fiq from irq(0x12)
	BEQ fiq_handler_return

	LDR     R0, =rt_thread_switch_interrupt_flag
	LDR     R1, [R0]
	CMP     R1, #1
	BEQ     rt_hw_context_switch_interrupt_fiq_do

fiq_handler_return:
	LDMFD   SP!,{R0-R7,LR}
	SUBS    PC, LR, #4

rt_hw_context_switch_interrupt_fiq_do:
	MOV     R1,  #0
	STR     R1,  [R0]

	MOV     R1, SP             @; pop {R0-R7,LR} but skip R0-R3
	ADD     SP, SP, #4*4
	LDMFD   SP!, {R4-R7,LR}

	MRS     R0,  SPSR
	SUB     R2,  LR, #4        @; Save old task's PC to R2

	MSR     CPSR_c, #ARM_FIQ_IRQ_MASK|BOOT_ARM_MODE_SVC

	STMFD   SP!, {R2}          @; Push old task's PC
	STMFD   SP!, {R4-R12,LR}   @; Push old task's LR,R12-R4
	LDMFD   R1,  {R1-R4}       @; pop old thread R0-R3 to R1-R4
	STMFD   SP!, {R1-R4}       @; Push old thread R0-R3
	STMFD   SP!, {R0}          @; Push old task's CPSR

	LDR     R4,  =rt_interrupt_from_thread
	LDR     R5,  [R4]
	STR     SP,  [R5]

	LDR     R6,  =rt_interrupt_to_thread
	LDR     R6,  [R6]
	LDR     SP,  [R6]

	LDMFD   SP!, {R4}
	MSR     SPSR_cxsf, R4

	LDMFD   SP!, {R0-R12,LR,PC}^
