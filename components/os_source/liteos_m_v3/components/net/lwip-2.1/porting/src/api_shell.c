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

#define LWIP_STATIC static
#define DHCP_STATE_OFF 0
#define LWIP_EXT_POLL_SUPPORT 1

#include "lwip/api.h"
#include "lwip/netdb.h"
#include "lwip/stats.h"
#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/icmp.h"
#include "los_config.h"
#include <string.h>

#include "shcmd.h"
#include "shell.h"
#include "los_debug.h"
#include "los_task.h"

/* Forward Declarations [START] */
#ifndef LWIP_TESTBED
LWIP_STATIC
#endif
int print_netif(struct netif *netif, char *print_buf, unsigned int buf_len);

#ifndef LWIP_TESTBED
LWIP_STATIC
#endif
void lwip_ifconfig_show_internal(void *arg);

#if LWIP_DNS
LWIP_STATIC unsigned int get_hostip(const char *hname);

#ifndef LWIP_TESTBED
LWIP_STATIC
#endif
struct hostent *gethostnameinfo(const char *host, char *tmphstbuf, size_t hstbuflen);
#endif /* LWIP_DNS */

#define PRINT_BUF_LEN 1024

struct ifconfig_option {
    char iface[IFNAMSIZ];
    unsigned int option;
    ip_addr_t ip_addr;
    ip_addr_t netmask;
    ip_addr_t gw;
    unsigned char ethaddr[6];
    u16_t mtu;
    /* when using telnet, print to the telnet socket will result in system  */
    /* deadlock. So we cahe the prinf data to a buf, and when the tcpip      */
    /* callback returns, then print the data out to the telnet socket       */
    sys_sem_t cb_completed;
    char cb_print_buf[PRINT_BUF_LEN];
    unsigned int print_len;
};

#ifndef LWIP_TESTBED
LWIP_STATIC
#endif
int print_netif(struct netif *netif, char *print_buf, unsigned int buf_len)
{
    int i, ret;
    char *tmp = print_buf;

    if (buf_len < 1) {
        goto out;
    }
    if (netif->link_layer_type == LOOPBACK_IF) {
        ret = snprintf_s(tmp, buf_len, (buf_len - 1), "%s\t", netif->name);
    } else {
        ret = snprintf_s(tmp, buf_len, (buf_len - 1), "%s%u\t", netif->name, netif->num);
    }

    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;
#if LWIP_IPV4
    ret = snprintf_s(tmp, buf_len, (buf_len - 1), "ip:%s ", ipaddr_ntoa(&netif->ip_addr));
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = snprintf_s(tmp, buf_len, (buf_len - 1), "netmask:%s ", ipaddr_ntoa(&netif->netmask));
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = snprintf_s(tmp, buf_len, (buf_len - 1), "gateway:%s\n", ipaddr_ntoa(&netif->gw));
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;
#endif
    ret = snprintf_s(tmp, buf_len, (buf_len - 1), "\tHWaddr ");
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    for (i = 0; i < netif->hwaddr_len - 1; i++) {
        ret = snprintf_s(tmp, buf_len, (buf_len - 1), "%02x:", netif->hwaddr[i]);
        if ((ret <= 0) || ((unsigned int)ret >= buf_len))
            goto out;
        tmp += ret;
        buf_len -= (unsigned int)ret;
    }

    ret = snprintf_s(tmp, buf_len, (buf_len - 1), "%02x", netif->hwaddr[i]);
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    ret = snprintf_s(tmp, buf_len, (buf_len - 1), " MTU:%d %s", netif->mtu,
                     netif->flags & NETIF_FLAG_UP ? "Running" : "Stop");
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;
    buf_len -= (unsigned int)ret;

    if (netif_default == netif && netif_is_up(netif)) {
        ret = snprintf_s(tmp, buf_len, (buf_len - 1), " %s", "Default");
        if ((ret <= 0) || ((unsigned int)ret >= buf_len))
            goto out;
        tmp += ret;
        buf_len -= (unsigned int)ret;
    }

    ret = snprintf_s(tmp, buf_len, (buf_len - 1), " %s\n", netif->flags & NETIF_FLAG_LINK_UP ? "Link UP" : "Link Down");
    if ((ret <= 0) || ((unsigned int)ret >= buf_len))
        goto out;
    tmp += ret;

out:
    return (int)(tmp - print_buf);
}

