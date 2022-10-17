/*
 * Copyright (c) 2012-2019 Andes Technology Corporation
 * All rights reserved.
 *
 */
#include "core_v5.h"

/*
** uint32_t hb_read_data(uint32_t addr)
**a1 addr
*/
    .globl hb_read_data
hb_read_data:
    LOAD a0, 0(a1)
    ret


/*
** void hb_write_data_12w(uint32_t addr ,uint32_t data)
*STORE a1,0(a0)
*/
#ifndef TEMP
    .globl hb_write_data_12w
hb_write_data_12w:
	lw s4, 0(a1)
	lw s5, 4(a1)
	lw s6, 8(a1)
	lw s7, 12(a1)
	lw s8, 16(a1)
	lw s9, 20(a1)
	lw s10, 24(a1)
	lw a2, 28(a1)
	lw a3, 32(a1)
	lw a4, 36(a1)
	lw a5, 40(a1)
	lw a6, 44(a1)
    STORE s4, 0(a0)
    STORE s5, 4(a0)
    STORE s6, 8(a0)
    STORE s7, 12(a0)
    STORE s8, 16(a0)
	STORE s9, 20(a0)
	STORE s10, 24(a0)
	STORE a2, 28(a0)
	STORE a3, 32(a0)
	STORE a4, 36(a0)
	STORE a5, 40(a0)
	STORE a6, 44(a0)
    ret

#else
    .globl hb_write_data_16w
hb_write_data_16w:
	LOAD t1, 0(a1)
    STORE t1, 0(a0)
    LOAD t1, 4(a1)
    STORE t1, 4(a0)
    LOAD t1, 8(a1)
    STORE t1, 8(a0)
    LOAD t1, 12(a1)
    STORE t1, 12(a0)
    LOAD t1, 16(a1)
    STORE t1, 16(a0)
    LOAD t1, 20(a1)
	STORE t1, 20(a0)
	LOAD t1, 24(a1)
	STORE t1, 24(a0)
	LOAD t1, 28(a1)
	STORE t1, 28(a0)
	LOAD t1, 32(a1)
	STORE t1, 32(a0)
	LOAD t1, 36(a1)
	STORE t1, 36(a0)
	LOAD t1, 40(a1)
	STORE t1, 40(a0)
	LOAD t1, 44(a1)
	STORE t1, 44(a0)
	LOAD t1, 48(a1)
	STORE t1, 48(a0)
	LOAD t1, 52(a1)
	STORE t1, 52(a0)
	LOAD t1, 56(a1)
	STORE t1, 56(a0)
	LOAD t1, 60(a1)
	STORE t1, 60(a0)
    ret
#endif


/*
** void hb_write_data_8w(uint32_t addr ,uint32_t data)
*STORE a1,0(a0)
*/
    .globl hb_write_data_8w
hb_write_data_8w:
	lw s4, 0(a1)
	lw s5, 4(a1)
	lw s6, 8(a1)
	lw s7, 12(a1)
	lw s8, 16(a1)
	lw s9, 20(a1)
	lw s10, 24(a1)
	lw a2, 28(a1)
    STORE s4, 0(a0)
    STORE s5, 4(a0)
    STORE s6, 8(a0)
    STORE s7, 12(a0)
    STORE s8, 16(a0)
	STORE s9, 20(a0)
	STORE s10, 24(a0)
	STORE a2, 28(a0)
    ret

/*
** void hb_write_data(uint32_t addr ,uint32_t data)
*/
    .globl hb_write_data
hb_write_data:
	LOAD  t1,0(a1)
 	STORE t1,0(a0)
    ret

//	LOAD s2, 28(a1)
//	STORE s2, 28(a0)


