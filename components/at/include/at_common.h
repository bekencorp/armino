#ifndef AT_COMMON_H_
#define AT_COMMON_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <components/log.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
//#if (CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
#include "modules/ble.h"
#include "modules/bt.h"
#include "ble_api_5_x.h"

//#endif
#include <common/bk_kernel_err.h>

#define MAX_BT_AT_CMD_LEN               20
#define MAX_BLE_AT_CMD_LEN              64
#define MAX_WIFI_AT_CMD_LEN             30
#define MAX_VIDEO_AT_CMD_LEN            10
#define AT_SYNC_CMD_TIMEOUT_MS          4000


#define AT_CMDRSP_HEAD                   "CMDRSP:"
#define AT_READY_MSG                     "\r\nready\r\n"
#define AT_CMD_RSP_SUCCEED               "CMDRSP:OK\r\n"
#define AT_CMD_RSP_ERROR                 "CMDRSP:ERROR\r\n"
#define AT_CMDMSG_ERROR_RSP              "CMDRSP:ERROR\r\n"
#define AT_RET_CHAR                      '\r'
#define AT_END_CHAR                      '\n'

#define AT_EVENT_HEAD                    "EVT:"
#define AT_EVT_WLAN_DISCONNECTED         "EVT:WLAN DISCONNECTED\r\n"
#define AT_EVT_WLAN_CONNECTED            "EVT:WLAN CONNECTED\r\n"
#define AT_EVT_GOT_IP                    "EVT:GOT-IP\r\n"

typedef struct
{
    /** The command index*/
    const uint8_t idx;
    /** The name of the at command */
    const char *name;
    /** is the unsync command**/
    bool is_sync_cmd;
    /** The help text associated with the command */
    const char *help;
    /** The function that should be invoked for this command. */
    int (*function) (char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
}at_command_t;

//#if (CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
const at_command_t *lookup_bt_at_command(char *str1);
const at_command_t *lookup_ble_at_command(char *str1);
const at_command_t *lookup_bt_at_command(char *str1);
void at_set_data_handle(uint8_t *out, char *buff, uint8_t len);
int get_addr_from_param(bd_addr_t *bdaddr, char *input_param);
int ble_at_cmd_cnt(void);
int bt_at_cmd_cnt(void);

extern const at_command_t ble_at_cmd_table[MAX_BLE_AT_CMD_LEN];
extern const at_command_t bt_at_cmd_table[MAX_BT_AT_CMD_LEN];

//#endif
#if CONFIG_LWIP
const at_command_t *lookup_wifi_at_command(char *str1);
int wifi_at_cmd_cnt(void);
extern const at_command_t wifi_at_cmd_table[MAX_WIFI_AT_CMD_LEN];
#endif
const at_command_t *lookup_video_at_command(char *str1);
int video_at_cmd_cnt(void);
extern const at_command_t video_at_cmd_table[MAX_VIDEO_AT_CMD_LEN];

#endif

