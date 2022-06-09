Hardware JPEGDEC APIs
=======================

:link_to_translation:`en:[English]`

JPEGDEC API Categories
----------------------------

JPEGDEC APIs:
 - :cpp:func:`bk_jpeg_dec_driver_init` - init jpeg decode hardware module
 - :cpp:func:`bk_jpeg_dec_driver_deinit` - deinit jpeg decode hardware module
 - :cpp:func:`bk_jpeg_dec_start_dec` - start jpeg decode
 - :cpp:func:`bk_jpeg_dec_complete_cb` - register dma2d cpu int isr
 - :cpp:func:`bk_jpeg_dec_isr_register` - register jpeg dec complete callback

API Reference
----------------------------------------

.. include:: ../../_build/inc/jpeg_dec.inc

API Typedefs
---------------------
.. include:: ../../_build/inc/jpeg_dec_types.inc