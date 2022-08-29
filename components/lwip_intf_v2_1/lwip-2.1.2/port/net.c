#include <common/bk_include.h>
#include <stdio.h>
#include <string.h>

#include <lwip/inet.h>
#include "netif/etharp.h"
#include "lwip/netif.h"
#include <lwip/netifapi.h>
#include <lwip/tcpip.h>
#include <lwip/dns.h>
#include <lwip/dhcp.h>
#include "lwip/prot/dhcp.h"
#include "lwip/apps/mdns.h"

#include <lwip/sockets.h>
#include "ethernetif.h"

#include <components/system.h>
#include "bk_drv_model.h"
#include <os/mem.h>
#include "bk_wifi_netif.h" //TODO use standard EVENT instead!!!
#include "lwip_netif_address.h"
#include <os/os.h>
#include "net.h"

#include "wlan_defs_pub.h"
#include "bk_wifi.h"
#include <os/str.h>
#include <modules/wifi.h>
#include "wlan_ui_pub.h"

/* forward declaration */
FUNC_1PARAM_PTR bk_wlan_get_status_cb(void);

struct ipv4_config sta_ip_settings = {
	.addr_type = ADDR_TYPE_DHCP,
	.address = 0,
	.gw = 0,
	.netmask = 0,
	.dns1 = 0,
	.dns2 = 0,
};

struct ipv4_config uap_ip_settings = {
	.addr_type = ADDR_TYPE_STATIC,
	.address = 0xc0a80001, //192.168.0.1
	.gw = 0xc0a80001,      //192.168.0.1
	.netmask = 0xffffff00, //255.255.255.0
	.dns1 = 0xc0a80001,    //192.168.0.1
	.dns2 = 0,
};
static char up_iface;
uint8 sta_ip_start_flag = 0;
uint8 uap_ip_start_flag = 0;

#ifdef CONFIG_IPV6
#define IPV6_ADDR_STATE_TENTATIVE       "Tentative"
#define IPV6_ADDR_STATE_PREFERRED       "Preferred"
#define IPV6_ADDR_STATE_INVALID         "Invalid"
#define IPV6_ADDR_STATE_VALID           "Valid"
#define IPV6_ADDR_STATE_DEPRECATED      "Deprecated"
#define IPV6_ADDR_TYPE_LINKLOCAL	    "Link-Local"
#define IPV6_ADDR_TYPE_GLOBAL		    "Global"
#define IPV6_ADDR_TYPE_UNIQUELOCAL	    "Unique-Local"
#define IPV6_ADDR_TYPE_SITELOCAL	    "Site-Local"
#define IPV6_ADDR_UNKNOWN		        "Unknown"
#endif

typedef void (*net_sta_ipup_cb_fn)(void* data);

struct interface {
	struct netif netif;
	ip_addr_t ipaddr;
	ip_addr_t nmask;
	ip_addr_t gw;
};
FUNCPTR sta_connected_func;

static struct interface g_mlan = {{0}};
static struct interface g_uap = {{0}};
net_sta_ipup_cb_fn sta_ipup_cb = NULL;

extern void *net_get_sta_handle(void);
extern void *net_get_uap_handle(void);
extern err_t lwip_netif_init(struct netif *netif);
extern err_t lwip_netif_uap_init(struct netif *netif);
extern int net_get_if_ip_addr(uint32_t *ip, void *intrfc_handle);
extern int net_get_if_ip_mask(uint32_t *nm, void *intrfc_handle);
extern int net_configure_address(struct ipv4_config *addr, void *intrfc_handle);
extern int dhcp_server_start(void *intrfc_handle);
extern void dhcp_server_stop(void);
extern void net_configure_dns(struct wlan_ip_config *ip);
extern bk_err_t bk_wifi_get_ip_status(IPStatusTypedef *outNetpara, WiFi_Interface inInterface);


#ifdef CONFIG_IPV6
char *ipv6_addr_state_to_desc(unsigned char addr_state)
{
	if (ip6_addr_istentative(addr_state))
		return IPV6_ADDR_STATE_TENTATIVE;
	else if (ip6_addr_ispreferred(addr_state))
		return IPV6_ADDR_STATE_PREFERRED;
	else if (ip6_addr_isinvalid(addr_state))
		return IPV6_ADDR_STATE_INVALID;
	else if (ip6_addr_isvalid(addr_state))
		return IPV6_ADDR_STATE_VALID;
	else if (ip6_addr_isdeprecated(addr_state))
		return IPV6_ADDR_STATE_DEPRECATED;
	else
		return IPV6_ADDR_UNKNOWN;
}

