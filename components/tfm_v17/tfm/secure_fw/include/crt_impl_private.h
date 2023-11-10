/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CRT_IMPL_PRIVATE_H__
#define __CRT_IMPL_PRIVATE_H__

#include <stddef.h>
#include <stdint.h>

#define ADDR_WORD_UNALIGNED(x)        ((x) & 0x3)

union composite_addr_t {
    uintptr_t uint_addr;        /* Address as integer value  */
    uint8_t   *p_byte;          /* Address in BYTE pointer   */
    uint32_t  *p_word;          /* Address in WORD pointer   */
};

#endif /* __CRT_IMPL_PRIVATE_H__ */
