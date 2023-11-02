wanson asr development guide
==============================

:link_to_translation:`zh_CN:[中文]`

1、Functional Overview
---------------------------------

	wanson asr Library is mainly used for offline speech recognition, and can support custom wake-up words and command words through model training.

2、development data
---------------------------------

	The armino platform has adapted and integrated wanson's offline speech recognition library as a platform component, and provided a project for Huazhen's speech recognition application development. Customers can develop based on this project.
	
	For a detailed description of the API interface of the wanson-asr library, please refer to the source file: ``.\components\bk_thirdparty\asr\wanson\include\asr.h``

	wanson—asr storage path：``.\components\bk_thirdparty\asr\wanson\lib\libasr.a``

.. note::
	1. The wanson Speech Recognition Library requires the audio stream format to be: mono, 16K sampling rate, and 16bit bit width.
	2. wanson Speech Recognition Library is based on floating-point arithmetic.
	3. After modifying the wake word or command word, the ``libasr.a`` library needs to be replaced.

3、Real-time offline speech recognition application development
-----------------------------------------------------------------

The process of developing real-time offline recognition based on the wanson speech recognition library is as follows:

	1. Initialize speech recognition
	2. Initialize audio sampling
	3. Run speech recognition
	4. Turn on audio sampling

	Examples of interface calls are as follows:

::

	/* init wanson asr lib */
	Wanson_ASR_Init()
	//reset wanson asr
	Wanson_ASR_Reset();

	/* init mic record */
	aud_intf_drv_setup.aud_intf_tx_mic_data = aud_asr_handle;
	//init audio component
	bk_aud_intf_drv_init(&aud_intf_drv_setup);
	aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
	//set audio component work mode
	bk_aud_intf_set_mode(aud_work_mode);
	//init audio mic
	aud_intf_mic_setup.samp_rate = AUD_ADC_SAMP_RATE_16K;
	ret = bk_aud_intf_mic_init(&aud_intf_mic_setup);

	/* start Speech Recognition */
	//Continuously send the collected data to the algorithm for recognition
	Wanson_ASR_Recog((short*)asr_buff, 480, &text, &score);

	/* turn on audio sampling */
	bk_aud_intf_mic_start();

::

	Please refer to the source code for the complete demo： ``\components\demos\media\audio\audio_asr``

Reference link
--------------------------

	`wanson asr project <../../projects_work/thirdparty/wanson_asr/index.html>`_
