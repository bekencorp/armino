#pragma once

#include <modules/audio_ring_buff.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	AUD_TRAS_IDLE = 0,
	AUD_TRAS_EXIT,
	AUD_TRAS_TX,
	AUD_TRAS_MAX
} aud_tras_op_t;

typedef struct {
	aud_tras_op_t op;
	void *param;
} aud_tras_msg_t;

typedef struct {
	uint32_t data_ptr;
	uint32_t length;
} aud_tx_data_t;


/* audio transfer speaker information in general mode */
typedef struct {
	RingBufferContext *aud_tx_rb;			/**< speaker received ring buffer context */

	int (*aud_tras_send_data_cb)(unsigned char *data, unsigned int len);
} aud_tras_setup_t;


bk_err_t aud_tras_send_msg(aud_tras_op_t op, void *param);
bk_err_t aud_tras_init(aud_tras_setup_t *setup_cfg);
bk_err_t aud_tras_deinit(void);


#ifdef __cplusplus
}
#endif

