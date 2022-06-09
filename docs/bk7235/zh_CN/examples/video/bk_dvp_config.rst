DVP_Camera及配置
========================

:link_to_translation:`en:[English]`

支持的DVP类型
--------------------

	当前支持两种类型的DVP Camera类型：
	
	- GC0328C

	- HM_1055

	系统中生效的类型由宏``USE_CAMERA``控制，路径：./include/components/dvp_camera_types.h

支持的帧率
---------------------

	针对GC0328C，支持以下帧率：
	
	- 5fps

	- 10fps

	- 20fps

	- 30fps
	
	针对HM_1055，支持一下帧率：
	
	- 5fps

	- 10fps

	- 15fps

	- 20fps

	- 25fps

	- 30fps

	系统中的帧率由枚举类型``fps_type_t``设置，路径：./include/components/video_types.h

支持的分辨率
------------------------

	针对GC0328C，支持以下分辨率：
	
	- WQVGA：480*272

	- QVGA：320*240

	- VGA：640*480

	针对HM_1055，支持一下分辨率：
	
	- 1280*720

	系统中的分辨率是由枚举类型``ppi_type_t``设置的，路径：``./include/components/video_types.h``

输出的图像格式
-----------------------

	当前的DVP只支持YUV422格式的数据输出

DVP配置参考代码
-----------------------

	路径:``./components/demos/media/video/dvp``