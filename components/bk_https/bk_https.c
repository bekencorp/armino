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
#if CONFIG_HTTPS
#include <bk_https.h>
#include <portmacro.h>
#include <netdb.h>
#include <http_parser.h>
#include <http_header.h>
#include <http_utils.h>
#include <mbedtls/base64.h>
#include <components/system.h>
#include <bk_rom_md5.h>

#define TAG "HTTPS"

static void bk_hex_dump(char *s, int length)
{
	   //os_printf("bk begin dump:\r\n");
		for (int i = 0; i < length; i++)
			os_printf("%c", *(u8 *)(s+i));
		os_printf("\r\n");
}

/*----------------------------------------HTTP API-----------------------------------------*/
#define DEFAULT_HTTP_PORT (80)
#define DEFAULT_HTTPS_PORT (443)
#define DEFAULT_HTTP_BUF_SIZE (512)
static const char *DEFAULT_HTTP_USER_AGENT = "BEKEN HTTP Client/1.0";
static const char *DEFAULT_HTTP_PROTOCOL = "HTTP/1.1";
static const char *DEFAULT_HTTP_PATH = "/";
static const int DEFAULT_MAX_REDIRECT = 10;
static const int DEFAULT_MAX_AUTH_RETRIES = 10;
static const int CONNECTION_TIMEOUT_MS = 5000;
static const int CONNECTION_KEEP_ALIVE_IDLE = 5;
static const int CONNECTION_KEEP_ALIVE_INTERVAL= 5;
static const int CONNECTION_KEEP_ALIVE_COUNT= 3;
#define IMAGE_HEADER_SIZE 5120
#define DEFAULT_OTA_BUF_SIZE IMAGE_HEADER_SIZE

static const char *HTTP_METHOD_MAPPING[] = {
	"GET",
	"POST",
	"PUT",
	"PATCH",
	"DELETE",
	"HEAD",
	"NOTIFY",
	"SUBSCRIBE",
	"UNSUBSCRIBE",
	"OPTIONS",
	"COPY",
	"MOVE",
	"LOCK",
	"UNLOCK",
	"PROPFIND",
	"PROPPATCH",
	"MKCOL"
};
    
static bk_err_t http_dispatch_event(bk_http_client_t *client, bk_http_client_event_id_t event_id, void *data, int len)
{
	bk_http_client_event_t *event = &client->event;

	if (client->event_handler) {
		event->event_id = event_id;
		event->user_data = client->user_data;
		event->data = data;
		event->data_len = len;
		return client->event_handler(event);
	}
	return BK_OK;
}

static int http_on_message_begin(http_parser *parser)
{
	bk_http_client_t *client = parser->data;
	BK_LOGE(TAG, "on_message_begin\r\n");

	client->response->is_chunked = false;
	client->is_chunk_complete = false;
	return 0;
}

static int http_on_url(http_parser *parser, const char *at, size_t length)
{
	BK_LOGE(TAG, "http_on_url\r\n");
	return 0;
}

static int http_on_status(http_parser *parser, const char *at, size_t length)
{
	return 0;
}

static int http_on_header_event(bk_http_client_handle_t client)
{
	if (client->current_header_key != NULL && client->current_header_value != NULL) {
		BK_LOGE(TAG, "HEADER=%s:%s\r\n", client->current_header_key, client->current_header_value);
		client->event.header_key = client->current_header_key;
		client->event.header_value = client->current_header_value;
		free(client->current_header_key);
		free(client->current_header_value);
		client->current_header_key = NULL;
		client->current_header_value = NULL;
	}
	return 0;
}

static int http_on_header_field(http_parser *parser, const char *at, size_t length)
{
	bk_http_client_t *client = parser->data;
	http_on_header_event(client);
	http_utils_append_string(&client->current_header_key, at, length);

	return 0;
}

static int http_on_header_value(http_parser *parser, const char *at, size_t length)
{
	bk_http_client_handle_t client = parser->data;
	if (client->current_header_key == NULL) {
		return 0;
	}
	if (strcasecmp(client->current_header_key, "Location") == 0) {
		http_utils_append_string(&client->location, at, length);
	} else if (strcasecmp(client->current_header_key, "Transfer-Encoding") == 0
			   && memcmp(at, "chunked", length) == 0) {
		client->response->is_chunked = true;
	} else if (strcasecmp(client->current_header_key, "WWW-Authenticate") == 0) {
		http_utils_append_string(&client->auth_header, at, length);
	}
	http_utils_append_string(&client->current_header_value, at, length);
	return 0;
}

static int http_on_headers_complete(http_parser *parser)
{
	bk_http_client_handle_t client = parser->data;
	http_on_header_event(client);
	client->response->status_code = parser->status_code;
	client->response->data_offset = parser->nread;
	client->response->content_length = parser->content_length;
	client->response->data_process = 0;
	BK_LOGE(TAG, "http_on_headers_complete, status=%d, offset=%d, nread=%d, content_length:%d\r\n", parser->status_code, 
		client->response->data_offset, parser->nread, client->response->content_length);
	client->state = HTTP_STATE_RES_COMPLETE_HEADER;
	if (client->connection_info.method == HTTP_METHOD_HEAD) {
		/* In a HTTP_RESPONSE parser returning '1' from on_headers_complete will tell the
		   parser that it should not expect a body. This is used when receiving a response
		   to a HEAD request which may contain 'Content-Length' or 'Transfer-Encoding: chunked'
		   headers that indicate the presence of a body.*/
		return 1;
	}
	return 0;
}

static int http_on_body(http_parser *parser, const char *at, size_t length)
{
	bk_http_client_t *client = parser->data;
	BK_LOGD(TAG, "http_on_body one time: %d\r\n", length);
	http_dispatch_event(client, HTTP_EVENT_ON_DATA, (void *)at, length);
	if (client->response->buffer->output_ptr) {
		memcpy(client->response->buffer->output_ptr, (char *)at, length);
		bk_hex_dump(client->response->buffer->output_ptr, 20);
		client->response->buffer->output_ptr += length;
	} else {
		/* Do not cache body when http_on_body is called from bk_http_client_perform */
		if (client->state < HTTP_STATE_RES_ON_DATA_START && client->cache_data_in_fetch_hdr) {
			BK_LOGE(TAG, "Body received in fetch header state, %p, %d\r\n", at, length);
			bk_http_buffer_t *res_buffer = client->response->buffer;
			res_buffer->orig_raw_data = (char *)realloc(res_buffer->orig_raw_data, res_buffer->raw_len + length);
			if (!res_buffer->orig_raw_data) {
				BK_LOGE(TAG, "Failed to allocate memory for storing decoded data\r\n");
				return -1;
			}
			memcpy(res_buffer->orig_raw_data + res_buffer->raw_len, at, length);
			res_buffer->raw_data = res_buffer->orig_raw_data;
		}
	}

	client->response->data_process += length;
	client->response->buffer->raw_len += length;
	BK_LOGE(TAG, "http_on_body already_data_process:%d, buffer->content_length:%d\r\n", client->response->data_process, client->response->content_length);
	return 0;
}

static int http_on_message_complete(http_parser *parser)
{
	BK_LOGE(TAG, "http_on_message_complete, parser=%x\r\n", (int)parser);
	bk_http_client_handle_t client = parser->data;
	client->is_chunk_complete = true;
	return 0;
}

static int http_on_chunk_complete(http_parser *parser)
{
	BK_LOGE(TAG, "http_on_chunk_complete\r\n");
	return 0;
}

static int http_on_chunk_header(http_parser *parser)
{
	bk_http_client_handle_t client = parser->data;
	client->response->chunk_length = parser->content_length;
	BK_LOGE(TAG, "http_on_chunk_header, chunk_length\r\n");
	return 0;
}

