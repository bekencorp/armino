#ifndef _SL_TLS_H_
#define _SL_TLS_H_


#include "tls_rtos.h"
#include "tls_client.h"
#include "tls_certificate.h"
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif





#define DEFAULT_MBEDTLS_READ_BUFFER               (1024U)








extern MbedTLSSession * ssl_create(char *url,char *port);
extern int ssl_txdat_sender(MbedTLSSession *tls_session,int len,char *data);
extern int ssl_read_data(MbedTLSSession *session,unsigned char *msg,unsigned int mlen,unsigned int timeout_ms);
extern int ssl_close(MbedTLSSession *session);

#endif
