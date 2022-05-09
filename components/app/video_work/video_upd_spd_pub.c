#include <common/bk_include.h>

#if (CONFIG_USE_APP_DEMO_VIDEO_TRANSSER)
#include "video_transfer_config.h"

#if (APP_DEMO_CFG_USE_UDP_SDP)
#include <os/os.h>
#include <common/bk_err.h>
#ifdef CFG_SUPPORT_RTT
#include <sys/socket.h>
#endif
#include "lwip/sockets.h"
#include <os/mem.h>
#include "video_upd_spd_pub.h"
#include "net.h"

extern uint8_t uap_ip_start_flag;

typedef struct vudp_sdp_st
{
    int sock;
    UINT8 *adv_buf;
    UINT32 adv_buf_len;
    beken_timer_t timer;
    UINT32 intval_ms;

    struct sockaddr_in sock_remote;
} VUDP_SDP_ST, *VUDP_SDP_PTR;

VUDP_SDP_PTR vudp_sdp = NULL;

static int vudp_sdp_init_socket(int *fd_ptr, UINT16 local_port)
{
    struct sockaddr_in l_socket;
    int so_broadcast = 1, sock = -1;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        VUPD_SDP_FATAL("Socket error\n");
        goto sta_udp_exit;
    }

    l_socket.sin_family = AF_INET;
    l_socket.sin_port = htons(local_port);
    l_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    os_memset(&(l_socket.sin_zero), 0, sizeof(l_socket.sin_zero));

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast)) != 0)
    {
        VUPD_SDP_FATAL("notify socket setsockopt error\n");
        goto sta_udp_exit;
    }

    if (bind(sock, (struct sockaddr *)&l_socket, sizeof(l_socket)) != 0)
    {
        VUPD_SDP_FATAL(" notify socket bind error\n");
        goto sta_udp_exit;
    }

    if(fd_ptr)
        *fd_ptr = sock;

    return 0;

sta_udp_exit:
    if((sock) != -1) {
        closesocket(sock);
        sock = -1;
    }

    return -1;
}

int vudp_sdp_pub_init(VUDP_SDP_INIT_PTR param)
{
    VUPD_SDP_PRT("vudp_sdp_int\r\n");
    if(param == NULL)
    {
        VUPD_SDP_FATAL("parm err\r\n");
        return kParamErr;
    }

    if(vudp_sdp == NULL)
    {
        vudp_sdp = os_malloc(sizeof(VUDP_SDP_ST));
        if(vudp_sdp == NULL)
        {
            VUPD_SDP_FATAL("malloc fail\r\n");
            return kNoMemoryErr;
        }
        os_memset(vudp_sdp, 0, sizeof(VUDP_SDP_ST));

        if((param->adv_buf != NULL) && (param->adv_buf_len != 0))
        {
            UINT8 *adv_buf = os_malloc(param->adv_buf_len);
            if(adv_buf == NULL)
            {
                VUPD_SDP_FATAL("malloc fail\r\n");
                os_free(vudp_sdp);
                vudp_sdp = NULL;
                return kNoMemoryErr;
            }

            os_memcpy(adv_buf, param->adv_buf,param->adv_buf_len);
            vudp_sdp->adv_buf = adv_buf;
            vudp_sdp->adv_buf_len = param->adv_buf_len;
        }

        if(vudp_sdp_init_socket(&vudp_sdp->sock, param->local_port) != 0)
        {
            VUPD_SDP_FATAL("socket fail\r\n");
            if(vudp_sdp->adv_buf)
                os_free(vudp_sdp->adv_buf);
            os_free(vudp_sdp);
            vudp_sdp = NULL;
            return kGeneralErr;
        }

        vudp_sdp->sock_remote.sin_family = AF_INET;
        vudp_sdp->sock_remote.sin_port = htons(param->remote_port);
        vudp_sdp->sock_remote.sin_addr.s_addr = INADDR_BROADCAST;

        VUPD_SDP_PRT("vudp_sdp_int ok\r\n");
        return kNoErr;
    }

    return kInProgressErr;
}

