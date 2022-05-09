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

#pragma once

#include "hal_config.h"
#include <soc/soc.h>
#include "i2s_ll_macro_def.h"
#include <driver/i2s_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t i2s_hal_config(const i2s_config_t *config);
bk_err_t i2s_hal_deconfig(void);
bk_err_t i2s_hal_int_status_get(i2s_int_status_t *int_status);
bk_err_t i2s_hal_read_ready_get(uint32_t *read_flag);
bk_err_t i2s_hal_write_ready_get(uint32_t *write_flag);
bk_err_t i2s_hal_en_set(uint32_t value);
bk_err_t i2s_hal_int_set(i2s_isr_id_t int_id, uint32_t value);
bk_err_t i2s_hal_role_set(uint32_t value);
bk_err_t i2s_hal_work_mode_set(uint32_t value);
bk_err_t i2s_hal_lrck_invert_set(uint32_t value);
bk_err_t i2s_hal_sck_invert_set(uint32_t value);
bk_err_t i2s_hal_lsb_first_set(uint32_t value);
bk_err_t i2s_hal_sync_len_set(uint32_t value);
bk_err_t i2s_hal_data_len_set(uint32_t value);
bk_err_t i2s_hal_pcm_dlen_set(uint32_t value);
bk_err_t i2s_hal_store_mode_set(uint32_t value);
bk_err_t i2s_hal_rxfifo_clear(void);
bk_err_t i2s_hal_txfifo_clear(void);
bk_err_t i2s_hal_txudf_int_clear(i2s_channel_id_t channel_id);
bk_err_t i2s_hal_rxovf_int_clear(i2s_channel_id_t channel_id);
bk_err_t i2s_hal_txint_level_set(uint32_t value);
bk_err_t i2s_hal_rxint_level_set(uint32_t value);
bk_err_t i2s_hal_data_write(uint32_t channel_id, uint32_t value);
bk_err_t i2s_hal_data_read(uint32_t *value);
bk_err_t i2s_hal_sample_ratio_set(uint32_t value);
bk_err_t i2s_hal_sck_ratio_set(uint32_t value);
bk_err_t i2s_hal_sample_ratio_h2b_set(uint32_t value);
bk_err_t i2s_hal_sck_ratio_h4b_set(uint32_t value);

#if CFG_HAL_DEBUG_I2S
void i2s_struct_dump(void);
#else
#define i2s_struct_dump()
#endif


#ifdef __cplusplus
}
#endif
