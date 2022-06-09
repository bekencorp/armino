Armino Wi-Fi说明
=======================================================

Armino Wi-Fi功能列表
-------------------------------------------------------

- 兼容IEEE802.11 b/g/n/ax 2.4GHz标准(BK7256支持802.11ax)
- 支持HT20 and HT40(BK7256支持HT40)
- 支持802.11N MCS0-7
- 支持STA、AP and Direct Modes
- 支持Concurrent AP+STA
- 支持WPA、WPA2及WPA3等加密方式
- 支持AMPDU、QoS
- Station模式下支持低功耗休眠

Armino Wi-Fi编程模型
-------------------------------------------------------
.. image:: ../../../_static/program.png
Wi-Fi驱动程序作为和上层代码(如TCP/IP协议栈、Application Task等)的隔离组件,通常由用户应用程序代
码负责调用Wi-Fi初始化及进行必要的配置,Wi-Fi驱动程序接收并处理API请求的数据,并将相关事件通知到应
用程序处理.

Armino Wi-Fi事件描述
-------------------------------------------------------
EVENT_WIFI_SCAN_DONE
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
扫描完成事件,通常由STA模式连接过程中扫描空中所有信道完成,收到此事件后,应用程序的事件回调函数可以
调用bk_wifi_scan_get_result获取扫描结果.

EVENT_WIFI_STA_CONNECTED
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
BEKEN STA已经成功连接目标AP的事件,接收到此事件后,Wi-Fi驱动程序根据应用程序响应返回结果决定是否启动
DHCP服务获取IP地址或者使用静态IP地址.

EVENT_WIFI_STA_DISCONNECTED
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
BK STA与AP断开事件,接收到此事件后,一般需要通知针对基于套接字编写的客户端应用程序关闭套接字.如果
此事件不是用户希望的,可以启动重连流程.

Armino Wi-Fi station模式正常连接场景示例
-------------------------------------------------------
.. image:: ../../../_static/connect_procedure.png

Wi-Fi初始化阶段
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
- app任务调用app_wifi_init接口,初始化bk_event_init、bk_netif_init、bk_wifi_init

Wi-Fi配置阶段
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
- 进入Wi-Fi初始化阶段后,初始化workqueue、Wi-Fi MAC、PHY等

Wi-Fi启动阶段
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
- Wi-Fi驱动执行bk_wifi_sta_start
- 用户可以在该函数中进行Wi-Fi初始化前的一些配置,如通信速率配置、当前模式等
- 初始化LwIP协议栈、初始化wpa_supplicant

Wi-Fi连接阶段
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
- Wi-Fi驱动启动后根据最新的配置(模式、SSID、密码等),进入对应的模式,默认为STA模式
- 根据命令执行扫描、连接的过程
- 根据扫描结果,上报EVENT_WIFI_SCAN_DONE事件
- 连接AP成功之后,Wi-Fi驱动返回EVENT_WIFI_STA_CONNECTED事件
- Wi-Fi事件回调函数将EVENT_WIFI_STA_CONNECTED发送到应用程序任务

Wi-Fi获取IP地址阶段
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
- 启动LWIP的DHCP客户端获取IP地址
- 获取IP地址之后,通过SM_DHCP_DONE_IND消息发送出去
- 应用程序收到通知后,可以创建TCP/UDP Socket进行连接服务器的操作

Wi-Fi Station连接命令示范
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
::

    net sta enable
    net sta scan once
    net sta scan result
    net sta config SSID PW  //keep PW NULL when OPEN Mode
    net sta connect

Armino Wi-Fi station模式断开连接场景示例
-------------------------------------------------------
.. image:: ../../../_static/sta_disconnect.png
- Wi-Fi Station已成功连接至AP的情况下,应用程序调用 ``bk_wifi_sta_disconnect`` 主动断开连接
- 当Wi-Fi因为主动断开、AP离线、RSSI弱等原因断开连接,Wi-Fi事件回调函数引发 ``EVENT_WIFI_STA_DISCONNECTED``
- 应用程序接收到网络断开事件后,进行网络应用程序的清除,例如关闭TCP/UDP客户端套接字等
- 通常由于非期待的原因导致Wi-Fi断开,应用程序会发起重新连接

Armino Wi-Fi station模式关闭Wi-Fi场景示例
-------------------------------------------------------
与上述station模式断开连接场景类似,不再赘述.

Armino Wi-Fi AP模式场景示例
-------------------------------------------------------
BEKEN SoftAP应用场景可以参考API开发指导中的 ``bk_wifi_ap_start`` ,接入AP的STA信息,由 ``bk_wifi_ap_get_sta_list`` 获取.

Armino Wi-Fi失败代码原因
-------------------------------------------------------
.. image:: ../../../_static/reason_codes.png

Armino Wi-Fi MAC地址配置
-------------------------------------------------------
Wi-Fi MAC地址配置位于 ``bk_system\mac.c`` , 分为base_mac、sta_mac、ap_mac,配置
STA MAC地址有以下几种方式:

- 通过EFUSE写入
- 随机数产生,需使能 ``bk72xx.defconfig`` 配置文件中 ``CONFIG_RANDOM_MAC_ADDR`` 宏定义
- 使用[MAC地址配置工具]:bk_writer烧写MAC地址到FLASH
- 使用mac命令写入flash,使用方式如下：

::

    mac c8478caabbcc
    txevm -e 2

:link_to_translation:`en:[English]`

