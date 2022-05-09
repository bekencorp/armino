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

/**
 * void rt_hw_context_switch(uint32_t from, uint32_t to);
 * r0 --> from
 * r1 --> to
 */
.globl rt_hw_context_switch
.type rt_hw_context_switch, %function
rt_hw_context_switch:
	STMFD   SP!, {LR}               @; push pc (lr should be pushed in place of pc)
	STMFD   SP!, {R0-R12, LR}       @; push lr & register file
	MRS     R4, CPSR
	tst     lr, #0x01
	beq     _ARM_MODE
	orr     r4, r4, #0x20           @; it's thumb code set T=1

_ARM_MODE:
	STMFD   SP!, {R4}               @; push cpsr
	STR     SP, [R0]                @; store sp in preempted tasks tcb
	LDR     SP, [R1]                @; get new task stack pointer
	LDMFD   SP!, {R4}               @; pop new task spsr
	MSR     SPSR_cxsf, R4
	LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
 * void rt_hw_context_switch_to(uint32_t to);
 * r0 --> to
 */
.globl rt_hw_context_switch_to
.type rt_hw_context_switch_to, %function
rt_hw_context_switch_to:
	LDR     SP, [R0]                @; get new task stack pointer
	LDMFD   SP!, {R4}               @; pop new task cpsr
	MSR     SPSR_cxsf, R4
	LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
 * void rt_hw_context_switch_interrupt(uint32_t from, uint32_t to);
 */
.globl rt_thread_switch_interrupt_flag
.globl rt_interrupt_from_thread
.globl rt_interrupt_to_thread
.globl rt_hw_context_switch_interrupt
.type rt_hw_context_switch_interrupt, %function
rt_hw_context_switch_interrupt:
	LDR     R2, =rt_thread_switch_interrupt_flag
	LDR     R3, [R2]
	CMP     R3, #1
	BEQ     _reswitch
	MOV     R3, #1                          @; set flag to 1
	STR     R3, [R2]
	LDR     R2, =rt_interrupt_from_thread   @; set rt_interrupt_from_thread
	STR     R0, [R2]
_reswitch:
	LDR     R2, =rt_interrupt_to_thread     @; set rt_interrupt_to_thread
	STR     R1, [R2]
	BX      LR
