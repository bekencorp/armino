#ifndef _APP_SD_H
#define _APP_SD_H
typedef enum {
	SD_NO_CHANGE = 0,
	SD_MOUNT_OK,
	SD_UNMOUNT,
	SD_MOUNT_ERR,
} SD_HANDLE_RESULT;

typedef enum {
	SD_ACTION_DETACH = 0,
	SD_ACTION_ATTACH,
	SD_NO_ACTION,
} SD_ACTION;

int sd_check_init(void);
SD_HANDLE_RESULT sd_action_handle(void);
int createfile_pre_handle(char *buf);
int get_filename_handle(char *file_name, int file_name_len, int ntp_timeout_cnt, int timout_max);
#endif
