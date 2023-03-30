Build overview
==============

:link_to_translation:`zh_CN:[中文]`

basic concept
-------------

 - ``project`` - Specifically the directory or subdirectory under armino/projects/,which contains all the files and configurations needed to build an executable application,as well as other supporting files such as partition tables, data/filesystem partitions, and bootloaders.
 - ``Project configuration`` - Specifically, the sdkconfig.h file generated in the build directory during the build process.
 - ``application`` - is an executable built by ARMINO. A project typically builds two applications : the project application (the main executable file, i.e. user-defined firmware) and the bootloader  (the project application that starts and initializes).
 - ``components`` -  is modular and self-contained code that is compiled into a static library (.a file) and linked into the application.
 - ``Target`` - Specifically refers to the hardware device that runs the built application, such as bk7231n, bk7236, etc.

Project example
---------------

::

    - armino/
        - components/
        - include/
        - middleware/
            - arch/
                - bk7231n/
                    - bk7231n.defconfig
                    - ...
        - projects/
            - my_project/
                - config/
                    - common.config
                    - bk7231n.config
                    - bk7236.config
                - CMakeLists.txt
                - Kconfig.projbuild
                - main/
                    - main.c
                    - Kconfig
                    - CMakeLists.txt
                - components/
                    - c1/
                        - c1.c
                        - Kconfig
                        - CMakeLists.txt
                    - c2/


Trigger a build with make
-------------------------

Run make in the armino root directory. The most commonly used make commands are as follows:

 - ``make bkxxx`` - Build the target bkxxx, and the project is the default project armino/projects/app.
 - ``make bkxxx`` PROJECT=a/b - Build bkxxx, the project is armino/projects/a/b
 - ``make all`` - build all targets
 - ``make cleanbkxxx`` - clean target bkxxx
 - ``make clean`` - clear all targets

The make build is to indirectly call the armino tool through armino/Makefile to build, which saves the trouble of setting environment variables when building with the armino tool.
It is recommended that you build in this way.


