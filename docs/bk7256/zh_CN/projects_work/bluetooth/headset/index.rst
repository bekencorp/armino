耳机
======================================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	用于手机、拨号盘等主设备场景，开启a2dp sink、avrcp ct/tg、ble等feature。

2 代码路径
-------------------------------------
	demo路径：``./projects/bluetooth/headset/main/armino_main.c``

	编译命令：``make bk7256 PROJECT=bluetooth/headset``

3 avrcp cli 命令
-------------------------------------
    连接后可以使用cli命令测试avrcp(注意，某些命令是否生效视手机、apk而定)

    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,play              | 播放          |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,pause             | 暂停          |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,next              | 下一曲        |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,prev              | 上一曲        |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,rewind            | 快退          |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,fast_forward      | 快进          |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,vol_up            | 本地音量增加  |
    +------------------------------------+---------------+
    | AT+BT=AVRCP_CTRL,vol_down          | 本地音量减少  |
    +------------------------------------+---------------+
    
参考链接
----------

    `API参考: <../../api-reference/bluetooth/index.html>`_ 介绍了蓝牙API接口

    `开发者指南: <../../developer-guide/bluetooth/index.html>`_ 介绍了蓝牙常用使用场景

    `样例演示: <../../examples/bluetooth/index.html>`_ 介绍了蓝牙样例使用和操作

    `蓝牙工程: <../../projects_work/bluetooth/index.html>`_ 介绍了蓝牙相关工程
