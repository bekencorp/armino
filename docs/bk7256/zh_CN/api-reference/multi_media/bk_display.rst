Display APIs
================

:link_to_translation:`en:[English]`

LCD Display Interface
----------------------------

The BK LCD Display Driver supports following interfaces:

 - i8080 LCD  interface
    1) 项目中使用的屏幕Driving IC 为ST7796S.
    2) 8080 接口的数据传输为8位
    3) 接口支持的数据为RGB565

 - RGB interface
    1) 项目中使用的屏幕Driving IC ST7282.
    2) 接口支持的数据包括RGB565,YUYV,UYVY,YYUV,UVYY,VUYY

LCD API Categories
----------------------------

+-------------------------------------------+----------------------------------------------+
| Commom API                                |                 Description                  |
+===========================================+==============================================+
| :cpp:func:`bk_lcd_driver_init`            | Lcd module init of clk and int               |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_pixel_config`           | Lcd transfer pixel config                    |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_isr_register`           | Lcd int tpe isr regidter                     |
+-------------------------------------------+----------------------------------------------+

+-------------------------------------------+----------------------------------------------+
| LCD 8080 API                              |                 Description                  |
+===========================================+==============================================+
| :cpp:func:`bk_lcd_8080_init`              | Init the 8080 lcd interface                  |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_8080_deinit`            | Deinit lcd 8080 interface reg                |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_8080_write_cmd`         | Write 8080 lcd cmd                           |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_8080_write_data`        | Write 8080 lcd data                          |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_8080_int_enable`        | Lcd transfer pixel config                    |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_8080_start_transfer`    | 8080 lcd transfer data to display            |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_8080_display_enable`    | Enable 8080 lcd display                      |
+-------------------------------------------+----------------------------------------------+

+-------------------------------------------+----------------------------------------------+
| LCD RGB API                               |                 Description                  |
+===========================================+==============================================+
| :cpp:func:`bk_lcd_rgb_init`               | Init the rgb lcd interface                   |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_rgb_deinit`             | Deinit rgb lcd interface reg                 |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_rgb_display_en`         | Enable rgb lcd                               |
+-------------------------------------------+----------------------------------------------+
| :cpp:func:`bk_lcd_rgb_int_enable`         | Enable rgb lcd interrupt                     |
+-------------------------------------------+----------------------------------------------+

LCD 8080 Config
----------------------------------------

.. figure:: ../../../_static/lcd_8080_flow.png
    :align: center
    :alt: 880 lcd Overview
    :figclass: align-center

    Figure 1. 8080 lcd config flow

LCD RGB Config
----------------------------------------

.. figure:: ../../../_static/lcd_rgb_flow.png
    :align: center
    :alt: rgb lcd Overview
    :figclass: align-center

    Figure 2. rgb lcd config flow


API Reference
----------------------------------------
.. include:: ../../_build/inc/lcd.inc

API Typedefs
---------------------
.. include:: ../../_build/inc/lcd_disp_types.inc