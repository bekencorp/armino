UVC_CAMERA APIs
================

:link_to_translation:`en:[English]`

.. important::

   The UVC_CAMERA API v1.0 is the lastest stable UVC_CAMERA APIs. All new applications should use UVC_CAMERA API v1.0.

UVC_CAMERA Interface
----------------------------
   UVC camera only support full speed type

UVC_CAMERA API Categories
----------------------------

Most of UVC_CAMERA APIs can be categoried as:

 - UVC_CAMERA APIs

   The common APIs are prefixed with bk_uvc, e.g. bk_uvc_init() etc.

UVC_CAMERA APIs:
 - :cpp:func:`bk_uvc_init` - init uvc camera(open uvc device)
 - :cpp:func:`bk_uvc_deinit` - deinit uvc camera(close uvc device)
 - :cpp:func:`bk_uvc_save_frame` - save a frame to sdcard
 - :cpp:func:`bk_uvc_set_ppi_fps` - set uvc camera pps(image resolution) and fps(frame rate)
 - :cpp:func:`bk_uvc_set_start` - start uvc, begin simple image information
 - :cpp:func:`bk_uvc_set_stop` - stop uvc, stop simple image information
 - :cpp:func:`bk_uvc_register_frame_end_callback` - register uvc finish simple a frame callback function

API Reference
----------------------------------------

.. include:: ../../_build/inc/uvc_camera.inc

API Typedefs
----------------------------------------
