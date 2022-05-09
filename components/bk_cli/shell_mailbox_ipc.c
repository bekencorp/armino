
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "shell_drv.h"
#include "mailbox_channel.h"

typedef struct
{
	u8     chnl_id;

	shell_ipc_rx_t		rx_callback;

} shell_ipc_ext_t;

static bool_t     shell_ipc_init(shell_dev_ipc_t * dev_ipc);
static bool_t     shell_ipc_open(shell_dev_ipc_t * dev_ipc, shell_ipc_rx_t rx_callback);
static u16        shell_ipc_read(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen);
static u16        shell_ipc_write_sync(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen);
static bool_t     shell_ipc_ctrl(shell_dev_ipc_t * dev_ipc, u8 cmd, void *param);
static bool_t     shell_ipc_close(shell_dev_ipc_t * dev_ipc);

static const shell_ipc_drv_t shell_ipc_drv = 
	{
		.init    = shell_ipc_init,
		.open    = shell_ipc_open,
		.read    = shell_ipc_read,
		.write_sync = shell_ipc_write_sync,
		.io_ctrl = shell_ipc_ctrl,
		.close   = shell_ipc_close
	};

static shell_ipc_ext_t dev_ipc_ext = 
	{
		.chnl_id = MB_CHNL_LOG
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

			result = ipc_ext->rx_callback(log_cmd->hdr.cmd, log_cmd->buf, log_cmd->len);

			if(result == 0)
				result = ACK_STATE_FAIL;  // log discarded.
			else
				result = ACK_STATE_COMPLETE;
		}
	}
	else if (cmd_buf->hdr.cmd == MB_CMD_ASSERT_OUT)
	{
		if(ipc_ext->rx_callback != NULL)
		{
			log_cmd_t * log_cmd = (log_cmd_t *)cmd_buf;

			result = ipc_ext->rx_callback(log_cmd->hdr.cmd, log_cmd->buf + 1, log_cmd->len - 1);

			if(result == 0)
				result = ACK_STATE_FAIL;  // log discarded.
			else
				result = ACK_STATE_COMPLETE;

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
	u8    dev_id;
	shell_ipc_ext_t *ipc_ext;

	if(dev_ipc == NULL)
		return bFALSE;

	ipc_ext = (shell_ipc_ext_t *)dev_ipc->dev_ext;
	dev_id = ipc_ext->chnl_id;

	memset(ipc_ext, 0, sizeof(shell_ipc_ext_t));
	ipc_ext->chnl_id = dev_id;

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
static u16 shell_ipc_write_sync(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen)
{
	shell_ipc_ext_t *ipc_ext;
	ipc_ext = (shell_ipc_ext_t *)dev_ipc->dev_ext;

	mb_chnl_cmd_t	mb_cmd_buf;
	user_cmd_t * user_cmd = (user_cmd_t *)&mb_cmd_buf;

	user_cmd->hdr.data = 0;
	user_cmd->hdr.cmd = MB_CMD_USER_INPUT;
	user_cmd->buf = pBuf;
	user_cmd->len = BufLen;

	bk_err_t		ret_code;

	ret_code = mb_chnl_write(ipc_ext->chnl_id, &mb_cmd_buf);

	if(ret_code == BK_OK)
	{
		return BufLen;
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


