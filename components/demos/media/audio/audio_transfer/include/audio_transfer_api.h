#pragma once

#include "audio_transfer_private.h"

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t bk_aud_tras_drv_init(aud_tras_drv_setup_t *setup_cfg);

bk_err_t bk_aud_tras_drv_deinit(void);

bk_err_t bk_aud_tras_aec_ec_depth_set(uint8_t value);

bk_err_t bk_aud_tras_drv_start_work(void);

#ifdef __cplusplus
}
#endif

