Wi-Fi AP/STA/P2P Mode Example
=============================================

This chapter shows three modes of BEKEN Wi-Fi module: STA、SoftAP、P2P.

- Wi-Fi Enables STA mode
- Wi-Fi Enables SoftAP mode
- Wi-Fi Enables P2P Mode (under development)

Example and Configuration
--------------------------------------------------
STA and SoftAP modes are supported by default. The function of P2P need to enable macro ``CONFIG_COMPONENTS_P2P`` in file ``middleware/soc/bk72xx.defconfig`` .

.. Note:: Please contact BEKEN support team for further details about P2P function

Source Code Process
--------------------------------------------------
Wi-Fi STA Mode
+++++++++++++++++++++++++++++++++++++++++++++++++
V1
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The BEKEN Wi-Fi module runs a command to enter the corresponding mode. After entering ``net mode``, user can view the supported mode.
::

     net mode <mode>
          - net mode sta
          - net mode ap

The entry of STA mode is ``cmd_wlan_sta_exec`` which is located in ``properties/modules/bk_wifi_impl/wifi_wpa_cmd.c``. This function configures/obtains the corresponding configuration through set/get command respectively. The common ones are as follows:
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

On the basis of V1, BEKEN has completed the adaptation of second version of commands by customizing customer requirements. Comparing with original one, this version simplifies use of commands, a single command to achieve STA mode connection. The file locates at ``components/bk_cli/cli_wifi.c`` and it needs to enable macro ``CLI_CFG_WIFI=1``. The usage of STA mode is as follows:
::

     {"scan", "scan [ssid]", cli_wifi_scan_cmd}
     {"sta", "sta ssid [password][bssid][channel]", cli_wifi_sta_cmd}
     {"state", "state - show STA/AP state", cli_wifi_state_cmd}
     {"stop", "stop {sta|ap}", cli_wifi_stop_cmd}

     cmd example:
     sta BEKEN_TEST 12345678

.. note:: V2 version is compatible with V1 version

Wi-Fi SoftAP Mode
++++++++++++++++++++++++++++++++++++++++++++++++++
The entry of SoftAP mode is ``cmd_wlan_ap_exec`` in V1 version and source code is same as STA mode. The main configuration parameters are SSID and password.
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

Entry of V2 version is ``cli_wifi_ap_cmd`` which is located in ``components/bk_cli/cli_wifi.c``, the detailed introduction is as follows:
::

     {"ap", "ap ssid [password]", cli_wifi_ap_cmd}
     {"stop", "stop {sta|ap}", cli_wifi_stop_cmd}
     {"state", "state - show STA/AP state", cli_wifi_state_cmd}

     cmd example:
     ap BEKEN_TEST 12345678

.. Note:: BEKEN Wi-Fi module supports AP+STA coexist

Wi-Fi P2P Mode
+++++++++++++++++++++++++++++++++++++++++++++++++++
P2P Mode is still under development. The entry function ``cmd_wlan_p2p_exec`` can be used to configure the SSID of P2P and default name is ``beken p2p`` . The role of P2P is decided by negotiating, but could be assigned by the value of ``param.intent`` . The command to enable P2P function is as follows:
::

     net p2p enable

.. Note:: Please contact BEKEN support team for further details about P2P function


