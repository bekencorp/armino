log output
===================

:link_to_translation:`en:[English]`

Here introduces the log output method for Armino platform, it also works for dual cores SoC.

 - log data from CPU0 are output through UART1.(The defualt baudrate of UART1 is 115.2kbps)
 - log data from CPU1 are forwarded to CPU0 via mailbox, and then output to UART1 by CPU0.
 - log data from CPU1 with the prefix TAG of "CPU1", (exception logs don't have this TAG)
 - because of the buffer size constraints, each log should not exceed 128 bytes in size, otherwise the log will be discarded, and replaced with a message of "!!some LOGs discarded!!". This message will also be output when log buffers were exhausted because of too many pending logs.
 - To check the log settings in the Armino, send the command of 'log' with no params in the input device.
 - To set the log work mode, send the command of 'log' with 1~4 params in the input device.
 - log [1 [3 [0 [0]]]], it is the default setting after reset, the first parameter is the input echo switch(0: disable, 1:enable echo), the 2nd param is the lowest log level that can be output(level 0~5, 5 is the lowest level), the 3rd param controls the log work mode(0:asynchronous,1:synchronous), the 4th param controls 'modlog' work mechnism, the module list is the whitelist or blacklist (0: blacklist, 1: whitelist). This command contains 1~4 params, if the param3 is provided, then param1~2 must be also provided, but param4 can be omitted(so no changes to this setting).

log work flow for dual core system
------------------------------------------

.. figure:: ../../../_static/log_system.png
    :align: center
    :alt: log work flow for BK7256 dual core.
    :figclass: align-center

    log work mechinsm for BK7256 dual cores.


API desriptions
-------------------------------------------

 - Shell API provides two types of log output control,one is the log level, the other is the log source specified by module-name or TAG.
 - these log control types decide together whether the log can really be sent to UART1 device.

API therefore with 2 params for log level, log Tag(module produced the log).

 - There are 6 log levels, as described below. log level 0 means no log output.
 - when the log level is set to N, then log data with level 1~N can be output to UART1, level (N+1)~5 log can't be output.
 - Log level definitions:

::

    #define BK_LOG_NONE       0      /*!< No log output */
    #define BK_LOG_ERROR      1      /*!< Critical errors, software module can not recover on its own */
    #define BK_LOG_WARN       2      /*!< Error conditions from which recovery measures have been taken */
    #define BK_LOG_INFO       3      /*!< Information messages which describe normal flow of events */
    #define BK_LOG_DEBUG      4      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    #define BK_LOG_VERBOSE    5      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    #define LOG_LEVEL         BK_LOG_INFO    /* log level configure. */


Following is the Log APIs: the suffix of the API implies the level of this log.
    - Tag: point out the module which produced the log. Module list will control whether this log can be output.
    - format: format control for log data.

::

    BK_LOGE( tag, format, ... );       // 'Tag' module output 'ERROR' level log.
    BK_LOGW( tag, format, ... );       // 'Tag' module output  'Warning' level log
    BK_LOGI( tag, format, ... );       // 'Tag' module output  'Information' level log
    BK_LOGD( tag, format, ... );       // 'Tag' module output  'Debug' level log
    BK_LOGV( tag, format, ... );       // 'Tag' module output  'VERBOSE' level log
    BK_ASSERT(exp);                    // if exp is false, API will disable system interrupts, output the calling function name, line number, system time. Then begin to dump data (include CPU registers, memory, task stack)
    BK_ASSERT_EX(exp, format, ... );   // if exp is false, API will disable system interrupts, output the calling function name, line number, system time, format some data. Then begin to dump data (include CPU registers, memory, task stack)
    BK_DUMP_OUT(format, ... );         // format the log data, then output log data with interrupt disabled.
    BK_DUMP_RAW_OUT(buf, len);         // output buf data in 'len' bytes after interrupt disabled.

