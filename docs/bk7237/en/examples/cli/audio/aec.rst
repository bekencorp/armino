AEC Cli
================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------------
	In order to facilitate the verification of AEC echo cancellation-related functions, a series of cli CMDs are designed for this purpose, which is also a scene demo of the echo cancellation function for customers' reference.


2、Code path
--------------------------
	path: ``components\demos\media\audio\aec\aec_demo.c``

3、Cli command introduction
------------------------------------
The supported CMDs are as follows:

	1.aec_test
	 - Function Description
		Used to test the echo cancellation function.
	 - CMD Format
		aec_test

	 - Test Case
		Perform echo cancellation on ``mic.pcm and ref.pcm`` in the TF card, and output to ``out.pcm``: ``aec_test``
