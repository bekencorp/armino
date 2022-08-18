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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int bk_wlan_ps_disable_send_msg(void);
int bk_wlan_mcu_suppress_and_sleep(UINT32);

uint8_t bk_wlan_has_role(uint8_t role);
int bk_wlan_dtim_rf_ps_mode_do_wakeup(void);
uint32_t bk_sta_cipher_is_open(void);
monitor_cb_t bk_wlan_get_bcn_cb(void);
int bk_wlan_is_monitor_mode(void);
uint32_t bk_wlan_get_INT_status(void);
void bk_wlan_enable_lsig(void);
void bk_wlan_disable_lsig(void);
int bk_sta_cipher_type(void);
uint32_t bk_wlan_ap_get_frequency(void);
uint8_t bk_wlan_ap_get_channel_config(void);
VOID bk_wlan_ap_set_channel_config(uint8_t channel);
monitor_cb_t bk_wlan_get_monitor_cb(void);

void bk_wlan_register_mgnt_monitor_cb(monitor_cb_t fn);
monitor_cb_t bk_wlan_get_mgnt_monitor_cb(void);

void bk_wlan_sta_init(network_InitTypeDef_st *inNetworkInitPara);
void bk_wlan_ap_init_adv(network_InitTypeDef_ap_st *inNetworkInitParaAP);

int auto_check_dtim_rf_ps_mode(void);
void bk_wlan_dhcp_timeout(void);
int wlan_sta_ap_info(struct ApListStruct *ap);
int wlan_sta_scan_result(ScanResult_adv *results);

void bk_wlan_reg_csa_cb_coexist_mode(void);

#ifdef CONFIG_AOS_MESH
monitor_cb_t wlan_get_mesh_monitor_cb(void);
int wlan_is_mesh_monitor_mode(void);
#endif

#ifdef __cplusplus
}
#endif
