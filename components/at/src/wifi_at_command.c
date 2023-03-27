#include "at_common.h"
#include <modules/wifi.h>
#include <components/netif.h>
#include "bk_wifi_wrapper.h"
#if CONFIG_LWIP
#include "net.h"
#include "lwip/ping.h"
#include <components/event.h>
#include <components/netif.h>

beken_semaphore_t wifi_at_cmd_sema = NULL;
int wifi_at_cmd_status = 0;

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
	
const char *wifi_type_string(wifi_security_t security)
{
	switch (security) {
	case WIFI_SECURITY_NONE:
		return "NONE";
	case WIFI_SECURITY_WEP:
		return "WEP";
	case WIFI_SECURITY_WPA_TKIP:
		return "WPA-TKIP";
	case WIFI_SECURITY_WPA_AES:
		return "WPA-AES";
	case WIFI_SECURITY_WPA2_TKIP:
		return "WPA2-TKIP";
	case WIFI_SECURITY_WPA2_AES:
		return "WPA2-AES";
	case WIFI_SECURITY_WPA2_MIXED:
		return "WPA2-MIX";
	case WIFI_SECURITY_WPA3_SAE:
		return "WPA3-SAE";
	case WIFI_SECURITY_WPA3_WPA2_MIXED:
		return "WPA3-WPA2-MIX";
	case WIFI_SECURITY_EAP:
		return "EAP";
	case WIFI_SECURITY_OWE:
		return "OWE";
	case WIFI_SECURITY_AUTO:
		return "AUTO";
	default:
		return "UNKNOWN";
	}
}

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

