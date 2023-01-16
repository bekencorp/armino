EQ
========================

:link_to_translation:`zh_CN:[Chinese]`

1. Function overview
---------------------------
	The main function of the EQ demo is to equalize the dance style.
	
2. Code path
--------------------
	demo path:``\components\demos\media\audio\aud``

	For the detailed description of the AUDIO DAC and EQ api interface, please refer to the webpage: ``/api-reference/multi_media/bk_aud.html``

3. Cli command introduction
---------------------------------
The commands supported by the demo are as follows:

+---------------------------------+----------------------+
|Command                          |Description           |
+=================================+======================+
|aud_eq_test {start|stop}         |equalize Dance style  |
+---------------------------------+----------------------+

The macro configuration that the demo runs depends on:

+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_AUDIO               |Configure audio function    |``\middleware\soc\bk7236_cp1\bk7236_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_RISCV_IP_V1_0 |Configure audio ip          |``\middleware\soc\bk7236_cp1\bk7236_cp1.defconfig`` |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+
|CONFIG_AUDIO_TEST          |Configure the demo enable   |``\middleware\soc\bk7236\bk7236.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+

Demo runs dependent libraries and drivers:
 - DMA DMA driver
 - AUD audio driver
 
4. Demo introduction
--------------------------

The steps performed by the demo are as follows:

	1.Perform test
	 - The serial port tool sends the command ``cpu1 aud_eq_test start`` to perform test. 
	
	2.Input a frequency sweep signal sampled by ADC and play the data through DAC.
	
	3.Use the instrument to collect the DAC output signal to check whether the frequency response signal is consistent with the designed EQ frequency response.

	4.Stop test
	 - The serial port tool sends the command ``cpu1 aud_eq_test stop`` to stop test. 

5. Detailed configuration and description
-----------------------------------------------
	The EQ function mainly configures parameters through the following structures. The parameter settings of EQ mainly include: A1, A2, B0, B1, B2, among which B0, B1 and B2 are normal configurations, and the symbols of A1 and A2 should be reversed.

::

	typedef struct {
		int32_t flt0_A1;
		int32_t flt0_A2;
		int32_t flt0_B0;
		int32_t flt0_B1;
		int32_t flt0_B2;

		int32_t flt1_A1;
		int32_t flt1_A2;
		int32_t flt1_B0;
		int32_t flt1_B1;
		int32_t flt1_B2;

		int32_t flt2_A1;
		int32_t flt2_A2;
		int32_t flt2_B0;
		int32_t flt2_B1;
		int32_t flt2_B2;

		int32_t flt3_A1;
		int32_t flt3_A2;
		int32_t flt3_B0;
		int32_t flt3_B1;
		int32_t flt3_B2;
	} aud_eq_config_t;


The specific EQ function mainly calls the following two APIs to realize the use and release of EQ.

 - :cpp:func:`bk_aud_eq_init` - init the eq module of audio
 - :cpp:func:`bk_aud_eq_deinit` - deinit the eq module of audio

The main code of the demo is as follows:

::

	aud_eq_config_t eq_config;
	eq_config.flt0_A1 = ~(-2085356);
	eq_config.flt0_A2 = ~(1036853);
	eq_config.flt0_B0 = 1054751;
	eq_config.flt0_B1 = -2085356;
	eq_config.flt0_B2 = 1030678;

	eq_config.flt1_A1 = ~(-2039888);
	eq_config.flt1_A2 = ~(992405);
	eq_config.flt1_B0 = 1041334;
	eq_config.flt1_B1 = -2039888;
	eq_config.flt1_B2 = 999646;

	eq_config.flt2_A1 = ~(-1694272);
	eq_config.flt2_A2 = ~(705464);
	eq_config.flt2_B0 = 1165692;
	eq_config.flt2_B1 = -1694272;
	eq_config.flt2_B2 = 588347;

	eq_config.flt3_A1 = ~(487698);
	eq_config.flt3_A2 = ~(-73179);
	eq_config.flt3_B0 = 1038338;
	eq_config.flt3_B1 = 487698;
	eq_config.flt3_B2 = -62942;
	bk_aud_eq_init(&eq_config);


