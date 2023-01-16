DVP_CAMERA APIs
================

:link_to_translation:`en:[English]`

.. important::

   The DVP_CAMERA API v1.0 is the lastest stable DVP_CAMERA APIs. All new applications should use DVP_CAMERA API v1.0.

DVP_CAMERA Interface
----------------------------

   Current software version support two types dvp sensor: gc0328c and hm1055. they are comunicate by i2c with chip.
   gc0328c have 20pins interface, hm1055 have 24pins interface.

   gc0328c support different image resolution and frame rate:
   1. WQVGA(480*272)
   2. QVGA(320*240)
   3. VGA(640*480)
   frame rate: 5fps/10fps/20fps/25fps

   hm1055 only support 720p(1280*720) image resolution, and different frame rates: 5fps/15fps/20fps

DVP_CAMERA API Categories
----------------------------

Most of DVP_CAMERA APIs can be categoried as:

 - DVP_CAMERA APIs

   The common APIs are prefixed with bk_camera, e.g. bk_camera_driver_init() etc.

DVP_CAMERA APIs:
 - :cpp:func:`bk_dvp_camera_driver_init` - init dvp camera(power on sensor, begin to sample image)
 - :cpp:func:`bk_dvp_camera_driver_deinit` - deinit dvp camera(power off sensor)
 - :cpp:func:`bk_dvp_camera_get_device` - get current dvp dendor config
 - :cpp:func:`bk_dvp_camera_encode_config` - change encode image upper and lower size or close auto encode
 - :cpp:func:`dvp_camera_i2c_read_uint8` - master read dvp senor register value by byte
 - :cpp:func:`dvp_camera_i2c_read_uint16` - master read dvp senor register value by double byte
 - :cpp:func:`dvp_camera_i2c_read_uint8` - master write dvp senor register value by byte
 - :cpp:func:`dvp_camera_i2c_write_uint16` - master write dvp senor register value by double byte

API Reference
----------------------------------------

.. include:: ../../_build/inc/dvp_camera.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/dvp_camera_types.inc
