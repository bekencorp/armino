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
#include "mailbox_driver_base.h"

#define MB_PHY_CMD_CHNL		(MAILBOX_BOX0)
#define MB_PHY_ACK_CHNL		(MAILBOX_BOX1)

#define CHNL_STATE_BUSY		1
#define CHNL_STATE_ILDE		0

typedef struct
{
	u8		tx_state;			/* physical channel tx state. */
	u8		tx_seq;				/* physical channel tx sequence. */
	u8		tx_log_chnl_id;		/* logical channel id. */
	u32		rx_fault_cnt;
	u32		tx_fault_cnt;
} mb_phy_chnl_cb_t;


#define CHNL_CTRL_MASK			0xF
/*
 *  there are 2 boxes in one MAILBOXn HW, 
 *  but no way to know which box this msg is from in current mailbox_driver design.
 *  use the CHNL_CTRL_ACK_BOX bit in the msg hdr.ctrl to distinguish where it is from.
 *  when CHNL_CTRL_ACK_BOX is set, it means from ack box ( MAILBOXn_BOX1 ).
 */
#define CHNL_CTRL_ACK_BOX		0x01

#define CHNL_CTRL_SYNC_TX		0x02

typedef union
{
	struct
	{
		u32		cmd            : 8;
		u32		state          : 4;
		u32		ctrl           : 4;
		u32		tx_seq         : 8;
		u32		logical_chnl   : 8;
	} ;
	u32		data;
} phy_chnnl_hdr_t;

typedef struct
{
	phy_chnnl_hdr_t	hdr;

	u32		param1;
	u32		param2;
	u32		param3;
} mb_phy_chnl_cmd_t;

typedef struct
{
	phy_chnnl_hdr_t	hdr;

	u32		data1;
	u32		data2;
	u32		data3;
} mb_phy_chnl_ack_t;

typedef union
{
	phy_chnnl_hdr_t		phy_ch_hdr;
	mb_phy_chnl_cmd_t	phy_ch_cmd;
	mb_phy_chnl_ack_t	phy_ch_ack;
	mailbox_data_t		mb_data;
} mb_phy_chnl_union_t;

typedef struct
{
	u8				log_chnl_id;		/* logical channel id. */
	u8				tx_state;			/* logical channel tx state. */
	u8				in_used;
	chnl_rx_isr_t		rx_isr;
	chnl_tx_isr_t		tx_isr;
	chnl_tx_cmpl_isr_t	tx_cmpl_isr;
	void *				isr_param;
	mailbox_data_t		chnnl_tx_buff;		/* logical channel tx buffer. */
} mb_log_chnl_cb_t;

static mb_phy_chnl_cb_t		phy_chnl_cb;	
static mb_log_chnl_cb_t		log_chnl_cb[MB_LOG_CHNL_NUM];

static u8				mb_chnnl_init_ok = 0;

/* =====================      physical channel functions      ==================*/

static u8 mb_phy_chnl_tx_cmd(u8 log_chnl)
{
	mb_phy_chnl_cmd_t	* cmd_ptr;
	bk_err_t		ret_code;
	u16 			chnl_type;

	if(log_chnl >= MB_LOG_CHNL_NUM)
		return 1;

	phy_chnl_cb.tx_seq++;
	phy_chnl_cb.tx_log_chnl_id = log_chnl;

	cmd_ptr = (mb_phy_chnl_cmd_t *)&log_chnl_cb[log_chnl].chnnl_tx_buff;

	cmd_ptr->hdr.logical_chnl = log_chnl;
	cmd_ptr->hdr.tx_seq = phy_chnl_cb.tx_seq;
	cmd_ptr->hdr.ctrl  = 0;
	cmd_ptr->hdr.state = 0;

	chnl_type = MB_PHY_CMD_CHNL;

	ret_code = bk_mailbox_send(&log_chnl_cb[log_chnl].chnnl_tx_buff, SRC_CPU, DST_CPU, (void *)&chnl_type);

	if(ret_code != BK_OK)
	{
		phy_chnl_cb.tx_fault_cnt++;

		return 2;
	}

	log_chnl_cb[log_chnl].tx_state = CHNL_STATE_ILDE;

	if(log_chnl_cb[log_chnl].tx_isr != NULL)
	{
		log_chnl_cb[log_chnl].tx_isr(log_chnl_cb[log_chnl].isr_param);  	/* phy_chnl is BUSY now, tx_isr will not trigger phy_chnl_start_tx. */
	}

	return 0;
}

