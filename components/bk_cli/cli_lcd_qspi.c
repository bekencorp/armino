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

extern void cli_lcd_qspi_display_pure_color_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_lcd_qspi_display_picture_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#define LCD_QSPI_CMD_CNT	(sizeof(s_lcd_qspi_commands) / sizeof(struct cli_command))

static const struct cli_command s_lcd_qspi_commands[] = {
	{"lcd_qspi_display_pure_color_test", "lcd_qspi_display_pure_color_test {start|stop} {device name}", cli_lcd_qspi_display_pure_color_test_cmd},
	{"lcd_qspi_display_picture_test", "lcd_qspi_display_picture_test {start|stop} {file name} {device name}", cli_lcd_qspi_display_picture_test_cmd},
};

int cli_lcd_qspi_init(void)
{
	return cli_register_commands(s_lcd_qspi_commands, LCD_QSPI_CMD_CNT);
}

