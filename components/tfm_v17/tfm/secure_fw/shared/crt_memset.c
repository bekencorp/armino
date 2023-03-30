/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crt_impl_private.h"

void *memset(void *s, int c, size_t n)
{
    union composite_addr_t p_mem;
    uint32_t pattern_word;

    p_mem.p_byte = (uint8_t *)s;
    pattern_word = (((uint8_t)c) << 24) | (((uint8_t)c) << 16) |
                   (((uint8_t)c) << 8) | ((uint8_t)c);

    while (n && ADDR_WORD_UNALIGNED(p_mem.uint_addr)) {
        *p_mem.p_byte++ = (uint8_t)c;
        n--;
    }

    while (n >= sizeof(uint32_t)) {
        *p_mem.p_word++ = pattern_word;
        n -= sizeof(uint32_t);
    }

    while (n--) {
        *p_mem.p_byte++ = (uint8_t)c;
    }

    return s;
}
