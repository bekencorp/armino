#include <string.h>
#include <common/sys_config.h>
#include "bk_uart.h"
#include "param_config.h"
#include "bk_private/bk_wifi_wrapper.h"
#if CONFIG_LWIP
#include "net.h"
#endif
#include "bk_private/bk_wifi.h"
#include "bk_wifi_private.h"
#include "bk_cli.h"
#include "cli.h"
#include <components/event.h>
#include <components/netif.h>
#include "bk_wifi_wpa.h"
#include "bk_wifi_wpa_cmd.h"
#include "bk_wifi_frame.h"
#include "bk_wifi_types.h"
#if CONFIG_WIFI6_CODE_STACK
#include "bk_wifi_netif.h"
#include "bk_wifi.h"
#endif

#define TAG "wifi_cli"
#define CMD_WLAN_MAX_BSS_CNT	50

#if (CLI_CFG_WIFI == 1)
void cli_wifi_scan_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		demo_scan_app_init();
		return;
	} else {
		uint8_t *ap_ssid;

		ap_ssid = (uint8_t *)argv[1];
		demo_scan_adv_app_init(ap_ssid);
	}
}

void cli_wifi_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ap_ssid = NULL;
	char *ap_key = "";

	if (argc == 2)
		ap_ssid = argv[1];
	else if (argc == 3) {
		ap_ssid = argv[1];
		ap_key = argv[2];
	}

	if (ap_ssid)
		demo_softap_app_init(ap_ssid, ap_key);
}

void cli_wifi_stop_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc == 2) {
		if (os_strcmp(argv[1], "sta") == 0)
			BK_LOG_ON_ERR(bk_wifi_sta_stop());
		else if (os_strcmp(argv[1], "ap") == 0)
			BK_LOG_ON_ERR(bk_wifi_ap_stop());
		else
			CLI_LOGI("unknown WiFi interface\n");
	} else
		CLI_LOGI("bad parameters\r\n");
}

void cli_wifi_iplog_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *iplog_mode = NULL;

	if (argc == 2)
		iplog_mode = argv[1];

	if (iplog_mode)
		demo_wifi_iplog_init(iplog_mode);
}
typedef struct {
	uint8_t channel;
	uint32_t rx_cnt_mgmt;
	uint32_t rx_cnt_data;
	uint32_t rx_cnt_ctrl;
	uint32_t rx_cnt_0_255;
	uint32_t rx_cnt_256_511;
	uint32_t rx_cnt_512_1023;
	uint32_t rx_cnt_1024;
	uint32_t rx_cnt_total;
} cli_monitor_result_t;
static cli_monitor_result_t *s_monitor_result = NULL;

bk_err_t cli_monitor_cb(const uint8_t *data, uint32_t len, const wifi_frame_info_t *info)
{
	if (s_monitor_result) {
		s_monitor_result->rx_cnt_total++;

		if (data) {
			if ((data[0] & 0xc) == 0x8)
				s_monitor_result->rx_cnt_data ++;
			else if ((data[0] & 0xc) == 0x0)
				s_monitor_result->rx_cnt_mgmt ++;
			else
				s_monitor_result->rx_cnt_ctrl ++;
		}

		if (len < 256)
			s_monitor_result->rx_cnt_0_255++;
		else if (len < 512)
			s_monitor_result->rx_cnt_256_511++;
		else if (len < 1024)
			s_monitor_result->rx_cnt_512_1023++;
		else
			s_monitor_result->rx_cnt_1024++;
	}

	return BK_OK;
}

void cli_monitor_show(void)
{
	if (s_monitor_result) {
		BK_LOG_RAW("total:      %u\n", s_monitor_result->rx_cnt_total);
		BK_LOG_RAW("mgmt:       %u\n", s_monitor_result->rx_cnt_mgmt);
		BK_LOG_RAW("data:       %u\n", s_monitor_result->rx_cnt_data);
		BK_LOG_RAW("ctrl:       %u\n", s_monitor_result->rx_cnt_ctrl);
		BK_LOG_RAW("0 - 255:    %u\n", s_monitor_result->rx_cnt_0_255);
		BK_LOG_RAW("256 - 511:  %u\n", s_monitor_result->rx_cnt_256_511);
		BK_LOG_RAW("512 - 1023: %u\n", s_monitor_result->rx_cnt_512_1023);
		BK_LOG_RAW(">=1024:     %u\n", s_monitor_result->rx_cnt_1024);
	}
}

