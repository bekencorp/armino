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

#include "audio_transfer_driver.h"
#include <common/bk_include.h>


/* AEC control */
/* set aec ec_depth parameter */
bk_err_t bk_aud_tras_aec_ec_depth_set(uint8_t value)
{
	audio_tras_ctrl_t ctrl_msg;

	ctrl_msg.op = AUD_TRAS_SET_EC_DEPTH;
	ctrl_msg.param = value;
	return audio_transfer_send_msg(AUD_TRAS_DRV_CONTROL, &ctrl_msg);
}

