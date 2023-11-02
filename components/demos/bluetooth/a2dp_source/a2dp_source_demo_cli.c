#include "cli.h"
#include "a2dp_source_demo.h"
#include "a2dp_source_demo_avrcp.h"
#include "modules/dm_bt_types.h"

static void a2dp_source_usage(void)
{
    CLI_LOGI("Usage:\n"
             "a2dp_source discover [sec] [report count]\n"
             "a2dp_source discover_cancel\n"
             "a2dp_source connect XX:XX:XX:XX:XX:XX\n"
             "a2dp_source disconnect XX:XX:XX:XX:XX:XX\n"
             "a2dp_source play <mp3 file path>\n"
             "a2dp_source stop\n"
             "a2dp_source pause\n"
             "a2dp_source resume\n"
             "a2dp_source next\n"
             "a2dp_source prev\n"

             "a2dp_source test_start XX:XX:XX:XX:XX:XX <mp3 file path>\n"
             "a2dp_source test_stop\n"
            );

    return;
}

static void a2dp_player_usage(void)
{
    CLI_LOGI("Usage:\n"
             "a2dp_player discover [sec] [report count]\n"
             "a2dp_player discover_cancel\n"
             "a2dp_player connect XX:XX:XX:XX:XX:XX\n"
             "a2dp_player disconnect XX:XX:XX:XX:XX:XX\n"
             "a2dp_player play <mp3 file path>\n"
             "a2dp_player stop\n"
             "a2dp_player pause\n"
             "a2dp_player resume\n"
             "a2dp_player next\n"
             "a2dp_player prev\n"
            );

    return;
}

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP

static void avrcp_usage(void)
{
    CLI_LOGI("Usage:\n"
             "a2dp_source_avrcp init\n"
             "a2dp_source_avrcp playback <stopped|playing|paused|fwd_seek|rev_seek>\n"
            );

    return;
}

#endif

static void cmd_a2dp_source(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int ret = 0;

    if (argc == 1)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "-h") == 0)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "discover") == 0)
    {
        uint32_t sec = 10;
        uint32_t num_report = 0;

        if (argc >= 3)
        {
            ret = sscanf(argv[2], "%d", &sec);

            if (ret != 1)
            {
                CLI_LOGE("param err, need sec, %d %s\n", ret, argv[2]);
                goto __usage;
            }

            if (argc >= 4)
            {
                ret = sscanf(argv[3], "%d", &num_report);

                if (ret != 1)
                {
                    CLI_LOGE("param err, need num_report, %d %s\n", ret, argv[3]);
                    goto __usage;
                }
            }
        }

        bt_a2dp_source_demo_discover(sec, num_report);
    }
    else if (os_strcmp(argv[1], "discover_cancel") == 0)
    {
        bt_a2dp_source_demo_discover_cancel();
    }
    else if (os_strcmp(argv[1], "connect") == 0)
    {
        if (argc >= 3)
        {
            uint8_t mac_final[6] = {0};
#if 1
            uint32_t mac[6] = {0};

            //sscanf bug: cant detect uint8_t size point
            ret = sscanf(argv[2], "%02x:%02x:%02x:%02x:%02x:%02x", //argv[1], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac + 5,
                         mac + 4,
                         mac + 3,
                         mac + 2,
                         mac + 1,
                         mac);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[1]);
                return;
            }

            for (uint8_t i = 0; i < sizeof(mac_final) / sizeof(mac_final[0]); ++i)
            {
                mac_final[i] = mac[i];
            }

#else
            ret = sscanf(argv[2], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac_final + 5,
                         mac_final + 4,
                         mac_final + 3,
                         mac_final + 2,
                         mac_final + 1,
                         mac_final);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[1]);
                return;
            }

#endif

            CLI_LOGI("%s mac %02x:%02x:%02x:%02x:%02x:%02x\n", __func__,
                     mac_final[5],
                     mac_final[4],
                     mac_final[3],
                     mac_final[2],
                     mac_final[1],
                     mac_final[0]);

            ret = bt_a2dp_source_demo_connect(mac_final);

            if (ret)
            {
                goto __error;
            }
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "disconnect") == 0)
    {
        if (argc >= 3)
        {
            uint8_t mac_final[6] = {0};
#if 1
            uint32_t mac[6] = {0};

            //sscanf bug: cant detect uint8_t size point
            ret = sscanf(argv[2], "%02x:%02x:%02x:%02x:%02x:%02x", //argv[1], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac + 5,
                         mac + 4,
                         mac + 3,
                         mac + 2,
                         mac + 1,
                         mac);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[1]);
                return;
            }

            for (uint8_t i = 0; i < sizeof(mac_final) / sizeof(mac_final[0]); ++i)
            {
                mac_final[i] = mac[i];
            }

#else
            ret = sscanf(argv[2], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac_final + 5,
                         mac_final + 4,
                         mac_final + 3,
                         mac_final + 2,
                         mac_final + 1,
                         mac_final);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[1]);
                return;
            }

