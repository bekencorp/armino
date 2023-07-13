分区修改规范
========================

:link_to_translation:`en:[English]`

一.物理地址与逻辑地址的关系介绍
--------------------------------

为确保存储在flash中数据的准确性，每32个字节会增加2个字节crc校验位（crc检验由硬件自动完成），从而构成的物理地址（flash视角）会比逻辑地址（cpu视角）多2个字节，对比关系见下图1：（左侧为cpu视角，右侧为flash视角，红色字体为crc校验）

.. figure:: ../../../_static/logic.png
    :align: center
    :alt: logic
    :figclass: align-center

    图1物理地址与逻辑地址的对比

二.分区修改的组成
----------------------------
修改分区涉及到四个部分:

.. important::
  - app的分区表制作
  - 打包脚本的配置文件
  - 链接脚本（ld）的制作
  - bootloader的分区表制作

三.分区修改作用域
----------------------------

.. important::
  - 制作新的项目工程时，需要重做分区
  - 修改分区表时，需要重做分区
  - 修改链接脚本（ld）时，需要重做分区

四．案例
----------------------------

案例1：以bk7237为例，当用户在SDK基础上加入代码后，出现以下编译错误时见图2，该如何操作？

.. figure:: ../../../_static/overflow.png
    :align: center
    :alt: overflow
    :figclass: align-center

    图2编译报错图示

有两个解决方案：

.. important::
  - 首先可以手动将middleware/soc/bk7237/bk7237_bsp.ld的对应的size范围增大（ld中最后几行）。
  - 方案一：如果超出的code size很小，可关闭一些不需要的功能，即在middleware/soc/bk7237/bk7237.defconfig关闭相应的宏配置，减小code size；如果超出的code size很大，则只能实行方案二。
  - 方案二：如果超出的code size很大，则需要重新修改分区表，

.. note::

  建议优先选择方案一，方案二涉及修改的点比较多。

分区表制作步骤如下：

- 1.app分区表制作：

  flash物理总长度为4M，需在middleware/boards/bk7237/vnd_flash/vnd_flash.c设定各个分区的partition_start_addr(物理地址)和partition_length(物理长度)见图3黄色区域;

.. figure:: ../../../_static/partition.png
    :align: center
    :alt: partition
    :figclass: align-center

    图3分区表(partition)图示

- 2.打包脚本配置：

  将middleware/boards/bk7237/configuration.json中的start_addr和size根据图3分区表中各分区大小修改即可，（其中start_addr = partition_start_addr，
  size = partition_length）见图4黄色区域；

.. figure:: ../../../_static/package_37.png
    :align: center
    :alt: package_37
    :figclass: align-center

    图4打包脚本配置图示

- 3.链接脚本配置：（bk7237是单核，需配置bk7237_bsp.sag）

bk7237_bsp.sag配置如下：

  - middleware/soc/bk7256/bk7237_bsp.sag的LMA和length根据图3分区表中Application和Application1的partition_start_addr和partition_length计算得出，见图5黄色区域；

.. figure:: ../../../_static/app0_sag_37.png
    :align: center
    :alt: app0_sag_37
    :figclass: align-center

    图5链接脚本配置(bk7237_bsp.sag)图示

.. note::

  计算规则：
    - bk7237_bsp.sag中CPU0_LOAD1的LMA（cpu视角0x10000）=  Application的partition_start_addr \*32/34;

    - bk7237_bsp.sag中CPU0_LOAD1的length（cpu视角0x8800） =  0x8800;

    - bk7237_bsp.sag中CPU0_LOAD2的LMA（cpu视角0x18800）=  CPU0_LOAD1的LMA + 0x8800;

    - bk7237_bsp.sag中CPU0_LOAD2的length（cpu视角0x217800） =  (Application的partition_length + Application1的partition_length)\*32/34 - 0x8800;


- 4.使用文档《RISC_V》中关于Andes sag转ld文件方法将bk7237_bsp.sag和bk7237_cp1_bsp.sag转化为对应的ld文件;

.. note::

  该步骤4仅限新建工程；已有工程不需要该步骤,建议直接修改对应ld文件

- 5.(双核需要执行步骤5，单核不需要，此处不需要)------需将middleware/soc/bk7237/bk7237.defconfig中CPU1核的偏移量重置一下 CONFIG_SLAVE_CORE_OFFSET=1835008 （十进制格式）(CONFIG_SLAVE_CORE_OFFSET 1835008 == 0x1C0000 in bk7237_cp1_bsp.sag LMA)

.. note::

  当图3分区表(partition)中ota区域的起始地址没有变化，则步骤6不需要执行。
  
- 6.使用tools/env_tools/rt_partition_tool/rt_partition_tool.exe 工具修改插入新的bootloader的分区表见图7，简要步骤如下：

  - 打开rt_partition_tool.exe工具
  - 加载middleware/boards/bk7237/bootloader.bin（若bootloader中没有分区表则会提示添加会导入分区表）
  - 编辑/导入分区表
  - 保存分区表到bootloader.bin
  - 导出分区表到json文件以作备用
  - 步骤四中的bootloader.bin,然后替换middleware/boards/bk7237/bootloader.bin。

.. figure:: ../../../_static/partition_tool.png
    :align: center
    :alt: partition_tool
    :figclass: align-center

    图7rt_partition_tool图示

- 7.详细步骤可自行参考文档tools/env_tools/rt_partition_tool/rt_partition_tool.pdf



案例2：以projects/customization/bk7256_configa为例，当用户在SDK基础上加入代码后，出现以下编译错误时见图2，该如何操作？

有两个解决方案：

