TOUCH APIs
================

:link_to_translation:`en:[English]`

.. important::

   The TOUCH API v1.0 is the lastest and stable TOUCH APIs. All new applications should use TOUCH API v1.0.

Touch User Guide
----------------------------
Touch module have three different types of working modes, which are automatic calibrattion mode, manul calibration mode and scan mode. The scan mode must work based on the automatic calibration mode or manul mode.
When the touch channel has been calibrated, then set multi channels that neet to scan and enable the scan mode. The automatic calibration mode and manul calibration mode are used as follows:

.. figure:: ../../../_static/touch_user_guide_flow.png
    :align: center
    :alt: Touch User Guide Flow
    :figclass: align-center

    Touch User Guide Flow

Touch APIs
----------------------------

 - :cpp:func:`bk_touch_gpio_init` - init the touch gpio pin
 - :cpp:func:`bk_touch_enable` - enable the touch module funtion
 - :cpp:func:`bk_touch_disable` - enable the touch module funtion
 - :cpp:func:`bk_touch_config` - config the touch parameters to enable touch work
 - :cpp:func:`bk_touch_calibration_start` - start calibrating the touch channel
 - :cpp:func:`bk_touch_scan_mode_enable` - enable or disable scan mode of touch module
 - :cpp:func:`bk_touch_manul_mode_enable` - enable manul mode of touch module
 - :cpp:func:`bk_touch_manul_mode_disable` - disable manul mode of touch module
 - :cpp:func:`bk_touch_scan_mode_multi_channl_set` - set multi channels that need to scan
 - :cpp:func:`bk_touch_int_enable` - enable or disable touch interrupt
 - :cpp:func:`bk_touch_get_calib_value` - get the calibration value of touch channel
 - :cpp:func:`bk_touch_get_touch_status` - get the status of touch channel
 - :cpp:func:`bk_touch_register_touch_isr` - register touch isr

API Reference
----------------------------------------

.. include:: ../../_build/inc/touch.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/touch_types.inc

