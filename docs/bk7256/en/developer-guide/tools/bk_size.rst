armino.py size
===========================

:link_to_translation:`zh_CN:[中文]`

armino.py The following commands can be used to analyze the static usage of FLASH/RAM, and can be used as a reference for code size and memory optimization.

+-------------------------------------------+----------------------------------------------------+
|  Command                                  | Introduce                                          |
+-------------------------------------------+----------------------------------------------------+
| armino.py size                            | Display the usage of FLASH/TCM/ITCM/RAM            |
+-------------------------------------------+----------------------------------------------------+
| armino.py size-components                 | Display the usage of each component                | 
+-------------------------------------------+----------------------------------------------------+
| armino.py size-component -c component_name| Show usage of component component_name             |
+-------------------------------------------+----------------------------------------------------+
| armino.py size-files                      | Display the usage of each file                     | 
+-------------------------------------------+----------------------------------------------------+
| armino.py size-diff -m another_map_file   | Display the difference between the compiled codede |
|                                           | and the corresponding code about FLASH/TCM/ITCM/RAM|
+-------------------------------------------+----------------------------------------------------+

.. note::

  Size obtains size's static information by analyzing map files.

.. note::

  armino.py size-files The execution takes a long time, which may take several minutes.

armino.py size-diff usage method
-------------------------------------------------

Assuming that the road strength of the project is '$ARMINO_PATH', and the generated '.map' file is 'project_name.map'. Now we want to see how much the ARMINO v3.2 version has increased compared with the v3.1 version FLASH, the steps are as follows：

 - ARMINO switches to the v3.1 branch and executes 'armino.py build'
 - Save $ARMINO_PATH/build, such as renaming to $ARMINO_PATH/build_v3.1
 - ARMINO switches to the v3.2 branch and executes 'armino.py size-diff -m $ARMINO_PATH/build_v3.1/project_name.map'