int vudp_sdp_change_adv_data(UINT8 *adv_data, UINT32 data_len)
{
    if((vudp_sdp) && (adv_data != NULL) && (data_len != 0))
    {
        GLOBAL_INT_DECLARATION();
        UINT8 *adv_buf = os_malloc(data_len);

        VUPD_SDP_PRT("set_adv_data\r\n");
        if(adv_buf == NULL)
        {
            VUPD_SDP_FATAL("malloc fail\r\n");
            return kNoMemoryErr;
        }

        os_memcpy(adv_buf, adv_data, data_len);

        GLOBAL_INT_DISABLE();
        if(vudp_sdp->adv_buf)
        {
            os_free(vudp_sdp->adv_buf);
            vudp_sdp->adv_buf_len = 0;
        }
        vudp_sdp->adv_buf = adv_buf;
        vudp_sdp->adv_buf_len = data_len;
        GLOBAL_INT_RESTORE();

        VUPD_SDP_PRT("succeed: %s, %d\r\n", vudp_sdp->adv_buf,
            vudp_sdp->adv_buf_len);

        return kNoErr;
    }

    return kInProgressErr;
}

int vudp_sdp_get_adv_data(UINT8 **adv_data, UINT32 *data_len)
{
    if((vudp_sdp) && (vudp_sdp->adv_buf != NULL) && (vudp_sdp->adv_buf_len != 0))
    {
        if(adv_data)
            *adv_data = vudp_sdp->adv_buf;

        if(data_len)
            *data_len = vudp_sdp->adv_buf_len;

        return kNoErr;
    }

    return kGeneralErr;
}

void ap_set_default_netif(void);
static void vudp_sdp_timer_handler(void *data)
{
    if((vudp_sdp) && (vudp_sdp->adv_buf != NULL) && (vudp_sdp->adv_buf_len != 0))
    {
    if (uap_ip_start_flag == 1)
		ap_set_default_netif();
        sendto(vudp_sdp->sock,
            vudp_sdp->adv_buf,
            vudp_sdp->adv_buf_len,
            0,
            (struct sockaddr *)&vudp_sdp->sock_remote,
            sizeof(struct sockaddr));
        VUPD_SDP_PRT("sendto anyone %s, %d\r\n", vudp_sdp->adv_buf,
            vudp_sdp->adv_buf_len);
    }
}

int vudp_sdp_start_timer(UINT32 time_ms)
{
    if(vudp_sdp)
    {
        int err;
        UINT32 org_ms = vudp_sdp->intval_ms;

        if(org_ms != 0)
        {
            if((org_ms != time_ms))
            {
                if(vudp_sdp->timer.handle != NULL)
                {
                    err = rtos_deinit_timer(&vudp_sdp->timer);
                    if(kNoErr != err)
                    {
                        VUPD_SDP_FATAL("deinit time fail\r\n");
                        return kGeneralErr;
                    }
                    vudp_sdp->timer.handle = NULL;
                }
            }
            else
            {
                VUPD_SDP_FATAL("timer aready start\r\n");
                return kNoErr;
            }
        }

        err = rtos_init_timer(&vudp_sdp->timer,
                                time_ms,
                                vudp_sdp_timer_handler,
                                NULL);
        if(kNoErr != err)
        {
            VUPD_SDP_FATAL("init timer fail\r\n");
            return kGeneralErr;
        }
        vudp_sdp->intval_ms = time_ms;

        err = rtos_start_timer(&vudp_sdp->timer);
        if(kNoErr != err)
        {
            VUPD_SDP_FATAL("start timer fail\r\n");
            return kGeneralErr;
        }
        VUPD_SDP_PRT("vudp_sdp_start_timer\r\n");

        return kNoErr;
    }
    return kInProgressErr;
}

int vudp_sdp_stop_timer(void)
{
    if(vudp_sdp)
    {
        int err;

        err = rtos_stop_timer(&vudp_sdp->timer);
        if(kNoErr != err)
        {
            VUPD_SDP_FATAL("stop time fail\r\n");
            return kGeneralErr;
        }

        return kNoErr;
    }
    return kInProgressErr;
}

int vudp_sdp_pub_deinit(void)
{
    VUPD_SDP_PRT("vudp_sdp_deint\r\n");

    if(vudp_sdp != NULL)
    {
        int err;
        if(vudp_sdp->timer.handle != NULL)
        {
            err = rtos_deinit_timer(&vudp_sdp->timer);
            if(kNoErr != err)
            {
                VUPD_SDP_FATAL("deinit time fail\r\n");
                return kGeneralErr;
            }
        }

        closesocket(vudp_sdp->sock);

        if(vudp_sdp->adv_buf)
            os_free(vudp_sdp->adv_buf);

        os_free(vudp_sdp);
        vudp_sdp = NULL;

        VUPD_SDP_PRT("vudp_sdp_deint ok\r\n");
        return kNoErr;
    }

    return kInProgressErr;
}
#endif // APP_DEMO_CFG_USE_SDP
#endif // CFG_USE_APP_DEMO_VIDEO_TRANSSER
