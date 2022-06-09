/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MPU_CONFIG_H__
#define __MPU_CONFIG_H__

#include "cmsis.h"
#include "region.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEX                     0b000U
#define NOT_SHAREABLE           0b0U
#define YES_CACHEABLE           0b1U
#define NOT_CACHEABLE           0b0U
#define NOT_BUFFERABLE          0b0U
#define SUB_REGION_DISABLE      0b0U

#define XN_EXEC_OK              0b0U
#define XN_EXEC_NOT_OK          0b1U

#define AP_RW_PRIV_ONLY         0b001U
#define AP_RW_PRIV_UNPRIV       0b011U
#define AP_RO_PRIV_ONLY         0b101U
#define AP_RO_PRIV_UNPRIV       0b110U
#define AP_RW_PRIV_R_UNPRIV     0b010U

#define FIREWALL_REGION_NUMBER 0
#define TFM_ISOLATION_REGION_START_NUMBER (FIREWALL_REGION_NUMBER + 1)

static inline int is_power_two(uint32_t x)
{
    return ((x & (x - 1)) == 0);
}

/* convert size based on the register requirement */
static uint32_t get_rbar_size_field(uint32_t size)
{
    /* MPU requirement:
       The associated region size, in bytes, is 2 ^ (SIZE+1).
     */
    int32_t count = 0; /* count number of bits till msb */
    int flag_two_power;

    if (size == 0 || size == 1) {
        return 0;
    }

    flag_two_power = is_power_two(size);

    while(size) {
        size >>= 1;
        count++;
    }

    if (flag_two_power) {
        count -= 1;
    }

    count--; /* 2 ^ (SIZE+1) */
    return count;
}

static inline void arm_mpu_enable(void)
{
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
}

#ifdef __cplusplus
}
#endif

#endif /* __MPU_CONFIG_H__ */
