Build APIs
==========

:link_to_translation:`en:[English]`


Build APIs
----------

.. code-block:: none

  armino_build_get_property(var property [GENERATOR_EXPRESSION])

Get a :ref:`build property <cmake-build-properties>`，and store in var.
The specific *GENERATOR_EXPRESSION* will get the generator expression string (not the actual value) for the property, which can be used with CMake commands that support generator expressions.

.. code-block:: none

  armino_build_set_property(property val [APPEND])

Set a :ref:`build property <cmake-build-properties>` to *val*. *APPEND* will append the specified value to the current value of the property.
If the attribute does not exist or is empty, the specified value becomes the first element/member.

.. code-block:: none

  armino_build_component(component_dir)

Submit a *component_dir* directory containing components to the build system.Relative paths are converted to absolute paths relative to the current directory.
All calls to this command must be executed before `armino_build_process`.

This command does not guarantee that the component will be processed during the build process（see the description of the `COMPONENTS` parameter in `armino_build_process` ）

.. code-block:: none

  armino_build_process(target
                    [PROJECT_DIR project_dir]
                    [PROJECT_VER project_ver]
                    [PROJECT_NAME project_name]
                    [SDKCONFIG sdkconfig]
                    [SDKCONFIG_DEFAULTS sdkconfig_defaults]
                    [BUILD_DIR build_dir]
                    [COMPONENTS component1 component2 ...])

Performs a lot of behind-the-scenes work for importing ARMINO components, including component configuration, library creation, dependency expansion, and resolution. Of these functions, probably the most important to the user is to create a library by calling ``armino_component_register`` on each component. This command creates libraries for each component, which can be accessed using aliases of the form ``armino::*component_name*``.
These aliases can be used to link components to the user's own target, library, or executable.

This call requires the *target* chip to be specified with the target parameter.Optional arguments to the call include:

- PROJECT_DIR - Project directory, defaults to CMAKE_SOURCE_DIR.
- PROJECT_NAME - Project name, defaults to CMAKE_PROJECT_NAME.
- PROJECT_VER - The version/version number of the project, defaults to "1".
- SDKCONFIG - The output path to the generated sdkconfig file, depending on whether PROJECT_DIR  is set, defaults to  PROJECT_DIR/sdkconfig or CMAKE_SOURCE_DIR/sdkconfig.
- SDKCONFIG_DEFAULTS - List of files containing default configurations (full path must be included in the list), empty by default;
  for each value *filename*in the list, the configuration in the *filename.target* is also loaded if it exists.
  For each value of *filename* in the list, the configuration in the *filename.target* (if it exists) is also loaded.
- BUILD_DIR - Directory to place ARMINO build related tools such as generated binaries, text files, components; defaults to CMAKE_BINARY_DIR。
- COMPONENTS - Select components to process from components known to the build system（added via ``armino_build_component`` ）.This parameter is used to streamline the build process.
  If other components are needed in the dependency chain, they will be added automatically, that is,the public and private dependencies of the components in this list are automatically added, and then the public and private dependencies of these dependencies are added, and so on. If not specified, all components known to the build system are processed.

.. code-block:: none

  armino_build_executable(executable)

Specifies the *executable* built by ARMINO.This will add extra targets like flash-related dependencies, generate extra binaries, etc.Should be called after ``armino_build_process``.

.. code-block:: none

  armino_build_get_config(var config [GENERATOR_EXPRESSION])

Get the value of the specified configuration. Just like build properties, the specific *GENERATOR_EXPRESSION* will get the generator expression string for that configuration,not the actual value, i.e. can be used with CMake commands that support generator expressions.However, the actual configuration values are not known until ``armino_build_process`` is called.

.. _cmake-build-properties:

build properties
----------------

The value of a build property can be obtained by using the build command ``armino_build_get_property``. For example, the following command can get information about the Python interpreter used in the build process.

.. code-block:: none

  armino_build_get_property(python PYTHON)
  message(STATUS "The Python intepreter is: ${python}")

