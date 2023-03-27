Wi-Fi STA低功耗指南
=============================================
Armino支持Wi-Fi6标准中定义的TWT省电模式与协议中定义的其他省电模式(简称 "传统省电模式"). 另外, 针对某些对性能要求不高, 但对功耗要求特别高的应用, Armino还做了很多专属优化.

传统省电模式
--------------------------------------------
省电流程
++++++++++++++++++++++++++++++++++++++++++++

.. figure:: ../../../_static/Based_on_DTIM.png
    :align: center
    :alt: Based on DTIM
    :figclass: align-center

    Based on DTIM

典型省电流程如上图所示:

- STA连接AP时, 与AP交换与休眠相关的关键参数:
    - Beacon周期 - AP周期性广播beacon, beacon中会携带指示已休眠的STA是否有单播包要接收
    - DTIM周期 - 为beacon interval的整数倍, DTIM beacon中会指示是否有广播包要发送, 并在随后的beacon周期中发送广播包
    - Listen Interval - STA在睡眠状态时, 需要AP为STA缓存单播包的最长时间
- 当睡眠条件具备时(通常是STA没有包要收发时)STA通知AP自己将要进入休眠模式
- 得到AP确认后, STA关闭RF/MAC电源或时钟, 进入休眠状态
- 当STA处于休眠状态时, AP会缓存发向STA的单播包; 如果AP有广播包要发送，会在随后的DTIM beacon中通知STA
- STA在DTIM周期醒来, 查看是否需要接收广播包; 如果有广播, 则醒来接收广播包, 如果有单播包要接收, 则通知AP自己已醒, 随后AP向STA发送单播包

STA定期睡眠和唤醒, 在睡眠状态下, RF、MAC和MODEM理论上处于关闭状态, 在Wi-Fi与BLE共存模式下, RF可能无法关闭.

进一步降低功耗
++++++++++++++++++++++++++++++++++++++++++

从 `省电流程`_ 可以看到, 在休眠模式下, STA每个DTIM周期都必须醒来, 这在某些场景下可能成为耗电的主要原因, 特别是当DTIM值很小时(如DTIM为1时)为降低STA醒来频率, Armino提供了一种机制: 仅在listen interval间隔醒来.流程如下:

.. figure:: ../../../_static/Based_on_Listen_Interval.png
    :align: center
    :alt: Based on Listen Interval
    :figclass: align-center

    Based on Listen Interval

应用可以通过 ``CONFIG_MM_PS_BASED_ON_LISTEN_INTERVAL`` 来使能这种机制, 通过 ``CONFIG_MM_PS_SET_LISTEN_INTERVAL_VALUE`` 来配置 listen interval值.

.. code::

 CONFIG_MM_PS_SET_LISTEN_INTERVAL_VALUE=30
 CONFIG_MM_PS_BASED_ON_LISTEN_INTERVAL=y

通常listen interval越大,节能效果越好, 但listen interval越大, 性能与连接稳定性越差.

.. note:: 因为AP会在DTIM间隔发送广播帧, 因此"仅在 listen interval醒来"机制使能时, STA可能会错失广播包; 另一方面, 如果listen interval设置过长, 可能会导致AP为STA缓存过多的单播包, 当AP内存不足时, 可能会引起丢包; 这两种情况都会影响Wi-Fi性能, 连接稳定性, 因此, 这种机制尽适合那些对性能要求不高, 但对功耗要求特别高的应用

关闭省电模式
++++++++++++++++++++++++++++++++++++++++++
默认STA休眠开启, 用户可通过 ``bk_wifi_sta_pm_disable()`` 关闭休眠, 通过 ``bk_wifi_sta_pm_enable()`` 使能休眠.要注意, 某些特定应用中, STA休眠有特定的要求, 如:

- 当STA与AP共存时, 由于AP存在, 即使开启, 实际上也不会进入低功耗
- 当STA与BLE共存时, 休眠模式必须开启. 因为STA RF在切向BLE时, 应该先进入休眠状态, 否则可能造成丢包
- 对功耗不敏感的设备, 如冰箱, 电饭煲等, 为获取最佳性能, 可选择关闭休眠

TWT 省电模式
--------------------------------------------
待完成.

:link_to_translation:`en:[English]`
