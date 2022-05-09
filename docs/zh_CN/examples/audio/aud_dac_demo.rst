AUD_DAC
========================

:link_to_translation:`en:[English]`

1、概述
--------------------
	AUDIO模块主要包含音频采样(AUDIO_ADC)和音频播放(AUDIO_DAC)两部分功能。
	本小节将主要围绕重要的demo来介绍AUDIO_DAC的功能使用。

2、API参考
--------------------
	AUDIO DAC API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``

3、性能说明
--------------------
- AUDIO_DAC性能

AUDIO_DAC支持的采样率Fs(bps):

  - 8K
  - 16K
  - 44.1K
  - 48K

此外，还采用手动设置采样率的功能来提供特殊的采样率设置，具体的采样如下，可通过调用API: bk_aud_set_dac_samp_rate(aud_dac_samp_rate_t samp_rate)来实现。
  
  - 11.025K
  - 12K
  - 22.05K
  - 24K

4、demo例程
--------------------
demo的源码和测试文件请参考以下工程路径：``demos/media/audio/aud/aud_cp0.c``

- ADC_TO_DAC(mic采样直接输出到DAC播放的demo)

  执行测试指令，case的工作流程如下图所示：

.. figure:: ../../../_static/aud_adc_to_dac_flow.png
    :align: center
    :alt: adc_to_dac_demo软件流程
    :figclass: align-center

    Figure 1. adc to dac demo flow chart

其中，DAC部分的主要代码如下所示：

::

	aud_dac_config_t dac_config;

	dac_config.dac_enable = AUD_DAC_DISABLE;
	dac_config.samp_rate = AUD_DAC_SAMP_RATE_SOURCE_48K;
	dac_config.dac_hpf2_coef_B2 = 0x3A22;
	dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
	dac_config.dac_set_gain = 0x2D;
	dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

	dac_config.dac_hpf2_coef_B0 = 0x3A22;
	dac_config.dac_hpf2_coef_B1 = 0x8BBF;

	dac_config.dac_hpf2_coef_A1 = 0x751C;
	dac_config.dac_hpf2_coef_A2 = 0xC9E6;

	dac_config.dacr_rd_threshold = 0x4;
	dac_config.dacl_rd_threshold = 0x4;
	dac_config.dacr_int_enable = 0x0;
	dac_config.dacl_int_enable = 0x0;

	dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;
	dac_config.dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
	dac_config.dac_fracmode_value = 0x0;

	bk_aud_driver_init();
	bk_aud_dac_init(&dac_config);


- PCM_TO_DAC(从SD卡读取PCM数据并通过DAC实时播放音乐的demo)

.. figure:: ../../../_static/aud_pcm_to_dac.png
    :align: center
    :alt: PCM_to_DAC_demo工作流程
    :figclass: align-center

    Figure 2. PCM to DAC demo flow chart

.. note::
 - 1.case执行前需要初始化PSRAM和挂载SD卡;
 - 2.执行此case时需要确保没有其他模块正在使用PSRAM;
 - 3.此demo源码较多, 所以就不在此处展开说明了，请参考源码;

