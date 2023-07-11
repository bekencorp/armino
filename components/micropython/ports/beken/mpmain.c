/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright 2020-2023 Beken
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "py/mpconfig.h"
#include "py/mphal.h"
#include "py/gc.h"
#include "py/parse.h"
#include "py/lexer.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "extmod/vfs.h"
#include "shared/runtime/pyexec.h"
#include "modmachine.h"

#include "components/system.h"
#include "driver/gpio.h"
#include "cli.h"

#define MP_TASK_STACK_SIZE      (16 * 1024)
#define MP_SD_CARD_GPIO         (6)

typedef struct {
	// task context
	beken_thread_t msg_task;
	beken_queue_t  msg_queue;
	uint32_t       flag_running : 1;
	uint32_t       flag_mounted : 1;
	/* gc heap */
	uint8_t  *heap_ptr;
	uint32_t  heap_len;

	// python context
	const char   *file;
	const char   *script;
} mp_context;

typedef enum {
	MP_CMD_RUN_REPL = 0,
	MP_CMD_RUN_FILE,
	MP_CMD_RUN_SCRIPT,
	MP_CMD_EXIT,
} mp_cmd;

typedef struct {
	int   cmd;
	void *ptr;
} mp_msg;

static mp_context g_mp;

static void cli_mp_help(void) {
	CLI_LOGI("mp [-g gcSIZE=16KB] [-h] [-e] [-f file]|[-s script]\n");
	CLI_LOGI("-e        - exit python\n");
	CLI_LOGI("-f file   - run python file, UNIMPLEMENT\n");
	CLI_LOGI("-g gcSIZE - gc stack size\n");
	CLI_LOGI("-h        - show this usage\n");
	CLI_LOGI("-s script - run python script\n");
#if MICROPY_DEBUG_PRINTERS
	CLI_LOGI("-v [0,2] - verbose level\n");
#endif
	CLI_LOGI("default - startup python task if needed, run repl and exit with 'CTRL+D'\n");
}

int mp_do_mount_detect(mp_context *mp) {
	bool flag_inserted;
	mp_obj_t bdev;
	mp_obj_t mount_point;

	flag_inserted = !bk_gpio_get_input(MP_SD_CARD_GPIO);

	if (mp->flag_mounted && !flag_inserted) {
        // Try to unmount the flash on "/sd"
        mount_point = MP_OBJ_NEW_QSTR(MP_QSTR__slash_sd);
        mp->flag_mounted = 0;

        DEBUG_printf("unmount(/sd)\r\n");
        mp_vfs_umount(mount_point);

        return 0;
	} else if (!mp->flag_mounted && flag_inserted) {
        bdev = MP_OBJ_TYPE_GET_SLOT(&machine_sdcard_type, make_new)(&machine_sdcard_type, 0, 0, NULL);
        // Try to mount the flash on "/sd" and chdir to it for the boot-up directory.
        mount_point = MP_OBJ_NEW_QSTR(MP_QSTR__slash_sd);

        DEBUG_printf("mount(/sd)\r\n");
        mp->flag_mounted = 1;
        return mp_vfs_mount_and_chdir_protected(bdev, mount_point);
	}

    return 0;
}

#define FORCED_EXIT (0x100)
// If exc is SystemExit, return value where FORCED_EXIT bit set,
// and lower 8 bits are SystemExit value. For all other exceptions,
// return 1.
STATIC int handle_uncaught_exception(mp_obj_base_t *exc) {
    // check for SystemExit
    if (mp_obj_is_subclass_fast(MP_OBJ_FROM_PTR(exc->type), MP_OBJ_FROM_PTR(&mp_type_SystemExit))) {
        // None is an exit value of 0; an int is its value; anything else is 1
        mp_obj_t exit_val = mp_obj_exception_get_value(MP_OBJ_FROM_PTR(exc));
        mp_int_t val = 0;
        if (exit_val != mp_const_none && !mp_obj_get_int_maybe(exit_val, &val)) {
            val = 1;
        }
        return FORCED_EXIT | (val & 255);
    }

    // Report all other exceptions
    mp_obj_print_exception(MICROPY_ERROR_PRINTER, MP_OBJ_FROM_PTR(exc));
    return 1;
}

#define LEX_SRC_STR (1)
#define LEX_SRC_VSTR (2)
#define LEX_SRC_FILENAME (3)
#define LEX_SRC_STDIN (4)

