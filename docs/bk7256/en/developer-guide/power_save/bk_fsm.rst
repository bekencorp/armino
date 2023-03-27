System state machine specification
=============================================

:link_to_translation:`zh_CN:[中文]`


.. image:: ../../../_static/pm_pcm.png



The system supports different sleep modes:
 - active
 - normal sleep
 - low voltage
 - deepsleep
 - shut down



active(work)
--------------------------------------------
 - The CPU is in working and WIFI and BT can receive and send data.


normal sleep(normal sleep)
++++++++++++++++++++++++++++++++++++++++++++
 - When rtos does not have a task to deal with, the system enters the idle task, and the CPU enters the WFI sleep in the idle task. When there is any interruption, it can exit the wfi state and go to work.


low voltage(low voltage sleep)
++++++++++++++++++++++++++++++++++++++++++
 - low voltage sleep mode is a relatively more power save sleep mode.In the sleep mode the system only has a 32k clock,and only a part of hardware modules are working that except AON module work ,other hardware modules are suspended(not run)

When it wakes up and exit from the low voltage sleep mode, the voltage of dvdddig is return to the work voltage(1.1v)。
 - when it is in the low voltage sleep mode，the (GPIO,RTC,Touch,WIFI,BT) wakeup source can let the system wake up from the low voltage sleep mode 。Attention：WIFI and BT according the usage scenario enter the low voltage sleep mode, which is special.
 
 - it select which 32k source(external 32k, rosc, external 26M division 32k) ,which according the usage scenario，please refer to <clock instructions>。
 
 - In order to achieve the optimal power consumption, the modules that are not needed, please close them before enter the low voltage,and then restore them when exit the low voltage.

deep sleep(deep sleep)
++++++++++++++++++++++++++++++++++++++++++
 - Deep sleep is a relatively more power-saving sleep mode。In the sleep mode the system only has a 32k clock,and only AON modules are working, other modules power off。

when it wakes up，the system exit the deep sleep state，then the system reset。
 - In the deep sleep mode，the wakeup source(GPIO,RTC,Touch)can let the system exit the deep sleep mode。
 
 - The rosc is the default 32k source in the sleep mode。


shut down(shut down)
--------------------------------------------
 - all the chip is powered off



State machine switching instructions
=============================================
 - The mcu will suspend(not run) in the low voltage and deep sleep sleep mode, it suggest WIFI,BT,audio,video enter sleep mode, then the mcu enter low voltage and deep sleep.
 
In order to make sure the corrent sleep sequence, we use the voting mechanism.

 - low voltage：The current system supports 32 tickets, and we will use the ticket for vote according the scenario. You don't need to pay much attention using the SDK, only focus on vote the APP ticket.
	The tickets of BT and WIFI are according to themselves work process to vote,the user of the SDK not need pay attention.APP ticket can be voted according to their own usage scenarios.
	(if you open the multi-meida(AUDP_FFT,AUDP_SBC,AUDP_AUDIO,AUDP_I2S,VIDP_JPEG_EN,VIDP_JPEG_DE,VIDP_DMA2D,VIDP_LCD), it need close them befor enter low voltage)
 - deep sleep：when user using the SDK set the deep sleep mode, the SDK function will let the BT and WIFI stop work(if BT and WIFI active)then the system enter deepsleep.
	(if you open the multi-meida(AUDP_FFT,AUDP_SBC,AUDP_AUDIO,AUDP_I2S,VIDP_JPEG_EN,VIDP_JPEG_DE,VIDP_DMA2D,VIDP_LCD), it need close them befor enter deepsleep)
 - if there is no higher priority task than idle task, it will enter idle task to do "WFI" for normal sleep。if it meet the ticks of enter low voltage sleep mode, it will enter low voltage mode.
 