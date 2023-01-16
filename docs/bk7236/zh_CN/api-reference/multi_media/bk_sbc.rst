SBC APIs
================

:link_to_translation:`en:[English]`

.. important::

   The SBC Driver is just only APIs for SBC decoding function. And the SBC API v1.0 is the lastest and stable SBC decoder APIs. All new applications should use SBC API v1.0.

SBC User Guide
----------------------------
The sbc and msbc are used as follow:

.. figure:: ../../../_static/sbc_user_guide_flow.png
    :align: center
    :alt: SBC User Guide Flow
    :figclass: align-center

    SBC User Guide Flow

SBC Interface
----------------------------

The BK SBC Driver supports following functions:

 - sbc function
 - msbc function

The SBC Driver support msbc decoder function. If the application need to use msbc decoder function, Just use the bk_sbc_decoder_support msbc() to enable msbc function.

SBC API Categories
----------------------------

Most of SBC APIs can be categoried as:

 - Common or sbc APIs

   The common APIs are prefixed with bk_sbc, the APIs may be common for sbc and msbc interfaces, e.g. bk_sbc_decoder_init() etc.

 - MSBC function APIs.

   The APIs provide support for msbc function. e.g. bk_sbc_decoder_support msbc().

Common APIs:
 - :cpp:func:`bk_sbc_decoder_init` - init the sbc module
 - :cpp:func:`bk_sbc_decoder_deinit` - deinit the sbc module
 - :cpp:func:`bk_sbc_decoder_bit_allocation` - calculate the bit allocation
 - :cpp:func:`bk_sbc_decoder_frame_decode` - decode one sbc frame
 - :cpp:func:`bk_sbc_decoder_interrupt_enable` - enable or disable the sbc interrupt
 - :cpp:func:`bk_sbc_decoder_support_msbc` - enable or disable msbc decoder function
 - :cpp:func:`bk_sbc_decoder_register_sbc_isr` - register sbc isr

API Reference
----------------------------------------

.. include:: ../../_build/inc/sbc.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/sbc_types.inc

