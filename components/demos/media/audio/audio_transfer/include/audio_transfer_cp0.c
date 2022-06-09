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
#include <os/mem.h>
#include <os/str.h>
#include "ff.h"
#include "diskio.h"
#include "stdio.h"
#include "mailbox_channel.h"
#include "sys_driver.h"
#include "audio_types.h"
#include "audio_mailbox.h"

#define TU_QITEM_COUNT      (60)
static beken_thread_t  audio_thread_hdl = NULL;
static beken_queue_t aud_int_msg_que = NULL;

static uint32_t* read_buffer = NULL;    //save data read from CPU1
static uint32_t read_buffer_size = 0;
static uint32_t* write_buffer = NULL;    //save data write to CPU1
static uint32_t write_buffer_size = 0;
static uint32_t encoder_used_size = 0;
static uint32_t decoder_remain_size = 0;

void (*audio_read_done_callback)(uint8_t *, uint32_t);
void (*audio_write_done_callback)(uint8_t *, uint32_t);
void (*audio_get_encoder_used_size_callback)(uint32_t);
void (*audio_get_decoder_remain_size_callback)(uint32_t);
void (*audio_encoder_read_req_handler)(void);
void (*audio_decoder_write_req_handler)(void);

extern void delay(int num);

static bk_err_t audio_send_msg(audio_cp0_msg_t msg)
{
	bk_err_t ret;

	if (aud_int_msg_que) {
		ret = rtos_push_to_queue(&aud_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("cp1: audio_send_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static bk_err_t audio_send_stop_msg(audio_cp0_msg_t msg)
{
	bk_err_t ret;

	if (aud_int_msg_que) {
		ret = rtos_push_to_queue_front(&aud_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("cp0: audio_send_stop_msg failed\r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

static void audio_cp0_mailbox_tx_cmpl_isr(aud_mb_t *aud_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_tx_cmpl \r\n");
}

static void audio_cp0_mailbox_rx_isr(aud_mb_t *aud_mb, mb_chnl_cmd_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_rx_isr \r\n");
	bk_err_t ret = BK_OK;
	audio_cp0_msg_t msg;

	msg.op = AUDIO_CP0_IDLE;

	/* check mailbox msg and send msg to task */
	switch(cmd_buf->hdr.cmd) {
		case AUD_MB_READ_ADC_DATA_DONE_CMD:
			msg.op = AUDIO_CP0_READ_DONE;
			break;

		case AUD_MB_WRITE_DAC_DATA_DONE_CMD:
			msg.op = AUDIO_CP0_WRITE_DONE;
			break;

		case AUD_MB_GET_ENCODER_USED_SIZE_DONE_CMD:
			encoder_used_size = cmd_buf->param1;
			msg.op = AUDIO_CP0_GET_ENCODER_USED_SIZE_DONE;
			break;

		case AUD_MB_GET_DECODER_REMAIN_SIZE_DONE_CMD:
			decoder_remain_size = cmd_buf->param1;
			//os_printf("cp0: decoder_remain_size: %d \r\n", decoder_remain_size);
			msg.op = AUDIO_CP0_GET_DECODER_REMAIN_SIZE_DONE;
			break;

		case AUD_MB_READ_ADC_DATA_REQ_CMD:
			msg.op = AUDIO_CP0_READ_REQ;
			break;

		case AUD_MB_WRITE_DAC_DATA_REQ_CMD:
			msg.op = AUDIO_CP0_WRITE_REQ;
			break;

		case AUD_MB_STOP_TRANSFER_CMD:
			msg.op = AUDIO_CP0_EXIT;
			ret = audio_send_stop_msg(msg);
			if (ret != kNoErr) {
				os_printf("cp0: send stop msg: %d fail \r\n", msg.op);
			}
			os_printf("cp0: send stop msg: %d complete \r\n", msg.op);
			return;

		default:
			break;
	}

	ret = audio_send_msg(msg);
	if (ret != kNoErr) {
		os_printf("cp1: send msg: %d fails \r\n", msg.op);
	}

}

static void audio_cp0_mailbox_tx_isr(aud_mb_t *aud_mb)
{
	//os_printf("enter cp0_mailbox_tx_isr \r\n");
}

/* register callbacks of read and write audio data done */
void bk_audio_register_rw_cb(void (*read_done_callback)(uint8_t * buffer_addr, uint32_t length),
										void (*write_done_callback)(uint8_t * buffer_addr, uint32_t length),
										void (*get_encoder_used_size_callback)(uint32_t used_size),
										void (*get_decoder_remain_size_callback)(uint32_t remain_size),
										void (*encoder_read_req_handler)(void),
										void (*decoder_write_req_handler)(void)
										)
{
	audio_read_done_callback = read_done_callback;
	audio_write_done_callback = write_done_callback;
	audio_get_encoder_used_size_callback = get_encoder_used_size_callback;
	audio_get_decoder_remain_size_callback = get_decoder_remain_size_callback;
	audio_encoder_read_req_handler = encoder_read_req_handler;
	audio_decoder_write_req_handler = decoder_write_req_handler;
}

bk_err_t bk_audio_read_req(uint8_t *buffer, uint32_t length)
{
	audio_mailbox_msg_t msg;

	read_buffer = (uint32_t *)buffer;
	read_buffer_size = length;

	msg.mb_cmd = AUD_MB_READ_ADC_DATA_REQ_CMD;
	msg.param1 = (uint32_t)read_buffer;
	msg.param2 = read_buffer_size;
	msg.param3 = 0;
//	os_printf("bk_audio_read_req: read_buffer=0x%x, read_buffer_size=%d \r\n", read_buffer, read_buffer_size);
	return aud_mailbox_send_msg(&msg);
}

bk_err_t bk_audio_write_req(uint8_t *buffer, uint32_t length)
{
	audio_mailbox_msg_t msg;

	write_buffer = (uint32_t *)buffer;
	write_buffer_size = length;

	msg.mb_cmd = AUD_MB_WRITE_DAC_DATA_REQ_CMD;
	msg.param1 = (uint32_t)write_buffer;
	msg.param2 = write_buffer_size;
	msg.param3 = 0;
//	os_printf("cp0: bk_audio_write_req: write_buffer=0x%x, write_buffer_size:%d \r\n", write_buffer, write_buffer_size);
	return aud_mailbox_send_msg(&msg);
}

bk_err_t bk_audio_get_encoder_used_size(void)
{
	audio_mailbox_msg_t msg;

	msg.mb_cmd = AUD_MB_GET_ENCODER_USED_SIZE_REQ_CMD;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}

bk_err_t bk_audio_get_decoder_remain_size(void)
{
	audio_mailbox_msg_t msg;

	msg.mb_cmd = AUD_MB_GET_DECODER_REMAIN_SIZE_REQ_CMD;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}

static bk_err_t audio_start_transfer(void)
{
	audio_mailbox_msg_t msg;

	msg.mb_cmd = AUD_MB_START_TRANSFER_CMD;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}

static bk_err_t audio_stop_transfer(void)
{
	audio_mailbox_msg_t msg;

	msg.mb_cmd = AUD_MB_STOP_TRANSFER_CMD;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = 0;
	return aud_mailbox_send_msg(&msg);
}


static void audio_cp0_transfer_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	//audio_setup_t *audio_setup = NULL;

	/* init maibox */
	aud_mailbox_init(audio_cp0_mailbox_rx_isr, audio_cp0_mailbox_tx_isr, audio_cp0_mailbox_tx_cmpl_isr);
	os_printf("cp0: config mailbox complete \r\n");

	/* malloc buffer */
/*	read_buffer_size = AUD_AEC_8K_FRAME_SAMP_SIZE/2;
	read_buffer = (uint32_t *)os_malloc(read_buffer_size);
	if (read == NULL) {
		os_printf("cp0: malloc read_buffer fail \r\n");
		goto audio_transfer_exit;
	}
*/

	ret = audio_start_transfer();
	if (ret != BK_OK) {
		os_printf("send start transfer mailbox msg fail \r\n");
		audio_stop_transfer();
		return ;
	}

	bk_audio_get_decoder_remain_size();

	while(1) {
		audio_cp1_msg_t msg;
		ret = rtos_pop_from_queue(&aud_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {
				case AUDIO_CP0_IDLE:
					break;

				case AUDIO_CP0_GET_ENCODER_USED_SIZE_DONE:
					//audio_read_req_process();
					if (audio_get_encoder_used_size_callback)
						audio_get_encoder_used_size_callback(encoder_used_size);
					break;

				case AUDIO_CP0_GET_DECODER_REMAIN_SIZE_DONE:
					if (audio_get_decoder_remain_size_callback)
						audio_get_decoder_remain_size_callback(decoder_remain_size);
					break;

				case AUDIO_CP0_READ_REQ:
					if (audio_encoder_read_req_handler)
						audio_encoder_read_req_handler();
					break;

				case AUDIO_CP0_READ_DONE:
					if (audio_read_done_callback)
						audio_read_done_callback((uint8_t *)read_buffer, read_buffer_size);
					break;

				case AUDIO_CP0_WRITE_REQ:
					if (audio_decoder_write_req_handler)
						audio_decoder_write_req_handler();
					break;

				case AUDIO_CP0_WRITE_DONE:
					if (audio_write_done_callback)
						audio_write_done_callback((uint8_t *)write_buffer, write_buffer_size);
					break;

				case AUDIO_CP0_MAILBOX:
					//audio_mailbox_msg_handle();
					break;

				case AUDIO_CP0_EXIT:
					/* receive mailbox msg of stop test from cpu1, and stop transfer */
					//TODO
					bk_audio_register_rw_cb(NULL, NULL, NULL, NULL, NULL, NULL);
					goto audio_transfer_exit;
					//os_printf("cp0: goto audio_transfer_exit \r\n");
					break;

				default:
					break;
			}
		}
	}


audio_transfer_exit:
	/* disable mailbox */
	aud_mailbox_deinit();

	/* delate msg queue */
	ret = rtos_deinit_queue(&aud_int_msg_que);
	if (ret != kNoErr) {
		os_printf("cp0: delate message queue fail \r\n");
		//return BK_FAIL;
	}
	aud_int_msg_que = NULL;
	os_printf("cp0: delate message queue complete \r\n");

	/* delate task */
	rtos_delete_thread(NULL);
	audio_thread_hdl = NULL;
	os_printf("cp0: delate audio transfer task \r\n");
}

//audio_setup_t audio_transfer_setup_bak = {0};

bk_err_t bk_audio_cp0_transfer_init(audio_setup_t *setup_cfg)
{
	bk_err_t ret = BK_OK;
	if ((!audio_thread_hdl) && (!aud_int_msg_que)) {
		os_printf("cp0: start audio_transfer test \r\n");
		//os_memcpy(&audio_transfer_setup_bak, setup_cfg, sizeof(audio_setup_t));

		ret = rtos_init_queue(&aud_int_msg_que,
							  "audio_internal_queue",
							  sizeof(audio_cp0_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("cp0: ceate audio internal message queue in cpu0 failed \r\n");
			return BK_FAIL;
		}

		//creat audio transfer task
		ret = rtos_create_thread(&audio_thread_hdl,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "audio_intf",
							 (beken_thread_function_t)audio_cp0_transfer_main,
							 4096,
							 0);
		if (ret != kNoErr) {
			os_printf("cp0: create audio transfer task fail \r\n");
			rtos_deinit_queue(&aud_int_msg_que);
			aud_int_msg_que = NULL;
			audio_thread_hdl = NULL;
		}

		return kNoErr;
	} else
		return kInProgressErr;
}

bk_err_t bk_audio_cp0_transfer_deinit(void)
{
	bk_err_t ret = BK_OK;

	/* send mailbox msg to CPU1 to stop audio transfer */
	ret = audio_stop_transfer();
	if (ret != BK_OK) {
		os_printf("cp0: send mailbox msg to stop audio transfer fail \r\n");
		return BK_FAIL;
	}

	/* wait audio_cp0_transfer_main delate */
	//while (audio_thread_hdl != NULL);

	os_printf("cp0: task delate complete \r\n");
	//bk_audio_register_rw_cb(NULL, NULL, NULL, NULL, NULL);

	/* delate msg queue */
/*	ret = rtos_deinit_queue(aud_int_msg_que);
	if (ret != kNoErr) {
		os_printf("cp0: delate message queue fail \r\n");
		return BK_FAIL;
	}
*/
	return BK_OK;
}

