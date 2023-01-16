OS
==========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	介绍OS相关的cli命令使用。

2 代码路径
-------------------------------------
	demo路径：``components\bk_cli\cli_os.c`` ``components\bk_cli\cli_mem.c``

3 cli命令简介
-------------------------------------

	demo运行依赖的宏配置：

	+--------------------------------------+------------------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description                   |                  File                      |  value  |
	+======================================+====================================+============================================+=========+
	|CONFIG_FREERTOS                       |support FreeRTOS                    | ``middleware\soc\bk7236\bk7236.defconfig`` |    y    |
	+--------------------------------------+------------------------------------+--------------------------------------------+---------+
	|CONFIG_FREERTOS_V9                    |support FreeRTOS V9                 | ``middleware\soc\bk7236\bk7236.defconfig`` |    n    |
	+--------------------------------------+------------------------------------+--------------------------------------------+---------+
	|CONFIG_FREERTOS_V10                   |support FreeRTOS V10                | ``middleware\soc\bk7236\bk7236.defconfig`` |    y    |
	+--------------------------------------+------------------------------------+--------------------------------------------+---------+
	|CONFIG_CUSTOMIZE_HEAP_SIZE            |customize freerots heap size(byte)  | ``middleware\soc\bk7236\bk7236.defconfig`` | 217088  |
	+--------------------------------------+------------------------------------+--------------------------------------------+---------+


	demo支持的命令如下表：

	+----------------------------------------+---------------------+--------------------------------------------+
	|             Command                    |       Param         |              Description                   |
	+========================================+=====================+============================================+
	| tasklist                               | none                | show task list                             |
	+----------------------------------------+---------------------+--------------------------------------------+
	| cpuload                                | none                | show the load of task on the cpu           |
	+----------------------------------------+---------------------+--------------------------------------------+
	| backtrace                              | none                | show task stack info and call relationship |
	+----------------------------------------+---------------------+--------------------------------------------+
	| osinfo                                 | none                | show tasklist/cpuload/backtrace            |
	+----------------------------------------+---------------------+--------------------------------------------+
	| assert                                 | none                | active assert                              |
	+----------------------------------------+---------------------+--------------------------------------------+
	| memstack                               | none                | show the usage of the current task stack   |
	+----------------------------------------+---------------------+--------------------------------------------+
	| memshow                                | none                | show the usage of the current heap         |
	+----------------------------------------+---------------------+--------------------------------------------+
	|                                        |{addr}: start address|                                            |
	| memdump {addr} {length}                +---------------------+ output data of specified length from the   |
	|                                        |{length}:dump length | specified address                          |
	+----------------------------------------+---------------------+--------------------------------------------+

