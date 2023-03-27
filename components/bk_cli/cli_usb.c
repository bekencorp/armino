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

#if CONFIG_USB_UVC_DEBUG
#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif

void uvc_view_support_attribute(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	E_UVC_ATTRIBUTE_T attribute;
	for(attribute = UVC_ATTRIBUTE_BACKLIGHT_COMPENSATION; attribute < UVC_ATTRIBUTE_NUM; attribute++)
	{
        switch(attribute)
        {
            case UVC_ATTRIBUTE_BACKLIGHT_COMPENSATION:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT BACKLIGHT_COMPENSATION: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT BACKLIGHT_COMPENSATION: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_BRIGHTNESS:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT BRIGHTNESS: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT BRIGHTNESS: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_CONTRAST:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT CONTRAST: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT CONTRAST: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_GAIN:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT GAIN: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT GAIN: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_POWER_LINE_FREQUENCY:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT POWER_LINE_FREQUENCY: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT POWER_LINE_FREQUENCY: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_HUE:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT HUE: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT HUE: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_SATURATION:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT SATURATION: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT SATURATION: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_SHARPNESS:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT SHARPNESS: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT SHARPNESS: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_GAMMA:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT GAMMA: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT GAMMA: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_TEMPERATURE: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_TEMPERATURE: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE_AUTO:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_TEMPERATURE_AUTO: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_TEMPERATURE_AUTO: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_COMPONENT: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_COMPONENT: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT_AUTO:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_COMPONENT_AUTO: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT WHITE_BALANCE_COMPONENT_AUTO: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_DIGITAL_MULTIPLIER:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT DIGITAL_MULTIPLIER: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT DIGITAL_MULTIPLIER: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_DIGITAL_MULTIPLIER_LIMIT:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT DIGITAL_MULTIPLIER_LIMIT: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT DIGITAL_MULTIPLIER_LIMIT: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_HUE_AUTO:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT HUE_AUTO: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT HUE_AUTO: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_ANALOG_LOCK_STATUS:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT ANALOG_LOCK_STATUS: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT ANALOG_LOCK_STATUS: 1  YES\r\n");
                break;
            case UVC_ATTRIBUTE_ANALOG_VIDEO_STANDARD:
                if(bk_usb_uvc_check_support_attribute(attribute))
                    CLI_LOGI("UVC SUPPORT ANALOG_VIDEO_STANDARD: 0  NO\r\n");
                else
                    CLI_LOGI("UVC SUPPORT ANALOG_VIDEO_STANDARD: 1  YES\r\n");
                break;
            default:
                break;
        }
	}
}

