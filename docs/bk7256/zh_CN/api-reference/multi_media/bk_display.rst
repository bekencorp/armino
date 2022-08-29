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
    1) 项目中支持的屏幕有Driving IC ST7282，HX8282, GC9503V.
    2) 接口支持的数据包括RGB565,YUYV,UYVY,YYUV,UVYY,VUYY

LCD API Categories
----------------------------

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

LCD parcical display 
----------------------------------------

.. figure:: ../../../_static/lcd_partical_display.png
    :align: center
    :alt: rgb lcd Overview
    :figclass: align-center

    Figure 3. rgb lcd config flow

局部显示参数配置

.. figure:: ../../../_static/partical_params_config.png
    :align: center
    :alt: rgb lcd Overview
    :figclass: align-center

    Figure 4. rgb lcd config flow

API Reference
----------------------------------------
.. include:: ../../_build/inc/lcd.inc

API Typedefs
-------------------------------------------
.. include:: ../../_build/inc/lcd_types.inc