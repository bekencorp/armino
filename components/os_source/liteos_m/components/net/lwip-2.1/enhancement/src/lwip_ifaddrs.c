/*
 * Copyright (c) 2021-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "lwip/opt.h"

#if LWIP_IFADDRS
#if (LWIP_IPV4 || LWIP_IPV6) && LWIP_SOCKET
#include "ifaddrs.h"

#include <stdlib.h>

#include "lwip/sys.h"
#include "lwip/tcpip.h"
#include "lwip/priv/sockets_priv.h"
#include "lwip/mem.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"

struct ifaddrs_storage {
    struct ifaddrs ifa;
    union {
        struct sockaddr sa;
        struct sockaddr_in s4;
#if LWIP_IPV6
        struct sockaddr_in6 s6;
#endif
    } addr, netmask, dstaddr;
    char name[IFNAMSIZ];
};

struct getifaddrs_arg {
    struct ifaddrs **ifap;
    sys_sem_t cb_completed;
    int ret;
};

static int tcpip_init_finish = 1;
void lwip_freeifaddrs(struct ifaddrs *ifa);
static void ifaddrs_add_tail(struct ifaddrs **ifap, struct ifaddrs *ifaddr)
{
    struct ifaddrs *temp = NULL;

    ifaddr->ifa_next = NULL;
    if (*ifap == NULL) {
        *ifap = ifaddr;
        return;
    }

    for (temp = *ifap; temp->ifa_next != NULL; temp = temp->ifa_next) {
        /* nothing */
    }

    temp->ifa_next = ifaddr;
}

static struct ifaddrs_storage *new_ifaddrs_storage(void)
{
    struct ifaddrs *ifaddr = NULL;
    struct ifaddrs_storage *if_storage = NULL;

    if_storage = (struct ifaddrs_storage *)mem_malloc(sizeof(struct ifaddrs_storage));
    if (if_storage != NULL) {
        (void)memset_s((void*)if_storage, sizeof(struct ifaddrs_storage), 0, sizeof(struct ifaddrs_storage));
        ifaddr = &if_storage->ifa;
        ifaddr->ifa_name = if_storage->name;
        ifaddr->ifa_addr = &if_storage->addr.sa;
        ifaddr->ifa_netmask = &if_storage->netmask.sa;
        ifaddr->ifa_dstaddr = &if_storage->dstaddr.sa;
    }

    return if_storage;
}

static int get_ifa_name(struct netif *netif, struct ifaddrs *ifaddr)
{
    int ret;

    if (netif->link_layer_type == LOOPBACK_IF) {
        ifaddr->ifa_flags |= IFF_LOOPBACK;
        ret = snprintf_s(ifaddr->ifa_name, NETIF_NAMESIZE, (NETIF_NAMESIZE - 1), "%.2s", netif->name);
    } else {
        ret = snprintf_s(ifaddr->ifa_name, NETIF_NAMESIZE, (NETIF_NAMESIZE - 1), "%s", netif_get_name(netif));
    }

    return ret;
}

#if LWIP_IPV4
static int get_ipv4_ifaddr(struct netif *netif, struct ifaddrs *ifaddr)
{
    struct sockaddr_in *addr_in = NULL;

    if (netif->flags & NETIF_FLAG_UP) {
        ifaddr->ifa_flags |= IFF_UP;
    }

    if (netif->flags & NETIF_FLAG_ETHARP) {
        ifaddr->ifa_flags = ifaddr->ifa_flags & ((unsigned int)(~IFF_NOARP));
    } else {
        ifaddr->ifa_flags |= IFF_NOARP;
    }

    if (netif->flags & NETIF_FLAG_BROADCAST) {
        ifaddr->ifa_flags |= IFF_BROADCAST;
    }

#if LWIP_DHCP
    if (dhcp_supplied_address(netif)) {
        ifaddr->ifa_flags |= IFF_DYNAMIC;
    }
#endif

#if LWIP_IGMP
    if (netif->flags & NETIF_FLAG_IGMP) {
        ifaddr->ifa_flags |= IFF_MULTICAST;
    }
#endif

    if (netif->flags & NETIF_FLAG_LINK_UP) {
        ifaddr->ifa_flags |= IFF_RUNNING;
    }

#if LWIP_HAVE_LOOPIF
    if (netif->link_layer_type == LOOPBACK_IF) {
        addr_in = (struct sockaddr_in *)ifaddr->ifa_addr;
        addr_in->sin_family = AF_INET;
        addr_in->sin_addr.s_addr = ((ip4_addr_t *)&netif->ip_addr)->addr;
    } else
#endif
    {
        addr_in = (struct sockaddr_in *)ifaddr->ifa_addr;
        addr_in->sin_family = AF_INET;
        addr_in->sin_addr.s_addr = ((ip4_addr_t *)&netif->ip_addr)->addr;

        addr_in = (struct sockaddr_in *)ifaddr->ifa_netmask;
        addr_in->sin_family = AF_INET;
        addr_in->sin_addr.s_addr = ((ip4_addr_t *)&netif->netmask)->addr;

        addr_in = (struct sockaddr_in *)ifaddr->ifa_broadaddr;
        addr_in->sin_family = AF_INET;
        addr_in->sin_addr.s_addr = (((ip4_addr_t *)&netif->ip_addr)->addr & ((ip4_addr_t *)&netif->netmask)->addr) |
                                  ~((ip4_addr_t *)&netif->netmask)->addr;
    }

    return get_ifa_name(netif, ifaddr);
}
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
/* Stack support to retrieve the below flags for ipv6
IFF_UP
IFF_MULTICAST
IFF_RUNNING
IFF_LOOPBACK
*/
static int get_ipv6_ifaddr(struct netif *netif, struct ifaddrs *ifaddr, int tmp_index)
{
    struct sockaddr_in6 *addr_in6 = NULL;

    /* As of now supports the below falgs only */
    if (netif->flags & NETIF_FLAG_UP) {
        ifaddr->ifa_flags |= IFF_UP;
    }

#if LWIP_IPV6_MLD
    if (netif->flags & NETIF_FLAG_MLD6) {
        ifaddr->ifa_flags |= IFF_MULTICAST;
    }
#endif

    if (netif->flags & NETIF_FLAG_LINK_UP) {
        ifaddr->ifa_flags |= IFF_RUNNING;
    }

    addr_in6 = (struct sockaddr_in6 *)ifaddr->ifa_addr;
    addr_in6->sin6_family = AF_INET6;
    inet6_addr_from_ip6addr(&addr_in6->sin6_addr, (ip6_addr_t *)&netif->ip6_addr[tmp_index]);

    return get_ifa_name(netif, ifaddr);
}
#endif

