示例
=====================

:link_to_translation:`en:[English]`

.. _001_build_project_demo:


001 - 最小项目
-----------------------------------------


每个项目都有一个顶层 ``CMakeLists.txt`` 文件，包含整个项目的构建设置，最小项目如下所示::

        cmake_minimum_required(VERSION 3.5) 
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

:example:`示例 <build_system/001_simplest_proj>`

CMakeLists.txt 说明
******************************************

每个项目都要按上述顺序添加两行代码：

- ``cmake_minimum_required(VERSION 3.5)`` 必须放在 CMakeLists.txt 文件的第一行，它会告诉 CMake 构建该项目所需要的最小版本号。ARMINO 支持 3.5 或者更高版本。
- ``include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)`` 会导入 CMake 的其余功能来完成配置项目、搜索组件等任务。
- ``project(myProject)`` 会创建项目，并指定项目名称。该名称会作为最终输出的二进制文件的名字，即 ``myProject.elf`` 和 ``myProject.bin``。每个 CMakeLists.txt 文件只能定义一个项目。


默认项目变量
******************************************

以下这些变量都有默认值，用户可以覆盖这些变量值以自定义构建行为。

- ``COMPONENT_DIRS``：组件的搜索目录，默认为 ``ARMINO_PATH/components``、 ``ARMINO_PATH/middleware``，``PROJECT_DIR/components``、
  和 ``EXTRA_COMPONENTS_DIRS``。如果您不想在这些位置搜索组件，请覆盖此变量。
- ``EXTRA_COMPONENTS_DIRS``：用于搜索组件的其它可选目录列表。路径可以是相对于项目目录的相对路径，也可以是绝对路径。
- ``COMPONENTS``：要构建进项目中的组件名称列表，默认为 ``COMPONENT_DIRS`` 目录下检索到的所有组件。使用此变量可以“精简”项目以缩短构建时间。
  请注意，如果一个组件通过 ``COMPONENT_REQUIRES`` 指定了它依赖的另一个组件，则会自动将其添加到 ``COMPONENTS`` 中。

以上变量中的路径可以是绝对路径，或者是相对于项目目录的相对路径。

.. note::

    请使用 `cmake 中的 set 命令 <cmake set_>`_ 来设置这些变量，如 ``set(VARIABLE "VALUE")``。请注意，``set()`` 命令需放在 ``include(...)`` 之前。

默认构建选项
******************************************

TODO

默认构建行为
******************************************

TODO

.. _002_build_project_demo:

002 - 添加组件搜索目录
-----------------------------------------

可以在项目顶层 CMakeLists.txt 中设置 EXTRA_COMPONENTS_DIRS 来导入除默认组件搜索目录之下的组件::

        cmake_minimum_required(VERSION 3.5) 
        set(EXTRA_COMPONENTS_DIRS my_component_dir)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)


:example:`示例 <build_system/002_add_components_dir>`

.. _003_build_project_demo:

003 - 不构建特定组件
-----------------------------------------

可以在项目顶层 CMakeLists.txt 中设置 EXCLUDE_COMPONENTS 来排除某些组件的构建::

        cmake_minimum_required(VERSION 3.5) 
        set(EXCLUDE_COMPONENTS c1_not_build)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

:example:`示例 <build_system/003_exclude_components>`

.. _004_build_project_demo:


004 - 仅构建特定组件
-----------------------------------------

可以在项目顶层 CMakeLists.txt 中设置 COMPONENTS 来只构建特定组件， 示例中仅构建 c3_contain_main 及依赖组件::

        cmake_minimum_required(VERSION 3.5) 
        set(COMPONENTS c3_contain_main)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

注意：

 - COMPONENTS 列表中所有组件及依赖组件均会被构建。
 - 由于所有组件依赖于通用组件，因此通用组件也会被构建。
 - 构建组件中必须包含 ``main()`` 函数实现。

:example:`示例 <build_system/004_set_components>`

