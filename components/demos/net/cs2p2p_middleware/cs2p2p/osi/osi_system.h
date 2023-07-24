#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <components/log.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>

#include <lwip/sockets.h>
#include <lwip/netdb.h>

typedef     beken_thread_t pthread_t;
typedef     beken_semaphore_t pthread_mutex_t;

typedef     pthread_t                           my_threadid;
typedef     pthread_mutex_t                     my_Thread_mutex_t;


#define     my_Mutex_Init(mutex)                pthread_mutex_init(&mutex, NULL)
#define     my_Mutex_Lock(mutex)                pthread_mutex_lock(&mutex)
#define     my_Mutex_UnLock(mutex)              pthread_mutex_unlock(&mutex)
#define     my_Mutex_Close(mutex)               pthread_mutex_destroy(&mutex)
#define     my_SocketClose(skt)                 closesocket(skt)
#define     my_Thread_exit(a)                   do{return (void *)a;} while(0)
#define     SNPRINTF                            snprintf
// #define     LocalTime_R(a, b)                   localtime_r(a, b)
#define     GmTime_R(a, b)                      //gmtime_r(a, b)
#define     MKDIR(Directory, mode)              //mkdir(Directory, mode)


#define setbuf(...)
#define printf(fmt, ...)        do{osi_log_lock(); os_printf("\n" fmt, ##__VA_ARGS__); osi_log_unlock();}while(0)
#define st_info(fmt, ...)       do{osi_log_lock(); os_printf("\n" fmt, ##__VA_ARGS__); osi_log_unlock();}while(0)
#define st_debug(fmt, ...)      do{osi_log_lock(); os_printf("\n" fmt, ##__VA_ARGS__); osi_log_unlock();}while(0)
#define st_debug_T(fmt, ...)    do{osi_log_lock(); os_printf("\n" fmt, ##__VA_ARGS__); osi_log_unlock();}while(0)
#define st_info_T(fmt, ...)     do{osi_log_lock(); os_printf("\n" fmt, ##__VA_ARGS__); osi_log_unlock();}while(0)
//#define error(x) os_printf("%s %s\n", __func__, x)
#define perror(x)               do{osi_log_lock(); os_printf("\n%s\n", x); osi_log_unlock();}while(0)
#define gettimeofday gettimeofday_fake
#define localtime_r localtime_r_fake
#define pthread_create(thread, attr, start_routine, arg) pthread_create_ext(thread, attr, start_routine, arg, #start_routine)



#define PROGRAM_NAME "p2p_test"
#define BUILD_DATE __DATE__

typedef long time_t;
typedef long suseconds_t;
typedef uint32_t pthread_attr_t;

#if 0
struct timeval
{
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* and microseconds */
};

struct timezone
{

};

struct tm
{

};

struct hostent
{
    const char  *h_name;    /* official name of host */
    char    **h_aliases;    /* alias list */
    short   h_addrtype; /* host address type */
    short   h_length;   /* length of address */
    char    **h_addr_list;  /* list of addresses from name server */
#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};
#endif

int pthread_mutex_init(pthread_mutex_t *sema, void *arg);
int pthread_mutex_destroy(pthread_mutex_t *sema);
int pthread_mutex_lock(pthread_mutex_t *sema);
int pthread_mutex_unlock(pthread_mutex_t *sema);
int pthread_create_ext(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg, char *func_name);
pthread_t pthread_self(void);
int pthread_join(pthread_t thread, void **retval);
int pthread_detach(pthread_t thread);
int gettimeofday_fake(struct timeval *tv, struct timezone *tz);
struct tm *localtime_r_fake(const time_t *timep, struct tm *result);
void usleep(uint32_t us);
struct hostent *gethostbyname(const char *name);

void osi_system_init(void);
void osi_log_lock(void);
void osi_log_unlock(void);
