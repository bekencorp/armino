构建 API
=====================

:link_to_translation:`en:[English]`


构建 API
------------------

.. code-block:: none

  armino_build_get_property(var property [GENERATOR_EXPRESSION])

获取一个 :ref:`构建属性 <cmake-build-properties>`，并存储在 var 中。
特定 *GENERATOR_EXPRESSION* 将获取该属性的生成器表达式字符串（不是实际值），它可与支持生成器表达式的 CMake 命令一起使用。

.. code-block:: none

  armino_build_set_property(property val [APPEND])

设置一个 :ref:`构建属性 <cmake-build-properties>` 的值为 *val*。*APPEND* 将把指定的值添加到属性当前值之后。
如果属性不存在或为空，则指定的值将变为第一个元素/成员。

.. code-block:: none

  armino_build_component(component_dir)

向构建系统提交一个包含组件的 *component_dir* 目录。相对路径会被转换为相对于当前目录的绝对路径。
所有对该命令的调用必须在`armino_build_process`之前执行。

该命令并不保证组件在构建过程中会被处理（参见 `armino_build_process` 中 `COMPONENTS` 参数说明）

.. code-block:: none

  armino_build_process(target 
                    [PROJECT_DIR project_dir]
                    [PROJECT_VER project_ver]
                    [PROJECT_NAME project_name]
                    [SDKCONFIG sdkconfig]
                    [SDKCONFIG_DEFAULTS sdkconfig_defaults]
                    [BUILD_DIR build_dir]
                    [COMPONENTS component1 component2 ...])

为导入 ARMINO 组件执行大量的幕后工作，包括组件配置、库创建、依赖性扩展和解析。在这些功能中，对于用户最重要的可能是通过调用每个组件的
``armino_component_register`` 来创建库。该命令为每个组件创建库，这些库可以使用别名来访问，其形式为 ``armino::*component_name*``。
这些别名可以用来将组件链接到用户自己的目标、库或可执行文件上。

该调用要求用 *target* 参数指定目标芯片。调用的可选参数包括：

- PROJECT_DIR - 项目目录，默认为 CMAKE_SOURCE_DIR。
- PROJECT_NAME - 项目名称，默认为 CMAKE_PROJECT_NAME。
- PROJECT_VER - 项目的版本/版本号，默认为 "1"。
- SDKCONFIG - 生成的 sdkconfig 文件的输出路径，根据是否设置 PROJECT_DIR，默认为 PROJECT_DIR/sdkconfig 或 CMAKE_SOURCE_DIR/sdkconfig。
- SDKCONFIG_DEFAULTS - 包含默认配置的文件列表（列表中必须包含完整的路径），默认为空；
  对于列表中的每个值 *filename*，如果存在的话，也会加载文件 *filename.target* 中的配置。
  对于列表中的 *filename* 的每一个值，也会加载文件 *filename.target* （如果存在的话）中的配置。
- BUILD_DIR - 用于放置 ARMINO 构建相关工具的目录，如生成的二进制文件、文本文件、组件；默认为CMAKE_BINARY_DIR。
- COMPONENTS - 从构建系统已知的组件中选择要处理的组件（通过 ``armino_build_component`` 添加）。这个参数用于精简构建过程。
  如果在依赖链中需要其它组件，则会自动添加，即自动添加这个列表中组件的公共和私有依赖项，进而添加这些依赖项的公共和私有依赖，以此类推。如果不指定，则会处理构建系统已知的所有组件。

.. code-block:: none

  armino_build_executable(executable)

指定 ARMINO 构建的可执行文件 *executable*。这将添加额外的目标，如与 flash 相关的依赖关系，生成额外的二进制文件等。应在 ``armino_build_process`` 之后调用。

.. code-block:: none

  armino_build_get_config(var config [GENERATOR_EXPRESSION])

获取指定配置的值。就像构建属性一样，特定 *GENERATOR_EXPRESSION* 将获取该配置的生成器表达式字符串，而不是实际值，即可以与支持生成器表达式的 CMake 命令一起使用。然而，实际的配置值只有在调用 ``armino_build_process`` 后才能知道。

.. _cmake-build-properties:

构建属性
--------------------

可以通过使用构建命令 ``armino_build_get_property`` 来获取构建属性的值。例如，以下命令可以获取构建过程中使用的 Python 解释器的相关信息。

.. code-block:: none

  armino_build_get_property(python PYTHON)
  message(STATUS "The Python intepreter is: ${python}")

