/*
 * Copyright (c) 2019-2022 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdint.h>
#include "Driver_Flash.h"
#include "platform_base_address.h"
#include "RTE_Device.h"
#include "flash_layout.h"
#include "cmsis_driver_config.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

#define FLASH0_SIZE                    0x00200000  /* 2 MB */
#define FLASH0_SECTOR_SIZE             0x00001000  /* 4 kB */
#define FLASH0_PAGE_SIZE               0x00001000  /* 4 kB */
#define FLASH0_PROGRAM_UNIT            0x1         /* Minimum write size */

/* Driver version */
#define ARM_FLASH_DRV_VERSION      ARM_DRIVER_VERSION_MAJOR_MINOR(1, 1)
#define ARM_FLASH_DRV_ERASE_VALUE  0xFF

/*
 * ARM FLASH device structure
 */
struct arm_flash_dev_t {
    const uint32_t memory_base;   /*!< FLASH memory base address */
    ARM_FLASH_INFO *data;         /*!< FLASH data */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/**
 * Data width values for ARM_FLASH_CAPABILITIES::data_width
 * \ref ARM_FLASH_CAPABILITIES
 */
 enum {
    DATA_WIDTH_8BIT   = 0u,
    DATA_WIDTH_16BIT,
    DATA_WIDTH_32BIT,
    DATA_WIDTH_ENUM_SIZE
};

static const uint32_t data_width_byte[DATA_WIDTH_ENUM_SIZE] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};

static int32_t is_range_valid(struct arm_flash_dev_t *flash_dev,
                              uint32_t offset)
{
    uint32_t flash_limit = 0;
    int32_t rc = 0;

    flash_limit = (flash_dev->data->sector_count * flash_dev->data->sector_size)
                   - 1;

    if (offset > flash_limit) {
        rc = -1;
    }
    return rc;
}

static int32_t is_write_aligned(struct arm_flash_dev_t *flash_dev,
                                uint32_t param)
{
    int32_t rc = 0;

    if ((param % flash_dev->data->program_unit) != 0) {
        rc = -1;
    }
    return rc;
}

static int32_t is_sector_aligned(struct arm_flash_dev_t *flash_dev,
                                 uint32_t offset)
{
    int32_t rc = 0;

    if ((offset % flash_dev->data->sector_size) != 0) {
        rc = -1;
    }
    return rc;
}

static int32_t is_flash_ready_to_write(const uint8_t *start_addr, uint32_t cnt)
{
    int32_t rc = 0;
    uint32_t i;

    for (i = 0; i < cnt; i++) {
        if(start_addr[i] != ARM_FLASH_DRV_ERASE_VALUE) {
            rc = -1;
            break;
        }
    }

    return rc;
}

#if (RTE_FLASH0)
static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info  = NULL,                  /* Uniform sector layout */
    .sector_count = FLASH0_SIZE / FLASH0_SECTOR_SIZE,
    .sector_size  = FLASH0_SECTOR_SIZE,
    .page_size    = FLASH0_PAGE_SIZE,
    .program_unit = FLASH0_PROGRAM_UNIT,
    .erased_value = ARM_FLASH_DRV_ERASE_VALUE};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
#if (__DOMAIN_NS == 1)
    .memory_base = MUSCA_S1_MRAM_NS_BASE,
#else
    .memory_base = MUSCA_S1_MRAM_S_BASE,
#endif /* __DOMAIN_NS == 1 */
    .data        = &(ARM_FLASH0_DEV_DATA)};

struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

/*
 * Functions
 */

static ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES ARM_Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

