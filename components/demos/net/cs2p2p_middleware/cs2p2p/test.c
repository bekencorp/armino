#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <components/log.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdint.h>

#include "cs2_p2p_main.h"

#include <lwip/sockets.h>

#define PPPP_API_API


typedef struct
{
    beken_thread_t tid;
    int (*func)(int argc, char **argv);
    int argc;
    char **argv;

} middle_layer_task_arg_t;

static void middle_layer_task(void *arg)
{
    middle_layer_task_arg_t *targ = (typeof(targ))arg;

    if (targ && targ->func)
    {
        targ->func(targ->argc, targ->argv);
    }


    if (targ)
    {
        if (targ->argv)
        {
            for (int i = 0; i < targ->argc; ++i)
            {
                if (targ->argv[i])
                {
                    os_free(targ->argv[i]);
                }
            }

            os_free(targ->argv);
        }

        os_free(targ);
    }

    rtos_delete_thread(NULL);

    return;
}


//beken_thread_function_t
static void create_task(int (*func)(int argc, char **argv), char *func_name, int argc, char **argv)
{
    bk_err_t ret = 0;
    beken_thread_t tid;
    middle_layer_task_arg_t *targ;

    targ = (typeof(targ))os_malloc(sizeof(*targ));

    if (!targ)
    {
        os_printf("%s malloc fail\n");
        goto CREATE_FAIL;
    }

    targ->func = func;
    targ->argc = argc;

    targ->argv = (typeof(targ->argv))os_malloc(sizeof(targ->argv[0]) * argc);

    if (!targ->argv)
    {
        os_printf("%s malloc argv fail\n", __func__);
        goto CREATE_FAIL;
    }

    memset(targ->argv, 0, sizeof(targ->argv[0]) * argc);

    for (int i = 0; i < argc; ++i)
    {
        targ->argv[i] = os_malloc(strlen(argv[i]) + 1);

        if (!targ->argv[i])
        {
            os_printf("%s malloc argv[%d] fail\n", __func__, i);
            goto CREATE_FAIL;
        }

        memset(targ->argv[i], 0, strlen(argv[i]) + 1);

        strcpy(targ->argv[i], argv[i]);
    }


    ret = rtos_create_thread(&tid,
                             4,
                             func_name,
                             middle_layer_task,
                             1024 * 12,
                             (void *)targ);

    if (ret != 0)
    {
        os_printf("%s create task fail %d\n", __func__, ret);
        goto CREATE_FAIL;
    }

    return;

CREATE_FAIL:;

    if (targ)
    {
        if (targ->argv)
        {
            for (int i = 0; i < targ->argc; ++i)
            {
                if (targ->argv[i])
                {
                    os_free(targ->argv[i]);
                }
            }

            os_free(targ->argv);
        }

        os_free(targ);
    }

    return;
}


static int32_t recv_callback(uint8_t channel, uint8_t *buff, uint32_t len)
{
    uint8_t *send_buff = NULL;
    uint32_t i = 0;

    if (len)
    {
        os_printf("%s len %d\n", __func__, len);
        send_buff = (typeof(send_buff))os_malloc(1024 * 30);
        uint32_t index = 0;
        int32_t ret = 0;

        do
        {
            index = 0;

            do
            {
                ret = cs2_p2p_send_raw(channel, send_buff + index, 1024 * 30 - index);

                if (ret < 0)
                {
                    break;
                }

                index += ret;

                if (ret == 0)
                {
                    rtos_delay_milliseconds(100);
                }
            }
            while (index < 1024 * 30);
        }
        while (i++ < 10);
    }


    if (send_buff)
    {
        os_free(send_buff);
    }

    return 0;
}

int cs2_p2p_test_main(int argc, char **argv)
{
    int err = 0;

    char *did_dskey = NULL;
    char *apilicense_crckey = NULL;
    char *initstring_p2pkey = NULL;

    char *cmd_option;

    if (0)
    {

    }

#if CONFIG_CS2_P2P_SERVER
    else if (!strcmp(argv[0], "listentester"))
    {
        if (argc < 4)
        {
            os_printf("input param error\n");
            err = kParamErr;
            goto error;
        }

        did_dskey = argv[1];
        apilicense_crckey = argv[2];
        initstring_p2pkey = argv[3];

        char *tmp_argv[] =
        {
            [0] = "listentester",
            [1] = did_dskey,
            [2] = apilicense_crckey,
            [3] = initstring_p2pkey,
            [4] = "999",
        };

        //        listentester_main(sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);

        create_task(listentester_main, "listentester", sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);
    }
    else if (!strcmp(argv[0], "server_send_with_crc_main"))
    {
        if (argc < 4)
        {
            os_printf("input param error\n");
            err = kParamErr;
            goto error;
        }

        did_dskey = argv[1];
        apilicense_crckey = argv[2];
        initstring_p2pkey = argv[3];

        char *tmp_argv[] =
        {
            [0] = "listentester",
            [1] = did_dskey,
            [2] = apilicense_crckey,
            [3] = initstring_p2pkey,
            [4] = "9999",
        };
        //int picserver_main2(char *pDIDString, char *pAPILicense, char *pInitString, char *pRepeat, char *pWakeupKey, int32_t *(*recv_callback)(uint8_t *buff, uint32_t len))
        create_task(server_send_with_crc_main, "server_send_with_crc_main", sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);
    }
    else if (!strcmp(argv[0], "server_raw_send"))
    {
        //        create_raw_send_task();

        cs2_p2p_main_task_create(argv[1], argv[2], argv[3], recv_callback);
    }

#endif
#if CONFIG_CS2_P2P_CLIENT
    else if (!strcmp(argv[0], "connectiontester"))
    {
        if (argc < 4)
        {
            os_printf("input param error\n");
            err = kParamErr;
            goto error;
        }

        cmd_option = argv[1];
        did_dskey = argv[2];
        initstring_p2pkey = argv[3];


        char *tmp_argv[] =
        {
            [0] = "connectiontester",
            [1] = cmd_option,
            [2] = did_dskey,
            [3] = initstring_p2pkey,
            [4] = "3",//count
            [5] = "5",//delay
        };

        //        connectiontester_main(sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);
        create_task(connectiontester_main, "connectiontester", sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);
    }
    else if (!strcmp(argv[0], "readwritetester"))
    {
        if (argc < 4)
        {
            os_printf("input param error\n");
            err = kParamErr;
            goto error;
        }

        cmd_option = argv[1];
        did_dskey = argv[2];
        initstring_p2pkey = argv[3];


        char *tmp_argv[] =
        {
            [0] = "readwritetester",
            [1] = cmd_option,
            [2] = did_dskey,
            [3] = initstring_p2pkey,
            [4] = "1",
            [5] = argv[5],
            [6] = argv[6],
        };

        //        readwritetester_main(sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);
        create_task(readwritetester_main, "readwritetester", sizeof(tmp_argv) / sizeof(tmp_argv[0]), tmp_argv);
    }

#endif

    else
    {
        os_printf("unknow test case %s\n", argv[0]);
        return -1;
    }

    (void)apilicense_crckey;
    (void)cmd_option;

error:;
    return err;
}


