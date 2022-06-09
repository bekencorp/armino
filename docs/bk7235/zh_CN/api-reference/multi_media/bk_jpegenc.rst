JPEGENC APIs
================

:link_to_translation:`en:[English]`

.. important::

   The JPEGENC API v1.0 is the lastest stable JPEGENC APIs. All new applications should use JPEGENC API v1.0.

JPEGENC Interface
----------------------------
The JPEGENC module can work two different mode
 - 0: JPEG ENCODER mode, output jpeg data
 - 1: YUV mode, output camera sample data, yuv data

JPEGENC API Categories
----------------------------

Most of JPEGENC APIs can be categoried as:

 - JPEGENC APIs

   The common APIs are prefixed with bk_jpeg_enc, e.g. bk_jpeg_enc_driver_init() etc.

JPEGENC APIs:
 - :cpp:func:`bk_jpeg_enc_driver_init` - init jpeg encode module driver
 - :cpp:func:`bk_jpeg_enc_driver_deinit` - deinit jpeg encode module driver
 - :cpp:func:`bk_jpeg_enc_init` - init jpeg encode module
 - :cpp:func:`bk_jpeg_enc_deinit` - deinit jpeg encode module
 - :cpp:func:`bk_jpeg_enc_dvp_init` - init jpeg encode module
 - :cpp:func:`bk_jpeg_enc_dvp_deinit` - deinit jpeg encode module
 - :cpp:func:`bk_jpeg_enc_set_yuv_mode` - set jpeg encode module work mode
 - :cpp:func:`bk_jpeg_enc_set_enable` - set encode enable/disable
 - :cpp:func:`bk_jpeg_enc_get_frame_size` - get a frame size output from jpeg  encode module
 - :cpp:func:`bk_jpeg_enc_register_frame_start_isr` - register sof callback function
 - :cpp:func:`bk_jpeg_enc_register_frame_end_isr` - register eof callback function
 - :cpp:func:`bk_jpeg_enc_register_end_yuv_isr` - register yof callback function
 - :cpp:func:`bk_jpeg_enc_dvp_gpio_enable` - enable jpeg encode module data gpio

API Reference
----------------------------------------

.. include:: ../../_build/inc/jpeg_enc.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/jpeg_enc_types.inc