.. _005_build_project_demo:

005 - 覆盖默认编译选项
-----------------------------------------

在顶层 CMakeLists.txt 的 project() 之后，使用 Wno-extra 来覆盖默认 Wextra::

        cmake_minimum_required(VERSION 3.5) 
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)
        
        armino_build_set_property(COMPILE_OPTIONS "-Wno-extra" APPEND)

.. note:

  应在 project() 之后设置您的编译选项，因为默认的构建规范是在 project() 内设置的。

:example:`示例 <build_system/005_proj_build_options>`

.. _101_build_component_demo:

101 - 最简组件
-----------------------------------------

最简单组件 CMakeLists.txt 如下所示，调用 ``armino_component_register()`` 将组件注册到构建系统中::
        
        armino_component_register(SRCS c1.c INCLUDE_DIRS include)

:example:`示例 <build_system/101_simplest_component>`

.. _102_build_component_demo:

102 - 设置组件编译选项
-----------------------------------------

在编译特定组件的源文件时，可以使用 ``target_compile_options`` 命令来传递编译器选项::

  target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)

如果给单个源文件指定编译器标志，可以使用 CMake 的 `set_source_files_properties`_ 命令::

    set_source_files_properties(compile_options.c
        PROPERTIES COMPILE_FLAGS
        -Wno-unused-variable
    )

请注意，上述两条命令只能在组件 CMakeLists 文件的 ``armino_component_register`` 命令之后调用。

:example:`示例 <build_system/102_build_options>`

.. _103_build_component_demo:

103 - 覆盖组件
-----------------------------------------

可以通过定义一个同名组件来覆盖 armino 默认组件，示例是使用项目组件中定义的 bk_log 来覆盖 :armino::`<components/bk_log>` 组件。

:example:`示例 <build_system/103_overwrite_component>`

.. _104_build_component_demo:

104 - 设置组件依赖
-----------------------------------------

示例包含了四个组件::

    - components/
        - c1/
            - c1.c
            - c1.h
            - c1_internal
                - c1_internal.c1
                - c1_internal.h
            - include/
                - bk_api_c1.h
        - c2/
            - c2.c
            - include
                - bk_api_c2.h
        - c3/
            - c3.c
            - include
                - bk_api_c3.h
        - c4/
            - c4.c
            - include
                - bk_api_c4.h

:example:`示例 <build_system/104_dependency>`

.. _105_build_component_demo:

105 - 组件中链接库
-----------------------------------------

可以通过 ``add_prebuilt_library`` 导入库::

  add_prebuilt_library(target_name lib_path [REQUIRES req1 req2 ...] [PRIV_REQUIRES req1 req2 ...])

其中：

- ``target_name``- 用于引用导入库的名称，如链接到其它目标时
- ``lib_path``- 预建库的路径，可以是绝对路径或是相对于组件目录的相对路径

可选参数 ``REQUIRES`` 和 ``PRIV_REQUIRES`` 指定对其它组件的依赖性。这些参数与 ``armino_component_register`` 的参数的意义相同。

.. note::

    注意预建库的编译目标需与目前的项目相同。预建库的相关参数也要匹配。如果不特别注意，这两个因素可能会导致应用程序中出现 bug。

:example:`示例 <build_system/105_link_lib>`

.. _106_build_component_demo:

106 - 纯 CMake ARMINO 组件
-----------------------------------------

通常 ARMINO 组件搜索路径上的组件应该按 ARMINO 要求进行注册，但如果您想要将 ARMINO 组件搜索路径上的组件改成纯 CMake 组件也是可以的，
代码如下::

    if (CMAKE_BUILD_EARLY_EXPANSION)
            return()
    endif()

    add_library(c1 STATIC c1.c)
    target_include_directories(c1 PUBLIC include)

