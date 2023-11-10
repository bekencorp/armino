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

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
#define ADC_KEY_TAG "adc_key"

#define ADC_KEY_LOGI(...) BK_LOGI(ADC_KEY_TAG, ##__VA_ARGS__)
#define ADC_KEY_LOGW(...) BK_LOGW(ADC_KEY_TAG, ##__VA_ARGS__)
#define ADC_KEY_LOGE(...) BK_LOGE(ADC_KEY_TAG, ##__VA_ARGS__)
#define ADC_KEY_LOGD(...) BK_LOGD(ADC_KEY_TAG, ##__VA_ARGS__)

#define ADC_KEY_GPIO_ID   GPIO_28
#define ADC_KEY_SADC_CHAN_ID   4
#define ADC_KEY_GET_VALUE_WAIT_MS 50
#define ADC_KEY_VALID_VALUE_STATISTICS 35
#define ADC_KEY_ADC_COLLECT_NUMBER 100

typedef void (*adc_key_isr_t)();

typedef struct
{
	adc_key_isr_t next_cb;
	adc_key_isr_t prev_cb;
	adc_key_isr_t play_pause_cb;
	adc_key_isr_t menu_cb;

} adc_key_callback_t;

typedef struct
{
	beken_semaphore_t adc_key_semaphore;
	beken_thread_t adc_key_handle;
	uint32_t anti_shake_count;

} adc_key_config_t;

void adc_key_init(void);
void adc_key_deinit(void);
void adc_key_register_next_cb(adc_key_isr_t isr);
void adc_key_register_prev_cb(adc_key_isr_t isr);
void adc_key_register_play_pause_cb(adc_key_isr_t isr);
void adc_key_register_menu_cb(adc_key_isr_t isr);

void adc_key_set_gpio_int_type(int adc_key_gpio_int_type);


#ifdef __cplusplus
}
#endif
