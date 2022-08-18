// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* TODO:
 * 1. bk_wifi.h is used to define the new beken WiFi APIs
 * 2. Move bk_wifi.h from private to public API after all these
 *    APIs are passed beken test
 * */
#pragma once
#include "bk_private/bk_wifi_types.h"
#include "modules/wifi_types.h"
#include "wlan_defs_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

int wlan_sta_set(const uint8_t *ssid, uint8_t ssid_len, const uint8_t *psk);
int wlan_sta_set_config(wlan_sta_config_t *config);
int wlan_sta_get_config(wlan_sta_config_t *config);
int wlan_sta_set_autoreconnect(wlan_auto_reconnect_t *config);
int wlan_sta_get_bss_size(uint32_t * size);
int wlan_sta_get_bss(wlan_sta_bss_info_t * bss_get);
int wlan_sta_set_bss(wlan_sta_bss_info_t * bss_set);
int wlan_sta_enable(void);
int wlan_sta_scan_once(void);
int wlan_sta_scan(wlan_sta_scan_param_t *param);
int wlan_sta_twt_setup(uint8_t annouced, uint16_t wake_interval_mantissa, uint8_t wake_interval_exp,
		uint8_t nominal_min_wake_dur, bool wake_dur_unit);
int wlan_sta_twt_teardown(int flow_id);
//It used legacy ScanResult_adv/ApListStruct, to put it to legacy API
//int wlan_sta_scan_result(ScanResult_adv *results);
//int wlan_sta_ap_info(struct ApListStruct *ap);
int wlan_sta_scan_interval(int sec);
int wlan_sta_bss_max_count(uint8_t count);
int wlan_sta_bss_flush(int age);
int wlan_sta_connect(int chan);
int wlan_sta_disconnect(void);
int wlan_sta_state(wlan_sta_states_t *state);
int wlan_sta_gen_psk(wlan_gen_psk_param_t *param);
int wlan_ap_set(uint8_t *ssid, uint8_t ssid_len, uint8_t *psk);
int wlan_sta_wps_pbc(void);
int wlan_sta_wps_pin_get(wlan_sta_wps_pin_t *wps);
int wlan_sta_wps_pin_set(wlan_sta_wps_pin_t *wps);
int wlan_sta_set_vendor_ie(wlan_sta_vsie_t *vsie);
int wlan_ap_set_config(wlan_ap_config_t *config);
int wlan_ap_get_config(wlan_ap_config_t *config);
int wlan_ap_enable(void);
int wlan_ap_reload(void);
int wlan_ap_sta_num(int *num);
int wlan_ap_sta_info(wlan_ap_stas_t *stas);
int wlan_ap_set_beacon_vsie(wlan_ap_vsie_t *vsie);
int wlan_p2p_listen(void);
int wlan_p2p_find(void);
int wlan_p2p_stop_find(void);
int wlan_p2p_cancel(void);
int wlan_p2p_connect(const uint8_t *mac, int method, int intent);
#if CONFIG_WIFI6_CODE_STACK
int wlan_dhcp_done_ind(u8 vif_idx);
#endif

int wlan_sta_disable(void);
int wlan_ap_disable(void);
int http_ota_download(const char *uri);
int bk_wlan_set_channel_with_band_width(int channel, int band_width);
int bmsg_tx_raw_sender(uint8_t *payload, uint16_t length);
void wlan_write_fast_connect_info(struct wlan_fast_connect_info *fci);
void wlan_read_fast_connect_info(struct wlan_fast_connect_info *fci);

int bk_wifi_sta_set_power(int power);
int bk_wifi_ap_set_power(int power);
uint32_t bk_wifi_get_mac_time(void);
int bk_wifi_get_beacon_rssi(void);
void bk_wifi_dump_ps_regs(void);
void bk_wifi_udelay(uint32_t us);
void wlan_hw_reinit(void);
void wlan_set_multicast_flag(void);
#ifdef CONFIG_COMPONENTS_P2P_GO
uint8_t bk_wlan_ap_get_default_channel(void);
void app_p2p_rw_event_func(void *new_evt);
void app_p2p_restart_thread(void);
#endif
void bk_wifi_rc_config(uint8_t sta_idx, uint16_t rate_cfg);
#ifdef __cplusplus
}
#endif