static void getifaddrs_internal(struct getifaddrs_arg *arg)
{
    struct netif *netif = NULL;
    struct ifaddrs *ifaddr = NULL;
    struct ifaddrs_storage *if_storage = NULL;

#if LWIP_IPV6
    int n;
#endif

    arg->ret = ENOMEM;
    for (netif = netif_list; netif != NULL; netif = netif->next) {
#if LWIP_IPV4
        if_storage = new_ifaddrs_storage();
        if (if_storage == NULL) {
            lwip_freeifaddrs(
                *(arg->ifap)); /* ifap is assigned to NULL in getifaddrs, so garbage value will not be there */
            arg->ret = ENOMEM;
            goto RETURN;
        }

        /* if get one or more netif info, then getifaddrs return 0(OK) */
        arg->ret = 0;
        ifaddr = &if_storage->ifa;
        (void)get_ipv4_ifaddr(netif, ifaddr);
        ifaddrs_add_tail(arg->ifap, ifaddr);
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
        for (n = 0; n < LWIP_IPV6_NUM_ADDRESSES; n++) {
            if ((netif->ip6_addr_state[n] & IP6_ADDR_VALID) == 0) {
                continue;
            }
            if_storage = new_ifaddrs_storage();
            if (if_storage == NULL) {
                lwip_freeifaddrs(
                *(arg->ifap)); /* ifap is assigned to NULL in getifaddrs, so garbage value will not be there */
                arg->ret = ENOMEM;
                goto RETURN;
            }

            /* if get one or more netif info, then getifaddrs return 0(OK) */
            arg->ret = 0;
            ifaddr = &if_storage->ifa;
            (void)get_ipv6_ifaddr(netif, ifaddr, n);
            ifaddrs_add_tail(arg->ifap, ifaddr);
        }
#endif
    }

RETURN:
#if !LWIP_TCPIP_CORE_LOCKING
    sys_sem_signal(&arg->cb_completed);
#endif
    return;
}

int lwip_getifaddrs(struct ifaddrs **ifap)
{
    struct getifaddrs_arg arg;

    LWIP_ERROR("lwip_getifaddrs : ifap is NULL", (ifap != NULL), return ERR_ARG);
    *ifap = NULL;

    if (!tcpip_init_finish) {
        set_errno(EACCES);
        return -1;
    }
    arg.ret = 0;
    arg.ifap = ifap;

#if LWIP_TCPIP_CORE_LOCKING
    LOCK_TCPIP_CORE();
    getifaddrs_internal(&arg);
    UNLOCK_TCPIP_CORE();
#else

    if (sys_sem_new(&arg.cb_completed, 0) != ERR_OK) {
        set_errno(ENOMEM);
        return -1;
    }

    tcpip_callback((tcpip_callback_fn)getifaddrs_internal, &arg);
    (void)sys_arch_sem_wait(&arg.cb_completed, 0);
    sys_sem_free(&arg.cb_completed);
#endif

    if (arg.ret != 0) {
        set_errno(arg.ret);
        *ifap = NULL;
        return -1;
    }

    return 0;
}

static void freeifaddrs_iteration(struct ifaddrs *ifa)
{
    if (ifa == NULL) {
        return;
    }

    if (ifa->ifa_next != NULL) {
        freeifaddrs_iteration(ifa->ifa_next);
    }

    mem_free(ifa);
}

void lwip_freeifaddrs(struct ifaddrs *ifa)
{
    freeifaddrs_iteration(ifa);
}

int getifaddrs(struct ifaddrs **ifap)
{
    return lwip_getifaddrs(ifap);
}

void freeifaddrs(struct ifaddrs *ifa)
{
    lwip_freeifaddrs(ifa);
}

#endif /* (LWIP_IPV4 || LWIP_IPV6) && LWIP_SOCKET */
#endif /* LWIP_IFADDRS */