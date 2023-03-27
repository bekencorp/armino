fault detection & handling
==================================

:link_to_translation:`zh_CN:[中文]`

it is required for a robust application programming to detect and handle runtime error in time. This section introduces SDK macros for fault dection & handing, and some error code definitions.

error code definitions
**********************************

The majority of current SDK APIs have been returned error code defined by bk_err_t type, any new added API will return error code defined by bk_err_t type, except for the void prototype APIs.

There are following error types defined in SDK:

 - BK_OK, define with 0, means no error or success.
 - general error codes defined in bk_err.h, for exampel, BK_ERR_NO_MEM means memory insufficient.
 - module error codes, defined like BK_ERR_MOD_xxx, where MOD specified the module generated the error. for example, BK_ERR_WIFI_STA_NOT_CONFIG indicates the error from WiFi module, error cause is STA_NOT_CONFIG.

Two general rules for SDK error code:

 - minus value for all error codes.
 - code value is unique in SDK, each module defines the base value for error code in bk_err.h, then allocate the error codes in the range for its module in the module API header file.

.. note::

    APIs from 3rd part will not return error codes of bk_err_t type, such as socket APIs, etc.

error code returns
************************************

Functions should return different error code for different fault location, even with the same fault cause, to facilitate debug.

.. note::

    In some SDK API, some return codes described as 'others', that usually means error code returned from lower level functions or from other modules.

Macros for error detection
************************************

we recommend using the Macros defined in SKD for error detection.

BK_LOG_ON_ERR
------------------------------------

when the macro parameter (maybe the result of a expression) is not BK_OK, the macro code will just output the running function name, current line number, error code, then go on with the called function.

BK_RETURN_ON_ERR
------------------------------------

when the macro parameter (maybe the result of a expression) is not BK_OK, the macro will return the parameter as the error code of the called function.

BK_ABORT_ON_ERR
------------------------------------

when the macro parameter is not BK_OK, macro code will call bk_reboot() to restart system.


