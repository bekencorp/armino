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

#if CONFIG_LCD_ST7282
extern const lcd_device_t lcd_device_st7282;
#endif

#if  CONFIG_LCD_HX8282
extern const lcd_device_t lcd_device_hx8282;
#endif

#if CONFIG_LCD_ST7796S
extern const lcd_device_t lcd_device_st7796s;
void lcd_st7796s_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye);
void lcd_st7796s_init(void);
#endif

#if CONFIG_LCD_GC9503V
extern const lcd_device_t lcd_device_gc9503v;
#endif

#if CONFIG_LCD_ST7710S
extern const lcd_device_t lcd_device_st7710s;
#endif

#if CONFIG_LCD_NT35512
extern const lcd_device_t lcd_device_nt35512;
#endif

#if CONFIG_LCD_NT35510
extern const lcd_device_t  lcd_device_nt35510;
#endif

#if CONFIG_LCD_NT35510_MCU
extern const lcd_device_t lcd_device_nt35510_mcu;
#endif


#if CONFIG_LCD_H050IWV
extern const lcd_device_t lcd_device_h050iwv;
#endif

#if CONFIG_LCD_MD0430R
extern const lcd_device_t lcd_device_md0430r;
#endif

#if CONFIG_LCD_MD0700R
extern const lcd_device_t lcd_device_md0700r;
#endif

#if CONFIG_LCD_ST7701S
	extern const lcd_device_t lcd_device_st7701s;
#endif

#if CONFIG_LCD_ST7701S_LY
extern const lcd_device_t lcd_device_st7701s_ly;
#endif


#ifdef __cplusplus
}
#endif