4 演示介绍
-------------------------------------

  1、显示os任务列表

	tasklist

   ::

	[10:43:38.934]发→◇tasklist
	□
	[10:43:38.938]收←◆tasklist
	>>>>dump task list begin.
	task           state   pri     water   no
	cli            	X	5	539	19
	IDLE           	R	0	328	3
	Tmr Svc        	B	5	688	4
	wpas_thread    	B	4	677	13
	tcp/ip         	B	7	423	6
	media_app_threa	B	3	955	15
	storage_task_th	B	5	951	17
	<<<<dump task list end.``

  2、显示cpu负载

	cpuload

   ::

	[10:48:26.552]发→◇cpuload
	□
	[10:48:26.555]收←◆cpuload
	>>>>dump task runtime begin.
	cli            	6		<1%
	IDLE           	447273		99%
	wpas_thread    	7		<1%
	Tmr Svc        	0		<1%
	tcp/ip         	0		<1%
	storage_task_th	0		<1%
	media_major_thr	1		<1%
	core_thread    	54		<1%
	kmsgbk         	8		<1%
	event          	1		<1%
	tempd          	1		<1%
	ble            	38		<1%
	syswq          	0		<1%
	media_app_threa	0		<1%
	<<<<dump task runtime end.

  3、显示任务栈调用信息

	backtrace     // 显示当前所有任务名称、任务栈空间地址范围、当前栈指针位置、栈溢出标志、栈调用关系

   ::

	[11:00:56.436]发→◇backtrace
	□
	[11:00:56.440]收←◆ba>>>>dump task backtrace begin.
	task               stack_addr              top        size   overflow   backtrace
	IDLE               [0x20001c6c ~ 0x2000226c]   0x2000218c   1536   0          1bc88 1bc50 1bafc c8d36 1800 1be1e
	cli                [0x30030390 ~ 0x30031390]   0x3003127c   4096   0          1000 1a964 1c3e4 45132 2057e 1f252 a5a4
	tcp/ip             [0x30025640 ~ 0x30025e40]   0x30025cdc   2048   0          1e37a 1e374 1bafc 125b74 1880 3e2e4 151208 3e2e4 c8d04 44822 3e2e4 3684c 191a0a 446a4 38ca2
	wpas_thread        [0x3002a1d0 ~ 0x3002b1d0]   0x3002b07c   4096   0          1e37a 1e374 1bafc 125b74 1880 10ebaa eb5a0 1388 c7b3e 1138ec 44822 10ebaa eb5a0 2710 10ea3c 662 eb5a0 1112f2
	Tmr Svc            [0x20001000 ~ 0x20001c00]   0x20001b2c   3072   0          1c6ce 136000 1c6c8 1bafc 125b74 c9816 134924 1880 c7aca c7aca 1c5f8
	syswq              [0x30025fe0 ~ 0x300263e0]   0x300262dc   1024   0          1e5f2 1e5ec 1bafc 125b74 1880 445a4 f8bf2
	media_app_threa    [0x3002c970 ~ 0x3002d970]   0x3002d85c   4096   0          1e37a 1e374 1bafc 125b74 1880 116 44822 32524
	storage_task_th    [0x3002f130 ~ 0x30030130]   0x3003000c   4096   0          1e37a 1e374 1bafc 125b74 1880 116 13f000 13f000 44822 33a4c
	media_major_thr    [0x3002b6a0 ~ 0x3002c6a0]   0x3002c57c   4096   0          1e37a 33a2a 1e374 1bafc 125b74 1880 116 44822 340a2 15088c 35cfe
	core_thread        [0x30028b50 ~ 0x30029350]   0x300291ec   2048   0          1e37a 1e374 1bafc 125b74 187000 187000 1880 187000 187000 178000 10034 187000 187000 187000 186ef0 187000 44822 186ef0 187000 b9e58 186ef0 187000 fabb6 454c2 454c2
	kmsgbk             [0x30027460 ~ 0x30028460]   0x3002834c   4096   0          1e5f2 1e5ec 1bafc 125b74 1880 187000 10034 187000 187000 187000 445a4 187000 187000 187000 fa71e
	tempd              [0x30026ca0 ~ 0x300270a0]   0x30026f4c   1024   0          1e37a 1e374 1bafc 125b74 161000 161000 1880 c7aca 136000 c7aca 160938 136000 44822 14a 148 136000 68674 154 149a5a4
	ble                [0x3002dab0 ~ 0x3002e6b0]   0x3002e56c   3072   0          1e37a c7116 1e374 1bafc 125b74 1880 17c6e0 4e200 c8476 44822 5014ee cb4a0 170634 cb586 cab32
	event              [0x300243b0 ~ 0x300253b0]   0x3002525c   4096   0          1e37a 1e374 1bafc 125b74 1880 10104 13e000 44822 13e000 4552a 13e000 5d26a
	<<<<dump task backtrace end.



  4、 显示os相关信息

	osinfo   //包括任务栈调用关系、任务列表、cpu负载

   ::

	[10:49:26.817]发→◇osinfo
	□
	[10:49:26.820]收←◆osinfo
	>>>>dump task backtrace begin.
	task               stack_addr              top        size   overflow   backtrace
	IDLE               [0x20001c6c ~ 0x2000226c]   0x2000218c   1536   0          1bc88 1bc50 1bafc 74a7c 1800 1be1e
	cli                [0x30030390 ~ 0x30031390]   0x3003121c   4096   0          135bd4 450ba 1000 1a964 1c3e4 45132 216b8 2057e 1f252 a5a4
	tcp/ip             [0x30025640 ~ 0x30025e40]   0x30025cdc   2048   0          1e37a 1e374 1bafc 125b74 1880 3e2e4 151208 3e2e4 74a4a 44822 3e2e4 3684c e9496 446a4 38ca2
	wpas_thread        [0x3002a1d0 ~ 0x3002b1d0]   0x3002b07c   4096   0          1e37a 1e374 1bafc 125b74 1880 10ebaa eb5a0 1388 73796 1138ec 44822 10ebaa eb5a0 2710 10ea3c 3b0 eb5a0 1112f2
	Tmr Svc            [0x20001000 ~ 0x20001c00]   0x20001b2c   3072   0          1c6ce 136000 1c6c8 1bafc 125b74 75440 134924 1880 736f4 736f4 1c5f8
	syswq              [0x30025fe0 ~ 0x300263e0]   0x300262dc   1024   0          1e5f2 1e5ec 1bafc 125b74 1880 445a4 f8bf2
	tempd              [0x30026ca0 ~ 0x300270a0]   0x30026f4c   1024   0          1e37a 1e374 1bafc 125b74 161000 161000 1880 736f4 136000 736f4 160938 136000 44822 14a 148 136000 68674 154 149a5a4
	ble                [0x3002dab0 ~ 0x3002e6b0]   0x3002e56c   3072   0          1e37a 1260f2 1e374 1bafc 125b74 1880 17c6e0 4e200 740c4 44822 2e63aa cb4a0 170634 cb586 cab32
	media_app_threa    [0x3002c970 ~ 0x3002d970]   0x3002d85c   4096   0          1e37a 1e374 1bafc 125b74 1880 116 44822 32524
	storage_task_th    [0x3002f130 ~ 0x30030130]   0x3003000c   4096   0          1e37a 1e374 1bafc 125b74 1880 116 13f000 13f000 44822 33a4c
	media_major_thr    [0x3002b6a0 ~ 0x3002c6a0]   0x3002c57c   4096   0          1e37a 33a2a 1e374 1bafc 125b74 1880 116 44822 340a2 15088c 35cfe
	core_thread        [0x30028b50 ~ 0x30029350]   0x300291ec   2048   0          1e37a 1e374 1bafc 125b74 187000 187000 1880 187000 187000 178000 10034 187000 187000 187000 186ef0 187000 44822 186ef0 187000 b9e58 186ef0 187000 fabb6 454c2 454c2
	kmsgbk             [0x30027460 ~ 0x30028460]   0x3002834c   4096   0          1e5f2 1e5ec 1bafc 125b74 1880 187000 10034 187000 187000 187000 445a4 187000 187000 187000 fa71e
	event              [0x300243b0 ~ 0x300253b0]   0x3002525c   4096   0          1e37a 1e374 1bafc 125b74 1880 10104 13e000 44822 13e000 4552a 13e000 5d26a
	<<<<dump task backtrace end.

	>>>>dump task list begin.
	task           state   pri     water   no
	cli            	X	5	536	19
	IDLE           	R	0	328	3
	wpas_thread    	B	4	677	13
	Tmr Svc        	B	5	688	4
	tcp/ip         	B	7	423	6
	media_major_thr	B	3	841	14
	core_thread    	B	7	295	10
	kmsgbk         	B	6	895	9
	event          	B	1	847	5
	syswq          	B	6	191	7
	tempd          	B	5	89	8
	ble            	B	5	565	16
	media_app_threa	B	3	955	15
	storage_task_th	B	5	951	17
	<<<<dump task list end.

	>>>>dump task runtime begin.
	cli            	10		<1%
	IDLE           	477400		99%
	Tmr Svc        	0		<1%
	tcp/ip         	0		<1%
	wpas_thread    	7		<1%
	core_thread    	54		<1%
	kmsgbk         	8		<1%
	event          	1		<1%
	syswq          	0		<1%
	tempd          	1		<1%
	ble            	39		<1%
	media_app_threa	0		<1%
	storage_task_th	0		<1%
	media_major_thr	1		<1%
	<<<<dump task runtime end.

  5、主动assert

	assert // 主动assert，输出当前通用寄存器和关键CSR的值、BTWF的寄存器数据和memory数据。

   ::

	[11:07:05.111]发→◇assert
	□
	[11:07:05.113]收←◆assert
	(2013834)Assert at: cli_assert_dump_cmd:43
	***********************************************************************************************
	***********************************user except handler begin***********************************
	***********************************************************************************************
	Current regs:
	1 ra x 0x2168e
	2 sp x 0x300312c0
	5 t0 x 0x77
	6 t1 x 0x1ceba
	7 t2 x 0x30056bd4
	8 fp x 0x1
	9 s1 x 0x30056b0c
	10 a0 x 0x1
	11 a1 x 0xa
	12 a2 x 0x2c
	13 a3 x 0x30056c93
	14 a4 x 0x180031
	15 a5 x 0x2c
	16 a6 x 0x0
	17 a7 x 0x0
	18 s2 x 0x30021000
	19 s3 x 0x30059000
	20 s4 x 0xc8
	21 s5 x 0x135be0
	22 s6 x 0x30058d70
	23 s7 x 0x22
	24 s8 x 0x0
	25 s9 x 0x30056b0b
	26 s10 x 0x30056b0c
	27 s11 x 0x1
	28 t3 x 0xffffffff
	29 t4 x 0x27
	30 t5 x 0xe0
	31 t6 x 0x2
	32 pc x 0x2168a
	833 mstatus x 0x80
	838 mtvec x 0x1d300
	897 mscratch x 0x30031248
	898 mepc x 0x12644a
	899 mcause x 0x77
	900 mtval x 0x0
	2058 mdcause x 0x0

	MAC is in doze, open maccore clock
	>>>>MAC PL reg dump begin, reg start=49108000, reg end=4910859c
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	........
	........

  6、memstack  显示任务栈的当前使用情况

   ::

	[10:38:13.578]发→◇memstack
	□
	[10:38:13.598]收←◆memstack
	task               stack_size   address                  peak_used    current_used    water
	cli                4096         0x300303e0- 0x300313e0   0xbf4        0x174           0x40c
	IDLE               1536         0x20001c6c- 0x2000226c   0x238        0xe0            0x3c8
	Tmr Svc            3072         0x20001000- 0x20001c00   0x140        0xd4            0xac0
	tcp/ip             2048         0x30025690- 0x30025e90   0x194        0x164           0x66c
	kmsgbk             4096         0x300274b0- 0x300284b0   0x114        0x114           0xeec
	core_thread        2048         0x30028ba0- 0x300293a0   0x164        0x164           0x69c
	wpas_thread        4096         0x3002a220- 0x3002b220   0x154        0x154           0xeac
	media_app_threa    4096         0x3002c9c0- 0x3002d9c0   0x114        0x114           0xeec
	storage_task_th    4096         0x3002f180- 0x30030180   0x124        0x124           0xedc
	media_major_thr    4096         0x3002b6f0- 0x3002c6f0   0x2dc        0x124           0xd24
	event              4096         0x30024400- 0x30025400   0x164        0x154           0xe9c
	ble                3072         0x3002db00- 0x3002e700   0x32c        0x144           0x8d4
	tempd              1024         0x30026cf0- 0x300270f0   0x29c        0x154           0x164
	syswq              1024         0x30026030- 0x30026430   0x104        0x104           0x2fc
	thread_stack: used:42496, the rest:38236


	arm_mode_stack: used:0, the rest:0
	total_stack_space: used:42496, the rest:38236

  7、memshow  显示heap的使用情况

   ::

	[10:36:41.587]发→◇memshow
	□
	[10:36:41.587]收←◆memshow
	name    total    free    minimum    peak
	heap    217088   163200  153216     63872
	psram   1048576  1048560 1048560    16

  8、memdump  将指定地址的数据输出

   ::

	[10:52:09.129]发→◇memdump 0x0 16    //显示从flash0x0地址开始的16个字节数据
	□
	[10:52:09.139]收←◆memdump 0x0 16
	f3 22 20 34 63 84 02 00 6f 00 20 06 f3 27 a0 7c

	$
	[10:52:09.229]收←◆
	$
	[10:53:02.349]发→◇memdump 0x44000400 16   //显示GPIO0-3的配置信息
	□
	[10:53:02.349]收←◆memdump 0x44000400 16
	78 03 00 00 78 03 00 00 78 03 00 00 78 03 00 00

