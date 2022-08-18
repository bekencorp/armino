// Copyright 2020-2021 Beken
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

#include <os/os.h>
#if CONFIG_AUD_TRAS_MODE_CPU1
#include "audio_transfer_api.h"
#include "audio_transfer_types.h"
#endif
#include <driver/dma.h>
#include "aud_hal.h"
#include "sys_driver.h"
#include "media_common.h"
#include "mailbox_channel.h"


beken_thread_t com_mb_thread_handle = NULL;
static beken_queue_t com_msg_que = NULL;
#define TU_QITEM_COUNT      (60)

//#if CONFIG_SLAVE_CORE

static bk_err_t com_send_msg(com_msg_t msg)
{
	bk_err_t ret;

	if (com_msg_que) {
		ret = rtos_push_to_queue(&com_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("[COM] send msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

bk_err_t media_send_com_mb_msg(common_maibox_cmd_t cmd, uint32_t param1, uint32_t param2, uint32_t param3)
{
	mb_chnl_cmd_t mb_cmd;

	//send mailbox msg to cpu0
	mb_cmd.hdr.cmd = cmd;
	mb_cmd.param1 = param1;
	mb_cmd.param2 = param2;
	mb_cmd.param3 = param3;
	return mb_chnl_write(MB_CHNL_COM, &mb_cmd);
}

static void com_mailbox_rx_isr(common_mailbox_msg_t *com_mb, mb_chnl_cmd_t *cmd_buf)
{
	os_printf("[COM] enter com_mailbox_rx_isr \r\n");
	bk_err_t ret = BK_OK;
	com_msg_t msg;

	/* check mailbox msg and send msg to task */
	switch(cmd_buf->hdr.cmd) {
		case CMD_COM_MB_AUD_TRAS_INIT:
			/* init audio transfer task */
			//os_printf("AUD_MB_READ_ADC_DATA_REQ_CMD: read_buffer=0x%p, read_buffer_size=%d \r\n", read_buffer, read_buffer_size);
			msg.op = COM_AUDIO;
			msg.param = cmd_buf->param1;
			ret = com_send_msg(msg);
			break;

		case COM_MB_START_VIDEO_CMD:
		{
			/* init video transfer task */
			//msg.op = COM_VIDEO;
			break;
		}

		default:
			break;
	}

	if (ret != kNoErr) {
		os_printf("[COM] send msg: %d fail \r\n", msg.op);
	}
}

static void com_mailbox_tx_isr(common_mailbox_msg_t *aud_mb)
{
	//os_printf("enter cp1_mailbox_tx_isr \r\n");
}

static void com_mailbox_tx_cmpl_isr(common_mailbox_msg_t *aud_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp1_mailbox_tx_cmpl_isr \r\n");
}

#if CONFIG_AUD_TRAS_MODE_CPU1
bk_err_t common_audio_process(uint32_t param)
{
	bk_err_t ret = BK_OK;

	/* init audio transfer task */
	ret = bk_aud_tras_drv_init((aud_tras_drv_setup_t *)param);
	if (ret != BK_OK) {
		os_printf("init audio transfer fail \r\n");
		return ret;
	}

	return BK_OK;
}
#endif

bk_err_t common_video_process(void)
{
	return BK_OK;
}

static void common_mb_main(void)
{
	bk_err_t ret = BK_OK;
	//common_mailbox_msg_t mb_msg;

	/* init common mailbox channel */
	//mb_chnl_init();
	mb_chnl_open(MB_CHNL_COM, NULL);
	mb_chnl_ctrl(MB_CHNL_COM, MB_CHNL_SET_RX_ISR, com_mailbox_rx_isr);
	mb_chnl_ctrl(MB_CHNL_COM, MB_CHNL_SET_TX_ISR, com_mailbox_tx_isr);
	mb_chnl_ctrl(MB_CHNL_COM, MB_CHNL_SET_TX_CMPL_ISR, com_mailbox_tx_cmpl_isr);

	while(1) {
		com_msg_t msg;
		ret = rtos_pop_from_queue(&com_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case COM_IDLE:
					break;

				case COM_AUDIO:
					/* call audio transfer init api */
					//audio_start_transfer_process();
#if CONFIG_AUD_TRAS_MODE_CPU1
					ret = common_audio_process(msg.param);
					if (ret != BK_OK) {
						os_printf("[COM] init audio transfer fail \r\n");
					} else {
						/* send COM_MB_CMPL mailbox message to cpu0 */
						//TODO
					}
#endif

					break;

				case COM_VIDEO:
					/* call video transfer init api */
					ret = common_video_process();
					if (ret != BK_OK) {
						os_printf("[COM] init video transfer fail \r\n");
					} else {
						/* send COM_MB_CMPL mailbox message to cpu0 */
						//TODO
					}
					break;

				case COM_SEND:
					//TODO
					media_send_com_mb_msg(COM_MB_ACK, 0, 0, 0);
					break;

				case COM_EXIT:
					goto com_thread_exit;
					break;

				default:
					break;
			}
		}
	}

com_thread_exit:
	/* disable mailbox */
	mb_chnl_close(MB_CHNL_COM);

	/* delate msg queue */
	ret = rtos_deinit_queue(&com_msg_que);
	if (ret != kNoErr) {
		os_printf("[COM] delate message queue fail \r\n");
	}
	com_msg_que = NULL;
	os_printf("[COM] delate message queue complete \r\n");

	/* delate task */
	rtos_delete_thread(NULL);
	com_mb_thread_handle = NULL;
	os_printf("[COM] delate task complete \r\n");

}


bk_err_t common_mb_init(void)
{
	bk_err_t ret = BK_OK;
	if ((!com_mb_thread_handle) && (!com_msg_que)) {
		//os_printf("cp1: start audio_transfer test \r\n");
		//os_memcpy(&audio_transfer_setup_bak, setup_cfg, sizeof(audio_setup_t));

		ret = rtos_init_queue(&com_msg_que,
							  "common_mb_queue",
							  sizeof(com_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("[COM] ceate common thread message queue failed \r\n");
			return BK_FAIL;
		}
		os_printf("[COM] ceate common thread message queue complete \r\n");

		/* creat common mailbox thread */
		ret = rtos_create_thread(&com_mb_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "common mailbox thread",
							 (beken_thread_function_t)common_mb_main,
							 4096,
							 NULL);
		if (ret != kNoErr) {
			os_printf("[COM] create common mailbox thread fail \r\n");
			rtos_deinit_queue(&com_msg_que);
			com_msg_que = NULL;
			com_mb_thread_handle = NULL;
		}
		os_printf("[COM] create common mailbox thread complete \r\n");

		return kNoErr;
	} else
		return kInProgressErr;
}

//#endif

