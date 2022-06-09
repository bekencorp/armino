#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mailbox_channel.h"

#define MB_CHNL_AUD     2

typedef struct {
	uint32_t chnl_id;
} aud_mb_t;

typedef enum {
	AUD_MB_READ_ADC_DATA_REQ_CMD = 0,
	AUD_MB_READ_ADC_DATA_DONE_CMD,
	AUD_MB_WRITE_DAC_DATA_REQ_CMD,
	AUD_MB_WRITE_DAC_DATA_DONE_CMD,
	AUD_MB_GET_ENCODER_USED_SIZE_REQ_CMD,
	AUD_MB_GET_ENCODER_USED_SIZE_DONE_CMD,
	AUD_MB_GET_DECODER_REMAIN_SIZE_REQ_CMD,
	AUD_MB_GET_DECODER_REMAIN_SIZE_DONE_CMD,
	AUD_MB_START_TRANSFER_CMD,
	AUD_MB_STOP_TRANSFER_CMD,
	AUD_MB_MAX_CMD,
} audio_maibox_cmd_t;

typedef struct {
	audio_maibox_cmd_t mb_cmd;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} audio_mailbox_msg_t;


void aud_mailbox_init(void *rx_isr, void *tx_isr, void *tx_cmpl_isr);
void aud_mailbox_deinit(void);
bk_err_t aud_mailbox_send_msg(audio_mailbox_msg_t *msg);

#ifdef __cplusplus
}
#endif

