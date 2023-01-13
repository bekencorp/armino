AUDIO Cli
================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------------
	In order to facilitate the verification of audio-related functions, a series of cli CMDs are designed for this purpose, which are also demos of various functional scenarios of audio for customers' reference.



2、Code path
--------------------------
	path: ``components\demos\media\audio\aud_intf\demo\aud_intf_demo.c``

3、Cli command introduction
------------------------------------------
The supported CMDs are as follows:

	1.aud_intf_record_test
	 - Function Description
		Used to test the recording to file function, including start recording, pause recording, end recording, etc.
	 - CMD Format
		aud_intf_record_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | init : Init record                                                  |
		|           |  | start : Start record                                                |
		|           |  | pause : Pause record                                                |
		|           |  | stop : Stop record                                                  |
		|           |  | deinit : Deinit record                                              |
		+-----------+------------------------------------------------------------------------+
		|param2     | file name                                                              |
		|           |  format: xxx.pcm                                                       |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		| Init record：``aud_intf_record_test init record.pcm``
		| Start record：``aud_intf_record_test start record.pcm``
		| Pause record：``aud_intf_record_test pause record.pcm``
		| Stop record：``aud_intf_record_test stop record.pcm``
		| Deinit record：``aud_intf_record_test deinit record.pcm``

	2.aud_intf_play_test
	 - Function Description
		Used to test the function of playing audio files, including starting playback, pausing playback, and ending playback, etc.
	 - CMD Format
		aud_intf_play_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | init : Init play                                                    |
		|           |  | start : Start play                                                  |
		|           |  | pause : Pause play                                                  |
		|           |  | stop : Stop play                                                    |
		|           |  | deinit : Deinit play                                                |
		+-----------+------------------------------------------------------------------------+
		|param2     | file name                                                              |
		|           |  format: xxx.pcm                                                       |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Init play：``aud_intf_play_test init record.pcm``
		| Start play：``aud_intf_play_test start record.pcm``
		| Pause play：``aud_intf_play_test pause record.pcm``
		| Stop play：``aud_intf_play_test stop record.pcm``
		| deinit play：``aud_intf_play_test deinit record.pcm``

	3.aud_intf_loop_test
	 - Function Description
		Through the software self-loopback (send the data collected by the mic to the speaker for playback) to test whether the functions of the mic and the speaker are normal.
	 - CMD Format
		aud_intf_loop_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start audio loop test                                       |
		|           |  | stop : Stop audio loop test                                         |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start self-loopback test：``aud_intf_loop_test start``
		| Stop self-loopback test：``aud_intf_loop_test stop``
