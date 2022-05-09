#include "atsvr_wlan.h"
#include "atsvr_core.h"
#include "at_server.h"
#include "atsvr_cmd.h"
#include "at_server.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include <common/bk_include.h>
#include "wlan_ui_pub.h"
#include "bk_rw_msg.h"
#include <os/mem.h>
#include <os/os.h>
#include <common/bk_generic.h>
#include "param_config.h"
#include <components/log.h>
#include <components/netif.h>
#include <modules/wifi.h>
#include <components/event.h>

typedef struct{
	unsigned dhcp : 1;
	unsigned station_status : 1;
#define STATION_UP     1
#define STATION_DOWN   0
	unsigned softap_status : 1;
#define SOFTAP_UP     1
#define SOFTAP_DOWN   0

	unsigned char static_ip[16];
	unsigned char static_maks[16];
	unsigned char static_gate[16];
	unsigned char static_dns1[16];
	unsigned char static_dns2[16];

	unsigned char softap_ip[16];
	unsigned char softap_mask[16];
	unsigned char softap_gate[16];
	unsigned char softap_dns[16];
}atsvr_wlan_t;

atsvr_wlan_t atsvr_wlan = {
	.dhcp = ATSVR_WLAN_STA_DHCP,
	.station_status = STATION_DOWN,
	.softap_status = SOFTAP_DOWN,
	.static_ip = ATSVR_WLAN_STA_DEFAULT_IP,
	.static_maks = ATSVR_WLAN_STA_DEFAULT_MASK,
	.static_gate = ATSVR_WLAN_STA_DEFAULT_GW,
	.static_dns1 = ATSVR_WLAN_STA_DEFAULT_DNS,
	.static_dns2 = ATSVR_WLAN_STA_DEFAULT_DNS,

	.softap_ip = ATSVR_WLAN_DEFAULT_IP,
	.softap_mask = ATSVR_WLAN_DEFAULT_MASK,
	.softap_gate = ATSVR_WLAN_DEFAULT_GW,
	.softap_dns = ATSVR_WLAN_DEFAULT_DNS,
};



void wlan_get_station_mac_address(char *mac)
{
	bk_wifi_sta_get_mac(mac);
}

void wlan_get_softap_mac_address(char *mac)
{
	bk_wifi_ap_get_mac(mac);
}

int wlan_start_station_connect(char *my_ssid,char* connect_key)
{
	wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();
	network_InitTypeDef_st wNetConfig;
	int dhcp_sta;
	unsigned char *bssid = NULL;

	memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));
	dhcp_sta = wlan_get_station_dhcp();

	memcpy(sta_config.ssid, my_ssid,32);
	sta_config.ssid[32] = '\0';
	memcpy(sta_config.password, connect_key,64);

	if(!dhcp_sta){
		netif_ip4_config_t ip4_config = {0};

		ATSVRLOG("DHCP Static ip:%s,maks:%s,gate:%s,dns1:%s,dns2:%s\r\n",
			atsvr_wlan.static_ip,atsvr_wlan.static_maks, atsvr_wlan.static_gate,
			atsvr_wlan.static_dns1, atsvr_wlan.static_dns2);
		strncpy(ip4_config.ip, (char *)atsvr_wlan.static_ip,16);
		strncpy(ip4_config.mask, (char *)atsvr_wlan.static_maks,16);
		strncpy(ip4_config.gateway, (char *)atsvr_wlan.static_gate,16);
		strncpy(ip4_config.dns, (char *)atsvr_wlan.static_dns1,16);
		BK_LOG_ON_ERR(bk_netif_set_ip4_config(NETIF_IF_STA, &ip4_config));
	}

	if ( bssid != NULL){
		memcpy(sta_config.bssid, bssid, 6);
	}

	ATSVRLOG("atsvr_ssid:%s atsvr_key:%s\r\n", sta_config.ssid, sta_config.password);
	BK_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
	BK_LOG_ON_ERR(bk_wifi_sta_start());

	return 0;
}

int wlan_stop_station(void)
{
	BK_LOG_ON_ERR(bk_wifi_sta_stop());
	return 0;
}

