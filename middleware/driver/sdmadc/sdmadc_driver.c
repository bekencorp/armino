// Copyright 2022-2023 Beken
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

#include <os/os.h>
#include <os/mem.h>
#include "sdmadc_hal.h"
#include "sys_driver.h"
#include <driver/int.h>

typedef struct {
	sdmadc_hal_t hal;
	uint16_t chan_init_bits;
} adc_driver_t;

typedef struct {
	beken_semaphore_t sdmadc_read_sema;
	beken_mutex_t sdmadc_mutex;
} sdmadc_dev_t;

typedef struct {
	int16_t *buf;
	/* The size is in unit of uint16_t */
	volatile uint16_t size;
	volatile uint16_t sample_cnt;
} sdmadc_buf_t;

static bool s_sdmadc_driver_is_init = false;
static sdmadc_dev_t s_sdmadc_dev = {0};
static sdmadc_buf_t s_sdmadc_buf = {0};
static sdmadc_config_t s_sdmadc_config = {0};

sdmadc_calibrate_val sdmadc_val = {
#if (CONFIG_SOC_BK7236XX)
    22444, 27490 /* 1Volt:-10234, 2Volt:-5278 +32768 offset*/
#else
    /*TBD*/
#endif
};

static void sdmadc_isr(void);
bk_err_t bk_sdmadc_set_cfg(sdmadc_config_t * config);

