#include <os/os.h>
#include <os/mem.h>
#include "adc_key_main.h"
#include "modules/pm.h"


#if CONFIG_ADC_KEY

#define ADCKEY_EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((ADCKEY_S*)handle)

beken_timer_t g_adckey_timer;
beken_mutex_t g_adckey_mutex;
static ADCKEY_S *adckey_head_handle = NULL;
static adc_chan_t s_adc_chan = ADC_MAX;
static bool s_adckey_inited_flag = 0;

static void adckey_adc_config(adc_chan_t chan)
{
	adc_config_t config = {0};
	os_memset(&config, 0, sizeof(adc_config_t));

	config.chan = chan;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.src_clk = ADC_SCLK_XTAL_26M;
	config.clk = 3203125;
	config.saturate_mode = ADC_SATURATE_MODE_3;
	config.steady_ctrl= 7;
	config.adc_filter = 0;

	BK_LOG_ON_ERR(bk_adc_init(chan));
	BK_LOG_ON_ERR(bk_adc_set_config(&config));
	BK_LOG_ON_ERR(bk_adc_enable_bypass_clalibration());
}

static uint32_t adc_key_get_gpio_voltage(adc_chan_t chan)
{
	uint32_t value = 0;
	float cali_value = 0;

	BK_LOG_ON_ERR(bk_adc_acquire());
	BK_LOG_ON_ERR(bk_adc_start());
	if(bk_adc_set_channel(chan)) {
		BK_LOG_ON_ERR(bk_adc_release());
		return 9999;
	}
	bk_adc_read((uint16_t *)&value, 100);
	cali_value = ((float)value/4096*2)*1.2;
	value = cali_value * 1000;

	BK_LOG_ON_ERR(bk_adc_stop());
	BK_LOG_ON_ERR(bk_adc_release());
	ADC_KEY_LOGD("adc_key_get_gpio_voltage value=%dmv\r\n", value);

	return value;
}
/*
if(value >= 1 && value < 100) ADC_KEY_LOGI("Please register next callback\r\n");
if(value >= 600 && value < 750) ADC_KEY_LOGI("Please register prev callback\r\n");
if(value >= 1300 && value < 1500) ADC_KEY_LOGI("Please register play pause callback\r\n");
if(value >= 1900 && value < 2100) ADC_KEY_LOGI("Please register menu callback\r\n");
}
*/
void adckey_button_handler(ADCKEY_S *handle)
{
	uint32_t read_level = adc_key_get_gpio_voltage(s_adc_chan);
	uint32_t lowest = handle->lowest_active_level;
	uint32_t highest = handle->highest_active_level;

	//ticks counter working..
	if ((handle->state) > 0) handle->ticks++;

	/*------------button debounce handle---------------*/
	if (read_level != handle->adc_read_level) { //not equal to prev one
		//continue read 3 times same new level change
		if (++(handle->debounce_cnt) >= ADCKEY_DEBOUNCE_TICKS) {
			handle->adc_read_level = read_level;
			handle->debounce_cnt = 0;
		}
	} else   //leved not change ,counter reset.
		handle->debounce_cnt = 0;

	/*-----------------State machine-------------------*/
	switch (handle->state) {
	case 0:
		if ((handle->adc_read_level >= lowest) && (highest >= handle->adc_read_level)) {	//start press down
			handle->event = (uint8_t)ADCKEY_PRESS_DOWN;
			ADCKEY_EVENT_CB(ADCKEY_PRESS_DOWN);
			handle->ticks = 0;
			handle->repeat = 1;
			handle->state = 1;
		} else
			handle->event = (uint8_t)ADCKEY_NONE_PRESS;
		break;

	case 1:
		if (!((handle->adc_read_level >= lowest) && (highest >= handle->adc_read_level))) { //released press up
			handle->event = (uint8_t)ADCKEY_PRESS_UP;
			ADCKEY_EVENT_CB(ADCKEY_PRESS_UP);
			handle->ticks = 0;
			handle->state = 2;

		} else if (handle->ticks > ADCKEY_LONG_TICKS) {
			handle->event = (uint8_t)ADCKEY_LONG_PRESS_START;
			ADCKEY_EVENT_CB(ADCKEY_LONG_PRESS_START);
			handle->state = 5;
		}
		break;

	case 2:
		if ((handle->adc_read_level >= lowest) && (highest >= handle->adc_read_level)) { //press down again
			handle->event = (uint8_t)ADCKEY_PRESS_DOWN;
			ADCKEY_EVENT_CB(ADCKEY_PRESS_DOWN);
			handle->repeat++;
			if (handle->repeat == 2) {
				ADCKEY_EVENT_CB(ADCKEY_DOUBLE_CLICK); // repeat hit
			}
			ADCKEY_EVENT_CB(ADCKEY_PRESS_REPEAT); // repeat hit
			handle->ticks = 0;
			handle->state = 3;
		} else if (handle->ticks > ADCKEY_SHORT_TICKS) { //released timeout
			if (handle->repeat == 1) {
				handle->event = (uint8_t)ADCKEY_SINGLE_CLICK;
				ADCKEY_EVENT_CB(ADCKEY_SINGLE_CLICK);
			} else if (handle->repeat == 2)
				handle->event = (uint8_t)ADCKEY_DOUBLE_CLICK;
			handle->state = 0;
		}
		break;

	case 3:
		if (!((handle->adc_read_level >= lowest) && (highest >= handle->adc_read_level))) { //released press up
			handle->event = (uint8_t)ADCKEY_PRESS_UP;
			ADCKEY_EVENT_CB(ADCKEY_PRESS_UP);
			if (handle->ticks < ADCKEY_SHORT_TICKS) {
				handle->ticks = 0;
				handle->state = 2; //repeat press
			} else
				handle->state = 0;
		}
		break;

	case 5:
		if ((handle->adc_read_level >= lowest) && (highest >= handle->adc_read_level)) {
			//continue hold trigger
			handle->event = (uint8_t)ADCKEY_LONG_PRESS_HOLD;
			ADCKEY_EVENT_CB(ADCKEY_LONG_PRESS_HOLD);

		} else { //releasd
			handle->event = (uint8_t)ADCKEY_PRESS_UP;
			ADCKEY_EVENT_CB(ADCKEY_PRESS_UP);
			handle->state = 0; //reset
		}
		break;
	}
}

