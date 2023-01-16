/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code which tailors the port to a specific RISC-V chip:
 *
 * + The code that is common to all RISC-V chips is implemented in
 *   FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S.  There is only one
 *   portASM.S file because the same file is used no matter which RISC-V chip is
 *   in use.
 *
 * + The code that tailors the kernel's RISC-V port to a specific RISC-V
 *   chip is implemented in freertos_risc_v_chip_specific_extensions.h.  There
 *   is one freertos_risc_v_chip_specific_extensions.h that can be used with any
 *   RISC-V chip that both includes a standard CLINT and does not add to the
 *   base set of RISC-V registers.  There are additional
 *   freertos_risc_v_chip_specific_extensions.h files for RISC-V implementations
 *   that do not include a standard CLINT or do add to the base set of RISC-V
 *   registers.
 *
 * CARE MUST BE TAKEN TO INCLDUE THE CORRECT
 * freertos_risc_v_chip_specific_extensions.h HEADER FILE FOR THE CHIP
 * IN USE.  To include the correct freertos_risc_v_chip_specific_extensions.h
 * header file ensure the path to the correct header file is in the assembler's
 * include path.
 *
 * This freertos_risc_v_chip_specific_extensions.h is for use on RISC-V chips
 * that include a standard CLINT and do not add to the base set of RISC-V
 * registers.
 *
 */
#if __riscv_xlen == 64
	#define portWORD_SIZE 8
	#define store_x sd
	#define load_x ld
#elif __riscv_xlen == 32
	#define store_x sw
	#define load_x lw
	#define portWORD_SIZE 4
#else
	#error Assembler did not define __riscv_xlen
#endif

#include "freertos_risc_v_chip_specific_extensions.h"
#include "core_v5.h"

/* Check the freertos_risc_v_chip_specific_extensions.h and/or command line
definitions. */
#if defined( portasmHAS_CLINT ) && defined( portasmHAS_MTIME )
	#error The portasmHAS_CLINT constant has been deprecated.  Please replace it with portasmHAS_MTIME.  portasmHAS_CLINT and portasmHAS_MTIME cannot both be defined at once.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifdef portasmHAS_CLINT
	#warning The portasmHAS_CLINT constant has been deprecated.  Please replace it with portasmHAS_MTIME and portasmHAS_SIFIVE_CLINT.  For now portasmHAS_MTIME and portasmHAS_SIFIVE_CLINT are derived from portasmHAS_CLINT.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
	#define portasmHAS_MTIME portasmHAS_CLINT
	#define portasmHAS_SIFIVE_CLINT portasmHAS_CLINT
#endif

#ifndef portasmHAS_MTIME
	#error freertos_risc_v_chip_specific_extensions.h must define portasmHAS_MTIME to either 1 (MTIME clock present) or 0 (MTIME clock not present).  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef portasmHANDLE_INTERRUPT
	#error portasmHANDLE_INTERRUPT must be defined to the function to be called to handle external/peripheral interrupts.  portasmHANDLE_INTERRUPT can be defined on the assembler command line or in the appropriate freertos_risc_v_chip_specific_extensions.h header file.  https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef portasmHAS_SIFIVE_CLINT
	#define portasmHAS_SIFIVE_CLINT 0
#endif

/* Only the standard core registers are stored by default.  Any additional
registers must be saved by the portasmSAVE_ADDITIONAL_REGISTERS and
portasmRESTORE_ADDITIONAL_REGISTERS macros - which can be defined in a chip
specific version of freertos_risc_v_chip_specific_extensions.h.  See the notes
at the top of this file. */
#define portCONTEXT_SIZE ( 30 * portWORD_SIZE )

.global xPortStartFirstTask
.global freertos_risc_v_trap_handler
.global pxPortInitialiseStack
.extern pxCurrentTCB
.extern freertos_trap_handler
.extern xISRStackTop
.extern g_trap_nest_cnt

/*-----------------------------------------------------------*/