前三行代码是用于告诉 ARMINO 构建系统不要将本组件加入到，c1 组件将以纯 CMake 方式进行构建。
除此之外，您还需要按 CMake 的方式将本组件加到构建系统中，本示例是在项目顶层 CMakeLists.txt 中将 c1 组件加到构建树中::

    cmake_minimum_required(VERSION 3.5)
    include($ENV{ARMINO_PATH}/tools/build_toos/cmake_project.cmake)
    project(cmake_exam)
    
    add_subdirectory(components/c1)

:example:`示例 <build_system/106_pure_cmake_component>`

.. note:

    通常情况下，ARMINO 搜索路径下的组件都应该调用 armino_component_register() 按 ARMINO 要求的方式
    编写组件。当您有特别理由要自己编写纯 CMake 组件时，可选择按 _<在 ARMINO 组件中导入第三方 CMake 组件> 或者是相对于项目目录的相对路径。
    _<在任意地方导入第三方 CMake 组件> 方式导入您的纯 CMake 组件。

.. _107_build_component_demo:

107 - 导入纯 CMake 组件 1
-----------------------------------------

示例中 foo 使用纯 CMake 构建，放在 main 组件中，可使用下面的方式导入::

    armino_component_register(SRCS "main.c" INCLUDE_DIRS .)
    add_subdirectory(foo)
    target_link_libraries(${COMPONENT_LIB} PUBLIC foo)

:example:`示例 <build_system/107_pure_cmake_in_main>`

.. _108_build_component_demo:

108 - 导入纯 CMake 组件 2
------------------------------------------------------------------

示例中 foo 使用纯 CMake 构建，放在 c1 组件中，可使用下面的方式导入::

    armino_component_register(SRCS "c1.c" INCLUDE_DIRS include)
    add_subdirectory(foo)
    target_link_libraries(${COMPONENT_LIB} PUBLIC foo)

:example:`示例 <build_system/108_pure_cmake_in_component>`

.. _109_build_component_demo:

109 - 导入纯 CMake 组件 3
------------------------------------------------------------------

示例中 anywhere 放在项目根目录下，通过纯 CMake 构建，可以在项目顶层 CMakeLists.txt 中加入这行代码进行导入::

    add_subdirectory(anywhere)

事实上可以使用您喜欢的任意方式导入第三方纯 CMake 组件。

:example:`示例 <build_system/109_pure_cake_in_anywhere>`

.. _110_use_armino_lib_in_pure_cmake:

110 - 纯 CMake 组件使用 armino 组件
------------------------------------------------------------------

在纯 CMake 组件中引用 armino 组件的方式是 armino::component_name。在示例中 anywhere 目录下的 c1 组件会使用到 armino 组件 c 组件::

    target_link_libraries(c1 armino::c)

:example:`示例 <build_system/110_use_armino_lib_in_pure_cmake>`

.. _111_build_component_demo:

111 - 导入 GNU Makefile 项目 1
-------------------------------------------------------------

如果您有一个组件不是使用 cmake 编写，例如，您的组件是通过 GNU Makefile 编写，您想在 ARMINO 中使用这个组件，
但您又不想将组件构建改写成 CMake 形式。这时，需要使用 CMake 的 ExternalProject 功能。

示例将一个使用 Makefile 编写的 foo 组件导入到 c1 组件中::

    # 用于 foo 的外部构建过程，在源目录中运行
    # 并生成 libfoo.a
    externalproject_add(foo_build
        PREFIX ${COMPONENT_DIR}
        SOURCE_DIR ${COMPONENT_DIR}/foo
        CONFIGURE_COMMAND ""
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make CC=${CMAKE_C_COMPILER} libfoo.a
        INSTALL_COMMAND ""
        )

    # 将 libfoo.a 添加到构建系统中
    add_library(foo STATIC IMPORTED GLOBAL)
    add_dependencies(foo foo_build)

    set_target_properties(foo PROPERTIES IMPORTED_LOCATION
        ${COMPONENT_DIR}/foo/libfoo.a)
    set_target_properties(foo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
        ${COMPONENT_DIR}/foo/include)

    set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
        "${COMPONENT_DIR}/foo/libfoo.a")

