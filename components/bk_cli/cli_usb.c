#include <common/bk_include.h>
#include "stdarg.h"
#include <os/mem.h>
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "bk_sys_ctrl.h"
#include "bk_cli.h"
#include <os/str.h>
#include <components/log.h>
#include "bk_usb.h"
#include "drv_model_pub.h"
#include "cli.h"

#if CONFIG_USB_HOST
#include "ff.h"
#include "diskio.h"

static void cli_usb_help(void)
{
	CLI_LOGI("cli_usb_help!\r\n");
}

#if CONFIG_USB_MSD

static FATFS *pfs = NULL;
uint8_t mount_flag = 0;

void usb_mount_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	char sys_path[10];
	int number = DISK_NUMBER_UDISK;

	if (pfs)
		os_free(pfs);

	pfs = os_malloc(sizeof(FATFS));
	if (NULL == pfs) {
		CLI_LOGI("f_mount malloc failed!\r\n");
		return;
	}

	os_memset(sys_path, 0, sizeof(sys_path));
	sprintf(sys_path, "%d:", number);
	fr = f_mount(pfs, sys_path, 1);
	if (fr != FR_OK) {
		mount_flag = 0;
		CLI_LOGI("usb mount failed:%d\r\n", fr);
	} else {
		mount_flag = 1;
		CLI_LOGI("usb mount OK!\r\n");
	}
}



void usb_unmount_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	char sys_path[10];
	int number = DISK_NUMBER_UDISK;

	if (mount_flag != 1) {
		CLI_LOGI("usb hasn't initialization!\r\n");
		return;
	}

	os_memset(sys_path, 0, sizeof(sys_path));
	sprintf(sys_path, "%d:", number);
	fr = f_mount(NULL, sys_path, 1);
	if (fr != FR_OK) {
		CLI_LOGI("unmount %s fail.\r\n", sys_path);
		return;
	}

	if (pfs) {
		os_free(pfs);
		pfs = NULL;
	}

	mount_flag = 0;
	CLI_LOGI("usb unmount OK!\r\n");
}

static FRESULT usb_scan_files(char *path)
{
	FRESULT fr;
	DIR dir;
	FILINFO fno;
	char path_temp[255];

	fr = f_opendir(&dir, path);
	if (fr == FR_OK) {
		while (1) {
			fr = f_readdir(&dir, &fno);
			if (fr != FR_OK)
				break;
			if (fno.fname[0] == 0)
				break;
			if (fno.fattrib & AM_DIR) {
				/* It is a directory */
				os_memset(path_temp, 0, sizeof(path_temp));
				sprintf(path_temp, "%s/%s", path, fno.fname);
				fr = usb_scan_files(path_temp);
				if (fr != FR_OK) break;
			} else {
				/* It is a file. */
				CLI_LOGI("%s/%s\r\n", path, fno.fname);
			}
		}
		f_closedir(&dir);
	} else
		CLI_LOGI("f_opendir failed\r\n");

	return fr;
}

void usb_ls_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	char sys_path[10];
	int number = DISK_NUMBER_UDISK;

	if (mount_flag != 1) {
		CLI_LOGI("usb hasn't initialization!\r\n");
		return;
	}

	os_memset(sys_path, 0, sizeof(sys_path));
	sprintf(sys_path, "%d:", number);
	fr = usb_scan_files(sys_path);
	if (fr != FR_OK)
		CLI_LOGI("scan_files failed!\r\n");
}
#endif

void uvc_get_param(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_usb_help();
		return;
	}
	uint32_t attribute = 0x00;
	uint32_t handle = ((DD_DEV_TYPE_USB & DD_HANDLE_ID_MASK) + DD_HANDLE_MAGIC_WORD);

	if (os_strcmp(argv[1], "backlight") == 0) {
		attribute = 0x01;
	} else if (os_strcmp(argv[1], "brightness") == 0) {
		attribute = 0x02;
	} else if (os_strcmp(argv[1], "contrast") == 0) {
		attribute = 0x03;
	} else if (os_strcmp(argv[1], "gain") == 0) {
		attribute = 0x04;
	} else if (os_strcmp(argv[1], "powerlinefre") == 0) {
		attribute = 0x05;
	} else if (os_strcmp(argv[1], "hue") == 0) {
		attribute = 0x06;
	} else if (os_strcmp(argv[1], "hueauto") == 0) {
		attribute = 0x10;
	} else if (os_strcmp(argv[1], "saturation") == 0) {
		attribute = 0x07;
	} else if (os_strcmp(argv[1], "sharpness") == 0) {
		attribute = 0x08;
	} else if (os_strcmp(argv[1], "gamma") == 0) {
		attribute = 0x09;
	} else if (os_strcmp(argv[1], "wbt") == 0) {
		attribute = 0x0a;
	} else if (os_strcmp(argv[1], "wbtauto") == 0) {
		attribute = 0x0b;
	} else if (os_strcmp(argv[1], "wbc") == 0) {
		attribute = 0x0c;
	} else if (os_strcmp(argv[1], "wbcauto") == 0) {
		attribute = 0x0d;
	} else if (os_strcmp(argv[1], "digmu") == 0) {
		attribute = 0x0e;
	} else if (os_strcmp(argv[1], "digmulimit") == 0) {
		attribute = 0x0f;
	} else if (os_strcmp(argv[1], "analogvideo") == 0) {
		attribute = 0x10;
	} else if (os_strcmp(argv[1], "analoglock") == 0) {
		attribute = 0x12;
	} else {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[2], "cur") == 0) {
		ddev_control(handle, UCMD_UVC_GET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "min") == 0) {
		ddev_control(handle, UCMD_UVC_GET_MIN, (void *)&attribute);
	} else if (os_strcmp(argv[2], "max") == 0) {
		ddev_control(handle, UCMD_UVC_GET_MAX, (void *)&attribute);
	} else if (os_strcmp(argv[2], "info") == 0) {
		ddev_control(handle, UCMD_UVC_GET_INFO, (void *)&attribute);
	} else if (os_strcmp(argv[2], "len") == 0) {
		ddev_control(handle, UCMD_UVC_GET_LEN, (void *)&attribute);
	} else if (os_strcmp(argv[2], "res") == 0) {
		ddev_control(handle, UCMD_UVC_GET_RES, (void *)&attribute);
	} else if (os_strcmp(argv[2], "def") == 0) {
		ddev_control(handle, UCMD_UVC_GET_DEF, (void *)&attribute);
	} else {
		cli_usb_help();
		return;
	}

}