void wlan_set_station_dhcp(int en)
{
	atsvr_wlan.dhcp = ( en ) ? 1 : 0;
}

int wlan_get_station_dhcp(void)
{
	return ( atsvr_wlan.dhcp != 0 ) ? 1 : 0;
}

int wlan_set_station_static_ip(char *ip,char *mask,char *gate,char *dns,char *dns2)
{
	strncpy((char *)atsvr_wlan.static_ip,ip,16);
    strncpy((char *)atsvr_wlan.static_maks,mask,16);
    strncpy((char *)atsvr_wlan.static_gate,gate,16);
	if( dns != NULL){
		strncpy((char *)atsvr_wlan.static_dns1,dns,16);
	}
	if( dns != NULL){
		strncpy((char *)atsvr_wlan.static_dns2,dns2,16);
	}
	return 0;
}

int wlan_set_station_dns(char *dns_ip,char *dns2_ip)
{
	if( dns_ip != NULL ) {
		strncpy((char *)atsvr_wlan.static_dns1,dns_ip,16);
	}
	if( dns2_ip != NULL ) {
		strncpy((char *)atsvr_wlan.static_dns2,dns2_ip,16);
	}
	return 0;
}

int wlan_get_station_cur_status(void)
{
	return (atsvr_wlan.station_status == STATION_UP) ? 1 : 0;
}

int wlan_softap_start(char *ap_ssid, char *ap_key)
{
	wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();
	netif_ip4_config_t ip4_config = {0};
	int len;

    len = strlen(ap_ssid);
    if(ATSVR_MAX_SSID_LEN < len) {
        ATSVRLOG("ssid name more than 32 Bytes\r\n");
        return -1;
    }

    strncpy(ap_config.ssid, ap_ssid,33);
    strncpy(ap_config.password, ap_key,64);

    strncpy(ip4_config.ip, (char *)atsvr_wlan.softap_ip,16);
    strncpy(ip4_config.mask, (char *)atsvr_wlan.softap_mask,16);
    strncpy(ip4_config.gateway, (char *)atsvr_wlan.softap_gate,16);
    strncpy(ip4_config.dns, (char *)atsvr_wlan.softap_dns,16);

    ATSVRLOG("softap-ssid:%s  |  key:%s\r\n", ap_config.ssid, ap_config.password);
	BK_LOG_ON_ERR(bk_netif_set_ip4_config(NETIF_IF_AP, &ip4_config));
	BK_LOG_ON_ERR(bk_wifi_ap_set_config(&ap_config));
	BK_LOG_ON_ERR(bk_wifi_ap_start());
	return 0;
}

int wlan_stop_softap(void)
{
	BK_LOG_ON_ERR(bk_wifi_ap_stop());
	return 0;
}

int wlan_set_softap_static_ip(char *ip,char *mask,char *gate,char *dns_ip)
{
	strncpy((char *)atsvr_wlan.softap_ip,ip,16);
    strncpy((char *)atsvr_wlan.softap_mask,mask,16);
    strncpy((char *)atsvr_wlan.softap_gate,gate,16);
	if(dns_ip  != NULL){
		strncpy((char *)atsvr_wlan.softap_dns,dns_ip,16);
	}
	return 0;
}

int wlan_set_softap_dns(char *dns_ip)
{
	strncpy((char *)atsvr_wlan.softap_dns,dns_ip,16);
	return 0;
}

int wlan_get_softap_cur_status(void)
{
	return (atsvr_wlan.softap_status == SOFTAP_UP) ? 1 : 0;
}

