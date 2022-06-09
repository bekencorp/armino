图传
========================

:link_to_translation:`en:[English]`

概述
""""""""""""""""""""""""""
图传的作用主要是将dvp sensor采集的原始数据，经jpeg encode模块压缩编码后，再由DMA模块搬运出来，并以UDP的格式组包，之后通过UDP发送给手机，手机的图传应用（app）接收到数据后，会对数据包进行解析，然后实时显示编码之后的视频数据流。

参考代码
""""""""""""""""""""""""
./components/demos/media/video/video_transfer

连接
""""""""""""""""""""""""""
数据是以UDP包的格式发出，所以在此之前手机与开发板必须已经实现WIFI的连接。
当前WIFI连接的形式有多种多样：

1.AP
	将开发板作为一个AP，手机去连接这个AP，做法与手机连接其他wifi设备类似。

2.station
	将开发板作为一个station，去连接手机，手机需要开启wifi热点，此时手机相当于一个路由器。

3.p2p
	采用手机直连的方式，与AP类似，做法是在手机上：WLAN-->高级设置-->WLAN直连，搜索到板子的设备后，点击连接。

图传的实现
""""""""""""""""""""""""""
根据上述的连接方式，开启图传的方式也有三种，命令如下：

1.video_transfer -a ssid key

2.video_transfer -s ssid key

3.video_transfer -p ssid key

	其中：
	-a：表示AP的连接方式

	-s：表示station的连接方式

	-p：表示p2p的直连方式
	
	-ssid：表示的设备的名字

	-key：表示连接时需要的密码，可以默认不输入，连接时不需要密码

例如以-a的方式连接时，ssid表示的是AP的名字，手机wifi搜索时，可以搜索到这个名字，搜索到后连接需要输入设置的key。

.. note::

	注意：第一种方式可能存在板子的带宽不够，显示的视频不够清晰；第二种方式，图传的效果最好，因为手机的带宽非常充足；第三种方式当前还未实现。

帧率和分辨率设置
-------------------

本demo在图传过程中不支持动态修改DVP类型、帧率和分辨率，仅支持修改完成后，重新编译生成bin文件，再烧录到MCU中。
如《DVP_Camera及配置》中所描述的，DVP类型修改的方式，由宏``USE_CAMERA``控制，而帧率和分辨率仅支持DVP协议上所支持的。
参考《DVP_Camera及配置》中所描述。
当前默认的帧率和分辨率：

- GC0328C:20fps, 640*480

理论上支持25fps，但是由于带宽的限制，一般设置为<=25fps

- HM_1055:15fps, 1280*720

理论上支持20fps，但是由于带宽的限制，一般设置为<=15fps

修改方式：

在路径：``./components/demos/media/video/video_transfer/video_transfer.c``中函数``tvideo_config_desc()``。
当使用GC0328C时打开上面两行，且注释下面两行。分辨率和帧率为函数的参数，支持修改到设备协议支持的值，
参考《DVP_Camera及配置》中所描述。当使用HM_1055时，注释上面两行，放开下面两行。

.. figure:: ../../../_static/set_ppi_fps.png
    :align: center
    :alt: 设置图传的分辨率和帧率
    :figclass: align-center

    Figure 1. video transfer set_ppi_fps

图传还支持SPI Camera的传输方式，但是当前MCU上没有外接SPI Camera的端口，是由宏``CONFIG_SPIDMA``和图传的传输枚举类型``video_open_type_t``控制。

图传的软件调用流程
--------------------

.. figure:: ../../../_static/video_transfer_function_call.png
    :align: center
    :alt: video_transfer软件流程
    :figclass: align-center

    Figure 2. video transfer function call

图传的模块间调用
--------------------

.. figure:: ../../../_static/video_transfer_message.png
    :align: center
    :alt: video_transfer模块调用
    :figclass: align-center

    Figure 3. video transfer modules communicate
