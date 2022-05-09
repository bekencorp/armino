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

.equ	ARM_PSR_THUMB,		0x20
.equ	ARM_PSR_INT_DIS,    0xC0
.equ	ARM_PSR_FIQ_DIS,    0x40
.equ	ARM_PSR_IRQ_DIS,    0x80
.equ	ARM_PSR_MODE_MASK,	0x1F
.equ	ARM_PSR_MODE_USR,	0x10
.equ	ARM_PSR_MODE_FIQ,	0x11
.equ	ARM_PSR_MODE_IRQ,	0x12
.equ	ARM_PSR_MODE_SVC,	0x13
.equ	ARM_PSR_MODE_ABT,	0x17
.equ	ARM_PSR_MODE_UND,	0x1B
.equ	ARM_PSR_MODE_SYS,	0x1F

.globl boot_swi
.globl do_swi
.globl intc_irq
.globl intc_fiq
.globl irq_handler
.globl fiq_handler
.globl do_irq
.globl do_fiq
.globl vPortStartFirstTask
.globl preempt_delayed_schedule_set_flag
.globl preempt_delayed_schedule_handler

.macro   portFIQ_SAVE_CONTEXT
	//Push R0 as we are going to use the register.
	STMDB	SP!, {R0}

	MRS R0, spsr
	AND R0, R0, #0x1F
	CMP R0, #0x1F
	BNE 10f

	//Set R0 to point to the task stack pointer.
	STMDB	SP, {SP}^
	NOP
	SUB		SP, SP, #4
	LDMIA	SP!, {R0}

	//Push the return address onto the stack.
	STMDB	R0!, {LR}

	//Now we have saved LR we can use it instead of R0.
	MOV		LR, R0

	//Pop R0 so we can save it onto the system mode stack.
	LDMIA	SP!, {R0}

	//Push all the system mode registers onto the task stack.
	STMDB	LR, {R0-R14}^
	NOP
	NOP
	SUB		LR, LR, #60

	//Push the SPSR onto the task stack.
	MRS		R0, SPSR
	STMDB	LR!, {R0}

	LDR		R0, =ulCriticalNesting
	LDR		R0, [R0]
	STMDB	LR!, {R0}

	//Store the new top of stack for the task.
	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	STR		LR, [R0]

	B       11f

10:
	LDMIA  SP!, {R0}

	STMDB	r13!, {r0-r7,r14}
	NOP
	NOP

	MRS		R0, SPSR
	STMDB   r13!, {R0}

	LDR		R0, =ulCriticalNesting
	LDR		R0, [R0]
	STMDB		r13!, {R0}

	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	STMDB		r13!, {R0}

	SUB r13, r13, #4

11:
	MOV R0, R0
.endm

