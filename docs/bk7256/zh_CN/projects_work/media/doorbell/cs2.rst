尚云P2P
========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	本功能是，基于尚云的P2P门铃演示工程。

2 代码路径
-------------------------------------
	demo路径：``./projects/media/doorbell/main/src/doorbell_cs2_service.c``

	apk下载路径：http://dl.bekencorp/apk/ArminoMedia.apk

3 cli命令简介
-------------------------------------
	本功能主要依靠APP去使用配置，暂时不需要cli命令辅助控制

4 演示介绍
-------------------------------------

    1.设置路由器配网信息
	
    在打开左边导航界面，点击设置界面
	
.. figure:: ../../../../_static/doorbell/db_01.png
    :align: center
    :alt: setting01
    :figclass: align-center

    Figure 1.

	
    2.在设置界面，可以通过添加ssid和password来完成配置。
	
.. figure:: ../../../../_static/doorbell/db_02.png
    :align: center
    :alt: setting02
    :figclass: align-center
	
    Figure 2.
	
	
    3.在设置界面，设置cs2的配置信息
	
.. figure:: ../../../../_static/doorbell/db_03.png
    :align: center
    :alt: setting03
    :figclass: align-center
	
    Figure 3.
	
    4.另外，我们可以通过json文本来快速配置。

``{ssid: "xxxxx", password: "xxxxx", cs2_did: "XXXXXX-000000-XXXXXX", cs2_api_license: "XXXXXX", cs2_key: "XXXXXX", cs2_init_string: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"}``
	
    5.点击右边菜单栏上的import boarding按钮，可以快速导入，路由器账号密码。
	通过点击import cs2，可以快速导入，尚云的配置信息。
	
.. figure:: ../../../../_static/doorbell/db_04.png
    :align: center
    :alt: setting04
    :figclass: align-center
	
    Figure 4.
	
    6.选择配网方案

.. figure:: ../../../../_static/doorbell/db_05.png
    :align: center
    :alt: setting05
    :figclass: align-center
	
    Figure 5.
	

    7.在蓝牙界面，搜索门铃，并连接配网
	
	
.. figure:: ../../../../_static/doorbell/db_06.png
    :align: center
    :alt: setting06
    :figclass: align-center
	
    Figure 6.
	
	
	
.. figure:: ../../../../_static/doorbell/db_07.png
    :align: center
    :alt: setting07
    :figclass: align-center
	
    Figure 7.
	
    8.在图传界面，连接设备。
	
.. figure:: ../../../../_static/doorbell/db_08.png
    :align: center
    :alt: setting08
    :figclass: align-center
	
    Figure 8.
	
    9.设备连接上之后，才可以在设备设置界面，选择camera/屏的类型。
	
.. figure:: ../../../../_static/doorbell/db_09.png
    :align: center
    :alt: setting09
    :figclass: align-center
	
    Figure 9.
	
    10.最后在图传界面，打开摄像头，和屏即可
	
.. figure:: ../../../../_static/doorbell/db_10.png
    :align: center
    :alt: setting10
    :figclass: align-center
	
    Figure 10.
	
.. figure:: ../../../../_static/doorbell/db_11.png
    :align: center
    :alt: setting11
    :figclass: align-center
	
    Figure 11.
	