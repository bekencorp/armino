AEC
===================================

:link_to_translation:`zh_CN:[中文]`

1 Functional Overview
--------------------------
	The main function of the echo cancellation library is to eliminate the echo in the voice call scene to ensure high-quality voice calls, and it also has the functions of noise reduction and volume adjustment.

2 Code Path
--------------------
	demo path: ``\components\demos\media\audio\aec``

	For the detailed description of the API interface of the AEC library, please refer to the same webpage: ``/api-reference/multi_media/bk_aec.html``

3 Cli command introduction
-----------------------------------
The commands supported by the demo are as follows:

+---------------------------+----------------------+
|Command                    |Description           |
+===========================+======================+
|aec_test                   |run AEC demo          |
+---------------------------+----------------------+

The macro configuration that the demo runs depends on:

+---------------------+-----------------------------------------------+--------------------------------------------+-----+
|Name                 |Description                                    |   File                                     |value|
+=====================+===============================================+============================================+=====+
|CONFIG_AUDIO_AEC     |Configure whether AEC is compiled as a library |``\properties\soc\bk7237\bk7237.defconfig`` |  y  |
+---------------------+-----------------------------------------------+--------------------------------------------+-----+
|CONFIG_AUDIO_TEST    |Whether the configuration demo takes effect    |``\middleware\soc\bk7237\bk7237.defconfig`` |  y  |
+---------------------+-----------------------------------------------+--------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - libaec.a AEC echo cancellation library
 - SD SDcard and file system driver

4 Demo introduction
--------------------

The steps performed by the demo are as follows:

	1.Prepare the audio files needed for the test
	 - Save the original mic audio file ``mic.pcm`` and the corresponding reference signal ``ref.pcm`` file that needs echo cancellation and noise reduction provided in the demo path in the root directory of the TF card
	
	2.Mount TF card
	 - Uart sends AT command ``fatfstest M 1`` to mount TF card

	3.execute the test
	 - Uart sends the AT command ``aec_test`` to execute the test and output the audio file after echo cancellation ``out.pcm``

Execute the test command, the workflow of the case is shown in the following figure:

.. figure:: ../../../_static/aec_demo_flow.png
    :align: center
    :alt: aec_demo software flow
    :figclass: align-center

    Figure 1. aec demo flow chart

5 Detailed configuration and description
----------------------------------------------
.. important::
  Precautions:
   - 1.The AEC algorithm requires more than 20 kBytes of buffers, and the memory of AECContext needs to be allocated before initialization;
   - 2.Algorithm parameters are adjusted using the ``aec_ctrl`` interface.
