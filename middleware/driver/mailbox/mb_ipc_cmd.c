// Copyright 2020-2022 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>

#include <os/os.h>
#include "mailbox_channel.h"
#include "mb_ipc_cmd.h"
#include "dma_driver.h"

#define MOD_TAG		"IPC"

#define IPC_RSP_CMD_FLAG		0x80
#define IPC_RSP_CMD_MASK		0x7F

#define IPC_RSP_TIMEOUT			10		/* 10ms */
#define IPC_XCHG_DATA_MAX		64

typedef union
{
	struct
	{
		mb_chnl_hdr_t	chnl_hdr;
		void *	cmd_buff;
		u16		cmd_data_len;
	};

	mb_chnl_cmd_t	mb_cmd;
} ipc_cmd_t;

typedef union
{
	struct
	{
		mb_chnl_hdr_t	chnl_hdr;
		void *	rsp_buff;
		u16		rsp_data_len;
	};

	mb_chnl_ack_t	mb_ack;
} ipc_rsp_t;

typedef struct
{
	u8		client_buff[IPC_XCHG_DATA_MAX];
	u8		server_buff[IPC_XCHG_DATA_MAX];
} ipc_chnl_buf_t;

typedef u32 (* ipc_rx_cmd_hdlr_t)(void * chnl_cb, mb_chnl_ack_t *ack_buf);

typedef struct
{
	/* chnl data */
	u8					chnl_inited;
	u8					chnl_id;
	beken_semaphore_t	chnl_sema;

	/* tx cmd data */
	beken_semaphore_t	rsp_sema;
	u8			*	tx_xchg_buff;
	u8				tx_cmd;
	volatile u8		tx_cmd_in_process;
	volatile u8		tx_cmd_failed;
	u32				rsp_buf[IPC_XCHG_DATA_MAX / sizeof(u32)];
	u16				rsp_len;

	/* rx cmd data */
	ipc_rx_cmd_hdlr_t	rx_cmd_handler;
	u8			*	rx_xchg_buff;
	u8				rx_cmd;
	volatile u8		rx_cmd_in_process;
	u32				cmd_buf[IPC_XCHG_DATA_MAX / sizeof(u32)];
	u16				cmd_len;
} ipc_chnl_cb_t;

extern char _swap_start;
static ipc_chnl_buf_t * const ipc_xchg_buf = (ipc_chnl_buf_t *)(&_swap_start);