bk_err_t bk_sdmadc_driver_init(void)
{
	int ret;

	if (s_sdmadc_driver_is_init) {
		return BK_OK;
	}

	if (s_sdmadc_buf.buf) {
		os_free(s_sdmadc_buf.buf);
	}

	s_sdmadc_buf.size = CONFIG_SDMADC_BUF_SIZE;
	s_sdmadc_buf.buf = (int16_t*)os_zalloc(CONFIG_SDMADC_BUF_SIZE << 1);
	if (!s_sdmadc_buf.buf) {
		return BK_ERR_NO_MEM;
	}

	if (!s_sdmadc_dev.sdmadc_mutex) {
		ret = rtos_init_mutex(&s_sdmadc_dev.sdmadc_mutex);
		if (kNoErr != ret) {
			os_free(s_sdmadc_buf.buf);
			return BK_FAIL;
		}
	}

	if (!s_sdmadc_dev.sdmadc_read_sema) {
		ret = rtos_init_semaphore(&(s_sdmadc_dev.sdmadc_read_sema), 1);
		if (BK_OK != ret) {
			os_free(s_sdmadc_buf.buf);
			rtos_deinit_mutex(&s_sdmadc_dev.sdmadc_mutex);
			return BK_FAIL;
		}
	}

	bk_int_isr_register(INT_SRC_SDMADC, sdmadc_isr, NULL);

	s_sdmadc_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_sdmadc_driver_deinit(void)
{
	if (!s_sdmadc_driver_is_init)
		return BK_OK;

	bk_int_isr_unregister(INT_SRC_SDMADC);

	if (s_sdmadc_dev.sdmadc_mutex)
		rtos_deinit_mutex(&s_sdmadc_dev.sdmadc_mutex);

	if (s_sdmadc_dev.sdmadc_read_sema)
		rtos_deinit_semaphore(&(s_sdmadc_dev.sdmadc_read_sema));

	os_free(s_sdmadc_buf.buf);
	s_sdmadc_buf.buf = NULL;
	s_sdmadc_buf.size = 0;

	os_memset(&s_sdmadc_config, 0, sizeof(s_sdmadc_config));

	s_sdmadc_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_sdmadc_init()
{
	if(!s_sdmadc_driver_is_init)
		return BK_FAIL;
	sdmadc_hal_enable_efuse_cken();
	sdmadc_hal_enable_int();
	sys_drv_analog_reg2_set(0xA0200000);
	return BK_OK;
}

bk_err_t bk_sdmadc_deinit()
{
	sdmadc_hal_disable_int();
	return BK_OK;
}

bk_err_t bk_sdmadc_acquire()
{
	return rtos_lock_mutex(&s_sdmadc_dev.sdmadc_mutex);
}

bk_err_t bk_sdmadc_release()
{
	return rtos_unlock_mutex(&s_sdmadc_dev.sdmadc_mutex);
}

bk_err_t bk_sdmadc_start()
{
	os_memset(s_sdmadc_buf.buf, 0, sizeof(s_sdmadc_buf.buf)*s_sdmadc_buf.size);
	s_sdmadc_buf.sample_cnt = 0;
	sdmadc_hal_enable_soft_rst();
	sdmadc_hal_enable_sample();
	bk_sdmadc_acquire();
	return BK_OK;

}

bk_err_t bk_sdmadc_stop()
{
	bk_sdmadc_release();
	sdmadc_hal_disable_sample();
	sdmadc_hal_disable_soft_rst();
	s_sdmadc_buf.size = CONFIG_SDMADC_BUF_SIZE;
	return BK_OK;
}

bk_err_t bk_sdmadc_read_average(int16_t* average, uint32_t size)
{
	if (size > s_sdmadc_buf.size) {
		os_printf("size %d too big\r\n",size);
		return BK_FAIL;
	} else {
		s_sdmadc_buf.size = size;
	}
	int32_t sum = 0;
	bk_sdmadc_start();
	rtos_get_semaphore(&(s_sdmadc_dev.sdmadc_read_sema),1000000);
	for(int i = 1; i < s_sdmadc_buf.sample_cnt; ++i){
		sum += s_sdmadc_buf.buf[i];
	}
	*average = sum/(s_sdmadc_buf.sample_cnt-1);
	return BK_OK;
}

bk_err_t bk_sdmadc_single_read(int16_t* average, uint16_t channel)
{
	int32_t sum = 0;

	s_sdmadc_config.samp_mode = 0x1;
	s_sdmadc_config.samp_numb = 0x3;
	s_sdmadc_config.samp_chan = channel;
	s_sdmadc_config.comp_bpss = 0x1;
	s_sdmadc_config.cic2_bpss = 0x1;
	s_sdmadc_config.cic2_gain = 0x2d;
	s_sdmadc_config.int_enable = 0x1;
	s_sdmadc_config.cali_offset = 0x0;
	s_sdmadc_config.cali_gains  = 0x1000;
	bk_sdmadc_set_cfg(&s_sdmadc_config);

	bk_sdmadc_start();
	rtos_get_semaphore(&(s_sdmadc_dev.sdmadc_read_sema),1000000);
	for(int i = 1; i < s_sdmadc_buf.sample_cnt; ++i){
		sum += s_sdmadc_buf.buf[i];
	}

	*average = sum/(s_sdmadc_buf.sample_cnt-1);
	return BK_OK;
}

bk_err_t bk_sdmadc_continuous_read(int16_t* average, uint32_t size, uint16_t channel)
{
	if (size > s_sdmadc_buf.size) {
		os_printf("size %d too big\r\n",size);
		return BK_FAIL;
	} else {
		s_sdmadc_buf.size = size;
	}

	int32_t sum = 0;

	s_sdmadc_config.samp_mode = 0x0;
	s_sdmadc_config.samp_numb = 0x0;
	s_sdmadc_config.samp_chan = channel;
	s_sdmadc_config.comp_bpss = 0x1;
	s_sdmadc_config.cic2_bpss = 0x1;
	s_sdmadc_config.cic2_gain = 0x2d;
	s_sdmadc_config.int_enable = 0x8;
	s_sdmadc_config.cali_offset = 0x0;
	s_sdmadc_config.cali_gains  = 0x1000;
	bk_sdmadc_set_cfg(&s_sdmadc_config);

	bk_sdmadc_start();
	rtos_get_semaphore(&(s_sdmadc_dev.sdmadc_read_sema),1000000);

	for(int i = 1; i < s_sdmadc_buf.sample_cnt; ++i){
		sum += s_sdmadc_buf.buf[i];
	}

	*average = sum/(s_sdmadc_buf.sample_cnt-1);

	return BK_OK;

}

bk_err_t bk_sdmadc_set_channel(uint32_t channel)
{
	sdmadc_hal_set_sample_channel(channel);
	return BK_OK;
}

bk_err_t bk_sdmadc_set_sample_mode(sdmadc_mode_t mode)
{
	sdmadc_hal_set_sample_mode(mode);
	return BK_OK;
}

bk_err_t bk_sdmadc_set_cfg(sdmadc_config_t *config)
{
	sdmadc_hal_set_sample_mode(config->samp_mode);
	sdmadc_hal_set_sample_numb(config->samp_numb);
	sdmadc_hal_set_sample_channel(config->samp_chan);
	sdmadc_hal_set_comp_bypass(config->comp_bpss);
	sdmadc_hal_set_cic2_bypass(config->cic2_bpss);
	sdmadc_hal_set_cic2_gains(config->cic2_gain);
	sdmadc_hal_set_intr_enable(config->int_enable);
	sdmadc_hal_set_cali_offset(config->cali_offset);
	sdmadc_hal_set_cali_gain(config->cali_gains);
	return BK_OK;
}

bk_err_t bk_sdmadc_read_raw(int16_t* buf, uint32_t size)
{
	int32_t sum = 0;
	if (size > s_sdmadc_buf.size) {
		os_printf("size %d too big\r\n",size);
		return BK_FAIL;
	} else {
		s_sdmadc_buf.size = size;
	}
	bk_sdmadc_start();
	rtos_get_semaphore(&(s_sdmadc_dev.sdmadc_read_sema),1000000);
	for(int i = 0; i < s_sdmadc_buf.sample_cnt;i++){
		sum += s_sdmadc_buf.buf[i];
	}
	os_memcpy(buf, s_sdmadc_buf.buf, (size<<1));
	return BK_OK;
}

float bk_sdmadc_calculate_voltage(int16_t adc_val)
{
	float practic_voltage;
	/* (adc_val - low) / (practic_voltage - 1Volt) = (high - low) / 1Volt */
	/* practic_voltage = (adc_val - low) / (high - low) + 1Volt */
	practic_voltage = (float)(adc_val - sdmadc_val.low);
	practic_voltage = (practic_voltage / (float)(sdmadc_val.high - sdmadc_val.low)) + 1;

	return practic_voltage;
}

static void sdmadc_isr()
{
	uint32_t stat = sdmadc_hal_get_sample_status();
	if (s_sdmadc_config.samp_mode == SDMADC_SINGLE_STEP_MODE) {
		while (smdadc_hal_is_fifo_empty_int_triggered()) {
			s_sdmadc_buf.buf[s_sdmadc_buf.sample_cnt++] = sdmadc_hal_get_sample_data();
		}
		bk_sdmadc_stop();
		rtos_set_semaphore(&(s_sdmadc_dev.sdmadc_read_sema));
	} else if(s_sdmadc_config.samp_mode == SDMADC_CONTINUOUS_MODE) {
		while ((smdadc_hal_is_fifo_empty_int_triggered()) &&
				(s_sdmadc_buf.sample_cnt < s_sdmadc_buf.size)) {
			if (s_sdmadc_buf.sample_cnt < s_sdmadc_buf.size) {
				s_sdmadc_buf.buf[s_sdmadc_buf.sample_cnt++] = sdmadc_hal_get_sample_data();
			}
			if (s_sdmadc_buf.sample_cnt == s_sdmadc_buf.size) {
				bk_sdmadc_stop();
				rtos_set_semaphore(&(s_sdmadc_dev.sdmadc_read_sema));
			}
		}
	}
	sdmadc_hal_clear_interrupt_status(stat);
}
