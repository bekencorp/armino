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
#include <os/str.h>
#include <os/os.h>
#include <driver/touch.h>
#include <driver/touch_types.h>
#include "sys_driver.h"
#include "touch_driver.h"
#include "aon_pmu_driver.h"
#include <driver/timer.h>


uint32_t g_gain_s = 0;
extern void delay(int num);

static void cli_touch_help(void)
{
	TOUCH_LOGI("touch_single_channel_calib_mode_test {0|1|2|...|15} {0|1|2|3}\r\n");
	TOUCH_LOGI("touch_single_channel_manul_mode_test {0|1|...|15} {calibration_value}\r\n");
	TOUCH_LOGI("touch_multi_channel_scan_mode_test {start|stop} {0|1|2|3}\r\n");
	TOUCH_LOGI("touch_single_channel_multi_calib_test {0|1|...|15} {0|1|2|3}");
}

static void cli_touch_isr(void *param)
{
//	uint32_t int_status = 0;
//	int_status = bk_touch_get_int_status();
//	TOUCH_LOGI("interrupt status = %x\r\n", int_status);
}

static void touch_cyclic_calib_timer_isr(timer_id_t chan)
{
	uint32_t cap_out = 0;
	uint32_t touch_id = 0;
	uint32_t touch_crg[16] = {0};
	uint32_t touch_crg_max = 0;
	uint32_t multi_chann_value = 0xffff;
	touch_config_t touch_config;

	TOUCH_LOGI("multi_channel_cyclic_calib_test start!\r\n");

	bk_touch_clear_int(multi_chann_value);
	bk_touch_int_enable(multi_chann_value, 0);
	bk_touch_scan_mode_enable(0);
	bk_touch_scan_mode_multi_channl_set(0);
	bk_touch_enable(0);

	for(touch_id = 0; touch_id < 16; touch_id++)
	{
		bk_touch_enable(1 << touch_id);
	
		touch_config.sensitivity_level = g_gain_s;
		touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
		touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
		bk_touch_config(&touch_config);
	
		bk_touch_calibration_start();
		cap_out = bk_touch_get_calib_value();
		if (cap_out >= 0x1F0) {
			touch_config.detect_range = TOUCH_DETECT_RANGE_12PF;
			bk_touch_config(&touch_config);
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			if (cap_out >= 0x1F0) {
				touch_config.detect_range = TOUCH_DETECT_RANGE_19PF;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
				cap_out = bk_touch_get_calib_value();
				if (cap_out >= 0x1F0) {
					touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
					bk_touch_config(&touch_config);
					bk_touch_calibration_start();
					cap_out = bk_touch_get_calib_value();
					if (cap_out >= 0x1F0) {
						TOUCH_LOGE("Calibration value is out of the detect range, the channel cannot be used, please select the other channel!\r\n");
						return;
					}
				}
			}
		}
		touch_crg[touch_id] = touch_config.detect_range;
		TOUCH_LOGI("touch[%d] crg = %d, calibration value = %x !\r\n", touch_id, touch_crg[touch_id], cap_out);
		delay(1000);
	}

	for (touch_id = 0; touch_id < 16; touch_id++)
	{
		if (touch_crg_max < touch_crg[touch_id]) {
			touch_crg_max = touch_crg[touch_id];
		}
	}
	TOUCH_LOGI("touch_crg_max = %d\r\n", touch_crg_max);

	for (touch_id = 0; touch_id < 16; touch_id++)
	{
		if (touch_crg_max != touch_crg[touch_id]) {
			bk_touch_enable(1 << touch_id);
			touch_config.detect_range = touch_crg_max;
			bk_touch_config(&touch_config);
			bk_touch_calibration_start();
		}
	}
	bk_touch_scan_mode_multi_channl_set(multi_chann_value);
	bk_touch_scan_mode_enable(1);
	bk_touch_int_enable(multi_chann_value, 1);

}
void cli_touch_single_channel_calib_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t touch_id = 0;
	uint32_t cap_out = 0;
	uint32_t gain_s = 0;
	touch_config_t touch_config;

	if (argc != 3) {
		cli_touch_help();
		return;
	}

	touch_id = os_strtoul(argv[1], NULL, 10) & 0xFF;
	gain_s = os_strtoul(argv[2], NULL, 10) & 0xFF;
	if(touch_id >= 0 && touch_id < 16) {
		TOUCH_LOGI("touch single channel calib mode test %d start!\r\n", touch_id);
		bk_touch_gpio_init(1 << touch_id);
		bk_touch_enable(1 << touch_id);
		bk_touch_register_touch_isr(1 << touch_id, cli_touch_isr, NULL);

		touch_config.sensitivity_level = gain_s;
		touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
		touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
		bk_touch_config(&touch_config);

		bk_touch_scan_mode_enable(0);
		bk_touch_calibration_start();
		cap_out = bk_touch_get_calib_value();
		TOUCH_LOGI("cap_out0 = %x\r\n", cap_out);
		if (cap_out >= 0x1F0) {
			touch_config.detect_range = TOUCH_DETECT_RANGE_12PF;
			bk_touch_config(&touch_config);
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			TOUCH_LOGI("cap_out1 = %x\r\n", cap_out);
			if (cap_out >= 0x1F0) {
				touch_config.detect_range = TOUCH_DETECT_RANGE_19PF;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
				cap_out = bk_touch_get_calib_value();
				TOUCH_LOGI("cap_out2 = %x\r\n", cap_out);
				if (cap_out >= 0x1F0) {
					touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
					bk_touch_config(&touch_config);
					bk_touch_calibration_start();
					cap_out = bk_touch_get_calib_value();
					TOUCH_LOGI("cap_out3 = %x\r\n", cap_out);
					if (cap_out >= 0x1F0) {
						TOUCH_LOGE("Calibration value is out of the detect range, the channel cannot be used, please select the other channel!\r\n");
						return;
					}
				}
			}
		}
		bk_touch_int_enable(1 << touch_id, 1);
	} else {
		TOUCH_LOGE("unsupported touch channel selection command!\r\n");
	}
}

