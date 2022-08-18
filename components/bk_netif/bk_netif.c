#include <components/netif.h>
#include <components/log.h>
#include <common/bk_err.h>
#include <os/mem.h>
#include <os/str.h>
#include <common/sys_config.h>
#if CONFIG_LWIP
#include "lwip/inet.h"
#include "net.h"
#endif
#include <components/event.h>
#include "bk_private/bk_wifi.h"
#if CONFIG_IPV6
#include "netif.h"
#endif

//TODO refactor BK netif module

bk_err_t netif_wifi_event_cb(void *arg, event_module_t event_module,
                int event_id, void *event_data)
{
	if (event_module != EVENT_MOD_WIFI) {
		return BK_OK;
	}

	switch (event_id) {
	case EVENT_WIFI_STA_CONNECTED:
#if CONFIG_LWIP
		sta_ip_start();
#endif
		break;
	case EVENT_WIFI_STA_DISCONNECTED:
#if CONFIG_LWIP
		sta_ip_down();
#endif
		break;
	default:
		return BK_OK;
	}

	return BK_OK;
}

bk_err_t bk_netif_init(void)
{
#if (!CONFIG_RTT) && CONFIG_NO_HOSTED
	extern void net_wlan_initial(void);
	net_wlan_initial();
#endif

	BK_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_ID_ALL,
			netif_wifi_event_cb, NULL));
	return BK_OK;
}

#if CONFIG_LWIP
bk_err_t netif_validate_ip4_config(const netif_ip4_config_t *ip4_config)
{
	//TODO add code to validate the IP address
	// 0. Validate IP string format
	// 1. IP of STA/AP can't on the same subnet
	// 2. multicast checking
	// 3. ...
	if (!ip4_config) {
		return BK_ERR_NULL_PARAM;
	}

	return BK_OK;
}

bk_err_t bk_netif_set_ip4_config(netif_if_t ifx, const netif_ip4_config_t *ip4_config)
{
	netif_ip4_config_t *config = (netif_ip4_config_t*)ip4_config;
	int ret;

	ret = netif_validate_ip4_config(ip4_config);
	if (ret != BK_OK) {
		return ret;
	}

	if (ifx == NETIF_IF_STA) {
		//TODO optimize ip_address_set
		ip_address_set(1 /*STA*/, 0/*static IP*/, config->ip, config->mask, config->gateway, config->dns);
	} else if (ifx == NETIF_IF_AP) {
		ip_address_set(0 /*AP*/, 0/*static IP*/, config->ip, config->mask, config->gateway, config->dns);
	} else {
		return BK_ERR_NETIF_IF;
	}

	return BK_OK;
}

bk_err_t bk_netif_get_ip4_config(netif_if_t ifx, netif_ip4_config_t *ip4_config)
{
	struct wlan_ip_config addr;

	if (!ip4_config) {
		return BK_ERR_NULL_PARAM;
	}

	os_memset(&addr, 0, sizeof(struct wlan_ip_config));
	if (ifx == NETIF_IF_STA) {
		net_get_if_addr(&addr, net_get_sta_handle());
	} else if (ifx == NETIF_IF_AP) {
		net_get_if_addr(&addr, net_get_uap_handle());
	} else {
		return BK_ERR_NETIF_IF;
	}

	os_strcpy(ip4_config->ip, inet_ntoa(addr.ipv4.address));
	os_strcpy(ip4_config->mask, inet_ntoa(addr.ipv4.netmask));
	os_strcpy(ip4_config->gateway, inet_ntoa(addr.ipv4.gw));
	os_strcpy(ip4_config->dns, inet_ntoa(addr.ipv4.dns1));

	return BK_OK;
}

#if CONFIG_IPV6

struct interface {
       struct netif netif;
       ip_addr_t ipaddr;
       ip_addr_t nmask;
       ip_addr_t gw;
};

bk_err_t bk_netif_get_ip6_addr_info(netif_if_t ifx)
{
       int i;
       int num = 0;
       struct netif *netif;
       struct interface *if_handle;

       if (ifx == NETIF_IF_STA) {
               if_handle = net_get_sta_handle();
       } else if (ifx == NETIF_IF_AP) {
               if_handle = net_get_uap_handle();
       } else {
               return BK_ERR_NETIF_IF;
       }

       netif = &if_handle->netif;

       for (i = 0; i < MAX_IPV6_ADDRESSES; i++) {
                       u8 *ipv6_addr;
                       ipv6_addr = (u8*)ip_2_ip6(&if_handle->netif.ip6_addr[i])->addr;///&addr->ipv6[i].address;

                       bk_printf("ipv6_addr[%d] %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\r\n", i,
                                       ipv6_addr[0], ipv6_addr[1], ipv6_addr[2], ipv6_addr[3],
                                       ipv6_addr[4], ipv6_addr[5], ipv6_addr[6], ipv6_addr[7],
                                       ipv6_addr[8], ipv6_addr[9], ipv6_addr[10], ipv6_addr[11],
                                       ipv6_addr[12], ipv6_addr[13], ipv6_addr[14], ipv6_addr[15]);
                       bk_printf("ipv6_state[%d] 0x%x\r\n", i, netif->ip6_addr_state[i]);
                       num++;
       }

       return num;
}
#endif

//TODO currently we only set the DHCP client flag
bk_err_t bk_netif_dhcpc_start(netif_if_t ifx)
{
	char *empty_ip = "";

	if (ifx != NETIF_IF_STA) {
		return BK_ERR_NETIF_IF;
	}

	ip_address_set(1/*STA*/, 1/*DHCP enabled*/, empty_ip, empty_ip, empty_ip, empty_ip);
	return BK_OK;
}
#endif
