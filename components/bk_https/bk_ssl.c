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

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#ifdef CONFIG_HTTPS
#include <bk_ssl.h>
#include <portmacro.h>
#include "mbedtls/ssl.h"
#include "mbedtls/x509_crl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/net_sockets.h"
#include <http_parser.h>
#include <http_header.h>
#include <http_utils.h>
#include <components/system.h>
#include <bk_rom_md5.h>
#define TAG "SSL"

/*-------------------------------mbedtls--------------------------------------*/
#define _bk_create_ssl_handle			   bk_create_mbedtls_handle
#define _bk_tls_handshake				   bk_mbedtls_handshake
#define _bk_tls_read					   bk_mbedtls_read
#define _bk_tls_write					   bk_mbedtls_write
#define _bk_tls_conn_delete 			   bk_mbedtls_conn_delete
#define _bk_tls_net_init				   bk_mbedtls_net_init
#define _bk_tls_get_bytes_avail 		   bk_mbedtls_get_bytes_avail

static mbedtls_x509_crt *global_cacert = NULL;

ssize_t bk_mbedtls_get_bytes_avail(bk_tls_t *tls)
{
	if (!tls) {
		BK_LOGE(TAG, "empty arg passed to bk_tls_get_bytes_avail()");
		return BK_FAIL;
	}
	return mbedtls_ssl_get_bytes_avail(&tls->ssl);
}

void bk_mbedtls_cleanup(bk_tls_t *tls)
{
	if (!tls) {
		return;
	}
	if (tls->cacert_ptr != global_cacert) {
		mbedtls_x509_crt_free(tls->cacert_ptr);
	}
	tls->cacert_ptr = NULL;
	mbedtls_x509_crt_free(&tls->cacert);
	mbedtls_x509_crt_free(&tls->clientcert);
	mbedtls_pk_free(&tls->clientkey);
	mbedtls_entropy_free(&tls->entropy);
	mbedtls_ssl_config_free(&tls->conf);
	mbedtls_ctr_drbg_free(&tls->ctr_drbg);
	mbedtls_ssl_free(&tls->ssl);
}

static bk_err_t set_global_ca_store(bk_tls_t *tls)
{
	if (global_cacert == NULL) {
		BK_LOGE(TAG, "global_cacert is NULL\r\n");
		return -1;
	}
	tls->cacert_ptr = global_cacert;
	mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_ca_chain(&tls->conf, tls->cacert_ptr, NULL);
	return BK_OK;
}
static bk_err_t set_ca_cert(bk_tls_t *tls, const unsigned char *cacert, size_t cacert_len)
{
	tls->cacert_ptr = &tls->cacert;
	mbedtls_x509_crt_init(tls->cacert_ptr);
	BK_LOGE(TAG, "mbedtls_x509_crt_parse cacert_len: %d\r\n", cacert_len);
	int ret = mbedtls_x509_crt_parse(tls->cacert_ptr, cacert, cacert_len);
	if (ret < 0) {
		BK_LOGE(TAG, "mbedtls_x509_crt_parse returned -0x%04X\r\n", -ret);
		return BK_ERR_MBEDTLS_X509_CRT_PARSE_FAILED;
	}
	if (ret > 0) {
		BK_LOGE(TAG, "mbedtls_x509_crt_parse was partly successful. No. of failed certificates: %d", ret);
	}
	if (ret == 0)
		BK_LOGD(TAG, "mbedtls_x509_crt_parse returned 0 \r\n");
	mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
	BK_LOGE(TAG, "mbedtls_ssl_conf_authmode, mode:%d\r\n", tls->conf.authmode);
	BK_LOGD(TAG, "mbedtls_ssl_conf_ca_chain\r\n");
	mbedtls_ssl_conf_ca_chain(&tls->conf, tls->cacert_ptr, NULL);
	return BK_OK;
}

static bk_err_t set_pki_context(bk_tls_t *tls, const bk_tls_pki_t *pki)
{
	int ret;

	if (pki->publiccert_pem_buf != NULL &&
		pki->public_cert != NULL &&
		pki->pk_key != NULL) {

		mbedtls_x509_crt_init(pki->public_cert);
		mbedtls_pk_init(pki->pk_key);

		ret = mbedtls_x509_crt_parse(pki->public_cert, pki->publiccert_pem_buf, pki->publiccert_pem_bytes);
		if (ret < 0) {
			BK_LOGE(TAG, "mbedtls_x509_crt_parse returned -0x%04X", -ret);
			return BK_ERR_MBEDTLS_X509_CRT_PARSE_FAILED;
		}

		if (pki->privkey_pem_buf != NULL) {
			ret = mbedtls_pk_parse_key(pki->pk_key, pki->privkey_pem_buf, pki->privkey_pem_bytes,
									   pki->privkey_password, pki->privkey_password_len);
		} else {
			return -1;
		}

		if (ret < 0) {
			BK_LOGE(TAG, "mbedtls_pk_parse_keyfile returned -0x%04X", -ret);
			return BK_ERR_MBEDTLS_PK_PARSE_KEY_FAILED;
		}

		ret = mbedtls_ssl_conf_own_cert(&tls->conf, pki->public_cert, pki->pk_key);
		if (ret < 0) {
			BK_LOGE(TAG, "mbedtls_ssl_conf_own_cert returned -0x%04X\r\n", -ret);
			return BK_ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED;
		}
	} else {
		return -1;
	}
	return BK_OK;
}

