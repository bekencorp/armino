#ifndef _AV_UDP_H__
#define _AV_UDP_H__

#define THREAD_SIZE             (4 * 1024)
#define THREAD_PROIRITY         4
#define AV_PORT              5001
#define AV_BUFSZ             (4 * 1024)
#define AV_TX_TIMEOUT_SEC    (3)
#define AV_RX_TIMEOUT_SEC    (3)
#define AV_MAX_TX_RETRY      10
#define AV_MAX_RX_RETRY      10
#define AV_REPORT_INTERVAL   1
#define AV_INVALID_INDEX     (-1)


void av_udp_server_init(void);

void av_udp_client_init(void);

#endif

