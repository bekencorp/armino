EQ Cli
================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------------
	In order to verify the function of EQ, a cli CMD is designed for this purpose. The design idea is to set a series of parameters of dance to test the equalization effect. Whether the current cli command takes effect is determined by the macro ``CLI_CFG_AUD``. The macro control position is in: ``components\bk_cli\cli_config.h`` file, whether this macro takes effect is determined by the internal macros ``CONFIG_AUDIO`` and ``CONFIG_AUDIO_TEST``.


2、Code path
--------------------------
	path: ``components\demos\media\audio\aud\aud_cp1.c``

3、Cli command introduction
-------------------------------
The supported CMDs are as follows:

	1.aud_eq_test
	 - Function Description
		Used to test the voice dance equalization function.
	 - CMD Format
		aud_eq_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		| Start testing: ``aud_eq_test start``
		| Stop the test and release resources: ``aud_eq_test stop``
