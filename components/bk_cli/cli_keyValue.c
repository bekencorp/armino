#include <stdlib.h>
#include "cli.h"
#include "key_value.h"
#include "func_convert.h"
#ifdef CONFIG_LWIP
#include "net.h"
#endif
#ifdef CONFIG_KEYVALUE
#if 0
#include "ADK.h"
#include "HAPBase.h"
#include "HAPPlatformRandomNumber.h"
#include "HAPCrypto.h"
#endif


typedef struct {
    uint8_t salt[16];      /**< SRP salt. */
    uint8_t verifier[384]; /**< SRP verifier. */
} HAPSetupInfo_ext;

extern void* AdkRunApplication(void* ctx );
extern void HAPPlatformRandomNumberFill(void* bytes, size_t numBytes);
extern void HAP_srp_verifier(
        uint8_t v[384],
        const uint8_t salt[16],
        const uint8_t* user,
        size_t user_len,
        const uint8_t* pass,
        size_t pass_len);
#endif

/**
 ************************************************************
 * @brief wirte_flash.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
static void key_value_command_test(char * pcWriteBuffer, int xWriteBufferLen, int argc, char * *argv)
{
    char cmd = 0;
    UINT32 len = 0, domain = 0, idx;
    UINT32 key = 0;
    UINT8 * ptr;
#ifdef CONFIG_KEYVALUE
    beken_thread_t homekit_app;
    bk_err_t ret;
    const char name[] = "Pair-Setup";
    HAPSetupInfo_ext info;
#endif
    ptr = ((uint8_t *) os_malloc(0x400));
    if (argc >= 4)
    {
        cmd = argv[1][0];
        domain = str_to_hexnum(argv[2]);
        key = str_to_hexnum(argv[3]);
        switch (cmd)
        {
            case 'e':
                erase_flash(domain);
                break;

            case 'r':
                read_flash(domain, ptr, key);
                for (idx = 0; idx < key; idx++) os_printf("%02x ", ptr[idx]);
                os_printf(" end.\r\n");
                break;

            case 'w':
                if (argc > 4)
                {
                    len = str_to_hexarray(argv[4], ptr);
                    write_flash(domain, ptr, len);
                }
                break;

            case 'E':
                rm_domain(KV_BASE_ADDR, domain);
                break;

            case 'R':
                if (get_domain_key_value(KV_BASE_ADDR, domain, key, ptr, 0x400, &len))
                {
                    os_printf("key %02x%02x= \r\n", domain, key);
                    for (idx = 0; idx < len; idx++) os_printf("%02x ", ptr[idx]);
                    os_printf(" end.\r\n");
                }
                break;

            case 'U':
                if (argc > 4) set_domain_key_value(KV_BASE_ADDR, domain, key, (UINT8 *)argv[4], os_strlen(argv[4]));
                break;

            case 'W':
                if (argc > 4) set_domain_key_value(KV_BASE_ADDR, domain, key, (UINT8 *)argv[4], os_strlen(argv[4]) + 1);
                break;

            case 'V':
                if (argc > 4)
                {
                    len = str_to_hexarray(argv[4], ptr);
                    set_domain_key_value(KV_BASE_ADDR, domain, key, ptr, len);
                }
                break;

            case 'N':
                search_domain_key(KV_BASE_ADDR, domain, ptr);
                break;
#ifdef CONFIG_LWIP
            case 'B':
                switch (argv[2][0])
                {
                    case 'U':
                        sta_ip_start();
                        break;

                    case 'D':
                        sta_ip_down();
                        break;
                }
#endif
                break;

            case 'M':
                rm_domain_key(KV_BASE_ADDR, domain, key);
                break;

#ifdef CONFIG_KEYVALUE
            case 'K':

                if (argc > 4)
                {
                    if (argc == 5) HAPPlatformRandomNumberFill((void *)info.salt, sizeof(info.salt));
                    else 
                    {
                        len = str_to_hexarray(argv[5], ptr);
                        os_memcpy((void *)info.salt, (void *)ptr, sizeof(info.salt));
                    }
                    HAP_srp_verifier(info.verifier, info.salt, (UINT8 *)name, os_strlen(name), (UINT8 *)argv[4], os_strlen(argv[4]));
                    set_domain_key_value(KV_BASE_ADDR, 0x40, 0x10, info.salt, sizeof(info));
                }
                break;

            case 'T':
                len = atoi(argv[2]);
                if (len < 2048) len = 2048;
                ret = rtos_create_thread(&homekit_app, 
                    BEKEN_APPLICATION_PRIORITY, 
                    "homekit", 
                    (beken_thread_function_t) AdkRunApplication, 
                    (unsigned short) len, 
                    (beken_thread_arg_t) 0);
                if(kNoErr != ret)
                    os_printf("!!! homekit task err\r\n");
                break;

#endif

            default:
                break;
        }
    }
    else 
    {
        os_printf("key_value <R/W/E/M/N/T> <domain> <key>\r\n");
    }
    os_free(ptr);
}


#define KV_CMD_CNT              (sizeof(s_kv_commands) / sizeof(struct cli_command))
static const struct cli_command s_kv_commands[] =
{
    {
        "KEY_VALUE", "KEY_VALUE <R/W/E/M/N/T> <domain> <key>", key_value_command_test
    },
};


int cli_keyVaule_init(void)
{ 
    return cli_register_commands(s_kv_commands, KV_CMD_CNT);
}


