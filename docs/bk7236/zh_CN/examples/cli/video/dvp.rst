DVP Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证video_transfer(dvp)的相关功能，为此设计了一系列的CLI CMD。所有的DVP CLI命令以 ``dvp`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。
	当前命令是否生效是由宏 ``CLI_CFG_DVP``，位置：``components\bk_cli\cli_config.h``。


2、代码路径
--------------------------
	路径: ``\components\demo\media\video\dvp_capture\dvp_capture.c``

3、cli命令简介
--------------------------
支持的命令如下:
	1.dvp set_cfg
	 - 功能说明
		设置当前使用的dvp摄像头类型，以及需要输出的分辨率和帧率，在初始化dvp成功之前调用。
	 - 命令格式
		dvp set_cfg param1,param2,param3

		+-----------+------------------------------------------------------------------------+
		|param1     | Camera type, reference ``components\dvp_camera_types.h``               |
		|           |  Range: 0 to 9, you can add by self if add other camera                |
		+-----------+------------------------------------------------------------------------+
		|param2     | Camera resolution, reference ``camera_intf_set_sener_cfg()``           |
		|           |  Range: 240: 320X240, 480: 640X480, 600: 800X600, 720: 1280X720, etc   |
		+-----------+------------------------------------------------------------------------+
		|param3     | Camera frame_rate                                                      |
		|           |  Range: 5 to 30, need different config for different frame_rate        |
		+-----------+------------------------------------------------------------------------+
	 - 测试用例
		| 使用GC0328C参数：``dvp set_cfg 3 480 25``
		| 使用HM1055参数： ``dvp set_cfg 7 720 20``
		| 使用GC2145参数： ``dvp set_cfg 7 720 20``

	2.dvp init
	 - 功能说明
		初始化当前dvp摄像头，并进行硬件压缩编码
	 - 命令格式
		dvp init

		此命令没有参数。
	 - 测试用例
		``dvp init``

	3.dvp deinit
	 - 功能说明
		关闭当前使用的摄像头，在初始化dvp成功之后调用。
	 - 命令格式
		dvp deinit

		此命令没有参数。
	 - 测试用例
		``dvp deinit``

	4.dvp read
	 - 功能说明
		读取压缩的数据到sram中，在初始化dvp成功之后调用。
	 - 命令格式
		dvp read param1

		+-----------+------------------------------------------------------------------------+
		|param1     | buffer length, unit byte                                               |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		``dvp read 40960``

	5.dvp capture
	 - 功能说明
		拍照功能，保存一张图像
	 - 命令格式
		dvp capture param1

		+-----------+------------------------------------------------------------------------+
		|param1     | capture name id                                                        |
		|           |  Range: 0-255                                                          |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		``dvp capture 1``

	6.dvp capture
	 - 功能说明
		拍照功能，保存一张图像
	 - 命令格式
		dvp capture param1

		+-----------+------------------------------------------------------------------------+
		|param1     | capture name id                                                        |
		|           |  Range: 0-255                                                          |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		``dvp capture 1``

	7.dvp auto_encode
	 - 功能说明
		设置jpeg自动压缩使能和压缩大小，在初始化dvp成功之后调用。
	 - 命令格式
		dvp auto_encode param1 param2 param3

		+-----------+------------------------------------------------------------------------+
		|param1     | Enable/diisable jpeg auto_encode                                       |
		|           |  Range: 1/0                                                            |
		+-----------+------------------------------------------------------------------------+
		|param2     | Set jpeg auto_encode output size upper limit                           |
		|           |  Range: 0-200, unit kbyte                                              |
		+-----------+------------------------------------------------------------------------+
		|param3     | Set jpeg auto_encode output size lower limit                           |
		|           |  Range: 0-200, unit kbyte                                              |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		``dvp auto_encode 1 10 40``

	8.dvp dump
	 - 功能说明
		通过I2C读取dvp sensor内部寄存器的值
	 - 命令格式
		dvp dump

		此命令没有参数，在初始化dvp成功之后调用。
	 - 测试用例
		``dvp dump``