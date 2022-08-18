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
#include "stdio.h"
#include "sys_driver.h"
#if CONFIG_AUD_TRAS_MODE_CPU1
#include "audio_mailbox.h"
#include "media_common.h"
#endif
#include "audio_transfer_private.h"
#if CONFIG_AUD_TRAS_MODE_CPU0
#include "audio_transfer_api.h"
#endif


#define MAILBOX_CHECK_CONTROL    0


aud_tras_drv_setup_t aud_tras_drv_setup;
audio_tras_info_t aud_tras_info;

#if CONFIG_AUD_TRAS_MODE_CPU1
#define TU_QITEM_COUNT      (60)
beken_thread_t aud_tras_demo_thread_hdl = NULL;
beken_queue_t aud_tras_demo_msg_que = NULL;

#if MAILBOX_CHECK_CONTROL
static uint32_t audio_tras_mb_busy_status = false;
#endif
#endif //CONFIG_AUD_TRAS_MODE_CPU1


/* extern api */
static void audio_tras_demo_deconfig(void);
#if CONFIG_AUD_TRAS_MODE_CPU1
static bk_err_t audio_tras_send_msg(audio_tras_opcode_t op, uint32_t param1, uint32_t param2, uint32_t param3);
#endif

static void audio_transfer_event_process(audio_tras_event_t event)
{
	switch (event) {
		case EVENT_AUD_TRAS_INIT_CMP:
			aud_tras_info.status = AUD_TRAS_STA_READY;
			os_printf("init audio transfer complete \r\n");
			break;

		case EVENT_AUD_TRAS_START_CMP:
			aud_tras_info.status = AUD_TRAS_STA_WORKING;
			os_printf("start audio transfer complete \r\n");
			break;

		case EVENT_AUD_TRAS_STOP_CMP:
#if CONFIG_AUD_TRAS_MODE_CPU1
			audio_tras_send_msg(AUD_TRAS_EXIT, 0, 0, 0);
#endif

#if CONFIG_AUD_TRAS_MODE_CPU0
			audio_tras_demo_deconfig();
			aud_tras_info.status = AUD_TRAS_STA_IDLE;
#endif
			os_printf("stop audio transfer complete \r\n");
			break;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
		case EVENT_AUD_TRAS_AEC_DUMP:
			audio_tras_aec_dump_handle();
			//os_printf("stop audio transfer complete \r\n");
			break;
#endif

		default:
			break;
	}
}

#if CONFIG_AUD_TRAS_MODE_CPU1
/* media common mailbox api */
static bk_err_t media_send_common_mb_msg(common_maibox_cmd_t cmd, uint32_t param1, uint32_t param2, uint32_t param3)
{
	mb_chnl_cmd_t mb_cmd;

	//send mailbox msg to cpu0
	mb_cmd.hdr.cmd = cmd;
	mb_cmd.param1 = param1;
	mb_cmd.param2 = param2;
	mb_cmd.param3 = param3;
	return mb_chnl_write(MB_CHNL_COM, &mb_cmd);
}

static void media_mb_rx_isr(common_mailbox_msg_t *com_mb, mb_chnl_ack_t *cmd_buf)
{
	bk_err_t ret = BK_OK;
	common_maibox_cmd_t op;

	/* check mailbox msg and send msg to task */
	switch(cmd_buf->hdr.cmd) {
		case CMD_COM_MB_AUD_TRAS_INIT_CMPL:
			audio_transfer_event_process(EVENT_AUD_TRAS_INIT_CMP);
			break;

		default:
			break;
	}

	if (ret != kNoErr) {
		os_printf("send msg: %d fail \r\n", op);
	}

}

static void media_mb_tx_isr(common_mailbox_msg_t *com_mb, mb_chnl_ack_t *cmd_buf)
{
	//TODO
}

static void media_mb_tx_cmpl_isr(common_mailbox_msg_t *com_mb, mb_chnl_ack_t *cmd_buf)
{
	//TODO
}

static void media_com_mailbox_init(void)
{
	//init maibox
	//mb_chnl_init();
	mb_chnl_open(MB_CHNL_COM, NULL);
	if (media_mb_rx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_COM, MB_CHNL_SET_RX_ISR, media_mb_rx_isr);
	if (media_mb_tx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_COM, MB_CHNL_SET_TX_ISR, media_mb_tx_isr);
	if (media_mb_tx_cmpl_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_COM, MB_CHNL_SET_TX_CMPL_ISR, media_mb_tx_cmpl_isr);
}


