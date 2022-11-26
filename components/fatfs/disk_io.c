/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include <common/bk_include.h>


#include "ff.h"

#include "sdcard.h"
#include "sdcard_pub.h"

#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "diskio.h"		/* FatFs lower layer API */

#include "bk_uart.h"
#include "driver_udisk.h"
#if CONFIG_SDCARD_V2P0
#include "driver/sd_card.h"
#endif

#include <driver/flash.h>
#include <driver/flash_partition.h>

#define FLASH_SECTOR_SIZE  512

/* Definitions of physical drive number for each drive */
#define DEV_RAM		DISK_NUMBER_RAM	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_SD		DISK_NUMBER_SDIO_SD	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		DISK_NUMBER_UDISK	/* Example: Map USB MSD to physical drive 2 */
#define DEV_FLASH	DISK_NUMBER_FLASH	/* Example: Map USB MSD to physical drive 3 */

DD_HANDLE sdcard_hdl = DD_HANDLE_UNVALID;

//static DD_HANDLE usb_hdl =DD_HANDLE_UNVALID;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM:
		return stat;

	case DEV_SD :
		stat = RES_OK;
		return stat;

	case DEV_USB :
		stat = RES_OK;
		return stat;

	case DEV_FLASH :
		stat = RES_OK;
		return stat;
		
	default:
		break;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
#if (CONFIG_SDCARD_V1P0)
	UINT status = RES_NOTRDY;
#endif
	switch (pdrv) {
	case DEV_RAM:
		return stat;
	case DEV_SD :
#if (CONFIG_SDCARD_V2P0)
		stat = bk_sd_card_init();
#elif (CONFIG_SDCARD_V1P0)
		sdcard_hdl = ddev_open(DD_DEV_TYPE_SDCARD, &status, 0);
		FATFS_LOGI("sdcard_hdl = 0x%x, status=%d\r\n", sdcard_hdl, status);
		stat = (DSTATUS)status;
		if(stat != FR_OK) {
			if(sdcard_hdl != DD_HANDLE_UNVALID) {
				stat = RES_OK;
				FATFS_LOGI("sdcard has initialized\r\n");
			} else {
				FATFS_LOGI("disk_initialize error\r\n");
				stat = RES_NOTRDY;
			}
		} else {
			FATFS_LOGI("disk_initialize suc\r\n");
			stat = RES_OK;
		}
#endif
		return stat;

	case DEV_USB :
#if (CONFIG_USB_HOST && CONFIG_USB_MSD)
		if(udisk_init() == USB_RET_OK)
			stat = RES_OK;
#endif
		return stat;

	case DEV_FLASH :
		stat = RES_OK;
		return stat;

	default:
		break;
	}
	return STA_NOINIT;
}

DSTATUS disk_close(void)
{
    FATFS_LOGI("disk_close\r\n");
    ddev_close(sdcard_hdl);

    sdcard_hdl = DD_HANDLE_UNVALID;
    return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_ERROR;
	int result;

	switch (pdrv) {
	case DEV_RAM:
		return res;
		
	case DEV_SD :
#if (CONFIG_SDCARD_V2P0)
		res = bk_sd_card_read_blocks((uint8_t *)buff, sector, count);
#elif (CONFIG_SDCARD_V1P0)
		// translate the arguments here
		if(sdcard_hdl!=DD_HANDLE_UNVALID) {
			result =  ddev_read(sdcard_hdl, (char *)buff, count, sector);
			if(result == RES_OK)
				res = RES_OK;
		}
#endif
		if(res != RES_OK) {
			FATFS_LOGI("%s ERROR res:%d\r\n", __func__, res);
			bk_sd_card_deinit();
			bk_sd_card_init();
		}

		return res;

	case DEV_USB :
#if (CONFIG_USB_HOST && CONFIG_USB_MSD)
		res = udisk_rd_blk_sync(sector, count, buff);
		if (res != USB_RET_OK) {
			FATFS_LOGI("dev usb disk_read_error res:%d\r\n", res);
			res = RES_ERROR;
		} else
			res = RES_OK;
#endif
		return res;

	case DEV_FLASH :
		result = bk_flash_partition_read(BK_PARTITION_USR_CONFIG, buff,
				sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE * count);
		if(result == BK_OK)
			res = RES_OK;
		return res;

	default:
		break;
	}

	return RES_PARERR;
}

static DRESULT sd_disk_check_space_size()
{
	DRESULT res = RES_ERROR;
	FATFS *checkspace_pfs = NULL;
	char cFileName[FF_MAX_LFN];
	DWORD freenclst;
	sprintf(cFileName, "%d:", 1);

	FATFS_LOGD("[+]%s\r\n", __func__);

	res = f_getfree(cFileName, &freenclst, &checkspace_pfs);
	if(res != RES_OK)
		return res;

	if(freenclst < SD_MIN_NUMBER_REMAINING_CLUSTERS) {
		FATFS_LOGI("The space is insufficient\r\n");
		FATFS_LOGI("freenclst: %d free_mem:%d MB\r\n",freenclst, freenclst/SD_CLUSTER_TO_MEM_64KB);
		return RES_NOTRDY;
	}

	FATFS_LOGD("[-]%s\r\n", __func__);

	return res;
}
/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	BYTE *buff,			/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_ERROR;

