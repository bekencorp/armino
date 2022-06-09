
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "shell_drv.h"

#define TX_QUEUE_LEN     8
#define RX_BUFF_SIZE     160
#define ECHO_BUFF_SIZE   64


typedef struct
{
	u8    * packet;
	u16     len;
	u16     tag;
} dev_tx_packet_t;


typedef struct
{
	u8     uart_id;

	/* ========  TX channel   ======= */
	/* tx queue */
	dev_tx_packet_t    tx_list[TX_QUEUE_LEN];
	u16      list_out_idx;
	u16      list_in_idx;

	/* currently tx packet info */
	u8    *cur_packet;
	u16    packet_len;
	u16    packet_tag;
	u16    packet_tx_len;

	u8     tx_stopped;

	u8     echo_buff[ECHO_BUFF_SIZE];
	u8     echo_wr_idx;
	u8     echo_rd_idx;

	tx_complete_t   tx_complete_callback;

	/* ========  RX channel   ======= */
	/* rx buffer */
	u8     rx_buff[RX_BUFF_SIZE];
	u16    rx_buff_wr_idx;
	u16    rx_buff_rd_idx;

	u8     rx_stopped;

	rx_indicate_t	rx_indicate_callback;

} shell_uart_ext_t;

static bool_t     shell_uart_init(shell_dev_t * shell_dev);
static bool_t     shell_uart_open(shell_dev_t * shell_dev, tx_complete_t tx_callback, rx_indicate_t rx_callback);
static u16        shell_uart_write_async(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen, u16 Tag);
static u16        shell_uart_read(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen);
static u16        shell_uart_write_sync(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen);
static u16        shell_uart_write_echo(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen);
static bool_t     shell_uart_ctrl(shell_dev_t * shell_dev, u8 cmd, void *param);
static bool_t     shell_uart_close(shell_dev_t * shell_dev);

static const shell_dev_drv_t shell_uart_drv =
	{
		.init    = shell_uart_init,
		.open    = shell_uart_open,
		.write_async  = shell_uart_write_async,
		.read    = shell_uart_read,
		.write_sync = shell_uart_write_sync,
		.write_echo = shell_uart_write_echo,
		.io_ctrl = shell_uart_ctrl,
		.close   = shell_uart_close
	};

static shell_uart_ext_t uart1_ext =
	{
		.uart_id = CONFIG_UART_PRINT_PORT
	};

shell_dev_t     shell_uart =
	{
		.dev_drv = (struct _shell_dev_drv *)&shell_uart_drv,
		.dev_type = SHELL_DEV_UART,
		.dev_ext = &uart1_ext
	};

static shell_uart_ext_t uart3_ext =
	{
		.uart_id = UART_ID_2
	};

shell_dev_t     shell_uart3 =
	{
		.dev_drv = (struct _shell_dev_drv *)&shell_uart_drv,
		.dev_type = SHELL_DEV_UART,
		.dev_ext = &uart3_ext
	};

/* ===============================  internal functions  =========================== */

static void shell_uart_rx_isr(int uartn, shell_uart_ext_t *uart_ext)
{
	u16   free_buff_len, rx_cnt = 0;
	u8    rx_data;
	int   ret = -1;

	(void)uartn;


	if(uart_ext->rx_buff_wr_idx >= uart_ext->rx_buff_rd_idx)
	{
	    free_buff_len = RX_BUFF_SIZE - uart_ext->rx_buff_wr_idx + uart_ext->rx_buff_rd_idx;
	}
	else
	{
		free_buff_len = uart_ext->rx_buff_rd_idx - uart_ext->rx_buff_wr_idx;
	}

	while(bTRUE)  /* read all data from rx-FIFO. */
	{
		ret = uart_read_byte_ex(uart_ext->uart_id, &rx_data);
		if (ret == -1)
			break;

		rx_cnt++;

		/* rx_buff_wr_idx == rx_buff_rd_idx means empty, so reserve one byte. */
		if(rx_cnt < free_buff_len)  /* reserved one byte space. */
		{
			uart_ext->rx_buff[uart_ext->rx_buff_wr_idx] = rx_data;

			uart_ext->rx_buff_wr_idx = (uart_ext->rx_buff_wr_idx + 1) % RX_BUFF_SIZE;

		}
		else
		{
			/* discard rx-data, rx overflow. */
			uart_ext->rx_stopped = 1; //  bTRUE; // rx overflow, disable rx interrupt to stop rx.
		}
	}

	if(uart_ext->rx_indicate_callback != NULL)
	{
		uart_ext->rx_indicate_callback();
	}
}

