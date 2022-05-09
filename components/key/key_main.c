#include <common/bk_include.h>
#include <common/bk_typedef.h>
#include "bk_arm_arch.h"
#include "bk_gpio.h"
#include "key_main.h"
#include "multi_button.h"
#include <os/os.h>
#include <os/mem.h>
#include <common/bk_kernel_err.h>

#if CONFIG_BUTTON
#include <drivers/pin.h>

beken_timer_t g_key_timer;
beken_mutex_t g_key_mutex;

void key_configure(void)
{
	bk_err_t result;

	result = rtos_init_mutex(&g_key_mutex);
	BK_ASSERT(kNoErr == result);

	result = rtos_init_timer(&g_key_timer,
							 KEY_TMR_DURATION,
							 button_ticks,
							 (void *)0);
	BK_ASSERT(kNoErr == result);

	result = rtos_start_timer(&g_key_timer);
	BK_ASSERT(kNoErr == result);
}

void key_unconfig(void)
{
	bk_err_t ret;

	ret = rtos_deinit_mutex(&g_key_mutex);
	BK_ASSERT(kNoErr == ret);

	if (rtos_is_timer_init(&g_key_timer)) {
		if (rtos_is_timer_running(&g_key_timer)) {
			ret = rtos_stop_timer(&g_key_timer);
			BK_ASSERT(kNoErr == ret);
		}

		ret = rtos_deinit_timer(&g_key_timer);
		BK_ASSERT(kNoErr == ret);
	}
}


#if (USING_DEMO_BOARD != 1)
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
	return gpio_input((uint32_t)handle->user_data);
}

int key_item_configure(uint32_t gpio, void short_press(void *), void double_press(void *), void long_press(void *), void hold_press(void *))
{
	BUTTON_S *handle;
	int result;

	handle = os_malloc(sizeof(BUTTON_S));
	if (NULL == handle)
		return kNoMemoryErr;
	rtos_lock_mutex(&g_key_mutex);
	gpio_config(gpio, GMODE_INPUT_PULLUP);
	button_init(handle, key_get_gpio_value, 0, (void *)gpio);

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
	key_configure();
}

void key_uninitialization(void)
{
	key_unconfig();
}

// eof
#endif
