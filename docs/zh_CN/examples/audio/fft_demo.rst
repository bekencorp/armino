FTT模块(fft_demo)
========================

:link_to_translation:`en:[English]`

1 概述
""""""""""""""""""""""""""
	FFT模块主要包含FFT和IFFT功能，可对数字信号进行时域和频域间的转换，主要用于音频图像等数字信号的滤波处理。
	本小节将主要围绕demo来介绍FFT模块库的使用。

2 API参考
""""""""""""""""""""""""""
	FFT模块API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_fft.html``

3 demo例程
""""""""""""""""""""""""""
demo的源码和测试文件请参考以下工程路径: ``demos/media/audio/fft``

- FFT test code

::

	void cli_fft_fft_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
	{
		bk_err_t ret = BK_OK;
		uint32_t i = 0;
		int32 temp_q = 0, temp_i = 0;
		int32 comp_q = 0, comp_i = 0;
		fft_input_t fft_conf = {0};

		if (argc != 2) {
			cli_fft_help();
			return;
		}

		if (os_strcmp(argv[1], "start") == 0) {
			os_printf("fft test start\n");

			//init fft driver
			ret = bk_fft_driver_init();
			if (ret != BK_OK)
				return;
			os_printf("fft driver successful\r\n");

			fft_conf.inbuf = os_malloc(4*FFT_TEST_DATA_SIZE);
			os_memset(fft_conf.inbuf, 0, 4*FFT_TEST_DATA_SIZE);

			fft_conf.mode = FFT_WORK_MODE_FFT;
			fft_conf.size = FFT_TEST_DATA_SIZE;
			os_printf("\r\nfft_data_source:\n");
			for (i = 0; i < FFT_TEST_DATA_SIZE; i++) {
				fft_conf.inbuf[i] = fft_data_source[i];
				//os_printf("0x%08x, ", fft_conf.inbuf[i]);
			}
			os_printf("\r\n");
			//start fft
			bk_fft_enable(&fft_conf);
			os_printf("start fft process \r\n");
			//fft_struct_dump();

			//wait fft complete
			while(bk_fft_is_busy())
				;
			os_printf("fft complete\r\n");

			os_free(fft_conf.inbuf);

			//read output data
			bk_fft_output_read(data_proc_i, data_proc_q, 2 * FFT_TEST_DATA_SIZE);
			os_printf("\r\ndata_proc:\n");
			for (i = 0; i < FFT_TEST_DATA_SIZE; i++) {
				//os_printf("0x%04hx%04hx, ", data_proc_q[i], data_proc_i[i]);
				temp_q = ((int32)data_proc_q[i]) & 0x0000ffff;
				temp_i = ((int32)data_proc_i[i]) & 0x0000ffff;
				comp_q = (data_proc[i] >> 16) & 0x0000ffff;
				comp_i = data_proc[i] & 0x0000ffff;
				os_printf("\r\ntemp_q:0x%8x, temp_i:0x%8x, comp_q:0x%8x, comp_i:0x%8x\r\n", temp_q, temp_i, comp_q, comp_i);
				if ((temp_q != comp_q) || (temp_i != comp_i)) {
					os_printf("\r\nfft test fail!\r\n");
					ret = BK_FAIL;
					break;
				}
			}

			if (ret == BK_OK)
				os_printf("start fft test successful\r\n");
			else
				os_printf("start fft test failed\r\n");

		} else if (os_strcmp(argv[1], "stop") == 0) {
			os_printf("fft test stop\n");
			bk_fft_driver_deinit();
			os_printf("fft test stop successful\n");
		} else {
			cli_fft_help();
			return;
		}
	}

- IFFT test code

::

	void cli_fft_ifft_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
	{
		bk_err_t ret = BK_OK;
		uint32_t i = 0;
		int32 temp_q = 0, temp_i = 0;
		int32 comp_q = 0, comp_i = 0;
		fft_input_t fft_conf = {0};

		if (argc != 2) {
			cli_fft_help();
			return;
		}

		if (os_strcmp(argv[1], "start") == 0) {
			os_printf("ifft test start\n");

			//init fft driver
			ret = bk_fft_driver_init();
			if (ret != BK_OK)
				return;
			os_printf("ifft driver successful\r\n");

			fft_conf.inbuf = os_malloc(4*FFT_TEST_DATA_SIZE);
			os_memset(fft_conf.inbuf, 0, 4*FFT_TEST_DATA_SIZE);

			fft_conf.mode = FFT_WORK_MODE_IFFT;
			fft_conf.size = FFT_TEST_DATA_SIZE;
			os_printf("\r\nfft_data_source:\n");
			for (i = 0; i < FFT_TEST_DATA_SIZE; i++) {
				fft_conf.inbuf[i] = data_proc[i];
				//os_printf("0x%08x, ", fft_conf.inbuf[i]);
			}
			os_printf("\r\n");
			//start fft
			bk_fft_enable(&fft_conf);
			os_printf("start ifft process \r\n");

			//wait fft complete
			while(bk_fft_is_busy())
				;
			os_printf("ifft complete\r\n");

			os_free(fft_conf.inbuf);

			//read output data
			bk_fft_output_read(data_proc_i, data_proc_q, 2 * FFT_TEST_DATA_SIZE);
			os_printf("\r\ndata_proc:\n");
			for (i = 0; i < FFT_TEST_DATA_SIZE; i++) {
				//os_printf("0x%04hx%04hx, ", data_proc_q[i], data_proc_i[i]);
				temp_q = ((int32)data_proc_q[i]) & 0x0000ffff;
				temp_i = ((int32)data_proc_i[i]) & 0x0000ffff;
				comp_q = (data_comp[i] >> 16) & 0x0000ffff;
				comp_i = data_comp[i] & 0x0000ffff;
				//os_printf("\r\ntemp_q:0x%8x, temp_i:0x%8x, comp_q:0x%8x, comp_i:0x%8x\r\n", temp_q, temp_i, comp_q, comp_i);
				if ((temp_q != comp_q) || (temp_i != comp_i)) {
					os_printf("\r\nifft test fail!\r\n");
					ret = BK_FAIL;
					break;
				}
			}

			if (ret == BK_OK)
				os_printf("start ifft test successful\r\n");
			else
				os_printf("start ifft test failed\r\n");

		} else if (os_strcmp(argv[1], "stop") == 0) {
			os_printf("ifft test stop\n");
			bk_fft_driver_deinit();
			os_printf("ifft test stop successful\n");
		} else {
			cli_fft_help();
			return;
		}
	}


4 demo工作流程
""""""""""""""""""""""""""

demo运行的前提:
 - 1.提前准备好用于处理的数字信号

执行测试指令，case的工作流程如下图所示：

.. figure:: ../../../_static/fft_demo_flow.png
    :align: center
    :alt: fft_demo软件流程
    :figclass: align-center

    Figure 1. fft demo flow chart

5 FFT使用注意事项
""""""""""""""""""""""""""
.. important::
  注意事项:
   - 1.FFT和IFFT功能同一时间只能选择其中一个;
