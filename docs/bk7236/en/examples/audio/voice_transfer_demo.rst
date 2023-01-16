Voice transfer demo
========================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------
	The two-way voice call between the board and the mobile phone apk is realized through wifi.

2、Code path
--------------------
	demo path: ``\components\demos\media\doorbell\src\doorbell.c``

	For the detailed description of the Aud_Intf API interface, please refer to the same webpage: ``/api-reference/multi_media/bk_aud_intf.html``

3、Cli command introduction
-------------------------------
The commands supported by the demo are as follows:

+-----------------------------------------------------------+-----------------------------+
|Command                                                    |Description                  |
+===========================================================+=============================+
|video_transfer -a test 12345678                            |Initialize the voice channel |
+-----------------------------------------------------------+-----------------------------+

The macro configuration that the demo runs depends on:

+---------------------------+-------------------------------------+-------------------------------------------+-----+
|Name                       |Description                          |   File                                    |value|
+===========================+=====================================+===========================================+=====+
|CONFIG_AUDIO               |Configure the audio function         |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+-------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |Configure audio ip                   |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+-------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_INTF            |Configure aud_intf enable            |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+-------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_INTF_TEST       |Configure demo enable                |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+-------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_TRANSFER      |Configure voice transmission enable  |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+-------------------------------------+-------------------------------------------+-----+
|CONFIG_AUD_TRAS_MODE_CPU0  |Configure Voice Transmission Mode    |``\middleware\soc\bk7236\bk7236.defconfig``|  y  |
+---------------------------+-------------------------------------+-------------------------------------------+-----+

Voice call debugging macro configuration:

+----------------------------------------+----------------------------------------+-------------------------------------------+
|Name                                    |Description                             |   File                                    |
+========================================+========================================+===========================================+
|CONFIG_AUD_TRAS_AEC_DUMP_DEBUG          |Configure aec dump                      |``\middleware\soc\bk7236\bk7236.defconfig``|
+----------------------------------------+----------------------------------------+-------------------------------------------+
|CONFIG_AUD_TRAS_LOST_COUNT_DEBUG        |Configuring Tx Packet Loss Statistics   |``\middleware\soc\bk7236\bk7236.defconfig``|
+----------------------------------------+----------------------------------------+-------------------------------------------+
|CONFIG_AUD_TRAS_AEC_MIC_DELAY_DEBUG     |Configure mic delay debug               |``\middleware\soc\bk7236\bk7236.defconfig``|
+----------------------------------------+----------------------------------------+-------------------------------------------+
|CONFIG_AUD_TRAS_AEC_MIC_DELAY_POINTS    |Configure mic delay points              |``\middleware\soc\bk7236\bk7236.defconfig``|
+----------------------------------------+----------------------------------------+-------------------------------------------+

Demo runs dependent libraries and drivers:
 - DMA DMA driver
 - AUD audio module driver
 - Sdcard module driver
 - WIFI module

4、Demo introduction
--------------------

The steps performed by the demo are as follows:

	1.Open the audio streaming app
	 - Uart sends AT command ``video_transfer -a test 12345678`` to initialize the voice call application

	2.establish a wifi connection
	 - The mobile phone wifi is connected to the route of the test name, the password is: 12345678

	3.setup mobile apk
	 - After the connection is successful, open the video transfer app, and set ``Settings->Demos Solution`` to ``Video transfer`` mode.

	4.Start a voice call
	 - Click the voice call button on the apk interface to start a voice call

	5.stop voice call
	 - Click the voice call button on the apk interface again to stop the voice call

5、Voice call function adaptation instructions
---------------------------------------------------

The debugging process of the voice call function is as follows:

(1) Functional debugging
	- 1、Turn off all debug macros
	- 2、Refer to the sample code of the ``bk_aud_intf_voc_init`` interface, call this interface to initialize the voice call;
	- 3、Call the ``bk_aud_intf_write_spk_data`` interface to write the voice data sent by the apk to the board for playback;

(2) mic delay point debugging
	- 1、Set the debug macros ``CONFIG_AUD_TRAS_AEC_DUMP_DEBUG`` and ``CONFIG_AUD_TRAS_AEC_MIC_DELAY_DEBUG`` to y, compile bin for testing, talk for a minute, and dump the data of the AEC algorithm to the tf card;
	- 2、According to the dump data, calculate the delay points of mic, and set the point value to the value of the macro ``CONFIG_AUD_TRAS_AEC_MIC_DELAY_POINTS`` (normally, the default value of 55 can be used);

(3) aec echo cancellation effect debugging
	- 1、open voice call;
	- 2、According to the echo effect heard, use ``Realtime Video`` apk (downloadable from the open source armino platform on github) to debug the parameters of AEC and NS online, and record the final optimal value;
	- 3、First adjust ``echo depth`` , the larger the echo, the larger the value will be. When you continue to increase the value, but the echo cancellation effect cannot be improved, stop setting the value;
	- 4、Secondly, set the value of ``max amplitude`` and ``min amplitude`` according to the size and range of the echo to optimize the echo cancellation effect;
	- 5、Then set the ``noise level`` according to the size of the noise floor you hear. The larger the noise floor, the larger the set value, to optimize the noise floor removal effect;
	- 4、Finally, set the value of ``noise param``, set to 0, 1, 2 respectively, and choose the best value;
	- 5、Record the online debugging results of all parameters and set them as the default parameters when the voice is initialized.

.. note::
 - 1.You can also use AT command to debug online, send serial command ``aud_intf_set_aec_param_test {param value}`` through uart to set the value of each parameter. The specific debugging process is the same as the above method. For details, please refer to the example in ``\components\demos\media\aud\aud_intf\demo\aud_intf_demo.c``.
 - 2.The AEC online debugging parameter setting interface is shown in the figure below.

.. figure:: ../../../_static/aud_voc_aec_1.png
    :align: center
    :alt: AEC parameter setting entry interface
    :figclass: align-center

    Figure 1. AEC parameter setting entry interface

.. figure:: ../../../_static/aud_voc_aec_2.png
    :align: center
    :alt: AEC parameter setting interface
    :figclass: align-center

    Figure 1. AEC parameter setting interface

(3) Turn off all debug macros
	- Close all debug macros when debugging work is done

.. note::
 - 1.The current voice call is based on a single mic scene, 8K sampling rate;
