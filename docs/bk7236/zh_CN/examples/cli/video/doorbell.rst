DOORBELL Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证门铃的相关功能，为此设计了一系列的CLI CMD。所有的DVP CLI命令以 ``doorbell`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。
	当前命令是否生效是由宏 ``CONFIG_DOORBELL``。


2、代码路径
--------------------------
	路径: ``\components\demos\media\doorbell\src\doorbell_core.c``

3、cli命令简介
--------------------------
支持的命令如下:
	1.doorbell tcp
	 - 功能说明
		初始化tcp传输，将多媒体音视频数据，通过网络进行传输
	 - 命令格式
		doorbell tcp

		此命令没有参数。
	 - 测试用例
		``doorbell tcp``

	2.doorbell udp
	 - 功能说明
		初始化udp传输，将多媒体音视频数据，通过网络进行传输
	 - 命令格式
		doorbell tcp

		此命令没有参数。
	 - 测试用例
		``doorbell udp``

	3.doorbell udp_c
	 - 功能说明
		将当前设备作为UDP通信的client端，负责将音视频数据发送给slave端。作为测试双向对讲功能进行测试
	 - 命令格式
		doorbell udp_c param1 param2

		+-----------+------------------------------------------------------------------------+
		|param1     | Camera type, open camera type                                          |
		|           |  Range: dvp, uvc                                                       |
		+-----------+------------------------------------------------------------------------+
		|param2     | Camera resolution, reference ``camera_intf_set_sener_cfg()``           |
		|           |  Range: 240: 320X240, 480: 640X480, 600: 800X600, 720: 1280X720, etc   |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 作为udp_client传输，打开dvp摄像头，且配置分辨率为640X480： ``doorbell udp_c dvp 640X480``

	4.doorbell udp_s
	 - 功能说明
		将当前设备作为UDP通信的slave端，负责将音频数据发送给client端，接收client端的音视频数据，并播放和LCD显示，作为测试双向对讲功能进行测试。
	 - 命令格式
		doorbell udp_s param1

		+-----------+------------------------------------------------------------------------+
		|param1     | LCD resolution type, open LCD display                                  |
		|           |  Range: 480X272, 320X480, 1024X600. etc                                |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 作为udp_slave传输，打开LCD屏幕显示，且配置分辨率为480X272： ``doorbell udp_s 480X272``