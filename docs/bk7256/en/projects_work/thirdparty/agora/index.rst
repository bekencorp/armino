Agora-iot-sdk demo
===================================

:link_to_translation:`zh_CN:[中文]`

1 Functional Overview
--------------------------
	The sound network-based Agora-IoT-SDK connecting sound network cloud to realize the one-way diagram of equipment, mobile phone voice calls and equipment to mobile phones and rotate display

2 Code Path
--------------------
	demo path: ``\projects\thirdparty\agora``

	For the detailed description of the API interface of the Agora-iot-sdk library, please refer to the source file: ``\components\bk_thirdparty\agora-iot-sdk\include\agora_rtc_api.h``

3 Cli command introduction
-----------------------------------
The commands supported by the demo are as follows:

+----------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|Command                                                                                                   |Description                                        |
+==========================================================================================================+===================================================+
|agora_test {audio start|stop appid audio_type sampple_rate aec_en}                                        |voice call                                         |
+----------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|agora_test {video start|stop appid video_type video_ppi}                                                  |image transmission                                 |
+----------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|agora_test {both start|stop appid audio_type sample_rate video_type video_ppi aec_en lcd_ppi lcd_driver}  |voice call, image transmission and rotate display  |
+----------------------------------------------------------------------------------------------------------+---------------------------------------------------+

The command parameters are described as follows:

    +--------------------+-------------------------------------------------+
    |appid               | an appid for registration                       |
    +--------------------+-------------------------------------------------+
    |audio_type          | mic and speaker type:                           |
    |                    |  - 0: ``onboard``                               |
    |                    |  - 1: ``UAC``                                   |
    +--------------------+-------------------------------------------------+
    |sampple_rate        | sample rate:                                    |
    |                    |  - ``8000``                                     |
    |                    |  - ``16000``                                    |
    +--------------------+-------------------------------------------------+
    |aec_en              | AEC function control:                           |
    |                    |  - 0: ``disable AEC``                           |
    |                    |  - 1: ``enable AEC``                            |
    +--------------------+-------------------------------------------------+
    |video_type          | camera type:                                    |
    |                    |  - ``APP_CAMERA_DVP_JPEG``                      |
    |                    |  - ``APP_CAMERA_DVP_YUV``                       |
    |                    |  - ``APP_CAMERA_UVC_MJPEG``                     |
    +--------------------+-------------------------------------------------+
    |video_ppi           | resolution:                                     |
    |                    |  - ``PPI_320X240``                              |
    |                    |  - ``PPI_320X480``                              |
    |                    |  - ``PPI_480X320``                              |
    |                    |  - ``PPI_480X480``                              |
    |                    |  - ``PPI_480X800``                              |
    |                    |  - ``PPI_640X480``                              |
    |                    |  - ``PPI_800X480``                              |
    +--------------------+-------------------------------------------------+
    |lcd_driver          | lcd driver name:                                |
    |                    |  - ``st7282``                                   |
    |                    |  - ``gc9503v``                                  |
    |                    |  - ``h050iwv``                                  |
    |                    |  - ``hx8282``                                   |
    |                    |  - ``md0430r``                                  |
    |                    |  - ``md0700r``                                  |
    |                    |  - ``nt35512``                                  |
    |                    |  - ``nt35510``                                  |
    |                    |  - ``st7796s``                                  |
    |                    |  - ``st7710s``                                  |
    |                    |  - ``st7701s``                                  |
    |                    |  - ``sn5st7701s``                               |
    |                    |  - ``st7701s_ly``                               |
    +--------------------+-------------------------------------------------+
    |lcd_ppi             | resolution:                                     |
    |                    |  - ``PPI_320X240``                              |
    |                    |  - ``PPI_320X480``                              |
    |                    |  - ``PPI_480X320``                              |
    |                    |  - ``PPI_480X480``                              |
    |                    |  - ``PPI_480X800``                              |
    |                    |  - ``PPI_640X480``                              |
    |                    |  - ``PPI_800X480``                              |
    +--------------------+-------------------------------------------------+


Project Compilation instruction: ``make bk7256 PROJECT = thirdparty/agora``

4. Demonstration introduction
-------------------------------------------

The preparation of DEMO execution is as follows:

	- 1、Prepare the Android machine that can be connected to the external network and install the APK ``Open Live`` to test the test
	- 2、Go to Agora official website to register to apply for testing for testing ``appid`` , please refer to the information below for detailed operation
	- 3、Prepare to access the 2.4GHz hotspot of the external network for Demo board connection and use

