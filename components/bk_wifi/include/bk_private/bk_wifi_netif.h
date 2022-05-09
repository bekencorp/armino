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

#include <components/netif.h>

// TODO
// Wrappers to hide internal details of WiFi stack, such as WiFi Driver etc.
// We should finally remove this wrapper and use the event instead.

int wifi_netif_mac_to_vifid(uint8_t *mac);
void* wifi_netif_vifid_to_vif(int vifid);
void* wifi_netif_mac_to_vif(uint8_t *mac);
netif_if_t wifi_netif_vif_to_netif_type(void *vif);
netif_if_t wifi_netif_mac_to_netif_type(uint8_t *mac);
void wifi_netif_set_vif_private_data(void *vif, void *data);
void* wifi_netif_get_vif_private_data(void *vif);
void wifi_netif_notify_sta_got_ip(void);
void wifi_netif_notify_sta_dhcp_timeout(void);
void wifi_netif_call_status_cb_when_sta_got_ip(void);
void wifi_netif_call_status_cb_when_sta_dhcp_timeout(void);
bool wifi_netif_sta_is_connected(void);
bool wifi_netif_sta_is_got_ip(void);
void wifi_netif_notify_sta_disconnect(void);

uint8_t* wifi_netif_vif_to_mac(void *vif);
int wifi_netif_vif_to_vifid(void *vif);
