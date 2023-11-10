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

//#define CONFIG_HW_ACC_MBEDTLS_TEST 1

#include "cli.h"
#include  "driver/securityip.h"
#if CONFIG_HW_ACC_MBEDTLS_TEST
#include "ecdsa.h"
#include "ccm.h"
#include "gcm.h"
#endif
static void cli_securityip_help(void)
{
	CLI_LOGI("securityip_driver init/deinit\r\n");
	CLI_LOGI("securityip_trng cfg/get\r\n");

//Note!!! Required CONFIG_MBEDTLS and MBEDTLS_SELF_TEST
//Note!!! ecdsa Using hardware acceleration, need to be defined MBEDTLS_ECDSA_VERIFY_ALT、MBEDTLS_ECDSA_SIGN_ALT
#if CONFIG_HW_ACC_MBEDTLS_TEST
	CLI_LOGI("securityip_sha_test 256/512\r\n");
	CLI_LOGI("securityip_ecdsa test 0/1 n\r\n");
	CLI_LOGI("securityip_aes ccm/gcm 0/1\r\n");
	CLI_LOGI("securityip_rsa_test pkcs1v15/pss");
#endif
}

static void cli_securityip_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_securityip_driver_init());
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_securityip_driver_deinit());
	} else {
		cli_securityip_help();
		return;
	}
}

static void cli_securityip_system_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}

	if (os_strcmp(argv[1], "version") == 0) {
		
		CLI_LOGI("get securityip system version sucess!\r\n");
	} else if (os_strcmp(argv[1], "state") == 0) {
		
		CLI_LOGI("get securityip system state sucess!\r\n");
	} else if (os_strcmp(argv[1], "reset") == 0) {
		
		CLI_LOGI("securityip system reset!\r\n");
	} else {
		cli_securityip_help();
		return;
	}
}

static void cli_securityip_trng_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}

	if (os_strcmp(argv[1], "cfg") == 0) {
		BK_LOG_ON_ERR(bk_securityip_trng_def_cfg());
	} else if (os_strcmp(argv[1], "get") == 0) {
		uint8_t * RandNum_p;
		uint8_t * Num_p;

		uint32_t random_size = os_strtoul(argv[2], NULL, 10)&0xffff;
		Num_p = RandNum_p = (uint8_t *)malloc(random_size+4);
		memset(RandNum_p, 0xaa, random_size+4);

		BK_LOG_ON_ERR(bk_securityip_get_trng(random_size, RandNum_p));
		CLI_LOGI("get random num is \r\n");
		for(int i=0; i<random_size + 4; i++){
			if((*Num_p == 0xaa)&&(*(Num_p+1) == 0xaa)){
				bk_printf("  \r\n");
				CLI_LOGI("get random num size = %d bytes!\r\n",i);
				break;
			}
			bk_printf("%02x",*Num_p);
			Num_p++;
		}
		free(RandNum_p);
	} else {
		cli_securityip_help();
		return;
	}
	
}

#if CONFIG_HW_ACC_MBEDTLS_TEST

extern int mbedtls_sha256_self_test(int verbose);
extern int mbedtls_sha512_self_test(int verbose);
extern int bk_mbedtls_rsa_self_test( int verbose, int type );

static void cli_sha_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv){
	int ret = -1;
	if (argc < 2) {
		cli_securityip_help();
		return;
	}
	if(os_strcmp(argv[1], "256") == 0){
		ret = mbedtls_sha256_self_test(0);
	}else if (os_strcmp(argv[1], "512") == 0){
		ret = mbedtls_sha512_self_test(0);
	}
	if (0 == ret)
		bk_printf("passed\r\n");
	else
		bk_printf("failed\r\n");
}

extern int mbedtls_ecdsa_self_test( int verbose ,uint32_t loop);
static void cli_securityip_ecdsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 4) {
		cli_securityip_help();
		return;
	}
	int verbose;
	uint32_t loop_count;
	verbose    = os_strtoul(argv[2], NULL, 10);
	loop_count = os_strtoul(argv[3], NULL, 10);

	if (os_strcmp(argv[1], "test") == 0) {
		mbedtls_ecdsa_self_test(verbose, loop_count);
	}else
	{
		cli_securityip_help();
	}
}

static void cli_securityip_rsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}	
	if(os_strcmp(argv[1],"pkcs1v15")==0)
	{
		bk_mbedtls_rsa_self_test(1, 0);
	}
	if(os_strcmp(argv[1],"pss")==0)
	{
		bk_mbedtls_rsa_self_test(1, 1);
	}
}

static void cli_securityip_aes_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_securityip_help();
		return;
	}
	int verbose;
	verbose = os_strtoul(argv[2], NULL, 10);
	
	if (os_strcmp(argv[1], "ccm") == 0) {
		mbedtls_ccm_self_test(verbose);
	}else if (os_strcmp(argv[1], "gcm") == 0) {
		mbedtls_gcm_self_test(verbose);
	}
}

#endif

#define SECURITYIP_CMD_CNT (sizeof(s_securityip_commands) / sizeof(struct cli_command))
static const struct cli_command s_securityip_commands[] = {
	{"securityip_driver", "securityip_driver {init|deinit}", cli_securityip_driver_cmd},
	{"securityip_system", "securityip_system {version|state|reset}", cli_securityip_system_cmd},
	{"securityip_trng", "securityip_trng {cfg|get}", cli_securityip_trng_cmd},
#if CONFIG_HW_ACC_MBEDTLS_TEST
	{"securityip_sha_test", "securityip_sha_test {256|512}", cli_sha_test_cmd},
	{"securityip_ecdsa", "securityip_ecdsa {test}", cli_securityip_ecdsa_cmd},
	{"securityip_rsa_test", "securityip_rsa_test {pkcs1v15|pss}", cli_securityip_rsa_cmd},
	{"securityip_aes", "securityip_aes {ccm|gcm}", cli_securityip_aes_cmd}
#endif
};

int cli_securityip_init(void)
{
	return cli_register_commands(s_securityip_commands, SECURITYIP_CMD_CNT);
}


