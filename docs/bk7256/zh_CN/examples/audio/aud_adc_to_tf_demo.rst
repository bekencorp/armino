ADC TF卡录音PCM数据demo
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------
	mic采集音频数据并保存为 ``xx.pcm`` 文件存储至TF卡。

2、代码路径
--------------------
	demo路径: ``\components\demos\media\audio\aud``

	AUDIO ADC/DAC API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``

3、cli命令简介
--------------------
demo支持的命令如下表:

+-----------------------------------------+----------------------+
|Command                                  |Description           |
+=========================================+======================+
|cpu1 aud_mic_to_pcm_test {start|stop}    |cpu1执行录音准备      |
+-----------------------------------------+----------------------+
|aud_cp0_psram_init {start|stop}          |初始化psram           |
+-----------------------------------------+----------------------+
|aud_cp0_mic_to_pcm {start|stop}          |cpu0执行录音测试      |
+-----------------------------------------+----------------------+

demo运行依赖的宏配置:

+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_AUDIO               |配置audio功能               |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |配置audio ip                |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_TEST          |配置demo使能                |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_TEST          |配置demo使能                |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+

demo运行依赖的库和驱动:
 - DMA DMA驱动
 - AUD audio模块驱动
 - PSRAM PSRAM驱动
 - MAILBOX mailbox驱动

4 演示介绍
--------------------

demo执行的步骤如下:

	1.初始化PSRAM
	 - Uart发送AT指令 ``aud_cp0_psram_init start`` 初始化PSRAM驱动

	2.挂载TF卡
	 - Uart发送AT指令 ``fatfstest M 1`` 挂载TF卡

	3.开始录音准备
	 - Uart发送AT指令 ``cpu1 aud_mic_to_pcm_test start`` 初始化录音准备

	4.执行录音测试
	 - Uart发送AT指令 ``aud_cp0_mic_to_pcm start`` 开始录音

	5.停止录音测试
	 - Uart发送AT指令 ``aud_cp0_mic_to_pcm stop`` 停止录音，结束测试，音频会保存在TF卡的根目录文件 ``aud_mic_data.pcm`` 中

执行测试指令，case的工作流程如下图所示：

.. figure:: ../../../_static/aud_adc_to_pcm.png
    :align: center
    :alt: adc_to_pcm_demo软件流程
    :figclass: align-center

    Figure 1. adc to pcm demo flow chart

5、详细配置及说明
--------------------------
.. note::
 - 1.case执行前需要初始化PSRAM和挂载TF卡;
 - 2.执行此case时需要确保没有其他模块正在使用PSRAM;
 - 3.此demo源码较多, 所以就不在此处展开说明了，请参考源码;
