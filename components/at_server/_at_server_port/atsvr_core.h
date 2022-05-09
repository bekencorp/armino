#ifndef _ATSVR_CORE_H_
#define _ATSVR_CORE_H_

typedef enum{
	ATSVR_MSG_INIT = 0,
	ATSVR_MSG_STREAM,

	ATSVR_MSG_WLAN_EVENT,
}atsvr_msgtype_e;

typedef enum{
	ATSVR_SUBMSG_NONE = 0x00,
	ATSVR_SUBMSG_ATFREE,
}atsvr_subtype_e;

typedef enum{
	ATSVR_WLAN_DISCONNECT,
	ATSVR_WLAN_CONNECTTED,
	ATSVR_WLAN_GOT_IP,

}ATSVR_EVT_T;

typedef struct{
	atsvr_msgtype_e type;
	atsvr_subtype_e sub_type;
	unsigned char addition_infor;
	unsigned int len;
	void *msg_param;
}atsvr_msg_t;

extern int atsvr_msg_get_input(unsigned char *inbuf, unsigned int *bp);
extern void atsvr_msg_handler(atsvr_msg_t *msg);

extern int atsvr_send_msg_queue(atsvr_msg_t *sd_atsvrmsg,unsigned int timeout);
extern int atsvr_send_msg_queue_block(atsvr_msg_t *sd_atsvrmsg);
extern int atsvr_port_send_msg_queue(atsvr_msg_t *sd_atsvrmsg,unsigned int timeout);

extern void atsvr_event_handler(int event);
extern void atsvr_event_sender(ATSVR_EVT_T event);

#endif

