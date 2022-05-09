#include "atsvr_core.h"
#include "atsvr_port.h"
#include "at_server.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "_at_server_port.h"

#if 0
typedef struct{
	unsigned uart_overflow : 1;
}atsvr_env_t;

static atsvr_env_t atsvr_env = {
	0
};
#endif

int atsvr_send_msg_queue(atsvr_msg_t *sd_atsvrmsg,unsigned int timeout)
{
	return atsvr_port_send_msg_queue(sd_atsvrmsg,timeout);
}

int atsvr_send_msg_queue_block(atsvr_msg_t *sd_atsvrmsg)
{
	return atsvr_port_send_msg_queue(sd_atsvrmsg,0xFFFFFFFF);
}

int __weak__ atsvr_port_send_msg_queue(atsvr_msg_t *sd_atsvrmsg,unsigned int timeout)
{
	return -1;
}

int atsvr_msg_get_input(unsigned char *inbuf, unsigned int *bp)
{
    if (inbuf == NULL){
        ATSVRLOG("inbuf_null\r\n");
        return 0;
    }

	if(get_at_uart_overflow()) {
		atsvr_overflow_handler();
		atsvr_input_msg_overflow();
		ATSVRLOG("[atsvr]overflow\r\n");
        (*bp) = 0;
        return -10;
	}

    while (atsvr_input_char(&inbuf[(*bp)]) == 1)
    {
		if ((inbuf[(*bp)] == ATSVR_RET_CHAR)
			|| (inbuf[(*bp)] == ATSVR_END_CHAR))  /* end of input line */
		{
			if((*bp) >= 2) {
				inbuf[(*bp)] = '\0';
				(*bp)++;
				return 1;
			} else if((*bp) == 0) {
				continue;
			}
			ATSVRLOG("[AC]%d bp:%d\r\n",__LINE__,(*bp));
			(*bp) = 0;
			return -1;
		}

		if((*bp) <= 1) {
			if((*bp) == 0) {
				if(inbuf[0] != 'A') {
					if(get_atsvr_echo_mode() != ATSVR_ECHO_NONE) {
						inbuf[1] = '\r';
						inbuf[2] = '\n';
						atsvr_output_msg((char*)&inbuf[0],3);
					}
					(*bp) = 0;
					ATSVRLOG("[AT]head 1(%x) error\r\n",inbuf[0]);
					return -1;
				}
			}else if((*bp) == 1) {
				if(inbuf[1] != 'T') {
					if(get_atsvr_echo_mode() != ATSVR_ECHO_NONE) {
						inbuf[2] = '\r';
						inbuf[3] = '\n';
						atsvr_output_msg((char*)&inbuf[0],4);
					}
					(*bp) = 0;
					ATSVRLOG("[AT]head 2(%x) error\r\n",inbuf[0]);
					return -1;
				}
			}
		}
		if(inbuf[(*bp)] == '\0') {
			ATSVRLOG("[AC]%d bp:%d\r\n",__LINE__,(*bp));
			///(*bp) = 0;
			(*bp)++;
			return -1;
		}

		(*bp)++;
		if ((*bp) >= ATSVR_INPUT_BUFF_MAX_SIZE) {
			atsvr_input_msg_overflow();
			ATSVRLOG("[AC]%d bp:%d\r\n",__LINE__,(*bp));
			(*bp) = 0;
			return 0;
		}
	}

	return 0;
}


void atsvr_msg_handler(atsvr_msg_t *msg)
{
	int ret;

	ATSVRLOG("[ATSVR]msg type:%d\r\n", msg->type);
	switch( msg->type ) {
	case ATSVR_MSG_STREAM:
		{
			ret = atsvr_input_msg_analysis_handler((char *)msg->msg_param,msg->len);
			if(ret != 0){
				///ATSVRLOG("[ATSVR]analysis stream error\r\n");
			}
		}
		break;
	case ATSVR_MSG_INIT:
		{
			at_server_init();
		}
		break;
	case ATSVR_MSG_WLAN_EVENT:
		{
			atsvr_event_handler(msg->addition_infor);
		}
		break;
	default:
		break;
	}

	if( msg->sub_type == ATSVR_SUBMSG_ATFREE ) {
		if( msg->msg_param ) {
			at_free(msg->msg_param);
		}
	}
}

void atsvr_event_sender(ATSVR_EVT_T event)
{
	atsvr_msg_t sdmsg;
	int ret;

	sdmsg.type = ATSVR_MSG_WLAN_EVENT;
	sdmsg.sub_type = ATSVR_SUBMSG_NONE;
	sdmsg.addition_infor = event;
	sdmsg.len = 0;
	sdmsg.msg_param = NULL;
	ret = atsvr_send_msg_queue(&sdmsg,0);
	if( ret != 0 ) {
		ATSVRLOG("wlan_event(%x) notice error\r\n",event);
	}
}

void __weak__ atsvr_event_handler(int event)
{

}








