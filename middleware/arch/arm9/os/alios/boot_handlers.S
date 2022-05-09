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
#include "boot_asm_macro.h"

.globl do_irq
.globl do_fiq
.globl do_swi
.globl boot_swi
.globl irq_handler
.globl fiq_handler
.globl fiq_pre_proc
.globl fiq_end_proc
.global boot_exception_swi

.align  5
boot_swi:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400008
	LDR    r0, [R1]
	BX     r0

.align  5
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
do_swi:
	B do_swi

.align  5
do_irq:
	LDMFD   SP!, {R0-R1}^
	STMFD   SP!, {R1-R3}           @We will use R1-R3 as temporary registers

	MOV     R1, SP
	ADD     SP, SP, #12            @Adjust IRQ stack pointer
	SUB     R2, LR, #4             @Adjust PC for return address to task

	MRS     R3, SPSR               @Copy SPSR (Task CPSR)

	MSR     CPSR_c, #0xd3          @Change to SVC mode with interrupt disabled

	/* SAVE TASK'S CONTEXT ONTO OLD TASK'S STACK */
	STMFD   SP!, {R2}              @Push task''s PC
	STMFD   SP!, {R4-R12, LR}
	LDMFD   R1!, {R4-R6}           @Load Task''s R1-R3 from IRQ stack
	STMFD   SP!, {R4-R6}           @Push Task''s R1-R3 to SVC stack
	STMFD   SP!, {R0}              @Push Task''s R0 to SVC stack

	STMFD   SP!, {R3}              @Push task''s CPSR

	LDR     R4,=g_active_task
	LDR     R5,[R4]
	STR     SP,[R5]

	BL      krhino_intrpt_enter

	MSR     CPSR_c,#0xd2           @Change to IRQ mode to use IRQ stack to handle interrupt with interrupt disbaled

	BL      intc_irq
	MSR     CPSR_c,#0xd3           @Change to SVC mode with interrupt disabled
	BL      krhino_intrpt_exit
	LDMFD   SP!,{R4}               @POP the task''s CPSR
	MSR     SPSR_cxsf,R4
	LDMFD   SP!,{R0-R12,LR,PC}^    @POP new Task''s context

.align  5
do_fiq:
	LDMFD   SP!, {R0-R1}^
	MRS     R8, spsr               @R8 R9 is unique to fiq
	MRS     R9, spsr               @save SPSR_fiq
	AND     R8, R8, #0x1f
	CMP     R8, #0x12
	BNE     88f
	STMFD   SP!, {R0-R12, LR}

	BL      fiq_pre_proc
	BL      intc_fiq
	BL      fiq_end_proc

	LDMFD   SP!,{R0-R12,LR}

	MSR      SPSR_cxsf,R9         @restore SPSR_fiq
	SUBS PC, LR,#4                @restore CPSR

	88:
	STMFD   SP!, {R1-R3}           @We will use R1-R3 as temporary registers

	MOV     R1, SP
	ADD     SP, SP, #12            @Adjust IRQ stack pointer
	SUB     R2, LR, #4             @Adjust PC for return address to task

	MRS     R3, SPSR               @Copy SPSR (Task CPSR)

	MSR     CPSR_c, #0xd3          @Change to SVC mode with interrupt disabled

	/* SAVE TASK'S CONTEXT ONTO OLD TASK'S STACK */
	STMFD   SP!, {R2}              @Push task''s PC
	STMFD   SP!, {R4-R12, LR}
	LDMFD   R1!, {R4-R6}           @Load Task''s R1-R3 from IRQ stack
	STMFD   SP!, {R4-R6}           @Push Task''s R1-R3 to SVC stack
	STMFD   SP!, {R0}              @Push Task''s R0 to SVC stack

	STMFD   SP!, {R3}              @Push task''s CPSR

	LDR     R4,=g_active_task
	LDR     R5,[R4]
	STR     SP,[R5]

	BL      krhino_intrpt_enter

	MSR     CPSR_c,#0xd1           @Change to FIRQ mode to use IRQ stack to handle interrupt with interrupt disbaled

	BL      intc_fiq
	MSR     CPSR_c,#0xd3           @Change to SVC mode with interrupt disabled
	BL      krhino_intrpt_exit
	LDMFD   SP!,{R4}               @POP the task''s CPSR
	MSR     SPSR_cxsf,R4
	LDMFD   SP!,{R0-R12,LR,PC}^    @POP new Task''s context
