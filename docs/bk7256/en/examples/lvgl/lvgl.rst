LVGL
================

:link_to_translation:`zh_CN:[中文]`

Overview
--------------------------
    The SDK integrates the graphical user interface of LVGL. This is the free open-source embedded graphics library to create beautiful UIs for any MCU, MPU and display type and has become the current popular GUI library. LVGL has rich features and provides many available widgets. Based on different widgets, corresponding examples are provided. For details, please refer to the code provided by the official website. LVGL official website address: https://lvgl.io. In addition, LVGL also provides five kinds of interactive demos. There are benchmark, keypad_encoder, music, stress and widgets. Among them, benchmark is a demo for testing performance, and widgets is a demo containing multiple widgets.

    This section mainly introduces the five interactive demos adapted to this SDK, so that developers can understand how to use the LVGL interface for application development based on the SDK.


Demo introduction
--------------------------
 - The demo is mainly based on the ST7701s LCD screen. A complete set of UI is drawn through the LVGL interface and display on the screen and touch the UI menu to experience. In addition, a meter demo of the LCD QSPI interface screen is also provided.
 - Demo code path: ``components\demos\peripheral\touch\touch_demo.c``
 - Demo compiler instruction: ``make bk7256 PROJECT=lvgl/xxx``. For example: ``make bk7256 PROJECT=lvgl/benchmark``.

Demo execution
--------------------------------
	The five interactive demos of LVGL are in different project directories, you only need to compile different projects and download them to flash through the image download tool to run.
	
Demo development steps
--------------------------
 - Set the value of the macro CONFIG_LV_COLOR_DEPTH according to the data type of the LCD screen, and open the corresponding macro configuration of the functions provided by LVGL according to the requirements.
 - Initialize the display of the lcd screen.
 - Call the interface of ``bk_gui_disp_task_init(int hor_size, int ver_size, int is_use_camera_blend)`` to initialize the display task of lvgl.
 - Call the interface of ``drv_tp_open(int hor_size, int ver_size)`` to initialize the function of TP.
 - Call the widget interface provided by lvgl to draw the corresponding UI.