Capture
========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	拍照的作用主要是将dvp/uvc sensor采集的一帧原始数据，经jpeg encode模块压缩编码后，存储到SDCard上。

2 代码路径
-------------------------------------
	demo路径：``./components/demos/media/video/capture/capture.c``

3 cli命令简介
-------------------------------------
	demo支持的命令如下表：

	+----------------------------------------+--------------------------+----------------------+
	|             Command                    |      Param               |   Description        |
	+========================================+==========================+======================+
	|                                        | param1:camera_type       |使用的dvp设备类型     |
	|                                        +--------------------------+----------------------+
	| dvp init param1 param2 param3          | param2:image resolution  |输出的分辨率          |
	|                                        +--------------------------+----------------------+
	|                                        | param3:frame rate        |输出的帧率            |
	+----------------------------------------+--------------------------+----------------------+
	| dvp capture param1                     | param1:file_id           |保存图片的名字前缀    |
	+----------------------------------------+--------------------------+----------------------+
	| dvp deinit                             | 无                       |释放dvp的相关传输功能 |
	+----------------------------------------+--------------------------+----------------------+

	demo运行依赖的宏配置：

	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description       |                  File                      |  value  |
	+======================================+========================+============================================+=========+
	|CONFIG_JPEGENC_HW                     |配置是否支持硬件编码    |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_DVP_CAMERA                     |配置是否使用dvp camera  |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_USE_APP_DEMO_VIDEO_TRANSSER    |配置是否使用图传应用    |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_NET_WORK_VIDEO_TRANSFER        |配置是否支持网络图传    |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_COMPONENTS_P2P                 |配置是否使用p2p传输     |``middleware\soc\bk7256\bk7256.defconfig``  |    n    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_DVP_CAMERA_I2C_ID              |配置是否使用I2C通信     |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+

4 演示介绍
-------------------------------------
	demo执行的步骤如下：

	1、准备好dvp摄像头，连接方式如下图所示：

	2、顺序发送如下命令

		fatfstest M 1

		dvp拍照：dvp init 或 dvp init 0 640X480 20

		uvc拍照：dvp init 4 640X480 20

		dvp capture 1(file_id)

		dvp deinit

.. figure:: ../../../../../common/_static/video_transfer_evb.png
    :align: center
    :alt: 图传时板子的连接示意
    :figclass: align-center

    Figure 1. dvp拍照时的连接示意

5 详细配置及其说明
----------------------------------
	1、设置dvp参数：dvp init param1 param2 param3

	param1:0-5, 表示使用的摄像头类型和输出数据的类型，参考：``media_camera_type_t``.

::

	typedef enum {
		MEDIA_DVP_MJPEG,         /**< dvp camera and output jpeg data */
		MEDIA_DVP_H264,          /**< dvp camera and output h264 data */
		MEDIA_DVP_YUV,           /**< dvp camera and output yuv data */
		MEDIA_DVP_MIX,           /**< dvp camera and output jpeg/yuv by turns */
		MEDIA_UVC_MJPEG,         /**< uvc camera and output jpeg data */
		MEDIA_UVC_H264,          /**< uvc camera and output h264 data */
		MEDIA_CAMERA_UNKNOW,     /**< camera not support */
	} media_camera_type_t;

::

	param2:分辨率，表示摄像头期望输出的分辨率，参考：``media_ppi_t``.

		GC0328C: 640X480, 480X272, 480X320

		HM_1055: 1280X720

		GC2145: 1280X720, 800X600, 640X480

	param3:帧率，表示摄像头期望输出的帧率，参考：``sensor_fps_t``

		GC0328C：5、10、20、25

		HM_1055：5、10、15、20

		GC2145: 10、15、20、25

	2、拍照命令：dvp save_image file_id

		file_id：范围(0-255)

.. note::

	注意：上面的拍照命令支持连续拍照，但是file_id每次都需要变化，不能每张图像的id都一致。
