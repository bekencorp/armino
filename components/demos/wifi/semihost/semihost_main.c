#include <common/bk_include.h>
#include <os/str.h>
#include "pbuf.h"
#include "semihost.h"
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include <os/mem.h>
#include <os/str.h>
#include "bk_rw.h"

xTaskHandle semi_host_thread_handle = NULL;
extern int bmsg_tx_sender(struct pbuf *p, uint32_t vif_idx);

static void semi_host_uart_rx_callback(int uport, struct semi_host_env *sh)
{
	if (sh->sema)
		rtos_set_semaphore(&sh->sema);
}

/* Main Semihost processing thread */
static void semi_host_main(struct semi_host_env *sh)
{
	int ret;
	int len;
	struct pbuf *p;

	bk_uart_set_rx_callback(SEMI_HOST_UART, (uart_callback)semi_host_uart_rx_callback, sh);

	while (1) {
		rtos_get_semaphore(&sh->sema, SEMI_HOST_UART);

		while ((ret = recv_packet(sh)) > 0) {
			//os_printf("ret: %d\n", ret);
			// overflow
			if (ret == 2) {
				sh->received = 0;
				continue;
			}

			/* have received a complete packet */
			len = sh->received;

			//print_hex_dump("UART RX: ", sh->buf, len);

			p = pbuf_alloc(0, len, 0);
			if (p) {
				uint8_t vif_idx;
				os_memcpy(p->payload, sh->buf, len);
				vif_idx = rwm_mgmt_vif_mac2idx(&p->payload[6]);	// get vif index based on source mac address
				//os_printf("vif_idx: %d\n", vif_idx);
				if (vif_idx != 0xff)
					bmsg_tx_sender(p, vif_idx);
			}

			sh->received = 0;	// reset
		}
	}

	os_printf("Semi host exited\r\n");

	bk_uart_set_rx_callback(SEMI_HOST_UART, NULL, NULL);
	rtos_delete_thread(NULL);
}

int semi_host_init(void)
{
    int ret;
	struct semi_host_env *sh;

	sh = os_zalloc(sizeof(*sh));
	if (!sh)
		return kNoMemoryErr;

    rtos_init_semaphore(&sh->sema, 10);

    ret = rtos_create_thread(&semi_host_thread_handle,
                             BEKEN_DEFAULT_WORKER_PRIORITY,
                             "semi-host",
                             (beken_thread_function_t)semi_host_main,
                             4096,
                             sh);
    if (ret != kNoErr)
        os_printf("Error: Failed to create semi host thread: %d\r\n", ret);


    return kNoErr;
}

