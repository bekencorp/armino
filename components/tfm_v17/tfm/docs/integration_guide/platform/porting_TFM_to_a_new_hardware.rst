##############################
Porting TF-M to a New Hardware
##############################

The purpose of this document is to provide a guide on how to integrate TF-M
with another hardware platform. This document will give general guidance on
how to port a platform on the TF-M build system and which interfaces must
exist on the platform for TF-M (S and NS) to run on this new platform.

******************
TF-M in a nutshell
******************
For an overview please refer to :doc:`Introduction </introduction/readme>`.

*************
Prerequisites
*************
Building environnement
======================
Make sure you have a working build environnement and that you can build
TF-M on AN521 following the
:doc:`Build instructions </building/tfm_build_instruction>`.

Toolchains and software requirements
====================================

Please follow the :doc:`Getting started guide </getting_started/tfm_getting_started>`.

CMSIS Drivers
=============
The TF-M stack requires at least two CMSIS HAL implementations:

    - `USART <https://www.keil.com/pack/doc/CMSIS/Driver/html/group__usart__interface__gr.html>`_
    - `FLASH <https://www.keil.com/pack/doc/CMSIS/Driver/html/group__flash__interface__gr.html>`_

************
Porting flow
************

In a nutshell, this should be a 6 iterative steps process:

    #. Adding all the mandatory files and expected objects/functions declarations

    #. Booting and configuring the core(s)

        - startup(s) code and SystemInit

    #. Adding the USART drivers

        - CMSIS HAL

    #. Adding the FLASH drivers

        - CMSIS HAL

    #. Enabling/Configuring/Disabling features including templated features

        - E.G. NV Counters, attestation, crypto keys....

    #. Running the regression tests

        - See :doc:`Running TF-M on Arm platforms </building/run_tfm_examples_on_arm_platforms>`
          as an example


*****************
File architecture
*****************
The platform selection when building TF-M is set via the CMake
variable TFM_PLATFORM. This variable holds part of the path to the platform.

When using ``-DTFM_PLATFORM=arm/mps2/an521`` or ``-DTFM_PLATFORM=an521``
TF-M build system will look for the platform in
<TF-M ROOT>/platform/ext/target/arm/mps2/an521.

Therefore all hardware dependent code for your platform should go to
<TF-M ROOT>/platform/ext/target/.

platform/ext/target
===================
This folder contains a first level of board vendor (such as ARM, STM, NXP,
Cypress ....), each folder will usually contain a second level for each
board. This second level is not mandatory.

    platform/ext/target/<vendor>/[<board name>/]

From now on this will be referred to as the ``platform folder``.

platform/ext/common
===================
This folder contains files and folder commons to the platforms, such as the
shims to the CMSIS drivers. It also contains the scatter files that can be
used for the bl2, tfm_s, tfm_ns partitions.

This folder also contains another folder named template. The latter contains
example implementations that are used for platforms by default, but which can be
altered or replaced if other functionality is required.

    +------------------------------+-----------------------------------------------------------------------------+
    |    name                      |        description                                                          |
    +==============================+=============================================================================+
    |PLATFORM_DEFAULT_ATTEST_HAL   |Use the default implementation of the attestation HAL (default True)         |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_NV_COUNTERS  |Use the default implementation of the counters in NV (default True)          |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_CRYPTO_KEYS  |Use the default implementation of crypto keys (default True)                 |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_ROTPK        |Use the default implementation of the RoT Public Key (default True)          |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_IAK          |Use the default implementation of the initial attestation key (default True) |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_UART_STDOUT  |Use the default implementation of the uart for stdout output (default True)  |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_NV_SEED      |Use the default implementation of the NV seed in the RNG (default True)      |
    +------------------------------+-----------------------------------------------------------------------------+
    |PLATFORM_DEFAULT_OTP          |Use the default implementation of the OTP (default True)                     |
    +------------------------------+-----------------------------------------------------------------------------+

***************
Platform Folder
***************

Description
===========

Depending on the level of integration you want with TF-M some files or
information will be mandatory for the build system to build working firmware.

