/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "flash_map/flash_map.h"
#include "target.h"
#include "Driver_Flash.h"

/* When undefined FLASH_DEV_NAME_0 or FLASH_DEVICE_ID_0 , default */
#if !defined(FLASH_DEV_NAME_0) || !defined(FLASH_DEVICE_ID_0)
#define FLASH_DEV_NAME_0  FLASH_DEV_NAME
#define FLASH_DEVICE_ID_0 FLASH_DEVICE_ID
#endif

/* When undefined FLASH_DEV_NAME_1 or FLASH_DEVICE_ID_1 , default */
#if !defined(FLASH_DEV_NAME_1) || !defined(FLASH_DEVICE_ID_1)
#define FLASH_DEV_NAME_1  FLASH_DEV_NAME
#define FLASH_DEVICE_ID_1 FLASH_DEVICE_ID
#endif

/* When undefined FLASH_DEV_NAME_2 or FLASH_DEVICE_ID_2 , default */
#if !defined(FLASH_DEV_NAME_2) || !defined(FLASH_DEVICE_ID_2)
#define FLASH_DEV_NAME_2  FLASH_DEV_NAME
#define FLASH_DEVICE_ID_2 FLASH_DEVICE_ID
#endif

/* When undefined FLASH_DEV_NAME_3 or FLASH_DEVICE_ID_3 , default */
#if !defined(FLASH_DEV_NAME_3) || !defined(FLASH_DEVICE_ID_3)
#define FLASH_DEV_NAME_3  FLASH_DEV_NAME
#define FLASH_DEVICE_ID_3 FLASH_DEVICE_ID
#endif

#if defined(MCUBOOT_SWAP_USING_SCRATCH)
/* When undefined FLASH_DEV_NAME_SCRATCH or FLASH_DEVICE_ID_SCRATCH , default */
#if !defined(FLASH_DEV_NAME_SCRATCH) || !defined(FLASH_DEVICE_ID_SCRATCH)
#define FLASH_DEV_NAME_SCRATCH  FLASH_DEV_NAME
#define FLASH_DEVICE_ID_SCRATCH FLASH_DEVICE_ID
#endif
#endif  /* defined(MCUBOOT_SWAP_USING_SCRATCH) */

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

/* Flash device names must be specified by target */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_0;
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_1;
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_2;
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_3;
extern ARM_DRIVER_FLASH FLASH_DEV_NAME_SCRATCH;

const struct flash_area flash_map[] = {
    {
        .fa_id = FLASH_AREA_PRIMARY_ALL_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = CONFIG_PRIMARY_ALL_PHY_PARTITION_OFFSET,
        .fa_size = CONFIG_PRIMARY_ALL_PHY_PARTITION_SIZE,
    },
    {
        .fa_id = FLASH_AREA_SECONDARY_ALL_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET,
        .fa_size = CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE,
    },
    {
        .fa_id = FLASH_AREA_PRIMARY_MANIFEST_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET,
        .fa_size = CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_SIZE,
    },
    {
        .fa_id = FLASH_AREA_SECONDARY_MANIFEST_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET,
        .fa_size = CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_SIZE,
    },
    {
        .fa_id = FLASH_AREA_PRIMARY_BL2_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = CONFIG_PRIMARY_BL2_PHY_PARTITION_OFFSET,
        .fa_size = CONFIG_PRIMARY_BL2_PHY_PARTITION_SIZE,
    },
    {
        .fa_id = FLASH_AREA_SECONDARY_BL2_ID,
        .fa_device_id = FLASH_DEVICE_ID,
        .fa_driver = &FLASH_DEV_NAME,
        .fa_off = CONFIG_SECONDARY_BL2_PHY_PARTITION_OFFSET,
        .fa_size = CONFIG_SECONDARY_BL2_PHY_PARTITION_SIZE,
    },
#if defined(MCUBOOT_SWAP_USING_SCRATCH)
    {
        .fa_id = FLASH_AREA_SCRATCH_ID,
        .fa_device_id = FLASH_DEVICE_ID_SCRATCH,
        .fa_driver = &FLASH_DEV_NAME_SCRATCH,
        .fa_off = FLASH_AREA_SCRATCH_OFFSET,
        .fa_size = FLASH_AREA_SCRATCH_SIZE,
    },
#endif
};

const int flash_map_entry_num = ARRAY_SIZE(flash_map);
