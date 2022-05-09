#ifndef _WIFI_UI_EXTRA_H_
#define _WIFI_UI_EXTRA_H_

#include "wlan_ui_pub.h"

void los_wlan_ap_para_info_get(network_InitTypeDef_st *ap_info, unsigned char *security, unsigned char *chann);
void los_wlan_ap_para_info_set(network_InitTypeDef_st *ap_info, unsigned char security, unsigned char chann);
void los_wlan_start_ap(network_InitTypeDef_st *inNetworkInitParaAP, unsigned char security, unsigned char chann);
int los_wlan_start_sta(network_InitTypeDef_st *inNetworkInitPara, char *psk, unsigned int psk_len, int chan);

#endif // _WIFI_UI_EXTRA_H_
// eof

