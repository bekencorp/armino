MEDIA Cli
================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------------

	In order to facilitate the verification of the relevant functions of MEDIA, this document collects the CLI CMD used. All MEDIA CLI commands start with ``media``, followed by specific commands and parameters. If the command is executed successfully, ``CMDRSP:OK`` is returned. If the command execution fails, ``CMDRSP:ERROR`` is returned.

	Whether the current command takes effect is controlled by the macro ``CONFIG_MEDIA``.

2、Code path
--------------------------
	path: ``\components\media\cli\media_cli.c``

3、Cli command introduction
------------------------------------------
The supported CMDs are as follows:

	1. media dvp open
	 - Function Description
		open dvp camera 
	 - CMD Format
		media dvp open [param1] [param2]

		+-----------+------------------------------------------------------------------------+
		| param1    | The camera output resolution, this can omit for 640X480                |
		|           |  Value: 320X240, 480X272, 1280X720, etc.                               |
		+-----------+------------------------------------------------------------------------+
		|           | The dvp data output type, this can omit for MIX                        |
		|           | Value: yuv for YUYV                                                    |
		| param2    | mix for MIX order jpg-YUYV-jpeg-                                       |
		|           | jpeg for JPEG                                                          |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| open dvp and ppi config 640X480:``media dvp open 640X480``

	2. media dvp close
	 - Function Description
		close dvp camera test
	 - CMD Format
		media dvp close

		this cmd no param.
	 - Test Case
		``media dvp close``

	3. media uvc open
	 - Function Description
		open uvc camera test
	 - CMD Format
		media uvc open [param1] [param2]

		+-----------+------------------------------------------------------------------------+
		| param1    | The camera output data type, this can omit for MJPEG                   |
		|           |  Value: MJPEG H264.                                                    |
		+-----------+------------------------------------------------------------------------+
		| param2    | The camera output resolution, this can omit for 640X480                |
		|           |  Value: 320X240, 480X272, 1280X720, etc.                               |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| open uvc and config 800X480 for test: ``media uvc open 800X480``

	4. media uvc close
	 - Function Description
		close uvc camera test
	 - CMD Format
		media uvc close

		this cmd no param.
	 - Test Case
		``media uvc close``

	5. media lcd open
	 - Function Description
		open lcd test
	 - CMD Format
		media lcd open [param1] [param2] [param3]

		+-----------+------------------------------------------------------------------------+
		| param1    | The LCD display resolution, defult by value 480X272                    |
		|           |  Value: 320X480, 1024X600, etc.                                        |
		+-----------+------------------------------------------------------------------------+
		| param2    | The LCD name for diaplay, defult by value st7282                     |
		|           |  Value: hx8282, st7282 etc.                                            |
		+-----------+------------------------------------------------------------------------+
		| param3    | Preamble or not, omit for not need rotate                              |
		|           |  Value: rotate                                                         |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| open lcd display and config ppi 480X800,if need rotate, the cmd:``media lcd open 480X800 nt35512 rotate``


.. Attention::


	SDK Adapted to a variety of screens, if there are multiple screens with the same resolution, you must pass in the param name;
	if the screen resolution in the SDK is unique, just pass in either the param resolution or the name.


6. media lcd dma2d_blend
	 - Function Description
		open lcd icon test
	 - CMD Format
		media lcd dma2d_blend [param1] [param2] 

		+-----------+--------------------------------------------------------------------------------+
		| param1    | display blend clock, wifi or version                                           |
		|           |  Value: clock, wifi, ver.                                                      |
		+-----------+--------------------------------------------------------------------------------+
		| param2    | if param1 is clock, this param2 is such as 12:00, if param1 is wifi,           |
		|           | param2 is wifi lever(0-4),if param1 is ver, the param2 is omit, display defult |
		+-----------+--------------------------------------------------------------------------------+


	 - Test Case
		| after open lcd, send cmd start icon: ``media lcd open 480X800 nt35512 rotate`` 
		`` media lcd dma2d_blend clock 15:19`` or `` media lcd dma2d_blend wifi 1`` 

	7. media lcd dma2d_blend close
	 - Function Description
		close lcd icon test
	 - CMD Format
		media lcd dma2d_blend [param1] [param2] 

		+-----------+--------------------------------------------------------------------------------+
		| param1    | close .if no use param2 will close all, include clock, wifi version            |
		|           |  Value: close.                                                                 |
		+-----------+--------------------------------------------------------------------------------+
		| param2    | if param1 is close, this param2 decided close which module                     |
		|           |  Value: wifi, clock, ver.                                                      |
		+-----------+--------------------------------------------------------------------------------+


	 - Test Case
		| close all icon blend: ``media lcd dma2d_blend close`` 
		| close clock icon blend: ``media lcd dma2d_blend close clock`` 

	8. media lcd close
	 - Function Description
		close lcd display test
	 - CMD Format
		media lcd close

		this cmd no param.
	 - Test Case
		``media lcd close``

	9. media capture
	 - Function Description
		Photograph test, To save a frame of camera image to the SD card, the camera must be opened first to work properly
	 - CMD Format
		media capture param1

		+-----------+------------------------------------------------------------------------+
		| param1    | The capture name, such as "test.jpg"                                   |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Photograph test: ``media capture 1.jpg``

	10. media save_start
	 - Function Description
		Store the data in the camera to a file in the SD card continuously, and each picture is separated by a space.
	 - CMD Format
		media save_start param1

		+-----------+------------------------------------------------------------------------+
		| param1    | The storage file name, such as "test.264"                              |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Photograph test:``media start_save frame.264``

	11. media save_stop
	 - Function Description
		Stop saving camera data to SD card
	 - CMD Format
		media save_stop

		this cmd no param.
	 - Test Case
		``media save_stop``