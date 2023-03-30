/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_UTILS_H__
#define __TFM_UTILS_H__

#include <stdbool.h>
#include <string.h>
#include "tfm_spm_log.h"

/*
 * CPU spin here.
 * Note: this function is used to handle PROGRAMMER ERROR.
 */
void tfm_core_panic(void);

/* SPM assert */
#ifndef NDEBUG
#define SPM_ASSERT(cond)                                                    \
            do {                                                            \
                if (!(cond)) {                                              \
                    SPMLOG_INFMSG("Assert:");                               \
                    SPMLOG_INFMSG(__func__);                                \
                    SPMLOG_INFMSGVAL(",", __LINE__);                        \
                    while (1)                                               \
                        ;                                                   \
                }                                                           \
            } while (0)
#else
#define SPM_ASSERT(cond)
#endif

/* Get container structure start address from member */
#define TO_CONTAINER(ptr, type, member) \
    (type *)((unsigned long)(ptr) - offsetof(type, member))

/* FixMe: Replace ERROR_MSG() in platform code with a suitable API */
#define ERROR_MSG(msg) SPMLOG_ERRMSG(msg "\r\n")

/* Stringify preprocessors, no leading underscore. ('STRINGIFY') */
#define STRINGIFY_EXPAND(x) #x
#define M2S(m) STRINGIFY_EXPAND(m)

/* Runtime memory operations forwarding */
#ifndef spm_memcpy
#define spm_memcpy memcpy
#endif /* spm_memcpy */

#ifndef spm_memset
#define spm_memset memset
#endif /* spm_memset */

#endif /* __TFM_UTILS_H__ */
