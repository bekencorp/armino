
MEDIA Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证MEDIA的相关功能，本文档汇集了所用到的CLI CMD。所有的MEDIA CLI命令以 ``media`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。
	
	当前命令是否生效是由宏 ``CONFIG_MEDIA`` 控制。


2、代码路径
--------------------------
	路径: ``\components\media\cli\media_cli.c``

3、cli命令简介
--------------------------

支持的命令如下:

	1. media dvp open
	 - 功能说明
		打开dvp摄像头相关测试
	 - 命令格式
		media dvp open [param1] [param2]

		+-----------+------------------------------------------------------------------------+
		| param1    | The camera output resolution, this can omit for 640X480                |
		|           |  Value: 320X240, 480X272, 1280X720, etc.                               |
		+-----------+------------------------------------------------------------------------+
		|           | The dvp data output tpye, this can omit for MIX                        |
		|           | Value: yuv for YUYV                                                    |
		| param2    | mix for MIX order jpg-YUYV-jpeg-                                       |
		|           | jpeg for JPEG                                                          |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 打开dvp，并输出640X480，mix模式测试：``media dvp open 320X480``

	2. media dvp close
	 - 功能说明
		关闭dvp摄像头测试
	 - 命令格式
		media dvp close

		此命令没有参数。
	 - 测试用例
		``media dvp close``

	3. media uvc open
	 - 功能说明
		打开uvc摄像头相关测试
	 - 命令格式
		media uvc open [param1] [param2]

		+-----------+------------------------------------------------------------------------+
		| param1    | The camera output data type, this can omit for MJPEG                   |
		|           |  Value: MJPEG H264.                                                    |
		+-----------+------------------------------------------------------------------------+
		| param2    | The camera output resolution, this can omit for 640X480                |
		|           |  Value: 320X240, 480X272, 1280X720, etc.                               |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 打开uvc，并输出640X480测试：``media uvc open 800X480``

	4. media uvc close
	 - 功能说明
		关闭uvc摄像头测试
	 - 命令格式
		media uvc close

		此命令没有参数。
	 - 测试用例
		``media uvc close``

	5. media lcd open
	 - 功能说明
		打开lcd相关测试
	 - 命令格式
		media lcd open [param1] [param2] [param3]

		+-----------+------------------------------------------------------------------------+
		| param1    | The LCD display resolution, defult by value 480X272                    |
		|           |  Value: 320X480, 1024X600, etc.                                        |
		+-----------+------------------------------------------------------------------------+
		| param2    | The LCD name for diaplay, defult by value st7282                     |
		|           |  Value: hx8282, st7282 etc.                                            |
		+-----------+------------------------------------------------------------------------+
		| param3    | Preamble or not, omit for not need rotate                              |
		|           |  Value: rotate                                                         |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 打开lcd显示，屏幕输出分辨率为480X800，且需要旋转，测试：``media lcd open 480X800 nt35512 rotate``



.. Attention::

	注意: SDK中适配了多种屏幕，如果有多个屏幕分辨率相同，必须传入名字;
	若SDK中屏幕分辨率唯一，传入分辨率或名字之一即可。


6. media lcd dma2d_blend
	 - 功能说明
		打开lcd小图标融合相关测试
	 - 命令格式
		media lcd dma2d_blend [param1] [param2] 

		+-----------+--------------------------------------------------------------------------------+
		| param1    | display blend clock, wifi or version                                           |
		|           |  Value: clock, wifi, ver.                                                      |
		+-----------+--------------------------------------------------------------------------------+
		| param2    | if param1 is clock, this param2 is such as 12:00, if param1 is wifi,           |
		|           | param2 is wifi lever(0-4),if param1 is ver, the param2 is omit, display defult |
		+-----------+--------------------------------------------------------------------------------+


	 - 测试用例
		| 打开lcd显示后，发送融合命令：``media lcd open 480X800 nt35512 rotate`` 
		`` media lcd dma2d_blend clock 15:19`` 或 `` media lcd dma2d_blend wifi 1`` 

	7. media lcd dma2d_blend close
	 - 功能说明
		关闭lcd小图标融合相关测试
	 - 命令格式
		media lcd dma2d_blend [param1] [param2] 

		+-----------+--------------------------------------------------------------------------------+
		| param1    | close .if no use param2 will close all, include clock, wifi version            |
		|           |  Value: close.                                                                 |
		+-----------+--------------------------------------------------------------------------------+
		| param2    | if param1 is close, this param2 decided close which module                     |
		|           |  Value: wifi, clock, ver.                                                      |
		+-----------+--------------------------------------------------------------------------------+


	 - 测试用例
		| 关闭所有的融合： ``media lcd dma2d_blend close`` 
		| 关闭的时间融合： ``media lcd dma2d_blend close clock`` 

	8. media lcd close
	 - 功能说明
		关闭lcd显示测试
	 - 命令格式
		media lcd close

		此命令没有参数。
	 - 测试用例
		``media lcd close``

	9. media capture
	 - 功能说明
		测试拍照功能，将摄像头一帧图像保存到SD卡中，必须要先打开摄像头才能工作正常
	 - 命令格式
		media capture param1

		+-----------+------------------------------------------------------------------------+
		| param1    | The capture name, such as "test.jpg"                                   |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 拍照测试：``media capture 1.jpg``

	10. media save_start
	 - 功能说明
		将摄像头中的数据连续存储到SD卡中的一个文件中,每张图片以空格分隔。
	 - 命令格式
		media save_start param1

		+-----------+------------------------------------------------------------------------+
		| param1    | The storage file name, such as "test.264"                              |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 拍照测试：``media start_save frame.264``

	11. media save_stop
	 - 功能说明
		停止将摄像头数据存入SD卡中
	 - 命令格式
		media save_stop

		此命令没有参数。
	 - 测试用例
		``media save_stop``