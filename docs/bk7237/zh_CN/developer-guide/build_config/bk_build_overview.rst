构建概述
=====================

:link_to_translation:`en:[English]`

基本概念
-------------------------

 - ``项目`` - 特指 armino/projects/ 下一个目录或者子目录，其中包含了构建可执行应用程序所需的全部文件和配置，以及其他支持型文件，例如分区表、数据/文件系统分区和引导程序。
 - ``项目配置`` - 特指构建过程中在构建目录下生成的 sdkconfig.h 文件。
 - ``应用程序`` - 是由 ARMINO 构建得到的可执行文件。一个项目通常会构建两个应用程序：项目应用程序（可执行的主文件，即用户自定义的固件）和引导程序（启动并初始化项目应用程序）。
 - ``组件`` - 是模块化且独立的代码，会被编译成静态库（.a 文件）并链接到应用程序。
 - ``目标`` - 特指运行构建后应用程序的硬件设备，如 bk7231n，bk7256 等。

项目示例
-------------------------

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
                    - bk7256.config
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


使用 make 触发构建
-------------------------

在 armino 根目录下运行　make， 最常用的 make 命令如下：

 - ``make bkxxx`` - 构建目标 bkxxx，项目为默认项目 armino/projects/legacy_app。
 - ``make bkxxx`` PROJECT=a/b - 构建 bkxxx，项目为 armino/projects/a/b
 - ``make all`` - 构建所有目标
 - ``make cleanbkxxx`` - 清理目标 bkxxx
 - ``make clean`` - 清理所有目标

make 构建是通过 armino/Makefile 间接调用 armino 工具来进行构建，它省去了使用 armino 工具构建时要
设置环境变量的麻烦，推荐您使用这种方式进行构建。


