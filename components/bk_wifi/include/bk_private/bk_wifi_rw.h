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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int bk_wifi_sta_set_power(int power);
int bk_wifi_ap_set_power(int power);
uint32_t bk_wifi_get_mac_time(void);
int bk_wifi_get_beacon_rssi(void);
void bk_wifi_dump_ps_regs(void);
uint32_t bk_wifi_get_la_base_address(void);
uint32_t bk_wifi_get_mac_base_address(void);
void bk_wifi_set_mpb_frameperburst(uint32_t value);
uint32_t bk_wifi_get_mpb_frameperburst(void);
uint32_t bk_wifi_get_mpb_ctrl(void);
void bk_wifi_set_mpb_ctrl(uint32_t value);
uint32_t bk_wifi_get_mpb_txv(int reg_idx);
void bk_wifi_set_mpb_txv(int reg_idx, uint32_t value);
void bk_wifi_set_mpb_payload(uint32_t value);
uint32_t bk_wifi_get_mpb_payload(void);
void bk_wifi_set_mpb_clken(uint32_t value);
void bk_wifi_set_mpb_interframe_delay(uint32_t value);
uint32_t bk_wifi_get_monotonic_counter_2_lo(void);
void bk_wifi_debug_port_sel_pack(uint8_t debugportsel2, uint8_t debugportsel1);

void bk_wifi_phy_wakeup_rf_reinit(void);
void bk_wifi_phy_wakeup_wifi_reinit(void);
void bk_wifi_get_mac_trx_status(bool reset_status);
#ifdef __cplusplus
}
#endif
