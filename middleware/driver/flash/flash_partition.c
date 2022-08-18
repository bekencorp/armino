// Copyright 2020-2021 Beken
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

#include <os/mem.h>
#include <driver/flash.h>
#include <driver/flash_partition.h>
#include "flash_driver.h"
#include "flash_hal.h"

#if CONFIG_FLASH_ORIGIN_API
#define PAR_OPT_READ_POS      (0)
#define PAR_OPT_WRITE_POS     (1)

#define PAR_OPT_READ_DIS      (0x0u << PAR_OPT_READ_POS)
#define PAR_OPT_READ_EN       (0x1u << PAR_OPT_READ_POS)
#define PAR_OPT_WRITE_DIS     (0x0u << PAR_OPT_WRITE_POS)
#define PAR_OPT_WRITE_EN      (0x1u << PAR_OPT_WRITE_POS)
#endif

/* Logic partition on flash devices */
#if (CONFIG_SOC_BK7256XX)
extern const bk_logic_partition_t bk7256_partitions[BK_PARTITION_MAX];
#else
static const bk_logic_partition_t bk7231_partitions[BK_PARTITION_MAX] = {
	[BK_PARTITION_BOOTLOADER] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Bootloader",
		.partition_start_addr      = 0x00000000,
		.partition_length          = 0x0F000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_APPLICATION] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "Application",
		.partition_start_addr      = 0x11000,
#if CONFIG_SUPPORT_MATTER	|| 	 CONFIG_FLASH_SIZE_4M
		.partition_length          = 0x1A9000,
#else
		.partition_length          = 0x143000,
#endif
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_OTA] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "ota",
#if CONFIG_FLASH_SIZE_4M
		.partition_start_addr      = 0x1BA000,
		.partition_length          = 0x1A9000, //1700KB
#elif CONFIG_SUPPORT_MATTER
		.partition_start_addr      = 0x1BA000,
		.partition_length          = 0x11000, //68KB
#else
		.partition_start_addr      = 0x132000,
		.partition_length          = 0xAE000, //696KB
#endif
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
#if CONFIG_SUPPORT_MATTER
	[BK_PARTITION_MATTER_FLASH] =
	{
		.partition_owner		   = BK_FLASH_EMBEDDED,
		.partition_description	   = "Matter",
		#if CONFIG_FLASH_SIZE_4M
		.partition_start_addr	   = 0x363000,
		#else
		partition_start_addr	   = 0x1CB000,
		#endif
		.partition_length		   = 0x15000, //84KB
		.partition_options		   = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
#endif
	[BK_PARTITION_RF_FIRMWARE] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "RF Firmware",
#if (CONFIG_SOC_BK7251)
		.partition_start_addr      = 0x1e0000,// bootloader unused space for rf cal+mac related info.
#elif (CONFIG_SOC_BK7271)
		.partition_start_addr      = 0x3FE000,
#else
#if (CONFIG_FLASH_SIZE_4M)
		.partition_start_addr      = 0x3FE000,
#else
		.partition_start_addr      = 0x1e0000,// for rf related info
#endif
#endif
		.partition_length          = 0x1000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	[BK_PARTITION_NET_PARAM] =
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "NET info",
#if (CONFIG_SOC_BK7251)
		.partition_start_addr      = 0x1FF000,// for net related info
#elif (CONFIG_SOC_BK7271)
		.partition_start_addr      = 0x3FF000,
#else
#if (CONFIG_FLASH_SIZE_4M)
		.partition_start_addr      = 0x3FF000,
#else
		.partition_start_addr      = 0x1e1000,// for net related info
#endif
#endif
		.partition_length          = 0x1000,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
};
#endif

static bool flash_partition_is_valid(bk_partition_t partition)
{
	if ((partition >= BK_PARTITION_BOOTLOADER) && (partition < BK_PARTITION_MAX)) {
		return true;
	} else {
		return false;
	}
}

bk_logic_partition_t *bk_flash_partition_get_info(bk_partition_t partition)
{
	bk_logic_partition_t *pt = NULL;

	BK_ASSERT(BK_PARTITION_BOOTLOADER < BK_PARTITION_MAX);

	if (flash_partition_is_valid(partition)) {
#if (CONFIG_SOC_BK7256XX)
		pt = (bk_logic_partition_t *)&bk7256_partitions[partition];
#else
		pt = (bk_logic_partition_t *)&bk7231_partitions[partition];
#endif
	}
	return pt;
}

bk_err_t bk_flash_partition_erase(bk_partition_t partition, uint32_t offset, uint32_t size)
{
	uint32_t erase_addr = 0;
	uint32_t start_sector, end_sector = 0;
	bk_logic_partition_t *partition_info = NULL;
	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_partition_get_info(partition);
	start_sector = offset >> FLASH_SECTOR_SIZE_OFFSET; /* offset / FLASH_SECTOR_SIZE */
	end_sector = (offset + size - 1) >> FLASH_SECTOR_SIZE_OFFSET;

	for (uint32_t i = start_sector; i <= end_sector; i ++) {
		erase_addr = partition_info->partition_start_addr + (i << FLASH_SECTOR_SIZE_OFFSET);
		GLOBAL_INT_DISABLE();
		bk_flash_erase_sector(erase_addr);
		GLOBAL_INT_RESTORE();
	}

	return BK_OK;
}

bk_err_t bk_flash_partition_write(bk_partition_t partition, const uint8_t *buffer, uint32_t offset, uint32_t buffer_len)
{
	BK_RETURN_ON_NULL(buffer);

	uint32_t start_addr;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_partition_get_info(partition);
	if (NULL == partition_info) {
		FLASH_LOGW("%s partition not found\r\n", __func__);
		return BK_ERR_FLASH_PARTITION_NOT_FOUND;
	}

	start_addr = partition_info->partition_start_addr + offset;
	GLOBAL_INT_DISABLE();
	bk_flash_write_bytes(start_addr, buffer, buffer_len);
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_flash_partition_read(bk_partition_t partition, uint8_t *out_buffer, uint32_t offset, uint32_t buffer_len)
{
	BK_RETURN_ON_NULL(out_buffer);

	uint32_t start_addr;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_partition_get_info(partition);
	if (NULL == partition_info) {
		FLASH_LOGW("%s partiion not found\r\n", __func__);
		return BK_ERR_FLASH_PARTITION_NOT_FOUND;
	}

	start_addr = partition_info->partition_start_addr + offset;
	GLOBAL_INT_DISABLE();
	bk_flash_read_bytes(start_addr, out_buffer, buffer_len);
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