static void ipc_cmd_rx_isr(ipc_chnl_cb_t *chnl_cb, mb_chnl_cmd_t *cmd_buf)
{
	u32		result = ACK_STATE_FAIL;
	mb_chnl_ack_t * ack_buf = (mb_chnl_ack_t *)cmd_buf;
	ipc_cmd_t     * ipc_cmd = (ipc_cmd_t *)cmd_buf;

	/* chnl_cb->rx_xchg_buff == ipc_cmd->cmd_buff. MUST be true. */

	if(cmd_buf->hdr.cmd & IPC_RSP_CMD_FLAG)  /* ipc rsp from other CPU. */
	{
		if(chnl_cb->tx_cmd_in_process == 0)	/* unsolicited ipc response. */
		{
			goto ipc_cmd_rx_isr_exit;
		}

		if(chnl_cb->tx_cmd != (cmd_buf->hdr.cmd & IPC_RSP_CMD_MASK))
		{
			/* un-matched rpc response. */
			goto ipc_cmd_rx_isr_exit;
		}

		/* communication ok, function is completed. */
		chnl_cb->tx_cmd_failed = 0;
		chnl_cb->tx_cmd_in_process = 0;

		BK_LOGI(MOD_TAG, "IPC_RSP_CMD: %d, %d, %d\r\n", cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);

		if(ipc_cmd->cmd_data_len > sizeof(chnl_cb->rsp_buf))
		{
			/* buffer insufficient, failed. */
			chnl_cb->tx_cmd_failed = 1;
			chnl_cb->rsp_len = 0;
		}
		else
		{
			/* returns ACK_STATE_COMPLETE, 
			   so must copy data from cmd_buff in case that the buffer is released. */
			if((ipc_cmd->cmd_buff != NULL) && (ipc_cmd->cmd_data_len > 0))
			{
				memcpy(chnl_cb->rsp_buf, ipc_cmd->cmd_buff, ipc_cmd->cmd_data_len);

				chnl_cb->rsp_len = ipc_cmd->cmd_data_len;
			}
			else
			{
				chnl_cb->rsp_len = 0;
			}
		}

		rtos_set_semaphore(&chnl_cb->rsp_sema);

		ipc_rsp_t * ipc_rsp = (ipc_rsp_t *)ack_buf;

		/* it is a rsp command, nothing need to be returned. */
		ipc_rsp->rsp_buff = NULL;
		ipc_rsp->rsp_data_len = 0;

		result = ACK_STATE_COMPLETE;

		goto ipc_cmd_rx_isr_exit;
	}
	else
	{
		chnl_cb->rx_cmd = cmd_buf->hdr.cmd;
		chnl_cb->rx_cmd_in_process = 1;

		BK_LOGD(MOD_TAG, "IPC_CMD: %d, %d, %d\r\n", cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);

		if(ipc_cmd->cmd_data_len > sizeof(chnl_cb->cmd_buf))
		{
			result = ACK_STATE_FAIL;
		}
		else
		{
			/* may return ACK_STATE_COMPLETE, 
			   so must copy data from cmd_buff in case that the buffer is released. */
			if((ipc_cmd->cmd_buff != NULL) && (ipc_cmd->cmd_data_len > 0))
			{
				memcpy(chnl_cb->cmd_buf, ipc_cmd->cmd_buff, ipc_cmd->cmd_data_len);

				chnl_cb->cmd_len = ipc_cmd->cmd_data_len;
			}
			else
			{
				chnl_cb->cmd_len = 0;
			}

			result = chnl_cb->rx_cmd_handler(chnl_cb, ack_buf);
		}

		if(result != ACK_STATE_PENDING)
			chnl_cb->rx_cmd_in_process = 0;
	}

ipc_cmd_rx_isr_exit:

	/* overwrite the cmd_buf after the ISR handle complete.
	 * return the ack info to caller using the SAME buffer with cmd buffer.
	 *     !!!! [input as param / output as result ]  !!!!
	 */
	ack_buf->ack_state = result;

	return;

}

static void ipc_cmd_tx_cmpl_isr(ipc_chnl_cb_t *chnl_cb, mb_chnl_ack_t *ack_buf)  /* tx_cmpl_isr */
{
	if(ack_buf->hdr.cmd == chnl_cb->tx_cmd)
	{
		if(chnl_cb->tx_cmd_in_process == 0)	/* RSP_CMD rx_isr may arrive before this tx_cmpl_isr. */
			return;

		/* IPC cmd tx complete. */

		if( (ack_buf->hdr.state & CHNL_STATE_COM_FAIL) 
			|| (ack_buf->ack_state == ACK_STATE_FAIL) )
		{
			chnl_cb->tx_cmd_failed = 1;
			chnl_cb->tx_cmd_in_process  = 0;

			/* communication failed or function failed. */
			rtos_set_semaphore(&chnl_cb->rsp_sema);
		}
		else if(ack_buf->ack_state == ACK_STATE_COMPLETE)
		{
			/* communication ok, function is completed. */
			chnl_cb->tx_cmd_failed = 0;
			chnl_cb->tx_cmd_in_process = 0;

			ipc_rsp_t * ipc_rsp = (ipc_rsp_t *)ack_buf;

			/* chnl_cb->tx_xchg_buff == ipc_rsp->rsp_buff. MUST be true. */

			if(ipc_rsp->rsp_data_len > sizeof(chnl_cb->rsp_buf))
			{
				/* buffer insufficient, failed. */
				chnl_cb->tx_cmd_failed = 1;
				chnl_cb->rsp_len = 0;
			}
			else
			{
				if((ipc_rsp->rsp_buff != NULL) && (ipc_rsp->rsp_data_len > 0))
				{
					memcpy(chnl_cb->rsp_buf, ipc_rsp->rsp_buff, ipc_rsp->rsp_data_len);
					chnl_cb->rsp_len = ipc_rsp->rsp_data_len;
				}
				else
				{
					chnl_cb->rsp_len = 0;
				}
			}
			
			rtos_set_semaphore(&chnl_cb->rsp_sema);
		}
		else
		{
			/* communication ok, function is pending. */
		}

		return;
	}

	/*
	 *   !!!  FAULT  !!!
	 */
	BK_LOGE(MOD_TAG, "Fault in %s,cmd:%d\r\n", __func__, ack_buf->hdr.cmd);

	return;

}

