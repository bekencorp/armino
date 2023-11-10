/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* Interop between TF-M fih.h and mcuboot fault_injection_hardening.h, so that
 * platform code can target fih.h and for bl2 this will be redirected to
 * fault_injection_hardening.h
 */

#ifndef __INTEROP_FIH_H__
#define __INTEROP_FIH_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stdint.h"

/*
 * FIH return type macro changes the function return types to fih_int.
 * All functions that need to be protected by FIH and called via FIH_CALL must
 * return a fih_int type.
 */
#define FIH_RET_TYPE(type)    fih_int

#include "bootutil/fault_injection_hardening.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTEROP_FIH_H__ */
