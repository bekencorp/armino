#ifndef _AV_AUDIO_H__
#define _AV_AUDIO_H__

#define THREAD_SIZE             (4 * 1024)
#define THREAD_PROIRITY         4
#define AV_PORT              5001
#define AV_BUFSZ             1470    //(4 * 1024)
/*
#define AV_TX_TIMEOUT_SEC    (3)
#define AV_RX_TIMEOUT_SEC    (3)
#define AV_MAX_TX_RETRY      10
#define AV_MAX_RX_RETRY      10
#define AV_REPORT_INTERVAL   1
#define AV_INVALID_INDEX     (-1)
*/

/* udp */
void av_voice_udp_server_init(void);
void av_voice_udp_client_init(void);

/* tcp */
void av_voice_tcp_client_init(void);
void av_voice_tcp_server_init(void);

#endif