static void adc_key_ticks(void *param)
{
	ADCKEY_S *target;
	rtos_lock_mutex(&g_adckey_mutex);
	for (target = adckey_head_handle; target; target = target->next)
		adckey_button_handler(target);
	rtos_unlock_mutex(&g_adckey_mutex);
}

static void adc_key_configure()
{
	bk_err_t result;

	result = rtos_init_mutex(&g_adckey_mutex);
	if(kNoErr != result)
	{
		ADC_KEY_LOGI("rtos_init_mutex fail\r\n");
		return;
	}

	result = rtos_init_timer(&g_adckey_timer,
							 ADCKEY_TMR_DURATION,
							 adc_key_ticks,
							 (void *)0);
	if(kNoErr != result)
	{
		ADC_KEY_LOGI("rtos_init_timer fail\r\n");
		return;
	}

	result = rtos_start_timer(&g_adckey_timer);
	if(kNoErr != result)
	{
		ADC_KEY_LOGI("rtos_start_timer fail\r\n");
		return;
	}

}

void bk_adc_key_init(gpio_id_t gpio_id, adc_chan_t adc_chan)
{
	if(s_adckey_inited_flag)
		return;

	BK_LOG_ON_ERR(gpio_dev_unmap(gpio_id));
	s_adc_chan = adc_chan;
	adckey_adc_config(adc_chan);
	adc_key_configure();

	s_adckey_inited_flag = 1;
}

static void adckey_unconfig(void)
{
	bk_err_t ret;

	ret = rtos_deinit_mutex(&g_adckey_mutex);
	if(kNoErr != ret)
	{
		ADC_KEY_LOGI("rtos_deinit_mutex fail\r\n");
		return;
	}

	if (rtos_is_timer_init(&g_adckey_timer)) {
		if (rtos_is_timer_running(&g_adckey_timer)) {
			ret = rtos_stop_timer(&g_adckey_timer);
			if(kNoErr != ret)
			{
				ADC_KEY_LOGI("rtos_stop_timer fail\r\n");
				return;
			}
		}

		ret = rtos_deinit_timer(&g_adckey_timer);
		if(kNoErr != ret)
		{
			ADC_KEY_LOGI("rtos_deinit_timer fail\r\n");
			return;
		}
	}
}