char *ipv6_addr_type_to_desc(struct ipv6_config *ipv6_conf)
{
	if (ip6_addr_islinklocal((ip6_addr_t *)&ipv6_conf->address))
		return IPV6_ADDR_TYPE_LINKLOCAL;
	else if (ip6_addr_isglobal((ip6_addr_t *)&ipv6_conf->address))
		return IPV6_ADDR_TYPE_GLOBAL;
	else if (ip6_addr_isuniquelocal((ip6_addr_t *)&ipv6_conf->address))
		return IPV6_ADDR_TYPE_UNIQUELOCAL;
	else if (ip6_addr_issitelocal((ip6_addr_t *)&ipv6_conf->address))
		return IPV6_ADDR_TYPE_SITELOCAL;
	else
		return IPV6_ADDR_UNKNOWN;
}
#endif /* CONFIG_IPV6 */

int net_dhcp_hostname_set(char *hostname)
{
	netif_set_hostname(&g_mlan.netif, hostname);
	return 0;
}

void net_ipv4stack_init(void)
{
	static bool tcpip_init_done = 0;
	if (tcpip_init_done)
		return;

	LWIP_LOGI("init TCP/IP\r\n");
	tcpip_init(NULL, NULL);
	tcpip_init_done = true;
}

#ifdef CONFIG_IPV6
void net_ipv6stack_init(struct netif *netif)
{
	bk_get_mac(netif->hwaddr, MAC_TYPE_STA);
	netif->hwaddr_len = 6;
}
#endif /* CONFIG_IPV6 */

void net_wlan_init(void)
{
	static int wlan_init_done = 0;
	int ret;

	if (!wlan_init_done) {
		net_ipv4stack_init();
        ip_addr_set_ip4_u32(&g_mlan.ipaddr,INADDR_ANY);
		ret = netifapi_netif_add(&g_mlan.netif,ip_2_ip4(&g_mlan.ipaddr),
					 ip_2_ip4(&g_mlan.ipaddr), ip_2_ip4(&g_mlan.ipaddr), NULL,
					 lwip_netif_init, tcpip_input);
		if (ret) {
			/*FIXME: Handle the error case cleanly */
			LWIP_LOGE("sta netif add failed");
		}
#ifdef CONFIG_IPV6
		net_ipv6stack_init(&g_mlan.netif);
#endif /* CONFIG_IPV6 */

		ret = netifapi_netif_add(&g_uap.netif, ip_2_ip4(&g_uap.ipaddr),
					 ip_2_ip4(&g_uap.ipaddr), ip_2_ip4(&g_uap.ipaddr), NULL,
					 lwip_netif_uap_init, tcpip_input);
		if (ret) {
			/*FIXME: Handle the error case cleanly */
			LWIP_LOGE("ap netif add failed");
		}
		wlan_init_done = 1;
	}

	return;
}

void net_set_sta_ipup_callback(void *fn)
{
    sta_ipup_cb = (net_sta_ipup_cb_fn)fn;
}

void user_connected_callback(FUNCPTR fn)
{
	sta_connected_func = fn;
}

static void wm_netif_status_static_callback(struct netif *n)
{
	if (n->flags & NETIF_FLAG_UP) {
		// static IP success;
		LWIP_LOGI("using static ip...\n");
		wifi_netif_notify_sta_got_ip();
#if CONFIG_LWIP_FAST_DHCP
		/* read stored IP from flash as the static IP */
		struct wlan_fast_connect_info fci = {0};
		wlan_read_fast_connect_info(&fci);
		os_memcpy((char *)&n->ip_addr, (char *)&fci.ip_addr, sizeof(fci.ip_addr));
		os_memcpy((char *)&n->netmask, (char *)&fci.netmask, sizeof(fci.netmask));
		os_memcpy((char *)&n->gw, (char *)&fci.gw, sizeof(fci.gw));
		os_memcpy((char *)&n->dns1, (char *)&fci.dns1, sizeof(fci.dns1));
		LWIP_LOGI("ip_addr: "BK_IP4_FORMAT" \r\n", BK_IP4_STR(ip_addr_get_ip4_u32(&n->ip_addr)));
#endif

#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
		if (sta_ipup_cb != NULL)
			sta_ipup_cb(NULL);

		if (sta_connected_func != NULL)
			(*sta_connected_func)();
#endif
	} else {
		// static IP fail;
	}
}

