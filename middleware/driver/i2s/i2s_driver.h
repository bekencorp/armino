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
#include <driver/i2s_types.h>

#define I2S_TAG "i2s"
#define I2S_LOGI(...) BK_LOGI(I2S_TAG, ##__VA_ARGS__)
#define I2S_LOGW(...) BK_LOGW(I2S_TAG, ##__VA_ARGS__)
#define I2S_LOGE(...) BK_LOGE(I2S_TAG, ##__VA_ARGS__)
#define I2S_LOGD(...) BK_LOGD(I2S_TAG, ##__VA_ARGS__)

/**
 * @brief     clear i2s tx_udf interrupt flag
 *
 * This API clear i2s tx_udf interrupt flag
 *
 * @param channel_id i2s channel id
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_clear_txudf_int(i2s_channel_id_t channel_id);

/**
 * @brief     clear i2s rx_ovf interrupt flag
 *
 * This API clear i2s rx_ovf interrupt flag
 *
 * @param channel_id i2s channel id
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_AUD_NOT_INIT: i2s driver is not init
 *    - others: other errors.
 */
bk_err_t bk_i2s_clear_rxovf_int(i2s_channel_id_t channel_id);