void cli_touch_single_channel_manul_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t calib_value = 0;
	uint32_t cap_out = 0;
	uint32_t touch_id = 0;
	touch_config_t touch_config;

	if (argc != 3) {
		cli_touch_help();
		return;
	}

	touch_id = os_strtoul(argv[1], NULL, 16) & 0xFF;
	if(touch_id >= 0 && touch_id < 16) {
		TOUCH_LOGI("touch single channel manul mode test %d start!\r\n", touch_id);
		bk_touch_gpio_init(1 << touch_id);
		bk_touch_enable(1 <<touch_id);
		bk_touch_register_touch_isr(1 << touch_id, cli_touch_isr, NULL);

		touch_config.sensitivity_level = TOUCH_SENSITIVITY_LEVLE_3;
		touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
		touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
		bk_touch_config(&touch_config);
		bk_touch_scan_mode_enable(0);

		calib_value = os_strtoul(argv[2], NULL, 16) & 0xFFF;
		TOUCH_LOGI("calib_value = %x\r\n", calib_value);
		bk_touch_manul_mode_enable(calib_value);
		bk_touch_int_enable(1 << touch_id, 1);
		cap_out = bk_touch_get_calib_value();
		TOUCH_LOGI("cap_out = %x\r\n", cap_out);
		if(calib_value == cap_out) {
			TOUCH_LOGI("single channel manul mode test is successful!\r\n");
		} else {
			TOUCH_LOGE("single channel manul mode test is failed!\r\n");
			TOUCH_LOGE("please input larger calibration value!\r\n");
			bk_touch_manul_mode_disable();
			bk_touch_disable();
		}
	} else {
		TOUCH_LOGE("unsupported touch channel selection command!\r\n");
	} 
}

