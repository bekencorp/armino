SBC_DEC Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证SBC或者MSBC硬件解码的功能，为此设计了一个cli CMD，设计思路是从SDCARD读取sbc或者msbc文件进行解码并播放，当前cli命令是否生效由宏 ``CLI_CFG_SBC`` 决定，宏控制位置在：``components\bk_cli\cli_config.h`` 文件中，该宏是否生效由内部宏 ``CONFIG_SBC_TEST`` 决定。


2、代码路径
--------------------------
	路径：``components\demos\media\audio\sbc_decode\sbc_decode_demo.c``

3、cli命令简介
--------------------------
支持的命令如下:

	1.sbc_decoder_test
	 - 功能说明
		用来测试SBC或者MSBC硬件解码并实时播放的功能。
	 - 命令格式
		sbc_decoder_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |  | start：开始                                                         |
		|           |  | stop ：停止                                                         |
		+-----------+------------------------------------------------------------------------+
		|param2     | sbc file                                                               |
		|           |  格式: xxx.sbc                                                         |
		+-----------+------------------------------------------------------------------------+
	 - 测试用例
		| 开始解码并存储PCM文件：``sbc_decoder_test start test.sbc``
		| 关闭测试文件并释放资源：``sbc_decoder_test stop test.sbc``
