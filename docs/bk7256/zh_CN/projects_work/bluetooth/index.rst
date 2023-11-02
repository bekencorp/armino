蓝牙工程
======================================

:link_to_translation:`en:[English]`

芯片功能
----------------------------


+------------+------------+
|            | classic    |
+============+============+
| BK7256     | Y          |
+------------+------------+


• classic支持：
    | a2dp source


工程介绍
----------------------------

+------------+----------------+
|            | 中控(central)  |
+============+================+
| BK7256     | Y              |
+------------+----------------+


• 中控：
    | 用于手机、拨号盘等主设备场景，开启a2dp source、avrcp ct/tg、spp、ble等feature。


工程路径
----------------------------
 - 中控(central): ``project/bluetooth/central``


测试方法
----------------------------

• central：
    | 1.准备一张sd卡，格式化成exfat，放入project/bluetooth/central/pcm_sample_s16l_44100_dual.pcm至根目录。
    | 2.插入sd卡，开机。
    | 3.令音响进入配对模式
    | 4.输入 ``AT+BT=A2DP_SOURCE_CONNECT,xx:xx:xx:xx:xx:xx``，其中xx为音响地址
    | 5.连接成功后，输入 ``AT+BT=A2DP_SOURCE_START,xx:xx:xx:xx:xx:xx``
    | 6.输入 ``AT+BT=A2DP_SOURCE_WRITE_TEST,xx:xx:xx:xx:xx:xx,file,1:/pcm_sample_s16l_44100_dual.pcm``
    | 7.此时可以听到音响播出声音

