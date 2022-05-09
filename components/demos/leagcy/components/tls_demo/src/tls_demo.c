#include "sl_tls.h"

#include "wlan_cli_pub.h"
#include <os/os.h>
#include "sockets.h"

#include "string.h"
#include "stdio.h"

extern void bk_printf(const char *fmt, ...);

static MbedTLSSession *tls_demo_session = NULL;

static void tls_demo_ssl_client_main( UINT32 data )
{
	MbedTLSSession *session = (MbedTLSSession *)data;
	int ret,i,len;
	fd_set readfds; 
	fd_set exceptset;
	struct timeval timeout;

	bk_printf("-------------------  %s  -------------------\r\n",__FUNCTION__);
	
	if(session->host)
	{
        tls_free(session->host);
		session->host = TLS_NULL;
    }
    
    if(session->port)
	{
        tls_free(session->port);
		session->port = TLS_NULL;
    }
	
	do
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = (50 * 1000);
		
		FD_ZERO( &readfds );
        FD_SET( session->server_fd.fd, &readfds );
		FD_ZERO( &exceptset );
		FD_SET( session->server_fd.fd, &exceptset );
		
		ret = select( session->server_fd.fd+1, &readfds, NULL, &exceptset, &timeout);
		if( ret <= -1 ) 
		{  ///-1£º³ö´í
            bk_printf("[TLS]select error:%d\r\n",ret); 
            goto __exit;
        }
		else if( 0 == ret ) 
		{  
			///0 £º³¬Ê±
            continue;
        }
		if ( FD_ISSET( session->server_fd.fd, &readfds ) ) /*one client has data*/
        {
        	///memset(session->buffer, 0x00, session->buffer_len);
			ret = mbedtls_client_read(session, (unsigned char *)session->buffer, session->buffer_len);
			if(ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			{
				continue;
			}
			
			if(ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
			{
				bk_printf("[AS]MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY\r\n");
				break;
			}
			
			if(ret == 0) 
			{
				bk_printf("[AS]connection closed\n");
				break;
			}

			if(ret < 0) 
			{
				bk_printf("[AS]mbedtls_ssl_read returned -0x%x\r\n", -ret);
				break;
			}
			len = ret;
		#if 1
			bk_printf("[TLS]ssl_RX(%d):\r\n",len);
			for(i = 0; i<len; i++)
			{
				bk_printf("%c", session->buffer[i]);
			}
			bk_printf("\r\n");
		#else
			bk_printf("[TLS]ssl_RX(%d):\r\n",len);
			for(i = 0; i<len; i++)
			{
				bk_printf("%x", session->buffer[i]);
			}
			bk_printf("\r\n");
		#endif
		}
		
		if(( FD_ISSET( session->server_fd.fd, &exceptset )) )
        {
        	bk_printf( "[TLS]client fd(%d) exceptset\r\n", session->server_fd.fd );
            goto __exit;
        }
	}while(1);
	
__exit:
	mbedtls_client_close(session);
	session = NULL;
	tls_demo_session = NULL;
	bk_printf("\r\n[TLS demo main]exit   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	
	rtos_delete_thread(NULL);
}


#define DEFAULT_MBEDTLS_WEB_SERVER  "www.baidu.com"
#define DEFAULT_MBEDTLS_WEB_PORT    "443"
#define DEFAULT_MBEDTLS_WEB_URL     "https://www.baidu.com/"

static const char *HTTP_REQUEST = "GET " DEFAULT_MBEDTLS_WEB_URL " HTTP/1.0\r\n"
    "Host: "DEFAULT_MBEDTLS_WEB_SERVER"\r\n"
    "User-Agent: bekencorp/1.0 BK7231s\r\n"
    "\r\n";

void tls_demo_ssl_send_handler(MbedTLSSession *tls_session, int len, char *dat)
{
	int length;
	unsigned char buff[512];

	if(tls_session == NULL)
		return;
	
	if(dat == NULL)
	{
		strcpy(buff,HTTP_REQUEST);
		dat = buff;
		length = strlen(buff);
	}
	else
	{
		length = len;
	}

	if(ssl_txdat_sender(tls_session,length,dat) > 0)
	{
		bk_printf("[TLS]send OK\r\n");
	}
	else
	{
		bk_printf("[TLS]send failed\r\n");
	}
}

static void tls_demo_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;
	int flag = 0;
	char *url = "www.baidu.com";
	char *port = "443";
	int len;
	char *dat = NULL;
	MbedTLSSession *tls_session = NULL;

	if(argc >= 2)
	{
		if(strcmp(argv[1], "sender") == 0)
		{
			if(argc > 2)
			{
				dat = argv[argc - 1];
				len = strlen(dat);
			}
			else
			{
				len = 0;
			}
			
			flag = 3;
		}
		else if(strcmp(argv[1], "create") == 0)
		{
			flag = 2;
			if(argc >= 3)
			{
				url = argv[2];
			}
			if(argc >= 4)
			{
				port = argv[3];
			}
		}
	}

	if(flag == 2 && tls_demo_session == NULL)  ///create
	{
		tls_session = ssl_create(url,port);
		if(tls_session != NULL)
		{
			ret = rtos_create_thread(NULL,
			                     BEKEN_DEFAULT_WORKER_PRIORITY,
			                     "at_ssl",
			                     (beken_thread_function_t)tls_demo_ssl_client_main,
			                     (1024 * 4),
			                     (void*)tls_session);
			if (ret != kNoErr)
			{
				bk_printf("Error: Failed to create at_ssl thread: %d\r\n",ret);
				mbedtls_client_close(tls_session);
				return;
		    }
			tls_demo_session = tls_session;
		}
		else
		{
			bk_printf("ssl build failed\r\n");
		}
	}
	else if(flag == 3 && tls_demo_session != NULL)  ///send
	{
		tls_demo_ssl_send_handler(tls_demo_session,len,dat);
	}
}

static const struct cli_command tls_cli_cmd[] =
{
    {"tls", "tls", tls_demo_command},
};

void app_demo_init(void)
{
	bk_printf("------>%s:%d\r\n",__FUNCTION__,__LINE__);
	
	cli_register_commands(tls_cli_cmd, sizeof(tls_cli_cmd) / sizeof(struct cli_command));


}