static bk_err_t audio_tras_send_msg(audio_tras_opcode_t op, uint32_t param1, uint32_t param2, uint32_t param3)
{
	bk_err_t ret;
	audio_tras_msg_t msg;

	msg.op = op;
	msg.param1 = param1;
	msg.param2 = param2;
	msg.param3 = param3;
	if (aud_tras_demo_msg_que) {
		ret = rtos_push_to_queue(&aud_tras_demo_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("send audio_tras_msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}

/*
static bk_err_t audio_tras_send_stop_msg(void)
{
	bk_err_t ret;
	audio_tras_msg_t msg;

	msg.op = AUD_TRAS_EXIT;

	if (aud_tras_demo_msg_que) {
		ret = rtos_push_to_queue_front(&aud_tras_demo_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			os_printf("send audio transfer stop msg fail \r\n");
			return kOverrunErr;
		}

		return ret;
	}
	return kNoResourcesErr;
}
*/

static bk_err_t audio_tras_send_aud_mb_msg(audio_tras_mb_cmd_t cmd, uint32_t param1, uint32_t param2, uint32_t param3)
{
	bk_err_t ret = BK_OK;

	audio_tras_mb_msg_t mb_msg;

	mb_msg.mb_cmd = cmd;
	mb_msg.param1 = param1;
	mb_msg.param2 = param2;
	mb_msg.param3 = param3;

#if MAILBOX_CHECK_CONTROL

	/* wait send audio_mailbox_msg complete */
	while(audio_cp0_mailbox_busy_status)
		;

	audio_cp0_mailbox_busy_status = true;

	ret = aud_tras_mb_send_msg(&mb_msg);
	if (ret != BK_OK)
		audio_cp0_mailbox_busy_status = false;

#else
	ret = aud_tras_mb_send_msg(&mb_msg);
#endif

	return ret;
}

static void audio_tras_mb_tx_isr(aud_mb_t *aud_mb)
{
	//os_printf("enter cp0_mailbox_tx_isr \r\n");
}

static void audio_tras_mb_tx_cmpl_isr(aud_mb_t *aud_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_tx_cmpl \r\n");

#if MAILBOX_CHECK_CONTROL
	audio_cp0_mailbox_busy_status = false;
#endif

}

static void audio_tras_mb_rx_isr(aud_mb_t *aud_mb, mb_chnl_cmd_t *cmd_buf)
{
	bk_err_t ret = BK_OK;
	//audio_tras_mb_msg_t msg;
	//audio_tras_mb_cmd_t op = AUD_TRAS_IDLE;

	/* check mailbox msg and send msg to task */
	switch (cmd_buf->hdr.cmd) {
		case AUD_TRAS_MB_CMD_TX_REQ:
			ret = audio_tras_send_msg(AUD_TRAS_TX_MIC_DATA, cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);
			if (ret != kNoErr) {
				os_printf("send audio_tras_msg: AUD_TRAS_TX_MIC_DATA fail \r\n");
			}
			break;

		case AUD_TRAS_MB_CMD_STOP_TRANSFER_DONE:
			audio_transfer_event_process(EVENT_AUD_TRAS_STOP_CMP);
			break;

#if 0
		case AUD_MB_WRITE_DAC_DATA_DONE_CMD:
			msg.op = AUDIO_CP0_WRITE_DONE;
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

		case AUD_MB_START_TRANSFER_DONE_CMD:
			msg.op = AUDIO_CP0_READY;
			break;
#endif
		default:
			break;
	}

}

#endif	//CONFIG_AUD_TRAS_MODE_CPU1

#if 0
bk_err_t bk_audio_start_transfer(void)
{
	audio_mailbox_msg_t mb_msg;

	mb_msg.mb_cmd = AUD_MB_START_TRANSFER_CMD;
	mb_msg.param1 = 0;
	mb_msg.param2 = 0;
	mb_msg.param3 = 0;
	return audio_cp0_send_aud_mailbox_msg(&mb_msg);
}

static bk_err_t audio_stop_transfer(void)
{
	audio_mailbox_msg_t mb_msg;

	mb_msg.mb_cmd = AUD_MB_STOP_TRANSFER_CMD;
	mb_msg.param1 = 0;
	mb_msg.param2 = 0;
	mb_msg.param3 = 0;
	return audio_cp0_send_aud_mailbox_msg(&mb_msg);
}
#endif

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
static void audio_tras_aec_dump_config(void)
{
	sprintf(aud_tras_info.sin_file_name, "1:/%s", "aec_dump_sin.pcm");
	sprintf(aud_tras_info.ref_file_name, "1:/%s", "aec_dump_ref.pcm");
	sprintf(aud_tras_info.out_file_name, "1:/%s", "aec_dump_out.pcm");
}

static bk_err_t audio_tras_aec_dump_file_open(void)
{
	FRESULT fr;

	/*open file to save sin data of AEC */
	fr = f_open(&(aud_tras_info.sin_file), aud_tras_info.sin_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", aud_tras_info.sin_file_name);
		return BK_FAIL;
	}

	/*open file to save ref data of AEC */
	fr = f_open(&(aud_tras_info.ref_file), aud_tras_info.ref_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", aud_tras_info.ref_file_name);
		return BK_FAIL;
	}

	/*open file to save out data of AEC */
	fr = f_open(&(aud_tras_info.out_file), aud_tras_info.out_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", aud_tras_info.out_file_name);
		return BK_FAIL;
	}

	return BK_OK;
}

static bk_err_t audio_tras_aec_dump_file_close(void)
{
	FRESULT fr;

	/* close sin file */
	fr = f_close(&(aud_tras_info.sin_file));
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", aud_tras_info.sin_file_name);
		return BK_FAIL;
	}
	os_printf("close sin file complete \r\n");

	/* close ref file */
	fr = f_close(&(aud_tras_info.ref_file));
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", aud_tras_info.ref_file_name);
		return BK_FAIL;
	}
	os_printf("close ref file complete \r\n");

	/* close unused file */
	fr = f_close(&(aud_tras_info.out_file));
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", aud_tras_info.out_file_name);
		return BK_FAIL;
	}
	os_printf("close out file complete \r\n");

	return BK_OK;
}

static bk_err_t audio_tras_aec_dump_handle(void)
{
	FRESULT fr;
	uint32 uiTemp = 0;

	/* write sin data to file */
	fr = f_write(&(aud_tras_info.sin_file), (void *)aud_tras_drv_setup.aec_dump.mic_dump_addr, aud_tras_drv_setup.aec_dump.len, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", aud_tras_info.sin_file_name);
		return BK_FAIL;
	}

	/* write ref data to file */
	fr = f_write(&(aud_tras_info.ref_file), (void *)aud_tras_drv_setup.aec_dump.ref_dump_addr, aud_tras_drv_setup.aec_dump.len, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", aud_tras_info.ref_file_name);
		return BK_FAIL;
	}

	/* write unused data to file */
	fr = f_write(&(aud_tras_info.out_file), (void *)aud_tras_drv_setup.aec_dump.out_dump_addr, aud_tras_drv_setup.aec_dump.len, &uiTemp);
	if (fr != FR_OK) {
		os_printf("write %s fail.\r\n", aud_tras_info.out_file_name);
		return BK_FAIL;
	}

	return BK_OK;
}
#endif

static void audio_tras_demo_deconfig(void)
{
	/* audio deconfig */
	aud_tras_drv_setup.samp_rate = AUD_TRAS_SAMP_RATE_8K;	//default value
	aud_tras_drv_setup.aud_setup.adc_gain = 0;
	aud_tras_drv_setup.aud_setup.dac_gain = 0;
	aud_tras_drv_setup.aud_setup.mic_frame_number = 0;
	aud_tras_drv_setup.aud_setup.mic_samp_rate_points = 0;
	aud_tras_drv_setup.aud_setup.speaker_frame_number = 0;
	aud_tras_drv_setup.aud_setup.speaker_samp_rate_points = 0;

	/* aec deconfig */
	if (aud_tras_drv_setup.aec_enable) {
		os_free(aud_tras_drv_setup.aec_setup);
	}
	aud_tras_drv_setup.aec_setup = NULL;
	aud_tras_drv_setup.aec_enable = false;

	/* tx deconfig */
	aud_tras_drv_setup.tx_context.buff_length = 0;
	aud_tras_drv_setup.tx_context.tx_buff_status = false;
	aud_tras_drv_setup.tx_context.ping.busy_status = false;
	os_free(aud_tras_drv_setup.tx_context.ping.buff_addr);
	aud_tras_drv_setup.tx_context.ping.buff_addr = NULL;
	aud_tras_drv_setup.tx_context.pang.busy_status = false;
	os_free(aud_tras_drv_setup.tx_context.pang.buff_addr);
	aud_tras_drv_setup.tx_context.pang.buff_addr = NULL;

	/* rx deconfig */
	aud_tras_drv_setup.rx_context.rx_buff_status = false;
	os_free(aud_tras_drv_setup.rx_context.decoder_ring_buff);
	aud_tras_drv_setup.rx_context.decoder_ring_buff = NULL;
	os_free(aud_tras_drv_setup.rx_context.decoder_rb);
	aud_tras_drv_setup.rx_context.decoder_rb = NULL;
	aud_tras_drv_setup.rx_context.frame_num = 0;
	aud_tras_drv_setup.rx_context.frame_size = 0;
	aud_tras_drv_setup.rx_context.fifo_frame_num = 0;
	aud_tras_drv_setup.rx_context.rx_buff_seq_tail = 0;
	aud_tras_drv_setup.rx_context.aud_trs_read_seq = 0;

	/* callback deconfig */
	aud_tras_drv_setup.aud_cb.audio_transfer_event_cb = NULL;
	aud_tras_drv_setup.aud_cb.audio_send_mic_data = NULL;

	/* delate audio transfer task */
#if CONFIG_AUD_TRAS_MODE_CPU1
//	audio_tras_send_msg(AUD_TRAS_EXIT, 0, 0, 0);
#endif

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	os_free(aud_tras_drv_setup.aec_dump.mic_dump_addr);
	aud_tras_drv_setup.aec_dump.mic_dump_addr = NULL;
	os_free(aud_tras_drv_setup.aec_dump.ref_dump_addr);
	aud_tras_drv_setup.aec_dump.ref_dump_addr = NULL;
	os_free(aud_tras_drv_setup.aec_dump.out_dump_addr);
	aud_tras_drv_setup.aec_dump.out_dump_addr = NULL;
	aud_tras_drv_setup.aec_dump.len = 0;

	bk_err_t ret = BK_OK;
	ret = audio_tras_aec_dump_file_close();
	if (ret != BK_OK) {
		os_printf("close aec dump file fail \r\n");
	}
#endif
}

#if CONFIG_AUD_TRAS_MODE_CPU1
/* audio transfer demo task in cpu1 mode */
static void audio_tras_demo_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = BK_OK;
	uint32_t size = 0;

	/* init media common mailbox channel */
	media_com_mailbox_init();
	/* send audio transfer init mailbox command to CPU1 */
	media_send_common_mb_msg(CMD_COM_MB_AUD_TRAS_INIT, (uint32_t)(&aud_tras_drv_setup), 0, 0);

	/* init maibox */
	aud_tras_mb_init(audio_tras_mb_rx_isr, audio_tras_mb_tx_isr, audio_tras_mb_tx_cmpl_isr);
	os_printf("config mailbox complete \r\n");

	while(1) {
		audio_tras_msg_t msg;
		ret = rtos_pop_from_queue(&aud_tras_demo_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch (msg.op) {

				case AUD_TRAS_IDLE:
					break;

				case AUD_TRAS_TX_MIC_DATA:
					if (aud_tras_drv_setup.aud_cb.audio_send_mic_data) {
						size = aud_tras_drv_setup.aud_cb.audio_send_mic_data((unsigned char *)msg.param1, msg.param2);
						if (size != msg.param2) {
							//os_printf("send audio data by wifi fail \r\n");
							//os_printf("audio lost \r\n");
						}
						//response Tx Ack
						audio_tras_send_aud_mb_msg(AUD_TRAS_MB_CMD_TX_DONE, msg.param1, msg.param2, 0);
					}
					break;
#if 0
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

				case AUDIO_CP0_READY:
					if (audio_transfer_ready_callback)
						audio_transfer_ready_callback();
					break;
#endif

				case AUD_TRAS_EXIT:
					/* receive mailbox msg of stop test from cpu1, and stop transfer */
					/* deinit audio transfer demo */
					audio_tras_demo_deconfig();
					aud_tras_info.status = AUD_TRAS_STA_IDLE;
					goto audio_transfer_exit;
					//os_printf("cp0: goto audio_transfer_exit \r\n");
					break;

				default:
					break;
			}
		}
	}

audio_transfer_exit:
	/* deinit audio transfer mailbox channel */
	aud_tras_mb_deinit();

	/* delate msg queue */
	ret = rtos_deinit_queue(&aud_tras_demo_msg_que);
	if (ret != kNoErr) {
		os_printf("delate message queue fail \r\n");
		//return BK_FAIL;
	}
	aud_tras_demo_msg_que = NULL;
	os_printf("delate message queue complete \r\n");

	/* delate task */
	aud_tras_demo_thread_hdl = NULL;
	rtos_delete_thread(NULL);
}
#endif	//CONFIG_AUD_TRAS_MODE_CPU1

