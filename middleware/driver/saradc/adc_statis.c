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

#include <os/mem.h>
#include "adc_hw.h"
#include "adc_statis.h"


#if  CONFIG_ADC_STATIS

#define TAG "adc_statis"

static adc_statis_t s_adc_statis = {0};

bk_err_t adc_statis_init(void)
{
	os_memset(&s_adc_statis, 0, sizeof(s_adc_statis));
	return BK_OK;
}


adc_statis_t* adc_statis_get_statis(void)
{
	return &(s_adc_statis);
}

void adc_statis_dump(void)
{
	BK_LOGI(TAG, "dump adc statis:\r\n");
	BK_LOGI(TAG, "adc_isr_cnt:     %d\r\n", s_adc_statis.adc_isr_cnt);
	BK_LOGI(TAG, "adc_rx_total_cnt:       %d\r\n", s_adc_statis.adc_rx_total_cnt);
	BK_LOGI(TAG, "adc_rx_succ_cnt:       %d\r\n", s_adc_statis.adc_rx_succ_cnt);
	BK_LOGI(TAG, "adc_rx_drop_cnt:       %d\r\n", s_adc_statis.adc_rx_drop_cnt);
}

#endif
