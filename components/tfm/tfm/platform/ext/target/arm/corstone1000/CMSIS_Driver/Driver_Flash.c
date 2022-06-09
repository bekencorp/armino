/*
 * Copyright (c) 2013-2022 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdint.h>
#include "Driver_Flash.h"
#include "RTE_Device.h"
#include "platform_base_address.h"
#include "flash_layout.h"
#include "cmsis_driver_config.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION      ARM_DRIVER_VERSION_MAJOR_MINOR(1, 1)
#define ARM_FLASH_DRV_ERASE_VALUE  0xFF


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

#if PLATFORM_IS_FVP
/*
 * ARM FLASH device structure
 */
struct arm_flash_dev_t {
    struct cfi_strataflashj3_dev_t *dev;         /*!< FLASH memory device structure */
    ARM_FLASH_INFO *data;         /*!< FLASH data */
};
/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES STRATAFLASHJ3DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};
#else
/*
 * ARM FLASH device structure
 */
struct arm_flash_dev_t {
    struct spi_n25q256a_dev_t *dev;         /*!< FLASH memory device structure */
    ARM_FLASH_INFO *data;         /*!< FLASH data */
};
struct arm_flash_se_dev_t {
    struct spi_sst26vf064b_dev_t* dev;         /*!< FLASH memory device structure */
    ARM_FLASH_INFO* data;         /*!< FLASH data */
};
/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES N25Q256ADriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};
/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES SST26VF064BDriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};
#endif

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};


/*
 * Common interface functions
 */

static ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
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


#if PLATFORM_IS_FVP

#if (RTE_FLASH0)
static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info    = NULL,     /* Uniform sector layout */
    .sector_count   = STRATA_NVM_FLASH_TOTAL_SIZE / STRATA_NVM_FLASH_SECTOR_SIZE,
    .sector_size    = STRATA_NVM_FLASH_SECTOR_SIZE,
    .page_size      = STRATA_NVM_FLASH_PAGE_SIZE,
    .program_unit   = STRATA_NVM_FLASH_PROGRAM_UNIT,
    .erased_value   = ARM_FLASH_DRV_ERASE_VALUE
};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
    .dev    = &FLASH0_DEV,
    .data   = &(ARM_FLASH0_DEV_DATA)
};
#endif /* RTE_FLASH0 */

#if (RTE_FLASH1)
static ARM_FLASH_INFO ARM_FLASH1_DEV_DATA = {
    .sector_info    = NULL,     /* Uniform sector layout */
    .sector_count   = STRATA_SE_FLASH_TOTAL_SIZE / STRATA_SE_FLASH_SECTOR_SIZE,
    .sector_size    = STRATA_SE_FLASH_SECTOR_SIZE,
    .page_size      = STRATA_SE_FLASH_PAGE_SIZE,
    .program_unit   = STRATA_SE_FLASH_PROGRAM_UNIT,
    .erased_value   = ARM_FLASH_DRV_ERASE_VALUE
};

static struct arm_flash_dev_t ARM_FLASH1_DEV = {
    .dev    = &FLASH1_DEV,
    .data   = &(ARM_FLASH1_DEV_DATA)
};
#endif /* RTE_FLASH1 */

#else /* PLATFORM_IS_FVP */

#if (RTE_FLASH0)
static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info    = NULL,     /* Uniform sector layout */
    .sector_count   = PMOD_SF3_FLASH_TOTAL_SIZE / PMOD_SF3_FLASH_SECTOR_SIZE,
    .sector_size    = PMOD_SF3_FLASH_SECTOR_SIZE,
    .page_size      = PMOD_SF3_FLASH_PAGE_SIZE,
    .program_unit   = PMOD_SF3_FLASH_PROGRAM_UNIT,
    .erased_value   = ARM_FLASH_DRV_ERASE_VALUE
};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
    .dev    = &FLASH0_DEV,
    .data   = &(ARM_FLASH0_DEV_DATA)
};
#endif /* RTE_FLASH0 */

#if (RTE_FLASH1)
static ARM_FLASH_INFO ARM_FLASH1_DEV_DATA = {
    .sector_info    = NULL,     /* Uniform sector layout */
    .sector_count   = SST26VF064B_FLASH_TOTAL_SIZE / SST26VF064B_FLASH_SECTOR_SIZE,
    .sector_size    = SST26VF064B_FLASH_SECTOR_SIZE,
    .page_size      = SST26VF064B_FLASH_PAGE_SIZE,
    .program_unit   = SST26VF064B_FLASH_PROGRAM_UNIT,
    .erased_value   = ARM_FLASH_DRV_ERASE_VALUE
};

