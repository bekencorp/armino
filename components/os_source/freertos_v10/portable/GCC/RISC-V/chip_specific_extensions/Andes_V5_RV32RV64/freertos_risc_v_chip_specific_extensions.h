/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and t

 o permit persons to whom the Software is furnished to do so,
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code that tailors the port to a specific RISC-V chip:
 *
 * + FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S contains the code that
 *   is common to all currently supported RISC-V chips.  There is only one
 *   portASM.S file because the same file is built for all RISC-V target chips.
 *
 * + Header files called freertos_risc_v_chip_specific_extensions.h contain the
 *   code that tailors the FreeRTOS kernel's RISC-V port to a specific RISC-V
 *   chip.  There are multiple freertos_risc_v_chip_specific_extensions.h files
 *   as there are multiple RISC-V chip implementations.
 *
 * !!!NOTE!!!
 * TAKE CARE TO INCLUDE THE CORRECT freertos_risc_v_chip_specific_extensions.h
 * HEADER FILE FOR THE CHIP IN USE.  This is done using the assembler's (not the
 * compiler's!) include path.  For example, if the chip in use includes a core
 * local interrupter (CLINT) and does not include any chip specific register
 * extensions then add the path below to the assembler's include path:
 * FreeRTOS\Source\portable\GCC\RISC-V-RV32\chip_specific_extensions\RV32I_CLINT_no_extensions
 *
 */

/*
 * This freertos_risc_v_chip_specific_extensions.h is for use with Andes V5 core
 * devices, developed and tested using the V5 platforms.
 */

#ifndef __FREERTOS_RISC_V_EXTENSIONS_H__
#define __FREERTOS_RISC_V_EXTENSIONS_H__

#if __riscv_flen == 64
	#define portFPWORD_SIZE 8
	#define fpstore_x fsd
	#define fpload_x fld
#elif __riscv_flen == 32
	#define portFPWORD_SIZE 4
	#define fpstore_x fsw
	#define fpload_x flw
#else
	#define portFPWORD_SIZE 0
#endif

#include "FreeRTOSConfig.h"

#define portasmHAS_MTIME 1

#define portasmHANDLE_INTERRUPT  mext_interrupt

#ifdef configUSE_CLIC
#define portasmHAS_CLIC  1
#else
#define portasmHAS_CLIC  0
#endif

/* Constants to define the additional registers. */
#define mxstatus 	0x7c4
#define ucode   	0x801

/* Additional FPU registers to save and restore (fcsr + 32 FPUs) */
#define portasmFPU_CONTEXT_SIZE        ( 1 + ( 32 * portFPWORD_SIZE ) / portWORD_SIZE )

/* One additional registers to save and restore, as per the #defines above. */
#define portasmADDITIONAL_CONTEXT_SIZE ( 2 + portasmFPU_CONTEXT_SIZE )  /* Must be even number on 32-bit cores. */

/* When the kernel update, TCB structure may be changed. So the offset need to be modified */
#if (configHSP_ENABLE==1)
	/* Set the bytes of stack's offset in TCB. Unit:byte */
	#define StackOffset_TCB			(12 * portWORD_SIZE) /* The offset of pxCurrentTCB->pxStack in TCB structure */

	/* Set the offset of top address of stack in TCB. Unit:byte */
	#define EndStackOffset_TCB		(13 * portWORD_SIZE + configMAX_TASK_NAME_LEN) /* The offset of pxCurrentTCB->pxEndOfStack in TCB structure */
#endif


