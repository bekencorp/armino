SBC_DEC Cli
================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------------
	In order to facilitate the verification of SBC or MSBC hardware decoding functions, a cli CMD is designed for this purpose. The design idea is to read sbc or msbc files from SDCARD to decode and play. Whether the current cli command takes effect is determined by the macro ``CLI_CFG_SBC``. The control location is in the ``components\bk_cli\cli_config.h`` file. Whether this macro takes effect is determined by the internal macro ``CONFIG_SBC_TEST``.


2、Code path
--------------------------
	path: ``components\demos\media\audio\sbc_decode\sbc_decode_demo.c``

3、Cli command introduction
------------------------------
The supported CMDs are as follows:

	1.sbc_decoder_test
	 - Function Description
		Used to test the function of SBC or MSBC hardware decoding and real-time playback.
	 - CMD Format
		sbc_decoder_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | start / stop                                                           |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | sbc file                                                               |
		|           |  Format: xxx.sbc                                                       |
		+-----------+------------------------------------------------------------------------+
	 - Test Case
		| Start decoding and storing PCM files: ``sbc_decoder_test start test.sbc``
		| Close the test file and release resources: ``sbc_decoder_test stop test.sbc``