#if (CONFIG_SDCARD_V1P0)
	int result;
#endif
	switch (pdrv) {
	case DEV_RAM:
		return res;

	case DEV_SD :
#if (CONFIG_SDCARD_V2P0)
		res = bk_sd_card_write_blocks((uint8_t *)buff, sector, count);
		if(res != RES_OK) {
			FATFS_LOGI("Check the remaining space!\r\n");
			FATFS_LOGI("Get the value of the remaining space. res: %d\r\n", sd_disk_check_space_size());
			bk_sd_card_deinit();
			bk_sd_card_init();
		}
#elif (CONFIG_SDCARD_V1P0)
		// translate the arguments here
		if(sdcard_hdl!=DD_HANDLE_UNVALID) {
			result =  ddev_write(sdcard_hdl, (char *)buff, count, sector);
			if(result == RES_OK)
				res = RES_OK;
		}
#endif

		return res;

	case DEV_USB :
#if (CONFIG_USB_HOST && CONFIG_USB_MSD)
		// translate the arguments here
		if (udisk_wr_blk_sync(sector, count, buff) !=  USB_RET_OK) {
			FATFS_LOGI("dev usb disk_write_error\r\n");
			res =	RES_ERROR;
		} else
			res = RES_OK;
#endif
		return res;

	case DEV_FLASH :
		// Fatfs flash read only
		return res;
	default:
		break;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
extern int sdcard_get_size( void );

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	bk_logic_partition_t *partition_info;
	DRESULT res = FR_OK;
#if (CONFIG_SDCARD_V1P0)
	int result;
#endif
	switch (pdrv) {
	case DEV_SD :
		// Process of the command for the MMC/SD card
		switch(cmd)
		{
		case CTRL_SYNC:
#if (CONFIG_SDCARD_V2P0)
			//res = sd_card_cmd_stop_transmission();
			os_printf("TODO:FATFS sync feature\r\n");
			res = RES_OK;
#elif (CONFIG_SDCARD_V1P0)
			//os_printf("not support CTRL_SYNC \r\n");
			if(sdcard_hdl!=DD_HANDLE_UNVALID)
			{
				result =  ddev_control(sdcard_hdl, 0, 0);
				if(result != RES_OK)
					res = result;
			}
			res = RES_OK;
#endif
			break;
		case GET_SECTOR_SIZE:
			*(WORD *)buff = 512;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(WORD *)buff = 8;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
#if (CONFIG_SDCARD_V2P0)
			*(DWORD *)buff = (uint32_t)bk_sd_card_get_card_size();	
			res = RES_OK;
			os_printf("sdcard sector cnt=%d\r\n", *(DWORD *)buff);
#elif (CONFIG_SDCARD_V1P0)
			*(DWORD *)buff = sdcard_get_size();
			os_printf("sdcard sector cnt=%d\r\n", *(DWORD *)buff);
			res = RES_OK;
#endif
			break;
		default:
			res = RES_PARERR;
			break;
		}

		return res;

	case DEV_USB :
#if (CONFIG_USB_HOST && CONFIG_USB_MSD)
		// Process of the command the USB drive
        switch(cmd)
        {
        case CTRL_SYNC:
            //    if (SD_WaitReady()==0)res = RES_OK;
            //   else res = RES_ERROR;
            //	Delay(10000);
            //		    	printf("CTRL_SYNC \r\n");
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = 512;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD *)buff = 8;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = udisk_get_size();
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
#endif
		return res;

	case DEV_FLASH :
		switch(cmd)
		{
		case GET_SECTOR_SIZE:
			*(WORD *)buff = FLASH_SECTOR_SIZE;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(WORD *)buff = FLASH_SECTOR_SIZE;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			partition_info = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
			if (NULL == partition_info) {
				os_printf("%s partiion not found.\r\n", __func__);
				break;
			}
			*(DWORD *)buff = partition_info->partition_length / FLASH_SECTOR_SIZE;
			os_printf("sdcard sector cnt=%d\r\n", *(DWORD *)buff);
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
		}

		return res;

	default:
		break;

	}

	return RES_PARERR;
}

DSTATUS disk_uninitialize ( BYTE pdrv/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	UINT32 result = RES_ERROR;
	switch (pdrv) {
	case DEV_RAM:
		return result;

	case DEV_SD :
#if (CONFIG_SDCARD_V2P0)
	result = bk_sd_card_deinit();
	stat = RES_OK;
#elif (CONFIG_SDCARD_V1P0)
	result = ddev_close(sdcard_hdl);
	sdcard_hdl = DD_HANDLE_UNVALID;
#endif
		break;

	case DEV_USB :
		break;

	case DEV_FLASH :
		stat = RES_OK;
		return stat;

	default:
		break;
	}
	if(result != DRV_SUCCESS)
		stat = RES_ERROR;
	else
		stat = RES_OK;
	
	return stat;
}

uint8 media_is_online(void)
{
#if (CONFIG_USB_HOST && CONFIG_USB_MSD)
    return udisk_is_attached();
#endif
	return RES_OK;
}

DRESULT disk_unmount(uint8 pdrv)
{
        //udisk_uninit();
        return RES_OK;
}

// eof