atsvr_wlan_sec_type wlan2atsvr_sec_type(int sec_type)
{
	switch(sec_type){
	case BK_SECURITY_TYPE_NONE:
		return ATSVR_SEC_TYPE_NONE;
	case BK_SECURITY_TYPE_WEP:
		return ATSVR_SEC_TYPE_WEP;
	case BK_SECURITY_TYPE_WPA_TKIP:
		return ATSVR_SEC_TYPE_WPA_TKIP;
	case BK_SECURITY_TYPE_WPA_AES:
		return ATSVR_SEC_TYPE_WPA_AES;
	case BK_SECURITY_TYPE_WPA2_TKIP:
		return ATSVR_SEC_TYPE_WPA2_TKIP;
	case BK_SECURITY_TYPE_WPA2_AES:
		return ATSVR_SEC_TYPE_WPA2_AES;
	case BK_SECURITY_TYPE_WPA2_MIXED:
		return ATSVR_SEC_TYPE_WPA2_MIXED;
	case BK_SECURITY_TYPE_WPA3_SAE:
		return ATSVR_SEC_TYPE_WPA3_SAE;
	case BK_SECURITY_TYPE_WPA3_WPA2_MIXED:
		return ATSVR_SEC_TYPE_WPA3_WPA2_MIXED;
	case BK_SECURITY_TYPE_OWE:
		return ATSVR_SEC_TYPE_OWE;
	case BK_SECURITY_TYPE_AUTO:
		return ATSVR_SEC_TYPE_AUTO;
	}
	return ATSVR_SEC_TYPE_AUTO;
}

int atsvr2wlan_sec_type(atsvr_wlan_sec_type sec_type)
{
	switch(sec_type){
	case ATSVR_SEC_TYPE_NONE:
		return BK_SECURITY_TYPE_NONE;
	case ATSVR_SEC_TYPE_WEP:
		return BK_SECURITY_TYPE_WEP;
	case ATSVR_SEC_TYPE_WPA_TKIP:
		return BK_SECURITY_TYPE_WPA_TKIP;
	case ATSVR_SEC_TYPE_WPA_AES:
		return BK_SECURITY_TYPE_WPA_AES;
	case ATSVR_SEC_TYPE_WPA2_TKIP:
		return BK_SECURITY_TYPE_WPA2_TKIP;
	case ATSVR_SEC_TYPE_WPA2_AES:
		return BK_SECURITY_TYPE_WPA2_AES;
	case ATSVR_SEC_TYPE_WPA2_MIXED:
		return BK_SECURITY_TYPE_WPA2_MIXED;
	case ATSVR_SEC_TYPE_WPA3_SAE:
		return BK_SECURITY_TYPE_WPA3_SAE;
	case ATSVR_SEC_TYPE_WPA3_WPA2_MIXED:
		return BK_SECURITY_TYPE_WPA3_WPA2_MIXED;
	case ATSVR_SEC_TYPE_OWE:
		return BK_SECURITY_TYPE_OWE;
	case ATSVR_SEC_TYPE_AUTO:
		return BK_SECURITY_TYPE_AUTO;
	default:
		break;
	}
	return BK_SECURITY_TYPE_AUTO;
}


static beken_semaphore_t atsvr_scan_sema = NULL;

static int wlan_scan_done_handler(void *arg, event_module_t event_module,
        int event_id, void *event_data)
{
	if (atsvr_scan_sema) {
		rtos_set_semaphore( &atsvr_scan_sema );
	}
	return BK_OK;
}

int atsvr_wlan_scan_ap_sorting(int ap_num,struct sta_scan_res *table,char **out)
{
	char *array = NULL;
	char t_rssi[64][2];
	char temp[2];
	unsigned char i,j;

	if(out == NULL){
		return -1;
	}
	*out = NULL;
	array = at_malloc(ap_num);
	if(array == NULL){
		return -1;
	}
	for(i=0;i<ap_num;i++) {
		t_rssi[i][0] = i;
		t_rssi[i][1] = table[i].level;
	}

	for(i=0;i<ap_num;i++){
		for(j=i;j<ap_num;j++){
			if((char)t_rssi[j][1] > (char)t_rssi[i][1]){
				temp[0] = t_rssi[i][0];
				temp[1] = t_rssi[i][1];
				t_rssi[i][0] = t_rssi[j][0];
				t_rssi[i][1] = t_rssi[j][1];
				t_rssi[j][0] = temp[0];
				t_rssi[j][1] = temp[1];
			}
		}
	}

	for(i = 0; i < ap_num; i++){
		array[i] = t_rssi[i][0];
	}
	*out = array;
	return 0;
}

static int atsvr_wlan_scan_find_ssid_len(unsigned char *ssid)
{
	int i;

	for(i = 0;i < 32; i++){
		if(ssid[i] == '\0'){
			break;
		}
	}
	return i;
}