extern wifi_connect_tick_t sta_tick;
static void wm_netif_status_callback(struct netif *n)
{
	struct dhcp *dhcp;

	if (n->flags & NETIF_FLAG_UP) {
		dhcp = netif_dhcp_data(n);
#ifdef CONFIG_IPV6        
        int i;
        u8 *ipv6_addr;
       
        {
            for (i = 0; i < MAX_IPV6_ADDRESSES; i++) {
                if (ip6_addr_isvalid(netif_ip6_addr_state(n, i))) {
                    ipv6_addr = (u8*)(ip_2_ip6(&n->ip6_addr[i]))->addr;
                    bk_printf("ipv6_addr[%d] : %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\r\n", i,
                        ipv6_addr[0], ipv6_addr[1], ipv6_addr[2], ipv6_addr[3],
                        ipv6_addr[4], ipv6_addr[5], ipv6_addr[6], ipv6_addr[7],
                        ipv6_addr[8], ipv6_addr[9], ipv6_addr[10], ipv6_addr[11],
                        ipv6_addr[12], ipv6_addr[13], ipv6_addr[14], ipv6_addr[15]);
                    bk_printf("ipv6_type[%d] :0x%x\r\n", i, n->ip6_addr[i].type);
                    bk_printf("ipv6_state[%d] :0x%x\r\n", i, n->ip6_addr_state[i]);
                }
            }
        }
#endif

#ifdef CONFIG_MDNS
		if(!ip_addr_isany(&n->ip_addr)){
			mdns_resp_restart(n);
		}
#endif

		if(dhcp != NULL) {
			/* dhcp success*/
			if (dhcp->state == DHCP_STATE_BOUND) {
				LWIP_LOGI("ip_addr: "BK_IP4_FORMAT" \r\n", BK_IP4_STR(ip_addr_get_ip4_u32(&n->ip_addr)));
				sta_tick.sta_ip_tick = rtos_get_time();
				LWIP_LOGI("STA assoc delta:%d, eapol delta:%d, dhcp delta:%d, total:%d\n", 
					sta_tick.sta_assoc_tick - sta_tick.sta_start_tick,
					sta_tick.sta_eapol_tick - sta_tick.sta_assoc_tick,
					sta_tick.sta_ip_tick - sta_tick.sta_eapol_tick,
					sta_tick.sta_ip_tick - sta_tick.sta_start_tick);
#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
				wifi_netif_call_status_cb_when_sta_got_ip();
#endif
				wifi_netif_notify_sta_got_ip();

#if CONFIG_LWIP_FAST_DHCP
				/* store current IP to flash */
				const ip_addr_t *dns_server;
				dns_server = dns_getserver(0);
				n->dns1 = ip_addr_get_ip4_u32(dns_server);
				struct wlan_fast_connect_info fci = {0};
				wlan_read_fast_connect_info(&fci);
				os_memset(&fci.ip_addr, 0, sizeof(ip_addr_t)*4);
				os_memcpy((char *)&fci.ip_addr, (char *)&n->ip_addr, sizeof(n->ip_addr));
				os_memcpy((char *)&fci.netmask, (char *)&n->netmask, sizeof(n->netmask));
				os_memcpy((char *)&fci.gw, (char *)&n->gw, sizeof(n->gw));
				os_memcpy((char *)&fci.dns1, (char *)&n->dns1, sizeof(n->dns1));
				wlan_write_fast_connect_info(&fci);
#endif

#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
				if(sta_ipup_cb != NULL)
					sta_ipup_cb(NULL);

				if(sta_connected_func != NULL)
					(*sta_connected_func)();
#endif
			} else {
				// dhcp fail
#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
				wifi_netif_call_status_cb_when_sta_dhcp_timeout();
#endif
			}
		} else {
			// static IP success;
		}
	} else {
		// dhcp fail;
	}
}