.align 4
.func
freertos_risc_v_trap_handler:
	addi sp, sp, -120
	store_x x1, 0 * portWORD_SIZE( sp )		/* ra */
	store_x x5, 1 * portWORD_SIZE( sp )		/* t0 */
	store_x x6, 2 * portWORD_SIZE( sp )		/* t1 */
	store_x x7, 3 * portWORD_SIZE( sp )		/* t2 */
	store_x x10, 4 * portWORD_SIZE( sp )		/* a0 */
	store_x x11, 5 * portWORD_SIZE( sp )
	store_x x12, 6 * portWORD_SIZE( sp )
	store_x x13, 7 * portWORD_SIZE( sp )
	store_x x14, 8 * portWORD_SIZE( sp )
	store_x x15, 9 * portWORD_SIZE( sp )
	store_x x16, 10 * portWORD_SIZE( sp )
	store_x x17, 11 * portWORD_SIZE( sp )		/* a7 */
	store_x x28, 12 * portWORD_SIZE( sp )		/* t3 */
	store_x x29, 13 * portWORD_SIZE( sp )
	store_x x30, 14 * portWORD_SIZE( sp )
	store_x x31, 15 * portWORD_SIZE( sp )		/* t6 */

	csrr t0, uepc
	csrr t1, ustatus
	store_x t0, 16 * portWORD_SIZE( sp )
	store_x t1, 17 * portWORD_SIZE( sp )

	mv a1, sp
	csrrw a0, ucause, x0

	la   t0, g_trap_nest_cnt
	lw   t2, 0(t0)
	addi t1, t2, 1
	sw   t1, 0(t0)
	beqz t2, _save_all_ctxt			/* NOT nested trap, need to write sp to first TCB member. */

	/* nested trap. */
	bltz a0, _trap_process		/* if a0 < 0 (interrupt), need not to save all regs to trap stack. */

_save_all_ctxt:
	store_x x8, 18 * portWORD_SIZE( sp )		/* s0 */
	store_x x9, 19 * portWORD_SIZE( sp )		/* s1 */
	store_x x18, 20 * portWORD_SIZE( sp )		/* s2 */
	store_x x19, 21 * portWORD_SIZE( sp )
	store_x x20, 22 * portWORD_SIZE( sp )
	store_x x21, 23 * portWORD_SIZE( sp )
	store_x x22, 24 * portWORD_SIZE( sp )
	store_x x23, 25 * portWORD_SIZE( sp )
	store_x x24, 26 * portWORD_SIZE( sp )
	store_x x25, 27 * portWORD_SIZE( sp )
	store_x x26, 28 * portWORD_SIZE( sp )
	store_x x27, 29 * portWORD_SIZE( sp )		/* s11 */

	bgtz t2, _trap_process			/* Nested trap, need not write sp to first TCB member. */

	portasmSAVE_ADDITIONAL_REGISTERS	/* Defined in freertos_risc_v_chip_specific_extensions.h to save any registers unique to the RISC-V implementation. */

	load_x  t0, pxCurrentTCB			/* Load pxCurrentTCB. */
	store_x  sp, 0( t0 )				/* Write sp to first TCB member. */

	load_x t0, xISRStackTop			/* Switch to ISR stack before function call. */
	mv   sp, t0

_trap_process:
	call freertos_trap_handler

	la   t0, g_trap_nest_cnt
	lw   t2, 0(t0)
	addi t2, t2, -1
	sw   t2, 0(t0)
	bgtz t2, _nested_trap_exit			/* nested trap. */

