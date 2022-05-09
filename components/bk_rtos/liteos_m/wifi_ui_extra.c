#include <common/bk_include.h>
#include "wlan_ui_pub.h"
#include "param_config.h"
#include "common/ieee802_11_defs.h"
#include "driver_beken.h"
#include "main_none.h"
#include <common/bk_err.h>
#include "lwip_netif_address.h"
#include "lwip/inet.h"
#include <stdbool.h>
#include "net.h"
#include "wpa_psk_cache.h"
#include "drv_model_pub.h"
#include <os/str.h>
#include "wifi_ui_extra.h"

extern void bk_wlan_set_coexist_at_init_phase(uint8_t current_role);

void los_wlan_ap_para_info_get(network_InitTypeDef_st *ap_info, unsigned char *security, unsigned char *chann)
{
	bk_printf("los_wlan_ap_para_info_get TODO\r\n");
}

void los_wlan_ap_para_info_set(network_InitTypeDef_st *ap_info, unsigned char security, unsigned char chann)
{
	bk_printf("los_wlan_ap_para_info_set TODO\r\n");
}

void los_wlan_ap_init(network_InitTypeDef_st *inNetworkInitPara, unsigned char security, unsigned char chann)
{
	bk_printf("los_wlan_ap_init TODO\r\n");
}

void los_wlan_start_ap(network_InitTypeDef_st *inNetworkInitParaAP, unsigned char security, unsigned char chann)
{
	bk_printf("los_wlan_start_ap TODO\r\n");
}

int los_wlan_start_sta(network_InitTypeDef_st *inNetworkInitPara, char *psk, unsigned int psk_len, int chan)
{
	bk_printf("los_wlan_start_sta TODO\r\n");
	return 0;
}
// eof

