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
#include "modules/wifi_types.h"
#ifdef __cplusplus
extern "C" {
#endif

void mhdr_set_station_status(wifi_linkstate_reason_t info);
wifi_linkstate_reason_t mhdr_get_station_status(void);
void rwm_mgmt_set_vif_netif(uint8_t *mac, void *netif);

#ifdef __cplusplus
}
#endif
