#include "at_server.h"
#include "_at_server.h"
#include "_atsvr_func.h"


_at_svr_ctrl_env_t _at_svr_env = {
	0
};

char *get_atsvr_version(void)
{
	return _get_atsvr_version();
}

void set_atsvr_echo_mode(atsvr_echo_t echo)
{
	_set_atsvr_echo_mode(&_at_svr_env,(_atsvr_echo_t)echo);
}

atsvr_echo_t get_atsvr_echo_mode(void)
{
	return (atsvr_echo_t)_get_atsvr_echo_mode(&_at_svr_env);
}

int atsvr_register_command(const struct _atsvr_command *command)
{
	return _atsvr_register_command(&_at_svr_env,command);
}

int atsvr_register_commands(const struct _atsvr_command *commands,int num_commands)
{
	return _atsvr_register_commands(&_at_svr_env,commands,num_commands);
}

void atsvr_notice_ready(void)
{
	_atsvr_notice_ready(&_at_svr_env);
}

void atsvr_cmd_rsp_ok(void)
{
	_atsvr_cmd_rsp_ok(&_at_svr_env);
}

void atsvr_cmd_rsp_error(void)
{
	_atsvr_cmd_rsp_error(&_at_svr_env);
}

void atsvr_cmd_analysis_notice_error(void)
{
	_atsvr_cmd_analysis_notice_error(&_at_svr_env);
}

void atsvr_input_msg_overflow(void)
{
	_ATSVR_SIZEOF_OUTPUT_STRING(&_at_svr_env,ATSVR_INPUT_MSG_OVERFLOW);
}

void atsvr_output_msg(char *msg,unsigned int msg_len)
{
	_atsvr_output_msg(&_at_svr_env,msg,msg_len);
}

void atsvr_register_output_func(output_func_t output_func)
{
	_atsvr_register_output_func(&_at_svr_env,output_func);
}

///Register the resource critical protection function
void atsvr_register_resources_protection_func(resources_protection res_prot)
{
	_atsvr_register_resources_protection_func(&_at_svr_env,res_prot);
}

void atsvr_register_input_msg_func(input_msg_get_t input_msg_func)
{
	_atsvr_register_input_msg_func(&_at_svr_env,input_msg_func);
}

atsvr_work_st get_atsvr_work_state(void)
{
	return (atsvr_work_st)_get_atsvr_work_state(&_at_svr_env);
}

void set_atsvr_work_state(atsvr_work_st wk_st)
{
	_set_atsvr_work_state(&_at_svr_env,(_atsvr_work_st)wk_st);
}

int atsvr_resource_protection_lock(void)
{
	if(_atsvr_resource_protection_lock(&_at_svr_env) == ATSVR_OK){
		return 0;
	}
	return -1;
}

int atsvr_resource_protection_lock_timeout(unsigned int timeout)
{
	if(_atsvr_resource_protection_lock_timeout(&_at_svr_env,timeout) == ATSVR_OK){
		return 0;
	}
	return -1;
}

void atsvr_resource_protection_unlock(void)
{
	_atsvr_resource_protection_unlock(&_at_svr_env);
}

int atsvr_input_msg_analysis_handler(char *msg,unsigned int msg_len)
{
	int ret = _atsvr_input_msg_analysis_handler(&_at_svr_env,msg,msg_len);

	if(ret != ATSVR_OK){
		ret = -1;
	}
	_set_atsvr_work_state(&_at_svr_env,_ATSVR_WK_IDLE);
	return ret;
}

unsigned int atsvr_input_msg_get(char *data,unsigned int data_len)
{
	unsigned int length = _atsvr_input_msg_get(&_at_svr_env,data,data_len);
	return length;
}


void atsvr_def_cmd_init(void)
{
	_atsvr_def_cmd_init(&_at_svr_env);
}

void at_server_init(void)
{
	_atsvr_def_config(&_at_svr_env);
}

