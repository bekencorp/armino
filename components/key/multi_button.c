#include <common/bk_include.h>
#include <common/bk_typedef.h>
#include "bk_arm_arch.h"
#include "bk_uart.h"
#include "multi_button.h"
#include <os/os.h>

#if CONFIG_BUTTON

#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((BUTTON_S*)handle)

//button handle list head.
static BUTTON_S *head_handle = NULL;
extern beken_mutex_t g_key_mutex;

/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @retval None
  */
void button_init(BUTTON_S *handle, uint8_t(*pin_level)(struct _button_ *), uint8_t active_level, void *user_data)
{
	memset(handle, 0, sizeof(BUTTON_S));

	handle->event = (uint8_t)NONE_PRESS;
	handle->active_level = active_level;
	handle->user_data = user_data;
	handle->hal_button_Level = pin_level;
	handle->button_level = handle->hal_button_Level(handle);
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void button_attach(BUTTON_S *handle, PRESS_EVT event, btn_callback cb)
{
	handle->cb[event] = cb;
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
PRESS_EVT button_get_event(BUTTON_S *handle)
{
	return (PRESS_EVT)(handle->event);
}

/**
  * @brief  BUTTON_S driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @retval None
  */
void button_handler(BUTTON_S *handle)
{
	uint8_t read_gpio_level = handle->hal_button_Level(handle);

	//ticks counter working..
	if ((handle->state) > 0) handle->ticks++;

	/*------------button debounce handle---------------*/
	if (read_gpio_level != handle->button_level) { //not equal to prev one
		//continue read 3 times same new level change
		if (++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	} else   //leved not change ,counter reset.
		handle->debounce_cnt = 0;

	/*-----------------State machine-------------------*/
	switch (handle->state) {
	case 0:
		if (handle->button_level == handle->active_level) {	//start press down
			handle->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			handle->ticks = 0;
			handle->repeat = 1;
			handle->state = 1;
		} else
			handle->event = (uint8_t)NONE_PRESS;
		break;

	case 1:
		if (handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			handle->ticks = 0;
			handle->state = 2;

		} else if (handle->ticks > LONG_TICKS) {
			handle->event = (uint8_t)LONG_PRESS_START;
			EVENT_CB(LONG_PRESS_START);
			handle->state = 5;
		}
		break;

	case 2:
		if (handle->button_level == handle->active_level) { //press down again
			handle->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			handle->repeat++;
			if (handle->repeat == 2) {
				EVENT_CB(DOUBLE_CLICK); // repeat hit
			}
			EVENT_CB(PRESS_REPEAT); // repeat hit
			handle->ticks = 0;
			handle->state = 3;
		} else if (handle->ticks > SHORT_TICKS) { //released timeout
			if (handle->repeat == 1) {
				handle->event = (uint8_t)SINGLE_CLICK;
				EVENT_CB(SINGLE_CLICK);
			} else if (handle->repeat == 2)
				handle->event = (uint8_t)DOUBLE_CLICK;
			handle->state = 0;
		}
		break;

	case 3:
		if (handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			if (handle->ticks < SHORT_TICKS) {
				handle->ticks = 0;
				handle->state = 2; //repeat press
			} else
				handle->state = 0;
		}
		break;

	case 5:
		if (handle->button_level == handle->active_level) {
			//continue hold trigger
			handle->event = (uint8_t)LONG_PRESS_HOLD;
			EVENT_CB(LONG_PRESS_HOLD);

		} else { //releasd
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			handle->state = 0; //reset
		}
		break;
	}
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int button_start(BUTTON_S *handle)
{
	BUTTON_S *target;
	rtos_lock_mutex(&g_key_mutex);
	target = head_handle;
	while (target) {
		if (target == handle) {
			rtos_unlock_mutex(&g_key_mutex);
			return -1;	//already exist.
		}
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	rtos_unlock_mutex(&g_key_mutex);
	return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void button_stop(BUTTON_S *handle)
{
	BUTTON_S **curr;

	rtos_lock_mutex(&g_key_mutex);
	for (curr = &head_handle; *curr;) {
		BUTTON_S *entry = *curr;
		if (entry == handle)
			*curr = entry->next;
		else
			curr = &entry->next;
	}
	rtos_unlock_mutex(&g_key_mutex);
}

BUTTON_S *button_find_with_user_data(void *user_data)
{
	BUTTON_S *entry = NULL;

	rtos_lock_mutex(&g_key_mutex);
	for (entry = head_handle; entry; entry = entry->next) {
		if (entry->user_data == user_data)
			break;
	}
	rtos_unlock_mutex(&g_key_mutex);

	return entry;
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
void button_ticks(void *param)
{
	BUTTON_S *target;
	rtos_lock_mutex(&g_key_mutex);
	for (target = head_handle; target; target = target->next)
		button_handler(target);
	rtos_unlock_mutex(&g_key_mutex);
}
#endif