/* Save additional registers found on the V5 core. */
.macro portasmSAVE_ADDITIONAL_REGISTERS
	addi sp, sp, -(portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE) /* Make room for the additional registers. */
	csrr t0, mxstatus							 /* Load additional registers into accessible temporary registers. */
	store_x t0, 1 * portWORD_SIZE( sp )

	#ifdef __riscv_dsp
		csrr t0, ucode
		store_x t0, 2 * portWORD_SIZE( sp )
	#endif

	#ifdef __riscv_flen
		frcsr t0
		fpstore_x f0, ( 3 * portWORD_SIZE + 0 * portFPWORD_SIZE )( sp )
		fpstore_x f1, ( 3 * portWORD_SIZE + 1 * portFPWORD_SIZE )( sp )
		fpstore_x f2, ( 3 * portWORD_SIZE + 2 * portFPWORD_SIZE )( sp )
		fpstore_x f3, ( 3 * portWORD_SIZE + 3 * portFPWORD_SIZE )( sp )
		fpstore_x f4, ( 3 * portWORD_SIZE + 4 * portFPWORD_SIZE )( sp )
		fpstore_x f5, ( 3 * portWORD_SIZE + 5 * portFPWORD_SIZE )( sp )
		fpstore_x f6, ( 3 * portWORD_SIZE + 6 * portFPWORD_SIZE )( sp )
		fpstore_x f7, ( 3 * portWORD_SIZE + 7 * portFPWORD_SIZE )( sp )
		fpstore_x f8, ( 3 * portWORD_SIZE + 8 * portFPWORD_SIZE )( sp )
		fpstore_x f9, ( 3 * portWORD_SIZE + 9 * portFPWORD_SIZE )( sp )
		fpstore_x f10, ( 3 * portWORD_SIZE + 10 * portFPWORD_SIZE )( sp )
		fpstore_x f11, ( 3 * portWORD_SIZE + 11 * portFPWORD_SIZE )( sp )
		fpstore_x f12, ( 3 * portWORD_SIZE + 12 * portFPWORD_SIZE )( sp )
		fpstore_x f13, ( 3 * portWORD_SIZE + 13 * portFPWORD_SIZE )( sp )
		fpstore_x f14, ( 3 * portWORD_SIZE + 14 * portFPWORD_SIZE )( sp )
		fpstore_x f15, ( 3 * portWORD_SIZE + 15 * portFPWORD_SIZE )( sp )
		fpstore_x f16, ( 3 * portWORD_SIZE + 16 * portFPWORD_SIZE )( sp )
		fpstore_x f17, ( 3 * portWORD_SIZE + 17 * portFPWORD_SIZE )( sp )
		fpstore_x f18, ( 3 * portWORD_SIZE + 18 * portFPWORD_SIZE )( sp )
		fpstore_x f19, ( 3 * portWORD_SIZE + 19 * portFPWORD_SIZE )( sp )
		fpstore_x f20, ( 3 * portWORD_SIZE + 20 * portFPWORD_SIZE )( sp )
		fpstore_x f21, ( 3 * portWORD_SIZE + 21 * portFPWORD_SIZE )( sp )
		fpstore_x f22, ( 3 * portWORD_SIZE + 22 * portFPWORD_SIZE )( sp )
		fpstore_x f23, ( 3 * portWORD_SIZE + 23 * portFPWORD_SIZE )( sp )
		fpstore_x f24, ( 3 * portWORD_SIZE + 24 * portFPWORD_SIZE )( sp )
		fpstore_x f25, ( 3 * portWORD_SIZE + 25 * portFPWORD_SIZE )( sp )
		fpstore_x f26, ( 3 * portWORD_SIZE + 26 * portFPWORD_SIZE )( sp )
		fpstore_x f27, ( 3 * portWORD_SIZE + 27 * portFPWORD_SIZE )( sp )
		fpstore_x f28, ( 3 * portWORD_SIZE + 28 * portFPWORD_SIZE )( sp )
		fpstore_x f29, ( 3 * portWORD_SIZE + 29 * portFPWORD_SIZE )( sp )
		fpstore_x f30, ( 3 * portWORD_SIZE + 30 * portFPWORD_SIZE )( sp )
		fpstore_x f31, ( 3 * portWORD_SIZE + 31 * portFPWORD_SIZE )( sp )
		sw t0, ( 3 * portWORD_SIZE + 32 * portFPWORD_SIZE )( sp )
	#endif

	/*
	 * If HSP mechanism is on. At the begining of trap handler, the mhsp_ctl should be turn off.
	 * Because the sp will be changed from task stack to interrupt stack.
	 */
	#if( configHSP_ENABLE == 1 )
		csrci mhsp_ctl, 3
	#endif
	.endm