bk_err_t bk_http_client_set_header(bk_http_client_handle_t client, const char *key, const char *value)
{
	return http_header_set(client->request->headers, key, value);
}

bk_err_t bk_http_client_set_method(bk_http_client_handle_t client, bk_http_client_method_t method)
{
	client->connection_info.method = method;
	return BK_OK;
}

static char *_get_host_header(char *host, int port)
{
	int err = 0;
	char *host_name;
	if (port != DEFAULT_HTTP_PORT && port != DEFAULT_HTTPS_PORT) {
		err = asprintf(&host_name, "%s:%d", host, port);
	} else {
		err = asprintf(&host_name, "%s", host);
	}
	if (err == -1) {
		return NULL;
	}
	return host_name;
}

bk_err_t bk_http_client_close(bk_http_client_handle_t client)
{
	if (client->state >= HTTP_STATE_INIT) {
		client->state = HTTP_STATE_INIT;
		return ssl_base_close(client->bk_ssl);
	}
	return BK_OK;
}

static bk_err_t _clear_connection_info(bk_http_client_handle_t client)
{
	free(client->connection_info.path);
	free(client->connection_info.host);
	free(client->connection_info.query);
	free(client->connection_info.username);
	if (client->connection_info.password) {
		memset(client->connection_info.password, 0, strlen(client->connection_info.password));
		free(client->connection_info.password);
	}
	free(client->connection_info.scheme);
	free(client->connection_info.url);
	memset(&client->connection_info, 0, sizeof(connection_info_t));
	return BK_OK;
}

static bk_err_t _clear_auth_data(bk_http_client_handle_t client)
{
	if (client->auth_data == NULL) {
		return BK_FAIL;
	}

	free(client->auth_data->method);
	free(client->auth_data->realm);
	free(client->auth_data->algorithm);
	free(client->auth_data->qop);
	free(client->auth_data->nonce);
	free(client->auth_data->opaque);
	memset(client->auth_data, 0, sizeof(bk_http_auth_data_t));
	return BK_OK;
}

bk_err_t bk_http_client_cleanup(bk_http_client_handle_t client)
{
	if (client == NULL) {
		return BK_FAIL;
	}
	BK_LOGE(TAG, "bk_http_client_cleanup\r\n");
	bk_http_client_close(client);
	if(client->bk_ssl) {
		if(client->bk_ssl->tls)
			free(client->bk_ssl->tls); 
		 free(client->bk_ssl); 
	}
	if (client->request) {
		http_header_destroy(client->request->headers);
		if (client->request->buffer) {
			free(client->request->buffer->data);
		}
		free(client->request->buffer);
		free(client->request);
	}
	if (client->response) {
		http_header_destroy(client->response->headers);
		if (client->response->buffer) {
			free(client->response->buffer->data);
			if (client->response->buffer->orig_raw_data) {
				free(client->response->buffer->orig_raw_data);
				client->response->buffer->orig_raw_data = NULL;
				client->response->buffer->raw_data = NULL;
			}
		}
		free(client->response->buffer);
		free(client->response);
	}
	if (client->if_name) {
		free(client->if_name);
	}
	free(client->parser);
	free(client->parser_settings);
	_clear_connection_info(client);
	_clear_auth_data(client);
	free(client->auth_data);
	free(client->current_header_key);
	free(client->location);
	free(client->auth_header);
	BK_LOGW(TAG, "free client\r\n");
	free(client);
	return BK_OK;
}

bk_err_t bk_http_client_set_url(bk_http_client_handle_t client, const char *url)
{
	char *old_host = NULL;
	struct http_parser_url purl;
	int old_port;

	if (client == NULL || url == NULL) {
		BK_LOGE(TAG, "client or url must not NULL\r\n");
		return -1;
	}

	http_parser_url_init(&purl);

	int parser_status = http_parser_parse_url(url, strlen(url), 0, &purl);

	if (parser_status != 0) {
		BK_LOGE(TAG, "Error parse url %s\r\n", url);
		return -1;
	}
	if (client->connection_info.host) {
		old_host = strdup(client->connection_info.host);
	}
	old_port = client->connection_info.port;

	if (purl.field_data[UF_HOST].len) {
		http_utils_assign_string(&client->connection_info.host, url + purl.field_data[UF_HOST].off, purl.field_data[UF_HOST].len);
		HTTP_MEM_CHECK(TAG, client->connection_info.host, {
			free(old_host);
			return BK_ERR_NO_MEM;
		});
	}
	// Close the connection if host was changed
	if (old_host && client->connection_info.host
			&& strcasecmp(old_host, (const void *)client->connection_info.host) != 0) {
		BK_LOGE(TAG, "New host assign = %s, Need close connection\r\n", client->connection_info.host);
		if (bk_http_client_set_header(client, "Host", client->connection_info.host) != BK_OK) {
			free(old_host);
			return BK_ERR_NO_MEM;
		}
		bk_http_client_close(client);
	}

	if (old_host) {
		free(old_host);
		old_host = NULL;
	}

	if (purl.field_data[UF_SCHEMA].len) {
		http_utils_assign_string(&client->connection_info.scheme, url + purl.field_data[UF_SCHEMA].off, purl.field_data[UF_SCHEMA].len);
		HTTP_MEM_CHECK(TAG, client->connection_info.scheme, return BK_ERR_NO_MEM);

		if (strcasecmp(client->connection_info.scheme, "http") == 0) {
			client->connection_info.port = DEFAULT_HTTP_PORT;
		} else if (strcasecmp(client->connection_info.scheme, "https") == 0) {
			client->connection_info.port = DEFAULT_HTTPS_PORT;
		}
	}

	if (purl.field_data[UF_PORT].len) {
		client->connection_info.port = strtol((const char*)(url + purl.field_data[UF_PORT].off), NULL, 10);
	}

	if (old_port != client->connection_info.port) {
		BK_LOGD(TAG, "old_port = %d\r\n", old_port);
		bk_http_client_close(client);
	}

	if (purl.field_data[UF_USERINFO].len) {
		char *user_info = NULL;
		http_utils_assign_string(&user_info, url + purl.field_data[UF_USERINFO].off, purl.field_data[UF_USERINFO].len);
		if (user_info) {
			char *username = user_info;
			char *password = strchr(user_info, ':');
			if (password) {
				*password = 0;
				password ++;
				http_utils_assign_string(&client->connection_info.password, password, -1);
				HTTP_MEM_CHECK(TAG, client->connection_info.password, return BK_ERR_NO_MEM);
			}
			http_utils_assign_string(&client->connection_info.username, username, -1);
			HTTP_MEM_CHECK(TAG, client->connection_info.username, return BK_ERR_NO_MEM);
			free(user_info);
		} else {
			return BK_ERR_NO_MEM;
		}
	}

	//Reset path and query if there are no information
	if (purl.field_data[UF_PATH].len) {
		http_utils_assign_string(&client->connection_info.path, url + purl.field_data[UF_PATH].off, purl.field_data[UF_PATH].len);
	} else {
		http_utils_assign_string(&client->connection_info.path, "/", -1);
	}
	HTTP_MEM_CHECK(TAG, client->connection_info.path, return BK_ERR_NO_MEM);

	if (purl.field_data[UF_QUERY].len) {
		http_utils_assign_string(&client->connection_info.query, url + purl.field_data[UF_QUERY].off, purl.field_data[UF_QUERY].len);
		HTTP_MEM_CHECK(TAG, client->connection_info.query, return BK_ERR_NO_MEM);
	} else if (client->connection_info.query) {
		free(client->connection_info.query);
		client->connection_info.query = NULL;
	}

	return BK_OK;
}