static int check_iface_mask(void *handle, uint32_t ipaddr)
{
	uint32_t interface_ip, interface_mask;
	net_get_if_ip_addr(&interface_ip, handle);
	net_get_if_ip_mask(&interface_mask, handle);
	if (interface_ip > 0)
		if ((interface_ip & interface_mask) ==
		    (ipaddr & interface_mask))
			return 0;
	return -1;
}

void *net_ip_to_interface(uint32_t ipaddr)
{
	int ret;
	void *handle;
	/* Check mlan handle */
	handle = net_get_sta_handle();
	ret = check_iface_mask(handle, ipaddr);
	if (ret == 0)
		return handle;

	/* Check uap handle */
	handle = net_get_uap_handle();
	ret = check_iface_mask(handle, ipaddr);
	if (ret == 0)
		return handle;

	/* If more interfaces are added then above check needs to done for
	 * those newly added interfaces
	 */
	return NULL;
}

void *net_sock_to_interface(int sock)
{
	struct sockaddr_in peer;
	unsigned long peerlen = sizeof(struct sockaddr_in);
	void *req_iface = NULL;

	getpeername(sock, (struct sockaddr *)&peer, &peerlen);
	req_iface = net_ip_to_interface(peer.sin_addr.s_addr);
	return req_iface;
}

void *net_get_sta_handle(void)
{
	return &g_mlan.netif;
}

void *net_get_uap_handle(void)
{
	return &g_uap.netif;
}

void *net_get_netif_handle(uint8_t iface)
{

    return NULL;
}

void net_interface_up(void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;
	netifapi_netif_set_up(&if_handle->netif);
}

void net_interface_down(void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;
	netifapi_netif_set_down(&if_handle->netif);
}


void net_interface_dhcp_stop(void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;
	netifapi_dhcp_stop(&if_handle->netif);
	netif_set_status_callback(&if_handle->netif, NULL);
}

void sta_ip_down(void)
{
	if(sta_ip_start_flag)
	{
		LWIP_LOGI("sta ip down\r\n");

		sta_ip_start_flag = 0;

		netifapi_netif_set_down(&g_mlan.netif);
		netif_set_status_callback(&g_mlan.netif, NULL);
		netifapi_dhcp_stop(&g_mlan.netif);
#if LWIP_IPV6
		for(u8_t addr_idx = 1; addr_idx < LWIP_IPV6_NUM_ADDRESSES; addr_idx++) {
			netif_ip6_addr_set(&g_mlan.netif, addr_idx, (const ip6_addr_t *)IP6_ADDR_ANY);
			g_mlan.netif.ip6_addr_state[addr_idx] = IP6_ADDR_INVALID;
		}
#endif
	}
}

void sta_ip_start(void)
{
	struct wlan_ip_config address = {0};

	if(!sta_ip_start_flag) {
		LWIP_LOGI("sta ip start\r\n");
		sta_ip_start_flag = 1;
		net_configure_address(&sta_ip_settings, net_get_sta_handle());
		return;
	}

	LWIP_LOGI("sta ip start: "BK_IP4_FORMAT" \r\n", BK_IP4_STR(address.ipv4.address));
	net_get_if_addr(&address, net_get_sta_handle());
	if(wifi_netif_sta_is_connected()
		&& (0 != address.ipv4.address)) {
		wifi_netif_notify_sta_got_ip();
	}
}

void sta_set_default_netif(void)
{
    netifapi_netif_set_default(net_get_sta_handle());
}

void ap_set_default_netif(void)
{
    // as the default netif is sta's netif, so ap need to send
    // boardcast or not sub net packets, need set ap netif before
    // send those packets, after finish sending, reset default netif
    // to sat's netif.
    netifapi_netif_set_default(net_get_uap_handle());
}

void reset_default_netif(void)
{
    netifapi_netif_set_default(NULL);
}

uint32_t sta_ip_is_start(void)
{
	return sta_ip_start_flag;
}

void uap_ip_down(void)
{
	if (uap_ip_start_flag )
	{
		LWIP_LOGI("uap ip down\r\n");
		uap_ip_start_flag = 0;

		netifapi_netif_set_down(&g_uap.netif);
		netif_set_status_callback(&g_uap.netif, NULL);
		dhcp_server_stop();
	}
}

