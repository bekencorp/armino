#include "atsvr_port.h"
#include "atsvr_core.h"
#include "at_server.h"
#include "atsvr_cmd.h"
#include "atsvr_wlan.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include <os/os.h>
#include <common/bk_include.h>
#include "uart_pub.h"
#include "BkDriverUart.h"
#include <os/mem.h>


typedef struct{
	unsigned uart_overflow : 1;

	beken_queue_t msg_queue;
	beken_semaphore_t at_rx_sema;
	beken_semaphore_t at_tx_sema;

	beken_semaphore_t resources_protection;

	unsigned char rxbuf[ATSVR_INPUT_BUFF_MAX_SIZE];
	unsigned int bp;
}atsvr_port_env;

atsvr_port_env atsvr_port = {
	.uart_overflow = 0,
	.msg_queue = NULL,
	.at_rx_sema = NULL,
	.at_tx_sema = NULL,
	.resources_protection = NULL,
};




void set_at_uart_overflow(int enable)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	atsvr_port.uart_overflow = (enable > 0)?1:0;
	GLOBAL_INT_RESTORE();
}

int get_at_uart_overflow(void)
{
	int flag;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	flag = atsvr_port.uart_overflow;
	GLOBAL_INT_RESTORE();
	return flag;
}

int IN atsvr_input_char(unsigned char *buf)
{
	if (bk_uart_recv(AT_UART_PORT_CFG, buf, 1, BEKEN_NO_WAIT) == 0){
        return 1;
    }else{
        return 0;
    }
}

#define REG_READ(addr)          (*((volatile UINT32 *)(addr)))
#define REG_WRITE(addr, _data)  (*((volatile UINT32 *)(addr)) = (_data))
static void atsvr_send_byte(UINT8 data)
{
	UINT8 uport = UART1_PORT;
#if AT_UART_SEND_DATA_INTTRRUPT_PROTECT
	GLOBAL_INT_DECLARATION();
#endif
    if(UART1_PORT == uport)
        while(!UART1_TX_WRITE_READY);
    else
        while(!UART2_TX_WRITE_READY);
#if AT_UART_SEND_DATA_INTTRRUPT_PROTECT
	GLOBAL_INT_DISABLE();
#endif
    UART_WRITE_BYTE(uport, data);
#if AT_UART_SEND_DATA_INTTRRUPT_PROTECT
	GLOBAL_INT_RESTORE();
#endif
}

void at_msg_intf_clear(unsigned int dnum)
{
	unsigned char dat;
	unsigned int count = 0;
	int ret;

	do{
		if(count >= dnum){
			break;
		}
		ret = atsvr_input_char(&dat);
		if(ret == 1){
			count++;
		}
	}while(ret);
}

void atsvr_overflow_handler(void)
{
	at_msg_intf_clear(RX_RB_LENGTH);
	set_at_uart_overflow(0);
}

static void atsvr_rx_sema_callback(int uport, void *param)
{
    if(atsvr_port.at_rx_sema){
        rtos_set_semaphore(&atsvr_port.at_rx_sema);
    }
}

void atsvr_msg_intf_init(void)
{
	rtos_init_semaphore(&atsvr_port.at_rx_sema, 32);
	rtos_init_semaphore(&atsvr_port.at_tx_sema, 1);
	rtos_init_semaphore(&atsvr_port.resources_protection, 1);
	if(atsvr_port.at_tx_sema){
        rtos_set_semaphore(&atsvr_port.at_tx_sema);
    }
	if(atsvr_port.resources_protection){
		rtos_set_semaphore(&atsvr_port.resources_protection);
	}
	bk_uart_set_rx_callback(AT_UART_PORT_CFG, atsvr_rx_sema_callback,NULL);
}

static int resources_protection_func(int is_lock,unsigned int timeout)
{
	if(atsvr_port.resources_protection){
		if(is_lock){
			if(rtos_get_semaphore(&atsvr_port.resources_protection,timeout) != kNoErr){
				return -1;
			}
		}else{
			rtos_set_semaphore(&atsvr_port.resources_protection);
		}
	}
	return 0;
}

int OUT atsvr_command_msg_handle_result_output_u(char *msg,unsigned int len,unsigned int timeout)
{
    int i = 0;
    int sta = 0;

    if(atsvr_port.at_tx_sema) {
        sta = rtos_get_semaphore(&atsvr_port.at_tx_sema, timeout);
		if(kNoErr != sta ){
			sta = -1;
		} else{
			for(i = 0; i < len; i++) {
				atsvr_send_byte(msg[i]);
			}
			rtos_set_semaphore(&atsvr_port.at_tx_sema);
		}
    }
    else {
        sta = -1;
    }

    return sta;
}

static void atsvr_output_func(char *msg,unsigned int msg_len)
{
	atsvr_command_msg_handle_result_output_u(msg,msg_len,0xFFFFFFFFU);
}

static unsigned int atsvr_input_msg_get_func(char *data,unsigned int dat_len)
{

	return 0;
}


void *at_malloc(unsigned int size)
{
	return os_malloc(size);
}

