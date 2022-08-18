LCD RGB Display JPEG
=================================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	lcd rgb display jpeg demo 实现了RGB屏幕实时显示dvp camera jpeg 数据的功能。

2、代码路径
--------------------------
	 - Demo 涉及的模块AP接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_display.html``
	
	 - Demo 具体的示例代码详见: ``\components\demos\media\lcd\lcd_rgb_demo.c``

LCD Demo 测试命令
--------------------------
demo支持的命令如下表:

+-------------------------------------------+------------------------+
|Command                                    |Description             |
+===========================================+========================+
|cpu1 lcd_video_jpeg_dec=clk_div,fps        |config clk div and pfs  |
+-------------------------------------------+------------------------+
|cpu1 lcd_rgb_close=jpeg_display            | close lcd              |
+-------------------------------------------+------------------------+

demo运行依赖的宏配置:

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

	1、Demo运行硬件环境:
	 1) BK7256开发板以及下载固件的环境
	 2) 配套RGB接口的LCD屏幕
	 3) 需要提供稳压电源供23V电压给RGB LCD
	 4) 可以接收串口命令的串口工具，波特率为115200.

	1.执行测试
	 - Uart发送AT指令 ``cpu1 lcd_video_jpeg_dec=15,20`` 执行测试

	3.停止测试
	 - Uart发送AT指令 ``cpu1 lcd_rgb_close=jpeg_display`` 停止测试


5、LCD 显示JPEG图像原理
--------------------------

JPEG数据不可以直接LCD显示，需要先经过解压缩，然后再显示。

.. figure:: ../../../../_static/lcd_disp_jpeg_simple.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 2. LCD display jpeg data


6、LCD 代码实现
--------------------------

LCD显示JPEG图像软件实现流程大致可以分为6个步骤：

	1)	配置RGB LCD

	2)	配置JPEG_ENC和camera

	3)	配置JPEG_DEC

	4)	处理JPEG_ENC完成一帧压缩的中断回调

	5)	处理JPEG_DEC完成一帧解压的中断回调

	6)	处理DMA搬数到LCD屏幕的中断回调

.. figure:: ../../../../_static/lcd_disp_jpeg.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 4. lcd jpeg display video

