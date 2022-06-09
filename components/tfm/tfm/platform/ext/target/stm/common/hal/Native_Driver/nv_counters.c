/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2021-2022, STMicroelectronics.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "nv_counters_device.h"
#include "tfm_plat_nv_counters.h"
#include <limits.h>
#include <stddef.h>
#include "Driver_Flash.h"
#include "nv_counters.h"

/* Compilation time checks to be sure the defines are well defined */
#ifndef DEVICE_NV_COUNTERS_AREA_SIZE
#error "DEVICE_NV_COUNTERS_AREA_SIZE must be defined"
#endif

#ifndef DEVICE_NV_COUNTERS_AREA_OFFSET
#error "DEVICE_NV_COUNTERS_AREA_OFFSET must be defined"
#endif

#ifndef DEVICE_NV_COUNTERS_SECTOR_SIZE
#error "DEVICE_NV_COUNTERS_SECTOR_SIZE must be defined"
#endif

#if DEVICE_NV_COUNTERS_AREA_SIZE!=(2*DEVICE_NV_COUNTERS_SECTOR_SIZE)
#error "DEVICE_NV_COUNTERS_AREA_SIZE !=  2 x DEVICE_NV_COUNTER_SIZE"
#endif

#ifndef DEVICE_NV_COUNTERS_FLASH_NAME
#error "DEVICE_NV_COUNTERS_FLASH_NAME must be defined"
#endif

#ifndef DEVICE_NUM_NV_COUNTERS
#error "DEVICE_NUM_NV_COUNTERS"
#endif
/* End of compilation time checks to be sure the defines are well defined */

#define SECTOR_OFFSET    0
#define NV_COUNTER_SIZE  sizeof(uint32_t)
#define INIT_VALUE_SIZE  sizeof(uint32_t)
#define CHECKSUM_SIZE    sizeof(uint32_t)
#define NUM_NV_COUNTERS  (DEVICE_NUM_NV_COUNTERS)


#define BACKUP_ADDRESS (DEVICE_NV_COUNTERS_AREA_OFFSET + DEVICE_NV_COUNTERS_SECTOR_SIZE)
#define VALID_ADDRESS  (DEVICE_NV_COUNTERS_AREA_OFFSET)


/* Import the CMSIS flash device driver */
extern ARM_DRIVER_FLASH DEVICE_NV_COUNTERS_FLASH_NAME;

static uint32_t calc_checksum(const uint32_t *data, size_t len)
{
    uint32_t sum = 0;

    for (uint32_t i = 0; i < len/sizeof(uint32_t); i++) {
        sum ^= data[i];
    }
    return sum;
}

static bool is_valid(const struct nv_counters_t *nv_counters)
{
    return ((nv_counters->init_value == NV_COUNTERS_INITIALIZED) &&
            (!calc_checksum(&nv_counters->checksum, sizeof(*nv_counters))));
}

static void set_checksum(struct nv_counters_t *nv_counters)
{
    uint32_t sum = calc_checksum(&nv_counters->init_value,
                                 sizeof(*nv_counters)
                                  - sizeof(nv_counters->checksum));

    nv_counters->checksum = sum;
}

