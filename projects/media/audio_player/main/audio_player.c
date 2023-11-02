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

#include "os/os.h"
#include "audio_player.h"
#include "audio_codec.h"
#include "netstream_player.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>


#define TAG  "audio_player"

extern const char *song_list[];
extern uint32_t current_song, song_number;

bk_err_t audio_player_start(const char *uri)
{
	bk_err_t ret = BK_OK;

	netstream_player_msg_t msg;
	msg.op = NETSTREAM_PLAYER_START;
	msg.netstream_uri = uri;
	ret = netstream_player_send_msg(msg);
	if (ret != kNoErr) {
		BK_LOGI(TAG, "netstream player send msg: %d fail\r\n", msg.op);
		return ret;
	}

	BK_LOGI(TAG, "audio player start complete\r\n");

	return ret;
}

bk_err_t audio_player_stop(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_spk_stop();
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio intf spk stop fail, ret:%d \r\n", ret);
		return ret;
	}

	BK_LOGI(TAG, "audio intf spk stop complete\r\n");

	ret = bk_aud_intf_spk_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio intf drvier deinit fail, ret:%d \r\n", ret);
		return ret;
	}

	BK_LOGI(TAG, "audio player stop complete\r\n");

	return ret;
}

bk_err_t audio_player_pause(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_spk_pause();
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio player pause fail, ret:%d\r\n", ret);
	}

	BK_LOGI(TAG, "audio player pause complete\r\n");

	return ret;
}

bk_err_t audio_player_play(void)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_spk_start();
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio player play fail, ret:%d \r\n", ret);
		return ret;
	}

	BK_LOGI(TAG, "audio player play complete\r\n");

	return ret;
}

bk_err_t audio_player_set_volume(uint8_t vol_value)
{
	bk_err_t ret = BK_OK;

	ret = bk_aud_intf_set_spk_gain(vol_value);
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio player volume set fail, ret:%d \r\n", ret);
	}

	BK_LOGI(TAG, "audio player volume set complete\r\n");

	return ret;
}

bk_err_t audio_player_last(void)
{
	bk_err_t ret = BK_OK;

	if ((current_song < 1) || (current_song > song_number)) {
		BK_LOGE(TAG, "The index of current song starts at 1!\r\n");
		return BK_FAIL;
	} else {
		if (current_song > 1) {
			current_song--;
		} else {
			current_song = song_number;
		}
	}

	netstream_player_stop();

	ret = audio_player_stop();
	if (ret != BK_OK) {
		BK_LOGE(TAG, "audio player stop fail, ret:%d \r\n", ret);
		return ret;
	}

	audio_codec_mp3_decoder_stop();

	netstream_player_msg_t download_msg;
	download_msg.op = NETSTREAM_PLAYER_START;
	download_msg.netstream_uri = song_list[current_song - 1];
	ret = netstream_player_send_msg(download_msg);
	if (ret != kNoErr) {
		BK_LOGE(TAG, "netstream player send msg: %d fail\r\n", download_msg.op);
		return ret;
	}

	return ret;
}

bk_err_t audio_player_next(void)
{
	bk_err_t ret = BK_OK;

	if ((current_song < 1) || (current_song > song_number)) {
		BK_LOGE(TAG, "The index of current song starts at 1!\r\n");
		return BK_FAIL;
	} else {
		if (current_song < song_number) {
			current_song++;
		} else {
			current_song = 1;
		}
	}

	netstream_player_stop();

	ret = audio_player_stop();
	if (ret != BK_OK) {
		BK_LOGE(TAG, "audio player stop fail, ret:%d \r\n", ret);
		return ret;
	}

	audio_codec_mp3_decoder_stop();

	netstream_player_msg_t download_msg;
	download_msg.op = NETSTREAM_PLAYER_START;
	download_msg.netstream_uri = song_list[current_song - 1];
	ret = netstream_player_send_msg(download_msg);
	if (ret != kNoErr) {
		BK_LOGE(TAG, "netstream player send msg: %d fail\r\n", download_msg.op);
		return ret;
	}

	return ret;
}

bk_err_t audio_player_init(void)
{
	bk_err_t ret = BK_OK;

	aud_intf_drv_setup_t aud_intf_drv_setup = DEFAULT_AUD_INTF_DRV_SETUP_CONFIG();
	aud_intf_work_mode_t aud_work_mode = AUD_INTF_WORK_MODE_NULL;

	aud_intf_drv_setup.aud_intf_rx_spk_data = audio_codec_mp3_decoder_handler;
	ret = bk_aud_intf_drv_init(&aud_intf_drv_setup);
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio intf drvier init fail, ret:%d\r\n", ret);
		return ret;
	}

	aud_work_mode = AUD_INTF_WORK_MODE_GENERAL;
	ret = bk_aud_intf_set_mode(aud_work_mode);
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "audio intf mode set fail, ret:%d \r\n", ret);
		return ret;
	}

	BK_LOGI(TAG, "audio player init complete\r\n");

	return ret;
}

bk_err_t audio_player_deinit(void)
{
	bk_err_t ret = BK_OK;

	ret = netstream_player_deinit();
	if (ret != BK_OK) {
		BK_LOGE(TAG, "netstream player deinit fail, ret:%d \r\n", ret);
		return ret;
	}

	ret = audio_player_stop();
	if (ret != BK_OK) {
		BK_LOGE(TAG, "audio player stop fail, ret:%d \r\n", ret);
		return ret;
	}

	audio_codec_mp3_decoder_stop();

	ret = bk_aud_intf_drv_deinit();
	if (ret != BK_ERR_AUD_INTF_OK) {
		BK_LOGE(TAG, "bk_aud_intf_drv_deinit fail, ret:%d \r\n", ret);
		return ret;
	}

	BK_LOGI(TAG, "audio player deinit complete\r\n");

	return ret;
}

