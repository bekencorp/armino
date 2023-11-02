#include <components/system.h>
#include <os/os.h>
#include "modules/dm_ble_types.h"
#include "modules/dm_ble.h"
#include "modules/dm_bt.h"
#include "dm_ble_demo.h"

static beken_semaphore_t s_bluetooth_sema = NULL;
static uint8_t s_bluetooth_ready;

static uint32_t bluetooth_event_cb(ble_event_enum_t event, void *param)
{
    switch (event)
    {
    case BK_DM_BLE_EVENT_STACK_OK:
    {
        if (!s_bluetooth_ready)
        {
            s_bluetooth_ready = 1;

            os_printf("bluetooth stack ok\n");

            if (s_bluetooth_sema != NULL)
            {
                rtos_set_semaphore( &s_bluetooth_sema );
            }
        }
    }
    break;


    default:
        break;
    }

    return 0;
}

int bluetooth_app_init(void)
{
    int retval = 0;

    retval = rtos_init_semaphore(&s_bluetooth_sema, 1);

    if (retval != kNoErr)
    {
        os_printf("%s: init sema err\n", __func__, retval);
        return -1;
    }

    bk_ble_set_event_callback(bluetooth_event_cb);

    return 0;
}

int bluetooth_app_main(void)
{
    int retval = 0;

    retval = rtos_get_semaphore(&s_bluetooth_sema, 3000);

    if (retval != kNoErr)
    {
        os_printf("%s: wait bk_ble_set_advertising_params failed. ret: 0x%04X\n", __func__, retval);
        goto error;
    }

#if CONFIG_A2DP_SINK_DEMO && !CONFIG_A2DP_SOURCE_DEMO

    extern int a2dp_sink_demo_init(uint8_t aac_supported);
    a2dp_sink_demo_init(0);

#elif !CONFIG_A2DP_SINK_DEMO && CONFIG_A2DP_SOURCE_DEMO

#else
    os_printf("%s CONFIG_A2DP_SINK_DEMO CONFIG_A2DP_SOURCE_DEMO can't both init now !\n", __func__);

#endif

#if CONFIG_BLE
    dm_ble_demo_main();
#endif

    if (s_bluetooth_sema)
    {
        rtos_deinit_semaphore(&s_bluetooth_sema);
    }

    return 0;

error:;

    if (s_bluetooth_sema)
    {
        rtos_deinit_semaphore(&s_bluetooth_sema);
    }

    return -1;
}