void uap_ip_start(void)
{
	if ( !uap_ip_start_flag )
	{
		LWIP_LOGI("uap ip start\r\n");
		uap_ip_start_flag = 1;
    	net_configure_address(&uap_ip_settings, net_get_uap_handle());
    }
}

uint32_t uap_ip_is_start(void)
{
	return uap_ip_start_flag;
}

#define DEF_UAP_IP	      0xc0a80a01UL /* 192.168.10.1 */

void ip_address_set(int iface, int dhcp, char *ip, char *mask, char*gw, char*dns)
{
	uint32_t tmp;
	struct ipv4_config addr;

	memset(&addr, 0, sizeof(struct ipv4_config));
	if (dhcp == 1) {
		addr.addr_type = ADDR_TYPE_DHCP;
	} else {
		addr.addr_type = ADDR_TYPE_STATIC;
	    tmp = inet_addr((char*)ip);
	    addr.address = (tmp);
	    tmp = inet_addr((char*)mask);
	    if (tmp == 0xFFFFFFFF)
	        tmp = 0x00FFFFFF;// if not set valid netmask, set as 255.255.255.0
	    addr.netmask= (tmp);
	    tmp = inet_addr((char*)gw);
	    addr.gw = (tmp);

	    tmp = inet_addr((char*)dns);
	    addr.dns1 = (tmp);
	}

	if (iface == 1) // Station
		memcpy(&sta_ip_settings, &addr, sizeof(addr));
	else
		memcpy(&uap_ip_settings, &addr, sizeof(addr));
}

void sta_ip_mode_set(int dhcp)
{
	if (dhcp == 1) {
		ip_address_set(1, DHCP_CLIENT, NULL, NULL, NULL, NULL);
	} else {
	        IPStatusTypedef ipStatus;
	        bk_err_t ret = kNoErr;
		os_memset(&ipStatus, 0x0, sizeof(IPStatusTypedef));
		ret = bk_wifi_get_ip_status(&ipStatus, BK_STATION);
		if (ret == kNoErr)
		    ip_address_set(1, DHCP_DISABLE, ipStatus.ip, ipStatus.mask, ipStatus.gate, ipStatus.dns);
	}
}

#if CONFIG_LWIP_FAST_DHCP
void net_restart_dhcp(void)
{
	sta_ip_down();
	ip_address_set(BK_STATION, DHCP_CLIENT, NULL, NULL, NULL, NULL);
	sta_ip_start();
}
#endif

int net_configure_address(struct ipv4_config *addr, void *intrfc_handle)
{
	if (!intrfc_handle)
		return -1;

	struct interface *if_handle = (struct interface *)intrfc_handle;

	LWIP_LOGI("configuring interface %s (with %s)\n",
		(if_handle == &g_mlan) ? "sta" :"ap",
		(addr->addr_type == ADDR_TYPE_DHCP)
		? "DHCP client" : "Static IP");
	netifapi_netif_set_down(&if_handle->netif);

	/* De-register previously registered DHCP Callback for correct
	 * address configuration.
	 */
	netif_set_status_callback(&if_handle->netif, NULL);

	switch (addr->addr_type) {
	case ADDR_TYPE_STATIC:
		ip_addr_set_ip4_u32(&if_handle->ipaddr , addr->address);
		ip_addr_set_ip4_u32(&if_handle->nmask,addr->netmask);
		ip_addr_set_ip4_u32(&if_handle->gw ,addr->gw);

		netifapi_netif_set_addr(&if_handle->netif, ip_2_ip4(&if_handle->ipaddr),
					ip_2_ip4(&if_handle->nmask), ip_2_ip4(&if_handle->gw));

		//AP never configure DNS server address!!!

		if(if_handle == &g_mlan)
		{
			netif_set_status_callback(&if_handle->netif,
										wm_netif_status_static_callback);
		}
		netifapi_netif_set_up(&if_handle->netif);
		net_configure_dns((struct wlan_ip_config *)addr);
		break;

	case ADDR_TYPE_DHCP:
		/* Reset the address since we might be transitioning from static to DHCP */
		memset(&if_handle->ipaddr, 0, sizeof(ip_addr_t));
		memset(&if_handle->nmask, 0, sizeof(ip_addr_t));
		memset(&if_handle->gw, 0, sizeof(ip_addr_t));
		netifapi_netif_set_addr(&if_handle->netif, ip_2_ip4(&if_handle->ipaddr),
				ip_2_ip4(&if_handle->nmask), ip_2_ip4(&if_handle->gw));

		netif_set_status_callback(&if_handle->netif,
					wm_netif_status_callback);
		netifapi_netif_set_up(&if_handle->netif);
		netifapi_dhcp_start(&if_handle->netif);
		break;

	default:
		break;
	}
	/* Finally this should send the following event. */
	if (if_handle == &g_mlan) {
		// static IP up;

		/* XXX For DHCP, the above event will only indicate that the
		 * DHCP address obtaining process has started. Once the DHCP
		 * address has been obtained, another event,
		 * WD_EVENT_NET_DHCP_CONFIG, should be sent to the wlcmgr.
		 */
		 up_iface = 1;

         // we always set sta netif as the default.
         sta_set_default_netif();
	} else {
		// softap IP up, start dhcp server;
		dhcp_server_start(net_get_uap_handle());
		up_iface = 0;
	}

	return 0;
}

