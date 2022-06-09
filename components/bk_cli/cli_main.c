/**
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 BEKEN Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of BEKEN Corporation.
 *
 */
#include <stdlib.h>
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "bk_cli.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include "bk_phy.h"
#include "cli.h"
#include "cli_config.h"
#include <components/log.h>
#include <driver/uart.h>
#include "bk_rtos_debug.h"
#if CONFIG_SHELL_ASYNCLOG
#include "shell_task.h"
#endif
#include "bk_uart_debug.h"
#include "bk_api_cli.h"

#define TAG "cli"

static struct cli_st *pCli = NULL;

#if (!CONFIG_SHELL_ASYNCLOG)
beken_semaphore_t log_rx_interrupt_sema = NULL;
#endif

static uint16_t s_running_command_index = MAX_COMMANDS;
#if CFG_CLI_DEBUG
static uint8_t s_running_status = 0;
#endif

extern int cli_putstr(const char *msg);
extern int hexstr2bin(const char *hex, u8 *buf, size_t len);

#if CONFIG_CAMERA
extern int video_demo_register_cmd(void);
#endif

#if CONFIG_BKREG
#define BKREG_MAGIC_WORD0                 (0x01)
#define BKREG_MAGIC_WORD1                 (0xE0)
#define BKREG_MAGIC_WORD2                 (0xFC)
#define BKREG_MIN_LEN                     3
#endif

#define SHELL_TASK_PRIORITY               1

/* Find the command 'name' in the cli commands table.
* If len is 0 then full match will be performed else upto len bytes.
* Returns: a pointer to the corresponding cli_command struct or NULL.
*/
const struct cli_command *lookup_command(char *name, int len)
{
	int i = 0;
	int n = 0;

	while (i < MAX_COMMANDS && n < pCli->num_commands) {
		if (pCli->commands[i]->name == NULL) {
			i++;
			continue;
		}

		/* See if partial or full match is expected */
		if (len != 0) {
			if (!os_strncmp(pCli->commands[i]->name, name, len)) {
				s_running_command_index = i;
				return pCli->commands[i];
			}
		} else {
			if (!os_strcmp(pCli->commands[i]->name, name)) {
				s_running_command_index = i;
				return pCli->commands[i];
			}
		}

		i++;
		n++;
	}

	return NULL;
}

int lookup_cmd_table(const struct cli_command *cmd_table, int table_items, char *name, int len)
{
	int i;

	for (i = 0; i < table_items; i++) 
	{
		if (cmd_table[i].name == NULL)
		{
			continue;
		}

		/* See if partial or full match is expected */
		if (len != 0)
		{
			if (!os_strncmp(cmd_table[i].name, name, len))
			{
				return i;
			}
		}
		else
		{
			if (!os_strcmp(cmd_table[i].name, name))
			{
				return i;
			}
		}
	}

	return -1;
}

#if (CONFIG_SHELL_ASYNCLOG && !CONFIG_ATE_TEST)

