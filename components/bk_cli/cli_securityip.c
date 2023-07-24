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
//Note!!! ecdsa Using hardware acceleration, need to be defined MBEDTLS_ECDSA_VERIFY_ALT��MBEDTLS_ECDSA_SIGN_ALT��MBEDTLS_ECDSA_GENKEY_ALT
#if CONFIG_HW_ACC_MBEDTLS_TEST
	CLI_LOGI("securityip_sha_test 256/512\r\n");
	CLI_LOGI("securityip_ecdsa genkey/sign/verify\r\n");
	CLI_LOGI("securityip_aes ccm/gcm\r\n");
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
static const uint8_t Vector_P384_SHA1_Message[] =
{
    0xAB, 0xE1, 0x0A, 0xCE, 0x13, 0xE7, 0xE1, 0xD9, 0x18, 0x6C, 0x48, 0xF7, 0x88, 0x9D, 0x51, 0x47,
    0x3D, 0x3A, 0x09, 0x61, 0x98, 0x4B, 0xC8, 0x72, 0xDF, 0x70, 0x8E, 0xCC, 0x3E, 0xD3, 0xB8, 0x16,
    0x9D, 0x01, 0xE3, 0xD9, 0x6F, 0xC4, 0xF1, 0xD5, 0xEA, 0x00, 0xA0, 0x36, 0x92, 0xBC, 0xC5, 0xCF,
    0xFD, 0x53, 0x78, 0x7C, 0x88, 0xB9, 0x34, 0xAF, 0x40, 0x4C, 0x03, 0x9D, 0x32, 0x89, 0xB5, 0xBA,
    0xC5, 0xAE, 0x7D, 0xB1, 0x49, 0x68, 0x75, 0xB5, 0xDC, 0x73, 0xC3, 0x09, 0xF9, 0x25, 0xC1, 0x3D,
    0x1C, 0x01, 0xAB, 0xDA, 0xAF, 0xEB, 0xCD, 0xAC, 0x2C, 0xEE, 0x43, 0x39, 0x39, 0xCE, 0x8D, 0x4A,
    0x0A, 0x5D, 0x57, 0xBB, 0x70, 0x5F, 0x3B, 0xF6, 0xEC, 0x08, 0x47, 0x95, 0x11, 0xD4, 0xB4, 0xA3,
    0x21, 0x1F, 0x61, 0x64, 0x9A, 0xD6, 0x27, 0x43, 0x14, 0xBF, 0x0D, 0x43, 0x8A, 0x81, 0xE0, 0x60
};

mbedtls_ecdsa_context ecdsa_context;
mbedtls_ecp_group     ecp_group;
mbedtls_ecp_point Q;
mbedtls_mpi r;
mbedtls_mpi s;

static void cli_securityip_ecdsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}
	ecp_group.id = MBEDTLS_ECP_DP_SECP384R1;

	if (os_strcmp(argv[1], "genkey") == 0) {
		mbedtls_ecdsa_genkey(&ecdsa_context, MBEDTLS_ECP_DP_SECP384R1, NULL, NULL);
	} else if (os_strcmp(argv[1], "sign") == 0) {
		mbedtls_ecdsa_sign(&ecp_group, &r, &s, NULL, Vector_P384_SHA1_Message, sizeof(Vector_P384_SHA1_Message), NULL,NULL );
	}else if (os_strcmp(argv[1], "verify") == 0) {
		mbedtls_ecdsa_verify(&ecp_group, Vector_P384_SHA1_Message, sizeof(Vector_P384_SHA1_Message), NULL, &r, &s );
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
	if (argc < 2) {
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
	{"securityip_ecdsa", "securityip_ecdsa {genkey|sign|verify}", cli_securityip_ecdsa_cmd},
	{"securityip_rsa_test", "securityip_rsa_test {pkcs1v15|pss}", cli_securityip_rsa_cmd},
	{"securityip_aes", "securityip_aes {ccm|gcm}", cli_securityip_aes_cmd}
#endif
};

int cli_securityip_init(void)
{
	return cli_register_commands(s_securityip_commands, SECURITYIP_CMD_CNT);
}


