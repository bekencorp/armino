/*
 * Copyright (c) 2013-2018 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Nordic Semiconductor ASA. All rights reserved.
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

#include <Driver_Flash.h>
#include <RTE_Device.h>
#include <flash_layout.h>
#include <string.h>
#include <nrfx_nvmc.h>

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  (void)arg
#endif

#define ARM_FLASH_DRV_VERSION  ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

#if RTE_FLASH0

/**
 * Data width values for ARM_FLASH_CAPABILITIES::data_width
 * \ref ARM_FLASH_CAPABILITIES
 */
enum {
    DATA_WIDTH_8BIT = 0u,
    DATA_WIDTH_16BIT,
    DATA_WIDTH_32BIT,
    DATA_WIDTH_ENUM_SIZE
};

static const uint32_t data_width_byte[DATA_WIDTH_ENUM_SIZE] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    .event_ready = 0,
    .data_width  = DATA_WIDTH_32BIT,
    .erase_chip  = 1
};

static ARM_FLASH_INFO FlashInfo = {
    .sector_info  = NULL, /* Uniform sector layout */
    .sector_count = FLASH_TOTAL_SIZE / FLASH_AREA_IMAGE_SECTOR_SIZE,
    .sector_size  = FLASH_AREA_IMAGE_SECTOR_SIZE,
    .page_size    = sizeof(uint32_t), /* 32-bit word = 4 bytes */
    .program_unit = sizeof(uint32_t), /* 32-bit word = 4 bytes */
    .erased_value = 0xFF
};

static bool is_range_valid(uint32_t addr, uint32_t cnt)
{
    uint32_t start_offset = (addr - FLASH_BASE_ADDRESS);

    if (start_offset > FLASH_TOTAL_SIZE) {
        return false;
    }

    if (cnt > (FLASH_TOTAL_SIZE - start_offset)) {
        return false;
    }

    return true;
}

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

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state) {
    case ARM_POWER_FULL:
        /* Nothing to be done */
        return ARM_DRIVER_OK;

    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

static int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    /* Conversion between data items and bytes */
    uint32_t bytes = cnt * data_width_byte[DriverCapabilities.data_width];

    if (!is_range_valid(addr, bytes)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    memcpy(data, (const void *)addr, bytes);

    return cnt;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    /* Conversion between data items and bytes */
    uint32_t bytes = cnt * data_width_byte[DriverCapabilities.data_width];

    /* Only aligned writes of full 32-bit words are allowed. */
    if (addr % sizeof(uint32_t)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if (!is_range_valid(addr, bytes)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    nrfx_nvmc_words_write(addr, data, cnt);

    /* Conversion between bytes and data items */
    return cnt;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    nrfx_err_t err_code = nrfx_nvmc_page_erase(addr);
    if (err_code != NRFX_SUCCESS) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_EraseChip(void)
{
    nrfx_nvmc_all_erase();
    return ARM_DRIVER_OK;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    ARM_FLASH_STATUS status = {
        .busy = !nrfx_nvmc_write_done_check()
    };

    return status;
}

static ARM_FLASH_INFO * ARM_Flash_GetInfo(void)
{
    return &FlashInfo;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    .GetVersion      = ARM_Flash_GetVersion,
    .GetCapabilities = ARM_Flash_GetCapabilities,
    .Initialize      = ARM_Flash_Initialize,
    .Uninitialize    = ARM_Flash_Uninitialize,
    .PowerControl    = ARM_Flash_PowerControl,
    .ReadData        = ARM_Flash_ReadData,
    .ProgramData     = ARM_Flash_ProgramData,
    .EraseSector     = ARM_Flash_EraseSector,
    .EraseChip       = ARM_Flash_EraseChip,
    .GetStatus       = ARM_Flash_GetStatus,
    .GetInfo         = ARM_Flash_GetInfo
};

#endif /* RTE_FLASH0 */
