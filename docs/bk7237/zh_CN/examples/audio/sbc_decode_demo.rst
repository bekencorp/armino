SBC_DEC
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	SBC（Sub Band Codec，子带编解码）是一种以中等比特率传递音频数据的低计算复杂度的音频编解码算法，基本原理是把信号的频率分为若干子带，然后对每个子带进行编码，并根据每个子带的重要性及特点分配不同的位数（采样深度）来表示数据。
	SBC算法包含编码和解码两种功能，由于该芯片只支持SBC硬件解码，所以该demo主要展示解码功能的使用，此外，SBC解码支持SBC和MSBC两种解码功能。

	sbc_decode demo主要功能是：将sbc数据通过sbc_decode解码后直接通过Audio DAC实时播放。

2、代码路径
--------------------------
	demo路径: ``\components\demos\media\audio\sbc_decode``
	
	sbc_decode API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_sbc.html``
	AUDIO DAC API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``

3、cli命令简介
--------------------------
demo支持的命令如下表:

+-------------------------------------------+----------------------------+
|Command                                    |Description                 |
+===========================================+============================+
|sbc_decoder_test {start|stop} {xxx.sbc}    |sbc解码并通过dac实时播放    |
+-------------------------------------------+----------------------------+

demo运行依赖的宏配置:

+---------------------------+----------------------------+--------------------------------------------+-----+
|Name                       |Description                 |   File                                     |value|
+===========================+============================+============================================+=====+
|CONFIG_SBC                 |配置sbc decode是否生效      |``\middleware\soc\bk7256\bk7256.defconfig`` |  y  |
+---------------------------+----------------------------+--------------------------------------------+-----+
|CONFIG_SBC_TEST            |配置sbc decode demo是否生效 |``\middleware\soc\bk7256\bk7256.defconfig`` |  y  |
+---------------------------+----------------------------+--------------------------------------------+-----+

demo运行依赖的库和驱动:
 - DMA DMA模块驱动
 - AUD audio模块驱动
 - SD SDcard及文件系统驱动
 
4、演示介绍
--------------------------
demo执行的步骤如下:

	1.准备测试需要的音频文件
	 - 将需要测试的xx.sbc文件保存在SDCARD卡根目录

	2.挂载TF卡
	 - 串口工具发送指令 ``fatfstest M 1`` 挂载TF卡

	3.执行测试
	 - 串口工具发送指令 ``sbc_decoder_test start xx.sbc`` 执行测试

	4.停止测试
	 - 串口工具发送指令 ``sbc_decoder_test stop xx.sbc`` 停止测试

.. note::
 - sbc_decoder_frame_decode()的API中自动检测是SBC解码还是MSBC解码，只需要将编码数据的存放地址作为地址传入该API即可得到解码后的PCM数据。

SBC解码demo工作流程如下图所示:

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