void cli_monitor_stop(void)
{
	if (s_monitor_result) {
		os_free(s_monitor_result);
		s_monitor_result = NULL;
	}

	BK_LOG_ON_ERR(bk_wifi_monitor_stop());
}

void cli_monitor_start(uint32_t primary_channel)
{
	wifi_channel_t chan = {0};

	chan.primary = primary_channel;

	if (!s_monitor_result) {
		s_monitor_result = os_zalloc(sizeof(cli_monitor_result_t));
		if (!s_monitor_result)
			CLI_LOGI("failed to alloc monitor result\n");
	}

	BK_LOG_ON_ERR(bk_wifi_monitor_register_cb(cli_monitor_cb));
	BK_LOG_ON_ERR(bk_wifi_monitor_start());
	BK_LOG_ON_ERR(bk_wifi_monitor_set_channel(&chan));
}

void cli_wifi_monitor_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t primary_channel;

	if (argc != 2) {
		CLI_LOGI("monitor_parameter invalid\r\n");
		return;
	}

	primary_channel = os_strtoul(argv[1], NULL, 10);
	if (99 == primary_channel)
		cli_monitor_stop();
	else if ((primary_channel > 0) && (primary_channel < 15))
		cli_monitor_start(primary_channel);
	else
		cli_monitor_show();
}

#include "conv_utf8_pub.h"
void cli_wifi_sta_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ssid = NULL;
	char *password = "";

	if ((argc < 2) || (argc > 5)) {
		CLI_LOGI("invalid argc number\n");
		return;
	}

	if (argc >= 2)
		ssid = argv[1];

	if (argc >= 3)
		password = argv[2];

#if 0 //TODO support BSSID/Channel configuration
	if (argc >= 4)
		bssid = argv[3];

	if (argc >= 5)
		channel = argv[4];
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
		CLI_LOGI("not buf for utf8\r\n");
	}
}

#if CONFIG_COMPONENTS_WPA2_ENTERPRISE
/**
 * cli command: sta_eap <ssid>, connect to EAP-TLS AP.
 *
 * restrictions: EAP-TLS is based on PKI, both AP and STA may have certificate. So
 * we must install certificate and private key to system. For example, `beken-iot-1.pem'
 * is STA's certificate, `beken-iot-1.key' is private key, `rootca.pem' is the RootCA.
 * These certificates and private key may be registered to system via `register_xfile'
 * function.
 */
void cli_wifi_sta_eap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ssid = NULL;
	char *ca = "cacert.pem";
	char *client_cert = "beken-iot-1.pem";
	char *private_key = "beken-iot-1.key";
	char *private_key_passwd = "12345678";
	char *identity = "user";

	if ((argc < 2) || (argc > 5)) {
		CLI_LOGI("invalid argc number\n");
		return;
	}

	ssid = argv[1];

	char *oob_ssid_tp = ssid;
#if CONFIG_USE_CONV_UTF8
	oob_ssid_tp = (char *)conv_utf8((uint8_t *)ssid);
#endif

	if (oob_ssid_tp) {
		int len;
		wifi_sta_config_t *sta_config;

		len = os_strlen((char *)oob_ssid_tp);
		if (WLAN_SSID_MAX_LEN < len) {
			CLI_LOGI("ssid name more than 32 Bytes\n");
			return;
		}

		sta_config = os_zalloc(sizeof(*sta_config));
		if (!sta_config) {
			CLI_LOGI("Cannot alloc STA config\n");
			return;
		}

		os_strlcpy(sta_config->ssid, oob_ssid_tp, sizeof(sta_config->ssid));
		sta_config->password[0] = '\0';	// No passwd needed fo EAP.
		os_strlcpy(sta_config->eap, "TLS", sizeof(sta_config->eap));
		os_strlcpy(sta_config->identity, identity, sizeof(sta_config->identity));
		os_strlcpy(sta_config->ca, ca, sizeof(sta_config->ca));
		os_strlcpy(sta_config->client_cert, client_cert, sizeof(sta_config->client_cert));
		os_strlcpy(sta_config->private_key, private_key, sizeof(sta_config->private_key));
		os_strlcpy(sta_config->private_key_passwd, private_key_passwd, sizeof(sta_config->private_key_passwd));
		os_strlcpy(sta_config->phase1, "tls_disable_time_checks=1", sizeof(sta_config->phase1));

		CLI_LOGI("ssid:%s key:%s\n", sta_config->ssid, sta_config->password);
		CLI_LOGI("eap:%s identity:%s\n", sta_config->eap, sta_config->identity);
		CLI_LOGI("ca:%s client_cert:%s\n", sta_config->ca, sta_config->client_cert);
		CLI_LOGI("private_key:%s\n", sta_config->private_key);
		BK_LOG_ON_ERR(bk_wifi_sta_set_config(sta_config));
		BK_LOG_ON_ERR(bk_wifi_sta_start());

		os_free(sta_config);

#if CONFIG_USE_CONV_UTF8
		os_free(oob_ssid_tp);
#endif
	} else {
		CLI_LOGI("not buf for utf8\r\n");
	}
}
#endif

