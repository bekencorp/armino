/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "target_cfg.h"
#include "region.h"
#include "cmsis.h"
#include "boot_hal.h"
#include "Driver_Flash.h"
#include "flash_layout.h"
#ifdef CRYPTO_HW_ACCELERATOR
#include "crypto_hw.h"
#include "fih.h"
#endif /* CRYPTO_HW_ACCELERATOR */

#ifdef MEASURED_BOOT_API
#include "region_defs.h"
#include "tfm_boot_status.h"
#include "boot_measurement.h"
#endif /* MEASURED_BOOT_API */

/* Flash device names must be specified by target */
#ifdef FLASH_DEV_NAME
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;
#endif /* FLASH_DEV_NAME */
#ifdef FLASH_DEV_NAME_2
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_2;
#endif /* FLASH_DEV_NAME_2 */
#ifdef FLASH_DEV_NAME_3
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_3;
#endif /* FLASH_DEV_NAME_3 */
#ifdef FLASH_DEV_NAME_SCRATCH
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_SCRATCH;
#endif /* FLASH_DEV_NAME_SCRATCH */

REGION_DECLARE(Image$$, ER_DATA, $$Base)[];
REGION_DECLARE(Image$$, ARM_LIB_HEAP, $$ZI$$Limit)[];
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
 || defined(__ARM_ARCH_8_1M_MAIN__)
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
       || defined(__ARM_ARCH_8_1M_MAIN__) */
#if defined(__ICCARM__)
#pragma required = ER_DATA$$Base
#pragma required = ARM_LIB_HEAP$$Limit
#endif

__WEAK __attribute__((naked)) void boot_clear_ram_area(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                             \n"
#endif
        "movs    r0, #0                              \n"
#if !defined(__ICCARM__)
        "ldr     r1, =Image$$ER_DATA$$Base           \n"
        "ldr     r2, =Image$$ARM_LIB_HEAP$$ZI$$Limit \n"
#else
        "ldr     r1, =ER_DATA$$Base                  \n"
        "ldr     r2, =ARM_LIB_HEAP$$Limit            \n"
#endif
        "subs    r2, r2, r1                          \n"
        "Loop:                                       \n"
        "subs    r2, #4                              \n"
        "blt     Clear_done                          \n"
        "str     r0, [r1, r2]                        \n"
        "b       Loop                                \n"
        "Clear_done:                                 \n"
        "bx      lr                                  \n"
         : : : "r0" , "r1" , "r2" , "memory"
    );
}

/*!
 * \brief Chain-loading the next image in the boot sequence.
 *
 * This function calls the Reset_Handler of the next image in the boot sequence,
 * usually it is the secure firmware. Before passing the execution to next image
 * there is conditional rule to remove the secrets from the memory. This must be
 * done if the following conditions are satisfied:
 *  - Memory is shared between SW components at different stages of the trusted
 *    boot process.
 *  - There are secrets in the memory: KDF parameter, symmetric key,
 *    manufacturer sensitive code/data, etc.
 */
#if defined(__ICCARM__)
#pragma required = boot_clear_ram_area
#endif

__WEAK __attribute__((naked)) void boot_jump_to_next_image(uint32_t reset_handler_addr)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                 \n"
#endif
        "mov     r7, r0                  \n"
        "bl      boot_clear_ram_area     \n" /* Clear RAM before jump */
        "movs    r0, #0                  \n" /* Clear registers: R0-R12, */
        "mov     r1, r0                  \n" /* except R7 */
        "mov     r2, r0                  \n"
        "mov     r3, r0                  \n"
        "mov     r4, r0                  \n"
        "mov     r5, r0                  \n"
        "mov     r6, r0                  \n"
        "mov     r8, r0                  \n"
        "mov     r9, r0                  \n"
        "mov     r10, r0                 \n"
        "mov     r11, r0                 \n"
        "mov     r12, r0                 \n"
        "mov     lr,  r0                 \n"
        "bx      r7                      \n" /* Jump to Reset_handler */
    );
}

/* bootloader platform-specific hw initialization */
__WEAK int32_t boot_platform_init(void)
{
    int32_t result;

#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
 || defined(__ARM_ARCH_8_1M_MAIN__)
    /* Initialize stack limit register */
    uint32_t msp_stack_bottom =
            (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base);

    __set_MSPLIM(msp_stack_bottom);
#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
       || defined(__ARM_ARCH_8_1M_MAIN__) */

#ifdef FLASH_DEV_NAME
    result = FLASH_DEV_NAME.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }
#endif /* FLASH_DEV_NAME */
#ifdef FLASH_DEV_NAME_2
    result = FLASH_DEV_NAME_2.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }
#endif /* FLASH_DEV_NAME_2 */
#ifdef FLASH_DEV_NAME_3
    result = FLASH_DEV_NAME_3.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }
#endif /* FLASH_DEV_NAME_3 */
#ifdef FLASH_DEV_NAME_SCRATCH
    result = FLASH_DEV_NAME_SCRATCH.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }
#endif /* FLASH_DEV_NAME_SCRATCH */

    return 0;
}

__WEAK int32_t boot_platform_post_init(void)
{
#ifdef CRYPTO_HW_ACCELERATOR
    int32_t result;

    result = crypto_hw_accelerator_init();
    if (result) {
        return 1;
    }

    (void)fih_delay_init();
#endif /* CRYPTO_HW_ACCELERATOR */

    return 0;
}

__WEAK void boot_platform_quit(struct boot_arm_vector_table *vt)
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
        while (1){}
    }
#endif /* CRYPTO_HW_ACCELERATOR */

