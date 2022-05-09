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
#include "net.h"
#include <string.h>
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

/* ping variables */
static u16_t ping_seq_num;
struct _ip_addr
{
    uint8_t addr0, addr1, addr2, addr3;
};

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
    inet_addr_from_ip4addr(&to.sin_addr,ip_2_ip4(addr));

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

#if LWIP_IPV6
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
}
    

static int ping6_recv(int s, int *ttl)
{
	struct icmp6_echo_hdr *iecho;
    struct sockaddr_in6 from;
    int fromlen = sizeof(struct sockaddr_in6),len;
    char buf[128];

    while ((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &from, (socklen_t*) &fromlen)) > 0)
    {
        if (len >= (int)(sizeof(struct ip6_hdr) + sizeof(struct icmp6_echo_hdr)))
        {
            iecho = (struct icmp6_echo_hdr *) (buf + IP6_HLEN);
            if ((iecho->type == ICMP6_TYPE_EREP) && (iecho->id == PING_ID) && (iecho->seqno == htons(ping_seq_num)))
            {
                *ttl = 5;
                return len;
            }
        }
    }
    return len;
}

static err_t ping6_send(int s, ip_addr_t *src_addr,ip_addr_t *dest_addr,int size)
{
    struct pbuf *p;
    struct icmp6_echo_hdr *iecho;
    struct sockaddr_in6 to;
	size_t ping_size = sizeof(struct icmp6_echo_hdr) + size;
	int error=0;

    p = pbuf_alloc(PBUF_IP, ping_size, PBUF_RAM);
    
    if (p != NULL) {
        iecho = (struct icmp6_echo_hdr *)p->payload;
		ping6_prepare_echo(iecho, (u16_t)ping_size,ip_2_ip6(src_addr),ip_2_ip6(dest_addr));
        memset(&to,0,sizeof(to));
        to.sin6_len = sizeof(to);
        to.sin6_family = AF_INET6;
        to.sin6_scope_id = ip6_addr_zone(ip_2_ip6(dest_addr));
        inet6_addr_from_ip6addr(&to.sin6_addr,ip_2_ip6(dest_addr));

        error = lwip_sendto(s, iecho, ping_size, 0 ,(struct sockaddr *)&to,sizeof(to));
        pbuf_free(p);
    }
    return (error == ping_size ? ERR_OK : ERR_VAL);
}
#endif

static int ping_mid_recv(int s, int *ttl,int af)
{
#if LWIP_IPV6
    if(af == AF_INET6)
        return ping6_recv(s,ttl);
    else
#endif
    return ping_recv(s,ttl);
}
static err_t ping_mid_send(int s, ip_addr_t *src_addr,ip_addr_t *dest_addr,int size)
{
#if LWIP_IPV6
    if(src_addr != NULL)
        return ping6_send(s,src_addr,dest_addr,size);
    else
#endif
    return ping_send(s,dest_addr,size); 
}


int ping(char* target_name, uint32_t times, size_t size)
{
    int s, ttl=0, recv_len,af=AF_INET,proto=IP_PROTO_ICMP;
    struct timeval timeout = { PING_RCV_TIMEO / 1000, PING_RCV_TIMEO % 1000 };
    ip_addr_t target_addr,*src_addr=NULL;
    uint32_t send_times;
    uint32_t recv_start_tick;
    struct addrinfo hint, *res = NULL;
    struct sockaddr_in *h = NULL;
#if LWIP_IPV6
    struct sockaddr_in6 *h6 = NULL;
    struct netif *netif=NULL;
#endif

    send_times = 0;
    ping_seq_num = 0;

    if (size == 0)
    {
        size = PING_DATA_SIZE;
    }

    memset(&hint, 0, sizeof(hint));
    /* convert URL to IP */
    if (lwip_getaddrinfo(target_name, NULL, &hint, &res) != 0)
    {
        LWIP_DEBUGF( PING_DEBUG, ("ping: unknown host\n"));
        return -1;
    }

    if(res->ai_family == AF_INET6)
    {
#if (LWIP_IPV6==0)
        {
            LWIP_DEBUGF( PING_DEBUG, ("ping: not support ipv6\n"));
            return -1;
        }
#else
        {
            proto = IP6_NEXTH_ICMP6;
            h6 = (struct sockaddr_in6 *)res->ai_addr;
            netif = (struct netif *)net_get_sta_handle();
            if(ip6_addr_isvalid(netif_ip6_addr_state(netif,0)))
                src_addr = &(netif->ip6_addr[0]);
            else
            {
                LWIP_DEBUGF( PING_DEBUG, ("ping: netif addr_ipv6 err\n"));
                return -1;
            }
            inet6_addr_to_ip6addr(ip_2_ip6(&target_addr),&h6->sin6_addr);
            IP_SET_TYPE(&target_addr,IPADDR_TYPE_V6);
            ip6_addr_set_zone(ip_2_ip6(&target_addr),ip6_addr_zone(ip_2_ip6(src_addr)));
        }
#endif        
    }
    else
    {
        h = (struct sockaddr_in *)res->ai_addr;
        inet_addr_to_ip4addr(ip_2_ip4(&target_addr),&h->sin_addr);
    }
    af = res->ai_family;
    lwip_freeaddrinfo(res);

    /* new a socket */
    if ((s = lwip_socket(af, SOCK_RAW, proto)) < 0)
    {
		LWIP_DEBUGF( PING_DEBUG, ("ping: create socket failed\n"));
        return -1;
    }

    lwip_setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));

    while (1)
    {
        if (ping_mid_send(s, src_addr,&target_addr, size) == ERR_OK)
        {
			recv_start_tick = sys_now();
            if ((recv_len = ping_mid_recv(s, &ttl,af)) >= 0)
            {
                LWIP_DEBUGF( PING_DEBUG, ("%d bytes from %s icmp_seq=%d ttl=%d time=%d ticks\n", recv_len, ipaddr_ntoa(&target_addr), send_times,
                        ttl, sys_now() - recv_start_tick));
            }
            else
            {
				LWIP_DEBUGF( PING_DEBUG, ("From %s icmp_seq=%d timeout\n", ipaddr_ntoa(&target_addr), send_times));
            }
        }
        else
        {
			LWIP_DEBUGF( PING_DEBUG, ("Send %s - error\n", ipaddr_ntoa(&target_addr)));
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

// eof
