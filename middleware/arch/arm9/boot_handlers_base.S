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

.globl entry_main
.globl boot_reset
.globl boot_undefined
.globl boot_pabort
.globl boot_dabort
.globl boot_reserved
.globl do_undefined
.globl do_pabort
.globl do_dabort
.globl do_reserved

.globl boot_exception_undefine
.globl boot_exception_prefetch_abort
.globl boot_exception_data_abort
.globl boot_exception_reserved

.globl boot_stack_base_UNUSED
.globl boot_stack_len_UNUSED
.globl boot_stack_base_IRQ
.globl boot_stack_len_IRQ
.globl boot_stack_base_SVC
.globl boot_stack_len_SVC
.globl boot_stack_base_FIQ
.globl boot_stack_len_FIQ
.globl boot_stack_base_SYS
.globl boot_stack_len_SYS


/**
 * Context save and restore macro definitions
 */
.macro PUSH_ALL_ARM_REG
	BOOT_CHANGE_MODE  BOOT_ARM_MODE_SYS BOOT_ARM_MODE_MASK
	LDR R1, = MCU_REG_BACKUP_TOP_SYS // get backup top.
	STMFD   R1!, {R8-R14}       // backup R8-R14
	MRS   R0, CPSR
	STMFD   R1!, {R0}           // backup CPSR

	BOOT_CHANGE_MODE  BOOT_ARM_MODE_IRQ BOOT_ARM_MODE_MASK
	LDR R1, = MCU_REG_BACKUP_TOP_IRQ // get backup top.
	STMFD   R1!, {R8-R14}       // backup R8-R14
	MRS   R0, SPSR
	STMFD   R1!, {R0}           // backup SPSR
	MRS   R0, CPSR
	STMFD   R1!, {R0}           // backup CPSR

	BOOT_CHANGE_MODE  BOOT_ARM_MODE_FIQ BOOT_ARM_MODE_MASK
	LDR R1, = MCU_REG_BACKUP_TOP_FIQ // get backup top.
	STMFD   R1!, {R8-R14}       // backup R8-R14
	MRS   R0, SPSR
	STMFD   R1!, {R0}           // backup SPSR
	MRS   R0, CPSR
	STMFD   R1!, {R0}           // backup CPSR


	BOOT_CHANGE_MODE  BOOT_ARM_MODE_ABT BOOT_ARM_MODE_MASK
	LDR R1, = MCU_REG_BACKUP_TOP_ABT // get backup top.
	STMFD   R1!, {R8-R14}       // backup R8-R14
	MRS   R0, SPSR
	STMFD   R1!, {R0}           // backup SPSR
	MRS   R0, CPSR
	STMFD   R1!, {R0}           // backup CPSR


	BOOT_CHANGE_MODE  BOOT_ARM_MODE_UND BOOT_ARM_MODE_MASK
	LDR R1, = MCU_REG_BACKUP_TOP_UND // get backup top.
	STMFD   R1!, {R8-R14}       // backup R8-R14
	MRS   R0, SPSR
	STMFD   R1!, {R0}           // backup SPSR
	MRS   R0, CPSR
	STMFD   R1!, {R0}           // backup CPSR


	BOOT_CHANGE_MODE  BOOT_ARM_MODE_SVC BOOT_ARM_MODE_MASK
	LDR R1, = MCU_REG_BACKUP_TOP_SVC // get backup top.
	STMFD   R1!, {R8-R14}       // backup R8-R14
	MRS   R0, SPSR
	STMFD   R1!, {R0}           // backup SPSR
	MRS   R0, CPSR
	STMFD   R1!, {R0}           // backup CPSR
.endm

/**
 * Macro for switching ARM mode
 */
.macro	BOOT_CHANGE_MODE, mode, mode_mask
	MRS   R0, CPSR
	BIC   R0, R0, #\mode_mask
	ORR   R0, R0, #\mode
	MSR   CPSR_c, R0
.endm

/**
 * Macro for setting the stack
 */
.macro  BOOT_SET_STACK, stackStart, stackLen, color
	LDR   R0, \stackStart
	LDR   R1, \stackLen

	ADD   R1, R1, R0
	MOV   SP, R1        //Set stack pointer

	LDR   R2, =\color

3:
	CMP   R0, R1        //End of stack?
	STRLT R2, [r0]      //Colorize stack word
	ADDLT R0, R0, #4
	BLT   3b            //branch to previous local label
.endm

.section .bss

.align 3
.global fiq_stack_start
fiq_stack_start:
	.space FIQ_STACK_SIZE

