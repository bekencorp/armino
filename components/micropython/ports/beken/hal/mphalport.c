#include <stdarg.h>
#include <common/bk_include.h>
#include <common/bk_include.h>
#include <modules/wifi.h>
#include <components/system.h>
#include <components/netif.h>
#include <os/os.h>
#include <os/mem.h>
#include "bk_misc.h"
#include <driver/uart.h>
#include "py/stream.h"
#include "py/mpthread.h"
#include "py/mphal.h"
#include "extmod/misc.h"

//const mp_print_t mp_stderr_print = {NULL, stderr_print_strn};
extern bk_err_t uart_write_byte(uart_id_t id, uint8_t data);
extern int uart_read_byte_ex(uart_id_t id, uint8_t *ch);
extern int bk_rand(void);

#define RX_BUFF_SIZE 256
typedef struct {
	uart_id_t uart_id;
	/* rx buffer */
	uint8_t   rx_buff[RX_BUFF_SIZE];
	uint16_t  rx_buff_wr_idx;
	uint16_t  rx_buff_rd_idx;
	uint8_t   rx_over_flow;
} mp_hal_context;

mp_hal_context g_hal_ctxt;

void bk_printf_port(int level, char *tag, const char *fmt, va_list args);
int DEBUG_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    bk_printf_port(g_hal_ctxt.uart_id, NULL, fmt, args);
    va_end(args);
    return 0;
}

void mp_hal_uart_isr(uart_id_t id, mp_hal_context *hal_ctxt) {
	uint16_t   free_buff_len, rx_cnt = 0;
	uint8_t    rx_data;
	int   ret = -1;

	(void)id;

	if(hal_ctxt->rx_buff_wr_idx >= hal_ctxt->rx_buff_rd_idx)
	{
	    free_buff_len = RX_BUFF_SIZE - hal_ctxt->rx_buff_wr_idx + hal_ctxt->rx_buff_rd_idx - 1;
	}
	else
	{
		free_buff_len = hal_ctxt->rx_buff_rd_idx - hal_ctxt->rx_buff_wr_idx - 1;
	}

	while(1)  /* read all data from rx-FIFO. */
	{
		ret = uart_read_byte_ex(hal_ctxt->uart_id, &rx_data);
		if (ret == -1)
			break;

		rx_cnt++;

		/* rx_buff_wr_idx == rx_buff_rd_idx means empty, so reserve one byte. */
		if(rx_cnt < free_buff_len)  /* reserved one byte space. */
		{
			hal_ctxt->rx_buff[hal_ctxt->rx_buff_wr_idx] = rx_data;

			hal_ctxt->rx_buff_wr_idx = (hal_ctxt->rx_buff_wr_idx + 1) % RX_BUFF_SIZE;
		}
		else
		{
			/* discard rx-data, rx overflow. */
			hal_ctxt->rx_over_flow = 1; //  bTRUE; // rx overflow, disable rx interrupt to stop rx.
		}
	}
}

int mp_hal_init(void) {
	g_hal_ctxt.uart_id = (uart_id_t)bk_get_printf_port();
	return 0;
}

int mp_hal_exit(void) {
	return 0;
}

int mp_hal_uart_hook(int hook) {
    if (hook) {
        return bk_uart_take_rx_isr(g_hal_ctxt.uart_id, (uart_isr_t)mp_hal_uart_isr, (void *)&g_hal_ctxt);
	} else {
        return bk_uart_recover_rx_isr(g_hal_ctxt.uart_id);
	}
}


uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags) {
    uintptr_t ret = 0;

    if (poll_flags & MP_STREAM_POLL_RD) {
    	if (g_hal_ctxt.rx_buff_wr_idx != g_hal_ctxt.rx_buff_rd_idx) {
	        ret |= MP_STREAM_POLL_RD;
        }
    }
    return ret;
}