#endif

            CLI_LOGI("%s mac %02x:%02x:%02x:%02x:%02x:%02x\n", __func__,
                     mac_final[5],
                     mac_final[4],
                     mac_final[3],
                     mac_final[2],
                     mac_final[1],
                     mac_final[0]);

            ret = bt_a2dp_source_demo_disconnect(mac_final);

            if (ret)
            {
                goto __error;
            }
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "play") == 0)
    {
        if (argc >= 3)
        {
            ret = bt_a2dp_source_demo_music_play(1, (uint8_t *)argv[2]);

            if (ret)
            {
                goto __error;
            }
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "stop") == 0)
    {
        ret = bt_a2dp_source_demo_music_stop();

        if (ret)
        {
            goto __error;
        }
    }
    else if (os_strcmp(argv[1], "pause") == 0)
    {
        ret = bt_a2dp_source_demo_music_pause();

        if (ret)
        {
            goto __error;
        }
    }
    else if (os_strcmp(argv[1], "resume") == 0)
    {
        ret = bt_a2dp_source_demo_music_resume();

        if (ret)
        {
            goto __error;
        }
    }
    else if (os_strcmp(argv[1], "prev") == 0)
    {
        ret = bt_a2dp_source_demo_music_prev();

        if (ret)
        {
            goto __error;
        }
    }
    else if (os_strcmp(argv[1], "next") == 0)
    {
        ret = bt_a2dp_source_demo_music_next();

        if (ret)
        {
            goto __error;
        }
    }
    else if (os_strcmp(argv[1], "test_start") == 0)
    {
        if (argc >= 4)
        {
            uint8_t mac_final[6] = {0};
            uint32_t mac[6] = {0};

            //sscanf bug: cant detect uint8_t size point
            ret = sscanf(argv[2], "%02x:%02x:%02x:%02x:%02x:%02x", //argv[1], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac + 5,
                         mac + 4,
                         mac + 3,
                         mac + 2,
                         mac + 1,
                         mac);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[2]);
                return;
            }

            for (uint8_t i = 0; i < sizeof(mac_final) / sizeof(mac_final[0]); ++i)
            {
                mac_final[i] = mac[i];
            }

            bt_a2dp_source_demo_test(mac_final, 1, (uint8_t *)argv[3]);
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "test_stop") == 0)
    {
        bt_a2dp_source_demo_stop_all();
    }
    else
    {
        goto __usage;
    }

    msg = CLI_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;

__usage:
    a2dp_source_usage();

__error:
    msg = CLI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}



#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
static void cmd_avrcp(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int ret = 0;

    if (argc == 1)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "-h") == 0)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "init") == 0)
    {
        ret = bt_avrcp_demo_init();

        if (ret)
        {
            goto __error;
        }
    }
    else if (os_strcmp(argv[1], "playback") == 0)
    {
        uint8_t play_status = 0;

        if (argc < 3)
        {
            goto __usage;
        }

        if (!os_strcasecmp(argv[2], "stopped"))
        {
            play_status = BK_AVRCP_PLAYBACK_STOPPED;
        }
        else if (!os_strcasecmp(argv[2], "playing"))
        {
            play_status = BK_AVRCP_PLAYBACK_PLAYING;
        }
        else if (!os_strcasecmp(argv[2], "paused"))
        {
            play_status = BK_AVRCP_PLAYBACK_PAUSED;
        }
        else if (!os_strcasecmp(argv[2], "fwd_seek"))
        {
            play_status = BK_AVRCP_PLAYBACK_FWD_SEEK;
        }
        else if (!os_strcasecmp(argv[2], "rev_seek"))
        {
            play_status = BK_AVRCP_PLAYBACK_REV_SEEK;
        }
        else
        {
            goto __usage;
        }

        ret = bt_avrcp_demo_report_playback(play_status);

        if (ret)
        {
            goto __error;
        }
    }

    else
    {
        goto __usage;
    }

    msg = CLI_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;

__usage:
    avrcp_usage();

__error:
    msg = CLI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#endif



static void cmd_a2dp_player_demo(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = NULL;
    int ret = 0;

    if (argc == 1)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "-h") == 0)
    {
        goto __usage;
    }
    else if (os_strcmp(argv[1], "discover") == 0)
    {
        uint32_t sec = 10;
        uint32_t num_report = 0;

        if (argc >= 3)
        {
            ret = sscanf(argv[2], "%d", &sec);

            if (ret != 1)
            {
                CLI_LOGE("param err, need sec, %d %s\n", ret, argv[2]);
                goto __usage;
            }

            if (argc >= 4)
            {
                ret = sscanf(argv[3], "%d", &num_report);

                if (ret != 1)
                {
                    CLI_LOGE("param err, need num_report, %d %s\n", ret, argv[3]);
                    goto __usage;
                }
            }
        }

        bt_a2dp_source_demo_discover(sec, num_report);
    }
    else if (os_strcmp(argv[1], "discover_cancel") == 0)
    {
        bt_a2dp_source_demo_discover_cancel();
    }
    else if (os_strcmp(argv[1], "connect") == 0)
    {
        if (argc >= 3)
        {
            uint8_t mac_final[6] = {0};
            uint32_t mac[6] = {0};

            //sscanf bug: cant detect uint8_t size point
            ret = sscanf(argv[2], "%02x:%02x:%02x:%02x:%02x:%02x", //argv[1], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac + 5,
                         mac + 4,
                         mac + 3,
                         mac + 2,
                         mac + 1,
                         mac);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[1]);
                return;
            }

            for (uint8_t i = 0; i < sizeof(mac_final) / sizeof(mac_final[0]); ++i)
            {
                mac_final[i] = mac[i];
            }


            CLI_LOGI("%s mac %02x:%02x:%02x:%02x:%02x:%02x\n", __func__,
                     mac_final[5],
                     mac_final[4],
                     mac_final[3],
                     mac_final[2],
                     mac_final[1],
                     mac_final[0]);

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
            bt_avrcp_demo_init();
