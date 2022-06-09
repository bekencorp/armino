WDT 使用指南
=====================

:link_to_translation:`en:[English]`

概述
-----------------

watchdog 实际上是一个硬件定时器，定时器超时会触发系统 watchdog 重启。

bk_wdt_start(timeout_ms) 可以设置 watchdog 定时器时间，目前时间配置范围为(1~0xFFFF)， bk_wdt_feed() 可重置 watchdog 定时器。


watchdog 使用方法
------------------

watchdog 有两种常见的使用方式：

 - 在周期性中断中喂狗，如在 OS tick 中断中喂狗，当因为软件问题，如关中断过长，tick 中断未能及时喂狗，导致 watchdog 超时，最终触发 watchdog 重启。通过这种方式，能及时发现软件中断异常问题。这类 watchdog 使用通常称为 interrupt watchdog。
 - 在周期性任务中喂狗，如在 idle task 中喂狗，通常 idle task 为整个系统中优先级最低的 task, 当软件出现问题，如某个任务出现死循环长期占用 CPU，使 idle task 无法调度，导致未能及时喂狗，最终触发 watchdog 重启。通过这种方式，能及时发现软件任务调度上的问题。这类 watchdog 使用通常称为 task watchdog。

ARMINO Interrupt Watchdog
-----------------------------

在 ARMINO 中，默认情况下，Interrupt Watchdog 是使能的，可通过 menuconfig 中 INT_WDT_PERIOD_MS 来设置超时时间，或者关闭 interrupt watchdog。通常，应用不应该关闭 interrupt watchdog。

.. note::

  当应用需要完全自己实现 interrupt watchdog 时，请通过 menuconfig 中 INT_WDT 来关闭系统默认的 interrupt watchdog。


ARMINO Task Watchdog
---------------------

ARMINO 中也实现了 Task Watchdog, 但并不是基于真正硬件 Watchdog，即不是基于 bk_wdt_start()/bk_wdt_feed() 实现， 具体实现方式如下：

全局变量 s_last_task_wdg_feed_tick 表示上次喂狗时间，在 idle task 中更新 s_last_task_wdg_feed_tick，在 tick 中断中判断距离上次喂狗时间是否超出 task watchdog 超时时间，如果超时，则打出 task watchdog 超时的警告信息，但不重启系统。

.. note::

  通常应用应该使能 task watchdog，除非应用实现自己的 task watchdog，可通过 menuconfig 中 TASK_WDT 来关闭系统默认的 task watchdog。