// Returns standard error codes: 0 for success, 1 for all other errors,
// except if FORCED_EXIT bit is set then script raised SystemExit and the
// value of the exit is in the lower 8 bits of the return value
int mp_do_exec(int source_kind, const void *source, mp_parse_input_kind_t input_kind, bool is_repl) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        // create lexer based on source kind
        mp_lexer_t *lex;
        if (source_kind == LEX_SRC_STR) {
            const char *line = source;
            lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, line, strlen(line), false);
        } else if (source_kind == LEX_SRC_VSTR) {
            const vstr_t *vstr = source;
            lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, vstr->buf, vstr->len, false);
        } else if (source_kind == LEX_SRC_FILENAME) {
            lex = mp_lexer_new_from_file((const char *)source);
        } else { // LEX_SRC_STDIN
#if MICROPY_HELPER_LEXER_UNIX
            lex = mp_lexer_new_from_fd(MP_QSTR__lt_stdin_gt_, 0, false);
#else
            nlr_pop();
            return 0;
#endif
        }

        qstr source_name = lex->source_name;

        #if MICROPY_PY___FILE__
        if (input_kind == MP_PARSE_FILE_INPUT) {
            mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        }
        #endif

        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);

        #if defined(MICROPY_UNIX_COVERAGE)
        // allow to print the parse tree in the coverage build
        if (mp_verbose_flag >= 3) {
            printf("----------------\n");
            mp_parse_node_print(&mp_plat_print, parse_tree.root, 0);
            printf("----------------\n");
        }
        #endif

        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, is_repl);

        // execute it
        mp_call_function_0(module_fun);

        mp_hal_set_interrupt_char(-1);
        mp_handle_pending(true);
        nlr_pop();
        return 0;

    } else {
        // uncaught exception
        mp_hal_set_interrupt_char(-1);
        mp_handle_pending(false);
        return handle_uncaught_exception(nlr.ret_val);
    }

    return 0;
}

int mp_do_repl(void) {
	int result;

	// use MicroPython supplied readline
	mp_hal_uart_hook(1);

	nlr_buf_t nlr;
	if (nlr_push(&nlr) == 0) {
		for (;;) {
			result = pyexec_friendly_repl();
			if (result != 0) {
				 break;
			}
		}
		nlr_pop();
	}

	mp_hal_uart_hook(0);
	return 0;
}

static void mp_task(void *param) {
	mp_context *mp = (mp_context *)param;
    volatile uint32_t sp;
	mp_msg msg;
	bk_err_t result;

#if MICROPY_PY_THREAD
    mp_thread_init(NULL, mp->heap_len);
#endif
    mp_stack_set_top((void *)&sp);
    mp_stack_set_limit((mp_uint_t)mp->heap_len);

#if MICROPY_ENABLE_GC
	mp->heap_ptr = os_malloc(mp->heap_len);
	if (NULL == mp->heap_ptr) {
		return;
	}
    gc_init(mp->heap_ptr, mp->heap_ptr + mp->heap_len);
#endif
    mp_init();
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_path), 0);
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_sd));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_sd_slash_lib));

    mp_do_mount_detect(mp);

	while (mp->flag_running) {
		result = rtos_pop_from_queue(&mp->msg_queue, &msg, BEKEN_WAIT_FOREVER);
		if (BK_OK != result) {
			continue;
		}
		switch (msg.cmd) {
		case MP_CMD_RUN_REPL:
			mp_do_repl();
			break;
		case MP_CMD_RUN_FILE:
			mp_do_exec(LEX_SRC_FILENAME, msg.ptr, MP_PARSE_FILE_INPUT, false);
			free(msg.ptr);
			break;
		case MP_CMD_RUN_SCRIPT:
			mp_do_exec(LEX_SRC_STR, msg.ptr, MP_PARSE_EVAL_INPUT, false);
			free(msg.ptr);
			break;
		case MP_CMD_EXIT:
			mp->flag_running = 0;
			break;
		}
	}

	rtos_delete_thread(NULL);
}

static int mp_do_initialize(mp_context *mp) {
    // initialise the stack pointer for the main thread
	bk_err_t result;
	gpio_config_t config = { 0 };

	if (NULL != mp->msg_task) {
		return BK_OK;
	}

	config.io_mode = GPIO_INPUT_ENABLE;
	config.pull_mode = GPIO_PULL_UP_EN;
	config.func_mode = GPIO_SECOND_FUNC_DISABLE;
	bk_gpio_set_config(MP_SD_CARD_GPIO, &config);
	mp_hal_init();
	result = rtos_init_queue(&mp->msg_queue, "mp_queue", sizeof(mp_msg), 4);
	if (BK_OK != result) {
		CLI_LOGI("ceate Q failed(%d)\r\n", result);
		return result;
	}

	mp->flag_running = 1;
	result = rtos_create_thread(&mp->msg_task,
		5,
		"mp_task",
		(beken_thread_function_t)mp_task,
		6 * 1024,
		(beken_thread_arg_t)mp);
	if (BK_OK != result) {
		rtos_deinit_queue(&mp->msg_queue);
		mp->msg_queue = NULL;
		mp->flag_running = 0;
		CLI_LOGI("create task failed(%d)\r\n", result);
		return result;
	}

	return result;
}

