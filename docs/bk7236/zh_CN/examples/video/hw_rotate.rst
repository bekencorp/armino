Rotate
=================================

:link_to_translation:`en:[English]`

1、功能概述
--------------------
	rott_demo.c 展示了 硬件rotate/pfc 接口的API使用

2、代码路径
--------------------------
	 - Demo 涉及的模块AP接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_hw_rotate.html``
	 - Demo 具体的示例代码详见: ``\components\demos\media\rott\rott_demo.c``
	 - Demo 验证功能会用到Display功能,Display代码使用详见: ``/components/demos/media/lcd``


demo运行依赖的宏配置:

+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_HW_ROTATE_PFC       |配置Rotate功能              |``\middleware\soc\bk7236\bk7236.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_HW_ROTT_TEST        |Rotate DEMO使能             |``\middleware\soc\bk7236\bk7236.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+

demo运行依赖的其他库和驱动:

 - LCD_Display LCD模块驱动

3、cli命令简介
--------------------
下面两个宏打开，可以使用存储在flash中的YUV数据：
	 - #define YUYV_640_480   0
	 - #define VUYY_640_480   0

但是可能flash空间不足,用户也可以使用SD卡将YUV数据读到psram地址中。

	1、Demo运行硬件环境:
	 - BK7236开发板以及下载固件的环境
	 - 480x272 LCD(或其他屏幕)
	 - 可以接收串口命令的串口工具，波特率为115200.

	2、yuv转RGB565,并显示到屏幕
	 - rott=init
	 - rott=bypass,vuyy
	 - rott=lcd_init,rgb565le,640,480
	 - rott=lcd_disp, 0x60000000,1

	3、yuv转RGB565，并旋转，显示到LCD
	 - rott=lcd_disp, 0x60000000,0
	 - rott=rotate, 90, vuyy
	 - rott=lcd_init,rgb565, 480, 640
	 - rott=lcd_disp, 0x60000000,1


