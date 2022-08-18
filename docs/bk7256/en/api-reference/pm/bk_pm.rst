PM APIs
================

:link_to_translation:`en:[English]`

.. important::

   The PM API v1.0 is the lastest stable PM APIs.

PM Interface
----------------------------

The BK PM Driver supports following logical PM interfaces:

 - BK enter different sleep mode interface
 - BK clock control interface
 - BK power control interface


PM API Categories
----------------------------

Most of PM APIs can be categoried as:

 - Common APIs
   The common APIs are frequently-used.
 - Specific APIs.
   The specific APIs are used in specific configuration.

Common PM APIs:
 - :cpp:func:`bk_pm_sleep_mode_set` - set sleep mode
 - :cpp:func:`bk_pm_module_vote_sleep_ctrl` - module vote sleep ctrl
 - :cpp:func:`bk_pm_module_vote_power_ctrl` - module vote power ctrl
 - :cpp:func:`bk_pm_wakeup_source_set` - set wakeup source

Specific APIs:
 - :cpp:func:`bk_pm_mcu_pm_state_get` - get the state of whether enter the sleep
 - :cpp:func:`bk_pm_mcu_pm_ctrl` - config whether enter the sleep

 - :cpp:func:`bk_pm_lpo_src_get` - get the lpo source
 - :cpp:func:`bk_pm_lpo_src_set` - set the lpo source

 - :cpp:func:`bk_pm_lp_vol_get` - get the voltage when enter low voltage
 - :cpp:func:`bk_pm_lp_vol_set` - set the voltage when enter low voltage

 - :cpp:func:`bk_pm_light_sleep_unregister_cb` - unregister the light sleep callback
 - :cpp:func:`bk_pm_light_sleep_register_cb` - register the light sleep callback

 - :cpp:func:`bk_pm_clock_ctrl` - module vote power ctrl
 - :cpp:func:`bk_pm_wakeup_from_lowvol_consume_time_get` - get the consume time wakeuping from low voltage
 - :cpp:func:`bk_pm_module_power_state_get` - get the module power domain state
 - :cpp:func:`bk_pm_phy_cali_state_get` - get the state whether phy have calibration
 - :cpp:func:`bk_pm_suppress_ticks_and_sleep` - enter sleep process(return int)
 - :cpp:func:`bk_pm_enter_sleep` - enter sleep process(return void)


Programing Principle
----------------------------------------

.. important::
  Here is some general principle for pm API users:
   - Enter deep sleep and low voltage ,it only need call the "Common PM APIs",if you want to call the Specific APIs,please contact us in case of make issue.
   - Use BK_ERR_CHECK to check the return value of the pm API
   - If you are not sure how to use pm APIs, study the pm used guide example code first.
   - If you are not sure how to initialize some fields of config struct, use the default configuration macro to use the config first and then set application specific fields

User Development Model
----------------------------------------

pm interface detail

API Reference
----------------------------------------

.. include:: ../../_build/inc/pm.inc


