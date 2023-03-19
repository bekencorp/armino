FFT demo
=============

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------
	FFT demo mainly includes FFT and IFFT function tests, which can convert digital signals between time domain and frequency domain, and are mainly used for filtering processing of digital signals such as audio images.

2、Code path
--------------------
	demo path: ``\components\demos\media\audio\fft``

	For the detailed description of the API interface of the FFT module, please refer to the same webpage: ``/api-reference/multi_media/bk_fft.html``

3、Cli command introduction
-------------------------------
The commands supported by the demo are as follows:

+--------------------------------+----------------------+
|Command                         |Description           |
+================================+======================+
|cpu1 fft_fft_test {start|stop}  |run fft test demo     |
+--------------------------------+----------------------+
|cpu1 fft_ifft_test {start|stop} |run ifft test demo    |
+--------------------------------+----------------------+

The macro configuration that the demo runs depends on:

+---------------------+-------------------------------+---------------------------------------------------+-----+
|Name                 |Description                    |   File                                            |value|
+=====================+===============================+===================================================+=====+
|CONFIG_FFT           |Configure FFT function enable  |``\properties\soc\bk7256_cp1\bk7256_cp1.defconfig``|  y  |
+---------------------+-------------------------------+---------------------------------------------------+-----+
|CONFIG_FFT_TEST      |configuration demo enable      |``\properties\soc\bk7256_cp1\bk7256_cp1.defconfig``|  y  |
+---------------------+-------------------------------+---------------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - FFT fft driver

4、Demo introduction
--------------------  

The steps performed by the demo are as follows:

	1.Execute the fft test
	 - Uart sends AT command ``cpu1 fft_fft_test start`` to execute fft function test

	2.stop fft test
	 - Uart sends AT command ``cpu1 fft_fft_test stop`` to stop executing the fft function test

	3.perform ifft test
	 - Uart sends AT command ``cpu1 fft_ifft_test start`` to execute ifft function test

	4.stop ifft test
	 - Uart sends AT command ``cpu1 fft_ifft_test stop`` to stop executing ifft function test

Execute the test command, the workflow of the case is shown in the following figure:

.. figure:: ../../../_static/fft_demo_flow.png
    :align: center
    :alt: fft_demo software flow
    :figclass: align-center

    Figure 1. fft demo flow chart

5、Detailed configuration and description
------------------------------------------------
.. important::
  Precautions:
   - 1.Only one of the FFT and IFFT functions can be selected for execution at the same time;
