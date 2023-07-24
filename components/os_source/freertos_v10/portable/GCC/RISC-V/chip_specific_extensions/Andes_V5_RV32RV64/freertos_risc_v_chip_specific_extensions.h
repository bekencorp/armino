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

#define portasmFPU_CONTEXT_SIZE        ( ( 32 * portFPWORD_SIZE ) / portWORD_SIZE )

#include "FreeRTOSConfig.h"

#define portasmHANDLE_INTERRUPT  mext_interrupt

#define HW_STACK_PROTECT

/* Constants to define the additional registers. */
#define ucode   	0x801

/* Additional FPU registers to save and restore (fcsr + 32 FPUs) */
/* One additional registers to save and restore, as per the #defines above. */
#define portasmADDITIONAL_CONTEXT_SIZE ( 2 + portasmFPU_CONTEXT_SIZE )  /* Must be even number on 32-bit cores. */

#ifdef __ASSEMBLER__

/* Save additional registers found on the V5 core. */
.macro portasmSAVE_ADDITIONAL_REGISTERS

	addi sp, sp, -(portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE) /* Make room for the additional registers. */

	#ifdef __riscv_dsp
		csrr t0, ucode
		store_x t0, 0 * portWORD_SIZE( sp )
	#endif

	#ifdef __riscv_flen
		frcsr t0
		sw t0, 1 * portWORD_SIZE( sp )
		fpstore_x f0, ( 2 * portWORD_SIZE + 0 * portFPWORD_SIZE )( sp )
		fpstore_x f1, ( 2 * portWORD_SIZE + 1 * portFPWORD_SIZE )( sp )
		fpstore_x f2, ( 2 * portWORD_SIZE + 2 * portFPWORD_SIZE )( sp )
		fpstore_x f3, ( 2 * portWORD_SIZE + 3 * portFPWORD_SIZE )( sp )
		fpstore_x f4, ( 2 * portWORD_SIZE + 4 * portFPWORD_SIZE )( sp )
		fpstore_x f5, ( 2 * portWORD_SIZE + 5 * portFPWORD_SIZE )( sp )
		fpstore_x f6, ( 2 * portWORD_SIZE + 6 * portFPWORD_SIZE )( sp )
		fpstore_x f7, ( 2 * portWORD_SIZE + 7 * portFPWORD_SIZE )( sp )
		fpstore_x f8, ( 2 * portWORD_SIZE + 8 * portFPWORD_SIZE )( sp )
		fpstore_x f9, ( 2 * portWORD_SIZE + 9 * portFPWORD_SIZE )( sp )
		fpstore_x f10, ( 2 * portWORD_SIZE + 10 * portFPWORD_SIZE )( sp )
		fpstore_x f11, ( 2 * portWORD_SIZE + 11 * portFPWORD_SIZE )( sp )
		fpstore_x f12, ( 2 * portWORD_SIZE + 12 * portFPWORD_SIZE )( sp )
		fpstore_x f13, ( 2 * portWORD_SIZE + 13 * portFPWORD_SIZE )( sp )
		fpstore_x f14, ( 2 * portWORD_SIZE + 14 * portFPWORD_SIZE )( sp )
		fpstore_x f15, ( 2 * portWORD_SIZE + 15 * portFPWORD_SIZE )( sp )
		fpstore_x f16, ( 2 * portWORD_SIZE + 16 * portFPWORD_SIZE )( sp )
		fpstore_x f17, ( 2 * portWORD_SIZE + 17 * portFPWORD_SIZE )( sp )
		fpstore_x f18, ( 2 * portWORD_SIZE + 18 * portFPWORD_SIZE )( sp )
		fpstore_x f19, ( 2 * portWORD_SIZE + 19 * portFPWORD_SIZE )( sp )
		fpstore_x f20, ( 2 * portWORD_SIZE + 20 * portFPWORD_SIZE )( sp )
		fpstore_x f21, ( 2 * portWORD_SIZE + 21 * portFPWORD_SIZE )( sp )
		fpstore_x f22, ( 2 * portWORD_SIZE + 22 * portFPWORD_SIZE )( sp )
		fpstore_x f23, ( 2 * portWORD_SIZE + 23 * portFPWORD_SIZE )( sp )
		fpstore_x f24, ( 2 * portWORD_SIZE + 24 * portFPWORD_SIZE )( sp )
		fpstore_x f25, ( 2 * portWORD_SIZE + 25 * portFPWORD_SIZE )( sp )
		fpstore_x f26, ( 2 * portWORD_SIZE + 26 * portFPWORD_SIZE )( sp )
		fpstore_x f27, ( 2 * portWORD_SIZE + 27 * portFPWORD_SIZE )( sp )
		fpstore_x f28, ( 2 * portWORD_SIZE + 28 * portFPWORD_SIZE )( sp )
		fpstore_x f29, ( 2 * portWORD_SIZE + 29 * portFPWORD_SIZE )( sp )
		fpstore_x f30, ( 2 * portWORD_SIZE + 30 * portFPWORD_SIZE )( sp )
		fpstore_x f31, ( 2 * portWORD_SIZE + 31 * portFPWORD_SIZE )( sp )
	#endif

	.endm

