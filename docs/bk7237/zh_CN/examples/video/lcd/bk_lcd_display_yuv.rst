LCD RGB Display YUV
=================================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	lcd rgb displey yuv demo 实现了RGB屏幕实时显示dvp camera yuv 数据的功能。

2、代码路径
--------------------------
	 - Demo 涉及的模块AP接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_display.html``
	
	 - Demo 具体的示例代码详见: ``/components/demos/media/lcd/lcd_rgb/lcd_rgb_demo.c``
	
	 - Demo 功能会用到DMA2D功能，DMA2D代码使用详见: ``/components/demos/media/dma2d``

3、cli命令简介
--------------------

demo支持的命令如下表:

+-------------------------------------------+------------------------+
|Command                                    |Description             |
+===========================================+========================+
|cpu1 lcd_video=clk_div,帧率                |配置clk div和帧率       |
+-------------------------------------------+------------------------+
|cpu1 lcd_rgb_close=yuv_display             |关闭屏幕                |
+-------------------------------------------+------------------------+


+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_LCD                 |配置LCD功能                 |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_LCD_TEST            |LCD DEMO使能                |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+

demo运行依赖的库和驱动:
 - DMA DMA驱动
 - LCD LCD模块驱动
 - JPEG JPEG模块驱动

4、演示介绍
--------------------------

demo执行的步骤如下:

	1.执行测试
	 - Uart发送AT指令 ``cpu1 lcd_video=9,25`` 执行测试

	2.停止测试
	 - Uart发送AT指令 ``cpu1 lcd_rgb_close=yuv_display`` 停止测试


1、LCD 显示YUV图像原理

YUV数据是直接可以被LCD显示的数据类型，jpeg输出配置为YUV模式输出，将直接将camera采集的YUV数据保存到内存中。

.. figure:: ../../../../_static/lcd_disp_yuv_simple.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 1. LCD display YUV data


6、LCD 代码实现
--------------------------

LCD显示YUV图像软件实现流程大致可以分为4个步骤：

	1)	配置RGB LCD

	2)	配置JPEG_ENC和camera

	3)	处理JPEG_ENC完成一帧压缩的中断回调

	4)	处理DMA搬数到LCD屏幕的中断回调

.. figure:: ../../../../_static/lcd_disp_yuv.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 3. lcd yuv display video

