AEC Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证AEC回声消除相关的功能，为此设计了一系列cli CMD，同时也是回声消除功能场景demo，供客户参考使用。


2、代码路径
--------------------------
	路径: ``components\demos\media\audio\aec\aec_demo.c``

3、cli命令简介
--------------------------
支持的命令如下:

	1.aec_test
	 - 功能说明
		用来测试回声消除功能。
	 - 命令格式
		aec_test

	 - 测试用例
		对TF卡中的 ``mic.pcm和ref.pcm`` 进行回声消除，并输出至 ``out.pcm`` ：``aec_test``