#ifndef LWIP_TESTBED
LWIP_STATIC
#endif
void lwip_ifconfig_show_internal(void *arg)
{
    struct netif *netif = NULL;
    struct ifconfig_option *ifconfig_cmd = (struct ifconfig_option *)arg;
    int ret;

    if (netif_list == NULL) {
        ret = snprintf_s(ifconfig_cmd->cb_print_buf, PRINT_BUF_LEN - ifconfig_cmd->print_len,
                         ((PRINT_BUF_LEN - ifconfig_cmd->print_len) - 1), "Device not init\n");
        if ((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) {
            ifconfig_cmd->print_len += (unsigned int)ret;
        }
        sys_sem_signal(&ifconfig_cmd->cb_completed);
        return;
    }

    if (ifconfig_cmd->iface[0] == '\0') {
        /* display all netif */
        for (netif = netif_list; netif != NULL; netif = netif->next) {
            ret = print_netif(netif, ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
                              PRINT_BUF_LEN - ifconfig_cmd->print_len);
            ifconfig_cmd->print_len += (unsigned int)ret;
        }
    } else {
        netif = netif_find(ifconfig_cmd->iface);
        if (netif == NULL) {
            ret = snprintf_s(ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
                             (PRINT_BUF_LEN - ifconfig_cmd->print_len),
                             ((PRINT_BUF_LEN - ifconfig_cmd->print_len) - 1), "Device not found\n");
            if ((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfig_cmd->print_len))) {
                ifconfig_cmd->print_len += (unsigned int)ret;
            }

            sys_sem_signal(&ifconfig_cmd->cb_completed);
            return;
        }

        ret = print_netif(netif, ifconfig_cmd->cb_print_buf + ifconfig_cmd->print_len,
                          PRINT_BUF_LEN - ifconfig_cmd->print_len);
        ifconfig_cmd->print_len += (unsigned int)ret;
    }
    sys_sem_signal(&ifconfig_cmd->cb_completed);
}

/*lint -e838 -e438*/
u32_t lwip_ifconfig(int argc, const char **argv)
{
    static struct ifconfig_option ifconfig_cmd;
    err_t ret;

#if LWIP_STATS
    u32_t stat_err_cnt;
    u32_t stat_drop_cnt;
    u32_t stat_rx_or_tx_cnt;
    u32_t stat_rx_or_tx_bytes;
#endif

    (void)memset_s(&ifconfig_cmd, sizeof(ifconfig_cmd), 0, sizeof(ifconfig_cmd));
    if (sys_sem_new(&ifconfig_cmd.cb_completed, 0) != ERR_OK) {
        PRINTK("%s: sys_sem_new fail\n", __FUNCTION__);
        return 1;
    }

    /* Get the interface */

    /* no more arguments, show all the interface state. */
    ret = tcpip_callback(lwip_ifconfig_show_internal, &ifconfig_cmd);
    if (ret != ERR_OK) {
        sys_sem_free(&ifconfig_cmd.cb_completed);
        PRINTK("ifconfig : internal error, l:%d err:%d\n", __LINE__, ret);
        return 1;
    }
    (void)sys_arch_sem_wait(&ifconfig_cmd.cb_completed, 0);
    sys_sem_free(&ifconfig_cmd.cb_completed);
    ifconfig_cmd.cb_print_buf[PRINT_BUF_LEN - 1] = '\0';
    PRINTK("%s", ifconfig_cmd.cb_print_buf);

    return 0;
}
/*lint +e838 +e438*/

#if LWIP_DNS
#ifndef LWIP_TESTBED
LWIP_STATIC
#endif
struct hostent *gethostnameinfo(const char *host, char *tmphstbuf, size_t hstbuflen)
{
    static struct hostent hostbuf;
    struct hostent *hp = NULL;
    int res;
    int herr;

