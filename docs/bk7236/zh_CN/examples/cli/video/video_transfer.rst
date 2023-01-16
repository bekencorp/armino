VIDEO_TRANSFER Cli
=======================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证video_transfer的相关功能，为此设计了一系列的CLI CMD。所有的图传CLI命令以 ``video_transfer`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。
	当前命令是否生效是由宏 ``CLI_CFG_DVP`` 和 ``CONFIG_NET_WORK_VIDEO_TRANSFER`` 控制, 路径：``components\bk_cli\cli_config.h``。


2、代码路径
--------------------------
	路径: ``\components\app\net_work\video_demo_main.c``

3、cli命令简介
--------------------------
支持的命令如下:
	1.video_transfer
	 - 功能说明
		开始进行图传测试
	 - 命令格式
		video_transfer param1 param2 param3

		+-----------+------------------------------------------------------------------------+
		|           | video transfer ways:                                                   |
		| param1    | -s: current device as station, connect a softap                        |
		|           | -a: current device as softap, peer device connect this softap          |
		+-----------+------------------------------------------------------------------------+
		| param2    | ssid: the connect device name                                          |
		+-----------+------------------------------------------------------------------------+
		| param3    | key: password, this can omit                                           |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 图传常规测试：``video_transfer -a demo 12345678``
