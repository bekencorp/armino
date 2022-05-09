/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
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

#include "lwip/sockets.h"
#include "lwip/priv/tcpip_priv.h"
#include "lwip/priv/sockets_priv.h"

#if !LWIP_COMPAT_SOCKETS
#if LWIP_SOCKET

#define CHECK_NULL_PTR(ptr) do { if ((ptr) == NULL) { set_errno(EFAULT); return -1; } } while (0)

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    return lwip_accept(s, addr, addrlen);
}

int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
    CHECK_NULL_PTR(name);
    if (namelen < sizeof(*name)) {
        set_errno(EINVAL);
        return -1;
    }
    return lwip_bind(s, name, namelen);
}

int shutdown(int s, int how)
{
    return lwip_shutdown(s, how);
}

int getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
    CHECK_NULL_PTR(name);
    CHECK_NULL_PTR(namelen);
    return lwip_getpeername(s, name, namelen);
}

int getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
    CHECK_NULL_PTR(name);
    CHECK_NULL_PTR(namelen);
    return lwip_getsockname(s, name, namelen);
}

int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
    return lwip_getsockopt(s, level, optname, optval, optlen);
}

int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
    return lwip_setsockopt(s, level, optname, optval, optlen);
}

int closesocket(int s)
{
    return lwip_close(s);
}

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    CHECK_NULL_PTR(name);
    if (namelen < sizeof(*name)) {
        set_errno(EINVAL);
        return -1;
    }
    return lwip_connect(s, name, namelen);
}

int listen(int s, int backlog)
{
    return lwip_listen(s, backlog);
}

ssize_t recv(int s, void *mem, size_t len, int flags)
{
    CHECK_NULL_PTR(mem);
    return lwip_recv(s, mem, len, flags);
}

ssize_t recvfrom(int s, void *mem, size_t len, int flags,
                 struct sockaddr *from, socklen_t *fromlen)
{
    CHECK_NULL_PTR(mem);
    return lwip_recvfrom(s, mem, len, flags, from, fromlen);
}

ssize_t recvmsg(int s, struct msghdr *message, int flags)
{
    CHECK_NULL_PTR(message);
    if (message->msg_iovlen) {
        CHECK_NULL_PTR(message->msg_iov);
    }
    return lwip_recvmsg(s, message, flags);
}

ssize_t send(int s, const void *dataptr, size_t size, int flags)
{
    CHECK_NULL_PTR(dataptr);
    return  lwip_send(s, dataptr, size, flags);
}

ssize_t sendmsg(int s, const struct msghdr *message, int flags)
{
    return lwip_sendmsg(s, message, flags);
}

ssize_t sendto(int s, const void *dataptr, size_t size, int flags,
               const struct sockaddr *to, socklen_t tolen)
{
    CHECK_NULL_PTR(dataptr);
    if (to && tolen < sizeof(*to)) {
        set_errno(EINVAL);
        return -1;
    }
    return lwip_sendto(s, dataptr, size, flags, to, tolen);
}

int socket(int domain, int type, int protocol)
{
    return lwip_socket(domain, type, protocol);
}

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
    return lwip_inet_ntop(af, src, dst, size);
}

int inet_pton(int af, const char *src, void *dst)
{
    return lwip_inet_pton(af, src, dst);
}

#ifndef LWIP_INET_ADDR_FUNC
in_addr_t inet_addr(const char *cp)
{
    return ipaddr_addr(cp);
}
#endif

#ifndef LWIP_INET_NTOA_FUNC
char *inet_ntoa(struct in_addr addr)
{
    return ip4addr_ntoa((const ip4_addr_t*)&(addr));
}
#endif

#ifndef LWIP_INET_ATON_FUNC
int inet_aton(const char *cp, struct in_addr *addr)
{
    return ip4addr_aton(cp, (ip4_addr_t*)addr);
}
#endif

int ioctlsocket(int s, long cmd, void *argp)
{
    return lwip_ioctl(s, cmd, argp);
}

#if LWIP_SOCKET_SELECT
int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
    return lwip_select(maxfdp1, readset, writeset, exceptset, timeout);
}
#endif

#if LWIP_SOCKET_POLL
int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    return lwip_poll(fds, nfds, timeout);
}
#endif

unsigned int if_nametoindex(const char *ifname)
{
    return lwip_if_nametoindex(ifname);
}

#endif
#endif /* !LWIP_COMPAT_SOCKETS */

#define IOCTL_CMD_CASE_HANDLER() do {                      \
    err_t  err;                                            \
    struct lwip_ioctl_apimsg msg;                          \
    msg.sock = sock;                                       \
    msg.cmd = cmd;                                         \
    msg.argp = argp;                                       \
                                                           \
    err = tcpip_api_call(lwip_do_ioctl_impl, &msg.call);   \
    if (err != ENOSYS) {                                   \
        sock_set_errno(sock, err);                         \
        done_socket(sock);                                 \
        return -(err != ERR_OK);                           \
    }                                                      \
} while (0)

struct lwip_ioctl_apimsg {
    struct tcpip_api_call_data call;
    struct lwip_sock *sock;
    long cmd;
    void *argp;
};

static err_t lwip_do_ioctl_impl(struct tcpip_api_call_data *call);

#include "../api/sockets.c"

static u8_t lwip_ioctl_internal_SIOCGIFBRDADDR(struct ifreq *ifr)
{
    struct netif *netif = NULL;
    struct sockaddr_in *sock_in = NULL;

    /* get netif subnet broadcast addr */
    netif = netif_find(ifr->ifr_name);
    if (netif == NULL) {
        return ENODEV;
    }
    if (ip4_addr_isany_val(*(ip_2_ip4(&netif->netmask)))) {
        return ENXIO;
    }
    sock_in = (struct sockaddr_in *)&ifr->ifr_addr;
    sock_in->sin_family = AF_INET;
    sock_in->sin_addr.s_addr = (ip_2_ip4(&((netif)->ip_addr))->addr | ~(ip_2_ip4(&netif->netmask)->addr));
    return 0;
}

static u8_t lwip_ioctl_impl(const struct lwip_sock *sock, long cmd, void *argp)
{
    u8_t err = 0;
    struct ifreq *ifr = (struct ifreq *)argp;
    bool is_ipv6 = 0;

    /* allow it only on IPv6 sockets... */
    is_ipv6 = NETCONNTYPE_ISIPV6((unsigned int)(sock->conn->type));

    switch ((u32_t)cmd) {
        case SIOCGIFBRDADDR:
            if (is_ipv6 != 0) {
                err = EINVAL;
            } else {
                err = lwip_ioctl_internal_SIOCGIFBRDADDR(ifr);
            }
            break;
        default:
            err = ENOSYS;
            LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_ioctl(UNIMPL: 0x%lx)\n", cmd));
            break;
    }

    return err;
}

static err_t lwip_do_ioctl_impl(struct tcpip_api_call_data *call)
{
    struct lwip_ioctl_apimsg *msg = (struct lwip_ioctl_apimsg *)(void *)call;
    return lwip_ioctl_impl(msg->sock, msg->cmd, msg->argp);
}