The steps of demo execution are as follows:

	1. Mobile APK configuration

	 - Open ``Open Live`` APK, enter the registration application ``appid`` , click ``Enter`` to enter
	 - Enter Channel name `` hello_bk7256demo`` , click Join ROOM to enter, select ``BroadCaster``

	2.Demo board wifi connection

	 - DEMO board sending instructions ``sta test xxxxxxx`` connect 2.4GHz hot spots named TEST

	3. Open and close voice calls

	 - The demo board sends the command ``agora_test audio start appid audio_type sample_rate aec_en`` to open the voice call
	 - The demo board sends the command ``agora_test audio stop appid audio_type sample_rate aec_en`` to close the voice call

	4. Turn on and off video transmission

	 - The demo board sends the command ``agora_test video start appid video_type video_ppi`` to open the image transmission
	 - The demo board sends the command ``agora_test video stop appid video_type video_ppi`` to close the image transmission

	5. Turn on and off voice call and image transmission

	 - The demo board sends the command ``agora_test both start appid audio_type sample_rate video_type video_ppi aec_en lcd_ppi lcd_driver`` to open voice call, image transmission and rotate display
	 - The demo board sends the command ``agora_test both stop appid audio_type sample_rate video_type video_ppi aec_en lcd_ppi lcd_driver`` to turn off voice calls, image transmission and rotate display

5. Audio configuration
----------------------------

The audio code in the demo is developed based on the ``aud_intf`` component. By configuring the input parameters of the ``bk_aud_intf_voc_init`` interface, the selection of mic and speaker types and the setting of the sampling rate can be realized.

The supported sample rates are as follows:
	- 1、AUD_INTF_VOC_SAMP_RATE_8K: 8K sampling rate (recommended configuration)
	- 2、AUD_INTF_VOC_SAMP_RATE_16K: 16K sampling rate (Currently, the SoundNet SDK does not support)

.. note::
    16K sampling rate will increase the load of network bandwidth, which will affect the frame rate of image transmission, so it is recommended to sample at 8K sampling rate

The supported mic and speaker types are as follows:
	- 1、AUD_INTF_MIC_TYPE_BOARD: onboard mic (recommended configuration)
	- 2、AUD_INTF_MIC_TYPE_UAC: uac type mic
	- 3、AUD_INTF_SPK_TYPE_BOARD: onboard speaker (recommended configuration)
	- 4、AUD_INTF_SPK_TYPE_UAC: uac type speaker

For detailed description of Aud_Intf API interface, please refer to the same page: ``/api-reference/multi_media/bk_aud_intf.html``

6. Video configuration
----------------------------

The part of the image transmission code in the demo is developed based on the ``media`` component. By configuring the input parameters of the ``media_app_camera_open`` interface, the selection of the camera type and the setting of the resolution can be realized.

The supported types are as follows:
	- 1、APP_CAMERA_DVP_JPEG: DVP camera in JPEG format (recommended configuration)
	- 2、APP_CAMERA_DVP_YUV: DVP camera in YUV format (Currently, the SoundNet SDK does not support)
	- 3、APP_CAMERA_UVC_MJPEG: UVC camera in MJPEG format

The supported resolutions are as follows:
	- 1、PPI_320X240
	- 2、PPI_320X480
	- 3、PPI_480X320
	- 4、PPI_480X480
	- 5、PPI_480X800
	- 6、PPI_640X480
	- 7、PPI_800X480

.. note::
  - 1、The underlying hardware supports a variety of common resolutions, but it is not recommended to configure a higher sampling rate due to the limitations of the Acoustics SDK and network bandwidth

7. Reference materials
----------------------------

	`agora development guide <../../../developer-guide/agora/index.html>`_

	agora reference document: https://docs.agora.io/cn/Agora%20Platform/manage_projects?platform=Android

	Agora appid register link：https://sso2.agora.io/cn/v5/login?_gl=1%2ardr355%2a_ga%2aMzkyNDM4ODYyLjE2NzM1MTM3MTU.%2a_ga_BFVGG7E02W%2aMTY3ODg1MjM0My4xMi4wLjE2Nzg4NTIzNDYuMC4wLjA.

	Apk download link: http://dl.bekencorp.com/apk/shengwang/OpenLive_input_appid.apk