static bk_err_t ipc_chnl_init(u8 client, ipc_chnl_cb_t *chnl_cb, u8 chnl_id, ipc_rx_cmd_hdlr_t rx_handler)
{
	bk_err_t		ret_code;

	if(chnl_cb->chnl_inited)
		return BK_OK;

	memset(chnl_cb, 0, sizeof(ipc_chnl_cb_t));
	chnl_cb->chnl_id = chnl_id;

	if(client)
	{
		/* client rx cmds from server. */
		chnl_cb->tx_xchg_buff = ipc_xchg_buf[chnl_id].client_buff;
		chnl_cb->rx_xchg_buff = ipc_xchg_buf[chnl_id].server_buff;
	}
	else
	{
		/* server rx cmds from client. */
		chnl_cb->tx_xchg_buff = ipc_xchg_buf[chnl_id].server_buff;
		chnl_cb->rx_xchg_buff = ipc_xchg_buf[chnl_id].client_buff;
	}

	ret_code = rtos_init_semaphore_adv(&chnl_cb->chnl_sema, 1, 1);
	if(ret_code != BK_OK)
	{
		return ret_code;
	}

	ret_code = rtos_init_semaphore(&chnl_cb->rsp_sema, 1);
	if(ret_code != BK_OK)
	{
		rtos_deinit_semaphore(&chnl_cb->chnl_sema);

		return ret_code;
	}

	ret_code = mb_chnl_open(chnl_id, chnl_cb);
	if(ret_code != BK_OK)
	{
		rtos_deinit_semaphore(&chnl_cb->chnl_sema);
		rtos_deinit_semaphore(&chnl_cb->rsp_sema);

		return ret_code;
	}

	chnl_cb->rx_cmd_handler = rx_handler;

	mb_chnl_ctrl(chnl_id, MB_CHNL_SET_RX_ISR, (void *)ipc_cmd_rx_isr);
	mb_chnl_ctrl(chnl_id, MB_CHNL_SET_TX_CMPL_ISR, (void *)ipc_cmd_tx_cmpl_isr);

	chnl_cb->chnl_inited = 1;

	return BK_OK;
}

static bk_err_t ipc_send_cmd(ipc_chnl_cb_t *chnl_cb, u8 cmd, u8 *cmd_buf, u16 cmd_len, u8 * rsp_buf, u16 buf_len)
{
	bk_err_t	ret_val = BK_FAIL;
	ipc_cmd_t	ipc_cmd;

	if(!chnl_cb->chnl_inited)
		return BK_FAIL;

	rtos_get_semaphore(&chnl_cb->chnl_sema, BEKEN_WAIT_FOREVER);

	chnl_cb->tx_cmd = cmd;
	chnl_cb->tx_cmd_failed = 0;
	chnl_cb->tx_cmd_in_process = 1;

	/* client uses the CLIENT buffer to exchange info(Cmd/Rsp) with server. */
	/* server uses the SERVER buffer to exchange info(Cmd/Rsp) with client. */
	void * dst_buf = (void *)chnl_cb->tx_xchg_buff;

	do
	{
		if(cmd_len > IPC_XCHG_DATA_MAX)
		{
			ret_val = BK_ERR_PARAM;
			break;
		}

		if(cmd_buf == NULL)
			cmd_len = 0;

		if(cmd_len > 0)
			memcpy(dst_buf, cmd_buf, cmd_len);

		ipc_cmd.chnl_hdr.data = 0;	/* clear hdr. */
		ipc_cmd.chnl_hdr.cmd  = cmd;
		ipc_cmd.cmd_buff      = dst_buf;
		ipc_cmd.cmd_data_len  = cmd_len;

		ret_val = mb_chnl_write(chnl_cb->chnl_id, (mb_chnl_cmd_t *)&ipc_cmd);
		if(ret_val != BK_OK)
		{
			break;
		}

		ret_val = rtos_get_semaphore(&chnl_cb->rsp_sema, IPC_RSP_TIMEOUT);  /* isr_callback will set this semaphore. */
		if(ret_val != BK_OK)
		{
			break;
		}

		chnl_cb->tx_cmd_in_process  = 0;  /* must have been set to 0 by callback. */

		if(chnl_cb->tx_cmd_failed)
		{
			ret_val = BK_FAIL;
			break;
		}

		if((rsp_buf == NULL) || (buf_len == 0))
		{
			ret_val = BK_OK;
			break;
		}

		if(buf_len < chnl_cb->rsp_len)
		{
			ret_val = BK_ERR_PARAM;
			break;
		}

		if(chnl_cb->rsp_len > 0)
			memcpy(rsp_buf, chnl_cb->rsp_buf, chnl_cb->rsp_len);

		ret_val = BK_OK;

	}while(0);

	chnl_cb->tx_cmd_in_process = 0;

	rtos_set_semaphore(&chnl_cb->chnl_sema);

	return ret_val;

}

