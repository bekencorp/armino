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
#include <os/str.h>
#include <stdio.h>
#include <stdlib.h>
#include "av_audio.h"

static void cli_av_audio_help(void)
{
	os_printf("av_audio_udp_test server|client\r\n");
	os_printf("av_audio_tcp_test server|client\r\n");
}

void cli_av_audio_udp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

	if (argc != 2) {
		cli_av_audio_help();
		return;
	}

	if (os_strcmp(argv[1], "server") == 0) {
		av_voice_udp_server_init();
		os_printf("server run \r\n");
	} else if (os_strcmp(argv[1], "client") == 0) {
		av_voice_udp_client_init();
		os_printf("client run \r\n");
	} else {
		cli_av_audio_help();
	}

	os_printf("test finish \r\n");
}

void cli_av_audio_tcp_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc != 2) {
		cli_av_audio_help();
		return;
	}

	if (os_strcmp(argv[1], "server") == 0) {
		av_voice_tcp_server_init();
		os_printf("server run \r\n");
	} else if (os_strcmp(argv[1], "client") == 0) {
		av_voice_tcp_client_init();
		os_printf("client run \r\n");
	} else {
		cli_av_audio_help();
	}

	os_printf("test finish \r\n");
}

