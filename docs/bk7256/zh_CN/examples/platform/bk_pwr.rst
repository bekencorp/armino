power manager
==========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	支持平台power manager（normal sleep、low voltage 、deepsleep）。

2 代码路径
-------------------------------------
	demo路径：``components\bk_cli\cli_pwr.c``

3 cli命令简介
-------------------------------------

demo运行依赖的宏配置：

	+--------------------------------------+--------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description         |                  File                      |  value  |
	+======================================+==========================+============================================+=========+
	|CONFIG_MCU_PS                         |support low power feature | ``properties\soc\bk7256\bk7256.defconfig`` |    y    |
	+--------------------------------------+--------------------------+--------------------------------------------+---------+

demo支持的命令如下：

cmd line 1:
####################################

  pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]

   ::

	sleep_mode：代表休眠模式(参数范围：0-3): 0:normal sleep; 1:low voltage; 2:deep sleep;3:none
	wake_source：表示唤醒源(参数范围：0-5)： 0：GPIO唤醒；1：RTC唤醒；2：WIFI或者BT唤醒；3：USB唤醒（BK72xx不支持，其他项目支持);4：touch唤醒；5：none
	Vote1: vote2: vote3: 代表投的票（取值范围8(BT)，9(WIFI)，12(APP))
	param1：根据唤醒源有不同含义 -- GPIO：gpio的id号  RTC：唤醒时间  WIFI或者BT： touch：touch的channel号
	param2：根据唤醒源有不同含义 -- GPIO：代表唤醒方式（0：低电平唤醒，1：高电平唤醒，2：上升沿唤醒，3：下降沿唤醒）  RTC和touch：唤醒时不用该参数，默认是0
	param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准


EXAMPLE 1: 使用GPIO作为唤醒源来唤醒deepsleep设备
*************************************************

  测试步骤：

   ::

	1）开机启动5s之后，（开机之后，BT、WIFI、CPU做初始化）；
	2）输入命令：
	A. pm 2 0 8 9 12 9 0 1000000000（让系统进入deepsleep，使用gpio进行唤醒，9表示gpio的id，0代表低电平唤醒)；
	B. 如果希望某个GPIO从高电平唤醒，则测试前把该GPIO初始状态接为低电平，唤醒时接高电平；
	C. 如果希望某个GPIO从低电平唤醒，则测试前把该GPIO初始状态接为高电平，唤醒时接低电平；
	D. 如果希望某个GPIO从上升沿唤醒，则测试前把该GPIO初始状态接为低电平，唤醒时接高电平；
	E. 如果希望某个GPIO从下降沿唤醒，则测试前不要把该GPIO初始状态接为高电平，唤醒时接低电平；
	3）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个deep sleep到唤醒过程的功耗。

  期望结果：

   ::

	1）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗；
	3）deepsleep之后的功耗符合预期为15ua-20ua左右。

EXAMPLE 2: 使用RTC设置唤醒时间来唤醒deepsleep设备
*************************************************

  测试步骤：

   ::

	1）开机启动5s之后（开机之后，BT、WIFI、CPU做初始化）；
	2）输入命令: pm 2 1 8 9 12 3000 0 1000000000（让系统进入deepsleep，使用RTC进行唤醒，3000表示设置的唤醒时间，可自行修改，单位为ms)；
	3）系统到达设定的的唤醒时间后，系统会被唤醒；
	4）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	5）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗。

  期望结果：

   ::

	1）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗；
	3）deepsleep之后的功耗符合预期为15ua-20ua左右。


EXAMPLE 3:使用触摸(touch)作为唤醒源来唤醒deepsleep设备
******************************************************

  测试步骤：

   ::

	1）开机启动5s之后（开机之后，BT、WIFI、CPU做初始化）；
	2）输入以下两条命令：
	A. touch_single_channel_calib_mode_test 2 1（数字2代表touch的channel号，现在测试版本默认是2,channnel根据实际使用确定；数字1表示touch channel设置的强度，取值范围0~3）；
	B. pm 2 4 8 9 12 2 0 1000000000（让系统进入deepsleep，使用touch进行唤醒）；
	C.手动触摸touch,可以把系统从deepsleep状态中唤醒；
	3）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗。

  期望结果：

   ::

	1）在deepsleep过程中，通过测量VDDDIG的电压会变成0；唤醒（使用touch）之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个deepsleep到唤醒过程的功耗；
	3）deepsleep之后的功耗符合预期为15ua-20ua左右。

