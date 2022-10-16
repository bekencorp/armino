#ifndef _AUD_DEBUG_UDP_H__
#define _AUD_DEBUG_UDP_H__

// UDP_SDP_CONFIG
#define AUD_DEBUG_UDP_CMD_PORT             7090
#define AUD_DEBUG_UDP_VOICE_PORT           7070

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

int bk_aud_debug_voc_udp_send_packet(unsigned char *data, unsigned int len);
uint32_t bk_aud_debug_udp_init(void);
bk_err_t bk_aud_debug_udp_deinit(void);

#endif