static ipc_chnl_cb_t	ipc_chnl_cb; // = { .chnl_id = MB_CHNL_HW_CTRL, .chnl_inited = 0 };

typedef struct
{
	u16		res_id;
	u16		cpu_id;
} ipc_res_req_t;

typedef struct
{
	u32		user_id;
	u32		chnl_id;
} ipc_dma_free_t;

#if CONFIG_SLAVE_CORE

/**    ============================      IPC client    ============================   **/

static u32 ipc_client_cmd_handler(ipc_chnl_cb_t *chnl_cb, mb_chnl_ack_t *ack_buf)
{
	/* must NOT change ack_buf->hdr. */

	u32		result = ACK_STATE_FAIL;
	ipc_rsp_t * ipc_rsp = (ipc_rsp_t *)ack_buf;

	/* chnl_cb->rx_xchg_buff == ipc_cmd->cmd_buff == ipc_rsp->rsp_buff. MUST be true. */

	ipc_rsp->rsp_buff = chnl_cb->rx_xchg_buff; //ipc_xchg_buf[chnl_cb->chnl_id].server_buff;

	switch(chnl_cb->rx_cmd)
	{
		case IPC_TEST_CMD:
			if(chnl_cb->cmd_len >= sizeof(u32))
			{
				ipc_rsp->rsp_data_len = sizeof(u32);

				u32 * p_src = (u32 *)chnl_cb->cmd_buf;
				u32 * p_dst = (u32 *)ipc_rsp->rsp_buff;

				*p_dst = (*p_src) + 1;
				
				result = ACK_STATE_COMPLETE;
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_GET_POWER_SAVE_FLAG:
			{
				ipc_rsp->rsp_data_len = sizeof(u32);

				u32 * p_dst = (u32 *)ipc_rsp->rsp_buff;

				// return PS flag to caller.
				*p_dst = 0xAA; //(u32)get_cpu1_ps_flag();

				result = ACK_STATE_COMPLETE;
			}
			break;

		case IPC_SET_CPU1_HEART_RATE:
			if(chnl_cb->cmd_len >= sizeof(u32))
			{
				ipc_rsp->rsp_data_len = 0;

				//u32 * p_src = (u32 *)chnl_cb->cmd_buf;

				// save the param.
				//set_cpu1_heart_rate(*p_src);
				
				result = ACK_STATE_COMPLETE;
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_GET_CPU1_HEART_RATE:
			{
				ipc_rsp->rsp_data_len = sizeof(u32);

				u32 * p_dst = (u32 *)ipc_rsp->rsp_buff;

				// return heart_rate to caller.
				*p_dst = 0xBB; //(u32)get_cpu1_heart_rate();

				result = ACK_STATE_COMPLETE;
			}
			break;

		case IPC_RES_AVAILABLE_INDICATION:
			if(chnl_cb->cmd_len >= sizeof(u16))
			{
				ipc_rsp->rsp_data_len = 0;

				u16  res_id = *((u16 *)chnl_cb->cmd_buf);

				if(amp_res_available(res_id) == BK_OK)
				{
					result = ACK_STATE_COMPLETE;
				}
				else
				{
					result = ACK_STATE_FAIL;
				}
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		default:
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;
	}

	return result;
}

bk_err_t ipc_client_init(void)
{
	return ipc_chnl_init(1, &ipc_chnl_cb, MB_CHNL_HW_CTRL, (ipc_rx_cmd_hdlr_t)ipc_client_cmd_handler);
}

/**    ============================    IPC client end  ============================   **/

bk_err_t ipc_send_power_up(void)
{
	return ipc_send_cmd(&ipc_chnl_cb, IPC_CPU1_POWER_UP_INDICATION, NULL, 0, NULL, 0);
}

bk_err_t ipc_send_heart_beat(u32 param)
{
	return ipc_send_cmd(&ipc_chnl_cb, IPC_CPU1_HEART_BEAT_INDICATION, (u8 *)&param, sizeof(param), NULL, 0);
}

bk_err_t ipc_send_res_acquire_cnt(u16 resource_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list)
{
	ipc_res_req_t	res_req;

	res_req.res_id = resource_id;
	res_req.cpu_id = cpu_id;

	return ipc_send_cmd(&ipc_chnl_cb, IPC_RES_ACQUIRE_CNT, \
						(u8 *)&res_req, sizeof(res_req), (u8 *)cnt_list, sizeof(amp_res_req_cnt_t));
}

bk_err_t ipc_send_res_release_cnt(u16 resource_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list)
{
	ipc_res_req_t	res_req;

	res_req.res_id = resource_id;
	res_req.cpu_id = cpu_id;

	return ipc_send_cmd(&ipc_chnl_cb, IPC_RES_RELEASE_CNT, \
						(u8 *)&res_req, sizeof(res_req), (u8 *)cnt_list, sizeof(amp_res_req_cnt_t));
}

u8 ipc_send_alloc_dma_chnl(u32 user_id)
{
	bk_err_t	ret_val = BK_FAIL;
	u8	dma_chnl_id = -1;

	ret_val = ipc_send_cmd(&ipc_chnl_cb, IPC_ALLOC_DMA_CHNL, \
						(u8 *)&user_id, sizeof(user_id), (u8 *)&dma_chnl_id, sizeof(dma_chnl_id));

	if(ret_val != BK_OK)
		return (u8)(-1);

	return dma_chnl_id;
}

bk_err_t ipc_send_free_dma_chnl(u32 user_id, u8 chnl_id)
{
	bk_err_t	ret_val = BK_FAIL;

	ipc_dma_free_t	dma_free;

	dma_free.user_id = user_id;
	dma_free.chnl_id = chnl_id;

	ret_val = ipc_send_cmd(&ipc_chnl_cb, IPC_FREE_DMA_CHNL, \
						(u8 *)&dma_free, sizeof(dma_free), NULL, 0);

	return ret_val;
}

u32 ipc_send_dma_chnl_user(u8 chnl_id)
{
	bk_err_t	ret_val = BK_FAIL;
	u32		user_id = -1;

	ret_val = ipc_send_cmd(&ipc_chnl_cb, IPC_DMA_CHNL_USER, \
						(u8 *)&chnl_id, sizeof(chnl_id), (u8 *)&user_id, sizeof(user_id));

	if(ret_val != BK_OK)
		return (u32)(-1);

	return user_id;
}

#endif

#ifdef CONFIG_DUAL_CORE

u32 ipc_send_test_cmd(u32 param)
{
	ipc_send_cmd(&ipc_chnl_cb, IPC_TEST_CMD, (u8 *)&param, sizeof(param), (u8 *)&param, sizeof(param));

	return param;
}

bk_err_t ipc_send_available_ind(u16 resource_id)
{
	return ipc_send_cmd(&ipc_chnl_cb, IPC_RES_AVAILABLE_INDICATION, \
						(u8 *)&resource_id, sizeof(resource_id), NULL, 0);
}

#endif

#if CONFIG_MASTER_CORE

/**    ============================      IPC server    ============================   **/

static u32 ipc_server_cmd_handler(ipc_chnl_cb_t *chnl_cb, mb_chnl_ack_t *ack_buf)
{
	/* must NOT change ack_buf->hdr. */

	u32		result = ACK_STATE_FAIL;
	ipc_rsp_t * ipc_rsp = (ipc_rsp_t *)ack_buf;

	/* chnl_cb->rx_xchg_buff == ipc_cmd->cmd_buff == ipc_rsp->rsp_buff. MUST be true. */

	ipc_rsp->rsp_buff = chnl_cb->rx_xchg_buff;

	switch(chnl_cb->rx_cmd)
	{
		case IPC_TEST_CMD:
			if(chnl_cb->cmd_len >= sizeof(u32))
			{
				ipc_rsp->rsp_data_len = sizeof(u32);

				u32 * p_src = (u32 *)chnl_cb->cmd_buf;
				u32 * p_dst = (u32 *)ipc_rsp->rsp_buff;

				*p_dst = (*p_src) + 1;
				
				result = ACK_STATE_COMPLETE;
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_CPU1_POWER_UP_INDICATION:		// cpu1 indication, power up successfully.
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_COMPLETE;

				/* no params, no returns. */

				/* inform modules who care CPU1 state. */
			}
			break;

		case IPC_CPU1_HEART_BEAT_INDICATION:	// cpu1 indication, alive indication.
			// contains the power save flag?
			if(chnl_cb->cmd_len >= sizeof(u32))
			{
				//u32 * p_src = (u32 *)chnl_cb->cmd_buf;

				// save the param.
				//set_cpu1_ps_flag(*p_src);
			}

			/* succeeded anyway but no returns. */
			ipc_rsp->rsp_data_len = 0;
			result = ACK_STATE_COMPLETE;
			break;

		case IPC_RES_ACQUIRE_CNT:
			if(chnl_cb->cmd_len >= sizeof(ipc_res_req_t))
			{
				ipc_res_req_t * res_req = (ipc_res_req_t *)chnl_cb->cmd_buf;
				amp_res_req_cnt_t * res_cnt_list = (amp_res_req_cnt_t *)ipc_rsp->rsp_buff;

				/* call amp_res_acquire_cnt in interrupt disabled state. */
				if(amp_res_acquire_cnt(res_req->res_id, res_req->cpu_id, res_cnt_list) == BK_OK)
				{
					ipc_rsp->rsp_data_len = sizeof(amp_res_req_cnt_t);

					result = ACK_STATE_COMPLETE;
				}
				else
				{
					ipc_rsp->rsp_data_len = 0;
					result = ACK_STATE_FAIL;
				}
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_RES_RELEASE_CNT:
			if(chnl_cb->cmd_len >= sizeof(ipc_res_req_t))
			{
				ipc_res_req_t * res_req = (ipc_res_req_t *)chnl_cb->cmd_buf;
				amp_res_req_cnt_t * res_cnt_list = (amp_res_req_cnt_t *)ipc_rsp->rsp_buff;

				/* call amp_res_release_cnt in interrupt disabled state. */
				if(amp_res_release_cnt(res_req->res_id, res_req->cpu_id, res_cnt_list) == BK_OK)
				{
					ipc_rsp->rsp_data_len = sizeof(amp_res_req_cnt_t);

					result = ACK_STATE_COMPLETE;
				}
				else
				{
					ipc_rsp->rsp_data_len = 0;
					result = ACK_STATE_FAIL;
				}
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_RES_AVAILABLE_INDICATION:
			if(chnl_cb->cmd_len >= sizeof(u16))
			{
				ipc_rsp->rsp_data_len = 0;

				u16  res_id = *((u16 *)chnl_cb->cmd_buf);

				if(amp_res_available(res_id) == BK_OK)
				{
					result = ACK_STATE_COMPLETE;
				}
				else
				{
					result = ACK_STATE_FAIL;
				}
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_ALLOC_DMA_CHNL:
			if(chnl_cb->cmd_len >= sizeof(u32))
			{
				u32  user_id = *((u32 *)chnl_cb->cmd_buf);

				ipc_rsp->rsp_data_len = sizeof(u8);

				u8 * chnl_id = (u8 *)ipc_rsp->rsp_buff;

				extern u8 dma_chnl_alloc(u32 user_id);

				*chnl_id = dma_chnl_alloc(user_id);

				result = ACK_STATE_COMPLETE;
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_FREE_DMA_CHNL:
			if(chnl_cb->cmd_len >= sizeof(ipc_dma_free_t))
			{
				ipc_dma_free_t * free_req = (ipc_dma_free_t *)chnl_cb->cmd_buf;

				extern bk_err_t dma_chnl_free(u32 user_id, dma_id_t chnl_id);

				bk_err_t ret_val = dma_chnl_free(free_req->user_id, (dma_id_t)free_req->chnl_id);

				ipc_rsp->rsp_data_len = 0;

				if(ret_val == BK_OK)
					result = ACK_STATE_COMPLETE;
				else
					result = ACK_STATE_FAIL;
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_DMA_CHNL_USER:
			if(chnl_cb->cmd_len >= sizeof(u8))
			{
				u8    chnl_id = *((u8 *)chnl_cb->cmd_buf);

				ipc_rsp->rsp_data_len = sizeof(u32);

				u32 * user_id = (u32 *)ipc_rsp->rsp_buff;

				extern u32 dma_chnl_user(dma_id_t user_id);

				*user_id = dma_chnl_user((dma_id_t)chnl_id);

				result = ACK_STATE_COMPLETE;
			}
			else
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;

		case IPC_CALL_CMD:
			/* will handle it in RPC channel. */
			break;

		default:
			{
				ipc_rsp->rsp_data_len = 0;
				result = ACK_STATE_FAIL;
			}
			break;
	}

	return result;
}

bk_err_t ipc_server_init(void)
{
	return ipc_chnl_init(0, &ipc_chnl_cb, MB_CHNL_HW_CTRL, (ipc_rx_cmd_hdlr_t)ipc_server_cmd_handler);
}

/**    ============================    IPC server end  ============================   **/

u32 ipc_send_get_ps_flag(void)
{
	u32  param = -1;
	
	ipc_send_cmd(&ipc_chnl_cb, IPC_GET_POWER_SAVE_FLAG, NULL, 0, (u8 *)&param, sizeof(param));

	return param;
}

u32 ipc_send_get_heart_rate(void)
{
	u32  param = -1;
	
	ipc_send_cmd(&ipc_chnl_cb, IPC_GET_CPU1_HEART_RATE, NULL, 0, (u8 *)&param, sizeof(param));

	return param;
}

bk_err_t ipc_send_set_heart_rate(u32 param)
{
	return ipc_send_cmd(&ipc_chnl_cb, IPC_SET_CPU1_HEART_RATE, (u8 *)&param, sizeof(param), NULL, 0);
}

#endif

static ipc_chnl_cb_t		rpc_chnl_cb; // = { .chnl_id = MB_CHNL_RPC, .chnl_inited = 0 };

#if CONFIG_SLAVE_CORE

/**    ============================      RPC client    ============================   **/

bk_err_t rpc_client_init(void)
{
	return ipc_chnl_init(1, &rpc_chnl_cb, MB_CHNL_RPC, (ipc_rx_cmd_hdlr_t)ipc_client_cmd_handler);
}

bk_err_t rpc_client_call(rpc_call_def_t *rpc_param, u16 param_len, rpc_ret_def_t *ret_buf, u16 buf_len)
{
	if(!rpc_chnl_cb.chnl_inited)
		return BK_FAIL;

	bk_err_t	ret_val;

	ret_val = ipc_send_cmd(&rpc_chnl_cb, IPC_CALL_CMD, (u8 *)rpc_param, param_len, (u8 *)ret_buf, buf_len);

	if(ret_val != BK_OK)
		return ret_val;

	if((ret_buf == NULL) || (buf_len == 0))
	{
		return BK_OK;
	}

	if( (rpc_param->call_hdr.mod_id != ret_buf->call_hdr.mod_id) || 
		(rpc_param->call_hdr.mod_id != ret_buf->call_hdr.mod_id) )
	{
		/* un-matched rpc response. */
		return BK_FAIL;
	}

	return BK_OK;

}


/**    ============================    RPC client end  ============================   **/

#endif

#if CONFIG_MASTER_CORE

/**    ============================      RPC server    ============================   **/

static beken_semaphore_t	server_ind_sema;

static void rpc_svr_task( void *para );

static u32 rpc_server_cmd_handler(ipc_chnl_cb_t *chnl_cb, mb_chnl_ack_t *ack_buf)
{
	/* must NOT change ack_buf->hdr. */

	u32		result = ACK_STATE_FAIL;

	switch(chnl_cb->rx_cmd)
	{
		case IPC_CALL_CMD:
			result = ACK_STATE_PENDING;

			/* informs server task to process the call. */
			rtos_set_semaphore(&server_ind_sema);

			break;

		default:
			return ipc_server_cmd_handler(chnl_cb, ack_buf);
			break;
	}

	return result;
}

bk_err_t rpc_server_init(void)
{
	bk_err_t		ret_code;
	beken_thread_t	thread_handle = NULL;

	ret_code = rtos_init_semaphore(&server_ind_sema, 1);
	if(ret_code != BK_OK)
	{
		return ret_code;
	}

	ret_code = ipc_chnl_init(0, &rpc_chnl_cb, MB_CHNL_RPC, (ipc_rx_cmd_hdlr_t)rpc_server_cmd_handler);

	if(ret_code != BK_OK)
	{
		rtos_deinit_semaphore(&server_ind_sema);

		return ret_code;
	}

	ret_code = rtos_create_thread(&thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY, //BK_SYS_TASK_PRIO_7,
							 "rpc_svr",
							 (beken_thread_function_t)rpc_svr_task,
							 4096,
							 0);

	return BK_OK;
}

int rpc_server_listen_cmd(u32 timeout_ms)
{
	bk_err_t	ret_val;

	ret_val = rtos_get_semaphore(&server_ind_sema, timeout_ms);

	if(ret_val == BK_OK)
		return 1;
	else
		return 0;
}

typedef int (*rpc_svr_handler_t)(rpc_call_def_t * call_buf);

typedef struct
{
	u8		rpc_mod_id;
	rpc_svr_handler_t	svr_handler;
} rpc_svr_dispatcher_t;

typedef struct
{
	rpc_call_hdr_t	call_hdr;
	bk_err_t		ret_val;		/* return fail code. */ /* struct of api_ret_data_t */
} rpc_ret_fail_t;

extern int bk_gpio_api_svr(rpc_call_def_t * call_buf);
extern int bk_dma_api_svr(rpc_call_def_t * call_buf);

static rpc_svr_dispatcher_t    svr_dispatcher[] =
{
	{RPC_MOD_GPIO,     bk_gpio_api_svr   },
	{RPC_MOD_DMA,      bk_dma_api_svr    },
};

void rpc_server_handle_cmd(void)
{
	int		rpc_handled = 0;
	int		i;

	if(rpc_chnl_cb.rx_cmd_in_process == 0)  /* no rpc call cmd to handle. */
		return;

	rpc_call_def_t *rpc_call = (rpc_call_def_t *)rpc_chnl_cb.cmd_buf;

	for(i = 0; i < ARRAY_SIZE(svr_dispatcher); i++)
	{
		if(rpc_call->call_hdr.mod_id == svr_dispatcher[i].rpc_mod_id)
		{
			rpc_handled = svr_dispatcher[i].svr_handler(rpc_call);
		}
	}

	if(rpc_handled == 0)  /* rpc api not handled. */
	{
		rpc_ret_fail_t	rsp_buf;

		rsp_buf.call_hdr.call_id = rpc_call->call_hdr.call_id;
		rsp_buf.call_hdr.data_len = sizeof(rsp_buf) - sizeof(rsp_buf.call_hdr);
		rsp_buf.ret_val = BK_ERR_NOT_SUPPORT;

		rpc_server_rsp((rpc_ret_def_t *)&rsp_buf, sizeof(rsp_buf)); // return fail info.
	}

	return;
}

bk_err_t rpc_server_rsp(rpc_ret_def_t *rsp_param, u16 param_len)
{
	if(!rpc_chnl_cb.chnl_inited)
		return BK_FAIL;

	if(rpc_chnl_cb.rx_cmd_in_process == 0)
		return BK_FAIL;

	bk_err_t	ret_val;

	ret_val = ipc_send_cmd(&rpc_chnl_cb, IPC_CALL_CMD | IPC_RSP_CMD_FLAG, (u8 *)rsp_param, param_len, NULL, 0);

	rpc_chnl_cb.rx_cmd_in_process  = 0;

	return ret_val;

}

static void rpc_svr_task( void *para )
{
	// rpc_server_init

	while(1)
	{
		rpc_server_listen_cmd(BEKEN_WAIT_FOREVER);
		rpc_server_handle_cmd();
	}
}

/**    ============================    RPC server end  ============================   **/

#endif

