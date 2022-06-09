/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bootutil/security_cnt.h"
#include "tfm_plat_nv_counters.h"
#include "tfm_plat_defs.h"
#include "bootutil/fault_injection_hardening.h"
#include <stdint.h>
#include "tfm_plat_provisioning.h"
#include "fwu_agent.h"

#define TFM_BOOT_NV_COUNTER_0    PLAT_NV_COUNTER_BL2_0   /* NV counter of Image 0 */
#define TFM_BOOT_NV_COUNTER_1    PLAT_NV_COUNTER_BL2_1   /* NV counter of Image 1 */
#define TFM_BOOT_NV_COUNTER_2    PLAT_NV_COUNTER_BL2_2   /* NV counter of Image 2 */
#define TFM_BOOT_NV_COUNTER_MAX  PLAT_NV_COUNTER_BL2_2 + 1

static enum tfm_nv_counter_t get_nv_counter_from_image_id(uint32_t image_id)
{
    uint32_t nv_counter;

    /* Avoid integer overflow */
    if ((UINT32_MAX - TFM_BOOT_NV_COUNTER_0) < image_id) {
        return TFM_BOOT_NV_COUNTER_MAX;
    }

    nv_counter = TFM_BOOT_NV_COUNTER_0 + image_id;

    /* Check the existence of the enumerated counter value */
    if (nv_counter >= TFM_BOOT_NV_COUNTER_MAX) {
        return TFM_BOOT_NV_COUNTER_MAX;
    }

    return (enum tfm_nv_counter_t)nv_counter;
}

fih_int boot_nv_security_counter_init(void)
{
    fih_int fih_rc = FIH_FAILURE;

    fih_rc = fih_int_encode_zero_equality(tfm_plat_init_nv_counter());

    FIH_RET(fih_rc);
}

fih_int boot_nv_security_counter_get(uint32_t image_id, fih_int *security_cnt)
{
    enum tfm_nv_counter_t nv_counter;
    fih_int fih_rc = FIH_FAILURE;
    uint32_t security_cnt_soft;

    /* Check if it's a null-pointer. */
    if (!security_cnt) {
        FIH_RET(FIH_FAILURE);
    }

    nv_counter = get_nv_counter_from_image_id(image_id);
    if (nv_counter >= TFM_BOOT_NV_COUNTER_MAX) {
        FIH_RET(FIH_FAILURE);
    }

    fih_rc = fih_int_encode_zero_equality(
             tfm_plat_read_nv_counter(nv_counter,
                                      sizeof(security_cnt_soft),
                                      (uint8_t *)&security_cnt_soft));
    *security_cnt = fih_int_encode(security_cnt_soft);

    FIH_RET(fih_rc);
}

int32_t boot_nv_security_counter_update(uint32_t image_id,
                                        uint32_t img_security_cnt)
{
    enum tfm_nv_counter_t nv_counter;
    enum tfm_plat_err_t err;
    enum fwu_agent_error_t fwu_err;

    nv_counter = get_nv_counter_from_image_id(image_id);
    if (nv_counter >= TFM_BOOT_NV_COUNTER_MAX) {
        return -1;
    }

    if (tfm_plat_provisioning_is_required()) {

        err = tfm_plat_set_nv_counter(nv_counter, img_security_cnt);
        if (err != TFM_PLAT_ERR_SUCCESS) {
            return -1;
        }

    } else {

        if (nv_counter == PLAT_NV_COUNTER_BL2_0) {
            fwu_err = fwu_stage_nv_counter(FWU_TFM_NV_COUNTER, img_security_cnt);
        } else if (nv_counter == PLAT_NV_COUNTER_BL2_1) {
            fwu_err = fwu_stage_nv_counter(FWU_TFA_NV_COUNTER, img_security_cnt);
        } else {
            return -1;
        }

        if (fwu_err != FWU_AGENT_SUCCESS) {
            return -1;
        }

    }

    return 0;
}
