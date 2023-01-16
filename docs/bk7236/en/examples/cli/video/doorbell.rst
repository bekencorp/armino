DOORBELL Cli
================

:link_to_translation:`zh_CN:[Chinese]`

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
		doorbell udp_c param1 param2

		+-----------+------------------------------------------------------------------------+
		|param1     | Camera type, open camera type                                          |
		|           |  Range: dvp, uvc                                                       |
		+-----------+------------------------------------------------------------------------+
		|param2     | Camera resolution, reference ``camera_intf_set_sener_cfg()``           |
		|           |  Range: 240: 320X240, 480: 640X480, 600: 800X600, 720: 1280X720, etc   |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		 | as udp_client transfer,open dvp camera and config ppi 640X480:  ``doorbell udp_c dvp 640X480``

	4.doorbell udp_s
	 - Function Description
		The current device is used as the slave end of UDP communication, responsible for sending audio data to the client end,
		receiving audio and video data from the client end, and playing and LCD display, as a test for the two-way intercom function.
	 - CMD Format
		doorbell udp_s param1

		+-----------+------------------------------------------------------------------------+
		|param1     | LCD resolution type, open LCD display                                  |
		|           |  Range: 480X272, 320X480, 1024X600. etc                                |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| as udp_slave transfer, open LCD and config ppi 480X272: ``doorbell udp_s 480X272``