- BUILD_DIR - 构建目录；由 ``armino_build_process`` 的 BUILD_DIR 参数设置。
- BUILD_COMPONENTS - 包含在构建中的组件列表；由 ``armino_build_process`` 设置。
- BUILD_COMPONENT_ALIASES - 包含在构建中的组件的库别名列表；由 ``armino_build_process`` 设置。
- C_COMPILE_OPTIONS - 适用于所有组件的 C 源代码文件的编译选项。
- COMPILE_OPTIONS - 适用于所有组件的源文件（无论是 C 还是 C++）的编译选项。
- COMPILE_DEFINITIONS - 适用于所有组件源文件的编译定义。
- CXX_COMPILE_OPTIONS - 适用于所有组件的 C++ 源文件的编译选项。
- EXECUTABLE - 项目可执行文件；通过调用 ``armino_build_executable`` 设置。
- EXECUTABLE_NAME - 不含扩展名的项目可执行文件的名称；通过调用 ``armino_build_executable`` 设置。
- EXECUTABLE_DIR - 输出的可执行文件的路径
- ARMINO_PATH - ARMINO 路径；由 ARMINO_PATH 环境变量设置，或者从 ``armino.cmake`` 的位置推断。
- ARMINO_SOC - 构建的目标芯片；由 ``armino_build_process`` 的目标参数设置。
- ARMINO_VER - ARMINO 版本；由版本文件或 ARMINO_PATH 仓库的 Git 版本设置。
- INCLUDE_DIRECTORIES - 包含所有组件源文件的目录。
- KCONFIGS - 构建过程中组件里的 Kconfig 文件的列表；由 ``armino_build_process`` 设置。
- KCONFIG_PROJBUILDS - 构建过程中组件中的 Kconfig.projbuild 文件的列表；由 ``armino_build_process`` 设置。
- PROJECT_NAME - 项目名称；由 ``armino_build_process`` 的 PROJECT_NAME 参数设置。
- PROJECT_DIR - 项目的目录；由 ``armino_build_process`` 的 PROJECT_DIR 参数设置。
- PROJECT_VER - 项目的版本；由 ``armino_build_process`` 的 PROJECT_VER 参数设置。
- PYTHON - 用于构建的 Python 解释器；如果有则从 PYTHON 环境变量中设置，如果没有，则使用 "python"。
- SDKCONFIG - 输出的配置文件的完整路径；由 ``armino_build_process`` SDKCONFIG 参数设置。
- SDKCONFIG_DEFAULTS - 包含默认配置的文件列表；由 ``armino_build_process`` SDKCONFIG_DEFAULTS 参数设置。
- SDKCONFIG_HEADER - 包含组件配置的 C/C++ 头文件的完整路径；由 ``armino_build_process`` 设置。
- SDKCONFIG_CMAKE - 包含组件配置的 CMake 文件的完整路径；由 ``armino_build_process`` 设置。
- SDKCONFIG_JSON - 包含组件配置的 JSON 文件的完整路径；由 ``armino_build_process`` 设置。
- SDKCONFIG_JSON_MENUS - 包含配置菜单的 JSON 文件的完整路径；由 ``armino_build_process`` 设置。

组件 API
----------------------

.. code-block:: none

  armino_component_get_property(var component property [GENERATOR_EXPRESSION])

获取一个指定的 *component* 的 :ref:`组件属性<cmake-component-properties>` *property*，并将其存储在当前作用域可访问的 *var* 中。指定 *GENERATOR_EXPRESSION* 将获取该属性的生成器表达式字符串（不是实际值），它可以在支持生成器表达式的 CMake 命令中使用。

.. code-block:: none

  armino_component_set_property(component property val [APPEND])

设置指定的 *component* 的 :ref:`组件属性<cmake-component-properties>`，*property* 的值为 *val*。特定 *APPEND* 将把指定的值追加到属性的当前值后。如果该属性之前不存在或当前为空，指定的值将成为第一个元素/成员。

.. _cmake-component-register:

.. code-block:: none

  armino_component_register([[SRCS src1 src2 ...] | [[SRC_DIRS dir1 dir2 ...] [EXCLUDE_SRCS src1 src2 ...]]
                         [INCLUDE_DIRS dir1 dir2 ...]
                         [PRIV_INCLUDE_DIRS dir1 dir2 ...]
                         [REQUIRES component1 component2 ...]
                         [PRIV_REQUIRES component1 component2 ...]
                         [LDFRAGMENTS ldfragment1 ldfragment2 ...]
                         [REQUIRED_ARMINO_SOCS target1 target2 ...]
                         [EMBED_FILES file1 file2 ...]
                         [EMBED_TXTFILES file1 file2 ...]
                         [KCONFIG kconfig]
                         [KCONFIG_PROJBUILD kconfig_projbuild])

将一个组件注册到构建系统中。就像 ``project()`` CMake 命令一样，该命令应该直接从组件的 CMakeLists.txt 中调用（而不是通过函数或宏），且建议在其他命令之前调用该命令。下面是一些关于在 ``armino_component_register`` 之前 *不能* 调用哪些命令的指南：

  - 在 CMake 脚本模式下无效的命令。
  - 在 project_include.cmake 中定义的自定义命令。
  - 除了 ``armino_build_get_property`` 之外，构建系统的 API 命令；但要考虑该属性是否有被设置。

对变量进行设置和操作的命令，一般可在 ``armino_component_register`` 之前调用。

``armino_component_register`` 的参数包括：

  - SRCS - 组件的源文件，用于为组件创建静态库；如果没有指定，组件将被视为仅配置组件，从而创建接口库。
  - SRC_DIRS, EXCLUDE_SRCS - 用于通过指定目录来 glob 源文件 (.c、.cpp、.S)，而不是通过 SRCS 手动指定源文件。请注意，这受 :ref:`CMake 中通配符的限制<cmake-file-globbing>`。 在 EXCLUDE_SRCS 中指定的源文件会从被 glob 的文件中移除。
  - INCLUDE_DIRS - 相对于组件目录的路径，该路径将被添加到需要当前组件的所有其他组件的 include 搜索路径中。
  - PRIV_INCLUDE_DIRS - 必须是相对于组件目录的目录路径，它仅被添加到这个组件源文件的 include 搜索路径中。
  - REQUIRES - 组件的公共组件依赖项。
  - PRIV_REQUIRES - 组件的私有组件依赖项；在仅用于配置的组件上会被忽略。
  - LDFRAGMENTS - 组件链接器片段文件。
  - REQUIRED_ARMINO_SOCS - 指定该组件唯一支持的目标。
  - KCONFIG - 覆盖默认的 Kconfig 文件。
  - KCONFIG_PROJBUILD - 覆盖默认的 Kconfig.projbuild 文件。

以下内容用于 :ref:`将数据嵌入到组件中<cmake_embed_data>`，并在确定组件是否仅用于配置时被视为源文件。这意味着，即使组件没有指定源文件，如果组件指定了以下其中之一，仍然会在内部为组件创建一个静态库。

  - EMBED_FILES - 嵌入组件的二进制文件
  - EMBED_TXTFILES - 嵌入组件的文本文件
  
.. _cmake-component-properties:

组件属性
------------------------

组件的属性值可以通过使用构建命令 ``armino_component_get_property`` 来获取。例如，以下命令可以获取 ``freertos`` 组件的目录。

.. code-block:: cmake

  armino_component_get_property(dir freertos COMPONENT_DIR)
  message(STATUS "The 'freertos' component directory is: ${dir}")

- COMPONENT_ALIAS - COMPONENT_LIB 的别名，用于将组件链接到外部目标；由 ``armino_build_component`` 设置，别名库本身由 ``armino_component_register`` 创建。
- COMPONENT_DIR - 组件目录；由 ``armino_build_component`` 设置。
- COMPONENT_OVERRIDEN_DIR - 如果 :ref:`这个组件覆盖了另一个组件<cmake-components-same-name>`，则包含原组件的目录。
- COMPONENT_LIB - 所创建的组件静态/接口库的名称；由 ``armino_build_component`` 设置，库本身由 ``armino_component_register`` 创建。
- COMPONENT_NAME - 组件的名称；由 ``armino_build_component`` 根据组件的目录名设置。
- COMPONENT_TYPE - 组件的类型（LIBRARY 或 CONFIG_ONLY）。如果一个组件指定了源文件或嵌入了一个文件，那么它的类型就是 LIBRARY。
- EMBED_FILES - 要嵌入组件的文件列表；由 ``armino_component_register`` EMBED_FILES 参数设置。
- EMBED_TXTFILES - 要嵌入组件的文本文件列表；由 ``armino_component_register`` EMBED_TXTFILES 参数设置。
- INCLUDE_DIRS - 组件 include 目录列表；由 ``armino_component_register`` INCLUDE_DIRS 参数设置。
- KCONFIG - 组件 Kconfig 文件；由 ``armino_build_component`` 设置。
- KCONFIG_PROJBUILD - 组件 Kconfig.projbuild；由 ``armino_build_component`` 设置。
- LDFRAGMENTS - 组件链接器片段文件列表；由 ``armino_component_register`` LDFRAGMENTS 参数设置。
- PRIV_INCLUDE_DIRS - 组件私有 include 目录列表；在 LIBRARY 类型的组件 ``armino_component_register`` PRIV_INCLUDE_DIRS 参数中设置。
- PRIV_REQUIRES - 私有组件依赖关系列表；由 ``armino_component_register`` PRIV_REQUIRES 参数设置。
- REQUIRED_ARMINO_SOCS - 组件支持的目标列表；由 ``armino_component_register``  EMBED_TXTFILES 参数设置。
- REQUIRES - 公共组件依赖关系列表；由 ``armino_component_register`` REQUIRES 参数设置。
- SRCS - 组件源文件列表；由 ``armino_component_register`` 的 SRCS 或 SRC_DIRS/EXCLUDE_SRCS 参数设置。