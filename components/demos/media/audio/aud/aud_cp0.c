// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/psram.h>
#include "ff.h"
#include "diskio.h"
#include "stdio.h"
#include "mailbox_channel.h"
#include "aud_demo.h"
#include <driver/aud.h>
#include <driver/aud_types.h>
#include <driver/dma.h>
#include "aud_hal.h"
#include "sys_driver.h"


extern void delay(int num);//TODO fix me

psram_block_status_t psram_block = {0};
//static uint32_t psram_block_id = 1;
static uint32_t adc_to_pcm_test_status = 0;
static uint32_t psram_block_to_sd_current = 0xFF;    //psram block that need to write sd
static uint32_t notify_stop_test = 0;
beken_thread_t aud_thread_handle = NULL;
beken_semaphore_t sdcard_play_sem = NULL;


static void cli_aud_cp0_help(void)
{
	os_printf("aud_cp0_adc_to_sd_test {start|stop} \r\n");
	os_printf("aud_cp0_psram_init {start|stop} \r\n");
	os_printf("aud_cp0_mic_to_pcm {start|stop} \r\n");
	os_printf("aud_cp0_sdcard_play_test {start} \r\n");
}

static void cp0_mailbox_tx_cmpl_isr(aud_mb_t *aud_mb, mb_chnl_ack_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_tx_cmpl \r\n");
	//os_printf("ack  hdr:0x%08x,data1:0x%08x,data2:0x%08x,state:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->ack_data1, cmd_buf->ack_data2, cmd_buf->ack_state);
}

static void cp0_mailbox_rx_isr(aud_mb_t *aud_mb, mb_chnl_cmd_t *cmd_buf)
{
	//os_printf("enter cp0_mailbox_rx_isr \r\n");
	//os_printf("cmd  hdr:0x%08x,pa1:0x%08x,pa2:0x%08x,pa3:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);

	if (cmd_buf->hdr.cmd == AUD_MB_DMA_DONE) {
		//os_printf("[mb_chnl_cmd_t] block_id:%x \r\n", cmd_buf->param1);
		psram_block.block_status |= 1<<(cmd_buf->param1);
	} else if (cmd_buf->hdr.cmd == AUD_MB_STOP_TEST) {
		//os_printf("[mb_chnl_cmd_t] stop test \r\n");
		adc_to_pcm_test_status = 0;
	} else {
		os_printf("do not process other cmd \r\n");
	}
	//check and set "psram_block_to_sd_current"
	if (psram_block.sd_status == IDLE)
		psram_block_to_sd_current = cmd_buf->param1;

}

static void cli_aud_dma_isr(dma_id_t param)
{
	rtos_set_semaphore(&sdcard_play_sem);
}

/*
static void cp0_mailbox_tx_isr(aud_mb_t *aud_mb)
{
	os_printf("enter cp0_mailbox_tx_isr \r\n");
//	os_printf("cmd  hdr:0x%08x,pa1:0x%08x,pa2:0x%08x,pa3:0x%08x\r\n", cmd_buf->hdr.data, cmd_buf->param1, cmd_buf->param2, cmd_buf->param3);
}
*/

