DOORBELL Cli
================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------------
	In order to facilitate the verification of doorbell functions, a series of cli CMDs are designed for this purpose, all cmd begin with doorbell.

2、Code path
--------------------------
	path: ``\components\demos\media\doorbell\src\doorbell_core.c``


3、Cli command introduction
------------------------------------------
The supported CMDs are as follows:


	1.doorbell tcp
	 - Function Description
		init tcp transfer, transfer media audio and video by net.
	 - CMD Format
		doorbell tcp
		this cmd no param

	 - Test Case
		``doorbell tcp``

	2.doorbell udp
	 - Function Description
		init udp transfer, transfer media audio and video by net.
	 - CMD Format
		doorbell udp
		this cmd no param

	 - Test Case
		``doorbell udp``

	3.doorbell udp_c
	 - Function Description
		Use the current device as the client of UDP communication, responsible for sending the audio data to the slave. Tested as a test of two-way speaking ability。

	 - CMD Format
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

	 - Test Case
		 | as udp_client transfer,open dvp camera and config ppi 640X480:  ``doorbell udp_c dvp 640X480``

	4.doorbell udp_s
	 - Function Description
		The current device is used as the slave end of UDP communication, responsible for sending audio data to the client end,
		receiving audio and video data from the client end, and playing and LCD display, as a test for the two-way intercom function.
	 - CMD Format
		doorbell udp_s param1 param2

		+-----------+------------------------------------------------------------------------+
		|param1     | LCD resolution type, open LCD display                                  |
		|           |  Range: 480X272, 320X480, 1024X600. etc                                |
		+-----------+------------------------------------------------------------------------+
		|param2     | LCD display name                                                       |
		|           |  "hx8282", "md0430r", "md0700r", "nt35510", "nt35512", etc             |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| as udp_slave transfer, open LCD and config ppi 480X272: ``doorbell udp_s 480X272``
		| as udp_slave transfer, open LCD and config ppi 1024X600, lcd name: ``doorbell udp_s 1024X600 md0700r``


    5.doorbell cs2_p2p_server
     - Function Description
        As a CS2(www.cs2-network.com)server, demo recv camera/audio data and display from client, and transfer audio data to client.
     - CMD format
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
        |transfer type       | recv transfer type                                                     |
        |                    | 1:video 2:audio 3:video and audio                                      |
        +--------------------+------------------------------------------------------------------------+
        |DID                 | device DID，apply to CS2                                               |
        +--------------------+------------------------------------------------------------------------+
        |APILicense[:CRCKey] | APILicense，apply to CS2                                               |
        |                    | CRCKey，need input if CS2 provide                                      |
        +--------------------+------------------------------------------------------------------------+
        |InitString          | InitString，CS2 server address，apply to CS2                           |
        +--------------------+------------------------------------------------------------------------+

     - Test Case
        | Only camera and LCD with 640X480 ``doorbell cs2_p2p_server 0 0 st7282 1 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | Camera/LCD/audio ``doorbell cs2_p2p_server 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | Camera/LCD/audio without CRCKey ``doorbell cs2_p2p_server 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``

     - Attention
        Currently, CS2 P2P server/client can't be enable at the same time. CS2 P2P API reference components/bk_thirdparty/P2P/CS2/include/PPCS_API.h

    6.doorbell cs2_p2p_client
     - Function Description
        As a CS2(www.cs2-network.com) client, demo recv audio data and display from client, and transfer camera/audio data to server.
     - CMD format
        doorbell cs2_p2p_client <camera type> <camera resolution> <LCD resolution> <LCD name> <transfer type> <DID> <APILicense[:CRCKey]> <InitString>

        +--------------------+------------------------------------------------------------------------+
        |camera type         | camera type                                                            |
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
        |transfer type       | recv transfer type                                                     |
        |                    | 1:video 2:audio 3:video and audio                                      |
        +--------------------+------------------------------------------------------------------------+
        |DID                 | device DID, apply to CS2                                               |
        +--------------------+------------------------------------------------------------------------+
        |APILicense[:CRCKey] | APILicense，apply to CS2                                               |
        |                    | CRCKey, need input if CS2 provide                                      |
        +--------------------+------------------------------------------------------------------------+
        |InitString          | InitString, CS2 server address, apply to CS2                           |
        +--------------------+------------------------------------------------------------------------+

     - Test Case
        | Only camera and LCD with 640X480 ``doorbell cs2_p2p_client uvc 0 0 st7282 1 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | Camera/LCD/audio  ``doorbell cs2_p2p_client uvc 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense:CRCKey XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``
        | Camera/LCD/audio without CRCKey ``doorbell uvc cs2_p2p_client 0 0 st7282 3 XXXXXXX-123456-XXXXX APILicense XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``

     - Attention
        Currently, CS2 P2P server/client can't be enable at the same time. CS2 P2P API reference components/bk_thirdparty/P2P/CS2/include/PPCS_API.h

     - How to get test ID
        | Please contract CS2 Inc.

     - ShenZhen CS2 network Tech Inc.
        | Room 2002B, Building 3, D part, Tian An Yun Gu center, north of Xuegang Road, Longgang Area, Shenzhen
        | +86 755 36600360
        | +86 755 36600361
        | charlie@cs2-network.com
