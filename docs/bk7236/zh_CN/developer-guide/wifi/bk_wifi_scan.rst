Wi-Fi 扫描示例工程说明
====================================================
本工程展示了BEKEN Wi-Fi STA模式下启动扫描空中SSID并返回结果的示例.

代码流程
------------------------------------------
- demo_start()入口
   A) 创建线程wifi_scan_thread
- wifi_scan_thread
   B) 调用事件回调函数 ``bk_event_register_cb``
   C) 调用函数 ``bk_wifi_scan_start`` 开始扫描
   D) 调用函数 ``show_scan_ap_result`` 上报扫描结果包括AP Num、SSID、BSSID、RSSI、Channel、加密方式等
- 删除线程

应用示例
-------------------------------------------
命令示例
+++++++++++++++++++++++++++++++++++++++++++
::

   net sta enable
   net sta scan once
   net sta scan result
   或者直接执行：
   scan

Log示例
++++++++++++++++++++++++++++++++++++++++++++
::

   scan
   wifi: I(67561682):scaning
   wifi: I(67561682):scan all APs
   wpa: I(67561682):Setting scan request: 0.000000 sec
   hitf: I(67562368):get scan result :32
   cli: I(67562372):rx event <1 0>
   wifi: I(67562374):scan found 32 AP
   SSID                             SSID                RSSI chan security
   -------------------------------- -----------------   ---- ---- ---------
   yt                               b0:48:7a:4f:11:1a    -44  6   NONE
   aclsemi                          74:50:4e:3f:99:50    -29  6   WPA2-AES
   PowerTest                        c8:3a:35:7f:7e:e0    -31 11   WPA2-AES
   aclsemi                          74:50:4e:3f:48:b0    -35  6   WPA2-AES
   aclsemi                          74:50:4e:3f:84:d0    -37  1   WPA2-AES
   D-Link                           70:62:b8:6d:61:90    -41  1   WPA2-MIX
   junbang                          02:8a:90:e9:d2:43    -44  1   WPA2-AES
   ChinaNet-HeLg                    cc:c2:e0:6e:0f:a9    -45 13   WPA2-MIX
   NETGEAR56                        6c:cd:d6:2c:10:2c    -50  1   WPA3-SAE

:link_to_translation:`en:[English]`