int net_get_if_addr(struct wlan_ip_config *addr, void *intrfc_handle)
{
	const ip_addr_t *tmp;
	struct interface *if_handle = (struct interface *)intrfc_handle;

    if(netif_is_up(&if_handle->netif)) {
    	addr->ipv4.address = ip_addr_get_ip4_u32(&if_handle->netif.ip_addr);
    	addr->ipv4.netmask = ip_addr_get_ip4_u32(&if_handle->netif.netmask);
    	addr->ipv4.gw = ip_addr_get_ip4_u32(&if_handle->netif.gw);

    	tmp = dns_getserver(0);
    	addr->ipv4.dns1 = ip_addr_get_ip4_u32(tmp);
    	tmp = dns_getserver(1);
    	addr->ipv4.dns2 = ip_addr_get_ip4_u32(tmp);
    }

	return 0;
}

int net_get_if_macaddr(void *macaddr, void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;

    os_memcpy(macaddr, &if_handle->netif.hwaddr[0], if_handle->netif.hwaddr_len);

	return 0;
}

#ifdef CONFIG_IPV6
int net_get_if_ipv6_addr(struct wlan_ip_config *addr, void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;
	int i;

	for (i = 0; i < MAX_IPV6_ADDRESSES; i++) {
		memcpy(&addr->ipv6[i].address,
			ip_2_ip6(&(if_handle->netif.ip6_addr[i])), 16);
		addr->ipv6[i].addr_state = if_handle->netif.ip6_addr_state[i];
	}
	/* TODO carry out more processing based on IPv6 fields in netif */
	return 0;
}

int net_get_if_ipv6_pref_addr(struct wlan_ip_config *addr, void *intrfc_handle)
{
	int i, ret = 0;
	struct interface *if_handle = (struct interface *)intrfc_handle;

	for (i = 0; i < MAX_IPV6_ADDRESSES; i++) {
		if (if_handle->netif.ip6_addr_state[i] == IP6_ADDR_PREFERRED) {
			memcpy(&addr->ipv6[ret++].address,
				&(if_handle->netif.ip6_addr[i]), 16);
		}
	}
	return ret;
}
#endif /* CONFIG_IPV6 */

int net_get_if_ip_addr(uint32_t *ip, void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;

	*ip = ip_addr_get_ip4_u32(&if_handle->netif.ip_addr);
	return 0;
}

int net_get_if_gw_addr(uint32_t *ip, void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;

	*ip = ip_addr_get_ip4_u32(&if_handle->netif.gw);

	return 0;
}

int net_get_if_ip_mask(uint32_t *nm, void *intrfc_handle)
{
	struct interface *if_handle = (struct interface *)intrfc_handle;

	*nm = ip_addr_get_ip4_u32(&if_handle->netif.netmask);
	return 0;
}