int at_netif_event_cb(void *arg, event_module_t event_module,
					   int event_id, void *event_data)
{
	netif_event_got_ip4_t *got_ip;

	switch (event_id) {
	case EVENT_NETIF_GOT_IP4:
		if (wifi_at_cmd_sema != NULL) {
			wifi_at_cmd_status = 1;
			rtos_set_semaphore(&wifi_at_cmd_sema);
		}
		got_ip = (netif_event_got_ip4_t *)event_data;
		os_printf("%s: %s got ip\n", __func__, got_ip->netif_if == NETIF_IF_STA ? "BK STA" : "unknown netif");
		break;
	default:
		os_printf("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
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
	if (wifi_at_cmd_table[1].is_sync_cmd)
	{
		err = rtos_init_semaphore(&wifi_at_cmd_sema, 1);
		if(err != kNoErr){
			goto error;
		}
	}
	if (oob_ssid_tp) {
		demo_sta_app_init((char *)oob_ssid_tp, password);
#if CONFIG_USE_CONV_UTF8
		os_free(oob_ssid_tp);
#endif
	} else {
		os_printf("not buf for utf8\r\n");
	}
	bk_event_register_cb(EVENT_MOD_NETIF, EVENT_NETIF_GOT_IP4,
			at_netif_event_cb, NULL);

	if (wifi_at_cmd_sema != NULL)
	{
		err = rtos_get_semaphore(&wifi_at_cmd_sema, 10000);
		if (err != kNoErr)
		{
			goto error;
		}
		else
		{
			if (wifi_at_cmd_status == 1)
			{
				msg = AT_CMD_RSP_SUCCEED;
				os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
				rtos_deinit_semaphore(&wifi_at_cmd_sema);
				wifi_at_cmd_status = 0;
				bk_event_unregister_cb(EVENT_MOD_NETIF, EVENT_NETIF_GOT_IP4,
							at_netif_event_cb);
				return 0;
			}
			else
			{
				err = BK_FAIL;
				goto error;
			}
		}
	}

error:
	msg = AT_CMDMSG_ERROR_RSP;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	if (wifi_at_cmd_sema != NULL) {
		rtos_deinit_semaphore(&wifi_at_cmd_sema);
		bk_event_unregister_cb(EVENT_MOD_NETIF, EVENT_NETIF_GOT_IP4,
					at_netif_event_cb);
	}
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
		err = demo_softap_app_init(ap_ssid, ap_key, ap_channel);

	if (err == kNoErr) {
		msg = AT_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return err;
	}
	else {
		goto error;
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
		if (os_strcmp(argv[0], "STA") == 0) {
			if (sta_ip_is_start()){
				sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, "STA_WIFI_CONNECT", AT_CMD_RSP_SUCCEED);
			}
			else {
				sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, "STA_WIFI_DISCONNECT", AT_CMD_RSP_SUCCEED);
			}
			return err;
		}

		else if (os_strcmp(argv[0], "AP") == 0) {
			if (uap_ip_is_start()){
				sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, "AP_WIFI_START", AT_CMD_RSP_SUCCEED);
			}
			else {
				sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, "AP_WIFI_CLOSE", AT_CMD_RSP_SUCCEED);
			}
			return err;
		}
	}
	else if (argc == 2) {
		if (os_strcmp(argv[0], "STA") == 0) {

			if (sta_ip_is_start()) {
				wifi_link_status_t link_status = {0};
				os_memset(&link_status, 0x0, sizeof(link_status));
				err = bk_wifi_sta_get_link_status(&link_status);
				if(err != kNoErr) {
					os_printf("get sta link status fail!\n");
					err = kGeneralErr;
					goto error;
				}
				if (os_strcmp(argv[1], "RSSI") == 0) {
					sprintf(pcWriteBuffer, "%s:%d\r\n%s", AT_CMDRSP_HEAD, link_status.rssi, AT_CMD_RSP_SUCCEED);
				}

				else if (os_strcmp(argv[1], "CHANNEL") == 0) {
					sprintf(pcWriteBuffer, "%s:%d\r\n%s", AT_CMDRSP_HEAD, link_status.channel, AT_CMD_RSP_SUCCEED);
				}

				else if (os_strcmp(argv[1], "BSSID") == 0) {
					sprintf(pcWriteBuffer, "%s:" MACSTR "\r\n%s", AT_CMDRSP_HEAD, MAC2STR(link_status.bssid), AT_CMD_RSP_SUCCEED);
				}

				else if (os_strcmp(argv[1], "SSID") == 0) {
					sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, link_status.ssid, AT_CMD_RSP_SUCCEED);
				}

				else if (os_strcmp(argv[1], "IP") == 0) {
					netif_ip4_config_t sta_ip4_info = {0};
					err = bk_netif_get_ip4_config(NETIF_IF_STA, &sta_ip4_info);
					if(err != kNoErr) {
						os_printf("get ip fail!\n");
						err = kGeneralErr;
						goto error;
					}
					sprintf(pcWriteBuffer, "%s:IP=%s,GATE=%s,MASK=%s,DNS=%s\r\n%s", AT_CMDRSP_HEAD, sta_ip4_info.ip, sta_ip4_info.gateway, sta_ip4_info.mask, sta_ip4_info.dns, AT_CMD_RSP_SUCCEED);
				}

				else {
					os_printf("bad parameters\r\n");
					err = kParamErr;
					goto error;
				}
			}
			else {
				sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, "STA_WIFI_DISCONNECT", AT_CMD_RSP_SUCCEED);
				err = kGeneralErr;
			}
			return err;
		}
		else if (os_strcmp(argv[0], "AP") == 0) {
			if (uap_ip_is_start()) {
				wifi_ap_config_t ap_info = {0};
				os_memset(&ap_info, 0x0, sizeof(ap_info));
				err = bk_wifi_ap_get_config(&ap_info);
				if(err != kNoErr) {
					os_printf("get ap link status fail!\n");
					err = kGeneralErr;
					goto error;
				}

				if (os_strcmp(argv[1], "SSID") == 0) {
					sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, ap_info.ssid, AT_CMD_RSP_SUCCEED);
				}
				else if (os_strcmp(argv[1], "CHANNEL") == 0) {
					sprintf(pcWriteBuffer, "%s:%d\r\n%s", AT_CMDRSP_HEAD, ap_info.channel, AT_CMD_RSP_SUCCEED);
				}
				else if (os_strcmp(argv[1], "SECURITY") == 0) {
					sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, wifi_type_string(ap_info.security), AT_CMD_RSP_SUCCEED);
				}

				else if (os_strcmp(argv[1], "IP") == 0) {
					netif_ip4_config_t ap_ip4_info = {0};
					err = bk_netif_get_ip4_config(NETIF_IF_AP, &ap_ip4_info);
					if(err != kNoErr) {
						os_printf("get ip fail!\n");
						err = kGeneralErr;
						goto error;
					}
					sprintf(pcWriteBuffer, "%s:IP=%s,GATE=%s,MASK=%s,DNS=%s\r\n%s", AT_CMDRSP_HEAD, ap_ip4_info.ip, ap_ip4_info.gateway, ap_ip4_info.mask, ap_ip4_info.dns, AT_CMD_RSP_SUCCEED);
				}
				else {
					os_printf("bad parameters\r\n");
					err = kParamErr;
					goto error;
				}
			}
			else {
				sprintf(pcWriteBuffer, "%s:%s\r\n%s", AT_CMDRSP_HEAD, "AP_WIFI_CLOSED", AT_CMD_RSP_SUCCEED);
				err = kGeneralErr;
			}
			return err;
		}
		else {
			os_printf("bad parameters\r\n");
			err = kParamErr;
			goto error;
		}
	}
	else {
		os_printf("bad parameters\r\n");
		err = kParamErr;
		goto error;
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

	if (argc == 1 && (os_strcmp("STOP", argv[0]) == 0)) {
		ping_stop();
		if(err == kNoErr) {
			msg = AT_CMD_RSP_SUCCEED;
			os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
			return err;
		}
	}
	
	if (argc > 1)
		cnt = os_strtoul(argv[1], NULL, 10);
	
	os_printf("ping IP address:%s\n", argv[0]);
	ping_start(argv[0], cnt, 0);
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
