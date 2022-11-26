#ifndef _AUD_DEBUG_UDP_H__
#define _AUD_DEBUG_UDP_H__

// UDP_SDP_CONFIG
#define AUD_DEBUG_UDP_CMD_PORT             7090
#define AUD_DEBUG_UDP_VOICE_PORT           7070

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


int bk_aud_debug_voc_udp_send_packet(unsigned char *data, unsigned int len);

uint32_t bk_aud_debug_udp_init(void);
bk_err_t bk_aud_debug_udp_deinit(void);

#endif

