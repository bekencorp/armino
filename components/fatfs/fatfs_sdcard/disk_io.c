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

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_SD		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#if CONFIG_SDCARD_HOST
DD_HANDLE sdcard_hdl = DD_HANDLE_UNVALID;
#endif

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
	UINT status = RES_NOTRDY;

	switch (pdrv) {
	case DEV_RAM:
		return stat;
	case DEV_SD :
			sdcard_hdl = ddev_open(DD_DEV_TYPE_SDCARD, &status, 0);
			os_printf("sdcard_hdl = 0x%x, status=%d\r\n", sdcard_hdl, status);
			stat = (DSTATUS)status;
			if(stat != FR_OK)
			{
				if(sdcard_hdl != DD_HANDLE_UNVALID)
				{
					stat = RES_OK;
					os_printf("sdcard has initialized\r\n");
				}
				else
				{
					os_printf("disk_initialize error\r\n");
					stat = RES_NOTRDY;
				}
			}
			else
			{
				os_printf("disk_initialize suc\r\n");
				stat = RES_OK;
			}

		return stat;

	case DEV_USB :
		return stat;
		
	default:
		break;
	}
	return STA_NOINIT;
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
#if CONFIG_SDCARD_HOST
		// translate the arguments here
		if(sdcard_hdl!=DD_HANDLE_UNVALID)
		{
			result =  ddev_read(sdcard_hdl, (char *)buff, count, sector);
			if(result == RES_OK)
				res = RES_OK;
		}
#endif
		return res;

	case DEV_USB :
		return res;
		
	default:
		break;
	}

	return RES_PARERR;
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
	int result;

	switch (pdrv) {
	case DEV_RAM:
		return res;
		
	case DEV_SD :
#if CONFIG_SDCARD_HOST
		// translate the arguments here
		if(sdcard_hdl!=DD_HANDLE_UNVALID)
		{
			result =  ddev_write(sdcard_hdl, (char *)buff, count, sector);
			if(result == RES_OK)
				res = RES_OK;
		}
#endif
		return res;

	case DEV_USB :
		// translate the arguments here
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
	DRESULT res = FR_OK;

	switch (pdrv) {
	case DEV_SD :
		// Process of the command for the MMC/SD card
		switch(cmd)
		{
		case CTRL_SYNC:
			os_printf("not support CTRL_SYNC \r\n");
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
			*(DWORD *)buff = sdcard_get_size();
			os_printf("sdcard sector cnt=%d\r\n", *(DWORD *)buff);
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
		}

		return res;

	case DEV_USB :
		// Process of the command the USB drive
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
	UINT32 result=RES_ERROR;
	switch (pdrv) {
	case DEV_RAM:
		return result;

	case DEV_SD :
#if CONFIG_SDCARD_HOST		
		 result = ddev_close(sdcard_hdl);
		sdcard_hdl = DD_HANDLE_UNVALID;
#endif
		break;

	case DEV_USB :
		break;
		
	default:
		break;
	}
	if(result != DRV_SUCCESS)
		stat = RES_ERROR;
	else
		stat = RES_OK;
	
	return stat;
}
// eof