void cli_aud_task_main(void)
{
	char file_name[] = "1:/aud_mic_data.pcm";
	FRESULT fr;
	FIL file;
	uint32 uiTemp = 0;
	mb_chnl_cmd_t mb_cmd;
	uint32_t test_flag = 0;
	uint32_t write_addr = 0;

	//init maibox
	//mb_chnl_init();
	//mb_chnl_open(MB_CHNL_AUD, NULL);
	//mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_RX_ISR, (void *)cp0_mailbox_rx_isr);
	//mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_TX_CMPL_ISR, (void *)cp0_mailbox_tx_cmpl_isr);
	//mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_TX_ISR, (void *)cp0_mailbox_tx_isr);

	//change test status to "1"
	adc_to_pcm_test_status = 1;

	//send mailbox msg to notify cpu1 that cpu0 write mic data to sd completely
	mb_cmd.hdr.cmd = AUD_MB_START_TEST;
	mb_cmd.param1 = 0;
	mb_cmd.param2 = 0;
	mb_cmd.param3 = 0;
	mb_chnl_write(MB_CHNL_AUD, &mb_cmd);

	os_printf("cp0: notify cpu1 to start test \r\n");

	while (1)
	{
		if ((notify_stop_test == 1) && (test_flag == 0)) {
			mb_cmd.hdr.cmd = AUD_MB_STOP_TEST;    //stop test
			mb_cmd.param1 = 0;
			mb_cmd.param2 = 0;
			mb_cmd.param3 = 0;
			mb_chnl_write(MB_CHNL_AUD, &mb_cmd);
			test_flag = 1;
			os_printf("CPU0->CPU1: stop test \r\n");
		}

		if (adc_to_pcm_test_status || psram_block.block_status != 0) {
			if (psram_block_to_sd_current != 0xFF)
			{
				//change sd_status to busing
				psram_block.sd_status = BUSING;

				/*open pcm file*/
				fr = f_open(&file, file_name, FA_OPEN_APPEND | FA_WRITE);
				if (fr != FR_OK) {
					os_printf("open %s fail.\r\n", file_name);
					return;
				}

				os_printf("write mic data to sd \r\n");
				write_addr = PSRAM_AUD_ADDR_BASE+(AUD_DMA_SIZE+1)*psram_block_to_sd_current;
				fr = f_write(&file, (void *)write_addr, AUD_DMA_SIZE + 1, &uiTemp);
				//fr = f_write(&file, (void *)PSRAM_AUD_ADDR_BASE, 10*(AUD_DMA_SIZE+1), &uiTemp);
				if (fr != FR_OK) {
					os_printf("write %s fail.\r\n", file_name);
					return;
				}
				os_printf("write done, block_id: %x, addr: %x \r\n", psram_block_to_sd_current, write_addr);

				fr = f_close(&file);
				if (fr != FR_OK) {
					os_printf("close %s fail!\r\n", file_name);
					return;
				}

				//change psram block to free "0"
				psram_block.block_status &= ~(1<<psram_block_to_sd_current);

				//send mailbox msg to notify cpu1 that cpu0 write mic data to sd completely
				mb_cmd.hdr.cmd = AUD_MB_WRITE_SD_DONE;
				mb_cmd.param1 = psram_block_to_sd_current;
				mb_cmd.param2 = 0;
				mb_cmd.param3 = 0;
				mb_chnl_write(MB_CHNL_AUD, &mb_cmd);

				//os_printf("psram_block.block_status: %x \r\n", psram_block.block_status);

				if (psram_block_to_sd_current == (psram_block_num-1))
					psram_block_to_sd_current = 0;
				else
					psram_block_to_sd_current +=1;

				if ((psram_block.block_status & 1<<psram_block_to_sd_current) != 0) {
					os_printf("cp0: write mic data to sd block_id: %x \r\n", psram_block_to_sd_current);
				} else {
					psram_block_to_sd_current = 0xFF;
					psram_block.sd_status = IDLE;
					psram_block.block_status = 0;
					os_printf("cp0: not have block_id to write sd \r\n");
				}
			}
			else
				delay(10);   // if (adc_to_pcm_test_status == 0 && psram_block.block_status = 0XFF)
		} else {
			os_printf("cp0: delete  aud task \r\n");
			rtos_delete_thread(&aud_thread_handle);
			delay(1000);
			mb_chnl_close(MB_CHNL_AUD);
		}
	}

}

