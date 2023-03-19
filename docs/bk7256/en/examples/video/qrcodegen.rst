QR-Code Generator
==========================

:link_to_translation:`zh_CN:[中文]`

1 Overview
---------------------------------------
	The qrcodegen demo mainly introduces the API usage of QR code generation. This demo mainly generates QR codes and saves the generated QR codes to SDCard, and finally opens the saved files through image viewing software..

2 Code Path
---------------------------------------
	The path of demos: ``\components\demos\media\lcd\qrcodegen\qrcodegen_demo.c`` 

	The macro configuration that the demo runs depends on:

	+---------------------------+----------------------------+----------------------------------------------------+-----+
	|Name                       |Description                 |   File                                             |value|
	+===========================+============================+====================================================+=====+
	|CONFIG_QRCODEGEN_TEST      |配置二维码生成demo使能      |``\middleware\soc\bk7256\bk7256.defconfig``         |  y  |
	+---------------------------+----------------------------+----------------------------------------------------+-----+

3 Client command introduction
---------------------------------------
The supported commands are as follows:

	1.qrcodegen_text_test
	 - Function Description
		Used to test the function of QR code generation.
	 - Command Format
		qrcodegen_text_test param1 param2

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | saved file name                                                        |
		|           |                                                                        |
		+-----------+------------------------------------------------------------------------+
		|param2     | scale                                                                  |
		|           | choose：1/2/3/4......                                                  |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		1.Mount SDCard: ``fatfstest M 1``.

		2.Test QR code generation: ``qrcodegen_text_test qr1.rgb 4``.


4 Demo description
---------------------------------------
 - When viewing the saved picture, the data type is set to RGB565.
 - Resolution is set to 116 * 116.
 - The default configuration in the demo is to automatically generate the side length of the QR code according to the length of the text content, which can be obtained according to the output log: ``size =``, and the text content can be changed freely for testing. The default output of the current size is 21.
 - The border length is set to 4 by default in the demo, so the actual QR code length is ``size + 4 * 2``, and then the final QR code side length can be obtained according to the input scale value.