// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include <stdint.h>
#include "Driver_Flash.h"
#include "platform_retarget.h"
#include "RTE_Device.h"
#include "driver/flash.h"
#include "components/log.h"
#include "common/bk_err.h"
#include "bk_tfm_log.h"

#define TAG "cmsis_flash"

#define BK_TFM_FLASH_LOGD BK_LOGD
#define BK_TFM_FLASH_LOGI BK_LOGI
#define BK_TFM_FLASH_LOGW BK_LOGW
#define BK_TFM_FLASH_LOGE BK_LOGE

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION      ARM_DRIVER_VERSION_MAJOR_MINOR(1, 1)
#define ARM_FLASH_DRV_ERASE_VALUE  0xFF

#define FLASH_CBUS_ADDR_FLAG          (1<<31)
#define FLASH_CLR_CBUS_ADDR_FLAG(x)   (x) &= ~FLASH_CBUS_ADDR_FLAG

/* these flash api MUST at the spe. For flash controller/device is exclusive,
 * it is used at the S/NS world. ns--nsc--s
 */
#define FLASH_SPE_PPRO_DECLARATION() uint32_t ppro_flash_ns_flag

/* TODO wangzhilei.switch to flash spe*/
#define FLASH_SPE_PPRO_LOCK() do {\
	uint32_t reg;\
	reg = *((volatile uint32_t *)(0x41040000 + 5 * 4));\
	ppro_flash_ns_flag = reg & 0x04;\
	*((volatile uint32_t *)(0x41040000 + 5 * 4)) = reg & (~0x04);\
}while(0)

/* restore ppro reg0x5 bit[2], 1--ns, 0--s*/
#define FLASH_SPE_PPRO_UNLOCK() do {\
	uint32_t reg;\
	reg = *((volatile uint32_t *)(0x41040000 + 5 * 4));\
	if(ppro_flash_ns_flag){\
		*((volatile uint32_t *)(0x41040000 + 5 * 4)) = reg | 0x04;\
	}\
}while(0)

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

static uint32_t s_flash_sector_count = 0;

/*
 * ARM FLASH device structure
 *
 * There is no real flash memory for code on MPS2 board. Instead a code SRAM is
 * used for code storage: ZBT SSRAM1. This driver just emulates a flash
 * interface and behaviour on top of the SRAM memory.
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

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    0, /* event_ready */
    0, /* data_width = 0:8-bit, 1:16-bit, 2:32-bit */
    1  /* erase_chip */
};

static bool is_access_from_code_bus(uint32_t absolute_addr)
{
	/*true condition:
	  0, disble flash write protection;
	  1, enable flash cpu write;*/
	#define CONFIG_CODE_BUS_OPS_FLASH 0
	#if CONFIG_CODE_BUS_OPS_FLASH  
	return true;
	#else
    return false;
	#endif
}

static uint32_t flash_sector_count(void)
{
    return s_flash_sector_count;
}

static int32_t is_range_valid(struct arm_flash_dev_t *flash_dev,
                              uint32_t offset)
{
    uint32_t flash_limit = 0;
    int32_t rc = 0;

    flash_limit = (flash_sector_count() * flash_dev->data->sector_size)
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
    .memory_base = FLASH0_BASE_NS,
#else
    .memory_base = FLASH0_BASE_S,
#endif /* __DOMAIN_NS == 1 */
    .data        = &(ARM_FLASH0_DEV_DATA)};

struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

/*
 * Functions
 */

static ARM_DRIVER_VERSION Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

static int32_t Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
	FLASH_SPE_PPRO_DECLARATION();
    uint32_t flash_size = 0;
    ARG_UNUSED(cb_event);

    FLASH_SPE_PPRO_LOCK();
    BK_LOG_ON_ERR(bk_flash_driver_init());
    bk_flash_set_protect_type(FLASH_PROTECT_NONE); //TODO wangzhilei double check

    flash_size = bk_flash_get_current_total_size();

    /* Optimze it if we support more than one flash */
    s_flash_sector_count = flash_size / FLASH0_SECTOR_SIZE;

    FLASH_SPE_PPRO_UNLOCK();

    return ARM_DRIVER_OK;
}

static int32_t Flash_Uninitialize(void)
{
    FLASH_SPE_PPRO_DECLARATION();

    FLASH_SPE_PPRO_LOCK();
    BK_LOG_ON_ERR(bk_flash_driver_deinit());
    FLASH_SPE_PPRO_UNLOCK();

    return ARM_DRIVER_OK;
}

static int32_t Flash_PowerControl(ARM_POWER_STATE state)
{
    return ARM_DRIVER_OK;
}

