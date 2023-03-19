set log
==========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	通过串口工具和BK72XX芯片的开发板连接，在串口工具的输入窗口，可以给开发板发cli命令来控制内部软件的动作，以及查询内部状态。可通过cli命令设置log输出等级和模式。

2 代码路径
-------------------------------------
	demo路径：``components\bk_cli\cli_main.c``

3 log设置相关cli命令简介
-------------------------------------

	demo运行依赖的宏配置：

	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description       |                  File                      |  value  |
	+======================================+========================+============================================+=========+
	|CONFIG_SHELL_ASYNCLOG                 |support async log       | ``middleware\soc\bk7256\bk7256.defconfig`` |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+

	demo支持的命令如下表：

	+----------------------------------------+-----------------------------------------------+---------------------------------------+
	|             Command                    |            Param                              |              Description              |
	+========================================+===============================================+=======================================+
	| help                                   | none                                          | output all support command lines      |
	+----------------------------------------+-----------------------------------------------+---------------------------------------+
	|                                        | none: if param none,show current log setting  |                                       |
	|                                        +-----------------------------------------------+                                       |
	|                                        | [echo(0,1)]: 1:log echo open 0:log echo close |                                       |
	|                                        +-----------------------------------------------+                                       |
	| log [echo(0,1)] [level(0~5)]           | [level(0~5)]:log level 0-5                    | set whether input command echo        |
	|                                        +-----------------------------------------------+ set output level of the log           |
	| [sync(0,1)] {Whitelist(0,1)]           | [sync(0,1)]:1:sync log; 0:async log           | set the log sync or asyn output       |
	|                                        +-----------------------------------------------+ set the log white list mechanism to   |
	|                                        | [Whitelist(0,1)]: 1:while list 0:black list   | control the log output of the module  |
	+----------------------------------------+-----------------------------------------------+---------------------------------------+
	|                                        | none: show current while/black list           |                                       |
	| modlog [tag_name] [on/off]             +-----------------------------------------------+  add/remove tag to/from the list      |
	|                                        | [tag_name]:function module tag                |                                       |
	|                                        +-----------------------------------------------+  note: if param is none, show current |
	|                                        | [on/off]: add/remove tag to/from the list     |  while/black list                     |
	+----------------------------------------+-----------------------------------------------+---------------------------------------+
	| loginfo                                | none                                          | log statistics                        |
	+----------------------------------------+-----------------------------------------------+---------------------------------------+

log 命令详细说明：

   ::

	log [echo(0,1)] [level(0~5)] [sync(0,1)] [Whitelist(0,1)]
	开发板log输出的设置命令。如果不带任何参数，显示当前的设置值。这个命令可以带1~4个参数，分别设置以下功能。如果想设置第4个参数，那么前面三个参数不能缺省。

	echo：0~1，控制回显的开关。0：关闭命令行的命令回显，1：打开命令回显功能。

	level：0~5，控制哪个等级的log可以输出。0：关闭log输出，1：输出Error log，2：输出Warning log，3：输出Infor log，4：输出Debug log，5：输出Verbose log。内部默认为3。比如level 设置为3，只能输出1，2，3等级的log，如果设置为5，可以输出任何等级的log。

	Sync：0~1，控制log输出模式。  0：异步输出，该模式下，log多的时候，可能丢log，但是不影响系统的性能。  1：同步输出，该模式下，log多也不会丢，但是因为在打印log的task中等待log输出，所以会影响task的性能，以及系统的性能。
	如果用串口工具调试时，如果某条命令的执行输出很重要，可以在该命令前打开同步log，执行命令后，再关闭同步log。

	Whitelist：0~1，log输出模块的黑名单/白名单工作模式设置，控制哪些模块能输出log，哪些模块不能输出log。0：黑名单，1：白名单。
	这个和modlog 命令配合使用，来开关某个模块的log打印。
	如果选择白名单机制，只有用modlog 命令on的模块，才可以输出log，否则不能输出log。
	如果选择用黑名单机制，只有用modlog命令off的模块，log输出才会被关闭，否则默认能输出log。
	系统开机默认是黑名单机制，也就是默认每个模块都可以输出log。

