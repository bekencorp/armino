Example
=======

:link_to_translation:`zh_CN:[中文]`

.. _001_build_project_demo:


001 - Minimum Project
---------------------


Each project has a top-level ``CMakeLists.txt`` file that contains build settings for the entire project, a minimal project looks like this::

        cmake_minimum_required(VERSION 3.5)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

:example:`example <build_system/001_simplest_proj>`

CMakeLists.txt Description
**************************

Add two lines of code to each project in the order above:

- ``cmake_minimum_required(VERSION 3.5)`` Must be placed on the first line of the CMakeLists.txt file, it tells CMake the minimum version number required to build the project. ARMINO supports version 3.5 or higher.
- ``include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)`` The rest of CMake's functions are imported to complete tasks such as configuring projects, searching for components, etc.
- ``project(myProject)`` A project is created, and a project name is specified. This name will be used as the name of the final output binaries,namely ``myProject.elf`` and ``myProject.bin``.Only one project can be defined per CMakeLists.txt file.


Default project variable
************************

The following variables have default values that users can override to customize build behavior.

- ``COMPONENT_DIRS``：The search directory for components, defaults to  ``ARMINO_PATH/components``, ``ARMINO_PATH/middleware``, ``PROJECT_DIR/components``,
  and ``EXTRA_COMPONENTS_DIRS``.Override this variable if you do not want to search for components in these locations.
- ``EXTRA_COMPONENTS_DIRS``：A list of additional optional directories to search for components. The path can be relative to the project directory or absolute.
- ``COMPONENTS``：List of component names to build into the project, defaults to all components retrieved under the  ``COMPONENT_DIRS`` directory.Use this variable to "lean" the project to reduce build time.
  Note that if a component specifies another component it depends on via  ``COMPONENT_REQUIRES`` directory.it will be automatically added to ``COMPONENTS``.

The paths in the above variables can be absolute paths, or relative paths relative to the project directory.

.. note::

    Please use `set command of cmake`_ to set these variables,Such as ``set(VARIABLE "VALUE")``. Note that,``set()`` command needs to be placed before ``include(...)`` .

Default build options
*********************

TODO

Default build behavior
**********************

TODO

.. _002_build_project_demo:

002 - Add component search directory
------------------------------------

You can set EXTRA_COMPONENTS_DIRS in the top-level CMakeLists.txt of the project to import components other than the default component search directory::

        cmake_minimum_required(VERSION 3.5)
        set(EXTRA_COMPONENTS_DIRS my_component_dir)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)


:example:`example <build_system/002_add_components_dir>`

.. _003_build_project_demo:

003 - Don't build specific components
-------------------------------------

EXCLUDE_COMPONENTS can be set in the top-level CMakeLists.txt of the project to exclude certain components from building::

        cmake_minimum_required(VERSION 3.5)
        set(EXCLUDE_COMPONENTS c1_not_build)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

:example:`example <build_system/003_exclude_components>`

.. _004_build_project_demo:


004 - Build only specific components
------------------------------------

You can set COMPONENTS in the top-level CMakeLists.txt of the project to build only specific components.In the example, only c3_contain_main and dependent components are built::

        cmake_minimum_required(VERSION 3.5)
        set(COMPONENTS c3_contain_main)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

Notice：

 - All components and dependencies in the COMPONENTS list will be built.
 - Since all components depend on the common component, the common component is also built.
 - The ``main()`` function implementation must be included in the build component.

:example:`example <build_system/004_set_components>`

.. _005_build_project_demo:

005 - Override default compile options
--------------------------------------

After project() in the top-level CMakeLists.txt, use Wno-extra to override the default Wextra::

        cmake_minimum_required(VERSION 3.5)
        include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
        project(myProject)

        armino_build_set_property(COMPILE_OPTIONS "-Wno-extra" APPEND)

.. note:

  Your compile options should be set after project() because the default build specification is set inside project() .

:example:`example <build_system/005_proj_build_options>`

.. _101_build_component_demo:

101 - Minimal Components
------------------------

The simplest component CMakeLists.txt is as follows, calling ``armino_component_register()`` Register the component with the build system::

        armino_component_register(SRCS c1.c INCLUDE_DIRS include)

