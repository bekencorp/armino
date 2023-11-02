#include "cli.h"
#include "bk_cli.h"
#include <common/sys_config.h>
#include "cli_config.h"
#include <components/system.h>

#if (CONFIG_AT_CMD)
#if CONFIG_BLUETOOTH//(CONFIG_BLE_5_X)
#include "ble_api_5_x.h"
#include <modules/ble.h>
#endif
#if CONFIG_BT
#include <modules/bt.h>
#include "bt_include.h"
#endif
#include "at_common.h"
#include "bk_private/bk_wifi_wpa_cmd.h"
#include <components/system.h>
#include <modules/wifi.h>
#include "bk_wifi_wrapper.h"
#if CONFIG_LWIP
#include "lwip/ping.h"
#endif


static void at_base_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;

    if(argc != 1){
        msg = AT_CMD_RSP_ERROR;
    }
    else
    {
        msg = AT_CMD_RSP_SUCCEED;
    }

    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

static void at_reset_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;

    if(argc != 1){
        msg = AT_CMD_RSP_ERROR;
    }
    else
    {
        bk_reboot();
        msg = AT_CMD_RSP_SUCCEED;
    }

    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

extern volatile const uint8_t build_version[];
static void at_version_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;

    if(argc != 1){
        msg = AT_CMD_RSP_ERROR;
    }
    else
    {
        os_printf("get_version\r\n");
        os_printf("firmware version : %s\n", build_version);
        msg = AT_CMD_RSP_SUCCEED;
    }

    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

}

//#if (CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
#if CONFIG_BLUETOOTH
static void bleat_command_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    const at_command_t *command = NULL;
    uint8_t type = bk_ble_get_controller_stack_type();

    if(type != BK_BLE_CONTROLLER_STACK_TYPE_BLE_5_X &&
        type != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        os_printf("%s stack type %d not support\n", __func__, type);
        return;
    }

    command = lookup_ble_at_command(argv[1]);
    if (command == NULL) {
        bk_printf("cannot find this cmd, please check again!!!\n");
        msg = AT_CMD_RSP_ERROR;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return;
    }

    command->function(pcWriteBuffer, xWriteBufferLen, argc - 2, argv + 2);
}
#endif /// CONFIG_BLE_5_X || CONFIG_BTDM_5_2

#if CONFIG_BT
static void bt_at_command_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    const at_command_t *command = NULL;
    uint8_t type = bk_bt_get_controller_stack_type();
    if(type != BK_BT_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        os_printf("%s stack type %d not support\n", __func__, type);
        return;
    }

    command = lookup_bt_at_command(argv[1]);
    if (command == NULL) {
        bk_printf("cannot find this cmd, please check again!!!\n");
        msg = AT_CMD_RSP_ERROR;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return;
    }

    command->function(pcWriteBuffer, xWriteBufferLen, argc - 2, argv + 2);
}
#endif

#if CONFIG_LWIP
static void wifi_Command_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	const at_command_t *command = NULL;

	command = lookup_wifi_at_command(argv[1]);
    if (command == NULL) {
        bk_printf("cannot find this cmd, please check again!!!\n");
        msg = AT_CMD_RSP_ERROR;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return;
    }

    command->function(pcWriteBuffer, xWriteBufferLen, argc - 2, argv + 2);
}
#endif

void videoat_command_handler(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    const at_command_t *command = lookup_video_at_command(argv[1]);
    if (command == NULL) {
        bk_printf("cannot find this cmd, please check again!!!\n");
        msg = AT_CMD_RSP_ERROR;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return;
    }

    command->function(pcWriteBuffer, xWriteBufferLen, argc - 2, argv + 2);
}