static int32_t Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
	FLASH_SPE_PPRO_DECLARATION();
    bool use_cbus = false;
    int32_t rc = 0;

    BK_TFM_FLASH_LOGD(TAG, "read off=%x cnt=%x\r\n", addr, cnt);

    if (cnt == 0) {
        BK_TFM_FLASH_LOGW(TAG, "read 0B\r\n");
        return ARM_DRIVER_OK;
    }

    use_cbus = is_access_from_code_bus(addr);
    FLASH_CLR_CBUS_ADDR_FLAG(addr);

    /* CMSIS ARM_FLASH_ReadData API requires the `addr` data type size aligned.
     * Data type size is specified by the data_width in ARM_FLASH_CAPABILITIES.
     */
    if (addr % data_width_byte[DriverCapabilities.data_width] != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check flash memory boundaries */
    rc = is_range_valid(FLASH0_DEV, addr + cnt);
    if (rc != 0) {
        BK_TFM_FLASH_LOGE(TAG, "invalid addr range, addr=%x cnt=%x\r\n", addr, cnt);
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    if (use_cbus) {
        BK_TFM_FLASH_LOGD(TAG, "flash cbus read, addr=%x cnt=%x+\r\n", FLASH0_DEV->memory_base + addr, cnt);
        memcpy(data, (void *)(FLASH0_DEV->memory_base + addr), cnt);
        BK_TFM_FLASH_LOGD(TAG, "flash cbus read-\r\n");
    } else {
        FLASH_SPE_PPRO_LOCK();
        BK_LOG_ON_ERR(bk_flash_read_bytes(addr, data, cnt));
        FLASH_SPE_PPRO_UNLOCK();
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[DriverCapabilities.data_width];

    BK_TFM_DUMP_BUF("cmsis flash read", data, cnt);
    return cnt;
}

static int32_t Flash_ProgramData(uint32_t addr, const void *data,
                                     uint32_t cnt)
{
	FLASH_SPE_PPRO_DECLARATION();
    bool use_cbus = false;
    int32_t rc = 0;

    BK_TFM_FLASH_LOGD(TAG, "write off=%x cnt=%x base=%x\r\n", addr, cnt, FLASH0_DEV->memory_base);

    if (cnt == 0) {
        BK_TFM_FLASH_LOGW(TAG, "write 0B\r\n");
        return ARM_DRIVER_OK;
    }
    use_cbus = is_access_from_code_bus(addr);
    FLASH_CLR_CBUS_ADDR_FLAG(addr);

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
    if (use_cbus) {
        memcpy((uint8_t*)(FLASH0_DEV->memory_base + addr), (uint8_t*)data, cnt);
    } else {
        FLASH_SPE_PPRO_LOCK();
        rc = is_flash_ready_to_write((const uint8_t*)addr, cnt);
        BK_LOG_ON_ERR(bk_flash_write_bytes(addr, data, cnt));
        FLASH_SPE_PPRO_UNLOCK();
    }

    /* Conversion between bytes and data items */
    cnt /= data_width_byte[DriverCapabilities.data_width];

    return cnt;
}

static int32_t Flash_EraseSector(uint32_t addr)
{
	FLASH_SPE_PPRO_DECLARATION();
    uint32_t offset = addr;
    uint32_t rc = 0;

    BK_TFM_FLASH_LOGD(TAG, "flash erase off=%x\r\n", addr);

    rc  = is_range_valid(FLASH0_DEV, addr);
    rc |= is_sector_aligned(FLASH0_DEV, addr);
    if (rc != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    FLASH_SPE_PPRO_LOCK();
    BK_LOG_ON_ERR(bk_flash_erase_sector(offset));
    FLASH_SPE_PPRO_UNLOCK();

    return ARM_DRIVER_OK;
}

static int32_t Flash_EraseChip(void)
{
    uint32_t i;
    uint32_t addr = FLASH0_DEV->memory_base;
    int32_t rc = ARM_DRIVER_ERROR_UNSUPPORTED;
    uint32_t offset = 0;

    FLASH_SPE_PPRO_DECLARATION();
    /* Check driver capability erase_chip bit */
    FLASH_SPE_PPRO_LOCK();
    if (DriverCapabilities.erase_chip == 1) {
        for (i = 0; i < flash_sector_count(); i++) {
                offset = addr - FLASH0_DEV->memory_base;
                BK_LOG_ON_ERR(bk_flash_erase_sector(addr)); //TODO double check address offset

            addr += FLASH0_DEV->data->sector_size;
            rc = ARM_DRIVER_OK;
        }
    }
    FLASH_SPE_PPRO_UNLOCK();

    return rc;
}

static ARM_FLASH_STATUS Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO * Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    Flash_GetVersion,
    Flash_GetCapabilities,
    Flash_Initialize,
    Flash_Uninitialize,
    Flash_PowerControl,
    Flash_ReadData,
    Flash_ProgramData,
    Flash_EraseSector,
    Flash_EraseChip,
    Flash_GetStatus,
    Flash_GetInfo
};
#endif /* RTE_FLASH0 */