:example:`example <build_system/101_simplest_component>`

.. _102_build_component_demo:

102 - Set component compilation options
---------------------------------------

When compiling source files for a specific component,you can use ``target_compile_options`` command to pass compiler options::

  target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)

If you specify compiler flags for a single source file, you can use CMake's `set_source_files_properties`_ command::

    set_source_files_properties(compile_options.c
        PROPERTIES COMPILE_FLAGS
        -Wno-unused-variable
    )

Note that, the above two commands can only be called after the ``armino_component_register`` command in the component CMakeLists file.

:example:`example <build_system/102_build_options>`

.. _103_build_component_demo:

103 - Overlay Components
------------------------

The armino default component can be overridden by defining a component with the same name, an example is using the bk_log defined in the project component to override the :armino::`<components/bk_log>` component.

:example:`example <build_system/103_overwrite_component>`

.. _104_build_component_demo:

104 - Set component dependencies
--------------------------------

The example contains four components::

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

:example:`example <build_system/104_dependency>`

.. _105_build_component_demo:

105 - Link Libraries in Components
----------------------------------

Libraries can be imported via ``add_prebuilt_library`` ::

  add_prebuilt_library(target_name lib_path [REQUIRES req1 req2 ...] [PRIV_REQUIRES req1 req2 ...])

among:

- ``target_name``- The name used to refer to the imported library, such as when linking to other targets
- ``lib_path``- the path to the prebuilt library, either absolute or relative to the component directory.

The optional parameters ``REQUIRES`` and ``PRIV_REQUIRES`` specify dependencies on other components. These parameters have the same meaning as the parameters of ``armino_component_register``.

.. note::

    Note that the compilation target of the prebuilt library should be the same as the current project.The relevant parameters of the pre-built library should also match. These two factors can lead to bugs in your application if you don't pay special attention.

:example:`example <build_system/105_link_lib>`

.. _106_build_component_demo:

106 - Pure CMake ARMINO Components
----------------------------------

Usually the components on the ARMINO component search path should be registered according to ARMINO requirements, but if you want to change the components on the ARMINO component search path to pure CMake components,
the code is as follows::

    if (CMAKE_BUILD_EARLY_EXPANSION)
            return()
    endif()

    add_library(c1 STATIC c1.c)
    target_include_directories(c1 PUBLIC include)

The first three lines of code are used to tell the ARMINO build system not to add this component, and the c1 component will be built in pure CMake. In addition, you also need to add this component to the build system in the way of CMake, This example is to add the c1 component to the build tree in the top-level CMakeLists.txt of the project::

    cmake_minimum_required(VERSION 3.5)
    include($ENV{ARMINO_PATH}/tools/build_toos/cmake_project.cmake)
    project(cmake_exam)

    add_subdirectory(components/c1)

:example:`example <build_system/106_pure_cmake_component>`

.. note::

    In general, components in the ARMINO search path should call armino_component_register() to write components in the way that ARMINO requires. When you have a specific reason to write your own pure CMake components, you can choose to press _<import third-party CMake components in ARMINO components> or a relative path relative to the project directory. _<Import 3rd party CMake components anywhere> way to import your pure CMake components.

.. _107_build_component_demo:

107 - Import pure CMake components 1
------------------------------------

In the example, foo is built using pure CMake, placed in the main component, and can be imported as follows::

    armino_component_register(SRCS "main.c" INCLUDE_DIRS .)
    add_subdirectory(foo)
    target_link_libraries(${COMPONENT_LIB} PUBLIC foo)

:example:`example <build_system/107_pure_cmake_in_main>`

.. _108_build_component_demo:

108 - Import pure CMake components 2
------------------------------------

In the example, foo is built with pure CMake, placed in the c1 component, and can be imported using the following methods::

    armino_component_register(SRCS "c1.c" INCLUDE_DIRS include)
    add_subdirectory(foo)
    target_link_libraries(${COMPONENT_LIB} PUBLIC foo)

:example:`example <build_system/108_pure_cmake_in_component>`

.. _109_build_component_demo:

