#include <common/bk_include.h>

#include "FreeRTOS.h"
#include "task.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "bk_uart.h"
#include <os/mem.h>
#include <os/str.h>
#include <sys/time.h>
#include "board.h"
#include "app_video_intf.h"

#include "app_demo_config.h"
#include "app_audio_intf.h"

#include "app_jpeg2avi.h"
#include <components/video_transfer.h>
#include "tzh_ppcs_api.h"
#include "app_demo_did.h"
#include <string.h>
#include <rtdevice.h>

#include "dfs_posix.h"
#include "app_sd.h"

#define SD_CARD_DETECT_PIN			   12

#define SD_DEBOUNCE_COUNT 			    100
#define SD_CARD_OFFLINE				    0
#define SD_CARD_ONLINE				    1
#define SD_CARD_ONLINE_ERROR 			2

#define SD_FULL_THRESHOLD 128/*128M,unit is MByte*/
//delete file count one time
#define DELETE_FILE_THRESHOLD 6
//20190909120754.avi
#define FILE_NAME_LEN 18
#define ONE_FILE_ENTRY (FILE_NAME_LEN + 2)

typedef struct _sd_check_st {
	UINT16 detect_cnt;
	UINT8 sd_online;
	SD_ACTION check_msg;
	rt_timer_t sd_check_timer;
} SD_CHECK_ST;

static SD_CHECK_ST sd_check;

extern int get_curtime_str(char *buf, char opera);

static void sd_detect_handle(void *arg)
{
	UINT32 sd_pin_level;
	GLOBAL_INT_DECLARATION();
	sd_pin_level = rt_pin_read(SD_CARD_DETECT_PIN);
	if (sd_pin_level) {
		if ((sd_check.sd_online == SD_CARD_ONLINE) ||
			(sd_check.sd_online == SD_CARD_ONLINE_ERROR)) {
			sd_check.detect_cnt = 0;
			sd_check.sd_online  = SD_CARD_OFFLINE;
			GLOBAL_INT_DISABLE();
			sd_check.check_msg = SD_ACTION_DETACH;
			GLOBAL_INT_RESTORE();
		}
	} else {
		if (sd_check.sd_online == SD_CARD_OFFLINE) {
			if (sd_check.detect_cnt < SD_DEBOUNCE_COUNT)
				sd_check.detect_cnt++;
			else {
				sd_check.sd_online = SD_CARD_ONLINE;
				GLOBAL_INT_DISABLE();
				sd_check.check_msg = SD_ACTION_ATTACH;
				GLOBAL_INT_RESTORE();
			}
		}
	}
}
void set_sd_action(SD_ACTION act)
{
	sd_check.check_msg = act;
}
static SD_ACTION get_sd_action(void)
{
	GLOBAL_INT_DECLARATION();
	SD_ACTION tmp = sd_check.check_msg;

	if (tmp != SD_NO_ACTION) {
		os_printf("===action:%x====\r\n", tmp);
		GLOBAL_INT_DISABLE();
		sd_check.check_msg = SD_NO_ACTION;
		GLOBAL_INT_RESTORE();
	}
	return tmp;
}
static inline void set_online_error(void)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	sd_check.sd_online = SD_CARD_ONLINE_ERROR;
	GLOBAL_INT_RESTORE();
}
static  inline UINT8 sd_is_online(void)
{
	return (SD_CARD_ONLINE == sd_check.sd_online);
}

/*
	binary insertion sort
*/
static void BinSort_filename(char *unsorted[], int len)
{
	char *temp;
	int low, high, mid, i, j;
	for (i = 1; i < len; i++) {
		temp = unsorted[i];
		low = 0;
		high = i - 1;
		while (low <= high) {
			mid = (low + high) / 2;
			if (strcmp(temp, unsorted[mid]) < 0)
				high = mid - 1;
			else
				low = mid + 1;
		}
		for (j = i - 1; j >= low; --j)
			unsorted[j + 1] = unsorted[j];
		unsorted[low] = temp;
	}
}
/*
full disk root scan
*/
static int scan_sd_root(char *tmpbuf)
{
	struct dirent  *ent  = NULL;
	DIR *pDir = NULL;
	short filecnt = 0;
	int i;
	rt_uint8_t tmp;
	char path[] = "/sd";
	char *pbuf = tmpbuf;

	if (NULL == pbuf)
		return filecnt;

	pDir = opendir(path);
	if (NULL != pDir) {
		tmp = strlen(path);
		while (1) {
			ent = readdir(pDir);
			if (NULL == ent)
				break;

			if ((0 == strcmp(ent->d_name, ".")) || (0 == strcmp(ent->d_name, "..")))
				continue;

			if (ent->d_type & DT_DIR)
				continue;
			else {
				if (0 == strncasecmp(strchr(ent->d_name, '.'), ".avi", 4)) {
					if (FILE_NAME_LEN == strlen(ent->d_name)) {
						snprintf(pbuf + filecnt * ONE_FILE_ENTRY, FILE_NAME_LEN + 1, ent->d_name);
						filecnt ++;
					}
				}
			}
		}
		closedir(pDir);
	}
	return filecnt;
}

