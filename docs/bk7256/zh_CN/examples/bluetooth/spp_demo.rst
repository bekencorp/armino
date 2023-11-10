SPP
========================

:link_to_translation:`en:[English]`

概述
--------------------------
SPP旨在通过蓝牙在两个支持spp profile的设备上做到数据透传功能。

简介
--------------------------
1. spp蓝牙工作流程如下图所示:

.. figure:: ../../../_static/bt_spp_work_flow.png
    :align: center
    :alt: spp工作流程图
    :figclass: align-center

    Figure 1. spp work flow

2. spp验证所需工具: 两块开发板或开发板与手机，手机需安装spp相关应用，两块开发板可做吞吐测试。

操作说明
--------------------------
 1) 开发板A作为server,串口下发AT命令初始化SPP SERVER: ``AT+BT=SPP_INIT``
 2) 开发板B作为client,串口下发AT命令发起连接: ``AT+BT=SPP_CONNECT,xx:xx:xx:xx:xx:xx`` (开发板A蓝牙地址或手机蓝牙地址)
 3) 待提示spp连接成功后，可通过AT命令发送数据: ``AT+BT=SPP_TX,beken_spp_tx_test!!!!`` (发送数据)
 4) spp连接成功后，可通过AT命令做吞吐测试: ``AT+BT=SPP_THROUGH_TEST,0xffff`` (数据长度，发送数据为随机生成数据，指定长度发送完成后，client与server log会输出传输速率以及crc值)
 5) 与手机测试时，手机开启SPP应用后，开发板直接进行操作2即可。

示例
--------------------------
 | SPP相关AT测试代码可参考: ``\components\at\src\bt_at_command.c``
 | AT命令详细介绍可参考: `examples/cli/bluetooth/bt.html <../cli/bluetooth/bt.html>`_


参考链接
----------

    `API参考: <../../api-reference/bluetooth/index.html>`_ 介绍了蓝牙API接口

    `开发者指南: <../../developer-guide/bluetooth/index.html>`_ 介绍了蓝牙常用使用场景

    `样例演示: <../../examples/bluetooth/index.html>`_ 介绍了蓝牙样例使用和操作

    `蓝牙工程: <../../projects_work/bluetooth/index.html>`_ 介绍了蓝牙相关工程