Wi-Fi Quick Get-Started
====================================================
在 ``projects/examples/wifi`` 目录中提供了三种Wi-Fi常见使用场景的Example Demo，客户可以根据Demo快速了解Wi-Fi的相关使用.

- ap
- scan
- sta_connect

.. note:: 上述三种Demo编译下载后，可实现开机自动开启SoftAP、自动扫描、自动连接的功能

AP/SoftAP
-------------------------------------------------------
AP Example Demo展示了使用Beken Wi-Fi芯片建立SoftAP的流程.

- 编译命令: ``make bk7256 PROJECT=examples/wifi/ap``
- 文件路径: ``build/ap/bk7256/all-app.bin``
- 参数配置: ``projects/examples/wifi/ap/main/Kconfig.projbuild``

::

    menu "Example Configuration"
    config EXAMPLE_WIFI_SSID
      string "WiFi SSID"
      default "myssid"                             //SoftAP SSID
    config EXAMPLE_WIFI_PASSWORD
      string "WiFi Password"
      default "12345678"                           //SoftAP Password
    config EXAMPLE_IP
      string "IP4 Address of AP"
      default "192.168.10.1"                       //Default IP Address
    config EXAMPLE_MASK
      string "Network mask of AP"
      default "255.255.255.0"                      //Default参数可以自定义
    config EXAMPLE_GW
      string "Gateway address of AP"
      default "192.168.10.1"
    config EXAMPLE_DNS
      string "DNS address of AP"
      default "192.168.10.1"
    endmenu

- main log

::

    wifi:I(310):wifi inited(1) ret(0)
    example:I(312):ssid:myssid  key:mypassword
    wifi:I(314):ap configuring
    wifi:I(316):ap configured
    wifi:I(318):ap starting
    wifi:I(320):init 1st vif
    hitf:I(544):start apm success, vif0, channel0, bcmc2

Scan
-------------------------------------------------------
Scan Example Demo介绍了Wi-Fi扫描的基本流程，包含全信道扫描和指定SSID扫描.

- 编译命令: ``make bk7256 PROJECT=examples/wifi/scan``
- 文件路径: ``build/scan/bk7256/all-app.bin``
- 参数配置: ``projects/examples/wifi/scan/main/Kconfig.projbuild``

::

    menu "Example Configuration"
    config EXAMPLE_SCAN_ALL_SSID
      bool "Scan All SSID"
      default y                                    //全信道扫描

    config EXAMPLE_WIFI_SSID
      string "WiFi SSID"
    depends on !EXAMPLE_SCAN_ALL_SSID
      default "aclsemi"                            //指定SSID扫描
    endmenu

- main log

::

    wifi:I(1134):scan found 4 AP                   //指定SSID扫描
    SSID                             BSSID               RSSI chan security
    -------------------------------- -----------------   ---- ---- ---------
    aclsemi                          74:50:4e:3f:48:b0    -29  6 WPA2-AES
    aclsemi                          74:50:4e:3f:84:d0    -32  1 WPA2-AES
    aclsemi                          74:50:4e:3f:99:50    -33  6 WPA2-AES
    aclsemi                          74:50:4e:3f:82:50    -58 11 WPA2-AES
    wifi:I(1136):scan found 32 AP                  //全信道扫描
    SSID                             BSSID               RSSI chan security
    -------------------------------- -----------------   ---- ---- ---------
    Redmi_253C                       24:cf:24:3a:25:3e    -24  4 WPA2-MIX
    TP-LINK-1                        18:f2:2c:74:1d:dd    -30  1 WPA2-AES
    aclsemi                          74:50:4e:3f:48:b0    -31  6 WPA2-AES
    aclsemi                          74:50:4e:3f:99:50    -33  6 WPA2-AES
    TP-LINK_6C28                     50:fa:84:8f:6c:28    -34  1 WPA2-AES
    Xiaomi_0296                      8c:de:f9:b6:07:c2    -35  6 WPA2-MIX
    aclsemi                          74:50:4e:3f:84:d0    -36  1 WPA2-AES
    PowerTest                        c8:3a:35:7f:7e:e0    -38 11 WPA2-AES
    MP                               c0:61:18:2b:82:b4    -44 11 WPA2-AES
    ChinaNet-HeLg                    cc:c2:e0:6e:0f:a9    -45 13 WPA2-MIX

sta_connect
-------------------------------------------------------
sta_connect通过提前配置SSID、Password，编译成功后烧录版本，可实现开机自动连接功能.

- 编译命令: ``make bk7256 PROJECT=examples/wifi/sta_connect``
- 文件路径: ``build/sta_connect/bk7256/all-app.bin``
- 配置参数: ``projects/examples/wifi/sta_connect/main/Kconfig.projbuild``

::

    menu "Example Configuration"
    config EXAMPLE_WIFI_SSID
      string "Wi-Fi SSID"
      default "aclsemi"                            //目标SSID

    config EXAMPLE_WIFI_PASSWORD
      string "Wi-Fi Password"
      default "ACL8semi"                           //目标SSID密码
    endmenu

- main log

::

    wifi:I(308):wifi inited(1) ret(0)              //开机后初始化完成
    example:I(310):ssid:aclsemi password:ACL8semi  //连接指定SSID、Password
    example:I(3618):STA connected to aclsemi       //连接成功
    example:I(3658):STA got ip                     //获取IP地址

.. note:: 客户编译命令需指定相应PROJECT和PROJECT_LIBS ``make bk7256 PROJECT=examples/wifi/sta_connect PROJECT_LIBS=bk7256_app``

:link_to_translation:`en:[English]`

