Wi-Fi 扫描示例工程说明
====================================================
本工程展示了BEKEN Wi-Fi STA模式下启动扫描空中SSID并返回结果的示例.

代码流程
------------------------------------------
- demo_start()入口:
   A) 创建线程wifi_scan_thread
- wifi_scan_thread
   B) 调用事件回调函数 ``bk_event_register_cb``
   C) 调用函数 ``bk_wifi_scan_start`` 开始扫描
   D) 调用函数 ``show_scan_ap_result`` 上报扫描结果包括AP Num、SSID、BSSID、RSSI、Channel、加密方式等
- 删除线程

扫描命令示例
-------------------------------------------
- net sta enable
- net sta scan once
- net sta scan result

:link_to_translation:`en:[English]`