void bk_adc_key_deinit(void)
{
	if(s_adckey_inited_flag)
		s_adckey_inited_flag = 0;
	else
		return;

	bk_adc_deinit(s_adc_chan);
	adckey_unconfig();
}

void adckey_button_init(ADCKEY_S *handle, adckey_configure_t *config)
{
	handle->event = (uint8_t)ADCKEY_NONE_PRESS;
	handle->lowest_active_level = config->lowest_level;
	handle->highest_active_level = config->highest_level;
	handle->user_data = (void *)(config->user_index);
}
void adckey_button_attach(ADCKEY_S *handle, ADCKEY_PRESS_EVT event, adc_key_callback cb)
{
	handle->cb[event] = cb;
}
int adckey_button_start(ADCKEY_S *handle)
{
	ADCKEY_S *target;
	rtos_lock_mutex(&g_adckey_mutex);
	target = adckey_head_handle;
	while (target) {
		if (target == handle) {
			rtos_unlock_mutex(&g_adckey_mutex);
			return -1;	//already exist.
		}
		target = target->next;
	}
	handle->next = adckey_head_handle;
	adckey_head_handle = handle;
	rtos_unlock_mutex(&g_adckey_mutex);
	return 0;
}

uint32_t bk_adckey_item_configure(adckey_configure_t *config)
{
	if(!s_adckey_inited_flag)
		return kGeneralErr;

	ADCKEY_S *handle;
	int result = 0;

	handle = os_malloc(sizeof(ADCKEY_S));
	if (NULL == handle)
		return kNoMemoryErr;
	os_memset(handle, 0, sizeof(ADCKEY_S));
	rtos_lock_mutex(&g_adckey_mutex);

	adckey_button_init(handle, config);
	adckey_button_attach(handle, ADCKEY_SINGLE_CLICK, (adc_key_callback)config->short_press_cb);
	adckey_button_attach(handle, ADCKEY_DOUBLE_CLICK, (adc_key_callback)config->double_press_cb);
	adckey_button_attach(handle, ADCKEY_LONG_PRESS_START,	(adc_key_callback)config->long_press_cb);
	adckey_button_attach(handle, ADCKEY_LONG_PRESS_HOLD, (adc_key_callback)config->hold_press_cb);

	rtos_unlock_mutex(&g_adckey_mutex);
	result = adckey_button_start(handle);
	if (result < 0) {
		ADC_KEY_LOGI("button_start failed\n");
		os_free(handle);
		return kGeneralErr;
	}

	return kNoErr;
}

ADCKEY_S *adckey_button_find_with_user_data(void *user_data)
{
	ADCKEY_S *entry = NULL;

	rtos_lock_mutex(&g_adckey_mutex);
	for (entry = adckey_head_handle; entry; entry = entry->next) {
		if (entry->user_data == user_data)
			break;
	}
	rtos_unlock_mutex(&g_adckey_mutex);

	return entry;
}

void adckey_button_stop(ADCKEY_S *handle)
{
	ADCKEY_S **curr;

	rtos_lock_mutex(&g_adckey_mutex);
	for (curr = &adckey_head_handle; *curr;) {
		ADCKEY_S *entry = *curr;
		if (entry == handle)
			*curr = entry->next;
		else
			curr = &entry->next;
	}
	rtos_unlock_mutex(&g_adckey_mutex);
}

uint32_t bk_adckey_item_unconfigure(ADCKEY_INDEX user_data)
{
	if(!s_adckey_inited_flag)
		return kGeneralErr;

	ADCKEY_S *handle = NULL;
	while (1) {
		handle = adckey_button_find_with_user_data((void *)user_data);
		if (NULL == handle)
			break;
		adckey_button_stop(handle);
		os_free(handle);
	}

	return kNoErr;
}

#endif
