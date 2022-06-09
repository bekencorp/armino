Lcd_display(Camera_lcd_display)
=================================

:link_to_translation:`en:[English]`

概述
--------------------------
	lcd displey video 主要是用LCD显示图像的功能，也可以显示不同类型的camera采集的视频。

LCD demo 代码
--------------------------

	 - LCD Demo涉及的模块API接口的详细说明请参考同网页/api-reference/multi_media目录。
	
	 - LCD Demo 具体的示例代码详见components/demos/media/lcd目录。
	
	 - LCD Demo 功能会用到DMA2D功能，DMA2D代码使用详见components/demos/media/dma2d目录。

LCD Demo 测试命令
--------------------------
	LCD Demo 主要包括三个示例

	1、8080 接口的LCD显示

		 - lcd_8080_init=start
		 - lcd_fill=x,y,width,hight,color
		 - 示例: lcd_fill=0,0,320,480,0xf800 功能为刷全屏为红色。

	2、RGB 接口显示DVP camera YUV数据的视频

		 - 需要连接DVP摄像头
		 - lcd_video=clk_div, 帧率
		 - 示例: lcd_video=9,25  clk_div是在96M系统下的分频。

	3、RGB 接口显示DVP camera JPEG数据的视频

		 - 需要连接DVP摄像头
		 - lcd_video_jpeg_dec=clk_div, 帧率
		 - 示例: lcd_video_jpeg_dec=9,25  clk_div是在96M系统下的分频。

video显示硬件环境
--------------------------

	1、Demo运行硬件环境:
	 1) BK7256开发板以及下载固件的环境
	 2) 配套8080 接口或RGB接口的LCD屏幕
	 3) 如果是RGB屏幕，需要提供 稳压电源供23V电压给RGB LCD
	 4) 可以接收串口命令的串口工具，波特率为115200.

	2、Video功能是一个集多个模块功能的应用场景，demo后续根据不同的应用场景，可能用到的硬件模块包括：
	 - LCD_Display
	 - JPEG_DEC
	 - JPEG_ENC
	 - DVP camera(GC0308C_DEV)
	 - UVC（USB Camera）
	 - I2c
	 - DMA
	 - USB

关于DVP 和UVC支持的camera类型请参考"样例演示->DVP_Camera及配置"


LCD 显示原理
--------------------------

1、LCD 显示YUV图像原理


YUV数据是直接可以被LCD显示的数据类型，jpeg输出配置为YUV模式输出，将直接将camera采集的YUV数据保存到内存中。

.. figure:: ../../../_static/lcd_disp_yuv_simple.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 1. LCD display YUV data

2、LCD 显示JPEG图像原理


JPEG数据不可以直接LCD显示，需要先经过解压缩，然后再显示。

.. figure:: ../../../_static/lcd_disp_jpeg_simple.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 2. LCD display jpeg data


LCD 代码实现
--------------------------

1、LCD显示YUV图像软件实现


显示流程大致可以分为4个步骤：

	1)	配置RGB LCD

	2)	配置JPEG_ENC和camera

	3)	处理JPEG_ENC完成一帧压缩的中断回调

	4)	处理DMA搬数到LCD屏幕的中断回调

.. figure:: ../../../_static/lcd_disp_yuv.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 3. lcd yuv display video


2、LCD显示JPEG图像软件实现

显示流程大致可以分为6个步骤：

	1)	配置RGB LCD

	2)	配置JPEG_ENC和camera

	3)	配置JPEG_DEC

	4)	处理JPEG_ENC完成一帧压缩的中断回调

	5)	处理JPEG_DEC完成一帧解压的中断回调

	6)	处理DMA搬数到LCD屏幕的中断回调

.. figure:: ../../../_static/lcd_disp_jpeg.png
    :align: center
    :alt: 
    :figclass: align-center

    Figure 4. lcd jpeg display video

