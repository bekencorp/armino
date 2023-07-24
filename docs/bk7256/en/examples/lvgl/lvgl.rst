LVGL
================

:link_to_translation:`zh_CN:[中文]`

Overview
--------------------------
    The SDK integrates the graphical user interface of LVGL. This is the free open-source embedded graphics library to create beautiful UIs for any MCU, MPU and display type and has become the current popular GUI library. LVGL has rich features and provides many available widgets. Based on different widgets, corresponding examples are provided. For details, please refer to the code provided by the official website. LVGL official website address: https://lvgl.io. In addition, LVGL also provides five kinds of interactive demos. There are benchmark, keypad_encoder, music, stress and widgets. Among them, benchmark is a demo for testing performance, and widgets is a demo containing multiple widgets.

    This section mainly introduces the five interactive demos adapted to this SDK, so that developers can understand how to use the LVGL interface for application development based on the SDK.


Demo introduction
--------------------------
 - Demo code path: ``components\demos\peripheral\touch\touch_demo.c``
 - Demo compiler instruction: ``make bk7256 PROJECT=lvgl/xxx``. For example: ``make bk7256 PROJECT=lvgl/benchmark``.
 - Each demo has different configurations for different scenarios. Please refer to the table below and select the corresponding project for development:
         .. csv-table::
            :header: "ID", "demo name", "description", "scence"
            :widths: 5, 15, 20, 60

            "1","benchmark","fps test of lvgl","Projects with high display frame rate requirements without camera"
            "2","camera","switch between lvgl and camera display","By default, PSRAM is used as the display buff. If you want to use SRAM, you can modify the project config: CONFIG_LVGL_USE_PSRAM=n"
            "3","keypad_encoder","demo of lvgl","Projects with high display frame rate requirements without camera"
            "4","lcd_qspi_meter","demo for qspi lcd","Projects with high display frame rate requirements without camera"
            "5","meter","meter","Projects with high display frame rate requirements without camera"
            "6","music","music of lvgl","Projects with high display frame rate requirements without camera"
            "7","soundbar","music of lvgl, and play sound when connect BT of phone","Projects with high display frame rate requirements without camera"
            "8","stress","demo of lvgl","Projects with high display frame rate requirements without camera"
            "9","widgets","demo of lvgl","Projects with high display frame rate requirements without camera"

Demo execution
--------------------------------
	The five interactive demos of LVGL are in different project directories, you only need to compile different projects and download them to flash through the image download tool to run.
	
Demo development steps
--------------------------
 - Set the value of the macro ``CONFIG_LV_COLOR_DEPTH`` according to the data type of the LCD screen, and open the corresponding macro configuration of the functions provided by LVGL according to the requirements.
 - Initialize the display of the lcd screen.
 - Initialize the memory buffer for the LVGL drawing.
 - Call the interface of ``lv_vendor_init(lv_vnd_config_t *config, uint16_t width, uint16_t height)`` to initialize the configuration of lvgl.
 - If the touch function exists, call the interface of ``drv_tp_open(int hor_size, int ver_size)`` to initialize the function of TP.
 - If the fatfs file system function exists, call the interface of ``lv_fatfs_init(void)`` to initialize the fatfs file system of lvgl.
 - Call the interface of ``lv_vendor_start()`` to create the lvgl task to start scheduing.
 - Call the widget interface provided by lvgl to draw the corresponding UI.


LVGL development description
-------------------------------
 - ``lv_vendor_disp_lock()`` and ``lv_vendor_disp_unlock()`` are required for code protection when calling the widget interfaces provided by LVGL to draw the corresponding UI.
 - LVGL source code itself provides a number of additional third-part libraries, including file system interface, JPG decoder, BMP decoder, PNG decoder and GIF decoder etc. Due to the limitations of the system SRAM memory, these decoders can only decode small resolution pictures for display. For large resolution pictures, the SDK currently provides option for PNG, JPG and GIF decoders to decode using PSRAM memory.
 - For the LVGL PNG, JPG and GIF decoders to select PSRAM memory for decoding, first you need to open the corresponding macros in the ``lv_conf.h`` file, which respectively are ``LV_USE_PNG`` , ``LV_USE_SJPG`` and ``LV_USE_GIF`` . Then you need to open its internal corresponding macro ``LV_XXX_USE_PSRAM`` . It represents using psram memory when the macro value is 1 and represents using default sram memory when the value is 0. Finally, you need to set the heap values of ``CONFIG_PSRAM_HEAP_BASE`` and ``CONFIG_PSRAM_HEAP_SIZE`` in the project config file, which need to be set in decimal. The current heap address space ranges from 0x60700000 to 0x60800000, and the size is 1M. Since large-resolution image decoding requires more memory space, these two macros need to be set for heap space expansion.
 - Regarding the LVGL file system: sdk currently supports both fatfs and littlefs, which is based on bk_vfs's posix interface (refer to cli_vfs.c), when using it, open CONFIG_VFS, choose littlefs or fatfs (CONFIG_FATFS/CONFIG_LITTLEFS) as needed. The lvgl/camera project already supports this (1. open LV_USE_SJPG and LV_SJPG_USE_PSRAM in lv_conf.h; 2. Place a screen size image and rename it to bg.jpg, which is stored in the sd root directory (if using littlefs, modify the CONFIG_VFS_DEMO_USE_LITTLEFS=y in the project, and the program will write a blue background jpg to littlefs by default).
 - There is no need to open the ``CONFIG_LVGL_USE_PSRAM`` macro when selecting PSRAM memory for decoding using PNG, JPG and GIF decoders.
 - Regarding the usage of LVGL rotation function, LVGL itself has its own software rotation function, which can be executed through the function ``lv_disp_set_rotation()``. The function pass the parameters of ``LV_DISP_ROT_90`` , ``LV_DISP_ROT_180`` or ``LV_DISP_ROT_270`` to achieve image rotation at 90, 180 and 270 degrees. However, due to the limitations of this feature, it is only applicable to screens with the same width and height. For the screen with different width and height, the image display will be abnormal. To solve this issue, and in the case where the display hardware cannot change the display direction, the SDK provides an additional rotation function, which can achieve image rotation display at 90 degree and 270 degree. The specific implementation is as follows: When initializing LVGL by call the function ``lv_vendor_init(lv_vnd_config_t *config, uint16_t width, uint16_t height)`` in main function, the ``rotation`` parameter in the structure ``lv_vnd_config_t`` can be assigned by passing ``ROTATE_NONE``, ``ROTATE_90`` and ``ROTATE_270`` to indicate no rotation, rotation 90 and rotation 270 respectively. If the parameter are ``ROTATE_90`` or  ``ROTATE_270``, the width and height of the display screen should be exchanged. The width of the screen should be passed to the ``height`` paremeter, and the height of the screen should be passed to the ``width`` parameter.

LVGL Development Suggestions
-------------------------------
 - It is recommended to use the official IDE tool squareline to draw UI, export code and add to the project directory of sdk;
 - Squareline currently only supports png format for images and does not support jpg. However, we have found that if a 480X480 background image is changed from png to RGB565 format, the frame rate will differ by about 8 frames. Therefore, for images without transparency requirements, we strongly recommend decoding them to RGB565 on the official website https://lvgl.io/tools/imageconverter or directly use the jpg software solution;
