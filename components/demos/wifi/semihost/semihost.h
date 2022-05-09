#ifndef __SEMIHOST_H_
#define __SEMIHOST_H_

#include <os/os.h>
#include "bk_uart.h"

#define SEMI_HOST_UART			UART_ID_0

struct semi_host_env {
	int received;
	int port;
	uint8_t buf[2048];
	uint8_t slip_state;
	beken_semaphore_t sema;
};
int recv_packet(struct semi_host_env *sh);


enum slip_state {
	SLIP_ST_NORMAL,
	SLIP_ST_ESC,
	SLIP_ST_DONE,
};


#endif // __SEMIHOST_H_

