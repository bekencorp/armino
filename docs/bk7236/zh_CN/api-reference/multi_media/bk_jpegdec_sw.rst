JPEGDEC APIs
================

:link_to_translation:`en:[English]`

.. important::

   The JPEGDEC API v1.0 is the lastest stable JPEGDEC APIs. All new applications should use JPEGDEC API v1.0. This component is software decode not hardware decode

JPEGDEC API Categories
----------------------------

Most of JPEGDEC APIs can be categoried as:

 - JPEGDEC APIs

   The common APIs are prefixed with bk_jpeg_dec_sw, e.g. bk_jpeg_dec_sw_init() etc.

JPEGDEC APIs:
 - :cpp:func:`bk_jpeg_dec_sw_init` - init jpeg decode software module
 - :cpp:func:`bk_jpeg_dec_sw_deinit` - deinit jpeg decode software module
 - :cpp:func:`bk_jpeg_sw_decoder_fun` - excute jpeg decode
 - :cpp:func:`bk_jpeg_decode_sw_register_finish_callback` - register jpeg decode finish callback

API Reference
----------------------------------------

.. include:: ../../_build/inc/jpeg_decode.inc

API Typedefs
----------------------------------------