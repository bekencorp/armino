设置touch作为唤醒源，系统进入深度睡眠和低压睡眠说明
=======================================================


设置touch作为唤醒源，系统进入深度睡眠调用函数和调用顺序说明
-----------------------------------------------------------------
- 1. 使用touch使用唤醒源时，首先需要touch模块初始化后才能使用，可以在代码中调用初始化代码，也可以通过cli command命令来测试。

touch初始化的cli command例如：

- 1.1. Cli command: "touch_single_channel_calib_mode_test 2 1" 

使用touch的channel 2作为唤醒的触发channel,设置的值为1


- 2. 调用pm模块接口进入深度睡眠

- 2.1 配置TOUCH唤醒源的参数
   touch_wakeup_param_t   touch_wakeup_param  = {0};
   touch_wakeup_param.touch_channel = 2;
   bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_TOUCHED, &touch_wakeup_param);
   使用touch的channel 2作为唤醒的触发channel

- 2.2 bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);
   设置睡眠模式，进入深度睡眠


设置touch作为唤醒源，系统进入低压睡眠调用函数说明
------------------------------------------------------------
- 1. 使用touch使用唤醒源时，首先需要touch模块初始化后才能使用，可以在代码中调用初始化代码，也可以通过cli command命令来测试。

touch初始化的cli command例如：

- 1.1. Cli command: "touch_single_channel_calib_mode_test 2 1" 
以上cli command 命令的含义：使用touch的channel 2作为唤醒的触发channel,设置的值为1


- 2. 调用pm模块接口进入低压睡眠

- 2.1 配置TOUCH唤醒源的参数 
       touch_wakeup_param_t    touch_wakeup_param  = {0};
       touch_wakeup_param.touch_channel = 2;  
       bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_TOUCHED, &touch_wakeup_param);

- 2.2 bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);
      设置睡眠模式，进入低压睡眠。(备注：当前系统默认设置为低压睡眠，只需要满足投的票，就可以进入低压睡眠)

- 2.3. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);
      向系统中投应用程序的票。


备注:

 - 1. BT和WIFI的票，BT和WIFI模块内部进入睡眠后自己投上，SDK内部做好，应用程序不用关注

 - 2. 当系统中BT或WIFI模块没有上电时，系统会自动给这两个模块投上sleep的票。

:link_to_translation:`en:[English]`

