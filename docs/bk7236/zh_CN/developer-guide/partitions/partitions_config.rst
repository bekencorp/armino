.. _partitions_config:

分区配置
========================

.. note::

  本节中 `BL1` 特指固化在芯片中的 BootROM，`BL2` 特指 Bootloader，即 mcuboot。

默认系统分区配置表为 ``middleware/boards/bk7236/partitions/sys_partitions.json``，
默认普通分区配置表为 ``middleware/boards/bk7236/partitions/partitions.json``，
如果默认配置无法满足您的需求，您可以在项目配置目录下定义自己的分区配置表，
构建系统会用项目分区表覆盖默认分区表。

通常您仅需要定义普通分区表。

快速配置分区
----------------------------

配置分区最简单的方法是从配置模板中拷贝一份配置到您的工程中，然后基于配置模板进行修改。

配置模板放置在示例
project/examples/platform/partition/config/partitions 下：

 - partitions.legacyboot.ota - 下载模式分区配置，包含 BL2，支持 OTA。
 - partitions.legacyboot - 下载模式分区配置，不包含 BL2，不支持 OTA。通常在开发阶段调试用。
 - partitions.secureboot.ota - 安全启动分区配置，包含 BL2，支持 OTA。
 - partitions.secureboot - 安全启动分区配置，不包含 BL2，不支持 OTA。通常在开发阶段调试用。
 - partitions.new_partition - 增加新的用户分区与系统配置分区。


通常可能需要修改的项有：

 - 如果 FLASH 大小不为 4M，则配置 ``flash_size`` 为合适大小。
 - 如果 FLASH 加密使能，则需要更新 ``aes_key``。
 - 如有必要，调整分区大小。
 - 如果需要支持 OTA，则配置合适的版本号。

