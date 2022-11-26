#ifndef _AV_UDP_COMMON_H__
#define _AV_UDP_COMMON_H__


typedef struct {
	int (*aud_udp_voc_send_packet)(unsigned char *data, unsigned int len);
} aud_voc_udp_setup_t;


void aud_voc_start(aud_voc_udp_setup_t setup);

#endif

