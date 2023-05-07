#include <stdio.h>
#include <string.h>
#include <os/os.h>
#include <common/bk_include.h>
#include "ff.h"
#include "modules/lvgl/lv_fatfs.h"
#include "modules/lvgl/lv_conf.h"


#if (CONFIG_FATFS) && (LV_USE_FS_FATFS)
FATFS *fs = NULL;
static void lv_fatfs_mount(int number)
{
	FRESULT fr;
    char cFileName[FF_MAX_LFN];

    if (fs != NULL)
    {
        os_free(fs);
    }

    fs = os_malloc(sizeof(FATFS));
	if(NULL == fs)
	{
		os_printf("f_mount malloc failed!\r\n");
		goto failed_mount;
	}

    sprintf(cFileName, "%d:", number);
    fr = f_mount(fs, cFileName, 1);
    if (fr != FR_OK)
    {
        os_printf("f_mount failed:%d\r\n", fr);
    }
    else
    {
        os_printf("f_mount OK!\r\n");
    }

failed_mount:
    os_printf("----- test_mount %d over  -----\r\n\r\n", number);
}

void lv_fatfs_init(void)
{
	char vol[3];
	lv_fatfs_mount(LV_FS_FATFS_DISK_NUM);
	sprintf(vol, "%d:", LV_FS_FATFS_DISK_NUM);
	f_chdrive(vol);
}

void lv_fatfs_deinit(int number)
{
	FRESULT fr;
	char cFileName[FF_MAX_LFN];
	sprintf(cFileName, "%d:", number);
	fr = f_unmount(LV_FS_FATFS_DISK_NUM, cFileName, 1);
	if (fr != FR_OK)
	{
	 os_printf("f_unmount failed:%d\r\n", fr);
	}
	else
	{
		os_free(fs);
		os_printf("f_unmount OK!\r\n");
	}

	os_printf("----- test_unmount %d over	-----\r\n\r\n", number);
}

#endif