/* Parse input line and locate arguments (if any), keeping count of the number
* of arguments and their locations.  Look up and call the corresponding cli
* function if one is found and pass it the argv array.
*
* Returns: 0 on success: the input line contained at least a function name and
*          that function exists and was called.
*          1 on lookup failure: there is no corresponding function for the
*          input line.
*          2 on invalid syntax: the arguments list couldn't be parsed
*/
int handle_shell_input(char *inbuf, int in_buf_size, char * outbuf, int out_buf_size)
{
	struct {
		unsigned inArg: 1;
		unsigned inQuote: 1;
		unsigned done: 1;
		unsigned limQ : 1;
		unsigned isD : 2;
	} stat;
	static char *argv[16];
	int argc = 0;
	int i = 0;
	const struct cli_command *command = NULL;
	const char *p;

	os_memset((void *)&argv, 0, sizeof(argv));
	os_memset(&stat, 0, sizeof(stat));

	if(outbuf != NULL)
		os_memset(outbuf, 0, out_buf_size);

	if (inbuf[i] == '\0')
		return 0;

	do {
		switch (inbuf[i]) {
		case '\0':
			if (((argc == 0)||(stat.isD == 1))||(stat.limQ)||(stat.inQuote))
			{
				if(outbuf != NULL)
					strcpy(&outbuf[0], "syntax error\r\n");
				return 2;
			}
			stat.done = 1;
			break;

		case '"':
			if (i > 0 && inbuf[i - 1] == '\\' && stat.inArg) {
				os_memcpy(&inbuf[i - 1], &inbuf[i],
						  os_strlen(&inbuf[i]) + 1);
				--i;
				break;
			}
			if (!stat.inQuote && stat.inArg)
				break;
			if (stat.inQuote && !stat.inArg)
			{
				if(outbuf != NULL)
					strcpy(&outbuf[0], "syntax error\r\n");
				return 2;
			}

			if (!stat.inQuote && !stat.inArg) {
				stat.inArg = 1;
				stat.inQuote = 1;
				argc++;
				argv[argc - 1] = &inbuf[i + 1];
			} else if (stat.inQuote && stat.inArg) {
				stat.inArg = 0;
				stat.inQuote = 0;
				inbuf[i] = '\0';
			}
			break;

		case ' ':
			if (i > 0 && inbuf[i - 1] == '\\' && stat.inArg) {
				os_memcpy(&inbuf[i - 1], &inbuf[i],
						  os_strlen(&inbuf[i]) + 1);
				--i;
				break;
			}
			if (!stat.inQuote && stat.inArg) {
				stat.inArg = 0;
				inbuf[i] = '\0';
			}
			break;

        case '=':
            if(argc == 1) {
                inbuf[i] = '\0';
                stat.inArg = 0;
                stat.isD = 1;
            }
            else if(argc == 0){
				if(outbuf != NULL)
					strcpy(&outbuf[0], "syntax error\r\n");
                return 2;
            }
            break;

        case ',':
            if((stat.isD == 1)&&(argc == 1))  ///=,
            {
				if(outbuf != NULL)
					strcpy(&outbuf[0], "syntax error\r\n");
                return 2;
            }
            if(stat.inArg) {
                stat.inArg = 0;
                inbuf[i] = '\0';
                stat.limQ = 1;
            }
            break;

		default:
			if (!stat.inArg) {
				stat.inArg = 1;
				argc++;
				argv[argc - 1] = &inbuf[i];
                stat.limQ = 0;
                if(stat.isD == 1) {
                    stat.isD = 2;
                }
			}
			break;
		}
	} while (!stat.done && ++i < in_buf_size);

	if (stat.inQuote)
	{
		 if(outbuf != NULL)
		 	strcpy(&outbuf[0], "syntax error\r\n");
		return 2;
	}

	if (argc < 1)
	{
		if(outbuf != NULL)
		 	strcpy(&outbuf[0], "argc = 0\r\n");
		return 0;
	}

	/*
	* Some comamands can allow extensions like foo.a, foo.b and hence
	* compare commands before first dot.
	*/
	i = ((p = os_strchr(argv[0], '.')) == NULL) ? 0 :
		(p - argv[0]);
	command = lookup_command(argv[0], i);
	if (command == NULL)
	{
		if(outbuf != NULL)
			sprintf(&outbuf[0], "cmd: %s NOT found.\r\n", inbuf);
		return 1;
	}

#if CONFIG_STA_PS
	/*if cmd,exit dtim ps*/
	if (os_strncmp(command->name, "ps", 2)) {
	}
#endif

#if CFG_CLI_DEBUG
	s_running_status |= CLI_COMMAND_IS_RUNNING;
	command->function(outbuf, out_buf_size , argc, argv);
	s_running_status &= ~CLI_COMMAND_IS_RUNNING;
#else
	command->function(outbuf, out_buf_size , argc, argv);
#endif

	return 0;
}

#elif CONFIG_ATE_TEST
static beken_semaphore_t ate_test_semaphore = NULL;
static void ate_uart_rx_isr(uart_id_t id, void *param)
{
	int ret;

	ret = rtos_set_semaphore(&ate_test_semaphore);
	if(kNoErr !=ret)
		os_printf("ate_uart_rx_isr: ATE set sema failed\r\n");
}

static void cli_ate_main(uint32_t data)
{

	char *msg = NULL;
	int ret = -1;
	int cnt = 0;
	uint8_t rx_data;

        if(NULL == ate_test_semaphore)
    	{
              ret = rtos_init_semaphore(&ate_test_semaphore, 1);
        	if (kNoErr != ret)
            		os_printf("cli_ate_main: ATE create background sema failed\r\n");
    	}

	bk_uart_disable_sw_fifo(CONFIG_UART_PRINT_PORT);
	bk_uart_register_rx_isr(CONFIG_UART_PRINT_PORT, (uart_isr_t)ate_uart_rx_isr, NULL);
	bk_uart_enable_rx_interrupt(CONFIG_UART_PRINT_PORT);

	send_device_id();

	while (1) {

		ret = rtos_get_semaphore(&ate_test_semaphore, BEKEN_WAIT_FOREVER);
		if(kNoErr == ret)
        	{
			while(1)  /* read all data from rx-FIFO. */
			{
				ret = uart_read_byte_ex(CONFIG_UART_PRINT_PORT, &rx_data);
				if (ret == -1)
					break;

				pCli->inbuf[cnt] = (char)rx_data;
				cnt++;

				if(cnt >= INBUF_SIZE)
					break;
			}

			bkreg_run_command1(pCli->inbuf, cnt);

			if (cnt > 0) {
				for (int i = 0;i < cnt; i++)
					pCli->inbuf[i] = 0;
				cnt = 0;
			}
        	}

		msg = pCli->inbuf;
		if (os_strcmp(msg, EXIT_MSG) == 0)
				break;
	}

	os_printf("CLI exited\r\n");
	os_free(pCli);
	pCli = NULL;

	rtos_delete_thread(NULL);
}

#else

