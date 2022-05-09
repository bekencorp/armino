/*
 * Copyright (c) 2012-2017 Andes Technology Corporation
 * All rights reserved.
 *
 */
#include "core_v5.h"

	.section .nds_vector, "ax"

.extern liteos_m_risc_v_trap_handler
.extern HalTrapVector
.option push
.option norelax
	.global reset_vector

reset_vector:
	/* Decide whether this is an NMI or cold reset */
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
	la t0, HalTrapVector
	csrw mtvec, t0


#ifdef CFG_CACHE_ENABLE
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
	csrr a0, mcause
	call user_nmi_handler
	li t0, TRAP_M_USER_NP
	csrw mcause, t0
	j HalTrapVector
	ret

.org  0x100
.word 0x32374B42
.word 0x00003635
.option pop
	.section .text

	.global trap_entry
	.align 2

trap_entry:
	li t0, TRAP_M_USER_ASSERT
	csrw mcause, t0
	j HalTrapVector
	ret


