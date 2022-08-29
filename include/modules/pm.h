#ifndef _PM_H_
#define _PM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <driver/int.h>
#include "common/bk_err.h"


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
	PM_POWER_MODULE_NAME_MEM1 = 0,   // 0
	PM_POWER_MODULE_NAME_MEM2,       // 1
	PM_POWER_MODULE_NAME_MEM3,       // 2
	PM_POWER_MODULE_NAME_ENCP,       // 3
	PM_POWER_MODULE_NAME_BAKP,       // 4
	PM_POWER_MODULE_NAME_AHBP,       // 5
	PM_POWER_MODULE_NAME_AUDP,       // 6
	PM_POWER_MODULE_NAME_VIDP,       // 7
	PM_POWER_MODULE_NAME_BTSP,       // 8
	PM_POWER_MODULE_NAME_WIFIP_MAC,  // 9
	PM_POWER_MODULE_NAME_PHY,        // 10
	PM_POWER_MODULE_NAME_CPU1 ,      // 11
	PM_POWER_MODULE_NAME_APP ,       // 12  app not power domain
	PM_POWER_SUB_MODULE_NAME_AUDP_FFT ,     // 13
	PM_POWER_SUB_MODULE_NAME_AUDP_SBC ,     // 14
	PM_POWER_SUB_MODULE_NAME_AUDP_AUDIO ,   // 15
	PM_POWER_SUB_MODULE_NAME_AUDP_I2S ,     // 16
	PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_EN , // 17
	PM_POWER_SUB_MODULE_NAME_VIDP_JPEG_DE , // 18
	PM_POWER_SUB_MODULE_NAME_VIDP_DMA2D ,   // 19
	PM_POWER_SUB_MODULE_NAME_VIDP_LCD ,     // 20
	PM_POWER_SUB_MODULE_NAME_PHY_BT ,       // 21
	PM_POWER_SUB_MODULE_NAME_PHY_WIFI ,     // 22
	PM_POWER_SUB_MODULE_NAME_AHBP_CAN ,     // 23
	PM_POWER_SUB_MODULE_NAME_AHBP_QSPI ,    // 24
	PM_POWER_SUB_MODULE_NAME_AHBP_USB ,     // 25
	PM_POWER_SUB_MODULE_NAME_AHBP_PSRAM ,   // 26

	PM_POWER_MODULE_NAME_NONE               // 27
}pm_power_module_name_e;
typedef enum
{
	PM_SLEEP_MODULE_NAME_MEM1 = 0,   // 0
	PM_SLEEP_MODULE_NAME_MEM2,       // 1
	PM_SLEEP_MODULE_NAME_MEM3,       // 2
	PM_SLEEP_MODULE_NAME_ENCP,       // 3
	PM_SLEEP_MODULE_NAME_BAKP,       // 4
	PM_SLEEP_MODULE_NAME_AHBP,       // 5
	PM_SLEEP_MODULE_NAME_AUDP,       // 6
	PM_SLEEP_MODULE_NAME_VIDP,       // 7
	PM_SLEEP_MODULE_NAME_BTSP,       // 8
	PM_SLEEP_MODULE_NAME_WIFIP_MAC,  // 9
	PM_SLEEP_MODULE_NAME_WIFI_PHY,   // 10
	PM_SLEEP_MODULE_NAME_CPU1 ,      // 11
	PM_SLEEP_MODULE_NAME_APP ,       // 12
	PM_SLEEP_MODULE_NAME_APP1 ,      // 13
	PM_SLEEP_MODULE_NAME_APP2 ,      // 14
	PM_SLEEP_MODULE_NAME_APP3 ,      // 15
	PM_SLEEP_MODULE_NAME_APP4 ,      // 16
	PM_SLEEP_MODULE_NAME_APP5 ,      // 17
	PM_SLEEP_MODULE_NAME_APP6 ,      // 18
	PM_SLEEP_MODULE_NAME_APP7 ,      // 19
	PM_SLEEP_MODULE_NAME_APP8 ,      // 20
	PM_SLEEP_SUB_MODULE_NAME_AUDP_FFT ,     // 21
	PM_SLEEP_SUB_MODULE_NAME_AUDP_SBC ,     // 22
	PM_SLEEP_SUB_MODULE_NAME_AUDP_AUDIO ,   // 23
	PM_SLEEP_SUB_MODULE_NAME_AUDP_I2S ,     // 24
	PM_SLEEP_SUB_MODULE_NAME_VIDP_JPEG_EN , // 25
	PM_SLEEP_SUB_MODULE_NAME_VIDP_JPEG_DE , // 26
	PM_SLEEP_SUB_MODULE_NAME_VIDP_DMA2D ,   // 27
	PM_SLEEP_SUB_MODULE_NAME_VIDP_LCD ,     // 28
	PM_SLEEP_MODULE_NAME_NONE               // 29
}pm_sleep_module_name_e;
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
typedef enum
{
	PM_LPO_SRC_DIVD = 0,//32K from 26m
	PM_LPO_SRC_X32K,    //extern 32k
	PM_LPO_SRC_ROSC,    //32K from ROSC
	PM_LPO_SRC_DEFAULT  //32K from ROSC
}pm_lpo_src_e;
typedef enum
{
	PM_CLK_ID_I2C1 = 0, // 0
	PM_CLK_ID_SPI_1,    // 1
	PM_CLK_ID_UART1,    // 2
	PM_CLK_ID_PWM_1,    // 3
	PM_CLK_ID_TIMER_1,  // 4
	PM_CLK_ID_SARADC,   // 5
	PM_CLK_ID_IRDA,     // 6
	PM_CLK_ID_EFUSE,    // 7
	PM_CLK_ID_I2C2,     // 8
	PM_CLK_ID_SPI_2,    // 9
	PM_CLK_ID_UART2,    // 10
	PM_CLK_ID_UART3,    // 11
	PM_CLK_ID_PWM_2,    // 12
	PM_CLK_ID_TIMER_2,  // 13
	PM_CLK_ID_TIMER_3,  // 14
	PM_CLK_ID_OTP,      // 15
	PM_CLK_ID_I2S_1,    // 16
	PM_CLK_ID_USB_1,    // 17
	PM_CLK_ID_CAN,      // 18
	PM_CLK_ID_PSRAM,    // 19
	PM_CLK_ID_QSPI_1,   // 20
	PM_CLK_ID_QSPI_2,   // 21
	PM_CLK_ID_SDIO,     // 22
	PM_CLK_ID_AUXS,     // 23
	PM_CLK_ID_BTDM,     // 24
	PM_CLK_ID_XVR,      // 25
	PM_CLK_ID_MAC,      // 26
	PM_CLK_ID_PHY,      // 27
	PM_CLK_ID_JPEG,     // 28
	PM_CLK_ID_DISP,     // 29
	PM_CLK_ID_AUDIO,    // 30
	PM_CLK_ID_WDG_CPU,  // 31

	PM_CLK_ID_NONE
}pm_dev_clk_e;
typedef enum
{
	PM_CLK_CTRL_PWR_DOWN = 0,
	PM_CLK_CTRL_PWR_UP,
}pm_dev_clk_pwr_e;
typedef enum
{
	PM_DEV_ID_I2C1 = 0, // 0
	PM_DEV_ID_SPI_1,    // 1
	PM_DEV_ID_UART1,    // 2
	PM_DEV_ID_PWM_1,    // 3
	PM_DEV_ID_TIMER_1,  // 4
	PM_DEV_ID_SARADC,   // 5
	PM_DEV_ID_IRDA,     // 6
	PM_DEV_ID_EFUSE,    // 7
	PM_DEV_ID_I2C2,     // 8
	PM_DEV_ID_SPI_2,    // 9
	PM_DEV_ID_UART2,    // 10
	PM_DEV_ID_UART3,    // 11
	PM_DEV_ID_PWM_2,    // 12
	PM_DEV_ID_TIMER_2,  // 13
	PM_DEV_ID_TIMER_3,  // 14
	PM_DEV_ID_OTP,      // 15
	PM_DEV_ID_I2S_1,    // 16
	PM_DEV_ID_USB_1,    // 17
	PM_DEV_ID_CAN,      // 18
	PM_DEV_ID_PSRAM,    // 19
	PM_DEV_ID_QSPI_1,   // 20
	PM_DEV_ID_QSPI_2,   // 21
	PM_DEV_ID_SDIO,     // 22
	PM_DEV_ID_AUXS,     // 23
	PM_DEV_ID_BTDM,     // 24
	PM_DEV_ID_XVR,      // 25
	PM_DEV_ID_MAC,      // 26
	PM_DEV_ID_PHY,      // 27
	PM_DEV_ID_JPEG,     // 28
	PM_DEV_ID_DISP,     // 29
	PM_DEV_ID_AUDIO,    // 30
	PM_DEV_ID_WDG,      // 31

	PM_DEV_ID_DEFAULT,  // 32  it is used by pm module set default cpu frequency

	PM_DEV_ID_MAX
}pm_dev_id_e;
typedef enum
{
	PM_CPU_FRQ_26M = 0,  // 0:CPU:26M,BUS:26M
	PM_CPU_FRQ_120M,     // 1:CPU:120M,BUS:120M
	PM_CPU_FRQ_240M,     // 2:CPU:240M,BUS:120M
	PM_CPU_FRQ_320M,     // 3:CPU:320M,BUS:160M
	PM_CPU_FRQ_DEFAULT   // default cpu frequency which control by pm module
}pm_cpu_freq_e;

