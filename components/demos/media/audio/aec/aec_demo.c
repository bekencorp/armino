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

#include <os/mem.h>
#include <os/os.h>
#include <stdio.h>
#include <stdlib.h>
#include "modules/aec.h"
#include "ff.h"
#include "diskio.h"

#define TxRxRScale     (16)

void cli_aec_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	//uint8_t buffer[sizeof(AECContext)];
	char mic_file_name[] = "1:/mic.pcm";
	char ref_file_name[] = "1:/ref.pcm";
	char out_file_name[] = "1:/out.pcm";
	FIL file_mic;
	FIL file_ref;
	FIL file_out;
	FRESULT fr;
	uint32 uiTemp = 0;
	FSIZE_t test_data_size = 0;

	int16_t* ref_addr = NULL;
	int16_t* mic_addr = NULL;
	int16_t* out_addr = NULL;

	AECContext* aec = NULL;
	uint32_t frame_sample = 0;
	uint32_t val = 0;
	int16_t samplerate = 16000;
	uint32_t aec_context_size = 0;

	fr = f_open(&file_mic, mic_file_name, FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", mic_file_name);
		return;
	}
	fr = f_open(&file_ref, ref_file_name, FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", ref_file_name);
		return;
	}
	fr = f_open(&file_out, out_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", out_file_name);
		return;
	}

	aec_context_size = aec_size();
	os_printf("sizeof(AECContext) = %d\n", aec_context_size);

	//初始化
	//需要buffer大约二十多kByte，具体值和AEC_MAX_MIC_DELAY有关, 宏设置需要大于一帧，建议400以上，并不小于实际延迟，默认写的2000
	aec = (AECContext*)os_malloc(aec_context_size);

	//采样率可以配置8000或者16000
	aec_init(aec, samplerate);

	//获取处理帧长，16000采样率320点(640字节)，8000采样率160点(320字节)  (对应20毫秒数据)
	aec_ctrl(aec, AEC_CTRL_CMD_GET_FRAME_SAMPLE, (uint32_t)(&frame_sample));

	//获取结构体内部可以复用的ram作为每帧tx,rx,out数据的临时buffer; ram很宽裕的话也可以在外部单独申请获取
	aec_ctrl(aec, AEC_CTRL_CMD_GET_TX_BUF, (uint32_t)(&val)); mic_addr = (int16_t*)val;
	aec_ctrl(aec, AEC_CTRL_CMD_GET_RX_BUF, (uint32_t)(&val)); ref_addr = (int16_t*)val;
	aec_ctrl(aec, AEC_CTRL_CMD_GET_OUT_BUF,(uint32_t)(&val)); out_addr = (int16_t*)val;

	//以下是参数调节示例,aec_init中都已经有默认值,可以直接先用默认值
	aec_ctrl(aec, AEC_CTRL_CMD_SET_FLAGS, 0x1f);							  //库内各模块开关; aec_init内默认赋值0x1f;

	///回声消除相关
	aec_ctrl(aec, AEC_CTRL_CMD_SET_MIC_DELAY, 10);							  //设置参考信号延迟(采样点数，需要dump数据观察)
	aec_ctrl(aec, AEC_CTRL_CMD_SET_EC_DEPTH, 5);							  //建议取值范围1~50; 后面几个参数建议先用aec_init内的默认值，具体需要根据实际情况调试; 总得来说回声越大需要调的越大
	aec_ctrl(aec, AEC_CTRL_CMD_SET_TxRxThr, 13);							  //建议取值范围10~64
	aec_ctrl(aec, AEC_CTRL_CMD_SET_TxRxFlr, 1); 							  //建议取值范围1~10
	aec_ctrl(aec, AEC_CTRL_CMD_SET_REF_SCALE, 0);							  //取值0,1,2；rx数据如果幅值太小的话适当放大
	aec_ctrl(aec, AEC_CTRL_CMD_SET_VOL, 14);								  //通话过程中如果需要经常调节喇叭音量就设置下当前音量等级
	///降噪相关
	aec_ctrl(aec, AEC_CTRL_CMD_SET_NS_LEVEL, 2);							  //建议取值范围1~8；值越小底噪越小
	aec_ctrl(aec, AEC_CTRL_CMD_SET_NS_PARA,  1);							  //只能取值0,1,2; 降噪由弱到强，建议默认值
	///drc(输出音量相关)
	aec_ctrl(aec, AEC_CTRL_CMD_SET_DRC, 0x15);								  //建议取值范围0x10~0x1f;   越大输出声音越大

	/////////////////////////
	{
		////	uint8_t para[10] = { 0x1f, 0x05, 0x02, 0x0d, 0x01, 0x08, 0x04, 0x0a, 0x02, 0x15 };
		////	aec_ctrl(aec, AEC_CTRL_CMD_SET_PARAMS, (uint32_t)para); 			//一次设置多个参数
	}
	/////////////////////////

	test_data_size = f_size(&file_mic);
	os_printf("frame_samples * 2 = %d \r\n", (frame_sample * 2));
	while(test_data_size >= (frame_sample * 2))
	{
		// 每帧处理前参考信号写入aec->rin，麦克风信号写入aec->sin
		// aec->rin，aec->sin，aec->out是三个指针，指向aec结构体内部可以复用的三段ram;
		// 可以把一帧数据直接写入rin和sin作为输入
		// 如果在外部设置了一帧数据的buffer，也可以把aec->rin，aec->sin，aec->out指向外部buffer
		// 每帧处理长度是 aec->frame_samples(采样点) * sizeof(int16)
		// 16k采样率frame_samples是320,  8k采样率frame_samples是160
		//fread(aec->rin, 1, aec->frame_samples * 2, frin);
		//fread(aec->sin, 1, aec->frame_samples * 2, fsin);
		fr = f_read(&file_ref, ref_addr, frame_sample * 2, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read ref file fail.\r\n");
			break;
		}

		fr = f_read(&file_mic, mic_addr, frame_sample * 2, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read mic file fail.\r\n");
			break;
		}

		aec_proc(aec, ref_addr, mic_addr, out_addr);

		//write output data to sd
		//fwrite(aec->out, 1, aec->frame_samples * 2, fout);
		fr = f_write(&file_out, (void *)out_addr, frame_sample * 2, &uiTemp);
		//fr = f_write(&file, (void *)PSRAM_AUD_ADDR_BASE, 10*(AUD_DMA_SIZE+1), &uiTemp);
		if (fr != FR_OK) {
			os_printf("write output data %s fail.\r\n", out_file_name);
			break;
		}

		//os_printf("frame %d \r\n",aec->frame_cnt);
		test_data_size -= frame_sample * 2;
	}

	os_free(aec);

	fr = f_close(&file_mic);
	if (fr != FR_OK) {
		os_printf("close mic file %s fail!\r\n", mic_file_name);
		return;
	}

	fr = f_close(&file_ref);
	if (fr != FR_OK) {
		os_printf("close ref file %s fail!\r\n", ref_file_name);
		return;
	}

	fr = f_close(&file_out);
	if (fr != FR_OK) {
		os_printf("close out file %s fail!\r\n", out_file_name);
		return;
	}

	os_printf("test finish \r\n");
}

