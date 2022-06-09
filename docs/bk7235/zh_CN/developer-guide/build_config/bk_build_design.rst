构建实现
=======================

设计思路
-------------

ARMINO 构建主要是采取面向对象的思想进行设计，理解的关键点：

    - CMakeLists.txt 二次扫描，有机会在依赖扩充期间做一些特别处理
    - 构建前先进行依赖扩充，简化依赖的编写
    - projects/components 派生自 cmake 的 projects/target， 通过在函数调用前设置特定变量的方式覆盖默认值，通过在函数调用之后设置特定属性的方式来
    - 允许 projects/components 在核心脚本执行之前，之中，之后增加 hook 处理
    - 提供封装好的 API 处理 90% 以上的绝大多数场景，只有少数特殊场景才需要复杂 cmake 脚本处理

构建脚本
-------------

ARMINO 构建系统的列表文件位于 :armino:`/tools/build_tools/cmake` 中。实现构建系统核心功能的模块如下

    - build.cmake - 构建相关函数，即构建初始化、检索/设置构建属性、构建处理。
    - component.cmake - 组件相关的函数，如添加组件、检索/设置组件属性、注册组件。
    - kconfig.cmake - 从 Kconfig 文件中生成配置文件（sdkconfig、sdkconfig.h、sdkconfig.cmake 等）。
    - target.cmake - 设置构建目标和工具链文件。
    - utilities.cmake - 其它帮助命令。

 除了这些文件，还有两个重要的 CMake 脚本在 :armino:`/tools/build_tools/cmake` 中：

    - armino.cmake - 设置构建参数并导入上面列出的核心模块。
    - project.cmake - 导入 ``armino.cmake``，并提供了一个自定义的``project()``命令，该命令负责处理建立可执行文件时所有的繁重工作。
      包含在标准 ARMINO 项目的顶层 CMakeLists.txt 中。

:armino:`/tools/build_tools/cmake` 中的其它文件都是构建过程中的支持性文件或第三方脚本。

构建过程
-------------

构建过程可以大致分为四个阶段：

.. blockdiag::
    :scale: 100%
    :caption: ARMINO Build System Process
    :align: center
    
    blockdiag armino-build-system-process {
        初始化 -> 生成组件列表
        生成组件列表 -> 组件处理
        组件处理 -> 完成
    }

初始化 
******************
  
 该阶段为构建设置必要的参数。

    - 在将 ``armino.cmake`` 导入 ``project.cmake`` 后，将执行以下步骤：
        - 在环境变量中设置 ``ARMINO_PATH`` 或从顶层 CMakeLists.txt 中包含的 ``project.cmake`` 路径推断相对路径。
        - 将 :armino:`/tools/build_tools/cmake` 添加到 ``CMAKE_MODULE_PATH`` 中，并导入核心模块和各种辅助/第三方脚本。
        - 设置构建工具/可执行文件，如默认的 Python 解释器。
        - 设置全局构建参数，即编译选项、编译定义、包括所有组件的 include 目录。
        - 将 :armino:`components` 与 :armino:`middleware` 中的组件添加到构建中。
    - 自定义 ``project()`` 命令的初始部分执行以下步骤：
        - 在环境变量或 CMake 缓存中设置 ``ARMINO_TARGET`` 以及设置相应要使用的``CMAKE_TOOLCHAIN_FILE``。
        - 添加 ``EXTRA_COMPONENTS_DIRS`` 中的组件至构建中
        - 从 ``COMPONENTS``/``EXCLUDE_COMPONENTS``、``SDKCONFIG``、``SDKCONFIG_DEFAULTS`` 等变量中为调用命令 ``armino_build_process()`` 准备参数。

调用 ``armino_build_process()`` 命令标志着这个阶段的结束。

生成组件列表
******************

  这个阶段会建立一个需要在构建过程中处理的组件列表，该阶段在 ``armino_build_process()`` 的前半部分进行。

    - 查找每个组件的公共和私有依赖。创建一个子进程，**以脚本模式执行每个组件的 CMakeLists.txt。**
      ``armino_component_register`` REQUIRES 和 PRIV_REQUIRES 参数的值会返回给父进程。
      这就是组件依赖扩充（或者称早期扩展）。**在这一步中定义变量 ``CMAKE_BUILD_EARLY_EXPANSION``**。
    - 根据公共和私有的依赖关系，递归地导入各个组件。

.. note:

    组件中每个 CMakeLists.txt 会被执行两遍，第一遍发生在_<生成组件列表>阶段，的目的是 ``armino_component_register()`` 扩充组件依赖，
    此时 Kconfig 还未加载，因此，不能通过 Kconfig 中的 CONFIG_XXX 值决定一个组件是否应该加载。 TODO。。。

组件处理
******************

  该阶段处理构建中的组件，是 ``armino_build_process()`` 的后半部分。

  - 从 sdkconfig 文件中加载项目配置，并生成 sdkconfig.cmake 和 sdkconfig.h 头文件。这两个文件分别定义了可以从构建脚本和 C/C++ 源文件/头文件中访问的配置变量/宏。
  - **导入各组件的 ``project_include.cmake``**。
  - 将每个组件添加为一个子目录，处理其 CMakeLists.txt。组件 CMakeLists.txt 调用注册命令 ``armino_component_register`` 添加源文件、导入目录、创建组件库、链接依赖关系等。

完成
^^^^^^^
  该阶段是 ``armino_build_process()`` 剩余的步骤。
  
  - 创建可执行文件并将其链接到组件库中。
  - 生成 project_description.json 等项目元数据文件并且显示所建项目等相关信息。

请参考 :armino_file:`/tools/build_tools/cmake/project.cmake` 获取更多信息。