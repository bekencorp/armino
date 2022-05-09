/*
 * File      : ping.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006-2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-04-01     Bernard      first version
 * 2018-01-25     armink       Add ping domain name
 */

#include <lwip/opt.h>
#include <lwip/mem.h>
#include <lwip/icmp.h>
#include <lwip/netif.h>
#include <lwip/sys.h>
#include <lwip/inet.h>
#include <lwip/inet_chksum.h>
#include <lwip/ip.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include <lwip/ping.h>
#include <string.h>
#include "ping.h"
#include "stdio.h"
#include "lwip_netif_address.h"
#include <stdlib.h>
#include "net.h"
#include "timeouts.h"
#include "raw.h"

/**
 * PING_DEBUG: Enable debugging for PING.
 */
#ifndef PING_DEBUG
#define PING_DEBUG     LWIP_DBG_ON
#endif

/** ping receive timeout - in milliseconds */
#define PING_RCV_TIMEO 2000
/** ping delay - in milliseconds */
#define PING_DELAY     1000

/** ping identifier - must fit on a u16_t */
#ifndef PING_ID
#define PING_ID        0xAFAF
#endif

/** ping additional data size to include in the packet */
#ifndef PING_DATA_SIZE
#define PING_DATA_SIZE 32
#endif

#if (LWIP_IPV4)
/* ping variables */
static u16_t ping_seq_num;
/** Prepare a echo ICMP request */
static void ping_prepare_echo( struct icmp_echo_hdr *iecho, u16_t len)
{
    size_t i;
    size_t data_len = len - sizeof(struct icmp_echo_hdr);

    ICMPH_TYPE_SET(iecho, ICMP_ECHO);
    ICMPH_CODE_SET(iecho, 0);
    iecho->chksum = 0;
    iecho->id     = PING_ID;
    iecho->seqno  = htons(++ping_seq_num);

    /* fill the additional data buffer with some data */
    for (i = 0; i < data_len; i++)
    {
        ((char*) iecho)[sizeof(struct icmp_echo_hdr) + i] = (char) i;
    }

    iecho->chksum = inet_chksum(iecho, len);
}

/* Ping using the socket ip */
static err_t ping_send(int s, ip_addr_t *addr, int size)
{
    int err;
    struct icmp_echo_hdr *iecho;
    struct sockaddr_in to;
    int ping_size = sizeof(struct icmp_echo_hdr) + size;
    LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);

	iecho = (struct icmp_echo_hdr *)mem_malloc((mem_size_t)ping_size);
	if (!iecho) {
		return ERR_MEM;
	}
    ping_prepare_echo(iecho, (u16_t) ping_size);

    to.sin_len = sizeof(to);
    to.sin_family = AF_INET;
	#if (LWIP_IPV6)
	to.sin_addr.s_addr = addr->u_addr.ip4.addr;
	#else
    to.sin_addr.s_addr = addr->addr;
	#endif
    err = lwip_sendto(s, iecho, ping_size, 0, (struct sockaddr*) &to, sizeof(to));
    mem_free(iecho);

    return (err == ping_size ? ERR_OK : ERR_VAL);
}

static int ping_recv(int s, int *ttl)
{
    char buf[64];
    int fromlen = sizeof(struct sockaddr_in), len;
    struct sockaddr_in from;
    struct ip_hdr *iphdr;
    struct icmp_echo_hdr *iecho;

    while ((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &from, (socklen_t*) &fromlen)) > 0)
    {
        if (len >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr)))
        {
            iphdr = (struct ip_hdr *) buf;
            iecho = (struct icmp_echo_hdr *) (buf + (IPH_HL(iphdr) * 4));
            if ((iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num)))
            {
                *ttl = iphdr->_ttl;
                return len;
            }
        }
    }

    return len;
}

