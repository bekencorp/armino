AUD_EQ
========================

:link_to_translation:`en:[English]`

1、概述
--------------------
本小节将主要围绕EQ(均衡器)的demo来介绍AUDIO_EQ的功能使用。

2、API参考
--------------------
audio_eq的API的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``
具体的EQ功能主要调用以下两个API来实现EQ的使用和释放。

 - :cpp:func:`bk_aud_eq_init` - init the eq module of audio
 - :cpp:func:`bk_aud_eq_deinit` - deinit the eq module of audio

3、参数配置说明
--------------------
EQ功能主要通过以下结构体进行参数的配置。EQ的参数设置主要有：A1、A2、B0、B1、B2，其中B0、B1和B2是正常配置，A1和A2符号要取反设置。

::

	typedef struct {
		int32_t flt0_A1;
		int32_t flt0_A2;
		int32_t flt0_B0;
		int32_t flt0_B1;
		int32_t flt0_B2;

		int32_t flt1_A1;
		int32_t flt1_A2;
		int32_t flt1_B0;
		int32_t flt1_B1;
		int32_t flt1_B2;

		int32_t flt2_A1;
		int32_t flt2_A2;
		int32_t flt2_B0;
		int32_t flt2_B1;
		int32_t flt2_B2;

		int32_t flt3_A1;
		int32_t flt3_A2;
		int32_t flt3_B0;
		int32_t flt3_B1;
		int32_t flt3_B2;
	} aud_eq_config_t;

4、demo示例
--------------------
demo的具体源码和测试代码请参考工程路径：demos/media/audio/aud/aud_cp1.c中的cli_aud_eq_test_cmd()函数。
当前demo采用Dance的一组均衡数据进行测试，demo主要代码如下所示：

::

	aud_eq_config_t eq_config;
	eq_config.flt0_A1 = ~(-2085356);
	eq_config.flt0_A2 = ~(1036853);
	eq_config.flt0_B0 = 1054751;
	eq_config.flt0_B1 = -2085356;
	eq_config.flt0_B2 = 1030678;

	eq_config.flt1_A1 = ~(-2039888);
	eq_config.flt1_A2 = ~(992405);
	eq_config.flt1_B0 = 1041334;
	eq_config.flt1_B1 = -2039888;
	eq_config.flt1_B2 = 999646;

	eq_config.flt2_A1 = ~(-1694272);
	eq_config.flt2_A2 = ~(705464);
	eq_config.flt2_B0 = 1165692;
	eq_config.flt2_B1 = -1694272;
	eq_config.flt2_B2 = 588347;

	eq_config.flt3_A1 = ~(487698);
	eq_config.flt3_A2 = ~(-73179);
	eq_config.flt3_B0 = 1038338;
	eq_config.flt3_B1 = 487698;
	eq_config.flt3_B2 = -62942;
	bk_aud_eq_init(&eq_config);

之后经过DAC输出的信号即均衡过的音频信号。

5、操作说明
--------------------
 1) 通过SSCOM发送测试命令;
 2) 输入一段扫频信号经ADC采样并将采集到的数据通过DAC播放出来;
 3) 采集DAC输出信号来查看频响信号与设计的EQ频响是否一致。

