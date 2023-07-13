EPA
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了方便验证EPA功能，SDK提供了打开EPA功能的宏开关，将EPA_ENABLE_FLAG置为1，打开EPA功能。



2、代码路径
--------------------------
	路径: ``\middleware\boards\bk7235\vnd_cal\vnd_cal.c``

3、EPA配置简介
--------------------------
1. 修改默认晶体值

 - 外置EPA方案，需要根据客户的板子调整晶体值，使频偏在-20ppm到20ppm内，将此时对应的晶体值給vnd_cal.c中被EPA_ENABLE_FLAG宏包着的默认晶体值DEFAULT_TXID_XTAL

2. 修改功率表

 - 外置EPA方案，需要根据客户的板子调整被宏EPA_ENABLE_FLAG包着的gtxpwr_tab_def_b，gtxpwr_tab_def_g，gtxpwr_tab_def_n_40，gtxpwr_tab_def_ble默认功率表
