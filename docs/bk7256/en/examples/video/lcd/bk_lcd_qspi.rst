LCD QSPI Display
==========================

:link_to_translation:`zh_CN:[中文]`

1 Overview
---------------------------------------
	The lcd qspi demo shows the API usage of the LCD QSPI, mainly including the demo of displaying pure color and the demo of reading an RGB888 image from SDCard and displaying it.

2 Code Path
---------------------------------------
	The path of demos: ``\components\demos\media\lcd\lcd_8080\lcd_8080_demo.c``

	The macro configuration that the demo runs depends on:

	+---------------------------+---------------------------------+----------------------------------------------------+-----+
	|Name                       |Description                      |   File                                             |value|
	+===========================+=================================+====================================================+=====+
	|CONFIG_QSPI                |configure QSPI driver enable     |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
	+---------------------------+---------------------------------+----------------------------------------------------+-----+
	|CONFIG_LCD_QSPI            |configure LCD QSPI driver enable |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
	+---------------------------+---------------------------------+----------------------------------------------------+-----+
	|CONFIG_LCD_QSPI_SH8601A    |                                 |                                                    |     |
	|                           |configure LCD QSPI screen enable |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
	|CONFIG_LCD_QSPI_ST77903    |                                 |                                                    |     |
	+---------------------------+---------------------------------+----------------------------------------------------+-----+
	|CONFIG_LCD_QSPI_TEST       |configure LCD QSPI demo enable   |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
	+---------------------------+---------------------------------+----------------------------------------------------+-----+

	Other libraries and drivers that demo runs depend on:
	 - DMA2D DMA2D module driver
	 - QSPI QSPI module driver


3 Client command introduction
---------------------------------------
The supported commands are as follows:

	1.lcd_qspi_display_pure_color_test
	 - Function Description
		Used to test the function of LCD QSPI displaying pure color.
	 - Command Format
		lcd_qspi_display_pure_color_test start param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | screen model                                                           |
		|           | choose：sh8601a/st77903                                                |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		Test sh8601a screen display pure color: ``lcd_qspi_display_pure_color_test start sh8601a``

	2.lcd_qspi_display_picture_test
	 - Function Description
		Used to test the function of LCD QSPI displaying picture.
	 - Command Format
		lcd_qspi_display_picture_test start param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | file name                                                              |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | screen model                                                           |
		|           | choose: sh8601a/st77903                                                |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		1.Mount SDCard: ``fatfstest M 1``.

		2.Test sh8601a screen display picture: ``lcd_qspi_display_picture_test start beken.rgb sh8601a``.
