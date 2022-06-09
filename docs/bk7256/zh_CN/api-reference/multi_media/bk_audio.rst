Audio APIs
================

:link_to_translation:`en:[English]`

.. important::

   The Audio API v1.0 is the lastest and stable Audio APIs. All new applications should use Audio API v1.0.

Audio Interface
----------------------------

The BK Audio Driver supports following functions:

 - adc function
 - dac function
 - eq function

The adc and dac function can be operated independently. However the eq function need to use with dac function together.

Audio API Categories
----------------------------

Most of Audio APIs can be categoried as:

 - Common APIs

   The common APIs are prefixed with bk_aud, the APIs may be common for adc and dac interfaces, e.g. bk_aud_driver_init() etc.

 - Adc function APIs.

   The APIs provide support for adc function. e.g. bk_aud_adc_init() etc.

 - Dac function APIs.

   The APIs provide support for dac function. e.g. bk_aud_dac_init() etc.
   
 - Eq function APIs.

   The APIs provide support for eq function. e.g. bk_aud_eq_init() etc.

Common APIs:
 - :cpp:func:`bk_aud_driver_init` - init the audio driver
 - :cpp:func:`bk_aud_driver_deinit` - deinit the audio driver
 - :cpp:func:`bk_aud_register_aud_isr` - register audio isr

Adc APIs:
 - :cpp:func:`bk_aud_adc_init` - init the adc module of audio
 - :cpp:func:`bk_aud_adc_deinit` - deinit the adc module of audio
 - :cpp:func:`bk_aud_set_adc_samp_rate` - set the sample rate in adc work mode
 - :cpp:func:`bk_aud_get_adc_fifo_addr` - get the adc fifo address in adc work mode
 - :cpp:func:`bk_aud_get_dtmf_fifo_addr` - get the dtmf fifo address in adc work mode
 - :cpp:func:`bk_aud_get_adc_status` - get the adc status information in adc work mode
 - :cpp:func:`bk_aud_get_dtmf_status` - get the dtmf status information in dtmf work mode
 - :cpp:func:`bk_aud_enable_adc_int` - enable adc interrupt
 - :cpp:func:`bk_aud_disable_adc_int` - disable adc interrupt
 - :cpp:func:`bk_aud_start_adc` - start adc function
 - :cpp:func:`bk_aud_stop_adc` - stop adc function
 - :cpp:func:`bk_aud_get_adc_fifo_data` - get adc data
 - :cpp:func:`bk_aud_get_dtmf_fifo_data` - get dtmf data

Dac APIs:
 - :cpp:func:`bk_aud_dac_init` - init the dac module of audio
 - :cpp:func:`bk_aud_dac_deinit` - deinit the dac module of audio
 - :cpp:func:`bk_aud_set_dac_samp_rate` - set the sample rate of audio dac function
 - :cpp:func:`bk_aud_get_dac_fifo_addr` - get the dac fifo address
 - :cpp:func:`bk_aud_enable_dac_int` - enable dac interrupt function
 - :cpp:func:`bk_aud_disable_dac_int` - disable dac interrupt function
 - :cpp:func:`bk_aud_get_dac_status` - get the dac fifo status information
 - :cpp:func:`bk_aud_start_dac` - start dac function
 - :cpp:func:`bk_aud_stop_dac` - stop dac function
 
Eq APIs:
 - :cpp:func:`bk_aud_eq_init` - init the eq module of audio
 - :cpp:func:`bk_aud_eq_deinit` - deinit the eq module of audio

API Reference
----------------------------------------

.. include:: ../../_build/inc/aud.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/aud_types.inc

