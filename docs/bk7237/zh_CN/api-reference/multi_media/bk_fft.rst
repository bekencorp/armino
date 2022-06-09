FFT APIs
================

:link_to_translation:`en:[English]`

.. important::

   The FFT API v1.0 is the lastest stable FFT APIs. All new applications should use FFT API v1.0.

FFT Interface
----------------------------

The BK FFT Driver supports following functional interfaces:

 - FFT function
 - IFFT function

Each function can be operated independently. However both function share the same resources,  only one function can exit and work at a time.

E.g. if the FFT function is working, the IFFT funtion can not work. And calling IFFT functional interface will fail when FFT function is working.

FFT API Categories
----------------------------
Most of FFT APIs can be categoried as:

 - Common APIs

   The common APIs are prefixed with bk_fft, the APIs may be common for all functions and interfaces, e.g. bk_fft_driver_init() etc.
 - FFT function APIs.

   The APIs provide support for FFT and IFFT function. e.g. bk_fft_enable() etc.

Common APIs:
 - :cpp:func:`bk_fft_driver_init` - init FFT
 - :cpp:func:`bk_fft_driver_deinit` - deinit FFT
 - :cpp:func:`bk_fft_is_busy` - get FFT work status
 - :cpp:func:`bk_fft_output_read` - get FFT/IFFT output data

FFT function APIs:
 - :cpp:func:`bk_fft_enable` - start FFT/IFFT function

API Reference
----------------------------------------

.. include:: ../../_build/inc/fft.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/fft_types.inc

