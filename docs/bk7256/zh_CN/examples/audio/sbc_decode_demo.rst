sbc_decode demo
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	SBC（Sub Band Codec，子带编解码）是一种以中等比特率传递音频数据的低计算复杂度的音频编解码算法，基本原理是把信号的频率分为若干子带，然后对每个子带进行编码，并根据每个子带的重要性及特点分配不同的位数（采样深度）来表示数据。
	SBC模块包含SBC和MSBC两种编解码，由于本SDK API只支持音频解码功能，所以本demo主要围绕sbc和msbc解码功能。

	sbc_decode demo主要功能是：将正弦音的sbc编码后的数据通过sbc_decode解码后通过DAC播放。

2、代码路径
--------------------------
	demo路径: ``\components\demos\media\audio\sbc_decode``
	
	sbc_decode API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_sbc.html``
	AUDIO DAC API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``

3、cli命令简介
--------------------------
demo支持的命令如下表:

+-------------------------------------------+------------------------+
|Command                                    |Description             |
+===========================================+========================+
|sbc_decoder_test {start|stop}              |sbc解码并通过dac播放    |
+-------------------------------------------+------------------------+
|msbc_decoder_test {start|stop}             |msbc解码并通过dac播放   |
+-------------------------------------------+------------------------+

demo运行依赖的宏配置:

+---------------------------+------------------------+----------------------------------------------------+-----+
|Name                       |Description             |   File                                             |value|
+===========================+========================+====================================================+=====+
|CONFIG_AUDIO               |配置audio功能           |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |配置audio功能           |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_TEST          |配置demo使能            |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+------------------------+----------------------------------------------------+-----+
|CONFIG_SBC                 |配置sbc decode使能      |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+------------------------+----------------------------------------------------+-----+
|CONFIG_SBC_TEST            |配置sbc decode demo使能 |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+------------------------+----------------------------------------------------+-----+

demo运行依赖的库和驱动:
 - DMA DMA驱动
 - AUD audio模块驱动
 
4、演示介绍
--------------------------
demo执行的步骤如下:

	1.执行测试
	 - Uart发送AT指令 ``cpu1 sbc_decoder_test start`` 执行测试

	2.停止测试
	 - Uart发送AT指令 ``cpu1 sbc_decoder_test stop`` 停止测试

.. note::
 - sbc_decoder_frame_decode()的API中自动检测是SBC解码还是MSBC解码，只需要将编码数据的存放地址作为地址传入该API即可得到解码后的PCM数据。
 - 测试msbc decode功能只需要发送测试命令 ``cpu1 msbc_decoder_test start`` 和 ``cpu1 msbc_decoder_test stop`` 即可进行测试和停止测试。

SBC和MSBC解码demo工作流程如下图所示:

.. figure:: ../../../_static/sbc_user_guide_flow.png
    :align: center
    :alt: SBC Decode work Flow Chart
    :figclass: align-center

    SBC Decode Work Flow Chart

5、详细配置及说明
--------------------------
- 采样率

 - 16KHz
 - 32KHz
 - 44.1KHz
 - 48KHz

- 通道模式

 - 单通道
 - 双通道
 - 立体声
 - 联合立体声

- Blocks

 - 4
 - 8
 - 12
 - 16
 
- Subbands

 - 4
 - 8
 
- 分配方式

 - SNR
 - Loudness

- 同步字

 - 0x9C(SBC)
 - 0xAD(MSBC)