int sd_check_init(void)
{
	// 	rt_kprintf("\r\n sd check init-----\r\n");
	os_printf("\r\n sd check init-----\r\n");

	rt_pin_mode(SD_CARD_DETECT_PIN, PIN_MODE_INPUT_PULLUP);
	sd_check.sd_check_timer = rt_timer_create("sd-det", sd_detect_handle, NULL, 20,
							  RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC);

	sd_check.sd_online = SD_CARD_OFFLINE;
	sd_check.check_msg = SD_NO_ACTION;
	if (NULL == sd_check.sd_check_timer)
		return -1;
	rt_timer_start(sd_check.sd_check_timer);
	return 0;
}

SD_HANDLE_RESULT sd_action_handle(void)
{
	SD_ACTION action;
	SD_HANDLE_RESULT ret = SD_NO_CHANGE;

	action = get_sd_action();
	if (SD_NO_ACTION != action) {
		os_printf("loop action:%x\r\n", action);
		if (SD_ACTION_ATTACH == action) {
			/* mount sd card fat partition 1 as root directory */
			ret = dfs_mount("sd0", "/sd", "elm", 0, 0);
			if (0 == ret) {
				ret = SD_MOUNT_OK;
				os_printf("SD File System initialized!\n");
			} else {
				os_printf("SD File System initialzation failed!\n");
				if (sd_is_online()) {
					set_online_error();
					ret = SD_MOUNT_ERR;//error
				} else //????
					ret = SD_UNMOUNT;
			}
		} else {
			/*unmount sd card,detatch*/
			ret = SD_UNMOUNT;
		}

		if (SD_UNMOUNT == ret)
			dfs_unmount("/sd");
	}
	return ret;
}
/*
if disk free size is too small,scan root and delete the oldest files
*/
int createfile_pre_handle(char *buf)
{
	struct statfs buffer;
	int filecnt, i, delet_file_cnt;
	int free_cap = 0;
	int ret = 0;
	char **file_name_ptr;
	char *file_name_arry;
	char (*pbuf)[ONE_FILE_ENTRY];
	char full_file_name[32];


	if (!sd_is_online())
		return -1;

	if (NULL == buf)
		return -1;

	//get sd free size
	if (dfs_statfs("/sd", &buffer) == 0) {
		free_cap = buffer.f_bfree >> 11;//sd block size is 512Byte
		///rt_kprintf("disk size:%d MBytes,free size:%d MBytes\r\n",buffer.f_blocks >> 11,free_cap);
	} else {
		os_printf("errrr\r\n");
		return -1;
	}

	if (free_cap < SD_FULL_THRESHOLD) {
		filecnt = scan_sd_root(buf);
		if (filecnt > DELETE_FILE_THRESHOLD) {
			file_name_arry = rt_malloc(sizeof(char *)*filecnt);
			if (NULL == file_name_arry)
				return -1;

			file_name_ptr = (char **)file_name_arry;
			pbuf = (char (*)[ONE_FILE_ENTRY])buf;
			//get all recorded files' name string addr
			for (i = 0; i < filecnt; i++) {
				*(file_name_ptr + i) = (char *)pbuf;
				pbuf++;
			}

			//sort by filename
			BinSort_filename(file_name_ptr, filecnt);
			os_printf("======start delete file======\r\n");
			msh_exec("free", strlen("free"));
			os_printf("\r\n");
			if (filecnt > DELETE_FILE_THRESHOLD)
				delet_file_cnt = DELETE_FILE_THRESHOLD;
			else
				delet_file_cnt = filecnt;

			for (i = 0; i < delet_file_cnt; i++) {
				snprintf(full_file_name, sizeof(full_file_name), "%s%s", "sd/", *(file_name_ptr + i));
				ret = unlink(full_file_name);
				if (0 != ret) {
					os_printf("delete error:%d\r\n", ret);
					ret = -1;
					break;
				}
			}
			rt_free(file_name_arry);
			os_printf("======after delete======\r\n");
			msh_exec("free", strlen("free"));
			os_printf("\r\n");
		} else
			ret = -1;
	}
	return ret;
}

/*
return : 0 -- get file name success (connect ntp server or timeout)
		 1 -- can't connect ntp server
*/
int get_filename_handle(char *file_name, int file_name_len, int ntp_timeout_cnt, int timout_max)
{
	int ret;
	char date[16];
	char opera;

	if (ntp_timeout_cnt < timout_max)
		opera = 1;
	else
		opera = 0;
	ret = get_curtime_str(date, opera);
	snprintf(file_name, file_name_len, "%s%s%s", "sd/", date, ".avi");
	///rt_kprintf("filename:%s,%d,%d\r\n",file_name,ret,opera);

	if (((0 == ret) && (1 == opera)) || (0 == opera)) {
		ret = 0;//app_video_intf_open_record(file_name);
	} else
		ret = 1;

	return ret;
}

