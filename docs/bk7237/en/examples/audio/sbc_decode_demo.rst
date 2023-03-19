SBC_DEC
========================

:link_to_translation:`zh_CN:[中文]`

1. Function overview
--------------------------
	SBC(Sub Band Codec)is an audio codec algorithm with low computational complexity that transmits audio data at a moderate bit rate. The basic principle is to divide the frequency of the signal into several sub-bands, and then encode each sub-band. properties and characteristics assign different bits (sampling depth) to represent the data.

	The SBC algorithm includes decoding and decoding functions. Since the SDK only supports the SBC decoder, it shows the main use functions of the decoder. In addition, there is a class of SBC decoders that support the decoding functions of the SBC and the decoder.

	The main function of sbc_decode demo is that decode the sbc data through sbc_decode and play it directly through the Audio DAC in real time.

2. Code path
--------------------------
	demo path: ``\components\demos\media\audio\sbc_decode``
	
	For the detailed description of the sbc_decode api interface, please refer to the webpage ``/api-reference/multi_media/bk_sbc.html``
	For the detailed description of the aud_intf api interface, please refer to the webpage ``/api-reference/multi_media/bk_aud_intf.html``

3. Cli command introduction
--------------------------------
The commands supported by the demo are as follows:

+-------------------------------------------+------------------------------------+
|Command                                    |Description                         |
+===========================================+====================================+
|sbc_decoder_test {start|stop} {xxx.sbc}    |sbc decoding and real-time playback |
+-------------------------------------------+------------------------------------+

The macro configuration that the demo runs depends on:

+----------------+---------------------------------------------+--------------------------------------------+-----+
|Name            |Description                                  |   File                                     |value|
+================+=============================================+============================================+=====+
|CONFIG_SBC      |Configure whether the sbc decode take effect |``\middleware\soc\bk7237\bk7237.defconfig`` |  y  |
+----------------+---------------------------------------------+--------------------------------------------+-----+
|CONFIG_SBC_TEST |Configure whether the demo take effect       |``\middleware\soc\bk7237\bk7237.defconfig`` |  y  |
+----------------+---------------------------------------------+--------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - DMA DMA driver
 - AUD audio driver
 - SD SDcard and file system driver
 
4. Demo introduction
--------------------------

	1.Prepare the audio files needed for the test
	 - Save the xx.sbc file to be tested in the root directory of the SDCARD card.

	2.Mount TF card
	 - The serial port tool sends the command ``fatfstest M 1`` to mount the TF card.

	3.Perform a decoding test
	 - The serial port tool sends the command ``sbc_decoder_test start xx.sbc`` to perform test.

	4.Stop a decoding test
	 - The serial port tool sends the command ``sbc_decoder_test stop xx.sbc`` to stop test.

.. note::
 - The API of sbc_decoder_frame_decode() automatically detects whether it is SBC decoding or MSBC decoding. You only need to pass the storage address of the encoded data as the address into the API to get the decoded PCM data.

SBC decode demo workflow is shown in the figure below:

.. figure:: ../../../_static/sbc_user_guide_flow.png
    :align: center
    :alt: SBC Decode work Flow Chart
    :figclass: align-center

    SBC Decode Work Flow Chart

5. Detailed configuration and description
----------------------------------------------
- Sample rate

 - 16KHz
 - 32KHz
 - 44.1KHz
 - 48KHz

- Channel mode

 - MONO
 - DUAL
 - STEREO
 - JOINT_STEREO

- Blocks

 - 4
 - 8
 - 12
 - 16
 
- Subbands

 - 4
 - 8
 
- Allocation

 - SNR
 - Loudness

- Syncword

 - 0x9C(SBC)
 - 0xAD(MSBC)



