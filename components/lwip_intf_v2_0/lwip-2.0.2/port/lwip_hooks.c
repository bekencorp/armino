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

#include <stdint.h>
#include <stdbool.h>
#include "lwip/netif.h"
#include "lwip_hooks.h"

extern struct netif *netif_list;

bool ip4_netif_exist(const ip4_addr_t *dest, const ip4_addr_t *src)
{
	struct netif *netif = NULL;

	for (netif = netif_list; netif != NULL; netif = netif->next) {
		/* is the netif up, does it have a link and a valid address? */
		if (netif_is_up(netif) && netif_is_link_up(netif)
			&& !ip4_addr_isany_val(*netif_ip4_addr(netif))) {
			/* source netif and dest netif match? */
			if (ip4_addr_netcmp(src, netif_ip4_addr(netif), netif_ip4_netmask(netif))
				|| ip4_addr_netcmp(dest, netif_ip4_addr(netif), netif_ip4_netmask(netif))) {
				/* return false when both netif don't match */
				return true;
			}
		}
	}

	return false;
}

/**
 * Source based IPv4 routing hook function.
 *
 * IP route algorithm:
 *
 * 1. Source IP based routing has highest priority
 * 2. Then the destination IP based routing
 * 3. Least priority is default netif
 *
 *
 * Default netif in different scenario:
 *
 * | STA | AP  | Default Netif |
 * -----------------------------
 * | ON  | OFF | STA           |
 * | ON  | ON  | STA           |
 * | OFF | ON  | AP            |
 *
 *
 * Routing examples:
 *
 * | sta-ip      | ap-ip      | src-ip/netif    | dst-ip           | out netif | notes        |
 * ---------------------------------------------------------------------------------------
 * | 192.168.0.5 | 10.10.10.1 | 10.10.10.1/ap   | 10.10.10.2       | ap        | src-ip-route |
 * | 192.168.0.5 | 10.10.10.1 | 10.10.10.1/ap   | 192.168.0.x      | ap        | src-ip-route |
 * | 192.168.0.5 | 10.10.10.1 | 10.10.10.2/ap   | 192.168.0.x      | sta       | dst-ip-route |
 * | 192.168.0.5 | 10.10.10.1 | 10.10.10.2/ap   | 192.168.1.x      | sta       | default netif|
 * | 192.168.0.5 | 10.10.10.1 | 192.168.0.5/sta | 192.168.0.x      | sta       | src-ip-route |
 * | 192.168.0.5 | 10.10.10.1 | 192.168.0.5/sta | 192.168.1.x      | sta       | src-ip-route |
 * | 192.168.0.5 | 10.10.10.1 | 192.168.0.5/sta | 10.10.10.x       | sta       | src-ip-route |
 * | unknown     | 10.10.10.1 | unknown/sta     | 10.10.10.x       | ap        | dst-ip-route |
 * | unknown     | 10.10.10.1 | unknown/sta     | 10.10.9.x        | sta       | default netif|
 * | 192.168.0.5 | 10.10.10.1 | 192.168.0.5/sta | 255.255.255.255  | sta       | src-ip-route |
 * | 192.168.0.5 | 10.10.10.1 | 10.10.10.1/ap   | 255.255.255.255  | ap        | src-ip-route |
 * | unknown     | 10.10.10.1 | unknown/sta     | 255.255.255.255  | sta       | default netif|
 *
 *
 * General rules for STA/AP coexisting routing:
 *
 * 1. The IP of AP is always known, so if the AP netif want to send multicast/broadcast
 *    traffic, bind the AP's IP to the socket
 * 2. Anytime if you want to send traffic that is different than the STA/IP's subnet via
 *    specific netif, bind the IP of the out netif to the socket
 * 3. Avoid configuring the IP of AP's address to the same sub-net as STA's, suggest using
 *    the special local sub-net address scope.
 */
struct netif* ip4_route_src_hook(const ip4_addr_t *dest, const ip4_addr_t *src)
{
	struct netif *netif = NULL;

	/* destination IP is broadcast IP? */
	if ((src != NULL) && !ip4_addr_isany(src)) {
		/* iterate through netifs */
		for (netif = netif_list; netif != NULL; netif = netif->next) {
			/* is the netif up, does it have a link and a valid address? */
			if (netif_is_up(netif) && netif_is_link_up(netif)
				&& !ip4_addr_isany_val(*netif_ip4_addr(netif))) {
				/* source IP matches? */
				if (ip4_addr_cmp(src, netif_ip4_addr(netif))) {
					/* return netif on which to forward IP packet */
					return netif;
				}
			}
		}
	}

	return netif;
}
