#include <common/bk_include.h>
#include <common/bk_typedef.h>
#include "bk_arm_arch.h"
#include "bk_gpio.h"
#include "key_main.h"
#include "multi_button.h"
#include <os/os.h>
#include <os/mem.h>
#include <common/bk_kernel_err.h>
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"


#if CONFIG_BUTTON
//#include <drivers/pin.h>

beken_timer_t g_key_timer;
beken_mutex_t g_key_mutex;
static bool s_key_init_status_flag = 0;

void key_configure(void)
{
	bk_err_t result;

	result = rtos_init_mutex(&g_key_mutex);
	if(kNoErr != result)
	{
		KEY_PRT("rtos_init_mutex fail\r\n");
		return;
	}

	result = rtos_init_timer(&g_key_timer,
							 KEY_TMR_DURATION,
							 button_ticks,
							 (void *)0);
	if(kNoErr != result)
	{
		KEY_PRT("rtos_init_timer fail\r\n");
		return;
	}

	result = rtos_start_timer(&g_key_timer);
	if(kNoErr != result)
	{
		KEY_PRT("rtos_start_timer fail\r\n");
		return;
	}
}

void key_unconfig(void)
{
	bk_err_t ret;

	if (rtos_is_timer_init(&g_key_timer)) {
		if (rtos_is_timer_running(&g_key_timer)) {
			ret = rtos_stop_timer(&g_key_timer);
			if(kNoErr != ret)
			{
				KEY_PRT("rtos_stop_timer fail\r\n");
				return;
			}
		}

		ret = rtos_deinit_timer(&g_key_timer);
		if(kNoErr != ret)
		{
			KEY_PRT("rtos_deinit_timer fail\r\n");
			return;
		}

	}

	ret = rtos_deinit_mutex(&g_key_mutex);
	if(kNoErr != ret)
	{
		KEY_PRT("rtos_deinit_mutex fail\r\n");
		return;
	}

}
static void key_gpio_config(uint32_t gpio_id, uint8_t active_level)
{
	gpio_dev_unmap(gpio_id);
	BK_LOG_ON_ERR(bk_gpio_disable_output(gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_input(gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_pull(gpio_id));
	if(active_level)
		BK_LOG_ON_ERR(bk_gpio_pull_down(gpio_id));
	else
		BK_LOG_ON_ERR(bk_gpio_pull_up(gpio_id));
}

#if USING_DEMO_BOARD
uint8_t matrix_key_get_value(BUTTON_S *handle)
{
	KEYITEM key;
	static uint8_t combo_state = 0, unactive_level, level;
	uint8_t combo_level_1, combo_level_2;

	key = (KEYITEM)handle->user_data;
	unactive_level = handle->active_level ? 0 : 1;

	if ((1 == combo_state) && (key != KEY_COMBO_S1S2_CONFIG_NET))
		level = unactive_level;
	else {
		switch (key) {
		case KEY_COMBO_S1S2_CONFIG_NET:
			rt_pin_mode(KEY_S1_GPIO6, PIN_MODE_INPUT_PULLUP);
			rt_pin_mode(KEY_S2_GPIO7, PIN_MODE_INPUT_PULLUP);
			rt_pin_mode(KEY_S3_GPIO8, PIN_MODE_INPUT_PULLUP);

			combo_level_1 = rt_pin_read(KEY_S1_GPIO6);
			combo_level_2 = rt_pin_read(KEY_S2_GPIO7);

			if (1 == combo_state) { //combo pressed
				if ((combo_level_1 == unactive_level) && (combo_level_2 == unactive_level)) {
					level = unactive_level;//combo release
					combo_state = 0;
				} else
					level = handle->active_level;
			} else { // not press
				if ((combo_level_1 == handle->active_level) && (combo_level_2 == handle->active_level)) {
					level = handle->active_level;//comobo press
					combo_state = 1;
				} else
					level = unactive_level;
			}
			break;

		case KEY_S1_PREV_PLAY:
			rt_pin_mode(KEY_S1_GPIO6, PIN_MODE_INPUT_PULLUP);
			level = rt_pin_read(KEY_S1_GPIO6);
			break;

		case KEY_S2_NEXT:
			rt_pin_mode(KEY_S2_GPIO7, PIN_MODE_INPUT_PULLUP);
			level = rt_pin_read(KEY_S2_GPIO7);
			break;

		case KEY_S3_COLLECT:
			rt_pin_mode(KEY_S3_GPIO8, PIN_MODE_INPUT_PULLUP);
			level = rt_pin_read(KEY_S3_GPIO8);
			break;
		case KEY_S4_WECHAT:
			rt_pin_mode(KEY_S2_GPIO7, PIN_MODE_INPUT_PULLDOWN);
			rt_pin_mode(KEY_S3_GPIO8, PIN_MODE_INPUT_PULLDOWN);
			rt_pin_mode(KEY_S1_GPIO6, PIN_MODE_OUTPUT);
			rt_pin_write(KEY_S1_GPIO6, 1);
			level = rt_pin_read(KEY_S2_GPIO7);
			break;
		case KEY_S5_VOICE:
			rt_pin_mode(KEY_S2_GPIO7, PIN_MODE_INPUT_PULLDOWN);
			rt_pin_mode(KEY_S1_GPIO6, PIN_MODE_INPUT_PULLDOWN);
			rt_pin_mode(KEY_S3_GPIO8, PIN_MODE_OUTPUT);
			rt_pin_write(KEY_S3_GPIO8, 1);
			level = rt_pin_read(KEY_S2_GPIO7);
			break;
		default:
			break;
		}
	}
	return level;
}

int matrix_key_item_configure(KEYITEM key, void short_press(void *), void double_press(void *), void long_press(void *), void hold_press(void *))
{
	BUTTON_S *handle;
	int result;
	uint8_t active_level;

	handle = os_malloc(sizeof(BUTTON_S));
	if (NULL == handle)
		return kNoMemoryErr;
	rtos_lock_mutex(&g_key_mutex);

	switch (key) {
	case KEY_COMBO_S1S2_CONFIG_NET:
		active_level = 0;
		break;
	case KEY_S1_PREV_PLAY:
		active_level = 0;
		break;
	case KEY_S2_NEXT:
		active_level = 0;
		break;
	case KEY_S3_COLLECT:
		active_level = 0;
		break;
	case KEY_S4_WECHAT:
		active_level = 1;
		break;
	case KEY_S5_VOICE:
		active_level = 1;
		break;
	default:
		break;
	}
	button_init(handle, matrix_key_get_value, active_level, (void *)key);
	button_attach(handle, SINGLE_CLICK, (btn_callback)short_press);
	button_attach(handle, DOUBLE_CLICK, (btn_callback)double_press);
	button_attach(handle, LONG_PRESS_START,	(btn_callback)long_press);
	button_attach(handle, LONG_PRESS_HOLD, (btn_callback)hold_press);
	rtos_unlock_mutex(&g_key_mutex);
	result = button_start(handle);
	if (result < 0) {
		KEY_PRT("button_start failed\n");
		os_free(handle);
		return kGeneralErr;
	}

	return kNoErr;
}

#else
uint8_t key_get_gpio_value(BUTTON_S *handle)
{
	return bk_gpio_get_input((uint32_t)handle->user_data);//gpio_input((uint32_t)handle->user_data);
}

int key_item_configure(uint32_t gpio, uint8_t active_level, void short_press(void *), void double_press(void *), void long_press(void *), void hold_press(void *))
{
	BUTTON_S *handle;
	int result;

	handle = os_malloc(sizeof(BUTTON_S));
	if (NULL == handle)
		return kNoMemoryErr;
	rtos_lock_mutex(&g_key_mutex);

	key_gpio_config(gpio, active_level);
	button_init(handle, key_get_gpio_value, active_level, (void *)gpio);
	button_attach(handle, SINGLE_CLICK, (btn_callback)short_press);
	button_attach(handle, DOUBLE_CLICK, (btn_callback)double_press);
	button_attach(handle, LONG_PRESS_START,	(btn_callback)long_press);
	button_attach(handle, LONG_PRESS_HOLD, (btn_callback)hold_press);

	rtos_unlock_mutex(&g_key_mutex);
	result = button_start(handle);
	if (result < 0) {
		KEY_PRT("button_start failed\n");
		os_free(handle);
		return kGeneralErr;
	}
	os_printf("key_item_configure\r\n");

	return kNoErr;
}

#endif

int key_item_unconfigure(uint32_t gpio)
{
	BUTTON_S *handle = NULL;
	while (1) {
		handle = button_find_with_user_data((void *)gpio);
		if (NULL == handle)
			break;
		button_stop(handle);
		os_free(handle);
	}

	return kNoErr;
}

void key_initialization(void)
{
	if(s_key_init_status_flag) {
		KEY_PRT("INITED\n");
		return;
	}

	key_configure();
	s_key_init_status_flag = 1;
}

void key_uninitialization(void)
{
	if(!s_key_init_status_flag) {
		KEY_PRT("UNINITED\n");
		return;
	}

	key_unconfig();
	s_key_init_status_flag = 0;
}

// eof
#endif
