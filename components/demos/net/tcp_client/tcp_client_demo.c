#include "tcp_client_demo.h"
#include <common/bk_include.h>

#if TCP_CLIENT_DEMO
#include <common/bk_include.h>
#include <os/os.h>
#include <common/bk_list.h>
#include <os/mem.h>
#include <os/str.h>
#include "lwip/sockets.h"
#include "Error.h"
#include "uart_pub.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"
#include "Common.h"

struct ip_port_msg
{
    ip4_addr_t IP;
    in_port_t  sin_port;
};

int num_string2uint16num(char *Numstring)
{
    int len;
    unsigned int Num = 0;
    unsigned int j, temp;
    unsigned int i = 0;

    len = os_strlen(Numstring);
    if((len) && (len < 6))
    {
        for(i = 0; i < len; i++)
        {
            if((Numstring[i] >= '0') && (Numstring[i] <= '9'))
            {
                temp = Numstring[i] - '0';
                for(j = 0; j < (len - 1 - i); j++)
                    temp *= 10;
                Num += temp;
            }
            else
            {
                return -1;
            }
        }
    }
    return (int)Num;
}

void tcp_connect_server_thread( beken_thread_arg_t arg )
{
    int SkFd = -1;
    int len = 0;
    char *buf = NULL;
    bk_err_t err = kNoErr;
    struct ip_port_msg *i_pMsg;
    struct sockaddr_in server_addr;
    socklen_t sockaddr_t_size = sizeof(server_addr);

    i_pMsg = (struct ip_port_msg *)arg;
    if(i_pMsg == NULL)
    {
        os_printf( "arg err!\r\n" );
        goto exit;
    }

    SkFd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = i_pMsg->IP.addr;    //0x0728a8c0 /*192.168.40.7*/
    server_addr.sin_port = htons(i_pMsg->sin_port);   //htons( 12345 );

    if(i_pMsg)
    {
        os_free(i_pMsg);
    }

    err = connect(SkFd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(err < 0)
    {
        os_printf( "connect err: %d\r\n", err );
        goto exit;
    }

    buf = (char *) os_malloc( 1024 );
    if(buf == NULL)
    {
        os_printf( "buf == NULL\r\n");
        goto exit;
    }

    while ( 1 )
    {
        len = recv(SkFd, buf, 1024, 0 );

        if ( len <= 0 )
        {
            os_printf( "TCP Client is disconnected, fd: %d\r\n", SkFd );
            goto exit;
        }

        len = send( SkFd, buf, len, 0 );
    }

exit:

    if ( err != kNoErr )
    {
        os_printf( "tcp_connect_server_thread exit with err: %d\r\n", err );
    }

    if ( buf != NULL )
    {
        os_free( buf );
    }

    close( SkFd );
    rtos_delete_thread( NULL );
}

/*****************************************************************
please add this function on struct cli_command built_ins[] of wlan_cli.c files.
such as,
static const struct cli_command built_ins[] =
{
...
//example
    {"tcp_client", "tcp_client ip port", tcp_make_connect_server_command},
    .....
 };
******************************************************************/
void tcp_make_connect_server_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    bk_err_t err = kNoErr;
    struct ip_port_msg *i_pMsg = NULL;
    int i;
    int port;
    i_pMsg = (struct ip_port_msg *) os_malloc( sizeof(struct ip_port_msg) );

    if(i_pMsg == NULL)
    {
        os_printf("i_pMsg Failed\r\n");
        return;
    }

    if(!inet_aton(argv[1], &i_pMsg->IP.addr))
    {
        os_printf("inet_aton Failed\r\n");
        goto exit;
    }

    port = num_string2uint16num(argv[2]);
    if(port < 0)
    {
        os_printf("port err:%d\r\n", port);
    }

    i_pMsg->sin_port = port;

    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "connt_Server",
                              tcp_connect_server_thread,
                              0x600,
                              i_pMsg );
    if(kNoErr != err)
    {
        os_printf("rtos_create_thread Failed(%d)\r\n", err);
        goto exit;
    }

    return;

exit:

    if(i_pMsg)
    {
        os_free(i_pMsg);
    }
}

#endif
// eof

