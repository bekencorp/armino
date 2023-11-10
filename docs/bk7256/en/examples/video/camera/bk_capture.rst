DVP Capture
========================

:link_to_translation:`zh_CN:[中文]`

1 Overview
-------------------------------------
	The main function of taking pictures is to store a frame of raw data collected by the dvp sensor on the SDCard after being compressed and encoded by the jpeg encode module.

2 Code Path
-------------------------------------
	demo path:``./components/demos/media/video/capture/capture.c``

3 Client command
---------------------------------------
	The commands support by demo

	+----------------------------------------+--------------------------+----------------------+
	|             Command                    |      Param               |   Description        |
	+========================================+==========================+======================+
	|                                        | param1:camera_type       |the type of using dvp |
	|                                        +--------------------------+----------------------+
	| dvp init param1 param2 param3          | param2:image resolution  |the resolution output |
	|                                        +--------------------------+----------------------+
	|                                        | param3:frame rate        |the frame rate output |
	+----------------------------------------+--------------------------+----------------------+
	| dvp capture param1                     | param1:file_id           |the head neme of imag |
	+----------------------------------------+--------------------------+----------------------+
	| dvp deinit                             | 无                       |deinit dvp function   |
	+----------------------------------------+--------------------------+----------------------+

	The macro configuration that the demo runs depends on:

	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description       |                  File                      |  value  |
	+======================================+========================+============================================+=========+
	|CONFIG_JPEGENC_HW                     |support hw jpeg encode  |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA                         |suport use dvp camera   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_USE_APP_DEMO_VIDEO_TRANSSER    |support video transfer  |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_NET_WORK_VIDEO_TRANSFER        |support wifi transfer   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_COMPONENTS_P2P                 |support p2p transfer    |``middleware\soc\bk7256\bk7256.defconfig``  |    n    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA_USE_I2C1                |suport use I2C1         |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+

4 Demo introduction
-------------------------------------
	The steps performed by the demo are as follows:

	1. Prepare the dvp camera, the connection method is as shown below:

	2. Send the following commands in sequence

		fatfstest M 1

		dvp capture：dvp init 或 dvp init 0 640X480 20

		dvp capture 1(file_id)

		dvp deinit

.. figure:: ../../../../../common/_static/video_transfer_evb.png
    :align: center
    :alt: evb connect when capture
    :figclass: align-center

    Figure 1. sensor connection

5 Detailed configuration and its description
-----------------------------------------------
	1.set dvp parameters: dvp init param1 param2 param3

	param1:0-5, set sensor type and output date type，reference: ``media_camera_type_t``.

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

	param2:resolution，sensor output ppi，reference: ``media_ppi_t``.

		GC0328C: 640X480, 480X272, 480X320

		HM_1055: 1280X720

		GC2145: 1280X720, 800X600, 640X480

	param3:frame_rate，sensor output frame_rate，reference: ``sensor_fps_t``

		GC0328C：5、10、20、25

		HM_1055：5、10、15、20

		GC2145: 10、15、20、25

	2.capture command: dvp save_image file_id

		file_id: range(0-255)

.. note::

	Note: The above camera command supports continuous photography, but the file_id needs to be changed every time, and the id of each image cannot be the same.