    if (tmphstbuf == NULL)
        return NULL;
    res = lwip_gethostbyname_r(host, &hostbuf, tmphstbuf, hstbuflen, &hp, &herr);
    /*  Check for errors.  */
    if (res || hp == NULL) {
        return NULL;
    }
    return hp;
}

LWIP_STATIC unsigned int get_hostip(const char *hname)
{
    unsigned int ip = 0;
    int ret;
    const size_t hstbuflen = 1024;
    char *tmphstbuf = NULL;

    tmphstbuf = (char *)zalloc(sizeof(char) * hstbuflen);
    if (tmphstbuf == NULL)
        return 0;

    struct hostent *pent = gethostnameinfo(hname, tmphstbuf, hstbuflen);
    if (pent == NULL || pent->h_addr == NULL) {
        free(tmphstbuf);
        return 0;
    }
    ret = memcpy_s(&ip, sizeof(ip), pent->h_addr, 4);
    if (ret != 0) {
        free(tmphstbuf);
        return 0;
    }
    free(tmphstbuf);
    return ip;
}

#endif

static int ping_taskid = -1;
static int ping_kill = 0;
#define LWIP_SHELL_CMD_PING_TIMEOUT 2000
#define LWIP_SHELL_CMD_PING_RETRY_TIMES 4
#define PING_ZERO_DATA_LEN 8

static void lwip_ping_usage(void)
{
    PRINTK("Usage:\n  ping [ip]\n");
}

