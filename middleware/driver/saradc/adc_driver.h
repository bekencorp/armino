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

#define ADC_TAG "adc"
#define ADC_LOGI(...) BK_LOGI(ADC_TAG, ##__VA_ARGS__)
#define ADC_LOGW(...) BK_LOGW(ADC_TAG, ##__VA_ARGS__)
#define ADC_LOGE(...) BK_LOGE(ADC_TAG, ##__VA_ARGS__)
#define ADC_LOGD(...) BK_LOGD(ADC_TAG, ##__VA_ARGS__)


#define DEFAULT_ADC_MODE             ADC_CONTINUOUS_MODE
#define DEFAULT_ADC_SAMPLE_RATE      0x20
#define DEFAULT_ADC_SCLK             ADC_SCLK_XTAL_26M
#define DEFAULT_ADC_CLK              2600000
#define DEFAULT_ADC_STEADY_TIME      7
#define DEFAULT_SATURATE_MODE        ADC_SATURATE_MODE_3

bk_err_t bk_adc_acquire(void);
bk_err_t bk_adc_init(adc_chan_t adc_chan);
bk_err_t bk_adc_enable_bypass_clalibration(void);
bk_err_t bk_adc_start(void);
bk_err_t bk_adc_read_raw(uint16_t* buf, uint32_t size, uint32_t timeout);
bk_err_t bk_adc_stop(void);
bk_err_t bk_adc_deinit(adc_chan_t chan);
bk_err_t bk_adc_release(void);

bk_err_t bk_adc_set_config(adc_config_t *config);
bk_err_t bk_adc_get_config(uint32 adc_ch, adc_config_t **config);
bk_err_t bk_adc_is_valid_ch(uint32_t ch);
bk_err_t bk_adc_register_isr_iot_callback(    void* iot_callback, void      * p_iot_context);
bk_err_t bk_adc_en(void);

