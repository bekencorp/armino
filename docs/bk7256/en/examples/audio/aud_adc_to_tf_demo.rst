Audio record demo
========================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
------------------------
	mic collects audio data and saves it as ``xx.pcm`` file to TF card.

2、Code path
--------------------
	demo path: ``\components\demos\media\audio\aud_intf\demo\aud_intf_demo.c``

	For the detailed description of the Aud_Intf API interface, please refer to the same webpage: ``/api-reference/multi_media/bk_aud_intf.html``

3、Cli command introduction
--------------------------------
The commands supported by the demo are as follows:

+-------------------------------------------------------------+-----------------------+
|Command                                                      |Description            |
+=============================================================+=======================+
|aud_intf_record_test {init|start|pause|stop|deinit} xx.pcm   |Record to xx.pcm file  |
+-------------------------------------------------------------+-----------------------+

The macro configuration that the demo runs depends on:

+---------------------------+---------------------------------------+-------------------------------------------+-----+
|Name                       |Description                            |   File                                    |value|
+===========================+=======================================+===========================================+=====+
|CONFIG_AUDIO               |Configure the audio function           |``\middleware\soc\bk7256\bk7256.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |Configure audio ip                     |``\middleware\soc\bk7256\bk7256.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_INTF            |Configure aud_intf enable              |``\middleware\soc\bk7256\bk7256.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_INTF_TEST       |Configure demo enable                  |``\middleware\soc\bk7256\bk7256.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_TRANSFER      |Configuring Voice Transmission Enable  |``\middleware\soc\bk7256\bk7256.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_TRAS_MODE_CPU0  |Configure Voice Transmission Mode      |``\middleware\soc\bk7256\bk7256.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - DMA DMA driver
 - AUD audio module driver
 - SD SDcard and file system driver

4 Demo introduction
----------------------

The steps performed by the demo are as follows:

	1.Mount TF card
	 - Uart sends AT command ``fatfstest M 1`` to mount TF card

	2.ready to record
	 - Uart sends AT command ``aud_intf_record_test init xx.pcm`` to initialize recording

	3.start recording
	 - Uart sends AT command ``aud_intf_record_test start xx.pcm`` to start recording, the audio will be saved in the root directory file of TF card ``xx.pcm``

	4.Pause recording
	 - Uart sends AT command ``aud_intf_record_test pause xx.pcm`` to pause recording

	5.Resume recording
	 - Uart sends AT command ``aud_intf_record_test start xx.pcm`` to resume recording

	6.stop recording
	 - Uart sends AT command ``aud_intf_record_test stop xx.pcm`` to stop recording

	7.end recording
	 - Uart sends AT command ``aud_intf_record_test deinit xx.pcm`` to end the recording, close the recording file, and log off the recording channel

5、Detailed configuration and description
-----------------------------------------------
.. note::
 - 1.Before executing this demo, please ensure that the tf card can be used normally;
 - 3.There are many source codes for this demo, so I won't explain it here, please refer to the source code.
