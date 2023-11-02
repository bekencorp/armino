中控
======================================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	用于手机、拨号盘等主设备场景，开启a2dp source、avrcp ct/tg、ble等feature。

2 代码路径
-------------------------------------
	demo路径：``./projects/bluetooth/central/main/armino_main.c``

	编译命令：``make bk7256 PROJECT=bluetooth/central``

3 a2dp source cli命令简介
-------------------------------------

    +--------------------------------------------------+------------+
    | a2dp_player connect <xx:xx:xx:xx:xx:xx>          | 连接音响   |
    +--------------------------------------------------+------------+
    | a2dp_player disconnect <xx:xx:xx:xx:xx:xx>       | 断开连接   |
    +--------------------------------------------------+------------+
    | a2dp_player play <xxx.mp3>                       | 播放mp3    |
    +--------------------------------------------------+------------+
    | a2dp_player stop                                 | 停止播放   |
    +--------------------------------------------------+------------+
    | a2dp_player pause                                | 暂停       |
    +--------------------------------------------------+------------+
    | a2dp_player resume                               | 恢复       |
    +--------------------------------------------------+------------+

4 a2dp source 测试过程
-------------------------------------

    | 1.准备一张sd卡，格式化成exfat，将project/bluetooth/central/1_qcs.mp3放入根目录。(必须是16bits的mp3)
    | 2.插入sd卡，开机。
    | 3.令音响进入配对模式
    | 4.输入 ``a2dp_player connect xx:xx:xx:xx:xx:xx``，其中xx为音响地址。
    | 5.输入 ``a2dp_player play xxx.mp3``
    | 6.此时可听到播出声音
    | 7.正在播放的情况下，可以stop pause，停止播放的情况下可以play。(尽量连接后立刻play，不要stop，参见章节5)
    | 8.如果音响支持avrcp，可以通过音响控制播放暂停。(存在兼容性问题，参见章节5)


5 a2dp source 兼容性说明
-------------------------------------

    | 1.仅播放歌曲场景，某些音响(例如JBL)如果长时间处于stop(本地a2dp_player stop 或对端avdtp suspend)状态，会主动断开连接(JBL直接下电蓝牙)。log会提示bt_api_event_cb:Disconnected from xx:xx:xx:xx:xx:xx
    | 2.某些音响(例如xiaomi)不会向本地注册avrcp playback notify，会导致两端操作播放暂停时状态不一致的现象。
    | 3.某些音响(例如xiaomi)不会向本地报告avrcp volume changel，此时音响调节音量，central无法得知。

6 a2dp source 其他注意事项
-------------------------------------

    | 1.如果sdcard有问题，会出现 f_mount failed 或 read data crc error 的提示

参考链接
----------

    `API参考: <../../api-reference/bluetooth/index.html>`_ 介绍了蓝牙API接口

    `开发者指南: <../../developer-guide/bluetooth/index.html>`_ 介绍了蓝牙常用使用场景

    `样例演示: <../../examples/bluetooth/index.html>`_ 介绍了蓝牙样例使用和操作

    `蓝牙工程: <../../projects_work/bluetooth/index.html>`_ 介绍了蓝牙相关工程
