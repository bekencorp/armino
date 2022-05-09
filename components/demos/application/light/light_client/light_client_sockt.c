#include "light_client_sockt.h"
#include <common/bk_include.h>

#if LIGHT_CLIENT_APP_DEMO
#include "sockets.h"
#include <os/os.h>
#include "Error.h"
#include "rw_pub.h"
#include "compiler.h"
#include <common/bk_list.h>
#include "light_commun_protocol.h"

typedef struct
{
    char ip4[16];
    unsigned short port;
} remote_server_addr_T;

typedef struct
{
    LIST_HEADER_T list;
    int fd;
} light_remote_server_socket_fd_T;

beken_timer_t *tcp_connect_timer = NULL;
LIST_HEADER_T remote_server_socket_node_list = {&remote_server_socket_node_list, &remote_server_socket_node_list};

int light_tcp_connect_server_fd_register(int fd)
{
    light_remote_server_socket_fd_T *LsckHdl;

    LsckHdl = (light_remote_server_socket_fd_T *) os_malloc(sizeof(light_remote_server_socket_fd_T));
    if(LsckHdl)
    {
        LsckHdl->fd = fd;
        list_add_head(&LsckHdl->list, &remote_server_socket_node_list);
    }
    else
    {
        os_printf( "add node failed");
        return t_error;
    }

    return t_ok;
}

int light_tcp_connect_server_fd_unregister(int fd)
{
    light_remote_server_socket_fd_T *LsckHdl;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &remote_server_socket_node_list)
    {
        LsckHdl = list_entry(pos, light_remote_server_socket_fd_T, list);
        if(LsckHdl->fd == fd)
        {
            list_del(pos);
            os_free(LsckHdl);
        }
    }

    return t_ok;
}

int light_tcp_connect_fd_check(int fd)
{
    light_remote_server_socket_fd_T *LsckHdl;
    LIST_HEADER_T *pos, *n;

    list_for_each_safe(pos, n, &remote_server_socket_node_list)
    {
        LsckHdl = list_entry(pos, light_remote_server_socket_fd_T, list);
        if(LsckHdl->fd == fd)
        {
            return fd;
        }
    }

    return t_error;
}


int tcp_client_create_new_socket(void)
{
    int socket_fd = -1;

    socket_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    return socket_fd;
}

int tcp_client_get_remote_server_addr(struct sockaddr_in *svr_addr, remote_server_addr_T **rm_svr_addr)
{
    ip4_addr_t remote_ip;

    if((!rm_svr_addr) || (!(*rm_svr_addr)))
        return t_error;

    if(!inet_aton((*rm_svr_addr)->ip4, &remote_ip.addr))
    {
        os_printf( "server ip err\r\n" );
        return t_error;
    }

    svr_addr->sin_family = AF_INET;
    svr_addr->sin_addr.s_addr = remote_ip.addr;  //such as 0x0728a8c0 == "192.168.40.7"
    svr_addr->sin_port = htons((*rm_svr_addr)->port);
    if((*rm_svr_addr))
    {
        os_free((*rm_svr_addr));
        *rm_svr_addr = NULL;
    }

    return t_ok;
}

int tcp_client_connect_remote_server(int skt_fd, struct sockaddr_in *svr_addr)
{
    int err = -1;

    err = connect(skt_fd, (struct sockaddr *)svr_addr, sizeof(struct sockaddr));
    if(!err)
    {
        os_printf( "connect ok\r\n");
        return t_ok;
    }

    os_printf( "connect err: %d\r\n", err );
    return t_error;
}

int tcp_client_receive_remote_server_data(int skt_fd)
{
    int len;
    char *buf = NULL;

    buf = (char *) os_malloc( 1024 );
    if(buf == NULL)
    {
        os_printf( "client malloc failed\r\n");
        goto exit;
    }

    light_tcp_connect_server_fd_register(skt_fd);
    protocol_serial_number_register(skt_fd | CLIENT_SOCKET_MSG, 0xFF, 0xFF);

    while ( 1 )
    {
        len = recv(skt_fd, buf, 1024, 0 );
        if ( len <= 0 )
        {
            os_printf( "tcp is disconnected, fd: %d\r\n", skt_fd );
            goto exit;
        }
        light_client_input_msg_sender(skt_fd | CLIENT_SOCKET_MSG, buf, len);
    }

    light_tcp_connect_server_fd_unregister(skt_fd);
    protocol_msg_unregister(skt_fd | CLIENT_SOCKET_MSG);

exit:
    if(buf != NULL)
    {
        os_free( buf );
    }
    return t_error;
}

int tcp_client_send_remote_server_data(int skt_fd, char *buf, int len)
{
    int ret;
    struct lwip_sock *sock;

    if (skt_fd < 0)
    {
        return t_error;
    }

    if (len <= 0)
    {
        return t_ok;
    }

    if(light_tcp_connect_fd_check(skt_fd) == skt_fd)
    {
        ret = send( skt_fd, buf, len, 0 );
        if ( ret < 0 )
        {
            light_tcp_connect_server_fd_unregister(skt_fd);
            os_printf( "fd(%d) disconnected\r\n", skt_fd );
            return t_error;
        }
    }
    else
    {
        os_printf( "fd(%d) disconnected\r\n", skt_fd );
        return t_error;
    }
    return t_ok;
}

