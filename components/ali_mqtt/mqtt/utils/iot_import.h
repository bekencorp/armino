#ifndef __IOT_IMPORT_H__
#define __IOT_IMPORT_H__
#if defined(__cplusplus)
extern "C" {
#endif

#define _IN_                /* 参数前缀,    表示后面跟着的参数是一个输入参数 */
#define _OU_                /* 参数前缀,    表示后面跟着的参数是一个输出参数 */
#define _IN_OPT_            /* 参数前缀,    表示后面跟着的参数是一个可选的输入参数 */
#define _OU_OPT_            /* 参数前缀,    表示后面跟着的参数是一个可选的输出参数 */
#define IOT_TRUE    (1)     /* 逻辑真 */
#define IOT_FALSE   (0)     /* 逻辑假 */


#define PID_STRLEN_MAX           (64)
#define MID_STRLEN_MAX           (64)
#define IOTX_URI_MAX_LEN         (135)  /* IoTx CoAP/HTTP URI & MQTT topic maximal length */

#define PLATFORM_WAIT_INFINITE (~0)

/// bk adapter code
#include <components/log.h>
#include <common/bk_assert.h>
#include <os/mem.h>

#define MQTT_TAG "mqtt"  /**< MQTT log tag*/

#define log_info(...) BK_LOGI(MQTT_TAG, ##__VA_ARGS__)   /**< Output MQTT Info log */
#define log_warning(...) BK_LOGW(MQTT_TAG, ##__VA_ARGS__)   /**< Output MQTT Warning log */
#define log_err(...) BK_LOGE(MQTT_TAG, ##__VA_ARGS__)   /**< Output MQTT Error log */
#define log_debug(...) BK_LOGI(MQTT_TAG, ##__VA_ARGS__)   /**< Output MQTT Debug log */

void *os_realloc(void *ptr, size_t size);

#define LITE_calloc(num, size, ...)     ((size) && (num) > (~( unsigned int) 0)/(size))? 0 : os_zalloc((nmemb)*(size))
#define LITE_malloc(size, ...)          os_malloc(size)
#define LITE_realloc(ptr, size, ...)    os_realloc(ptr, size)
#define LITE_free(ptr)                  os_free(ptr)


#define HAL_Malloc(size)                os_malloc(size)
#define HAL_Free(ptr)                   os_free(ptr)

#define POINTER_SANITY_CHECK(ptr, err) \
    do { \
        if (NULL == (ptr)) { \
            log_err("Invalid argument, %s = %p", #ptr, ptr); \
            return (err); \
        } \
    } while(0)

#define STRING_PTR_SANITY_CHECK(ptr, err) \
    do { \
        if (NULL == (ptr)) { \
            log_err("Invalid argument, %s = %p", #ptr, (ptr)); \
            return (err); \
        } \
        if (0 == strlen((ptr))) { \
            log_err("Invalid argument, %s = '%s'", #ptr, (ptr)); \
            return (err); \
        } \
    } while(0)


void       *LITE_malloc_routine(int size, ...);
void        LITE_free_routine(void *ptr);
void       *LITE_calloc_routine(size_t n, size_t s, ...);

/**
 * @brief   创建一个互斥锁
 *
 * @retval  NULL : 创建互斥锁失败
 * @retval  非空 : 创建互斥锁成功, 返回值即该互斥锁的句柄
 */
void *HAL_MutexCreate(void);

/**
 * @brief   销毁一个互斥锁, 释放其所占用的资源
 *
 * @param   mutex : 被销毁互斥锁的句柄
 * @return  无返回值
 */
void HAL_MutexDestroy(_IN_ void *mutex);

/**
 * @brief   给互斥锁加锁, 开始临界代码段
 *
 * @param   mutex : 互斥锁的句柄
 * @return  无返回值
 */
void HAL_MutexLock(_IN_ void *mutex);

/**
 * @brief   给互斥锁解锁, 结束临界代码段
 *
 * @param   mutex : 互斥锁的句柄
 * @return  无返回值
 */
void HAL_MutexUnlock(_IN_ void *mutex);

/**
 * @brief   获取自加电以来到当前时间, 已经过去的毫秒数
 *
 * @return  加电以来的毫秒数
 */
uint64_t HAL_UptimeMs(void);

/**
 * @brief   按照指定格式将一系列变量输出到指定的字符串, 变参是变量的形式
 *
 * @param   str : 存放输出结果的字符串
 * @param   len : 可以向目标字符串中输出的最大长度, 单位是字节, 包含结束符'\0'
 * @param   fmt : 打印格式参数, 可以内含%s和%d等占位符, 语法等同C标准函数printf()
 * @param   ... : 变参列表, 用于替换fmt中内含的%s和%d等占位符
 * @return  fmt格式字符串中用变参替换掉所有占位符之后的实际字符串长度, 可能超过入参len
 */
int HAL_Snprintf(_OU_ char *str, _IN_ const int len, _IN_ const char *fmt, ...);

/**
 * @brief   按照指定格式将一系列变量输出到指定的字符串, 变参是`va_list`的形式
 *
 * @param   str : 存放输出结果的字符串
 * @param   len : 可以向目标字符串中输出的最大长度, 单位是字节, 包含结束符'\0'
 * @param   fmt : 打印格式参数, 可以内含%s和%d等占位符, 语法等同C标准函数printf()
 * @param   ap :  变参列表, 用于替换fmt中内含的%s和%d等占位符
 * @return  fmt格式字符串中用变参替换掉所有占位符之后的实际字符串长度, 可能超过入参len
 */
int HAL_Vsnprintf(_OU_ char *str, _IN_ const int len, _IN_ const char *fmt, _IN_ va_list ap);

/**
 * @brief   使调用线程睡眠入参ms指定的毫秒数
 *
 * @param   ms : 需要睡眠的时间长度, 单位是毫秒
 * @return  无返回值
 */
void HAL_SleepMs(_IN_ uint32_t ms);

/**
 * @brief   设置一个伪随机数种子, 影响HAL_Random()的返回值
 *
 * @param   seed : 被设置的种子参数
 * @return  无返回值
 */
void HAL_Srandom(_IN_ uint32_t seed);

/**
 * @brief   获取指定范围内的一个随机无符号整数
 *
 * @param   region : 给定一个范围, 返回值将在[0, region]之间随机分布
 * @return  随机无符号整数
 */
uint32_t HAL_Random(_IN_ uint32_t region);

#if defined(__cplusplus)
}
#endif
#endif  /* __IOT_IMPORT_H__ */