_trap_run_task:

	load_x  t0, pxCurrentTCB			/* Load pxCurrentTCB. */
	load_x  sp, 0( t0 )				 	/* Read sp from first TCB member. */

	portasmRESTORE_ADDITIONAL_REGISTERS	/* Defined in freertos_risc_v_chip_specific_extensions.h to restore any registers unique to the RISC-V implementation. */

	load_x  x8, 18 * portWORD_SIZE( sp )	/* s0/fp */
	load_x  x9, 19 * portWORD_SIZE( sp )	/* s1 */
	load_x  x18, 20 * portWORD_SIZE( sp )	/* s2 */
	load_x  x19, 21 * portWORD_SIZE( sp )	/* s3 */
	load_x  x20, 22 * portWORD_SIZE( sp )	/* s4 */
	load_x  x21, 23 * portWORD_SIZE( sp )	/* s5 */
	load_x  x22, 24 * portWORD_SIZE( sp )	/* s6 */
	load_x  x23, 25 * portWORD_SIZE( sp )	/* s7 */
	load_x  x24, 26 * portWORD_SIZE( sp )	/* s8 */
	load_x  x25, 27 * portWORD_SIZE( sp )	/* s9 */
	load_x  x26, 28 * portWORD_SIZE( sp )	/* s10 */
	load_x  x27, 29 * portWORD_SIZE( sp )	/* s11 */

_nested_trap_exit:
	load_x  t0, 16 * portWORD_SIZE( sp )
	load_x  t1, 17 * portWORD_SIZE( sp )
	csrw uepc, t0
	csrw ustatus, t1

	load_x  x1, 0 * portWORD_SIZE( sp )
	load_x  x5, 1 * portWORD_SIZE( sp )		/* t0 */
	load_x  x6, 2 * portWORD_SIZE( sp )		/* t1 */
	load_x  x7, 3 * portWORD_SIZE( sp )		/* t2 */
	load_x  x10, 4 * portWORD_SIZE( sp )	/* a0 */
	load_x  x11, 5 * portWORD_SIZE( sp )	/* a1 */
	load_x  x12, 6 * portWORD_SIZE( sp )	/* a2 */
	load_x  x13, 7 * portWORD_SIZE( sp )	/* a3 */
	load_x  x14, 8 * portWORD_SIZE( sp )	/* a4 */
	load_x  x15, 9 * portWORD_SIZE( sp )	/* a5 */
	load_x  x16, 10 * portWORD_SIZE( sp )	/* a6 */
	load_x  x17, 11 * portWORD_SIZE( sp )	/* a7 */
	load_x  x28, 12 * portWORD_SIZE( sp )	/* t3 */
	load_x  x29, 13 * portWORD_SIZE( sp )	/* t4 */
	load_x  x30, 14 * portWORD_SIZE( sp )	/* t5 */
	load_x  x31, 15 * portWORD_SIZE( sp )	/* t6 */
	addi sp, sp, 120

	uret
	.endfunc

.align 2
.func
xPortStartFirstTask:
	j _trap_run_task
	.endfunc

/*-----------------------------------------------------------*/

/*
 * Unlike other ports pxPortInitialiseStack() is written in assembly code as it
 * needs access to the portasmADDITIONAL_CONTEXT_SIZE constant.  The prototype
 * for the function is as per the other ports:
 * StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );
 *
 * As per the standard RISC-V ABI pxTopcOfStack is passed in in a0, pxCode in
 * a1, and pvParameters in a2.  The new top of stack is passed out in a0.
 *
 * RISC-V maps registers to ABI names as follows (X1 to X31 integer registers
 * for the 'I' profile, X1 to X15 for the 'E' profile, currently I assumed).
 *
 * Register		ABI Name	Description						Saver
 * x0			zero		Hard-wired zero					-
 * x1			ra			Return address					Caller
 * x2			sp			Stack pointer					Callee
 * x3			gp			Global pointer					-
 * x4			tp			Thread pointer					-
 * x5-7			t0-2		Temporaries						Caller
 * x8			s0/fp		Saved register/Frame pointer	Callee
 * x9			s1			Saved register					Callee
 * x10-11		a0-1		Function Arguments/return values Caller
 * x12-17		a2-7		Function arguments				Caller
 * x18-27		s2-11		Saved registers					Callee
 * x28-31		t3-6		Temporaries						Caller
 *
 * The RISC-V context is saved t FreeRTOS tasks in the following stack frame,
 * where the global and thread pointers are currently assumed to be constant so
 * are not saved:
 *
 * [chip specific registers go here]
 * x27
 * x26
 * x25
 * x24
 * x23
 * x22
 * x21
 * x20
 * x19
 * x18
 * x9
 * x8
 * mstatus
 * pxCode
 * x31
 * x30
 * x29
 * x28
 * x17
 * x16
 * x15
 * x14
 * x13
 * x12
 * x11
 * pvParameters
 * x7
 * x6
 * x5
 * portTASK_RETURN_ADDRESS
 */
