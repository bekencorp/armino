声网Agora-iot-sdk demo
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------
	基于声网的Agora-iot-sdk连接声网云，实现设备和手机语音通话及设备至手机的单向图传。

2、代码路径
--------------------
	demo路径: ``\components\demos\media\agora``

	Agora-iot-sdk API接口的详细说明请参考源文件: ``\components\bk_thirdparty\agora-iot-sdk\include\agora_rtc_api.h``

3、cli命令简介
--------------------
demo支持的命令如下表:

+-----------------------------------------------------------+----------------------+
|Command                                                    |Description           |
+===========================================================+======================+
|agora_test {audio|video start|stop appid}                  |控制语音和图传        |
+-----------------------------------------------------------+----------------------+

demo编译指令： ``make bk7256 PROJECT=thirdparty/agora``


4、演示介绍
--------------------

demo执行的准备工作如下:
	1.准备可以连接外网的安卓机，并安装用于测试的APK ``Open Live``
	2.去Agora官网注册申请用于测试的 ``APPID`` ，详细操作见下文参考资料
	3.准备可以访问外网的2.4GHz热点，供demo板连接使用

demo执行的步骤如下:

	1.手机APK配置
	 - 打开 ``Open Live`` apk，输入注册申请的 ``APPID`` , 点击 ``ENTER`` 进入
	 - 输入channel name ``hello_bk7256demo``,点击JOIN ROOM进入，选择 ``BROADCASTER``

	2.Demo板wifi连接
	 - demo板发送指令 ``sta test xxxxxx`` 连接2.4GHz名为test的热点

	3.打开语音通话或图传
	 - demo板发送指令 ``agora_test audio start appid`` 打开语音通话，其中appid是注册申请的appid
	 - demo板发送指令 ``agora_test voideo start appid`` 打开图传，其中appid是注册申请的appid

	4.关闭语音通话或图传
	 - demo板发送指令 ``agora_test audio stop appid`` 关闭语音通话，其中appid是注册申请的appid
	 - demo板发送指令 ``agora_test voideo stop appid`` 关闭图传，其中appid是注册申请的appid

5、参考资料
--------------------
	声网参考文档：https://docs.agora.io/cn/Agora%20Platform/manage_projects?platform=Android

	声网APPID申请链接：https://sso2.agora.io/cn/v5/login?_gl=1%2ardr355%2a_ga%2aMzkyNDM4ODYyLjE2NzM1MTM3MTU.%2a_ga_BFVGG7E02W%2aMTY3ODg1MjM0My4xMi4wLjE2Nzg4NTIzNDYuMC4wLjA.

	APK下载链接：http://dl.bekencorp.com/apk/shengwang/OpenLive_input_appid.apk