bk_err_t set_client_config(const char *hostname, size_t hostlen, bk_tls_cfg_t *cfg, bk_tls_t *tls)
{
	int ret;
	if (!cfg->skip_common_name) {
		char *use_host = NULL;
		if (cfg->common_name != NULL) {
			use_host = strndup(cfg->common_name, strlen(cfg->common_name));
		} else {
			use_host = strndup(hostname, hostlen);
		}

		if (use_host == NULL) {
			return BK_ERR_NO_MEM;
		}
		/* Hostname set here should match CN in server certificate */
		BK_LOGE(TAG, "mbedtls_ssl_set_hostname use_host: %s\r\n", use_host);
		if ((ret = mbedtls_ssl_set_hostname(&tls->ssl, use_host)) != 0) {
			BK_LOGE(TAG, "mbedtls_ssl_set_hostname returned -0x%04X\r\n", -ret);
			free(use_host);
			return BK_ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED;
		}
		free(use_host);
	}
	BK_LOGD(TAG, "mbedtls_ssl_config_defaults\r\n");
	if ((ret = mbedtls_ssl_config_defaults(&tls->conf,
										   MBEDTLS_SSL_IS_CLIENT,
										   MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		BK_LOGE(TAG, "mbedtls_ssl_config_defaults returned -0x%04X\r\n", -ret);
		return BK_ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED;
	}

#ifdef CONFIG_MBEDTLS_SSL_RENEGOTIATION
	BK_LOGD(TAG, "mbedtls_ssl_conf_renegotiation\r\n");
	mbedtls_ssl_conf_renegotiation(&tls->conf, MBEDTLS_SSL_RENEGOTIATION_ENABLED);
#endif

	if (cfg->alpn_protos) {
#ifdef CONFIG_MBEDTLS_SSL_ALPN
		BK_LOGE(TAG, "mbedtls_ssl_conf_alpn_protocols\r\n");
		if ((ret = mbedtls_ssl_conf_alpn_protocols(&tls->conf, cfg->alpn_protos)) != 0) {
			BK_LOGE(TAG, "mbedtls_ssl_conf_alpn_protocols returned -0x%04X\r\n", -ret);
			return BK_ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED;
		}
#else
		BK_LOGE(TAG, "alpn_protos configured but not enabled in menuconfig: Please enable MBEDTLS_SSL_ALPN option\r\n");
		return -1;
#endif
	}

#ifdef CONFIG_BK_TLS_CLIENT_SESSION_TICKETS
		BK_LOGE(TAG, "Enabling client-side tls session ticket support\r\n");
		mbedtls_ssl_conf_session_tickets(&tls->conf, MBEDTLS_SSL_SESSION_TICKETS_ENABLED);
		mbedtls_ssl_conf_renegotiation(&tls->conf, MBEDTLS_SSL_RENEGOTIATION_ENABLED);
#endif
	
	if (cfg->crt_bundle_attach != NULL) {
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
		BK_LOGD(TAG, "mbedtls_ssl_conf_authmode\r\n");
		mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
		cfg->crt_bundle_attach(&tls->conf);
		BK_LOGE(TAG, "use_crt_bundle configured but not enabled in menuconfig: Please enable MBEDTLS_CERTIFICATE_BUNDLE option\r\n");
		return -1;
#endif
	} else if (cfg->use_global_ca_store == true) {
		 BK_LOGD(TAG, "set_global_ca_store\r\n");
		int bk_ret = set_global_ca_store(tls);
		if (bk_ret != BK_OK) {
			return bk_ret;
		}
	} else if (cfg->cacert_buf != NULL) {
		 BK_LOGD(TAG, "set_ca_cert\r\n");
		int bk_ret = set_ca_cert(tls, cfg->cacert_buf, cfg->cacert_bytes);
		if (bk_ret != BK_OK) {
			return bk_ret;
		}
		 BK_LOGD(TAG, "mbedtls_ssl_conf_ca_chain\r\n");
		mbedtls_ssl_conf_ca_chain(&tls->conf, tls->cacert_ptr, NULL);
	} else if (cfg->psk_hint_key) {
#if defined(CONFIG_BK_TLS_PSK_VERIFICATION)
		// PSK encryption mode is configured only if no certificate supplied and psk pointer not null
		BK_LOGE(TAG, "mbedtls_ssl_conf_psk\r\n");
		ret = mbedtls_ssl_conf_psk(&tls->conf, cfg->psk_hint_key->key, cfg->psk_hint_key->key_size,
										 (const unsigned char *)cfg->psk_hint_key->hint, strlen(cfg->psk_hint_key->hint));
		if (ret != 0) {
			BK_LOGE(TAG, "mbedtls_ssl_conf_psk returned -0x%04X\r\n", -ret);
			return BK_ERR_MBEDTLS_SSL_CONF_PSK_FAILED;
		}
#else
		BK_LOGE(TAG, "psk_hint_key configured but not enabled in menuconfig: Please enable BK_TLS_PSK_VERIFICATION option\r\n");
		return -1;
#endif
#ifdef CONFIG_BK_TLS_CLIENT_SESSION_TICKETS
		} else if (cfg->client_session != NULL) {
			BK_LOGE(TAG, "Resuing the saved client session\r\n");
#endif
	} else {
#ifdef CONFIG_BK_TLS_SKIP_SERVER_CERT_VERIFY
			BK_LOGE(TAG, "mbedtls_ssl_conf_authmode\r\n");
			mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
#else
			BK_LOGE(TAG, "No server verification option set in bk_tls_cfg_t structure.\r\n");
			return BK_ERR_MBEDTLS_SSL_SETUP_FAILED;
#endif
	}


	if (cfg->use_secure_element) {
#ifdef CONFIG_BK_TLS_USE_SECURE_ELEMENT
		ret = bk_set_atecc608a_pki_context(tls, (bk_tls_cfg_t *)cfg);
		if (ret != BK_OK) {
			return ret;
		}
#else

		BK_LOGE(TAG, "Please enable secure element support for BK-TLS in menuconfig\r\n");
		return BK_FAIL;
#endif
	} else if (cfg->ds_data != NULL) {
#ifdef CONFIG_BK_TLS_USE_DS_PERIPHERAL
			if (cfg->clientcert_pem_buf == NULL) {
				BK_LOGE(TAG, "Client certificate is also required with the DS parameters\r\n");
				return -1;
			}
			bk_ds_set_session_timeout(cfg->timeout_ms);
			/* set private key pointer to NULL since the DS peripheral with its own configuration data is used */
			bk_tls_pki_t pki = {
				.public_cert = &tls->clientcert,
				.pk_key = &tls->clientkey,
				.publiccert_pem_buf = cfg->clientcert_buf,
				.publiccert_pem_bytes = cfg->clientcert_bytes,
				.privkey_pem_buf = NULL,
				.privkey_pem_bytes = 0,
				.privkey_password = NULL,
				.privkey_password_len = 0,
				.bk_ds_data = cfg->ds_data,
		};
		
		bk_err_t bk_ret = set_pki_context(tls, &pki);
		if (bk_ret != BK_OK) {
			BK_LOGE(TAG, "Failed to set client pki context for the DS peripheral, returned [0x%04X]]\r\n", ret);
			return bk_ret;
		}
#else
		BK_LOGE(TAG, "Please enable the DS peripheral support for the BK-TLS in menuconfig.\r\n");
		return BK_FAIL;
#endif
	} else if (cfg->clientcert_pem_buf != NULL && cfg->clientkey_pem_buf != NULL) {
		BK_LOGD(TAG, "clientcert_pem_buf\r\n");
		bk_tls_pki_t pki = {
			.public_cert = &tls->clientcert,
			.pk_key = &tls->clientkey,
			.publiccert_pem_buf = cfg->clientcert_buf,
			.publiccert_pem_bytes = cfg->clientcert_bytes,
			.privkey_pem_buf = cfg->clientkey_buf,
			.privkey_pem_bytes = cfg->clientkey_bytes,
			.privkey_password = cfg->clientkey_password,
			.privkey_password_len = cfg->clientkey_password_len,
		};
		int bk_ret = set_pki_context(tls, &pki);
		if (bk_ret != BK_OK) {
			BK_LOGE(TAG, "Failed to set client pki context\r\n");
			return bk_ret;
		}
	} else if (cfg->clientcert_buf != NULL || cfg->clientkey_buf != NULL) {
		BK_LOGE(TAG, "You have to provide both clientcert_buf and clientkey_buf for mutual authentication\r\n");
		return -1;
	}
	return BK_OK;
}

bk_err_t bk_create_mbedtls_handle(const char *hostname, size_t hostlen, const void *cfg, bk_tls_t *tls)
{

	int ret;
	int bk_ret = BK_FAIL;
	tls->server_fd.fd = tls->sockfd;
	mbedtls_ssl_init(&tls->ssl);
	mbedtls_ctr_drbg_init(&tls->ctr_drbg);
	mbedtls_ssl_config_init(&tls->conf);
	mbedtls_entropy_init(&tls->entropy);
	
	if (tls->role == BK_TLS_CLIENT) {
		BK_LOGD(TAG, "BK_TLS_CLIENT\r\n");
		bk_ret = set_client_config(hostname, hostlen, (bk_tls_cfg_t *)cfg, tls);
		if (bk_ret != BK_OK) {
			BK_LOGE(TAG, "Failed to set client configurations, returned [0x%04X]", bk_ret);
			goto exit;
		}
	}
	BK_LOGD(TAG, "mbedtls_ctr_drbg_seed\r\n");
	if ((ret = mbedtls_ctr_drbg_seed(&tls->ctr_drbg,
									 mbedtls_entropy_func, &tls->entropy, NULL, 0)) != 0) {
		BK_LOGE(TAG, "mbedtls_ctr_drbg_seed returned -0x%04X", -ret);
		bk_ret = BK_ERR_MBEDTLS_CTR_DRBG_SEED_FAILED;
		goto exit;
	}
	BK_LOGD(TAG, "mbedtls_ssl_conf_rng\r\n");
	mbedtls_ssl_conf_rng(&tls->conf, mbedtls_ctr_drbg_random, &tls->ctr_drbg);
	BK_LOGD(TAG, "mbedtls_ssl_setup\r\n");
	if ((ret = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
		BK_LOGE(TAG, "mbedtls_ssl_setup returned -0x%04X", -ret);
		bk_ret = BK_ERR_MBEDTLS_SSL_SETUP_FAILED;
		goto exit;
	}
	BK_LOGD(TAG, "mbedtls_ssl_set_bio\r\n");
	mbedtls_ssl_set_bio(&tls->ssl, &tls->server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

	return BK_OK;

exit:
	bk_mbedtls_cleanup(tls);
	return bk_ret;
}

void bk_mbedtls_verify_certificate(bk_tls_t *tls)
{
	int flags;

	if ((flags = mbedtls_ssl_get_verify_result(&tls->ssl)) != 0) {
		BK_LOGE(TAG, "Failed to verify peer certificate!\r\n");
		char buf[100];
		bzero(buf, sizeof(buf));
		mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
		BK_LOGE(TAG, "verification info: %s\r\n", buf);
	} else {
		BK_LOGE(TAG, "Certificate verified.\r\n");
	}
}

int bk_mbedtls_handshake(bk_tls_t *tls, const bk_tls_cfg_t *cfg)
{
	int ret;
	BK_LOGE(TAG, "mbedtls_ssl_handshake\r\n");
	ret = mbedtls_ssl_handshake(&tls->ssl);
	if (ret == 0) {
		BK_LOGE(TAG, "ssl_handshake done\r\n");
		tls->conn_state = BK_TLS_DONE;

		return 1;
	} else {
		if (ret != BK_TLS_ERR_SSL_WANT_READ && ret != BK_TLS_ERR_SSL_WANT_WRITE) {
			BK_LOGE(TAG, "mbedtls_ssl_handshake returned -0x%04X\r\n", -ret);
			if (cfg->cacert_buf != NULL || cfg->use_global_ca_store == true) {
				/* This is to check whether handshake failed due to invalid certificate*/
				bk_mbedtls_verify_certificate(tls);
			}
			tls->conn_state = BK_TLS_FAIL;
			return -1;
		}
		else
			BK_LOGE(TAG, "ssl_handshake ret: -0x%04X\r\n", -ret);
		return 0;
	}
}

ssize_t bk_mbedtls_read(bk_tls_t *tls, char *data, size_t datalen)
{

	ssize_t ret = mbedtls_ssl_read(&tls->ssl, (unsigned char *)data, datalen);
	if (ret < 0) {
		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			BK_LOGE(TAG, "bk_mbedtls_read MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY\r\n");
			return 0;
		}
		if (ret != BK_TLS_ERR_SSL_WANT_READ  && ret != BK_TLS_ERR_SSL_WANT_WRITE) {
			BK_LOGE(TAG, "read error :-0x%04X:\r\n", -ret);
		}
	}
	return ret;
}

ssize_t bk_mbedtls_write(bk_tls_t *tls, const char *data, size_t datalen)
{
	size_t written = 0;
	size_t write_len = datalen;
	BK_LOGD(TAG, "bk_mbedtls_write, write_len:%d\r\n", write_len);
	while (written < datalen) {
		if (write_len > MBEDTLS_SSL_OUT_CONTENT_LEN) {
			write_len = MBEDTLS_SSL_OUT_CONTENT_LEN;
		}
		if (datalen > MBEDTLS_SSL_OUT_CONTENT_LEN) {
			BK_LOGE(TAG, "Fragmenting data of excessive size :%d, offset: %d, size %d\r\n", datalen, written, write_len);
		}
		ssize_t ret = mbedtls_ssl_write(&tls->ssl, (unsigned char*) data + written, write_len);
		if (ret <= 0) {
			if (ret != BK_TLS_ERR_SSL_WANT_READ  && ret != BK_TLS_ERR_SSL_WANT_WRITE && ret != 0) {
				BK_LOGE(TAG, "write error :-0x%04X:\r\n", -ret);
				return ret;
			} else {
				// Exiting the tls-write process as less than desired datalen are writable
				BK_LOGE(TAG, "mbedtls_ssl_write() returned -0x%04X, already written %d, exitting...\r\n", -ret, written);
				return (written > 0) ? written : ret;
			}
		}
		else
			BK_LOGE(TAG, "mbedtls_ssl_write, ret:%d\r\n", ret);
		written += ret;
		write_len = datalen - written;
	}
	return written;
}

void bk_mbedtls_conn_delete(bk_tls_t *tls)
{
	if (tls != NULL) {
		bk_mbedtls_cleanup(tls);
		if (tls->is_tls) {
			mbedtls_net_free(&tls->server_fd);
			tls->sockfd = tls->server_fd.fd;
		}
	}
}

static inline void bk_mbedtls_net_init(bk_tls_t *tls)
{
	mbedtls_net_init(&tls->server_fd);
}

/*---------------------------------tls------------------------------------*/
static bk_err_t bk_tls_hostname_to_fd(const char *host, size_t hostlen, int port, struct sockaddr_storage *address, int* fd)
{
	struct addrinfo *address_info;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	char *use_host = strndup(host, hostlen);
	if (!use_host) {
		return -1;
	}

	BK_LOGE(TAG, "host:%s: strlen %lu\r\n", use_host, (unsigned long)hostlen);
	int res = getaddrinfo(use_host, NULL, &hints, &address_info);
	if (res != 0 || address_info == NULL) {
		BK_LOGE(TAG, "couldn't get hostname for :%s: "
					  "getaddrinfo() returns %d, addrinfo=%p\r\n", use_host, res, address_info);
		free(use_host);
		return BK_ERR_BK_TLS_CANNOT_RESOLVE_HOSTNAME;
	}
	free(use_host);
	*fd = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
	if (*fd < 0) {
		BK_LOGE(TAG, "Failed to create socket (family %d socktype %d protocol %d)", address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);
		freeaddrinfo(address_info);
		return BK_ERR_BK_TLS_CANNOT_CREATE_SOCKET;
	}

	if (address_info->ai_family == AF_INET) {
		struct sockaddr_in *p = (struct sockaddr_in *)address_info->ai_addr;
		p->sin_port = htons(port);
		BK_LOGD(TAG, "[sock=%d] Resolved IPv4 address: %s", *fd, ipaddr_ntoa((const ip_addr_t*)&p->sin_addr.s_addr));
		memcpy(address, p, sizeof(struct sockaddr ));
	}
#if CONFIG_LWIP_IPV6
	else if (address_info->ai_family == AF_INET6) {
		struct sockaddr_in6 *p = (struct sockaddr_in6 *)address_info->ai_addr;
		p->sin6_port = htons(port);
		p->sin6_family = AF_INET6;
		BK_LOGD(TAG, "[sock=%d] Resolved IPv6 address: %s", *fd, ip6addr_ntoa((const ip6_addr_t*)&p->sin6_addr));
		memcpy(address, p, sizeof(struct sockaddr_in6 ));
	}
#endif
	else {
		BK_LOGE(TAG, "Unsupported protocol family %d", address_info->ai_family);
		close(*fd);
		freeaddrinfo(address_info);
		return BK_ERR_BK_TLS_UNSUPPORTED_PROTOCOL_FAMILY;
	}

	freeaddrinfo(address_info);
	return BK_OK;
}

static void ms_to_timeval(int timeout_ms, struct timeval *tv)
{
	tv->tv_sec = timeout_ms / 1000;
	tv->tv_usec = (timeout_ms % 1000) * 1000;
}

static bk_err_t bk_tls_set_socket_options(int fd, const bk_tls_cfg_t *cfg)
{
	if (cfg) {
		if (cfg->timeout_ms >= 0) {
			struct timeval tv;
			ms_to_timeval(cfg->timeout_ms, &tv);
			if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
				BK_LOGE(TAG, "Fail to setsockopt SO_RCVTIMEO");
				return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
			}
			if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) != 0) {
				BK_LOGE(TAG, "Fail to setsockopt SO_SNDTIMEO");
				return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
			}
		}
		if (cfg->keep_alive_cfg && cfg->keep_alive_cfg->keep_alive_enable) {
			int keep_alive_enable = 1;
			int keep_alive_idle = cfg->keep_alive_cfg->keep_alive_idle;
			int keep_alive_interval = cfg->keep_alive_cfg->keep_alive_interval;
			int keep_alive_count = cfg->keep_alive_cfg->keep_alive_count;

			BK_LOGD(TAG, "Enable TCP keep alive. idle: %d, interval: %d, count: %d", keep_alive_idle, keep_alive_interval, keep_alive_count);
			if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keep_alive_enable, sizeof(keep_alive_enable)) != 0) {
				BK_LOGE(TAG, "Fail to setsockopt SO_KEEPALIVE");
				return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
			}
			if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keep_alive_idle, sizeof(keep_alive_idle)) != 0) {
				BK_LOGE(TAG, "Fail to setsockopt TCP_KEEPIDLE");
				return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
			}
			if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &keep_alive_interval, sizeof(keep_alive_interval)) != 0) {
				BK_LOGE(TAG, "Fail to setsockopt TCP_KEEPINTVL");
				return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
			}
			if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keep_alive_count, sizeof(keep_alive_count)) != 0) {
				BK_LOGE(TAG, "Fail to setsockopt TCP_KEEPCNT");
				return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
			}
		}
		if (cfg->if_name) {
			if (cfg->if_name->ifr_name[0] != 0) {
				BK_LOGD(TAG, "Bind [sock=%d] to interface %s", fd, cfg->if_name->ifr_name);
				if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,  cfg->if_name, sizeof(struct ifreq)) != 0) {
					BK_LOGE(TAG, "Bind [sock=%d] to interface %s fail", fd, cfg->if_name->ifr_name);
					return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
				}
			}
		}
	}
	return BK_OK;
}

