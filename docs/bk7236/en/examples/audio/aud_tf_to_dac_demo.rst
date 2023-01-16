DAC play TF card PCM data demo
=================================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------
	Read the specified PCM music file from TF card and play it through DAC.

2、Code path
--------------------
	demo path: ``\components\demos\media\audio\aud_intf\demo\aud_intf_demo.c``

	For the detailed description of the Aud_Intf API interface, please refer to the same webpage: ``/api-reference/multi_media/bk_aud_intf.html``

3、Cli command introduction
-------------------------------
The commands supported by the demo are as follows:

+-----------------------------------------------------------+----------------------+
|Command                                                    |Description           |
+===========================================================+======================+
|aud_intf_play_test {init|start|pause|stop|deinit} xx.pcm   |Play xx.pcm file      |
+-----------------------------------------------------------+----------------------+

The macro configuration that the demo runs depends on:

+---------------------------+---------------------------------------+-------------------------------------------+-----+
|Name                       |Description                            |   File                                    |value|
+===========================+=======================================+===========================================+=====+
|CONFIG_AUDIO               |Configure the audio function           |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |Configure audio ip                     |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_INTF            |Configure aud_intf enable              |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_INTF_TEST       |Configure demo enable                  |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_TRANSFER      |Configuring Voice Transmission Enable  |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_TRAS_MODE_CPU0  |Configure Voice Transmission Mode      |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+---------------------------------------+-------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - DMA DMA driver
 - AUD audio module driver
 - SD SDcard and file system driver

4、Demo introduction
--------------------

The steps performed by the demo are as follows:

	1.Store the PCM audio file to be played in the TF card
	 - Copy the pcm format file to the TF card, and change the Chinese character file name to pinyin or number

	2.Mount TF card
	 - Uart sends AT command ``fatfstest M 1`` to mount TF card

	3.ready to play
	 - Uart sends AT command ``aud_intf_play_test init xx.pcm`` to initialize playback

	4.Start playing
	 - Uart sends AT command ``aud_intf_play_test start xx.pcm`` to start playing

	5.Pause playback
	 - Uart sends AT command ``aud_intf_play_test pause xx.pcm`` to pause playback

	6.Resume playback
	 - Uart sends AT command ``aud_intf_play_test start xx.pcm`` to resume playback

	7.Stop play
	 - Uart sends AT command ``aud_intf_play_test stop xx.pcm`` to stop playing

	8.end playback
	 - Uart sends AT command ``aud_intf_play_test deinit xx.pcm`` to end playback, close the pcm file, and log out of the playback path

Execute the test command, the workflow of the case is shown in the following figure:

.. figure:: ../../../_static/aud_pcm_to_dac.png
    :align: center
    :alt: tf_pcm_to_dac demo software flow
    :figclass: align-center

    Figure 1. tf pcm to dac demo flow chart

5、Detailed configuration and description
---------------------------------------------
.. note::
 - 1.The audio file name needs to be changed to character type, Chinese characters are invalid;
 - 2.The current demo sets the DAC sampling rate to 8K.
 