void uvc_get_param(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_usb_help();
		return;
	}
	uint32_t attribute = 0x00;
	uint32_t param = 0;

	if (os_strcmp(argv[1], "backlight") == 0) {
		attribute = UVC_ATTRIBUTE_BACKLIGHT_COMPENSATION;
	} else if (os_strcmp(argv[1], "brightness") == 0) {
		attribute = UVC_ATTRIBUTE_BRIGHTNESS;
	} else if (os_strcmp(argv[1], "contrast") == 0) {
		attribute = UVC_ATTRIBUTE_CONTRAST;
	} else if (os_strcmp(argv[1], "gain") == 0) {
		attribute = UVC_ATTRIBUTE_GAIN;
	} else if (os_strcmp(argv[1], "powerlinefre") == 0) {
		attribute = UVC_ATTRIBUTE_POWER_LINE_FREQUENCY;
	} else if (os_strcmp(argv[1], "hue") == 0) {
		attribute = UVC_ATTRIBUTE_HUE;
	} else if (os_strcmp(argv[1], "hueauto") == 0) {
		attribute = UVC_ATTRIBUTE_HUE_AUTO;
	} else if (os_strcmp(argv[1], "saturation") == 0) {
		attribute = UVC_ATTRIBUTE_SATURATION;
	} else if (os_strcmp(argv[1], "sharpness") == 0) {
		attribute = UVC_ATTRIBUTE_SHARPNESS;
	} else if (os_strcmp(argv[1], "gamma") == 0) {
		attribute = UVC_ATTRIBUTE_GAMMA;
	} else if (os_strcmp(argv[1], "wbt") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE;
	} else if (os_strcmp(argv[1], "wbtauto") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE_AUTO;
	} else if (os_strcmp(argv[1], "wbc") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT;
	} else if (os_strcmp(argv[1], "wbcauto") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT_AUTO;
	} else if (os_strcmp(argv[1], "digmu") == 0) {
		attribute = UVC_ATTRIBUTE_DIGITAL_MULTIPLIER;
	} else if (os_strcmp(argv[1], "digmulimit") == 0) {
		attribute = UVC_ATTRIBUTE_DIGITAL_MULTIPLIER_LIMIT;
	} else if (os_strcmp(argv[1], "analogvideo") == 0) {
		attribute = UVC_ATTRIBUTE_ANALOG_VIDEO_STANDARD;
	} else if (os_strcmp(argv[1], "analoglock") == 0) {
		attribute = UVC_ATTRIBUTE_ANALOG_LOCK_STATUS;
	} else {
		cli_usb_help();
		return;
	}

	if (os_strcmp(argv[2], "cur") == 0) {
		CLI_LOGI("uvc cur 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_CUR, attribute, &param));
	} else if (os_strcmp(argv[2], "min") == 0) {
		CLI_LOGI("uvc min 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_MIN, attribute, &param));
	} else if (os_strcmp(argv[2], "max") == 0) {
		CLI_LOGI("uvc max 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_MAX, attribute, &param));
	} else if (os_strcmp(argv[2], "info") == 0) {
		CLI_LOGI("uvc info 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_INFO, attribute, &param));
	} else if (os_strcmp(argv[2], "len") == 0) {
		CLI_LOGI("uvc len 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_LEN, attribute, &param));
	} else if (os_strcmp(argv[2], "res") == 0) {
		CLI_LOGI("uvc res 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_RES, attribute, &param));
	} else if (os_strcmp(argv[2], "def") == 0) {
		CLI_LOGI("uvc def 0x%x!\r\n", bk_usb_uvc_attribute_op(USB_ATTRIBUTE_GET_DEF, attribute, &param));
	} else {
		cli_usb_help();
		return;
	}
	CLI_LOGI("uvc param: 0x%x!\r\n", param);

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
		attribute = UVC_ATTRIBUTE_BACKLIGHT_COMPENSATION;
	} else if (os_strcmp(argv[1], "brightness") == 0) {
		attribute = UVC_ATTRIBUTE_BRIGHTNESS;
	} else if (os_strcmp(argv[1], "contrast") == 0) {
		attribute = UVC_ATTRIBUTE_CONTRAST;
	} else if (os_strcmp(argv[1], "gain") == 0) {
		attribute = UVC_ATTRIBUTE_GAIN;
	} else if (os_strcmp(argv[1], "powerlinefre") == 0) {
		attribute = UVC_ATTRIBUTE_POWER_LINE_FREQUENCY;
	} else if (os_strcmp(argv[1], "hue") == 0) {
		attribute = UVC_ATTRIBUTE_HUE;
	} else if (os_strcmp(argv[1], "hueauto") == 0) {
		attribute = UVC_ATTRIBUTE_HUE_AUTO;
	} else if (os_strcmp(argv[1], "saturation") == 0) {
		attribute = UVC_ATTRIBUTE_SATURATION;
	} else if (os_strcmp(argv[1], "sharpness") == 0) {
		attribute = UVC_ATTRIBUTE_SHARPNESS;
	} else if (os_strcmp(argv[1], "gamma") == 0) {
		attribute = UVC_ATTRIBUTE_GAMMA;
	} else if (os_strcmp(argv[1], "wbt") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE;
	} else if (os_strcmp(argv[1], "wbtauto") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_TEMPERATURE_AUTO;
	} else if (os_strcmp(argv[1], "wbc") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT;
	} else if (os_strcmp(argv[1], "wbcauto") == 0) {
		attribute = UVC_ATTRIBUTE_WHITE_BALANCE_COMPONENT_AUTO;
	} else if (os_strcmp(argv[1], "digmu") == 0) {
		attribute = UVC_ATTRIBUTE_DIGITAL_MULTIPLIER;
	} else if (os_strcmp(argv[1], "digmulimit") == 0) {
		attribute = UVC_ATTRIBUTE_DIGITAL_MULTIPLIER_LIMIT;
	} else if (os_strcmp(argv[1], "analogvideo") == 0) {
		attribute = UVC_ATTRIBUTE_ANALOG_VIDEO_STANDARD;
	} else if (os_strcmp(argv[1], "analoglock") == 0) {
		attribute = UVC_ATTRIBUTE_ANALOG_LOCK_STATUS;
	} else {
		cli_usb_help();
		return;
	}

	param = os_strtoul(argv[2], NULL, 10);
	bk_usb_uvc_attribute_op(USB_ATTRIBUTE_SET_CUR, attribute, &param);

}

static void uvc_disconnect_uvc_configed(void)
{
	CLI_LOGI("fuvc_notify_uvc_disconnect\r\n");
	return;
}

static void uvc_notify_uvc_configed(void)
{
	CLI_LOGD("uvc_notify_uvc_configed\r\n");
	return;
}

static void uvc_fiddle_rx_vs(void)
{
	CLI_LOGD("uvc_fiddle_rx_vs\r\n");
	bk_uvc_receive_video_stream();
	return;
}

#if (CONFIG_FATFS)
static FIL fp1;
#endif
static beken_semaphore_t s_uvc_test_fifo_data_to_sdcard_sema = NULL;
static beken_thread_t s_uvc_test_fifo_data_to_sdcard_handle = NULL;
static char s_uvc_save_to_sdcard_testname[] = "tduvc.jpg";
static uint32_t s_save_sdcard_count = 0;
static uint32_t s_get_packets_count = 0;
static uint32_t s_get_packet_buff_size_count = 0;
static uint32_t s_packet_max_count_size = 1024;
static uint8_t *s_uvc_stream_data_to_sdcard_1;
static uint8_t *s_uvc_stream_data_to_sdcard_2;
static bool s_uvc_stream_data_to_sdcard_pp_flag = false;
static uint8_t *s_uvc_stream_test_buff;
static bool s_uvc_test_inited_flag = false;

static void uvc_get_packet_rx_vs(uint8_t *arg, uint32_t count)
{
	s_get_packets_count++;
	if((s_get_packets_count%1000) == 0)
		CLI_LOGI("get_packets_rx_vs: arg: %x count %d s_get_packets_count: %d\r\n", arg, count, s_get_packets_count);

	uint8_t *get_data_to_sram_buff;
	int dwIndex = 0;

	if(((s_get_packet_buff_size_count + count) > s_packet_max_count_size) && !s_uvc_stream_data_to_sdcard_pp_flag) {
		s_uvc_stream_data_to_sdcard_pp_flag = true;
		rtos_set_semaphore(&s_uvc_test_fifo_data_to_sdcard_sema);
		s_get_packet_buff_size_count = 0;
	} else if(((s_get_packet_buff_size_count + count) > s_packet_max_count_size) && s_uvc_stream_data_to_sdcard_pp_flag) {
		s_uvc_stream_data_to_sdcard_pp_flag = false;
		rtos_set_semaphore(&s_uvc_test_fifo_data_to_sdcard_sema);
		s_get_packet_buff_size_count = 0;
	}

	if(s_uvc_stream_data_to_sdcard_pp_flag) {
		get_data_to_sram_buff = (s_uvc_stream_data_to_sdcard_2 + s_get_packet_buff_size_count);
		s_get_packet_buff_size_count += count;
		CLI_LOGD("To SDCARD2 count:%d s_save_sdcard_count %d s_get_packet_buff_size_count: %d\r\n",count, s_save_sdcard_count, s_get_packet_buff_size_count);
	} else {
		get_data_to_sram_buff = (s_uvc_stream_data_to_sdcard_1 + s_get_packet_buff_size_count);
		s_get_packet_buff_size_count += count;
		CLI_LOGD("To SDCARD1 count:%d s_save_sdcard_count %d s_get_packet_buff_size_count: %d\r\n",count, s_save_sdcard_count, s_get_packet_buff_size_count);
	}

    for(dwIndex = 0; dwIndex < count; dwIndex++)
    {
        get_data_to_sram_buff[dwIndex] = *((volatile uint8_t *)(arg));
    }
	return;
}

static void uvc_sram_data_write_to_sdcard(void)
{
#if (CONFIG_FATFS)
	int ret = kNoErr;
	unsigned int uiTemp = 0;
	char file_name[50] = {0};
	int num = 1;
	FRESULT fr;

	CLI_LOGI("uvc_sram_data_write_to_sdcard\r\n");

	sprintf(file_name, "%d:/%s", num, s_uvc_save_to_sdcard_testname);

	fr = f_open(&fp1, file_name, FA_OPEN_APPEND | FA_WRITE);
	if (fr != 0)
	{
		CLI_LOGI("f_open fail\r\n");
		return;
	}

	while(1)
	{
		ret = rtos_get_semaphore(&s_uvc_test_fifo_data_to_sdcard_sema, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			s_save_sdcard_count++;
			if(s_uvc_stream_data_to_sdcard_pp_flag)
				fr = f_write(&fp1, (char *)(s_uvc_stream_data_to_sdcard_2), s_get_packet_buff_size_count, &uiTemp);
			else
				fr = f_write(&fp1, (char *)(s_uvc_stream_data_to_sdcard_1), s_get_packet_buff_size_count, &uiTemp);
			if (fr != 0)
			{
				CLI_LOGI("f_write failed\n");
			}
		}
	}
#endif
}

static void uvc_register_rx_vstream_buffptr_len(void *param, uint32_t len)
{
	bk_uvc_register_rx_vstream_buffptr(param);
	bk_uvc_register_rx_vstream_bufflen(len);
}

static void uvc_test_init(uint16_t width, uint16_t height, uint16_t fps, uint16_t len)
{
	if(!s_uvc_test_inited_flag)
		s_uvc_test_inited_flag = true;
	else {
		CLI_LOGI("uvc_test_init Please Deinit first\r\n");
		return;
	}

	void *parameter;
	s_uvc_stream_data_to_sdcard_1 = os_malloc(sizeof(uint8_t) * s_packet_max_count_size);
	if(!s_uvc_stream_data_to_sdcard_1) {
		CLI_LOGI("uvc_test_init s_uvc_stream_data_to_sdcard_1 is null\r\n");
		return;
	}

	s_uvc_stream_data_to_sdcard_2 = os_malloc(sizeof(uint8_t) * s_packet_max_count_size);
	if(!s_uvc_stream_data_to_sdcard_2) {
		CLI_LOGI("uvc_test_init s_uvc_stream_data_to_sdcard_2 is null\r\n");
		return;
	}
	s_uvc_stream_test_buff = os_malloc(sizeof(uint8_t) * len);
	if(!s_uvc_stream_test_buff) {
		CLI_LOGI("uvc_test_init s_uvc_stream_test_buff is null\r\n");
		return;
	} else
		os_memset((void *)s_uvc_stream_test_buff, 0x77, sizeof(uint8_t) * len);//for debug

	parameter = (void *)uvc_disconnect_uvc_configed;
	bk_uvc_register_disconnect_callback(parameter);
	parameter = (void *)uvc_notify_uvc_configed;
	bk_uvc_register_config_callback(parameter);

	parameter = (void *)uvc_fiddle_rx_vs;
	bk_uvc_register_VSrxed_callback(parameter);

	parameter = (void *)uvc_get_packet_rx_vs;
	bk_uvc_register_VSrxed_packet_callback(parameter);

	parameter = (void *)s_uvc_stream_test_buff;
	uvc_register_rx_vstream_buffptr_len(parameter, len);

	bk_uvc_register_link(1);
	UVC_ResolutionFramerate uvc_param;
	uvc_param.fps = fps;
	uvc_param.width = width;
	uvc_param.height = height;
	bk_uvc_set_resolution_framerate(&uvc_param);

#if (CONFIG_FATFS)
	int ret = kNoErr;

	if ((!s_uvc_test_fifo_data_to_sdcard_sema) && (!s_uvc_test_fifo_data_to_sdcard_handle))
	{
		ret = rtos_init_semaphore(&s_uvc_test_fifo_data_to_sdcard_sema, 1);
		if (ret != kNoErr)
			return;


		ret = rtos_create_thread(&s_uvc_test_fifo_data_to_sdcard_handle,
								 5,
								 "uvc_fifo_to_SD",
								 (beken_thread_function_t)uvc_sram_data_write_to_sdcard,
								 4 * 1024,
								 (beken_thread_arg_t)0);

	}
#endif

}

static void uvc_test_printf_buff_data(uint16_t len)
{

	for(int i = 0;i < len; i++)
	{
		os_printf("%x ", s_uvc_stream_test_buff[i]);
		if((i%16) == 0)
			os_printf("\r\n");
	}

}

static void uvc_test_save_buff_data_to_sdcard()
{
#if (CONFIG_FATFS)
	CLI_LOGI("uvc_test delete write to sdcard thread\r\n");
	if(s_uvc_test_fifo_data_to_sdcard_handle)
		rtos_delete_thread(&s_uvc_test_fifo_data_to_sdcard_handle);
	s_uvc_test_fifo_data_to_sdcard_handle = NULL;

	f_close(&fp1);

	if(s_uvc_test_fifo_data_to_sdcard_sema)
		rtos_deinit_semaphore(&s_uvc_test_fifo_data_to_sdcard_sema);
	s_uvc_test_fifo_data_to_sdcard_sema = NULL;
#endif
}

static void uvc_test_deinit()
{
	if(s_uvc_test_inited_flag)
		s_uvc_test_inited_flag = false;
	else {
		CLI_LOGI("uvc_test_deinit please init first\r\n");
		return;
	}

	uint32_t param;
	void *parameter;

	parameter = NULL;
	bk_uvc_register_disconnect_callback(parameter);
	parameter = NULL;
	bk_uvc_register_config_callback(parameter);

	parameter = NULL;
	bk_uvc_register_VSrxed_callback(parameter);

	parameter = NULL;
	bk_uvc_register_VSrxed_packet_callback(parameter);

	parameter = NULL;
	bk_uvc_register_rx_vstream_buffptr(parameter);
	param = 0;
	bk_uvc_register_rx_vstream_bufflen(param);

	if(s_uvc_stream_test_buff)
		os_free(s_uvc_stream_test_buff);
	if(s_uvc_stream_data_to_sdcard_1)
		os_free(s_uvc_stream_data_to_sdcard_1);
	if(s_uvc_stream_data_to_sdcard_2)
		os_free(s_uvc_stream_data_to_sdcard_2);

	s_get_packets_count = 0;
	s_save_sdcard_count = 0;

}

void cli_uvc_test_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	uint16_t width, height, fps, buffer_len;
	bool ret = 0;

	if (os_strcmp(argv[1], "init") == 0) {
		if (argc > 2)
			width = os_strtoul(argv[2], NULL, 10);
		else
			width = 640;

		if (argc > 3)
			height = os_strtoul(argv[3], NULL, 10);
		else
			height = 480;

		if (argc > 4)
			fps = os_strtoul(argv[4], NULL, 10);
		else
			fps = 25;

		if (argc > 5)
			buffer_len = os_strtoul(argv[5], NULL, 10);
		else
			buffer_len = 512;

		if (argc > 6)
			s_packet_max_count_size = os_strtoul(argv[6], NULL, 10);
		else
			s_packet_max_count_size = 1024;

		uvc_test_init(width, height, fps, buffer_len);
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		uvc_test_deinit();
	} else if(os_strcmp(argv[1], "debug_info") == 0) {
		CLI_LOGI("debug info s_uvc_test_inited_flag: %d\r\n", s_uvc_test_inited_flag);
		CLI_LOGI("debug info s_save_sdcard_count: %d\r\n", s_save_sdcard_count);
		CLI_LOGI("debug info s_get_packets_count: %d\r\n", s_get_packets_count);
	} else if(os_strcmp(argv[1], "check_uvc") == 0) {
		ret = bk_usb_check_device_supported (USB_UVC_DEVICE);
		if(ret != BK_OK)
			CLI_LOGI("cli_uac_operation check_uvc UNsupport\r\n");
		else
			CLI_LOGI("cli_uac_operation check_uvc Support\r\n");
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

	if(!s_uvc_test_inited_flag) {
		CLI_LOGI("uvc_start_stream Please Init first\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		usb_device_set_using_status(1, USB_UVC_DEVICE);
		bk_uvc_start();
	} else if(os_strcmp(argv[1], "stop") == 0) {
		usb_device_set_using_status(0, USB_UVC_DEVICE);
		bk_uvc_stop();
	} else if(os_strcmp(argv[1], "enable_h264") == 0) {
		if(!bk_uvc_enable_H264())
			CLI_LOGI("H264 Support!\r\n");
		else
			CLI_LOGI("H264 Unsupport!\r\n");
	} else if(os_strcmp(argv[1], "enable_h265") == 0) {
		if(!bk_uvc_enable_H265())
			CLI_LOGI("H265 Support!\r\n");
		else
			CLI_LOGI("H265 Unsupport!\r\n");
	} else if(os_strcmp(argv[1], "enable_mjpeg") == 0) {
		if(!bk_uvc_enable_mjpeg())
			CLI_LOGI("MJPEG Support!\r\n");
		else
			CLI_LOGI("MJPEG Unsupport!\r\n");
	} else if(os_strcmp(argv[1], "get_resolution_number") == 0) {
		uint16_t number = 0;
		bk_uvc_get_resolution_number(&number);
		CLI_LOGI("get_resolution_number number:%d!\r\n", number);
	} else if(os_strcmp(argv[1], "get_resolution_framerate") == 0) {
		UVC_ResolutionFramerate support_config[20] = {0};
		bk_uvc_get_resolution_framerate((void *)support_config, 20);
	} else if(os_strcmp(argv[1], "rx_vs") == 0){
		uvc_fiddle_rx_vs();
	}else {
		cli_usb_help();
		return;
	}

}

static void cli_uvc_test_buff_ops(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}

	uint16_t buffer_len;
	if (os_strcmp(argv[1], "print") == 0) {
		if (argc > 2)
			buffer_len = os_strtoul(argv[2], NULL, 10);
		else
			buffer_len = 256;
		uvc_test_printf_buff_data(buffer_len);
	} else if(os_strcmp(argv[1], "save") == 0){
		uvc_test_save_buff_data_to_sdcard();
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

#if CONFIG_USB_UAC_DEBUG
static void usb_uac_disconnect_callback(void)
{
	CLI_LOGI("usb_uac_disconnect_callback\r\n");
	return;
}

static void usb_uac_connect_callback(void)
{
	CLI_LOGI("usb_uac_connect_callback\r\n");
	return;
}
static void uac_print_descriptor(	s_audio_as_general_descriptor *interfacedesc, s_audio_format_type_descriptor *formatdesc)
{
	uint32_t tSamFreq_hz = 0x0;

	CLI_LOGI(" %s GeneralInerfaceDesc->bLength %x\r\n", __func__, interfacedesc->bLength);
	CLI_LOGI(" %s GeneralInerfaceDesc->bDescriptorType %x\r\n", __func__, interfacedesc->bDescriptorType);
	CLI_LOGI(" %s GeneralInerfaceDesc->bDescriptorSubtype %x\r\n", __func__,interfacedesc->bDescriptorSubtype);
	CLI_LOGI(" %s GeneralInerfaceDesc->bTerminalLink %x\r\n", __func__, interfacedesc->bTerminalLink);
	CLI_LOGI(" %s GeneralInerfaceDesc->bDelay %x\r\n", __func__, interfacedesc->bDelay);
	CLI_LOGI(" %s GeneralInerfaceDesc->wFormatTag %x\r\n", __func__, interfacedesc->wFormatTag);
	CLI_LOGI(" %s FormatTypeDesc->bLength %x\r\n", __func__, formatdesc->bLength);
	CLI_LOGI(" %s FormatTypeDesc->bDescriptorType %x\r\n", __func__, formatdesc->bDescriptorType);
	CLI_LOGI(" %s FormatTypeDesc->bDescriptorSubtype %x\r\n", __func__,formatdesc->bDescriptorSubtype);
	CLI_LOGI(" %s FormatTypeDesc->bFormatType %x\r\n", __func__, formatdesc->bFormatType);
	CLI_LOGI(" %s FormatTypeDesc->bNrChannels %x\r\n", __func__, formatdesc->bNrChannels);
	CLI_LOGI(" %s FormatTypeDesc->bSubframeSize %x\r\n", __func__, formatdesc->bSubframeSize);
	CLI_LOGI(" %s FormatTypeDesc->bBitResolution %x\r\n", __func__, formatdesc->bBitResolution);
	CLI_LOGI(" %s FormatTypeDesc->bSamFreqType %x\r\n", __func__, formatdesc->bSamFreqType);

	if(formatdesc->bSamFreqType > 0) {
		for(int i = 0, j = 1; i < (formatdesc->bSamFreqType) * 3; i += 3, j++)
		{
			tSamFreq_hz |= (uint32_t)formatdesc->tSamFreq[i];
			tSamFreq_hz |= ((uint32_t)formatdesc->tSamFreq[i+1]) << 8;
			tSamFreq_hz |= ((uint32_t)formatdesc->tSamFreq[i+2]) << 16;
			CLI_LOGI(" %s FormatTypeDesc->SamFreq[%d] = 0x%x, %d Hz\r\n", __func__, j, tSamFreq_hz, tSamFreq_hz);
			tSamFreq_hz = 0x0;
		}
	}
}

#if CONFIG_USB_UAC_MIC
#define UAC_TEST_BUFFER_SIZE        1024 * 1
UINT8 uac_test_buff[UAC_TEST_BUFFER_SIZE] = {0};
static uint16_t count_mic_num = 0;
static void uac_mic_get_packet_rx_vs(uint8_t *arg, uint32_t count)
{
    int dwIndex = 0;

    count_mic_num++;
	if (count_mic_num == 1000){
        CLI_LOGI("uac_get_packet_rx_vs : arg: %x count %d\r\n", arg, count);
        count_mic_num = 0;
    }
    for (dwIndex = 0; dwIndex < count; dwIndex += 4)
    {
        *((uint32_t *) & (uac_test_buff[dwIndex])) = *((volatile uint32_t *)(arg));
    }

    return;
}

void uac_printf_test_buff(void)
{
	for(int i = 0;i < UAC_TEST_BUFFER_SIZE; i++)
	{
		os_printf("%x ", uac_test_buff[i]);
		if((i%16) == 0)
			os_printf("\r\n");
	}
}

#endif

#if CONFIG_USB_UAC_SPEAKER
static const uint32_t uac_speaker_PCM_8000[16] = {
	0x00010000, 0x5A825A81, 0x7FFF7FFF, 0x5A825A83, 0x00000000, 0xA57FA57E, 0x80018002, 0xA57EA57E, 0x00010000, 0x5A825A81, 0x7FFF7FFF, 0x5A825A83, 0x00000000, 0xA57FA57E, 0x80018002, 0xA57EA57E,
};

static uint16_t count_speaker_num = 0;
static void uac_speaker_fiddle_txed(void)
{
	count_speaker_num++;

	if (count_speaker_num == 1000){
		CLI_LOGI("fuac_speaker_fiddle_txed\r\n");
		count_speaker_num = 0;
	}


	return;
}
#endif

void cli_uac_operation(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_usb_help();
		return;
	}
	void *parameter;
	uint32_t data = 0;
	if(os_strcmp(argv[1], "init") == 0) {
		parameter = (void *)usb_uac_disconnect_callback;
		bk_usb_uac_register_disconnect_callback(parameter);
		parameter = (void *)usb_uac_connect_callback;
		bk_usb_uac_register_connect_callback(parameter);
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		parameter = NULL;
		bk_usb_uac_register_disconnect_callback(parameter);
		parameter = NULL;
		bk_usb_uac_register_connect_callback(parameter);
	}

	if(os_strcmp(argv[1], "set_volume") == 0) {
		if (argc > 2)
			data = os_strtoul(argv[2], NULL, 16);
		else
			bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MIN, UAC_ATTRIBUTE_VOLUME, &data);
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_VOLUME, &data);
		CLI_LOGI("cli_uac_operation set_volume = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "get_volume") == 0) {
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_CUR, UAC_ATTRIBUTE_VOLUME, &data);
		CLI_LOGI("cli_uac_operation get_volume = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "get_volume_min") == 0) {
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MIN, UAC_ATTRIBUTE_VOLUME, &data);
		CLI_LOGI("cli_uac_operation get_volume_min = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "get_volume_max") == 0) {
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MAX, UAC_ATTRIBUTE_VOLUME, &data);
		CLI_LOGI("cli_uac_operation get_volume_max = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "check_attribute") == 0) {
		for(E_UAC_ATTRIBUTE_T attribute = UAC_ATTRIBUTE_MUTE; attribute < UAC_ATTRIBUTE_NUM; attribute++)
			CLI_LOGI("cli_uac_operation attribute:%d\r\n", !bk_usb_uac_check_support_attribute(attribute));
	} else if(os_strcmp(argv[1], "set_mute") == 0) {
		data = 1;
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_MUTE, &data);
		CLI_LOGI("cli_uac_operation set_mute = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "set_unmute") == 0) {
		data = 0;
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_MUTE, &data);
		CLI_LOGI("cli_uac_operation set_unmute = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "get_mute") == 0) {
		bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_CUR, UAC_ATTRIBUTE_MUTE, &data);
		CLI_LOGI("cli_uac_operation get_mute = 0x%x\r\n", data);
	} else if(os_strcmp(argv[1], "check_mic") == 0) {
		if(BK_OK == bk_usb_check_device_supported (USB_UAC_MIC_DEVICE))
			data = 1;
		CLI_LOGI("cli_uac_operation check_mic %d\r\n", data);
	} else if(os_strcmp(argv[1], "check_speaker") == 0) {
		if(BK_OK == bk_usb_check_device_supported (USB_UAC_SPEAKER_DEVICE))
			data = 1;
		CLI_LOGI("cli_uac_operation check_speaker %d\r\n", data);
	}

	s_audio_as_general_descriptor interfacedesc;
	s_audio_format_type_descriptor formatdesc;
	if(os_strcmp(argv[1], "get_mic_desc") == 0){
		CLI_LOGI("cli_uac_operation get_mic_desc\r\n");
		bk_usb_uac_get_format_descriptor(USB_UAC_MIC_DEVICE, &interfacedesc, &formatdesc);
		uac_print_descriptor(&interfacedesc, &formatdesc);
	} else if(os_strcmp(argv[1], "get_speaker_desc") == 0){
		CLI_LOGI("cli_uac_operation get_speaker_desc\r\n");
		bk_usb_uac_get_format_descriptor(USB_UAC_SPEAKER_DEVICE, &interfacedesc, &formatdesc);
		uac_print_descriptor(&interfacedesc, &formatdesc);
	}

#if CONFIG_USB_UAC_MIC
	if(os_strcmp(argv[1], "init_mic") == 0) {
		parameter = (void *)uac_mic_get_packet_rx_vs;
		bk_uac_register_micrxed_packet_callback(parameter);
	} else if(os_strcmp(argv[1], "start_mic") == 0){
		CLI_LOGI("cli_uac_operation start_mic\r\n");
		usb_device_set_using_status(1, USB_UAC_MIC_DEVICE);
		bk_uac_start_mic();
	} else if(os_strcmp(argv[1], "stop_mic") == 0){
		CLI_LOGI("cli_uac_operation stop_mic\r\n");
		usb_device_set_using_status(0, USB_UAC_MIC_DEVICE);
		bk_uac_stop_mic();
	} else if(os_strcmp(argv[1], "mic_sethz") == 0){
		if (argc > 2)
			data = os_strtoul(argv[2], NULL, 10);
		else
			data = 16000;
		CLI_LOGI("cli_uac_operation mic_sethz = %d\r\n", data);
		bk_usb_uac_set_hz(USB_UAC_MIC_DEVICE, data);
	}
#endif
#if CONFIG_USB_UAC_SPEAKER
	if(os_strcmp(argv[1], "init_speaker") == 0) {
		parameter = (void *)uac_speaker_fiddle_txed;
		bk_uac_register_speakerstream_txed_callback(parameter);
		parameter = (void *)uac_speaker_PCM_8000;
		bk_uac_register_tx_speakerstream_buffptr(parameter, 64);
	} else if(os_strcmp(argv[1], "start_speaker") == 0){
		CLI_LOGI("cli_uac_operation start_speaker\r\n");
		usb_device_set_using_status(1, USB_UAC_SPEAKER_DEVICE);
		bk_uac_start_speaker();
	} else if(os_strcmp(argv[1], "stop_speaker") == 0){
		CLI_LOGI("cli_uac_operation stop_speaker\r\n");
		usb_device_set_using_status(0, USB_UAC_SPEAKER_DEVICE);
		bk_uac_stop_speaker();
	} else if(os_strcmp(argv[1], "speaker_sethz") == 0){
		if (argc > 2)
			data = os_strtoul(argv[2], NULL, 10);
		else
			data = 16000;
		CLI_LOGI("cli_uac_operation speaker_sethz = %d\r\n", data);
		bk_usb_uac_set_hz(USB_UAC_SPEAKER_DEVICE, data);
	}
#endif
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

const struct cli_command usb_host_clis[] = {

#if CONFIG_USB_MSD
	{"usb_mount", "usb mount", usb_mount_command},
	{"usb_unmount", "usb unmount", usb_unmount_command},
	{"usb_ls", "usb list system", usb_ls_command},
#endif
#ifdef CONFIG_USB_UVC_DEBUG
	{"uvc", "uvc [init|deinit|debug_info]", cli_uvc_test_init},
	{"uvc_data", "uvc_data [print|save]", cli_uvc_test_buff_ops},
	{"uvc_ctrl_support", "uvc ctrl support attribute information", uvc_view_support_attribute},
	{"uvc_ctrl_get", "uvc ctrl get attribute information", uvc_get_param},
	{"uvc_ctrl_set", "uvc ctrl set attribute param", uvc_set_param},
	{"uvc_stream", "uvc ctrl set attribute param", uvc_start_stream},
#endif

#if CONFIG_USB_UAC_DEBUG
	{"uac", "uac init|deinit|init_mic|init_speaker|start_mic|stop_mic|start_speaker|stop_speaker", cli_uac_operation},
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