void cli_aud_cp0_mic_to_pcm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	char file_name[] = "1:/aud_mic_data.pcm";
	FIL file;
	mb_chnl_cmd_t mb_cmd;
	FRESULT fr;

	if (argc != 2) {
		cli_aud_cp0_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("cp0: write audio mic data to file start\n");

		//init maibox
		//mb_chnl_init();
		mb_chnl_open(MB_CHNL_AUD, NULL);
		mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_RX_ISR, (void *)cp0_mailbox_rx_isr);
		mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_TX_CMPL_ISR, (void *)cp0_mailbox_tx_cmpl_isr);

		/*open pcm file*/
		fr = f_open(&file, file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", file_name);
			mb_cmd.hdr.cmd = AUD_MB_STOP_TEST;    //stop test
			mb_cmd.param1 = 0;
			mb_cmd.param2 = 0;
			mb_cmd.param3 = 0;
			mb_chnl_write(MB_CHNL_AUD, &mb_cmd);
			delay(1000);
			mb_chnl_close(MB_CHNL_AUD);
			os_printf("stop test \r\n", file_name);
			return;
		}

		//creat aud task to write mic data to sd
		ret = rtos_create_thread(&aud_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "cli",
							 (beken_thread_function_t)cli_aud_task_main,
							 4096,
							 0);
		if (ret != BK_OK)
			os_printf("cp0: create cli aud tash fail \r\n");

		os_printf("cp0: write audio mic data to file successful\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("cp0: audio mic data to file test stop\n");

		notify_stop_test = 1;

		os_printf("cp0: audio mic data to file test stop successful\n");
	} else {
		cli_aud_cp0_help();
		return;
	}

}

void cli_aud_cp0_adc_to_sd_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	//uint32_t i = 0;
	uint32_t psram = 0x60000000;

	char file_name[] = "1:/aud_mic_data1.pcm";
	FRESULT fr;
	FIL file;
	//int number = DISK_NUMBER_SDIO_SD;
	uint32 uiTemp = 0;

	if (argc != 2) {
		cli_aud_cp0_help();
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("cp0: write audio mic data to file start\n");

		/*open pcm file*/
		fr = f_open(&file, file_name, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", file_name);
			return;
		}

		os_printf("data:\n");
		//for (i = 0; i < 10; i++) {
			fr = f_write(&file, (void *)psram, (0xffff+1)*4, &uiTemp);
			if (fr != FR_OK) {
				os_printf("write %s fail.\r\n", file_name);
				return;
			}
			//psram = psram + 0xFFFF +1;
		//}
		os_printf("\n");

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", file_name);
			return;
		}

		os_printf("cp0: write audio mic data to file successful\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("cp0: audio mic data to file test stop\n");

		os_printf("cp0: audio mic data to file test stop successful\n");
	} else {
		cli_aud_cp0_help();
		return;
	}
}

void cli_aud_cp0_psram_init_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t mode = 0x00054043;
	bk_err_t ret = BK_OK;

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("cp0: init psram \n");

		ret = bk_psram_init(mode);
		if (ret != BK_OK) {
			os_printf("psram init error\n");
		}

		os_printf("cp0: init psram successful\r\n");
	} else if (os_strcmp(argv[1], "stop") == 0) {
		os_printf("cp0: deinit psram \n");
		bk_psram_deinit();
		os_printf("cp0: deinit psram successful\n");
	} else {
		cli_aud_cp0_help();
		return;
	}
}

