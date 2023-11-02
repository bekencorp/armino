agora development guide
========================

:link_to_translation:`zh_CN:[中文]`

1、Functional Overview
---------------------------------

	The main function of Agora-iot-sdk is to connect to the Agora cloud to realize the transmission of audio and video data streams between the device and the cloud server.

2、development data
---------------------------------

	The armino platform has adapted and integrated the Agora-iot-sdk library of Agora as a platform component, so customers only need to focus on the development of audio and video transmission applications of Agora.

	The armino platform provides a project for the development of Acoustics applications, which provides common functional scenarios for the development of Acoustics applications such as voice calls, image transmission, and rotating display. In addition, the platform also optimizes the memory and configuration of the project to meet the customer's application development needs. Therefore, the customer needs to develop based on this project.

Customers have two development options:

	1. Develop based on the api provided by agora
		For detailed description of the Agora-iot-sdk API interface, please refer to the source file: ``\components\bk_thirdparty\agora-iot-sdk\include\agora_rtc_api.h``

	2. Armino encapsulates the Acoustic Network API into a component, and customers can carry out quick and easy secondary development based on this component
		Please refer to the source file for the source code of the packaged agora component: ``\projects\thirdparty\agora\main\agora_sdk\agora_rtc.c``
		Please refer to the source file for the API of the encapsulated agora component: ``\projects\thirdparty\agora\main\include\agora_rtc.h``

.. note::
	1. It is recommended to adopt the second development plan, which is less difficult to develop and provides a reference demo. The following chapters will describe in detail how to use the packaged AVoice components to quickly develop application codes for common functional scenarios.
	2. If you choose the first development plan, please refer directly to the development guide provided by agora.

3、Audio and video transmission scene development
----------------------------------------------------------

The process of developing voice calls and one-way image transmission from devices to the cloud based on Acoustics Cloud is as follows:

	1. Initialize the sound network sdk
	2. Run the agora sdk
	3. Turn on audio transmission
	4. Open image transmission

	Examples of interface calls are as follows:

::

	/* Initialize AcousticNet with default configuration sdk */
	agora_rtc_config_t agora_rtc_config = DEFAULT_AGORA_RTC_CONFIG();
	//Create a frame rate calculator for flow control based on network speed
	cc_handle = create_bps_fps_calculator(2);
	//init agora sdk
	bk_agora_rtc_create(&agora_rtc_config, (agora_rtc_msg_notify_cb)agora_rtc_user_notify_msg_handle);

	/* run agora sdk */
	bk_agora_rtc_start(&agora_rtc_option);

	/* init audio transfer */
	aud_intf_drv_setup.aud_intf_tx_mic_data = send_agora_audio_frame;
	//init audio component
	bk_aud_intf_drv_init(&aud_intf_drv_setup);
	aud_work_mode = AUD_INTF_WORK_MODE_VOICE;
	//set audio component work mode
	bk_aud_intf_set_mode(aud_work_mode);
	//init voice call
	bk_aud_intf_voc_init(aud_intf_voc_setup);
	//Register the agora sdk to receive the voice data processing function
	bk_aggora_rtc_register_audio_rx_handle((agora_rtc_audio_rx_data_handle)agora_rtc_user_audio_rx_data_handle);
	//Turn on voice transmission
	bk_aud_intf_voc_start();

	/* Turn on image transmission */
	media_app_camera_open(camera_type, camera_ppi);	// PPI_320X240   PPI_640X480
	media_app_register_read_frame_cb(app_media_read_frame_callback);
	media_app_save_start("unused_name");

::

	When initializing the agora SDK, you need to pass in the callback function of the SDK to receive and process the messages reported by the SoundNet SDK.

	The following designs can be referred to:

::

	void agora_rtc_user_notify_msg_handle(agora_rtc_msg_t *p_msg)
	{
		switch(p_msg->code)
		{
			case AGORA_RTC_MSG_JOIN_CHANNEL_SUCCESS:	//join channel success
				g_connected_flag = true;
				break;
			case AGORA_RTC_MSG_USER_JOINED:	
			case AGORA_RTC_MSG_USER_OFFLINE:
				break;
			case AGORA_RTC_MSG_CONNECTION_LOST:			//agora connection disconnect
				LOGE("Lost connection. Please check wifi status.\n");
				g_connected_flag = false;
				break;
			case AGORA_RTC_MSG_INVALID_APP_ID:			//appid invalid
				LOGE("Invalid App ID. Please double check.\n");
				break;
			case AGORA_RTC_MSG_INVALID_CHANNEL_NAME:	//channel invalid
				LOGE("Invalid channel name. Please double check.\n");
				break;
			case AGORA_RTC_MSG_INVALID_TOKEN:
			case AGORA_RTC_MSG_TOKEN_EXPIRED:			//token is error
				LOGE("Invalid token. Please double check.\n");
				break;
			case AGORA_RTC_MSG_BWE_TARGET_BITRATE_UPDATE:	//bps update
				g_target_bps = p_msg->data.bwe.target_bitrate;
				break;
			default:
				break;
		}
	}

::

	Please refer to the source code for the complete demo： ``\projects\thirdparty\agora\main\agora_rtc_demo.c``

Reference link
--------------------------

	`Agora project <../../projects_work/thirdparty/agora/index.html>`_
