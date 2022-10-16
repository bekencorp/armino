DOORBELL
==========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	门铃的主要功能是联合多个模块，实时的进行图像采集，屏幕显示，声音的采集和通信。

2 代码路径
-------------------------------------
	demo路径：``./components/media/cli/media_cli.c``, ``./components/media/camera/camera_act.c``, ``./components/media/transfer/transfer_act.c``, ``./components/media/lcd/lcd_act.c``

3 cli命令简介
-------------------------------------
	demo支持的命令如下表：

	+----------------------------------------+--------------------------------+---------------------------------------+
	|             Command                    |            Param               |              Description              |
	+========================================+================================+=======================================+
	|                                        | param1:softap_ssid             | setup a softap and set name           |
	| ap param1 param2                       +--------------------------------+---------------------------------------+
	|                                        | param2:softap_key              | set softap connect key                |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:station_ssid            | connect a station and set name        |
	| sta param1 param2                      +--------------------------------+---------------------------------------+
	|                                        | param2:station_key             | set station connect key               |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:3                       | open doorbell function                |
	| doorbell param1 [param2]               +--------------------------------+---------------------------------------+
	|                                        | param2:if open uvc, choosable  | set open/disopen uvc 1/0              |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media dvp open [param]                 | param:dvp resolution, choosable| set sensor resolution, and open dvp   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media dvp close                        | NULL                           | close dvp sensor                      |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media lcd open [param]                 | param1:display size            | lcd display size, default:480X272     |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media lcd close                        | NULL                           | close lcd function, and jpeg decode   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media capture param                    | param:xxx.jpg                  | capture save to sdcard, and set name  |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media uvc open [param]                 | param:uvc resolution, choosable| set sensor resolution, and open uvc   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media uvc close                        | NULL                           | close uvc sensor                      |
	+----------------------------------------+--------------------------------+---------------------------------------+

	demo运行依赖的宏配置：

	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description       |                  File                      |  value  |
	+======================================+========================+============================================+=========+
	|CONFIG_DOORBELL                       |support DOORBELL        |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_JPEG_ENCODE                    |suport hw jpeg ecoder   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA                         |suport use dvp camera   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA_USE_I2C1                |suport use I2C1         |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_AUDIO                          |support audio function  |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_LCD                            |support LCD function    |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_USB_UVC                        |support UVC camera      |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+

4 演示介绍
-------------------------------------
	demo执行的步骤如下：

	1、准备好dvp摄像头或者UVC摄像头，LCD屏幕(rgb或者8080):

	2、起一个softap:

		ap doorbell_test 1234567890

	3、手机连接这个ap

	4、发送doorbell命令:

		doorbell

	5、手机app(RealtimeVideo)设置顺序和流程如下图所示：

	6、如果需要显示到LCD屏幕上

		media lcd open

5 app使用流程
--------------------------
	以下流程以板子作为softap为例，介绍apk的使用，在手机连接到板子使能的ap后：

	1.图1为app示意图

	2.图2为app的主界面

		其中，选择1可以看到图3

		- 1：设置菜单；
		- 2：更新apk和回退apk菜单；

	3.图3为设置界面菜单

		其中：

		- 1：为设置界面如图4所示；
		- 2：为返回主界面按钮；

	4.图4为真正的设置界面

		其中，设置说明如下，设置完成后返回图2主界面

		- 1：设置解决方案，当前支持video_transfer和doorbell，此处选择doorbell；
		- 2：设置数据传输模式，当前支持UDP和TCP，默认选择UDP；
		- 3：设置对端的ip地址，ap模式下默认为``192.168.0.1``不用修改，sta模式下设置为对端的ip地址；
		- 4：设置摄像头类型，当前支持DVP和UVC，根据自己使用摄像头类型进行设置；
		- 5：设置摄像头的输出分辨率；
		- 6：设置LCD输出的分辨率，根据自己使用的LCD屏幕分辨率进行设置；

	5.图5为功能使能设置界面

		其中，功能使能说明如下所示：

		- 1：开关视频图传；
		- 2：开关语音；
		- 3：拍照开关，当前暂不支持；
		- 4：开关LCD屏幕显示；

.. note::

	在图4中设置对端IP地址，当板子为softap时，默认为``192.168.0.1``，当板子作为staion时，手机和板子连接同一个ap，填入的IP地址可以通过命令``ip``获得。
	另外该app还支持手机图传功能，即命令``video_transfer -a|s ssid key``，但是必须将图4步骤1设置为``video_transfer``模式。

.. figure:: ../../../../../common/_static/RealtimeVideo_app.jpg
    :align: center
    :alt: RealtimeVideo_app
    :figclass: align-center

    Figure 1. doorbell apk

.. figure:: ../../../../../common/_static/app_set0.jpg
    :align: center
    :alt: RealtimeVideo_app_screen
    :figclass: align-center

    Figure 2. RealtimeVideo_app Main screen

.. figure:: ../../../../../common/_static/app_set1.jpg
    :align: center
    :alt: RealtimeVideo_app_set_menu
    :figclass: align-center

    Figure 3. RealtimeVideo_app Set menu

.. figure:: ../../../../../common/_static/app_set2.jpg
    :align: center
    :alt: RealtimeVideo_app_set
    :figclass: align-center

    Figure 4. RealtimeVideo_app set

.. figure:: ../../../../../common/_static/app_set3.jpg
    :align: center
    :alt: RealtimeVideo_function_set
    :figclass: align-center

    Figure 5. RealtimeVideo_function set