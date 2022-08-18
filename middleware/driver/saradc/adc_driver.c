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
#include <os/mem.h>
#include <driver/gpio.h>
#include "gpio_driver.h"
#include <os/os.h>
#include "adc_hal.h"
#include "adc_statis.h"
#include "adc_driver.h"
#include <driver/int.h>
#include "icu_driver.h"
#include "power_driver.h"
#include "clock_driver.h"
#include <driver/adc.h>
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "sys_driver.h"
#include "iot_adc.h"
#include "bk_saradc.h"
#include "common/bk_err.h"

static void adc_isr(void) __SECTION(".itcm");
static bk_err_t adc_read_fifo(void) __SECTION(".itcm");
bk_err_t bk_adc_stop(void) __SECTION(".itcm");

typedef struct
{
	adc_isr_t callback;
	uint32_t param;
    IotAdcCallback_t     iot_callback;
    void *               p_iot_context; 
} adc_callback_t;

typedef struct {
	adc_hal_t hal;
	uint16_t chan_init_bits;
} adc_driver_t;

typedef struct {
	beken_semaphore_t adc_read_sema;
	beken_mutex_t adc_mutex;
} adc_dev_t;

typedef struct {
	uint16_t *buf;
	/* The size is in unit of uint16_t */
	uint16_t size;
	uint16_t sample_cnt;
} adc_buf_t;

static adc_driver_t s_adc = {0};
static bool s_adc_driver_is_init = false;
static adc_dev_t s_adc_dev = {NULL};
static adc_buf_t s_adc_buf = {0};
static adc_callback_t s_adc_read_isr = {NULL};
static adc_statis_t* s_adc_statis = NULL;

saradc_calibrate_val saradc_val = {
#if (CONFIG_SOC_BK7256XX)
    0xD40, 0x1A72 /* 1Volt, 2Volt*/
#else
    0x55, 0x354
#endif
};

adc_config_t g_adc_cfg[10] = {0};


//TODO - by Frank
//1. ADC id range confirm
//2. Add API for ADC saturate mode, hide it for application
//3. Considering how/when to use cli_done/calib_trig bits?
//4. adc_hal_init to reset the ADC to default value
//5. Currently ADC ISR is always enabled once ADC is started, can we provide ADC
//   API that don't need to enable ADC ISR?
//6. Make adc_buf_t.buf configurable in bk_adc_init() since different application may
//   use different buffer size.
//7. If necessary, provide ADC API specific for calibration that don't need ADC ISR.
//8. Hide the difference of ADC RAW data
//9. Update doc accordingly
//10. adc_chan_t to adc_chan_t

#define ADC_SAMPLE_CNT_DEFAULT 32

#define ADC_RETURN_ON_NOT_INIT() do {\
		if (!s_adc_driver_is_init) {\
			ADC_LOGE("adc driver not init\r\n");\
			return BK_ERR_ADC_NOT_INIT;\
		}\
	} while(0)

#define ADC_RETURN_ON_INVALID_CHAN(id) do {\
		if (!adc_hal_is_valid_channel(&s_adc.hal, (id))) {\
			ADC_LOGE("ADC id number(%d) is invalid\r\n", (id));\
			return BK_ERR_ADC_INVALID_CHAN;\
		}\
	} while(0)

#define ADC_RETURN_ON_INVALID_MODE(mode) do {\
		if ((mode) >= ADC_NONE_MODE) {\
			return BK_ERR_ADC_INVALID_MODE;\
		}\
	} while(0)

#define ADC_RETURN_ON_INVALID_SRC_CLK(src_clk) do {\
		if ((src_clk) >= ADC_SCLK_NONE) {\
			return BK_ERR_ADC_INVALID_SCLK_MODE;\
		}\
	} while(0)


static void adc_isr(void);
static void adc_flush(void);

static void adc_init_gpio(adc_chan_t chan)
{
	if (adc_hal_is_analog_channel(&s_adc.hal, chan))
		return;

	adc_gpio_map_t adc_map_table[] = ADC_DEV_MAP;
	adc_gpio_map_t *adc_map = &adc_map_table[chan];

	//TODO optimize it
	if(chan == 0) {
		uint32_t param = PARAM_SARADC_BT_TXSEL_BIT;

#if CONFIG_SYSTEM_CTRL
		sys_drv_analog_reg4_bits_or(param);// to do,need remove old interface after all adaption is finished
#else
		sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_ANALOG_CTRL4_SET, &param);
#endif
	}

	gpio_dev_map(adc_map->gpio_id, adc_map->gpio_dev);
	bk_gpio_disable_pull(adc_map->gpio_id);
	bk_gpio_disable_input(adc_map->gpio_id);
	bk_gpio_disable_output(adc_map->gpio_id);
}

