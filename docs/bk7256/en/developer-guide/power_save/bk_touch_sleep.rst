enter deepsleep or low voltage sleep mode,  using the TOUCH wakeup source description
==============================================================================================


enter deepsleep, using the TOUCH wakeup source description
---------------------------------------------------------------
- 1. if you use the touch to wakeup the system, you should init the touch module before entering deepsleep;it also can send the command using the PC SSCOM to test.

comment:using the test command to init touch：

- 1.1. Cli command: "touch_single_channel_calib_mode_test 2 1" 

comment:(2: touch channel,which is used to wakeup the system; 1:Driving capacity)


- 2. enter deepsleep setting

- 2.1. touch_wakeup_param_t   touch_wakeup_param  = {0};

  - touch_wakeup_param.touch_channel = 2;

  comment:using the touch channel 2 as the wakeup trigger channel
 
  - bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_TOUCHED, &touch_wakeup_param);
  
  comment: set wakeup source(touch)


- 2.2. bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);

  comment：set sleep mode(deep sleep)

comment：
- 1)if the WIFI and BT are working, the SDK will stop the WIFI and BT work, WIFI and BT will enter sleep before entering deepsleep.
- 2)if you open mult-media(audio,video) before entering deepsleep,you should close them before entering deepsleep.


enter low voltage sleep mode,  using the TOUCH wakeup source description
---------------------------------------------------------------------------
- 1. if you use the touch to wakeup the system, you should init the touch module before entering deepsleep;it also can send the command using the PC SSCOM to test.

comment:using the test command to init touch：

- 1.1. Cli command: "touch_single_channel_calib_mode_test 2 1" 

comment:(2: touch channel,which is used to wakeup the system; 1:Driving capacity)


- 2. enter deepsleep setting

- 2.1. - touch_wakeup_param_t    touch_wakeup_param  = {0};
       - touch_wakeup_param.touch_channel = 2;  
       
	   comment:using the touch channel 2 as the wakeup trigger channel

       -  bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_TOUCHED, &touch_wakeup_param);
	   comment: set wakeup source(touch)


- 2.2. bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

//eg：set sleep mode(low voltage)(comment：the low voltage is default sleep mode.When it meet the entering low voltage required tickets, it will enter the low voltage sleep mode)


- 2.3. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);
  
  comment：vote the APP ticket.。


comment：
 - 1. BT and WIFI vote themselves sleep ticket.The APP ticket is voted by the user code.Util all the modules vote themselves sleep ticket, the system can enter low voltage sleep mode。

WIFI and BT call "bk_pm_module_vote_sleep_ctrl()"function to vote their ticket，tell the system(mcu) that they have entered sleep.

 - 2. when BT and WIFI power off，the system(pm module) will auto vote the WIFI and BT sleep tickets。

:link_to_translation:`en:[English]`

