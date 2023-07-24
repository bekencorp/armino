安全术语
=====================

.. note::

 在描述系统安全时，会直接使用本节介绍的安全术语!

术语
---------------------

相关述语与简写含义：

 - ``S`` - Security world，表示安全世界。为 ARM Cortex-M33 中与 TrustZone 相关的概念。
 - ``NS`` - Non-Security world，表示非安全世界。为 ARM Cortex-M33 中与 TrustZone 相关的概念。
 - ``S/NS Aware`` - 特指自身能依据 S/NS 信号进行安全控制的设备。
 - ``P`` - Privilege，表示 M33 运行在特权级。
 - ``NP`` - Non-Privilege，表示 M33 运行在非特权级。
 - ``M33`` - ARM Cortex-M33 处理器。
 - ``TZ`` - TrustZone，M33 中引入的安全机制，通过 TZ 将 CPU 核隔离成 S/NS。
 - ``PPC`` - Peripheral Protection Control，BK7236 外设安全属性配置与控制器。
 - ``MPC`` - Memory Protection Control，BK7236 块存储安全属性配置与控制器。
 - ``BL1`` - 特指固化在芯片中的 BootROM。
 - ``BL2`` - Bootloader，在 BK7236 中特指 MCUBOOT。