AUD_ADC
========================

:link_to_translation:`en:[English]`

1、概述
--------------------
	AUDIO模块主要包含音频采样(AUDIO_ADC)和音频播放(AUDIO_DAC)两部分功能。
	本小节将主要围绕重要的demo来介绍AUDIO_ADC的功能使用。

2、API参考
--------------------
	AUDIO ADC API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_aud.html``

3、性能说明
--------------------
- AUDIO_ADC性能

  AUDIO_ADC支持DTMF和ADC两种工作模式:
   - DTMF工作模式 直接数字混频生成数字音频信号，不需要mic采样
   - ADC工作模式 对mic输入信号进行ADC采样，最多支持2路mic的同时采样

  AUDIO_ADC支持的采样率Fs(bps):

   - 8K
   - 16K
   - 44.1K
   - 48K

.. important::

  - 1.ADC工作模式支持DMA搬运mic采样结果
  - 2.DTMF工作模式不支持DMA搬运混频结果, 只能cpu搬运混频结果
  - 3.ADC模式和DTMF模式同一时间仅支持一种

4、demo例程
--------------------
demo的源码和测试文件请参考以下工程路径：``demos/media/audio/aud/``

- ADC_TO_DAC(mic采样直接输出到DAC播放的demo)

  执行测试指令，case的工作流程如下图所示：

.. figure:: ../../../_static/aud_adc_to_dac_flow.png
    :align: center
    :alt: adc_to_dac_demo软件流程
    :figclass: align-center

    Figure 1. adc to dac demo flow chart

