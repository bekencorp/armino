#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"
#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"
#include <os/os.h>
#include "adc_key_main.h"
#include "modules/pm.h"


#if CONFIG_ADC_KEY

static adc_key_config_t s_adc_key_config = {NULL};
static adc_key_callback_t s_adc_key_cb = {NULL};

void adc_key_register_next_cb(adc_key_isr_t isr)
{
	s_adc_key_cb.next_cb = isr;
}

void adc_key_register_prev_cb(adc_key_isr_t isr)
{
	s_adc_key_cb.prev_cb = isr;
}

void adc_key_register_play_pause_cb(adc_key_isr_t isr)
{
	s_adc_key_cb.play_pause_cb = isr;
}

void adc_key_register_menu_cb(adc_key_isr_t isr)
{
	s_adc_key_cb.menu_cb = isr;
}

static void adc_key_config(void)
{
	adc_config_t config = {0};
	config.chan = 4;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.src_clk = ADC_SCLK_XTAL_26M;
	config.clk = 203125;
	config.saturate_mode = 3;
	config.steady_ctrl= 7;
	config.adc_filter = 0;
	config.sample_rate = 0;
	BK_LOG_ON_ERR(bk_adc_set_config(&config));

	s_adc_key_config.adc_key_semaphore = NULL;
	s_adc_key_config.anti_shake_count = 0;

}

static uint32_t adc_key_get_gpio_voltage(void)
{
	uint16_t value = 0;
	float cali_value = 0;

	if(bk_adc_set_channel(ADC_KEY_SADC_CHAN_ID))
		return 9999;

	bk_adc_read(&value, 100);
	cali_value = saradc_calculate(value);
	value = cali_value * 1000;
	ADC_KEY_LOGD("adc_key_get_gpio_voltage value:%d mv\n", value);

	return value;

}
static void adc_key_check_volate_to_callback(uint16_t value)
{
	if(value > 3300)
		return;
	ADC_KEY_LOGD("adc_key_check_volate_to_callback value:%d mv\n", value);

	if(value >= 0 && value < 100) {
		s_adc_key_config.anti_shake_count++;
		if(s_adc_key_config.anti_shake_count > ADC_KEY_VALID_VALUE_STATISTICS) {
			if (s_adc_key_cb.next_cb)
				s_adc_key_cb.next_cb();
			else
				ADC_KEY_LOGI("Please register next callback\r\n");

			s_adc_key_config.anti_shake_count = 0;
		}
		ADC_KEY_LOGD("adc value:%d mv\n", value);
	}
	
	if(value >= 300 && value < 400) {
		s_adc_key_config.anti_shake_count++;
		if(s_adc_key_config.anti_shake_count > ADC_KEY_VALID_VALUE_STATISTICS) {
			if (s_adc_key_cb.prev_cb)
				s_adc_key_cb.prev_cb();
			else
				ADC_KEY_LOGI("Please register prev callback\r\n");

			s_adc_key_config.anti_shake_count = 0;
		}
		ADC_KEY_LOGD("adc value:%d mv\n", value);
	}
	
	if(value >= 700 && value < 800) {
		s_adc_key_config.anti_shake_count++;
		if(s_adc_key_config.anti_shake_count > ADC_KEY_VALID_VALUE_STATISTICS) {
			if (s_adc_key_cb.play_pause_cb)
				s_adc_key_cb.play_pause_cb();
			else
				ADC_KEY_LOGI("Please register play pause callback\r\n");

			s_adc_key_config.anti_shake_count = 0;

		}
		ADC_KEY_LOGD("adc value:%d mv\n", value);
	}
	
	if(value >= 1000 && value < 1100) {
		s_adc_key_config.anti_shake_count++;
		if(s_adc_key_config.anti_shake_count > ADC_KEY_VALID_VALUE_STATISTICS) {
			if (s_adc_key_cb.menu_cb)
				s_adc_key_cb.menu_cb();
			else
				ADC_KEY_LOGI("Please register menu callback\r\n");

			s_adc_key_config.anti_shake_count = 0;
		}
		ADC_KEY_LOGD("adc value:%d mv\n", value);
	}

}

static void adc_key_check_witch_key(void)
{
	uint16_t value = 0;
	BK_LOG_ON_ERR(gpio_dev_unmap(ADC_KEY_GPIO_ID));
	BK_LOG_ON_ERR(bk_gpio_pull_down(ADC_KEY_GPIO_ID));
	BK_LOG_ON_ERR(gpio_dev_map(ADC_KEY_GPIO_ID, GPIO_DEV_ADC4));

	value = adc_key_get_gpio_voltage();
	adc_key_check_volate_to_callback(value);
}
static void adc_key_release_adc_function()
{
	BK_LOG_ON_ERR(gpio_dev_unmap(ADC_KEY_GPIO_ID));
	BK_LOG_ON_ERR(bk_gpio_enable_input(ADC_KEY_GPIO_ID));
	BK_LOG_ON_ERR(bk_gpio_pull_up(ADC_KEY_GPIO_ID));
	rtos_delay_milliseconds(ADC_KEY_GET_VALUE_WAIT_MS);
}

static void adc_key_running_task(uint32_t data)
{
	int i = 0;
	while (1) {
		adc_key_release_adc_function();
		for(i = 0; i < ADC_KEY_ADC_COLLECT_NUMBER; i++){
			adc_key_check_witch_key();
		}
		s_adc_key_config.anti_shake_count = 0;
	}

}

gpio_int_type_t s_adc_key_gpio_int_type = 0;

void adc_key_set_gpio_int_type(int adc_key_gpio_int_type)
{
	s_adc_key_gpio_int_type = adc_key_gpio_int_type;
	ADC_KEY_LOGI("s_adc_key_gpio_int_type: %d\n",s_adc_key_gpio_int_type);
}

void adc_key_init(void)
{
	BK_LOG_ON_ERR(gpio_dev_unmap(ADC_KEY_GPIO_ID));
	BK_LOG_ON_ERR(bk_adc_init(ADC_KEY_SADC_CHAN_ID));
	adc_key_config();
	BK_LOG_ON_ERR(bk_adc_set_channel(ADC_KEY_SADC_CHAN_ID));
	BK_LOG_ON_ERR(bk_adc_start());

	if(!s_adc_key_config.adc_key_handle){
		ADC_KEY_LOGI("adc_key_init creat thread\n");
		if(rtos_create_thread(&s_adc_key_config.adc_key_handle, 5, "adc_key",
					(beken_thread_function_t) adc_key_running_task, 2048, 0)) {
			ADC_KEY_LOGD("adc_key_initialization rtos_create_thread FAILED!\n");
			return;
		}
	} else
		ADC_KEY_LOGI("adc_key_init PLEASE deinit\n");
}
void adc_key_deinit(void)
{
	if(s_adc_key_config.adc_key_handle) {
		rtos_delete_thread(&s_adc_key_config.adc_key_handle);
		s_adc_key_config.adc_key_handle = NULL;
		s_adc_key_cb.menu_cb = NULL;
		s_adc_key_cb.next_cb = NULL;
		s_adc_key_cb.prev_cb = NULL;
		s_adc_key_cb.play_pause_cb = NULL;
	} else
		ADC_KEY_LOGI("adc_key_deinit PLEASE init\n");

}

#endif
