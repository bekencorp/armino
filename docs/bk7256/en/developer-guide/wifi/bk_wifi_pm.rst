WiFi STA 低功耗指南
=============================================

:link_to_translation:`en:[English]`


Armino 支持 WiFi-6 标准中定义的 TWT 省电模式与协议中定义的其他省电模式（简称 “传统省电模式”）。

另外，针对某些但对性能要求不高，但对功耗要求特别高应用，Amino 还做了很多专属优化。


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

 - STA 连接 AP 时，与 AP 交换与休眠相关的关键参数:
   - Beacon 周期 - AP 周期性广播 beacon，beacon 中会携带指示已休眠的 STA 是否有单播包要接收。
   - DTIM 周期 - 为 beacon interval 的整数倍，DTIM beacon 中会指示是否有广播包要发送，并在随后的 beacon 周期中发送广播包。
   - Listen Interval - STA 在睡眠状态时,需要 AP 为 STA 缓存单播包的最长时间。
 - 当睡眠条件具备时(通常是 STA 没有包要收发时），STA 通知 AP 自己将要进入休眠模式。
 - 得到 AP 确认后，STA 关闭 RF/MAC 电源或时钟，进入休眠状态。
 - 当 STA 处于休眠状态时，AP 会缓存发向 STA 的单播包；如果 AP 有广播包要发送，会在随后的 DTIM beacon 中通知 STA。
 - STA 在 DTIM 周期醒来，查看是否需要接收广播包；如果有广播，则醒来接收广播包，如果有单播包要接收，则通知 AP 自己已醒，随后 AP 向 STA 发送单播包。

 STA 定期睡眠和唤醒，在睡眠状态下，RF、MAC 和 MODEM 理论上处于关闭状态，在 WiFi 与 BLE 共存模式下，RF 可能无法关闭。

进一步降低功耗
++++++++++++++++++++++++++++++++++++++++++

从 <省电流程>_ 可以看到，在休眠模式下，STA 每个 DTIM 周期都必须醒来，这在某些场景下可能成为耗电的主要原因，特别是
当 DTIM 值很小时（如 DTIM 为 1 时）；为降低 STA 醒来频率，Armino 提供了一种机制：仅在 listen interval 间隔醒来。
流程如下：

.. figure:: ../../../_static/Based_on_Listen_Interval.png
    :align: center
    :alt: Based on Listen Interval
    :figclass: align-center

    Based on Listen Interval

应用可以通过 ``CONFIG_MM_PS_BASED_ON_LISTEN_INTERVAL`` 来使能这种机制，通过 ``CONFIG_MM_PS_SET_LISTEN_INTERVAL_VALUE``
来配置 listen interval 值。

.. code::

 CONFIG_MM_PS_SET_LISTEN_INTERVAL_VALUE=30
 CONFIG_MM_PS_BASED_ON_LISTEN_INTERVAL=y

通常 listen interval 越大，节能效果越好，但 listen interval 越大，性能与连接稳定性越差。

.. note:

  因为 AP 会在 DTIM 间隔发送广播帧，因此“仅在 listen interval 醒来" 机制使能时，STA 可能会错失广播包；
  另一方面，如果 listen interval 设置过长，可能会导致 AP 为 STA 缓存过多的单播包，当 AP 内存不足
  时，可能会引起丢包；这两种情况都会影响 WiFi 性能，连接稳定性，因此，这种机制尽适合那些对性能要
  求不高，但对功耗要求特别高的应用。

关闭省电模式
++++++++++++++++++++++++++++++++++++++++++

默认 STA 休眠开启，用户可通过 bk_wifi_sta_pm_disable() 关闭休眠, 通过 bk_wifi_sta_pm_enable() 使能休眠。

要注意，在某些特定应用中，STA 休眠有特定的要求，如：

 - 当 STA 与 AP 共存时，由于 AP 存在，即使开启，实际上也不会进入低功耗;
 - 当 STA 与 BLE 共存时，休眠模式必须开启。因为 STA RF 在切向 BLE 时，应该先进入休眠状态，否则可能造成丢包。
 - 对功耗不敏感的设备，如冰箱，电饭煲等，为获取最佳性能，可选择关闭休眠。


TWT 省电模式
--------------------------------------------

待完成。