typedef int (*pm_cb)(uint64_t sleep_time, void *args);
typedef struct {
    pm_cb cb;
    void *args;
} pm_cb_conf_t;

/**
 * @brief using the gpio to control the external ldo
 *
 * control the external ldo
 *
 * @attention
 * - This API is used to use the specific gpio(define in  GPIO_CTRL_LDO_OUTPUT_HIGH_MAP or GPIO_CTRL_LDO_OUTPUT_LOW_MAP in gpio_map.h) control the external ldo
 *
 * @param
 * -value:0x1:output high; 0x0:output low
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_external_ldo_ctrl(uint32_t value);
/**
 * @brief get the state of phy calibration
 *
 * get the state of phy calibration
 *
 * @attention
 * - This API is used to get the state of phy calibration
 *
 * @param
 * -void
 * @return
 *  - the state of phy calibration(0x1:have calibration;0x0:not calibration)
 */
uint32_t bk_pm_phy_cali_state_get();
/**
 * @brief get the flag of phy reinit part1
 *
 * get the flag of phy reinit part1
 *
 * @attention
 * - This API is used to get the flag of phy reinit part1
 *
 * @param
 * -void
 * @return
 *  - the flag of phy reinit part1 (True:have part1;false:not do it)
 */
bool bk_pm_phy_reinit_flag_get();
/**
 * @brief clear the flag of phy reinit part1
 *
 * clear the flag of phy reinit part1
 *
 * @attention
 * - This API is used to clear the flag of phy reinit part1
 *
 * @param
 * -void
 * @return
 * -void
 */
