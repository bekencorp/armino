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

#include <common/sys_config.h>
#include <components/log.h>
#include <os/mem.h>
#include <driver/adc.h>
#include "sys_rtos.h"
#include <os/os.h>

#define TAG "adc_exam"

#define ADC_BUF_SIZE_1		10
#define ADC_BUF_SIZE_2		5

#define ADC_READ_TIMEOUT	20000
#define ADC_DETEC_CLK	750000
#define ADC_DETEC_SAMPLE_RATE	32
#define ADC_DETEC_STEADY_CTRL	7

#define ADC_DETECT_CHANNEL1	ADC_1
#define ADC_DETECT_CHANNEL2	ADC_2
#define ADC_DETECT_CHANNEL3	ADC_3
#define ADC_DETECT_CHANNEL4	ADC_4
#define ADC_DETECT_CHANNEL5	ADC_5

#define ADC_DETECT_CNT_MAX	0x1000


typedef struct {
	adc_config_t adc_config;
	int task_id;
	int delay_ms;
} adc_task_config_t;

static uint32_t s_adc_task1_sample_cnt = 0;
static adc_task_config_t s_adc_task1_config = {0};
static adc_task_config_t s_adc_task2_config = {0};

static void adc_isr_cb(uint32_t arg)
{
	adc_task_config_t *task_config = (adc_task_config_t *)arg;
	BK_LOGI(TAG, "adc task(%d) sample(%d) finished\n", task_config->task_id, s_adc_task1_sample_cnt);
}

static void adc_task_main(void *arg)
{
	uint16_t average_sample_value = 0;
	uint32_t adc_sample_cnt = 0;
	adc_task_config_t *task_config = (adc_task_config_t *)arg;
	adc_config_t *adc_config = &task_config->adc_config;

	//TODO should NOT init size
	BK_LOG_ON_ERR(bk_adc_init(adc_config->id));

	while (adc_sample_cnt++ < 50) {
		BK_LOG_ON_ERR(bk_adc_set_config(adc_config));
		BK_LOG_ON_ERR(bk_adc_register_isr(adc_isr_cb, ADC_BUF_SIZE_1));
		BK_LOG_ON_ERR(bk_adc_start());
		BK_LOG_ON_ERR(bk_adc_set_channel(adc_config->id));
		BK_LOG_ON_ERR(bk_adc_read(&average_sample_value, ADC_READ_TIMEOUT));
		BK_LOGI(TAG, "adc task(%d) sample value=%d\n", task_config->task_id, average_sample_value);
		rtos_delay_milliseconds(task_config->delay_ms);
	}

	rtos_delete_thread(NULL);
}

int start_adc_task(adc_task_config_t *config)
{
	uint32_t ret = kNoErr;

	ret = rtos_create_thread(NULL,
							 BEKEN_APPLICATION_PRIORITY,
							 "adc_task",
							 (beken_thread_function_t)adc_task_main,
							 1024,
							 config);
	if (ret != kNoErr) {
		BK_LOGE(TAG, "adc task create failed, ret(%x)\n", ret);
		return BK_FAIL;
	}

	return BK_OK;
}

int adc_start_app1(void)
{
	s_adc_task1_config.adc_config.id = ADC_DETECT_CHANNEL1;
	s_adc_task1_config.adc_config.adc_mode = ADC_CONTINUOUS_MODE;
	s_adc_task1_config.adc_config.clk = ADC_DETEC_CLK;
	s_adc_task1_config.adc_config.src_clk = ADC_SCLK_XTAL_26M;
	s_adc_task1_config.adc_config.saturate_mode = ADC_SATURATE_MODE_3;
	s_adc_task1_config.adc_config.sample_rate = ADC_DETEC_SAMPLE_RATE;
	s_adc_task1_config.adc_config.steady_ctrl = ADC_DETEC_STEADY_CTRL;
	s_adc_task1_config.adc_config.adc_filter = 0;

	s_adc_task1_config.task_id = 1;
	s_adc_task1_config.delay_ms = 1000;

	return start_adc_task(&s_adc_task1_config);
}

int adc_start_app2(void)
{
	s_adc_task2_config.adc_config.id = ADC_DETECT_CHANNEL2;
	s_adc_task2_config.adc_config.adc_mode = ADC_CONTINUOUS_MODE;
	s_adc_task2_config.adc_config.clk = ADC_DETEC_CLK;
	s_adc_task2_config.adc_config.src_clk = ADC_SCLK_XTAL_26M;
	s_adc_task2_config.adc_config.saturate_mode = ADC_SATURATE_MODE_3;
	s_adc_task2_config.adc_config.sample_rate = ADC_DETEC_SAMPLE_RATE;
	s_adc_task2_config.adc_config.steady_ctrl = ADC_DETEC_STEADY_CTRL;
	s_adc_task2_config.adc_config.adc_filter = 0;

	s_adc_task2_config.task_id = 2;
	s_adc_task2_config.delay_ms = 2000;

	return start_adc_task(&s_adc_task2_config);
}

int main(void)
{
	os_printf("==================> adc main\n");
	BK_LOG_ON_ERR(bk_adc_driver_init());
	BK_LOG_ON_ERR(adc_start_app1());
	BK_LOG_ON_ERR(adc_start_app2());
	return 0;
}

