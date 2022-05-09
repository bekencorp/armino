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

#ifdef __cplusplus
extern "C" {
#endif

/* This header mainly for configuring the HAL debug options.
 * Normally the HAL debug option should be disabled to reduce the image size.
 **/

//TODO disable all of them
#define CFG_HAL_DEBUG_ICU        1
#define CFG_HAL_DEBUG_PWM        1
#define CFG_HAL_DEBUG_GPIO       1
#define CFG_HAL_DEBUG_TIMER      1
#define CFG_HAL_DEBUG_DMA        1
#define CFG_HAL_DEBUG_UART       1
#define CFG_HAL_DEBUG_WDT        1
#define CFG_HAL_DEBUG_TRNG       1
#define CFG_HAL_DEBUG_EFUSE      1
#define CFG_HAL_DEBUG_ADC        1
#define CFG_HAL_DEBUG_SPI        1
#define CFG_HAL_DEBUG_QSPI       1
#define CFG_HAL_DEBUG_AON_RTC    1
#define CFG_HAL_DEBUG_I2C        1
#define CFG_HAL_DEBUG_AUD        1
#define CFG_HAL_DEBUG_JPEG       1
#define CFG_HAL_DEBUG_FFT        1
#define CFG_HAL_DEBUG_LCD_DISP   1
#define CFG_HAL_DEBUG_I2S        1
#define CFG_HAL_DEBUG_CALENDAR   1
#define CFG_HAL_DEBUG_FLASH      1

#ifdef __cplusplus
}
#endif