- BUILD_DIR - build directory; set by the BUILD_DIR parameter of ``armino_build_process``.
- BUILD_COMPONENTS - List of components to include in the build; set by ``armino_build_process``.
- BUILD_COMPONENT_ALIASES - List of library aliases for components included in the build; set by ``armino_build_process``.
- C_COMPILE_OPTIONS - Compilation options for C source code files for all components.
- COMPILE_OPTIONS - Compilation options that apply to all component source files (whether C or C++).
- COMPILE_DEFINITIONS - Compilation definitions that apply to all component source files.
- CXX_COMPILE_OPTIONS - Compilation options for C++ source files for all components.
- EXECUTABLE - Project executable; set by calling ``armino_build_executable``.
- EXECUTABLE_NAME - The name of the project executable without the extension; set by calling ``armino_build_executable``.
- EXECUTABLE_DIR - Path to the output executable
- ARMINO_PATH - ARMINO  path; set by the ARMINO_PATH environment variable, or inferred from the location of ``armino.cmake`` .
- ARMINO_SOC - the target chip to build; set by the target parameter of ``armino_build_process``.
- ARMINO_VER - ARMINO version; set by the version file or the Git version of the ARMINO_PATH repository.
- INCLUDE_DIRECTORIES - Directories containing all component source files.
- KCONFIGS - List of Kconfig files in the component during the build process; set by ``armino_build_process``.
- KCONFIG_PROJBUILDS -  List of Kconfig.projbuild files in the component during the build process; set by ``armino_build_process``.
- PROJECT_NAME - Project name; set by the PROJECT_NAME parameter of ``armino_build_process``.
- PROJECT_DIR - The project's directory; set by the PROJECT_DIR parameter of ``armino_build_process``.
- PROJECT_VER - The version of the project; set by the PROJECT_VER parameter of ``armino_build_process``.
- PYTHON - The Python interpreter to use for the build; set from the PYTHON environment variable if available, use "python" if not.
- SDKCONFIG - The full path to the output configuration file; set by the ``armino_build_process`` SDKCONFIG parameter.
- SDKCONFIG_DEFAULTS - List of files containing default configuration; set by ``armino_build_process`` SDKCONFIG_DEFAULTS parameter.
- SDKCONFIG_HEADER - Full path to C/C++ header files containing component configuration; set by ``armino_build_process``.
- SDKCONFIG_CMAKE - Full path to CMake file containing component configuration; set by ``armino_build_process``.
- SDKCONFIG_JSON - Full path to JSON file containing component configuration; set by ``armino_build_process``.
- SDKCONFIG_JSON_MENUS - Full path to JSON file containing configuration menus; set by ``armino_build_process``.

Component API
-------------

.. code-block:: none

  armino_component_get_property(var component property [GENERATOR_EXPRESSION])

Gets the *component* of a specified component :ref:`component property<cmake-component-properties>` *property*，and stores it in a *var* accessible by the current scope. Specifying *GENERATOR_EXPRESSION*  will get the generator expression string (not the actual value) for the property,which can be used in CMake commands that support generator expressions.

.. code-block:: none

  armino_component_set_property(component property val [APPEND])

Set the *component* of a specified component :ref:`component property<cmake-component-properties>`，the value of *property* is *val*. A specific *APPEND* will append the specified value to the current value of the property. If the attribute did not exist before or is currently empty, the specified value will be the first element/member.

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

Register a component with the build system. Just like the ``project()`` CMake command, this command should be called directly from the component's CMakeLists.txt (not through a function or macro) and is recommended to be called before other commands. Here are some guidelines on which commands *cannot* be called before ``armino_component_register``：

  - Invalid command in CMake script mode.
  - Custom commands defined in project_include.cmake.
  - API commands for build systems, except ``armino_build_get_property``,except armino_build_get_property; but consider whether this property is set.

Commands for setting and operating variables can generally be called before ``armino_component_register``.

