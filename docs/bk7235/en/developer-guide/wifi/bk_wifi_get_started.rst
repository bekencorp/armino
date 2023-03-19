Wi-Fi Quick Get-Started
====================================================

:link_to_translation:`zh_CN:[中文]`

The example demo of three common BEKEN Wi-Fi usage scenarios is provided in the directory ``projects/examples/wifi``. User can quickly learn about Wi-Fi usage based on the demo.

- ap
- scan
- sta_connect

.. note:: After above three kinds of Demo are compiled and downloaded, user can automatically start SoftAP、scan and connect

AP/SoftAP
-------------------------------------------------------
AP example demo shows the process of setting up SoftAP based on BEKEN Wi-Fi module.

- Compile command: ``make bk7256 PROJECT=examples/wifi/ap``
- File path: ``build/ap/bk7256/all-app.bin``
- Parameter configuratioin: ``projects/examples/wifi/ap/main/Kconfig.projbuild``

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
      default "255.255.255.0"                      //Default value can be customized
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
Scam example demo describes the basic Wi-Fi scan process, including full-channel scan and specific SSID scan.

- Compile command: ``make bk7256 PROJECT=examples/wifi/scan``
- File path: ``build/scan/bk7256/all-app.bin``
- Parameter configuratioin: ``projects/examples/wifi/scan/main/Kconfig.projbuild``

::

    menu "Example Configuration"
    config EXAMPLE_SCAN_ALL_SSID
      bool "Scan All SSID"
      default y                                    //full-channel scan

    config EXAMPLE_WIFI_SSID
      string "WiFi SSID"
    depends on !EXAMPLE_SCAN_ALL_SSID
      default "aclsemi"                            //specific SSID scan
    endmenu

- main log

::

    wifi:I(1134):scan found 4 AP                   //specific SSID scan
    SSID                             BSSID               RSSI chan security
    -------------------------------- -----------------   ---- ---- ---------
    aclsemi                          74:50:4e:3f:48:b0    -29  6 WPA2-AES
    aclsemi                          74:50:4e:3f:84:d0    -32  1 WPA2-AES
    aclsemi                          74:50:4e:3f:99:50    -33  6 WPA2-AES
    aclsemi                          74:50:4e:3f:82:50    -58 11 WPA2-AES
    wifi:I(1136):scan found 32 AP                  //full-channel scan
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

Sta_connect
-------------------------------------------------------
By configuring ssid and password in advance, sta_connect could achieve connect automatically after startup.

- Compile command: ``make bk7256 PROJECT=examples/wifi/sta_connect``
- File path: ``build/sta_connect/bk7256/all-app.bin``
- Parameter configuratioin: ``projects/examples/wifi/sta_connect/main/Kconfig.projbuild``

::

    menu "Example Configuration"
    config EXAMPLE_WIFI_SSID
      string "Wi-Fi SSID"
      default "aclsemi"                            //target SSID

    config EXAMPLE_WIFI_PASSWORD
      string "Wi-Fi Password"
      default "ACL8semi"                           //target SSID password
    endmenu

- main log

::

    wifi:I(308):wifi inited(1) ret(0)              //initialization completed after startup
    example:I(310):ssid:aclsemi password:ACL8semi  //connect specific SSID、Password
    example:I(3618):STA connected to aclsemi       //connect successfully
    example:I(3658):STA got ip                     //obtain IP address

.. note:: The client compile command should assign appropriate PROJECT and PROJECT_LIBS ``make bk7256 PROJECT=examples/wifi/sta_connect PROJECT_LIBS=bk7256_app``

