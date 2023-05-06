/*
 * Copyright (c) 2012-2017 Andes Technology Corporation
 * All rights reserved.
 *
 */
#include "core_v5.h"

	.section .nds_vector, "ax"

.option push
.option norelax
	.global reset_vector

reset_vector:
	/* Decide whether this is an NMI or cold reset */
	csrw mscratch, t0
	csrr t0, mcause
	bnez t0, nmi_handler
	.global _start
	.type _start,@function

_start:
#ifndef CACHE_DISABLE
   /* enable icache   */
    csrr	a5,mcache_ctl
     ori	a5,a5,1
    csrw	mcache_ctl,a5
   /* enable dcache */
    csrr	a5,mcache_ctl
     ori	a5,a5,2
    csrw	mcache_ctl,a5
#endif
	/* Initialize global pointer */
	.option push
	.option norelax
	la gp, __global_pointer$
	.option pop

	/* Initialize stack pointer */
	la t0, _stack
	mv sp, t0

#ifdef __nds_execit
	/* Initialize EXEC.IT table */
	la t0, _ITB_BASE_
	csrw uitb, t0
#endif

#ifdef __riscv_flen
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrrs t0, mstatus, t0

	/* Initialize FCSR */
	fscsr zero
#endif

	/* Initial machine trap-vector Base. Use FreeRTOS trap function. */
    #ifdef INT_VECTOR_EN
	    la t0, __vectors
	#else
	    la t0, freertos_risc_v_trap_handler
    #endif
	csrw mtvec, t0

	/* Enable vectored external PLIC interrupt */
	#ifdef INT_VECTOR_EN
	    csrsi mmisc_ctl, 2
	#endif

#ifndef CACHE_DISABLE
	/* Enable I/D cache with HW prefetcher and D-cache write-around (threshold: 4 cache lines) */
	li t0, (0x3 << 13)
	csrc mcache_ctl, t0
	li t0, (1 << 13) | (1 << 10) | (1 << 9) | (0x3)
	csrs mcache_ctl, t0
#endif

	/* Do system low level setup. It must be a leaf function */
	call __platform_init

	/* System reset handler */
	call reset_handler

	/* Infinite loop, if returned accidently */
1:	j 1b

	.weak __platform_init
__platform_init:
	ret

	.weak nmi_handler
nmi_handler:
	csrrw t0, mscratch, x0
	addi sp, sp, -64
//	sw ra, 0(sp)
	
	sw x5, 4(sp)			
	sw x6, 8(sp)
	sw x7, 12(sp)

	sw x10, 16(sp)
	sw x11, 20(sp)
	sw x12, 24(sp)
	sw x13, 28(sp)
	sw x14, 32(sp)
	sw x15, 36(sp)
	sw x16, 40(sp)
	sw x17, 44(sp)
	sw x28, 48(sp)
	sw x29, 52(sp)
	sw x30, 56(sp)
	sw x31, 60(sp)

	csrrw a0, mcause, x0
	mv  a1, ra
	call user_nmi_handler
	li a0, TRAP_M_USER_NP_WDT
	csrw mcause, a0

//	lw ra, 0(sp)
	lw x5, 4(sp)			
	lw x6, 8(sp)
	lw x7, 12(sp)

	lw x10, 16(sp)
	lw x11, 20(sp)
	lw x12, 24(sp)
	lw x13, 28(sp)
	lw x14, 32(sp)
	lw x15, 36(sp)
	lw x16, 40(sp)
	lw x17, 44(sp)
	lw x28, 48(sp)
	lw x29, 52(sp)
	lw x30, 56(sp)
	lw x31, 60(sp)

	addi sp, sp, 64

	j freertos_risc_v_trap_handler

.org  0x100
.word 0x32374B42
.word 0x00003635
.word 0x00000000
.word 0x00000000
.org  0x110
.word 0x64616568    /*head: sign head address*/
.word 0x00000000
.word 0x00000000
.word 0x00000000
.org  0x120
.word 0x00000000    /*version: provide to version number setting*/
.word 0x00000000
.word 0x00000000
.word 0x00000000
.option pop
	.section .text

	.global trap_entry
	.align 2

trap_entry:
	li t0, TRAP_M_USER_ASSERT
	csrw mcause, t0
	j freertos_risc_v_trap_handler
	ret

