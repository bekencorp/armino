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

#include <driver/lcd_types.h>


#ifdef __cplusplus
extern "C" {
#endif


extern const lcd_device_t lcd_device_st7282;
extern const lcd_device_t lcd_device_hx8282;
extern const lcd_device_t lcd_device_st7796s;
extern const lcd_device_t lcd_device_gc9503v;
extern const lcd_device_t lcd_device_nt35512;
extern const lcd_device_t  lcd_device_nt35510;
extern const lcd_device_t lcd_device_h050iwv;
extern const lcd_device_t lcd_device_md0430r;
extern const lcd_device_t lcd_device_md0700r;

void lcd_st7796s_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye);

void lcd_st7796s_init(void);

#ifdef __cplusplus
}
#endif