static void shell_uart_tx_isr(int uartn, shell_uart_ext_t *uart_ext)
{
	int   ret;
	(void)uartn;

	while(bTRUE)  /* write data to tx-FIFO. */
	{
		ret = uart_write_ready(uart_ext->uart_id);
		if(ret != 0)
			break;

		/* previous packet tx complete, check ECHO before new packet. */
		if((uart_ext->cur_packet == NULL) || (uart_ext->packet_len == 0))
		{
			if(uart_ext->echo_rd_idx != uart_ext->echo_wr_idx) /* tx echo firstly. */
			{
				uart_write_byte(uart_ext->uart_id, uart_ext->echo_buff[uart_ext->echo_rd_idx]);

				uart_ext->echo_rd_idx = (uart_ext->echo_rd_idx + 1) % ECHO_BUFF_SIZE;

				continue;  /* continue to ECHO next byte to tx-FIFO. */
			}
			else
			{
				if(uart_ext->list_out_idx != uart_ext->list_in_idx)
				{
					uart_ext->cur_packet = uart_ext->tx_list[uart_ext->list_out_idx].packet;
					uart_ext->packet_len = uart_ext->tx_list[uart_ext->list_out_idx].len;
					uart_ext->packet_tag = uart_ext->tx_list[uart_ext->list_out_idx].tag;
					uart_ext->packet_tx_len = 0;
				}
				else
				{
					/* all packets tx complete. */
					/* disable tx interrupt ? */  // disable TX firstly, then set tx_stopped to 1.
					bk_uart_disable_tx_interrupt(uart_ext->uart_id);

					uart_ext->tx_stopped = 1; /* bTRUE;*/  /* all data tranferred, tx stopped.*/
					break;
				}
			}
		}

		if(uart_ext->packet_tx_len < uart_ext->packet_len)
		{
			uart_write_byte(uart_ext->uart_id, uart_ext->cur_packet[uart_ext->packet_tx_len]);

			uart_ext->packet_tx_len++;

			continue;  /* continue to TX next byte to tx-FIFO. */
		}
		else
		{
			/* sent the whole packet, notify app. */
			if(uart_ext->tx_complete_callback != NULL)
			{
				uart_ext->tx_complete_callback(uart_ext->cur_packet, uart_ext->packet_tag);
			}

			uart_ext->cur_packet = NULL;
			uart_ext->packet_len = 0;
			uart_ext->packet_tx_len = 0;

			/* to next packet. */
			uart_ext->list_out_idx = (uart_ext->list_out_idx + 1) % TX_QUEUE_LEN;

			continue;   /* continue to TX next packet. */
		}
	}

}

static void shell_uart_flush(shell_uart_ext_t *uart_ext)
{
	int   ret;

	while(uart_ext->tx_stopped == 0) /* log tx pending. */
	{
		ret = uart_write_ready(uart_ext->uart_id);
		if(ret == BK_OK)
		{
			shell_uart_tx_isr(uart_ext->uart_id, uart_ext);
		}
	}
}

static void shell_uart_tx_trigger(shell_uart_ext_t *uart_ext)
{
	if(uart_ext->tx_stopped == 0)
		return;

	uart_ext->tx_stopped = 0;   // set tx_stopped to 0 firstly, then enable TX.

	bk_uart_enable_tx_interrupt(uart_ext->uart_id);
}

/* ===============================  shell uart driver APIs  =========================== */

static bool_t shell_uart_init(shell_dev_t * shell_dev)
{
	u8    uart_id;
	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return bFALSE;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;
	uart_id = uart_ext->uart_id;

	memset(uart_ext, 0, sizeof(shell_uart_ext_t));
	uart_ext->rx_stopped = 0;
	uart_ext->tx_stopped = 1;
	uart_ext->uart_id = uart_id;

	return bTRUE;
}

static bool_t shell_uart_open(shell_dev_t * shell_dev, tx_complete_t tx_callback, rx_indicate_t rx_callback)
{
	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return bFALSE;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	uart_ext->tx_complete_callback = tx_callback;
	uart_ext->rx_indicate_callback = rx_callback;

	bk_uart_disable_sw_fifo(uart_ext->uart_id);

	// call uart driver to register isr callback;
	bk_uart_register_rx_isr(uart_ext->uart_id, (uart_isr_t)shell_uart_rx_isr, uart_ext);
	bk_uart_register_tx_isr(uart_ext->uart_id, (uart_isr_t)shell_uart_tx_isr, uart_ext);

	bk_uart_enable_rx_interrupt(uart_ext->uart_id);

	return bTRUE;
}

static u16 shell_uart_write_async(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen, u16 Tag)
{
	u16   free_items;
	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return 0;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	if((pBuf == NULL) /*|| (BufLen == 0)*/)
		return 0;

	/* enqueue pBuf even if BufLen is 0, upper layer need tx-complete-callback to free this pBuf. */

	if(uart_ext->list_out_idx > uart_ext->list_in_idx)
		free_items = uart_ext->list_out_idx - uart_ext->list_in_idx;
	else
		free_items = TX_QUEUE_LEN - uart_ext->list_in_idx + uart_ext->list_out_idx;

	/* list_out_idx == list_in_idx means empty, so reserved one item. */
	if(free_items > 1)
	{
		uart_ext->tx_list[uart_ext->list_in_idx].packet = pBuf;
		uart_ext->tx_list[uart_ext->list_in_idx].len  = BufLen;
		uart_ext->tx_list[uart_ext->list_in_idx].tag  = Tag;

		uart_ext->list_in_idx = (uart_ext->list_in_idx + 1) % TX_QUEUE_LEN;

		shell_uart_tx_trigger(uart_ext);

		return 1;
	}
	else
	{
		return 0;
	}
}

