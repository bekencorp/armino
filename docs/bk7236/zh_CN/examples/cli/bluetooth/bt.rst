BT Cli
===============

:link_to_translation:`en:[English]`


1. 功能概述
--------------------------
    为了便于验证BT相关功能，为此设计了一系列AT CMD。所有BLE AT CMD命令以 ``AT+BT=`` 开头，后接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。


2. 代码路径
--------------------------
	路径: ``\components\at\src\bt_at_command.c``


3. cli命令简介
--------------------------
支持的命令如下:

    1.AT+BT=INQUIRY
     - 功能说明
        扫描命令，用于扫描周围可见的classic设备
     - 命令格式
        AT+BT=INQUIRY
     - 参数说明
        无参数
     - 返回值说明
        扫描到的设备地址

    2.AT+BT=READ_SCAN_ENABLE
     - 功能说明
        获取当前设备可见性
     - 命令格式
        AT+BT=READ_SCAN_ENABLE
     - 参数说明
        无参数
     - 返回值说明
        .. csv-table::
            :header: "Value", "Parameter Description"
            :widths: 10, 50

            0x00,"No Scans enabled."
            0x01,"Inquiry Scan enabled. Page Scan disabled."
            0x02,"Inquiry Scan disabled. Page Scan enabled."
            0x03,"Inquiry Scan enabled. Page Scan enabled."

    3.AT+BT=WRITE_SCAN_ENABLE
     - 功能说明
        设置当前设备可见性
     - 命令格式
        AT+BT=WRITE_SCAN_ENABLE,param1
     - 参数说明
        .. csv-table::
            :header: "Param1", "Parameter Description"
            :widths: 10, 40

            0x00,"No Scans enabled."
            0x01,"Inquiry Scan enabled. Page Scan disabled."
            0x02,"Inquiry Scan disabled. Page Scan enabled."
            0x03,"Inquiry Scan enabled. Page Scan enabled."

    4.AT+BT=CONNECT
     - 功能说明
        连接指定设备
     - 命令格式
        AT+BT=CONNECT,param1,param2
     - 参数说明
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "Bluetooth address"
            param2, "Allow_Role_Switch
             | 0x00 not accept role switch
             | 0x01 accept role switch"

    5.AT+BT=DISCONNECT
     - 功能说明
        断开指定设备
     - 命令格式
        AT+BT=DISCONNECT,param1
     - 参数说明
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "Bluetooth address"

    6.AT+BT=SPP_INIT
     - 功能说明
        SPP初始化，注册SDP
     - 命令格式
        AT+BT=SPP_INIT
     - 参数说明
        无参数

    7.AT+BT=SPP_CONNECT
     - 功能说明
        SPP连接指定设备
     - 命令格式
        AT+BT=SPP_CONNECT,param1
     - 参数说明
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "Bluetooth address"

    8.AT+BT=SPP_TX
     - 功能说明
        SPP发送指定数据
     - 命令格式
        AT+BT=SPP_TX,param1
     - 参数说明
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "send data"
     - 测试用例
        AT+BT=SPP_TX,beken-spp-tx_test!!!!

    9.AT+BT=SPP_THROUGH_TEST
     - 功能说明
        SPP吞吐测试，发送的数据为随机生成的数据
     - 命令格式
        AT+BT=SPP_THROUGH_TEST,param1
     - 参数说明
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "send data length"
     - 返回值说明
        指定长度的数据发送完成后，client和server会从log中输出以发送数据计算的CRC值以及传输速率。
     - 测试用例
        AT+BT=SPP_THROUGH_TEST,0xfffff