.align 3
.global irq_stack_start
irq_stack_start:
	.space IRQ_STACK_SIZE

.align 3
.global und_stack_start
und_stack_start:
	.space UND_STACK_SIZE

.align 3
.global sys_stack_start
sys_stack_start:
	 .space SYS_STACK_SIZE

.align 3
.global abt_stack_start
abt_stack_start:
	.space ABT_STACK_SIZE

.align 3
.global svc_stack_start
svc_stack_start:
	 .space SVC_STACK_SIZE


.section ".boot", "ax"

boot_reset:
	//Disable IRQ and FIQ before starting anything
	MRS   R0, CPSR
	ORR   R0, R0, #0xC0
	MSR   CPSR_c, R0

	/*Init the BSS section*/
	BL _sysboot_zi_init

	//Setup all stacks //Note: Abt and Usr mode are not used
	BOOT_CHANGE_MODE BOOT_ARM_MODE_SYS BOOT_ARM_MODE_MASK
	BOOT_SET_STACK   boot_stack_base_SYS boot_stack_len_SYS BOOT_COLOR_SYS

	BOOT_CHANGE_MODE BOOT_ARM_MODE_ABT BOOT_ARM_MODE_MASK
	BOOT_SET_STACK   boot_stack_base_UNUSED boot_stack_len_UNUSED BOOT_COLOR_UNUSED

	BOOT_CHANGE_MODE BOOT_ARM_MODE_UND BOOT_ARM_MODE_MASK
	BOOT_SET_STACK   boot_stack_base_UNUSED boot_stack_len_UNUSED BOOT_COLOR_UNUSED

//TODO optimize it, put the magic number to soc.h
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236)
	B AFTER_FLAG
	.org  0xc0 //0x100 - sizeof(section.vector) = 0x100 - 0x40
	.word 0x32374B42
	.word 0x00003133

AFTER_FLAG:
#elif  (CONFIG_SOC_BK7271)
	B AFTER_FLAG
	.org  0xc0
	.word 0x32374B42
	.word 0x00003137
AFTER_FLAG:
#endif

	BOOT_CHANGE_MODE BOOT_ARM_MODE_IRQ BOOT_ARM_MODE_MASK
	BOOT_SET_STACK   boot_stack_base_IRQ boot_stack_len_IRQ BOOT_COLOR_IRQ

	BOOT_CHANGE_MODE BOOT_ARM_MODE_FIQ BOOT_ARM_MODE_MASK
	BOOT_SET_STACK   boot_stack_base_FIQ boot_stack_len_FIQ BOOT_COLOR_FIQ

	//Clear FIQ banked registers while in FIQ mode
	MOV     R8,  #0
	MOV     R9,  #0
	MOV     R10, #0
	MOV     R11, #0
	MOV     R12, #0

	BOOT_CHANGE_MODE BOOT_ARM_MODE_SVC BOOT_ARM_MODE_MASK
	BOOT_SET_STACK   boot_stack_base_SVC boot_stack_len_SVC BOOT_COLOR_SVC

	/*Stay in Supervisor Mode, copy data from binary to ram*/
	BL _sysboot_copy_data_to_ram
#if ((CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7271)) || (CONFIG_SOC_BK7236)
	BL _sysboot_copy_code_to_itcm
#endif

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236)
	BL _sysboot_tcmbss_init
#endif

	//Clear Registers
	MOV R0, #0
	MOV R1, #0
	MOV R2, #0
	MOV R3, #0
	MOV R4, #0
	MOV R5, #0
	MOV R6, #0
	MOV R7, #0
	MOV R8, #0
	MOV R9, #0
	MOV R10, #0
	MOV R11, #0
	MOV R12, #0

	/* start main entry*/
	B entry_main
	B .

//Globals
boot_stack_base_UNUSED:
	.word und_stack_start

boot_stack_len_UNUSED:
	.word UND_STACK_SIZE

boot_stack_base_IRQ:
	.word irq_stack_start

boot_stack_len_IRQ:
	.word IRQ_STACK_SIZE

boot_stack_base_SVC:
	.word svc_stack_start

boot_stack_len_SVC:
	 .word SVC_STACK_SIZE

boot_stack_base_FIQ:
	 .word fiq_stack_start

boot_stack_len_FIQ:
	.word FIQ_STACK_SIZE

boot_stack_base_SYS:
	 .word sys_stack_start

boot_stack_len_SYS:
	 .word SYS_STACK_SIZE