/* Parse input line and locate arguments (if any), keeping count of the number
* of arguments and their locations.  Look up and call the corresponding cli
* function if one is found and pass it the argv array.
*
* Returns: 0 on success: the input line contained at least a function name and
*          that function exists and was called.
*          1 on lookup failure: there is no corresponding function for the
*          input line.
*          2 on invalid syntax: the arguments list couldn't be parsed
*/
static int handle_input(char *inbuf)
{
	struct {
		unsigned inArg: 1;
		unsigned inQuote: 1;
		unsigned done: 1;
		unsigned limQ : 1;
		unsigned isD : 2;
	} stat;
	static char *argv[16];
	int argc = 0;
	int i = 0;
	const struct cli_command *command = NULL;
	const char *p;

	os_memset((void *)&argv, 0, sizeof(argv));
	os_memset(&stat, 0, sizeof(stat));

	if (inbuf[i] == '\0')
		return 0;

	do {
		switch (inbuf[i]) {
		case '\0':
			if (((argc == 0)||(stat.isD == 1))||(stat.limQ)||(stat.inQuote))
				return 2;
			stat.done = 1;
			break;

		case '"':
			if (i > 0 && inbuf[i - 1] == '\\' && stat.inArg) {
				os_memcpy(&inbuf[i - 1], &inbuf[i],
						  os_strlen(&inbuf[i]) + 1);
				--i;
				break;
			}
			if (!stat.inQuote && stat.inArg)
				break;
			if (stat.inQuote && !stat.inArg)
				return 2;

			if (!stat.inQuote && !stat.inArg) {
				stat.inArg = 1;
				stat.inQuote = 1;
				argc++;
				argv[argc - 1] = &inbuf[i + 1];
			} else if (stat.inQuote && stat.inArg) {
				stat.inArg = 0;
				stat.inQuote = 0;
				inbuf[i] = '\0';
			}
			break;

		case ' ':
			if (i > 0 && inbuf[i - 1] == '\\' && stat.inArg) {
				os_memcpy(&inbuf[i - 1], &inbuf[i],
						  os_strlen(&inbuf[i]) + 1);
				--i;
				break;
			}
			if (!stat.inQuote && stat.inArg) {
				stat.inArg = 0;
				inbuf[i] = '\0';
			}
			break;

        case '=':
            if(argc == 1) {
                inbuf[i] = '\0';
                stat.inArg = 0;
                stat.isD = 1;
            }
            else if(argc == 0){
                os_printf("The data does not conform to the regulations %d\r\n",__LINE__);
                return 2;
            }
            break;

        case ',':
            if((stat.isD == 1)&&(argc == 1))  ///=,
            {
                os_printf("The data does not conform to the regulations %d\r\n",__LINE__);
                return 2;
            }
            if(stat.inArg) {
                stat.inArg = 0;
                inbuf[i] = '\0';
                stat.limQ = 1;
            }
            break;

		default:
			if (!stat.inArg) {
				stat.inArg = 1;
				argc++;
				argv[argc - 1] = &inbuf[i];
                stat.limQ = 0;
                if(stat.isD == 1) {
                    stat.isD = 2;
                }
			}
			break;
		}
	} while (!stat.done && ++i < INBUF_SIZE);

	if (stat.inQuote)
		return 2;

	if (argc < 1)
		return 0;

	if (!pCli->echo_disabled)
		os_printf("\r\n");

	/*
	* Some comamands can allow extensions like foo.a, foo.b and hence
	* compare commands before first dot.
	*/
	i = ((p = os_strchr(argv[0], '.')) == NULL) ? 0 :
		(p - argv[0]);
	command = lookup_command(argv[0], i);
	if (command == NULL)
		return 1;

	os_memset(pCli->outbuf, 0, OUTBUF_SIZE);
	cli_putstr("\r\n");

#if CONFIG_STA_PS
	/*if cmd,exit dtim ps*/
	if (os_strncmp(command->name, "ps", 2)) {
	}
#endif

#if CFG_CLI_DEBUG
	s_running_status |= CLI_COMMAND_IS_RUNNING;
	command->function(pCli->outbuf, OUTBUF_SIZE, argc, argv);
	s_running_status &= ~CLI_COMMAND_IS_RUNNING;
#else
	command->function(pCli->outbuf, OUTBUF_SIZE, argc, argv);
#endif
	cli_putstr(pCli->outbuf);
	return 0;
}