.align 2
.func
pxPortInitialiseStack:

	csrr t0, ustatus					/* Obtain current ustatus value. */
	andi t0, t0, ~0x1					/* Ensure interrupts are disabled when the stack is restored within an ISR.  Required when a task is created after the schedulre has been started, otherwise interrupts would be disabled anyway. */
    ori t0, t0, 0x10					/* Set UPIE bit in ustatus value. */

	addi a0, a0, -120
	store_x x0, 0 * portWORD_SIZE( a0 )		/* ra *//* Return address onto the stack, could be portTASK_RETURN_ADDRESS */
	store_x x0, 1 * portWORD_SIZE( a0 )		/* t0 */
	store_x x0, 2 * portWORD_SIZE( a0 )		/* t1 */
	store_x x0, 3 * portWORD_SIZE( a0 )		/* t2 */
	store_x a2, 4 * portWORD_SIZE( a0 )		/* a0 *//* Task parameters (pvParameters parameter) goes into register X10/a0 on the stack. */
	store_x x0, 5 * portWORD_SIZE( a0 )
	store_x x0, 6 * portWORD_SIZE( a0 )
	store_x x0, 7 * portWORD_SIZE( a0 )
	store_x x0, 8 * portWORD_SIZE( a0 )
	store_x x0, 9 * portWORD_SIZE( a0 )
	store_x x0, 10 * portWORD_SIZE( a0 )
	store_x x0, 11 * portWORD_SIZE( a0 )		/* a7 */
	store_x x0, 12 * portWORD_SIZE( a0 )		/* t3 */
	store_x x0, 13 * portWORD_SIZE( a0 )
	store_x x0, 14 * portWORD_SIZE( a0 )
	store_x x0, 15 * portWORD_SIZE( a0 )		/* t6 */

	store_x a1, 16 * portWORD_SIZE( a0 )		/* uepc, uret value (pxCode parameter) onto the stack. */
	store_x t0, 17 * portWORD_SIZE( a0 )		/* ustatus onto the stack. */

	store_x x0, 18 * portWORD_SIZE( a0 )		/* s0 */
	store_x x0, 19 * portWORD_SIZE( a0 )		/* s1 */
	store_x x0, 20 * portWORD_SIZE( a0 )		/* s2 */
	store_x x0, 21 * portWORD_SIZE( a0 )
	store_x x0, 22 * portWORD_SIZE( a0 )
	store_x x0, 23 * portWORD_SIZE( a0 )
	store_x x0, 24 * portWORD_SIZE( a0 )
	store_x x0, 25 * portWORD_SIZE( a0 )
	store_x x0, 26 * portWORD_SIZE( a0 )
	store_x x0, 27 * portWORD_SIZE( a0 )
	store_x x0, 28 * portWORD_SIZE( a0 )
	store_x x0, 29 * portWORD_SIZE( a0 )		/* s11 */

	addi t0, x0, portasmADDITIONAL_CONTEXT_SIZE /* The number of chip specific additional registers. */
chip_specific_stack_frame:				/* First add any chip specific registers to the stack frame being created. */
	beq t0, x0, 1f						/* No more chip specific registers to save. */
	addi a0, a0, -portWORD_SIZE			/* Make space for chip specific register. */
	store_x x0, 0(a0)					/* Give the chip specific register an initial value of zero. */
	addi t0, t0, -1						/* Decrement the count of chip specific registers remaining. */
	j chip_specific_stack_frame			/* Until no more chip specific registers. */
1:
	ret
	.endfunc
/*-----------------------------------------------------------*/
