#include <common/bk_include.h>

#ifdef CONFIG_USE_APP_DEMO_VIDEO_TRANSSER
#include "video_transfer_config.h"

#if (APP_DEMO_CFG_USE_UDP_SDP)
#include <os/os.h>
#include <common/bk_err.h>
#ifdef CFG_SUPPORT_RTT
#include <sys/socket.h>
#endif
#include "lwip/sockets.h"
#include <os/mem.h>
#include "video_upd_spd.h"
#include "video_upd_spd_pub.h"
#include "wlan_ui_pub.h"
#include "param_config.h"
#include "net.h"
#include "string.h"
#include <components/netif.h>

static int vudp_sdp_generate_adv(char **adv_ptr, UINT32 *adv_length)
{
    #define ADV_ALLOC_LEN    (500)
    UINT32 adv_len = ADV_ALLOC_LEN;
    char *adv_temp = "{\"type\":\"0x%02x\",\"sta_ip\":\"%s\",\"ap_ip\":\"%s\",\"udp_port\":\"%d\",\"tcp_port\":\"%d\"}"; 
    char *adv_buf = os_malloc(adv_len);
    
    if(adv_buf == NULL)
    {
        VUPD_SDP_FATAL("no memory\r\n");
        return -1;
    }
    os_memset(adv_buf,0,adv_len);

	if (uap_ip_is_start())
		os_printf("%s, %d, ap mode\n", __func__, __LINE__);
	else if (sta_ip_is_start())
		os_printf("%s, %d, sta mode\n", __func__, __LINE__);
	else
		os_printf("%s, %d\n", __func__, __LINE__);

    netif_ip4_config_t ip4_config_ap, ip4_config_sta;
    os_memset(&ip4_config_ap, 0x0, sizeof(netif_ip4_config_t));
    os_memset(&ip4_config_sta, 0x0, sizeof(netif_ip4_config_t));
    bk_netif_get_ip4_config(NETIF_IF_AP, &ip4_config_ap);
    if (uap_ip_is_start())
		bk_netif_get_ip4_config(NETIF_IF_AP, &ip4_config_sta);
    else
		bk_netif_get_ip4_config(NETIF_IF_STA, &ip4_config_sta);

    UINT8 transfer_type = (APP_DEMO_CFG_USE_UDP << 1) + APP_DEMO_CFG_USE_TCP;
    transfer_type |= (0 << 3) | (1 << 2);  // bit3 sta mode,  bit2 softap_mode
    sprintf(adv_buf, adv_temp, 
        transfer_type,
        ip4_config_ap.ip,
        ip4_config_sta.ip,
        
        APP_DEMO_UDP_IMG_PORT,
        APP_DEMO_TCP_SERVER_PORT);

    bk_printf("adv_data:%s,%d\r\n", adv_buf, strlen(adv_buf));

    if(adv_ptr)
        *adv_ptr = adv_buf;
    if(adv_length)
        *adv_length = strlen(adv_buf);

    return 0;
}

int vudp_sdp_start(void)
{
    VUDP_SDP_INIT_ST spd;
    VUPD_SDP_PRT("vudp_sdp_start\r\n");
    int ret = 0;

    spd.local_port = UDP_SDP_LOCAL_PORT;
    spd.remote_port = UDP_SDP_REMOTE_PORT;
    spd.adv_buf = NULL;
    spd.adv_buf_len = 0;

    if(vudp_sdp_pub_init(&spd) != kNoErr)
    {
        ret = -1;
        goto sdp_int_err;
    }

    if(vudp_sdp_generate_adv((char **)&spd.adv_buf, &spd.adv_buf_len) != kNoErr)
    {
        ret = -3;
        goto sdp_int_err;
    }

    if(vudp_sdp_change_adv_data(spd.adv_buf, spd.adv_buf_len) != kNoErr)
    {
        ret = -4;
        goto sdp_int_err;
    }

    if(vudp_sdp_start_timer(1000) != kNoErr)
    {
        ret = -5;
        goto sdp_int_err;
    }

    os_free(spd.adv_buf);
    VUPD_SDP_PRT("done\r\n");
    
    return 0;

sdp_int_err:
    if(spd.adv_buf)
        os_free(spd.adv_buf);
    vudp_sdp_pub_deinit();
    return ret;
}

int vudp_sdp_stop(void)
{
    VUPD_SDP_PRT("vudp_sdp_stop\r\n");

    if(vudp_sdp_stop_timer() != kNoErr)
        return -1;

    if(vudp_sdp_pub_deinit() != kNoErr)
        return -2;

    VUPD_SDP_PRT("done\r\n");

    return 0;
}

#endif // APP_DEMO_CFG_USE_UDP_SDP
#endif // CONFIG_USE_APP_DEMO_VIDEO_TRANSSER
