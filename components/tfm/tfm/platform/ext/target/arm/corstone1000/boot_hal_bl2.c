/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "cmsis.h"
#include "region.h"
#include "boot_hal.h"
#include "Driver_Flash.h"
#include "flash_layout.h"
#include "bootutil/fault_injection_hardening.h"
#include "bootutil/bootutil_log.h"
#include "fip_parser.h"
#include "flash_map/flash_map.h"
#include "watchdog.h"
#include <string.h>
#include "tfm_plat_otp.h"
#include "tfm_plat_provisioning.h"
#include "fwu_agent.h"

#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
#include "psa_adac_platform.h"
#endif

#if defined(CRYPTO_HW_ACCELERATOR) || \
    defined(CRYPTO_HW_ACCELERATOR_OTP_PROVISIONING)
#include "crypto_hw.h"
#endif

/* Flash device name must be specified by target */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

REGION_DECLARE(Image$$, ER_DATA, $$Base)[];
REGION_DECLARE(Image$$, ARM_LIB_HEAP, $$ZI$$Limit)[];

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))
extern struct flash_area flash_map[];

int32_t fill_bl2_flash_map_by_parsing_fips(uint32_t bank_offset)
{
    int result;
    uint32_t tfa_offset = 0;
    uint32_t tfa_size = 0;

    /* parse directly from flash using XIP mode */
    /* FIP is large so its not a good idea to load it in memory */
    result = parse_fip_and_extract_tfa_info(bank_offset + FLASH_FIP_ADDRESS,
                  FLASH_FIP_SIZE,
                  &tfa_offset, &tfa_size);
    if (result != FIP_PARSER_SUCCESS) {
        BOOT_LOG_ERR("parse_fip_and_extract_tfa_info failed");
        return 1;
    }

    flash_map[2].fa_off = FLASH_FIP_OFFSET + tfa_offset;
    flash_map[2].fa_size = tfa_size;
    flash_map[3].fa_off = flash_map[2].fa_off + flash_map[2].fa_size;
    flash_map[3].fa_size = tfa_size;

    return 0;
}

#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
int psa_adac_to_tfm_apply_permissions(uint8_t permissions_mask[16])
{
    (void)permissions_mask;

    int ret;
    uint32_t dcu_reg_values[4];

    /* Below values provide same access as when platform is in development
       life cycle state */
    dcu_reg_values[0] = 0xffffe7fc;
    dcu_reg_values[1] = 0x800703ff;
    dcu_reg_values[2] = 0xffffffff;
    dcu_reg_values[3] = 0xffffffff;

    ret = crypto_hw_apply_debug_permissions((uint8_t*)dcu_reg_values, 16);
    BOOT_LOG_INF("%s: debug permission apply %s\n\r", __func__,
            (ret == 0) ? "success" : "fail");

    return ret;
}

uint8_t secure_debug_rotpk[32];

#endif

extern void add_bank_offset_to_image_offset(uint32_t bank_offset);

int32_t boot_platform_init(void)
{
    int32_t result;

    result = corstone1000_watchdog_init();
    if (result != ARM_DRIVER_OK) {
        return 1;
    }

    result = fill_bl2_flash_map_by_parsing_fips(BANK_0_PARTITION_OFFSET);
    if (result) {
        return 1;
    }

    result = FLASH_DEV_NAME.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }

    return 0;
}

int32_t boot_platform_post_init(void)
{
    int32_t result;
    uint32_t bank_offset;
    enum tfm_plat_err_t plat_err;

#ifdef CRYPTO_HW_ACCELERATOR
    result = crypto_hw_accelerator_init();
    if (result) {
        return 1;
    }
#endif /* CRYPTO_HW_ACCELERATOR */

    plat_err = tfm_plat_otp_init();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            BOOT_LOG_ERR("OTP system initialization failed");
            FIH_PANIC;
    }

#ifdef PLATFORM_PSA_ADAC_SECURE_DEBUG
    if (!tfm_plat_provisioning_is_required()) {

        plat_err = tfm_plat_otp_read(PLAT_OTP_ID_SECURE_DEBUG_PK, 32, secure_debug_rotpk);
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            return plat_err;
        }

        result = tfm_to_psa_adac_corstone1000_secure_debug(secure_debug_rotpk, 32);
        BOOT_LOG_INF("%s: dipda_secure_debug is a %s.\r\n", __func__,
                (result == 0) ? "success" : "failure");

    }
#endif

    bl2_get_boot_bank(&bank_offset);
    add_bank_offset_to_image_offset(bank_offset);

    return 0;
}

void boot_platform_quit(struct boot_arm_vector_table *vt)
{
    /* Clang at O0, stores variables on the stack with SP relative addressing.
     * When manually set the SP then the place of reset vector is lost.
     * Static variables are stored in 'data' or 'bss' section, change of SP has
     * no effect on them.
     */
    static struct boot_arm_vector_table *vt_cpy;
    int32_t result;

#ifdef CRYPTO_HW_ACCELERATOR
    result = crypto_hw_accelerator_finish();
    if (result) {
        while (1);
    }

    (void)fih_delay_init();
#endif /* CRYPTO_HW_ACCELERATOR */

    result = FLASH_DEV_NAME.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while (1);
    }

    vt_cpy = vt;

    __set_MSP(vt->msp);
    __DSB();
    __ISB();

    boot_jump_to_next_image(vt_cpy->reset);
}
