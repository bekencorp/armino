/*
 * Copyright (c) 2013-2022 ARM Limited. All rights reserved.
 * Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Driver_Flash.h"
#include "NuMicro.h"
#include "RTE_Device.h"
#include "flash_layout.h"
#include "region_defs.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

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

/*
 * ARM FLASH device structure
 */
struct arm_flash_dev_t
{
    const uint32_t memory_base;   /*!< FLASH memory base address */
    ARM_FLASH_INFO *data;         /*!< FLASH data */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion =
{
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities =
{
    0, /* event_ready */
    2, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};

static int32_t is_range_valid(struct arm_flash_dev_t *flash_dev,
                              uint32_t offset)
{
    uint32_t flash_size = 0;
    int32_t rc = 0;

    flash_size = (flash_dev->data->sector_count * flash_dev->data->sector_size);

    if(offset >= flash_size)
    {
        rc = -1;
    }
    return rc;
}

static int32_t is_write_aligned(struct arm_flash_dev_t *flash_dev,
                                uint32_t param)
{
    int32_t rc = 0;

    if((param % flash_dev->data->program_unit) != 0)
    {
        rc = -1;
    }
    return rc;
}

static int32_t is_sector_aligned(struct arm_flash_dev_t *flash_dev,
                                 uint32_t offset)
{
    int32_t rc = 0;

    if((offset % flash_dev->data->sector_size) != 0)
    {
        rc = -1;
    }
    return rc;
}

#if (RTE_FLASH0)
static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA =
{
    .sector_info  = NULL,                  /* Uniform sector layout */
    .sector_count = FMC_APROM_SIZE / FMC_FLASH_PAGE_SIZE,
    .sector_size  = FMC_FLASH_PAGE_SIZE,
    .page_size    = FMC_FLASH_PAGE_SIZE,
    .program_unit = 4,
    .erased_value = 0xFF
};

static struct arm_flash_dev_t ARM_FLASH0_DEV =
{
#if (__DOMAIN_NS == 1)
    .memory_base = FLASH0_BASE_NS,
#else
    .memory_base = FMC_APROM_BASE,
#endif /* __DOMAIN_NS == 1 */
    .data        = &(ARM_FLASH0_DEV_DATA)
};

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

    FMC_Open();
    FMC_ENABLE_AP_UPDATE();
    FMC_ENABLE_LD_UPDATE();


    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch(state)
    {
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
    uint8_t* pu8;
    uint32_t i, taddr;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries */
    rc = is_range_valid(FLASH0_DEV, addr + cnt);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    pu8 = (uint8_t*)data;

    for(i = 0; i < cnt; i++)
    {
        taddr = start_addr + i;
        if(taddr >= SCU->FNSADDR)
            taddr += NS_OFFSET;
        pu8[i] = *(uint8_t*)taddr;
    }

    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
    volatile uint32_t mem_base = FLASH0_DEV->memory_base;
    uint32_t start_addr = mem_base + addr;
    int32_t rc = 0;
    int32_t i,j;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries and alignment with minimal write size */
    rc  = is_range_valid(FLASH0_DEV, addr + cnt);
    rc |= is_write_aligned(FLASH0_DEV, addr);
    rc |= is_write_aligned(FLASH0_DEV, cnt);
    if(rc != 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    {
        uint8_t *pu8;
        uint32_t u32Data;

        pu8 = (uint8_t *)data; // Note the data buffer address may not be word alignment

        u32Data = 0;
        j = 0;
        for(i=0;i<cnt;i++)
        {
            u32Data += (uint32_t)pu8[i] << (8*j);
            j++;

            if(j>=4)
            {
                FMC_Write(start_addr+(i+1-4), u32Data);
                // verify
                if(M32(start_addr + i - 3) != u32Data)
                {
                    printf("flash write verify fail @ %08x W:%08x R:%08x\r\n", start_addr + i - 3, u32Data, M32(start_addr + i - 3));
                }

                j = 0;
                u32Data = 0;

            }
        }

        if(j)
        {
            // This should not happen.
            while(1);
        }

    }

    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    volatile uint32_t mem_base = FLASH0_DEV->memory_base;
    uint32_t start_addr = mem_base + addr;
    uint32_t rc = 0;

    rc  = is_range_valid(FLASH0_DEV, addr);
    rc |= is_sector_aligned(FLASH0_DEV, addr);
    if(rc != 0)
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    FMC_Erase(start_addr);

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_EraseChip(void)
{
    uint32_t i;
    uint32_t addr = FLASH0_DEV->memory_base;
    int32_t rc = ARM_DRIVER_ERROR_UNSUPPORTED;

    /* Check driver capability erase_chip bit */
    if(DriverCapabilities.erase_chip == 1)
    {
        for(i = 0; i < FLASH0_DEV->data->sector_count; i++)
        {
            FMC_Erase(addr);
            addr += FLASH0_DEV->data->sector_size;
            rc = ARM_DRIVER_OK;
        }
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

ARM_DRIVER_FLASH Driver_FLASH0 =
{
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
