/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "flash_map/flash_map.h"
#include "bootutil/ramload.h"
#include "platform_base_address.h"
#include "region_defs.h"

/* Flash device name must be specified by target */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))
/*
 * flash_map[0]: TF-M Primary
 * flash_map[1]: TF-M Secondary
 * flash_map[2]: TF-A Primary
 * flash_map[3]: TF-A Secondaary
 *
 * TF-A flash area's fa_off and fa_size are populated
 * by parsing FIP.
 */
struct flash_area flash_map[] = {
    {
        .fa_id = FLASH_AREA_0_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = FLASH_AREA_0_OFFSET,
        .fa_size = FLASH_AREA_0_SIZE,
    },
    {
        .fa_id = FLASH_AREA_1_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = FLASH_AREA_1_OFFSET,
        .fa_size = FLASH_AREA_1_SIZE,
    },
    {
        .fa_id = FLASH_AREA_2_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = FLASH_INVALID_OFFSET,
        .fa_size = FLASH_INVALID_SIZE,
    },
    {
        .fa_id = FLASH_AREA_3_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = FLASH_INVALID_OFFSET,
        .fa_size = FLASH_INVALID_SIZE,
    },
};

const int flash_map_entry_num = ARRAY_SIZE(flash_map);

void add_bank_offset_to_image_offset(uint32_t bank_offset)
{
    for (int i = 0; i < flash_map_entry_num; i++) {
        flash_map[i].fa_off += bank_offset;
    }
}

int boot_get_image_exec_ram_info(uint32_t image_id,
                                 uint32_t *exec_ram_start,
                                 uint32_t *exec_ram_size)
{
    int32_t rc =  -1;

    if (image_id == 0) {
        (*exec_ram_start) = SRAM_BASE;
        (*exec_ram_size) = TFM_PARTITION_SIZE;
        rc = 0;
    }
    else if (image_id == 1 || image_id == 2) {
        (*exec_ram_start) = CORSTONE1000_HOST_SHARED_RAM_BASE;
        (*exec_ram_size) = 0x20000000U;
        rc = 0;
    }

    return rc;
}