static bk_err_t _set_config(bk_http_client_handle_t client, const bk_http_input_t *config)
{
	client->connection_info.method = config->method;
	client->connection_info.port = config->port;
	client->connection_info.auth_type = config->auth_type;
	client->event_handler = config->event_handler;
	client->timeout_ms = config->timeout_ms;
	client->max_redirection_count = config->max_redirection_count;
	client->max_authorization_retries = config->max_authorization_retries;
	client->user_data = config->user_data;
	client->buffer_size_rx = config->buffer_size;
	client->buffer_size_tx = config->buffer_size_tx;
	client->disable_auto_redirect = config->disable_auto_redirect;

	if (config->buffer_size == 0) {
		client->buffer_size_rx = DEFAULT_HTTP_BUF_SIZE;
	}

	if (config->buffer_size_tx == 0) {
		client->buffer_size_tx = DEFAULT_HTTP_BUF_SIZE;
	}

	if (client->max_redirection_count == 0) {
		client->max_redirection_count = DEFAULT_MAX_REDIRECT;
	}

	if (client->max_authorization_retries == 0) {
		client->max_authorization_retries = DEFAULT_MAX_AUTH_RETRIES;
	} else if (client->max_authorization_retries == -1) {
		client->max_authorization_retries = 0;
	}

	if (config->path) {
		client->connection_info.path = strdup(config->path);
	} else {
		client->connection_info.path = strdup(DEFAULT_HTTP_PATH);
	}
	HTTP_MEM_CHECK(TAG, client->connection_info.path, {
		return BK_ERR_NO_MEM;
	});

	if (config->host) {
		client->connection_info.host = strdup(config->host);

		HTTP_MEM_CHECK(TAG, client->connection_info.host, {
			_clear_connection_info(client);
			return BK_ERR_NO_MEM;
		});
	}

	if (config->query) {
		client->connection_info.query = strdup(config->query);
		HTTP_MEM_CHECK(TAG, client->connection_info.query, {
			_clear_connection_info(client);
			return BK_ERR_NO_MEM;
		});
	}

	if (config->username) {
		client->connection_info.username = strdup(config->username);
		HTTP_MEM_CHECK(TAG, client->connection_info.username, {
			_clear_connection_info(client);
			return BK_ERR_NO_MEM;
		});
	}

	if (config->password) {
		client->connection_info.password = strdup(config->password);
		HTTP_MEM_CHECK(TAG, client->connection_info.password, {
			_clear_connection_info(client);
			return BK_ERR_NO_MEM;
		});
	}

	if (config->type == HTTP_TRANSPORT_OVER_SSL) {
		http_utils_assign_string(&client->connection_info.scheme, "https", -1);
		if (client->connection_info.port == 0) {
			client->connection_info.port = DEFAULT_HTTPS_PORT;
		}
	} else {
		http_utils_assign_string(&client->connection_info.scheme, "http", -1);
		if (client->connection_info.port == 0) {
			client->connection_info.port = DEFAULT_HTTP_PORT;
		}
	}
	if (client->timeout_ms == 0) {
		client->timeout_ms = CONNECTION_TIMEOUT_MS;
	}
	if (config->is_async) {
		client->is_async = true;
	}

	return BK_OK;
}

bk_http_client_handle_t bk_http_client_init(const bk_http_input_t *config)
{

	bk_http_client_handle_t client;
	char *host_name;
	bool _success;
	_success = (
				   (client						   = calloc(1, sizeof(struct bk_http_client)))			 &&
				   (client->parser				   = calloc(1, sizeof(struct http_parser))) 		 &&
				   (client->parser_settings 	   = calloc(1, sizeof(struct http_parser_settings))) &&
				   (client->auth_data			   = calloc(1, sizeof(bk_http_auth_data_t)))		&&
				   (client->request 			   = calloc(1, sizeof(bk_http_data_t))) 			&&
				   (client->request->headers	   = http_header_init())							 &&
				   (client->request->buffer 	   = calloc(1, sizeof(bk_http_buffer_t)))			&&
				   (client->response			   = calloc(1, sizeof(bk_http_data_t))) 			&&
				   (client->response->headers	   = http_header_init())							 &&
				   (client->response->buffer	   = calloc(1, sizeof(bk_http_buffer_t)))			 &&
				   (client->bk_ssl		 = calloc(1, sizeof(transport_bk_tls_t)))
			   );

	if (!_success) {
		BK_LOGE(TAG, "Error allocate memory\r\n");
		goto error;
	}
	transport_bk_tls_t *bk_ssl = client->bk_ssl;

	if (config->keep_alive_enable == true) {
		client->keep_alive_cfg.keep_alive_enable = true;
		client->keep_alive_cfg.keep_alive_idle = (config->keep_alive_idle == 0) ? CONNECTION_KEEP_ALIVE_IDLE : config->keep_alive_idle;
		client->keep_alive_cfg.keep_alive_interval = (config->keep_alive_interval == 0) ? CONNECTION_KEEP_ALIVE_INTERVAL : config->keep_alive_interval;
		client->keep_alive_cfg.keep_alive_count =  (config->keep_alive_count == 0) ? CONNECTION_KEEP_ALIVE_COUNT : config->keep_alive_count;
		bk_transport_ssl_set_keep_alive(bk_ssl, &client->keep_alive_cfg);
	}

	if (config->if_name) {
		client->if_name = calloc(1, sizeof(struct ifreq) + 1);
		memcpy(client->if_name, config->if_name, sizeof(struct ifreq));
		bk_transport_ssl_set_interface_name(bk_ssl, client->if_name);
	}

	if (config->crt_bundle_attach != NULL) {
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
		bk_transport_ssl_crt_bundle_attach(bk_ssl, config->crt_bundle_attach);
#else
		BK_LOGE(TAG, "use_crt_bundle configured but not enabled in menuconfig: Please enable MBEDTLS_CERTIFICATE_BUNDLE option\r\n");
#endif
	} else if (config->use_global_ca_store == true) {
		bk_transport_ssl_enable_global_ca_store(bk_ssl);
	} else if (config->cert_pem) {
		if (!config->cert_len) {
			BK_LOGD(TAG, "bk_transport_ssl_set_cert_data, cert_len:%d\r\n", strlen(config->cert_pem));
			bk_transport_ssl_set_cert_data(bk_ssl, config->cert_pem, strlen(config->cert_pem));
		} else {
			BK_LOGD(TAG, "bk_transport_ssl_set_cert_data_der\r\n");
			bk_transport_ssl_set_cert_data_der(bk_ssl, config->cert_pem, config->cert_len);
		}
	}

	if (config->client_cert_pem) {
		BK_LOGE(TAG, "bk_transport_ssl_set_client_cert_data, client_cert_len:%d\r\n", strlen(config->client_cert_pem));
		if (!config->client_cert_len) {
			bk_transport_ssl_set_client_cert_data(bk_ssl, config->client_cert_pem, strlen(config->client_cert_pem));
		} else {
			bk_transport_ssl_set_client_cert_data_der(bk_ssl, config->client_cert_pem, config->client_cert_len);
		}
	}

	if (config->client_key_pem) {
		if (!config->client_key_len) {
			BK_LOGE(TAG, "bk_transport_ssl_set_client_key_data, client_key_len:%d\r\n", strlen(config->client_key_pem));
			bk_transport_ssl_set_client_key_data(bk_ssl, config->client_key_pem, strlen(config->client_key_pem));
		} else {
			bk_transport_ssl_set_client_key_data_der(bk_ssl, config->client_key_pem, config->client_key_len);
		}
	}

	if (config->client_key_password && config->client_key_password_len > 0) {
		bk_transport_ssl_set_client_key_password(bk_ssl, config->client_key_password, config->client_key_password_len);
	}

	if (config->skip_cert_common_name_check) {
		bk_transport_ssl_skip_common_name_check(bk_ssl);
	}

	if (_set_config(client, config) != BK_OK) {
		BK_LOGE(TAG, "Error set configurations\r\n");
		goto error;
	}
	_success = (
				   (client->request->buffer->data  = malloc(client->buffer_size_tx))  &&
				   (client->response->buffer->data = malloc(client->buffer_size_rx))
			   );

	if (!_success) {
		BK_LOGE(TAG, "Allocation failed\r\n");
		goto error;
	}
	else {
		BK_LOGE(TAG, "Allocation buffer_size_tx: %d, rx: %d\r\n", client->buffer_size_tx, client->buffer_size_rx);
	}
		

	const char *user_agent = config->user_agent == NULL ? DEFAULT_HTTP_USER_AGENT : config->user_agent;

	if (config->host != NULL && config->path != NULL) {
		if (client->connection_info.host == NULL) {
			BK_LOGE(TAG, "invalid host\r\n");
			goto error;
		}
		host_name = _get_host_header(client->connection_info.host, client->connection_info.port);
		if (host_name == NULL) {
			BK_LOGE(TAG, "Failed to allocate memory for host header\r\n");
			goto error;
		}
		_success = (
			(bk_http_client_set_header(client, "User-Agent", user_agent) == BK_OK) &&
			(bk_http_client_set_header(client, "Host", host_name) == BK_OK)
		);
		free(host_name);
		if (!_success) {
			BK_LOGE(TAG, "Error while setting default configurations\r\n");
			goto error;
		}
	} else if (config->url != NULL) {
		if (bk_http_client_set_url(client, config->url) != BK_OK) {
			BK_LOGE(TAG, "Failed to set URL\r\n");
			goto error;
		}
		if (client->connection_info.host == NULL) {
			BK_LOGE(TAG, "invalid host\r\n");
			goto error;
		}
		BK_LOGE(TAG, "_get_host_header host:%s, port:%d\r\n", client->connection_info.host, client->connection_info.port);
		host_name = _get_host_header(client->connection_info.host, client->connection_info.port);
		if (host_name == NULL) {
			BK_LOGE(TAG, "Failed to allocate memory for host header\r\n");
			goto error;
		}
		
		_success = (
					(bk_http_client_set_header(client, "User-Agent", user_agent) == BK_OK) &&
					(bk_http_client_set_header(client, "Host", host_name) == BK_OK)
				);
		//BK_LOGE(TAG, "bk_http_client_set_header: user_agent: %s. host_name: %s\r\n", user_agent, host_name);
		//BK_LOGE(TAG, "bk_http_client_set_header: %s\r\n", client->request->headers);
		free(host_name);
		if (!_success) {
			BK_LOGE(TAG, "Error while setting default configurations\r\n");
			goto error;
		}
	} else {
		BK_LOGE(TAG, "config should have either URL or host & path\r\n");
		goto error;
	}

	/* As default behavior, cache data received in fetch header state. This will be
	 * used in bk_http_client_read API only. For bk_http_perform we shall disable
	 * this as data will be processed by event handler */
	client->cache_data_in_fetch_hdr = 1;

	client->parser_settings->on_message_begin = http_on_message_begin;
	client->parser_settings->on_url = http_on_url;
	client->parser_settings->on_status = http_on_status;
	client->parser_settings->on_header_field = http_on_header_field;
	client->parser_settings->on_header_value = http_on_header_value;
	client->parser_settings->on_headers_complete = http_on_headers_complete;
	client->parser_settings->on_body = http_on_body;
	client->parser_settings->on_message_complete = http_on_message_complete;
	client->parser_settings->on_chunk_complete = http_on_chunk_complete;
	client->parser_settings->on_chunk_header = http_on_chunk_header;
	client->parser->data = client;
	client->event.client = client;

	client->state = HTTP_STATE_INIT;
	return client;
error:
	bk_http_client_cleanup(client);
	return NULL;
}