static struct arm_flash_se_dev_t ARM_FLASH1_DEV = {
    .dev    = &FLASH1_DEV,
    .data   = &(ARM_FLASH1_DEV_DATA)
};
#endif /* RTE_FLASH1 */
#endif /* PLATFORM_IS_FVP */


/*
 * Functions
 */
#if PLATFORM_IS_FVP

static int32_t ARM_Flash_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

#if (RTE_FLASH0)

int32_t Select_XIP_Mode_For_Shared_Flash(void)
{
    return ARM_DRIVER_OK;
}

int32_t Select_Write_Mode_For_Shared_Flash(void)
{
    return ARM_DRIVER_OK;
}

static int32_t STRATAFLASHJ3_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);
    enum strataflashj3_error_t ret;
    struct cfi_strataflashj3_dev_t* dev = ARM_FLASH0_DEV.dev;
    ARM_FLASH_INFO* data = ARM_FLASH0_DEV.data;

    dev->total_sector_cnt = data->sector_count;
    dev->page_size = data->page_size;
    dev->sector_size = data->sector_size;
    dev->program_unit = data->program_unit;


    ret = cfi_strataflashj3_initialize(ARM_FLASH0_DEV.dev);

    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: Initialization failed.\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static ARM_FLASH_CAPABILITIES STRATAFLASHJ3_Driver_GetCapabilities(void)
{
    return STRATAFLASHJ3DriverCapabilities;
}



static int32_t STRATAFLASHJ3_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    enum strataflashj3_error_t ret;

    if (STRATAFLASHJ3DriverCapabilities.data_width > 2 || STRATAFLASHJ3DriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];

    ret = cfi_strataflashj3_read(ARM_FLASH0_DEV.dev, addr, data, cnt);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: read failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];

    return cnt;
}

static int32_t STRATAFLASHJ3_Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    enum strataflashj3_error_t ret;
    
    if (STRATAFLASHJ3DriverCapabilities.data_width > 2 || STRATAFLASHJ3DriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];

    ret = cfi_strataflashj3_program(ARM_FLASH0_DEV.dev, addr, data, cnt);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: program failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];

    return cnt;
}