int ping(char* target_name, uint32_t times, size_t size)
{
    int s, ttl=0, recv_len;
    struct timeval timeout = { PING_RCV_TIMEO / 1000, PING_RCV_TIMEO % 1000 };
    ip_addr_t target_addr;
    uint32_t send_times;
    uint32_t recv_start_tick;
    struct addrinfo hint, *res = NULL;
    struct sockaddr_in *h = NULL;
    struct in_addr ina;

    send_times = 0;
    ping_seq_num = 0;

    if (size == 0)
    {
        size = PING_DATA_SIZE;
    }

    memset(&hint, 0, sizeof(hint));
    if (!sta_ip_is_start() && !uap_ip_is_start())
    {
        LWIP_DEBUGF( PING_DEBUG, ("ping: unknown host\n"));
        return -1;
    }
    /* convert URL to IP */
    if (lwip_getaddrinfo(target_name, NULL, &hint, &res) != 0)
    {
        LWIP_DEBUGF( PING_DEBUG, ("ping: unknown host\n"));
        return -1;
    }
    memcpy(&h, &res->ai_addr, sizeof(struct sockaddr_in *));
    memcpy(&ina, &h->sin_addr, sizeof(ina));
    lwip_freeaddrinfo(res);
	#if (LWIP_IPV6)
	target_addr.type = IPADDR_TYPE_V4;
	if (inet_aton(inet_ntoa(ina), &target_addr.u_addr.ip4) == 0)
	#else
    if (inet_aton(inet_ntoa(ina), &target_addr) == 0)
	#endif
    {
		LWIP_DEBUGF( PING_DEBUG, ("ping: unknown host\n"));
        return -1;
    }
    /* new a socket */
    if ((s = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0)
    {
		LWIP_DEBUGF( PING_DEBUG, ("ping: create socket failed\n"));
        return -1;
    }

    lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    while (1)
    {
        if (ping_send(s, &target_addr, size) == ERR_OK)
        {
			recv_start_tick = sys_now();
		
            if ((recv_len = ping_recv(s, &ttl)) >= 0)
            {
                LWIP_DEBUGF( PING_DEBUG, ("%d bytes from %s icmp_seq=%d ttl=%d time=%d ticks\n", recv_len, inet_ntoa(ina), send_times,
                        ttl, sys_now() - recv_start_tick));
            }
            else
            {
				LWIP_DEBUGF( PING_DEBUG, ("From %s icmp_seq=%d timeout\n", inet_ntoa(ina), send_times));
            }
        }
        else
        {
			LWIP_DEBUGF( PING_DEBUG, ("Send %s - error\n", inet_ntoa(ina)));
        }

        send_times++;
        if (send_times >= times)
        {
            /* send ping times reached, stop */
            break;
        }
		sys_msleep(PING_DELAY);
    }

    lwip_close(s);

    return 0;
}
#endif
#if LWIP_IPV6
///#elif LWIP_IPV6
#include "lwip/icmp6.h"
#include "lwip/prot/icmp6.h"
#include "lwip/raw.h"

static u16_t ping_seq_num;
static ip6_addr_t ping_dst;
static ip6_addr_t ping_src;

extern u16_t lwip_standard_chksum(const void *dataptr, int len);
/** Parts of the pseudo checksum which are common to IPv4 and IPv6 */
static u16_t
_inet_cksum_pseudo_base(unsigned char *p,unsigned int p_len, u8_t proto, u16_t proto_len, u32_t acc)
{
	u8_t swapped = 0;

	///acc += LWIP_CHKSUM(p, p_len);
	acc += lwip_standard_chksum(p, p_len);
	acc = FOLD_U32T(acc);
	if (p_len % 2 != 0) {
		swapped = 1 - swapped;
		acc = SWAP_BYTES_IN_WORD(acc);
	}

	if (swapped) {
		acc = SWAP_BYTES_IN_WORD(acc);
	}

	acc += (u32_t)lwip_htons((u16_t)proto);
	acc += (u32_t)lwip_htons(proto_len);

	acc = FOLD_U32T(acc);
	acc = FOLD_U32T(acc);

	return (u16_t)~(acc & 0xffffUL);
}

static u16_t _ip6_chksum_pseudo(unsigned char *p,unsigned int p_len, u8_t proto, u16_t proto_len,
       const ip6_addr_t *src, const ip6_addr_t *dest)
{
	u32_t acc = 0;
	u32_t addr;
	u8_t addr_part;

	for (addr_part = 0; addr_part < 4; addr_part++) {
		addr = src->addr[addr_part];
		acc += (addr & 0xffffUL);
		acc += ((addr >> 16) & 0xffffUL);
		addr = dest->addr[addr_part];
		acc += (addr & 0xffffUL);
		acc += ((addr >> 16) & 0xffffUL);
	}
	/* fold down to 16 bits */
	acc = FOLD_U32T(acc);
	acc = FOLD_U32T(acc);

	return _inet_cksum_pseudo_base(p,p_len, proto, proto_len, acc);
}

/** Prepare a echo ICMP request */
static void ping6_prepare_echo( struct icmp6_echo_hdr *iecho, u16_t tot_len,
	ip6_addr_t *reply_src,ip6_addr_t *dest_add)
{
    size_t i;
    size_t data_len = tot_len - sizeof(struct icmp6_echo_hdr);

    iecho->type = ICMP6_TYPE_EREQ;
    iecho->code = 0;
    iecho->chksum = 0;
    iecho->id     = htons(PING_ID);///PING_ID;
    iecho->seqno  = htons(++ping_seq_num);

    /* fill the additional data buffer with some data */
    for (i = 0; i < data_len; i++)
    {
        ((char*) iecho)[sizeof(struct icmp6_echo_hdr) + i] = (char) i;
    }
#if CHECKSUM_GEN_ICMP6
    iecho->chksum = _ip6_chksum_pseudo((unsigned char*)iecho,tot_len,
          IP6_NEXTH_ICMP6,tot_len, reply_src, dest_add);
#endif
}

/* ping variables */
static u32_t ping_time;
static struct raw_pcb *ping_pcb = NULL;
static u16_t ping_data_size = PING_DATA_SIZE;

/* Ping using the raw ip */
///u8_t (*raw_recv_fn)(void *arg, struct raw_pcb *pcb, struct pbuf *p,const ip_addr_t *addr);

static u8_t ping6_recv(void *arg, struct raw_pcb *pcb, struct pbuf *p,const ip_addr_t *addr)
{
	struct icmp6_echo_hdr *iecho;

	//we can also check src ip here, but just egnore it
	if ((p->tot_len >= (PBUF_IP_HLEN + sizeof(struct icmp6_echo_hdr))))
	{
		iecho = (struct icmp6_echo_hdr *)((u8_t*)p->payload + PBUF_IP_HLEN);
		if ((iecho->type == ICMP6_TYPE_EREP) && (iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num))) {
			LWIP_DEBUGF( PING_DEBUG, ("ping: recv "));
			ip_addr_debug_print(PING_DEBUG, addr);
			LWIP_DEBUGF( PING_DEBUG, (" time=%"U32_F" ms\n", (sys_now()-ping_time)));

			pbuf_free(p);
			return 1; /* eat the packet */
		}
	}
	return 0; /* don't eat the packet */
}