void cli_wifi_state_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	demo_state_app_init();
}

#if CONFIG_WIFI_SENSOR
static void cli_wifi_sensor_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int status;

	if (argc != 2)
		bk_printf("param error");


	if (os_strcmp(argv[1], "start") == 0)
		bk_wifi_detect_movement_start();

	if (os_strcmp(argv[1], "stop") == 0)
		bk_wifi_detect_movement_stop();

	if (os_strcmp(argv[1], "status") == 0) {
		status = bk_get_movement_status();

		if (status == 0)
			bk_printf("detect something");
		else
			bk_printf("detect nothing");
	}
}
#endif

#if CONFIG_COMPONENTS_WFA_CA
extern void wfa_ca_start();
extern void wfa_ca_stop();

void cli_wifi_wfa_ca_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc != 2) {
		os_printf("param error");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0)
		wfa_ca_start();
	else if (os_strcmp(argv[1], "stop") == 0)
		wfa_ca_stop();
}
#endif

typedef struct {
	uint16_t cnt_probe_req;
	uint16_t cnt_probe_rsp;
	uint16_t cnt_beacon;
	uint16_t cnt_action;
	uint16_t cnt_auth;
	uint16_t cnt_assoc_req;
	uint16_t cnt_assoc_rsp;
	uint16_t cnt_others;
	uint16_t cnt_total;
} wifi_filter_result_t;

static wifi_filter_result_t *s_filter_result = NULL;

static int wifi_filter_cb(const uint8_t *data, uint32_t len, const wifi_frame_info_t *frame_info)
{
	if (!data) {
		CLI_LOGE("null data\n");
		return BK_OK;
	}

	if (!s_filter_result)
		return BK_OK;

	uint16_t framectrl = *(uint16_t*)(data);
	uint16_t type_subtype = framectrl & MAC_FCTRL_TYPESUBTYPE_MASK;

	if (type_subtype == MAC_FCTRL_BEACON)
		s_filter_result->cnt_beacon ++;
	else if (type_subtype == MAC_FCTRL_PROBEREQ)
		s_filter_result->cnt_probe_req++;
	else if (type_subtype == MAC_FCTRL_PROBERSP)
		s_filter_result->cnt_probe_rsp++;
	else if (type_subtype == MAC_FCTRL_ACTION)
		s_filter_result->cnt_action++;
	else if (type_subtype == MAC_FCTRL_AUTHENT)
		s_filter_result->cnt_auth++;
	else if (type_subtype == MAC_FCTRL_ASSOCREQ)
		s_filter_result->cnt_assoc_req++;
	else if (type_subtype == MAC_FCTRL_ASSOCRSP)
		s_filter_result->cnt_assoc_rsp++;
	else
		s_filter_result->cnt_others++;

	s_filter_result->cnt_total++;
	return BK_OK;
}

static void wifi_filter_result_dump(void)
{
	if (!s_filter_result)
		return;

	bk_printf("filter result:\n");
	bk_printf("total: %u\n", s_filter_result->cnt_total);
	bk_printf("beacon: %u\n", s_filter_result->cnt_beacon);
	bk_printf("probe req: %u\n", s_filter_result->cnt_probe_req);
	bk_printf("probe rsp: %u\n", s_filter_result->cnt_probe_rsp);
	bk_printf("auth: %u\n", s_filter_result->cnt_auth);
	bk_printf("assoc req: %u\n", s_filter_result->cnt_assoc_req);
	bk_printf("assoc rsp: %u\n", s_filter_result->cnt_assoc_rsp);
	bk_printf("action: %u\n", s_filter_result->cnt_action);
	bk_printf("others: %u\n", s_filter_result->cnt_others);
}

