声网(agora)开发指南
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------

	声网Agora-iot-sdk主要功能是连接声网云，实现设备和云端服务器之间的音视频数据流的传输。

2、开发资料
--------------------
	armino平台已经将声网Agora-iot-sdk库适配并集成为平台组件，因此客户只需要专注于声网音视频传输应用的开发。

	armino平台提供了用于声网应用开发的工程，该工程提供了语音通话、图传及旋转显示等声网应用开发常见的功能场景。此外平台还对并对该工程进行内存和配置的优化，以满足客户的应用开发需求，因此，客户需要基于此工程开发。

客户有两种开发方案：

	1、基于声网提供的api进行开发
		Agora-iot-sdk API接口的详细说明请参考源文件: ``\components\bk_thirdparty\agora-iot-sdk\include\agora_rtc_api.h``

	2、armino将声网api封装成了一个组件，客户可基于该组件进行快速简便的二次开发
		封装的声网组件源码请参考源文件：``\projects\thirdparty\agora\main\agora_sdk\agora_rtc.c``
		封装的声网组件的api请参考源文件：``\projects\thirdparty\agora\main\include\agora_rtc.h``

.. note::
	1、建议采用第二种开发方案，开发难度较小，且提供了参考demo，下述章节会详细描述如何使用封装后的声网组件快速开发常用的功能场景应用代码。
	2、如选择第一种开发方案，请直接参考声网提供的开发指南。

3、音视频传输场景开发
--------------------------

	基于声网云开发语音通话和设备至云端单向图传的流程如下：

	1、初始化声网sdk
	2、运行声网sdk
	3、打开音频传输
	4、打开图传

	接口调用实例如下：

::

	/* 采用默认配置初始化声网sdk */
	agora_rtc_config_t agora_rtc_config = DEFAULT_AGORA_RTC_CONFIG();
	//创建一个帧率计算器，用于根据网速进行流控
	cc_handle = create_bps_fps_calculator(2);
	//初始化声网sdk
	bk_agora_rtc_create(&agora_rtc_config, (agora_rtc_msg_notify_cb)agora_rtc_user_notify_msg_handle);

	/* 运行声网sdk */
	bk_agora_rtc_start(&agora_rtc_option);

	/* 初始化音频传输 */
	aud_intf_drv_setup.aud_intf_tx_mic_data = send_agora_audio_frame;
	//初始化音频组件
	bk_aud_intf_drv_init(&aud_intf_drv_setup);
	aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
	//设置音频组件工作模式
	bk_aud_intf_set_mode(aud_work_mode);
	//初始化语音通话
	bk_aud_intf_voc_init(aud_intf_voc_setup);
	//注册声网sdk接收语音数据的处理函数
	bk_aggora_rtc_register_audio_rx_handle((agora_rtc_audio_rx_data_handle)agora_rtc_user_audio_rx_data_handle);
	//开启语音传输
	bk_aud_intf_voc_start();

	/* 开启图传 */
	media_app_camera_open(camera_type, camera_ppi);	// PPI_320X240   PPI_640X480
	media_app_register_read_frame_cb(app_media_read_frame_callback);
	media_app_save_start("unused_name");

::

	初始化声网sdk时需要传入sdk的回调函数，用于接收处理声网sdk上报的消息。
	可参考下述设计：

::

	void agora_rtc_user_notify_msg_handle(agora_rtc_msg_t *p_msg)
	{
		switch(p_msg->code)
		{
			case AGORA_RTC_MSG_JOIN_CHANNEL_SUCCESS:	//成功加入channel
				g_connected_flag = true;
				break;
			case AGORA_RTC_MSG_USER_JOINED:	
			case AGORA_RTC_MSG_USER_OFFLINE:
				break;
			case AGORA_RTC_MSG_CONNECTION_LOST:			//声网连接断开
				LOGE("Lost connection. Please check wifi status.\n");
				g_connected_flag = false;
				break;
			case AGORA_RTC_MSG_INVALID_APP_ID:			//appid无效
				LOGE("Invalid App ID. Please double check.\n");
				break;
			case AGORA_RTC_MSG_INVALID_CHANNEL_NAME:	//channel无效
				LOGE("Invalid channel name. Please double check.\n");
				break;
			case AGORA_RTC_MSG_INVALID_TOKEN:
			case AGORA_RTC_MSG_TOKEN_EXPIRED:			//token无效或超期
				LOGE("Invalid token. Please double check.\n");
				break;
			case AGORA_RTC_MSG_BWE_TARGET_BITRATE_UPDATE:	//网速上报
				g_target_bps = p_msg->data.bwe.target_bitrate;
				break;
			default:
				break;
		}
	}

::

	完整的demo请参考源码： ``\projects\thirdparty\agora\main\agora_rtc_demo.c``

参考链接
---------------------

	`声网(agora)工程 <../../projects_work/thirdparty/agora/index.html>`_