void cli_touch_multi_channel_scan_mode_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t multi_chann_value = 0xffff;
	uint32_t touch_crg[16] = {0};
	uint32_t touch_crg_max = 0;
	uint32_t touch_id = 0;
	uint32_t cap_out = 0;
	uint32_t gain_s = 0;
	touch_config_t touch_config;

	if (argc != 3) {
		cli_touch_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		TOUCH_LOGI("multi_channel_scan_mode_test start!\r\n");
		gain_s = os_strtoul(argv[2], NULL, 10) & 0xFF;
		for(touch_id = 0; touch_id < 16; touch_id++)
		{
			bk_touch_gpio_init(1 << touch_id);
			bk_touch_enable(1 << touch_id);
			bk_touch_register_touch_isr(1 << touch_id, cli_touch_isr, NULL);
	
			touch_config.sensitivity_level = gain_s;
			touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
			touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
			bk_touch_config(&touch_config);
	
			bk_touch_scan_mode_enable(0);
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			if (cap_out >= 0x1F0) {
				touch_config.detect_range = TOUCH_DETECT_RANGE_12PF;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
				cap_out = bk_touch_get_calib_value();
				if (cap_out >= 0x1F0) {
					touch_config.detect_range = TOUCH_DETECT_RANGE_19PF;
					bk_touch_config(&touch_config);
					bk_touch_calibration_start();
					cap_out = bk_touch_get_calib_value();
					if (cap_out >= 0x1F0) {
						touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
						bk_touch_config(&touch_config);
						bk_touch_calibration_start();
						cap_out = bk_touch_get_calib_value();
						if (cap_out >= 0x1F0) {
							TOUCH_LOGE("Calibration value is out of the detect range, the channel cannot be used, please select the other channel!\r\n");
							return;
						}
					}
				}
			}
			touch_crg[touch_id] = touch_config.detect_range;
			TOUCH_LOGI("touch[%d] crg = %d, calibration value = %x !\r\n", touch_id, touch_crg[touch_id], cap_out);
			delay(1000);
		}

		for (touch_id = 0; touch_id < 16; touch_id++)
		{
			if (touch_crg_max < touch_crg[touch_id]) {
				touch_crg_max = touch_crg[touch_id];
			}
		}
		TOUCH_LOGI("touch_crg_max = %d\r\n", touch_crg_max);

		for (touch_id = 0; touch_id < 16; touch_id++)
		{
			if (touch_crg_max != touch_crg[touch_id]) {
				bk_touch_enable(1 << touch_id);
				touch_config.detect_range = touch_crg_max;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
			}
		}

		bk_touch_scan_mode_multi_channl_set(multi_chann_value);
		bk_touch_scan_mode_enable(1);
		bk_touch_int_enable(multi_chann_value, 1);
	} else if (os_strcmp(argv[1], "stop") == 0) {
		TOUCH_LOGI("multi_channel_scan_mode_test stop!\r\n");
		bk_touch_scan_mode_enable(0);
		bk_touch_disable();
	}
}

void cli_touch_single_channel_multi_calib_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t touch_id = 0;
	uint32_t cap_out = 0;
	uint32_t gain_s = 0;
	uint32_t count = 0;
	touch_config_t touch_config;

	if (argc != 3) {
		cli_touch_help();
		return;
	}

	touch_id = os_strtoul(argv[1], NULL, 10) & 0xFF;
	gain_s = os_strtoul(argv[2], NULL, 10) & 0xFF;
	if(touch_id >= 0 && touch_id < 16) {
		TOUCH_LOGI("touch single channel calib mode test %d start!\r\n", touch_id);
		bk_touch_gpio_init(1 << touch_id);
		bk_touch_enable(1 << touch_id);
		bk_touch_register_touch_isr(1 << touch_id, cli_touch_isr, NULL);

		touch_config.sensitivity_level = gain_s;
		touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
		touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
		bk_touch_config(&touch_config);

		bk_touch_scan_mode_enable(0);
		bk_touch_calibration_start();
		cap_out = bk_touch_get_calib_value();
		TOUCH_LOGI("cap_out0 = %x\r\n", cap_out);
		if (cap_out >= 0x1F0) {
			touch_config.detect_range = TOUCH_DETECT_RANGE_12PF;
			bk_touch_config(&touch_config);
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			TOUCH_LOGI("cap_out1 = %x\r\n", cap_out);
			if (cap_out >= 0x1F0) {
				touch_config.detect_range = TOUCH_DETECT_RANGE_19PF;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
				cap_out = bk_touch_get_calib_value();
				TOUCH_LOGI("cap_out2 = %x\r\n", cap_out);
				if (cap_out >= 0x1F0) {
					touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
					bk_touch_config(&touch_config);
					bk_touch_calibration_start();
					cap_out = bk_touch_get_calib_value();
					TOUCH_LOGI("cap_out3 = %x\r\n", cap_out);
					if (cap_out >= 0x1F0) {
						TOUCH_LOGE("Calibration value is out of the detect range, the channel cannot be used, please select the other channel!\r\n");
						return;
					}
				}
			}
		}

		for (count = 0; count < 5000; count++)
		{
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			TOUCH_LOGI("cap_out = %x\r\n", cap_out);
			delay(10000);
		}

		bk_touch_int_enable(1 << touch_id, 1);
	} else {
		TOUCH_LOGE("unsupported touch channel selection command!\r\n");
	}
}

