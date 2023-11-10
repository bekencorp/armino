/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _UTILS_HTTPC_H_
#define _UTILS_HTTPC_H_

#include "iot_export_errno.h"
#include "utils_net.h"
#if (CONFIG_TFM_FWU)
#include "psa/update.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup HttpClient
 * @{
 * HttpClient API implements the client-side of HTTP/1.1. It provides base interfaces to execute an HTTP request on a given URL. It also supports HTTPS (HTTP over TLS) to provide secure communication.\n
 * @section HttpClient_Usage_Chapter How to use this module
 * In this release, MediaTek provides two types of APIs: high level APIs and low level APIs.\n
 * - \b The \b high \b level \b APIs
 *  - Enables to execute a single HTTP request on a given URL.
 *  - Call #httpclient_get(), #httpclient_post(), #httpclient_put() or #httpclient_delete() to get, post, put or delete and HTTP request.\n
 * - \b The \b low \b level \b APIs
 *  - Enables to execute more than one HTTP requests during a Keep-Alive connection. Keep-alive is the idea of using a single TCP connection to send and receive multiple HTTP requests/responses, as opposed to opening a new connection for every single request/response pair.
 *  - Step1: Call #httpclient_connect() to connect to a remote server.
 *  - Step2: Call #httpclient_send_request() to send an HTTP request to the server.
 *  - Step3: Call #httpclient_recv_response() to receive an HTTP response from the server.
 *  - Step4: Repeat Steps 2 and 3 to execute more requests.
 *  - Step5: Call #httpclient_close() to close the connection.
 *  - Sample code: Please refer to the example under <sdk_root>/project/mt7687_hdk/apps/http_client/http_client_keepalive folder.
 */

/** @defgroup httpclient_define Define
 * @{
 */
/** @brief   This macro defines the HTTP port.  */
#define HTTP_PORT   80

/** @brief   This macro defines the HTTPS port.  */
#define HTTPS_PORT 443

/** @brief   This macro defines the status of the download.  */
#define DOWNLOAD_STATUS_POS			(12)
#define DOWNLOAD_START_FLAG			(0xFE)
#define DOWNLOAD_SUCCESS_FLAG		(0xFC)

/**
 * @}
 */

/** @defgroup httpclient_enum Enum
 *  @{
 */
/** @brief   This enumeration defines the HTTP request type.  */
typedef enum {
	HTTPCLIENT_GET,
	HTTPCLIENT_POST,
	HTTPCLIENT_PUT,
	HTTPCLIENT_DELETE,
	HTTPCLIENT_HEAD
} HTTPCLIENT_REQUEST_TYPE;

#define HTTP_RESP_CONTENT_LEN   (256)

#if (CONFIG_TFM_FWU)
#define OTA_BL2_FLAG   (1 << 0)
#define OTA_SPE_FLAG   (1 << 1)
#define OTA_NSPE_FLAG  (1 << 2)

extern psa_image_id_t primary_manifest_image;
extern psa_image_id_t secondary_manifest_image;
extern psa_image_id_t primary_bl2_image;
extern psa_image_id_t secondary_bl2_image;
extern psa_image_id_t spe_image;
extern psa_image_id_t nspe_image;
#endif

/** @defgroup httpclient_struct Struct
 * @{
 */
/** @brief   This structure defines the httpclient_t structure.  */
typedef struct {
	//intptr_t handle; /**< handle of TCP connection or SSL connection. */
	int remote_port; /**< HTTP or HTTPS port. */
	utils_network_t net;
	int response_code; /**< Response code. */
	char *header; /**< Custom header. */
	char *auth_user; /**< Username for basic authentication. */
	char *auth_password; /**< Password for basic authentication. */
} httpclient_t;

/** @brief   This structure defines the HTTP data structure.  */
typedef struct {
	bool is_more; /**< Indicates if more data needs to be retrieved. */
	bool is_chunked; /**< Response data is encoded in portions/chunks.*/
	int retrieve_len; /**< Content length to be retrieved. */
	uint32_t response_content_len; /**< Response content length. */
	uint32_t post_buf_len; /**< Post data length. */
	uint32_t response_buf_len; /**< Response buffer length. */
	char *post_content_type; /**< Content type of the post data. */
	char *post_buf; /**< User data to be posted. */
	char *response_buf; /**< Buffer to store the response data. */
} httpclient_data_t;



/**
 * @brief            This function executes a POST request on a given URL. It blocks until completion.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in]        port is #HTTP_PORT or #HTTPS_PORT.
 * @param[in, out]   client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request. It also contains the data to be posted.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 * @par              HttpClient Post Example
 * @code
 *                   char *url = "https://api.mediatek.com/mcs/v2/devices/D0n2yhrl/datapoints.csv";
 *                   char *header = "deviceKey:FZoo0S07CpwUHcrt\r\n";
 *                   char *content_type = "text/csv";
 *                   char *post_data = "1,,I am string!";
 *                   httpclient_t client = {0};
 *                   httpclient_data_t client_data = {0};
 *                   char *buf = NULL;
 *                   buf = pvPortMalloc(BUF_SIZE);
 *                   if (buf == NULL) {
 *                       printf("Malloc failed.\r\n");
 *                       return;
 *                   }
 *                   memset(buf, 0, sizeof(buf));
 *                   client_data.response_buf = buf;  //Sets a buffer to store the result.
 *                   client_data.response_buf_len = BUF_SIZE;  //Sets the buffer size.
 *                   httpclient_set_custom_header(&client, header);  //Sets the custom header if needed.
 *                   client_data.post_buf = post_data;  //Sets the user data to be posted.
 *                   client_data.post_buf_len = strlen(post_data);  //Sets the post data length.
 *                   client_data.post_content_type = content_type;  //Sets the content type.
 *                   httpclient_post(&client, url, HTTPS_PORT, &client_data);
 *                   printf("Data received: %s\r\n", client_data.response_buf);
 * @endcode
 */
int http_post(
	httpclient_t *client,
	const char *url,
	int port,
	const char *ca_crt,
	uint32_t timeout,
	httpclient_data_t *client_data);

extern int httpclient_common(httpclient_t *client,
							 const char *url,
							 int port,
							 const char *ca_crt,
							 int method,
							 uint32_t timeout_ms,
							 httpclient_data_t *client_data);

void ble_sleep_cb(uint8_t is_sleeping, uint32_t slp_period);
#ifdef CONFIG_HTTP_OTA_WITH_BLE
typedef void (*ble_sleep_state_cb)(uint8_t is_sleeping, uint32_t slp_period);
extern void bk_ble_register_sleep_state_callback(ble_sleep_state_cb cb);
#endif

#if CONFIG_UVC_OTA_DEMO
typedef void (*http_data_process_callback_func)(char *buf, UINT32 len, uint32_t recived, uint32_t total);
extern void http_data_process_register_callback(http_data_process_callback_func cb);
#endif

void http_flash_init(void);
void http_flash_deinit(void);
void http_flash_wr(UINT8 *src, unsigned len);
void http_wr_to_flash(char *page, UINT32 len);

#if (CONFIG_TFM_FWU)
void bk_ota_set_flag(uint32_t flag);
uint32_t bk_ota_get_flag(void);
void bk_ota_clear_flag(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __HTTPCLIENT_H__ */

