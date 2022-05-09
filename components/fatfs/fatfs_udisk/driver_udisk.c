#include <stdio.h>
#include <string.h>
#include <common/bk_include.h>
#include "driver_udisk.h"
#include <os/os.h>
#include "bk_uart.h"
//#include "usb_msd.h"
#include "diskio.h"

#if CONFIG_USB_HOST
extern uint8_t MUSB_GetConnect_Flag(void);
#if CONFIG_USB_MSD
extern uint8_t MGC_MsdGetMediumstatus(void);
#endif

uint8 udisk_is_attached(void)
{
    return MUSB_GetConnect_Flag();
}

uint8 udisk_init(void)
{
	uint32 ret = USB_RET_ERROR;

	FAT_PRT("udisk_init_wzl\r\n");

#if CONFIG_USB_MSD

	while (1)
	{
		if (MGC_MsdGetMediumstatus()) {
			ret = USB_RET_OK;
			break;
		} else {
			rtos_delay_milliseconds(100);
			bk_printf("need plug in usb device\r\n");
			ret = USB_RET_DISCONNECT;
			break;
		}
	}
#endif
	return ret;
}



int udisk_rd_blk_sync(uint32 first_block, uint32 block_num, uint8 *dest )
{
    int ret = USB_RET_ERROR;

	FAT_PRT("disk_rd:%d:%d\r\n", first_block, block_num);
#if CONFIG_USB_MSD
    if (!MGC_MsdGetMediumstatus())
    {
    	os_printf("disk_rd_failed\r\n");
        return ret;
    }

	#ifdef CFG_ENABLE_SYC_OP
    ret = MUSB_HfiRead_sync(first_block, block_num, dest);
	#endif
#endif

    return ret;
}

int udisk_wr_blk_sync(uint32 first_block, uint32 block_num, uint8 *dest)
{
    int ret = USB_RET_ERROR;
	
	FAT_PRT("disk_wr:%d:%d\r\n", first_block, block_num);
#if CONFIG_USB_MSD
    if (!MGC_MsdGetMediumstatus())
    {
    	os_printf("disk_wr_failed\r\n");
        return ret;
    }

	#ifdef CFG_ENABLE_SYC_OP
    ret = MUSB_HfiWrite_sync(first_block, block_num, dest);
	#endif
#endif
    return ret;
}

uint32 udisk_get_size(void)
{
    return 0;//driver_udisk.total_block;
}

#endif

