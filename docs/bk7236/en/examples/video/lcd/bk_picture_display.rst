LCD rgb display jpeg
==========================

:link_to_translation:`zh_CN:[Chinese]`

1 Overview
---------------------------------------
	demo of rgb lcd display a jpeg.

2 Code Path
---------------------------------------
	The path of demos: ``\components\demos\media\lcd\picture_display\jpeg_dec_display.c``, 


3 Client command
---------------------------------------
The commands support by rgb lcd

+-----------------------------------------+----------------------------------+
|lcd display a jpeg cmd                   |Description                       |
+=========================================+==================================+
|fatfstest M 1                            |camera 480p/720p, fps             |
+-----------------------------------------+----------------------------------+
|lcd=file_display,filename                |display ad card jpeg file in lcd  |
+-----------------------------------------+----------------------------------+

The macro configuration that the demo runs depends on:

+--------------------------------------+------------------------+--------------------------------------------+---------+
|                 NAME                 |      Description       |                  File                      |  value  |
+======================================+========================+============================================+=========+
|CONFIG_LCD                            |lcd function enable     |``middleware\soc\bk7236\bk7236.defconfig``  |    y    |
+--------------------------------------+------------------------+--------------------------------------------+---------+
|CONFIG_LCD_TEST                       |DEMO enable             |``middleware\soc\bk7236\bk7236.defconfig``  |    y    |
+--------------------------------------+------------------------+--------------------------------------------+---------+