void bk_pm_phy_reinit_flag_clear();
/**
 * @brief get the consume time from lowvol wakeup
 *
 * the consume time from lowvol wakeup
 *
 * @attention
 * - This API is used to get the consume time from lowvol wakeup
 *
 * @param
 * -void
 * @return
 *  - the consume time of wakeup from lowvol
 */
uint32_t bk_pm_wakeup_from_lowvol_consume_time_get();
/**
 * @brief register sleep mode(low voltage and deepsleep) config
 *
 * register sleep config(include callback function and parameter)
 *
 * @attention
 * - This API is used to register low voltage and deepsleep config
 *
 * @param
 * -sleep_mode:low voltage or deepsleep mode
 * -dev_id:dev id
 * -enter_config:enter sleep config
 * -exit_config:exit sleep config
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_sleep_register_cb(pm_sleep_mode_e sleep_mode,pm_dev_id_e dev_id,pm_cb_conf_t *enter_config, pm_cb_conf_t *exit_config);
/**
 * @brief unregister sleep mode(low voltage and deepsleep) config
 *
 * unregister sleep config(include callback function and parameter)
 *
 * @attention
 * - This API is used to unregister low voltage and deepsleep config
 *
 * @param
 * -sleep_mode:low voltage or deepsleep mode
 * -dev_id:dev id
 * -enter_cb:whether unregister the enter call back
 * -exit_cb:whether unregister the exit call back
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_sleep_unregister_cb(pm_sleep_mode_e sleep_mode,pm_dev_id_e dev_id,bool enter_cb, bool exit_cb);
/**
 * @brief register light sleep config
 *
 * register light sleep config(include callback function and parameter)
 *
 * @attention
 * - This API is used to register light sleep config
 *
 * @param
 * -enter_config:enter light sleep config
 * -exit_config:exit light sleep config
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_light_sleep_register_cb(pm_cb_conf_t *enter_config, pm_cb_conf_t *exit_config);
/**
 * @brief unregister light sleep callback
 *
 * unregister light sleep callback(enter light sleep callback and exit light sleep callback)
 *
 * @attention
 * - This API is used to unregister light sleep callback
 *
 * @param
 * -enter_cb:whether unregister the enter call back
 * -exit_cb:whether unregister the exit call back
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_light_sleep_unregister_cb(bool enter_cb, bool exit_cb);
/**
 * @brief get power domain of module state
 *
 * get the power domain state
 *
 * @attention
 * - This API is used to get the power domain state
 *
 * @param
 * -module:module name
 * @return
 *  -the state of power domain state
 *
 */
int32 bk_pm_module_power_state_get(pm_power_module_name_e module);
/**
 * @brief get the cpu frequency of the module vote
 *
 * get the module voting cpu frequency
 *
 * @attention
 * - This API is used to get the module voting cpu frequency
 *
 * @param
 * -module:module id
 * @return
 *  - the cpu frequency of the module vote
 *
 */