（上述 CMakeLists.txt 可用于创建名为 ``foo`` 的组件，该组件使用自己的 Makefile 构建 libfoo.a。）

- ``externalproject_add`` 定义了一个外部构建系统。

  - 设置 ``SOURCE_DIR``、``CONFIGURE_COMMAND``、``BUILD_COMMAND`` 和 ``INSTALL_COMMAND``。如果外部构建系统没有配置这一步骤，可以将 ``CONFIGURE_COMMAND`` 设置为空字符串。在 Armino 的构建系统中，一般会将 ``INSTALL_COMMAND`` 变量设置为空。
  - 设置 ``BUILD_IN_SOURCE``，即构建目录与源目录相同。否则，您也可以设置 ``BUILD_DIR`` 变量。
  - 有关 ``externalproject_add()`` 命令的详细信息，请参阅 ExternalProject_。

- 第二组命令添加了一个目标库，指向外部构建系统生成的库文件。为了添加 include 目录，并告知 CMake 该文件的位置，需要再设置一些属性。
- 最后，生成的库被添加到 `ADDITIONAL_MAKE_CLEAN_FILES`_ 中。即执行 ``make clean`` 后会删除该库。请注意，构建系统中的其他目标文件不会被删除。

:example:`示例 <build_system/111_use_gnu_make_project1>`

.. _112_build_component_demo:

112 - 导入 GNU Makefile 项目 2
-------------------------------------------------------------

另一种导入 GNU 项目的方式是通过 add_custom_command 的方式导入 GNU Makefile 编写的项目。

示例将一个使用 Makefile 编写的 foo 组件导入到 c1 组件中::

    armino_component_register(SRCS c1.c INCLUDE_DIRS include)

    add_custom_command(OUTPUT ${COMPONENT_DIR}/foo/libfoo.a
        COMMAND ${COMPONENT_DIR}/foo/build.sh ${COMPONENT_DIR}/foo ${CMAKE_C_COMPILER}
        VERBATIM
        COMMENT "Build external project"
        )
    add_custom_target(foo_build DEPENDS ${COMPONENT_DIR}/foo/libfoo.a)

    add_library(foo STATIC IMPORTED GLOBAL)
    add_dependencies(foo foo_build)
    set_target_properties(foo PROPERTIES IMPORTED_LOCATION ${COMPONENT_DIR}/foo/libfoo.a)
    set_target_properties(foo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${COMPONENT_DIR}/foo/include)

    target_link_libraries(${COMPONENT_LIB} PUBLIC foo)

上述用例首先调用 ``armino_component_register`` 注册一个标准的 armino 组件 ``c1``，然后通过 ``add_custom_command()`` 增加
一条命令用于生成 ``libfoo.a``， 再增调用 ``add_custom_target()`` 增加目标 ``foo_build``。

随后的四条命令创建 ``foo`` 目标，设置 ``foo`` 所对应库的位置与头文件目录。最后，将目标 ``foo`` 关联到 armino 标准组件 ``c1`` 上。

.. note::

    在使用 ``add_custom_command()`` 时应该注意 OUTPUT 之后生成的文件必须用直接用到 armino 组件作所对应的组件 CMakeLists.txt 中，
    这样，只有这样才会触发 Makefile 的依赖规则去调用增加的 COMMAND。否则，由于生成的文件未用在 CMakeLists.txt 中，Makefile 会认为
    构建系统不需要这个文件，因而不会触发命令调用!

:example:`示例 <build_system/112_use_gnu_make_project2>`

.. _113_build_component_demo:

113 - 自定义 cmake 项目中使用 armino
-------------------------------------------------------------

当需要将 armino 移植到像 zephyr/rtt/alios 这类开源平台时，一种方式是将 armino 编译成库的方式放到其中。

:example:`示例 <build_system/113_armino_as_lib>`

