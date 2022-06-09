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

#include "cli.h"

extern void image_save_dvp(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define DVP_CMD_CNT (sizeof(s_dvp_commands) / sizeof(struct cli_command))
static const struct cli_command s_dvp_commands[] = {
	{"dvp", "dvp init/save_image file_id/deinit", image_save_dvp},
};

int cli_dvp_init(void)
{
	return cli_register_commands(s_dvp_commands, DVP_CMD_CNT);
}

