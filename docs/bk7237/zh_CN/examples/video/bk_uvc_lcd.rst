UVC屏显
========================

:link_to_translation:`en:[English]`

概述
-------------------------

UVC屏显将USB camera采集的图像信息，经过其内部的ISP模块编码后，再由MCU解码，最后实时的显示到RGB屏幕上。
UVC内部ISP编码输出的图像数据为JPEG格式，MCU当前支持软件解码，后续会支持硬件解码，解码后的数据为YUV422（YYUV）。

实现方式
-------------------------

在系统上电后，接入UVC设备，从log中可以看出设备能被正常识别，通过串口发送命令来触发使能UVC的功能，命令流程如下：

1.lcd_jpegdec init

	初始化用于屏显的各个模块，以及相关电源的power on。

2.lcd_jpegdec uvc_dispaly_init 480 10

	初始化UVC模块，480：表示分辨率， 10：表示帧率

.. note::

	注意：UVC的分辨率和帧率由UVC本身的协议所决定，此处分辨率和帧率默认设置为480和10，是常规设备都支持的。不支持动态修改
	分辨率和帧率，需要修改时，需要将UVC屏显功能关闭（即触发命令5后），重新发送命令，使能该功能。支持的分辨率和帧率参考
	《UVC_Camera及配置》。

3.lcd_jpegdec start_uvc

	开启UVC，UVC输出视频流，开始显示

4.lcd_jpegdec close_uvc

	关闭UVC，UVC停止输出视频流，停止显示

5.lcd_jpegdec deinit

	关闭UVC屏显功能

UVC屏显得软件调用流程
---------------------------

.. figure:: ../../../_static/uvc_display_function_call.png
    :align: center
    :alt: uvc_display软件流程
    :figclass: align-center

    Figure 1. uvc_display function call

图传的模块间调用
""""""""""""""""""""""""""

.. figure:: ../../../_static/uvc_display_message.png
    :align: center
    :alt: uvc_display模块调用
    :figclass: align-center

    Figure 2. uvc_display modules communicate
	