#include "sl_tls.h"

#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"

extern void bk_printf(const char *fmt, ...);


MbedTLSSession * sl_ssl_handle_create(void)
{
	MbedTLSSession *tls_session  = TLS_NULL;
	
	tls_session = (MbedTLSSession *)tls_malloc(sizeof(MbedTLSSession));
	if(!tls_session) 
	{
		bk_printf("no memory for at_tls_session struct\n");
		return NULL;
	}
	memset(tls_session,0,sizeof(MbedTLSSession));
	
	return tls_session;
}

int sl_ssl_connect_create(MbedTLSSession *session)
{
	char *pers = "beken";
	int ret = 0;
	
	if((ret = mbedtls_client_init(session, (void *)pers, strlen(pers))) != 0)
	{
		bk_printf("MbedTLSClientInit err return : -0x%x\r\n", -ret);
		goto __exit;
	}

	if((ret = mbedtls_client_context(session)) < 0)
	{
		bk_printf("MbedTLSCLlientContext err return : -0x%x\r\n", -ret);
		goto __exit;
	}

	if((ret = mbedtls_client_connect(session)) != 0)
	{
		bk_printf("MbedTLSCLlientConnect err return : -0x%x\r\n", -ret);
		goto __exit;
	}

	return 0;

__exit:
	mbedtls_client_close(session);
	return -1;
}


MbedTLSSession * ssl_create(char *url,char *port)
{
	int ret;
	MbedTLSSession *tls_session = NULL;

	if(!url && !port)
		return NULL;
	
	bk_printf("[SSL]ssl create session,url:%s,port:%s\r\n",url,port);
	tls_session = sl_ssl_handle_create();
	if(tls_session == NULL)
	{
		bk_printf("[SSL]%s %d error\r\n",__FUNCTION__,__LINE__);
		return -1;
	}

	tls_session->host = os_strdup(url);  ///os_strdup(DEFAULT_MBEDTLS_WEB_SERVER);
	if(tls_session->host == NULL) 
	{
		bk_printf("no memory for tls_session->host malloc\n");
		goto err_exit;
	}
	
    tls_session->port = os_strdup(port);///os_strdup(DEFAULT_MBEDTLS_WEB_PORT);
	if(tls_session->port == NULL)
	{
		bk_printf("no memory for tls_session->port malloc\n");
		goto err_exit;
	}
	
	tls_session->buffer_len = DEFAULT_MBEDTLS_READ_BUFFER;
    tls_session->buffer = tls_malloc(tls_session->buffer_len);
	if(tls_session->buffer == NULL) 
	{
        bk_printf("no memory for tls_session->buffer malloc\n");
       	goto err_exit;
    }
	
	if(sl_ssl_connect_create(tls_session) != 0)
	{
		tls_session = NULL;
		goto error;
	}
	
	return tls_session;
err_exit:
	if((tls_session)&&(tls_session->host != NULL)) {
		tls_free(tls_session->host);
		tls_session->host = NULL;
	}
	if((tls_session)&&(tls_session->port != NULL)) {
		tls_free(tls_session->port);
		tls_session->port = NULL;
	}
	if((tls_session)&&(tls_session->buffer)) {
        tls_free(tls_session->buffer);
		tls_session->buffer = NULL;
    }
	if(tls_session){
		tls_free(tls_session);
		tls_session = NULL;
	}
	goto error;
	return 0;
error:
	return 0;
}

int ssl_txdat_sender(MbedTLSSession *tls_session,int len,char *data)
{
	int ret;
	int offset = 0,sd_len = 0,wr_len = 0;
	int max_len = 512;
	unsigned int cur_time,p_time;
        
    if( data && len) 
	{
		if( tls_session)
		{
		#if defined(MBEDTLS_SSL_MAX_FRAGMENT_LENGTH)
			max_len = mbedtls_ssl_get_max_frag_len( &tls_session->ssl );
			max_len = min(1024,max_len);
		#endif
			///bk_printf("[AS]frag len:%d,mlen:%d\r\n",max_len,len);
			sd_len = len;
			p_time = rtos_get_time();
			
			do
			{
				if((sd_len - offset) > max_len) 
				{
					wr_len = max_len;
				}
				else 
				{
					wr_len = (sd_len - offset);
				}
				
				cur_time = rtos_get_time();
				///bk_printf("[ssl]cur_time:%d ms\r\n",cur_time);
				if((cur_time - p_time) > (18 * 1000))
				{
					bk_printf("[%d]ssl send timeout\r\n",__LINE__);
					goto retu;
				}
				
				while((ret = mbedtls_client_write(tls_session, (unsigned char*)(data + offset), wr_len)) <= 0)
				{
					if(MBEDTLS_ERR_NET_SEND_FAILED == ret)
					{
						bk_printf("mbedtls_client_write fialed\r\n");
						goto retu;
					}
					if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
					{
						bk_printf("mbedtls_ssl_write returned -0x%x\r\n", -ret);
						goto __exit;
					}
					rtos_delay_milliseconds(10);
				}
				offset += (ret > 0)?ret:0;
			}while(offset < len);
		retu:
			return offset;
		}
	}
	
	return -1;
 __exit:
 	return -1;
}

int ssl_read_data(MbedTLSSession *session,unsigned char *msg,unsigned int mlen,unsigned int timeout_ms)
{
	int len;
	int ret;
	fd_set readfds; 
	struct timeval timeout;
	do
	{
		if(mbedtls_ssl_get_bytes_avail(&session->ssl) == 0)
		{
		#if 1
			FD_ZERO( &readfds );
		    FD_SET( session->server_fd.fd, &readfds );

			if(timeout_ms < 0xFFFFFFFFU)
			{
				if(timeout_ms != 0)
				{
					timeout.tv_sec = timeout_ms/1000;
					timeout.tv_usec = (timeout_ms%1000) * 1000;
				}
				else
				{
					timeout.tv_sec = 0;
					timeout.tv_usec = 1000;
				}
			}
			else
			{
				timeout.tv_sec = 0;
				timeout.tv_usec = 0;
			}
			ret = select( session->server_fd.fd+1, &readfds, NULL, NULL, &timeout);
			if( ret <= -1 ) 
			{  ///-1£º³ö´í
		        bk_printf("[TLS]select error:%d\r\n",ret); 
		        return -1;
		    }
			else if( 0 == ret ) 
			{  
				///0 £º³¬Ê±
				///bk_printf("[TLS]read select timeout(%d)\r\n",timeout_ms); 
		        return 0;
		    }
			if (!FD_ISSET( session->server_fd.fd, &readfds ) ) /*one client has data*/
		    {
				return 0;
			}
		#endif
		}
		if(mlen == 0)
		{
			return 0;
		}
		len = (session->buffer_len > mlen)?mlen:session->buffer_len;
		ret = mbedtls_client_read(session, (unsigned char *)session->buffer, len);
		if(ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
		{
			return 0;
		}
		
		if(ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
		{
			bk_printf("[AS]MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY\r\n");
			return -1;
		}
		
		if(ret == 0) 
		{
			bk_printf("[AS]connection closed\n");
			return -1;
		}

		if(ret < 0) 
		{
			bk_printf("[AS]mbedtls_ssl_read returned -0x%x\r\n", -ret);
			return -1;
		}
	}while(0);
	len = ret;
	memcpy(msg,session->buffer,len);
	
	return len;
}

int ssl_close(MbedTLSSession *session)
{
	mbedtls_client_close(session);
}

