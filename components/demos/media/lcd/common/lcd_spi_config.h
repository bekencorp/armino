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
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

void lcd_set_spi_delay_time(uint32_t time);
void lcd_init_gpio(void);
void spi_init_gpio(void);
void lcd_gc9503v_set_config(void);
void lcd_st7701s_set_config(void);

#ifdef __cplusplus
}
#endif


