LCD rgb display yuv
==========================

:link_to_translation:`zh_CN:[Chinese]`

1 Overview
---------------------------------------
	demo of rgb lcd display camera raw yuv data.

2 Code Path
---------------------------------------
	The path of demos: ``\components\demos\media\lcd\lcd_rgb\lcd_rgb_demo.c``, 


3 Client command
---------------------------------------
The commands support by rgb lcd


+-----------------------------------------+----------------------------------+
|lcd base cmd                             |Description                       |
+=========================================+==================================+
|lcd_8080=dma2d_fill,0x60000000,0xf800    |fill addr 0x60000000 red          |
+-----------------------------------------+----------------------------------+
|lcd_rgb=clk                              |lcd display                       |
+-----------------------------------------+----------------------------------+


+-----------------------------------------+----------------------------------+
|lcd display camera yuv data              |Description                       |
+=========================================+==================================+
|lcd_rgb_yuv=480p                         |camera 480p/720p, fps             |
+-----------------------------------------+----------------------------------+
|lcd_close=yuv                            |close lcd                         |
+-----------------------------------------+----------------------------------+


The macro configuration that the demo runs depends on:

+--------------------------------------+------------------------+--------------------------------------------+---------+
|                 NAME                 |      Description       |                  File                      |  value  |
+======================================+========================+============================================+=========+
|CONFIG_LCD                            |lcd function enable     |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
+--------------------------------------+------------------------+--------------------------------------------+---------+
|CONFIG_LCD_TEST                       |DEMO enable             |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
+--------------------------------------+------------------------+--------------------------------------------+---------+


The steps performed by the lcd_rgb_jpeg demo are as follows:

	1.config rgb lcd
	 - rgb clk
	 - lcd init if lcd need
	 - display module x y pixel
	 - if image pixel is biger then lcd size, config lcd x y pixel and partical display params

	2.config jpeg and camera

	3.jpeg encode frame isr

	4.change lcd base addr if you want