int mp_hal_stdin_rx_chr(void) {
	int c = 0;
    for (;;) {
		if (g_hal_ctxt.rx_buff_rd_idx != g_hal_ctxt.rx_buff_wr_idx)
		{
			c = (int)g_hal_ctxt.rx_buff[g_hal_ctxt.rx_buff_rd_idx];

			g_hal_ctxt.rx_buff_rd_idx = (g_hal_ctxt.rx_buff_rd_idx + 1) % RX_BUFF_SIZE;

            return c;
		}
        MICROPY_EVENT_POLL_HOOK
    }
}

int uart_stdout_tx_strn(const char *str, size_t len) {
    const char *end = str + len;
    bk_err_t ret;

    for (; str < end; str++) {
        ret = uart_write_byte(g_hal_ctxt.uart_id, (uint8_t)*str);
        if (ret != BK_OK) {
            break;
        }
    }
    return (int)ret;
}

void mp_hal_stdout_tx_strn(const char *str, size_t len) {
    // Only release the GIL if many characters are being sent
    bool release_gil = len > 20;
    if (release_gil) {
        MP_THREAD_GIL_EXIT();
    }
    uart_stdout_tx_strn(str, len);
    if (release_gil) {
        MP_THREAD_GIL_ENTER();
    }
    mp_uos_dupterm_tx_strn(str, len);
}

void nlr_jump_fail(void *val) {
    printf("NLR jump failed, val=%p\n", val);
    bk_reboot();
    MP_UNREACHABLE
}

void mp_hal_delay_ms(mp_uint_t ms) {
    rtos_delay_milliseconds(ms);
}

/**
 * 13 instructs for each loop
 * 13/Freq seconds for each loop
 * 1 second run Freq/13 loops
 * 1 mocrosecond run Freq/13000000 loops
 */
void mp_hal_delay_us(mp_uint_t us) {
    //delay(us);
    volatile uint32_t index;
    uint32_t count = us * 120 / 13 + 1;
    for(index=0;index<count;++index);
}

mp_uint_t mp_hal_ticks_ms(void) {
	return (mp_uint_t)rtos_get_time();
}

mp_uint_t mp_hal_ticks_us(void) {
	return (mp_uint_t)rtos_get_time() * 1000;
}

mp_uint_t mp_hal_ticks_cpu(void) {
	return (mp_uint_t)rtos_get_time() / beken_ms_per_tick();
}

uint64_t mp_hal_time_ns(void) {
	return (uint64_t)rtos_get_time() * 1000000;
}

void mp_hal_get_random(size_t n, void *buf) {
	uint8_t *ptr = (uint8_t *)buf;
#if 0
	uint8_t *end = ptr + n;

	for (; ptr < end; ptr++) {
		*ptr = (uint8_t)bk_rand();
	}
#else
    uint32_t r = 0;
    for (int i = 0; i < n; i++) {
        if ((i & 3) == 0) {
            r = (uint32_t)bk_rand(); // returns 32-bit hardware random number
        }
        *ptr++ = r;
        r >>= 8;
    }
#endif
}

void mp_hal_exceptions(bk_err_t e) {
    switch (e) {
        case BK_ERR_NOT_INIT:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Not Initialized"));
        case BK_ERR_PARAM:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Invalid Param"));
        case BK_ERR_NOT_FOUND:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Not Found"));
        case BK_ERR_NO_MEM:
            mp_raise_OSError(MP_ENOMEM);
        case BK_ERR_TIMEOUT:
            mp_raise_OSError(MP_ETIMEDOUT);
        case BK_ERR_STATE:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Internal State Error"));
        case BK_ERR_TRY_AGAIN:
            mp_raise_OSError(MP_EAGAIN);
        case BK_ERR_BUSY:
            mp_raise_OSError(MP_EBUSY);
        case BK_ERR_NOT_SUPPORT:
            mp_raise_OSError(MP_EOPNOTSUPP);
        case BK_ERR_WIFI_NOT_INIT:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Wifi Not Initialized"));
        case BK_ERR_NETIF_IF:
            mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("Invalid NetIF"));
        default:
		    if (e != BK_OK) {
	            mp_raise_msg_varg(&mp_type_RuntimeError, MP_ERROR_TEXT("Unknown Error -0x%04x"), -e);
		    }
    }
}

