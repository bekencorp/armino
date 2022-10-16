MP3
========================

:link_to_translation:`zh_CN:[Chinese]`

1. Function overview
--------------------------
	Perform MP3 decoding on the specified 'xx.mp3' MP3 file, and output the decoded audio to the 'xx.pcm' file.
	Perform MP3 decoding on the specified 'xx.mp3' MP3 file, and output the decoded audio for playback through Audio DAC.

2. Code path
--------------------------
	demo path: ``\components\demos\media\audio\mp3``

	Please refer to the same webpage for the detailed description of the MP3 library API interface: ``/api-reference/multi_media/bk_mp3.html``

3. Cli command introduction
----------------------------------
The commands supported by the demo are as follows:

+-------------------------------------------+------------------------------------------------+
|Command                                    |Description                                     |
+===========================================+================================================+
|mp3_decode_test                            |MP3 decoding and store decoded audio to sdcard  |
+-------------------------------------------+------------------------------------------------+
|mp3_play_test                              |MP3 decoding and real-time playback             |
+-------------------------------------------+------------------------------------------------+

The macro configuration that the demo runs depends on:

+-----------------------+-------------------------------------------------+-------------------------------------------+-----+
|Name                   |Description                                      |   File                                    |value|
+=======================+=================================================+===========================================+=====+
|CONFIG_AUDIO_MP3_TEST  |Configure whether the demo take effect           |``\middleware\soc\bk7256\bk7256.deconfig`` |  y  |
+-----------------------+-------------------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO           |Configure whether the audio take effect          |``\middleware\soc\bk7256\bk7256.deconfig`` |  y  |
+-----------------------+-------------------------------------------------+-------------------------------------------+-----+
|CONFIG_AUDIO_MP3       |Configure whether helix is compiled as a library |``\properties\soc\bk7256\bk7256.deconfig`` |  y  |
+-----------------------+-------------------------------------------------+-------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - libhelix.a  MP3 decoder library
 - SD SDcard and file system driver

4. Demo introduction
--------------------------

	1.Prepare the audio files needed for the test
	 - Save the xx.mp3 file to be tested in the root directory of the SDCARD card. 

	2.Mount TF card
	 - The serial port tool sends the command ``fatfstest M 1`` to mount the TF card. 

	3.Perform a decoding test
	 - The serial port tool sends the command ``mp3_decode_test start xx.mp3 xx.pcm`` to perform test, and output the decoded xx.pcm audio file to the root directory of the TF card. 

	4.Perform decoding and play test
	 - The serial port tool sends the command ``mp3_play_test start/stop xx.mp3`` to perform test, and play or stop MP3 files in real time. 

5. Detailed configuration and description
--------------------------------------------
.. important::
  Attention:
   - 1. The size of the buffer allocation space for storing PCM data after decoding needs to be allocated according to the MP3 type. The common allocation size is 1152B or 2304B.
   - 2. MP3 decoding process is ``MP3DecoderInit ---> MP3FindSyncWord ---> MP3Decode ---> MP3GetLastFrameInfo ---> MP3FreeDecoder``
   - 3. The specific use of MP3 can be completely ported to the test demo of cli_mp3_play_test_cmd() in the mp3_demo.c file.