log 等级的含义:

   ::

	#define BK_LOG_NONE    0 /*!< No log output */
	#define BK_LOG_ERROR   1 /*!< Critical errors, software module can not recover on its own */
	#define BK_LOG_WARN    2 /*!< Error conditions from which recovery measures have been taken */
	#define BK_LOG_INFO    3 /*!< Information messages which describe normal flow of events */
	#define BK_LOG_DEBUG   4 /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
	#define BK_LOG_VERBOSE 5 /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */

4 演示介绍
-------------------------------------

SDK工程默认设置为 log 1 3 0，即开启回显，log等级设置为3，异步log输出。

1、回显   完成设置后下一条命令生效

   ::

	[15:21:00.014]发→◇log 0 3 0
	□
	[15:21:00.014]收←◆log 0 3 0
	log: echo 0, level 3, sync 0, white_list 0.

	$
	[15:21:00.969]发→◇log 0 3 0
	□
	[15:21:00.974]收←◆log: echo 0, level 3, sync 0, white_list 0.

	$
	[15:21:04.914]发→◇log 1 3 0
	□
	[15:21:04.918]收←◆log: echo 1, level 3, sync 0, white_list 0.

	[15:21:06.475]发→◇log 1 3 0
	□
	[15:21:06.478]收←◆log 1 3 0
	log: echo 1, level 3, sync 0, white_list 0.



2、设置log等级

   ::

	[15:25:18.779]发→◇log 1 0 0      //关闭log输出
	□
	[15:25:18.782]收←◆log 1 0 0
	log: echo 1, level 0, sync 0, white_list 0.

	[15:25:29.299]发→◇scan
	□
	[15:25:29.303]收←◆scan
	CMDRSP:OK
	[15:26:05.929]收←◆
	$

3、设置同步log

   ::

	[15:30:50.243]发→◇log 1 3 1
	□
	[15:30:50.247]收←◆log 1 3 1
	log: echo 1, level 3, sync 1, white_list 0.

4、黑名单模式添加wifid到黑名单，查看当前黑名单

   ::

	[10:33:37.287]发→◇log 1 3 0 0
	□
	[10:33:37.291]收←◆log 1 3 0 0
	log: echo 1, level 3, sync 0, white_list 0.

	$
	[10:33:37.389]收←◆
	$
	[10:33:41.055]发→◇log
	□
	[10:33:41.058]收←◆log
	log: echo 1, level 3, sync 0, white_list 0.

	$
	[10:33:41.157]收←◆
	$
	[10:33:53.598]发→◇modlog wifid off
	□
	[10:33:53.603]收←◆modlog wifid off

	$
	[10:33:53.700]收←◆
	$
	[10:34:01.407]发→◇modlog
	□
	[10:34:01.410]收←◆modlog
	Usage: modlog tag_name on/off
	wifid

	$
	[10:34:01.509]收←◆
	$
	[10:34:01.798]收←◆tempd:I(30520):change config, interval=15, threshold=20, dist=0

	[10:34:07.231]发→◇scan
	□
	[10:34:07.234]收←◆scan
	wifi:I(35958):scaning
	wifi:I(35958):init 1st vif
	rwnx:I(35960):PHY features: [NSS=1][CHBW=40][LDPC][VHT][HE][BFMEE][MUMIMORX]

	!!some LOGs discarded!!
	rwnx:I(35960):HT supp 1, VHT supp 0, HE supp 0
	wpa:I(35964):sizeof(wpa_supplicant)=1000
	hitf:I(35964):add vif0, type=2, status=0
	wpa:I(35966):wpa_dInit
	hitf:I(35966):del hw key idx=0
	hitf:I(35966):del hw key idx=0
	hitf:I(35966):del hw key idx=0
	hitf:I(35968):del hw key idx=0
	hitf:I(35968):del hw key idx=0
	hitf:I(35968):del hw key idx=0
	wpa:I(35970):DISCONNECTED -> INACTIVE
	wpa:I(35970):INACTIVE -> DISCONNECTED
	lwip:I(35970):enter low level!
	lwip:I(35970):mac c8:47:8c:6e:5c:9d
	lwip:I(35970):leave low level!
	lwip:I(35970):add vif0
	wifi:I(35970):scan all APs
	wpa:I(35970):Setting scan request: 0.000000 sec
	CMDRSP:OK

	$wpa:I(35970):wpa_supplicant_scan
	wpa:I(35970):DISCONNECTED -> SCANNING
	wpa:I(35970):use ssids from scan command
	wpa:I(35970):add scan work
	wpa:I(35972):scan work start, deinit=0
	wpa:I(35972):req driver to scan
	wpa:I(35972):Scan specified BSSID ff:ff:ff:ff:ff:ff ,wpa_driver_scan2
	hitf:I(35972):send scan req to driver

	$
	[10:34:07.817]收←◆wpa:I(36542):scan completed in 0.568000 seconds
	hitf:I(36542):get scan result:36
	wpa:I(36546):SCANNING -> DISCONNECTED
	cli:I(36546):rx event <1 0>
	wifi:I(36546):scan found 36 AP
	              SSID                     BSSID         RSSI chan security
	-------------------------------- -----------------   ---- ---- ---------

	aclsemi_5G                       74:50:4e:3f:84:d1    -37  1 WPA2-AES