static bk_err_t bk_tls_set_socket_non_blocking(int fd, bool non_blocking)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		BK_LOGE(TAG, "[sock=%d] get file flags error: %s", fd, strerror(errno));
		return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
	}

	if (non_blocking) {
		flags |= O_NONBLOCK;
	} else {
		flags &= ~O_NONBLOCK;
	}

	if (fcntl(fd, F_SETFL, flags) < 0) {
		BK_LOGE(TAG, "[sock=%d] set blocking/nonblocking error: %s", fd, strerror(errno));
		return BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
	}
	return BK_OK;
}

ssize_t bk_tls_get_bytes_avail(bk_tls_t *tls)
{
	return _bk_tls_get_bytes_avail(tls);
}

int bk_tls_conn_destroy(bk_tls_t *tls)
{
	if (tls != NULL) {
		int ret = 0;
		BK_LOGE(TAG, "bk_tls_conn_destroy\r\n");
		_bk_tls_conn_delete(tls);
		if (tls->sockfd >= 0) {
			ret = close(tls->sockfd);
		}
		free(tls);
		return ret;
	}
	return -1; // invalid argument
}

static bk_err_t create_ssl_handle(const char *hostname, size_t hostlen, const void *cfg, bk_tls_t *tls)
{
	return _bk_create_ssl_handle(hostname, hostlen, cfg, tls);
}

