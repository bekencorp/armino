#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lwip/sockets.h>
#include <netdb.h>
#include <bk_ssl.h>

#define CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#define CONFIG_MBEDTLS_SSL_ALPN
#define CONFIG_MBEDTLS_SSL_RENEGOTIATION
#define CONFIG_BK_HTTP_CLIENT_ENABLE_DIGEST_AUTH
#define CONFIG_BK_HTTP_CLIENT_ENABLE_BASIC_AUTH
//#define CONFIG_BK_TLS_CLIENT_SESSION_TICKETS
//#define CONFIG_BK_TLS_PSK_VERIFICATION

/*not support http for now, only https connection*/
#define BK_ERR_HTTP_BASE			   (0x7000) 				   /*!< Starting number of HTTP error codes */
#define BK_ERR_HTTP_MAX_REDIRECT	   (BK_ERR_HTTP_BASE + 1)	  /*!< The error exceeds the number of HTTP redirects */
#define BK_ERR_HTTP_CONNECT 		   (BK_ERR_HTTP_BASE + 2)	  /*!< Error open the HTTP connection */
#define BK_ERR_HTTP_WRITE_DATA		   (BK_ERR_HTTP_BASE + 3)	  /*!< Error write HTTP data */
#define BK_ERR_HTTP_FETCH_HEADER	   (BK_ERR_HTTP_BASE + 4)	  /*!< Error read HTTP header from server */
#define BK_ERR_HTTP_INVALID_TRANSPORT  (BK_ERR_HTTP_BASE + 5)	  /*!< There are no transport support for the input scheme */
#define BK_ERR_HTTP_CONNECTING		   (BK_ERR_HTTP_BASE + 6)	  /*!< HTTP connection hasn't been established yet */
#define BK_ERR_HTTP_EAGAIN			   (BK_ERR_HTTP_BASE + 7)	  /*!< Mapping of errno EAGAIN to bk_err_t */
#define BK_ERR_HTTP_CONNECTION_CLOSED  (BK_ERR_HTTP_BASE + 8)	  /*!< Read FIN from peer and the connection closed */

/**
 * HTTP status codes.
 */
typedef enum {
	/* 2xx - Success */
	HttpStatus_Ok				 = 200,

	/* 3xx - Redirection */
	HttpStatus_MultipleChoices	 = 300,
	HttpStatus_MovedPermanently  = 301,
	HttpStatus_Found			 = 302,
	HttpStatus_SeeOther 		 = 303,
	HttpStatus_TemporaryRedirect = 307,
	HttpStatus_PermanentRedirect = 308,

	/* 4xx - Client Error */
	HttpStatus_BadRequest		 = 400,
	HttpStatus_Unauthorized 	 = 401,
	HttpStatus_Forbidden		 = 403,
	HttpStatus_NotFound 		 = 404,

	/* 5xx - Server Error */
	HttpStatus_InternalError	 = 500
} HttpStatus_Code;


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

typedef enum {
	HTTP_AUTH_TYPE_NONE = 0,
	HTTP_AUTH_TYPE_BASIC,
	HTTP_AUTH_TYPE_DIGEST,
} bk_http_client_auth_type_t;

typedef enum {
	HTTP_METHOD_GET = 0,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_PATCH,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_NOTIFY,
	HTTP_METHOD_SUBSCRIBE,
	HTTP_METHOD_UNSUBSCRIBE,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_COPY,
	HTTP_METHOD_MOVE,
	HTTP_METHOD_LOCK,
	HTTP_METHOD_UNLOCK,
	HTTP_METHOD_PROPFIND,
	HTTP_METHOD_PROPPATCH,
	HTTP_METHOD_MKCOL,
	HTTP_METHOD_MAX,
} bk_http_client_method_t;


typedef enum {
	HTTP_TRANSPORT_UNKNOWN = 0x0,
	HTTP_TRANSPORT_OVER_TCP,		/*http not support*/
	HTTP_TRANSPORT_OVER_SSL,
} bk_http_client_transport_t;

typedef struct bk_http_client *bk_http_client_handle_t;

typedef enum {
	HTTP_EVENT_ERROR = 0,
	HTTP_EVENT_ON_CONNECTED,
	HTTP_EVENT_HEADERS_SENT,
	HTTP_EVENT_HEADER_SENT = HTTP_EVENT_HEADERS_SENT,
	HTTP_EVENT_ON_HEADER,
	HTTP_EVENT_ON_DATA,
	HTTP_EVENT_ON_FINISH,
	HTTP_EVENT_DISCONNECTED,
} bk_http_client_event_id_t;


typedef struct bk_http_client_event {
	bk_http_client_event_id_t event_id;
	bk_http_client_handle_t client;
	void *data;
	int data_len;
	void *user_data;
	char *header_key;
	char *header_value;
} bk_http_client_event_t;

typedef bk_err_t (*http_event_handle_cb)(bk_http_client_event_t *evt);