Questions to be answered:
    - Will the platform use MCUboot as the second stage bootloader?

      BL2/MCUboot provides a secure bootloader that enables simple software
      upgrades.

      This optional second stage bootloader is set-up via the bl2 target in
      the CMakelists.txt file (see below).


    - Will the platform support the Non-Secure world?

      A platform can be designed to only support the secure world, in which
      case we would refer to it as a secure enclave. TF-M build system allows
      the developer to strip all Non-Secure world related code out of the
      final image. Most platforms, and especially the ones intended to be
      generic or to have a Non-Secure application will require Non-Secure world
      support.

      This optional Non-Secure world is set-up via the tfm_ns target in the
      CMakelists.txt file (see below).

    - How does the non-secure world communicate with the secure world?

      TF-M supports running the non-secure world on the same CPU as the secure
      world, communicating via TrustZone or running the non-secure world on
      a separate CPU, communicating via a mailbox.

      The architecture is configured in the config.cmake file (see below).

    - How does the FLASH need to be split between worlds?

      The flash split is very dependent on the support of BL2 and NS world.
      When porting a new platform, one shall arrange enough flash size for each
      of them.

      If supporting upgrades (via MCUboot), additional flash area will be
      required to store the updates before upgrading the whole system.

    - How does the RAM need to be split between worlds?

      The RAM split is very dependent on the support of the NS world.

      If you're not porting the platform for a specific project but are enabling
      the Non-Secure world, you should ensure that you leave enough RAM
      available for it to run.

.. Note::

   TF-M S world size in RAM and Flash varies greatly with different build
   options.

   TF-M project provides `metrics <https://qa-reports.linaro.org/tf/tf-m/metrics/?environment=DefaultProfileM&environment=DefaultProfileS&environment=DefaultProfileL&metric=:summary:>`_
   of the S world size for existing platforms, which may help to get a rough
   guide to the sizes needed.

Files
=====

CMakeLists.txt :
----------------

    (MANDATORY)

    This is the entry point for the build system to build your platform.

    it must:

        - Add a folder to the target platform_region_defs. [PLATFORM_REGION_DEFS_]

            This folder will contain two files flash_layout.h_ and region_defs.h_

        - Add scatter files to the bl2, tfm_s, and/or tfm_ns target. [SCATTER_]

            Please note that TF-M provides a common scatter file, for the bl2, tfm_s, tfm_ns targets, which can be used in most cases. [SCATTER_COMMON_]

        - Add startup files to the bl2, tfm_s, and/or tfm_ns target. [STARTUP_]
        - Add required sources and includes for the bl2, tfm_s and tfm_ns target (if supported) [SOURCES_INCLUDES_]
        - Link to the correct version of the CMSIS RTX libraries (from ``lib/ext/CMSIS_5/CMakeLists.txt``) [CMSIS_RTX_]

preload.cmake :
---------------

    (MANDATORY)

    This file tells the build system the main hardware information such as the
    main processor (e.g. m0plus) and architecture (e.g. armv6-m).

    This file should not contain any other CMake configuration variables.
    [preload_cmake_]

preload_ns.cmake:
-----------------

    If platform is a dual core then it may provide a preload_ns.cmake, which is
    the equivalent of preload.cmake but for the second core.

    If the platform is single core, this file should not be present.

    If the platform is dual core but both cores have the same architecture,
    this file is optional.

    [preload_cmake_]

config.cmake:
-------------

    (MANDATORY)

    This file is use to setup default build configuration for TF-M.

    It must specify the values below, and should also specify other
    configuration values that are fixed for the platform.

    +------------------------------+-------------------------------------------------------------------+
    |    name                      |        description                                                |
    +==============================+===================================================================+
    |CONFIG_TFM_USE_TRUSTZONE      | Use TrustZone to transition between NSPE and SPE on the same CPU  |
    +------------------------------+-------------------------------------------------------------------+
    |TFM_MULTI_CORE_TOPOLOGY       | NSPE runs on a separate CPU to SPE                                |
    +------------------------------+-------------------------------------------------------------------+

    [config_cmake_]

