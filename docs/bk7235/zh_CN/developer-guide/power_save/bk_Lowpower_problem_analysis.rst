低功耗问题分析
=======================================================

一、低压问题分析指导
--------------------------------------------------------------

1.进低压失败
##############################################################

如果进低压失败，首先确认是否满足进入低压的条件：

- 1. 业务线程可能有类似while(1)的操作,业务线程一直在运行，导致系统无法进入IDLE task。

该问题debug方法：

  a)  异常后，通过串口发送测试命令：pm_debug 1
  观察是否有以下log打印：

   ::

    “lowvol1 0x3 0xFFFFEFFF 0xFFFFFFFF”
    “lowvol2 0x0 0x0”

  如果没有以上打印，则有可能是这个问题。

  b) 以上问题的解决方案：
  线程运行不能一直while(1)进行运行或者等待，应该使用信号量或者事件标识，没有业务工作时，线程进入sleep。

- 2. 相应的模块没有投sleep的票，导致无法进入低压。

该问题debug方法：

  a) 异常后，通过串口发送测试命令：pm_debug 1
  有以下log打印：

   ::

    “lowvol1 0x3 0xFFFFEFFF  0xFFFFFFFF”
    “lowvol2 0x0 0x0”

   当前投的票的值：0xFFFFEFFF(bit位为1，表示该位的票投上了票)
   需要满足的票值：0xFFFFFFFF

  从以上两个值对比来看，bit12的值为0，表示该位置的票没有投上，
  查看pm_sleep_module_name_e枚举类型，值为12的票
  为PM_SLEEP_MODULE_NAME_APP，则表示该APP票没有投上。

  b) 以上问题的解决方案：
  需要在业务线程处理完响应的工作后，调用以下接口把票投上：

   ::

    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x1,0x1);
    typedef enum
    {
        PM_SLEEP_MODULE_NAME_I2C1 = 0, // 0
        PM_SLEEP_MODULE_NAME_SPI_1,    // 1
        PM_SLEEP_MODULE_NAME_UART1,    // 2
        PM_SLEEP_MODULE_NAME_PWM_1,    // 3
        PM_SLEEP_MODULE_NAME_TIMER_1,  // 4
        PM_SLEEP_MODULE_NAME_SARADC,   // 5
        PM_SLEEP_MODULE_NAME_AUDP,     // 6
        PM_SLEEP_MODULE_NAME_VIDP,     // 7
        PM_SLEEP_MODULE_NAME_BTSP,     // 8
        PM_SLEEP_MODULE_NAME_WIFIP_MAC,// 9
        PM_SLEEP_MODULE_NAME_WIFI_PHY, // 10
        PM_SLEEP_MODULE_NAME_TIMER_2,  // 11
        PM_SLEEP_MODULE_NAME_APP,      // 12
        PM_SLEEP_MODULE_NAME_OTP,      // 13
        PM_SLEEP_MODULE_NAME_I2S_1,    // 14
        PM_SLEEP_MODULE_NAME_USB_1,    // 15
        PM_SLEEP_MODULE_NAME_CAN,      // 16
        PM_SLEEP_MODULE_NAME_PSRAM,    // 17
        PM_SLEEP_MODULE_NAME_QSPI_1,   // 18
        PM_SLEEP_MODULE_NAME_QSPI_2,   // 19
        PM_SLEEP_MODULE_NAME_SDIO,     // 20
        PM_SLEEP_MODULE_NAME_AUXS,     // 21
        PM_SLEEP_MODULE_NAME_CPU,      // 22
        PM_SLEEP_MODULE_NAME_XVR,      // 23
        PM_SLEEP_MODULE_NAME_I2C2,     // 24
        PM_SLEEP_MODULE_NAME_UART2,    // 25
        PM_SLEEP_MODULE_NAME_UART3,    // 26
        PM_SLEEP_MODULE_NAME_WDG,      // 27
        PM_SLEEP_MODULE_NAME_TIMER_3,  // 28
        PM_SLEEP_MODULE_NAME_APP1,     // 29
        PM_SLEEP_MODULE_NAME_APP2,     // 30
        PM_SLEEP_MODULE_NAME_APP3,     // 31

        PM_SLEEP_MODULE_NAME_MAX
    }pm_sleep_module_name_e;

2.低压唤醒后系统自动复位
##############################################################

低压唤醒后系统自动复位，可能的原因如下：

- 1. wdt复位:低压唤醒后，有中断没有清除掉，导致中断不断的上报，WDT无法喂狗导致

该问题debug方法：

  a)  该类问题，一般会进行dump，通过查看LA的地址，地址是否在pm.c出开中断后或者在mext_interrupt()或者mext_interrupt_sub()函数中，如果在，一般是这类问题。
  在接收中断的函数中例如：mext_interrupt()或者mext_interrupt_sub()把把中断编号irq_source的值打印出来，通过看irq_source的值，确定哪个模块，再结合代码和流程找出问题

  b) 以上问题的解决方案：
  中断响应完后，需要把中断清除，防止中断不断往上报。


二、深度睡眠问题分析指导
--------------------------------------------------------------

1.进深度睡眠失败
##############################################################

如果进深度睡眠失败，首先确认是否满足进入深度睡眠的条件：

- 1.  业务线程可能有类似while(1)的操作,业务线程一直在运行，导致系统无法进入IDLE task。

该问题debug方法：

  a)  异常后，通过串口发送测试命令：pm_debug 2

观察是否有以下log打印：

   ::

    deepsleep1 0x3C4 0x3C0
    deepsleep2 0x0 0x0

如果没有以上打印，则有可能是这个问题。

  b)  以上问题的解决方案：

线程运行不能一直while(1)进行运行或者等待，应该使用信号量或者事件标识，没有业务工作时，线程进入sleep。

- 2.  没有满足了进入深度睡眠的票（BT和WIFI进入sleep并下电,多媒体关闭并下电）

.. important::
    1)BT和WIFI进入SLEEP并下电，是有SDK内部代码完成，客户或者应用层代码不需要调用关闭和下电的接口
    2)进入deepsleep的前提之一是多媒体(audio,video,lcd需要关闭和下电，需要调用他们模块的接口)

该问题debug方法：

  a)异常后，通过串口发送测试命令：pm_debug 1

有以下log打印：

   ::

    deepsleep1 0x300 0x3C0
    deepsleep2 0x0 0x0

   当前投的票的值：0x300 (bit位为1，表示该位的票投上了票)
   需要满足的票值：0x3C0

从以上两个值对比来看，bit6和bit7的值为0，表示该位置的票没有投上，
查看PM_ENTER_DEEP_SLEEP_MODULES_CONFIG定义的值，bit6为AUDIO,
Bit7为VIDEO，则表示以上两个模块的票没有投上。

   ::

    #define PM_ENTER_DEEP_SLEEP_MODULES_CONFIG \
    {\
        PM_POWER_MODULE_NAME_AUDP,\      /*6*/
        PM_POWER_MODULE_NAME_VIDP,\      /*7*/
        PM_POWER_MODULE_NAME_BTSP,\      /*8*/
        PM_POWER_MODULE_NAME_WIFIP_MAC,\ /*9*/
    }

  b)以上问题的解决方案：
  Audio,video模块需要在进deepsleep之前，调用各自模块的接口关闭audio,video。



:link_to_translation:`en:[English]`

