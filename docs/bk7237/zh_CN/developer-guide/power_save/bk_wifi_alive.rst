设置WIFI低压保活说明
=======================================================


设置WIFI低压保活调用函数和调用顺序说明
--------------------------------------------------------------
- 1. 当使用BT配置完网络，不要使用BT时，可以把它，这样可以达到更低的功耗

    bk_ble_deinit();

    关闭BT

- 2.bk_wifi_send_listen_interval_req(10);

    设置DTIM10(1:DTIM1)

- 3. bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

    设置睡眠模式，进入低压睡眠。

- 4. demo_sta_app_init("a", "b");

    连接路由器（a:路由器的名字;b:路由器的密码）

- 5. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);

    向系统中投应用程序的票。

(备注：一定需要设备连接上路由器，拿到IP地址，才调用以上函数bk_pm_module_vote_sleep_ctrl()投APP的票，不然移动设备不能网络唤醒测试设备)


备注：

 - 1. BT和WIFI的票，BT和WIFI模块内部进入睡眠后自己投上，SDK内部做好，应用程序不用关注

 - 2. 当系统中BT或WIFI模块没有上电时，系统会自动给这两个模块投上sleep的票。

:link_to_translation:`en:[English]`

