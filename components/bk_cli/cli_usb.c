#include <common/bk_include.h>
#include "sdkconfig.h"
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


#if CONFIG_USB
#if CONFIG_USB_MSD
#include "ff.h"
#include "diskio.h"
#endif

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
		bk_uvc_get_cur(attribute);
	} else if (os_strcmp(argv[2], "min") == 0) {
		bk_uvc_get_min(attribute);
	} else if (os_strcmp(argv[2], "max") == 0) {
		bk_uvc_get_max(attribute);
	} else if (os_strcmp(argv[2], "info") == 0) {
		bk_uvc_get_info(attribute);
	} else if (os_strcmp(argv[2], "len") == 0) {
		bk_uvc_get_len(attribute);
	} else if (os_strcmp(argv[2], "res") == 0) {
		bk_uvc_get_res(attribute);
	} else if (os_strcmp(argv[2], "def") == 0) {
		bk_uvc_get_def(attribute);
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
	uint32_t param = 0;

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

	param = os_strtoul(argv[2], NULL, 10);
	bk_uvc_set_cur(attribute, param);

}

void uvc_start_stream(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		usb_device_set_using_status(1, USB_UVC_DEVICE);
		bk_uvc_start();
	} else if(os_strcmp(argv[1], "stop") == 0) {
		usb_device_set_using_status(0, USB_UVC_DEVICE);
		bk_uvc_stop();
	} else {
		cli_usb_help();
		return;
	}

}


#if CONFIG_USB_UVC
void cli_fuvc_test_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}
	uint8_t param;
	if (os_strcmp(argv[1], "init") == 0) {
		param = os_strtoul(argv[2], NULL, 10);
		fuvc_test_init(param);
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

#if CONFIG_USB_PLUG_IN_OUT
void cli_usb_plug_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		usb_plug_inout_init();
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		usb_plug_inout_deinit();
	} else {
		cli_usb_help();
		return;
	}

}

void cli_usb_plug_inout(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[1], "open") == 0) {
		usb_plug_inout_open();
		CLI_LOGI("cli_usb_plug_inout inout open!\r\n");
	} else if (os_strcmp(argv[1], "close") == 0) {
		usb_plug_inout_close();
		CLI_LOGI("cli_usb_plug_inout inout close!\r\n");
	} else {
		cli_usb_help();
		return;
	}

}
#endif

void cli_usb_open_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[1], "open_host") == 0) {
		CLI_LOGI("cli_usb_open host! %d\r\n", bk_usb_open(0));
	} else if (os_strcmp(argv[1], "open_dev") == 0) {
		CLI_LOGI("cli_usb_open device! %d\r\n", bk_usb_open(1));
	} else if (os_strcmp(argv[1], "close") == 0) {
		bk_usb_close();
	} else {
		cli_usb_help();
		return;
	}

}

