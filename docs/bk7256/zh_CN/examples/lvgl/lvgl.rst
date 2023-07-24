LVGL
================

:link_to_translation:`en:[English]`

概述
--------------------------
	SDK集成了LVGL（Light and Versatile Graphics Library，轻量级多功能图形库）的图形用户界面，这是一种免费、开源且低消耗的嵌入式图形库，可以为任何MCU、MPU及显示类型创建美观的界面，已成为当前热门的图形用户界面库。LVGL具有丰富的特性，并且提供了许多可用的小组件，基于不同的小组件提供了与之相对应的example，具体可参考官网提供的代码，LVGL官方网站地址：https://lvgl.io 。此外，LVGL还提供了5种互动演示，分别是：benchmark、keypad_encoder、music、stress以及widgets，其中benchmark是测试性能的demo，widgets是包含多个小组件的demo。

	本小节主要介绍本SDK上所适配的这5种互动演示demo，以便开发人员了解基于本SDK如何使用LVGL的接口进行应用开发。


demo简介
--------------------------
 - demo代码路径： ``\projects\lvgl\...``
 - demo编译指令： ``make bk7256 PROJECT=lvgl/xxx``，例如： ``make bk7256 PROJECT=lvgl/benchmark``。 
 - 每个demo里面配置针对的场景不一样，请查阅下表然后选择对应的工程进行开发：
        .. csv-table::
            :header: "序号", "demo名称", "demo说明", "适用场景"
            :widths: 5, 15, 20, 60

            "1","benchmark","官网帧率测试程序","没有摄像头，对显示帧率要求高的项目"
            "2","camera","lvgl显示同camera显示可平滑切换","网络或本地摄像头显示与lvgl界面显示切换的项目，默认使用psram做显示buff，如果想用sram，可修改工程config：CONFIG_LVGL_USE_PSRAM=n"
            "3","keypad_encoder","官网测试程序","不使用摄像头，对显示帧率要求高的项目"
            "4","lcd_qspi_meter","qspi接口屏显示demo","不使用摄像头，显示屏为QSPI接口，对显示帧率要求高的项目"
            "5","meter","仪表显示demo","不使用摄像头，对显示帧率要求高的项目"
            "6","music","官网音乐播放器","不使用摄像头，对显示帧率要求高的项目"
            "7","soundbar","基于官网音乐播放器，增加BT连接播放手机音乐","不使用摄像头，对显示帧率要求高的项目"
            "8","stress","官网显示测试程序","不使用摄像头，对显示帧率要求高的项目"
            "9","widgets","官网widgets","不使用摄像头，对显示帧率要求高的项目"

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
 - 关于LVGL PNG、JPG和GIF解码器选择PSRAM内存进行解码的使用方法，首先需要打开lv_conf.h文件中对应的宏，分别为 ``LV_USE_PNG`` ``LV_USE_SJPG`` 和 ``LV_USE_GIF`` ，然后打开其内部对应的宏 ``LV_XXX_USE_PSRAM`` ，该宏的值为1时使用PSRAM的内存，值为0时使用默认SRAM的内存，最后需要在project的config文件中设置 ``CONFIG_PSRAM_HEAP_BASE`` 和 ``CONFIG_PSRAM_HEAP_SIZE`` 的heap值，这两个值的设置需要使用十进制，当前heap地址空间范围为0x60700000~0x60800000，大小为1M，由于大分辨率的图像解码需要更多的内存空间，所以需要设置这两个宏进行heap空间扩大。
 - 关于LVGL文件系统：sdk目前即可支持fatfs，也可支持littlefs，是基于bk_vfs的posix接口实现（可参考cli_vfs.c)，在使用时，打开CONFIG_VFS，依据需要选择littlefs还是fatfs（CONFIG_FATFS/CONFIG_LITTLEFS），lvgl/camera工程已支持演示（1、lv_conf.h打开LV_USE_SJPG和LV_SJPG_USE_PSRAM； 2、放一张屏幕大小图片改名为bg.jpg，存放在sd根目录（如果用littlefs，修改工程下的CONFIG_VFS_DEMO_USE_LITTLEFS=y即可，程序默认会写一张蓝色背景的jpg到littlefs)）。
 - 在使用PNG、JPG和GIF解码器选择PSRAM内存进行解码时，不需要打开 ``CONFIG_LVGL_USE_PSRAM`` 的宏。
 - 关于LVGL旋转功能的使用方法，LVGL本身自带软件旋转功能，可通过函数 ``lv_disp_set_rotation()`` 传入参数 ``LV_DISP_ROT_90`` 、 ``LV_DISP_ROT_180`` 和 ``LV_DISP_ROT_270`` 实现90度、180度和270度的图像旋转，但由于该功能存在缺陷，仅适用于显示屏宽和高相同的屏幕，对于宽和高不相同的屏幕就会造成图像显示异常。针对该问题，且在显示屏硬件无法更改显示方向的情况下，SDK提供了一种额外的旋转功能，通过该功能可以实现图像90度旋转显示和270度旋转显示。具体实现是：在主函数中调用函数 ``lv_vendor_init(lv_vnd_config_t *config, uint16_t width, uint16_t height)`` 初始化LVGL时，对结构体 ``lv_vnd_config_t`` 中的 ``rotation`` 参数进行赋值，可传入 ``ROTATE_NONE`` 、 ``ROTATE_90`` 和 ``ROTATE_270`` 三种值，分别表示不旋转、旋转90度和旋转270度，若参数是 ``ROTATE_90`` 和 ``ROTATE_270`` 时，需要将显示屏的宽和高进行互换，屏幕的宽度值传入 ``height`` 参数，屏幕的高度值传入 ``width`` 参数。

LVGL开发建议
--------------------------
 - 建议先使用官网IDE工具squareline绘制好ui界面，然后导出代码加入到工程目录下，碰到需调试的界面问题，可先在windows模拟器里面调试：https://github.com/lvgl/lv_port_pc_visual_studio 或 https://github.com/lvgl/lv_port_pc_eclipse ；
 - squareline对于图片，目前只支持png格式，不支持jpg，但是我们发现，一张480X480的背景图片，如果由png改为RGB565格式，帧率会相差8帧左右，所以无透明度需求的图片，我们强烈建议在官网解码为RGB565 https://lvgl.io/tools/imageconverter 或者直接使用jpg软解；