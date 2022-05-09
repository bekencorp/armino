#ifndef _KEY_MAIN_H_
#define _KEY_MAIN_H_

#include "bk_gpio.h"
#include "bk_uart.h"

#define KEY_DEBUG

#ifdef KEY_DEBUG
#define KEY_PRT                 os_printf
#define KEY_WPRT                warning_prf
#else
#define KEY_PRT                 os_null_printf
#define KEY_WPRT                os_null_printf
#endif

#define GPIO_TEST_ID           GPIO23
#define KEY_TMR_DURATION       5

typedef enum KEY_ITEM_T {
	KEY_COMBO_S1S2_CONFIG_NET = 0,
	KEY_S1_PREV_PLAY,
	KEY_S2_NEXT,
	KEY_S3_COLLECT,
	KEY_S4_WECHAT,
	KEY_S5_VOICE,
} KEYITEM;

#define KEY_S1_GPIO6 6
#define KEY_S2_GPIO7 7
#define KEY_S3_GPIO8 8

int key_item_configure(uint32_t gpio, void short_press(void *), void double_press(void *), void long_press(void *), void hold_press(void *));
int key_item_unconfigure(uint32_t gpio);

#endif // 
