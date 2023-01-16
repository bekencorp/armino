power manager
==========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	支持平台power manager（low volage 、deepsleep、normal sleep）。

2 代码路径
-------------------------------------
	demo路径：``components\bk_cli\cli_pwr.c``

3 cli命令简介
-------------------------------------

demo运行依赖的宏配置：

	+--------------------------------------+--------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description         |                  File                      |  value  |
	+======================================+==========================+============================================+=========+
	|CONFIG_MCU_PS                         |support low power feature | ``properties\soc\bk7236\bk7236.defconfig`` |    y    |
	+--------------------------------------+--------------------------+--------------------------------------------+---------+

demo支持的命令如下：

cmd line 1:
  pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]

   ::

	sleep_mode：代表休眠模式(参数范围：0-3): 0:normal sleep; 1:low voltage; 2:deep sleep;3:none
	wake_source：表示唤醒源（取值范围0-5）： 0：GPIO唤醒；1：RTC唤醒；2：WIFI或者BT唤醒；3：USB唤醒（BK7236不支持，其他项目支持）4：touch唤醒；5：none
	Vote1: vote2: vote3: 代表投的票（取值范围8(BT)，9(WIFI)，12(APP)
	param1：代表休眠时间;
	param2：在RTC唤醒时没有用该参数，默认是0;
	param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准。

EXAMPLE:

  测试步骤：

   ::

	1） 开机启动5s之后（开机之后，BT、WIFI、CPU做初始化）；
	2） 输入命令: pm 2 1 8 9 8 3000 0 1000000000（让系统进入deepsleep，使用RTC进行唤醒，3000表示设置的唤醒时间，可自行修改，单位为ms)；
	3）系统到达设定的的唤醒时间后，系统会被唤醒。
	4） 在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	5） 同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗

  期望结果：

   ::

	1） 在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	2） 同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗。


cmd line 2:
  pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]

   ::

	备注：其他参数参考cmd line 1
	param1：代表touch的channel号;
	param2：在touch唤醒时没有用该参数，默认是0;
	param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准。

EXAMPLE:

  测试步骤：

   ::

	1） 开机启动5s之后（开机之后，BT|、WIFI、CPU做初始化）；
	2） 输入以下两条命令：
	A. touch_single_channel_calib_mode_test 2 （数字2代表touch的channel号，现在测试版本默认是2）；
	B. pm 2 4 8 9 8 2 0 1000000000（让系统进入deepsleep，使用touch进行唤醒）；
	C.手动触摸touch,可以把系统从deepsleep状态中唤醒
	3）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗。

  期望结果：

   ::

	1）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒（使用touch）之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗。

cmd line 3:
  pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]

   ::

	备注：其他参数参考cmd line 1
	param1：代表gpio的id号；
	param2：代表唤醒方式（0：低电平唤醒，1：高电平唤醒，2：上升沿唤醒，2：下降沿唤醒）;
	param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准。

EXAMPLE:

  测试步骤：

   ::

	1） 开机启动5s之后，（开机之后，BT|、WIFI、CPU做初始化）；
	2） 输入以下三条命令：
	A.  pm 1 0 8 9 12 9 0 1000000000（让系统进入deepsleep，使用gpio进行唤醒，9表示gpio的id)；
	B.如果希望某个GPIO从高电平唤醒，则测试前把该GPIO初始状态接为低电平，唤醒时接高电平；
	C.如果希望某个GPIO从低电平唤醒，则测试前把该GPIO初始状态接为高电平，唤醒时接低电平；
	D.如果希望某个GPIO从上升沿唤醒，则测试前把该GPIO初始状态接为低电平，唤醒时接高电平；；
	E.如果希望某个GPIO从下降沿唤醒，则测试前不要把该GPIO初始状态接为高电平，唤醒时接低电平；
	3）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个deep sleep到唤醒过程的功耗。

  期望结果：

   ::

	1）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗。

cmd line 4:
  pm_vote [pm_sleep_mode] [pm_vote] [pm_vote_value] [pm_sleep_time]

   ::

	pm_sleep_mode:代表休眠模式(取值范围0-3)：0：normal sleep;1:low voltage;2:deep sleep;3:none
	pm_vote:代表投的票（取值范围8，9，12）8：BT；9：WIFI；12：APP应用
	pm_sleep_time：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准

EXAMPLE:

  测试步骤：

   ::

	测试步骤如下：
	1） 开机启动5s之后，（开机之后，BT|、WIFI、CPU做初始化）；
	2） 输入以下三条命令：
	A.  sta 123(路由器名称) 12345678（路由器密码）（连接路由器，WIFI会进入sleep wakeup流程）（如果需要额外操作，请结威补充）
	B.  蓝牙默认进入sleep，（如果需要额外操作，请周灵补充）
	C.  pm_vote 1 12 1 0（投app票）
	3）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。


  期望结果：

   ::

	1）系统可以周期性的休眠和唤醒
	2）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；

cmd line 5:
  pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [parm3]

   ::

	备注：其他参数参考cmd line 1的参数说明；
	Vote1: vote2: vote3代码投的票（取值范围8，9，12）8：BT；9：WIFI；12：APP应用
	param1：代表休眠时间;
	param2：在RTC唤醒时没有用该参数，默认是0;
	param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准。

EXAMPLE:

  测试步骤：

   ::

	1） 开机启动5s之后，（开机之后，BT|、WIFI、CPU做初始化）；
	2） 输入以下三条命令：
	A.  WIFI没有业务的时候，默认进入sleep状态；
	B.  蓝牙默认进入sleep，（如果需要额外操作，请周灵补充）
	C.  pm 1 1 8 9 12 10000 0 100000000（让系统进入low voltage，使用RTC进行唤醒，10000表示设置的唤醒时间，可自行修改，单位为ms)；
	3）系统到了指定的唤醒时间后，系统会被从低压状态下唤醒
	4）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	5）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

  期望结果：

   ::

	1）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

cmd line 6:
  pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]

   ::

	备注：其他参数参考deep sleep中case 1的cmd line参数说明；
	Vote1: vote2: vote3代码投的票（取值范围8(BT)，9(WIFI)，12(APP)）
	param1：代表gpio的id号;
	param2：代表唤醒方式（0：低电平唤醒，1：高电平唤醒，2：上升沿唤醒，2：下降沿唤醒）;
	param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准。

EXAMPLE:

  测试步骤：

   ::

	1） 开机启动5s之后，（开机之后，BT|、WIFI、CPU做初始化）；
	2） 输入以下三条命令：
	A.  touch_single_channel_calib_mode_test 2 （数字2代表touch的channel号，现在测试版本默认是2）；
	B.  pm 1 4 8 9 12 2 0 1000000000（让系统进入low voltage，使用touch进行唤醒，2表示touch的channel号)；
	C.手动触摸touch,可以把系统从low voltage状态中唤醒
	3）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

  期望结果：

   ::

	1）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。
