/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MMIO_DEFS_H__
#define __MMIO_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Boundary handle binding macros. */
#define HANDLE_PER_ATTR_BITS            (0x4)
#define HANDLE_ATTR_PRIV_MASK           ((1 << HANDLE_PER_ATTR_BITS) - 1)

#ifdef __cplusplus
}
#endif

#endif /* __MMIO_DEFS_H__ */
