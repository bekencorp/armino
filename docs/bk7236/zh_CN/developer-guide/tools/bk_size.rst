armino.py size
===========================

:link_to_translation:`en:[English]`

armino.py 下述命令可以用于分析 FLASH/RAM 静态使用情况，可以作为 code size 以及内存优化的参考。

+-------------------------------------------+----------------------------------------------------+
|  命令                                     | 介绍                                               |
+-------------------------------------------+----------------------------------------------------+
| armino.py size                            | 显示 FLASH/TCM/ITCM/RAM 的使用情况                 |
+-------------------------------------------+----------------------------------------------------+
| armino.py size-components                 | 显示各组件 FLASH/TCM/ITCM/RAM 的使用情况           | 
+-------------------------------------------+----------------------------------------------------+
| armino.py size-component -c component_name| 显示组件 component_name FLASH/TCM/ITCM/RAM 的使用情|
|                                           | 况                                                 |
+-------------------------------------------+----------------------------------------------------+
| armino.py size-files                      | 显示各文件 FLASH/TCM/ITCM/RAM 的使用情况           | 
+-------------------------------------------+----------------------------------------------------+
| armino.py size-diff -m another_map_file   | 显示当前编译代码与 another_map_file 对应的代码使用 |
|                                           | FLASH/TCM/ITCM/RAM 时的差值                        |
+-------------------------------------------+----------------------------------------------------+

.. note::

  size 通过分析 map 文件得到 size 的静态信息。

.. note::

  armino.py size-files 执行时间较长，可能会有数分钟。

armino.py size-diff 使用方法
---------------------------------

假设项目路劲为 $ARMINO_PATH，生成的 .map 文件为 project_name.map， 现想查看 ARMINO v3.2 版本相对于 v3.1 版本 FLASH 增大了多少，则步骤为：

 - ARMINO 切到 v3.1 分支， 执行 armino.py build
 - 将 $ARMINO_PATH/build 保存，如重命名为 $ARMINO_PATH/build_v3.1
 - ARMINO 切到 v3.2 分支， 执行 armino.py size-diff -m $ARMINO_PATH/build_v3.1/project_name.map

