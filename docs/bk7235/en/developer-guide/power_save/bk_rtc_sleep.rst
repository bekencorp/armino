enter deepsleep or low voltage sleep mode,  using the RTC wakeup source description
============================================================================================

:link_to_translation:`zh_CN:[中文]`

enter deepsleep , using the RTC wakeup source description
----------------------------------------------------------------

- 1. Configure the parameters of the RTC wakeup source

    alarm_info_t low_valtage_alarm = {

                                      "low_vol",

                                      3000*AON_RTC_MS_TICK_CNT,//eg:set wakeup time: 3000ms

                                      1,

                                      cli_pm_rtc_callback,

                                      NULL

                                      };

    bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);


- 2. bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);
  comment：set wakeup source(RTC)


- 3. bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);

comment：set sleep mode(deep sleep), it will enter deepsleep

comment::
 - 1. if the WIFI and BT are working, the SDK will stop the WIFI and BT work, WIFI and BT will enter sleep before entering deepsleep.
 - 2. if you open mult-media(audio,video) before entering deepsleep,you should close them before entering deepsleep.

enter low voltage sleep mode,  using the RTC wakeup source description
-------------------------------------------------------------------------

- 1. Configure the parameters of the RTC wakeup source

    alarm_info_t low_valtage_alarm = {

                                    "low_vol",

                                    3000*AON_RTC_MS_TICK_CNT,//eg:set wakeup time: 3000ms

                                    1,

                                    cli_pm_rtc_callback,

                                    NULL

                                    };

     bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);

- 2. bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);
     comment：set wakeup source(RTC)


- 3. bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

comment：set sleep mode(low voltage)(comment：the low voltage is default sleep mode.When it meet the entering low voltage required tickets, it will enter the low voltage sleep mode)


- 4. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);

comment：vote the APP ticket.


comment:

 - 1. BT and WIFI vote themselves sleep ticket.The APP ticket is voted by the user code.Util all the modules vote themselves sleep ticket, the system can enter low voltage sleep mode.

WIFI and BT call "bk_pm_module_vote_sleep_ctrl()"function to vote their ticket，tell the system(mcu) that they have entered sleep.


 - 2. when BT and WIFI power off，the system(pm module) will auto vote the WIFI and BT sleep tickets.


