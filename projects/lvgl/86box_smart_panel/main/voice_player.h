#ifndef __VOICE_PLAY_H__
#define __VOICE_PLAY_H__

typedef enum {
	VOICE_PLAY_START = 0,
	VOICE_PLAY_STOP,
	VOICE_PLAY_EXIT
} voice_play_op_t;

typedef struct {
	voice_play_op_t op;
	void * param;
} voice_play_msg_t;

bk_err_t voice_play_init(void);
bk_err_t voice_play_deinit(void);
bk_err_t voice_play_send_msg(voice_play_op_t op, void * param);
bk_err_t read_voice_data_from_sd(unsigned int size);

#endif