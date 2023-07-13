Aud_Intf APIs
=============================

:link_to_translation:`en:[English]`

.. important::

	The Aud_Intf API v1.0 is the lastest and stable Audio interface APIs. All new applications should use Audio interface API v1.0.
	And all Aud_Intf API cannot be called in mic and speaker callback registed in  `bk_aud_intf_drv_init` .

Audio Interface
----------------------------
The BK Audio interface supports two work mode:
 - General mode
 - Voice mode

The general work mode supports general functions, and the Voice mode supports voice transfer functions. However, the audio only work in one mode at a time.

The General mode supports following functions:
 - record function
 - play function

The record and play function can be operated independently.

The Voice mode supports following features:
 - voice transfer by G711A data
 - voice transfer by PCM data
 - 8k and 16k sample rate
 - software AEC
 - set mic gain
 - set speaker gain
 - onboard mic and speaker
 - UAC mic and speaker

Audio Interface API Categories
------------------------------------------

Most of Audio APIs can be categoried as:

 - Common APIs

   The common APIs are prefixed with bk_aud_intf, the APIs may be common for General mode and Voice mode, e.g. bk_aud_intf_drv_init() etc.

 - Mic function APIs.

   The APIs provide support for record function. e.g. bk_aud_intf_mic_init() etc.

 - Spk function APIs.

   The APIs provide support for play function. e.g. bk_aud_intf_spk_init() etc.

 - Voc function APIs.

   The APIs provide support for voice transfer function. e.g. bk_aud_intf_voc_init() etc.

Common APIs:
 - :cpp:func:`bk_aud_intf_drv_init` - init the audio interface driver
 - :cpp:func:`bk_aud_intf_drv_deinit` - deinit the audio interface driver
 - :cpp:func:`bk_aud_intf_set_mode` - set audio interface work mode
 - :cpp:func:`bk_aud_intf_set_mic_gain` - set audio mic gain
 - :cpp:func:`bk_aud_intf_set_spk_gain` - set audio speaker gain
 - :cpp:func:`bk_aud_intf_write_spk_data` - write data to sperker

Mic APIs:
 - :cpp:func:`bk_aud_intf_mic_init` - init the mic function
 - :cpp:func:`bk_aud_intf_mic_deinit` - deinit the mic function
 - :cpp:func:`bk_aud_intf_mic_start` - start mic work
 - :cpp:func:`bk_aud_intf_mic_pause` - pause mic work
 - :cpp:func:`bk_aud_intf_mic_stop` - stop mic work
 - :cpp:func:`bk_aud_intf_set_mic_chl` - set mic channel in work
 - :cpp:func:`bk_aud_intf_get_mic_chl` - get mic channel in work
 - :cpp:func:`bk_aud_intf_set_mic_samp_rate` - set mic sample rate
 - :cpp:func:`bk_aud_intf_get_mic_samp_rate` - get mic sample rate

Spk APIs:
 - :cpp:func:`bk_aud_intf_spk_init` - init the speaker function
 - :cpp:func:`bk_aud_intf_spk_deinit` - deinit the speaker function
 - :cpp:func:`bk_aud_intf_spk_start` - start speaker work
 - :cpp:func:`bk_aud_intf_spk_pause` - pause speaker work
 - :cpp:func:`bk_aud_intf_spk_stop` - stop speaker work
 - :cpp:func:`bk_aud_intf_set_spk_chl` - set speaker channel in work
 - :cpp:func:`bk_aud_intf_get_spk_chl` - get speaker channel in work
 - :cpp:func:`bk_aud_intf_set_spk_samp_rate` - set speaker sample rate
 - :cpp:func:`bk_aud_intf_get_spk_samp_rate` - get speaker sample rate

Voc APIs:
 - :cpp:func:`bk_aud_intf_voc_init` - init the voice transfer function
 - :cpp:func:`bk_aud_intf_voc_deinit` - deinit the voice transfer function
 - :cpp:func:`bk_aud_intf_voc_start` - start voice transfer
 - :cpp:func:`bk_aud_intf_voc_stop` - stop voice transfer
 - :cpp:func:`bk_aud_intf_set_aec_para` - set aec parameter
 - :cpp:func:`bk_aud_intf_get_aec_para` - get aec parameter
 - :cpp:func:`bk_aud_intf_voc_tx_debug` - register voice tx debug callback
 - :cpp:func:`bk_aud_intf_voc_rx_debug` - register voice rx debug callback
 - :cpp:func:`bk_aud_intf_voc_aec_debug` - register voice aec debug callback

API Reference
----------------------------------------

.. include:: ../../_build/inc/aud_intf.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/aud_intf_types.inc
