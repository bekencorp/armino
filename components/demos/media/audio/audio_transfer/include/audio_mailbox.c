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
#include "audio_mailbox.h"


void aud_mailbox_init(void *rx_isr, void *tx_isr, void *tx_cmpl_isr)
{
	//init maibox
	//mb_chnl_init();
	mb_chnl_open(MB_CHNL_AUD, NULL);
	if (rx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_RX_ISR, rx_isr);
	if (tx_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_TX_ISR, tx_isr);
	if (tx_cmpl_isr != NULL)
		mb_chnl_ctrl(MB_CHNL_AUD, MB_CHNL_SET_TX_CMPL_ISR, tx_cmpl_isr);
}

void aud_mailbox_deinit(void)
{
	mb_chnl_close(MB_CHNL_AUD);
}

bk_err_t aud_mailbox_send_msg(audio_mailbox_msg_t *msg)
{
	mb_chnl_cmd_t mb_cmd;

	//send mailbox msg to notify cpu1 that cpu0 write mic data to sd completely
	mb_cmd.hdr.cmd = msg->mb_cmd;
	mb_cmd.param1 = msg->param1;
	mb_cmd.param2 = msg->param2;
	mb_cmd.param3 = msg->param3;
	return mb_chnl_write(MB_CHNL_AUD, &mb_cmd);
}