static bk_err_t bk_tls_handshake(bk_tls_t *tls, const bk_tls_cfg_t *cfg)
{
	return _bk_tls_handshake(tls, cfg);
}

static inline bk_err_t tls_tcp_connect(const char *host, int hostlen, int port, const bk_tls_cfg_t *cfg, int *sockfd)
{
	struct sockaddr_storage address;
	int fd;
	bk_err_t ret = bk_tls_hostname_to_fd(host, hostlen, port, &address, &fd);
	if (ret != BK_OK) {
		return ret;
	}

	// Set timeout options, keep-alive options and bind device options if configured
	ret = bk_tls_set_socket_options(fd, cfg);
	if (ret != BK_OK) {
		goto err;
	}

	// Set to non block before connecting to better control connection timeout
	ret = bk_tls_set_socket_non_blocking(fd, true);
	if (ret != BK_OK) {
		goto err;
	}

	ret = BK_ERR_BK_TLS_FAILED_CONNECT_TO_HOST;
	BK_LOGE(TAG, "[sock=%d] Connecting to server. HOST: %s, Port: %d\r\n", fd, host, port);
	if (connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr)) < 0) {
		if (errno == EINPROGRESS) {
			fd_set fdset;
			struct timeval tv = { .tv_usec = 0, .tv_sec = 10 }; // Default connection timeout is 10 s

			if (cfg && cfg->non_block) {
				// Non-blocking mode -> just return successfully at this stage
				*sockfd = fd;
				return BK_OK;
			}

			if ( cfg && cfg->timeout_ms > 0 ) {
				ms_to_timeval(cfg->timeout_ms, &tv);
			}
			FD_ZERO(&fdset);
			FD_SET(fd, &fdset);

			int res = select(fd+1, NULL, &fdset, NULL, &tv);
			if (res < 0) {
				BK_LOGE(TAG, "[sock=%d] select() error: %s\r\n", fd, strerror(errno));
				goto err;
			}
			else if (res == 0) {
				ret = BK_ERR_BK_TLS_CONNECTION_TIMEOUT;
				goto err;
			} else {
				int sockerr;
				socklen_t len = (socklen_t)sizeof(int);

				if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&sockerr), &len) < 0) {
					ret = BK_ERR_BK_TLS_SOCKET_SETOPT_FAILED;
					goto err;
				}
				else if (sockerr) {
					BK_LOGE(TAG, "[sock=%d] delayed connect error: %s\r\n", fd, strerror(sockerr));
					goto err;
				}
			}
		} else {
			BK_LOGE(TAG, "[sock=%d] connect() error: %s\r\n", fd, strerror(errno));
			goto err;
		}
	}

	if (cfg && cfg->non_block == false) {
		// reset back to blocking mode (unless non_block configured)
		ret = bk_tls_set_socket_non_blocking(fd, false);
		if (ret != BK_OK) {
			goto err;
		}
	}
	BK_LOGE(TAG, "%s, sockfd:%d\r\n", __func__, *sockfd);
	*sockfd = fd;
	return BK_OK;