int bk_http_client_get_post_field(bk_http_client_handle_t client, char **data)
{
	if (client->post_data) {
		*data = client->post_data;
		return client->post_len;
	}
	return 0;
}

int bk_http_client_get_status_code(bk_http_client_handle_t client)
{
	return client->response->status_code;
}

int bk_http_client_get_content_length(bk_http_client_handle_t client)
{
	return client->response->content_length;
}

char *http_auth_basic(const char *username, const char *password)
{
	int out;
	char *user_info = NULL;
	char *digest = NULL;
	size_t n = 0;
	asprintf(&user_info, "%s:%s", username, password);
	HTTP_MEM_CHECK(TAG, user_info, return NULL);
	mbedtls_base64_encode(NULL, 0, &n, (const unsigned char *)user_info, strlen(user_info));
	digest = calloc(1, 6 + n + 1);
	HTTP_MEM_CHECK(TAG, digest, goto _basic_exit);
	strcpy(digest, "Basic ");
	mbedtls_base64_encode((unsigned char *)digest + 6, n, (size_t *)&out, (const unsigned char *)user_info, strlen(user_info));
_basic_exit:
	free(user_info);
	return digest;
}

#define MD5_MAX_LEN (33)
#define HTTP_AUTH_BUF_LEN (1024)

static int md5_printf(char *md, const char *fmt, ...)
{
	unsigned char *buf;
	unsigned char digest[MD5_MAX_LEN];
	int len, i;
	struct MD5Context md5_ctx;
	va_list ap;
	va_start(ap, fmt);
	len = vasprintf((char **)&buf, fmt, ap);
	if (buf == NULL) {
		va_end(ap);
		return BK_FAIL;
	}

	bk_rom_md5_init(&md5_ctx);
	bk_rom_md5_update(&md5_ctx, buf, len);
	bk_rom_md5_final(digest, &md5_ctx);

	for (i = 0; i < 16; ++i) {
		sprintf(&md[i * 2], "%02x", (unsigned int)digest[i]);
	}
	va_end(ap);

	free(buf);
	return MD5_MAX_LEN;
}

char *http_auth_digest(const char *username, const char *password, bk_http_auth_data_t *auth_data)
{
	char *ha1, *ha2 = NULL;
	char *digest = NULL;
	char *auth_str = NULL;
	char *temp_auth_str = NULL;

	if (username == NULL ||
		password == NULL ||
		auth_data->nonce == NULL ||
		auth_data->uri == NULL ||
		auth_data->realm == NULL) {
		return NULL;
	}

	ha1 = calloc(1, MD5_MAX_LEN);
	HTTP_MEM_CHECK(TAG, ha1, goto _digest_exit);

	ha2 = calloc(1, MD5_MAX_LEN);
	HTTP_MEM_CHECK(TAG, ha2, goto _digest_exit);

	digest = calloc(1, MD5_MAX_LEN);
	HTTP_MEM_CHECK(TAG, digest, goto _digest_exit);

	if (md5_printf(ha1, "%s:%s:%s", username, auth_data->realm, password) <= 0) {
		goto _digest_exit;
	}

	BK_LOGD(TAG, "%s %s %s %s\r\n", "Digest", username, auth_data->realm, password);
	if (strcasecmp(auth_data->algorithm, "md5-sess") == 0) {
		if (md5_printf(ha1, "%s:%s:%016llx", ha1, auth_data->nonce, auth_data->cnonce) <= 0) {
			goto _digest_exit;
		}
	}
	if (md5_printf(ha2, "%s:%s", auth_data->method, auth_data->uri) <= 0) {
		goto _digest_exit;
	}

	//support qop = auth
	if (auth_data->qop && strcasecmp(auth_data->qop, "auth-int") == 0) {
		if (md5_printf(ha2, "%s:%s", ha2, "entity") <= 0) {
			goto _digest_exit;
		}
	}

	if (auth_data->qop) {
		// response=MD5(HA1:nonce:nonceCount:cnonce:qop:HA2)
		if (md5_printf(digest, "%s:%s:%08x:%016llx:%s:%s", ha1, auth_data->nonce, auth_data->nc, auth_data->cnonce, auth_data->qop, ha2) <= 0) {
			goto _digest_exit;
		}
	} else {
		// response=MD5(HA1:nonce:HA2)
		if (md5_printf(digest, "%s:%s:%s", ha1, auth_data->nonce, ha2) <= 0) {
			goto _digest_exit;
		}
	}
	asprintf(&auth_str, "Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", algorithm=\"MD5\", "
			 "response=\"%s\", qop=%s, nc=%08x, cnonce=\"%016llx\"",
			 username, auth_data->realm, auth_data->nonce, auth_data->uri, digest, auth_data->qop, auth_data->nc, auth_data->cnonce);
	if (auth_data->opaque) {
		asprintf(&temp_auth_str, "%s, opaque=\"%s\"", auth_str, auth_data->opaque);
		free(auth_str);
		auth_str = temp_auth_str;
	}
_digest_exit:
	free(ha1);
	free(ha2);
	free(digest);
	return auth_str;
}

