Partition modification specification
======================================
:link_to_translation:`zh_CN:[中文]`

一.The relationship between physical addresses and logical addresses 
----------------------------------------------------------------------

To ensure the accuracy of data stored in flash, 2 bytes of crc check bits are added for every 32 bytes (crc check is done automatically by the hardware), so that the physical address (flash view) is 2 bytes more than the logical address (cpu view). The comparison is shown in Figure 1 below: 
(Left side is cpu view, right side is flash view, red font is crc check)

.. figure:: ../../../_static/logic.png
    :align: center
    :alt: logic
    :figclass: align-center

    Figure 1 Physical versus logical addresses

二.Components of the partition modification
--------------------------------------------
Changing a partition involves four parts:

.. important::
  - Make partition table of app
  - Package the configuration file of the script
  - Link script (ld) production
  - bootloader makes a partition table

三.Partition change scope
----------------------------

.. important::
  - When making a new project project, you need to redo the partitions
  - When modifying the partition table, you need to redo the partition
  - When you modify the linked script (ld), you need to redo the partition

四．case
----------------------------

Example 1: Take bk7256 as an example. If the following compiling error occurs after the user adds code to the SDK, see Figure 2. What should I do?

.. figure:: ../../../_static/overflow.png
    :align: center
    :alt: overflow
    :figclass: align-center

    Figure 2 Compile error graph

There are two solutions:

.. important::
  - First of all, you can manually will middleware/soc/bk7256/bk7256_bsp. Corresponding increases the size range of ld (ld in the last few lines).
  - Solution 1: if you exceed code size is small, can shut down a few do not need to function, that is in the middleware/soc/bk7256/bk7256.defconfig close corresponding macro configuration, reduce the code size. If the excess code size is large, only Solution 2 can be implemented.
  - Solution 2: If the code size is too large, the partition table needs to be modified.

.. note::

  Solution 1 is recommended. Solution 2 involves many modification points.

The steps to make a partition table are as follows:

- 1. Making app partition table:

  The total physical length of flash is 4M, Must be in the middleware/boards/bk7256/vnd_flash/vnd_flash.c set partitions partition_start_addr (physical address) and partition_length (physical length) as shown in figure 3 yellow area;

.. figure:: ../../../_static/partition.png
    :align: center
    :alt: partition
    :figclass: align-center

    Figure 3 partition table diagram

- 2. Package script configuration:

  The middleware/boards/bk7256/configuration.json, The start_addr and size according to figure 3 each partition in the partition table size changes, (including start_addr = partition_start_addr,size = partition_length) see yellow area in Figure 4;

.. figure:: ../../../_static/package_35.png
    :align: center
    :alt: package_35
    :figclass: align-center

    Figure 4 Packaging script configuration diagram

- 3. Link script configuration: If bk7256 is single-core, bk7256_bsp.sag needs to be configured.

The bk7256_bsp.sag configuration is as follows:

  - Middleware/soc/bk7256/bk7256_bsp, the LMA and length according to the Application in figure 3 the partition table and Application1 partition_start_addr and partition_length is calculated See the yellow area in Figure 5.

.. figure:: ../../../_static/app0_sag_35.png
    :align: center
    :alt: app0_sag_35
    :figclass: align-center

    Figure 5 Link script configuration (bk7256_bsp.sag)

.. note::

  Calculation rules:
    - In bk7256_bsp.sag, LMA (cpu view 0x10000) of CPU0_LOAD1 = partition_start_addr \*32/34 of Application;

    - The length (cpu view 0x8800) of CPU0_LOAD1 in bk7256_bsp.sag = 0x8800;

    - In bk7256_bsp.sag, the LMA of CPU0_LOAD2 (cpu view 0x18800) = the LMA of CPU0_LOAD1 + 0x8800;

    - length (cpu Angle 0x217800) of CPU0_LOAD2 in bk7256_bsp.sag = (partition_length of Application + partition_length of Application1)\*32/34 - 0x8800;


- 4. Use the Andes sag to convert bk7256_bsp.sag and bk7256_cp1_bsp.sag method in RISC_V to convert bK7256_cp1_bsp.sag to corresponding ld files.

.. note::

  Step 4 applies only to new projects. This step is not required for an existing project. You are advised to modify the ld file directly

- 5. (For dual-core, perform Step 5. For single-core, do not perform Step 5. Here does not need to) ------ should be middleware/soc/bk7256/bk7235.defconfig CPU1 nuclear offset in the reset CONFIG_SLAVE_CORE_OFFSET = 1835008 (in decimal format) (CONFIG_SLAVE_CORE_OFFSET 1835008 == 0x1C0000 in bk7235_cp1_bsp.sag LMA)


- 6.Use tools/env_tools/rt_partition_tool/rt_partition_tool.exe to modify the partition table of the new bootloader. See Figure 7.

  - Open the rt_partition_tool.exe tool
  - loading middleware/boards/bk7256/bootloader.bin (if no partition table in the bootloader will prompt will add import partition table)
  - Edit/import partition table
  - Save the partition table to bootloader.bin
  - Export the partition table to a json file for backup
  - step 4 the bootloader. Bin, and then replace the middleware/boards/bk7256/bootloader.bin.

