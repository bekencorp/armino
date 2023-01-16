WDT User Guide
===============================================================

:link_to_translation:`zh_CN:[中文]`

Summary
-----------------------------------------------------------------

watchdog In fact, it is a hardware timer. The timeout of the timer will trigger the system 'watchdog' to restart.

'bk_wdt_start(timeout_ms)' can set the time of timer 'watchdog', the current time configuration range is (1~0xFFFF), and 'bk_wdt_feed()' can reset timer 'watchdog'.



Watchdog Usage
------------------------------------------------------------------

watchdog There are two common ways to use：

 - The dog is fed in periodic interrupts, such as interrupt 'os tick', and when the tick interrupt fails to feed the dog in time due to software problems, such as too long off interrupt, the watchdog times out and finally triggers the watchdog to restart. In this way, software interruption exceptions can be found in time. This kind of watchdog is commonly called interrupt watchdog.
 - Feeding dogs in periodic tasks, such as feeding dogs in idle tasks, is usually the task with the lowest priority in the whole system. When the software has problems, such as a task with an endless loop that occupies the CPU for a long time, the idle task cannot be scheduled, resulting in the failure to feed dogs in time, and finally triggering the watchdog to restart. In this way, problems in software task scheduling can be found in time. This kind of watchdog is commonly called task watchdog.


ARMINO Interrupt Watchdog
-----------------------------------------------------------------

In ARMINO, the Interrupt Watchdog is enabled by default. You can set the timeout through 'INT_WDT_PERIOD_MS' in menuconfig, or turn off the interrupt watchdog. Generally, the application should not close the interrupt watchdog. 

.. note::

  When the application needs to completely implement the interrupt watchdog, please use 'INT_WDT' in 'menuconfig' to close the system default interrupt watchdog.


ARMINO Task Watchdog
---------------------------------------------------------------------

ARMINO also implements the Task Watchdog, but it is not based on the real hardware Watchdog, that is, it is not based on 'follows bk_wdt_start()/bk_wdt_feed()'. The specific implementation method is as：

The global variable 's_last_task_wdg_feed_tick' represents the last dog feeding time. Update 's_last_task_wdg_feed_tick' in 'idle task'. Judge whether the last dog feeding time exceeds the 'task watchdog' timeout in the 'tick' interrupt. If the timeout occurs, the warning message of 'task watchdog' timeout will be displayed, but the system will not be restarted.

.. note::


  Generally, the application should enable 'task watchdog'. Unless the application implements its own 'task watchdog', the default 'task watchdog' of the system can be turned off through 'TASK_WDT' in menuconfig.



