#include "_at_server.h"
#include "_atsvr_func.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"


static const char *_atsvr_version_num = ATSVR_VERSION_NUM;


char *_get_atsvr_version(void)
{
	return (char*)_atsvr_version_num;
}


void _set_atsvr_echo_mode(_atsvr_env_t *env,_atsvr_echo_t echo)
{
	if(env){
		env->echo = echo;
	}
}

_atsvr_echo_t _get_atsvr_echo_mode(_atsvr_env_t *env)
{
	return env->echo;
}

int _atsvr_register_command(_atsvr_env_t *env,const struct _atsvr_command *command)
{
	int i;

	if (!command->name || !command->function) {
		return ATSVR_GENERAL;
	}

	if (env->num_commands < ATSVR_MAX_COMMANDS){
		/* Check if the command has already been registered.
		* Return 0, if it has been registered.
		*/
		for (i = 0; i < env->num_commands; i++){
			if (env->commands[i] == command){
				return ATSVR_GENERAL;
			}
		}
		env->commands[env->num_commands++] = command;
		ATSVRLOG("register \"%s\" command\r\n",command->name);
		return ATSVR_OK;
	}

	return ATSVR_ERROR;
}

int _atsvr_unregister_command(_atsvr_env_t *env,const struct _atsvr_command *command)
{
	int i;

	if (!command->name || !command->function) {
		return ATSVR_GENERAL;
	}

	if (env->num_commands < ATSVR_MAX_COMMANDS){
		/* Check if the command has already been registered.
		* Return 0, if it has been registered.
		*/
		for (i = 0; i < env->num_commands; i++){
			if (env->commands[i] == command){
				ATSVRLOG("unregister \"%s\" command\r\n",command->name);
				env->commands[i] = NULL;
				break;
			}
		}
	}

	return ATSVR_OK;
}

int _atsvr_register_commands(_atsvr_env_t *env,const struct _atsvr_command *commands,int num_commands)
{
	int i,sta;

	for (i = 0; i < num_commands; i++) {
		sta = _atsvr_register_command(env, commands++);
		if (sta != ATSVR_OK){
			return sta;
		}
	}

	return ATSVR_OK;
}

int _atsvr_unregister_commands(_atsvr_env_t *env,const struct _atsvr_command *commands,int num_commands)
{
	int i,sta;

	for (i = 0; i < num_commands; i++) {
		sta = _atsvr_unregister_command(env, commands++);
		if (sta != ATSVR_OK){
			return sta;
		}
	}

	return ATSVR_OK;
}


void _atsvr_notice_ready(_atsvr_env_t *env)
{
	if(env && env->output_func){
		env->output_func(ATSVR_READY_MSG,sizeof(ATSVR_READY_MSG)-1);
	}
}

void _atsvr_cmd_analysis_notice_error(_atsvr_env_t *env)
{
	if(env && env->output_func){
		env->output_func(ATSVR_CMDMSG_ERROR_RSP,sizeof(ATSVR_CMDMSG_ERROR_RSP)-1);
	}
}

void _atsvr_cmd_rsp_ok(_atsvr_env_t *env)
{
	if(env && env->output_func){
		env->output_func(ATSVR_CMD_RSP_SUCCEED,sizeof(ATSVR_CMD_RSP_SUCCEED)-1);
	}
}

void _atsvr_cmd_rsp_error(_atsvr_env_t *env)
{
	if(env && env->output_func){
		env->output_func(ATSVR_CMD_RSP_ERROR,sizeof(ATSVR_CMD_RSP_ERROR)-1);
	}
}

void _atsvr_output_msg(_atsvr_env_t *env,char *msg,unsigned int msg_len)
{
	if(env && env->output_func){
		env->output_func(msg,msg_len);
	}
}



