CS2(尚云)P2P demo
========================

:link_to_translation:`en:[English]`

1. 功能概述
--------------------
	基于CS2(尚云)云服务平台，实现双板语音对传和图像单传。

2. 代码路径
--------------------
 - client demo: ``components/demos/media/doorbell/src/doorbell_cs2_p2p_client.c``
 - server demo: ``components/demos/media/doorbell/src/doorbell_cs2_p2p_server.c``
 - 中间组件: ``components/demos/net/p2p/cs2``


 - cs2 api及库: ``components/bk_thirdparty/P2P/CS2``


3. 编译命令
--------------------
 - server: ``make bk7256 PROJECT=thirdparty/p2p_server``
 - client: ``make bk7256 PROJECT=thirdparty/p2p_client``


4. server测试命令
--------------------
 - 功能说明
    将当前设备作为CS2(深圳市尚云互联技术有限公司 www.cs2-network.cn)server端，接收client的摄像头、音频数据并播放和LCD显示，也会将本端的音频数据传给对端。
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

5. client测试命令
--------------------
 - 功能说明
    将当前设备作为CS2(深圳市尚云互联技术有限公司 www.cs2-network.cn)client端，接收server的音频数据并播放，也会将本端的视音频数据传给对端。
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

6. 注意
--------------------
    server client目前为不同的库，不能同时开启。

7. 如何获得测试ID
--------------------
    需要向尚云互联申请

8. 深圳市尚云互联技术有限公司
-------------------------------
    | www.cs2-network.com
    | 深圳市龙岗区坂田街道雪岗北路天安云谷3栋D座2002B
    | +86 755 36600360
    | +86 755 36600361
    | charlie@cs2-network.com

