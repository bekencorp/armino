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

#include "modules/wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

int mcu_suppress_and_sleep(uint32_t sleep_ticks);
int bkreg_tx_get_uart_port(void);
void write_cal_result_to_flash_secure_op1(void);
void write_cal_result_to_flash_secure_op2(void);
void connection_fail_cb(wifi_linkstate_reason_t info);

bool bk_is_audio_adc_en(void);
bool bk_is_audio_dac_en(void);
bool bk_is_audio_en(void);

void bk_audio_intf_dac_pause(void);
void bk_audio_intf_adc_pause(void);
void bk_audio_intf_dac_play(void);
void bk_audio_intf_adc_play(void);
void bk_audio_intf_uninit(void);
void bk_audio_intf_init(void);
void bk_audio_intf_dac_set_volume(void);
void bk_audio_intf_dac_set_sample_rate(void);
#ifdef __cplusplus
}
#endif

