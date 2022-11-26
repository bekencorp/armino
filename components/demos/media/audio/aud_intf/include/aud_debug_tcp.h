#ifndef _AUD_DEBUG_TCP_H__
#define _AUD_DEBUG_TCP_H__


#define CMD_VOICE_HEADER                  0x30
#define CMD_VOICE_START                   0x66
#define CMD_VOICE_STOP                    0x67

#define CMD_HEADER_CODE                   0x66
#define CMD_TAIL_CODE                     0x99
#define CMD_LEN                           8
#define AUD_DEBUG_TCP_LISTEN_MAX           1

#define AUD_DEBUG_TCP_VOICE_PORT          7010


/** define audio transfer cmd */
typedef enum
{
	AUD_DEBUG_ECHO_DEPTH = 4,
	AUD_DEBUG_MAX_AMPLITUDE = 5,
	AUD_DEBUG_MIN_AMPLITUDE = 6,
	AUD_DEBUG_NOISE_LEVEL = 7,
	AUD_DEBUG_NOISE_PARAM = 8,
	//AUD_DEBUG_REF_SCALE = 9,
	//AUD_DEBUG_OPEN = 10,
	//AUD_DEBUG_CLOSE = 11,
	AUD_DEBUG_TX = 12,
	AUD_DEBUG_RX = 13,
	AUD_DEBUG_AEC = 14,
} aud_debug_cmd_t;

int bk_aud_debug_voc_tcp_send_packet(unsigned char *data, unsigned int len);
int bk_aud_debug_voc_tx_tcp_send_packet(unsigned char *data, unsigned int len);
int bk_aud_debug_voc_rx_tcp_send_packet(unsigned char *data, unsigned int len);
int bk_aud_debug_voc_aec_tcp_send_packet(unsigned char *data, unsigned int len);
uint32_t bk_aud_debug_tcp_init(void);
bk_err_t bk_aud_debug_tcp_deinit(void);

#endif

