华镇语音识别(wanson_asr)开发指南
=================================

:link_to_translation:`en:[English]`

1、功能概述
--------------------

	华镇语音识别库主要用于离线语音识别，通过模型训练可支持自定义唤醒词和命令词。

2、开发资料
--------------------
	armino平台已经将华镇离线语音识别库适配并集成为平台组件，并提供了用于华镇语音识别应用开发的工程，客户可基于此工程开发。
	
	wanson—asr库的API接口的详细说明请参考源文件: ``.\components\bk_thirdparty\asr\wanson\include\asr.h``

	wanson—asr库存放路径：``.\components\bk_thirdparty\asr\wanson\lib\libasr.a``

.. note::
	1、华镇语音识别库要求音频流格式为：单声道、16K采样率、16bit位宽。
	2、华镇语音识别库基于浮点运算。
	3、当修改唤醒词或命令词后，需要替换 ``libasr.a`` 库。

3、实时离线语音识别应用开发
-------------------------------

	基于华镇语音识别库开发实时离线识别的流程如下：

	1、初始化语音识别
	2、初始化音频采样
	2、运行语音识别
	3、打开音频采样

	接口调用实例如下：

::

	/* 初始化语音识别库 */
	Wanson_ASR_Init()
	//复位语音识别库
	Wanson_ASR_Reset();

	/* 初始化音频传输 */
	aud_intf_drv_setup.aud_intf_tx_mic_data = aud_asr_handle;
	//初始化音频组件
	bk_aud_intf_drv_init(&aud_intf_drv_setup);
	aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
	//设置音频组件工作模式
	bk_aud_intf_set_mode(aud_work_mode);
	//初始化语音通话
	aud_intf_mic_setup.samp_rate = AUD_ADC_SAMP_RATE_16K;
	ret = bk_aud_intf_mic_init(&aud_intf_mic_setup);

	/* 开启语音识别 */
	//将采集的数据不断送给算法识别
	Wanson_ASR_Recog((short*)asr_buff, 480, &text, &score);

	/* 开启音频采样 */
	bk_aud_intf_mic_start();

::

	完整的demo请参考源码： ``\components\demos\media\audio\audio_asr``

参考链接
---------------------

	`华镇离线语音识别(wanson_asr)工程 <../../projects_work/thirdparty/wanson_asr/index.html>`_
