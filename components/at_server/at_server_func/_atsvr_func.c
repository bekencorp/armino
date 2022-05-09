#include "_atsvr_func.h"
#include "_at_server.h"
#include "_at_server_port.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


static void _atsvr_at_command(
#if ATSVR_HANDLER_ENV
		void* env,
#endif
		int argc, char **argv)
{
	_at_svr_ctrl_env_t *p_env = NULL;

#if ATSVR_HANDLER_ENV
	p_env = env;
#else
	if(argc < ATSVR_MAX_ARG) {
		p_env = (_at_svr_ctrl_env_t*)argv[argc];
	}
#endif
    if(argc != 1){
        _atsvr_cmd_rsp_error(p_env);
        return;
    }
    _atsvr_cmd_rsp_ok(p_env);
}

static void _atsvr_at_echo_command(
#if ATSVR_HANDLER_ENV
		void* env,
#endif
		int argc, char **argv)
{
	_at_svr_ctrl_env_t *p_env = NULL;

#if ATSVR_HANDLER_ENV
	p_env = env;
#else
	if(argc < ATSVR_MAX_ARG) {
		p_env = (_at_svr_ctrl_env_t*)argv[argc];
	}
#endif
	if(argc != 2){
		_atsvr_cmd_rsp_error(p_env);
		return;
	}
	int en = atoi(argv[1]);
	if(en == 0){
		_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_NONE);
	}else if(en == 1){
		_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_NORMAL);
	}else if(en == 2){
		_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_ALL);
	}else{
		if(strcmp(argv[1],"en") == 0){
			_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_NORMAL);
		}else if(strcmp(argv[1],"EN") == 0){
			_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_NORMAL);
		}else if(strcmp(argv[1],"dis") == 0){
			_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_NONE);
		}else if(strcmp(argv[1],"DIS") == 0){
			_set_atsvr_echo_mode(p_env,_ATSVR_ECHO_NONE);
		}else{
			_atsvr_cmd_rsp_error(p_env);
			return;	
		}
	}
	_atsvr_cmd_rsp_ok(p_env);
}

#if ATSVR_CMD_HELP
static void _atsvr_help_command(
#if ATSVR_HANDLER_ENV
	void* env,
#endif
	int argc, char **argv);
#endif

static void _atsvr_at_verion(
#if ATSVR_HANDLER_ENV
		void* env,
#endif
		int argc, char **argv)
{
	_at_svr_ctrl_env_t *p_env = NULL;
#if ATSVR_HANDLER_ENV
	p_env = env;
#else
	if(argc < ATSVR_MAX_ARG) {
		p_env = (_at_svr_ctrl_env_t*)argv[argc];
	}
#endif
    if(argc != 1){
    	_atsvr_cmd_rsp_error(p_env);
        return;
    }
    char output[100];
    int n;

    n = snprintf(output,100,"CMDRSP:ATVERSION:%s\r\n",ATSVR_VERSION_NUM);
    n += snprintf(output+n,100 - n,ATSVR_CMD_RSP_SUCCEED);
	_atsvr_output_msg(p_env,output,n);

    return;
}


const struct _atsvr_command _atsvc_cmds_table[] = {
	_ATSVR_CMD_HADLER("AT","AT",_atsvr_at_command),
#if defined(ATSVR_CMD_HELP) && ATSVR_CMD_HELP
	_ATSVR_CMD_HADLER("AT+HELP","AT+HELP",_atsvr_help_command),
#endif
	_ATSVR_CMD_HADLER("AT+VERSION","AT+VERSION",_atsvr_at_verion),
	_ATSVR_CMD_HADLER("AT+ECHO","AT+ECHO=0\\1\\2",_atsvr_at_echo_command),
	
};

#if ATSVR_CMD_HELP
static void _atsvr_help_command(
#if ATSVR_HANDLER_ENV
	void* env,
#endif
	int argc, char **argv)
{
	unsigned int i;
	int n,len;
	char *resultbuf;
	_at_svr_ctrl_env_t *p_env = NULL;

#if ATSVR_HANDLER_ENV
	p_env = env;
#else
	if(argc < ATSVR_MAX_ARG) {
		p_env = (_at_svr_ctrl_env_t*)argv[argc];
	}
#endif
	resultbuf = at_malloc(ATSVR_CMD_HELP_BUF_SIZE);
	if(resultbuf) {
		len = 0;
		if(p_env != NULL){
			for (i = 0, n = 0; i < ATSVR_MAX_COMMANDS && n < p_env->num_commands; i++){
				if (p_env->commands[i]->name) {
					len += snprintf(resultbuf+len,ATSVR_CMD_HELP_BUF_SIZE - len,
						"CMDRSP:cmd:%s,help:%s\r\n", p_env->commands[i]->name,
						p_env->commands[i]->help ? p_env->commands[i]->help : "");
					n++;
				}
			}
			len += snprintf(resultbuf+len,ATSVR_CMD_HELP_BUF_SIZE - len,"%s",ATSVR_CMD_RSP_SUCCEED);
		}else {
			len += snprintf(resultbuf+len,ATSVR_CMD_HELP_BUF_SIZE - len,"%s",ATSVR_CMD_RSP_ERROR);
		}
		if(p_env && p_env->output_func) {
			p_env->output_func(resultbuf,len);
		}
		at_free(resultbuf);
	}else {
		_atsvr_cmd_rsp_error(p_env);
	}
}
#endif


void _atsvr_def_cmd_init(_atsvr_env_t *env)
{
	_atsvr_register_commands(env,_atsvc_cmds_table, sizeof(_atsvc_cmds_table) / sizeof(struct _atsvr_command));
}

void _atsvr_def_cmd_deinit(_atsvr_env_t *env)
{
	_atsvr_unregister_commands(env,_atsvc_cmds_table, sizeof(_atsvc_cmds_table) / sizeof(struct _atsvr_command));
}