uint32_t bk_random(void)
{
	return (uint32_t)rand();
}

bk_err_t bk_http_client_prepare(bk_http_client_handle_t client)
{
	client->process_again = 0;
	client->response->data_process = 0;
	client->first_line_prepared = false;

	if (client->location != NULL) {
		free(client->location);
		client->location = NULL;
	}
	http_parser_init(client->parser, HTTP_RESPONSE);
	if (client->connection_info.username) {
		char *auth_response = NULL;
		BK_LOGE(TAG, "auth_response\r\n");
		if (client->connection_info.auth_type == HTTP_AUTH_TYPE_BASIC) {
			auth_response = http_auth_basic(client->connection_info.username, client->connection_info.password);
#ifdef CONFIG_BK_HTTP_CLIENT_ENABLE_DIGEST_AUTH
		} else if (client->connection_info.auth_type == HTTP_AUTH_TYPE_DIGEST && client->auth_data) {
			client->auth_data->uri = client->connection_info.path;
			client->auth_data->cnonce = ((uint64_t)bk_random() << 32) + bk_random();
			auth_response = http_auth_digest(client->connection_info.username, client->connection_info.password, client->auth_data);
			client->auth_data->nc ++;
#endif
		}

		if (auth_response) {
			BK_LOGE(TAG, "auth_response=%s\r\n", auth_response);
			bk_http_client_set_header(client, "Authorization\r\n", auth_response);
			free(auth_response);
		}
	}
	return BK_OK;
}

static int http_client_prepare_first_line(bk_http_client_handle_t client, int write_len)
{
	if (write_len >= 0) {
		http_header_set_format(client->request->headers, "Content-Length", "%d", write_len);
	} else {
		bk_http_client_set_header(client, "Transfer-Encoding", "chunked");
	}

	const char *method = HTTP_METHOD_MAPPING[client->connection_info.method];

	int first_line_len = snprintf(client->request->buffer->data,
								  client->buffer_size_tx, "%s %s",
								  method,
								  client->connection_info.path);
	if (first_line_len >= client->buffer_size_tx) {
		BK_LOGE(TAG, "Out of buffer, a:%d\r\n", first_line_len);
		return -1;
	}

	if (client->connection_info.query) {
		first_line_len += snprintf(client->request->buffer->data + first_line_len,
								   client->buffer_size_tx - first_line_len, "?%s", client->connection_info.query);
		if (first_line_len >= client->buffer_size_tx) {
			BK_LOGE(TAG, "Out of buffer, b:%d\r\n", first_line_len);
			return -1;

		}
	}
	first_line_len += snprintf(client->request->buffer->data + first_line_len,
							   client->buffer_size_tx - first_line_len, " %s\r\n", DEFAULT_HTTP_PROTOCOL);
	if (first_line_len >= client->buffer_size_tx) {
		BK_LOGE(TAG, "Out of buffer, c:%d\r\n", first_line_len);
		return -1;
	}
	return first_line_len;
}

int bk_http_client_write(bk_http_client_handle_t client, const char *buffer, int len)
{
	if (client->state < HTTP_STATE_REQ_COMPLETE_HEADER) {
		return BK_FAIL;
	}

	int wlen = 0, widx = 0;
	while (len > 0) {
		wlen = ssl_write(client->bk_ssl, buffer + widx, len, client->timeout_ms);
		if (client->is_async || wlen <= 0) {
			return wlen;
		}
		widx += wlen;
		len -= wlen;
	}
	return widx;
}

bk_err_t bk_http_client_request_send(bk_http_client_handle_t client, int write_len)
{
	int first_line_len = 0;
	if (!client->first_line_prepared) {
		if ((first_line_len = http_client_prepare_first_line(client, write_len)) < 0) {
			return first_line_len;
		}
		//BK_LOGE(TAG, "http_client_prepare_first_line over, len:%d\r\n", first_line_len);
		client->first_line_prepared = true;
		client->header_index = 0;
		client->data_written_index = 0;
		client->data_write_left = 0;
	}
	BK_LOGE(TAG, "---------BEGIN SEND REQUEST---------\r\n");
	if (client->data_write_left > 0) {
		BK_LOGE(TAG, "sending leftover data\r\n");
		/* sending leftover data from previous call to bk_http_client_request_send() API */
		int wret = 0;
		if (((wret = bk_http_client_write(client, client->request->buffer->data + client->data_written_index, client->data_write_left)) < 0)) {
			BK_LOGE(TAG, "Error write request\r\n");
			return BK_ERR_HTTP_WRITE_DATA;
		}
		client->data_write_left -= wret;
		client->data_written_index += wret;
		if (client->is_async && client->data_write_left > 0) {
			return BK_ERR_HTTP_WRITE_DATA;		/* In case of EAGAIN error, we return BK_ERR_HTTP_WRITE_DATA,
												 and the handling of EAGAIN should be done in the higher level APIs. */
		}
	}

	int wlen = client->buffer_size_tx - first_line_len;
	while ((client->header_index = http_header_generate_string(client->request->headers, client->header_index, client->request->buffer->data + first_line_len, &wlen))) {
		BK_LOGE(TAG, "max buffer_size_tx:%d, first_line_len:%d, header_index:%d\r\n", 
			client->buffer_size_tx, first_line_len, client->header_index);
		if (wlen <= 0) {
			break;
		}
		if (first_line_len) {
			wlen += first_line_len;
			first_line_len = 0;
		}
		client->request->buffer->data[wlen] = 0;
		BK_LOGE(TAG, "Send Http Request Header wlen:%d,  Header[%d]:\r\n", wlen, client->header_index);
		bk_hex_dump(client->request->buffer->data, wlen);
		
		client->data_write_left = wlen;
		client->data_written_index = 0;
		while (client->data_write_left > 0) {
			int wret = ssl_write(client->bk_ssl, client->request->buffer->data + client->data_written_index, client->data_write_left, client->timeout_ms);
			if (wret <= 0) {
				BK_LOGE(TAG, "Error write request\r\n");
				bk_http_client_close(client);
				return -1;
			}
			else
				BK_LOGD(TAG, "SEND REQUEST OK, wret:%d\r\n", wret);
			client->data_write_left -= wret;
			client->data_written_index += wret;
		}
		wlen = client->buffer_size_tx;
	}

	client->data_written_index = 0;
	client->data_write_left = client->post_len;
	client->state = HTTP_STATE_REQ_COMPLETE_HEADER;
	BK_LOGD(TAG, "HTTP_STATE_REQ_COMPLETE_HEADER\r\n");
	return BK_OK;
}