#ifdef FLASH_DEV_NAME
    result = FLASH_DEV_NAME.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while(1) {}
    }
#endif /* FLASH_DEV_NAME */
#ifdef FLASH_DEV_NAME_2
    result = FLASH_DEV_NAME_2.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while(1) {}
    }
#endif /* FLASH_DEV_NAME_2 */
#ifdef FLASH_DEV_NAME_3
    result = FLASH_DEV_NAME_3.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while(1) {}
    }
#endif /* FLASH_DEV_NAME_3 */
#ifdef FLASH_DEV_NAME_SCRATCH
    result = FLASH_DEV_NAME_SCRATCH.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while(1) {}
    }
#endif /* FLASH_DEV_NAME_SCRATCH */

    vt_cpy = vt;
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
 || defined(__ARM_ARCH_8_1M_MAIN__)
    /* Restore the Main Stack Pointer Limit register's reset value
     * before passing execution to runtime firmware to make the
     * bootloader transparent to it.
     */
    __set_MSPLIM(0);
#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
       || defined(__ARM_ARCH_8_1M_MAIN__) */

    __set_MSP(vt_cpy->msp);
    __DSB();
    __ISB();

    boot_jump_to_next_image(vt_cpy->reset);
}

__WEAK int boot_platform_pre_load(uint32_t image_id)
{
    return 0;
}

__WEAK int boot_platform_post_load(uint32_t image_id)
{
    return 0;
}

#ifdef MEASURED_BOOT_API
static int boot_add_data_to_shared_area(uint8_t        major_type,
                                        uint16_t       minor_type,
                                        size_t         size,
                                        const uint8_t *data)
{
    struct shared_data_tlv_entry tlv_entry = {0};
    struct tfm_boot_data *boot_data;
    uintptr_t tlv_end, offset;

    if (data == NULL) {
        return -1;
    }

    boot_data = (struct tfm_boot_data *)BOOT_TFM_SHARED_DATA_BASE;

    /* Check whether the shared area needs to be initialized. */
    if ((boot_data->header.tlv_magic != SHARED_DATA_TLV_INFO_MAGIC) ||
        (boot_data->header.tlv_tot_len > BOOT_TFM_SHARED_DATA_SIZE)) {

        memset((void *)BOOT_TFM_SHARED_DATA_BASE, 0, BOOT_TFM_SHARED_DATA_SIZE);
        boot_data->header.tlv_magic   = SHARED_DATA_TLV_INFO_MAGIC;
        boot_data->header.tlv_tot_len = SHARED_DATA_HEADER_SIZE;
    }

    /* Get the boundaries of TLV section. */
    tlv_end = BOOT_TFM_SHARED_DATA_BASE + boot_data->header.tlv_tot_len;
    offset  = BOOT_TFM_SHARED_DATA_BASE + SHARED_DATA_HEADER_SIZE;

    /* Check whether TLV entry is already added. Iterates over the TLV section
     * looks for the same entry if found then returns with error.
     */
    while (offset < tlv_end) {
        /* Create local copy to avoid unaligned access */
        memcpy(&tlv_entry, (const void *)offset, SHARED_DATA_ENTRY_HEADER_SIZE);
        if (GET_MAJOR(tlv_entry.tlv_type) == major_type &&
            GET_MINOR(tlv_entry.tlv_type) == minor_type) {
            return -1;
        }

        offset += SHARED_DATA_ENTRY_SIZE(tlv_entry.tlv_len);
    }

    /* Add TLV entry. */
    tlv_entry.tlv_type = SET_TLV_TYPE(major_type, minor_type);
    tlv_entry.tlv_len  = size;

    /* Check integer overflow and overflow of shared data area. */
    if (SHARED_DATA_ENTRY_SIZE(size) >
        (UINT16_MAX - boot_data->header.tlv_tot_len)) {
        return -1;
    } else if ((SHARED_DATA_ENTRY_SIZE(size) + boot_data->header.tlv_tot_len) >
               BOOT_TFM_SHARED_DATA_SIZE) {
        return -1;
    }

    offset = tlv_end;
    memcpy((void *)offset, &tlv_entry, SHARED_DATA_ENTRY_HEADER_SIZE);

    offset += SHARED_DATA_ENTRY_HEADER_SIZE;
    memcpy((void *)offset, data, size);

    boot_data->header.tlv_tot_len += SHARED_DATA_ENTRY_SIZE(size);

    return 0;
}

__WEAK int boot_store_measurement(
                            uint8_t index,
                            const uint8_t *measurement,
                            size_t measurement_size,
                            const struct boot_measurement_metadata *metadata,
                            bool lock_measurement)
{
    uint16_t minor_type;
    uint8_t claim;
    int rc;

    if (index >= BOOT_MEASUREMENT_SLOT_MAX) {
        return -1;
    }

    minor_type = SET_MBS_MINOR(index, SW_MEASURE_METADATA);
    rc = boot_add_data_to_shared_area(TLV_MAJOR_MBS,
                                      minor_type,
                                      sizeof(struct boot_measurement_metadata),
                                      (const uint8_t *)metadata);
    if (rc) {
        return rc;
    }

    claim = lock_measurement ? SW_MEASURE_VALUE_NON_EXTENDABLE
                             : SW_MEASURE_VALUE;
    minor_type = SET_MBS_MINOR(index, claim);
    rc = boot_add_data_to_shared_area(TLV_MAJOR_MBS,
                                      minor_type,
                                      measurement_size,
                                      measurement);

    return rc;
}
#endif /* MEASURED_BOOT_API */