static void ping6_send(struct raw_pcb *raw, ip_addr_t *addr)
{
	struct pbuf *p;
	struct icmp6_echo_hdr *iecho;
	size_t ping_size = sizeof(struct icmp6_echo_hdr) + ping_data_size;
	err_t error;

	p = pbuf_alloc(PBUF_IP, (u16_t)ping_size, PBUF_RAM);
	if (!p) {
		return;
	}
	if ((p->len == p->tot_len) && (p->next == NULL)) {
		iecho = (struct icmp6_echo_hdr *)p->payload;
		ping6_prepare_echo(iecho, (u16_t)ping_size,&ping_src,&ping_dst);
		error = raw_sendto(raw, p, addr);
		ping_time = sys_now();

		LWIP_DEBUGF(PING_DEBUG, ("ping:[%"U32_F"] send ", ping_seq_num));
		ip_addr_debug_print(PING_DEBUG, addr);
		if(error != ERR_OK){
			LWIP_DEBUGF( PING_DEBUG, (" error (%d)\n",error));
		}else{
			LWIP_DEBUGF( PING_DEBUG, ("\n"));
		}
	}
	pbuf_free(p);
}

static void ping_timeout(void *arg)
{
	struct raw_pcb *pcb = (struct raw_pcb*)arg;

	if( ping_pcb != NULL ) {
		ip_addr_t ip;
		ip.type = IPADDR_TYPE_V6;
		ip.u_addr.ip6 = ping_dst;
		ping6_send(pcb, &ip);
		sys_timeout(PING_DELAY, ping_timeout, pcb);
	}
}

