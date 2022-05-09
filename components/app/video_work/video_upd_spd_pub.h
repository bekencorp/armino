#ifndef __VIDEO_UDP_SDP_PUB_H__
#define __VIDEO_UDP_SDP_PUB_H__

#include <common/bk_include.h>
#include "uart_pub.h"

#define VUPD_SDP_DEBUG              1
#if VUPD_SDP_DEBUG
#define VUPD_SDP_PRT                warning_prf
#define VUPD_SDP_WARN               warning_prf
#define VUPD_SDP_FATAL              fatal_prf
#else
#define VUPD_SDP_PRT                null_prf
#define VUPD_SDP_WARN               null_prf
#define VUPD_SDP_FATAL              null_prf
#endif

typedef struct vudp_sdp_init_st
{
    UINT8 *adv_buf;
    UINT32 adv_buf_len;
    UINT16 local_port;
    UINT16 remote_port;
} VUDP_SDP_INIT_ST, *VUDP_SDP_INIT_PTR;

int vudp_sdp_pub_init(VUDP_SDP_INIT_PTR param);
int vudp_sdp_pub_deinit(void);
int vudp_sdp_change_adv_data(UINT8 *adv_data, UINT32 data_len);
int vudp_sdp_get_adv_data(UINT8 **adv_data, UINT32 *data_len);
int vudp_sdp_start_timer(UINT32 time_ms);
int vudp_sdp_stop_timer(void);

#endif //__VIDEO_UDP_SDP_PUB_H__
