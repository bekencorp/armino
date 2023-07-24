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
#include "bk_gpio.h"
#include <driver/gpio.h>
#include "gpio_driver.h"
#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"
#include <driver/hal/hal_gpio_types.h>
#include <driver/hal/hal_adc_types.h>

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

#define ADCKEY_TMR_DURATION 10
#define ADCKEY_TICKS_INTERVAL    10	//ms
#define ADCKEY_DEBOUNCE_TICKS    6	//MAX 8
#define ADCKEY_SHORT_TICKS       (400 /ADCKEY_TICKS_INTERVAL)
#define ADCKEY_LONG_TICKS        (1000 /ADCKEY_TICKS_INTERVAL)

typedef void (*adc_key_callback)(void *);

typedef enum {
	ADCKEY_PRESS_DOWN = 0,
	ADCKEY_PRESS_UP,
	ADCKEY_PRESS_REPEAT,
	ADCKEY_SINGLE_CLICK,
	ADCKEY_DOUBLE_CLICK,
	ADCKEY_LONG_PRESS_START,
	ADCKEY_LONG_PRESS_HOLD,
	ADCKEY_NUMBER_OF_EVENT,
	ADCKEY_NONE_PRESS
} ADCKEY_PRESS_EVT;

typedef struct _adckey_ {
	uint16_t ticks;
	uint16_t lowest_active_level;
	uint16_t highest_active_level;
	uint16_t adc_read_level;
	uint8_t repeat;
	uint8_t event;
	uint8_t state;
	uint8_t debounce_cnt;

	void *user_data;
	adc_key_callback  cb[ADCKEY_NUMBER_OF_EVENT];
	struct _adckey_ *next;
} ADCKEY_S;

typedef enum {

	ADCKEY_PEV = 0,
	ADCKEY_NEXT,
	ADCKEY_MENU,
	ADCKEY_PLAY_PAUSE,
	ADCKEY_NULL,
} ADCKEY_INDEX;

typedef struct
{
	uint16_t lowest_level;
	uint16_t highest_level;
	ADCKEY_INDEX user_index;
	adc_key_callback short_press_cb;
	adc_key_callback double_press_cb;
	adc_key_callback long_press_cb;
	adc_key_callback hold_press_cb;
} adckey_configure_t;

void bk_adc_key_init(gpio_id_t gpio_id, adc_chan_t adc_chan);
void bk_adc_key_deinit(void);
uint32_t bk_adckey_item_configure(adckey_configure_t *config);
uint32_t bk_adckey_item_unconfigure(ADCKEY_INDEX user_data);

#ifdef __cplusplus
}
#endif