/* Restore the additional registers found on the V5 core. */
.macro portasmRESTORE_ADDITIONAL_REGISTERS

	/*
	 * Set the msp_bound & msp_base to task's stack state
	 * and turn on the mhsp_ctl.
	 */
	#if( configHSP_ENABLE == 1 )
		/* Load current hardware stack protection and recording CSR */
		load_x t0, pxCurrentTCB

		load_x t1, StackOffset_TCB(t0)
		csrw msp_bound, t1

		#if( configRECORD_STACK_HIGH_ADDRESS == 1 )
			load_x t1, EndStackOffset_TCB(t0)
			csrw msp_base, t1
			li t0, 0x23
		#else
			li t0, 0x21
		#endif
		csrw mhsp_ctl, t0
	#endif

	load_x t0, 1 * portWORD_SIZE( sp )			/* Load additional registers into accessible temporary registers. */
	csrw mxstatus, t0

	#ifdef __riscv_dsp
		load_x t0, 2 * portWORD_SIZE( sp )
		csrw ucode, t0
	#endif

	#ifdef __riscv_flen
		lw t0, ( 3 * portWORD_SIZE + 32 * portFPWORD_SIZE )( sp )
		fpload_x f0, ( 3 * portWORD_SIZE + 0 * portFPWORD_SIZE )( sp )
		fpload_x f1, ( 3 * portWORD_SIZE + 1 * portFPWORD_SIZE )( sp )
		fpload_x f2, ( 3 * portWORD_SIZE + 2 * portFPWORD_SIZE )( sp )
		fpload_x f3, ( 3 * portWORD_SIZE + 3 * portFPWORD_SIZE )( sp )
		fpload_x f4, ( 3 * portWORD_SIZE + 4 * portFPWORD_SIZE )( sp )
		fpload_x f5, ( 3 * portWORD_SIZE + 5 * portFPWORD_SIZE )( sp )
		fpload_x f6, ( 3 * portWORD_SIZE + 6 * portFPWORD_SIZE )( sp )
		fpload_x f7, ( 3 * portWORD_SIZE + 7 * portFPWORD_SIZE )( sp )
		fpload_x f8, ( 3 * portWORD_SIZE + 8 * portFPWORD_SIZE )( sp )
		fpload_x f9, ( 3 * portWORD_SIZE + 9 * portFPWORD_SIZE )( sp )
		fpload_x f10, ( 3 * portWORD_SIZE + 10 * portFPWORD_SIZE )( sp )
		fpload_x f11, ( 3 * portWORD_SIZE + 11 * portFPWORD_SIZE )( sp )
		fpload_x f12, ( 3 * portWORD_SIZE + 12 * portFPWORD_SIZE )( sp )
		fpload_x f13, ( 3 * portWORD_SIZE + 13 * portFPWORD_SIZE )( sp )
		fpload_x f14, ( 3 * portWORD_SIZE + 14 * portFPWORD_SIZE )( sp )
		fpload_x f15, ( 3 * portWORD_SIZE + 15 * portFPWORD_SIZE )( sp )
		fpload_x f16, ( 3 * portWORD_SIZE + 16 * portFPWORD_SIZE )( sp )
		fpload_x f17, ( 3 * portWORD_SIZE + 17 * portFPWORD_SIZE )( sp )
		fpload_x f18, ( 3 * portWORD_SIZE + 18 * portFPWORD_SIZE )( sp )
		fpload_x f19, ( 3 * portWORD_SIZE + 19 * portFPWORD_SIZE )( sp )
		fpload_x f20, ( 3 * portWORD_SIZE + 20 * portFPWORD_SIZE )( sp )
		fpload_x f21, ( 3 * portWORD_SIZE + 21 * portFPWORD_SIZE )( sp )
		fpload_x f22, ( 3 * portWORD_SIZE + 22 * portFPWORD_SIZE )( sp )
		fpload_x f23, ( 3 * portWORD_SIZE + 23 * portFPWORD_SIZE )( sp )
		fpload_x f24, ( 3 * portWORD_SIZE + 24 * portFPWORD_SIZE )( sp )
		fpload_x f25, ( 3 * portWORD_SIZE + 25 * portFPWORD_SIZE )( sp )
		fpload_x f26, ( 3 * portWORD_SIZE + 26 * portFPWORD_SIZE )( sp )
		fpload_x f27, ( 3 * portWORD_SIZE + 27 * portFPWORD_SIZE )( sp )
		fpload_x f28, ( 3 * portWORD_SIZE + 28 * portFPWORD_SIZE )( sp )
		fpload_x f29, ( 3 * portWORD_SIZE + 29 * portFPWORD_SIZE )( sp )
		fpload_x f30, ( 3 * portWORD_SIZE + 30 * portFPWORD_SIZE )( sp )
		fpload_x f31, ( 3 * portWORD_SIZE + 31 * portFPWORD_SIZE )( sp )
		fscsr t0
	#endif

	addi sp, sp, (portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE )/* Remove space added for additional registers. */
	.endm

.macro portasmSWITCH_TO_ISRSTACK_HSP
	/*
	 * The HSP is disabled and stack pointer is switched to interrupt stack already when
	 * this macro is called. Now we can set the SP bound to _end and enable HSP mechanism before
	 * entering the C ISR.
	 */
	la t0, _end
	csrw msp_bound, t0			/* Set the SP bound to _end and enable HSP mechanism */

	#if( configRECORD_STACK_HIGH_ADDRESS == 1 )
		csrw msp_base, sp
		li t0, 0x23
	#else
		li t0, 0x21
	#endif

	csrw mhsp_ctl, t0
	.endm


#endif /* __FREERTOS_RISC_V_EXTENSIONS_H__ */