void uvc_set_param(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_usb_help();
		CLI_LOGI("cli_usb_help!\r\n");
		return;
	}
	uint32_t attribute = 0x00;
	uint32_t handle = ((DD_DEV_TYPE_USB & DD_HANDLE_ID_MASK) + DD_HANDLE_MAGIC_WORD);

	if (os_strcmp(argv[1], "backlight") == 0) {
		attribute = 0x01;
	} else if (os_strcmp(argv[1], "brightness") == 0) {
		attribute = 0x02;
	} else if (os_strcmp(argv[1], "contrast") == 0) {
		attribute = 0x03;
	} else if (os_strcmp(argv[1], "gain") == 0) {
		attribute = 0x04;
	} else if (os_strcmp(argv[1], "powerlinefre") == 0) {
		attribute = 0x05;
	} else if (os_strcmp(argv[1], "hue") == 0) {
		attribute = 0x06;
	} else if (os_strcmp(argv[1], "hueauto") == 0) {
		attribute = 0x10;
	} else if (os_strcmp(argv[1], "saturation") == 0) {
		attribute = 0x07;
	} else if (os_strcmp(argv[1], "sharpness") == 0) {
		attribute = 0x08;
	} else if (os_strcmp(argv[1], "gamma") == 0) {
		attribute = 0x09;
	} else if (os_strcmp(argv[1], "wbt") == 0) {
		attribute = 0x0a;
	} else if (os_strcmp(argv[1], "wbtauto") == 0) {
		attribute = 0x0b;
	} else if (os_strcmp(argv[1], "wbc") == 0) {
		attribute = 0x0c;
	} else if (os_strcmp(argv[1], "wbcauto") == 0) {
		attribute = 0x0d;
	} else if (os_strcmp(argv[1], "digmu") == 0) {
		attribute = 0x0e;
	} else if (os_strcmp(argv[1], "digmulimit") == 0) {
		attribute = 0x0f;
	} else if (os_strcmp(argv[1], "analogvideo") == 0) {
		attribute = 0x10;
	} else if (os_strcmp(argv[1], "analoglock") == 0) {
		attribute = 0x12;
	} else {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[2], "0") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "11") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "22") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "33") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "44") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "55") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else if (os_strcmp(argv[2], "1000") == 0) {
		ddev_control(handle, UCMD_UVC_SET_CUR, (void *)&attribute);
	} else {
		cli_usb_help();
		return;
	}

}

void uvc_start_stream(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}
	uint32_t attribute = 0x00;
	uint32_t handle = ((DD_DEV_TYPE_USB & DD_HANDLE_ID_MASK) + DD_HANDLE_MAGIC_WORD);

	if (os_strcmp(argv[1], "start") == 0) {
		ddev_control(handle, UCMD_UVC_START_STREAM, (void *)&attribute);
	} else if(os_strcmp(argv[1], "stop") == 0) {
		ddev_control(handle, UCMD_UVC_STOP_STREAM, (void *)&attribute);
	}
	else {
		cli_usb_help();
		return;
	}

}


#ifdef CONFIG_USB_UVC
void cli_fuvc_test_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		fuvc_test_init(1);
	} else {
		cli_usb_help();
		return;
	}

}

void cli_fuvc_printf_test_buff(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[1], "print") == 0) {
		printf_test_buff();
	} else {
		cli_usb_help();
		return;
	}

}
#endif

const struct cli_command usb_host_clis[] = {

#if CONFIG_USB_MSD
	{"usb_mount", "usb mount", usb_mount_command},
	{"usb_unmount", "usb unmount", usb_unmount_command},
	{"usb_ls", "usb list system", usb_ls_command},
#endif
#ifdef CONFIG_USB_UVC
	{"fuvc_test", "cli_fuvc_test_init", cli_fuvc_test_init},
	{"fuvc_printf_buff", "fuvc_printf_buff", cli_fuvc_printf_test_buff},
	{"uvc_ctrl_get", "uvc ctrl get attribute information", uvc_get_param},
	{"uvc_ctrl_set", "uvc ctrl set attribute param", uvc_set_param},
	{"uvc_stream", "uvc ctrl set attribute param", uvc_start_stream},
#endif

};

void usb_cli_init(void)
{
	int ret;
	ret = cli_register_commands(usb_host_clis, sizeof(usb_host_clis) / sizeof(struct cli_command));
	if (ret)
		CLI_LOGI("register usb host commands fail.\r\n");
}
#endif
