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

#include "hal_config.h"
#include <soc/soc.h>
#include "psram_ll_macro_def.h"
//#include <driver/aud_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* REG_0x00 */
#define psram_hal_get_reg0_value()                 psram_ll_get_reg0_value()
#define psram_hal_set_reg0_value(value)            psram_ll_set_reg0_value(value)

/* REG_0x01 */
#define psram_hal_get_reg1_value()                 psram_ll_get_reg1_value()
#define psram_hal_set_reg1_value(value)            psram_ll_set_reg1_value(value)

/* REG_0x01 */
#define psram_hal_get_reg2_value()                 psram_ll_get_reg2_value()
#define psram_hal_set_reg2_value(value)            psram_ll_set_reg2_value(value)

/* REG_0x03 */
#define psram_hal_get_reg3_value()                 psram_ll_get_reg3_value()
#define psram_hal_set_reg3_value(value)            psram_ll_set_reg3_value(value)

/* REG_0x04 */
#define psram_hal_get_mode_value()                 psram_ll_get_mode_value()
#define psram_hal_set_mode_value(value)            psram_ll_set_mode_value(value)

/* REG_0x05 */
#define psram_hal_get_reg5_value()                 psram_ll_get_reg5_value()
#define psram_hal_set_reg5_value(value)            psram_ll_set_reg5_value(value)

/* REG_0x06 */
#define psram_hal_get_reg6_value()                 psram_ll_get_reg6_value()
#define psram_hal_set_reg6_value(value)            psram_ll_set_reg6_value(value)

/* REG_0x07 */
#define psram_hal_get_reg7_value()                 psram_ll_get_reg7_value()
#define psram_hal_set_reg7_value(value)            psram_ll_set_reg7_value(value)

/* REG_0x08 */
#define psram_hal_get_reg8_value()                 psram_ll_get_reg8_value()
#define psram_hal_set_reg8_value(value)            psram_ll_set_reg8_value(value)

/* REG_0x09 */
#define psram_hal_get_reg9_value()                 psram_ll_get_write_address()
#define psram_hal_set_reg9_value(value)            psram_ll_set_write_address(value)

/* REG_0x0a */
#define psram_hal_get_write_data()                 psram_ll_get_write_data()
#define psram_hal_set_write_data(value)            psram_ll_set_write_data(value)

/* REG_0x0b */
#define psram_hal_get_regb_value()                 psram_ll_get_regb_value()
#define psram_hal_set_regb_value(value)            psram_ll_set_regb_value(value)

void psram_hal_set_sf_reset(uint32_t value);
void psram_hal_set_cmd_reset(void);
void psram_hal_cmd_write(uint32_t addr, uint32_t value);
uint32_t psram_hal_cmd_read(uint32_t addr);
void psram_hal_set_transfer_mode(uint32_t value);

#if CFG_HAL_DEBUG_PSRAM
void psram_struct_dump(void);
#else
#define psram_struct_dump(void)
#endif


#ifdef __cplusplus
}
#endif