static const struct _atsvr_command *_atsvr_lookup_at_command(_atsvr_env_t *env,char *name, int len)
{
	int i = 0;
	int n = 0;
	int name_len,cmd_name_len;

	name_len = strlen(name);
	while (i < ATSVR_MAX_COMMANDS && n < env->num_commands) {
		if (env->commands[i]->name == NULL) {
			i++;
			continue;
		}
		#if defined(ATSVR_OPTIM_FD_CMD) && ATSVR_OPTIM_FD_CMD
		cmd_name_len = env->commands[i]->name_len;
		#else
		cmd_name_len = strlen(env->commands[i]->name);
		#endif
		/* See if partial or full match is expected */
		if (len != 0) {
			if ((cmd_name_len >= len)
				&& (!strncmp(env->commands[i]->name, name, len))) {
				return env->commands[i];
			}
		}else {
			if ((cmd_name_len == name_len)
				&& (!strcmp(env->commands[i]->name, name))) {
				return env->commands[i];
			}
		}

		i++;
		n++;
	}

	return NULL;
}

int _atsvc_command_handle(_atsvr_env_t *env,unsigned char argc,char **argv)
{
    const struct _atsvr_command *command = NULL;
    int i;
    char *p;

    i = ((p = strchr(argv[0], '.')) == NULL) ? 0 : (p - argv[0]);

    command = _atsvr_lookup_at_command(env,argv[0], i);
    if ((command == NULL) || (NULL == command->function)) {
		ATSVRLOG("[ATSVR]no find \"%s\" command\r\n",argv[0]);
        return ATSVR_GENERAL;
    }

#if ATSVR_HANDLER_ENV
    command->function(env, argc, argv);
#else
	if(argc < ATSVR_MAX_ARG) {
		argv[argc] = (char *)env;
	}
	command->function(argc, argv);
#endif
    return ATSVR_OK;
}

typedef struct{
	unsigned inArg : 1;
	unsigned done : 1;
	unsigned isD : 2;
	unsigned limQ : 1;
}_atsvr_handle_input_stat;

static int _atsvr_handle_input(_atsvr_env_t *env,unsigned char *inbuf,int len)
{
	_atsvr_handle_input_stat stat;
#if ATSVR_HANDLER_ENV
    char *argv[ATSVR_MAX_ARG];
#else
	char *argv[ATSVR_MAX_ARG + 1];
#endif
    int argc = 0;
    int i = 0;
	int offset = 0;

    memset((void *)&argv, 0, sizeof(argv));
    memset(&stat, 0, sizeof(stat));

    do
    {
		offset++;
		if(offset > len){
			ATSVRLOG("The data is incomplete\r\n");
			return 7;  ////error
		}
        switch (inbuf[i])
        {
        case '\0':
			if(((argc == 0)||(stat.isD == 1))||(stat.limQ)){
				ATSVRLOG("The data does not conform to the regulations %d\r\n",__LINE__);
				return 2;
			}

			stat.done = 1;
            break;
        case '=':
            if(argc == 1) {
				inbuf[i] = '\0';
				stat.inArg = 0;
				stat.isD = 1;
			}
            else if(argc == 0){
				ATSVRLOG("The data does not conform to the regulations %d\r\n",__LINE__);
				return 4;
            }
            break;
#if ATSVR_ADD_ESCAPE_CFG
		case '\\':  ////"\"
			offset += 1;
			if((offset + 1) > len){
				ATSVRLOG("The data does not conform to the regulations %d\r\n",__LINE__);
				return 6;  ////error
			}
			memmove(&inbuf[i],&inbuf[i+1],(len-offset) + 1);
            break;
#endif
        case ',':
            if((stat.isD == 1)&&(argc == 1))  ///=,
            {
				ATSVRLOG("The data does not conform to the regulations %d\r\n",__LINE__);
                return 5;
            }
            if(stat.inArg) {
                stat.inArg = 0;
                inbuf[i] = '\0';
                stat.limQ = 1;
            }
            break;
        default:
            if(!stat.inArg) {
                stat.inArg = 1;
                argc++;
                argv[argc - 1] = (char*)&inbuf[i];
                stat.limQ = 0;
                if(stat.isD == 1) {
                    stat.isD = 2;
                }
            }
            break;
        }
    }
    while (!stat.done && (++i < ATSVR_INPUT_BUFF_MAX_SIZE));

    if (argc < 1) {
		ATSVRLOG("Data parsing exception\r\n");
        return 3;
    }

    return _atsvc_command_handle(env,argc,argv);
}


