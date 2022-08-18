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

#include "aon_pmu_ll.h"   


//static aon_pmu_hal_t s_aon_pmu_hal;

bk_err_t aon_pmu_hal_init(void)
{                            
    //s_aon_pmu_hal.hw = (aon_pmu_hw_t *)AON_PMU_LL_REG_BASE;
    return BK_OK;
}
uint32_t aon_pmu_hal_lpo_src_get()
{
	return 0;
}
#if 0
#if (CONFIG_SOC_BK7256XX)
#define PM_LOW_VOL_PARAMETER_32K_FROM_26M      (0x29111111)
#define PM_LOW_VOL_PARAMETER_DEFAULT           (0x2B111111)
#define PM_DEEP_SLEEP_PARAMETER_DEFAULT        (0x4e111111)
int aon_pmu_hal_set_sleep_parameters(uint32_t value)
{
	uint32_t lpo_src = 0;
	uint32_t sleep_parm = 0;
	lpo_src = aon_pmu_ll_get_reg41_lpo_config();
	if(value == 0x1)//low voltage
	{
		if(lpo_src == 0x0)//32k from 26m
		{
			sleep_parm = PM_LOW_VOL_PARAMETER_32K_FROM_26M;
		}
		else
		{
			sleep_parm = PM_LOW_VOL_PARAMETER_DEFAULT;
		}
	}
	else if(value == 0x2)//deep sleep
	{
		if(lpo_src == 0x0)//32k from 26m
		{
			aon_pmu_ll_set_reg41_lpo_config(0x2);//select rosc
			sleep_parm = PM_DEEP_SLEEP_PARAMETER_DEFAULT;
		}
		else//rosc and external 32k
		{
			sleep_parm = PM_DEEP_SLEEP_PARAMETER_DEFAULT;
		}
	}
	else
	{
		os_printf("error: not set sleep para\r\n");
		return -1;
	}

    aon_pmu_ll_set_reg40_value(sleep_parm);
	return 0;
}

void aon_pmu_hal_set_wakeup_source_reg(uint32_t value)
{
	aon_pmu_ll_set_reg41_value(value);
}

uint32_t aon_pmu_hal_get_wakeup_source_reg(void)
{
	return aon_pmu_ll_get_reg41_value();
}

void aon_pmu_hal_clear_wakeup_source(wakeup_source_t value)
{
    uint32_t wakeup_source = 0;
    wakeup_source = aon_pmu_ll_get_reg41_wakeup_ena();
	wakeup_source &= ~(0x1 << value);
    aon_pmu_ll_set_reg41_wakeup_ena( wakeup_source);
}
void aon_pmu_hal_set_wakeup_source(wakeup_source_t value)
{
    uint32_t wakeup_source = 0;
    wakeup_source = aon_pmu_ll_get_reg41_wakeup_ena();
	wakeup_source |= (0x1 << value);
    aon_pmu_ll_set_reg41_wakeup_ena( wakeup_source);
}
void aon_pmu_hal_usbplug_int_en(uint32_t value)
{
   uint32_t int_state = 0;
   int_state = aon_pmu_ll_get_reg1_usbplug_int_en();
   int_state |= value;
   aon_pmu_ll_set_reg1_usbplug_int_en( int_state);
}

void aon_pmu_hal_touch_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_reg1_touch_int_en();
	int_state |= value;
	aon_pmu_ll_set_reg1_touch_int_en(int_state);
}

void aon_pmu_hal_touch_int_disable(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_reg1_touch_int_en();
	int_state &= ~value;
	aon_pmu_ll_set_reg1_touch_int_en(int_state);
}

 uint32_t aon_pmu_hal_get_adc_cal(void)
{
	return aon_pmu_ll_get_reg7d_adc_cal();
}

void aon_pmu_hal_touch_select(uint32_t value)
{
	aon_pmu_ll_set_reg1_touch_select(value);
}

uint32_t aon_pmu_hal_get_cap_cal(void)
{
	return aon_pmu_ll_get_reg7e_capcal();
}

uint32_t aon_pmu_hal_get_touch_state(void)
{
	return aon_pmu_ll_get_reg71_touch_state();
}