/* Perform basic tab-completion on the input buffer by string-matching the
* current input line against the cli functions table.  The current input line
* is assumed to be NULL-terminated. */
static void tab_complete(char *inbuf, unsigned int *bp)
{
	int i, n, m;
	const char *fm = NULL;

	os_printf("\r\n");

	/* show matching commands */
	for (i = 0, n = 0, m = 0; i < MAX_COMMANDS && n < pCli->num_commands;
		 i++) {
		if (pCli->commands[i]->name != NULL) {
			if (!os_strncmp(inbuf, pCli->commands[i]->name, *bp)) {
				m++;
				if (m == 1)
					fm = pCli->commands[i]->name;
				else if (m == 2)
					os_printf("%s %s ", fm,
							  pCli->commands[i]->name);
				else
					os_printf("%s ",
							  pCli->commands[i]->name);
			}
			n++;
		}
	}

	/* there's only one match, so complete the line */
	if (m == 1 && fm) {
		n = os_strlen(fm) - *bp;
		if (*bp + n < INBUF_SIZE) {
			os_memcpy(inbuf + *bp, fm + *bp, n);
			*bp += n;
			inbuf[(*bp)++] = ' ';
			inbuf[*bp] = '\0';
		}
	}

	/* just redraw input line */
	os_printf("%s%s", PROMPT, inbuf);
}
/* Get an input line.
*
* Returns: 1 if there is input, 0 if the line should be ignored. */
static int get_input(char *inbuf, unsigned int *bp)
{
	if (inbuf == NULL) {
		os_printf("inbuf_null\r\n");
		return 0;
	}

	while (cli_getchar(&inbuf[*bp]) == 1) {
#if CONFIG_BKREG
		if ((0x01U == (UINT8)inbuf[*bp]) && (*bp == 0)) {
			(*bp)++;
			continue;
		} else if ((0xe0U == (UINT8)inbuf[*bp]) && (*bp == 1)) {
			(*bp)++;
			continue;
		} else if ((0xfcU == (UINT8)inbuf[*bp]) && (*bp == 2)) {
			(*bp)++;
			continue;
		} else {
			if ((0x01U == (UINT8)inbuf[0])
				&& (0xe0U == (UINT8)inbuf[1])
				&& (0xfcU == (UINT8)inbuf[2])
				&& (*bp == 3)) {
				uint8_t ch = inbuf[*bp];
				uint8_t left = ch, len = 4 + (uint8_t)ch;
				inbuf[*bp] = ch;
				(*bp)++;

				if (ch >= INBUF_SIZE) {
					os_printf("Error: input buffer overflow\r\n");
					os_printf(PROMPT);
					*bp = 0;
					return 0;
				}

				while (left--) {
					if (0 == cli_getchar((char *)&ch))
						break;

					inbuf[*bp] = ch;
					(*bp)++;
				}

				bkreg_run_command(inbuf, len);
				os_memset(inbuf, 0, len);
				*bp = 0;
				continue;
			}
		}
#endif
		if (inbuf[*bp] == RET_CHAR)
			continue;
		if (inbuf[*bp] == END_CHAR) {   /* end of input line */
			inbuf[*bp] = '\0';
			*bp = 0;
			return 1;
		}

		if ((inbuf[*bp] == 0x08) || /* backspace */
			(inbuf[*bp] == 0x7f)) { /* DEL */
			if (*bp > 0) {
				(*bp)--;
				if (!pCli->echo_disabled)
					os_printf("%c %c", 0x08, 0x08);
			}
			continue;
		}

		if (inbuf[*bp] == '\t') {
			inbuf[*bp] = '\0';
			tab_complete(inbuf, bp);
			continue;
		}

		if (!pCli->echo_disabled)
			os_printf("%c", inbuf[*bp]);

		(*bp)++;
		if (*bp >= INBUF_SIZE) {
			os_printf("Error: input buffer overflow\r\n");
			os_printf(PROMPT);
			*bp = 0;
			return 0;
		}
	}

	return 0;
}

/* Print out a bad command string, including a hex
* representation of non-printable characters.
* Non-printable characters show as "\0xXX".
*/
static void print_bad_command(char *cmd_string)
{
	if (cmd_string != NULL) {
		char *c = cmd_string;
		os_printf("command '");
		while (*c != '\0') {
			if (is_print(*c))
				os_printf("%c", *c);
			else
				os_printf("\\0x%x", *c);
			++c;
		}
		os_printf("' not found\r\n");
	}
}

/* Main CLI processing thread
*
* Waits to receive a command buffer pointer from an input collector, and
* then processes.  Note that it must cleanup the buffer when done with it.
*
* Input collectors handle their own lexical analysis and must pass complete
* command lines to CLI.
*/
void icu_struct_dump(void);

static void cli_main(uint32_t data)
{

	char *msg = NULL;
	int ret;

#if CONFIG_RF_OTA_TEST
	demo_sta_app_init("CMW-AP", "12345678");
#endif /* CONFIG_RF_OTA_TEST*/

	bk_uart_enable_rx_interrupt(CONFIG_UART_PRINT_PORT);

	while (1) {

		if (get_input(pCli->inbuf, &pCli->bp)) {
			msg = pCli->inbuf;

			if (os_strcmp(msg, EXIT_MSG) == 0)
				break;

			ret = handle_input(msg);
			if (ret == 1)
				print_bad_command(msg);
			else if (ret == 2)
				os_printf("syntax error\r\n");

			os_printf(PROMPT);
		}
	}

	os_printf("CLI exited\r\n");
	os_free(pCli);
	pCli = NULL;

	rtos_delete_thread(NULL);
}

#endif // #if (!CONFIG_SHELL_ASYNCLOG)

void help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cli_sort_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

extern const struct cli_command * cli_debug_cmd_table(int *num);

void cli_debug_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i, tbl_num = 0;
	const struct cli_command *cmd_tbl;

	if (argc < 2)
	{
		if(pcWriteBuffer != NULL)
		 	strcpy(&pcWriteBuffer[0], "argc < 2\r\n");
		return;
	}

	cmd_tbl = cli_debug_cmd_table(&tbl_num);

	i = lookup_cmd_table(cmd_tbl, tbl_num, argv[1], 0);

	if (i < 0)
	{
		if(pcWriteBuffer != NULL)
			sprintf(&pcWriteBuffer[0], "cmd: %s NOT found.\r\n", argv[1]);
		return;
	}

	cmd_tbl[i].function(pcWriteBuffer, xWriteBufferLen , argc - 1, &argv[1]);

}

