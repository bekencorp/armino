I2S APIs
================

:link_to_translation:`en:[English]`

.. important::

   The I2S API v1.0 is the lastest stable I2S APIs. All new applications should use I2S API v1.0.

I2S Interface
----------------------------

The BK I2S Driver supports following work modes:

 - I2S (Philips)
 - Left Justified
 - Right Justified
 - Short Frame Sync
 - Long Frame Sync
 - Normal 2B+D
 - Delay 2B+D

 The BK I2S Driver supports following sample rates:

 - 8K
 - 12K
 - 16K
 - 24K
 - 32K
 - 48K
 - 96K
 - 8.0182K
 - 11.025K
 - 22.050K
 - 44.1K
 - 88.2K

Each function can be operated independently. However both function share the same resources,  only one function can exit and work at a time.

I2S API Categories
----------------------------

Most of I2S APIs can be categoried as:

 - I2S APIs

   The common APIs are prefixed with bk_i2s, e.g. bk_i2s_driver_init() etc.

I2S APIs:
 - :cpp:func:`bk_i2s_driver_init` - init i2s module driver
 - :cpp:func:`bk_i2s_driver_deinit` - deinit i2s module driver
 - :cpp:func:`bk_i2s_init` - init i2s module
 - :cpp:func:`bk_i2s_deinit` - deinit i2s module
 - :cpp:func:`bk_i2s_get_read_ready` - get the i2s Rx fifo status
 - :cpp:func:`bk_i2s_get_write_ready` - get the i2s Tx fifo status
 - :cpp:func:`bk_i2s_enable` - enable i2s
 - :cpp:func:`bk_i2s_int_enable` - config i2s interrupt
 - :cpp:func:`bk_i2s_set_role` - config i2s work role
 - :cpp:func:`bk_i2s_set_work_mode` - config i2s work mode
 - :cpp:func:`bk_i2s_set_lrck_invert` - config i2s lrck invert
 - :cpp:func:`bk_i2s_set_sck_invert` - config i2s sck invert
 - :cpp:func:`bk_i2s_set_lsb_first` - config i2s lsb first
 - :cpp:func:`bk_i2s_set_sync_len` - config i2s sync length
 - :cpp:func:`bk_i2s_set_data_len` - config i2s data length
 - :cpp:func:`bk_i2s_set_pcm_dlen` - config i2s pcm D length of 2B+D
 - :cpp:func:`bk_i2s_set_store_mode` - config i2s store mode
 - :cpp:func:`bk_i2s_clear_rxfifo` - clear i2s rx fifo
 - :cpp:func:`bk_i2s_clear_txfifo` - clear i2s tx fifo
 - :cpp:func:`bk_i2s_set_txint_level` - config i2s tx interrupt level
 - :cpp:func:`bk_i2s_set_rxint_level` - config i2s rx interrupt level
 - :cpp:func:`bk_i2s_write_data` - write data to i2s tx fifo
 - :cpp:func:`bk_i2s_read_data` - read data from i2s rx fifo
 - :cpp:func:`bk_i2s_get_data_addr` - get i2s data address
 - :cpp:func:`bk_i2s_set_ratio` - config i2s sample rate
 - :cpp:func:`bk_i2s_register_i2s_isr` - register i2s isr

API Reference
----------------------------------------

.. include:: ../../_build/inc/i2s.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/i2s_types.inc