static void ping_raw_init(unsigned int ping_interval)
{
	ip_addr_t ip;

	ip.type = IPADDR_TYPE_V6;
	ip.u_addr.ip6 = ping_src;
	ping_pcb = raw_new(IPPROTO_ICMPV6);//IP_PROTO_ICMP);
	raw_recv(ping_pcb, ping6_recv, NULL);
	//raw_bind(ping_pcb, IP6_ADDR_ANY);
	///raw_bind(ping_pcb, &ip);
	///ip_addr_set_ipaddr(&ping_pcb->local_ip, &ip);
	ping_pcb->local_ip = ip;
	sys_timeout(ping_interval, ping_timeout, ping_pcb);
}

void ping_test_init(void)
{
	ip6addr_aton("2001::e040:768:8a26:2",&ping_dst);
	///ip6addr_aton(IP6_ADDR_ANY,&ping_src);
	ip6addr_aton("2001:0000:0000:0000:a268:1cff:fe74:d3d0",&ping_src);
	ping_raw_init(PING_DELAY);
}

extern void *net_get_sta_handle(void);
extern int net_get_if_ipv6_addr_valid(struct wlan_ip_config *addr, void *intrfc_handle);
int ping6(unsigned char idx_netif,ip6_addr_t *ip6_dst,unsigned int ping_interval)
{
	struct wlan_ip_config netif_ip_addr;
	int num;

	num = net_get_if_ipv6_addr_valid(&netif_ip_addr,net_get_sta_handle());
	if( num <= 0 ) {
		return -1;
	}

	if( num > idx_netif ) {
		memcpy(&ping_src,&netif_ip_addr.ipv6[idx_netif].address,sizeof(ip6_addr_t));
	}
	memcpy(&ping_dst,ip6_dst,sizeof(ip6_addr_t));
	ping_raw_init(ping_interval);

	return 0;
}