static u16 shell_uart_read(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen)
{
	u16		read_cnt = 0;

	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return 0;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	if((pBuf == NULL) || (BufLen == 0))
		return 0;

	while(uart_ext->rx_buff_rd_idx != uart_ext->rx_buff_wr_idx)
	{
		pBuf[read_cnt] = uart_ext->rx_buff[uart_ext->rx_buff_rd_idx];

		uart_ext->rx_buff_rd_idx = (uart_ext->rx_buff_rd_idx + 1) % RX_BUFF_SIZE;

		read_cnt++;
		if(read_cnt >= BufLen)
			break;
	}

	if((uart_ext->rx_stopped == 1) && (read_cnt > 0))
	{
		uart_ext->rx_stopped = 0;  // set rx_stopped to 0 firstly, then enable RX.
	}

	return read_cnt;

}

/* call this after interrupt is DISABLED. */
static u16 shell_uart_write_sync(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen)
{
	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return 0;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	if((pBuf == NULL) || (BufLen == 0))
		return 0;

	u16  wr_cnt = 0;

	while(bTRUE)  /* write data to tx-FIFO. */
	{
		uart_write_byte(uart_ext->uart_id, *pBuf);  // it is macro define, do NOT use *pBuf++;

		pBuf++; wr_cnt++;

		if(wr_cnt >= BufLen)
			break;
	}

	return BufLen;
}

static u16 shell_uart_write_echo(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen)
{
	u16    free_buff_len, wr_cnt = 0;

	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return 0;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	if((pBuf == NULL) || (BufLen == 0))
		return 0;

	if(uart_ext->echo_wr_idx >= uart_ext->echo_rd_idx)
    {
	    free_buff_len = ECHO_BUFF_SIZE - uart_ext->echo_wr_idx + uart_ext->echo_rd_idx;
	}
	else
	{
		free_buff_len = uart_ext->echo_rd_idx - uart_ext->echo_wr_idx;
	}

	/* echo_wr_idx == echo_rd_idx means empty, so reserved one byte. */
	while(free_buff_len > 1)
	{
		uart_ext->echo_buff[uart_ext->echo_wr_idx] = pBuf[wr_cnt];
		uart_ext->echo_wr_idx = (uart_ext->echo_wr_idx + 1) % ECHO_BUFF_SIZE;

		free_buff_len--;

		wr_cnt++;
		if(wr_cnt >= BufLen)
			break;
	}

	if(wr_cnt > 0)
		shell_uart_tx_trigger(uart_ext);

	return wr_cnt;

}

static bool_t shell_uart_ctrl(shell_dev_t * shell_dev, u8 cmd, void *param)
{
	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return bFALSE;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	switch(cmd)
	{
		case SHELL_IO_CTRL_GET_STATUS:
			if(param == NULL)
				return bFALSE;

			u16   free_items;

			if(uart_ext->list_out_idx > uart_ext->list_in_idx)
				free_items = uart_ext->list_out_idx - uart_ext->list_in_idx;
			else
				free_items = TX_QUEUE_LEN - uart_ext->list_in_idx + uart_ext->list_out_idx;

			if(free_items > 1)
				*((u16 *)param) = free_items - 1;
			else
				*((u16 *)param) = 0;

			break;

		case SHELL_IO_CTRL_RX_RESET:
			uart_ext->rx_buff_rd_idx = 0;
			uart_ext->rx_buff_wr_idx = 0;
			break;

		case SHELL_IO_CTRL_TX_RESET:
			uart_ext->list_out_idx = 0;
			uart_ext->list_in_idx  = 0;

			uart_ext->cur_packet = NULL;
			uart_ext->packet_len = 0;
			uart_ext->packet_tx_len = 0;

			break;

		case SHELL_IO_CTRL_FLUSH:
			shell_uart_flush(uart_ext);
			break;

		default:
			return bFALSE;
			break;
	}

	return bTRUE;
}

static bool_t shell_uart_close(shell_dev_t * shell_dev)
{
	shell_uart_ext_t *uart_ext;

	if(shell_dev == NULL)
		return bFALSE;

	uart_ext = (shell_uart_ext_t *)shell_dev->dev_ext;

	// call uart driver to register isr callback;
	bk_uart_register_rx_isr(uart_ext->uart_id, NULL, NULL);
	bk_uart_register_tx_isr(uart_ext->uart_id, NULL, NULL);

	uart_ext->tx_complete_callback = NULL;
	uart_ext->rx_indicate_callback = NULL;

	return bTRUE;
}