void cli_touch_multi_channel_cyclic_calib_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	uint32_t multi_chann_value = 0xffff;
	uint32_t touch_crg[16] = {0};
	uint32_t touch_crg_max = 0;
	uint32_t touch_id = 0;
	uint32_t cap_out = 0;
	touch_config_t touch_config;

	if (argc != 3) {
		cli_touch_help();
		return;
	}
	
	if (os_strcmp(argv[1], "start") == 0) {
		g_gain_s = os_strtoul(argv[2], NULL, 10) & 0xFF;
		for(touch_id = 0; touch_id < 16; touch_id++)
		{
			bk_touch_gpio_init(1 << touch_id);
			bk_touch_enable(1 << touch_id);
			bk_touch_register_touch_isr(1 << touch_id, cli_touch_isr, NULL);
	
			touch_config.sensitivity_level = g_gain_s;
			touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
			touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
			bk_touch_config(&touch_config);
	
			bk_touch_scan_mode_enable(0);
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			if (cap_out >= 0x1F0) {
				touch_config.detect_range = TOUCH_DETECT_RANGE_12PF;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
				cap_out = bk_touch_get_calib_value();
				if (cap_out >= 0x1F0) {
					touch_config.detect_range = TOUCH_DETECT_RANGE_19PF;
					bk_touch_config(&touch_config);
					bk_touch_calibration_start();
					cap_out = bk_touch_get_calib_value();
					if (cap_out >= 0x1F0) {
						touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
						bk_touch_config(&touch_config);
						bk_touch_calibration_start();
						cap_out = bk_touch_get_calib_value();
						if (cap_out >= 0x1F0) {
							TOUCH_LOGE("Calibration value is out of the detect range, the channel cannot be used, please select the other channel!\r\n");
							return;
						}
					}
				}
			}
			touch_crg[touch_id] = touch_config.detect_range;
			TOUCH_LOGI("touch[%d] crg = %d, calibration value = %x !\r\n", touch_id, touch_crg[touch_id], cap_out);
			delay(1000);
		}

		for (touch_id = 0; touch_id < 16; touch_id++)
		{
			if (touch_crg_max < touch_crg[touch_id]) {
				touch_crg_max = touch_crg[touch_id];
			}
		}
		TOUCH_LOGI("touch_crg_max = %d\r\n", touch_crg_max);

		for (touch_id = 0; touch_id < 16; touch_id++)
		{
			if (touch_crg_max != touch_crg[touch_id]) {
				bk_touch_enable(1 << touch_id);
				touch_config.detect_range = touch_crg_max;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
			}
		}

		bk_touch_scan_mode_multi_channl_set(multi_chann_value);
		bk_touch_scan_mode_enable(1);
		bk_touch_int_enable(multi_chann_value, 1);


		ret = bk_timer_start(TIMER_ID1, 10000, touch_cyclic_calib_timer_isr);
		if (ret != BK_OK) {
			os_printf("Timer start failed\r\n");
		}
	}
}


