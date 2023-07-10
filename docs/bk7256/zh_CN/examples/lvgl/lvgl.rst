LVGL
================

:link_to_translation:`en:[English]`

概述
--------------------------
	SDK集成了LVGL（Light and Versatile Graphics Library，轻量级多功能图形库）的图形用户界面，这是一种免费、开源且低消耗的嵌入式图形库，可以为任何MCU、MPU及显示类型创建美观的界面，已成为当前热门的图形用户界面库。LVGL具有丰富的特性，并且提供了许多可用的小组件，基于不同的小组件提供了与之相对应的example，具体可参考官网提供的代码，LVGL官方网站地址：https://lvgl.io 。此外，LVGL还提供了5种互动演示，分别是：benchmark、keypad_encoder、music、stress以及widgets，其中benchmark是测试性能的demo，widgets是包含多个小组件的demo。

	本小节主要介绍本SDK上所适配的这5种互动演示demo，以便开发人员了解基于本SDK如何使用LVGL的接口进行应用开发。


demo简介
--------------------------
 - demo主要是基于ST7701s的LCD屏幕，通过LVGL的接口绘制出一套完整的UI，从而在屏幕上进行显示并触摸体验，此外还提供了一个LCD QSPI接口屏幕的meter演示demo。
 - demo代码路径： ``\projects\lvgl\...``
 - demo编译指令： ``make bk7256 PROJECT=lvgl/xxx``，例如： ``make bk7256 PROJECT=lvgl/benchmark``。 

demo执行
--------------------------
	LVGL的五种互动演示demo分别在不同的project目录下，只需要编译不同的project并通过image下载工具下载到flash中即可执行。

demo开发步骤
--------------------------
 - 根据LCD屏幕的数据类型设置宏 ``CONFIG_LV_COLOR_DEPTH`` 的值，并根据需求打开LVGL所提供功能的相应宏配置；
 - 初始化LCD屏幕的显示；
 - 初始化LVGL绘图所需的内存buffer；
 - 调用 ``lv_vendor_init(lv_vnd_config_t *config, uint16_t width, uint16_t height)`` 的接口初始化lvgl的相关配置；
 - 若存在触摸功能，调用 ``drv_tp_open(int hor_size, int ver_size)`` 的接口初始化TP的功能；
 - 若存在FATFS文件系统功能，调用 ``lv_fatfs_init(void)`` 的接口初始化fatfs文件系统；
 - 调用 ``lv_vendor_start()`` 的接口创建LVGL的task开始调度执行；
 - 调用lvgl提供的组件接口绘制相应的UI。

LVGL开发说明
--------------------------
 - 调用LVGL提供的组件接口绘制相应的UI时需要使用 ``lv_vendor_disp_lock()`` 和 ``lv_vendor_disp_unlock()`` 的接口进行代码保护；
 - LVGL源码本身额外提供了许多第三方的库，包含文件系统接口、JPG解码器、BMP解码器、PNG解码器以及GIF解码器等等，由于系统SRAM内存的限制，这些解码器只能解码小分辨率的图片进行显示，对于大分辨率的图像，本SDK当前提供了PNG、JPG以及GIF解码器使用PSRAM内存进行解码的选择。
 - 关于PNG、JPG和GIF解码器选择PSRAM内存进行解码的使用方法，首先需要打开lv_conf.h文件中对应的宏，分别为 ``LV_USE_PNG`` ``LV_USE_SJPG`` 和 ``LV_USE_GIF`` ，然后打开其内部对应的宏 ``LV_XXX_USE_PSRAM`` ，该宏的值为1时使用PSRAM的内存，值为0时使用默认SRAM的内存，最后需要在project的config文件中设置 ``CONFIG_PSRAM_HEAP_BASE`` 和 ``CONFIG_PSRAM_HEAP_SIZE`` 的heap值，这两个值的设置需要使用十进制，当前heap地址空间范围为0x60700000~0x60800000，大小为1M，由于大分辨率的图像解码需要更多的内存空间，所以需要设置这两个宏进行heap空间扩大。
 - 关于LVGL FATFS文件系统的使用方法，首先需要打开lv_conf.h文件中 ``LV_USE_FS_FATFS`` 的宏，并通过设置宏 ``LV_FS_FATFS_DISK_NUM`` 来选择对应的disk number，当前默认为3，表示flash的disk number，最后需要在main函数中调用 ``lv_fatfs_init()`` 的接口进行初始化即可使用fatfs文件系统。
 - 在使用PNG、JPG和GIF解码器选择PSRAM内存进行解码时，不需要打开 ``CONFIG_LVGL_USE_PSRAM`` 的宏。