pm_cpu_freq_e bk_pm_module_current_cpu_freq_get(pm_dev_id_e module);
/**
 * @brief get the current max and used cpu frequency
 *
 * select the cpu frequency
 *
 * @attention
 * - This API is used to get the current max and used cpu frequency
 *
 * @param
 * -void
 * @return
 *  - get the current max and used cpu frequency
 *
 */
pm_cpu_freq_e bk_pm_current_max_cpu_freq_get();
/**
 * @brief vote cpu frequency
 *
 * select the cpu frequency
 *
 * @attention
 * - This API is used to vote ,then select the cpu frequency
 *
 * @param
 * -module:module id;cpu_freq:cpu frequency(320M,240M,120M,26M)
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_module_vote_cpu_freq(pm_dev_id_e module,pm_cpu_freq_e cpu_freq);
/**
 * @brief clock ctrl
 *
 * enable or disable dev clock
 *
 * @attention
 * - This API is used to enable or disable dev clock
 *
 * @param
 * -module:device id;clock_state:PM_CLK_CTRL_PWR_DOWN or  PM_CLK_CTRL_PWR_DOWN
 * @return
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_clock_ctrl(pm_dev_clk_e module,pm_dev_clk_pwr_e clock_state);
/**
 * @brief lp voltage set
 *
 * set the lp voltage
 *
 * @attention
 * - This API is used to set lp voltage when enter low voltage
 *
 * @param
 * -uint32_t:0x0:0.6v;0x1:0.7v;0x2:0.8v;0x3:0.9v;0x4:1.0v;0x5:1.1v;0x6:1.2v;0x7:1.3v;
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_lp_vol_set( uint32_t lp_vol);

/**
 * @brief lp voltage get
 *
 * get the lpo voltage
 *
 * @attention
 * - This API is used to get  lpo voltage value
 *
 * @param
 * -void
 * @return
 * - the lp voltage value
 *
 */
uint32_t bk_pm_lp_vol_get();
/**
 * @brief lpo source set
 *
 * set the lpo source
 *
 * @attention
 * - This API is used to select 32k source when enter low voltage and deepsleep
 *
 * @param
 * -lpo_src:0x0:32K from 26m;0x1:32K from 26m;0x2:32K from ROSC;0x3:32K from ROSC
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 */
bk_err_t bk_pm_lpo_src_set(pm_lpo_src_e lpo_src);

/**
 * @brief lpo source get
 *
 * get the lpo source
 *
 * @attention
 * - This API is used to get 32k lpo source
 *
 * @param
 * void
 * @return
 * -lpo_src:0x0:32K from 26m;0x1:32K from 26m;0x2:32K from ROSC;0x3:32K from ROSC
 *
 */
pm_lpo_src_e bk_pm_lpo_src_get();
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
 *  - BK_OK: succeed
 *  - others: other errors.
 */
bk_err_t bk_pm_mcu_pm_ctrl(uint32_t power_state);

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
uint32_t bk_pm_mcu_pm_state_get();

/**
 * @brief set sleep mode
 *
 * set sleep mode.
 *
 * @attention
 * - This API set sleep mode
 *
 * @param sleep mode:0x0:NORMAL_SLEEP;0x1:LOW_VOLTAGE;0x2:DEEP_SLEEP;0x3:DEFAULT(if it meet low voltage,enter low voltage,otherwise enter normal sleep);
 *
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 */
bk_err_t bk_pm_sleep_mode_set(pm_sleep_mode_e sleep_mode);

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
 * - others: other errors.
 */
bk_err_t bk_pm_wakeup_source_set(pm_wakeup_source_e wakeup_source, void* source_param);

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
 * - BK_OK: succeed
 * - others: other errors.
 */
bk_err_t bk_pm_module_vote_sleep_ctrl(pm_sleep_module_name_e module,uint32_t sleep_state,uint32_t sleep_time);

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
 * - others: other errors.
 */
bk_err_t bk_pm_module_vote_power_ctrl(pm_power_module_name_e module,pm_power_module_state_e power_state);

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
 * - others: other errors.
 */
bk_err_t bk_pm_suppress_ticks_and_sleep(uint32_t sleep_ticks);
/**
 * @brief enter sleep
 *
 * the function is for adapting harmony os
 *
 * @attention
 * - This API is used for power manager.
 *
 * @param
 * - void
 * @return
 * - void
 *
 */
void  bk_pm_enter_sleep();

#ifdef __cplusplus
}
#endif

#endif