static void log_setting_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int echo_level;
	int level;
	int sync_lvl;
	
	if (argc > 1)
	{
		echo_level = strtoul(argv[1], NULL, 0);

#if (CONFIG_SHELL_ASYNCLOG)
		shell_echo_set(echo_level);
#else
		if(echo_level)
			echo_level = 0;
		else
			echo_level = 1;
		pCli->echo_disabled = echo_level;
#endif //#if (CONFIG_SHELL_ASYNCLOG)		
	}

#if (CONFIG_SHELL_ASYNCLOG)
	if (argc > 2)
	{
		level = strtoul(argv[2], NULL, 0);
		shell_set_log_level(level);
	}
	if (argc > 3)
	{
		sync_lvl = strtoul(argv[3], NULL, 0);
		bk_set_printf_sync(sync_lvl);
	}
#endif

#if (CONFIG_SHELL_ASYNCLOG)
	echo_level = shell_echo_get();
	level = shell_get_log_level();
	sync_lvl = bk_get_printf_sync();
	
	sprintf(pcWriteBuffer, "log: echo %d, level %d, sync %d.\r\n", echo_level, level, sync_lvl);
#else

	sprintf(pcWriteBuffer,"log: echo %d.\r\n", pCli->echo_disabled ? 0 : 1);
#endif //#if (CONFIG_SHELL_ASYNCLOG)

	return;

	(void)level;
	(void)sync_lvl;

}

#if (CONFIG_SHELL_ASYNCLOG && CONFIG_MASTER_CORE)
#if 0
#define CPU1_CMD_BUF_SIZE    128
static char  cpu1_cmd_buf[CPU1_CMD_BUF_SIZE];
void cli_cpu1_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int 		i;
	int 		buf_len = 0;
	int  		str_len = 0;

	for(i = 1; i < argc; i++)
	{
		str_len = strlen(argv[i]);
		if ((buf_len + str_len + 3) < CPU1_CMD_BUF_SIZE) 
		{
			os_memcpy(&cpu1_cmd_buf[buf_len], argv[i], str_len);
			buf_len += str_len;
			cpu1_cmd_buf[buf_len++] = ' ';
		}
		else
			break;
	}

	cpu1_cmd_buf[buf_len++] = '\r';
	cpu1_cmd_buf[buf_len++] = '\n';

	shell_cmd_forward(cpu1_cmd_buf, buf_len);
}
#endif

/* it is a new implementation of the cli_cpu1_command, combine cpu1 cmd & paramters into argv[0] buffer. */
void cli_cpu1_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int 		i, j;
	int 		buf_len = 0;
	int  		str_len = 0;

	for(i = 1; i < argc; i++)
	{
		str_len = strlen(argv[i]);
		for(j = 0; j < str_len; j++)
		{
			if(argv[i][j] == ' ')
			{
				break;
			}
		}

		if(j < str_len) /* contains ' ' in string. */
		{
			argv[0][buf_len++] = '"';
			for(j = 0; j < str_len; j++)
			{
				if(argv[i][j] == '"')
				{
					argv[0][buf_len++] = '\\';
				}
				argv[0][buf_len++] = argv[i][j];
			}
			argv[0][buf_len++] = '"';
		}
		else
		{
			memcpy(&argv[0][buf_len], argv[i], str_len);
			buf_len += str_len;
			argv[0][buf_len++] = ' ';
		}
	}

	argv[0][buf_len++] = '\r';
	argv[0][buf_len++] = '\n';

	shell_cmd_forward(argv[0], buf_len);
}

#endif

#if (CONFIG_SHELL_ASYNCLOG)
void cli_log_statist(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int		data_cnt, i, buf_len = 0;
	u32		log_statist[5];
	data_cnt = shell_get_log_statist(log_statist, ARRAY_SIZE(log_statist));

	if(data_cnt == 0)
	{
		strcpy(pcWriteBuffer, "\r\n Nothing \r\n");
		return;
	}

	buf_len  = sprintf(&pcWriteBuffer[0], "\r\nlog overflow: %d.", log_statist[0]);
	buf_len += sprintf(&pcWriteBuffer[buf_len], "\r\nlog out count: %d.", log_statist[1]);

	for(i = 2; i < data_cnt; i++)
	{
		buf_len += sprintf(&pcWriteBuffer[buf_len], "\r\nBuffer[%d] run out count: %d.", i - 2, log_statist[i]);
	}

	return;

}