void at_free(void *p)
{
	os_free(p);
}

int atsvr_port_send_msg_queue(atsvr_msg_t *sd_atsvrmsg,unsigned int timeout)
{
	if(atsvr_port.msg_queue != NULL){
		int ret = rtos_push_to_queue(&atsvr_port.msg_queue,sd_atsvrmsg,timeout);
		if(ret != kNoErr){
			ATSVRLOG("[ATSVR]msg_queue push error:%d\r\n",ret);
			return -1;
		}
		return 0;
	}else{
		ATSVRLOG("[ATSVR]atsvr_port.msg_queue is invaild\r\n");
		return -1;
	}
}

void atsvr_event_handler(int event)
{
	ATSVRLOG("[ATSVR]treated event(%x)\r\n",event);
	if(!wlan_event_handler(event)){
		return;
	}else{
		ATSVRLOG("[ATSVR]untreated event(%x)\r\n",event);
	}
}

int get_atsvr_cmd_message_state(unsigned int timeout)
{
    int32 sta = -1;

    if(atsvr_port.at_rx_sema){
		sta = rtos_get_semaphore(&atsvr_port.at_rx_sema, timeout);
    }else{
		ATSVRLOG("[%s]at_rx_sema error\r\n",__FUNCTION__);
    }
    return sta;
}


static void atsvr_msg_main( UINT32 data )
{
	unsigned int timeout = BEKEN_NEVER_TIMEOUT;
	int state;

	at_server_init();
	atsvr_msg_intf_init();
	atsvr_cmd_init();
	atsvr_register_resources_protection_func(resources_protection_func);
	atsvr_register_output_func(atsvr_output_func);
	atsvr_register_input_msg_func(atsvr_input_msg_get_func);
	rtos_delay_milliseconds(ATSVR_POWER_UP_READY_DELAY);
	at_msg_intf_clear(0xFFFFFFFF);
	atsvr_notice_ready();

	while (1)
    {
		state =  get_atsvr_cmd_message_state(timeout);
		if(state == kNoErr) {
		con:
            state = atsvr_msg_get_input(&atsvr_port.rxbuf[0],&atsvr_port.bp);
            if( state == 1 ) {
				atsvr_msg_t msg;
				msg.type = ATSVR_MSG_STREAM;
				msg.sub_type = ATSVR_SUBMSG_NONE;
				msg.len = atsvr_port.bp;
				msg.msg_param = (void*)atsvr_port.rxbuf;
				atsvr_msg_handler(&msg);
				atsvr_port.bp = 0;
				goto con;
            }else if( state <= -1 ) {
				ATSVRLOG("[ATSVR]READ LINE ERROR(%d)\r\n",state);
				atsvr_cmd_analysis_notice_error();
				atsvr_port.bp = 0;
				goto con;
            }else if ( state != 0 ){
				atsvr_port.bp = 0;
            }
		}
		else {
			ATSVRLOG("[ATSVR]at msg receive timeout error\r\n");
        }
	}

	rtos_delete_thread(NULL);
}

static void atsvr_handler_main( UINT32 data )
{
	unsigned int timeout = BEKEN_NEVER_TIMEOUT;
	atsvr_msg_t atsvrmsg;
	int ret;

	while (1)
    {
		ret = rtos_pop_from_queue(&atsvr_port.msg_queue,&atsvrmsg,timeout);
		if(ret == kNoErr){
			atsvr_msg_handler(&atsvrmsg);
		}
	}

	rtos_delete_thread(NULL);
}


int atsvr_app_init(void)
{
    int ret;
	beken_thread_t msg_thread = NULL,handler_thread = NULL;

	if(atsvr_port.msg_queue == NULL){
		ret = rtos_init_queue(&atsvr_port.msg_queue,"atsvr-q",sizeof(atsvr_msg_t),ATSVR_QUEUE_MAX_NB);
		if(ret){
			ATSVRLOG("Error: Failed to create atsvr-q (ret:%d)\r\n",ret);
			goto init_general_err;
		}
	}
    ret = rtos_create_thread(&msg_thread,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "atsvr-m",
                             (beken_thread_function_t)atsvr_msg_main,
                             (1024 * 3),
                             0);
	if (ret != kNoErr) {
        ATSVRLOG("Error: Failed to create cli thread: %d\r\n",ret);
        goto init_general_err;
    }

	ret = rtos_create_thread(&handler_thread,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "atsvr-h",
                             (beken_thread_function_t)atsvr_handler_main,
                             (1024 * 3),
                             0);
    if (ret != kNoErr) {
        ATSVRLOG("Error: Failed to create cli thread: %d\r\n",ret);
        goto init_general_err;
    }

    return kNoErr;
init_general_err:
	if( msg_thread != NULL ) {
		rtos_delete_thread(&msg_thread);
	}
	if( handler_thread != NULL ) {
		rtos_delete_thread(&handler_thread);
	}
	if(atsvr_port.msg_queue != NULL){
		ret = rtos_deinit_queue( &atsvr_port.msg_queue );
		atsvr_port.msg_queue = NULL;
	}
    return kGeneralErr;
}