LWIP_STATIC int OsPingFunc(u32_t *parg)
{
    int sfd;
    struct sockaddr_in to;
    struct pbuf *pbuf_resp = NULL;
    struct icmp_echo_hdr *iecho = NULL;
    struct icmp_echo_hdr *iecho_resp = NULL;
    struct ip_hdr *iphdr_resp = NULL;
    u32_t iecho_len;
    s16_t ip_hlen;
    u32_t forever;
    u32_t i;
    u32_t succ_cnt = 0;
    u32_t failed_cnt = 0;
    struct timespec start, end;
    long timout_ms;
    struct pollfd pfd;
    long rtt;
    int ret = 0;
    u32_t intrvl;
    char *data_buf = NULL;
    BOOL timeout_flag = false;
    char buf[50];

    u32_t destip = parg[0];
    u32_t cnt = parg[1];
    u32_t interval = parg[2];
    u32_t data_len = parg[3];

    iecho_len = sizeof(struct icmp_echo_hdr) + data_len;
    sfd = lwip_socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sfd < 0) {
        perror("Ping socket");
        return -1;
    }
    pbuf_resp = pbuf_alloc(PBUF_RAW, IP_HLEN + sizeof(struct icmp_echo_hdr), PBUF_RAM);
    if (pbuf_resp == NULL) {
        PRINTK("Ping: pbuf_resp malloc failed\n");
        ret = -1;
        goto FAILURE;
    }
    iecho = (struct icmp_echo_hdr *)mem_malloc(iecho_len);
    if (iecho == NULL) {
        PRINTK("Ping: echo request malloc failed\n");
        ret = -1;
        goto FAILURE;
    }

    to.sin_family = AF_INET;
    to.sin_addr.s_addr = destip; /* already in network order */
    to.sin_port = 0;

    if (data_len > PING_ZERO_DATA_LEN) {
        (void)memset_s(iecho, sizeof(struct icmp_echo_hdr) + PING_ZERO_DATA_LEN,
                       0, sizeof(struct icmp_echo_hdr) + PING_ZERO_DATA_LEN);
        data_buf = (char *)iecho + sizeof(struct icmp_echo_hdr) + PING_ZERO_DATA_LEN;
        for (i = 0; i < data_len - PING_ZERO_DATA_LEN; i++) {
            *(data_buf + i) = i + 0x10;
        }
    } else {
        (void)memset_s(iecho, sizeof(struct icmp_echo_hdr) + data_len, 0, sizeof(struct icmp_echo_hdr) + data_len);
    }
    iecho->id = htons((u16_t)LOS_CurTaskIDGet());
    ICMPH_TYPE_SET(iecho, (u8_t)ICMP_ECHO);
    forever = (cnt ? 0 : 1);
    i = 0;
    while (!ping_kill && (forever || (i < cnt))) {
        iecho->seqno = htons((u16_t)i);
        iecho->chksum = 0;
        iecho->chksum = inet_chksum((void *)iecho, iecho_len);

        ret = sendto(sfd, iecho, iecho_len, 0, (struct sockaddr *)&to, (socklen_t)sizeof(to));
        if (ret < 0) {
            perror("Ping: sending ICMP echo request failed\n");
            goto FAILURE;
        }

        /* capture the start time to calculate RTT */
        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        /* poll for ICMP echo response msg */
        pfd.fd = sfd;

        do {
            pfd.events = POLLIN;
            pfd.revents = 0;
            timeout_flag = false;
            ret = poll(&pfd, 1, LWIP_SHELL_CMD_PING_TIMEOUT);
            if (ret < 0) {
                perror("Ping: poll\n");
                goto FAILURE;
            } else if (ret == 0) {
                /* first type timeout event */
                timeout_flag = true;
                break;
            }

            ret = recv(sfd, pbuf_resp->payload, pbuf_resp->len, MSG_DONTWAIT);
            if (ret < 0) {
                perror("Ping: recv echo reply failed\n");
                goto FAILURE;
            }

            /* Accessing ip header and icmp header */
            iphdr_resp = pbuf_resp->payload;

            ip_hlen = (IPH_HL(iphdr_resp) << 2);
            if (pbuf_header(pbuf_resp, -ip_hlen)) {
                /* this failure will never happen, but failure handle is written just to be in safe side */
                PRINTK("Ping : memory management failure\n");
                goto FAILURE;
            }
            iecho_resp = (struct icmp_echo_hdr *)pbuf_resp->payload;
            /* Reverting back pbuf to its original state */
            if (pbuf_header(pbuf_resp, ip_hlen)) {
                /* this failure will never happen, but failure handle is written just to be in safe side */
                PRINTK("ping : memory management failure\n");
                goto FAILURE;
            }

            if ((iphdr_resp->src.addr != to.sin_addr.s_addr) ||
                ((ICMPH_TYPE(iecho_resp) == ICMP_ECHO) && (iphdr_resp->src.addr == to.sin_addr.s_addr))) {
                /* second type timeout event */
                (void)clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                timout_ms = ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000);
                timout_ms = LWIP_SHELL_CMD_PING_TIMEOUT - timout_ms;
            } else {
                timout_ms = 0;
                break;
            }
        } while (timout_ms >= 0);

        /* all timeout events are true timeout */
        if ((timout_ms < 0) || (timeout_flag == true)) {
            failed_cnt++;
            i++;
            PRINTK("\nPing: destination unreachable ...");
            continue;
        }
        /* capture the end time to calculate round trip time */
        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        rtt = ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000);

        if (iphdr_resp->src.addr == to.sin_addr.s_addr) {
            switch (ICMPH_TYPE(iecho_resp)) {
                case ICMP_ER:
                    PRINTK("\n[%u]Reply from %s: ", i, inet_ntop(AF_INET, &to.sin_addr, buf, sizeof(buf)));
                    if (rtt > 0) {
                        PRINTK("time=%ims ", rtt);
                    } else {
                        PRINTK("time<1ms ");
                    }
                    PRINTK("TTL=%u", iphdr_resp->_ttl);

                    /* delay 1s for every successful ping */
                    intrvl = interval;
                    do {
                        if (intrvl < 1000) {
                            sys_msleep(intrvl);
                            break;
                        }
                        intrvl -= 1000;
                        sys_msleep(1000);
                        if (ping_kill == 1)
                            break;
                    } while (intrvl > 0);
                    succ_cnt++;
                    break;
                case ICMP_DUR:
                    PRINTK("\nPing: destination host unreachable ...");
                    break;
                case ICMP_SQ:
                    PRINTK("\nPing: source quench ...");
                    break;
                case ICMP_RD:
                    PRINTK("\nPing: redirect ...");
                    break;
                case ICMP_TE:
                    PRINTK("\nPing: time exceeded ...");
                    break;
                case ICMP_PP:
                    PRINTK("\nPing: parameter problem ...");
                    break;
                default:
                    PRINTK("\nPing: unknow error ...");
                    break;
            }
            i++;
        }
    }

    PRINTK("\n--- %s ping statistics ---\n", inet_ntop(AF_INET, &to.sin_addr, buf, sizeof(buf)));
    PRINTK("%u packets transmitted, %u received, %u loss\n", i, succ_cnt, failed_cnt);