static void cli_log_disable(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) 
	{
		int 	i = 0, buf_len = 0;
		char *	mod_name;

		buf_len = sprintf(&pcWriteBuffer[0], "Usage: modlog tag_name on/off\r\n");

		while(1)
		{
			mod_name = bk_get_disable_mod(&i);

			if(mod_name != NULL)
			{
				if(buf_len == 0)
				{
					buf_len = sprintf(&pcWriteBuffer[0], "disabled mod list:\r\n%s\r\n", mod_name);
				}
				else
				{
					buf_len += sprintf(&pcWriteBuffer[buf_len], "%s\r\n", mod_name);
				}
			}
			else
				break;
		}
		
		return;
	}

	if (!os_strcasecmp(argv[2], "on")) 
	{
		bk_disable_mod_printf(argv[1], 0);
	} 
	else if (!os_strcasecmp(argv[2], "off")) 
	{
		bk_disable_mod_printf(argv[1], 1);
	}
}
#endif

#if CONFIG_BKREG
#define BKCMD_RXSENS_R      'r'
#define BKCMD_RXSENS_X      'x'
#define BKCMD_RXSENS_s      's'

#define BKCMD_TXEVM_T       't'
#define BKCMD_TXEVM_X       'x'
#define BKCMD_TXEVM_E       'e'

void bkreg_cmd_handle_input(char *inbuf, int len)
{
	if (((char)BKREG_MAGIC_WORD0 == inbuf[0])
		&& ((char)BKREG_MAGIC_WORD1 == inbuf[1])
		&& ((char)BKREG_MAGIC_WORD2 == inbuf[2])) {
		if (cli_getchars(inbuf, len)) {
			bkreg_run_command(inbuf, len);
			os_memset(inbuf, 0, len);
		}
	} else if ((((char)BKCMD_RXSENS_R == inbuf[0])
				&& ((char)BKCMD_RXSENS_X == inbuf[1])
				&& ((char)BKCMD_RXSENS_s == inbuf[2]))
			   || (((char)BKCMD_TXEVM_T == inbuf[0])
				   && ((char)BKCMD_TXEVM_X == inbuf[1])
				   && ((char)BKCMD_TXEVM_E == inbuf[2]))) {
		if (cli_getchars(inbuf, len)) {
#if (CONFIG_SHELL_ASYNCLOG)
			handle_shell_input(inbuf, len, 0, 0);
#else //#if (CONFIG_SHELL_ASYNCLOG)
			handle_input(inbuf);
#endif // #if (CONFIG_SHELL_ASYNCLOG)
			os_memset(inbuf, 0, len);
		}
	}

}
#endif

static const struct cli_command built_ins[] = {
	{"help", NULL, help_command},
	{"log", "log [echo(0,1)] [level(0~5)] [sync(0,1)]", log_setting_cmd},
//	{"sort", NULL, cli_sort_command},
	{"debug", "debug cmd [param] (ex:debug help)", cli_debug_command},
#if (CONFIG_SHELL_ASYNCLOG && CONFIG_MASTER_CORE)
	{"cpu1", "cpu1 cmd (ex:cpu1 help)", cli_cpu1_command},
#endif
#if (CONFIG_SHELL_ASYNCLOG)
	{"loginfo", "log statistics.", cli_log_statist},
	{"modlog", "modlog tag_name on/off", cli_log_disable},
#endif
};

static int _cli_name_cmp(const void *a, const void *b)
{
	struct cli_command *cli0, *cli1;

	cli0 = *(struct cli_command **)a;
	cli1 = *(struct cli_command **)b;

	if ((NULL == a) || (NULL == b))
		return 0;

	return os_strcmp(cli0->name, cli1->name);
}

void cli_sort_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t build_in_count;
	GLOBAL_INT_DECLARATION();

	build_in_count = sizeof(built_ins) / sizeof(struct cli_command);

	os_printf("cmd_count:%d, built_in_count:%d\r\n", pCli->num_commands, build_in_count);

	GLOBAL_INT_DISABLE();
	qsort(&pCli->commands[build_in_count], pCli->num_commands - build_in_count, sizeof(struct cli_command *), _cli_name_cmp);
	GLOBAL_INT_RESTORE();
}

/* Built-in "help" command: prints all registered commands and their help
* text string, if any. */
void help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i, n;
	uint32_t build_in_count = sizeof(built_ins) / sizeof(struct cli_command);

#if (DEBUG)
	build_in_count++; //For command: micodebug
#endif

	os_printf("====Build-in Commands====\r\n");
	for (i = 0, n = 0; i < MAX_COMMANDS && n < pCli->num_commands; i++) {
		if (pCli->commands[i]->name) {
			if (pCli->commands[i]->help)
				os_printf("%s: %s\r\n", pCli->commands[i]->name,
						  pCli->commands[i]->help ?
						  pCli->commands[i]->help : "");
			else
				os_printf("%s\r\n", pCli->commands[i]->name);

			n++;
			if (n == build_in_count)
				os_printf("\r\n====User Commands====\r\n");
		}
	}
}

int cli_register_command(const struct cli_command *command)
{
	int i;
	if (!command->name || !command->function)
		return 1;

	if (pCli->num_commands < MAX_COMMANDS) {
		/* Check if the command has already been registered.
		* Return 0, if it has been registered.
		*/
		for (i = 0; i < pCli->num_commands; i++) {
			if (pCli->commands[i] == command)
				return 0;
		}
		pCli->commands[pCli->num_commands++] = command;
		return 0;
	}

	return 1;
}

