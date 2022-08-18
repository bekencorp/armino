#pragma once

#include <components/aud_intf_types.h>
#include "aud_tras_drv.h"

#ifdef __cplusplus
extern "C" {
#endif


bk_err_t aud_tras_drv_init(aud_intf_drv_setup_t *setup_cfg);

bk_err_t aud_tras_drv_deinit(void);

bk_err_t aud_tras_drv_send_msg(aud_tras_drv_op_t op, void *param);

//bk_err_t aud_tras_drv_aec_ec_depth_set(uint8_t value);

//bk_err_t aud_tras_drv_start_work(void);

#ifdef __cplusplus
}
#endif