/* Restore the additional registers found on the V5 core. */
.macro portasmRESTORE_ADDITIONAL_REGISTERS

	#ifdef __riscv_dsp
		load_x t0, 0 * portWORD_SIZE( sp )
		csrw ucode, t0
	#endif

	#ifdef __riscv_flen
		fpload_x f0, ( 2 * portWORD_SIZE + 0 * portFPWORD_SIZE )( sp )
		fpload_x f1, ( 2 * portWORD_SIZE + 1 * portFPWORD_SIZE )( sp )
		fpload_x f2, ( 2 * portWORD_SIZE + 2 * portFPWORD_SIZE )( sp )
		fpload_x f3, ( 2 * portWORD_SIZE + 3 * portFPWORD_SIZE )( sp )
		fpload_x f4, ( 2 * portWORD_SIZE + 4 * portFPWORD_SIZE )( sp )
		fpload_x f5, ( 2 * portWORD_SIZE + 5 * portFPWORD_SIZE )( sp )
		fpload_x f6, ( 2 * portWORD_SIZE + 6 * portFPWORD_SIZE )( sp )
		fpload_x f7, ( 2 * portWORD_SIZE + 7 * portFPWORD_SIZE )( sp )
		fpload_x f8, ( 2 * portWORD_SIZE + 8 * portFPWORD_SIZE )( sp )
		fpload_x f9, ( 2 * portWORD_SIZE + 9 * portFPWORD_SIZE )( sp )
		fpload_x f10, ( 2 * portWORD_SIZE + 10 * portFPWORD_SIZE )( sp )
		fpload_x f11, ( 2 * portWORD_SIZE + 11 * portFPWORD_SIZE )( sp )
		fpload_x f12, ( 2 * portWORD_SIZE + 12 * portFPWORD_SIZE )( sp )
		fpload_x f13, ( 2 * portWORD_SIZE + 13 * portFPWORD_SIZE )( sp )
		fpload_x f14, ( 2 * portWORD_SIZE + 14 * portFPWORD_SIZE )( sp )
		fpload_x f15, ( 2 * portWORD_SIZE + 15 * portFPWORD_SIZE )( sp )
		fpload_x f16, ( 2 * portWORD_SIZE + 16 * portFPWORD_SIZE )( sp )
		fpload_x f17, ( 2 * portWORD_SIZE + 17 * portFPWORD_SIZE )( sp )
		fpload_x f18, ( 2 * portWORD_SIZE + 18 * portFPWORD_SIZE )( sp )
		fpload_x f19, ( 2 * portWORD_SIZE + 19 * portFPWORD_SIZE )( sp )
		fpload_x f20, ( 2 * portWORD_SIZE + 20 * portFPWORD_SIZE )( sp )
		fpload_x f21, ( 2 * portWORD_SIZE + 21 * portFPWORD_SIZE )( sp )
		fpload_x f22, ( 2 * portWORD_SIZE + 22 * portFPWORD_SIZE )( sp )
		fpload_x f23, ( 2 * portWORD_SIZE + 23 * portFPWORD_SIZE )( sp )
		fpload_x f24, ( 2 * portWORD_SIZE + 24 * portFPWORD_SIZE )( sp )
		fpload_x f25, ( 2 * portWORD_SIZE + 25 * portFPWORD_SIZE )( sp )
		fpload_x f26, ( 2 * portWORD_SIZE + 26 * portFPWORD_SIZE )( sp )
		fpload_x f27, ( 2 * portWORD_SIZE + 27 * portFPWORD_SIZE )( sp )
		fpload_x f28, ( 2 * portWORD_SIZE + 28 * portFPWORD_SIZE )( sp )
		fpload_x f29, ( 2 * portWORD_SIZE + 29 * portFPWORD_SIZE )( sp )
		fpload_x f30, ( 2 * portWORD_SIZE + 30 * portFPWORD_SIZE )( sp )
		fpload_x f31, ( 2 * portWORD_SIZE + 31 * portFPWORD_SIZE )( sp )
		lw t0,  1 * portWORD_SIZE( sp )
		fscsr t0
	#endif

	addi sp, sp, (portasmADDITIONAL_CONTEXT_SIZE * portWORD_SIZE )/* Remove space added for additional registers. */

	.endm

#endif /* __ASSEMBLER__ */
#endif /* __FREERTOS_RISC_V_EXTENSIONS_H__ */