uint32_t aon_pmu_hal_get_touch_int_status(void)
{
	return aon_pmu_ll_get_reg70_int_touched();
}

void aon_pmu_hal_clear_touch_int(uint32_t value)
{
	uint32_t clear_touch_int = 0;
	clear_touch_int = aon_pmu_ll_get_reg43_clr_int_touched();
	clear_touch_int |= value;
	aon_pmu_ll_set_reg43_clr_int_touched(clear_touch_int);
}
void aon_pmu_hal_reg_set(pmu_reg_e reg, uint32_t value)
{
    pmu_address_map_t pmu_addr_map[] = PMU_ADDRESS_MAP;
    pmu_address_map_t *pmu_addr = &pmu_addr_map[reg];
    
    uint32_t pmu_reg_addr = pmu_addr->reg_address;

	REG_WRITE(pmu_reg_addr, value);
}
uint32_t aon_pmu_hal_reg_get(pmu_reg_e reg)
{
    pmu_address_map_t pmu_addr_map[] = PMU_ADDRESS_MAP;
    pmu_address_map_t *pmu_addr = &pmu_addr_map[reg];
    
    uint32_t pmu_reg_addr = pmu_addr->reg_address;

	return REG_READ(pmu_reg_addr);
}

void aon_pmu_hal_wdt_rst_dev_enable()
{
	aon_pmu_ll_set_reg2_value(AON_PMU_REG2_WDT_RST_MASK);
}
void aon_pmu_hal_lpo_src_set(uint32_t lpo_src)
{
    aon_pmu_ll_set_reg41_lpo_config(lpo_src);
}
uint32_t aon_pmu_hal_lpo_src_get()
{
	return aon_pmu_ll_get_reg41_lpo_config();
}
#else
void aon_pmu_hal_set_sleep_parameters(uint32_t value)
{
    aon_pmu_ll_set_reg40_value(s_aon_pmu_hal.hw,value);
}

void aon_pmu_hal_set_wakeup_source_reg(uint32_t value)
{
	aon_pmu_ll_set_reg41_value(s_aon_pmu_hal.hw,value);
}

uint32_t aon_pmu_hal_get_wakeup_source_reg(void)
{
	return aon_pmu_ll_get_reg41_value(s_aon_pmu_hal.hw);
}

void aon_pmu_hal_clear_wakeup_source(wakeup_source_t value)
{
    uint32_t wakeup_source = 0;
    wakeup_source = aon_pmu_ll_get_reg41_wakeup_ena(s_aon_pmu_hal.hw);
	wakeup_source &= ~(0x1 << value);
    aon_pmu_ll_set_reg41_wakeup_ena(s_aon_pmu_hal.hw, wakeup_source);
}
void aon_pmu_hal_set_wakeup_source(wakeup_source_t value)
{
    uint32_t wakeup_source = 0;
    wakeup_source = aon_pmu_ll_get_reg41_wakeup_ena(s_aon_pmu_hal.hw);
	wakeup_source |= (0x1 << value);
    aon_pmu_ll_set_reg41_wakeup_ena(s_aon_pmu_hal.hw, wakeup_source);
}
void aon_pmu_hal_usbplug_int_en(uint32_t value)
{
   uint32_t int_state = 0;
   int_state = aon_pmu_ll_get_reg1_usbplug_int_en(s_aon_pmu_hal.hw);
   int_state |= value;
   aon_pmu_ll_set_reg1_usbplug_int_en(s_aon_pmu_hal.hw, int_state);
}

void aon_pmu_hal_touch_int_en(uint32_t value)
{
	uint32_t int_state = 0;
	int_state = aon_pmu_ll_get_reg1_touch_int_en(s_aon_pmu_hal.hw);
	int_state |= value;
	aon_pmu_ll_set_reg1_touch_int_en(s_aon_pmu_hal.hw, int_state);
}

 uint32_t aon_pmu_hal_get_adc_cal()
{
	return aon_pmu_ll_get_reg7d_adc_cal(s_aon_pmu_hal.hw);
}

#endif
#endif