install.cmake:
--------------

    If there are platform-specific files that need to be installed, this file
    can be provided to do that.


startup files:
---------------

    (MANDATORY)

    These files (one for BL2, one for S, one for NS) are the expected startup
    files. The reset handler should call SystemInit and then should end up
    calling __START which should be defined as _start if not defined elsewhere.

.. _flash_layout.h:

flash_layout.h:
---------------

    (MANDATORY)

    This file can be anywhere in the platform folder, usually in a sub folder
    named ``partition``.
    TF-M doesn't provide a template for this file, common practice is to copy it
    from another platform (e.g. arm/mps2/an521) and update the following entries.

    Note: all size are in bytes

    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |    name                      |        description                                                |    Requisiteness                          |
    +==============================+===================================================================+===========================================+
    |FLASH_S_PARTITION_SIZE        | Size of the Secure partition in flash                             | Yes                                       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_NS_PARTITION_SIZE       | Size of the Non-Secure partition in flash                         | if tfm_ns is built                        |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_AREA_IMAGE_SECTOR_SIZE  | Size of the flash sector                                          | if bl2 is built                           |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_TOTAL_SIZE              | Flash total size                                                  | Yes                                       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_BASE_ADDRESS            | Flash base memory address                                         | if bl2 is built                           |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_AREA_BL2_OFFSET         | BL2 offset in flash                                               | if bl2 is built                           |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_AREA_BL2_SIZE           | BL2 flash size                                                    | if bl2 is built                           |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_PS_AREA_SIZE            | Allocated size for the protected storage data in flash            | Yes                                       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_ITS_AREA_SIZE           | Allocated size for the internal trusted storage data in flash     | Yes                                       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |SECURE_IMAGE_OFFSET           | Offset of the secure image data in flash                          | if bl2 is built                           |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |FLASH_DEV_NAME                | Name as defined in the CMSIS flash drivers                        | Yes                                       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_HAL_PS_FLASH_DRIVER       | Name as defined in the CMSIS flash drivers                        | used by protected storage partition       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_HAL_PS_SECTORS_PER_BLOCK  | Number of physical erase sectors per logical FS block             | used by protected storage partition       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_HAL_PS_PROGRAM_UNIT       | Smallest flash programmable unit in bytes                         | used by protected storage partition       |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_HAL_ITS_FLASH_DRIVER      | Name as defined in the CMSIS flash drivers                        | used by internal trusted storage partition|
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_HAL_ITS_SECTORS_PER_BLOCK | Number of physical erase sectors per logical ITS block            | used by internal trusted storage partition|
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_HAL_ITS_PROGRAM_UNIT      | Smallest flash programmable unit in bytes                         | used by internal trusted storage partition|
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+
    |TFM_NV_COUNTERS_AREA_SIZE     | Allocated size for the NV counters data in flash                  | if using TF-M templates                   |
    +------------------------------+-------------------------------------------------------------------+-------------------------------------------+

.. _region_defs.h:

