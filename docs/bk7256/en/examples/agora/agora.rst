Agora-iot-sdk demo
===================================

:link_to_translation:`zh_CN:[中文]`

1 Functional Overview
--------------------------
	The sound network-based Agora-IoT-SDK connecting sound network cloud to realize the one-way diagram of equipment and mobile phone voice calls and equipment to mobile phones.

2 Code Path
--------------------
	demo path: ``\components\demos\media\agora``

	For the detailed description of the API interface of the AEC library, please refer to the same webpage: ``/api-reference/multi_media/bk_aec.html``

3 Cli command introduction
-----------------------------------
The commands supported by the demo are as follows:

+-----------------------------------------------------------+---------------------------+
|Command                                                    |Description                |
+===========================================================+===========================+
|agora_test {audio|video start|stop appid}                  |Control voice and drawing  |
+-----------------------------------------------------------+---------------------------+

DEMO Compilation instruction: ``Make BK7256 Project = ThirdParty/Agora``

4. Demonstration introduction
-------------------------------------------

The preparation of DEMO execution is as follows:

	1. Prepare the Android machine that can be connected to the external network and install the APK ``Open Live`` to test the test
	2. Go to Agora official website to register to apply for testing for testing ``appid`` , please refer to the information below for detailed operation
	3. Prepare to access the 2.4GHz hotspot of the external network for Demo board connection and use

The steps of demo execution are as follows:

	1. Mobile APK configuration

	 - Open ``Open Live`` APK, enter the registration application ``appid`` , click ``Enter`` to enter
	 - Enter Channel name `` hello_bk7256demo`` , click Join ROOM to enter, select ``BroadCaster``

	2.Demo board wifi connection

	 - DEMO board sending instructions ``sta test xxxxxxx`` connect 2.4GHz hot spots named TEST

	3. Open voice calls or diagrams

	 - DEMO board sending instructions ``Agora_test Audio Start Appid`` to open voice calls. Among them, Appid is an appid for registration
	 - DEMO board sending instructions ``Agora_test Voideo Start Appid`` to open the map, of which appid is the appid application appid

	4. Close voice calls or diagrams

	 - DEMO board sending instructions ``Agora_test Audio Stop Appid`` Close voice calls, of which Appid is an appid for registration
	 - DEMO board sending instructions ``Agora_test Voideo Stop Appid`` Close the map, of which Appid is the Appid for registration

5. Reference materials
----------------------------
	Sound network reference document: https://docs.agora.io/cn/Agora%20Platform/manage_projects?platform=Android

	Agora appid register link：https://sso2.agora.io/cn/v5/login?_gl=1%2ardr355%2a_ga%2aMzkyNDM4ODYyLjE2NzM1MTM3MTU.%2a_ga_BFVGG7E02W%2aMTY3ODg1MjM0My4xMi4wLjE2Nzg4NTIzNDYuMC4wLjA.

	Apk download link: http://dl.bekencorp.com/apk/shengwang/OpenLive_input_appid.apk
