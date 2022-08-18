set(incs)
set(srcs)

if (CONFIG_LWIP_V2_1)
	list(APPEND incs
	lwip-2.1.2/src
	lwip-2.1.2/port
	lwip-2.1.2/src/include
	lwip-2.1.2/src/include/netif
	lwip-2.1.2/src/include/lwip
	)

	list(APPEND srcs
	lwip-2.1.2/port/ethernetif.c
	lwip-2.1.2/port/net.c
	lwip-2.1.2/port/sys_arch.c
	lwip-2.1.2/src/api/api_lib.c
	lwip-2.1.2/src/api/api_msg.c
	lwip-2.1.2/src/api/err.c
	lwip-2.1.2/src/api/netbuf.c
	lwip-2.1.2/src/api/netdb.c
	lwip-2.1.2/src/api/netifapi.c
	lwip-2.1.2/src/api/sockets.c
	lwip-2.1.2/src/api/tcpip.c
	lwip-2.1.2/src/apps/mdns/mdns.c
	lwip-2.1.2/src/apps/ping/ping.c
	lwip-2.1.2/src/core/def.c
	lwip-2.1.2/src/core/dns.c
	lwip-2.1.2/src/core/inet_chksum.c
	lwip-2.1.2/src/core/init.c
	lwip-2.1.2/src/core/ip.c
	lwip-2.1.2/src/core/ipv4/autoip.c
	lwip-2.1.2/src/core/ipv4/dhcp.c
	lwip-2.1.2/src/core/ipv4/etharp.c
	lwip-2.1.2/src/core/ipv4/icmp.c
	lwip-2.1.2/src/core/ipv4/igmp.c
	lwip-2.1.2/src/core/ipv4/ip4_addr.c
	lwip-2.1.2/src/core/ipv4/ip4.c
	lwip-2.1.2/src/core/ipv4/ip4_frag.c
	lwip-2.1.2/src/core/ipv6/dhcp6.c
	lwip-2.1.2/src/core/ipv6/ethip6.c
	lwip-2.1.2/src/core/ipv6/icmp6.c
	lwip-2.1.2/src/core/ipv6/inet6.c
	lwip-2.1.2/src/core/ipv6/ip6_addr.c
	lwip-2.1.2/src/core/ipv6/ip6.c
	lwip-2.1.2/src/core/ipv6/ip6_frag.c
	lwip-2.1.2/src/core/ipv6/mld6.c
	lwip-2.1.2/src/core/ipv6/nd6.c
	lwip-2.1.2/src/core/mem.c
	lwip-2.1.2/src/core/memp.c
	lwip-2.1.2/src/core/netif.c
	lwip-2.1.2/src/core/pbuf.c
	lwip-2.1.2/src/core/raw.c
	lwip-2.1.2/src/core/stats.c
	lwip-2.1.2/src/core/sys.c
	lwip-2.1.2/src/core/tcp.c
	lwip-2.1.2/src/core/tcp_in.c
	lwip-2.1.2/src/core/tcp_out.c
	lwip-2.1.2/src/core/timeouts.c
	lwip-2.1.2/src/core/udp.c
	lwip-2.1.2/src/netif/ethernet.c
	dhcpd/dhcp-server.c
	dhcpd/dhcp-server-main.c
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES driver
	PRIV_REQUIRES bk_common bk_wifi
	)
