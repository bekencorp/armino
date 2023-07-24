flashdb Cli
================

:link_to_translation:`zh_CN:[English]`

1. Introduction to Flashdb
------------------------------------------------------------------------------
	FlashDB is an ultra-lightweight embedded database that focuses on providing data storage solutions for embedded products. Different from traditional database based on file system, FlashDB combines the features of Flash and has strong performance and reliability. And under the premise of ensuring extremely low resource occupation, the service life of Flash should be extended as much as possible.
	
	FlashDB provides two database modes:
	 - Key-value database: It is a non-relational database that stores data as a collection of key-value pairs, where the key is used as a unique identifier. KVDB has simple operation and strong scalability.
	 - Time Series Database: Time Series Database (TSDB), which stores data in time sequence. TSDB data has a timestamp, a large amount of data storage, and high insertion and query performance.

	Api:	 
	https://armink.github.io/FlashDB/#/api
	
	Introduction to demo:	 
	http://armink.gitee.io/flashdb/#/demo-details

	Get more information:	 	
	https://armink.gitee.io/flashdb/#/README

.. important::
    The kvdb of FlashDB is equal easyflash, so if you use FlashDB, we suggest close easyflash.
	
2. path
--------------------------
	path: ``\components\bk_cli\cli_flashdb.c``
	
3. Dependency configuration
------------------------------------------------------------------------------

	+-----------------------------------+------------------------------------------------+
	|CONFIG                             | value                                          |
	+===================================+================================================+
	|CONFIG_FLASHDB                     | y,                                             |
	|                                   | switch of flashdb                              |
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_USING_KVDB          | y,                                             |
	|                                   | switch of kvdb                                 |
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_KVDB_START_ADDR     | 0x3EF000,                                      |
	|                                   | the start addr in flash of kvdb, you can change|
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_KVDB_SIZE           | 0x2000,                                        |
	|                                   | the size in flash of kvdb, you can change      |
	+-----------------------------------+------------------------------------------------+
	|CONFIG_NTP_SYNC_RTC                | y,                                             |
	|                                   | switch of ntp, in order to give time to tsdb   |
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_USING_TSDB          | y,                                             |
	|                                   | switch of tsdb                                 |
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_TSDB_START_ADDR     | 0x3F1000,                                      |
	|                                   | the start addr in flash of tsdb, you can change|
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_TSDB_SIZE           | 0x4000,                                        |
	|                                   | the size in flash of kvdb, you can change      |
	+-----------------------------------+------------------------------------------------+
	|CONFIG_FLASHDB_DEMO                | y,                                             |
	|                                   | switch of flashdb test demo                    |
	+-----------------------------------+------------------------------------------------+

4. Introduction to the cli command
------------------------------------------------------------------------------
	The supported commands are as follows:

	1.flashdb_init
	 - Function description
	   Initialize kvdb and tsdb in flashdb.
		
	2.flashdb_kvdb
	 - Function description
	   This example mainly demonstrates the basic functions of kvdb, including write and read of blob and string types, and traversal of kvdb. Please refer to:
	   http://armink.gitee.io/flashdb/#/zh-cn/sample-kvdb-basic

    3.flashdb_tsdb
	 - Function description
	   This example mainly demonstrates the basic functions of TSDB, including the addition, query, and status modification functions of TSL (time series record). Please refer to:
	   http://armink.gitee.io/flashdb/#/zh-cn/sample-tsdb-basic