volatile int once=0;
enum tfm_plat_err_t tfm_plat_init_nv_counter(void)
{
    int32_t  ret;
    struct nv_counters_t nv_counters;
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint8_t data_width;
    /* Valid entries for data item width */
    uint32_t data_width_byte[] = {
        sizeof(uint8_t),
        sizeof(uint16_t),
        sizeof(uint32_t),
    };
#if defined(NVM_COUNTER_INIT)
    uint32_t i;
#endif
    ret = DEVICE_NV_COUNTERS_FLASH_NAME.Initialize(NULL);
    if (ret != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    DriverCapabilities = DEVICE_NV_COUNTERS_FLASH_NAME.GetCapabilities();
    /* Since struct nv_counter is aligned on 32 bits , a single read /write is possible */
    data_width = data_width_byte[DriverCapabilities.data_width];

    /* Read the whole sector so we can write it back to flash later */
    ret = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(VALID_ADDRESS,
                                             &nv_counters,
                                             sizeof(struct nv_counters_t) / data_width);
    if (ret != (sizeof(struct nv_counters_t) / data_width)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    if (is_valid(&nv_counters)) {
        return TFM_PLAT_ERR_SUCCESS;
    }

    /* Check the backup watermark */
    ret = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(BACKUP_ADDRESS,
                                             &nv_counters,
                                             sizeof(struct nv_counters_t) / data_width);
    if (ret != (sizeof(struct nv_counters_t) / data_width)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    /* Erase sector before writing to it */
    ret = DEVICE_NV_COUNTERS_FLASH_NAME.EraseSector(VALID_ADDRESS);
    if (ret != ARM_DRIVER_OK) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    if (is_valid(&nv_counters)) {
        /* Copy from the backup to the main */
        ret = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(VALID_ADDRESS,
                                                    &nv_counters,
                                                    sizeof(struct nv_counters_t) / data_width);
        if (ret != (sizeof(struct nv_counters_t) / data_width)) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        return TFM_PLAT_ERR_SUCCESS;
    }
#if !defined(NVM_COUNTER_INIT)
    return TFM_PLAT_ERR_SYSTEM_ERR;
#else
    /* Add watermark to indicate that NV counters have been initialized */
    nv_counters.init_value = NV_COUNTERS_INITIALIZED;

    /* Initialize all counters to 0 */
    for (i = 0; i < NUM_NV_COUNTERS; i++) {
        nv_counters.counters[i] = 0;
    }
    set_checksum(&nv_counters);
    /* Write the in-memory block content after modification to flash */
    ret = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(VALID_ADDRESS,
                                                &nv_counters,
                                                sizeof(struct nv_counters_t)  / data_width);
    if (ret != (sizeof(struct nv_counters_t) / data_width)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
#endif
}

enum tfm_plat_err_t tfm_plat_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                             uint32_t size, uint8_t *val)
{
    int32_t  ret;
    uint32_t flash_addr = VALID_ADDRESS
                           + offsetof(struct nv_counters_t, counters)
                           + (counter_id * NV_COUNTER_SIZE);
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint8_t data_width;
    /* Valid entries for data item width */
    uint32_t data_width_byte[] = {
        sizeof(uint8_t),
        sizeof(uint16_t),
        sizeof(uint32_t),
    };

    if (size != NV_COUNTER_SIZE) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    DriverCapabilities = DEVICE_NV_COUNTERS_FLASH_NAME.GetCapabilities();
    /* Since struct nv_counter is aligned on 32 bits , a single read /write is possible */
    data_width = data_width_byte[DriverCapabilities.data_width];

    ret = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(flash_addr, val, NV_COUNTER_SIZE / data_width);
    if (ret != (NV_COUNTER_SIZE / data_width)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_set_nv_counter(enum tfm_nv_counter_t counter_id,
                                            uint32_t value)
{
    int32_t  ret;
    struct nv_counters_t nv_counters;
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint8_t data_width;
    /* Valid entries for data item width */
    uint32_t data_width_byte[] = {
        sizeof(uint8_t),
        sizeof(uint16_t),
        sizeof(uint32_t),
    };
    
    DriverCapabilities = DEVICE_NV_COUNTERS_FLASH_NAME.GetCapabilities();
    /* Since struct nv_counter is aligned on 32 bits , a single read /write is possible */
    data_width = data_width_byte[DriverCapabilities.data_width];
    
    /* Read the whole sector so we can write it back to flash later */
    ret = DEVICE_NV_COUNTERS_FLASH_NAME.ReadData(VALID_ADDRESS,
                                             &nv_counters,
                                             sizeof(struct nv_counters_t) / data_width);
    if (ret != (sizeof(struct nv_counters_t) / data_width)) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    if (value != nv_counters.counters[counter_id]) {

        if (value < nv_counters.counters[counter_id]) {
            return TFM_PLAT_ERR_INVALID_INPUT;
        }

        /* Erase backup sector */
        ret = DEVICE_NV_COUNTERS_FLASH_NAME.EraseSector(BACKUP_ADDRESS);
        if (ret != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        nv_counters.counters[counter_id] = value;

        set_checksum(&nv_counters);

        /* write sector data to backup sector */
        ret = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(BACKUP_ADDRESS,
                                                    &nv_counters,
                                                    sizeof(struct nv_counters_t) / data_width);
        if (ret != (sizeof(struct nv_counters_t) / data_width)) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        /* Erase sector before writing to it */
        ret = DEVICE_NV_COUNTERS_FLASH_NAME.EraseSector(VALID_ADDRESS);
        if (ret != ARM_DRIVER_OK) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        /* Write the in-memory block content after modification to flash */
        ret = DEVICE_NV_COUNTERS_FLASH_NAME.ProgramData(VALID_ADDRESS,
                                                    &nv_counters,
                                                    sizeof(struct nv_counters_t) / data_width);
        if (ret != (sizeof(struct nv_counters_t) / data_width)) {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_increment_nv_counter(
                                           enum tfm_nv_counter_t counter_id)
{
    uint32_t security_cnt;
    enum tfm_plat_err_t err;

    err = tfm_plat_read_nv_counter(counter_id,
                                   sizeof(security_cnt),
                                   (uint8_t *)&security_cnt);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    if (security_cnt == UINT32_MAX) {
        return TFM_PLAT_ERR_MAX_VALUE;
    }

    return tfm_plat_set_nv_counter(counter_id, security_cnt + 1u);
}
