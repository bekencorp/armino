低功耗模块的典型配置说明
==================================

:link_to_translation:`en:[English]`

1、 CONFIG_MEM_AUTO_POWER_DISABLE=n
-----------------------------------------------

a.	该宏的作用是控制第三块内存是否根据业务的使用情况，自动上下电第三块内存。 当该宏为n时，开启了自动上下电第三块内存功能（重定向后的第三块内存地址范围为0x3000000-0x3001FFFF）。

b.	下电第三块内存的条件是：BT,AUDIO ,VIDEO模块都关闭并且下电后，底层SDK会自动把第三块内存下电。BT,AUDIO,VIDEO任何模块上电，都不会自动把第三块内存下电。BT,AUDIO ,VIDEO任何一个模块上电，都会上电第三块内存。

c.	当关闭第三块内存自动上下电功能时(CONFIG_MEM_AUTO_POWER_DISABLE=y)，低压睡眠模式的底电流会增加8uA左右。

d.	当开启第三块内存自动上下电时(CONFIG_MEM_AUTO_POWER_DISABLE=n)，低压睡眠模式的底电流会减少8uA左右，当追求最优功耗时，可以把以上宏置为n。需要关注以下注意事项：

e.	为了节约第三块内存的功耗，当前设计把BT,AUDIO ,VIDEO模块使用的内存放在了上面。如果在实际的开发项目中，内存不够，想复用第三块内存，可以把CONFIG_MEM_AUTO_POWER_DISABLE=y。

.. important::
   当第三块内存下电后，在该内存不可用，并且内存的值为随机值。上电时会重新初始化第三块内存，内存上的变量的都变成0。

2、 CONFIG_CP1_AUTO_POWER_DISABLE=n
-----------------------------------------------

a.	该宏的作用是根据业务的使用情况，自动上下电CPU1, 当该宏为n时，开启了自动上下电CPU1。

b.	下电CPU1条件是：AUDIO ,VIDEO模块都关闭并且下电后，底层SDK会自动把CPU1下电。AUDIO,VIDEO任何模块上电，都不会自动把CPU1下电。AUDIO ,VIDEO任何一个模块上电，都会上电CPU1。

c.	当关闭CPU1自动上下电功能时(CONFIG_CP1_AUTO_POWER_DISABLE=y), 需要关注以下注意事项:

    如果AUDIO,VIDEO或者另外一些应用模块需要在CPU1上运行时，需要提前调用接口开启CPU1,开启CPU1的接口如下：

    c.1开启CPU1：
    bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
    start_slave_core();

    c.2关闭CPU1
    stop_slave_core ();
    bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_OFF);

    c.3注意当有一个模块在CPU1上运行时，上电时序是：先把CPU1上电，再把使用模块上电；下电时序是：先把使用模块下电，再把CPU1下电。

d.当没有模块使用CPU1时，可以默认关闭该功能。该宏置为y:CONFIG_CP1_AUTO_POWER_DISABLE=y
