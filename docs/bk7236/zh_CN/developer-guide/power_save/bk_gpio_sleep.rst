设置GPIO作为唤醒源，系统进入深度睡眠和低压睡眠说明
=======================================================


设置GPIO作为唤醒源，系统进入深度睡眠调用函数和调用顺序说明
--------------------------------------------------------------------
- 1. bk_gpio_register_wakeup_source(GPIO_18,GPIO_INT_TYPE_FALLING_EDGE);

  a. 设置唤醒的GPIO id : GPIO18

  b. 设置GPIO唤醒模式:

    GPIO_INT_TYPE_FALLING_EDGE: 下降延唤醒;

    GPIO_INT_TYPE_RISING_EDGE: 上升延唤醒;

    GPIO_INT_TYPE_LOW_LEVEL: 低电平唤醒;

    GPIO_INT_TYPE_HIGH_LEVEL: 低电平唤醒;

- 2. bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

    向系统中设置GPIO方式作为唤醒源；

- 3. bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);

    设置睡眠模式，进入深度睡眠

    (系统从深度睡眠唤醒后，可以通过调用 - bk_gpio_get_wakeup_gpio_id() - 函数接口获取当前时哪个GPIO ID唤醒系统)


设置GPIO作为唤醒源，系统进入低压睡眠调用函数和调用顺序说明
--------------------------------------------------------------
- 1. bk_gpio_register_wakeup_source(GPIO_18,GPIO_INT_TYPE_FALLING_EDGE);

  a.设置唤醒的GPIO id:GPIO18;

  b.设置GPIO唤醒模式:

    GPIO_INT_TYPE_FALLING_EDGE: 下降延唤醒;

    GPIO_INT_TYPE_RISING_EDGE: 上升延唤醒;

    GPIO_INT_TYPE_LOW_LEVEL: 低电平唤醒;

    GPIO_INT_TYPE_HIGH_LEVEL: 低电平唤醒;

- 2. bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

    向系统中设置GPIO方式作为唤醒源；

- 3. bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

    设置睡眠模式，进入低压睡眠。(备注：当前系统默认设置为低压睡眠，只需要满足投的票，就可以进入低压睡眠)

- 4. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);

    向系统中投应用程序的票。


备注：
 - 1. BT和WIFI的票，BT和WIFI模块内部进入睡眠后自己投上，SDK内部做好，应用程序不用关注

 - 2. 当系统中BT或WIFI模块没有上电时，系统会自动给这两个模块投上sleep的票。

:link_to_translation:`en:[English]`