void cli_aud_cp0_sdcard_to_dac_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	aud_dac_config_t dac_config;
	dma_config_t dma_config;
	uint32_t dac_fifo_addr;
	dma_id_t dma_id = DMA_ID_MAX;

	char file_name[50];
	FRESULT fr;
	FIL file;
	int number = DISK_NUMBER_SDIO_SD;
	uint32_t *data_buffer = os_malloc(0x10000);
	UINT uiTemp = 0;

	if (argc != 3) {
		cli_aud_cp0_help();
		return;
	}

	/*init sdcard play semaphore*/
	ret = rtos_init_semaphore(&sdcard_play_sem, 1);
	if (ret) {
		os_printf("create sdcard play semaphore fail.\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		os_printf("audio file to dac test start\n");

		dac_config.samp_rate = AUD_DAC_SAMP_RATE_SOURCE_48K;
		dac_config.dac_enable = AUD_DAC_DISABLE;
		dac_config.dac_hpf2_coef_B2 = 0x3A22;
		dac_config.dac_hpf2_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		dac_config.dac_hpf1_bypass_enable = AUD_DAC_HPF_BYPASS_ENABLE;
		dac_config.dac_set_gain = 0x15;
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

		ret = bk_aud_driver_init();
		ret = bk_aud_dac_init(&dac_config);
		os_printf("init audio driver and dac successful\n");

		//start dac
		ret = bk_aud_start_dac();
		os_printf("enable dac successful\n");

		dma_config.mode = DMA_WORK_MODE_REPEAT;
		dma_config.chan_prio = 1;
		dma_config.src.dev = DMA_DEV_DTCM;
		dma_config.src.width = DMA_DATA_WIDTH_32BITS;
		dma_config.dst.dev = DMA_DEV_AUDIO;
		dma_config.dst.width = DMA_DATA_WIDTH_32BITS;

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

		dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
		dma_config.src.start_addr = (uint32_t)data_buffer;
		dma_config.src.end_addr = (uint32_t)data_buffer + 0x10000;

		//init dma driver
		ret = bk_dma_driver_init();

		//init dma channel
		dma_id = bk_dma_alloc(DMA_DEV_AUDIO);
		if ((dma_id < DMA_ID_0) || (dma_id >= DMA_ID_MAX)) {
			os_printf("malloc dma fail \r\n");
			return;
		}
		ret = bk_dma_init(dma_id, &dma_config);

		bk_dma_register_isr(dma_id, NULL, cli_aud_dma_isr);
		bk_dma_enable_finish_interrupt(dma_id);
		bk_dma_set_transfer_len(dma_id, 0x10000);

		/*open pcm file*/
		os_memset(file_name, 0, sizeof(file_name));
		sprintf(file_name, "%d:/%s", number, argv[2]);
		fr = f_open(&file, file_name, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", file_name);
			return;
		}

		while(1)
		{
			fr = f_read(&file, data_buffer, 0x10000, &uiTemp);
			if (fr != FR_OK) {
				os_printf("read file fail.\r\n");
				break;
			} else {
				ret = bk_dma_start(dma_id);
				bk_aud_start_dac();
				if (ret != BK_OK)
					return;

				ret = rtos_get_semaphore(&sdcard_play_sem, 5000);
				if (ret) {
					os_printf("get sdcard play semaphore fail.\r\n");
				}
			}

			if(uiTemp == 0) {
				bk_aud_stop_dac();
				bk_aud_driver_deinit();
				bk_dma_stop(dma_id);
				bk_dma_driver_deinit();
				ret = bk_dma_free(DMA_DEV_AUDIO, dma_id);
				if (ret == BK_OK)
					os_printf("free dma: %d success\r\n", dma_id);
				rtos_deinit_semaphore(&sdcard_play_sem);
				break;
			}
		}

		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", file_name);
			return;
		}
	}
}

void cli_aud_cp0_signal_to_dual_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char signal_file_name[] = "1:/signal_data.pcm";
	char dual_file_name[] = "1:/dual_data.pcm";
	FIL file_signal;
	FIL file_dual;
	int16_t data = 0;
	int16_t data_temp[2] = {0};
	FRESULT fr;
	uint32 uiTemp = 0;
	uint32_t file_size = 0;
	uint32_t i = 0;

	fr = f_open(&file_signal, signal_file_name, FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", signal_file_name);
		return;
	}

	fr = f_open(&file_dual, dual_file_name, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", dual_file_name);
		return;
	}

	file_size = f_size(&file_signal)/2;
	os_printf("file_size = %d \r\n", file_size);
	for (i=0; i<file_size; i++) {
		fr = f_read(&file_signal, &data, 2, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			break;
		}
		data_temp[0] = data;
		data_temp[1] = data;
		//os_printf("decoder_temp = %d \r\n", decoder_temp);
		fr = f_write(&file_dual, (void *)data_temp, 4, &uiTemp);
		if (fr != FR_OK) {
			os_printf("write output data %s fail.\r\n", dual_file_name);
			break;
		}
	}

	fr = f_close(&file_signal);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", signal_file_name);
		return;
	}
	fr = f_close(&file_dual);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", dual_file_name);
		return;
	}

	os_printf("test finish \r\n");
}