int cli_unregister_command(const struct cli_command *command)
{
	int i;
	if (!command->name || !command->function)
		return 1;

	for (i = 0; i < pCli->num_commands; i++) {
		if (pCli->commands[i] == command) {
			pCli->num_commands--;
			int remaining_cmds = pCli->num_commands - i;
			if (remaining_cmds > 0) {
				os_memmove(&pCli->commands[i], &pCli->commands[i + 1],
						   (remaining_cmds *
							sizeof(struct cli_command *)));
			}
			pCli->commands[pCli->num_commands] = NULL;
			return 0;
		}
	}

	return 1;
}


int cli_register_commands(const struct cli_command *commands, int num_commands)
{
	int i;
	for (i = 0; i < num_commands; i++)
		if (cli_register_command(commands++))
			return 1;
	return 0;
}

int cli_unregister_commands(const struct cli_command *commands,
							int num_commands)
{
	int i;
	for (i = 0; i < num_commands; i++)
		if (cli_unregister_command(commands++))
			return 1;

	return 0;
}

/* ========= CLI input&output APIs ============ */
int cli_printf(const char *msg, ...)
{
	va_list ap;
	char *pos, message[256];
	int sz;
	int nMessageLen = 0;

	os_memset(message, 0, 256);
	pos = message;

	sz = 0;
	va_start(ap, msg);
	nMessageLen = vsnprintf(pos, 256 - sz, msg, ap);
	va_end(ap);

	if (nMessageLen <= 0) return 0;

	cli_putstr((const char *)message);
	return 0;
}

int cli_putstr(const char *msg)
{
	if (msg[0] != 0)
		uart_write_string(CLI_UART, msg);
	return 0;
}

int cli_getchar(char *inbuf)
{
	if (bk_uart_read_bytes(CLI_UART, inbuf, 1, CLI_GETCHAR_TIMEOUT) > 0)
		return 1;
	else
		return 0;
}

int cli_getchars(char *inbuf, int len)
{
	if (bk_uart_read_bytes(CLI_UART, inbuf, len, CLI_GETCHAR_TIMEOUT) > 0)
		return 1;
	else
		return 0;
}

int cli_getchars_prefetch(char *inbuf, int len)
{
	return 0;
}

int cli_get_all_chars_len(void)
{
	return uart_get_length_in_buffer(CLI_UART);
}

static const struct cli_command user_clis[] = {
};

