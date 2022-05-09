#ifndef __AT_SERVER_H_
#define __AT_SERVER_H_

#include "_at_svr_opts.h"
#include "atsvr_unite.h"

typedef enum{
	ATSVR_OK = 0,
	ATSVR_SEVERE_ERR = -2,
	ATSVR_ERROR = -1,

	ATSVR_GENERAL = 1,
	ATSVR_NnMemoryErr = 2,

}_atsvr_st;

typedef enum{
	_ATSVR_ECHO_NONE = 0,
	_ATSVR_ECHO_NORMAL,
	_ATSVR_ECHO_ALL,
}_atsvr_echo_t;

typedef enum{
	_ATSVR_WK_IDLE = 0,
	_ATSVR_WK_DOING = 1,
	_ATSVR_WK_DONE = 2,
}_atsvr_work_st;

#if 0
/* Structure for registering at server commands */
struct _atsvr_command{
    const char *name;
    const char *help;

    void (*function)(int argc, char **argv);
};

typedef void (*output_func_t)(char *msg,unsigned int msg_len);
typedef int (*resources_protection)(int is_lock,unsigned int timeout);
typedef unsigned int (*input_msg_get_t)(char *data,unsigned int dat_len);

#endif

typedef struct{
	_atsvr_echo_t echo;
	_atsvr_work_st wk_st;
	resources_protection res_prot;

	const struct _atsvr_command *commands[ATSVR_MAX_COMMANDS];
	unsigned int num_commands;

	output_func_t output_func;
	input_msg_get_t input_msg_func;
}_at_svr_ctrl_env_t;

typedef _at_svr_ctrl_env_t _atsvr_env_t;

#ifndef _ATSVR_SIZEOF_OUTPUT_STRING
#define _ATSVR_SIZEOF_OUTPUT_STRING(env,string)   _atsvr_output_msg(env,string,ATSVR_SIZEOF_STR_LEN_NOEOF(string));
#endif

#ifndef _ATSVR_OUTPUT_STRING
#define _ATSVR_OUTPUT_STRING(env,string)   _atsvr_output_msg(env,string,strlen(string));
#endif


extern char *_get_atsvr_version(void);

extern void _set_atsvr_echo_mode(_atsvr_env_t *env,_atsvr_echo_t echo);
extern _atsvr_echo_t _get_atsvr_echo_mode(_atsvr_env_t *env);

extern int _atsvr_register_command(_atsvr_env_t *env,const struct _atsvr_command *command);
extern int _atsvr_unregister_command(_atsvr_env_t *env,const struct _atsvr_command *command);
extern int _atsvr_register_commands(_atsvr_env_t *env,const struct _atsvr_command *commands,int num_commands);
extern int _atsvr_unregister_commands(_atsvr_env_t *env,const struct _atsvr_command *commands,int num_commands);

extern int _atsvr_input_msg_analysis_handler(_atsvr_env_t *env,char *msg,unsigned int msg_len);

extern void _atsvr_notice_ready(_atsvr_env_t *env);
extern void _atsvr_cmd_analysis_notice_error(_atsvr_env_t *env);
extern void _atsvr_cmd_rsp_ok(_atsvr_env_t *env);
extern void _atsvr_cmd_rsp_error(_atsvr_env_t *env);
extern void _atsvr_output_msg(_atsvr_env_t *env,char *msg,unsigned int msg_len);

extern void _atsvr_register_output_func(_atsvr_env_t *env,output_func_t output_func);
///Register the resource critical protection function
extern void _atsvr_register_resources_protection_func(_atsvr_env_t *env,resources_protection res_prot);
extern int _atsvr_resource_protection_lock(_atsvr_env_t *env);
extern int _atsvr_resource_protection_lock_timeout(_atsvr_env_t *env,unsigned int timeout);
extern void _atsvr_resource_protection_unlock(_atsvr_env_t *env);
extern void _atsvr_register_input_msg_func(_atsvr_env_t *env,input_msg_get_t input_msg_func);
extern unsigned int _atsvr_input_msg_get(_atsvr_env_t *env,char *data,unsigned int data_len);
extern _atsvr_work_st _get_atsvr_work_state(_atsvr_env_t *env);
extern void _set_atsvr_work_state(_atsvr_env_t *env,_atsvr_work_st wk_st);
extern int _atsvr_def_config(_atsvr_env_t *env);

#if defined(ATSVR_OPTIM_FD_CMD) && ATSVR_OPTIM_FD_CMD
#define _ATSVR_CMD_HADLER(name_st,help_st,func) {.name = name_st,.help = help_st,.function = func,.name_len = (sizeof(name_st) - 1)}
#else
#define _ATSVR_CMD_HADLER(name_st,help_st,func) {.name = name_st,.help = help_st,.function = func}
#endif


#endif
