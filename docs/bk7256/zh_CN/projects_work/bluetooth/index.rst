蓝牙工程
======================================

:link_to_translation:`en:[English]`

芯片功能
----------------------------


+------------+------------+------------+------------+
|            | classic    | ble        | mesh       |
+============+============+============+============+
| BK7256     | Y          | Y          | Y          |
+------------+------------+------------+------------+


• classic支持：
    | a2dp sink/source
	| avrcp ct/tg
    | hfp hf
    | spp

• ble支持：
    | central
    | peripheral

• mesh支持：
    | provisioner
    | provisionee
    | low power node
    | friend node
    | relay node
    | pb adv


工程介绍
----------------------------

+------------+----------------+----------------+----------------+----------------+
|            | 头戴(headset)  | 中控(central)  | 自组网(mesh)   | HCI            |
+============+================+================+================+================+
| BK7256     | Y              | Y              | Y              | Y              |
+------------+----------------+----------------+----------------+----------------+

• 头戴：
    | 用于耳机、音响等从设备场景，开启a2dp sink、avrcp ct/tg、hfp hf等feature。

• 中控：
    | 用于手机、拨号盘等主设备场景，开启a2dp source、avrcp ct/tg、spp、ble等feature。

• 自组网：
    | 用于低功耗节点等自组网场景，开启ble、mesh feature。

• HCI：
    | 单controller，用于客户自己提供host的场景，开启classic、ble feature。

工程路径
----------------------------
 - 头戴(headset): ``project/bluetooth/headset``
 - 中控(central): ``project/bluetooth/central``
 - 自组网(mesh): ``project/bluetooth/mesh``
 - HCI: ``project/bluetooth/hci``

参考链接
----------

    `API参考: <../../api-reference/bluetooth/index.html>`_ 介绍了蓝牙API接口

    `开发者指南: <../../developer-guide/bluetooth/index.html>`_ 介绍了蓝牙常用使用场景

    `样例演示: <../../examples/bluetooth/index.html>`_ 介绍了蓝牙样例使用和操作

    `蓝牙工程: <../../projects_work/bluetooth/index.html>`_ 介绍了蓝牙相关工程