static void mb_phy_chnl_rx_ack_isr(mb_phy_chnl_ack_t *ack_ptr)
{
	u8		log_chnl;
	u8		ret_code;

	log_chnl = ack_ptr->hdr.logical_chnl;
	
	if( (log_chnl != phy_chnl_cb.tx_log_chnl_id) || 
		(ack_ptr->hdr.tx_seq != phy_chnl_cb.tx_seq) )
	{
		phy_chnl_cb.rx_fault_cnt++;

		return;
	}

	if(log_chnl_cb[log_chnl].tx_cmpl_isr != NULL)
	{
		/* clear following header members. */
		ack_ptr->hdr.logical_chnl = 0;
		ack_ptr->hdr.tx_seq       = 0;
		ack_ptr->hdr.ctrl         = 0;

		/* hdr.state, hdr.cmd these 2 members keep untouched. */

		log_chnl_cb[log_chnl].tx_cmpl_isr(log_chnl_cb[log_chnl].isr_param, (mb_chnl_ack_t *)ack_ptr);
	}

	for(log_chnl = 0; log_chnl < MB_LOG_CHNL_NUM; log_chnl++)  /* priority descended search. */
	{
		if(log_chnl_cb[log_chnl].tx_state != CHNL_STATE_ILDE)
			break;
	}

	if(log_chnl >= MB_LOG_CHNL_NUM)
	{
		phy_chnl_cb.tx_state = CHNL_STATE_ILDE;
		return;
	}

	ret_code = mb_phy_chnl_tx_cmd(log_chnl);

	if(ret_code != 0)
	{
		phy_chnl_cb.tx_state = CHNL_STATE_ILDE;
		return;
	}

	return;
	
}

static void mb_phy_chnl_rx_cmd_isr(mb_phy_chnl_cmd_t *cmd_ptr)
{
	phy_chnnl_hdr_t  chnl_hdr;
	u8		log_chnl = cmd_ptr->hdr.logical_chnl;
	bk_err_t	ret_code;
	u16 		chnl_type;

	chnl_hdr.data = cmd_ptr->hdr.data;

	if(log_chnl >= MB_LOG_CHNL_NUM)
	{
		phy_chnl_cb.rx_fault_cnt++;

		return;
	}

	if(log_chnl_cb[log_chnl].rx_isr != NULL)
	{
		/* clear all other hdr members except hdr.cmd. */
		cmd_ptr->hdr.logical_chnl = 0;
		cmd_ptr->hdr.tx_seq       = 0;
		cmd_ptr->hdr.ctrl         = 0;
		cmd_ptr->hdr.state        = 0;

		log_chnl_cb[log_chnl].rx_isr(log_chnl_cb[log_chnl].isr_param, (mb_chnl_cmd_t *)cmd_ptr);

		/* !!!! cmd_ptr buffer now contains ACK data !!!!. */
	}
	else
	{
		chnl_hdr.state |= CHNL_STATE_COM_FAIL;		/* cmd NO target app, it is an ACK bit to peer CPU. */
	}

	if(chnl_hdr.ctrl & CHNL_CTRL_SYNC_TX)
	{
		/* sync tx cmd, do NOT send ACK. */
		return;
	}

	/* mb_phy_chnl_tx_ack. */

	/* RE-USE the cmd buffer for ACK !!! */
	cmd_ptr->hdr.data  = chnl_hdr.data;
	cmd_ptr->hdr.ctrl |= CHNL_CTRL_ACK_BOX;			/* ACK msg, use the ACK channel.  */

	if(cmd_ptr->hdr.ctrl & CHNL_CTRL_ACK_BOX)
		chnl_type = MB_PHY_ACK_CHNL;
	else
		chnl_type = MB_PHY_CMD_CHNL;

	ret_code = bk_mailbox_send((mailbox_data_t *)cmd_ptr, SRC_CPU, DST_CPU, (void *)&chnl_type);	/* mb_phy_chnl_tx_ack. */

	if(ret_code != BK_OK)
	{
		phy_chnl_cb.tx_fault_cnt++;

		return;
	}

	return;
}

