声网Agora-iot-sdk demo
========================

:link_to_translation:`en:[English]`

1、功能概述
--------------------
	基于声网的Agora-iot-sdk连接声网云，实现设备和手机语音通话、设备至手机的单向图传及本地lcd旋转显示。

2、代码路径
--------------------
	demo路径: ``\projects\thirdparty\agora``

	Agora-iot-sdk API接口的详细说明请参考源文件: ``\components\bk_thirdparty\agora-iot-sdk\include\agora_rtc_api.h``

3、cli命令简介
--------------------
demo支持的命令如下表:

+----------------------------------------------------------------------------------------------------------+--------------------------+
|Command                                                                                                   |Description               |
+==========================================================================================================+==========================+
|agora_test {audio start|stop appid audio_type sampple_rate aec_en}                                        |语音通话                  |
+----------------------------------------------------------------------------------------------------------+--------------------------+
|agora_test {video start|stop appid video_type video_ppi}                                                  |图传                      |
+----------------------------------------------------------------------------------------------------------+--------------------------+
|agora_test {both start|stop appid audio_type sample_rate video_type video_ppi aec_en lcd_ppi lcd_driver}  |语音通话、图传和旋转显示  |
+----------------------------------------------------------------------------------------------------------+--------------------------+

命令参数说明如下：

    +--------------------+-------------------------------------------------+
    |appid               | 注册申请的appid                                 |
    +--------------------+-------------------------------------------------+
    |audio_type          | mic 和 speaker 的类型:                          |
    |                    |  - 0: ``板载``                                  |
    |                    |  - 1: ``UAC``                                   |
    +--------------------+-------------------------------------------------+
    |sampple_rate        | 采样率:                                         |
    |                    |  - ``8000``                                     |
    |                    |  - ``16000``                                    |
    +--------------------+-------------------------------------------------+
    |aec_en              | AEC功能控制:                                    |
    |                    |  - 0: ``关闭AEC``                               |
    |                    |  - 1: ``打开AEC``                               |
    +--------------------+-------------------------------------------------+
    |video_type          | camera的类型:                                   |
    |                    |  - ``APP_CAMERA_DVP_JPEG``                      |
    |                    |  - ``APP_CAMERA_DVP_YUV``                       |
    |                    |  - ``APP_CAMERA_UVC_MJPEG``                     |
    +--------------------+-------------------------------------------------+
    |video_ppi           | 分辨率:                                         |
    |                    |  - ``PPI_320X240``                              |
    |                    |  - ``PPI_320X480``                              |
    |                    |  - ``PPI_480X320``                              |
    |                    |  - ``PPI_480X480``                              |
    |                    |  - ``PPI_480X800``                              |
    |                    |  - ``PPI_640X480``                              |
    |                    |  - ``PPI_800X480``                              |
    +--------------------+-------------------------------------------------+
    |lcd_driver          | lcd驱动型号:                                    |
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
    |lcd_ppi             | 分辨率:                                         |
    |                    |  - ``PPI_320X240``                              |
    |                    |  - ``PPI_320X480``                              |
    |                    |  - ``PPI_480X320``                              |
    |                    |  - ``PPI_480X480``                              |
    |                    |  - ``PPI_480X800``                              |
    |                    |  - ``PPI_640X480``                              |
    |                    |  - ``PPI_800X480``                              |
    +--------------------+-------------------------------------------------+

project编译指令： ``make bk7256 PROJECT=thirdparty/agora``


4、演示介绍
--------------------

demo执行的准备工作如下:
	- 1、准备可以连接外网的安卓机，并安装用于测试的APK ``Open Live``
	- 2、去Agora官网注册申请用于测试的 ``APPID`` ，详细操作见下文参考资料
	- 3、准备可以访问外网的2.4GHz热点，供demo板连接使用

