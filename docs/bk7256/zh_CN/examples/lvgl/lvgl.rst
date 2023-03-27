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
 - 根据LCD屏幕的数据类型设置宏CONFIG_LV_COLOR_DEPTH的值，并根据需求打开LVGL所提供功能的相应宏配置；
 - 初始化LCD屏幕的显示；
 - 调用 ``bk_gui_disp_task_init(int hor_size, int ver_size, int is_use_camera_blend)`` 的接口初始化lvgl的显示task；
 - 若存在触摸功能，调用 ``drv_tp_open(int hor_size, int ver_size)`` 的接口初始化TP的功能；
 - 调用lvgl提供的组件接口绘制相应的UI。