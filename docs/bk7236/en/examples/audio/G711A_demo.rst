G711A encoder and decoder demo
===================================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------
	Perform G711A codec on the specified ``xx.pcm`` audio file, and output the codec processed audio to the ``xx.pcm`` file

2、Code path
--------------------
	demo path: ``\components\demos\media\audio\g711``

	For the detailed description of the API interface of G711A library, please refer to the same webpage: ``/api-reference/multi_media/bk_aec.html``

3、Cli command introduction
-------------------------------
The commands supported by the demo are as follows:

+---------------------------+----------------------------+
|Command                    |Description                 |
+===========================+============================+
|g711_encoder_test          |run G711 encoder test demo  |
+---------------------------+----------------------------+
|g711_decoder_test          |run G711 decoder test demo  |
+---------------------------+----------------------------+

The macro configuration that the demo runs depends on:

+-----------------------+------------------------------------------------+--------------------------------------------+-----+
|Name                   |Description                                     |   File                                     |value|
+=======================+================================================+============================================+=====+
|CONFIG_AUDIO_G711      |Configure whether G711 is compiled as a library |``\properties\soc\bk7236\bk7236.defconfig`` |  y  |
+-----------------------+------------------------------------------------+--------------------------------------------+-----+
|CONFIG_AUDIO_G711_TEST |Configure demo enable                           |``\middleware\soc\bk7236\bk7236.defconfig`` |  y  |
+-----------------------+------------------------------------------------+--------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - libg711.a G711 library
 - SD SDcard driver

4、Demo introduction
--------------------

The steps performed by the demo are as follows:

	1.Prepare the audio files needed for the test
	 - Save the audio file ``mic_data.pcm`` for encoding test and ``encoder_mic_data.pcm`` for decoding test provided in the demo path in the root directory of the TF card

	2.Mount TF card
	 - Uart sends AT command ``fatfstest M 1`` to mount TF card

	3.Perform coding tests
	 - Uart sends the AT command ``g711_encoder_test`` to execute the test and output the encoded audio file ``out_encoder_data.pcm`` to the root directory of the TF card

	4.Perform a decoding test
	 - Uart sends the AT command ``g711_decoder_test`` to execute the test, and output the decoded audio file ``out_decoder_data.pcm`` to the root directory of the TF card

5、Detailed configuration and description
------------------------------------------------
.. important::
  Precautions:
   - 1.G711 codec library also supports G711U codec, the test demo demonstrates G711A codec;
   - 2.The audio signal encoded and decoded by G711 is mono 16bit.