static int32_t STRATAFLASHJ3_Flash_EraseSector(uint32_t addr)
{
    enum strataflashj3_error_t ret;

    ret = cfi_strataflashj3_erase(ARM_FLASH0_DEV.dev, addr);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: erase failed: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static int32_t STRATAFLASHJ3_Flash_EraseChip(void)
{
    enum strataflashj3_error_t ret;

    ret = cfi_strataflashj3_erase_chip(ARM_FLASH0_DEV.dev);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: erase chip failed\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static ARM_FLASH_INFO * STRATAFLASHJ3_Flash_GetInfo(void)
{
    return &ARM_FLASH0_DEV_DATA;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    ARM_Flash_GetVersion,
    STRATAFLASHJ3_Driver_GetCapabilities,
    STRATAFLASHJ3_Initialize,
    ARM_Flash_Uninitialize,
    ARM_Flash_PowerControl,
    STRATAFLASHJ3_Flash_ReadData,
    STRATAFLASHJ3_Flash_ProgramData,
    STRATAFLASHJ3_Flash_EraseSector,
    STRATAFLASHJ3_Flash_EraseChip,
    ARM_Flash_GetStatus,
    STRATAFLASHJ3_Flash_GetInfo
};

#endif /* RTE_FLASH0 */

#if (RTE_FLASH1)
static int32_t STRATAFLASHJ3_Initialize_SE(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);
    enum strataflashj3_error_t ret;
    struct cfi_strataflashj3_dev_t* dev = ARM_FLASH1_DEV.dev;
    ARM_FLASH_INFO* data = ARM_FLASH1_DEV.data;

    dev->total_sector_cnt = data->sector_count;
    dev->page_size = data->page_size;
    dev->sector_size = data->sector_size;
    dev->program_unit = data->program_unit;


    ret = cfi_strataflashj3_initialize(ARM_FLASH1_DEV.dev);

    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: Initialization failed.\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static ARM_FLASH_CAPABILITIES STRATAFLASHJ3_Driver_GetCapabilities_SE(void)
{
    return STRATAFLASHJ3DriverCapabilities;
}

static int32_t STRATAFLASHJ3_Flash_ReadData_SE(uint32_t addr, void *data, uint32_t cnt)
{
    enum strataflashj3_error_t ret;

    if (STRATAFLASHJ3DriverCapabilities.data_width > 2 || STRATAFLASHJ3DriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];

    ret = cfi_strataflashj3_read(ARM_FLASH1_DEV.dev, addr, data, cnt);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: read failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];
    return cnt;
}

static int32_t STRATAFLASHJ3_Flash_ProgramData_SE(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    enum strataflashj3_error_t ret;

    if (STRATAFLASHJ3DriverCapabilities.data_width > 2 || STRATAFLASHJ3DriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];

    ret = cfi_strataflashj3_program(ARM_FLASH1_DEV.dev, addr, data, cnt);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: program failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[STRATAFLASHJ3DriverCapabilities.data_width];
    return cnt;
}

static int32_t STRATAFLASHJ3_Flash_EraseSector_SE(uint32_t addr)
{
    enum strataflashj3_error_t ret;

    ret = cfi_strataflashj3_erase(ARM_FLASH1_DEV.dev, addr);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: erase failed: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static int32_t STRATAFLASHJ3_Flash_EraseChip_SE(void)
{
    enum strataflashj3_error_t ret;

    ret = cfi_strataflashj3_erase_chip(ARM_FLASH1_DEV.dev);
    if (ret != STRATAFLASHJ3_ERR_NONE) {
        CFI_FLASH_LOG_MSG("%s: erase chip failed\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static ARM_FLASH_INFO * STRATAFLASHJ3_Flash_GetInfo_SE(void)
{
    return &ARM_FLASH1_DEV_DATA;
}

ARM_DRIVER_FLASH Driver_FLASH1 = {
    ARM_Flash_GetVersion,
    STRATAFLASHJ3_Driver_GetCapabilities_SE,
    STRATAFLASHJ3_Initialize_SE,
    ARM_Flash_Uninitialize,
    ARM_Flash_PowerControl,
    STRATAFLASHJ3_Flash_ReadData_SE,
    STRATAFLASHJ3_Flash_ProgramData_SE,
    STRATAFLASHJ3_Flash_EraseSector_SE,
    STRATAFLASHJ3_Flash_EraseChip_SE,
    ARM_Flash_GetStatus,
    STRATAFLASHJ3_Flash_GetInfo_SE
};

#endif /* RTE_FLASH1 */

#else /* PLATFORM_IS_FVP */

#if (RTE_FLASH0)

int32_t Select_XIP_Mode_For_Shared_Flash(void)
{
    select_xip_mode(&AXI_QSPI_DEV_S);
    return ARM_DRIVER_OK;
}

int32_t Select_Write_Mode_For_Shared_Flash(void)
{
    select_qspi_mode(&AXI_QSPI_DEV_S);
    return ARM_DRIVER_OK;
}

static ARM_FLASH_CAPABILITIES N25Q256A_Driver_GetCapabilities(void)
{
    return N25Q256ADriverCapabilities;
}

static int32_t N25Q256A_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);
    enum n25q256a_error_t ret;
    struct spi_n25q256a_dev_t* dev = ARM_FLASH0_DEV.dev;
    ARM_FLASH_INFO* data = ARM_FLASH0_DEV.data;

    dev->total_sector_cnt = data->sector_count;
    dev->page_size = data->page_size;
    dev->sector_size = data->sector_size;
    dev->program_unit = data->program_unit;

    ret = spi_n25q256a_initialize(ARM_FLASH0_DEV.dev);
    if (ret != N25Q256A_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: Initialization failed.\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static int32_t N25Q256A_Flash_Uninitialize(void)
{
    spi_n25q256a_uninitialize(ARM_FLASH0_DEV.dev);
    return ARM_DRIVER_OK;
}

static int32_t N25Q256A_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    enum n25q256a_error_t ret;

    if (N25Q256ADriverCapabilities.data_width > 2 || N25Q256ADriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    cnt *= data_width_byte[N25Q256ADriverCapabilities.data_width];

    ret = spi_n25q256a_read(ARM_FLASH0_DEV.dev, addr, data, cnt);
    if (ret != N25Q256A_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: read failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    cnt /= data_width_byte[N25Q256ADriverCapabilities.data_width];

    return cnt;
}

static int32_t N25Q256A_Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    enum n25q256a_error_t ret;

    if (N25Q256ADriverCapabilities.data_width > 2 || N25Q256ADriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    cnt *= data_width_byte[N25Q256ADriverCapabilities.data_width];

    ret = spi_n25q256a_program(ARM_FLASH0_DEV.dev, addr, data, cnt);
    if (ret != N25Q256A_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: program failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    cnt /= data_width_byte[N25Q256ADriverCapabilities.data_width];

    return cnt;
}

static int32_t N25Q256A_Flash_EraseSector(uint32_t addr)
{
    enum n25q256a_error_t ret;

    ret = spi_n25q256a_erase(ARM_FLASH0_DEV.dev, addr);
    if (ret != N25Q256A_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: erase failed: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static int32_t N25Q256A_Flash_EraseChip(void)
{
    enum n25q256a_error_t ret;

    ret = spi_n25q256a_erase_chip(ARM_FLASH0_DEV.dev);
    if (ret != N25Q256A_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: erase chip failed\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static ARM_FLASH_INFO * N25Q256A_Flash_GetInfo(void)
{
    return ARM_FLASH0_DEV.data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    ARM_Flash_GetVersion,
    N25Q256A_Driver_GetCapabilities,
    N25Q256A_Flash_Initialize,
    N25Q256A_Flash_Uninitialize,
    ARM_Flash_PowerControl,
    N25Q256A_Flash_ReadData,
    N25Q256A_Flash_ProgramData,
    N25Q256A_Flash_EraseSector,
    N25Q256A_Flash_EraseChip,
    ARM_Flash_GetStatus,
    N25Q256A_Flash_GetInfo
};

#endif /* RTE_FLASH0 */

#if (RTE_FLASH1)

static ARM_FLASH_CAPABILITIES SST26VF064B_Driver_GetCapabilities(void)
{
    return SST26VF064BDriverCapabilities;
}

static int32_t SST26VF064B_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);
    enum sst26vf064b_error_t ret;
    struct spi_sst26vf064b_dev_t* dev = ARM_FLASH1_DEV.dev;
    ARM_FLASH_INFO* data = ARM_FLASH1_DEV.data;

    dev->total_sector_cnt = data->sector_count;
    dev->page_size = data->page_size;
    dev->sector_size = data->sector_size;
    dev->program_unit = data->program_unit;

    ret = spi_sst26vf064b_initialize(ARM_FLASH1_DEV.dev);
    if (ret != SST26VF064B_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: Initialization failed.\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

static int32_t SST26VF064B_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    enum sst26vf064b_error_t ret;

    if (SST26VF064BDriverCapabilities.data_width > 2 || SST26VF064BDriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    cnt *= data_width_byte[SST26VF064BDriverCapabilities.data_width];

    ret = spi_sst26vf064b_read(ARM_FLASH1_DEV.dev, addr, data, cnt);
    if (ret != SST26VF064B_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: read failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    cnt /= data_width_byte[SST26VF064BDriverCapabilities.data_width];

    return cnt;
}

static int32_t SST26VF064B_Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
    enum sst26vf064b_error_t ret;

    if (SST26VF064BDriverCapabilities.data_width > 2 || SST26VF064BDriverCapabilities.data_width < 0)
    {
        SPI_FLASH_LOG_MSG("%s: Incorrect data width selected: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }

    cnt *= data_width_byte[SST26VF064BDriverCapabilities.data_width];

    ret = spi_sst26vf064b_program(ARM_FLASH1_DEV.dev, addr, data, cnt);
    if (ret != SST26VF064B_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: program failed: addr=0x%x, cnt=%u\n\r", __func__, addr, cnt);
        return ARM_DRIVER_ERROR;
    }

    cnt /= data_width_byte[SST26VF064BDriverCapabilities.data_width];

    return cnt;
}

static int32_t SST26VF064B_Flash_EraseSector(uint32_t addr)
{
    enum sst26vf064b_error_t ret;

    ret = spi_sst26vf064b_erase_sector(ARM_FLASH1_DEV.dev, addr);
    if (ret != SST26VF064B_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: erase failed: addr=0x%x\n\r", __func__, addr);
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

static int32_t SST26VF064B_Flash_EraseChip(void)
{
    enum sst26vf064b_error_t ret;

    ret = spi_sst26vf064b_erase_chip(ARM_FLASH1_DEV.dev);
    if (ret != SST26VF064B_ERR_NONE) {
        SPI_FLASH_LOG_MSG("%s: erase chip failed\n\r", __func__);
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

static ARM_FLASH_INFO * SST26VF064B_Flash_GetInfo(void)
{
    return ARM_FLASH1_DEV.data;
}

static int32_t SST26VF064B_Uninitialize(void)
{
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

ARM_DRIVER_FLASH Driver_FLASH1 = {
    ARM_Flash_GetVersion,
    SST26VF064B_Driver_GetCapabilities,
    SST26VF064B_Flash_Initialize,
    SST26VF064B_Uninitialize,
    ARM_Flash_PowerControl,
    SST26VF064B_Flash_ReadData,
    SST26VF064B_Flash_ProgramData,
    SST26VF064B_Flash_EraseSector,
    SST26VF064B_Flash_EraseChip,
    ARM_Flash_GetStatus,
    SST26VF064B_Flash_GetInfo
};
#endif /* RTE_FLASH1 */

#endif /* PLATFORM_IS_FVP */
