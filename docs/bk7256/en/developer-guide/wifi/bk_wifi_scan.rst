Wi-Fi Scan Example
====================================================

:link_to_translation:`zh_CN:[中文]`

This chapter show the example of BEKEN Wi-Fi STA mode to start scanning the SSID around and return the results.

Source Code Process
------------------------------------------
- demo_start() entrance
   A) create thread: wifi_scan_thread
- wifi_scan_thread
   B) call callback function ``bk_event_register_cb``
   C) call function ``bk_wifi_scan_start`` to start scanning
   D) call function ``show_scan_ap_result`` to report results, including AP Num、SSID、BSSID、RSSI、Channel、encryption, etc
- delete thread

Application Example
-------------------------------------------
Command Example
+++++++++++++++++++++++++++++++++++++++++++
::

   net sta enable
   net sta scan once
   net sta scan result
   or just execute:
   scan

Log Example
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


