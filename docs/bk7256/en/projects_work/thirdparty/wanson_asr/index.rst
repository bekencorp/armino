Wanson asr demo
===================================

:link_to_translation:`zh_CN:[中文]`

1 Functional Overview
--------------------------
	Based on the local speech recognition library provided by the third-party company ``Huazhen``, the local offline voice wake-up word and command word recognition functions are realized.

2 Code Path
--------------------
	demo path: ``\components\demos\media\audio\audio_asr``

	Wanson local speech recognition library (floating point library) path: ``\components\bk_thirdparty\asr\wanson``

	For detailed description of Wanson's local speech recognition API interface, please refer to the source file: ``\components\bk_thirdparty\asr\wanson\include\asr.h``

3 Cli command introduction
-----------------------------------
The commands supported by the demo are as follows:

+--------------------------------------+--------------------------+
|Command                               |Description               |
+======================================+==========================+
|asr_file_test {xxx.pcm}               |Identify recordings       |
+--------------------------------------+--------------------------+
|aud_intf_asr_test {start|stop}        |real-time recognition     |
+--------------------------------------+--------------------------+

DEMO Compilation instruction: ``make bk7256 PROJECT=thirdparty/wanson_asr``

4. Demonstration introduction
-------------------------------------------

The steps performed to identify recordings are as follows:

	1. Mount TF card

	 - Uart sends AT command ``fatfstest M 1`` to mount TF card

	2. Identify audio files

	 - The demo board sends the command ``asr_file_test xxx.pcm`` to recognize the wake-up word and command word in the audio file

.. note::
  - 1. The format of the audio file required to identify the recording file is: ``mono, 16K sampling rate, 16 bit width``
  - 2. Recognition of recording files does not require CPU computing power, which is convenient for verifying that the function of the speech recognition library is normal

The steps performed by real-time recognition are as follows:

	1. Turn on real-time speech recognition

	 - The demo board sends the command ``aud_intf_asr_test start`` to open real-time speech recognition

	2. Test the wake-up words and command words, the supported wake-up words and command words are as follows:

	 - ``Little Bee Steward`` recognizes successfully and prints the log ``xiao feng guan jia`` on the serial port
	 - ``armino`` recognizes successfully and prints the log ``a er mi nuo`` on the serial port
	 - ``Visitor mode`` recognize successful serial port print log ``hui ke mo shi``
	 - ``dining mode`` recognizes successfully and prints the log ``yong can mo shi`` on the serial port
	 - ``Leave mode`` recognize successful serial port print log ``li kai mo shi``
	 - ``home mode`` identification success serial port print log ``hui jia mo shi``

	3. Turn off real-time speech recognition

5. Reference link
--------------------

	`wanson—asr development guide <../../../developer-guide/wanson_asr/index.html>`_

6. Shanghai Huazhen Electronic Technology Co., Ltd.
-----------------------------------------------------------
     | Official website: http://www.wanson.cn/
     | Headquarters Address: Room 307-308, Huigaoguang Innovation Park, No. 789 Shenwang Road, Minhang District, Shanghai
	 | Shenzhen Office Address: Room 2215-16, East Block, Building 1A, Huiyi City One Center, Xixiang, Baoan District, Shenzhen
     | Tel: 021-61557858
     | Mobile: 13524859176
     |         13296017858
