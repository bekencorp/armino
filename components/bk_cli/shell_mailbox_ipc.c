
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "shell_drv.h"
#include <driver/mailbox_channel.h>

#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif

typedef struct
{
	u8     chnl_id;
	u8     cpu_id;

	shell_ipc_rx_t		rx_callback;

} shell_ipc_ext_t;

static bool_t     shell_ipc_init(shell_dev_ipc_t * dev_ipc);
static bool_t     shell_ipc_open(shell_dev_ipc_t * dev_ipc, shell_ipc_rx_t rx_callback);
static u16        shell_ipc_read(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen);
static u16        shell_ipc_write_cmd(shell_dev_ipc_t * dev_ipc, mb_chnl_cmd_t * cmd_buf);
static bool_t     shell_ipc_ctrl(shell_dev_ipc_t * dev_ipc, u8 cmd, void *param);
static bool_t     shell_ipc_close(shell_dev_ipc_t * dev_ipc);

static const shell_ipc_drv_t shell_ipc_drv = 
	{
		.init    = shell_ipc_init,
		.open    = shell_ipc_open,
		.read    = shell_ipc_read,
		.write_cmd = shell_ipc_write_cmd,
		.io_ctrl = shell_ipc_ctrl,
		.close   = shell_ipc_close
	};

static shell_ipc_ext_t dev_ipc_ext = 
	{
		.chnl_id = MB_CHNL_LOG,
		.cpu_id  = MAILBOX_CPU1
	};

shell_dev_ipc_t     shell_dev_ipc = 
	{
		.dev_drv = (struct _shell_ipc_drv *)&shell_ipc_drv,
		.dev_type = SHELL_DEV_MAILBOX,
		.dev_ext = &dev_ipc_ext
	};

/* ===============================  internal functions  =========================== */
static void shell_ipc_rx_isr(shell_ipc_ext_t *ipc_ext, mb_chnl_cmd_t *cmd_buf)
{
	u32		result = ACK_STATE_FAIL;

	if(cmd_buf->hdr.cmd == MB_CMD_LOG_OUT)
	{
		if(ipc_ext->rx_callback != NULL)
		{
			log_cmd_t * log_cmd = (log_cmd_t *)cmd_buf;

#if CONFIG_CACHE_ENABLE
			flush_dcache((void *)log_cmd->buf, log_cmd->len);
#endif

			result = ipc_ext->rx_callback(log_cmd->hdr.cmd, log_cmd, ipc_ext->cpu_id);
		}
	}
	else if (cmd_buf->hdr.cmd == MB_CMD_ASSERT_OUT)
	{
		if(ipc_ext->rx_callback != NULL)
		{
			log_cmd_t * log_cmd = (log_cmd_t *)cmd_buf;

#if CONFIG_CACHE_ENABLE
			flush_dcache((void *)log_cmd->buf, log_cmd->len);
#endif
			log_cmd_t    new_log_cmd_buf;

			new_log_cmd_buf.hdr.data = log_cmd->hdr.data;
			new_log_cmd_buf.buf = log_cmd->buf + 1;
			new_log_cmd_buf.len = log_cmd->len - 1;
			new_log_cmd_buf.tag = log_cmd->tag;

			result = ipc_ext->rx_callback(log_cmd->hdr.cmd, &new_log_cmd_buf, ipc_ext->cpu_id);

			/* assert_out must be handled in synchrously, so the buffer is free when return. */
			log_cmd->buf[0] = 0; /* notify the sender that buffer can be freed now. */
		}
	}
	else   /* unknown cmd. */
	{
		result = ACK_STATE_FAIL;
	}

	/* overwrite the cmd_buf->param3 after the ISR handle complete.
	 * return the ack info to caller using the SAME buffer with cmd buffer.
	 *     !!!! [input as param / outpu as result ]  !!!!
	 */
	mb_chnl_ack_t * ack_buf = (mb_chnl_ack_t *)cmd_buf;
	ack_buf->ack_state = result;

	return;
}

/* ===============================  shell ipc driver APIs  =========================== */

static bool_t shell_ipc_init(shell_dev_ipc_t * dev_ipc)
{
	u8    dev_id, cpu_id;
	shell_ipc_ext_t *ipc_ext;

	if(dev_ipc == NULL)
		return bFALSE;

	ipc_ext = (shell_ipc_ext_t *)dev_ipc->dev_ext;
	dev_id = ipc_ext->chnl_id;
	cpu_id = ipc_ext->cpu_id;

	memset(ipc_ext, 0, sizeof(shell_ipc_ext_t));
	ipc_ext->chnl_id = dev_id;
	ipc_ext->cpu_id  = cpu_id;

	return bTRUE;
}

static bool_t shell_ipc_open(shell_dev_ipc_t * dev_ipc, shell_ipc_rx_t rx_callback)
{
	shell_ipc_ext_t *ipc_ext;

	if(dev_ipc == NULL)
		return bFALSE;

	ipc_ext = (shell_ipc_ext_t *)dev_ipc->dev_ext;

	ipc_ext->rx_callback = rx_callback;

	bk_err_t	ret_code = mb_chnl_open(ipc_ext->chnl_id, ipc_ext);

	if(ret_code != BK_OK)
		return bFALSE;
	
	// call chnl driver to register isr callback;
	mb_chnl_ctrl(ipc_ext->chnl_id, MB_CHNL_SET_RX_ISR, (void *)shell_ipc_rx_isr);

	return bTRUE;
}

static u16 shell_ipc_read(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen)
{
	return 0;
}

/* call this after interrupt is DISABLED. */
static u16 shell_ipc_write_cmd(shell_dev_ipc_t * dev_ipc, mb_chnl_cmd_t * cmd_buf)
{
	shell_ipc_ext_t *ipc_ext;
	ipc_ext = (shell_ipc_ext_t *)dev_ipc->dev_ext;

	bk_err_t		ret_code;

	ret_code = mb_chnl_write(ipc_ext->chnl_id, cmd_buf);

	if(ret_code == BK_OK)
	{
		return 1;
	}

	return 0;
}

static bool_t shell_ipc_ctrl(shell_dev_ipc_t * dev_ipc, u8 cmd, void *param)
{
	if(dev_ipc == NULL)
		return bFALSE;

	switch(cmd)
	{
		/*
		case SHELL_IPC_SET_RX_CALLBACK:
			ipc_ext->rx_callback = (shell_ipc_rx_t)param;
			break;
			*/

		default:
			return bFALSE;
			break;
	}

	return bTRUE;
}

static bool_t shell_ipc_close(shell_dev_ipc_t * dev_ipc)
{
	shell_ipc_ext_t *ipc_ext;

	if(dev_ipc == NULL)
		return bFALSE;

	ipc_ext = (shell_ipc_ext_t *)dev_ipc->dev_ext;

	// call chnl driver to register isr callback;
	mb_chnl_ctrl(ipc_ext->chnl_id, MB_CHNL_SET_RX_ISR, NULL);
	mb_chnl_ctrl(ipc_ext->chnl_id, MB_CHNL_SET_TX_ISR, NULL);
	mb_chnl_ctrl(ipc_ext->chnl_id, MB_CHNL_SET_TX_CMPL_ISR, NULL);

	mb_chnl_close(ipc_ext->chnl_id);

	ipc_ext->rx_callback = NULL;

	return bTRUE;
}


