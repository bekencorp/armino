系统日志
===================

:link_to_translation:`en:[English]`

本文档主要介绍Armino平台BK7256双核log机制

 - BK7256 CPU0的log通过串口uart1输出(默认波特率为115200)
 - BK7256 CPU1的log通过mailbox转发到CPU0串口uart1输出
 - CPU1 log带cpu1标签（异常log除外）
 - 由于内存缓冲区的限制，每条log数据的字节数，要小于128字节。超过这个大小的log都会被shell 模块丢弃，并输出一条 !!some LOGs discarded!! 如果log数量太多，来不及输出导致log堆积，缓冲区用完，也会输出这条提示字符串。
 - 通过串口输入log命令查看当前log配置
 - 通过串口输入log 1 3 1命令设置log为同步机制
 - log 1 3 1 命令第一个参数为echo开关 (0/1)，第二个参数为log级别(0~6)，第三个参数为同步开关(0异步,1同步)

BK7256 双核log机制
------------------------

.. figure:: ../../../_static/log_system.png
    :align: center
    :alt: BK7256 双核log机制
    :figclass: align-center

    BK7256 双核log机制


API说明
------------------------

 - Shell 模块支持log 等级机制，支持APP模块log输出的控制机制。
 - APP模块log的输出控制，主要用在电脑的显示界面上，而不是在程序内部。

API 中支持log等级，模块名字等参数。

 - Log的等级目前设计了6级（0~5），每级的含义如下所述，系统配置的log输出等级为0时，关闭所有log的输出。
 - 系统配置的log输出等级为N时，可以输出1~N 等级的log，(N+1)~5 等级的log 不能输出。
 - Log的等级定义如下：

::

    #define BK_LOG_NONE       0      /*!< No log output */
    #define BK_LOG_ERROR      1      /*!< Critical errors, software module can not recover on its own */
    #define BK_LOG_WARN       2      /*!< Error conditions from which recovery measures have been taken */
    #define BK_LOG_INFO       3      /*!< Information messages which describe normal flow of events */
    #define BK_LOG_DEBUG      4      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    #define BK_LOG_VERBOSE    5      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    #define LOG_LEVEL         BK_LOG_INFO    /* 配置系统的log输出等级 */


如下是经过封装后的Log 输出API：
    - Tag：表示输出log的模块名。
    - format：是log输出的格式字符串。

::

    BK_LOGE( tag, format, ... );       // Tag所指的模块输出 ERROR 等级的log
    BK_LOGW( tag, format, ... );       // Tag所指的模块输出 Warning 等级的log
    BK_LOGI( tag, format, ... );       // Tag所指的模块输出 Information 等级的log
    BK_LOGD( tag, format, ... );       // Tag所指的模块输出 Debug等级的log
    BK_LOGV( tag, format, ... );       // Tag所指的模块输出 一些不重要的log
    BK_ASSERT(exp);                    // 如果 exp表达式为假，关闭系统中断，shell会输出当前的函数名，当前的行号，当前的系统时间等信息。然后输出系统的dump数据（CPU寄存器，内存，栈等信息）
    BK_ASSERT_EX(exp, format, ... );  // 如果 exp表达式为假，关闭系统中断，shell会输出当前的函数名，当前的行号，当前的系统时间，format格式串的相关变量，等信息。然后输出系统的dump数据（CPU寄存器，内存，栈等信息）
    BK_DUMP_OUT(format, ... );         // 关中断下，用format格式的方式输出相关信息。
    BK_DUMP_RAW_OUT(buf, len);         // 关中断下，输出buf 中len长度的数据。

