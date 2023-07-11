CS2(CS2 network Tech Inc)P2P demo
===================================

:link_to_translation:`en:[English]`

1. summary
--------------------
	Transfer video and audio data through CS2 P2P platform.

2. source code path
--------------------
 - client demo: ``components/demos/media/doorbell/src/doorbell_cs2_p2p_client.c``
 - server demo: ``components/demos/media/doorbell/src/doorbell_cs2_p2p_server.c``
 - middleware compoment: ``components/demos/net/p2p/cs2``


 - cs2 api/lib: ``components/bk_thirdparty/P2P/CS2``


3. build command
--------------------
 - server: ``make bk7256 PROJECT=thirdparty/p2p_server``
 - client: ``make bk7256 PROJECT=thirdparty/p2p_client``

4. test server
--------------------
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

5. test client
--------------------
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

6. Attention
--------------------
    Currently, CS2 P2P server/client can't be enable at the same time.

7. How to get test ID
-----------------------------------
    | Please contract CS2 Inc.

8. ShenZhen CS2 network Tech Inc.
-----------------------------------
    | www.cs2-network.com
    | Room 2002B, Building 3, D part, Tian An Yun Gu center, north of Xuegang Road, Longgang Area, Shenzhen
    | +86 755 36600360
    | +86 755 36600361
    | charlie@cs2-network.com