typedef struct {
	const char					*url;
	const char					*host;
	int 						port;
	const char					*username;
	const char					*password;
	bk_http_client_auth_type_t auth_type;
	const char					*path;				 /*!< HTTP Path, if not set, default is `/` */
	const char					*query;
	const char					*cert_pem;
	size_t						cert_len;
	const char					*client_cert_pem;
	size_t						client_cert_len;
	const char					*client_key_pem;
	size_t						client_key_len;
	const char					*client_key_password;
	size_t						client_key_password_len;
	const char					*user_agent;
	bk_http_client_method_t    method;
	int 						timeout_ms;
	bool						disable_auto_redirect;
	int 						max_redirection_count;
	int 						max_authorization_retries;
	http_event_handle_cb		event_handler;
	bk_http_client_transport_t	type;
	int 						buffer_size;
	int 						buffer_size_tx;
	void						*user_data;
	bool						is_async;				  /*!<not supported now */
	bool						use_global_ca_store;
	bool						skip_cert_common_name_check;
	bk_err_t (*crt_bundle_attach)(void *conf);
	bool						keep_alive_enable;
	int 						keep_alive_idle;	 /*!< Keep-alive idle time. Default is 5 (second) */
	int 						keep_alive_interval; /*!< Keep-alive interval time. Default is 5 (second) */
	int 						keep_alive_count;	 /*!< Keep-alive packet retry send count. Default is 3 counts */
	struct ifreq				*if_name; 
} bk_http_input_t;

typedef struct {
	char *data;
	int len;
	char *raw_data;
	char *orig_raw_data;
	int raw_len;
	char *output_ptr;
} bk_http_buffer_t;
typedef struct http_header *http_header_handle_t;

typedef struct {
	http_header_handle_t headers;
	bk_http_buffer_t   *buffer;
	int 				status_code;
	int 				content_length;
	int 				chunk_length;
	int 				data_offset;
	int 				data_process;
	int 				method;
	bool				is_chunked;
} bk_http_data_t;

typedef struct {
	char						 *url;
	char						 *scheme;
	char						 *host;
	int 						 port;
	char						 *username;
	char						 *password;
	char						 *path;
	char						 *query;
	char						 *cert_pem;
	bk_http_client_method_t 	method;
	bk_http_client_auth_type_t	auth_type;
	bk_http_client_transport_t	transport_type;
	int 						 max_store_header_size;
} connection_info_t;

typedef struct {
	char *method;
	char *algorithm;
	char *uri;
	char *realm;
	char *nonce;
	char *qop;
	char *opaque;
	uint64_t cnonce;
	int nc;
} bk_http_auth_data_t;

typedef enum {
	HTTP_STATE_UNINIT = 0,
	HTTP_STATE_INIT,
	HTTP_STATE_CONNECTED,
	HTTP_STATE_REQ_COMPLETE_HEADER,
	HTTP_STATE_REQ_COMPLETE_DATA,
	HTTP_STATE_RES_COMPLETE_HEADER,
	HTTP_STATE_RES_ON_DATA_START,
	HTTP_STATE_RES_COMPLETE_DATA,
	HTTP_STATE_CLOSE
} bk_http_state_t;


struct bk_http_client {
	int 						redirect_counter;
	int 						max_redirection_count;
	int 						max_authorization_retries;
	int 						process_again;
	struct http_parser			*parser;
	struct http_parser_settings *parser_settings;
	transport_bk_tls_t			*bk_ssl;
	bk_http_data_t				   *request;
	bk_http_data_t				   *response;
	void						*user_data;
	bk_http_auth_data_t 	   *auth_data;
	char						*post_data;
	char						*location;
	char						*auth_header;
	char						*current_header_key;
	char						*current_header_value;
	int 						post_len;
	connection_info_t			connection_info;
	bool						is_chunk_complete;
	bk_http_state_t 		   state;
	http_event_handle_cb		event_handler;
	int 						timeout_ms;
	int 						buffer_size_rx;
	int 						buffer_size_tx;
	bool						disable_auto_redirect;
	bk_http_client_event_t	   event;
	int 						data_written_index;
	int 						data_write_left;
	bool						first_line_prepared;
	int 						header_index;
	bool						is_async;
	bk_transport_keep_alive_t  keep_alive_cfg;
	struct ifreq				*if_name;
	unsigned					cache_data_in_fetch_hdr: 1;
};
typedef struct bk_http_client bk_http_client_t;

int https_test_download_task(bk_http_input_t *input);
bk_http_client_handle_t bk_http_client_init(const bk_http_input_t *config);
bk_err_t bk_http_client_cleanup(bk_http_client_handle_t client);
bk_err_t bk_http_client_perform(bk_http_client_handle_t client);
bk_err_t bk_http_client_send_post_data(bk_http_client_handle_t client);
int bk_http_client_get_data(bk_http_client_handle_t client);
bk_err_t bk_http_client_connect(bk_http_client_handle_t client);
bk_err_t bk_http_client_prepare(bk_http_client_handle_t client);
bk_err_t bk_http_client_request_send(bk_http_client_handle_t client, int write_len);
int bk_http_client_fetch_headers(bk_http_client_handle_t client);
bk_err_t bk_http_client_close(bk_http_client_handle_t client);
