/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_OTP_DEFS_H
#define CC3XX_OTP_DEFS_H

#include "cc3xx_config.h"
#include "cmsis_compiler.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CC3XX_OTP_BASE
#define CC3XX_OTP_BASE (CC_BASE + 0x2000)
#endif /* !CC_OTP_BASE */

#ifndef CC3XX_OTP_GENERAL_PURPOSE_CUSTOM_VIEW
#define CC3XX_OTP_GENERAL_PURPOSE_CUSTOM_VIEW __PACKED_UNION {};
#endif /* CC3XX_OTP_GENERAL_PURPOSE_CUSTOM_VIEW */

typedef __PACKED_STRUCT {
    uint32_t huk[0x8];
    uint32_t icv_provisioning_key[0x4];
    uint32_t icv_code_encryption_key[0x4];
    uint32_t icv_programmed_flags[0x1];
    __PACKED_UNION {
        uint32_t rot_public_key[0x8];
        __PACKED_STRUCT {
            uint32_t icv_rot_public_key[0x4];
            uint32_t oem_rot_public_key[0x4];
        };
    };
    uint32_t oem_provisioning_secret[0x4];
    uint32_t oem_code_encryption_key[0x4];
    uint32_t oem_programmed_flags[0x1];
    __PACKED_UNION {
        uint32_t nv_counter[0x5];
        __PACKED_STRUCT {
            uint32_t icv_nv_counter[0x2];
            uint32_t oem_nv_counter[0x3];
        };
    };
    uint32_t general_purpose_configuration_flags[0x4];
    uint32_t dcu_debug_lock_mask[0x4];
    __PACKED_UNION {
        uint32_t general_purpose_code_data[0x7d4];
        CC3XX_OTP_GENERAL_PURPOSE_CUSTOM_VIEW;
    };
} cc3xx_otp_memory_t;

#define CC3XX_OTP ((volatile cc3xx_otp_memory_t *) CC3XX_OTP_BASE)

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_OTP_DEFS_H */
