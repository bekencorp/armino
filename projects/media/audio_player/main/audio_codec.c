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
#include <modules/mp3dec.h>
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>
#include "audio_player.h"
#include "audio_codec.h"
#include "netstream_player.h"


#define TAG  "audio_codec"

#define PCM_SIZE_MAX		(MAX_NSAMP * MAX_NCHAN * MAX_NGRAN)

HMP3Decoder hMP3Decoder;
MP3FrameInfo mp3FrameInfo;
unsigned char *readBuf;
short *pcmBuf;
int bytesLeft = 0;
int offset = 0;
unsigned char *g_readptr;
static unsigned char *psram_ptr = NULL;
static uint8_t aud_player_spk_init_flag = 0;
extern uint32_t psram_buffer;
extern unsigned char *psram_end_addr;


static const uint16_t AUDIO_SAMPLE_RATES[] = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};

static bk_err_t audio_codec_mp3_decoder_init(void)
{
	readBuf = os_malloc(MAINBUF_SIZE);
	if (readBuf == NULL) {
		os_printf("readBuf malloc failed!\r\n");
		return BK_FAIL;
	}

	pcmBuf = os_malloc(PCM_SIZE_MAX * 2);
	if (pcmBuf == NULL) {
		os_printf("pcmBuf malloc failed!\r\n");
		return BK_FAIL;
	}

	hMP3Decoder = MP3InitDecoder();
	if (hMP3Decoder == 0) {
		os_free(readBuf);
		os_free(pcmBuf);
		os_printf("MP3Decoder init failed!\r\n");
		return BK_FAIL;
	}

	g_readptr = readBuf;
	psram_ptr = (unsigned char *)psram_buffer;

	return BK_OK;
}

static void audio_codec_mp3_decoder_deinit(void)
{
	bytesLeft = 0;
	offset = 0;
	aud_player_spk_init_flag = 0;

	MP3FreeDecoder(hMP3Decoder);
	os_free(readBuf);
	os_free(pcmBuf);
}

static void audio_codec_mp3_find_id3(void)
{
	char tag_header[10];
	int tag_size = 0;

	os_memcpy((void *)tag_header, (const void *)psram_buffer, 10);

	if (os_memcmp(tag_header, "ID3", 3) == 0) {
		tag_size = ((tag_header[6] & 0x7F) << 21) | ((tag_header[7] & 0x7F) << 14) | ((tag_header[8] & 0x7F) << 7) | (tag_header[9] & 0x7F);
		BK_LOGI(TAG, "tag_size = %d\r\n", tag_size);
		psram_ptr += tag_size + 10;
	} else {
		BK_LOGI(TAG, "tag_header not found!\r\n");
	}
}

bk_err_t audio_codec_mp3_decoder_handler(unsigned int size)
{
	bk_err_t ret = BK_OK;
	uint8_t idx = 0;

	if (psram_ptr >= psram_end_addr) {
		audio_codec_mp3_decoder_stop();

		netstream_player_msg_t msg;
		msg.op = NETSTREAM_PLAYER_STOP;
		msg.netstream_uri = NULL;
		ret = netstream_player_send_msg(msg);
		if (ret != kNoErr) {
			BK_LOGE(TAG, "netstream_download send msg: %d fail\r\n", msg.op);
			return ret;
		}

		return BK_OK;
	}

	if (bytesLeft < MAINBUF_SIZE) {
		os_memmove(readBuf, g_readptr, bytesLeft);
		os_memcpy((void *)(readBuf + bytesLeft), (const void *)psram_ptr, MAINBUF_SIZE - bytesLeft);

		psram_ptr += MAINBUF_SIZE - bytesLeft;
		bytesLeft = MAINBUF_SIZE;
		g_readptr = readBuf;
	}

	offset = MP3FindSyncWord(g_readptr, bytesLeft);
	if (offset < 0) {
		BK_LOGI(TAG, "MP3FindSyncWord not find!\r\n");
		bytesLeft = 0;
	} else {
		g_readptr += offset;
		bytesLeft -= offset;

		ret = MP3Decode(hMP3Decoder, &g_readptr, &bytesLeft, pcmBuf, 0);
		if (ret != ERR_MP3_NONE) {
			BK_LOGE(TAG, "MP3Decode failed, code is %d", ret);
			return ret;
		}

		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);

		/* write a frame speaker data to speaker_ring_buff */
		ret = bk_aud_intf_write_spk_data((uint8_t*)pcmBuf, mp3FrameInfo.outputSamps * 2);
		if (ret != BK_OK) {
			BK_LOGE(TAG, "write spk data fail \r\n");
			return ret;
		}

		if (!aud_player_spk_init_flag) {
			aud_intf_spk_setup_t aud_intf_spk_setup = DEFAULT_AUD_INTF_SPK_SETUP_CONFIG();

			for (idx = 0; idx < sizeof(AUDIO_SAMPLE_RATES) / sizeof(AUDIO_SAMPLE_RATES[0]); idx++) {
				if (AUDIO_SAMPLE_RATES[idx] == mp3FrameInfo.samprate) {
					aud_intf_spk_setup.samp_rate = idx;
					break;
				}
			}
			aud_intf_spk_setup.spk_chl = AUD_INTF_SPK_CHL_DUAL;
			aud_intf_spk_setup.frame_size = mp3FrameInfo.outputSamps * 2;
			aud_intf_spk_setup.work_mode = AUD_DAC_WORK_MODE_DIFFEN;
			ret = bk_aud_intf_spk_init(&aud_intf_spk_setup);
			if (ret != BK_ERR_AUD_INTF_OK) {
				BK_LOGE(TAG, "audio player spk init fail, ret:%d \r\n", ret);
				return ret;
			}

			aud_player_spk_init_flag = 1;
		}
	}

	return ret;

}

bk_err_t audio_codec_mp3_decoder_start(void)
{
	bk_err_t ret = BK_OK;

	ret = audio_codec_mp3_decoder_init();
	if (ret != BK_OK) {
		BK_LOGE(TAG, "audio codec mp3 decoder init fail\r\n");
		return ret;
	}

	BK_LOGI(TAG, "audio codec mp3 decoder init complete\r\n");

	audio_codec_mp3_find_id3();

	ret = audio_codec_mp3_decoder_handler(0);
	if (ret != BK_OK) {
		BK_LOGE(TAG, "audio player decode fail\r\n");
		return ret;
	}

	return ret;
}

bk_err_t audio_codec_mp3_decoder_stop(void)
{
	audio_codec_mp3_decoder_deinit();

	return BK_OK;
}


