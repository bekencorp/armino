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

                            
#include <common/bk_include.h>          
#include "aon_pmu_hal.h"        


void aon_pmu_drv_init()               
{                            
    aon_pmu_hal_init();             
}
void aon_pmu_drv_set_sleep_parameters(uint32_t value)
{
    aon_pmu_hal_set_sleep_parameters(value);
}
void aon_pmu_drv_clear_wakeup_source(wakeup_source_t value)
{
    aon_pmu_hal_clear_wakeup_source(value);
}
void aon_pmu_drv_set_wakeup_source(wakeup_source_t value)
{
    aon_pmu_hal_set_wakeup_source(value);
}

uint32_t aon_pmu_drv_get_adc_cal(void)
{
	return aon_pmu_hal_get_adc_cal();
}

void aon_pmu_drv_touch_select(uint32_t value)
{
	aon_pmu_hal_touch_select(value);
}

uint32_t aon_pmu_drv_get_cap_cal(void)
{
	return aon_pmu_hal_get_cap_cal();
}

uint32_t aon_pmu_drv_get_touch_state(void)
{
	return aon_pmu_hal_get_touch_state();
}

void aon_pmu_drv_touch_int_enable(uint32_t enable)
{
	aon_pmu_hal_touch_int_en(enable);
}

uint32_t aon_pmu_drv_get_touch_int_status(void)
{
	return aon_pmu_hal_get_touch_int_status();
}

void aon_pmu_drv_clear_touch_int(uint32_t value)
{
	aon_pmu_hal_clear_touch_int(value);
}
void aon_pmu_drv_reg_set(pmu_reg_e reg, uint32_t value)
{
    aon_pmu_hal_reg_set(reg,value);
}
uint32_t aon_pmu_drv_reg_get(pmu_reg_e reg)
{
	return aon_pmu_hal_reg_get(reg);
}

#if (CONFIG_SYSTEM_CTRL)
void aon_pmu_drv_wdt_rst_dev_enable(void)
{
	aon_pmu_hal_wdt_rst_dev_enable();
}
#endif