5、白名单模式添加wifid到白名单，查看当前白名单

   ::

	[10:35:25.839]发→◇log 1 3 0 1
	□
	[10:35:25.843]收←◆log 1 3 0 1
	log: echo 1, level 3, sync 0, white_list 1.

	$
	[10:35:25.941]收←◆
	$
	[10:35:27.894]发→◇modlog
	□
	[10:35:27.898]收←◆modlog
	Usage: modlog tag_name on/off

	$
	$
	[10:35:31.050]发→◇modlog wifid on
	□
	[10:35:31.050]收←◆modlog wifid on

	$
	[10:35:31.150]收←◆
	$
	[10:35:32.638]发→◇modlog
	□
	[10:35:32.643]收←◆modlog
	Usage: modlog tag_name on/off
	wifid

	$
	[10:35:32.741]收←◆
	$
	[10:35:41.871]发→◇scan
	□
	[10:35:41.880]收←◆scan
	CMDRSP:OK

	$
	$wifid:I(130608):ht in scan
	wifid:W(130608):scanu_start_req:src_id = 13,dur = 0,chan_cnt = 13,ssid_len = 0,ssid = ,bssid = 0xffff-ffff-ffff
	wifid:W(130610):scan_start_req,vif = 0,chan_cnt = 13

	[10:35:41.921]收←◆wifid:I(130652):scan_chan_end,scan_idx=0,freq=2412MHz

	[10:35:41.971]收←◆wifid:I(130696):scan_chan_end,scan_idx=1,freq=2417MHz

	[10:35:42.011]收←◆wifid:I(130740):scan_chan_end,scan_idx=2,freq=2422MHz

	[10:35:42.051]收←◆wifid:I(130784):scan_chan_end,scan_idx=3,freq=2427MHz

	[10:35:42.101]收←◆wifid:I(130828):scan_chan_end,scan_idx=4,freq=2432MHz

	[10:35:42.141]收←◆wifid:I(130872):scan_chan_end,scan_idx=5,freq=2437MHz

	[10:35:42.190]收←◆wifid:I(130916):scan_chan_end,scan_idx=6,freq=2442MHz

	[10:35:42.231]收←◆wifid:I(130958):scan_chan_end,scan_idx=7,freq=2447MHz

	[10:35:42.271]收←◆wifid:I(131002):scan_chan_end,scan_idx=8,freq=2452MHz

	[10:35:42.321]收←◆wifid:I(131046):scan_chan_end,scan_idx=9,freq=2457MHz

	[10:35:42.361]收←◆wifid:I(131090):scan_chan_end,scan_idx=10,freq=2462MHz

	[10:35:42.401]收←◆wifid:I(131134):scan_chan_end,scan_idx=11,freq=2467MHz

	[10:35:42.453]收←◆wifid:I(131178):scan_chan_end,scan_idx=12,freq=2472MHz
	wifid:W(131178):scan_chan_end,scan end
	wifid:W(131178):scanu_confirm:status = 0,req_type =0,upload_cnt = 110
	              SSID                     BSSID         RSSI chan security
	-------------------------------- -----------------   ---- ---- ---------

	aclsemi_5G                       74:50:4e:3f:84:d1    -38  1 WPA2-AES






