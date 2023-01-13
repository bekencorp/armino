TOUCH Cli
================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------------
	In order to verify the function of TOUCH, a series of cli CMDs are designed for this purpose, mainly including single-channel working mode and multi-channel scanning working mode. Whether the current cli command takes effect is determined by the macro ``CLI_CFG_TOUCH``, and the macro control position is in the: ``components\bk_cli\cli_config.h`` file, whether this macro takes effect is determined by the internal macro ``CONFIG_TOUCH_TEST``.


2、Code path
--------------------------
	path: ``components\demos\peripheral\touch\touch_demo.c``

3、Cli command introduction
--------------------------------
The supported CMDs are as follows:

	1.touch_single_channel_calib_mode_test
	 - Function Description
		Used to test the function of touch single channel.
	 - CMD Format
		touch_single_channel_calib_mode_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | touch channel id                                                       |
		|           |  option: 2 - 15                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | Sensitivity level selection                                            |
		|           |  option: 0 / 1 / 2 / 3                                                 |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		Test touch channel 2: ``touch_single_channel_calib_mode_test 2 1``

	2.touch_multi_channel_scan_mode_test
	 - Function Description
		Used to test the function of touch multi-channel scanning work.
	 - CMD Format
		touch_multi_channel_scan_mode_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | Sensitivity level selection                                            |
		|           |  option: 0 / 1 / 2 / 3                                                 |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		Test all touch channels：``touch_multi_channel_scan_mode_test start 1``