#if (CONFIG_WIFI_ENABLE)
void at_mac_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int ret1,ret2,ret3,ret4;
	uint8_t base_mac[BK_MAC_ADDR_LEN] = {0};
	uint8_t sta_mac[BK_MAC_ADDR_LEN] = {0};
	uint8_t ap_mac[BK_MAC_ADDR_LEN] = {0};

	if (argc == 1) {
		ret1 = bk_get_mac(base_mac, MAC_TYPE_BASE);
		ret2 = bk_wifi_sta_get_mac(sta_mac);
		ret3 = bk_wifi_ap_get_mac(ap_mac);
		if ((ret1 != BK_OK) && (ret2 != BK_OK) && (ret3 != BK_OK))
			msg = AT_CMD_RSP_ERROR;
		else
			msg = AT_CMD_RSP_SUCCEED;
		os_printf("base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
		os_printf("sta mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(sta_mac));
		os_printf("ap mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(ap_mac));

	} else if (argc == 2) {
		hexstr2bin_cli(argv[1], base_mac, BK_MAC_ADDR_LEN);
		ret4 = bk_set_base_mac(base_mac);
		os_printf("set base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
		if (ret4 != BK_OK)
			msg = AT_CMD_RSP_ERROR;
		else
			msg = AT_CMD_RSP_SUCCEED;

	} else {
		os_printf("invalid cmd\r\n");
		msg = AT_CMD_RSP_ERROR;
	}
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}
#endif /// CONFIG_WIFI_ENABLE
#endif /// CONFIG_AT_CMD

#define AT_CMD_CNT (sizeof(s_at_commands) / sizeof(struct cli_command))

static const struct cli_command s_at_commands[] = {
#if (CONFIG_AT_CMD)
    {"AT", "AT", at_base_command},
    {"AT+RST", "AT+RST", at_reset_command},
    {"AT+VERSION", "AT+VERSION", at_version_command},
#if CONFIG_BLE//(CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
    {"AT+BLE", "AT+TYPE_CMD=CMD_name,param1,...,paramn", bleat_command_handler},
#endif
#if CONFIG_BT
    {"AT+BT", "AT+TYPE_CMD=CMD_name,param1,...,paramn", bt_at_command_handler},
#endif
#if CONFIG_LWIP
    {"AT+WIFI", "AT+TYPE_CMD=CMD_name,param1,...,paramn, refer to the at guide.", wifi_Command_handler},
#endif
    {"AT+VIDEO", "video cmd(open/read/close)", videoat_command_handler},
#if (CONFIG_WIFI_ENABLE)
    {"AT+MAC", "AT+MAC=<param>, e.g. AT+MAC=c8478c000001", at_mac_command},
#endif
    /*
    {"AT+HELP","AT+HELP", at_help_command},
    {"AT+VERSION", "AT+VERSION", at_version_command},
    {"AT+ECHO","AT+ECHO=0\\1\\2", at_echo_command},

    {"AT+WLMAC","AT+WLMAC", at_get_station_mac_command},
    {"AT+STADHCP", "AT+STADHCP", at_set_station_dhcp_command},
    {"AT+STAAPINFO","AT+STAAPINFO", at_set_station_ssid_pwd_command},
    {"AT+STASTOP","AT+STASTOP",at_station_stop},
    {"AT+STASTATIC","AT+STASTATIC=ip,mask,gate[,dns]",at_set_station_static_ip_command},
    {"AT+STAST","AT+STAST",at_get_station_status_command},
    {"AT+WSCAN","AT+WSCAN",at_wlan_scan_command},

    {"AT+SOFTAP","AT+SOFTAP=SSID,PWD",at_softap_start_command},
    {"AT+SOFTAPSTOP","AT+STASTOP",at_softap_stop_command},
    {"AT+APSTATIC","AT+APSTATIC=ip,mask,gate[,dns]",at_softap_static_ip_command},
    {"AT+APSTATE","AT+APSTATE",at_get_softap_status_command},*/
#endif
};

int cli_at_init(void)
{
    return cli_register_commands(s_at_commands, AT_CMD_CNT);
}
