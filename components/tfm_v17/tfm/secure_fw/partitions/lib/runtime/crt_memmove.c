/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "crt_impl_private.h"

static void *memcpy_r(void *dest, const void *src, size_t n)
{
    union composite_addr_t p_dst, p_src;

    p_dst.uint_addr = (uintptr_t)dest + n;
    p_src.uint_addr = (uintptr_t)src  + n;

    /* Byte copy for unaligned address. check the last bit of address. */
    while (n && (ADDR_WORD_UNALIGNED(p_dst.uint_addr) ||
                 ADDR_WORD_UNALIGNED(p_src.uint_addr))) {
        *(--p_dst.p_byte) = *(--p_src.p_byte);
        n--;
    }

    /* Quad byte copy for aligned address. */
    while (n >= sizeof(uint32_t)) {
        *(--p_dst.p_word) = *(--p_src.p_word);
        n -= sizeof(uint32_t);
    }

    /* Byte copy for the remaining bytes. */
    while (n--) {
        *(--p_dst.p_byte) = *(--p_src.p_byte);
    }

    return dest;
}

/* Mind the direction for copying in memcpy! */
#define memcpy_f memcpy

/*
 * For overlapped memory area:
 * 1) overlapped: use reverse memory move.
 * 2) non-overlapped: use forward memory move.
 */
void *memmove(void *dest, const void *src, size_t n)
{
    if (src >= dest) {
        return memcpy_f(dest, src, n);
    } else {
        return memcpy_r(dest, src, n);
    }
}