static void adc_deinit_gpio(adc_chan_t chan)
{
	if (adc_hal_is_analog_channel(&s_adc.hal, chan))
		return;

	adc_gpio_map_t adc_map_table[] = ADC_DEV_MAP;
	adc_gpio_map_t *adc_map = &adc_map_table[chan];

	gpio_dev_unmap(adc_map->gpio_id);
}

static void adc_enable_block(void)
{
#if (!CONFIG_SOC_BK7231N) && (!CONFIG_SOC_BK7256XX)
	//TODO - optimize it after sysctrl driver optimized!
	uint32_t param = BLK_BIT_SARADC;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
#endif
}

static bk_err_t adc_chan_init_common(adc_chan_t chan)
{
	bk_err_t ret = 0;

	sys_drv_sadc_pwr_up();
	adc_enable_block();
	adc_hal_init(&s_adc.hal);
	adc_init_gpio(chan);
	adc_hal_sel_channel(&s_adc.hal, chan);

	s_adc.chan_init_bits |= BIT(chan);

	return ret;
}

static bk_err_t adc_chan_deinit_common(adc_chan_t chan)
{
	s_adc.chan_init_bits &= ~(BIT(chan));

	adc_hal_stop_commom(&s_adc.hal);
	sys_drv_sadc_int_disable();

	adc_flush();

	sys_drv_sadc_pwr_down();
	adc_deinit_gpio(chan);
	return BK_OK;
}

static bk_err_t adc_read_fifo(void)
{

	while(!adc_hal_is_fifo_empty(&s_adc.hal)) {
		ADC_STATIS_INC(s_adc_statis->adc_rx_total_cnt);

		if(s_adc_buf.sample_cnt < s_adc_buf.size) {
			s_adc_buf.buf[s_adc_buf.sample_cnt++] = adc_hal_get_adc_data(&s_adc.hal);

			ADC_STATIS_INC(s_adc_statis->adc_rx_succ_cnt);
		} else {
			if((adc_hal_get_mode(&s_adc.hal) == ADC_CONTINUOUS_MODE)) {
				adc_hal_get_adc_data(&s_adc.hal);

				ADC_STATIS_INC(s_adc_statis->adc_rx_drop_cnt);
			} else {
				//software mode is NOT used!
			}
		}
	}


	return BK_OK;
}

static void adc_flush(void)
{
	adc_hal_clear_int_status(&s_adc.hal);

	while(!adc_hal_is_fifo_empty(&s_adc.hal)) {
		adc_hal_get_adc_data(&s_adc.hal);
	}
}

