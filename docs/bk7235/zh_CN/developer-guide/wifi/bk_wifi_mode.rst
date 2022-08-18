Wi-Fi AP/STA/P2P模式示例说明
=============================

本工程展示了BEKEN Wi-Fi模块AP/STA/P2P三种模式的使用示例:

- Wi-Fi启动STA模式的使用
- Wi-Fi启动AP模式的使用
- Wi-Fi启动P2P模式的使用(开发中)

工程示例及配置说明
--------------------------------------------------
BEKEN Wi-Fi STA&AP模式无需特殊配置,默认支持;P2P需使能 ``middleware\soc\bk72xx.defconfig`` 中的宏 ``CONFIG_COMPONENTS_P2P``

.. Note:: P2P功能开发需求请联系BEKEN技术支持团队

代码流程
--------------------------------------------------
Wi-Fi STA模式
+++++++++++++++++++++++++++++++++++++++++++++++++
V1
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

BEKEN Wi-Fi模块通过命令进入对应的模式,输入 ``net mode`` 之后可以看到当前支持的模式:
::
     net mode <mode>
          - net mode sta
          - net mode ap

STA模式的入口函数是 ``cmd_wlan_sta_exec``,代码路径: ``properties\modules\bk_wifi_impl\wifi_wpa_cmd.c`` ,该函数中通过set/get命令分别配置/获取相应的配置,常用的有如下:
::
     net sta config <ssid> [psk]
          - net sta config ssid_example
          - net sta config ssid_example psk_example

     net sta set <field> <value>
          - net sta set ssid ssid_example
          - net sta set psk psk_example
          - net sta set scan_ssid (0, 1)
     net sta enable
     net sta disable

     cmd example:
     net sta enable
     net sta config BEKEN_TEST 12345678
     net sta connect

V2
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

BEKEN在V1的基础上,通过定制化客户需求,完成第二版命令的适配.该版在原有基础上,简化了命令使用方式,单一命令实现STA模式连接.代码路径: ``components\bk_cli\cli_wifi.c`` 使用前需使能宏 ``CLI_CFG_WIFI=1``, STA模式使用方式介绍如下:
::
     {"scan", "scan [ssid]", cli_wifi_scan_cmd}
     {"sta", "sta ssid [password][bssid][channel]", cli_wifi_sta_cmd}
     {"state", "state - show STA/AP state", cli_wifi_state_cmd}
     {"stop", "stop {sta|ap}", cli_wifi_stop_cmd}

     cmd example:
     sta BEKEN_TEST 12345678

.. note:: V2版兼容V1版的使用方式

Wi-Fi AP模式
++++++++++++++++++++++++++++++++++++++++++++++++++
AP模式V1版入口函数是 ``cmd_wlan_ap_exec`` ,代码路径同STA模式,主要配置参数有SSID和密码:
::
     net ap set <field> <value>
          - net ap set ssid ssid_example
          - net ap set psk psk_example
     net ap enable
     net ap reload
     net ap disable

     cmd example:
     net ap enable
     net ap set ssid BEKEN_TEST
     net ap set psk 12345678
     net ap start

V2版本AP模式入口函数 ``cli_wifi_ap_cmd`` ,代码路径: ``components\bk_cli\cli_wifi.c`` ,详细介绍如下:
::
     {"ap", "ap ssid [password]", cli_wifi_ap_cmd}
     {"stop", "stop {sta|ap}", cli_wifi_stop_cmd}
     {"state", "state - show STA/AP state", cli_wifi_state_cmd}

     cmd example:
     ap BEKEN_TEST 12345678

.. Note:: BEKEN Wi-Fi支持AP+STA共存

Wi-Fi P2P模式
+++++++++++++++++++++++++++++++++++++++++++++++++++
P2P模式仍然在开发中,通过入口函数 ``cmd_wlan_p2p_exec`` 可以配置协商时P2P的SSID,目前默认为 ``beken p2p``
P2P身份通过协商获取,可以通过配置 ``param.intent`` 值来指定身份.P2P开启的命令为:
::
     net p2p enable

.. Note:: P2P功能开发需求请联系BEKEN技术支持团队

:link_to_translation:`en:[English]`