.. important::
  - 首先可以手动将projects/customization/bk7256_configa/bk7256_bsp.ld的对应的size范围增大（ld中最后几行）。
  - 方案一：如果超出的code size很小，可关闭一些不需要的功能，即在projects/customization/bk7256_configa/config/bk7256.defconfig关闭相应的宏配置，减小code size；如果超出的code size很大，则只能实行方案二。
  - 方案二：如果超出的code size很大，则需要重新修改分区表，

.. note::

  建议优先选择方案一，方案二涉及修改的点比较多。

分区表制作步骤如下：

- 1.app分区表制作：

  flash物理总长度为4M，需在projects/customization/bk7256_configa/main/vnd_flash.c设定各个分区的partition_start_addr(物理地址)和partition_length(物理长度)见图8黄色区域；

.. figure:: ../../../_static/partition_a.png
    :align: center
    :alt: partition_a
    :figclass: align-center

    图8分区表(partition)图示

- 2.打包脚本配置：

  将projects/customization/bk7256_configa/configuration.json中的start_addr和size按照图8分区表中各分区大小修改即可，（其中start_addr = partition_start_addr，
  size = partition_length）见图9黄色区域；

.. figure:: ../../../_static/package.png
    :align: center
    :alt: package
    :figclass: align-center

    图9打包脚本配置图示

- 3.链接脚本配置：（bk7256是双核，需配置bk7256_bsp.sag和bk7256_cp1_bsp.sag）

bk7256_bsp.sag配置如下：

  - projects/customization/bk7256_configa/bk7256_bsp.sag的LMA和length根据图8分区表中Application的partition_start_addr和partition_length计算得出，见图10黄色区域；

.. figure:: ../../../_static/app0_a_sag.png
    :align: center
    :alt: app0_a_sag
    :figclass: align-center

    图10链接脚本配置(bk7256_bsp.sag)图示

.. note::

  计算规则：
    - bk7256_bsp.sag中CPU0_LOAD1的LMA（cpu视角0x10000）=  Application的partition_start_addr \*32/34;

    - bk7256_bsp.sag中CPU0_LOAD1的length（cpu视角0x8800） =  0x8800;

    - bk7256_bsp.sag中CPU0_LOAD2的LMA（cpu视角0x18800）=  CPU0_LOAD1的LMA + 0x8800;

    - bk7256_bsp.sag中CPU0_LOAD2的length（cpu视角0x197800） =  Application的partition_length\*32/34 - 0x8800;

bk7256_cp1_bsp.sag配置如下：

  - projects/customization/bk7256_configa/bk7256_cp1_bsp.sag的LMA和length根据图8分区表中Application1的partition_start_addr和partition_length计算得出，见图11黄色区域；

.. figure:: ../../../_static/app1_a_sag.png
    :align: center
    :alt: app1_a_sag
    :figclass: align-center

    图11链接脚本配置(bk7256_cp1_bsp.sag)图示

.. note::

  计算规则:
    - bk7256_cp1_bsp.sag中CPU1的LMA（cpu视角0x1B0000）= Application1的partition_start_addr \*32/34;

    - bk7256_cp1_bsp.sag中CPU1的length（cpu视角0x20000） = Application1的partition_length\*32/34;

- 4.使用文档《RISC_V》中关于Andes sag转ld文件方法将bk7256_bsp.sag和bk7256_cp1_bsp.sag转化为对应的ld文件；

.. note::

  该步骤4仅限新建工程；已有工程不需要该步骤,建议直接修改对应ld文件

- 5.需将projects/customization/bk7256_configa/properties_libs/bk7256.defconfig中CPU1核的偏移量重置一下 CONFIG_SLAVE_CORE_OFFSET=1769472 （十进制格式）
  (CONFIG_SLAVE_CORE_OFFSET 1769472 == 0x1B0000 in bk7256_cp1_bsp.sag LMA)


- 6.使用tools/env_tools/rt_partition_tool/rt_partition_tool.exe 工具修改插入新的bootloader的分区表见图12，简要步骤如下：

  - 打开rt_partition_tool.exe工具
  - 加载projects/customization/bk7256_configa/bootloader.bin（若bootloader中没有分区表则会提示添加会导入分区表）
  - 编辑/导入分区表
  - 保存分区表到bootloader.bin
  - 导出分区表到json文件以作备用
  - 步骤四中的bootloader.bin,然后替换projects/customization/bk7256_configa/bootloader.bin。

.. figure:: ../../../_static/partition_tool_a.png
    :align: center
    :alt: partition_tool_a
    :figclass: align-center

    图12rt_partition_tool图示

- 7.详细步骤可自行参考文档tools/env_tools/rt_partition_tool/rt_partition_tool.pdf

五、分区功能介绍
----------------------------

各分区功能介绍如表格所示

+------------------+---------------+----------------+--------------------------------------------+
| partition name   | start address | length         | functional description                     |
+==================+===============+================+============================================+
| bootloader       | 0x00000000    | 0x11000        | Store bootloader                           |
+------------------+---------------+----------------+--------------------------------------------+
| app              | 0x11000       | 0x1CB000       | Store application image                    |
+------------------+---------------+----------------+--------------------------------------------+
| app1             | 0x1DC000      | 0x77000        | Store application1 image                   |
+------------------+---------------+----------------+--------------------------------------------+
| ota              | 0x253000      | 0x187000       | store updating image                       |
+------------------+---------------+----------------+--------------------------------------------+
| user_data        | 0x3DA000      | 0x24000        | Store customer data                        |
+------------------+---------------+----------------+--------------------------------------------+
| RF_Firmware      | 0x3FE000      | 0x1000         | Store calibration parameter                |
+------------------+---------------+----------------+--------------------------------------------+
| NEI_info         | 0x3FF000      | 0x1000         | Store wifi mac address and fast connection |
|                  |               |                | parameter                                  |
+------------------+---------------+----------------+--------------------------------------------+