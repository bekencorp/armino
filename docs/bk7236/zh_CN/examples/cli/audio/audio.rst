AUDIO Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证audio相关的功能，为此设计了一系列cli CMD，同时也是audio的各功能场景demo，供客户参考使用。



2、代码路径
--------------------------
	路径: ``components\demos\media\audio\aud_intf\demo\aud_intf_demo.c``

3、cli命令简介
--------------------------
支持的命令如下:

	1.aud_intf_record_test
	 - 功能说明
		用来测试录音到文件功能，包括开始录音、暂停录音、结束录音等。
	 - 命令格式
		aud_intf_record_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | init : Init record                                                  |
		|           |  | start : Start record                                                |
		|           |  | pause : Pause record                                                |
		|           |  | stop : Stop record                                                  |
		|           |  | deinit : Deinit record                                              |
		+-----------+------------------------------------------------------------------------+
		|param2     | file name                                                              |
		|           |  format: xxx.pcm                                                       |
		+-----------+------------------------------------------------------------------------+
	 - 测试用例
		| 初始化录音：``aud_intf_record_test init record.pcm``
		| 开始录音：``aud_intf_record_test start record.pcm``
		| 暂停录音：``aud_intf_record_test pause record.pcm``
		| 停止录音：``aud_intf_record_test stop record.pcm``
		| 结束录音：``aud_intf_record_test deinit record.pcm``

	2.aud_intf_play_test
	 - 功能说明
		用来测试播放音频文件功能，包括开始播放、暂停播放、结束播放等。
	 - 命令格式
		aud_intf_play_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | init : Init play                                                    |
		|           |  | start : Start play                                                  |
		|           |  | pause : Pause play                                                  |
		|           |  | stop : Stop play                                                    |
		|           |  | deinit : Deinit play                                                |
		+-----------+------------------------------------------------------------------------+
		|param2     | file name                                                              |
		|           |  format: xxx.pcm                                                       |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 初始化播放：``aud_intf_play_test init record.pcm``
		| 开始播放：``aud_intf_play_test start record.pcm``
		| 暂停播放：``aud_intf_play_test pause record.pcm``
		| 停止播放：``aud_intf_play_test stop record.pcm``
		| 结束播放：``aud_intf_play_test deinit record.pcm``

	3.aud_intf_loop_test
	 - 功能说明
		通过软件自回环（将mic采集的数据送至speaker播放）来测试mic和speaker功能是否正常。
	 - 命令格式
		aud_intf_loop_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start audio loop test                                       |
		|           |  | stop : Stop audio loop test                                         |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 开始自回环测试：``aud_intf_loop_test start``
		| 停止自回环测试：``aud_intf_loop_test stop``
