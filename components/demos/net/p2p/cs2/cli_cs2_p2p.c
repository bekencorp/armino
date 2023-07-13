#include "cli.h"
#include "cs2_p2p_main.h"

static void cs2_p2p_usage(void)
{
    os_printf("Usage: cs2_p2p start <DID_dskey> <APIlicense_crckey> <initstring_p2pkey>\n"
              "cs2_p2p stop\n"
              "cs2_p2p send <channel> <string>\n"
              "cs2_p2p send_count <channel> <count>\n"
              "cs2_p2p log <level>\n"
#if CONFIG_CS2_P2P_TEST

#if CONFIG_CS2_P2P_SERVER
              "cs2_p2p test server_send_with_crc_main <DID_dskey> <APIlicense_crckey> <initstring_p2pkey>\n"
              "cs2_p2p test server_raw_send <DID_dskey> <APIlicense_crckey> <initstring_p2pkey>\n"
#endif

#if CONFIG_CS2_P2P_CLIENT
              "cs2_p2p test listentester <DID_dskey> <APIlicense_crckey> <initstring_p2pkey>\n"
#endif

#endif

             );

    return;
}


static int32_t recv_callback(uint8_t channel, uint8_t *buff, uint32_t len)
{
    if (len)
    {
        os_printf("%s len %d, last 0x%X\n", __func__, len, buff[len - 1]);
    }

    return 0;
}


static void cmd_cs2_p2p(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;


    if (argc == 1)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "-h") == 0)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "start") == 0)
    {
        if (argc >= 5)
        {
            os_printf("%s argv len %d %d %d\n", __func__, strlen(argv[2]), strlen(argv[3]), strlen(argv[4]));
            //            create_cli_cs2_p2p_task(argv[2], argv[3], argv[4]);
            cs2_p2p_main_task_create(argv[2], argv[3], argv[4], recv_callback);
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "stop") == 0)
    {
        //        stop_cli_cs2_p2p_task();
        cs2_p2p_main_task_destory();
    }
    else if (os_strcmp(argv[1], "send") == 0)
    {
        if (argc >= 4)
        {
            cs2_p2p_send_raw(atoi(argv[2]), (uint8_t *)argv[3], strlen(argv[3]));
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "send_count") == 0)
    {
        if (argc >= 4)
        {
            uint8_t *tmp = os_malloc(atoi(argv[3]) + 1);
            uint8_t tmp_sum = 0;
            if(!tmp)
            {
                os_printf("%s alloc fail\n", __func__);
            }
            else
            {
                for (int i = 0; i < atoi(argv[3]); ++i)
                {
                    tmp_sum += tmp[i];
                }

                tmp[atoi(argv[3])] = tmp_sum;

                os_printf("%s last sum 0x%X\n", __func__, tmp_sum);
                cs2_p2p_send_raw(atoi(argv[2]), tmp, atoi(argv[3]) + 1);

                os_free(tmp);
            }

        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "log") == 0)
    {
        if (argc >= 3)
        {
            cs2_p2p_set_log_level(atoi(argv[2]));
        }
        else
        {
            goto __usage;
        }
    }

#if (CONFIG_CS2_P2P_TEST)
    else if (os_strcmp(argv[1], "test") == 0)
    {
        if (argc < 2)
        {
            goto __usage;
        }

        extern int cs2_p2p_test_main(int argc, char **argv);

        os_printf("%s argv len %d %d %d\n", __func__, strlen(argv[3]), strlen(argv[4]), strlen(argv[5]));

        if (cs2_p2p_test_main(argc - 2, argv + 2) < 0)
        {
            goto __usage;
        }
    }

#endif
    else
    {
        goto __usage;
    }

    msg = CLI_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;

__usage:
    cs2_p2p_usage();
    msg = CLI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));

}

static const struct cli_command s_cs2_p2p_commands[] =
{
    {"cs2_p2p", "see -h", cmd_cs2_p2p},
};


int cli_cs2_p2p_init(void)
{
    return cli_register_commands(s_cs2_p2p_commands, sizeof(s_cs2_p2p_commands) / sizeof(s_cs2_p2p_commands[0]));
}