#endif
            ret = bt_a2dp_source_demo_connect(mac_final);

            if (ret)
            {
                goto __error;
            }

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
            uint32_t count = 0;

            while (!bt_avrcp_is_ready() && count <= 20)
            {
                count++;
                rtos_delay_milliseconds(100);
            }

            if (!bt_avrcp_is_ready())
            {
                goto __error;
            }

#endif
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "disconnect") == 0)
    {
        if (argc >= 3)
        {
            uint8_t mac_final[6] = {0};
            uint32_t mac[6] = {0};

            //sscanf bug: cant detect uint8_t size point
            ret = sscanf(argv[2], "%02x:%02x:%02x:%02x:%02x:%02x", //argv[1], "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         mac + 5,
                         mac + 4,
                         mac + 3,
                         mac + 2,
                         mac + 1,
                         mac);

            if (ret != 6)
            {
                CLI_LOGE("param err, need mac, %d %s\n", ret, argv[1]);
                return;
            }

            for (uint8_t i = 0; i < sizeof(mac_final) / sizeof(mac_final[0]); ++i)
            {
                mac_final[i] = mac[i];
            }

            CLI_LOGI("%s mac %02x:%02x:%02x:%02x:%02x:%02x\n", __func__,
                     mac_final[5],
                     mac_final[4],
                     mac_final[3],
                     mac_final[2],
                     mac_final[1],
                     mac_final[0]);

            ret = bt_a2dp_source_demo_disconnect(mac_final);

            if (ret)
            {
                goto __error;
            }
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "play") == 0)
    {
        if (argc >= 3)
        {
            ret = bt_a2dp_source_demo_music_play(1, (uint8_t *)argv[2]);

            if (ret)
            {
                goto __error;
            }

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
            ret = bt_avrcp_demo_report_playback(BK_AVRCP_PLAYBACK_PLAYING);

            if (ret)
            {
                goto __error;
            }

#endif
        }
        else
        {
            goto __usage;
        }
    }
    else if (os_strcmp(argv[1], "stop") == 0)
    {
        ret = bt_a2dp_source_demo_music_stop();

        if (ret)
        {
            goto __error;
        }

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
        ret = bt_avrcp_demo_report_playback(BK_AVRCP_PLAYBACK_STOPPED);

        if (ret)
        {
            goto __error;
        }

#endif
    }
    else if (os_strcmp(argv[1], "pause") == 0)
    {
        ret = bt_a2dp_source_demo_music_pause();

        if (ret)
        {
            goto __error;
        }

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
        ret = bt_avrcp_demo_report_playback(BK_AVRCP_PLAYBACK_PAUSED);

        if (ret)
        {
            goto __error;
        }

#endif
    }
    else if (os_strcmp(argv[1], "resume") == 0)
    {
        ret = bt_a2dp_source_demo_music_resume();

        if (ret)
        {
            goto __error;
        }

#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
        ret = bt_avrcp_demo_report_playback(BK_AVRCP_PLAYBACK_PLAYING);

        if (ret)
        {
            goto __error;
        }

#endif
    }
    else if (os_strcmp(argv[1], "prev") == 0)
    {
        ret = bt_a2dp_source_demo_music_prev();

        if (ret)
        {
            goto __error;
        }

    }
    else if (os_strcmp(argv[1], "next") == 0)
    {
        ret = bt_a2dp_source_demo_music_next();

        if (ret)
        {
            goto __error;
        }
    }
    else
    {
        goto __usage;
    }

    msg = CLI_CMD_RSP_SUCCEED;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;

__usage:
    a2dp_player_usage();

__error:
    msg = CLI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

static const struct cli_command s_a2dp_source_commands[] =
{
    {"a2dp_player", "see -h", cmd_a2dp_player_demo},
    //{"a2dp_source", "see -h", cmd_a2dp_source},
#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
    //{"a2dp_source_avrcp", "see -h", cmd_avrcp},
#endif

};


int cli_a2dp_source_demo_init(void)
{
    (void)cmd_a2dp_source;
#if CONFIG_A2DP_SOURCE_DEMO_AVRCP
    (void)cmd_avrcp;
#endif
    return cli_register_commands(s_a2dp_source_commands, sizeof(s_a2dp_source_commands) / sizeof(s_a2dp_source_commands[0]));
}