The parameters of ``armino_component_register`` include:

  - SRCS - The component's source file, used to create a static library for the component; if not specified, the component will be treated as a configuration-only component, creating an interface library.
  - SRC_DIRS, EXCLUDE_SRCS - Used to glob source files (.c, .cpp, .S) by specifying directories instead of manually specifying source files via SRCS. Note that this is subject to :ref:`wildcards in CMake.<cmake-file-globbing>`. Source files specified in EXCLUDE_SRCS are removed from the globbed files.
  - INCLUDE_DIRS - A path relative to the component directory that will be added to the include search paths of all other components that require the current component.
  - PRIV_INCLUDE_DIRS - Must be a directory path relative to the component directory, it is only added to the include search path for this component's source files.
  - REQUIRES - Common component dependencies of the component.
  - PRIV_REQUIRES - The component's private component dependencies; ignored on configuration-only components.
  - LDFRAGMENTS - Component linker fragment files.
  - REQUIRED_ARMINO_SOCS - Specifies the only supported targets for this component.
  - KCONFIG - Overrides the default Kconfig file.
  - KCONFIG_PROJBUILD - Overrides the default Kconfig.projbuild file.

The following :ref:`are used to embed data into a component <cmake_embed_data>`，and are considered source files when determining whether a component is only used for configuration. This means that even if the component does not specify a source file, if the component specifies one of the following, a static library will still be created internally for the component.

  - EMBED_FILES - Binary files for embedded components
  - EMBED_TXTFILES - Text files for embedded components

.. _cmake-component-properties:

component properties
--------------------

Component property values can be obtained by using the build command ``armino_component_get_property``. For example, the following command can get the directory of the ``freertos`` component.

.. code-block:: cmake

  armino_component_get_property(dir freertos COMPONENT_DIR)
  message(STATUS "The 'freertos' component directory is: ${dir}")

- COMPONENT_ALIAS - Alias for COMPONENT_LIB, used to link components to external targets; set by ``armino_build_component``, the alias library itself is created by ``armino_component_register``.
- COMPONENT_DIR -  Component directory; set by ``armino_build_component``.
- COMPONENT_OVERRIDEN_DIR - If :ref:`If this component overwrites another component<cmake-components-same-name>`, the directory containing the original component.
- COMPONENT_LIB - The name of the component static/interface library created; set by ``armino_build_component``, the library itself is created by ``armino_component_register``.
- COMPONENT_NAME - The name of the component; set by ``armino_build_component`` based on the component's directory name.
- COMPONENT_TYPE - The type of component (LIBRARY or CONFIG_ONLY). If a component specifies a source file or embeds a file, its type is LIBRARY.
- EMBED_FILES - List of files to embed the component; set by the ``armino_component_register`` EMBED_FILES parameter.
- EMBED_TXTFILES - List of text files to embed the component; set by the ``armino_component_register`` EMBED_TXTFILES parameter.
- INCLUDE_DIRS -  List of component include directories; set by the ``armino_component_register`` INCLUDE_DIRS parameter.
- KCONFIG - Component Kconfig file; set by ``armino_build_component``.
- KCONFIG_PROJBUILD -  Component Kconfig.projbuild; set by ``armino_build_component``.
- LDFRAGMENTS - List of component linker fragment files; set by the ``armino_component_register`` LDFRAGMENTS parameter.
- PRIV_INCLUDE_DIRS -  List of component private include directories; set in the LIBRARY type component ``armino_component_register`` PRIV_INCLUDE_DIRS parameter.
- PRIV_REQUIRES - List of private component dependencies; set by the ``armino_component_register`` PRIV_REQUIRES parameter.
- REQUIRED_ARMINO_SOCS - List of targets supported by the component; set by the ``armino_component_register``  EMBED_TXTFILES parameter.
- REQUIRES - List of public component dependencies; set by the ``armino_component_register`` REQUIRES parameter.
- SRCS - List of component source files; set by the SRCS or SRC_DIRS/EXCLUDE_SRCS parameter of ``armino_component_register`` .