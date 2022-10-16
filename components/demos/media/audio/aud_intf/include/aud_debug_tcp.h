#ifndef _AUD_DEBUG_TCP_H__
#define _AUD_DEBUG_TCP_H__


#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define AUD_DEBUG_TCP_LISTEN_MAX           1

#define DEMO_DOORBELL_TCP_SERVER_PORT          7010


/** define audio transfer cmd */
typedef enum
{
	DISPLAY_CLOSE = 2,
	DISPLAY_OPEN = 3,
	ECHO_DEPTH = 4,
	MAX_AMPLITUDE = 5,
	MIN_AMPLITUDE = 6,
	NOISE_LEVEL = 7,
	NOISE_PARAM = 8,
} aud_debug_cmd_t;

int bk_aud_debug_voc_tcp_send_packet(unsigned char *data, unsigned int len);
uint32_t bk_aud_debug_tcp_init(void);
bk_err_t bk_aud_debug_tcp_deinit(void);

#endif

