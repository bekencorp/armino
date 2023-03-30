ADC/DAC software loopback demo
=================================

:link_to_translation:`zh_CN:[中文]`

1、Function overview
--------------------
	The main functions of the ADC/DAC software loopback demo of the AUDIO module are: transfer the audio data of the audio sampling (AUDIO_ADC) to the audio playback (AUDIO_DAC) through DMA for playback.

2、Code path
--------------------
	demo path: ``\components\demos\media\audio\aud``

	For the detailed description of the AUDIO ADC/DAC API interface, please refer to the same webpage: ``/api-reference/multi_media/bk_aud.html``

3、Cli command introduction
-------------------------------
The commands supported by the demo are as follows:

+---------------------------------+---------------------------------+
|Command                          |Description                      |
+=================================+=================================+
|aud_adc_mcp_test {start|stop}    |cpu transfers mic data to dac    |
+---------------------------------+---------------------------------+
|aud_dtmf_mcp_test {start|stop}   |cpu transfers dtmf data to dac   |
+---------------------------------+---------------------------------+
|aud_adc_dma_test {start|stop}    |DMA transfers mic data to dac    |
+---------------------------------+---------------------------------+
|aud_adc_loop_test {start|stop}   |mic data to dac hard loopback    |
+---------------------------------+---------------------------------+
|aud_dtmf_loop_test {start|stop}  |dtmf data to dac hard loopback   |
+---------------------------------+---------------------------------+

The macro configuration that the demo runs depends on:

+---------------------------+------------------------------+----------------------------------------------------+-----+
|Name                       |Description                   |   File                                             |value|
+===========================+==============================+====================================================+=====+
|CONFIG_AUDIO               |Configure the audio function  |``\middleware\soc\bk7236_cp1\bk7236_cp1.defconfig`` |  y  |
+---------------------------+------------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |Configure the audio function  |``\middleware\soc\bk7236_cp1\bk7236_cp1.defconfig`` |  y  |
+---------------------------+------------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_TEST          |Configure the demo function   |``\middleware\soc\bk7236_cp1\bk7236_cp1.defconfig`` |  y  |
+---------------------------+------------------------------+----------------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - DMA DMA driver
 - AUD audio module driver

4、Demo introduction
--------------------

The steps performed by the demo are as follows:

	1.execute the test
	 - Uart sends AT command ``cpu1 aud_adc_dma_test start`` to execute the test

	2.Verification function
	 - Connect the headset to hear the audio captured by the onboard mic

	3.stop test
	 - Uart sends AT command ``cpu1 aud_adc_dma_test stop`` to stop the test

.. note::
 - For other demos, you can start or stop the test by sending the corresponding commands according to the command table supported by the cli according to the above steps.

Execute the test command, the workflow of the case is shown in the following figure:

.. figure:: ../../../_static/aud_adc_to_dac_flow.png
    :align: center
    :alt: adc_to_dac_demo software flow
    :figclass: align-center

    Figure 1. adc to dac demo flow chart

5、Detailed configuration and description
------------------------------------------------

- AUDIO_ADC performance

  AUDIO_ADC supports two working modes: DTMF and ADC:
   - DTMF working mode Direct digital mixing generates digital audio signal without mic sampling
   - ADC working mode ADC sampling of mic input signal, supports simultaneous sampling of up to 2 channels of mic

  The sampling rate Fs(bps) supported by AUDIO_ADC:

   - 8K
   - 16K
   - 44.1K
   - 48K

.. important::

  - 1.ADC working mode supports DMA transfer of mic sampling results
  - 2.DTMF working mode does not support DMA transfer of mixing results, only cpu transfer of mixing results
  - 3.ADC mode and DTMF mode only support one at the same time
