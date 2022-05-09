#include "iot_import.h"
#include "iot_export.h"
#include "lite-utils.h"
#include "lite-system.h"
#include <os/os.h>

#define PID_STR_MAXLEN              (64+1)
#define MID_STR_MAXLEN              (64+1)
#define PRODUCT_KEY_MAXLEN          (20+1)
#define DEVICE_NAME_MAXLEN          (32+1)
#define DEVICE_ID_MAXLEN            (64+1)
#define DEVICE_SECRET_MAXLEN        (32+1)
#define FIRMWARE_VERSION_MAXLEN     (32+1)

void *HAL_MutexCreate(void)
{
    beken_mutex_t mutex;

    if (kNoErr != rtos_init_mutex(&mutex)) {
        return NULL;
    }

    return mutex;
}

void HAL_MutexDestroy(_IN_ void *mutex)
{
    if (NULL != mutex)
        rtos_deinit_mutex((beken_mutex_t *)&mutex);
}

void HAL_MutexLock(_IN_ void *mutex)
{
    if (NULL != mutex)
        rtos_lock_mutex((beken_mutex_t *)&mutex);
}

void HAL_MutexUnlock(_IN_ void *mutex)
{
    if (NULL != mutex)
        rtos_unlock_mutex((beken_mutex_t *)&mutex);
}

uint64_t HAL_UptimeMs(void)
{
    return rtos_get_time();
}


int HAL_Snprintf(_IN_ char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int     rc;

    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);

    return rc;
}


int HAL_Vsnprintf(_IN_ char *str, _IN_ const int len, _IN_ const char *format, va_list ap)
{
    return vsnprintf(str, len, format, ap);
}

void HAL_SleepMs(_IN_ uint32_t ms)
{
    rtos_delay_milliseconds(ms);
}

void HAL_Srandom(uint32_t seed)
{
    //srandom(seed);
    srand(seed);
}

uint32_t HAL_Random(uint32_t region)
{
    return (region > 0) ? (rand() % region) : 0;
    //return 0;
}

void *LITE_malloc_routine(int size, ...)
{
    return LITE_malloc(size);
}

void *LITE_calloc_routine(size_t n, size_t s, ...)
{
    return LITE_malloc(n * s);
}

void LITE_free_routine(void *ptr)
{
    LITE_free(ptr);
}


#if defined(MQTT_COMM_ENABLED)
int IOT_SetupConnInfo(const char *product_key,
                      const char *device_name,
                      const char *device_secret,
                      void **info_ptr)
{
    int                 rc = -1;

    if (!info_ptr) {
        log_err("Invalid argument, info_ptr = %p.\n", info_ptr);
        return -1;
    }

    STRING_PTR_SANITY_CHECK(product_key, -1);
    STRING_PTR_SANITY_CHECK(device_name, -1);
    STRING_PTR_SANITY_CHECK(device_secret, -1);

    iotx_device_info_init();
    iotx_device_info_set(product_key, device_name, device_secret);

    rc = iotx_guider_authenticate();
    if (rc == 0) {
        *info_ptr = (void *)iotx_conn_info_get();
        return 0;
    } else {
        *info_ptr = NULL;
        return -1;
    }
}

#ifdef MQTT_ID2_AUTH
int IOT_SetupConnInfoSecure(const char *product_key,
                            const char *device_name,
                            const char *device_secret,
                            void **info_ptr)
{
    int rc;

    STRING_PTR_SANITY_CHECK(product_key, -1);
    STRING_PTR_SANITY_CHECK(device_name, -1);
    STRING_PTR_SANITY_CHECK(device_secret, -1);
    POINTER_SANITY_CHECK(info_ptr, -1);
    iotx_device_info_init();
    iotx_device_info_set(product_key, device_name, device_secret);

    rc = iotx_guider_id2_authenticate();
    if (rc == 0) {
        *info_ptr = (void *)iotx_conn_info_get();
    } else {
        *info_ptr = NULL;
    }

    return rc;
}
#endif  /* #ifdef MQTT_ID2_AUTH */
#endif  /* #if defined(MQTT_COMM_ENABLED)   */