demo执行的步骤如下:

	1.手机APK配置
	 - 打开 ``Open Live`` apk，输入注册申请的 ``APPID`` , 点击 ``ENTER`` 进入
	 - 输入channel name ``hello_bk7256demo``,点击JOIN ROOM进入，选择 ``BROADCASTER``

	2.Demo板wifi连接
	 - demo板发送指令 ``sta test xxxxxx`` 连接2.4GHz名为test的热点

	3.打开和关闭语音通话
	 - demo板发送指令 ``agora_test audio start appid audio_type sampple_rate aec_en`` 打开语音通话
	 - demo板发送指令 ``agora_test audio stop appid audio_type sampple_rate aec_en`` 关闭语音通话

	4.打开和关闭图传
	 - demo板发送指令 ``agora_test video start appid video_type video_ppi`` 打开图传
	 - demo板发送指令 ``agora_test video stop appid video_type video_ppi`` 关闭图传

	5.打开和关闭语音通话、图传及旋转显示
	 - demo板发送指令 ``agora_test both start appid audio_type sample_rate video_type video_ppi aec_en lcd_ppi lcd_driver`` 打开语音通话、图传及旋转显示
	 - demo板发送指令 ``agora_test both stop appid audio_type sample_rate video_type video_ppi aec_en  lcd_ppi lcd_driver`` 关闭语音通话、图传及旋转显示

5、音频配置
--------------------

demo中音频部分代码基于 ``aud_intf`` 组件开发，通过配置 ``bk_aud_intf_voc_init`` 接口的入参可实现mic及speaker类型的选择和采样率的设置。

支持的采样率如下：
	- 1、AUD_INTF_VOC_SAMP_RATE_8K: 8K采样率（建议配置）
	- 2、AUD_INTF_VOC_SAMP_RATE_16K: 16K采样率（目前声网SDK不支持）

.. note::
   16K采样率会增加网络带宽的负载，进而影响图传的帧率，因此建议采样8K采样率

支持的mic和speaker类型如下：
	- 1、AUD_INTF_MIC_TYPE_BOARD: 板载mic（建议配置）
	- 2、AUD_INTF_MIC_TYPE_UAC: uac类型mic
	- 3、AUD_INTF_SPK_TYPE_BOARD: 板载speaker（建议配置）
	- 4、AUD_INTF_SPK_TYPE_UAC: uac类型speaker

Aud_Intf API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud_intf.html``

6、图传配置
--------------------

demo中图传部分代码基于 ``media`` 组件开发，通过配置 ``media_app_camera_open`` 接口的入参可实现camera类型的选择和分辨率的设置。

支持的类型如下：
	- 1、APP_CAMERA_DVP_JPEG: JPEG格式的DVP camera（建议配置）
	- 2、APP_CAMERA_DVP_YUV: YUV格式的DVP camera（目前声网SDK不支持）
	- 3、APP_CAMERA_UVC_MJPEG: MJPEG格式的UVC camera

支持的分辨率如下：
	- 1、PPI_320X240
	- 2、PPI_320X480
	- 3、PPI_480X320
	- 4、PPI_480X480
	- 5、PPI_480X800
	- 6、PPI_640X480
	- 7、PPI_800X480

.. note::
 - 1、底层硬件支持多种常见的分辨率，但受到声网sdk和网络带宽的限制，不建议配置太高的采样率

7、参考链接
--------------------

	`agora开发指南 <../../../developer-guide/agora/index.html>`_

	声网参考文档：https://docs.agora.io/cn/Agora%20Platform/manage_projects?platform=Android

	声网APPID申请链接：https://sso2.agora.io/cn/v5/login?_gl=1%2ardr355%2a_ga%2aMzkyNDM4ODYyLjE2NzM1MTM3MTU.%2a_ga_BFVGG7E02W%2aMTY3ODg1MjM0My4xMi4wLjE2Nzg4NTIzNDYuMC4wLjA.

	APK下载链接：http://dl.bekencorp.com/apk/shengwang/OpenLive_input_appid.apk
