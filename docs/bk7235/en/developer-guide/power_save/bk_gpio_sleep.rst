设置GPIO作为唤醒源，系统进入深度睡眠和低压睡眠说明
====================================================


设置GPIO作为唤醒源，系统进入深度睡眠调用函数说明
----------------------------------------------------------
- 1. bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);

//例如：设置睡眠模式，进入深度睡眠


- 2. bk_gpio_register_wakeup_source(GPIO_18,GPIO_INT_TYPE_FALLING_EDGE);

//例如： a.设置唤醒的GPIO id:GPIO18;   b.设置GPIO唤醒模式：下降延唤醒。


- 3. bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

//例如：向系统中设置GPIO方式作为唤醒源；


- 4. bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_BTSP,PM_POWER_MODULE_STATE_OFF); 

//例如：向系统中投BT进入deep sleep,此时会把BT电源域下电。


- 5. bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC,PM_POWER_MODULE_STATE_OFF);

//例如：向系统中投WIFI进入deep sleep,此时会把MAC电源域下电。


备注：进入深度睡眠，当前只需要BT，WIFI两个模块投进入深度睡眠的票。

(系统从深度睡眠唤醒后，可以通过调用 - bk_gpio_get_wakeup_gpio_id() - 函数接口获取当前时哪个GPIO ID唤醒系统)


设置GPIO作为唤醒源，系统进入低压睡眠调用函数说明
-----------------------------------------------------------
- 1. bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

//例如：设置睡眠模式，进入低压睡眠。(备注：当前系统默认设置为低压睡眠，只需要满足投的票，就可以进入低压睡眠)


- 2. bk_gpio_register_wakeup_source(GPIO_18,GPIO_INT_TYPE_FALLING_EDGE);

//例如： a.设置唤醒的GPIO id:GPIO18;b.设置GPIO唤醒模式：下降延唤醒。


- 3. bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

//例如：向系统中设置GPIO方式作为唤醒源；


- 4. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);

//例如：向系统中投应用程序的票。


备注：
 - 1. 当前系统的设置为需要BT和WIFI模块进入睡眠状态，以及一张APP应用程序的票，系统才能进入低压模式。

WIFI和BT两个模块根据自己业务会调用bk_pm_module_vote_sleep_ctrl()接口向系统投票，告诉系统自己进入睡眠状态。
APP应用程序可以设置以上的APP的票。

 - 2. 当系统中BT或WIFI模块没有上电时，系统会自动给这两个模块投上sleep的票。

:link_to_translation:`en:[English]`

