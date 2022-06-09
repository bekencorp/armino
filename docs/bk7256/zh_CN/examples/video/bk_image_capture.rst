拍照
========================

:link_to_translation:`en:[English]`

参考代码
---------------

	./components/demos/media/video/image_save

功能说明
---------------

拍照的功能主要是将sensor采集的一帧图像保存到SD卡，根据不同的sensor，也存在不同的操作方式。需要注意的是，
保存的图片都是jpeg格式的。

操作说明
---------------

当前支持两种类型的snsor：DVP和UVC。
在DVP类型下，命令执行如下：

1.fatfstest M 1

初始化SD卡

2.dvp init

初始化DVP，让与sensor相关的模块正常工作起来，并持续采样。

3.dvp save_image file_id

-file_id：保存的文件的索引，未避免命名重复，范围（0-255），功能为拍照，并保存为file_id_dvp.jpeg。如果需要继续拍照
可以继续输入这条命令，但是file_id需要修改。

4.dvp deinit

关闭sensor

在UVC类型下，命令执行如下：

1.fatfstest M 1

初始化SD卡

2.uvc init 480 30

初始化uvc，480：表示输出分辨率，640*480；30：表示输出帧率，30fps。支持其他分辨率和帧率，请参考``UVC_Camera``。

3.uvc start

uvc开始采样

4.uvc save_image file_id

-file_id：保存的文件的索引，未避免命名重复，范围（0-255），功能为拍照，并保存为file_id_uvc.jpeg。如果需要继续拍照
可以继续输入这条命令，但是file_id需要修改。

5.uvc stop

uvc停止采样，如果需要重新开始采样，输出命令3即可。

6.uvc deinit

关闭uvc