static void mb_phy_chnl_rx_isr(mailbox_data_t * mb_data)
{
	mb_phy_chnl_union_t	rx_data;

	rx_data.mb_data.param0 = mb_data->param0;
	rx_data.mb_data.param1 = mb_data->param1;
	rx_data.mb_data.param2 = mb_data->param2;
	rx_data.mb_data.param3 = mb_data->param3;
	/* the following process will damage the input parameter, 
	so pass in the pointer of copied parameter instad of the original. */

	/*
	 *  there are 2 boxes in one MAILBOXn HW, 
	 *  but no way to know which box this msg is from in current mailbox_driver design.
	 *  so use the CHNL_CTRL_ACK_BOX bit in the msg hdr.ctrl to distinguish where it is from.
	 *  when CHNL_CTRL_ACK_BOX is set, it means from ack box ( MAILBOXn_BOX1 ).
	 */
	if(rx_data.phy_ch_hdr.ctrl & CHNL_CTRL_ACK_BOX)		/* rx ack. */
	{
		mb_phy_chnl_rx_ack_isr(&rx_data.phy_ch_ack);
	}
	else		/* rx cmd. */
	{
		mb_phy_chnl_rx_cmd_isr(&rx_data.phy_ch_cmd);
	}
}

static void mb_phy_chnl_start_tx(u8 log_chnl)
{
	u32  	int_mask;
	u8		ret_code;

	int_mask = rtos_disable_int();

	if(phy_chnl_cb.tx_state == CHNL_STATE_ILDE)
	{
		phy_chnl_cb.tx_state = CHNL_STATE_BUSY;		/* MUST set channel state to BUSY firstly. */
		/* start_tx->tx_cmd->tx_isr callback->mb_chnl_write->start_tx, it is a loop. 
		   break the loop by setting the phy_chnl_cb.tx_state to busy. */

		ret_code = mb_phy_chnl_tx_cmd(log_chnl);

		if(ret_code != 0)
		{
			phy_chnl_cb.tx_state = CHNL_STATE_ILDE;
		}
	}
	
	rtos_enable_int(int_mask);

	return;
}

static bk_err_t mb_phy_chnl_tx_cmd_sync(u8 log_chnl, mb_phy_chnl_cmd_t *cmd_ptr)
{
	bk_err_t		ret_code;
	u16 			chnl_type;

	cmd_ptr->hdr.logical_chnl = log_chnl;
	cmd_ptr->hdr.tx_seq = 0;
	cmd_ptr->hdr.ctrl  = CHNL_CTRL_SYNC_TX;
	cmd_ptr->hdr.state = 0;

	chnl_type = MB_PHY_CMD_CHNL;

	/* 
	 * can't wait 'phy_chnl_cb.tx_state' to be CHNL_STATE_ILDE here,
	 * 'phy_chnl_cb.tx_state' is set to CHNL_STATE_ILDE in interrupt callback.
	 * but the interrupt may be disabled when this API is called.
	 *    wait physical channel HW to be IDLE by <POLLing> !!
	 */
	while(1)
	{
		ret_code = bk_mailbox_send((mailbox_data_t *)cmd_ptr, SRC_CPU, DST_CPU, (void *)&chnl_type);

		if(ret_code != BK_ERR_MAILBOX_TIMEOUT)
		{
			break;
		}
	}

	return ret_code;
}

/* =====================      logical channel APIs      ==================*/
/*
  * init logical chnanel module.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_init(void)
{
	bk_err_t		ret_code;
	int				i;

	if(mb_chnnl_init_ok)
	{
		return BK_OK;
	}

	memset(&phy_chnl_cb, 0, sizeof(phy_chnl_cb));
	phy_chnl_cb.tx_state = CHNL_STATE_ILDE;

	memset(&log_chnl_cb, 0, sizeof(log_chnl_cb));
	for(i = 0; i < MB_LOG_CHNL_NUM; i++)
	{
		log_chnl_cb[i].log_chnl_id = i;
		log_chnl_cb[i].tx_state    = CHNL_STATE_ILDE;
	}

	ret_code = bk_mailbox_init();
	if(ret_code != BK_OK)
	{
		return ret_code;
	}

	ret_code = bk_mailbox_recv_callback_register(DST_CPU, SRC_CPU, mb_phy_chnl_rx_isr);
	if(ret_code != BK_OK)
	{
		return ret_code;
	}

	mb_chnnl_init_ok = 1;

	return BK_OK;
}

/*
  * open logical chnanel.
  * input:
  *     log_chnl  : logical channel id to open.
  *     callback_param : param passsed to all callbacks.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_open(u8 log_chnl, void * callback_param)
{
	if(!mb_chnnl_init_ok)	/* if driver is not initialized. */
	{
		bk_err_t		ret_code;

		ret_code = mb_chnl_init();

		if(ret_code != BK_OK)
		{
			return ret_code;
		}
	}

	if(log_chnl >= MB_LOG_CHNL_NUM)
		return BK_ERR_PARAM;

	if(log_chnl_cb[log_chnl].in_used)
		return BK_ERR_OPEN;

	log_chnl_cb[log_chnl].in_used = 1;		/* chnl in used. */
	log_chnl_cb[log_chnl].isr_param = callback_param;

	return BK_OK;
}

