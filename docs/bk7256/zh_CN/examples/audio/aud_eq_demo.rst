EQ demo
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------
	EQ demo主要功能是均衡dance的风格。
	
2、代码路径
--------------------
	demo路径: ``\components\demos\media\audio\aud``

	AUDIO DAC和EQ API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``

3、cli命令简介
--------------------
demo支持的命令如下表:

+---------------------------------+----------------------+
|Command                          |Description           |
+=================================+======================+
|aud_eq_test {start|stop}         |均衡Dance风格         |
+---------------------------------+----------------------+

demo运行依赖的宏配置:

+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_AUDIO               |配置audio功能               |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |配置audio ip                |``\middleware\soc\bk7256_cp1\bk7256_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_TEST          |配置demo使能                |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+

demo运行依赖的库和驱动:
 - DMA DMA驱动
 - AUD audio模块驱动
 
4、演示介绍
--------------------

demo执行的步骤如下:

	1.执行测试
	 - Uart发送AT指令 ``cpu1 aud_eq_test start`` 执行测试
	
	2.输入一段扫频信号经ADC采样并将采集到的数据通过DAC播放出来
	
	3.通过仪器采集DAC输出信号来查看频响信号与设计的EQ频响是否一致
	
	4.停止测试
	 - Uart发送AT指令 ``cpu1 aud_eq_test stop`` 停止测试

5、详细配置及说明
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


具体的EQ功能主要调用以下两个API来实现EQ的使用和释放。

 - :cpp:func:`bk_aud_eq_init` - init the eq module of audio
 - :cpp:func:`bk_aud_eq_deinit` - deinit the eq module of audio

demo主要代码如下所示：

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


