VIDEO AT CMD
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证video的相关功能，为此设计了一系列的AT CMD。所有的VIDEO AT命令以 ``AT+VIDEO`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。


2、代码路径
--------------------------
	路径: ``\components\at\src\video_at_command.c``

3、cli命令简介
--------------------------
支持的命令如下:
	1.AT+VIDEO=SETYUV
	 - 功能说明
		测试硬件JPEG模块和摄像头的功能正常与否
	 - 命令格式
		AT+VIDEO=SETYUV,param1

		+-----------+------------------------------------------------------------------------+
		|           | JPEG work mode: 0/1                                                    |
		| param1    | 0: work for jpeg encode mode                                           |
		|           | 1: work for jpeg yuv mode                                              |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| dvp jpeg模式测试：``AT+VIDEO=SETYUV,0``
		| dvp yuv模式测试：``AT+VIDEO=SETYUV,1``

	2.AT+VIDEO=CLOSEYUV
	 - 功能说明
		关闭摄像头
	 - 命令格式
		AT+VIDEO=CLOSEYUV

		此命令没有参数。
	 - 测试用例
		``AT+VIDEO=CLOSEYUV``

	3.AT+VIDEO=PSRAMENABLE
	 - 功能说明
		使能/关闭 PSRAM的功能
	 - 命令格式
		AT+VIDEO=PSRAMENABLE,param1

		+-----------+------------------------------------------------------------------------+
		|param1     | psram enable/disable: 1/0                                              |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 打开PSRAM测试：``AT+VIDEO=PSRAMENABLE,1``

	4.AT+VIDEO=PSRAMREAD
	 - 功能说明
		psram读写测试
	 - 命令格式
		AT+VIDEO=PSRAMREAD

		此命令没有参数。
	 - 测试用例
		``AT+VIDEO=PSRAMREAD``

	5.AT+VIDEO=LCDENABLE
	 - 功能说明
		打开LCD显示功能
	 - 命令格式
		AT+VIDEO=LCDENABLE,param1

		+-----------+------------------------------------------------------------------------+
		|param1     | LCD enable/disable: 1/0                                                |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 打开LCD显示功能：``AT+VIDEO=LCDENABLE,1``

	6.AT+VIDEO=MAILBOX
	 - 功能说明
		测试多媒体的Mailbox video通道是否畅通
	 - 命令格式
		AT+VIDEO=MAILBOX

		此命令没有参数。
	 - 测试用例
		``AT+VIDEO=MAILBOX``