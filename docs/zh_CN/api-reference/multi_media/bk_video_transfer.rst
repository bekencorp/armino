VIDEO_TRANSFER APIs
=============================

:link_to_translation:`en:[English]`

.. important::

   The VIDEO_TRANSFER API v1.0 is the lastest stable VIDEO_TRANSFER APIs. All new applications should use VIDEO_TRANSFER API v1.0.

VIDEO_TRANSFER API Categories
-----------------------------------------

Most of VIDEO_TRANSFER APIs can be categoried as:

 - VIDEO_TRANSFER APIs

   The common APIs are prefixed with bk_video, e.g. bk_video_transfer_init() etc.

VIDEO_TRANSFER APIs:
 - :cpp:func:`bk_video_transfer_init` - init video transfer
 - :cpp:func:`bk_video_transfer_deinit` - deinit video transfer
 - :cpp:func:`bk_video_send_msg` - send msg deal video data
 - :cpp:func:`bk_video_buffer_open` - open video buff
 - :cpp:func:`bk_video_buffer_close` - close video buff
 - :cpp:func:`bk_video_buffer_read_frame` - read fix length video data

API Reference
----------------------------------------

.. include:: ../../_build/inc/video_transfer.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/video_types.inc