err:
	close(fd);
	return ret;
}

static ssize_t tls_tcp_read(bk_tls_t *tls, char *data, size_t datalen)
{
	return recv(tls->sockfd, data, datalen, 0);
}

static ssize_t tls_tcp_write(bk_tls_t *tls, const char *data, size_t datalen)
{
	return send(tls->sockfd, data, datalen, 0);
}

static int tls_low_level_conn(const char *hostname, int hostlen, int port, const bk_tls_cfg_t *cfg, bk_tls_t *tls)
{
	if (!tls) {
		BK_LOGE(TAG, "empty bk_tls parameter\r\n");
		return -1;
	}
	int bk_ret;

	switch (tls->conn_state) {
	case BK_TLS_INIT:
		tls->sockfd = -1;
		if (cfg != NULL && cfg->is_plain_tcp == false) {
			_bk_tls_net_init(tls);
			tls->is_tls = true;
		}
		if ((bk_ret = tls_tcp_connect(hostname, hostlen, port, cfg, &tls->sockfd)) != BK_OK) {
			return -1;
		}
		if (tls->is_tls == false) {
			tls->read = tls_tcp_read;
			tls->write = tls_tcp_write;
			BK_LOGE(TAG, "non-tls connection established");
			return 1;
		}

		if (cfg && cfg->non_block) {
			FD_ZERO(&tls->rset);
			FD_SET(tls->sockfd, &tls->rset);
			tls->wset = tls->rset;
		}
		tls->conn_state = BK_TLS_CONNECTING;
	/* falls through */
	case BK_TLS_CONNECTING:
		if (cfg && cfg->non_block) {
			BK_LOGE(TAG, "connecting...\r\n");
			struct timeval tv;
			ms_to_timeval(cfg->timeout_ms, &tv);

			/* In case of non-blocking I/O, we use the select() API to check whether
			   connection has been established or not*/
			if (select(tls->sockfd + 1, &tls->rset, &tls->wset, NULL,
					   cfg->timeout_ms>0 ? &tv : NULL) == 0) {
				BK_LOGD(TAG, "select() timed out\r\n");
				return 0;
			}
			if (FD_ISSET(tls->sockfd, &tls->rset) || FD_ISSET(tls->sockfd, &tls->wset)) {
				int error;
				socklen_t len = sizeof(error);
				/* pending error check */
				if (getsockopt(tls->sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
					BK_LOGD(TAG, "Non blocking connect failed\r\n");
					tls->conn_state = BK_TLS_FAIL;
					return -1;
				}
			}
		}
		/* By now, the connection has been established */
		bk_ret = create_ssl_handle(hostname, hostlen, cfg, tls);
		if (bk_ret != BK_OK) {
			BK_LOGE(TAG, "create_ssl_handle failed\r\n");
			tls->conn_state = BK_TLS_FAIL;
			return -1;
		}
		tls->read = _bk_tls_read;
		tls->write = _bk_tls_write;
		tls->conn_state = BK_TLS_HANDSHAKE;
	/* falls through */
	case BK_TLS_HANDSHAKE:
		BK_LOGE(TAG, "handshake in progress...\r\n");
		return bk_tls_handshake(tls, cfg);
		break;
	case BK_TLS_FAIL:
		BK_LOGE(TAG, "failed to open a new connection");
		break;
	default:
		BK_LOGE(TAG, "invalid bk-tls state");
		break;
	}
	return -1;
}

#define pdMS_TO_TICKS( xTimeInMs )	  ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInMs ) * ( TickType_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000U ) )

int tls_conn_new_sync(const char *hostname, int hostlen, int port, const bk_tls_cfg_t *cfg, bk_tls_t *tls)
{

	size_t start = xTaskGetTickCount();
	while (1) {
		int ret = tls_low_level_conn(hostname, hostlen, port, cfg, tls);
		if (ret == 1) {
			BK_LOGE(TAG, "open new connection\r\n");
			return ret;
		} else if (ret == -1) {
			BK_LOGE(TAG, "Failed to open new connection\r\n");
			return -1;
		} else if (ret == 0 && cfg->timeout_ms >= 0) {
			size_t timeout_ticks = pdMS_TO_TICKS(cfg->timeout_ms);
			uint32_t expired = xTaskGetTickCount() - start;
			if (expired >= timeout_ticks) {
				BK_LOGE(TAG, "Failed to open new connection in specified timeout\r\n");
				return 0;
			}
		}
	}
	return 0;
}

bk_tls_t *bk_tls_init(void)
{
	bk_tls_t *tls = (bk_tls_t *)calloc(1, sizeof(bk_tls_t));
	if (!tls) {
		return NULL;
	}
	_bk_tls_net_init(tls);
	tls->sockfd = -1;
	return tls;
}

/*---------------------------------ssl-------------------------------------*/
struct timeval* bk_transport_utils_ms_to_timeval(int timeout_ms, struct timeval *tv)
{
	if (timeout_ms == -1) {
		return NULL;
	}
	tv->tv_sec = timeout_ms / 1000;
	tv->tv_usec = (timeout_ms - (tv->tv_sec * 1000)) * 1000;
	return tv;
}

static int ssl_base_poll_read(transport_bk_tls_t *ssl, int timeout_ms)
{
	int ret = -1;
	int remain = 0;
	struct timeval timeout;
	fd_set readset;
	fd_set errset;
	FD_ZERO(&readset);
	FD_ZERO(&errset);
	FD_SET(ssl->sockfd, &readset);
	FD_SET(ssl->sockfd, &errset);

	if (ssl->tls && (remain = bk_tls_get_bytes_avail(ssl->tls)) > 0) {
		BK_LOGD(TAG, "remain data in cache, need to read again\r\n");
		return remain;
	}
	else
		BK_LOGD(TAG, "NO data available\r\n");
	ret = select(ssl->sockfd + 1, &readset, NULL, &errset, bk_transport_utils_ms_to_timeval(timeout_ms, &timeout));
	if (ret > 0 && FD_ISSET(ssl->sockfd, &errset)) {
		int sock_errno = 0;
		uint32_t optlen = sizeof(sock_errno);
		getsockopt(ssl->sockfd, SOL_SOCKET, SO_ERROR, &sock_errno, &optlen);
		BK_LOGE(TAG, "poll_read select error %d, errno = %s, fd = %d\r\n", sock_errno, strerror(sock_errno), ssl->sockfd);
		ret = -1;
	}
	return ret;
}

static int ssl_base_poll_write(transport_bk_tls_t *ssl, int timeout_ms)
{
	int ret = -1;
	struct timeval timeout;
	fd_set writeset;
	fd_set errset;
	FD_ZERO(&writeset);
	FD_ZERO(&errset);
	FD_SET(ssl->sockfd, &writeset);
	FD_SET(ssl->sockfd, &errset);
	ret = select(ssl->sockfd + 1, NULL, &writeset, &errset, bk_transport_utils_ms_to_timeval(timeout_ms, &timeout));
	if (ret > 0 && FD_ISSET(ssl->sockfd, &errset)) {
		int sock_errno = 0;
		uint32_t optlen = sizeof(sock_errno);
		getsockopt(ssl->sockfd, SOL_SOCKET, SO_ERROR, &sock_errno, &optlen);
		BK_LOGE(TAG, "poll_write select error %d, errno = %s, fd = %d\r\n", sock_errno, strerror(sock_errno), ssl->sockfd);
		ret = -1;
	}
	return ret;
}

int ssl_connect(transport_bk_tls_t *ssl, const char *host, int port, int timeout_ms)
{
	ssl->cfg.timeout_ms = timeout_ms;
	ssl->cfg.is_plain_tcp = false;

	ssl->ssl_initialized = true;
	BK_LOGD(TAG, "%s\r\n", __func__);
	ssl->tls = bk_tls_init();
	if (ssl->tls == NULL) {
		BK_LOGE(TAG, "Failed to initialize new connection object\r\n");
		return -1;
	}

	if (tls_conn_new_sync(host, strlen(host), port, &ssl->cfg, ssl->tls) <= 0) {
		BK_LOGE(TAG, "Failed to open a new connection\r\n");
		bk_tls_conn_destroy(ssl->tls);
		ssl->tls = NULL;
		ssl->sockfd = -1;
		return -1;
	}
	ssl->sockfd = ssl->tls->sockfd;
	BK_LOGE(TAG, "%s, sockfd:%d\r\n", __func__, ssl->sockfd);
	return 0;
}

int ssl_write(transport_bk_tls_t *ssl, const char *buffer, int len, int timeout_ms)
{
	int poll;
	BK_LOGD(TAG, "ssl_write\r\n");
	if ((poll = ssl_base_poll_write(ssl, timeout_ms)) <= 0) {
		BK_LOGE(TAG, "Poll timeout or error, errno=%s, fd=%d, timeout_ms=%d\r\n", strerror(errno), ssl->sockfd, timeout_ms);
		return poll;
	}
	else
		BK_LOGD(TAG, "ssl_base_poll_write, poll=%d, poll success\r\n", poll);
	int ret = _bk_tls_write(ssl->tls, (const char *) buffer, len);
	if (ret < 0) {
		BK_LOGE(TAG, "bk_tls_conn_write error, errno=%s\r\n", strerror(errno));
	}
	return ret;
}

int ssl_read(transport_bk_tls_t *ssl, char *buffer, int len, int timeout_ms)
{
	int poll;
	BK_LOGD(TAG, "ssl_read, len:%d\r\n", len);
	if ((poll = ssl_base_poll_read(ssl, timeout_ms)) <= 0) {
		BK_LOGE(TAG, "ssl_base_poll_read fail\r\n");
		return poll;
	}
	else
		BK_LOGD(TAG, "ssl_base_poll_read:%d\r\n", poll);
	int ret = _bk_tls_read(ssl->tls, ( char *)buffer, len);
	if (ret < 0) {
		BK_LOGE(TAG, "bk_tls_conn_read error, errno=%s, ret:-0x%04x", strerror(errno), ret);
	}
	if (ret == 0) {
		if (poll > 0) {
			BK_LOGE(TAG, "_bk_tls_read, socket reads 0\r\n");
			// no error, socket reads 0 while previously detected as readable -> connection has been closed cleanly
		}
		ret = -1;
	}
	if(ret > 0)
		BK_LOGD(TAG, "_bk_tls_read, ret:%d\r\n", ret);
	return ret;
}

int ssl_base_close(transport_bk_tls_t *ssl)
{
	int ret = -1;
	BK_LOGE(TAG, "ssl_base_close\r\n");
	if (ssl && ssl->ssl_initialized) {
		ret = bk_tls_conn_destroy(ssl->tls);
		ssl->tls = NULL;
		ssl->conn_state = TRANS_SSL_INIT;
		ssl->ssl_initialized = false;
		ssl->sockfd = -1;
	} else if (ssl && ssl->sockfd >= 0) {
		ret = close(ssl->sockfd);
		ssl->sockfd = -1;
	}
	return ret;
}

void bk_transport_ssl_set_interface_name(transport_bk_tls_t *ssl, struct ifreq *if_name)
{
	ssl->cfg.if_name = if_name;
}

void bk_transport_ssl_enable_global_ca_store(transport_bk_tls_t *ssl)
{
	ssl->cfg.use_global_ca_store = true;
}

#ifdef CONFIG_BK_TLS_PSK_VERIFICATION
void bk_transport_ssl_set_psk_key_hint(transport_bk_tls_t *ssl, const psk_hint_key_t* psk_hint_key)
{
	ssl->cfg.psk_hint_key = psk_hint_key;
}
#endif

void bk_transport_ssl_set_cert_data(transport_bk_tls_t *ssl, const char *data, int len)
{
	ssl->cfg.cacert_pem_buf = (void *)data;
	ssl->cfg.cacert_pem_bytes = len + 1;
}

void bk_transport_ssl_set_cert_data_der(transport_bk_tls_t *ssl, const char *data, int len)
{
	ssl->cfg.cacert_buf = (void *)data;
	ssl->cfg.cacert_bytes = len;
}

void bk_transport_ssl_set_client_cert_data(transport_bk_tls_t *ssl, const char *data, int len)
{
	ssl->cfg.clientcert_pem_buf = (void *)data;
	ssl->cfg.clientcert_pem_bytes = len + 1;
}

void bk_transport_ssl_set_client_cert_data_der(transport_bk_tls_t *ssl, const char *data, int len)
{
	ssl->cfg.clientcert_buf = (void *)data;
	ssl->cfg.clientcert_bytes = len;
}

void bk_transport_ssl_set_client_key_data(transport_bk_tls_t *ssl, const char *data, int len)
{
	ssl->cfg.clientkey_pem_buf = (void *)data;
	ssl->cfg.clientkey_pem_bytes = len + 1;
}

void bk_transport_ssl_set_client_key_password(transport_bk_tls_t *ssl, const char *password, int password_len)
{
	ssl->cfg.clientkey_password = (void *)password;
	ssl->cfg.clientkey_password_len = password_len;
}
void bk_transport_ssl_set_client_key_data_der(transport_bk_tls_t *ssl, const char *data, int len)
{
	ssl->cfg.clientkey_buf = (void *)data;
	ssl->cfg.clientkey_bytes = len;
}

void bk_transport_ssl_set_keep_alive(transport_bk_tls_t *ssl, bk_transport_keep_alive_t *keep_alive_cfg)
{
	ssl->cfg.keep_alive_cfg = (tls_keep_alive_cfg_t *) keep_alive_cfg;
}

#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
void bk_transport_ssl_crt_bundle_attach(transport_bk_tls_t *ssl, bk_err_t ((*crt_bundle_attach)(void *conf)))
{
	ssl->cfg.crt_bundle_attach = crt_bundle_attach;
}
#endif

void bk_transport_ssl_skip_common_name_check(transport_bk_tls_t *ssl)
{
	ssl->cfg.skip_common_name = true;
}
#endif