static int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);

    if (DriverCapabilities.data_width >= DATA_WIDTH_ENUM_SIZE) {
        return ARM_DRIVER_ERROR;
    }

    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state) {
    case ARM_POWER_FULL:
        /* Nothing to be done */
        return ARM_DRIVER_OK;
        break;

    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

static int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    uint32_t start_addr = FLASH0_DEV->memory_base + addr;
    int32_t rc = 0;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries */
    rc = is_range_valid(FLASH0_DEV, addr + cnt);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Flash interface just emulated over MRAM, use memcpy */
    memcpy(data, (void *)start_addr, cnt);
    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    uint32_t start_addr = FLASH0_DEV->memory_base + addr;
    int32_t rc;
    bool cache_is_used = false;
    bool mram_fast_read_is_used = false;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries and alignment with minimal write size */
    rc  = is_range_valid(FLASH0_DEV, addr + cnt);
    rc |= is_write_aligned(FLASH0_DEV, addr);
    rc |= is_write_aligned(FLASH0_DEV, cnt);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Check if the flash area to write the data was erased previously */
    rc = is_flash_ready_to_write((const uint8_t*)start_addr, cnt);
    if (rc != 0) {
        return ARM_DRIVER_ERROR;
    }

    /* Disable cache and invalidate before changing MRAM content */
    if (arm_cache_is_enabled(&SSE_200_CACHE_DEV)) {
        cache_is_used = true;
        arm_cache_disable_blocking(&SSE_200_CACHE_DEV);
        arm_cache_full_invalidate_blocking(&SSE_200_CACHE_DEV);
    }

    /* Disable mram fast read mode to avoid faults */
    if (musca_s1_scc_mram_is_fast_read_enabled(&MUSCA_S1_SCC_DEV)) {
        mram_fast_read_is_used = true;
        musca_s1_scc_mram_fast_read_disable(&MUSCA_S1_SCC_DEV);
    }

    /* Flash interface just emulated over MRAM, use memcpy */
    memcpy((void *)start_addr, data, cnt);

    if (mram_fast_read_is_used) {
        musca_s1_scc_mram_fast_read_enable(&MUSCA_S1_SCC_DEV);
    }

    if (cache_is_used) {
        arm_cache_enable_blocking(&SSE_200_CACHE_DEV);
    }

    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    uint32_t start_addr = FLASH0_DEV->memory_base + addr;
    uint32_t rc;
    bool cache_is_used = false;
    bool mram_fast_read_is_used = false;

    rc  = is_range_valid(FLASH0_DEV, addr);
    rc |= is_sector_aligned(FLASH0_DEV, addr);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Disable cache and invalidate before changing MRAM content */
    if (arm_cache_is_enabled(&SSE_200_CACHE_DEV)) {
        cache_is_used = true;
        arm_cache_disable_blocking(&SSE_200_CACHE_DEV);
        arm_cache_full_invalidate_blocking(&SSE_200_CACHE_DEV);
    }

    /* Disable mram fast read mode to avoid faults */
    if (musca_s1_scc_mram_is_fast_read_enabled(&MUSCA_S1_SCC_DEV)) {
        mram_fast_read_is_used = true;
        musca_s1_scc_mram_fast_read_disable(&MUSCA_S1_SCC_DEV);
    }

    /* Flash interface just emulated over MRAM, use memset */
    memset((void *)start_addr,
           FLASH0_DEV->data->erased_value,
           FLASH0_DEV->data->sector_size);

    if (mram_fast_read_is_used) {
        musca_s1_scc_mram_fast_read_enable(&MUSCA_S1_SCC_DEV);
    }

    if (cache_is_used) {
        arm_cache_enable_blocking(&SSE_200_CACHE_DEV);
    }

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_EraseChip(void)
{
    uint32_t i;
    uint32_t addr = FLASH0_DEV->memory_base;
    int32_t rc = ARM_DRIVER_ERROR_UNSUPPORTED;
    bool cache_is_used = false;
    bool mram_fast_read_is_used = false;

    /* Disable cache and invalidate before changing MRAM content */
    if (arm_cache_is_enabled(&SSE_200_CACHE_DEV)) {
        cache_is_used = true;
        arm_cache_disable_blocking(&SSE_200_CACHE_DEV);
        arm_cache_full_invalidate_blocking(&SSE_200_CACHE_DEV);
    }
    /* Disable mram fast read mode to avoid faults */
    if (musca_s1_scc_mram_is_fast_read_enabled(&MUSCA_S1_SCC_DEV)) {
        mram_fast_read_is_used = true;
        musca_s1_scc_mram_fast_read_disable(&MUSCA_S1_SCC_DEV);
    }

    /* Check driver capability erase_chip bit */
    if (DriverCapabilities.erase_chip == 1) {
        for (i = 0; i < FLASH0_DEV->data->sector_count; i++) {
            /* Flash interface just emulated over MRAM, use memset */
            memset((void *)addr,
                   FLASH0_DEV->data->erased_value,
                   FLASH0_DEV->data->sector_size);

            addr += FLASH0_DEV->data->sector_size;
            rc = ARM_DRIVER_OK;
        }
    }

    if (mram_fast_read_is_used) {
        musca_s1_scc_mram_fast_read_enable(&MUSCA_S1_SCC_DEV);
    }

    if (cache_is_used) {
        arm_cache_enable_blocking(&SSE_200_CACHE_DEV);
    }

    return rc;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO * ARM_Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    ARM_Flash_GetVersion,
    ARM_Flash_GetCapabilities,
    ARM_Flash_Initialize,
    ARM_Flash_Uninitialize,
    ARM_Flash_PowerControl,
    ARM_Flash_ReadData,
    ARM_Flash_ProgramData,
    ARM_Flash_EraseSector,
    ARM_Flash_EraseChip,
    ARM_Flash_GetStatus,
    ARM_Flash_GetInfo
};
#endif /* RTE_FLASH0 */