int atsvr_wlan_scan_ap_result(void)
{
        wifi_scan_result_t scan_result = {0};
	char *resultbuf  = NULL;
	int i;
	int n = 0;

        BK_LOG_ON_ERR(bk_wifi_scan_dump_result(&scan_result));
        bk_wifi_scan_free_result(&scan_result);

#define ATSVR_WLAN_SCAN_BUF_SIZE     (5 * 1024)
	resultbuf = at_malloc(ATSVR_WLAN_SCAN_BUF_SIZE);
	if(resultbuf == NULL) {
		ATSVRLOG("resultbuf malloc failed!\r\n");
		return 1;
	}
		if (bk_wifi_scan_get_result(&scan_result) == BK_OK) {
			int ap_num = scan_result.ap_num;

			bk_printf("Got ap count: %d\r\n", scan_result.ap_num);
			resultbuf[0] = '\0';
			for (i = 0; i < ap_num; i++) {
				if(atsvr_wlan_scan_find_ssid_len((unsigned char*)scan_result.aps[i].ssid) >= 32){
					char temp[33];
					memcpy(temp,scan_result.aps[i].ssid,32);
					temp[32] = '\0';
					n += sprintf(resultbuf+n,ATSVR_CMDRSP_HEAD"+WSCAN:%s,",temp);
				}else{
					n += sprintf(resultbuf+n,ATSVR_CMDRSP_HEAD"+WSCAN:%s,",scan_result.aps[i].ssid);
				}
				switch(scan_result.aps[i].security) {
				case BK_SECURITY_TYPE_NONE:
					n += sprintf(resultbuf+n,"%s","OPEN");
					break;
				case BK_SECURITY_TYPE_WEP:
					n += sprintf(resultbuf+n,"%s","WEP_SHARED");
					break;
				case BK_SECURITY_TYPE_WPA_TKIP:
					n += sprintf(resultbuf+n,"%s","WPA_TKIP_PSK");
					break;
				case BK_SECURITY_TYPE_WPA_AES:
					n += sprintf(resultbuf+n,"%s","WPA_AES_PSK");
					break;
				case BK_SECURITY_TYPE_WPA2_TKIP:
					n += sprintf(resultbuf+n,"%s","WPA2_TKIP_PSK");
					break;
				case BK_SECURITY_TYPE_WPA2_AES:
					n += sprintf(resultbuf+n,"%s","WPA2_AES_PSK");
					break;
				case BK_SECURITY_TYPE_WPA2_MIXED:
					n += sprintf(resultbuf+n,"%s","WPA2_MIXED_PSK");
					break;
				case BK_SECURITY_TYPE_WPA3_SAE:
					n += sprintf(resultbuf+n,"%s","WPA3_SAE");
					break;
				case BK_SECURITY_TYPE_WPA3_WPA2_MIXED:
					n += sprintf(resultbuf+n,"%s","WPA3_WPA2_MIXED");
					break;
				case BK_SECURITY_TYPE_OWE:
					n += sprintf(resultbuf+n,"%s","OWE");
					break;
				case BK_SECURITY_TYPE_AUTO:
					n += sprintf(resultbuf+n,"%s","AUTO_PSK");
					break;
				default:
					n += sprintf(resultbuf+n,"%s","UNKNOWN");
					break;
				}
				n += sprintf(resultbuf+n,",%d",ATSVR_RSSI2APPOWER(scan_result.aps[i].rssi)); ///RSSI
				n += sprintf(resultbuf+n,","BK_MAC_FORMAT, BK_MAC_STR(scan_result.aps[i].bssid));
				n += sprintf(resultbuf+n,",%d\r\n", scan_result.aps[i].channel);
				if(n > (ATSVR_WLAN_SCAN_BUF_SIZE - 512)){
					atsvr_output_msg(resultbuf,strlen(resultbuf));
					resultbuf[0] = '\0';
					n = 0;
				}
			}

			bk_wifi_scan_free_result(&scan_result);
			atsvr_output_msg(resultbuf,strlen(resultbuf));
		}

	if(resultbuf) {
		at_free(resultbuf);
		resultbuf = NULL;
	}
	return 0;
}

