DOORBELL
==========================

:link_to_translation:`zh_CN:[Chinese]`

1 Overview
---------------------------------------
	The main function of the doorbell is the joint module, real-time image acquisition, display screen, sound acquisition and communication.

2 Code Path
---------------------------------------
	The path of demos: ./components/media/cli/media_cli.c``, ``./components/media/camera/camera_act.c``, ``./components/media/transfer/transfer_act.c``, ``./components/media/lcd/lcd_act.c``

3 Client command
---------------------------------------
	The commands support by doorbell

	+----------------------------------------+--------------------------------+---------------------------------------+
	|             Command                    |            Param               |              Description              |
	+========================================+================================+=======================================+
	|                                        | param1:softap_ssid             | setup a softap and set name           |
	| ap param1 param2                       +--------------------------------+---------------------------------------+
	|                                        | param2:softap_key              | set softap connect key                |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:station_ssid            | connect a station and set name        |
	| sta param1 param2                      +--------------------------------+---------------------------------------+
	|                                        | param2:station_key             | set station connect key               |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:3                       | open doorbell function                |
	| doorbell param1 [param2]               +--------------------------------+---------------------------------------+
	|                                        | param2:if open uvc, choosable  | set open/disopen uvc 1/0              |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media dvp open [param]                 | param:dvp resolution, choosable| set sensor resolution, and open dvp   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media dvp close                        | NULL                           | close dvp sensor                      |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media lcd open [param]                 | param1:display size            | lcd display size, default:480X272     |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media lcd close                        | NULL                           | close lcd function, and jpeg decode   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media capture param                    | param:xxx.jpg                  | capture save to sdcard, and set name  |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media uvc open [param]                 | param:uvc resolution, choosable| set sensor resolution, and open uvc   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media uvc close                        | NULL                           | close uvc sensor                      |
	+----------------------------------------+--------------------------------+---------------------------------------+

	The macro configuration that the demo runs depends on:

	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description       |                  File                      |  value  |
	+======================================+========================+============================================+=========+
	|CONFIG_DOORBELL                       |support DOORBELL        |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_JPEG_ENCODE                    |suport hw jpeg ecoder   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA                         |suport use dvp camera   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA_USE_I2C1                |suport use I2C1         |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_AUDIO                          |support audio function  |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_LCD                            |support LCD function    |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_USB_UVC                        |support UVC camera      |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+

4 Demo introduction
-------------------------------------
	The steps performed by the demo are as follows:

	1. Prepare dvp camera or UVC camera, LCD screen (rgb or 8080):

	2. Start a softap:
		ap doorbell_test 1234567890

	3. Connect the mobile phone to this app

	4. Send the doorbell command:

		doorbell


	5. The mobile app (RealtimeVideo) setting sequence and process are shown in the following figure:

	6. If you need to display it on the LCD screen

		media lcd open


5 App usage process
--------------------------
	The following process takes the board as a softap as an example to introduce the use of apk, after the mobile phone is connected to the ap enabled by the board:

	1. Figure 1 is a schematic diagram of the app

	2. Figure 2 is the main interface of the app

		Among them, choose 1 to see Figure 3.

		- 1: Settings menu;
		- 2: Update apk and rollback apk menu;

	3. Figure 3 is the setting interface menu

		Among them:

		- 1: For the setting interface as shown in Figure 4;
		- 2: button to return to the main interface;

	4. Figure 4 is the real setting interface

		Among them, the setting instructions are as follows, after the setting is completed, return to the main interface of Figure 2

		- 1: Set the solution, currently support video_transfer and doorbell, select doorbell here;
		- 2: Set the data transmission mode, currently supports UDP and TCP, and UDP is selected by default;
		- 3: Set the ip address of the peer, the default is ``192.168.0.1`` in ap mode, and it is not required to be modified, and in sta mode, it is set to the ip address of the peer;
		- 4: Set the camera type, currently supports DVP and UVC, set according to the type of camera you use;
		- 5: Set the output resolution of the camera;
		- 6: Set the resolution of the LCD output, according to the LCD screen resolution you use;

	5. Figure 5 is the function enable setting interface

		Among them, the function enable description is as follows:

		- 1: Switch video image transmission;
		- 2: switch voice;
		- 3: Photo switch, currently not supported;
		- 4: Switch LCD screen display;

.. note::

	Set the peer IP address in Figure 4. When the board is softap, the default is ``192.168.0.1``. When the board is used as a staion, the mobile phone and the board are connected to the same ap,
	and the filled IP address can be passed through the command `` ip`` to get.
	In addition, the app also supports the function of mobile phone image transfer, that is, the command ``video_transfer -a|s ssid key``, but step 1 in Figure 4 must be set to ``video_transfer`` mode.

.. figure:: ../../../../../common/_static/RealtimeVideo_app.jpg
    :align: center
    :alt: RealtimeVideo_app
    :figclass: align-center

    Figure 1. doorbell apk

.. figure:: ../../../../../common/_static/app_set0.jpg
    :align: center
    :alt: RealtimeVideo_app_screen
    :figclass: align-center

    Figure 2. RealtimeVideo_app Main screen

.. figure:: ../../../../../common/_static/app_set1.jpg
    :align: center
    :alt: RealtimeVideo_app_set_menu
    :figclass: align-center

    Figure 3. RealtimeVideo_app Set menu

.. figure:: ../../../../../common/_static/app_set2.jpg
    :align: center
    :alt: RealtimeVideo_app_set
    :figclass: align-center

    Figure 4. RealtimeVideo_app set

.. figure:: ../../../../../common/_static/app_set3.jpg
    :align: center
    :alt: RealtimeVideo_function_set
    :figclass: align-center

    Figure 5. RealtimeVideo_function set