int _atsvr_input_msg_analysis_handler(_atsvr_env_t *env,char *msg,unsigned int msg_len)
{
	int ret;

	if(env == NULL){
		return ATSVR_SEVERE_ERR;
	}

	if( env->echo != _ATSVR_ECHO_NONE ) {
		if(env->output_func){
			env->output_func(msg, msg_len - 1);
			env->output_func("\r\n\r\n",4);
		}
	}

	ret = _atsvr_handle_input(env,(unsigned char*)msg,msg_len);
	if(ret != ATSVR_OK) {
		goto error;
	}

	return ATSVR_OK;
error:
	_atsvr_cmd_analysis_notice_error(env);
	return ATSVR_ERROR;
}


int _atsvr_def_config(_atsvr_env_t *env)
{
	int i;

	if(env == NULL){
		return ATSVR_SEVERE_ERR;
	}

	env->num_commands = 0;
	for(i = 0;i < ATSVR_MAX_COMMANDS; i++){
		env->commands[i] = NULL;
	}
	env->output_func = NULL;
	env->echo = ATSVR_ECHO_DEFAULT;
#if ATSVR_INTERNAL_CMD
	_atsvr_def_cmd_init(env);
#endif
	return ATSVR_OK;
}

void _atsvr_register_output_func(_atsvr_env_t *env,output_func_t output_func)
{
	if( env ){
		env->output_func = output_func;
	}
}

///Register the resource critical protection function
void _atsvr_register_resources_protection_func(_atsvr_env_t *env,resources_protection res_prot)
{
	if( env ) {
		env->res_prot = res_prot;
	}
}

int _atsvr_resource_protection_lock(_atsvr_env_t *env)
{
	if(env->res_prot) {
		if(env->res_prot(1,0xFFFFFFFFU) != 0) {
			return ATSVR_ERROR;
		}
	}
	return ATSVR_OK;
}

int _atsvr_resource_protection_lock_timeout(_atsvr_env_t *env,unsigned int timeout)
{
	if( env->res_prot ) {
		if(env->res_prot(1,timeout) != 0){
			return ATSVR_ERROR;
		}
	}
	return ATSVR_OK;
}

void _atsvr_resource_protection_unlock(_atsvr_env_t *env)
{
	if(env->res_prot) {
		env->res_prot(0, 0);
	}
}

void _atsvr_register_input_msg_func(_atsvr_env_t *env,input_msg_get_t input_msg_func)
{
	if( env ) {
		env->input_msg_func = input_msg_func;
	}
}

unsigned int _atsvr_input_msg_get(_atsvr_env_t *env,char *data,unsigned int data_len)
{
	int length = 0;

	if(env && env->input_msg_func) {
		env->input_msg_func(data,data_len);
	}
	return length;
}


_atsvr_work_st _get_atsvr_work_state(_atsvr_env_t *env)
{
	_atsvr_work_st wk_st;
	int ret;

	ret = _atsvr_resource_protection_lock(env);
	if(ret == ATSVR_OK){
		wk_st = env->wk_st;
		_atsvr_resource_protection_unlock(env);
	}else{
		wk_st = env->wk_st;
	}
	return wk_st;
}

void _set_atsvr_work_state(_atsvr_env_t *env,_atsvr_work_st wk_st)
{
	int ret;

	ret = _atsvr_resource_protection_lock(env);
	if(ret == ATSVR_OK){
		env->wk_st = wk_st;
		_atsvr_resource_protection_unlock(env);
	}else{
		env->wk_st = wk_st;
	}
	return;
}


