I2S模块(i2s_demo)
========================

:link_to_translation:`en:[English]`

1 概述
--------------------------
	I2S模块主要主要用于音频信号的传输。
	本小节将主要围绕demo来介绍FFT模块库的使用。

2 API参考
--------------------------
	I2S模块API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_i2s.html``

3 工作模式&采样率
--------------------------
- 工作模式

	I2S模块支持下述多种工作模式:
	 - Philips
	 - Left Justified
	 - Right Justified
	 - Short Sync Frame
	 - Long Sync Frame
	 - Normal 2B+D模式
	 - Delay 2B+D模式
	 - TDM模式

.. important::

  使用TDM模式时, 只需要将工作模式配置为其他的7中工作模式中的一种, 然后使能多个通道就可，模块内部会将多个通道的数据组合成时分复用的形式发出

- 采样率

	I2S模块支持下述采样率Fs(bps):
	 - 8K
	 - 12K
	 - 16K
	 - 24K
	 - 32K
	 - 48K
	 - 96K
	 - 8.0182K
	 - 11.025K
	 - 22.05K
	 - 44.1K
	 - 88.2K

.. note::
  建议采用前6种常用的采样率

4 demo例程
--------------------------
demo的源码和测试文件请参考以下工程路径: ``demos/media/audio/i2s``

- i2s send sin data test code

::

	void cli_i2s_master_sin_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
	{
		i2s_config_t i2s_config;
		i2s_rate_t rate;
		//uint32_t data_buf_rl = 0xffff0000;
		uint32_t write_flag = 0;
		uint32_t i = 0;

		if (argc != 2) {
			cli_i2s_help();
			return;
		}

		if (os_strcmp(argv[1], "start") == 0) {
			os_printf("i2s master test start\n");
			i2s_config.i2s_en = I2S_DISABLE;
			i2s_config.role = I2S_ROLE_MASTER;
			i2s_config.work_mode = I2S_WORK_MODE_I2S;
			i2s_config.lrck_invert = I2S_LRCK_INVERT_DISABLE;
			i2s_config.sck_invert = I2S_SCK_INVERT_DISABLE;
			i2s_config.lsb_first_en = I2S_LSB_FIRST_DISABLE;
			i2s_config.sync_length = 0;
			i2s_config.data_length = 15;
			i2s_config.pcm_dlength = 0;
			i2s_config.sample_ratio = 0;
			i2s_config.sck_ratio = 0;
			i2s_config.parallel_en = I2S_PARALLEL_DISABLE;
			i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
			i2s_config.sck_ratio_h4b = 0;
			i2s_config.sample_ratio_h2b = 0;
			i2s_config.txint_level = I2S_TXINT_LEVEL_1;
			i2s_config.rxint_level = I2S_RXINT_LEVEL_24;


			//init i2s driver
			bk_i2s_driver_init();

			//init i2s configure
			bk_i2s_init(I2S_GPIO_GROUP_0, &i2s_config);
			os_printf("init i2s driver and config successful\n");

			//register isr
			bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXUDF, cli_i2s_master_txudf_isr, NULL);
			bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXOVF, cli_i2s_master_rxovf_isr, NULL);
			bk_i2s_register_i2s_isr(I2S_ISR_CHL1_TXINT, cli_i2s_master_txint_isr, NULL);
			bk_i2s_register_i2s_isr(I2S_ISR_CHL1_RXINT, cli_i2s_master_rxint_isr, NULL);
			os_printf("register i2s isr successful\n");

			//set sample and bitclk ratio
			rate.datawidth = I2S_DATA_WIDTH_16;
			rate.samp_rate = I2S_SAMP_RATE_48000;
			bk_i2s_set_ratio(&rate);

			//enable i2s
			bk_i2s_enable(I2S_ENABLE);
			os_printf("enable i2s successful\n");

			i2s_struct_dump();

			//for (i=0; i < 60; i++)
			while (1)
			{
				for (i = 0; i<I2S_TEST_DATA_SIZE; i++)
				{
					bk_i2s_get_write_ready(&write_flag);
					while (!write_flag)
						bk_i2s_get_write_ready(&write_flag);
					bk_i2s_write_data(1, &data_source[i], 1);
					//delay(10);
					//os_printf("write data: 0x%08x\r\n", data_source[i]);
				}
			}

			os_printf("start i2s master send test successful\r\n");
		} else if (os_strcmp(argv[1], "stop") == 0) {
			os_printf("i2s sin test stop\n");
			bk_i2s_driver_deinit();
			os_printf("i2s sin test stop successful\n");
		} else {
			cli_i2s_help();
			return;
		}
	}


4 demo工作流程
--------------------------

执行测试指令, case的工作流程如下图所示:

.. figure:: ../../../_static/i2s_demo_flow.png
    :align: center
    :alt: i2s软件流程
    :figclass: align-center

    Figure 1. i2s work flow chart

5 I2S使用注意事项
--------------------------
.. important::
  注意事项:
   - 1.I2S通讯时master和slave的DIN和DOUT引脚连接要正确，master的DIN和slave的DOUT连接，master的DOUT和slave的DIN连接;
   - 2.master和slave的工作模式要一致;
