#include <stdio.h>
#include <string.h>
#include <common/bk_include.h>
#include "driver_udisk.h"
#include <os/os.h>
#include "bk_uart.h"
#include "diskio.h"
#include <modules/usb.h>

#if CONFIG_USB_HOST
#include "bk_usb.h"
extern uint8_t MUSB_GetConnect_Flag(void);
#if CONFIG_USB_MSD
extern void MUSB_Host_init(void);
extern uint8_t MGC_MsdGetMediumstatus(void);
extern uint32_t MUSB_NoneRunBackground(void);
#endif

uint8 udisk_is_attached(void)
{
    return MUSB_GetConnect_Flag();
}

uint8 udisk_init(void)
{
	uint32 ret = USB_RET_ERROR;

	FATFS_LOGI("udisk_init\r\n");

#if CONFIG_USB_MSD
	if (MGC_MsdGetMediumstatus()) {
		ret = USB_RET_OK;
	}
#elif CONFIG_USB_DEVICE
	{
		//MUSB_Host_init();

		while (1)
		{
			ret = MUSB_NoneRunBackground();
			FATFS_LOGI("udisk_init: ret = 0x%lx\r\n", ret);
			if (MGC_MsdGetMediumstatus()) {
				ret = USB_RET_OK;
				break;
			} else {
				if((USB_RET_DISCONNECT == ret) || (USB_RET_ERROR == ret)) {
					rtos_delay_milliseconds(100);
					FATFS_LOGI("need plug in usb device\r\n");
					break;
				}
			}
		}
	}
#endif
	return ret;
}

int udisk_rd_blk_sync(uint32 first_block, uint32 block_num, uint8 *dest )
{
    int ret = USB_RET_ERROR;

    FATFS_LOGD("disk_rd:%d:%d\r\n", first_block, block_num);
#if CONFIG_USB_MSD
    if (!MGC_MsdGetMediumstatus())
    {
        FATFS_LOGI("disk_rd_failed\r\n");
        return ret;
    }

    ret = usb_read(first_block, dest, block_num);
#endif
    FATFS_LOGD("%s: ret=%d\r\n", __func__, ret);

    return ret;
}

int udisk_wr_blk_sync(uint32 first_block, uint32 block_num, uint8 *dest)
{
    int ret = USB_RET_ERROR;

    FATFS_LOGD("disk_wr:%d:%d\r\n", first_block, block_num);
#if CONFIG_USB_MSD
    if (!MGC_MsdGetMediumstatus())
    {
        FATFS_LOGI("disk_wr_failed\r\n");
        return ret;
    }
    ret = usb_write(first_block, dest, block_num);
#endif
    FATFS_LOGD("%s: ret=%d\r\n", __func__, ret);

    return ret;
}

uint32 udisk_get_size(void)
{
    return 0;//driver_udisk.total_block;
}

#endif