bk_err_t audio_tras_init(audio_tras_setup_t setup)
{
	bk_err_t ret = BK_OK;
	if (aud_tras_info.status != AUD_TRAS_STA_IDLE) {
		os_printf("audio transfer already init \r\n");
		return BK_OK;
	}

	//aud_tras_drv_setup.aud_trs_mode = demo_setup.mode;
	aud_tras_drv_setup.samp_rate = setup.samp_rate;
	aud_tras_drv_setup.aec_enable = setup.aec_enable;
	/* audio config */
	aud_tras_drv_setup.aud_setup.adc_gain = 0x2d;
	aud_tras_drv_setup.aud_setup.dac_gain = 0x2d;
	aud_tras_drv_setup.aud_setup.mic_frame_number = 2;
	aud_tras_drv_setup.aud_setup.mic_samp_rate_points = 160; 	//if AEC enable , the value is equal to aec_samp_rate_points, and the value not need to set
	aud_tras_drv_setup.aud_setup.speaker_frame_number = 2;
	aud_tras_drv_setup.aud_setup.speaker_samp_rate_points = 160; 	//if AEC enable , the value is equal to aec_samp_rate_points, and the value not need to set

	if (aud_tras_drv_setup.aec_enable) {
		/* aec config */
		aud_tras_drv_setup.aec_setup = os_malloc(sizeof(aec_config_t));
		if (aud_tras_drv_setup.aec_setup == NULL) {
			os_printf("malloc aec_setup fail \r\n");
			goto audio_transfer_exit;
		}
		aud_tras_drv_setup.aec_setup->init_flags = 0x01f;
		aud_tras_drv_setup.aec_setup->mic_delay = 0;
		aud_tras_drv_setup.aec_setup->ec_depth = 20;
		aud_tras_drv_setup.aec_setup->ref_scale = 0;
		aud_tras_drv_setup.aec_setup->TxRxThr = 30;
		aud_tras_drv_setup.aec_setup->TxRxFlr = 6;
		aud_tras_drv_setup.aec_setup->voice_vol = 14;
		aud_tras_drv_setup.aec_setup->ns_level = 2;
		aud_tras_drv_setup.aec_setup->ns_para = 1;
		aud_tras_drv_setup.aec_setup->drc = 15;
	} else {
		aud_tras_drv_setup.aec_setup = NULL;
	}

	/* tx config */
	aud_tras_drv_setup.tx_context.buff_length = aud_tras_drv_setup.aud_setup.mic_samp_rate_points;
	aud_tras_drv_setup.tx_context.ping.busy_status = false;
	aud_tras_drv_setup.tx_context.ping.buff_addr = os_malloc(aud_tras_drv_setup.tx_context.buff_length);
	if (aud_tras_drv_setup.tx_context.ping.buff_addr == NULL) {
		os_printf("malloc pingpang buffer of tx fail \r\n");
		goto audio_transfer_exit;
	}
	aud_tras_drv_setup.tx_context.pang.busy_status = false;
	aud_tras_drv_setup.tx_context.pang.buff_addr = os_malloc(aud_tras_drv_setup.tx_context.buff_length);
	if (aud_tras_drv_setup.tx_context.pang.buff_addr == NULL) {
		os_printf("malloc pang buffer of tx fail \r\n");
		goto audio_transfer_exit;
	}
	aud_tras_drv_setup.tx_context.tx_buff_status = true;

	/* rx config */
	aud_tras_drv_setup.rx_context.aud_trs_read_seq = 0;
	aud_tras_drv_setup.rx_context.frame_size = 320;
	aud_tras_drv_setup.rx_context.frame_num = 15;
	aud_tras_drv_setup.rx_context.rx_buff_seq_tail = 0;
	aud_tras_drv_setup.rx_context.fifo_frame_num = 10;
	aud_tras_drv_setup.rx_context.decoder_ring_buff = os_malloc(aud_tras_drv_setup.rx_context.frame_size * aud_tras_drv_setup.rx_context.frame_num);
	if (aud_tras_drv_setup.rx_context.decoder_ring_buff == NULL) {
		os_printf("malloc decoder ring buffer of rx fail \r\n");
		goto audio_transfer_exit;
	}
	os_printf("malloc decoder_ring_buff:%p, size:%d \r\n", aud_tras_drv_setup.rx_context.decoder_ring_buff, aud_tras_drv_setup.rx_context.frame_size * aud_tras_drv_setup.rx_context.frame_num);
	aud_tras_drv_setup.rx_context.decoder_rb = os_malloc(sizeof(RingBufferContext));
	if (aud_tras_drv_setup.rx_context.decoder_rb == NULL) {
		os_printf("malloc decoder_rb fail \r\n");
		goto audio_transfer_exit;
	}
	ring_buffer_init(aud_tras_drv_setup.rx_context.decoder_rb, (uint8_t *)aud_tras_drv_setup.rx_context.decoder_ring_buff, (aud_tras_drv_setup.rx_context.frame_size * aud_tras_drv_setup.rx_context.frame_num), DMA_ID_MAX, RB_DMA_TYPE_NULL);
	aud_tras_drv_setup.rx_context.rx_buff_status = true;

	//os_printf("decoder_rb:%p \r\n", aud_tras_drv_setup.rx_context.decoder_rb);

	/* callback config */
	aud_tras_drv_setup.aud_cb.audio_transfer_event_cb = audio_transfer_event_process;
	aud_tras_drv_setup.aud_cb.audio_send_mic_data = setup.audio_send_mic_data;

#if CONFIG_AUD_TRAS_AEC_DUMP_DEBUG
	if (aud_tras_drv_setup.samp_rate == AUD_TRAS_SAMP_RATE_8K)
		aud_tras_drv_setup.aec_dump.len = 320;
	else if (aud_tras_drv_setup.samp_rate == AUD_TRAS_SAMP_RATE_16K)
		aud_tras_drv_setup.aec_dump.len = 640;
	else
		aud_tras_drv_setup.aec_dump.len = 0;

	if (aud_tras_drv_setup.aec_dump.len) {
		aud_tras_drv_setup.aec_dump.mic_dump_addr = (int16_t *)os_malloc(aud_tras_drv_setup.aec_dump.len);
		if (aud_tras_drv_setup.aec_dump.mic_dump_addr == NULL) {
			os_printf("malloc mic_dump_addr fail \r\n");
			goto audio_transfer_exit;
		}
		aud_tras_drv_setup.aec_dump.ref_dump_addr = (int16_t *)os_malloc(aud_tras_drv_setup.aec_dump.len);
		if (aud_tras_drv_setup.aec_dump.ref_dump_addr == NULL) {
			os_printf("malloc ref_dump_addr fail \r\n");
			goto audio_transfer_exit;
		}
		aud_tras_drv_setup.aec_dump.out_dump_addr = (int16_t *)os_malloc(aud_tras_drv_setup.aec_dump.len);
		if (aud_tras_drv_setup.aec_dump.out_dump_addr == NULL) {
			os_printf("malloc ref_dump_addr fail \r\n");
			goto audio_transfer_exit;
		}
	}

	audio_tras_aec_dump_config();
	ret = audio_tras_aec_dump_file_open();
	if (ret != BK_OK) {
		os_printf("open dump file fail \r\n");
		goto audio_transfer_exit;
	}
#endif

	/* init audio transfer driver */
#if CONFIG_AUD_TRAS_MODE_CPU0
	os_printf("init audio transfer in CPU0 mode \r\n");
	ret = bk_aud_tras_drv_init(&aud_tras_drv_setup);
	if (ret != BK_OK) {
		os_printf("init audio transfer driver fail \r\n");
		goto audio_transfer_exit;
	}
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	os_printf("init audio transfer in CPU1 mode \r\n");
	/* AUD_TRAS_STA_IDLE -> AUD_TRAS_STA_INITING */
	aud_tras_info.status = AUD_TRAS_STA_INITING;

	/* create audio transfer app task in cpu0 */
	if ((!aud_tras_demo_thread_hdl) && (!aud_tras_demo_msg_que)) {
		ret = rtos_init_queue(&aud_tras_demo_msg_que,
							  "audio_transfer_demo_queue",
							  sizeof(audio_tras_msg_t),
							  TU_QITEM_COUNT);
		if (ret != kNoErr) {
			os_printf("ceate audio internal message queue fail \r\n");
			goto audio_transfer_exit;
		}

		//creat audio transfer task
		ret = rtos_create_thread(&aud_tras_demo_thread_hdl,
							 3,
							 "audio_intf",
							 (beken_thread_function_t)audio_tras_demo_main,
							 4096,
							 0);
		if (ret != kNoErr) {
			os_printf("create audio transfer task fail \r\n");
			rtos_deinit_queue(&aud_tras_demo_msg_que);
			aud_tras_demo_msg_que = NULL;
			aud_tras_demo_thread_hdl = NULL;
			goto audio_transfer_exit;
		}
	} else {
		os_printf("task and queue is exist \r\n");
	}
#endif

	return kNoErr;

audio_transfer_exit:
	//TODO
	os_printf("init audio transfer fail \r\n");
	aud_tras_info.status = AUD_TRAS_STA_IDLE;

	return BK_FAIL;
}