static int mp_do_finalize(mp_context *mp) {
	mp_msg msg;
	int retry_timeout = 100;

	if (NULL == mp->msg_task) {
		return BK_OK;
	}

	memset((void *)&msg, 0x00, sizeof(msg));
	msg.cmd = MP_CMD_EXIT;
	rtos_push_to_queue(&mp->msg_queue, &msg, BEKEN_NO_WAIT);
	while ((mp->flag_running) && (retry_timeout > 0)) {
		rtos_delay_milliseconds(10);
		retry_timeout--;
	}

	rtos_deinit_queue(&mp->msg_queue);
	mp->msg_queue = NULL;
	mp->msg_task = NULL;

	mp->heap_len = 0;
	if (NULL != mp->heap_ptr) {
		os_free(mp->heap_ptr);
		mp->heap_ptr = NULL;
	}
	mp_hal_exit();

	return 0;
}

static int mp_param_parse(int argc, char *const argv[], mp_context *mp)
{
	int    arg_cnt = argc;
	int    arg_id = 1;
	int    arg_used;

	while (arg_cnt > 1) {
		if ('-' != argv[arg_id][0]) {
			return -1;
		}

		arg_used = 2;
		switch (argv[arg_id][1]) {
		case 'e':
			mp_do_finalize(mp);
			return 0;
		case 'f':
			mp->file = argv[arg_id + 1];
			break;
		case 'g':
			mp->heap_len = strtoul(argv[arg_id + 1], NULL, 10) * 1024;
			break;
		case 's':
			mp->script = argv[arg_id + 1];
			break;
		case 'v':
#if MICROPY_DEBUG_PRINTERS
			mp_verbose_flag = strtoul(argv[arg_id + 1], NULL, 10);
#endif
			break;

		default:
			cli_mp_help();
			return 0;
		}

		arg_cnt -= arg_used;
		arg_id  += arg_used;
	}

	return 1;
}

int mp_do_startup(int heap_len) {
	g_mp.heap_len = heap_len;
	if (0 == g_mp.heap_len) {
		g_mp.heap_len = MP_TASK_STACK_SIZE;
	}

	return mp_do_initialize(&g_mp);
}

int mp_do_shutdown(void) {
	return mp_do_finalize(&g_mp);
}

static void cli_mp_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv) {
	mp_msg msg;
	int result;

	result = mp_param_parse(argc, argv, &g_mp);
	if (result <= 0) {
		return;
	}
	if (0 == g_mp.heap_len) {
		g_mp.heap_len = MP_TASK_STACK_SIZE;
	}
	mp_do_initialize(&g_mp);
	memset((void *)&msg, 0x00, sizeof(msg));
	if (NULL != g_mp.file) {
		msg.cmd = MP_CMD_RUN_FILE;
		msg.ptr = os_strdup(g_mp.file);
		g_mp.file = NULL;
		rtos_push_to_queue(&g_mp.msg_queue, &msg, BEKEN_NO_WAIT);
	} else if (NULL != g_mp.script) {
		msg.cmd = MP_CMD_RUN_SCRIPT;
		msg.ptr = os_strdup(g_mp.script);
		g_mp.script = NULL;
		rtos_push_to_queue(&g_mp.msg_queue, &msg, BEKEN_NO_WAIT);
	} else {
#if 1//MICROPY_PY_THREAD
		msg.cmd = MP_CMD_RUN_REPL;
		rtos_push_to_queue(&g_mp.msg_queue, &msg, BEKEN_NO_WAIT);
#else
		mp_do_repl();
#endif
	}
}

#define MP_CMD_CNT (sizeof(s_mp_commands) / sizeof(struct cli_command))
static const struct cli_command s_mp_commands[] = {
	{"mp", "mp [-f file]|[-s script]|[-r]", cli_mp_cmd},
};

int cli_mp_init(void) {
	//mp_do_startup(0);
	return cli_register_commands(s_mp_commands, MP_CMD_CNT);
}