void net_configure_dns(struct wlan_ip_config *ip)
{
	ip_addr_t tmp;

	if (ip->ipv4.addr_type == ADDR_TYPE_STATIC) {

		if (ip->ipv4.dns1 == 0){
#ifdef CONFIG_LWIP_FAST_DHCP
			struct wlan_fast_connect_info fci = {0};
			wlan_read_fast_connect_info(&fci);
			os_memcpy((char *)&ip->ipv4.dns1, (char *)&fci.dns1, sizeof(fci.dns1));
#else
			ip->ipv4.dns1 = ip->ipv4.gw;
#endif
		}
		if (ip->ipv4.dns2 == 0)
			ip->ipv4.dns2 = ip->ipv4.dns1;

		ip_addr_set_ip4_u32(&tmp,ip->ipv4.dns1);
		dns_setserver(0, &tmp);
		ip_addr_set_ip4_u32(&tmp, ip->ipv4.dns2);
		dns_setserver(1, &tmp);
	}

	/* DNS MAX Retries should be configured in lwip/dns.c to 3/4 */
	/* DNS Cache size of about 4 is sufficient */
}

void net_wlan_initial(void)
{
    net_ipv4stack_init();

#ifdef CONFIG_IPV6
    net_ipv6stack_init(&g_mlan.netif);
#endif /* CONFIG_IPV6 */
}

int net_wlan_add_netif(uint8_t *mac)
{
	struct interface *wlan_if = NULL;
	netif_if_t netif_if;
	void *vif = NULL;
	int vifid = 0;
	err_t err;

	vifid = wifi_netif_mac_to_vifid(mac);
	vif = wifi_netif_mac_to_vif(mac);
	netif_if = wifi_netif_vif_to_netif_type(vif);
	if (netif_if == NETIF_IF_AP) {
		wlan_if = &g_uap;
	} else if(netif_if == NETIF_IF_STA) {
		wlan_if = &g_mlan;
	} else {
		LWIP_LOGE("unknown netif(%d)\n", netif_if);
		return ERR_ARG;
	}

	ip_addr_set_ip4_u32(&wlan_if->ipaddr, INADDR_ANY);
	err = netifapi_netif_add(&wlan_if->netif,
		ip_2_ip4(&wlan_if->ipaddr),
		ip_2_ip4(&wlan_if->ipaddr),
		ip_2_ip4(&wlan_if->ipaddr),
		vif,
		ethernetif_init,
		tcpip_input);
	if (err) {
		LWIP_LOGE("net_wlan_add_netif failed(%d)\n", err);
		return err;
	} else {
		wifi_netif_set_vif_private_data(vif, &wlan_if->netif);
		#if CONFIG_IPV6
		if(netif_if == NETIF_IF_STA) {
			netif_create_ip6_linklocal_address(&wlan_if->netif, 1);
			netif_set_ip6_autoconfig_enabled(&wlan_if->netif, 1);
		}
		#endif
	}

	LWIP_LOGI("add vif%d\n", vifid);
	return ERR_OK;
}

int net_wlan_remove_netif(uint8_t *mac)
{
	struct netif *netif = NULL;
	void *vif;
	err_t err;
	int vifid;

	vifid = wifi_netif_mac_to_vifid(mac);
	vif = wifi_netif_mac_to_vif(mac);
	if(!vif) {
		LWIP_LOGE("remove netif, vif%d not found\n", vifid);
		return ERR_ARG;
	}

	netif = (struct netif *)wifi_netif_get_vif_private_data(vif);
	if(!netif) {
		LWIP_LOGE("remove netif, netif is null\n");
		return ERR_ARG;
	}

	err = netifapi_netif_remove(netif);
	if(err != ERR_OK) {
		LWIP_LOGE("remove netif, failed(%d)\n", err);
		return err;
	} else {
		netif->state = NULL;
	}
	
	LWIP_LOGI("remove vif%d\n", vifid);
	return ERR_OK;
}
#if CONFIG_WIFI6_CODE_STACK
bool etharp_tmr_flag = false;
void net_begin_send_arp_reply(bool is_send_arp, bool is_allow_send_req)
{
	//send reply
	if(is_send_arp && !is_allow_send_req) {
		etharp_tmr_flag = true;
		LWIP_LOGI("send reply %s\n", __func__);
	}
	//stop send reply
	if(!is_send_arp && is_allow_send_req) {
		etharp_tmr_flag = false;
		LWIP_LOGI("stop send reply %s\n", __func__);
		return;
	}
	etharp_reply();

}
#endif
