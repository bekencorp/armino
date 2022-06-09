#ifndef _DEMOS_DOORBELL_H__
#define _DEMOS_DOORBELL_H__

typedef struct {
	UINT8 id;
	UINT8 is_eof;
	UINT8 pkt_cnt;
	UINT8 size;
#if SUPPORT_TIANZHIHENG_DRONE
	UINT32 unused;
#endif
} media_hdr_t;

int demo_doorbell_udp_send_packet(uint8_t *data, uint32_t len);


UINT32 demo_doorbell_udp_init(void);
void demo_doorbell_udp_deinit(void);
int demo_doorbell_send_packet(uint8_t *data, uint32_t len);
void demo_doorbell_disconnect_cmd_udp(void);

#endif
// eof

