#include "at_common.h"
#include <modules/wifi.h>
#include <components/netif.h>
#include "bk_wifi_wrapper.h"
#if CONFIG_LWIP
#include "net.h"
#include "lwip/ping.h"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x "
int at_wifi_scan_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int at_wifi_staconn_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int at_wifi_stadisconn_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int at_wifi_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int at_wifi_stop_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int at_wifi_state_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
int at_wifi_ping_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

const at_command_t wifi_at_cmd_table[] = {
    {0, "SCAN", 1, "scan ssid...", at_wifi_scan_cmd},
    {1, "STACONNECT", 1, "sta connect ap", at_wifi_staconn_cmd},
 //   {2, "STADISCONNECT", 0, "sta disconnect ap", at_wifi_stadisconn_cmd},
    {3, "APSTART", 0, "start ap", at_wifi_ap_cmd},
    {4, "STOP", 0, "disable sta/ap", at_wifi_stop_cmd},
    {5, "STATE", 0, "get wifi state", at_wifi_state_cmd},
    {6, "PING", 0, "wifi ping", at_wifi_ping_cmd}
};

int wifi_at_cmd_cnt(void)
{
    return sizeof(wifi_at_cmd_table) / sizeof(wifi_at_cmd_table[0]);
}

int at_wifi_scan_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	if (argc == 0) {
		demo_scan_app_init();
	} else if (argc == 1){
		uint8_t *ap_ssid;

		ap_ssid = (uint8_t *)argv[0];
		demo_scan_adv_app_init(ap_ssid);
	}
	else {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}

	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

#include "conv_utf8_pub.h"
int at_wifi_staconn_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	char *ssid = NULL;
	char *password = "";
	
	if ((argc < 1) || (argc > 4)) {
		os_printf("invalid argc number\n");
		err = kParamErr;
		goto error;
	}
	
	if (argc >= 1)
		ssid = argv[0];
	
	if (argc >= 2)
		password = argv[1];
	
#if 0 //TODO support BSSID/Channel configuration
	if (argc >= 3)
		bssid = argv[2];
	
	if (argc >= 4)
		channel = argv[3];
#endif
	
	char *oob_ssid_tp = ssid;
#if CONFIG_USE_CONV_UTF8
	oob_ssid_tp = (char *)conv_utf8((uint8_t *)ssid);
#endif
	
	if (oob_ssid_tp) {
		demo_sta_app_init((char *)oob_ssid_tp, password);
#if CONFIG_USE_CONV_UTF8
		os_free(oob_ssid_tp);
#endif
	} else {
		os_printf("not buf for utf8\r\n");
	}

	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
/*
int at_wifi_stadisconn_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	if (argc != 0)
	{
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}
	err = bk_wifi_sta_disconnect();
	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
*/
int at_wifi_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	char *ap_ssid = NULL;
	char *ap_key = "";
	char *ap_channel = NULL;

	if (argc == 1)
		ap_ssid = argv[0];
	else if (argc == 2) {
		ap_ssid = argv[0];
		ap_key = argv[1];
	}
	else if (argc == 3) {
		ap_ssid = argv[0];
		ap_key = argv[1];
		ap_channel = argv[2];
	}
	else {
		os_printf("input param error\n");
		err = kParamErr;
		goto error;
	}
	if (ap_ssid)
		demo_softap_app_init(ap_ssid, ap_key, ap_channel);

	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int at_wifi_stop_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	if (argc == 1) {
		if (os_strcmp(argv[0], "STA") == 0)
			err = bk_wifi_sta_stop();
		else if (os_strcmp(argv[0], "AP") == 0)
			err = bk_wifi_ap_stop();
		else {
			os_printf("unknown WiFi interface\n");
			err = kParamErr;
			goto error;
		}
	} else {
		os_printf("bad parameters\r\n");
		err = kParamErr;
		goto error;
	}

	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}

int at_wifi_state_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;

	if (argc == 0) {
		err = demo_state_app_init();
		if (err == kNoErr) {
			msg = AT_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return err;
		}
		else {
			os_printf("get link status fail!\n");
			err = kGeneralErr;
			goto error;
		}
	}
	else if (argc == 1) {
		wifi_link_status_t link_status = {0};
		//wifi_ap_config_t ap_info = {0};
		char ssid[33] = {0};
		if (sta_ip_is_start()) {
			os_memset(&link_status, 0x0, sizeof(link_status));
			err = bk_wifi_sta_get_link_status(&link_status);
			if(err != kNoErr) {
				os_printf("get link status fail!\n");
				err = kGeneralErr;
				goto error;
			}
			os_memcpy(ssid, link_status.ssid, 32);
			if (os_strcmp(argv[0], "RSSI") == 0) {
				os_printf("sta:rssi=%d\n", link_status.rssi);
			}
			else if (os_strcmp(argv[0], "CHANNEL") == 0) {
				os_printf("sta:channel=%d\n", link_status.channel);
			}
			else if (os_strcmp(argv[0], "SNR") == 0) {
				os_printf("pending\n");
			}
			else if (os_strcmp(argv[0], "BSSID") == 0) {
				os_printf("sta:bssid=" MACSTR "\n", MAC2STR(link_status.bssid));
			}
			else if (os_strcmp(argv[0], "IP") == 0) {
				netif_ip4_config_t ap_ip4_info = {0};
				err = bk_netif_get_ip4_config(NETIF_IF_STA, &ap_ip4_info);
				if(err != kNoErr) {
					os_printf("get ip fail!\n");
					err = kGeneralErr;
					goto error;
				}
				os_printf("ip=%s,gate=%s,mask=%s,dns=%s\r\n",
				   			ap_ip4_info.ip, ap_ip4_info.gateway, ap_ip4_info.mask, ap_ip4_info.dns);
			}
			else if (os_strcmp(argv[0], "SSID") == 0) {
				os_printf("sta:ssid=%d\n", ssid);
			}
			else {
				os_printf("bad parameters\r\n");
				err = kParamErr;
				goto error;
			}
		}
		else {
			os_printf("sta: 0\n");
		}
		if(err == kNoErr) {
			msg = AT_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return err;
		}
	}
error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
int at_wifi_ping_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	int err = kNoErr;
	uint32_t cnt = 4;
	if (argc == 0) {
		os_printf("Please input: ping <host address>\n");
		goto error;
	}
	
	if (argc > 1)
		cnt = os_strtoul(argv[1], NULL, 10);
	
	os_printf("ping IP address:%s\n", argv[1]);
	err = ping(argv[0], cnt, 0);
	if(err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}
	else
		goto error;

error:
	msg = AT_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return err;
}
#endif