.. figure:: ../../../_static/partition_tool.png
    :align: center
    :alt: partition_tool
    :figclass: align-center

    Figure 7 rt_partition_tool diagram

- 7. For details, see tools/env_tools/rt_partition_tool/rt_partition_tool.pdf

Case 2: the projects/customization/bk7256_configa, for example, when a user based on the SDK to join the code, a compiler error. See figure 2 below, how to operate?

There are two solutions:

.. important::
  - first of all, you can manually will projects/customization/bk7256_configa/bk7256_bsp corresponding increases the size range of ld (ld in the last few lines).
  - Solution 1: if the above code size is small, can't close some required functionality, namely in the projects/customization/bk7256_configa/config/bk7256.defconfig close corresponding macro configuration, reduce the code size. If the excess code size is large, only solution 2 can be implemented.
  - Solution 2: If the code size exceeds a large amount, the partition table needs to be modified.

.. note::

  Solution 1 is recommended. Solution 2 involves many modification points.

The steps to make a partition table are as follows:

- 1. Making app partition table:

  The total physical length of flash is 4M, Must be in the projects/customization/bk7256_configa/main/vnd_flash.c set partitions partition_start_addr (physical address) and partition_length (physical length) as shown in figure 8 yellow area;

.. figure:: ../../../_static/partition_a.png
    :align: center
    :alt: partition_a
    :figclass: align-center

    Figure 8 partition table

- 2. Package script configuration:

  Will the projects/customization/bk7256_configa/configuration.json start_addr and the size of according to the figure 8 each partition in the partition table size changes, (Where start_addr = partition_start_addr,size = partition_length) see yellow area in Figure 9;

.. figure:: ../../../_static/package.png
    :align: center
    :alt: package
    :figclass: align-center

    Figure 9 Packaging script configuration diagram

- 3. Link script configuration: If bk7256 is dual-core, configure bk7256_bsp.sag and bk7256_cp1_bsp.sag.

The bk7256_bsp.sag configuration is as follows:

  - The projects/customization/bk7256_configa/bk7256_bsp. Sag of the LMA and length according to the figure 8 in the partition table partition_start_addr and partition_length Application See the yellow area in Figure 10.

.. figure:: ../../../_static/app0_a_sag.png
    :align: center
    :alt: app0_a_sag
    :figclass: align-center

    Figure 10 Link script configuration (bk7256_bsp.sag)

.. note::

  Calculation rules:
    - In bk7256_bsp.sag, LMA (cpu view 0x10000) of CPU0_LOAD1 = partition_start_addr \*32/34 of Application;

    - The length of CPU0_LOAD1 (cpu view 0x8800) in bk7256_bsp.sag = 0x8800;

    - In bk7256_bsp.sag, the LMA of CPU0_LOAD2 (cpu view 0x18800) = the LMA of CPU0_LOAD1 + 0x8800;

    - length of CPU0_LOAD2 in bk7256_bsp.sag (cpu view 0x197800) = partition_length\* 32/34-0x8800 of Application;

The bk7256_cp1_bsp.sag configuration is as follows:

  - The projects/customization/bk7256_configa/bk7256_cp1_bsp. Sag of the LMA and length according to the figure 8 in the partition table Application1 partition_start_addr and partition_l ength calculated, see the yellow area in Figure 11;

.. figure:: ../../../_static/app1_a_sag.png
    :align: center
    :alt: app1_a_sag
    :figclass: align-center

    Figure 11 Link script configuration (bk7256_cp1_bsp.sag)

.. note::

  Calculation rules:
    - In bk7256_cp1_bsp.sag, CPU1's LMA (cpu view 0x1B0000) = partition_start_addr \*32/34 of Application1;

    - length of CPU1 in bk7256_cp1_bsp.sag (cpu view 0x20000) = partition_length\*32/34 of Application1;

- 4. Use the Andes sag to convert bk7256_bsp.sag and bk7256_cp1_bsp.sag method in RISC_V to convert bK7256_bsp. sag to corresponding ld files.

.. note::

  Step 4 applies only to new projects. This step is not required for an existing project. You are advised to modify the ld file directly

- 5. Will the projects/customization/bk7256_configa/properties_libs/bk7256 defconfig CPU1 nuclear offset in the reset CONFIG_SLAVE_CORE_OFFSET=1769472 (decimal format)(CONFIG_SLAVE_CORE_OFFSET 1769472 == 0x1B0000 in bk7256_cp1_bsp.sag LMA)

- 6. Use tools/env_tools/rt_partition_tool/rt_partition_tool.exe to modify the partition table of the new bootloader, as shown in Figure 12.

  - Open the rt_partition_tool.exe tool
  - loading projects/customization/bk7256_configa/bootloader. Bin (if no partition table in the bootloader will prompt will add import partition table)
  - Edit/import partition table
  - Save the partition table to bootloader.bin
  - Export the partition table to a json file for backup
  - step 4 the bootloader. Bin, and then replace the projects/customization/bk7256_configa/bootloader.bin.

.. figure:: ../../../_static/partition_tool_a.png
    :align: center
    :alt: partition_tool_a
    :figclass: align-center

    Figure 12rt_partition_tool diagram

- 7.For details, see tools/env_tools/rt_partition_tool/rt_partition_tool.pdf

五、partition function introduction
-------------------------------------

The table describes the functions of each partition.

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
