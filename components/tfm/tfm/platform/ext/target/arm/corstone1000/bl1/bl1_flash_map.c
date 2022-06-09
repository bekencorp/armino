/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "flash_map/flash_map.h"
#include "flash_map_backend/flash_map_backend.h"
#include "bootutil/bootutil_log.h"
#include "sysflash/sysflash.h"
#include "Driver_Flash.h"

/* Flash device name must be specified by target */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

struct flash_area flash_map[] = {
    {
        .fa_id = FLASH_AREA_8_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = FLASH_AREA_8_OFFSET,
        .fa_size = FLASH_AREA_8_SIZE,
    },
    {
        .fa_id = FLASH_AREA_9_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = FLASH_AREA_9_OFFSET,
        .fa_size = FLASH_AREA_9_SIZE,
    },
};

const int flash_map_entry_num = ARRAY_SIZE(flash_map);

void add_bank_offset_to_image_offset(uint32_t bank_offset)
{
    for (int i = 0; i < flash_map_entry_num; i++) {
        flash_map[i].fa_off += bank_offset;
    }
}
