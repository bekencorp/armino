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

#include <components/log.h>

#define AUD_TAG "aud"
#define AUD_LOGI(...) BK_LOGI(AUD_TAG, ##__VA_ARGS__)
#define AUD_LOGW(...) BK_LOGW(AUD_TAG, ##__VA_ARGS__)
#define AUD_LOGE(...) BK_LOGE(AUD_TAG, ##__VA_ARGS__)
#define AUD_LOGD(...) BK_LOGD(AUD_TAG, ##__VA_ARGS__)

/**
 * @brief     Start ADC to DAC loop test
 *
 * This API start loop test:
 *   - Start adc to dac loop test if work mode is adc work mode
 *   - Start dtmf to dac loop test if work mode is dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_start_loop_test(void);

/**
 * @brief     Stop ADC to DAC loop test
 *
 * This API start loop test:
 *   - Stop adc to dac loop test if work mode is adc work mode
 *   - Stop dtmf to dac loop test if work mode is dtmf work mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_stop_loop_test(void);

/**
 * @brief     write the sampled value to dac fifo 
 *
 * @param
 *    - pcm_value: sampled value
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_dac_write(uint32_t pcm_value);

/**
 * @brief     Set the interrupt threshold of dac fifo read
 *
 * @param
 *    - dacl_throld: the interrupt threshold of dac left channel
 *    - dacr_throld: the interrupt threshold of dac right channel
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: audio driver is not init
 *    - others: other errors.
 */
bk_err_t bk_aud_set_dac_read_threshold(uint16_t dacl_throld, uint16_t dacr_throld);

/**
 * @brief     Control audio interrupt enable/disable of cpu
 *
 * @param
 *    - value: enable/disable audio interrupt of cpu
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_cpu_int_en(uint32_t value);

