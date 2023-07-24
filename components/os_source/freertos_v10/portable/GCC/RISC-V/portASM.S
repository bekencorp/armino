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

// no need include trcRecorder.h for PortASM.s compile
#define TRC_RECORDER_H

#include "freertos_risc_v_chip_specific_extensions.h"
#include "core_v5.h"

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

	csrw uscratch, sp
	//mv  a1, sp

	portasmSAVE_ADDITIONAL_REGISTERS	/* Defined in freertos_risc_v_chip_specific_extensions.h to save any registers unique to the RISC-V implementation. */

	la   t0, g_trap_nest_cnt
	lw   t2, 0(t0)
	addi t1, t2, 1
	sw   t1, 0(t0)
	bgtz t2, _trap_process			/* Nested trap, need not write sp to first TCB member. */

	load_x  t0, pxCurrentTCB		/* Load pxCurrentTCB. */
	store_x  sp, 0( t0 )			/* Write sp to first TCB member. */

#ifndef HW_STACK_PROTECT	
//	load_x t0, xISRStackTop			/* Switch to ISR stack before function call. */
	la   t0, _stack					/* Switch to ISR stack before function call. */
	mv   sp, t0
#else
	la   t0, _stack
	li   a0, 6                  // SYS_CALL_SET_STACK_LIMIT
	mv   a1, t0
	la   a2, _dtcm_bss_end
	mv   a3, t0
	ecall                       // mon_set_stack_limit(u32 statc_max_addr, u32 stack_min_addr, u32 sp);
#endif

_trap_process:
	csrr a1, uscratch
	csrrw a0, ucause, x0
	call freertos_trap_handler

	la   t0, g_trap_nest_cnt
	lw   t2, 0(t0)
	addi t2, t2, -1
	sw   t2, 0(t0)
	bgtz t2, _nested_trap_exit			/* nested trap. */

_trap_run_task:

#ifndef HW_STACK_PROTECT	
	load_x  t0, pxCurrentTCB			/* Load pxCurrentTCB. */
	load_x  sp, 0( t0 )				 	/* Read sp from first TCB member. */
#else
	call  vTaskStackAddr
	mv    s1, a0
	call  vTaskStackSize
	c.slli  a0, 2
	add   a1, a0, s1
	mv    a2, s1

	load_x  t0, pxCurrentTCB			/* Load pxCurrentTCB. */
	load_x  a3, 0( t0 )				 	/* Read sp from first TCB member. */
	li      a0, 6               // SYS_CALL_SET_STACK_LIMIT
	ecall                       // mon_set_stack_limit(u32 statc_max_addr, u32 stack_min_addr, u32 sp);
#endif

_nested_trap_exit:

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