.macro   portFIQ_RESTORE_CONTEXT

	MRS R0, SPSR
	AND R0, R0, #0x1F
	CMP R0, #0x1F
	BNE 20f

	//Set the LR to the task stack.
	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	LDR		LR, [R0]

	//The critical nesting depth is the first item on the stack.
	//Load it into the ulCriticalNesting variable.
	LDR		R0, =ulCriticalNesting
	LDMFD	LR!, {R1}
	STR		R1, [R0]

	//Get the SPSR from the stack.
	LDMFD	LR!, {R0}
	MSR		SPSR_cxsf, R0

	//Restore all system mode registers for the task.
	LDMFD	LR, {R0-R14}^
	NOP
	NOP

	//Restore the return address.
	LDR		LR, [LR, #+60]

	//And return - correcting the offset in the LR to obtain the
	//correct address.
	SUBS	PC, LR, #4

20:
	ADD r13, r13, #0x4

	LDR		R0, =pxCurrentTCB
	LDMFD		r13!, {R1}
	STR		R1, [R0]

	LDR		R0, =ulCriticalNesting
	LDMFD		r13!, {R1}
	STR		R1, [R0]

	LDMFD		r13!, {R0}
	MSR		SPSR_cxsf, R0

	LDMIA	r13!, {r0-r7,r14}
	NOP
	NOP

	SUBS    pc, r14, #0x4
.endm

.macro   portSAVE_CONTEXT
	//Push R0 as we are going to use the register.
	STMDB	SP!, {R0}

	MRS R0, spsr
	AND R0, R0, #0x1F
	CMP R0, #0x1F
	BNE 10f

	//Set R0 to point to the task stack pointer.
	STMDB	SP, {SP}^
	NOP
	SUB		SP, SP, #4
	LDMIA	SP!, {R0}

	//Push the return address onto the stack.
	STMDB	R0!, {LR}

	//Now we have saved LR we can use it instead of R0.
	MOV		LR, R0

	//Pop R0 so we can save it onto the system mode stack.
	LDMIA	SP!, {R0}

	//Push all the system mode registers onto the task stack.
	STMDB	LR, {R0-R14}^
	NOP
	NOP
	SUB		LR, LR, #60

	//Push the SPSR onto the task stack.
	MRS		R0, SPSR
	STMDB	LR!, {R0}

	LDR		R0, =ulCriticalNesting
	LDR		R0, [R0]
	STMDB	LR!, {R0}

	//Store the new top of stack for the task.
	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	STR		LR, [R0]

	B       11f

10:
	LDMIA  SP!, {R0}

	STMDB	r13!, {r0-r12,r14}
	NOP
	NOP

	MRS		R0, SPSR
	STMDB		r13!, {R0}

	LDR		R0, =ulCriticalNesting
	LDR		R0, [R0]
	STMDB		r13!, {R0}

	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	STMDB		r13!, {R0}

	SUB r13, r13, #4

11:
	MOV R0, R0
.endm

.macro   portRESTORE_CONTEXT

	MRS R0, spsr
	AND R0, R0, #0x1F
	CMP R0, #0x1F
	BNE 20f

	//Set the LR to the task stack.
	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	LDR		LR, [R0]

	//The critical nesting depth is the first item on the stack.
	//Load it into the ulCriticalNesting variable.
	LDR		R0, =ulCriticalNesting
	LDMFD	LR!, {R1}
	STR		R1, [R0]

	//Get the SPSR from the stack.
	LDMFD	LR!, {R0}
	MSR		SPSR_cxsf, R0

	//Restore all system mode registers for the task.
	LDMFD	LR, {R0-R14}^
	NOP
	NOP

	//Restore the return address.
	LDR		LR, [LR, #+60]

	//And return - correcting the offset in the LR to obtain the
	//correct address.
	SUBS	PC, LR, #4

20:
	ADD r13, r13, #0x4

	LDR		R0, =pxCurrentTCB
	LDMFD		r13!, {R1}
	STR		R1, [R0]

	LDR		R0, =ulCriticalNesting
	LDMFD		r13!, {R1}
	STR		R1, [R0]

	LDMFD		r13!, {R0}
	MSR		SPSR_cxsf, R0

	LDMIA	r13!, {r0-r12,r14}
	NOP
	NOP

	SUBS    pc, r14, #0x4
.endm

.align	5
do_swi:
	LDMFD   SP!, {R0-R1}
	B vPortYieldProcessor

.align	5
boot_swi:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400008
	LDR    r0, [R1]
	BX     r0

.align	5
/* Manual context switch function.  This is the SWI hander.*/
vPortYieldProcessor:
	//Add 4 to the LR to make the LR appear exactly
	//as if the context was saved during and IRQ handler.
	ADD		LR, LR, #4

	/*Save the context of the current task...*/
	portSAVE_CONTEXT

	/*Check the previous arm mode*/
	MRS R0, SPSR
	AND R0, R0, #ARM_PSR_MODE_SYS
	CMP R0, #ARM_PSR_MODE_SYS
	BNE 88f

	LDR R0, =vTaskSwitchContext
	MOV LR, PC
	BX R0
	B 89f

88:
	BLX preempt_delayed_schedule_set_flag
89:
	//Restore the context of the selected task.
	portRESTORE_CONTEXT

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

.align	5
do_irq:
	LDMFD   SP!, {R0-R1}
	portSAVE_CONTEXT
	LDR R0, =intc_irq
	MOV LR, PC
	BX R0

	BLX preempt_delayed_schedule_handler
	portRESTORE_CONTEXT

.align	5
do_fiq:
	LDMFD   SP!, {R0-R1}
	portFIQ_SAVE_CONTEXT
	LDR R0, =intc_fiq
	MOV LR, PC
	BX R0

	BLX preempt_delayed_schedule_handler
	portFIQ_RESTORE_CONTEXT

/*Starting the first task is just a matter of restoring the context that*/
/*was created by pxPortInitialiseStack().*/
.code 32
.global vPortStartFirstTask
.type vPortStartFirstTask,%function
vPortStartFirstTask:
	//Set the LR to the task stack.
	LDR 	R0, =pxCurrentTCB
	LDR 	R0, [R0]
	LDR 	LR, [R0]

	//The critical nesting depth is the first item on the stack.
	//Load it into the ulCriticalNesting variable.
	LDR 	R0, =ulCriticalNesting
	LDMFD	LR!, {R1}
	STR 	R1, [R0]

	//Get the SPSR from the stack.
	LDMFD	LR!, {R0}
	MSR 	SPSR_cxsf, R0

	//Restore all system mode registers for the task.
	LDMFD	LR, {R0-R14}^
	NOP
	NOP

	//Restore the return address.
	LDR 	LR, [LR, #+60]

	//And return - correcting the offset in the LR to obtain the
	//correct address.
	SUBS	PC, LR, #4