#ifdef CONFIG_USB_UAC
void uac_start_stop(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	s_audio_as_general_descriptor interfacedesc;
	s_audio_format_type_descriptor formatdesc;
	if(os_strcmp(argv[1], "get_mic_desc") == 0){
		CLI_LOGI("uac_start_stop get_mic_desc\r\n");
		bk_usb_uac_get_format_descriptor(USB_UAC_MIC_DEVICE, &interfacedesc, &formatdesc);
		CLI_LOGI(" %s GeneralInerfaceDesc->bLength %x\r\n", __func__, interfacedesc.bLength);
		CLI_LOGI(" %s GeneralInerfaceDesc->bDescriptorType %x\r\n", __func__, interfacedesc.bDescriptorType);
		CLI_LOGI(" %s GeneralInerfaceDesc->bDescriptorSubtype %x\r\n", __func__,interfacedesc.bDescriptorSubtype);
		CLI_LOGI(" %s GeneralInerfaceDesc->bTerminalLink %x\r\n", __func__, interfacedesc.bTerminalLink);
		CLI_LOGI(" %s GeneralInerfaceDesc->bDelay %x\r\n", __func__, interfacedesc.bDelay);
		CLI_LOGI(" %s GeneralInerfaceDesc->wFormatTag %x\r\n", __func__, interfacedesc.wFormatTag);
		CLI_LOGI(" %s FormatTypeDesc->bLength %x\r\n", __func__, formatdesc.bLength);
		CLI_LOGI(" %s FormatTypeDesc->bDescriptorType %x\r\n", __func__, formatdesc.bDescriptorType);
		CLI_LOGI(" %s FormatTypeDesc->bDescriptorSubtype %x\r\n", __func__,formatdesc.bDescriptorSubtype);
		CLI_LOGI(" %s FormatTypeDesc->bFormatType %x\r\n", __func__, formatdesc.bFormatType);
		CLI_LOGI(" %s FormatTypeDesc->bNrChannels %x\r\n", __func__, formatdesc.bNrChannels);
		CLI_LOGI(" %s FormatTypeDesc->bSubframeSize %x\r\n", __func__, formatdesc.bSubframeSize);
		CLI_LOGI(" %s FormatTypeDesc->bBitResolution %x\r\n", __func__, formatdesc.bBitResolution);
		CLI_LOGI(" %s FormatTypeDesc->bSamFreqType %x\r\n", __func__, formatdesc.bSamFreqType);
		CLI_LOGI(" %s FormatTypeDesc->tSamFreq[0] %x\r\n", __func__, formatdesc.tSamFreq[0]);
		CLI_LOGI(" %s FormatTypeDesc->tSamFreq[1] %x\r\n", __func__, formatdesc.tSamFreq[1]);
		CLI_LOGI(" %s FormatTypeDesc->tSamFreq[2] %x\r\n", __func__, formatdesc.tSamFreq[2]);
	} else if(os_strcmp(argv[1], "get_speaker_desc") == 0){
		CLI_LOGI("uac_start_stop get_speaker_desc\r\n");
		bk_usb_uac_get_format_descriptor(USB_UAC_MIC_DEVICE, &interfacedesc, &formatdesc);
		CLI_LOGI(" %s GeneralInerfaceDesc->bLength %x\r\n", __func__, interfacedesc.bLength);
		CLI_LOGI(" %s GeneralInerfaceDesc->bDescriptorType %x\r\n", __func__, interfacedesc.bDescriptorType);
		CLI_LOGI(" %s GeneralInerfaceDesc->bDescriptorSubtype %x\r\n", __func__,interfacedesc.bDescriptorSubtype);
		CLI_LOGI(" %s GeneralInerfaceDesc->bTerminalLink %x\r\n", __func__, interfacedesc.bTerminalLink);
		CLI_LOGI(" %s GeneralInerfaceDesc->bDelay %x\r\n", __func__, interfacedesc.bDelay);
		CLI_LOGI(" %s GeneralInerfaceDesc->wFormatTag %x\r\n", __func__, interfacedesc.wFormatTag);
		CLI_LOGI(" %s FormatTypeDesc->bLength %x\r\n", __func__, formatdesc.bLength);
		CLI_LOGI(" %s FormatTypeDesc->bDescriptorType %x\r\n", __func__, formatdesc.bDescriptorType);
		CLI_LOGI(" %s FormatTypeDesc->bDescriptorSubtype %x\r\n", __func__,formatdesc.bDescriptorSubtype);
		CLI_LOGI(" %s FormatTypeDesc->bFormatType %x\r\n", __func__, formatdesc.bFormatType);
		CLI_LOGI(" %s FormatTypeDesc->bNrChannels %x\r\n", __func__, formatdesc.bNrChannels);
		CLI_LOGI(" %s FormatTypeDesc->bSubframeSize %x\r\n", __func__, formatdesc.bSubframeSize);
		CLI_LOGI(" %s FormatTypeDesc->bBitResolution %x\r\n", __func__, formatdesc.bBitResolution);
		CLI_LOGI(" %s FormatTypeDesc->bSamFreqType %x\r\n", __func__, formatdesc.bSamFreqType);
		CLI_LOGI(" %s FormatTypeDesc->tSamFreq[0] %x\r\n", __func__, formatdesc.tSamFreq[0]);
		CLI_LOGI(" %s FormatTypeDesc->tSamFreq[1] %x\r\n", __func__, formatdesc.tSamFreq[1]);
		CLI_LOGI(" %s FormatTypeDesc->tSamFreq[2] %x\r\n", __func__, formatdesc.tSamFreq[2]);
	}


#if CONFIG_USB_UAC_MIC
	if(os_strcmp(argv[1], "start_mic") == 0){
		CLI_LOGI("uac_start_stop start_mic\r\n");
		usb_device_set_using_status(1, USB_UAC_MIC_DEVICE);
		bk_uac_start_mic();
	} else if(os_strcmp(argv[1], "stop_mic") == 0){
		CLI_LOGI("uac_start_stop stop_mic\r\n");
		usb_device_set_using_status(0, USB_UAC_MIC_DEVICE);
		bk_uac_stop_mic();
	}
#endif
#if CONFIG_USB_UAC_SPEAKER
	if(os_strcmp(argv[1], "start_speaker") == 0){
		CLI_LOGI("uac_start_stop start_mic\r\n");
		usb_device_set_using_status(1, USB_UAC_SPEAKER_DEVICE);
		bk_uac_start_speaker();
	} else if(os_strcmp(argv[1], "stop_speaker") == 0){
		CLI_LOGI("uac_start_stop start_mic\r\n");
		usb_device_set_using_status(0, USB_UAC_SPEAKER_DEVICE);
		bk_uac_stop_speaker();
	}
#endif
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

#if CONFIG_USB_UAC
	{"uac", "uac start_mic|stop_mic|start_speaker|stop_speaker", uac_start_stop},
#endif

#if CONFIG_USB_PLUG_IN_OUT
	{"usb_plug", "usb plug init|out", cli_usb_plug_init},
	{"usb_plug_inout", "usb open|close", cli_usb_plug_inout},
#endif
	{"usb", "usb open_host|open_dev|close", cli_usb_open_close},
};

int cli_usb_init(void)
{
	int ret;
	ret = cli_register_commands(usb_host_clis, sizeof(usb_host_clis) / sizeof(struct cli_command));
	if (ret)
		CLI_LOGI("register usb host commands fail.\r\n");

	return ret;
}
#endif
