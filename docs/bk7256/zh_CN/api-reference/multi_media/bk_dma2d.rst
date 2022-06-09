DMA2D APIs
=======================

:link_to_translation:`en:[English]`

DMA2D Interface
----------------------------

DMA2D Features
----------------------------

 - fill specific color
       Fill part or all of the target image with a specific color
 - mem to mem copy
       Copy part or all of the source image to part or all of the target image
 - mem to mem with pixel conversion
       Copy part or all of the source image to part or all of  the target image through pixel format conversion
 - mem to mem with pixel conversionwith and blending
       Part or all of the two source images with different pixel formats are mixed,
       and the result is copied to the part or the entire target :image with different color formats

DMA2D API Categories
----------------------------


+-------------------------------------------+----------------------------------------------+
| DMA2D API                                 |                 Description                  |
+===========================================+==============================================+
| :cpp:func:`bk_dma2d_driver_init`          | initializes the DMA2D                        |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_driver_deinit`        | Deinitializes the DMA2D peripheral           |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_layer_config`         | Configure the background or foreground Layer |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_start_transfer`       | Start the DMA2D Transfer                     |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_start_blending`       | Start the multi-source DMA2D Transfer        |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_is_transfer_busy`     | check dma2d is transfer busy or not          |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_int_enable`           | clear dma2d int status                       |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_int_status_clear`     | Lcd transfer pixel config                    |
+-------------------------------------------+----------------------------------------------+

+------------------------------------------------+----------------------------------------------+
| DMA2D Register ISR API                         |                 Description                  |
+================================================+==============================================+
| :cpp:func:`bk_dma2d_isr_register`              | register dma2d cpu int isr                   |
+------------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_dma2d_register_int_callback_isr` | register dma2d int type isr                  |
+------------------------------------------------+----------------------------------------------+


DMA2D Work Config
----------------------

1、fill specific color

.. figure:: ../../../_static/dma2d_fill_color.png
    :align: center
    :alt: fill specific color
    :figclass: align-center

    Figure 1. dma2d_fill_color


2、mem to mem copy

.. figure:: ../../../_static/dma2d_mem_to_mem.png
    :align: center
    :alt: fill specific color
    :figclass: align-center

    Figure 2. dma2d_mem_to_mem


3、mem to mem with pixel conversion

    The Figure API is the same as dma2d_mem_to_mem
    please  reference the API Reference for the different config


4、mem to mem with pixel conversion and blending

.. figure:: ../../../_static/dma2d_mem_to_mem_with_pixel_conv_blend.png
    :align: center
    :alt: fill specific color
    :figclass: align-center

    Figure 3. dma2d_mem_to_mem_with_pixel_conversion_and_blending


API Reference
----------------------------------------

.. include:: ../../_build/inc/dma2d.inc

API Typedefs
-------------------------------------------------
.. include:: ../../_build/inc/hal_dma2d_types.inc