static void wifi_mgmt_filter_help(void)
{
	bk_printf("filter {filter_bitmap}\n");
	bk_printf("    bit0 - default management\n");
	bk_printf("    bit1 - probe req\n");
	bk_printf("    bit2 - probe rsp\n");
	bk_printf("    bit3 - all beacon\n");
	bk_printf("    bit4 - action\n");
	bk_printf("       0 - stop filter\n");
	bk_printf("      -1 - display result\n");
}

static void cli_wifi_filter_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_filter_config_t filter_config = {0};
	uint32_t filter = 0;

	if (argc != 2) {
		wifi_mgmt_filter_help();
		return;
	}

	filter = os_strtoul(argv[1], NULL, 0);

	if (filter == 0) {
		if (s_filter_result) {
			os_free(s_filter_result);
			s_filter_result = NULL;
		}
		BK_LOG_ON_ERR(bk_wifi_filter_set_config(&filter_config));
		BK_LOG_ON_ERR(bk_wifi_filter_register_cb(NULL));
		return;
	} else if (filter == -1) {
		wifi_filter_result_dump();
		return;
	}

	if (!s_filter_result) {
		s_filter_result = (wifi_filter_result_t *)os_zalloc(sizeof(wifi_filter_result_t));
		if (!s_filter_result)
			return;
	}

	if (filter & (1 << 0))
		filter_config.rx_all_default_mgmt = 1;

	if (filter & (1 << 1))
		filter_config.rx_probe_req = 1;

	if (filter & (1 << 2))
		filter_config.rx_probe_rsp = 1;

	if (filter & (1 << 3))
		filter_config.rx_all_beacon = 1;

	if (filter & (1 << 4))
		filter_config.rx_action = 1;

	BK_LOG_ON_ERR(bk_wifi_filter_set_config(&filter_config));
	BK_LOG_ON_ERR(bk_wifi_filter_register_cb(wifi_filter_cb));
}

#if CONFIG_WIFI_RAW_TX_TEST

typedef struct {
	uint32_t interval;
	uint32_t counter;
} wifi_raw_tx_param_t;

static void wifi_raw_tx_thread(void *arg)
{
	char frame[] = {
		0xB0, //version, type, subtype
		0x00, //frame control
		0x3A, 0x01, //duration
		0xC8, 0x47, 0x8C, 0x42, 0x00, 0x48, //Address1 - destination
		0x4C, 0xD1, 0xA1, 0xC5, 0x38, 0xE4, //Address2 - source
		0x4C, 0xD1, 0xA1, 0xC5, 0x38, 0xE4, //Address3 - bssid
		0x20, 0xC0, //sequence

		//Auth Response
		0x00, 0x00, //Auth algorithm - open system
		0x02, 0x00, //Auth seq num
		0x00, 0x00, //Status code
	};
	wifi_raw_tx_param_t *tx_param;
	int ret;

	tx_param = (wifi_raw_tx_param_t *)arg;
	CLI_LOGI("wifi raw tx begin, interval=%u counter=%d\n", tx_param->interval,
			 tx_param->counter);

	for (uint32_t i = 0; i < tx_param->counter; i++) {
		ret = bk_wlan_send_80211_raw_frame((unsigned char *)frame, sizeof(frame));
		if (ret != kNoErr)
			CLI_LOGI("raw tx error, ret=%d\n", ret);

		rtos_delay_milliseconds(tx_param->interval);
	}

	os_free(arg);
	CLI_LOGI("wifi raw tx end\n");
	rtos_delete_thread(NULL);
}