bk_err_t audio_tras_deinit(void)
{
	bk_err_t ret = BK_OK;

	if (aud_tras_info.status != AUD_TRAS_STA_WORKING) {
		os_printf("audio transfer not working \r\n");
		return BK_OK;
	}

#if CONFIG_AUD_TRAS_MODE_CPU0
	ret = bk_aud_tras_drv_deinit();
	if (ret != BK_OK) {
		os_printf("deinit audio transfer fail \r\n");
		return ret;
	}
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
	/* send mailbox msg to CPU1 to stop audio transfer */
	ret = audio_tras_send_aud_mb_msg(AUD_TRAS_MB_CMD_STOP_TRANSFER, 0, 0, 0);
	if (ret != BK_OK) {
		os_printf("send msg to stop audio transfer fail \r\n");
		return BK_FAIL;
	}
#endif

	aud_tras_info.status = AUD_TRAS_STA_STOPING;
	os_printf("audio transfer stoping \r\n");

	return BK_OK;
}

bk_err_t audio_tras_write_spk_data(uint8_t *dac_buff, uint32_t len)
{
	uint32_t size;
	bk_err_t ret = BK_OK;

	if (aud_tras_info.status == AUD_TRAS_STA_READY || aud_tras_info.status == AUD_TRAS_STA_WORKING) {
		if (ring_buffer_get_free_size(aud_tras_drv_setup.rx_context.decoder_rb) >= len) {
			size = ring_buffer_write(aud_tras_drv_setup.rx_context.decoder_rb, dac_buff, len);
			if (size != len) {
				os_printf("write decoder_ring_buff fail, size:%d \r\n", size);
				return BK_FAIL;
			}
			aud_tras_drv_setup.rx_context.rx_buff_seq_tail += len/(aud_tras_drv_setup.rx_context.frame_size);
		}

		if (aud_tras_info.status == AUD_TRAS_STA_WORKING) {
			return BK_OK;
		}

		/* send audio msg to start audio transfer */
		if ((aud_tras_info.status == AUD_TRAS_STA_READY) && (ring_buffer_get_fill_size(aud_tras_drv_setup.rx_context.decoder_rb)/aud_tras_drv_setup.rx_context.frame_size == aud_tras_drv_setup.rx_context.fifo_frame_num)) {
#if CONFIG_AUD_TRAS_MODE_CPU0
			ret = bk_aud_tras_drv_start_work();
#endif

#if CONFIG_AUD_TRAS_MODE_CPU1
			audio_tras_send_aud_mb_msg(AUD_TRAS_MB_CMD_START_TRANSFER, 0, 0, 0);
#endif
			if (ret != BK_OK) {
				os_printf("start audio transfer fail \r\n");
				return ret;
			}
			aud_tras_info.status = AUD_TRAS_STA_WORKING;
		}
	}

	return BK_OK;
}

