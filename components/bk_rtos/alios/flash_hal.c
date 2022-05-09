#include "beken_hal.h"
#include <os/os.h>
#include "bk_flash.h"
#include "bk_drv_model.h"
#include "BkDriverFlash.h"

int bk_flash_enable_security(PROTECT_TYPE type)
{
	DD_HANDLE flash_hdl;
	UINT32 status;
	uint32_t param = type;

	flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	BK_ASSERT(DD_HANDLE_UNVALID != flash_hdl);
	ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, (void *)&param);

	return kNoErr;
}

bk_logic_partition_t *bk_flash_get_info(bk_partition_t inPartition)
{
	hal_logic_partition_t *logic_partition;

	if (inPartition == BK_PARTITION_RF_FIRMWARE)
		logic_partition = hal_flash_get_info(HAL_PARTITION_RF_FIRMWARE);
	else if (inPartition == BK_PARTITION_NET_PARAM)
		logic_partition = hal_flash_get_info(HAL_PARTITION_PARAMETER_4);
	else
		logic_partition = NULL;

	return (bk_logic_partition_t *)logic_partition;
}

bk_err_t bk_flash_erase(bk_partition_t inPartition, uint32_t off_set, uint32_t size)
{
	uint32_t i;
	uint32_t param;
	UINT32 status;
	DD_HANDLE flash_hdl;
	uint32_t start_sector, end_sector;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_get_info(inPartition);
	start_sector = off_set >> 12;
	end_sector = (off_set + size - 1) >> 12;

	flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	BK_ASSERT(DD_HANDLE_UNVALID != flash_hdl);
	for (i = start_sector; i <= end_sector; i ++) {
		param = partition_info->partition_start_addr + (i << 12);
		GLOBAL_INT_DISABLE();
		ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, (void *)&param);
		GLOBAL_INT_RESTORE();
	}

	return kNoErr;
}

bk_err_t bk_flash_write(bk_partition_t inPartition, volatile uint32_t off_set, uint8_t *inBuffer, uint32_t inBufferLength)
{
	UINT32 status;
	DD_HANDLE flash_hdl;
	uint32_t start_addr;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();

	BK_ASSERT(inBuffer);

	partition_info = bk_flash_get_info(inPartition);
	start_addr = partition_info->partition_start_addr + off_set;

	flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	BK_ASSERT(DD_HANDLE_UNVALID != flash_hdl);

	GLOBAL_INT_DISABLE();
	ddev_write(flash_hdl, (char *)inBuffer, inBufferLength, start_addr);
	GLOBAL_INT_RESTORE();

	return kNoErr;
}

bk_err_t bk_flash_read(bk_partition_t inPartition, volatile uint32_t off_set, uint8_t *outBuffer, uint32_t inBufferLength)
{
	UINT32 status;
	uint32_t start_addr;
	DD_HANDLE flash_hdl;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();

	BK_ASSERT(outBuffer);

	partition_info = bk_flash_get_info(inPartition);
	start_addr = partition_info->partition_start_addr + off_set;

	flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	BK_ASSERT(DD_HANDLE_UNVALID != flash_hdl);

	GLOBAL_INT_DISABLE();
	ddev_read(flash_hdl, (char *)outBuffer, inBufferLength, start_addr);
	GLOBAL_INT_RESTORE();

	return kNoErr;
}