::

	void cli_aud_adc_dma_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
	{
		bk_err_t ret = BK_OK;
		aud_adc_config_t adc_config;
		uint32_t adc_fifo_addr;
		dma_config_t dma_config;
		uint32_t dac_fifo_addr;
		aud_dac_config_t dac_config;

		if (argc != 2) {
			cli_aud_help();
			return;
		}

		if (os_strcmp(argv[1], "start") == 0) {
			os_printf("audio adc dma test start\n");
			adc_test_mode = ADC_TEST_MODE_DMA;
			adc_config.samp_rate = AUD_ADC_SAMP_RATE_48K;
			adc_config.adc_enable = AUD_ADC_DISABLE;
			adc_config.line_enable = AUD_ADC_LINE_DISABLE;
			adc_config.dtmf_enable = AUD_DTMF_DISABLE;
			adc_config.adc_hpf2_coef_B2 = 0;
			adc_config.adc_hpf2_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
			adc_config.adc_hpf1_bypass_enable = AUD_ADC_HPF_BYPASS_ENABLE;
			adc_config.adc_set_gain = 0x2d;
			adc_config.adc_samp_edge = AUD_ADC_SAMP_EDGE_RISING;
			adc_config.adc_hpf2_coef_B0 = 0;
			adc_config.adc_hpf2_coef_B1 = 0;
			adc_config.adc_hpf2_coef_A0 = 0;
			adc_config.adc_hpf2_coef_A1 = 0;
			adc_config.dtmf_wr_threshold = 8;
			adc_config.adcl_wr_threshold = 8;
			adc_config.dtmf_int_enable = AUD_DTMF_INT_DISABLE;
			adc_config.adcl_int_enable = AUD_ADCL_INT_DISABLE;
			adc_config.loop_adc2dac = AUD_LOOP_ADC2DAC_DISABLE;
			adc_config.agc_noise_thrd = 101;
			adc_config.agc_noise_high = 101;
			adc_config.agc_noise_low = 160;
			adc_config.agc_noise_min = 1;
			adc_config.agc_noise_tout = 0;
			adc_config.agc_high_dur = 3;
			adc_config.agc_low_dur = 3;
			adc_config.agc_min = 1;
			adc_config.agc_max = 4;
			adc_config.agc_ng_method = AUD_AGC_NG_METHOD_MUTE;
			adc_config.agc_ng_enable = AUD_AGC_NG_DISABLE;
			adc_config.agc_decay_time = AUD_AGC_DECAY_TIME_128;
			adc_config.agc_attack_time = AUD_AGC_ATTACK_TIME_128;
			adc_config.agc_high_thrd = 18;
			adc_config.agc_low_thrd = 0;
			adc_config.agc_iir_coef = AUD_AGC_IIR_COEF_1_1024;
			adc_config.agc_enable = AUD_AGC_DISABLE;
			adc_config.manual_pga_value = 0;
			adc_config.manual_pga_enable = AUD_GAC_MANUAL_PGA_DISABLE;
			adc_config.adc_fracmod_manual = AUD_ADC_TRACMOD_MANUAL_DISABLE;
			adc_config.adc_fracmod = 0;

			dac_config.dac_enable = AUD_DAC_DISABLE;
			dac_config.samp_rate = AUD_DAC_SAMP_RATE_SOURCE_48K;
			dac_config.dac_hpf2_coef_B2 = 0x3A22;
			dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
			dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
			dac_config.dac_set_gain = 0x2D;
			dac_config.dac_clk_invert = AUD_DAC_CLK_INVERT_RISING;

			dac_config.dac_hpf2_coef_B0 = 0x3A22;
			dac_config.dac_hpf2_coef_B1 = 0x8BBF;

			dac_config.dac_hpf2_coef_A1 = 0x751C;
			dac_config.dac_hpf2_coef_A2 = 0xC9E6;

			dac_config.dacr_rd_threshold = 0x4;
			dac_config.dacl_rd_threshold = 0x4;
			dac_config.dacr_int_enable = 0x0;
			dac_config.dacl_int_enable = 0x0;

			dac_config.dac_filt_enable = AUD_DAC_FILT_DISABLE;
			dac_config.dac_fracmod_manual_enable = AUD_DAC_FRACMOD_MANUAL_DISABLE;
			dac_config.dac_fracmode_value = 0x0;

			//init audio driver
			bk_aud_driver_init();

			bk_aud_adc_init(AUD_ADC_WORK_MODE_ADC, &adc_config, NULL);
			bk_aud_dac_init(&dac_config);
			os_printf("init audio driver, adc and dac successful\n");

			//init dma driver
			ret = bk_dma_driver_init();
			if (ret != BK_OK) {
				os_printf("dma driver init failed\r\n");
				return;
			}
			dma_config.mode = DMA_WORK_MODE_REPEAT;
			dma_config.chan_prio = 1;
			dma_config.src.dev = DMA_DEV_AUDIO;
			dma_config.src.width = DMA_DATA_WIDTH_32BITS;
			dma_config.dst.dev = DMA_DEV_AUDIO;
			dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
			//get adc fifo address
			if (bk_aud_get_adc_fifo_addr(&adc_fifo_addr) != BK_OK) {
				os_printf("get adc fifo address failed\r\n");
				return;
			} else {
				dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
				dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
				dma_config.src.start_addr = adc_fifo_addr;
				dma_config.src.end_addr = adc_fifo_addr + 4;
			}
			//get dac fifo address
			if (bk_aud_get_dac_fifo_addr(&dac_fifo_addr) != BK_OK) {
				os_printf("get dac fifo address failed\r\n");
				return;
			} else {
				dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
				dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
				dma_config.dst.start_addr = dac_fifo_addr;
				dma_config.dst.end_addr = dac_fifo_addr + 4;
			}

			os_printf("source_addr:0x%x, dest_addr:0x%x\r\n", dma_config.src.start_addr, dma_config.dst.start_addr);

			//init dma channel
			ret = bk_dma_init(DMA_ID_0, &dma_config);
			if (ret != BK_OK) {
				os_printf("dma init failed\r\n");
				return;
			}
			bk_dma_set_transfer_len(DMA_ID_0, 4);
			ret = bk_dma_start(DMA_ID_0);

			//start adc and dac
			bk_aud_start_adc();
			bk_aud_start_dac();
			os_printf("enable adc and dac successful\n");

			//aud_struct_dump();

			if (ret == BK_OK)
				os_printf("start audio adc test successful\r\n");
			else
				os_printf("start audio adc test failed\r\n");
		} else if (os_strcmp(argv[1], "stop") == 0) {
			os_printf("audio adc test stop\n");
			adc_test_mode = ADC_TEST_MODE_NULL;
			//disable adc and dac
			bk_aud_stop_adc();
			bk_aud_stop_dac();
			bk_aud_driver_deinit();
			//stop dma
			bk_dma_stop(DMA_ID_0);
			bk_dma_deinit(DMA_ID_0);
			os_printf("audio adc test stop successful\n");
		} else {
			cli_aud_help();
			return;
		}
	}


- ADC_TO_PCM(录音到SD卡demo)

  执行测试指令，case的工作流程如下图所示：

.. figure:: ../../../_static/aud_adc_to_pcm.png
    :align: center
    :alt: adc_to_pcm_demo软件流程
    :figclass: align-center

    Figure 2. adc to pcm demo flow chart


.. note::
 - 1.case执行前需要初始化PSRAM和挂载SD卡;
 - 2.执行此case时需要确保没有其他模块正在使用PSRAM;
 - 3.此demo源码较多, 所以就不在此处展开说明了，请参考源码;


5、AUDIO ADC使用注意事项
--------------------------
.. important::
  注意事项:
   - DTMF模式不能使用DMA搬运数据;
