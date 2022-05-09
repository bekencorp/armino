#ifndef _AT_SERVER_H_
#define _AT_SERVER_H_

#include "_at_svr_opts.h"
#include "atsvr_unite.h"

typedef enum{
	ATSVR_ECHO_NONE = 0,
	ATSVR_ECHO_NORMAL,
	ATSVR_ECHO_ALL,
}atsvr_echo_t;

typedef enum{
	ATSVR_WK_IDLE = 0,
	ATSVR_WK_DOING = 1,
	ATSVR_WK_DONE = 2,
}atsvr_work_st;

#if defined(ATSVR_OPTIM_FD_CMD) && ATSVR_OPTIM_FD_CMD
#define ATSVR_CMD_HADLER(name_st,help_str,func) {.name = name_st,.help = help_str,.function = func,.name_len = (sizeof(name_st) - 1)}
#else
#define ATSVR_CMD_HADLER(name_st,help_str,func) {.name = name_st,.help = help_str,.function = func}
#endif


#ifndef ATSVR_SIZEOF_OUTPUT_STRRING
#define ATSVR_SIZEOF_OUTPUT_STRRING(string)   atsvr_output_msg(string,ATSVR_SIZEOF_STR_LEN_NOEOF(string));
#endif

#ifndef ATSVR_OUTPUT_STRRING
#define ATSVR_OUTPUT_STRRING(string)   atsvr_output_msg(string,strlen(string));
#endif

extern char *get_atsvr_version(void);

extern void set_atsvr_echo_mode(atsvr_echo_t echo);
extern atsvr_echo_t get_atsvr_echo_mode(void);

extern int atsvr_register_command(const struct _atsvr_command *command);
extern int atsvr_register_commands(const struct _atsvr_command *commands,int num_commands);

extern void atsvr_notice_ready(void);
extern void atsvr_cmd_rsp_ok(void);
extern void atsvr_cmd_rsp_error(void);
extern void atsvr_cmd_analysis_notice_error(void);
extern void atsvr_input_msg_overflow(void);
extern void atsvr_output_msg(char *msg,unsigned int msg_len);
extern void atsvr_register_output_func(output_func_t output_func);
extern void atsvr_register_input_msg_func(input_msg_get_t input_msg_func);
extern void atsvr_register_resources_protection_func(resources_protection res_prot);
extern atsvr_work_st get_atsvr_work_state(void);
extern void set_atsvr_work_state(atsvr_work_st wk_st);
extern int atsvr_resource_protection_lock(void);
extern int atsvr_resource_protection_lock_timeout(unsigned int timeout);
extern void atsvr_resource_protection_unlock(void);

extern int atsvr_input_msg_analysis_handler(char *msg,unsigned int msg_len);
extern unsigned int atsvr_input_msg_get(char *data,unsigned int data_len);

extern void at_server_init(void);


extern void *at_malloc(unsigned int size);
extern void at_free(void *p);

#endif