static void cli_wifi_raw_tx_cmd(char *pcWriteBuffer, int xWriteBufferLen,
								int argc, char **argv)
{
	bk_err_t ret;

	if (argc != 3) {
		CLI_LOGE("param error");
		CLI_LOGI("usage: wifi_raw_tx interval counter");
		return;
	}

	wifi_raw_tx_param_t *tx_param;
	tx_param = (wifi_raw_tx_param_t *)os_malloc(sizeof(wifi_raw_tx_param_t));
	if (!tx_param) {
		CLI_LOGE("out of memory\n");
		return;
	}

	tx_param->interval = os_strtoul(argv[1], NULL, 10);
	tx_param->counter = os_strtoul(argv[2], NULL, 10);
	ret = rtos_create_thread(NULL, 2, "raw_tx",
		(beken_thread_function_t)wifi_raw_tx_thread,
		2048, tx_param);
	if (kNoErr != ret) {
		os_free(tx_param);
		CLI_LOGI("Create raw tx thread failed, ret=%d\r\n", ret);
		return;
	}
}
#endif

static void cli_wifi_monitor_channel_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	wifi_channel_t chan = {0};
	int channel, i = 0;

	if (argc == 1) {
		CLI_LOGI("Usage: channel [1~13].");
		return;
	}

	while (argv[1][i]) {
		if ((argv[1][i] < '0') || (argv[1][i] > '9')) {
			CLI_LOGE("parameter should be a number\r\n");
			return ;
		}
		i++;
	}

	channel = atoi(argv[1]);

	if ((channel < 1) || (channel > 13)) {
		CLI_LOGE("Invalid channel number \r\n");
		return ;
	}
	BK_LOG_RAW("monitor mode, set to channel %d\r\n", channel);
	chan.primary = channel;
	BK_LOG_ON_ERR(bk_wifi_monitor_set_channel(&chan));
}

