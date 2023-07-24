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
/* If SSID blacklist is enabled, the SSID is added into the
  * blacklist if the STA fails to connect it; the blacklist will be
  * cleared once the STA successfully connects one SSID or when
  * STA tries all found SSIDs.
  *
  * This feature only impacts the connect behavior when STA finds
  * several APs with the same SSIDs,
  *
  * e.g.  STA find AP1/AP2/AP3 ... with same SSID xxx. STA puts
  * AP1 into blacklist if it fails to connect AP1 and then tries AP2, STA
  * puts AP2 into blacklist if it fails to connect AP2 and then tries AP3 ...
  * the STA repeats this process until it successfully connects one AP or
  * fails to connect all APs, then the blacklist is cleared.
  *
  * Please be notified that the blacklist feature may consume more time
  * before it can successfully connect to the AP if it finds more than one AP
  * with same SSID, especially if only one AP has correct password but has
  * very low RSSI.
  *
  * Generally speaking, if you want the STA to iterate all the scanned APs (with
  * same SSID) one by one, you should enable the blacklist feature.
  *
  * The SSID blacklist is enabled by default.
  */
int wlan_sta_enable_ssid_blacklist(void);
int wlan_sta_disable_ssid_blacklist(void);
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
int bmsg_tx_raw_ex_sender(wifi_raw_tx_info_t *raw_tx, void *cb, void *param);
int bmsg_tx_raw_cb_sender(uint8_t *buffer, int length, void *cb, void *param);
void wlan_write_fast_connect_info(struct wlan_fast_connect_info *fci);
void wlan_read_fast_connect_info(struct wlan_fast_connect_info *fci);
void wlan_clear_fast_connect_info(struct wlan_fast_connect_info *fci);

int bk_wifi_sta_set_power(int power);
int bk_wifi_ap_set_power(int power);
uint32_t bk_wifi_get_mac_time(void);
int bk_wifi_get_beacon_rssi(void);
void bk_wifi_dump_ps_regs(void);
void bk_wifi_udelay(uint32_t us);
void wlan_hw_reinit(void);
void wlan_set_multicast_flag(void);
#ifdef CONFIG_P2P_GO
uint8_t bk_wlan_ap_get_default_channel(void);
void app_p2p_rw_event_func(void *new_evt);
void app_p2p_restart_thread(void);
#endif
void bk_wifi_rc_config(uint8_t sta_idx, uint16_t rate_cfg);
void bk_wifi_capa_config(uint32_t capa_id, uint32_t capa_val);
void bk_wifi_ps_config(uint8_t ps_id, uint8_t ps_val);
#ifdef __cplusplus
}
#endif
