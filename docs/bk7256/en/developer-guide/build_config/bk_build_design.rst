build implementation
====================

Design ideas
------------

ARMINO construction is mainly designed with object-oriented thinking, and the key points to understand:

    - Second scan of CMakeLists.txt, there is a chance to do some special processing during dependency expansion
    - Dependency expansion is performed before building to simplify the writing of dependencies
    - projects/components is derived from cmake's projects/target, overriding default values by setting specific variables before the function call, and by setting specific properties after the function call
    - Allow projects/components to be hooked before, during, and after core script execution
    - Provide packaged APIs to handle more than 90% of the vast majority of scenarios, only a few special scenarios require complex cmake script processing

build script
------------

The listing file for the ARMINO build system is located in :armino:`/tools/build_tools/cmake`.The modules that implement the core functions of the build system are as follows

    - build.cmake - build related functions, i.e. build initialization, retrieving/setting build properties, build processing.
    - component.cmake - Component related functions, such as adding components, retrieving/setting component properties, registering components.
    - kconfig.cmake - Generate configuration files (sdkconfig, sdkconfig.h, sdkconfig.cmake, etc.) from Kconfig files.
    - target.cmake - sets the build target and toolchain files.
    - utilities.cmake - other help commands.

 In addition to these files, there are two important CMake scripts in :armino:`/tools/build_tools/cmake`:

    - armino.cmake - sets build parameters and imports the core modules listed above.
    - project.cmake - imports ``armino.cmake`` and provides a custom ``project()`` command that handles all the heavy lifting of building the executable.
      Included in the top-level CMakeLists.txt of a standard ARMINO project

The other files in :armino:`/tools/build_tools/cmake` are support files or third-party scripts for the build process.

build process
-------------

The build process can be roughly divided into four stages:

.. blockdiag::
    :scale: 100%
    :caption: ARMINO Build System Process
    :align: center

    blockdiag armino-build-system-process {
        initialization -> Generate-component-list
        Generate-component-list -> component-handling
        component-handling -> Finish
    }

initialization
**************

 This phase sets the necessary parameters for the build.

    - After importing ``armino.cmake`` into ``project.cmake``, the following steps are performed:
        - Set ``ARMINO_PATH`` in an environment variable or infer a relative path from the ``project.cmake`` path contained in the top-level CMakeLists.txt.
        - Add :armino:`/tools/build_tools/cmake` to ``CMAKE_MODULE_PATH`` and import core modules and various helper/3rd party scripts.
        - Set build tools/executables like default Python interpreter.
        - Set global build parameters, i.e. compile options, compile definitions, include directories for all components.
        - Add :armino:`components` from components and :armino:`middleware` to the build.
    - The initial part of the custom ``project()`` command performs the following steps:
        - Set ``ARMINO_TARGET`` in an environment variable or CMake cache and  set ``CMAKE_TOOLCHAIN_FILE``to use accordingly.
        - Add components from ``EXTRA_COMPONENTS_DIRS`` to the build
        - Prepare parameters for calling command ``armino_build_process()`` from variables such as ``COMPONENTS``/``EXCLUDE_COMPONENTS``、``SDKCONFIG``、``SDKCONFIG_DEFAULTS`` etc. phase.

``armino_build_process()`` command marks the end of this.

Generate component list
***********************

  This phase builds a list of components that need to be processed during the build process, and is done in the first half of ``armino_build_process()``.

    - Find public and private dependencies for each component. Create a subprocess to **execute each component's CMakeLists.txt in script mode.**
      The values of the ``armino_component_register`` REQUIRES and PRIV_REQUIRES parameters are returned to the parent process.
      This is the component dependency extension (or early extension). **In this step define the variable ``CMAKE_BUILD_EARLY_EXPANSION``**.
    - Recursively import individual components based on public and private dependencies.

.. note:

    Each CMakeLists.txt in the component will be executed twice,the first time occurs in the _<generate component list> stage, the purpose is to expand the component dependencies with ``armino_component_register()``, At this point Kconfig is not loaded, so it is not possible to determine whether a component should be loaded or not by the CONFIG_XXX values in Kconfig. TODO. . .

component handling
******************

  This stage handles components in the build and is the second half of ``armino_build_process()``.

  - Load the project configuration from the sdkconfig file and generate the sdkconfig.cmake and sdkconfig.h header files. These two files define configuration variables/macros that can be accessed from build scripts and C/C++ source/header files, respectively.
  - **Import ``project_include.cmake``for each component.**
  - Add each component as a subdirectory, processing its CMakeLists.txt. The component CMakeLists.txt calls the registration command ``armino_component_register`` to add source files, import directories, create component libraries, link dependencies, etc.

Finish
^^^^^^^
  This stage is the remaining step of ``armino_build_process()``.

  - Create an executable and link it into the component library.
  - Generate project metadata files such as project_description.json and display relevant information such as the built project.

Please refer to :armino_file:`/tools/build_tools/cmake/project.cmake` for more information.