bk_err_t bk_http_client_connect(bk_http_client_handle_t client)
{
	bk_err_t err;

	if (client->state == HTTP_STATE_UNINIT) {
		BK_LOGE(TAG, "Client has not been initialized\r\n");
		return BK_ERR_HTTP_INVALID_TRANSPORT;
	}
	
	if ((err = bk_http_client_prepare(client)) != BK_OK) {
		BK_LOGE(TAG, "Failed to initialize request data\r\n");
		bk_http_client_close(client);
		return err;
	}
	BK_LOGE(TAG, "---------BEGIN CONNECT---------\r\n");
	if (client->state < HTTP_STATE_CONNECTED) {
		BK_LOGD(TAG, "Begin connect to: %s://%s:%d\r\n", client->connection_info.scheme, client->connection_info.host, client->connection_info.port);
		if (!client->is_async) {
			if (ssl_connect(client->bk_ssl, client->connection_info.host, client->connection_info.port, client->timeout_ms) < 0) {
				BK_LOGE(TAG, "Connection failed, sock < 0\r\n");
				return BK_ERR_HTTP_CONNECT;
			}
		}
		client->state = HTTP_STATE_CONNECTED;
		BK_LOGD(TAG, "HTTP_STATE_CONNECTED\r\n");
	}
	return BK_OK;
}

bk_err_t bk_http_client_open(bk_http_client_handle_t client, int write_len)
{
	client->post_len = write_len;
	bk_err_t err;
	if ((err = bk_http_client_connect(client)) != BK_OK) {
		BK_LOGE(TAG, "bk_http_client_connect fail\r\n");
		return err;
	}
	if ((err = bk_http_client_request_send(client, write_len)) != BK_OK) {
		BK_LOGE(TAG, "bk_http_client_request_send fail\r\n");
		return err;
	}
	return BK_OK;
}

int bk_http_client_fetch_headers(bk_http_client_handle_t client)
{
	if (client->state < HTTP_STATE_REQ_COMPLETE_HEADER) {
		return BK_FAIL;
	}

	client->state = HTTP_STATE_REQ_COMPLETE_DATA;
	BK_LOGD(TAG, "HTTP_STATE_REQ_COMPLETE_DATA\r\n");
	bk_http_buffer_t *buffer = client->response->buffer;
	client->response->status_code = -1;
	BK_LOGE(TAG, "---------BEGIN FEATCH HEADERS---------\r\n");

	while (client->state < HTTP_STATE_RES_COMPLETE_HEADER) {
		buffer->len = ssl_read(client->bk_ssl, buffer->data, client->buffer_size_rx, client->timeout_ms);
		if (buffer->len <= 0) {
			return BK_FAIL;
		}
		BK_LOGE(TAG, "Read First len:%d !!!!!!!!!!\r\n", buffer->len);
		http_parser_execute(client->parser, client->parser_settings, buffer->data, buffer->len);
	}
	client->state = HTTP_STATE_RES_ON_DATA_START;
	BK_LOGD(TAG, "content_length = %d\r\n", client->response->content_length);
	if (client->response->content_length <= 0) {
		client->response->is_chunked = true;
		BK_LOGD(TAG, "client->response->content_length:%d r\n", client->response->content_length);
		return 0;
	}
	return client->response->content_length;
}

static bool redirection_required(int status_code)
{
	switch (status_code) {
		case HttpStatus_MovedPermanently:
		case HttpStatus_Found:
		case HttpStatus_SeeOther:
		case HttpStatus_TemporaryRedirect:
		case HttpStatus_PermanentRedirect:
			return true;
		default:
			return false;
	}
	return false;
}

static bool process_again(int status_code)
{
	switch (status_code) {
		case HttpStatus_MovedPermanently:
		case HttpStatus_Found:
		case HttpStatus_SeeOther:
		case HttpStatus_TemporaryRedirect:
		case HttpStatus_PermanentRedirect:
		case HttpStatus_Unauthorized:
			return true;
		default:
			return false;
	}
	return false;
}

void bk_http_client_add_auth(bk_http_client_handle_t client)
{
	if (client == NULL) {
		return;
	}
	if (client->state != HTTP_STATE_RES_ON_DATA_START) {
		return;
	}
	if (client->redirect_counter >= client->max_authorization_retries) {
		BK_LOGE(TAG, "Error, reached max_authorization_retries count=%d", client->redirect_counter);
		return;
	}

	char *auth_header = client->auth_header;
	if (auth_header) {
		http_utils_trim_whitespace(&auth_header);
		BK_LOGD(TAG, "UNAUTHORIZED: %s", auth_header);
		client->redirect_counter++;
#ifdef CONFIG_BK_HTTP_CLIENT_ENABLE_DIGEST_AUTH
		if (http_utils_str_starts_with(auth_header, "Digest") == 0) {
			BK_LOGD(TAG, "type = Digest");
			client->connection_info.auth_type = HTTP_AUTH_TYPE_DIGEST;
		} else {
#endif
#ifdef CONFIG_BK_HTTP_CLIENT_ENABLE_BASIC_AUTH
		if (http_utils_str_starts_with(auth_header, "Basic") == 0) {
			BK_LOGD(TAG, "type = Basic");
			client->connection_info.auth_type = HTTP_AUTH_TYPE_BASIC;
		} else {
#endif
			client->connection_info.auth_type = HTTP_AUTH_TYPE_NONE;
			BK_LOGE(TAG, "This authentication method is not supported: %s", auth_header);
			return;
#ifdef CONFIG_BK_HTTP_CLIENT_ENABLE_BASIC_AUTH
		}
#endif
#ifdef CONFIG_BK_HTTP_CLIENT_ENABLE_DIGEST_AUTH
		}
#endif

		_clear_auth_data(client);

		client->auth_data->method = strdup(HTTP_METHOD_MAPPING[client->connection_info.method]);

		client->auth_data->nc = 1;
		client->auth_data->realm = http_utils_get_string_between(auth_header, "realm=\"", "\"");
		client->auth_data->algorithm = http_utils_get_string_between(auth_header, "algorithm=", ",");
		if (client->auth_data->algorithm == NULL) {
			client->auth_data->algorithm = strdup("MD5");
		}
		client->auth_data->qop = http_utils_get_string_between(auth_header, "qop=\"", "\"");
		client->auth_data->nonce = http_utils_get_string_between(auth_header, "nonce=\"", "\"");
		client->auth_data->opaque = http_utils_get_string_between(auth_header, "opaque=\"", "\"");
		client->process_again = 1;
	} else {
		client->connection_info.auth_type = HTTP_AUTH_TYPE_NONE;
		BK_LOGW(TAG, "This request requires authentication, but does not provide header information for that");
	}
}

bk_err_t bk_http_client_set_redirection(bk_http_client_handle_t client)
{
	if (client == NULL) {
		return -1;
	}
	if (client->location == NULL) {
		return -1;
	}
	BK_LOGE(TAG, "Redirect to %s \r\n", client->location);
	return bk_http_client_set_url(client, client->location);
}

bool bk_http_client_is_complete_data_received(bk_http_client_handle_t client)
{
	if (client->response->is_chunked) {
		if (!client->is_chunk_complete) {
			BK_LOGE(TAG, "Chunks were not completely read\r\n");
			return false;
		}
	} else {
		if (client->response->data_process != client->response->content_length) {
			BK_LOGE(TAG, "Data processed %d != Data specified in content length %d\r\n", client->response->data_process, client->response->content_length);
			return false;
		}
	}
	return true;
}

