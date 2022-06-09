SBC模块
========================

:link_to_translation:`en:[English]`

1、概述
--------------------------
SBC（Sub Band Codec，子带编解码）是一种以中等比特率传递音频数据的低计算复杂度的音频编解码算法，基本原理是把信号的频率分为若干子带，然后对每个子带进行编码，并根据每个子带的重要性及特点分配不同的位数（采样深度）来表示数据。
SBC模块包含SBC和MSBC两种编解码，由于本SDK API只支持音频解码功能，所以本小结主要围绕sbc和msbc解码功能介绍demo。

2、SBC解码常见参数
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

3、demo示例
--------------------------
 - SBC解码和MSBC解码demo使用的API接口详细说明请参考 ``/api-reference/multi_media/bk_sbc.html``

 - SBC解码和MSBC解码demo具体的示例代码详见audio/sbc_decode/sbc_decode_demo.c

4 demo工作流程
--------------------------
SBC和MSBC解码demo工作流程如下图所示，其中，sbc_decoder_frame_decode()的API中自动检测是SBC解码还是MSBC解码，只需要将编码数据的存放地址作为地址传入该API即可得到解码后的PCM数据。

.. figure:: ../../../_static/sbc_user_guide_flow.png
    :align: center
    :alt: SBC Decode work Flow Chart
    :figclass: align-center

    SBC Decode Work Flow Chart

