#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mailbox_channel.h"

typedef struct {
	uint32_t chnl_id;
} aud_mb_t;

typedef enum {
	AUD_TRAS_MB_CMD_NULL = 0,
	AUD_TRAS_MB_CMD_TX_REQ,
	AUD_TRAS_MB_CMD_TX_DONE,
	//AUD_MB_WRITE_DAC_DATA_REQ_CMD,
	//AUD_MB_WRITE_DAC_DATA_DONE_CMD,
	AUD_TRAS_MB_CMD_START_TRANSFER,
	AUD_TRAS_MB_CMD_STOP_TRANSFER,
	AUD_TRAS_MB_CMD_START_TRANSFER_DONE,
	AUD_TRAS_MB_CMD_STOP_TRANSFER_DONE,
	AUD_MB_MAX_CMD,
} audio_tras_mb_cmd_t;

typedef struct {
	audio_tras_mb_cmd_t mb_cmd;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
} audio_tras_mb_msg_t;


void aud_tras_mb_init(void *rx_isr, void *tx_isr, void *tx_cmpl_isr);
void aud_tras_mb_deinit(void);
bk_err_t aud_tras_mb_send_msg(audio_tras_mb_msg_t *msg);

#ifdef __cplusplus
}
#endif