FAILURE:
    ping_kill = 0;
    (void)lwip_close(sfd);
    if (pbuf_resp != NULL) {
        (void)pbuf_free(pbuf_resp);
    }
    if (iecho != NULL) {
        mem_free(iecho);
    }
    return ret;
}

static void ping_cmd(u32_t *parg)
{
    int ret;

    ret = OsPingFunc(parg);
    if (ret < 0) {
        PRINTK("Ping cmd failed due to some errors\n");
    }

    ping_taskid = -1;
}

u32_t OsShellPing(int argc, const char **argv)
{
    int ret;
    u32_t i = 0;
    u32_t count = 0;
    int count_set = 0;
    u32_t interval = 1000; /* default ping interval */
    u32_t data_len = 48; /* default data length */
    ip4_addr_t dst_ipaddr;
    TSK_INIT_PARAM_S stPingTask;
    u32_t *parg = NULL;

    if ((argc < 1) || (argv == NULL)) {
        PRINTK("Ping: require dest ipaddr at least\n");
        goto ping_error;
    }

    if (!count_set) {
        count = LWIP_SHELL_CMD_PING_RETRY_TIMES;
    }

    /* initialize dst IP address */
    if (argc <= 0) {
        goto ping_error;
    }
#if LWIP_DNS
    dst_ipaddr.addr = get_hostip(argv[i]);
#endif /* LWIP_DNS */

    if (dst_ipaddr.addr == IPADDR_NONE || dst_ipaddr.addr == IPADDR_ANY) {
        PRINTK("Invalid dest ipaddr: NONE or ANY\n");
        return LOS_NOK;
    }

    parg = (u32_t *)malloc(4 * sizeof(int));
    if (parg == NULL) {
        return LOS_NOK;
    }
    parg[0] = dst_ipaddr.addr;
    parg[1] = count;
    parg[2] = interval;
    parg[3] = data_len;

    /* start one task if ping forever or ping count greater than 60 */
    if (count == 0 || count > LWIP_SHELL_CMD_PING_RETRY_TIMES) {
        if (ping_taskid > 0) {
            free(parg);
            PRINTK("Ping task already running and only support one now\n");
            return LOS_NOK;
        }
        stPingTask.pfnTaskEntry = (TSK_ENTRY_FUNC)ping_cmd;
        stPingTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
        stPingTask.pcName = "ping_task";
        stPingTask.usTaskPrio = 8; /* higher than shell */
        stPingTask.uwResved = LOS_TASK_STATUS_DETACHED;
        stPingTask.uwArg = (UINTPTR)parg;
        ret = LOS_TaskCreate((UINT32 *)(&ping_taskid), &stPingTask);
        if (ret != LOS_OK) {
            free(parg);
            PRINTK("ping_task create failed 0x%08x.\n", ret);
            count = LWIP_SHELL_CMD_PING_RETRY_TIMES;
        } else {
            return LOS_OK;
        }
    }

    /* two cases:
     1, ping cout less than LWIP_SHELL_CMD_PING_RETRY_TIMES;
     2, ping task create failed;
    */
    if (OsPingFunc(parg) < 0) {
        PRINTK("Ping cmd failed due some errors\n");
    }

    return LOS_OK;
ping_error:
    lwip_ping_usage();
    return LOS_NOK;
}