region_defs.h:
--------------

    (MANDATORY)

    This file can be anywhere in the platform folder, usually in a sub folder
    named ``partition``.
    TF-M doesn't provide a template for this file, common practice is to copy it
    from another platform (e.g. arm/mps2/an521) and update the following entries.

    General advice: if you don't know beforehand the size you will want for
    these elements you will have to make it iterative from an abitrary value
    taken from another platform (e.g. arm/mps2/an521)

    Note: all size are in bytes

    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |    name                          |        description                                                | Requisiteness                                 |
    +==================================+===================================================================+===============================================+
    |BL2_HEAP_SIZE                     | Size of the Bootloader (MCUboot) heap                             | if bl2 is built                               |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |BL2_MSP_STACK_SIZE                | (if bl2 is built) Size of the Bootloader (MCUboot) Main stack     | if bl2 is built                               |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |S_HEAP_SIZE                       | Size of the Secure (S) world Heap                                 | yes                                           |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |S_MSP_STACK_SIZE                  | Size of the Secure (S) world Main stack                           | yes                                           |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |S_PSP_STACK_SIZE                  | Size of the Secure (S) world Process stack                        | no for IPC model                              |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |NS_HEAP_SIZE                      | Size of the Non-Secure (NS) world Heap                            | if tfm_ns is built                            |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |NS_STACK_SIZE                     | Size of the Non-Secure (NS) world stack                           | if tfm_ns is built                            |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE | Size of the buffer that will store the initial attestation        | used by initial attestation partition         |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |TFM_ATTEST_BOOT_RECORDS_MAX_SIZE  | Size of buffer that can store the encoded list of boot records    | used by delegated attestation partition       |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |BL2_HEADER_SIZE                   | Size of the Header for the Bootloader (MCUboot)                   | if bl2 is built                               |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |BL2_TRAILER_SIZE                  | Size of the Trailer for the Bootloader (MCUboot)                  | if bl2 is built                               |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+
    |SHARED_SYMBOL_AREA_SIZE           | Size of shared common code between bl2 and tfm_s                  | if bl2 is built and want to reduce image size |
    +----------------------------------+-------------------------------------------------------------------+-----------------------------------------------+

    (OPTIONAL)

    If the TF-M common linker script is used then:

    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+
    |    name                          |        description                                                    | Requisiteness                     |
    +==================================+=======================================================================+===================================+
    |S_CODE_START                      | Start address for the S code                                          | Yes                               |
    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+
    |S_CODE_SIZE                       | Size of the S code                                                    | Yes                               |
    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+
    |S_DATA_START                      | Start address for the S data                                          | Yes                               |
    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+
    |S_DATA_SIZE                       | Size of the S data                                                    | Yes                               |
    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+
    |S_CODE_SRAM_ALIAS_BASE            | Start address for the S code                                          | if no XIP on flash                |
    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+
    |TOTAL_CODE_SRAM_SIZE              | Size of the S code                                                    | if no XIP on flash                |
    +----------------------------------+-----------------------------------------------------------------------+-----------------------------------+

CMSIS_Driver/Config/cmsis_driver_config.h:
------------------------------------------

    (location as defined in CMakeLists.txt)

    This file should include the CMSIS drivers implementation headers.

CMSIS_Driver/Config/RTE_Device.h:
---------------------------------

    (location as defined in CMakeLists.txt)

    This is the Run-Time Environnement file from CMSIS, which is there to allow
    enabling or disabling drivers prior to building. If your platform is
    designed as a general use platform, this file should contain all the
    available CMSIS drivers, and you should provide a recommended configuration.
    If your platform is designed for a specific use-case then you should
    reference and enable only the mandatory drivers.

CMSIS_Driver/Driver_Flash.c:
----------------------------

    (location as defined in CMakeLists.txt)

    TF-M relies on CMSIS Drivers, as such it requires the CMSIS functions to
    be implemented. As a platform owner you can decide to either implement the
    drivers in the CMSIS functions or to use the CMSIS functions as a shim to
    your native drivers.

    Refer to the CMSIS `FLASH <https://www.keil.com/pack/doc/CMSIS/Driver/html/group__flash__interface__gr.html>`_
    documentation.

CMSIS_Driver/Driver_USART.c:
----------------------------

    (location as defined in CMakeLists.txt)

    TF-M relies on CMSIS Drivers, as such it requires the CMSIS functions to
    be implemented. As a platform owner you can decide to either implement the
    drivers in the CMSIS functions or to use the CMSIS functions as a shim to
    your native drivers.

    Refer to the CMSIS `USART <https://www.keil.com/pack/doc/CMSIS/Driver/html/group__usart__interface__gr.html>`_
    documentation.

target_cfg.[ch]:
----------------

    (location as defined in CMakeLists.txt)

    It is expected that these files contain all platform specific code related
    to memory protection (e.g. SAU/PPC/MPC). These functions will not be called
    by TF-M directly, but are expected to be called from the function
    tfm_hal_set_up_static_boundaries() in tfm_hal_isolation.c.

