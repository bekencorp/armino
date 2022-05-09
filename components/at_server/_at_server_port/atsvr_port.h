#ifndef _ATSVR_PORT_H_
#define _ATSVR_PORT_H_

#define ATSVR_QUEUE_MAX_NB              6

#define AT_UART_PORT_CFG                UART1_PORT
#define AT_CTS_RTS_SOFTWARE_CFG         1
#define AT_UART_SEND_DATA_INTTRRUPT_PROTECT   0


#define IN
#define OUT
#define CONST const
#define STRING

extern void set_at_uart_overflow(int enable);
extern int get_at_uart_overflow(void);

extern void atsvr_overflow_handler(void);

extern int IN atsvr_input_char(unsigned char *buf);

#endif

