/*
 * Copyright (c) 2021-2022 Beken Corporation
 * All rights reserved.
 *
 */

#include "core_v5.h"

#define MTIMER                     (0xE6000000)
#define MTIMERCMP                  (0xE6000008)

.global  arch_disable_irq
.global  arch_enable_irq
.global  arch_int_lock
.global  arch_int_unlock
.global  arch_int_restore
.global  arch_fence
.global  arch_atomic_clear
.global  arch_atomic_set

.global  riscv_get_mtimer
.global  riscv_set_mtimercmp
.global  riscv_get_cycle
.global  riscv_get_instruct_cnt

.section .text

/*
 * void arch_disable_irq(void);
 */
arch_disable_irq:
    li      t0, (MSTATUS_MPIE | MSTATUS_MIE) // mpie | mie
    csrrc   x0, mstatus, t0
    ret

/*
 * void arch_enable_irq(void);
 */
arch_enable_irq:
    csrsi  mstatus, MSTATUS_MIE
    ret

/*
 * u32 arch_int_lock(void);
 */
arch_int_lock:
    csrr    a0, mstatus           // return value
	csrci	mstatus, MSTATUS_MIE
    ret

/*
 * u32 arch_int_unlock(void);
 */
arch_int_unlock:
    csrr    a0, mstatus           // return value
	csrsi  mstatus, MSTATUS_MIE
    ret

/*
 * void arch_int_restore(u32 int_flag);
 */
arch_int_restore:
    csrw mstatus, a0
    ret

/*
 * void arch_fence(void);
 */
arch_fence:
	fence iorw, iorw
	ret

/*
 * void arch_atomic_clear(u32 * lock_addr);
 */
arch_atomic_clear:
	amoswap.w.rl x0, x0, (a0)
	ret

/*
 * u32 arch_atomic_set(u32 * lock_addr);
 */
arch_atomic_set:
	addi t0, x0, 1
//swap_again: 
	amoswap.w.aq t0, t0, (a0)
	c.mv  a0, t0
//	bnez t0, swap_again
	ret

/*
 * u64 riscv_get_mtimer(void);
 */
riscv_get_mtimer:
	li t0, MTIMER
read_mtimer: 
	lw a1, 4(t0)
	lw a0, 0(t0)
	lw t1, 4(t0)
	bne a1, t1, read_mtimer
	ret

/*
 * void riscv_set_mtimercmp(u64 new_time);
 */
riscv_set_mtimercmp:
	li t0, MTIMERCMP
	li t1, -1
	sw t1, 4(t0)
	fence
	sw a0, 0(t0)
	sw a1, 4(t0)
	ret

/*
 * u64 riscv_get_cycle(void);
 */
riscv_get_cycle:
read_cycle: 
	csrrs a1, mcycleh, x0
	csrrs a0, mcycle, x0
	csrrs t0, mcycleh, x0
	bne a1, t0, read_cycle
	ret

/*
 * u64 riscv_get_instruct_cnt(void);
 */
riscv_get_instruct_cnt:
read_instr_cnt: 
	csrrs a1, minstreth, x0
	csrrs a0, minstret, x0
	csrrs t0, minstreth, x0
	bne a1, t0, read_instr_cnt
	ret