tfm_hal_platform.c:
-------------------

    (location as defined in CMakeLists.txt)

    Each platform is expected to implement the following API declared in
    platform/include/tfm_hal_platform.h

.. code-block:: c

    enum tfm_hal_status_t tfm_hal_platform_init(void);

    The function will be called before SPM initialization.

tfm_hal_isolation.c:
--------------------

    (location as defined in CMakeLists.txt)

    Each platform is expected to implement all the functions declared in
    platform/include/tfm_hal_isolation.h.

    These functions will be called from TF-M.

tfm_platform_system.c:
----------------------

    (location as defined in CMakeLists.txt)

    Each platform is expected to implement all the functions declared in
    platform/include/tfm_platform_system.h.

check_config.cmake:
-------------------

    As a platform owner you may want to enforce some configuration or to prevent
    the use of unsupported configurations.

    This file (CMake format) allows you to do so by allowing you to check for
    invalid configuration values.

    This file is optional.

    TF-M build system already provides a generic configuration checker that will
    be called on top of one provided by the platform owner. The generic checker
    is located in <TF-M ROOT>/config/.

    [check_config.cmake_]

.. _Functions:

Functions
=========

    There are a few functions that need to be declared and properly
    initialized for TF-M to work. The function declarations can be found in
    platform/include/tfm_platform_system.h and platform/include/tfm_spm_hal.h.

tfm_platform_hal_system_reset:
------------------------------

    This function will in most cases end up calling the NVIC System Reset.

    The platform can uninitialize or store some resources before reset.

.. code-block:: c

    void tfm_platform_hal_system_reset(void);


tfm_platform_hal_ioctl:
-----------------------

    A single entry point to platform-specific code across the HAL is provided by the
    IOCTL service.

.. code-block:: c

    enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request, psa_invec  *in_vec, psa_outvec *out_vec);

tfm_spm_hal_get_mem_security_attr:
----------------------------------

    This function shall fill the security_attr_info_t argument with the current
    active security configuration.

.. code-block:: c

    void tfm_spm_hal_get_mem_security_attr(const void *p, size_t s, struct security_attr_info_t *p_attr);

tfm_spm_hal_get_secure_access_attr:
-----------------------------------

    This function shall fill the mem_attr_info_t argument with the current
    active secure memory configuration.

.. code-block:: c

    void tfm_spm_hal_get_secure_access_attr(const void *p, size_t s, struct mem_attr_info_t *p_attr);

tfm_spm_hal_get_ns_access_attr:
-------------------------------

    This function shall fill the mem_attr_info_t argument with the current
    active secure memory configuration for the NS side.

.. code-block:: c

    void tfm_spm_hal_get_ns_access_attr(const void *p, size_t s, struct mem_attr_info_t *p_attr);

tfm_hal_irq_clear_pending:
--------------------------

    This function clears any pending IRQ.

.. code-block:: c

    void tfm_hal_irq_clear_pending(uint32_t irq_num);

tfm_hal_irq_enable:
-------------------

    This function enable an IRQ.

.. code-block:: c

    void tfm_hal_irq_enable(uint32_t irq_num);

tfm_hal_irq_disable:
--------------------

    This function disable an IRQ.

.. code-block:: c

    void tfm_hal_irq_disable(uint32_t irq_num);

Annex
=====

.. _PLATFORM_REGION_DEFS:

::

    [PLATFORM_REGION_DEFS]
    target_include_directories(platform_region_defs
        INTERFACE
        <folder name under the platform folder - usually named platform>
    )

------------

.. _SCATTER:

::

    [SCATTER]
    target_add_scatter_file(bl2
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_bl2.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_bl2.ld>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_bl2.icf>
    )
    target_add_scatter_file(tfm_s
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_s.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_s.icf>
    )
    target_add_scatter_file(tfm_ns
        $<$<C_COMPILER_ID:ARMClang>:${CMAKE_SOURCE_DIR}/platform/ext/common/armclang/tfm_common_ns.sct>
        $<$<C_COMPILER_ID:GNU>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_ns.ld>
        $<$<C_COMPILER_ID:IAR>:${CMAKE_SOURCE_DIR}/platform/ext/common/iar/tfm_common_ns.icf>
    )

