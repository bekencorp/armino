config wifi alive in low voltage sleep mode description
===============================================================

:link_to_translation:`zh_CN:[中文]`

config wifi alive using functions and the sequence illustrate
------------------------------------------------------------------
- 1. after you config the net work using BT, you not need bt again, please close it, which can achieve more better power save

  bk_ble_deinit();
  close BT

- 2.bk_wifi_send_listen_interval_req(10);

    set DTIM10(wakeup interval:1s);  if set 1 means DTIM1(wakeup interval:100ms)

- 3. bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);

    set sleep mode: low voltage

- 4. demo_sta_app_init("a", "b");

    conncet the ap（a:ap name;b:ap password）

- 5. bk_pm_module_vote_sleep_ctrl(PM_POWER_MODULE_NAME_APP,0x1,0x0);

    vote the app ticket

(comment：make sure the device connected the ap and get the ip address, then call the function: bk_pm_module_vote_sleep_ctrl(),otherwise it can not wakeup the device through the network)


comment::

 - 1. BT and WIFI tickets，which will be voted by themselves when they enter sleep. it has done them in SDK, the application not care them.

 - 2. when the WIFI and BT not power on, the system will auto vote their sleep tickets.