beken_thread_t cli_thread_handle = NULL;
int bk_cli_init(void)
{
	int ret;

	pCli = (struct cli_st *)os_malloc(sizeof(struct cli_st));
	if (pCli == NULL)
		return kNoMemoryErr;

	os_memset((void *)pCli, 0, sizeof(struct cli_st));

	if (cli_register_commands(&built_ins[0],
							  sizeof(built_ins) / sizeof(struct cli_command)))
		goto init_general_err;

	if (cli_register_commands(user_clis, sizeof(user_clis) / sizeof(struct cli_command)))
		goto init_general_err;

#if (CLI_CFG_WIFI == 1)
	cli_wifi_init();
#endif

#if (CLI_CFG_NETIF == 1)
	cli_netif_init();
#endif

#if (CLI_CFG_BLE == 1)
	cli_ble_init();
#endif

#if (CLI_CFG_MISC == 1)
	cli_misc_init();
#endif

#if (CLI_CFG_MEM == 1)
	cli_mem_init();
#endif

#if (CLI_CFG_AIRKISS == 1)
	cli_airkiss_init();
#endif

#if (CLI_CFG_PHY == 1)
	cli_phy_init();
#endif

#if (CLI_CFG_IPERF == 1)
	cli_phy_init();
#endif

#if (CLI_CFG_TIMER == 1)
	cli_timer_init();
#endif

#if (CLI_CFG_WDT == 1)
	cli_wdt_init();
#endif

#if (CLI_CFG_TRNG == 1)
	cli_trng_init();
#endif

#if (CLI_CFG_TRNG == 1)
	cli_efuse_init();
#endif

#if (CLI_CFG_DMA == 1)
	cli_dma_init();
#endif

#if (CLI_CFG_GPIO == 1)
	cli_gpio_init();
#endif

#if (CLI_CFG_OS == 1)
	cli_os_init();
#endif

#if (CLI_CFG_OTA == 1)
	cli_ota_init();
#endif

#if (CLI_CFG_FLASH == 1)
	cli_flash_init();
#endif

#if (CONFIG_FLASH_QUAD_ENABLE == 1)
	cli_flash_test_init();
#endif

#if (CLI_CFG_KEYVALUE == 1)
    cli_keyVaule_init();
#endif

#if (CLI_CFG_MATTER == 1)
    cli_matter_init();
#endif

#if (CLI_CFG_UART == 1)
	cli_uart_init();
#endif

#if (CLI_CFG_SPI == 1)
	cli_spi_init();
#endif

#if (CLI_CFG_QSPI == 1)
	cli_qspi_init();
#endif

#if (CLI_CFG_AON_RTC == 1)
	cli_aon_rtc_init();
#endif

#if (CLI_CFG_I2C == 1)
	cli_i2c_init();
#endif

#if (CLI_CFG_JPEGENC == 1)
	cli_jpeg_init();
#endif

#if (CLI_CFG_ADC == 1)
	cli_adc_init();
#endif

#if (CLI_CFG_SD == 1)
	cli_sd_init();
#endif

#if (CLI_FATFS == 1)
	cli_fatfs_init();
#endif

#if (CLI_CFG_TEMP_DETECT == 1)
	cli_temp_detect_init();
#endif

#if (CLI_CFG_SECURITY == 1)
	cli_security_init();
#endif

#if (CLI_CFG_MICO == 1)
	cli_mico_init();
#endif

#if (CLI_CFG_EVENT == 1)
	cli_event_init();
#endif

#if (CLI_CFG_PWR == 1)
	cli_pwr_init();
#endif

#if (CLI_CFG_REG == 1)
	cli_reg_init();
#endif

#if CONFIG_CAMERA
	if (video_demo_register_cmd())
		goto init_general_err;
#endif
#if (CONFIG_SOC_BK7271)
#if CONFIG_BT
	bk7271_ble_cli_init();
#endif
#if CONFIG_USB_HOST
	bk7271_dsp_cli_init();
#endif
#endif

#if (CONFIG_SOC_BK7256XX)
#if CONFIG_USB_HOST
	usb_cli_init();
#endif
#endif

#if (CLI_CFG_PWM == 1)
	cli_pwm_init();
#endif

#if (CLI_CFG_IPERF == 1)
	cli_iperf_init();
#endif

#if CONFIG_LWIP
	cli_lwip_init();
#endif

#if (CLI_CFG_EXCEPTION == 1)
	cli_exception_init();
#endif

#if (CLI_CFG_ICU == 1)
	cli_icu_init();
#endif

#if (CLI_CFG_VAULT == 1)
	cli_vault_init();
#endif

#if (CLI_CFG_AUD == 1)
	cli_aud_init();
#endif

#if (CLI_CFG_AUD_CP0 == 1)
	cli_aud_cp0_init();
#endif

#if (CLI_CFG_FFT == 1)
	cli_fft_init();
#endif

#if (CLI_CFG_SBC == 1)
	cli_sbc_init();
#endif

#if (CLI_CFG_I2S == 1)
	cli_i2s_init();
#endif

#if (CONFIG_TOUCH)
	cli_touch_init();
#endif

#if (CLI_CFG_LCD == 1)
	cli_lcd_init();
#endif

#if (CLI_CFG_DMA2D == 1)
	cli_dma2d_init();
#endif

#if (CLI_CFG_CALENDAR == 1)
	cli_calendar_init();
#endif

#if (CLI_CFG_UVC == 1)
	cli_uvc_init();
#endif

#if (CONFIG_AT_CMD)
    cli_at_init();
#endif

#if (CLI_CFG_JPEGDEC == 1)
	cli_jpegdec_init();
#endif

#if (CLI_CFG_AEC == 1)
	cli_aec_init();
#endif

#if (CLI_CFG_G711 == 1)
	cli_g711_init();
#endif

#if (CLI_CFG_DVP == 1)
	cli_dvp_init();
#endif

#if (CONFIG_DOORBELL == 1)
#if (CONFIG_DUAL_CORE)
	cli_doorbell_init();
#endif
#endif


	/* sort cmds after registered all cmds. */
	cli_sort_command(NULL, 0, 0, NULL);

#if CONFIG_SHELL_ASYNCLOG
#if CONFIG_ATE_TEST
	ret = rtos_create_thread(&cli_thread_handle,
							 SHELL_TASK_PRIORITY,
							 "cli",
							 (beken_thread_function_t)cli_ate_main /*cli_main*/,
							 4096,
							 0);
#else
	ret = rtos_create_thread(&cli_thread_handle,
							 SHELL_TASK_PRIORITY,
							 "cli",
							 (beken_thread_function_t)shell_task /*cli_main*/,
							 4096,
							 0);
#endif
#else // #if CONFIG_SHELL_ASYNCLOG
	ret = rtos_create_thread(&cli_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "cli",
							 (beken_thread_function_t)cli_main,
							 4096,
							 0);
#endif // #if CONFIG_SHELL_ASYNCLOG
	if (ret != kNoErr) {
		os_printf("Error: Failed to create cli thread: %d\r\n",
				  ret);
		goto init_general_err;
	}

	pCli->initialized = 1;
#if (!CONFIG_SHELL_ASYNCLOG)
	pCli->echo_disabled = 0;
#endif

	return kNoErr;

init_general_err:
	if (pCli) {
		os_free(pCli);
		pCli = NULL;
	}

	return kGeneralErr;
}

#if CFG_CLI_DEBUG
void cli_show_running_command(void)
{
	if (s_running_command_index < MAX_COMMANDS) {
		const struct cli_command *cmd = pCli->commands[s_running_command_index];

		CLI_LOGI("last cli command[%d]: %s(%s)\n", s_running_command_index, cmd->name,
				 (s_running_status & CLI_COMMAND_IS_RUNNING) ? "running" : "stopped");
		rtos_dump_task_list();
		rtos_dump_backtrace();
	} else
		CLI_LOGI("no command running\n");
}
#endif

// eof

