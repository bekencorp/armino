// Copyright 2021-2022 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lwip/sockets.h>
#include "mbedtls/ssl.h"
#include "mbedtls/x509_crl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/net_sockets.h"
#include <netdb.h>
#include <FreeRTOSConfig.h>
#include <mbedtls/base64.h>
#include "FreeRTOS.h"
#include "task.h"

#define CONFIG_MBEDTLS_CERTIFICATE_BUNDLE

typedef struct bk_tls_pki_t {
	mbedtls_x509_crt *public_cert;
	mbedtls_pk_context *pk_key;
	const unsigned char *publiccert_pem_buf;
	unsigned int publiccert_pem_bytes;
	const unsigned char *privkey_pem_buf;
	unsigned int privkey_pem_bytes;
	const unsigned char *privkey_password;
	unsigned int privkey_password_len;
} bk_tls_pki_t;

typedef enum bk_tls_role {
	BK_TLS_CLIENT = 0,
	BK_TLS_SERVER,		   /* not support */
} bk_tls_role_t;
	
typedef enum bk_tls_conn_state {
	BK_TLS_INIT = 0,
	BK_TLS_CONNECTING,
	BK_TLS_HANDSHAKE,
	BK_TLS_FAIL,
	BK_TLS_DONE,
} bk_tls_conn_state_t;

#if 0 /* not set */
typedef enum {
	BK_TLS_ERR_TYPE_UNKNOWN = 0,
	BK_TLS_ERR_TYPE_SYSTEM, 			   /*!< System error -- errno */
	BK_TLS_ERR_TYPE_MBEDTLS,			   /*!< Error code from mbedTLS library */
	BK_TLS_ERR_TYPE_MBEDTLS_CERT_FLAGS,    /*!< Certificate flags defined in mbedTLS */
	BK_TLS_ERR_TYPE_BK, 				  /*!< BK-IDF error type -- bk_err_t  */
	BK_TLS_ERR_TYPE_MAX,				   /*!< Last err type -- invalid entry */
} bk_tls_error_type_t;
#endif

#define BK_TLS_ERR_SSL_WANT_READ						  MBEDTLS_ERR_SSL_WANT_READ
#define BK_TLS_ERR_SSL_WANT_WRITE						  MBEDTLS_ERR_SSL_WANT_WRITE
#define BK_TLS_ERR_SSL_TIMEOUT							  MBEDTLS_ERR_SSL_TIMEOUT

#define BK_ERR_BK_TLS_BASE			 0x8000 			/*!< Starting number of BK-TLS error codes */

/* generic bk-tls error codes */
#define BK_ERR_BK_TLS_CANNOT_RESOLVE_HOSTNAME			(BK_ERR_BK_TLS_BASE + 0x01)  /*!< Error if hostname couldn't be resolved upon tls connection */
#define BK_ERR_BK_TLS_CANNOT_CREATE_SOCKET				(BK_ERR_BK_TLS_BASE + 0x02)  /*!< Failed to create socket */
#define BK_ERR_BK_TLS_UNSUPPORTED_PROTOCOL_FAMILY		(BK_ERR_BK_TLS_BASE + 0x03)  /*!< Unsupported protocol family */
#define BK_ERR_BK_TLS_FAILED_CONNECT_TO_HOST			(BK_ERR_BK_TLS_BASE + 0x04)  /*!< Failed to connect to host */
#define BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED				(BK_ERR_BK_TLS_BASE + 0x05)  /*!< failed to set/get socket option */
#define BK_ERR_BK_TLS_CONNECTION_TIMEOUT				(BK_ERR_BK_TLS_BASE + 0x06)  /*!< new connection in bk_tls_low_level_conn connection timeouted */
#define BK_ERR_BK_TLS_SE_FAILED 						(BK_ERR_BK_TLS_BASE + 0x07)  /*< bk-tls use Secure Element returned failed */
#define BK_ERR_BK_TLS_TCP_CLOSED_FIN					(BK_ERR_BK_TLS_BASE + 0x08)  /*< bk-tls's TPC transport connection has benn closed (in a clean way) */

/* mbedtls specific error codes */
#define BK_ERR_MBEDTLS_CERT_PARTLY_OK					 (BK_ERR_BK_TLS_BASE + 0x10)
#define BK_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED 			 (BK_ERR_BK_TLS_BASE + 0x11)
#define BK_ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED			 (BK_ERR_BK_TLS_BASE + 0x12)
#define BK_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED		 (BK_ERR_BK_TLS_BASE + 0x13)
#define BK_ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED	 (BK_ERR_BK_TLS_BASE + 0x14)
#define BK_ERR_MBEDTLS_X509_CRT_PARSE_FAILED			 (BK_ERR_BK_TLS_BASE + 0x15)
#define BK_ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED 		 (BK_ERR_BK_TLS_BASE + 0x16)
#define BK_ERR_MBEDTLS_SSL_SETUP_FAILED 				 (BK_ERR_BK_TLS_BASE + 0x17)
#define BK_ERR_MBEDTLS_SSL_WRITE_FAILED 				 (BK_ERR_BK_TLS_BASE + 0x18)
#define BK_ERR_MBEDTLS_PK_PARSE_KEY_FAILED				 (BK_ERR_BK_TLS_BASE + 0x19)
#define BK_ERR_MBEDTLS_SSL_HANDSHAKE_FAILED 			 (BK_ERR_BK_TLS_BASE + 0x1A)
#define BK_ERR_MBEDTLS_SSL_CONF_PSK_FAILED				 (BK_ERR_BK_TLS_BASE + 0x1B)
#define BK_ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED			 (BK_ERR_BK_TLS_BASE + 0x1C)


