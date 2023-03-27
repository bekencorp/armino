#include "osi_system.h"

static beken_semaphore_t s_log_sema = 0;


int pthread_mutex_init(beken_semaphore_t *sema, void *arg)
{
    bk_err_t ret;
    ret = rtos_init_semaphore(sema, 1);
    //    assert_err(ret, "%s\n");

    return ret;
}


int pthread_mutex_lock(beken_semaphore_t *sema)
{
    bk_err_t ret;
    ret = rtos_get_semaphore(sema, BEKEN_WAIT_FOREVER);

    return ret;
}

int pthread_mutex_unlock(beken_semaphore_t *sema)
{
    bk_err_t ret;
    ret = rtos_set_semaphore(sema);

    return ret;
}

int pthread_mutex_destroy(beken_semaphore_t *sema)
{
    rtos_deinit_semaphore(sema);

    return 0;
}


typedef struct
{
    beken_thread_t tid;
    void *(*func) (void *);
    void *arg;
} middle_beken_thread_arg_t;

static void middle_beken_thread(void *arg)
{
    middle_beken_thread_arg_t *targ = (typeof(targ))arg;

    if (targ->func)
    {
        targ->func(targ->arg);
    }


    os_free(targ);
    rtos_delete_thread(NULL);

    return;
}

int pthread_create_ext(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg, char *func_name)
{
    bk_err_t ret = 0;
    beken_thread_t tid;
    middle_beken_thread_arg_t *targ;

    targ = (typeof(targ))os_malloc(sizeof(*targ));

    if (!targ)
    {
        os_printf("%s malloc fail\n");
        return -1;
    }

    targ->func = start_routine;
    targ->arg = arg;


    ret = rtos_create_thread(&tid,
                             4,
                             func_name,
                             middle_beken_thread,
                             1024 * 8,
                             (void *)targ);

    if (ret != 0)
    {
        os_printf("%s rtos_create_thread fail %d\n", __func__, ret);
        goto PTH_FAIL;
    }


    *thread = tid;
    targ->tid = tid;

    return ret;

PTH_FAIL:
    ret = -1;
    *thread = 0;
    os_free(targ);

    return ret;
}

pthread_t pthread_self(void)
{
    beken_thread_t *tid = rtos_get_current_thread();
    return *tid;
}

int pthread_join(pthread_t thread, void **retval)
{
    return rtos_thread_join(&thread);
}

int pthread_detach(pthread_t thread)
{
    os_printf("\n!!!! %s not impl, please impl with armino api or remodify sample code !!!!\n", __func__);
    return 0;
}

void usleep(uint32_t us)
{
    rtos_delay_milliseconds(us / 1000);
}

int gettimeofday_fake(struct timeval *tv, struct timezone *tz)
{
    beken_time_t t = rtos_get_time();

    if (tv)
    {
        memset(tv, 0, sizeof(*tv));
    }

    if (tz)
    {
        memset(tz, 0, sizeof(*tz));
    }

    tv->tv_sec = t / 1000;
    tv->tv_usec = (t % 1000) * 1000;

    return 0;
}

struct tm *localtime_r_fake(const time_t *timep, struct tm *result)
{
    /*
     struct tm
{
  int   tm_sec;
  int   tm_min;
  int   tm_hour;
  int   tm_mday;
  int   tm_mon;
  int   tm_year;
  int   tm_wday;
  int   tm_yday;
  int   tm_isdst;
     */
    uint32_t sec = *timep;

    if (result)
    {
        memset(result, 0, sizeof(*result));
    }

    result->tm_mon = sec / 60 / 60 / 24 / 30;
    result->tm_mday = (sec / 60 / 60 / 24) % 30 + 1;
    result->tm_hour = (sec / 60 / 60 % 24);
    result->tm_min = (sec / 60) % 60;
    result->tm_sec = sec % 60;

    return result;
}

#if 0
struct hostent *gethostbyname(const char *name)
{
    os_printf("!!!! %s not impl, please impl with armino api or remodify sample code !!!!\n", __func__);
    return NULL;
}
#endif


void osi_system_init(void)
{
    if (bk_get_printf_sync())
    {
        rtos_init_semaphore(&s_log_sema, 1);
    }
}

void osi_log_lock(void)
{
    if (s_log_sema)
    {
        rtos_get_semaphore(&s_log_sema, BEKEN_WAIT_FOREVER);
    }
}

void osi_log_unlock(void)
{
    if (s_log_sema)
    {
        rtos_set_semaphore(&s_log_sema);
    }
}