109 - Import pure CMake components 3
------------------------------------

In the example, anywhere is placed in the root directory of the project, and built with pure CMake, you can add this line of code to the top-level CMakeLists.txt of the project to import::

    add_subdirectory(anywhere)

In fact third-party pure CMake components can be imported in any way you like.

:example:`example <build_system/109_pure_cake_in_anywhere>`

.. _110_use_armino_lib_in_pure_cmake:

110 - Pure CMake components use armino components
-------------------------------------------------

The way to reference armino components in pure CMake components is armino::component_name. In the example, the c1 component in the anywhere directory will use the armino component c component::

    target_link_libraries(c1 armino::c)

:example:`example <build_system/110_use_armino_lib_in_pure_cmake>`

.. _111_build_component_demo:

111 - Import GNU Makefile project 1
-----------------------------------

If you have a component not written using cmake, for example, your component is written in a GNU Makefile, and you want to use this component in ARMINO, but you don't want to rewrite the component build to CMake. In this case, you need to use the ExternalProject function of CMake.

Example to import a foo component written with a Makefile into the c1 component::

    # External build process for foo, run in the source directory
    # and generate libfoo.a
    externalproject_add(foo_build
        PREFIX ${COMPONENT_DIR}
        SOURCE_DIR ${COMPONENT_DIR}/foo
        CONFIGURE_COMMAND ""
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make CC=${CMAKE_C_COMPILER} libfoo.a
        INSTALL_COMMAND ""
        )

    # Add libfoo.a to the build system
    add_library(foo STATIC IMPORTED GLOBAL)
    add_dependencies(foo foo_build)

    set_target_properties(foo PROPERTIES IMPORTED_LOCATION
        ${COMPONENT_DIR}/foo/libfoo.a)
    set_target_properties(foo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
        ${COMPONENT_DIR}/foo/include)

    set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
        "${COMPONENT_DIR}/foo/libfoo.a")

（The above CMakeLists.txt can be used to create a component named ``foo`` that builds libfoo.a with its own Makefile.）

- ``externalproject_add`` defines an external build system.

  - Set ``SOURCE_DIR``, ``CONFIGURE_COMMAND``, ``BUILD_COMMAND`` and ``INSTALL_COMMAND``.If the external build system does not configure this step, ``CONFIGURE_COMMAND`` can be set to an empty string.In Armino's build system, the ``INSTALL_COMMAND`` variable is generally set to empty.
  - Set ``BUILD_IN_SOURCE``, i.e. the build directory is the same as the source directory. Otherwise,you can also set the ``BUILD_DIR`` variable.
  - See `ExternalProject_Add`_ for details on the ``externalproject_add()`` command.

- The second set of commands adds a target library that points to the library files generated by the external build system. In order to add the include directory and tell CMake where the file is, some more properties need to be set.
- Finally, the generated library is added to `ADDITIONAL_MAKE_CLEAN_FILES`_ . That is, the library will be removed after ``make clean`` is executed. Note that other object files in the build system are not removed.

:example:`example <build_system/111_use_gnu_make_project1>`

.. _112_build_component_demo:

112 - Import GNU Makefile project 2
-----------------------------------

Another way to import a GNU project is to import a project written in a GNU Makefile via add_custom_command.

Example to import a foo component written with a Makefile into the c1 component::

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

The above use case first calls ``armino_component_register`` to register a standard armino component ``c1``, then adds a command to generate ``libfoo.a`` through ``add_custom_command()``, and then calls ``add_custom_target()`` to add the target ``foo_build``.

The next four commands create the ``foo`` target, setting the location and header directory for the library corresponding to ``foo``. Finally, attach the target ``foo`` to the armino standard component ``c1``.

.. note::

    When using ``add_custom_command()`` it should be noted that the file generated after OUTPUT must be directly used in the component CMakeLists.txt corresponding to the armino component, so that only in this way will the dependency rule of the Makefile be triggered to call the added COMMAND. Otherwise, since the generated file is not used in CMakeLists.txt, the Makefile will think this file is not needed by the build system and will not trigger the command call!

:example:`example <build_system/112_use_gnu_make_project2>`