.. _201_build_project_demo:

201 - 最简单组件 Kconfig
-------------------------------------------------------------

每个组件都可以包含一个 ``Kconfig`` 文件，``Kconfig`` 文件中包含要添加到该组件配置菜单中的一些配置设置信息。

运行 menuconfig 时，可以在 ``Component Settings`` 菜单栏下找到这些设置。

创建一个组件的 Kconfig 文件，最简单的方法就是使用 ARMINO 中现有的 Kconfig 文件作为模板，在这基础上进行修改。

最简单组件 Kconfig::

        config C1
            bool "Enable component c1"
            default y

构建系统会在生成的 sdkconfig 添加如下配置项::

        CONFIG_C1=y

构建系统会构建根目录（通常为 build 目录）下的 sdkconfig.h 添加如下配置项::

        #define CONFIG_C1 1

.. note:

  如果在源文件中使用 CONFIG_C1 时应该包含 sdkconfig.h。

:example:`示例 <build_system/201_simplest_kconfig>` 

.. _202_build_project_demo: 

202 - 添加全局配置
-------------------------------------------------------------

可以为组件定义一个 KConfig 文件以实现全局的 组件配置。如果要在 menuconfig 的顶层添加配置选项，
而不是在 “Component Configuration” 子菜单中，则可以在 CMakeLists.txt 文件所在目录的 KConfig.projbuild 文件中定义这些选项。

通常会为 main 组件增加一个项目相关的 Kconfig.projbuild。但在在此文件中添加配置时要小心，因为这些配置会包含在整个项目配置中。
在可能的情况下，请为 组件配置 创建 KConfig 文件。

:example:`示例 <build_system/202_global_kconfig>` 

.. _203_build_component_demo:

203 - 纯配置组件
-----------------------------------------

一个组件可不包含任何源文件与头文件，仅包含 Kconfig 配置文件，称之为纯配置组件::

        armino_component_register()

.. note:

    纯配置组件需要调用 armino_component_register() 将自己注册到构建系统中。

:example:`示例 <build_system/203_config_only>`


204 - 自定义项目配置
-------------------------------------------------------------

armino 加载 Kconfig 的顺序如下，对于同一配置项，后加载的值会覆盖先加载的值：

 - 组 Kconfig 默认配置
 - :middleware:: `<arch/bkxxx/bkxxx.defconfig>` 中目标特定默认配置
 - 项目根目录/config/common.config 中定义项目相关，目标通用的配置
 - 项目根目录/config/bkxxx.config 中定义的项目相关，特定目标的配置

应用可以通过配置 “项目根目录/config/common.config“ 与 ”项目根目录/config/bkxxx.config” 来设置项目/目标相关的配置项，其中 bkxxx 为具体的 SoC，如 bk7256 等。

:example:`示例 <build_system/205_project_per_soc_config>`

207 - 通过 Kconfig 禁用组件
-------------------------------------------------------------

有多种方式可以禁用一个组件，其中一种方法是通过 Kconfig 中组件使能配置来禁用::

    set(src)
    set(inc)

    if (CONFIG_C1)
        list(APPEND src c1.c)
        list(APPEND inc include)
    endif()

    armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc})

注意，下面是错误的禁用组件方法。因为 armino 构建系统在生成组件列表阶段（即早期扩充阶段）依赖 ``armino_component_register()``
来生成该组件，而只有先生成该组件才会在组件处理阶段加载组件的 Kconfig 配置::

    if (CONFIG_C1)
        armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc})
    endif()

下述写法也可能带来问题，当组件（TODO）::

    if (CONFIG_C1)
        armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc} REQUIRES c2 c3 c4)
    else()
        armino_component_register()
    endif()

其他禁用组件的方法是：

 - 通过 ARMINO_SOC 来禁用 (TODO)
 - 通过 EXCLUDE_COMPONENTS 来禁用 (TODO)

:example:`示例 <build_system/207_disable_components>`

