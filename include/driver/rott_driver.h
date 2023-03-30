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

/***********************************************************************************************************************************
* This file is generated from BK7236_ADDR_Mapping_s.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7236,GENARATE TIME:2023-03-20 20:21:54                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      
#include <driver/rott_types.h>

bk_err_t bk_rott_driver_init(void);
bk_err_t bk_rott_driver_deinit(void);

bk_err_t bk_rott_set_module_contol_clk_gate(uint32_t en);
bk_err_t bk_rott_int_enable(rott_int_type_t int_type, bool en);
bk_err_t bk_rott_mode_config(rott_mode_t rot_mode);
bk_err_t bk_rott_input_data_format(pixel_format_t rot_input_fmt);
bk_err_t bk_rott_input_data_reverse(rott_input_data_flow_t input_flow,  rott_output_data_flow_t output_flow);
bk_err_t bk_rott_block_rotate_config(uint16_t picture_line_pixel, uint16_t picture_clum_pixel, uint16_t block_line_pixel, uint16_t block_clum_pixel, uint16_t block_cnt);
bk_err_t bk_rott_wr_addr_config(uint32_t rd_base_addr, uint32_t wr_base_addr);
bk_err_t bk_rott_wartermark_block_config(uint16_t wtmk_block);
bk_err_t bk_rott_enable(bool en);
bk_err_t bk_rott_soft_rst(void);

#if (USE_ROTT_REGISTER_CALLBACKS == 1)
bk_err_t  bk_rott_isr_register(rott_int_type_t int_type, rott_isr_t isr);
#else
bk_err_t bk_rott_isr_register(rott_isr_t rott_isr);
bk_err_t bk_rott_int_status_clear(rott_int_type_t int_type);
uint32_t bk_rott_int_status_get(void);

#endif


#ifdef __cplusplus 
}                  
#endif             
