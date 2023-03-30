PSRAM Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证PSRAM的相关功能，为此设计了一系列的CLI CMD。所有的DVP CLI命令以 ``psram_test`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。
	当前命令是否生效是由宏 ``CLI_CFG_PSRAM``, 路径：``components\bk_cli\cli_config.h``。


2、代码路径
--------------------------
	路径: ``\components\bk_cli\cli_psram.c``

3、cli命令简介
--------------------------
支持的命令如下:
	1.psram_test start
	 - 功能说明
		开始进行psram测试
	 - 命令格式
		psram_test start param1 param2

		+-----------+------------------------------------------------------------------------+
		|           | dtcm: use cpu to write and read                                        |
		| param1    | conexist: when doorbell is woorking and process other psram            |
		|           | continue_write: sram and psram process test                            |
		+-----------+------------------------------------------------------------------------+
		| param2    | 1: psram start 0x64000000                                              |
		|           | 0: psram start 0x60000000                                              |
		+-----------+------------------------------------------------------------------------+
		| param3    | every time read/write delay time, unit ms                              |
		|           |  Range: 0-100000                                                       |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| psram常规测试：``psram_test start dtcm 0 500``

	2.psram_test stop
	 - 功能说明
		停止psram测试
	 - 命令格式
		psram_test stop

		此命令没有参数。
	 - 测试用例
		``psram_test stop``

	3.psram_test clk
	 - 功能说明
		设置PSAM工作的时钟
	 - 命令格式
		psram_test clk

		+-----------+------------------------------------------------------------------------+
		|param1     | psram work clk: 60/120/160/240                                         |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| psram设置120MHz：``psram_test clk 120``

	4.psram_test init
	 - 功能说明
		初始化psram
	 - 命令格式
		psram_test init

		此命令没有参数。
	 - 测试用例
		``psram_test init``

	5.psram_test deinit
	 - 功能说明
		close psram功能
	 - 命令格式
		psram_test deinit

		此命令没有参数。
	 - 测试用例
		``psram_test deinit``

	6.psram_test strcat
	 - 功能说明
		将数据连接到预设psram地址后面功能
	 - 命令格式
		psram_test strcat param1

		+-----------+------------------------------------------------------------------------+
		|param1     | data you want cat after sram_buffe                                     |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		``psram_test strcat 1234455678888757476``