/*FUNCTION:     _sysboot_copy_data_to_ram*/
/*DESCRIPTION:  copy main stack code from FLASH/ROM to SRAM*/
_sysboot_copy_data_to_ram:
	LDR     R0, =_data_flash_begin
	LDR     R1, =_data_ram_begin
	LDR     R2, =_data_ram_end

4:
	CMP R1, R2
	LDRLO   R4, [R0], #4
	STRLO   R4, [R1], #4
	BLO     4b
	BX LR

/*FUNCTION:     _sysboot_zi_init*/
/*DESCRIPTION:  Initialise Zero-Init Data Segment*/
_sysboot_zi_init:
	LDR     R0, =_bss_start
	LDR     R1, =_bss_end

	MOV R3, R1
	MOV R4, R0
	MOV R2, #0
5:
	CMP R4, R3
	STRLO R2, [R4], #4
	BLO 5b
	BX LR

#if ((CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7271)) || (CONFIG_SOC_BK7236)
/*FUNCTION:     _sysboot_copy_code_to_itcm*/
/*DESCRIPTION:  copy itcm code from FLASH/ROM to SRAM*/
_sysboot_copy_code_to_itcm:
	LDR     R0, =_itcmcode_flash_begin
	LDR     R1, =_itcmcode_ram_begin
	LDR     R2, =_itcmcode_ram_end

6:
	CMP R1, R2
	LDRLO   R4, [R0], #4
	STRLO   R4, [R1], #4
	BLO     6b
	BX LR
#endif

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236)
/*FUNCTION:     _sysboot_sdbss_init*/
/*DESCRIPTION:  Initialise Zero-Init Data Segment of TCM */
_sysboot_tcmbss_init:
	LDR     R0, =_tcmbss_start
	LDR     R1, =_tcmbss_end

	MOV R3, R1
	MOV R4, R0
	MOV R2, #0
8:
	CMP R4, R3
	STRLO R2, [R4], #4
	BLO 8b
	BX LR
#endif

.align	5
do_undefined:
	LDMFD   SP!, {R0-R1}
	PUSH_SVC_REG

	STMFD  sp!,{r0-r1}
	PUSH_ALL_ARM_REG
	BOOT_CHANGE_MODE  BOOT_ARM_MODE_UND BOOT_ARM_MODE_MASK
	LDMFD   SP!, {R0-R1}

	B boot_exception_undefine

.align	5
do_pabort:
	LDMFD   SP!, {R0-R1}
	PUSH_SVC_REG

	STMFD  sp!,{r0-r1}
	PUSH_ALL_ARM_REG
	BOOT_CHANGE_MODE  BOOT_ARM_MODE_ABT BOOT_ARM_MODE_MASK
	LDMFD   SP!, {R0-R1}

	B boot_exception_prefetch_abort

.align	5
do_dabort:
	LDMFD   SP!, {R0-R1}
	PUSH_SVC_REG

	STMFD  sp!,{r0-r1}
	PUSH_ALL_ARM_REG
	BOOT_CHANGE_MODE  BOOT_ARM_MODE_ABT BOOT_ARM_MODE_MASK
	LDMFD   SP!, {R0-R1}

	B boot_exception_data_abort

.align	5
do_reserved:
	LDMFD   SP!, {R0-R1}
	PUSH_SVC_REG
	B boot_exception_reserved

.align  5
boot_undefined:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x40000c
	LDR    r0, [R1]
	BX     r0

.align  5
boot_pabort:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400010
	LDR    r0, [R1]
	BX     r0

.align  5
boot_dabort:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400014
	LDR    r0, [R1]
	BX     r0

.align  5
boot_reserved:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400018
	LDR    r0, [R1]
	BX     r0

.code 32
.global arch_wait_for_interrupt
.type arch_wait_for_interrupt,%function
arch_wait_for_interrupt:
	MOV R0, #0
	MCR p15, 0, R0, c7, c0, 4
	BX LR

.code 32
.globl arch_enable_align_fault
.type arch_enable_align_fault, %function
arch_enable_align_fault:
	MRC p15, 0, R0, c1, c0, 0
	ORR R0, R0, #0x02
	MCR p15, 0, R0, c1, c0, 0
	BX LR

.globl arch_disable_align_fault
.type arch_disable_align_fault, %function
arch_disable_align_fault:
	MRC p15, 0, R0, c1, c0, 0
	BIC R0, R0, #0x02
	MCR p15, 0, R0, c1, c0, 0
	BX LR