EXAMPLE 4: 使用GPIO来唤醒低压设备
*************************************************

  测试步骤：

   ::

	1）开机启动5s之后，（开机之后，BT、WIFI、CPU做初始化）；
	2）输入以下命令：
	A0.p(为了最优的功耗，BT使用完毕后，请关闭，关闭命令：AT+BLE=POWER,0)
	A. pm 1 0 8 9 12 9 2 1000000000（让系统进入low voltage，使用gpio进行唤醒，9表示gpio的id，2代表上升沿唤醒)；
	B. 如果希望某个GPIO从高电平唤醒，则测试前把该GPIO初始状态接为低电平，唤醒时接高电平；
	C. 如果希望某个GPIO从低电平唤醒，则测试前把该GPIO初始状态接为高电平，唤醒时接低电平；
	D. 如果希望某个GPIO从上升沿唤醒，则测试前把该GPIO初始状态接为低电平，唤醒时接高电平；
	E. 如果希望某个GPIO从下降沿唤醒，则测试前不要把该GPIO初始状态接为高电平，唤醒时接低电平；
	3）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

  期望结果：

   ::

	1）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

EXAMPLE 5: 使用RTC作为唤醒源来唤醒低压设备
*************************************************

  测试步骤：

   ::

	测试步骤如下：
	1）开机启动5s之后，（开机之后，BT、WIFI、CPU做初始化）；
	2）输入以下命令：
	A. WIFI没有业务的时候，默认进入sleep状态；
	B. 蓝牙默认进入sleep(为了最优的功耗，BT使用完毕后，请关闭，关闭命令：AT+BLE=POWER,0)；
	C. pm 1 1 0 0 12 10000 0 1（让系统进入low voltage，使用RTC进行唤醒，10000表示设置的唤醒时间，可自行修改，单位为ms)；
	3）系统到了指定的唤醒时间后，系统会被从低压状态下唤醒
	4）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.6或0.9v）；唤醒之后，VDDDIG的电压会变成1.1v；
	5）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

  期望结果：

   ::

	1）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.6或0.9v）；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗；

EXAMPLE 6: 使用触摸(touch)来唤醒低压设备
*************************************************

  测试步骤：

   ::

	1）开机启动5s之后，（开机之后，BT、WIFI、CPU做初始化）；
	2）输入以下命令：
	A0.为了最优的功耗，BT使用完毕后，请关闭，关闭命令：AT+BLE=POWER,0)
	A. touch_single_channel_calib_mode_test 2 1（数字2代表touch的channel号，现在测试版本默认是2,channnel根据实际使用确定；数字1表示touch channel设置的强度，取值范围0~3）
	B. pm 1 4 8 9 12 2 0 1000000000（让系统进入low voltage，使用touch进行唤醒，2表示touch的channel号)
	C. 手动触摸touch,可以把系统从low voltage状态中唤醒；
	3）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.6或0.9v）；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。

  期望结果：

   ::

	1）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.9或1.0v）；唤醒之后，VDDDIG的电压会变成1.1v；
	2）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗;

cmd line 2:
####################################

  pm_vote [sleep_mode] [pm_vote] [pm_vote_value] [pm_sleep_time]

   ::

	sleep_mode：代表休眠模式(参数范围：0-3): 0:normal sleep; 1:low voltage; 2:deep sleep;3:none
	pm_vote:代表投的票（取值范围8(BT)，9(WIFI)，12(APP)）
	pm_vote_value：投票值（取值范围0：MODULE_STATE_ON   1：MODULE_STATE_OFF）
	pm_sleep_time：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准

EXAMPLE 1: 使用WIFI保活来唤醒低压设备
*************************************************

  测试步骤：

   ::

	测试步骤如下：
	1）开机启动5s之后，（开机之后，BT、WIFI、CPU做初始化）；
	2）输入以下四条命令：
	A. set_interval 10 (该命令是设置DTIM10）
    (备注：配置其他值：DTIM1命令：set_interval 1； DTIM3命令：set_interval 3)
	B. sta 123(路由器名称) 12345678（路由器密码）（连接路由器，WIFI会进入sleep wakeup流程）
	C. 蓝牙默认进入sleep(为了最优的功耗，BT使用完毕后，请关闭，关闭命令：AT+BLE=POWER,0)
	D. pm_vote 1 12 1 0（投app票）
	3）在low voltage过程中，通过测量VDDDIG的电压会变成设定的电压值，（例如0.6或0.9v）；唤醒之后，VDDDIG的电压会变成1.1v；
	4）同时通过使用精密电源来测量整个low voltage到唤醒过程的功耗。