int cli_netif_event_cb(void *arg, event_module_t event_module,
					   int event_id, void *event_data)
{
	netif_event_got_ip4_t *got_ip;

	switch (event_id) {
	case EVENT_NETIF_GOT_IP4:
		got_ip = (netif_event_got_ip4_t *)event_data;
		CLI_LOGI("%s got ip\n", got_ip->netif_if == NETIF_IF_STA ? "BK STA" : "unknown netif");
#if CONFIG_WIFI6_CODE_STACK
		unsigned char vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);
              wlan_dhcp_done_ind(vif_idx);
#endif
		break;
	default:
		CLI_LOGI("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

int cli_wifi_event_cb(void *arg, event_module_t event_module,
					  int event_id, void *event_data)
{
	wifi_event_sta_disconnected_t *sta_disconnected;
	wifi_event_sta_connected_t *sta_connected;
	wifi_event_ap_disconnected_t *ap_disconnected;
	wifi_event_ap_connected_t *ap_connected;

	switch (event_id) {
	case EVENT_WIFI_STA_CONNECTED:
		sta_connected = (wifi_event_sta_connected_t *)event_data;
		CLI_LOGI("BK STA connected %s\n", sta_connected->ssid);
		break;

	case EVENT_WIFI_STA_DISCONNECTED:
		sta_disconnected = (wifi_event_sta_disconnected_t *)event_data;
		CLI_LOGI("BK STA disconnected, reason(%d)%s\n", sta_disconnected->disconnect_reason,
			sta_disconnected->local_generated ? ", local_generated" : "");
		break;

	case EVENT_WIFI_AP_CONNECTED:
		ap_connected = (wifi_event_ap_connected_t *)event_data;
		CLI_LOGI(BK_MAC_FORMAT" connected to BK AP\n", BK_MAC_STR(ap_connected->mac));
		break;

	case EVENT_WIFI_AP_DISCONNECTED:
		ap_disconnected = (wifi_event_ap_disconnected_t *)event_data;
		CLI_LOGI(BK_MAC_FORMAT" disconnected from BK AP\n", BK_MAC_STR(ap_disconnected->mac));
		break;

	default:
		CLI_LOGI("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

void cli_wifi_net_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char buf[128];
	int i, left = sizeof(buf) - 1, len = 0;

	// net sta xxx
	// net ap xxx
	if (argc <= 2) {
		CLI_LOGI("Usage: net sta/ap <param...>\n");
		return;
	}

	buf[0] = 0;
	for (i = 2; i < argc; i++) {
		len = os_strlen(buf);
		snprintf(buf + len, left - len, "%s ", argv[i]);
	}
	buf[strlen(buf) - 1] = 0;
	//CLI_LOGI("CMD: |%s|\n", buf);

#if 1
	if (os_strcmp(argv[1], "sta") == 0)
		cmd_wlan_sta_exec(buf);
	else if (os_strcmp(argv[1], "ap") == 0)
		cmd_wlan_ap_exec(buf);
#if CONFIG_COMPONENTS_P2P
	else if (os_strcmp(argv[1], "p2p") == 0)
		cmd_wlan_p2p_exec(buf);
#endif
	else {
		CLI_LOGI("Usage: net sta/ap <param...>\n");
		return;
	}
#endif
}

void cli_wifi_get_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv) {
	// get pm status
	// get xx status
	if (argc <= 2) {
		CLI_LOGI("Usage get xx status\n");
		return;
	}

	if(os_strcmp(argv[1], "ps") == 0) {
		int state = 0;
		if(os_strcmp(argv[2], "status") == 0) {
			state = cmd_wlan_get_ps_status();
			CLI_LOGI("ps status: %s \n", (state?"sleep":"active"));
		} else {
			CLI_LOGI("Usage get ps status\n");
		}
	}
}

#ifdef CONFIG_COMPONENTS_WPA_TWT_TEST
void cli_wifi_twt_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint16_t mantissa = 0;
	uint8_t min_twt = 0;
	
	if(os_strcmp(argv[1], "setup") == 0) {
		int setup_command = 0;

		if(os_strcmp(argv[2], "suggest") == 0) {
			setup_command = 1;
		}
		else if(os_strcmp(argv[2], "demand") == 0) {
			setup_command = 2;
		}
		else {
			CLI_LOGI("Usage: twt setup suggest/demand <param...>\n");
			return;
		}
		mantissa = os_strtoul(argv[3], NULL, 10) & 0xFF;
		min_twt = os_strtoul(argv[4], NULL, 10) & 0xFF;
		bk_wifi_twt_setup(setup_command, mantissa, min_twt);
	}
	else if (os_strcmp(argv[1], "teardown") == 0)
		bk_wifi_twt_teardown();
	else
		CLI_LOGI("Usage: twt setup/teardown \n");
}
#endif

#define WIFI_CMD_CNT (sizeof(s_wifi_commands) / sizeof(struct cli_command))
static const struct cli_command s_wifi_commands[] = {
	{"scan", "scan [ssid]", cli_wifi_scan_cmd},
	{"ap", "ap ssid [password]", cli_wifi_ap_cmd},
	{"sta", "sta ssid [password][bssid][channel]", cli_wifi_sta_cmd}, //TODO support connect speicific BSSID
#if CONFIG_COMPONENTS_WPA2_ENTERPRISE
	{"sta_eap", "sta_eap ssid password [identity] [client_cert] [private_key]", cli_wifi_sta_eap_cmd},
#endif
	{"stop", "stop {sta|ap}", cli_wifi_stop_cmd},
	{"monitor", "monitor {1~13|15|99}", cli_wifi_monitor_cmd},
	{"state", "state - show STA/AP state", cli_wifi_state_cmd},
	{"channel", "channel {1~13} - set monitor channel", cli_wifi_monitor_channel_cmd},
	{"net", "net {sta/ap} ... - wifi net config", cli_wifi_net_cmd},
	{"get", "get wifi status", cli_wifi_get_cmd},
	{"iplog", "iplog [modle]", cli_wifi_iplog_cmd},

#ifdef CONFIG_COMPONENTS_WPA_TWT_TEST
	{"twt", "twt {setup|teardown}", cli_wifi_twt_cmd},
#endif

#if CONFIG_COMPONENTS_WFA_CA
	{"wfa_ca", "wfa_ca <start|stop>", cli_wifi_wfa_ca_cmd},
#endif

#if CONFIG_WIFI_SENSOR
	{"wifisensor", "wifi sensor", cli_wifi_sensor_cmd},
#endif
	{"filter", "filter <bits> - bit0/d, 1/preq, 2/prsp, 3/b, 4/a", cli_wifi_filter_cmd},
#if CONFIG_WIFI_RAW_TX_TEST
	{"wifi_tx", "wifi_tx - Tx WiFi raw frame", cli_wifi_raw_tx_cmd},
#endif
};

int cli_wifi_init(void)
{
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_ID_ALL, cli_wifi_event_cb, NULL));
	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_NETIF, EVENT_ID_ALL, cli_netif_event_cb, NULL));
	return cli_register_commands(s_wifi_commands, WIFI_CMD_CNT);
}

#endif //#if (CLI_CFG_WIFI == 1)
