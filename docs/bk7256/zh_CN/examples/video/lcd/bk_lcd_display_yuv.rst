LCD RGB Display YUV
=================================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	lcd rgb display yuv demo 实现了RGB屏幕实时显示dvp camera yuv 数据的功能。

2、代码路径
--------------------------
	 - Demo 涉及的模块AP接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_display.html``
	
	 - Demo 具体的示例代码详见: ``/components/demos/media/lcd/lcd_rgb/lcd_rgb``



+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_LCD                 |配置LCD功能                 |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_LCD_TEST            |LCD DEMO使能                |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+

demo运行依赖的库和驱动:
 - DMA DMA驱动
 - LCD LCD模块驱动
 - JPEG JPEG模块驱动

3、cli命令简介
--------------------
	命令针对480*272 LCD屏幕

	 1. RGB 屏幕刷红色
	 -  `` lcd_8080=dma2d_fill,0x60000000,0xf800``
	 -  `` rgb565_display=8``

	2、RGB屏幕显示dvp摄像头480*272 像素的图片
	 -  `` lcd_rgb_yuv=480p``

	3、RGB屏幕显示dvp摄像头640*480 像素的图片
	 -  `` lcd_rgb_yuv=480p,0,display_partical``

	4、RGB屏幕显示720p摄像头480*272 像素的图片
	 -  `` lcd_rgb_yuv=720p,0,display_partical``

	5、RGB屏幕显示dvp摄像头640*480 像素的图片
	 -  `` lcd_rgb_yuv=480p,0,display_partical``


	2.停止测试
	 - Uart发送AT指令 `` lcd_close=yuv`` 停止测试



4、LCD 代码实现
--------------------------

LCD显示YUV图像软件实现流程大致可以分为4个步骤：

	1)	配置RGB LCD

	2)	配置JPEG_ENC和camera

	3)	处理JPEG_ENC完成一帧压缩的中断回调

	4)	配置LCD 刷屏的数据地址，并使能传输

