MP3 Cli
================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------------
	In order to facilitate the verification of the function of MP3 decoding, a series of cli CMDs are designed, mainly including reading MP3 files from SDCARD for decoding and decoding and playing. Whether the current cli command takes effect is determined by the macro ``CLI_CFG_MP3``. The macro control location is in the ``components\bk_cli\cli_config.h`` file, whether the macro takes effect is determined by the internal macro ``CONFIG_AUDIO_MP3_TEST``。


2、Code path
--------------------------
	path: ``components\demos\media\audio\mp3\mp3_demo.c``

3、Cli command introduction
--------------------------------
The supported CMDs are as follows:

	1.mp3_decode_test
	 - Function Description
		Used to test the MP3 decoding function, and store the decoded PCM files in SDcard.
	 - CMD Format
		mp3_decode_test param1 param2 param3

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | mp3 file                                                               |
		|           |  fromat: xxx.mp3                                                       |
		+-----------+------------------------------------------------------------------------+
		|param3     | pcm file                                                               |
		|           |  fromat: xxx.pcm                                                       |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		| Start decoding and storing PCM files: ``mp3_decode_test start qingtian.mp3 qingtian.pcm``
		| Close the test file and release resources: ``mp3_decode_test stop qingtian.mp3 qingtian.pcm``

	2.mp3_play_test
	 - Function Description
		Used to test the function of MP3 decoding and real-time playback (Use audio driver basic API).
	 - CMD Format
		mp3_play_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | mp3 file                                                               |
		|           |  format: xxx.mp3                                                       |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		| Start decoding and play in real time：``mp3_play_test start qingtian.mp3``
		| Close the test file and release resources: ``mp3_play_test stop qingtian.mp3``

	3.aud_intf_mp3_play_test
	 - Function Description
		Used to test the function of MP3 decoding and real-time playback (Use aud_intf basic API).
	 - CMD Format
		aud_intf_mp3_play_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | mp3 file                                                               |
		|           |  format: xxx.mp3                                                       |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		| Start decoding and play in real time: ``aud_intf_mp3_play_test start qingtian.mp3``
		| Close the test file and release resources: ``aud_intf_mp3_play_test stop qingtian.mp3``
