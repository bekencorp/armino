Wi-Fi STA Low Power
=============================================

:link_to_translation:`zh_CN:[中文]`

Armino supports the TWT power saving mode defined in Wi-Fi6 and other power saving modes defined in the protocol(traditional power saving mode for short). In addition, Armino has made a number of proprietary optimizations for applications that require low performance but high power consumption.

Traditional Power Saving Mode
--------------------------------------------
Power Save Process
++++++++++++++++++++++++++++++++++++++++++++

.. figure:: ../../../_static/Based_on_DTIM_EN.png
    :align: center
    :alt: Based on DTIM
    :figclass: align-center

    Based on DTIM

The typical power saving process is shown in the figure above:

- When STA connects to AP, it exchanges key parameters related to sleep with the AP:
    - Beacon Period - AP periodically broadcasts beacon, which indicates whether the dormant STA has unicast pakcets to receive
    - DTIM Period - integer multiple of beacon interval. DTIM beacon indicates whether there are broadcast packets to be sent, and the broadcast packets are sent in the subsequent beacon period
    - Listen Interval - STA specifies the maximum time for the AP to store unicast packets for STA when it is asleep
- When sleep conditions are available(usually when the STA has no packets to send or receive), the STA notifies AP that it is going to sleep mode
- After receiving AP confirmation, STA turns off RF/MAC power or clock and enters sleep mode
- When STA is in sleep state, AP will cache unicast packets sent to the STA. If AP has a broadcast packet to send, the STA is notified in a subsequent DTIM beacon
- STA wakes up during the DTIM cycle to see if it needs to receive broadcast packets. If there is broadcast packet, it wakes up to receive; if there is a unicast packet, it notifies AP that it is awake, and then AP will send the unicast to STA

STA sleeps and wakes up regularly, RF、MAC and modem will turn off in sleep state theoretically. RF could not be off in Wi-Fi and BLE coexistence mode.

Further Power Reduction
++++++++++++++++++++++++++++++++++++++++++
From `Power Save Process`_ , as you can see, in sleep mode, STA must wake up every DTIM cycle, which can be a major cause of power consumption in some scenarios, especially when DTIM is very small (like 1). To reduce the frequency of STAs waking up, Armino provides a mechanism to wake up only during listen interval. The process is as follows:

.. figure:: ../../../_static/Based_on_Listen_Interval_EN.png
    :align: center
    :alt: Based on Listen Interval
    :figclass: align-center

    Based on Listen Interval

Application can enable this mechanism by setting ``CONFIG_MM_PS_BASED_ON_LISTEN_INTERVAL`` and configure listen interval by definition ``CONFIG_MM_PS_SET_LISTEN_INTERVAL_VALUE``. The example setting is as follows:

.. code::

  CONFIG_MM_PS_SET_LISTEN_INTERVAL_VALUE=30
  CONFIG_MM_PS_BASED_ON_LISTEN_INTERVAL=y

Generally, the larger listen interval, the better power saving , accompanying worse performance and connection stability.

.. note:: AP sends broadcast frames at DTIM interval, STA may miss broadcast packet when this "wake up only at listen interval" mechanism is enabled. On the other hand, if listen interval is too large, AP may cache too many unicast packets for STA. When AP memory is insufficient, packet loss may occur. Both of these conditions affect Wi-Fi performance and connection stability, so this mechanism is only suitable for applications that require low performance but high power saving.

Disable Power Saving Mode
++++++++++++++++++++++++++++++++++++++++++
STA hibernation is enabled by default. User can disable it by function ``bk_wifi_sta_pm_disable()`` and enable by ``bk_wifi_sta_pm_enable()`` .

Note that STA hibernation has specific requirements for certain applications, such as:

- When STA and SoftAP coexist, even the mode is enabled, it will not actually enter low power
- When STA and BLE coexist, sleep mode must be enabled. When STA RF handovers to BLE, it should enter sleep mode first. Otherwise, packet loss may occur
- For devices that are not sensitive to power consumption, such as refrigerators and rice cookers, user can choose to turn off sleep for the best performance

TWT Power Saving Mode
--------------------------------------------
To Be Continued.


