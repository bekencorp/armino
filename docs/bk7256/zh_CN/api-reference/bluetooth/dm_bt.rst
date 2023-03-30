DM BT APIs
================

:link_to_translation:`en:[English]`

.. important::

   The DM BT API v1.0 is the lastest stable DM BT APIs. All new applications should use DM BT API v1.0.


DM BT API Categories
----------------------------

Most of DM BT APIs can be categoried as:

Interface specific DM BT APIs:
 - BT common interface
 - BT A2DP Sink interface

Application Example
-------------------

Check :example:`projects/lvgl/soundbar` folder, which contains the following application:

* There is a A2DP sink client demo. This demo can be discovered and connected by A2DP source device and receive the audio stream from remote device


API Reference
----------------------------------------

.. include:: ../../_build/inc/dm_bt.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/dm_bt_types.inc