int wlan_scan_start(char *ssid)
{
	int err;

	if( atsvr_scan_sema == NULL ) {
		err = rtos_init_semaphore( &atsvr_scan_sema, 1 );
		if(err != kNoErr){
			return -1;
		}
	}

        bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
                wlan_scan_done_handler, NULL);

	BK_LOG_ON_ERR(bk_wifi_scan_start(NULL));
	if(atsvr_scan_sema != NULL) {
		err = rtos_get_semaphore(&atsvr_scan_sema, ATSVR_SCAN_TIMEOUT_MS);
		if(err != kNoErr) {
			ATSVRLOG("atsvr scan wait semaphore error!\r\n");
		} else {
			ATSVRLOG("atsvr scan wait semaphore error!\r\n");
		}
	} else {
		ATSVRLOG("atsvr waited semaphore!\r\n");
		return -1;
	}

	return 0;
}


int wlan_event_handler(int event)
{
	int ret = 0;
	switch(event){
	case ATSVR_WLAN_DISCONNECT:
		atsvr_wlan.station_status = STATION_DOWN;
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_EVT_WLAN_DISCONNECTED);
		break;
	case ATSVR_WLAN_CONNECTTED:
		atsvr_wlan.station_status = STATION_UP;
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_EVT_WLAN_CONNECTED);
		break;
	case ATSVR_WLAN_GOT_IP:
		ATSVR_SIZEOF_OUTPUT_STRRING(ATSVR_EVT_GOT_IP);
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}


static void wlan_status_callback(void *ctxt)
{
	wifi_linkstate_reason_t info = *(wifi_linkstate_reason_t info*)ctxt;

	ATSVRLOG("[WLAN]event(%x)\r\n", info.state);
	switch(info.state){
	case WIFI_LINKSTATE_STA_GOT_IP:
		atsvr_event_sender(ATSVR_WLAN_GOT_IP);
		break;
	case WIFI_LINKSTATE_STA_CONNECTED:
		atsvr_event_sender(ATSVR_WLAN_CONNECTTED);
		break;
	case WIFI_LINKSTATE_STA_DISCONNECTED:
		atsvr_event_sender(ATSVR_WLAN_DISCONNECT);
		break;
	default:
		break;
	}
}

int judge_the_string_is_ipv4_string(char *is_ip_string)
{
	int len = strlen(is_ip_string);

	if((len > 16 ) || (len < 7 )){
		return -1;
	}
	if(is_ip_string[(len - 1)] == '.'){
		return -1;
	}
	int num;
	int point_num = 0;
	char *p,*p_prev = is_ip_string;
	char ip_num[4];
	int i,j;

	while(1){
		p = strchr(p_prev, '.');
		if((p == NULL) && (point_num != 3)) {
			return -1;
		}
		point_num++;
		i = p - p_prev;
		if((i <= 3) && (i > 0)){
			for(j = 0; j < i; j++) {
				ip_num[j] = p_prev[j];
				if((ip_num[j] < '0') || (ip_num[j] > '9')){
					return -1;
				}
			}
			ip_num[j] = '\0';
			num = atoi( ip_num );
			if(num < 0 || num > 255){
				return -1;
			}else {
				p_prev = p + 1;
				if(point_num == 3) {
					p = strchr(p_prev, '.');
					if(p != NULL){
						return -1;
					}
					i = strlen(p_prev);
					if(i > 3 || i <= 0){
						return -1;
					}
					for(j = 0; j < i; j++) {
						ip_num[j] = p_prev[j];
						if((ip_num[j] < '0') || (ip_num[j] > '9')){
							return -1;
						}
					}
					ip_num[j] = '\0';
					num = atoi( ip_num );
					if(num < 0 || num > 255){
						return -1;
					}
					return 0;
				}
			}
		}else{
			return -1;
		}
	}

	return 0;
}

void atsvr_wlan_init(void)
{
	extern void bk_wlan_status_register_cb(FUNC_1PARAM_PTR cb);
	bk_wlan_status_register_cb(wlan_status_callback);
}