bk_err_t bk_adc_driver_init(void)
{
	int ret;
    uint32_t i;

	if (s_adc_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_adc, 0, sizeof(s_adc));
    os_memset(&g_adc_cfg, 0, sizeof(g_adc_cfg));

	if (s_adc_buf.buf) {
		os_free(s_adc_buf.buf);
	}

	s_adc_buf.size = CONFIG_ADC_BUF_SIZE;
	s_adc_buf.buf = (uint16_t*)os_zalloc(CONFIG_ADC_BUF_SIZE<<1);
	if (!s_adc_buf.buf) {
		return BK_ERR_NO_MEM;
	}

	if (!s_adc_dev.adc_mutex) {
		ret = rtos_init_mutex(&s_adc_dev.adc_mutex);
		if (kNoErr != ret) {
			os_free(s_adc_buf.buf);
			return BK_ERR_ADC_INIT_MUTEX;
		}
	}

	if (!s_adc_dev.adc_read_sema) {
		ret = rtos_init_semaphore(&(s_adc_dev.adc_read_sema), 1);
		if (BK_OK != ret) {
			os_free(s_adc_buf.buf);
			rtos_deinit_mutex(&s_adc_dev.adc_mutex);
			return BK_ERR_ADC_INIT_READ_SEMA;
		}
	}

	bk_int_isr_register(INT_SRC_SARADC, adc_isr, NULL);

	adc_statis_init();
	s_adc_statis = adc_statis_get_statis();

    for(i = 0; i < (sizeof(g_adc_cfg)/sizeof(g_adc_cfg[0])); i++)
    {
        g_adc_cfg[i].clk = DEFAULT_ADC_CLK;
        g_adc_cfg[i].sample_rate = DEFAULT_ADC_SAMPLE_RATE;
        g_adc_cfg[i].adc_filter = 0;
        g_adc_cfg[i].steady_ctrl = DEFAULT_ADC_STEADY_TIME;
        g_adc_cfg[i].adc_mode = DEFAULT_ADC_MODE;
        g_adc_cfg[i].chan = i;
        g_adc_cfg[i].src_clk = DEFAULT_ADC_SCLK;
        g_adc_cfg[i].saturate_mode = DEFAULT_SATURATE_MODE;
        g_adc_cfg[i].output_buf = NULL;
        g_adc_cfg[i].output_buf_len = 0;
        g_adc_cfg[i].is_open = 0;
    }

	s_adc_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_adc_acquire(void)
{
	ADC_LOGD("acquire\n");
	return rtos_lock_mutex(&s_adc_dev.adc_mutex);
}

bk_err_t bk_adc_release(void)
{
	ADC_LOGD("release\n");
	return rtos_unlock_mutex(&s_adc_dev.adc_mutex);
}

bk_err_t bk_adc_driver_deinit(void)
{
	if (!s_adc_driver_is_init)
		return BK_OK;

	bk_adc_deinit(ADC_1); // Deinit ADC_1 to deinit ADC
	adc_hal_deinit(&s_adc.hal);
	bk_int_isr_unregister(INT_SRC_SARADC);

	if (s_adc_dev.adc_mutex)
		rtos_deinit_mutex(&s_adc_dev.adc_mutex);

	if (s_adc_dev.adc_read_sema)
		rtos_deinit_semaphore(&(s_adc_dev.adc_read_sema));

	os_free(s_adc_buf.buf);
	s_adc_buf.buf = NULL;
	s_adc_buf.size = 0;

    os_memset(&g_adc_cfg, 0, sizeof(g_adc_cfg));

	s_adc_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_adc_init(adc_chan_t adc_chan)
{
	ADC_RETURN_ON_NOT_INIT();
	ADC_RETURN_ON_INVALID_CHAN(adc_chan);

#if (CONFIG_SARADC_NEED_FLUSH)
	adc_flush();
#endif
	adc_chan_init_common(adc_chan);
	bk_adc_set_sample_cnt(ADC_SAMPLE_CNT_DEFAULT);

	return BK_OK;
}

bk_err_t bk_adc_deinit(adc_chan_t chan)
{
	ADC_RETURN_ON_NOT_INIT();
	ADC_RETURN_ON_INVALID_CHAN(chan);

	adc_chan_deinit_common(chan);

	return BK_OK;
}

bk_err_t bk_adc_start(void)
{
	if(adc_hal_check_adc_busy(&s_adc.hal))
		return BK_ERR_ADC_BUSY;

	if (adc_hal_check_adc_enable(&s_adc.hal))
		return BK_OK;

	sys_drv_sadc_int_enable();

	return BK_OK;
}

bk_err_t bk_adc_stop(void)
{
	uint32_t ret = 0;

	if (!adc_hal_check_adc_enable(&s_adc.hal))
		return BK_OK;

	s_adc_buf.sample_cnt = 0;

	adc_hal_stop_commom(&s_adc.hal);
	adc_flush();

	return ret;
}

static bk_err_t adc_block_read(uint32_t timeout)
{
	if(adc_hal_check_adc_busy(&s_adc.hal))
		return BK_ERR_ADC_BUSY;

	if(adc_hal_get_mode(&s_adc.hal) == ADC_SINGLE_STEP_MODE) {
		adc_hal_start_commom(&s_adc.hal);

		return adc_hal_get_single_step_adc_data(&s_adc.hal);
	}

	adc_hal_start_commom(&s_adc.hal);

	int ret = rtos_get_semaphore(&(s_adc_dev.adc_read_sema), timeout);
	if(ret != kNoErr)
		return BK_ERR_ADC_GET_READ_SEMA;

	return BK_OK;
}

bk_err_t bk_adc_read_raw(uint16_t* buf, uint32_t size, uint32_t timeout)
{
	uint32_t ret = 0;

	//TODO init check

	if (size > s_adc_buf.size) {
		ADC_LOGE("adc size %d too big\n", size);
		return BK_ERR_ADC_SIZE_TOO_BIG;
	}

	ret = adc_block_read(timeout);
	if (BK_OK != ret)
		return ret;

	os_memcpy(buf, s_adc_buf.buf, (size<<1));

	return s_adc_buf.size;
}

bk_err_t bk_adc_register_isr(adc_isr_t isr, uint32_t param)
{
	ADC_RETURN_ON_NOT_INIT();

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_adc_read_isr.callback = isr;
	s_adc_read_isr.param = param;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_adc_set_clk(adc_src_clk_t src_clk, uint32_t clk)
{
	ADC_RETURN_ON_INVALID_SRC_CLK(src_clk);

	adc_hal_set_clk(&s_adc.hal, src_clk, clk);

	return BK_OK;
}

bk_err_t bk_adc_set_channel(adc_chan_t   adc_chan)
{
	ADC_RETURN_ON_INVALID_CHAN(adc_chan);

	adc_hal_sel_channel(&s_adc.hal, adc_chan);

	adc_deinit_gpio(adc_chan);

	return BK_OK;
}

bk_err_t bk_adc_set_sample_rate(uint32_t sample_rate)
{
	adc_hal_set_sample_rate(&s_adc.hal, sample_rate);

	return BK_OK;
}

bk_err_t bk_adc_set_filter(uint32_t filter)
{
	adc_hal_set_adc_filter(&s_adc.hal, filter);

	return BK_OK;
}

bk_err_t bk_adc_set_steady_time(uint32_t steady_ctrl)
{
	adc_hal_set_steady_ctrl(&s_adc.hal, steady_ctrl);

	return BK_OK;
}

bk_err_t bk_adc_set_sample_cnt(uint32_t cnt)
{
	if (cnt == 0) {
		cnt = SOC_ADC_SAMPLE_CNT_MAX;
	}

	if (cnt > SOC_ADC_SAMPLE_CNT_MAX) {
		ADC_LOGW("sample cnt %d too big, default to %d\n", cnt, SOC_ADC_SAMPLE_CNT_MAX);
		cnt = SOC_ADC_SAMPLE_CNT_MAX;
	}

	adc_hal_set_fifo_threshold(&s_adc.hal, cnt);
	return BK_OK;
}

bk_err_t bk_adc_set_saturate_mode(adc_saturate_mode_t mode)
{
	return adc_hal_set_saturate_mode(&s_adc.hal, mode);
}

bk_err_t bk_adc_enable_bypass_clalibration(void)
{
	adc_hal_enable_bypass_calib(&s_adc.hal);
	return BK_OK;
}

bk_err_t bk_adc_disable_bypass_clalibration(void)
{
	adc_hal_disable_bypass_calib(&s_adc.hal);
	return BK_OK;
}

bk_err_t bk_adc_get_saturate_mode(adc_saturate_mode_t *mode)
{
	return BK_OK;
	//return adc_hal_get_sat_ctrl(&s_adc.hal);
}

bk_err_t bk_adc_set_mode(adc_mode_t adc_mode)
{
	ADC_RETURN_ON_INVALID_MODE(adc_mode);

	return adc_hal_set_mode(&s_adc.hal, adc_mode);
}

adc_mode_t bk_adc_get_mode(void)
{
	return adc_hal_get_mode(&s_adc.hal);
}

bk_err_t bk_adc_set_config(adc_config_t *config)
{
    BK_RETURN_ON_NULL(config);
	ADC_RETURN_ON_INVALID_MODE(config->adc_mode);
	ADC_RETURN_ON_INVALID_SRC_CLK(config->src_clk);
	ADC_RETURN_ON_INVALID_CHAN(config->chan);

	adc_hal_set_clk(&s_adc.hal, config->src_clk, config->clk);
	adc_hal_set_mode(&s_adc.hal, config->adc_mode);

	adc_hal_set_steady_ctrl(&s_adc.hal, config->steady_ctrl);
	adc_hal_set_saturate_mode(&s_adc.hal, config->saturate_mode);

	bk_adc_set_channel(config->chan);

	if(config->adc_mode == ADC_CONTINUOUS_MODE) {
		adc_hal_set_sample_rate(&s_adc.hal, config->sample_rate);
		adc_hal_set_adc_filter(&s_adc.hal, config->adc_filter);
	}
	return BK_OK;
}

bk_err_t bk_adc_get_config(uint32 adc_ch, adc_config_t **config)
{
    *config = &g_adc_cfg[adc_ch];
    return BK_OK;
}

int saradc_hal_is_fifo_empty(void)
{
    return (!adc_hal_is_fifo_empty(&s_adc.hal));
}

bk_err_t bk_saradc_start_int_disable(void)
{
    if(adc_hal_check_adc_busy(&s_adc.hal))
        return BK_ERR_ADC_BUSY;
    if (adc_hal_check_adc_enable(&s_adc.hal))
        return BK_OK;
    sys_drv_sadc_int_disable();
    return BK_OK;
}

void saradc_hal_start_enable(void)
{
    adc_hal_start_commom(&s_adc.hal);
}

uint32_t bk_saradc_read_raw_data(uint32_t timeout)
{
    uint32_t raw_data = 0;
    raw_data = adc_hal_get_adc_data(&s_adc.hal);
    return raw_data;
}

#if SARADC_AUTOTEST
bk_err_t bk_saradc_set_config(adc_config_t *config, uint32_t div)
{
    BK_RETURN_ON_NULL(config);
    ADC_RETURN_ON_INVALID_MODE(config->adc_mode);
    ADC_RETURN_ON_INVALID_SRC_CLK(config->src_clk);
    ADC_RETURN_ON_INVALID_CHAN(config->chan);
    adc_hal_set_div(&s_adc.hal, div);
    adc_hal_set_mode(&s_adc.hal, config->adc_mode);
    adc_hal_set_steady_ctrl(&s_adc.hal, config->steady_ctrl);
    adc_hal_set_saturate_mode(&s_adc.hal, config->saturate_mode);
    if(config->adc_mode == ADC_CONTINUOUS_MODE)
    {
        os_printf("config->sample_rate = %d\r\n",config->sample_rate);
        adc_hal_set_sample_rate(&s_adc.hal, config->sample_rate);
        adc_hal_set_adc_filter(&s_adc.hal, config->adc_filter);
    }
    return BK_OK;
}
#endif

bk_err_t bk_adc_read(uint16_t* data, uint32_t timeout)
{
	uint32_t sum = 0;
	int ret = BK_OK;

	//TODO init check

	if (s_adc_buf.size == 0)
		return BK_ERR_ADC_NOT_INIT;

	ret = adc_block_read(timeout);
	if (BK_OK != ret)
		return ret;

	for (uint16_t i = 0; i < s_adc_buf.size; i++) {
		sum += s_adc_buf.buf[i];
	}

	sum = sum / s_adc_buf.size;

#if CONFIG_SARADC_RANGE_DIVIDE
	sum = sum >> 1;
#endif

	*data = sum;

	return ret;
}


static void adc_isr(void)
{
	adc_hal_clear_int_status(&s_adc.hal);

	ADC_STATIS_INC(s_adc_statis->adc_isr_cnt);

	adc_read_fifo();

	if (s_adc_buf.sample_cnt >= s_adc_buf.size) {
		rtos_set_semaphore(&(s_adc_dev.adc_read_sema));
		bk_adc_stop();
	}

	if (s_adc_read_isr.callback) {
		s_adc_read_isr.callback(s_adc_read_isr.param);
	}

    if (s_adc_read_isr.iot_callback) {
		s_adc_read_isr.iot_callback(s_adc_buf.buf, s_adc_read_isr.p_iot_context);
	}
}

bk_err_t bk_adc_is_valid_ch(uint32_t ch)
{
    if (!adc_hal_is_valid_channel(&s_adc.hal, ch)) 
    {
        ADC_LOGE("ADC id number(%d) is invalid\r\n", (ch));
        return BK_ERR_ADC_INVALID_CHAN;
    }

    return BK_OK;
}

bk_err_t bk_adc_register_isr_iot_callback(    void* iot_callback, void      * p_iot_context)
{
	ADC_RETURN_ON_NOT_INIT();

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_adc_read_isr.iot_callback = (IotAdcCallback_t)iot_callback;
	s_adc_read_isr.p_iot_context = p_iot_context;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_adc_unregister_isr_iot_callback(void)
{
	ADC_RETURN_ON_NOT_INIT();

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_adc_read_isr.iot_callback = NULL;
	s_adc_read_isr.p_iot_context = NULL;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_adc_en(void)
{
	if(adc_hal_check_adc_busy(&s_adc.hal))
    {   
        os_printf("adc_start:adc busy\n");
		return BK_ERR_ADC_BUSY;
    }

	adc_hal_start_commom(&s_adc.hal);

	int ret = rtos_get_semaphore(&(s_adc_dev.adc_read_sema), 1000);
	if(ret != kNoErr)
    {   
        os_printf("adc_start:rtos_get_semaphore fail\n");
		return BK_ERR_ADC_GET_READ_SEMA;
    }

	return BK_OK;
}

void saradc_config_param_init_for_temp(saradc_desc_t* adc_config)
{
    os_memset(adc_config, 0x00, sizeof(saradc_desc_t));
    adc_config->channel = 1;
    adc_config->current_read_data_cnt = 0;
    adc_config->current_sample_data_cnt = 0;
    adc_config->filter = 0;
    adc_config->has_data = 0;
    adc_config->all_done = 0;
    adc_config->mode = (ADC_CONFIG_MODE_CONTINUE << 0)
                      |(ADC_CONFIG_MODE_4CLK_DELAY << 2)
                      |(ADC_CONFIG_MODE_SHOULD_OFF);
    adc_config->pre_div = 0x10;
    adc_config->samp_rate = 0x20;
}

float saradc_calculate(UINT16 adc_val)
{
    float practic_voltage;
#if (CFG_SOC_NAME == SOC_BK7256)
    /* (adc_val - low) / (practic_voltage - 1Volt) = (high - low) / 1Volt */
    /* practic_voltage = (adc_val - low) / (high - low) + 1Volt */
    practic_voltage = (float)(adc_val - saradc_val.low);
    practic_voltage = (practic_voltage / (float)(saradc_val.high - saradc_val.low)) + 1;
#elif ( (CFG_SOC_NAME != SOC_BK7271) && (CFG_SOC_NAME != SOC_BK7221U))
    practic_voltage = ((adc_val - saradc_val.low) * 1.8);
    practic_voltage = (practic_voltage / (saradc_val.high - saradc_val.low)) + 0.2;
#else
	 practic_voltage = (adc_val -(saradc_val.low-4096));
	 practic_voltage = practic_voltage/(saradc_val.high  - (saradc_val.low-4096));
	 practic_voltage = 2*practic_voltage;
#endif
    return practic_voltage;
}

UINT32 saradc_set_calibrate_val(uint16_t *value, SARADC_MODE mode)
{
    uint32_t irq_level;
    irq_level = rtos_disable_int();
    if(SARADC_CALIBRATE_LOW == mode)
    {
        saradc_val.low = *value;
    }
    else if(SARADC_CALIBRATE_HIGH == mode)
    {
        saradc_val.high = *value;
    }
    else
    {
		rtos_enable_int(irq_level);
        return SARADC_FAILURE;
    }

    rtos_enable_int(irq_level);
    return SARADC_SUCCESS;
}

void bk_adc_read_for_ate(uint32_t saradc_num, uint16_t *saradc_buf)
{
    uint32_t i = 0;
    int irq_level=0;

    BK_LOG_ON_ERR(bk_saradc_start_int_disable());
    //saradc enable
    saradc_hal_start_enable();
    //need check saradc fifo empty
    //os_printf("saradc_hal_is_fifo_empty=%x\r\n",saradc_hal_is_fifo_empty());
    //os_printf("sardata_start\r\n");
    irq_level = rtos_disable_int();

    for(i = 0; i < saradc_num; i++)
    {
        if(saradc_hal_is_fifo_empty())
        {
            saradc_buf[i] = bk_saradc_read_raw_data(1000);
            //saradc_buf[i] = *((volatile unsigned long *) (BASEADDR_SADC+0x4*4));
        }
        else
        {
            i--;
            continue;
        }

    }
    rtos_enable_int(irq_level);
}

void test_adc_for_ate(adc_chan_t channel, adc_mode_t mode,
					  uint32_t pre_div, uint32_t samp_rate,
					  uint32_t filter, uint32_t sta_ctrl,
					  uint32_t usCount, uint16_t *pDataBuffer)
{
    adc_config_t config = {0};
    uint32_t adc_clk;

    adc_clk = 26000000/2/(pre_div + 1);
    BK_LOG_ON_ERR(bk_adc_acquire());
    BK_LOG_ON_ERR(bk_adc_init(channel));

    config.chan = channel;
    config.adc_mode = mode;
    config.clk = adc_clk;
    config.src_clk = 1;
    config.saturate_mode = 4;
    config.sample_rate = samp_rate;
    config.steady_ctrl= sta_ctrl;
    config.adc_filter = filter;

    BK_LOG_ON_ERR(bk_adc_set_config(&config));
    BK_LOG_ON_ERR(bk_adc_enable_bypass_clalibration());
    BK_LOG_ON_ERR(bk_adc_start());

    bk_adc_read_for_ate(usCount, pDataBuffer);
    BK_LOG_ON_ERR(bk_adc_stop());
    BK_LOG_ON_ERR(bk_adc_deinit(channel));
    BK_LOG_ON_ERR(bk_adc_release());
}