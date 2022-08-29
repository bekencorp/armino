Hardware JPEGDEC APIs
=======================

:link_to_translation:`en:[English]`

JPEGDEC API Categories
----------------------------

JPEGDEC APIs:
 - :cpp:func:`bk_jpeg_dec_driver_init` - init jpeg decode hardware module
 - :cpp:func:`bk_jpeg_dec_driver_deinit` - deinit jpeg decode hardware module
 - :cpp:func:`bk_jpeg_dec_hw_start` - start jpeg decode
  - :cpp:func:`bk_jpeg_dec_stop` - stop jpeg decode


 - :cpp:func:`bk_jpeg_dec_isr_register` - register jpeg dec complete callback



HW JPEG DECODE Config
----------------------------------------

.. figure:: ../../../_static/hw_jpeg_decode.png
    :align: center
    :alt: jpeg dec Overview
    :figclass: align-center

    Figure 1. jpeg decode config flow


API Reference
--------------------------------------------
.. include:: ../../_build/inc/jpeg_dec.inc

API Typedefs
-----------------------------------------------
.. include:: ../../_build/inc/jpeg_dec_types.inc