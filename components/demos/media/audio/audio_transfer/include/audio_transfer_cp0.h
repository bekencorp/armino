#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "audio_types.h"

bk_err_t bk_audio_cp0_transfer_init(audio_setup_t *setup_cfg);

bk_err_t bk_audio_cp0_transfer_deinit(void);

void bk_audio_register_rw_cb(void (*read_done_callback)(uint8_t * buffer_addr, uint32_t length),
										void (*write_done_callback)(uint8_t * buffer_addr, uint32_t length),
										void (*get_encoder_used_size_callback)(uint32_t used_size),
										void (*get_decoder_remain_size_callback)(uint32_t),
										void (*encoder_read_req_handler)(void),
										void (*decoder_write_req_handler)(void)
										);

bk_err_t bk_audio_read_req(uint8_t *buffer, uint32_t length);

bk_err_t bk_audio_write_req(uint8_t *buffer, uint32_t length);

bk_err_t bk_audio_get_encoder_used_size(void);

bk_err_t bk_audio_get_decoder_remain_size(void);


#ifdef __cplusplus
}
#endif