/*
  * close logical chnanel.
  * input:
  *     log_chnl  : logical channel id to close.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_close(u8 log_chnl)
{
	if(log_chnl >= MB_LOG_CHNL_NUM)
		return BK_ERR_PARAM;

	if(log_chnl_cb[log_chnl].in_used == 0)
		return BK_ERR_STATE;

	log_chnl_cb[log_chnl].in_used = 0;
	log_chnl_cb[log_chnl].tx_state = CHNL_STATE_ILDE;
	log_chnl_cb[log_chnl].rx_isr = NULL;
	log_chnl_cb[log_chnl].tx_isr = NULL;
	log_chnl_cb[log_chnl].tx_cmpl_isr = NULL;

	return BK_OK;
}

/*
  * read from logical chnanel.
  * input:
  *     log_chnl     : logical channel id to read.
  *     read_buf       : buffer to receive channel cmd data.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_read(u8 log_chnl, mb_chnl_cmd_t * read_buf)
{
	return BK_ERR_NOT_SUPPORT;
}

/*
  * write to logical chnanel.
  * input:
  *     log_chnl     : logical channel id to write.
  *     cmd_buf     : buffer of channel cmd data.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_write(u8 log_chnl, mb_chnl_cmd_t * cmd_buf)
{
	u16		write_len;

	if(log_chnl >= MB_LOG_CHNL_NUM)
		return BK_ERR_PARAM;

	if(log_chnl_cb[log_chnl].in_used == 0)
		return BK_ERR_STATE;

	if(log_chnl_cb[log_chnl].tx_state != CHNL_STATE_ILDE)
		return BK_ERR_BUSY;

	write_len = sizeof(mailbox_data_t) > sizeof(mb_chnl_cmd_t) ? sizeof(mb_chnl_cmd_t) : sizeof(mailbox_data_t);

	memcpy(&log_chnl_cb[log_chnl].chnnl_tx_buff, cmd_buf, write_len);

	/* set to BUSY means there is data in tx-buff. mb_phy_chnl_rx_ack_isr will get it to send. */
	log_chnl_cb[log_chnl].tx_state = CHNL_STATE_BUSY;   /* MUST set to BUSY after data was copied. */

	mb_phy_chnl_start_tx(log_chnl);

	return BK_OK;
}

/*
  * logical chnanel misc io (set/get param).
  * input:
  *     log_chnl     : logical channel id to set/get param.
  *     cmd          : control command for logical channel.
  *     param      :  parameter of the command.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_ctrl(u8 log_chnl, u8 cmd, void * param)
{
	bk_err_t	ret_code;

	if(log_chnl >= MB_LOG_CHNL_NUM)
		return BK_ERR_PARAM;

	if(log_chnl_cb[log_chnl].in_used == 0)
		return BK_ERR_STATE;

	switch(cmd)
	{
		case MB_CHNL_GET_STATUS:

			if(param == NULL)
				return BK_ERR_NULL_PARAM;

			*((u8 *)param) = log_chnl_cb[log_chnl].tx_state;
			
			break;

		case MB_CHNL_SET_RX_ISR:
			log_chnl_cb[log_chnl].rx_isr = (chnl_rx_isr_t)param;
			break;

		case MB_CHNL_SET_TX_ISR:
			log_chnl_cb[log_chnl].tx_isr = (chnl_tx_isr_t)param;
			break;

		case MB_CHNL_SET_TX_CMPL_ISR:
			log_chnl_cb[log_chnl].tx_cmpl_isr = (chnl_tx_cmpl_isr_t)param;
			break;

		case MB_CHNL_WRITE_SYNC:
			if(param == NULL)
				return BK_ERR_NULL_PARAM;

			ret_code = mb_phy_chnl_tx_cmd_sync(log_chnl, (mb_phy_chnl_cmd_t *)param);
			return ret_code;
			break;

		default:
			return BK_ERR_NOT_SUPPORT;
			break;
	}

	return BK_OK;
}