------------

.. _SCATTER_COMMON:

::

   <TF-M ROOT>/platform/ext/common/<TOOLCHAIN>/

------------

.. _STARTUP:

::

    [STARTUP]
    target_sources(bl2
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/<folder to platform>/device/source/startup_<platform name>.c
    )
    target_sources(tfm_s
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/<folder to platform>/device/source/startup_<platform name>.c
    )
    target_sources(tfm_ns
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/<folder to platform>/device/source/startup_<platform name>.c
    )

------------

.. _SOURCES_INCLUDES:

::

    [SOURCES_INCLUDES]
    target_include_directories(platform_bl2
        PUBLIC
    )
    target_include_directories(platform_s
        PUBLIC
    )
    target_include_directories(platform_ns
        PUBLIC
    )

    target_sources(platform_bl2
        PRIVATE
    )
    target_sources(platform_s
        PRIVATE
    )
    target_sources(platform_ns
        PRIVATE
    )
    target_sources(tfm_spm
        PRIVATE
            target_cfg.c
            tfm_hal_isolation.c
            tfm_hal_platform.c
    )

------------

.. _CMSIS_RTX:

::

    [CMSIS_RTX]
    target_link_libraries(CMSIS_5_tfm_ns
        INTERFACE
            <CMSIS_5_RTX_CM0 | CMSIS_5_RTX_CM3 | CMSIS_5_RTX_V8MBN | CMSIS_5_RTX_V8MMN>
    )

------------

.. _preload_cmake:

::

    [preload_cmake]
    set(TFM_SYSTEM_PROCESSOR <value>)    # The format is that same as the format used in the -mcpu= argument of GNUARM or ARMCLANG. The special +modifier syntax must not be used.
    set(TFM_SYSTEM_ARCHITECTURE <value>) # The format is that same as the format used in the -march= argument of GNUARM or ARMCLANG. The special +modifier syntax must not be used.
    set(TFM_SYSTEM_DSP <value>)
    set(CRYPTO_HW_ACCELERATOR_TYPE <value>)

------------

.. _config_cmake:

::

    [config_cmake]
    set(CONFIG_TFM_USE_TRUSTZONE            ON          CACHE BOOL      "Enable use of TrustZone to transition between NSPE and SPE")
    set(TFM_MULTI_CORE_TOPOLOGY             OFF         CACHE BOOL      "Whether to build for a dual-cpu architecture")
    set(BL2                                 OFF         CACHE BOOL      "Whether to build BL2")
    set(NS                                  FALSE       CACHE BOOL      "Whether to build NS app" FORCE)

------------

.. _check_config.cmake:

::

    function(tfm_invalid_config)
        if (${ARGV})
            string (REPLACE ";" " " ARGV_STRING "${ARGV}")
            string (REPLACE "STREQUAL"     "=" ARGV_STRING "${ARGV_STRING}")
            string (REPLACE "GREATER"      ">" ARGV_STRING "${ARGV_STRING}")
            string (REPLACE "LESS"         "<" ARGV_STRING "${ARGV_STRING}")
            string (REPLACE "VERSION_LESS" "<" ARGV_STRING "${ARGV_STRING}")
            string (REPLACE "EQUAL"        "=" ARGV_STRING "${ARGV_STRING}")
            string (REPLACE "IN_LIST"      "in" ARGV_STRING "${ARGV_STRING}")

            message(FATAL_ERROR "INVALID CONFIG: ${ARGV_STRING}")
        endif()
    endfunction()

    # Requires armclang >= 6.10.1
    tfm_invalid_config((CMAKE_C_COMPILER_ID STREQUAL "ARMClang") AND (CMAKE_C_COMPILER_VERSION VERSION_LESS "6.10.1"))

*Copyright (c) 2021-2022, Arm Limited. All rights reserved.*
*Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
or an affiliate of Cypress Semiconductor Corporation. All rights reserved.*