typedef struct bk_tls {
	mbedtls_ssl_context ssl;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_config conf;
	mbedtls_net_context server_fd;
	mbedtls_x509_crt cacert;
	mbedtls_x509_crt *cacert_ptr;
	mbedtls_x509_crt clientcert;
	mbedtls_pk_context clientkey;																		
	int sockfd;
	ssize_t (*read)(struct bk_tls  *tls, char *data, size_t datalen);
	ssize_t (*write)(struct bk_tls *tls, const char *data, size_t datalen);
	bk_tls_conn_state_t  conn_state;
	fd_set rset;
	fd_set wset;
	bool is_tls;
	bk_tls_role_t role;
} bk_tls_t;

typedef struct psk_key_hint {
	const uint8_t* key; 					/*!< key in PSK authentication mode in binary format */
	const size_t   key_size;				/*!< length of the key */
	const char* hint;						/*!< hint in PSK authentication mode in string format */
} psk_hint_key_t;

#ifdef CONFIG_BK_TLS_CLIENT_SESSION_TICKETS
typedef struct bk_tls_client_session {
	mbedtls_ssl_session saved_session;
} bk_tls_client_session_t;
#endif

typedef struct bk_transport_keepalive {
	bool			keep_alive_enable;
	int 			keep_alive_idle;		/* (second) */
	int 			keep_alive_interval;	/* (second) */
	int 			keep_alive_count;		/* retry send count */
} bk_transport_keep_alive_t;

typedef struct tls_keep_alive_cfg {
	bool keep_alive_enable;
	int keep_alive_idle;
	int keep_alive_interval;
	int keep_alive_count;
} tls_keep_alive_cfg_t;

typedef struct bk_tls_cfg {
	const char **alpn_protos;

	union {
	const unsigned char *cacert_buf;
	const unsigned char *cacert_pem_buf;
	};

	union {
	unsigned int cacert_bytes;
	unsigned int cacert_pem_bytes;
	};

	union {
	const unsigned char *clientcert_buf;
	const unsigned char *clientcert_pem_buf;
	};

	union {
	unsigned int clientcert_bytes;
	unsigned int clientcert_pem_bytes;
	};

	union {
	const unsigned char *clientkey_buf;
	const unsigned char *clientkey_pem_buf;
	};

	union {
	unsigned int clientkey_bytes;
	unsigned int clientkey_pem_bytes;
	};
	const unsigned char *clientkey_password;
	unsigned int clientkey_password_len;
	bool non_block;
	bool use_secure_element;
	int timeout_ms;
	bool use_global_ca_store;
	const char *common_name;
	bool skip_common_name;
	tls_keep_alive_cfg_t *keep_alive_cfg;
	const psk_hint_key_t* psk_hint_key;
	int (*crt_bundle_attach)(void *conf);
	void *ds_data;
	bool is_plain_tcp;
	struct ifreq *if_name;
#ifdef CONFIG_BK_TLS_CLIENT_SESSION_TICKETS
	bk_tls_client_session_t *client_session;
#endif
} bk_tls_cfg_t;

typedef enum {
	TRANS_SSL_INIT = 0,
	TRANS_SSL_CONNECTING,
} transport_ssl_conn_state_t;

typedef struct transport_bk_tls {
	bk_tls_t				*tls;
	bk_tls_cfg_t			cfg;
	bool					 ssl_initialized;
	transport_ssl_conn_state_t conn_state;
	int 					 sockfd;
} transport_bk_tls_t;

int ssl_read(transport_bk_tls_t *ssl, char *buffer, int len, int timeout_ms);
int ssl_write(transport_bk_tls_t *ssl, const char *buffer, int len, int timeout_ms);
int ssl_connect(transport_bk_tls_t *ssl, const char *host, int port, int timeout_ms);
int ssl_base_close(transport_bk_tls_t *ssl);

void bk_transport_ssl_set_interface_name(transport_bk_tls_t *ssl, struct ifreq *if_name);
void bk_transport_ssl_enable_global_ca_store(transport_bk_tls_t *ssl);
void bk_transport_ssl_set_psk_key_hint(transport_bk_tls_t *ssl, const psk_hint_key_t* psk_hint_key);
void bk_transport_ssl_set_cert_data(transport_bk_tls_t *ssl, const char *data, int len);
void bk_transport_ssl_set_cert_data_der(transport_bk_tls_t *ssl, const char *data, int len);
void bk_transport_ssl_set_client_cert_data(transport_bk_tls_t *ssl, const char *data, int len);
void bk_transport_ssl_set_client_cert_data_der(transport_bk_tls_t *ssl, const char *data, int len);
void bk_transport_ssl_set_client_key_data(transport_bk_tls_t *ssl, const char *data, int len);
void bk_transport_ssl_set_client_key_password(transport_bk_tls_t *ssl, const char *password, int password_len);
void bk_transport_ssl_set_client_key_data_der(transport_bk_tls_t *ssl, const char *data, int len);
void bk_transport_ssl_set_keep_alive(transport_bk_tls_t *ssl, bk_transport_keep_alive_t *keep_alive_cfg);
void bk_transport_ssl_crt_bundle_attach(transport_bk_tls_t *ssl, bk_err_t ((*crt_bundle_attach)(void *conf)));
void bk_transport_ssl_skip_common_name_check(transport_bk_tls_t *ssl);

