DOORBELL Cli
================

:link_to_translation:`en:[English]`

1、功能概述
--------------------------
	为了便于验证门铃的相关功能，为此设计了一系列的CLI CMD。所有的DVP CLI命令以 ``doorbell`` 开头，后面接具体命令和参数。如果命令执行成功，则返回 ``CMDRSP:OK``。如果命令执行失败，则返回 ``CMDRSP:ERROR``。
	当前命令是否生效是由宏 ``CONFIG_DOORBELL``。


2、代码路径
--------------------------
	路径: ``\components\demos\media\doorbell\src\doorbell_core.c``

3、cli命令简介
--------------------------
支持的命令如下:
	1.doorbell tcp
	 - 功能说明
		初始化tcp传输，将多媒体音视频数据，通过网络进行传输
	 - 命令格式
		doorbell tcp

		此命令没有参数。
	 - 测试用例
		``doorbell tcp``

	2.doorbell udp
	 - 功能说明
		初始化udp传输，将多媒体音视频数据，通过网络进行传输
	 - 命令格式
		doorbell tcp

		此命令没有参数。
	 - 测试用例
		``doorbell udp``

	3.doorbell udp_c
	 - 功能说明
		将当前设备作为UDP通信的client端，负责将音视频数据发送给slave端。作为测试双向对讲功能进行测试
	 - 命令格式
		doorbell udp_c param1 param2 param3 param4

		+-----------+------------------------------------------------------------------------+
		|param1     | Camera type, open camera type, reference ``app_camera_type_t``         |
		|           |  Range: dvp, uvc                                                       |
		+-----------+------------------------------------------------------------------------+
		|param2     | Camera resolution, reference ``media_ppi_t``                           |
		|           |  Range: 240: 320X240, 480: 640X480, 600: 800X600, 720: 1280X720, etc   |
		+-----------+------------------------------------------------------------------------+
		|param3     | LCD display resolution, reference ``media_ppi_t``                      |
		|           |  Range: 240: 320X240, 480: 640X480, 600: 800X600, 720: 1280X720, etc   |
		+-----------+------------------------------------------------------------------------+
		|param4     | LCD display name                                                       |
		|           |  "hx8282", "md0430r", "md0700r", "nt35510", "nt35512", etc             |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 作为udp_client传输，打开dvp摄像头，且配置分辨率为640X480： ``doorbell udp_c dvp 640X480``

	4.doorbell udp_s
	 - 功能说明
		将当前设备作为UDP通信的slave端，负责将音频数据发送给client端，接收client端的音视频数据，并播放和LCD显示，作为测试双向对讲功能进行测试。
	 - 命令格式
		doorbell udp_s param1 param2

		+-----------+------------------------------------------------------------------------+
		|param1     | LCD resolution type, open LCD display                                  |
		|           |  Range: 480X272, 320X480, 1024X600. etc                                |
		+-----------+------------------------------------------------------------------------+
		|param2     | LCD display name                                                       |
		|           |  "hx8282", "md0430r", "md0700r", "nt35510", "nt35512", etc             |
		+-----------+------------------------------------------------------------------------+

	 - 测试用例
		| 作为udp_slave传输，打开LCD屏幕显示，且配置分辨率为480X272： ``doorbell udp_s 480X272``
		| 作为udp_slave传输，打开LCD屏幕显示，且配置分辨率为1024X600, 屏幕的名字： ``doorbell udp_s 1024X600 md0700r``

    5.doorbell cs2_p2p_server
     - 功能说明
        将当前设备作为CS2(尚云P2P云服务)server端，接收client的摄像头、音频数据并播放和LCD显示，也会将本端的音频数据传给对端。
     - 命令格式
        doorbell cs2_p2p_server <camera resolution> <LCD resolution> <LCD name> <transfer type> <DID> <APILicense[:CRCKey]> <InitString>

        +--------------------+------------------------------------------------------------------------+
        |camera resolution   | camera resolution type                                                 |
        |                    |  Range: 480X272, 320X480, 1024X600. etc. set 0 means default(640X480)  |
        +--------------------+------------------------------------------------------------------------+
        |LCD resolution      | LCD resolution type, open LCD display                                  |
        |                    |  Range: 480X272, 320X480, 1024X600. etc. set 0 means default(640X480)  |
        +--------------------+------------------------------------------------------------------------+
        |LCD name            | LCD display name                                                       |
        |                    |  "hx8282", "md0430r", "md0700r", "nt35510", "nt35512", etc             |
        +--------------------+------------------------------------------------------------------------+
        |transfer type       | 接收、传输数据类型                                                     |
        |                    | 1:视频 2:音频 3:视音频                                                 |
        +--------------------+------------------------------------------------------------------------+
        |DID                 | 设备DID，需要向CS2申请                                                 |
        +--------------------+------------------------------------------------------------------------+
        |APILicense[:CRCKey] | APILicense，需要向CS2申请                                              |
        |                    | CRCKey，如果CS2有提供，则必须输入                                      |
        +--------------------+------------------------------------------------------------------------+
        |InitString          | InitString，CS2服务器地址，需要CS2提供                                 |
        +--------------------+------------------------------------------------------------------------+

     - 测试用例
        | 仅测试摄像头显示屏640X480 ``doorbell cs2_p2p_server 0 0 st7282 1 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | 测试摄像头显示屏640X480及音频 ``doorbell cs2_p2p_server 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | 测试摄像头显示屏640X480及音频，不带CRCKey ``doorbell cs2_p2p_server 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``

     - 注意
        CS2 P2P server client目前不能同时开启。CS2 P2P api参见components/bk_thirdparty/P2P/CS2/include/PPCS_API.h

    6.doorbell cs2_p2p_client
     - 功能说明
        将当前设备作为CS2(尚云P2P云服务)client端，接收server的音频数据并播放，也会将本端的视音频数据传给对端。
     - 命令格式
        doorbell cs2_p2p_client <camera type> <camera resolution> <LCD resolution> <LCD name> <transfer type> <DID> <APILicense[:CRCKey]> <InitString>

        +--------------------+------------------------------------------------------------------------+
        |camera type         | camera 类型                                                            |
        |                    |  uvc dvp                                                               |
        +--------------------+------------------------------------------------------------------------+
        |camera resolution   | camera resolution type                                                 |
        |                    |  Range: 480X272, 320X480, 1024X600. etc. set 0 means default(640X480)  |
        +--------------------+------------------------------------------------------------------------+
        |LCD resolution      | LCD resolution type, open LCD display                                  |
        |                    |  Range: 480X272, 320X480, 1024X600. etc. set 0 means default(640X480)  |
        +--------------------+------------------------------------------------------------------------+
        |LCD name            | LCD display name                                                       |
        |                    |  "hx8282", "md0430r", "md0700r", "nt35510", "nt35512", etc             |
        +--------------------+------------------------------------------------------------------------+
        |transfer type       | 接收、传输数据类型                                                     |
        |                    | 1:视频 2:音频 3:视音频                                                 |
        +--------------------+------------------------------------------------------------------------+
        |DID                 | 设备DID，需要向CS2申请                                                 |
        +--------------------+------------------------------------------------------------------------+
        |APILicense[:CRCKey] | APILicense，需要向CS2申请                                              |
        |                    | CRCKey，如果CS2有提供，则必须输入                                      |
        +--------------------+------------------------------------------------------------------------+
        |InitString          | InitString，CS2服务器地址，需要CS2提供                                 |
        +--------------------+------------------------------------------------------------------------+

     - 测试用例
        | 仅测试摄像头显示屏640X480 ``doorbell cs2_p2p_client uvc 0 0 st7282 1 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | 测试摄像头显示屏640X480及音频 ``doorbell cs2_p2p_client uvc 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | 测试摄像头显示屏640X480及音频，不带CRCKey ``doorbell uvc cs2_p2p_client 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``

     - 注意
        CS2 P2P server client目前不能同时开启。CS2 P2P api参见components/bk_thirdparty/P2P/CS2/include/PPCS_API.h