int bk_http_client_read(bk_http_client_handle_t client, char *buffer, int len)
{
	bk_http_buffer_t *res_buffer = client->response->buffer;

	int rlen = BK_FAIL, ridx = 0;
	if (res_buffer->raw_len) {
		int remain_len = client->response->buffer->raw_len;
		if (remain_len > len) {
			remain_len = len;
		}
		memcpy(buffer, res_buffer->raw_data, remain_len);
		res_buffer->raw_len -= remain_len;
		res_buffer->raw_data += remain_len;
		ridx = remain_len;
		if (res_buffer->raw_len == 0) {
			free(res_buffer->orig_raw_data);
			res_buffer->orig_raw_data = NULL;
			res_buffer->raw_data = NULL;
		}
	}
	int need_read = len - ridx;
	bool is_data_remain = true;
	while (need_read > 0 && is_data_remain) {
		if (client->response->is_chunked) {
			is_data_remain = !client->is_chunk_complete;
		} else {
			is_data_remain = client->response->data_process < client->response->content_length;
		}
		BK_LOGE(TAG, "is_data_remain=%d, is_chunked=%d, content_length=%d\r\n", is_data_remain, client->response->is_chunked, client->response->content_length);
		if (!is_data_remain) {
			break;
		}
		int byte_to_read = need_read;
		if (byte_to_read > client->buffer_size_rx) {
			byte_to_read = client->buffer_size_rx;
		}
		errno = 0;
		rlen = ssl_read(client->bk_ssl, res_buffer->data, byte_to_read, client->timeout_ms);
		BK_LOGE(TAG, "need_read=%d, byte_to_read=%d, rlen=%d, ridx=%d\r\n", need_read, byte_to_read, rlen, ridx);

		if (rlen <= 0) {
			if (errno != 0) {
				/* On connection close from server, recv should ideally return 0 but we have error conversion
				 * in `tcp_transport` SSL layer which translates it `-1` and hence below additional checks */
				if (rlen == -1 && errno == ENOTCONN && client->response->is_chunked) {
					/* Explicit call to parser for invoking `message_complete` callback */
					http_parser_execute(client->parser, client->parser_settings, res_buffer->data, 0);
				}
				BK_LOGD(TAG, "bk_transport_read returned:%d and errno:%d \r\n", rlen, errno);
			}
			if (rlen == BK_TLS_ERR_SSL_WANT_READ || errno == EAGAIN) {
				BK_LOGD(TAG, "Received EAGAIN! rlen = %d, errno %d\r\n", rlen, errno);
				return ridx;
			}
			if (rlen < 0 && ridx == 0 && !bk_http_client_is_complete_data_received(client)) {
			   return BK_FAIL;
			}
			return ridx;
		}
		res_buffer->output_ptr = buffer + ridx;
		http_parser_execute(client->parser, client->parser_settings, res_buffer->data, rlen);
		ridx += res_buffer->raw_len;
		need_read -= res_buffer->raw_len;

		res_buffer->raw_len = 0; //clear
		res_buffer->output_ptr = NULL;
	}
	return ridx;
}

static bk_err_t _http_handle_response_code(bk_http_client_handle_t http_client, int status_code)
{
	bk_err_t err;
	if (redirection_required(status_code)) {
		err = bk_http_client_set_redirection(http_client);
		if (err != BK_OK) {
			BK_LOGE(TAG, "URL redirection Failed");
			return err;
		}
	} else if (status_code == HttpStatus_Unauthorized) {
		BK_LOGE(TAG, "HttpStatus_Unauthorized\r\n");
		bk_http_client_add_auth(http_client);
	} else if(status_code == HttpStatus_NotFound || status_code == HttpStatus_Forbidden) {
		BK_LOGE(TAG, "File not found(%d)", status_code);
		return BK_FAIL;
	} else if (status_code >= HttpStatus_BadRequest && status_code < HttpStatus_InternalError) {
		BK_LOGE(TAG, "Client error (%d)", status_code);
		return BK_FAIL;
	} else if (status_code >= HttpStatus_InternalError) {
		BK_LOGE(TAG, "Server error (%d)", status_code);
		return BK_FAIL;
	}
	else {
		BK_LOGE(TAG, "status_code: (%d)", status_code);
	}

	char upgrade_data_buf[DEFAULT_OTA_BUF_SIZE];
	// process_again() returns true only in case of redirection.
	if (process_again(status_code)) {
		 BK_LOGE(TAG, "_http_handle_response_code status_code: %d\r\n", status_code);
		while (1) {
			/*
			 *	In case of redirection, bk_http_client_read() is called
			 *	to clear the response buffer of http_client.
			 */
			int data_read = bk_http_client_read(http_client, upgrade_data_buf, DEFAULT_OTA_BUF_SIZE);
			BK_LOGE(TAG, "_http_handle_response_code data_read: !!!!!!!!!!%d\r\n", data_read);
			if (data_read <= 0) {
				return BK_OK;
			}
		}
	}
	return BK_OK;
}

static bk_err_t _http_connect(bk_http_client_handle_t http_client)
{
	bk_err_t err = BK_FAIL;
	int status_code, header_ret;
	do {
		char *post_data = NULL;
		/* Send POST request if body is set.
		 * Note: Sending POST request is not supported if partial_http_download
		 * is enabled
		 */
		int post_len = bk_http_client_get_post_field(http_client, &post_data);
		err = bk_http_client_open(http_client, post_len);
		if (err != BK_OK) {
			BK_LOGE(TAG, "Failed to open HTTP connection\r\n");
			return err;
		}
		if (post_len) {
			BK_LOGE(TAG, "post_len:%d\r\n", post_len);
			int write_len = 0;
			while (post_len > 0) {
				write_len = bk_http_client_write(http_client, post_data, post_len);
				if (write_len < 0) {
					BK_LOGE(TAG, "Write failed\r\n");
					return BK_FAIL;
				}
				post_len -= write_len;
				post_data += write_len;
			}
		}
		BK_LOGE(TAG, "BEGIN FEATCH HEADER\r\n");
		header_ret = bk_http_client_fetch_headers(http_client);
		if (header_ret < 0) {
			BK_LOGE(TAG, "bk_http_client_fetch_headers fail\r\n");
			return header_ret;
		}
		else {
			BK_LOGD(TAG, "header_ret:%d\r\n", header_ret);
		}
		status_code = bk_http_client_get_status_code(http_client);
		err = _http_handle_response_code(http_client, status_code);
		if (err != BK_OK) {
			return err;
		}
	} while (process_again(status_code));
	BK_LOGD(TAG, "_http_connect over\r\n");
	return err;
}

static void _http_cleanup(bk_http_client_handle_t client)
{
	bk_http_client_close(client);
	bk_http_client_cleanup(client);
}

static bk_err_t bk_http_check_response(bk_http_client_handle_t client)
{
	if (client->response->status_code >= HttpStatus_Ok && client->response->status_code < HttpStatus_MultipleChoices) {
		return BK_OK;
	}
	else
		BK_LOGE(TAG, "Error, http status code: %d\r\n", client->response->status_code);
	if (client->redirect_counter >= client->max_redirection_count || client->disable_auto_redirect) {
		BK_LOGE(TAG, "Error, reach max_redirection_count count=%d\r\n", client->redirect_counter);
		return BK_ERR_HTTP_MAX_REDIRECT;
	}
	switch (client->response->status_code) {
		case HttpStatus_MovedPermanently:
		case HttpStatus_Found:
		case HttpStatus_SeeOther:
		case HttpStatus_TemporaryRedirect:
		case HttpStatus_PermanentRedirect:
			bk_http_client_set_redirection(client);
			client->redirect_counter ++;
			client->process_again = 1;
			break;
		case HttpStatus_Unauthorized:
			bk_http_client_add_auth(client);
	}
	return BK_OK;
}

int bk_http_client_get_data(bk_http_client_handle_t client)
{
	if (client->state < HTTP_STATE_RES_ON_DATA_START) {
		BK_LOGE(TAG, "state was changed :%d\r\n", client->state);
		return BK_FAIL;
	}

	if (client->connection_info.method == HTTP_METHOD_HEAD) {
		BK_LOGE(TAG, "method == HTTP_METHOD_HEAD\r\n");
		return 0;
	}

	bk_http_buffer_t *res_buffer = client->response->buffer;
	if(!client->response->is_chunked)
		BK_LOGD(TAG, "data_process=%d, content_length=%d\r\n", client->response->data_process, client->response->content_length);
	else
		BK_LOGE(TAG, "is_chunked, data_process=%d\r\n", client->response->data_process);
	BK_LOGE(TAG, "---------BEGIN GET DATA---------\r\n");
	int rlen = ssl_read(client->bk_ssl, res_buffer->data, client->buffer_size_rx, client->timeout_ms);
	if (rlen >= 0) {
		http_parser_execute(client->parser, client->parser_settings, res_buffer->data, rlen);
	}

	return rlen;
}

