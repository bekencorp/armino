/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SECURITY_DEFS_H__
#define __SECURITY_DEFS_H__

/* Generic security-related definitions */

/*
 * Stack SEAL is involved since Security Extension exists, it becomes
 * a generic security defs used by both SPM and Partitions.
 */
#define STACK_SEAL_PATTERN    0xFEF5EDA5

#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3))

/* Attributes for psa api secure gateway functions */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
/*
 * GNUARM requires noclone attribute to protect gateway function symbol from
 * being renamed and cloned
 */
#define __tz_c_veneer \
        __attribute__((cmse_nonsecure_entry, noclone, section("SFN")))
#define __tz_naked_veneer \
        __attribute__((cmse_nonsecure_entry, noclone, naked, section("SFN")))

#else /* __GNUC__ && !__ARMCC_VERSION */

#define __tz_c_veneer \
        __attribute__((cmse_nonsecure_entry, section("SFN")))
#define __tz_naked_veneer \
        __attribute__((cmse_nonsecure_entry, naked, section("SFN")))

#endif /* __GNUC__ && !__ARMCC_VERSION */

#endif /* __ARM_FEATURE_CMSE */

#endif /* __SECURITY_DEFS_H__ */
