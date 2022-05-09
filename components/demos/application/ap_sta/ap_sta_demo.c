#include "ap_sta_demo.h"
#include <common/bk_include.h>

#if AP_STA_DEMO
#include <os/mem.h>
#include "uart_pub.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/os.h>
#include "Error.h"
#include "string.h"
#include "sockets.h"
#include "wlan_ui_pub.h"
#include "rw_pub.h"
#include "param_config.h"
#include <os/str.h>

#define FRAME_HEAD_HEX        '0'
#define PROTOCOL_VERSION_MASK 'A'
#define FRAME_END_HEX         'E'

#define AP_STA_SERVER_PORT 22000
int g_Clietfd = 0;

typedef struct
{
    unsigned CloseAP: 1;
    unsigned ReturnMSG: 1;
    short MsgLen;
    char msg[0];
} AP_STA_MSG_T;

char ap_sta_full_frame_checksum(char *msg, int len)
{
    char sum = 0;
    int i;
    for(i = 0; i < len; i++)
    {
        sum += msg[i];
    }
    return sum;
}

char ap_sta_full_frame_check_xor(char *msg, int len)
{
    char XOR = 0;
    int i;
    for(i = 0; i < len; i++)
    {
        XOR ^= msg[i];
    }
    return XOR;
}

void ap_sta_softap_app_init(char *ap_ssid, char *ap_key)
{
    network_InitTypeDef_st wNetConfig;

    os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

    os_strcpy((char *)wNetConfig.wifi_ssid, ap_ssid);
    os_strcpy((char *)wNetConfig.wifi_key, ap_key);

    wNetConfig.wifi_mode = SOFT_AP;
    wNetConfig.dhcp_mode = DHCP_Server;
    wNetConfig.wifi_retry_interval = 100;
    os_strcpy((char *)wNetConfig.local_ip_addr, "192.168.10.1");
    os_strcpy((char *)wNetConfig.net_mask, "255.255.255.0");
    os_strcpy((char *)wNetConfig.dnsServer_ip_addr, "192.168.10.1");

    bk_printf("SSID:%s , KEY:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
    bk_wlan_start(&wNetConfig);
}

void ap_sta_stop_Softap(void)
{
    bk_wlan_stop(SOFT_AP);
}

void ap_sta_softap_start( void )
{
    char *ap_ssid = "AP_STA_Bk7231";
    char *ap_key = "12345678";

    ap_sta_softap_app_init(ap_ssid, ap_key);
}

int ap_sta_target_wifi_station_connect(const char *ssid, const char *passwd)
{
    network_InitTypeDef_st wNetConfig;
    os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

    os_strcpy((char *)wNetConfig.wifi_ssid, ssid);
    os_strcpy((char *)wNetConfig.wifi_key, passwd);

    wNetConfig.wifi_mode = STATION;
    wNetConfig.dhcp_mode = DHCP_CLIENT;
    wNetConfig.wifi_retry_interval = 100;

    bk_wlan_start(&wNetConfig);

    return 0;
}

int calc_msg_length(char *Rxbuf, int len)
{
    int Len = -1;

    if((len >= 2) && (Rxbuf[0] >= '0') && (Rxbuf[0] <= '9') && (Rxbuf[1] >= '0') && (Rxbuf[1] <= '9'))
    {
        Len = (Rxbuf[0] - '0') * 10;
        Len += (Rxbuf[1] - '0');
    }
    return Len;
}

/***
'0' 'A' 'checksum' 'checkxor' Len(2Byte)ssidLen(2Byte)"ssid"keyLen(2Byte)"Key"E
Such As,SSID:"5050",Key:"12345678"
		"0A??120450500812345678E"
***/
AP_STA_MSG_T *ap_sta_analysis_message(char *Rxbuf, int Len)
{
    char ssid[33];
    char passwd[65];
    char CheckSum;
    char CheckXOR;
    int offset = 0;
    int strLen;
    int ssidLen, keyLen;
    int MsgLen;
    AP_STA_MSG_T *Result = NULL;

    if((Rxbuf[offset] == FRAME_HEAD_HEX) && (Rxbuf[offset + 1] == PROTOCOL_VERSION_MASK) && (Rxbuf[Len - 1] == FRAME_END_HEX))
    {
        CheckSum = ap_sta_full_frame_checksum(&Rxbuf[offset + 4], Len - 5);
        //if(CheckSum != Rxbuf[offset+2]) return;
        CheckXOR = ap_sta_full_frame_check_xor(&Rxbuf[offset + 4], Len - 5);
        //if(CheckXOR != Rxbuf[offset+3]) return;

        memset(ssid, 0, sizeof(ssid));
        memset(passwd, 0, sizeof(passwd));
        offset += 4;
        if((Len - offset) < 7)
        {
            return Result;
        }

        MsgLen = calc_msg_length(&Rxbuf[offset], Len - offset);
        offset += 2;
        ssidLen = calc_msg_length(&Rxbuf[offset], Len - offset);
        offset += 2;
        if(ssidLen <= 0)
        {
            return Result;
        }

        if((Len - offset) > ssidLen)
        {
            memcpy(ssid, &Rxbuf[offset], ssidLen);
        }

        offset += ssidLen;
        keyLen = calc_msg_length(&Rxbuf[offset], Len - offset);
        if(keyLen < 0)
        {
            return Result;
        }

        offset += 2;
        if(keyLen > 0)
        {
            memcpy(passwd, &Rxbuf[offset], keyLen);
        }

        if(MsgLen == (ssidLen + keyLen))
        {
            ap_sta_target_wifi_station_connect(ssid, passwd);

            MsgLen = 5 + ssidLen + 1 + 4 + keyLen + 1;

            Result = os_malloc(sizeof(AP_STA_MSG_T) + MsgLen);
            if(Result)
            {
                Result->CloseAP = 1;
                Result->MsgLen = (short)(MsgLen & 0x7FFF);
                Result->ReturnMSG = 1;
                memcpy(Result->msg, "SSID:", 5);
                offset = 5;
                memcpy(&Result->msg[offset], ssid, ssidLen);
                offset += ssidLen;
                Result->msg[offset] = ';';
                offset += 1;
                memcpy(&Result->msg[offset], "Key:", 4);
                offset += 4;
                memcpy(&Result->msg[offset], passwd, keyLen);
                offset += keyLen;
                Result->msg[offset] = '\0';
            }
        }
    }
    return Result;
}

void ap_sta_tcp_client_connect_thread( beken_thread_arg_t arg )
{
    bk_err_t err = kNoErr;
    int fd = (int) arg;
    int len = 0;
    fd_set readfds;
    char *buf = NULL;
    struct timeval t;
    AP_STA_MSG_T *RtMsg;

    buf = (char *) os_malloc( 1024 );
    ASSERT(buf);

    t.tv_sec = 5;
    t.tv_usec = 0;

    while ( 1 )
    {
        FD_ZERO( &readfds );
        FD_SET( fd, &readfds );

        select( fd + 1, &readfds, NULL, NULL, &t);

        if ( FD_ISSET( fd, &readfds ) )
        {
            len = recv( fd, buf, 1024, 0 );

            if ( len == 0 )
            {
                os_printf( "TCP Client is disconnected, fd: %d\r\n", fd );
                goto exit;
            }

            RtMsg = ap_sta_analysis_message( buf, len);
            if(RtMsg)
            {
                len = send( fd, RtMsg->msg, RtMsg->MsgLen, 0 );
                if ( len == 0 )
                {
                    os_printf( "TCP Client is disconnected, fd: %d\r\n", fd );
                    goto exit;
                }
                if(RtMsg->CloseAP)
                {
                    os_printf("delete Softap!\r\n");
                    rtos_delay_milliseconds((TickType_t)3000);
                    ap_sta_stop_Softap();
                    goto exit;
                }
                os_free(RtMsg);
            }
        }
    }

exit:
    if ( err != kNoErr )
    {
        os_printf( "TCP client thread exit with err: %d\r\n", err );
    }

    if ( buf != NULL )
    {
        os_free( buf );
    }

    g_Clietfd = 0;
    close( fd );
    rtos_delete_thread( NULL );
}

void ap_sta_tcp_server_listener_thread( beken_thread_arg_t arg )
{
    (void)( arg );
    bk_err_t err = kNoErr;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sockaddr_t_size = sizeof(client_addr);
    char client_ip_str[16];
    int tcp_listen_fd = -1, client_fd = -1;
    fd_set readfds;

    ap_sta_softap_start();
    g_Clietfd = 0;

    tcp_listen_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( AP_STA_SERVER_PORT );

    err = bind( tcp_listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr) );

    err = listen( tcp_listen_fd, 0);

    while ( 1 )
    {
        FD_ZERO( &readfds );
        FD_SET( tcp_listen_fd, &readfds );

        select( tcp_listen_fd + 1, &readfds, NULL, NULL, NULL);

        if( FD_ISSET( tcp_listen_fd, &readfds ) )
        {
            client_fd = accept( tcp_listen_fd, (struct sockaddr *) &client_addr, &sockaddr_t_size );
            if( client_fd >= 0 )
            {
                if(g_Clietfd != 0)
                {
                    close( client_fd );
                    client_fd = -1;
                    continue;
                }

                g_Clietfd = client_fd;
                os_strcpy( client_ip_str, inet_ntoa( client_addr.sin_addr ) );
                os_printf( "TCP Client %s:%d connected, fd: %d\r\n", client_ip_str, client_addr.sin_port, client_fd );

                if( kNoErr != rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                                                  "TCP Clients",
                                                  (beken_thread_function_t)ap_sta_tcp_client_connect_thread,
                                                  0x800,
                                                  (beken_thread_arg_t)client_fd ))
                {
                    close( client_fd );
                    client_fd = -1;
                    g_Clietfd = 0;
                }
            }
        }
    }

    if ( err != kNoErr )
    {
        os_printf( "Server listerner thread exit with err: %d", err );
    }

    close( tcp_listen_fd );
    rtos_delete_thread( NULL );
}

int ap_sta_server_light_application_start(void)
{
    bk_err_t err = kNoErr;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "AP_STA_DEMO",
                              (beken_thread_function_t)ap_sta_tcp_server_listener_thread,
                              0x800,
                              (beken_thread_arg_t)0 );
    return err;
}

int demo_start(void)
{
    bk_err_t err = kNoErr;

    err = ap_sta_server_light_application_start();

    return err;
}

#endif /*AP_STA_DEMO*/