int light_check_wifi_got_ip(void)
{
    msg_sta_states sta_sts_prv;

    sta_sts_prv =  mhdr_get_station_status();

    if(sta_sts_prv == MSG_GOT_IP)
    {
        return t_ok;
    }

    return t_error;
}

int light_tcp_connect_keep_active(int skt_fd)
{
    int opt = 1;
    int ret = 0;

    // open tcp keepalive
    ret = setsockopt(skt_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int));
    if(ret < 0)
    {
        return t_error;
    }

    opt = 5;  // 5 second
    ret = setsockopt(skt_fd, IPPROTO_TCP, TCP_KEEPIDLE, &opt, sizeof(int));
    if(ret < 0)
    {
        return t_error;
    }

    opt = 1;  // 1s second for intval
    ret = setsockopt(skt_fd, IPPROTO_TCP, TCP_KEEPINTVL, &opt, sizeof(int));
    if(ret < 0)
    {
        return t_error;
    }

    opt = 3;  // 3 times
    ret = setsockopt(skt_fd, IPPROTO_TCP, TCP_KEEPCNT, &opt, sizeof(int));
    if(ret < 0)
    {
        return t_error;
    }

    return t_ok;
}

void tcp_connect_remote_server_thread( beken_thread_arg_t arg )
{
    bk_err_t err = t_error;
    struct sockaddr_in server_addr;
    int socket_fd = -1;
    remote_server_addr_T *s_add = (remote_server_addr_T *)arg;

    socket_fd = tcp_client_create_new_socket();
    if(socket_fd < 0)
    {
        os_printf("socket failed\r\n");
        goto exit;
    }

    err = tcp_client_get_remote_server_addr(&server_addr, &s_add);
    if(t_ok != err)
    {
        goto exit;
    }

    err = tcp_client_connect_remote_server(socket_fd, &server_addr);
    if(t_ok != err)
    {
        goto exit;
    }

    err = light_tcp_connect_keep_active(socket_fd);
    if(t_ok != err)
    {
        os_printf( "socket keep active error\r\n");
        goto exit;
    }

    tcp_client_receive_remote_server_data(socket_fd);

exit:
    rtos_start_timer(tcp_connect_timer);
    if ( err != kNoErr )
    {
        os_printf( "light tcp connect server thread exit!\r\n" );
    }

    if(socket_fd > 0)
    {
        close( socket_fd );
    }
    rtos_delete_thread( NULL );
}

void light_start_connect_remote_server(void)
{
    bk_err_t err = kNoErr;
    remote_server_addr_T *s_add = NULL;

    s_add = (remote_server_addr_T *)os_malloc(sizeof(remote_server_addr_T));
    if(s_add == NULL)
    {
        os_printf("s_add malloc failed\r\n");
        goto exit;
    }

    memcpy(s_add->ip4, LIGHT_REMOTE_SERVER_IP, sizeof(LIGHT_REMOTE_SERVER_IP));
    s_add->port = LIGHT_REMOTE_SERVER_PORT;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "tcp_clt_cnct",
                              (beken_thread_function_t)tcp_connect_remote_server_thread,
                              0x800,
                              (beken_thread_arg_t)s_add);
    if(kNoErr != err )
    {
        os_printf("tcp_connect_remote_server_thread failed\r\n");
        goto exit;
    }
    return;

exit:
    rtos_start_timer(tcp_connect_timer);
}

void tcp_connect_timer_destroy( void )
{
    rtos_deinit_timer( tcp_connect_timer );
    if(tcp_connect_timer)
    {
        os_free(tcp_connect_timer);
        tcp_connect_timer = NULL;
    }
}

void tcp_connect_timer_alarm( void *arg )
{
    if(light_check_wifi_got_ip() == t_ok)
    {
        rtos_stop_timer( tcp_connect_timer );
        light_start_connect_remote_server();
    }
}

int create_light_client_check_net_timer(void)
{
    bk_err_t err;
    if(tcp_connect_timer == NULL)
    {
        tcp_connect_timer = (beken_timer_t *) os_malloc( sizeof(beken_timer_t) );
    }

    if(tcp_connect_timer == NULL)
    {
        os_printf("tcp_connect_timer error!\r\n");
    }

    err = rtos_init_timer(tcp_connect_timer, 1000, tcp_connect_timer_alarm, 0);
    if(kNoErr != err)
    {
        if(tcp_connect_timer)
        {
            os_free(tcp_connect_timer);
            tcp_connect_timer = NULL;
        }
        os_printf("tcp_connect_timer error!\r\n");
        return t_error;
    }
    return t_ok;
}

int light_net_connect_start(void)
{
    bk_err_t err = kNoErr;

    err = create_light_client_check_net_timer();
    if(t_ok != err)
    {
        if(tcp_connect_timer)
        {
            os_free(tcp_connect_timer);
            tcp_connect_timer = NULL;
        }
        os_printf("tcp_connect_timer error!\r\n");

        return t_error;
    }

    err = rtos_start_timer(tcp_connect_timer);
    if(kNoErr != err)
    {
        tcp_connect_timer_destroy();
        os_printf("tcp_connect_timer start failed!\r\n");

        return t_error;
    }

    return t_ok;
}

#endif
/// EOF