.. _113_build_component_demo:

113 - Use armino in custom cmake projects
-----------------------------------------

When you need to port armino to open source platforms like zephyr/rtt/alios, one way is to put armino compiled into a library.

:example:`example <build_system/113_armino_as_lib>`

.. _201_build_project_demo:

201 - Simplest component Kconfig
--------------------------------

Each component can contain a ``Kconfig`` file that contains some configuration setting information to be added to the component's configuration menu.

These settings can be found under the ``Component Settings`` menu bar when you run menuconfig.

The easiest way to create a Kconfig file for a component is to use an existing Kconfig file in ARMINO as a template and modify it based on that.

Simplest component Kconfig::

        config C1
            bool "Enable component c1"
            default y

The build system will add the following configuration items to the generated sdkconfig::

        CONFIG_C1=y

The build system will add the following configuration items to sdkconfig.h in the build root directory (usually the build directory)::

        #define CONFIG_C1 1

.. note:

  sdkconfig.h should be included if CONFIG_C1 is used in the source file.

:example:`example <build_system/201_simplest_kconfig>`

.. _202_build_project_demo:

202 - Add global configuration
------------------------------

A KConfig file can be defined for a component to implement global component configuration.If you want to add configuration options at the top level of menuconfig,rather than in the "Component Configuration" submenu, you can define these options in the KConfig.projbuild file in the same directory as the CMakeLists.txt file.

Usually a project-specific Kconfig.projbuild is added to the main component. But be careful when adding configurations in this file as these are included in the overall project configuration.
Where possible, create a KConfig file for component configuration.

:example:`example <build_system/202_global_kconfig>`

.. _203_build_component_demo:

203 - Pure Configuration Components
-----------------------------------

A component may not contain any source files and header files, but only the Kconfig configuration file, which is called a pure configuration component::

        armino_component_register()

.. note:

    Pure configuration components need to call armino_component_register() to register themselves with the build system.

:example:`example <build_system/203_config_only>`


204 - Custom project configuration
----------------------------------

The order in which armino loads Kconfig is as follows. For the same configuration item, the value loaded later will override the value loaded first：

 - group Kconfig default configuration
 - :middleware:: Target specific default configuration in `<arch/bkxxx/bkxxx.defconfig>`
 - Project-related, target-common configuration is defined in the project root directory /config/common.config
 - Project-related, target-specific configuration defined in the project root directory /config/bkxxx.config

Applications can set project/target related configuration items by configuring
"project root directory/config/common.config" and "project root directory/config/bkxxx.config", where bkxxx is a specific SoC, such as bk7256, etc.

:example:`example <build_system/205_project_per_soc_config>`

207 - Disable components via Kconfig
------------------------------------

There are several ways to disable a component, one of which is through the component enable configuration in Kconfig::

    set(src)
    set(inc)

    if (CONFIG_C1)
        list(APPEND src c1.c)
        list(APPEND inc include)
    endif()

    armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc})

Note that the following is the wrong way to disable a component. Because the armino build system relies on ``armino_component_register()`` to generate the component during the component list generation phase (ie, the early augmentation phase), and the component's Kconfig configuration is loaded during the component processing phase only if the component is generated first::

    if (CONFIG_C1)
        armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc})
    endif()

The following way of writing may also cause problems, when the component (TODO) ::

    if (CONFIG_C1)
        armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc} REQUIRES c2 c3 c4)
    else()
        armino_component_register()
    endif()

Other ways to disable components are:

 - Disable (TODO) via ARMINO_SOC
 - Disable (TODO) with EXCLUDE_COMPONENTS

:example:`example <build_system/207_disable_components>`

.. _set command of cmake: https://cmake.org/cmake/help/latest/command/set.html
.. _set_source_files_properties: https://cmake.org/cmake/help/latest/command/set_source_files_properties.html
.. _ExternalProject_Add: https://cmake.org/cmake/help/latest/module/ExternalProject.html
.. _ADDITIONAL_MAKE_CLEAN_FILES: https://cmake.org/cmake/help/latest/prop_dir/ADDITIONAL_MAKE_CLEAN_FILES.html