///{"ping6",	"ping6 xxx",		 ping_cmd},
///such as: ping6 2001::e040:768:8a26:2
void ping_cmd(char *buf, int len, int argc, char **argv)
{
	char *dst_ip = argv[1];
	struct wlan_ip_config netif_ip_addr;
	int num = 0;
	int idx = -1;
	int i;
	unsigned int ping_interval = PING_DELAY;

	if(argc > 1){
		if(ping_pcb != NULL) {
			if (strcmp(argv[1], "stop") == 0) {
				raw_remove(ping_pcb);
				ping_pcb = NULL;
				bk_printf("ping6 stop\r\n");
			}
		}else {
			dst_ip = argv[1];
			if(argc > 2){
				idx = atoi(argv[2]);
				if(idx < -1 || idx >= MAX_IPV6_ADDRESSES){
					idx = -1;
				}
			}
			if(argc > 3){
				ping_interval = atoi(argv[3]);
				if(ping_interval <= 0){
					ping_interval = PING_DELAY;
				}
			}
			num = net_get_if_ipv6_addr_valid(&netif_ip_addr,net_get_sta_handle());
			if(num <= 0){
				bk_printf("ipv6 is invalid\r\n");
				return;
			}
			for(i = 0; i < num; i++) {
				bk_printf("ipv6:%08x-%08x-%08x-%08x,state:%02x\r\n",netif_ip_addr.ipv6[i].address.addr[0],
					netif_ip_addr.ipv6[i].address.addr[1],netif_ip_addr.ipv6[i].address.addr[2],
					netif_ip_addr.ipv6[i].address.addr[3],netif_ip_addr.ipv6[i].addr_state);
			}
			if(idx >= num || idx >= MAX_IPV6_ADDRESSES){
				bk_printf("ipv6 idx is invalid\r\n");
				return;
			}
			if(idx < 0){
				idx = num - 1;
				bk_printf("un-assign,used ipv6:%08x-%08x-%08x-%08x,state:%02x\r\n",
					netif_ip_addr.ipv6[idx].address.addr[0],
					netif_ip_addr.ipv6[idx].address.addr[1],
					netif_ip_addr.ipv6[idx].address.addr[2],
					netif_ip_addr.ipv6[idx].address.addr[3],
					netif_ip_addr.ipv6[idx].addr_state);
			}else{
				bk_printf("used ipv6:%08x-%08x-%08x-%08x,state:%02x\r\n",
					netif_ip_addr.ipv6[idx].address.addr[0],
					netif_ip_addr.ipv6[idx].address.addr[1],
					netif_ip_addr.ipv6[idx].address.addr[2],
					netif_ip_addr.ipv6[idx].address.addr[3],
					netif_ip_addr.ipv6[idx].addr_state);
			}

			ip6addr_aton(dst_ip, &ping_dst);
			if(num > 0) {
				memcpy(&ping_src,&netif_ip_addr.ipv6[idx].address,sizeof(ip6_addr_t));
				ping_raw_init(ping_interval);
			}
		}
	}
}
#else
void ping_cmd(char *buf, int len, int argc, char **argv)
{

}
#endif
#define TCP_IVP6_CLIENT   0
#if TCP_IVP6_CLIENT
#include "errno.h"
extern void bk_printf(const char *fmt, ...);

void tcp_client_test_demo(void)
{
	int client_fd = socket(AF_INET6, SOCK_STREAM, 0);

	if (client_fd < 0){
        bk_printf("create socket error=%d(%s)!!!\n", errno, strerror(errno));
		return;
    }
	struct sockaddr_in6 server_addr;
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(18883);
    if (inet_pton(AF_INET6, "2001::e040:768:8a26:2", &server_addr.sin6_addr) <= 0) {
        bk_printf("inet_pton error!!!\n");
        return;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        bk_printf("socket connect error=%d(%s)!!!\n", errno, strerror(errno));
        return;
    }
    bk_printf("connect to server ok!\n");
	char buf[512];
	snprintf(buf,sizeof(buf),"This is tcp client\r\n");
	send(client_fd,buf,strlen(buf),0);
	int nRecv;
	nRecv = recv(client_fd,buf,sizeof(buf),0);
	if(nRecv > 0) {
		if(nRecv < sizeof(buf)){
			buf[nRecv] = '\0';
		}
		bk_printf("socket(%d) recv(%d):%s\r\n",client_fd,nRecv,buf);
	}else{
		bk_printf("socket(%d)recv error:%d\r\n",client_fd,nRecv);
	}

	rtos_delay_milliseconds(100);
	close(client_fd); ///1??¨¤??¡§a??¨¬??¡§??¨¢?¡§??¨¢?
	bk_printf("close client_fd:%d\r\n",client_fd);
}

void tcp_ipv6_cmd(char *buf, int len, int argc, char **argv)
{
	tcp_client_test_demo();
}
#else
void tcp_ipv6_cmd(char *buf, int len, int argc, char **argv)
{

}
#endif
// eof
