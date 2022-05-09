#ifndef _PM_H_
#define _PM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <driver/int.h>

#define PARAM_DATA_VALID  (0xFFFF)


typedef enum
{
	GPIO_TRIGGER_INTERRUPE_LEVEL_LOW_ACTIVE = 0,
	GPIO_TRIGGER_INTERRUPE_LEVEL_HIGH_ACTIVE,
	GPIO_TRIGGER_INTERRUPE_EDGE_RISING,
	GPIO_TRIGGER_INTERRUPE_EDGE_FALLING,
}gpio_trigger_interrupt_type_e;

typedef struct
{
	uint32_t gpio_id;
	gpio_trigger_interrupt_type_e gpio_trigger_interrupt_type;
	int_group_isr_t isr_callback;
	uint32_t gpio_valid;  //PARAM_DATA_VALID
}gpio_wakeup_param_t;
typedef struct
{
	uint32_t  period;
	int_group_isr_t isr_callback;
	uint32_t rtc_valid;   //PARAM_DATA_VALID
}rtc_wakeup_param_t;
typedef enum
{
	WIFI_WAKEUP = 1,
	BT_WAKEUP,
}wifi_bt_wakeup_type_e;
typedef struct
{
	wifi_bt_wakeup_type_e  wifi_bt_wakeup;
	uint32_t  sleep_time;
	int_group_isr_t isr_callback;
}system_wakeup_param_t;
typedef struct
{
    int_group_isr_t isr_callback;
}usbplug_wakeup_param_t;
typedef struct
{
	uint32_t  touch_channel;
    int_group_isr_t isr_callback;
}touch_wakeup_param_t;
typedef enum
{
	PM_MODE_NORMAL_SLEEP = 0,
	PM_MODE_LOW_VOLTAGE ,
	PM_MODE_DEEP_SLEEP ,
	PM_MODE_DEFAULT
}pm_sleep_mode_e;

typedef enum
{
	PM_WAKEUP_SOURCE_INT_GPIO = 0,
	PM_WAKEUP_SOURCE_INT_RTC ,
	PM_WAKEUP_SOURCE_INT_SYSTEM_WAKE ,
	PM_WAKEUP_SOURCE_INT_USBPLUG ,
	PM_WAKEUP_SOURCE_INT_TOUCHED ,
	PM_WAKEUP_SOURCE_INT_NONE ,
}pm_wakeup_source_e;
typedef enum
{
	PM_POWER_MODULE_NAME_MEM1 = 0,
	PM_POWER_MODULE_NAME_MEM2,
	PM_POWER_MODULE_NAME_MEM3,
	PM_POWER_MODULE_NAME_ENCP,
	PM_POWER_MODULE_NAME_BAKP,
	PM_POWER_MODULE_NAME_AHBP,
	PM_POWER_MODULE_NAME_AUDP,
	PM_POWER_MODULE_NAME_VIDP,
	PM_POWER_MODULE_NAME_BTSP,      //8
	PM_POWER_MODULE_NAME_WIFIP_MAC, //9
	PM_POWER_MODULE_NAME_WIFI_PHY,
	PM_POWER_MODULE_NAME_CPU1 ,     //11
	PM_POWER_MODULE_NAME_APP ,      //12
	PM_POWER_MODULE_NAME_NONE
}pm_power_module_name_e;

typedef enum
{
	PM_POWER_MODULE_STATE_ON = 0,
    PM_POWER_MODULE_STATE_OFF,
	PM_POWER_MODULE_STATE_NONE
}pm_power_module_state_e;
typedef enum
{
	PM_MODULE_NAME_WIFI = 0,
	PM_MODULE_NAME_BT,
	PM_MODULE_NAME_NONE
}pm_module_name_e;

/**
 * @brief mcu pm ctrl
 *
 * enabel and disable the mcu power manage
 *
 * @attention
 * - This API is used to enabel and disable the mcu power manage
 *
 * @param
 * -power_state:0x0:enable the mcu power manage;0x1:disable the mcu power manage
 * @return
 *    - BK_OK: succeed
 *
 */
int pm_mcu_pm_ctrl(uint32_t power_state);

/**
 * @brief get the mcu power feature state
 *
 * get the mcu power feature state
 *
 * @attention
 *-This API is used to get the mcu power manage feature state(open or close)
 *
 * @param
 * none
 * @return
 *-mcu power manage state(0x0:enable the mcu power manage;0x1:disable the mcu power manage)
 *
 */
uint32_t pm_mcu_pm_state_get();

/**
 * @brief set sleep mode
 *
 * set sleep mode.
 *
 * @attention
 * - This API set sleep mode
 *
 * @param sleep mode
 *
 * @return
 * - BK_OK: succeed
 *
 */
int pm_sleep_mode_set(pm_sleep_mode_e sleep_mode);

/**
 * @brief set wakeup source
 *
 * set wakeup source(eg.rtc,gpio)
 *
 * @attention
 * - This API set wakeup source,wifi and bt themselves not need set wakup source when in volatage
 *
 * @param
 * -wakeup_source:wake up source
 * -source_param: the wakeup source parameter
 * @return
 * - BK_OK: succeed
 *
 */
int pm_wakeup_source_set(pm_wakeup_source_e wakeup_source, void* source_param);

/**
 * @brief module vote sleep ctrl
 *
 * other module tell pm module, they have entered sleep.(eg.wifi or bt enter sleep,then call the function ,tell pm it have entered sleep)
 *
 * @attention
 * - This API is used by wifi or bt... let pm module know them(wifi or bt...) enter sleep or exit sleep
 * - if all the specific module enter sleep, the mcu will enter low voltage
 *
 * @param
 * -module:module name
 * -sleep_state:0x1:enter sleep;0x0:exit sleep
 * -sleep_time: sleep time
 * @return
 *    - BK_OK: succeed
 *
 */
int pm_module_vote_sleep_ctrl(pm_power_module_name_e module,uint32_t sleep_state,uint32_t sleep_time);

/**
 * @brief pm module vote power ctrl
 *
 * ther module tell pm module, they can enter power on or power off
 *
 * @attention
 *  - This API is used for the module power on and power off.
 *  - if all the specific module power off, the mcu will enter deep sleep
 * @param 
 * -module:module name
 * -power_state:0x1:power off;0x0:power on
 * @return
 * - BK_OK: succeed
 *
 */
int pm_module_vote_power_ctrl(pm_power_module_name_e module,pm_power_module_state_e power_state);

/**
 * @brief pm suppress ticks and sleep
 *
 * when the rtos enter idle task and sleep time > 2*(time per tick),it will call the function enter power manager
 *
 * @attention
 * - This API is used for power manager.
 *
 * @param
 * -sleep_ticks:sleep time using tick unit
 * @return
 * - BK_OK: succeed
 *
 */
int pm_suppress_ticks_and_sleep(UINT32 sleep_ticks);



#ifdef __cplusplus
}
#endif

#endif