bk_err_t bk_http_client_send_post_data(bk_http_client_handle_t client)
{
	if (client->state != HTTP_STATE_REQ_COMPLETE_HEADER) {
		BK_LOGE(TAG, "Invalid state\r\n");
		return -1;
	}
	if (!(client->post_data && client->post_len)) {
		goto success;
	}

	int wret = bk_http_client_write(client, client->post_data + client->data_written_index, client->data_write_left);
	if (wret < 0) {
		return wret;
	}
	BK_LOGE(TAG, "bk_http_client_send_post_data, wret:%d\r\n", wret);
	client->data_write_left -= wret;
	client->data_written_index += wret;

	if (client->data_write_left <= 0) {
		goto success;
	} else {
		BK_LOGE(TAG, "Invalid state\r\n");
		return BK_ERR_HTTP_WRITE_DATA;
	}

success:
	client->state = HTTP_STATE_REQ_COMPLETE_DATA;
	return BK_OK;
}

bk_err_t bk_http_client_perform(bk_http_client_handle_t client)
{
	bk_err_t err;
	do {
		if (client->process_again) {
			bk_http_client_prepare(client);
		}
		switch (client->state) {
			case HTTP_STATE_INIT:
				err = bk_http_client_connect(client);
				if (err != BK_OK) {
					BK_LOGE(TAG, "Connection fail\r\n");
					http_dispatch_event(client, HTTP_EVENT_ERROR, NULL, 0);
					return err;
				}
				else {
					BK_LOGD(TAG, "Connection ok\r\n");
				}

			case HTTP_STATE_CONNECTED:
				err = bk_http_client_request_send(client, client->post_len);
				if (err != BK_OK) {
					BK_LOGE(TAG, "request send fail\r\n");
					http_dispatch_event(client, HTTP_EVENT_ERROR, NULL, 0);
					return err;
				}
				else {
					BK_LOGD(TAG, "request send ok\r\n");
				}

			case HTTP_STATE_REQ_COMPLETE_HEADER:
				err = bk_http_client_send_post_data(client);
				if (err != BK_OK) {
					BK_LOGE(TAG, "bk_http_client_send_post_data fail\r\n");
					http_dispatch_event(client, HTTP_EVENT_ERROR, NULL, 0);
					return err;
				}
				else {
					BK_LOGD(TAG, "bk_http_client_send_post_data ok\r\n");
				}

			case HTTP_STATE_REQ_COMPLETE_DATA:
				client->cache_data_in_fetch_hdr = 0;
				if (bk_http_client_fetch_headers(client) < 0) {
					client->cache_data_in_fetch_hdr = 1;
					BK_LOGE(TAG, "bk_http_client_fetch_headers <0\r\n");
					//if connection ENOTCONN
					BK_LOGW(TAG, "Close connection due to FIN received\r\n");
					bk_http_client_close(client);
					http_dispatch_event(client, HTTP_EVENT_ERROR, NULL, 0);
					return BK_ERR_HTTP_CONNECTION_CLOSED;;
				}
				else {
					BK_LOGD(TAG, "bk_http_client_fetch_headers ok\r\n");
				}				

			case HTTP_STATE_RES_ON_DATA_START:
				client->cache_data_in_fetch_hdr = 1;
				if ((err = bk_http_check_response(client)) != BK_OK) {
					BK_LOGE(TAG, "Error response\r\n");
					return err;
				}
				while (client->response->is_chunked && !client->is_chunk_complete) {
					BK_LOGE(TAG, "begin get data, is_chunked TRUE\r\n");
					if (bk_http_client_get_data(client) <= 0) {
						if (client->is_async && errno == EAGAIN) {
							return BK_ERR_HTTP_EAGAIN;
						}
						BK_LOGE(TAG, "Read finish or server requests close\r\n");
						break;
					}
				}
				while (client->response->data_process < client->response->content_length) {
					BK_LOGD(TAG, "begin get data\r\n");
					if (bk_http_client_get_data(client) <= 0) {
						if (client->is_async && errno == EAGAIN) {
							return -1;
						}
						BK_LOGE(TAG, "Read finish or server requests close\r\n");
						http_dispatch_event(client, HTTP_EVENT_ERROR, NULL, 0);
						break;
					}
					//bk_hex_dump(client->response->data, 10);
				}
				BK_LOGE(TAG, "---------RECEIVED ALL DATA OVER---------\r\n");
				http_dispatch_event(client, HTTP_EVENT_ON_FINISH, NULL, 0);
				client->response->buffer->raw_len = 0;
				if (!http_should_keep_alive(client->parser)) {
					BK_LOGE(TAG, "Close connection\r\n");
					bk_http_client_close(client);
				} else {
					if (client->state > HTTP_STATE_CONNECTED) {
						client->state = HTTP_STATE_CONNECTED;
						client->first_line_prepared = false;
					}
				}
				break;
				default:
				break;
		}
	} while (client->process_again);
	return BK_OK;
}

/*--------------------------------------------------demo api-----------------------------------------------*/
/* user can get data from cb*/
bk_err_t https_test_event_cb(bk_http_client_event_t *evt)
{
	switch (evt->event_id) {
	case HTTP_EVENT_ERROR:
		BK_LOGE(TAG, "HTTPS_EVENT_ERROR\r\n");
		break;
	case HTTP_EVENT_ON_CONNECTED:
		BK_LOGE(TAG, "HTTPS_EVENT_ON_CONNECTED\r\n");
		break;
	case HTTP_EVENT_HEADER_SENT:
		BK_LOGE(TAG, "HTTPS_EVENT_HEADER_SENT\r\n");
		break;
	case HTTP_EVENT_ON_HEADER:
		BK_LOGE(TAG, "HTTPS_EVENT_ON_HEADER\r\n");
		break;
	case HTTP_EVENT_ON_DATA:
		//do something: evt->data, evt->data_len
		BK_LOGD(TAG, "HTTP_EVENT_ON_DATA, length:%d\r\n", evt->data_len);
		break;
	case HTTP_EVENT_ON_FINISH:
		BK_LOGE(TAG, "HTTPS_EVENT_ON_FINISH\r\n");
		break;
	case HTTP_EVENT_DISCONNECTED:
		BK_LOGE(TAG, "HTTPS_EVENT_DISCONNECTED\r\n");
		break;

	}
	return BK_OK;
}

/* url: https://baidu.com/ */
int https_test_download_task(bk_http_input_t *input)
{
	int err;
	bk_http_input_t config = {
		.url = input->url,
		.cert_pem = input->cert_pem,
		.event_handler = input->event_handler,
		//.buffer_size_tx = 2000, /*when is_chunk true, header request need more buffer, default 512*/
		.buffer_size = 5120
	};
	bk_http_client_handle_t client = bk_http_client_init(&config);

	err = bk_http_client_perform(client);
	if(err == BK_OK) {
		BK_LOGD(TAG, "bk_http_client_perform ok\r\n");
	}
	else
		BK_LOGE(TAG, "bk_http_client_perform fail, err:%x\r\n", err);

	err = bk_http_client_cleanup(client);
	if(err == BK_OK) {
		BK_LOGD(TAG, "bk_http_client_cleanup ok\r\n");
	}
	else
		BK_LOGE(TAG, "bk_http_client_perform fail, err:%x\r\n", err);

	return err;
}

#endif
