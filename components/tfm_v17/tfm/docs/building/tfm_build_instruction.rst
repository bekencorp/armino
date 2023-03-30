##################
Build instructions
##################

Please make sure you have all required software installed as explained in the
:doc:`TF-M getting started </getting_started/tfm_getting_started>`.
The additional building materials you can find in the following links:

.. toctree::
    :maxdepth: 1

    Run TF-M examples <run_tfm_examples_on_arm_platforms>
    Building the documentation <documentation_generation>
    IAR toolchain <tfm_build_instruction_iar>

****************
TF-M build steps
****************
TF-M uses `cmake <https://cmake.org/overview/>`__ to provide an out-of-source
build environment. The instructions are below.

Cmake version ``3.15.0`` or higher is required.

.. _Getting the source-code:

Getting the source-code
=======================
.. code-block:: bash

    cd <base folder>
    git clone https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git

To simplify documentation commands, the new created repository under
``trusted-firmware-m`` would be referenced as ``<TF-M base folder>`` and
its parent, the ``<base folder>``. Dependency management is now handled by
cmake. If you wish to alter this behaviour, see
:ref:`building/tfm_build_instruction:Manual dependency management`

.. Note::

 - For building with Armclang compiler version 6.10.0+, please follow the note
   in :doc:`TF-M getting started </getting_started/tfm_getting_started>`.
 - For building with the IAR toolchain, please see the notes in
   :doc:`IAR software requirements <tfm_build_instruction_iar>`

Basic configuration
===================

TF-M has many options for configuration and fine-tuning. Please check the
:ref:`tf-m_configuration` section for details. The **base** configuration
will be build by default with only essential modules SPM and platform hence
a platform must be selected via ``TFM_PLATFORM`` options, it can be:

 - A relative path under ``<TF-M_root>/platform/ext/target``,
   for example ``arm/mps2/an521``.
 - An absolute path of target platform, mainly used for out-of-tree platform
   build.
 - A target platform name that is supported under
   <TF-M_root>/platform/ext/target, for example ``an521``.

Build type
----------

By default, a MinSizeRel configuration is built. Alternate build types can be
specified with the ``CMAKE_BUILD_TYPE`` variable. The possible
types are:

 - ``Debug``
 - ``RelWithDebInfo``
 - ``Release``
 - ``MinSizeRel`` - default

Debug symbols are added by default to all builds, but can be removed
from ``Release`` and ``MinSizeRel`` builds by setting
``TFM_DEBUG_SYMBOLS`` to ``OFF``.

``RelWithDebInfo``, ``Release`` and ``MinSizeRel`` all have different
optimizations turned on and hence will produce smaller, faster code
than ``Debug``. ``MinSizeRel`` will produce the smallest code, and
hence is often a good idea on RAM or flash constrained systems.

Other cmake parameters
----------------------

The full list of default options is in ``config/config_base.cmake`` and
explained in :ref:`tfm_cmake_configuration`. Several important options
are listed below.

+---------------------+----------------------------------------+---------------+
| Parameter           | Description                            | Default value |
+=====================+========================================+===============+
| BL2                 | Build level 2 secure bootloader.       | ON            |
+---------------------+----------------------------------------+---------------+
| NS                  | Build NS app. Required for test code.  | ON            |
+---------------------+----------------------------------------+---------------+
| TFM_ISOLATION_LEVEL | Set TFM isolation level.               | 1             |
+---------------------+----------------------------------------+---------------+
| TFM_PROFILE         | Set TFM profile.                       |               |
+---------------------+----------------------------------------+---------------+
| TEST_S              | Build secure regression tests.         | OFF           |
+---------------------+----------------------------------------+---------------+
| TEST_NS             | Build non-secure regression tests.     | OFF           |
+---------------------+----------------------------------------+---------------+
| TEST_PSA_API        | Build PSA API TESTS for the given      |               |
|                     | suite. Takes a PSA api ``SUITE`` as an |               |
|                     | argument (``CRYPTO`` etc).             |               |
+---------------------+----------------------------------------+---------------+

TF-M Profiles
-------------

TF-M Profiles are implemented as a single cmake configuration file, under the
``config/profile`` directory. A good understanding can be gained quickly by
looking at the Profile configuration files, but the ultimate reference for
Profiles is in :ref:`tf-m_profiles`.

*******************
TF-M build examples
*******************

Example: building TF-M for AN521 platform using GCC:
====================================================
.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521
    make install

.. Note::

    It is recommended to build each different build configuration in a separate
    build directory.

The default build uses Unix Makefiles. The ``-G`` option can be used to change
this. The default build uses the GNU ARM toolchain and creates a Release build.
These options can be overridden using the ``TFM_TOOLCHAIN_FILE`` and
``CMAKE_BUILD_TYPE`` parameters, as shown below

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -GNinja -DTFM_TOOLCHAIN_FILE=toolchain_ARMCLANG.cmake -DCMAKE_BUILD_TYPE=Debug
    cmake --build cmake_build -- install

Regression Tests for the AN521 target platform
==============================================

Regression tests can be build by using the TEST_S and TEST_NS settings. Either
can be used in isolation or both can be used to enable both suites. All tests
for all enabled partitions are run, along with IPC and Multicore tests if those
features are enabled.

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTEST_S=ON -DTEST_NS=ON
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTEST_S=ON -DTEST_NS=ON
    make install

Build for PSA API tests
=======================
The build system provides support for building and integrating the PSA API tests
from https://github.com/ARM-software/psa-arch-tests. PSA API tests are
controlled using the TEST_PSA_API variable. Enabling both regression tests and
PSA API tests simultaneously is **not** supported.

The value of the TEST_PSA_API variable is the suite to be run.

.. code-block:: bash

    -DTEST_PSA_API=INTERNAL_TRUSTED_STORAGE
    -DTEST_PSA_API=PROTECTED_STORAGE
    -DTEST_PSA_API=STORAGE
    -DTEST_PSA_API=CRYPTO
    -DTEST_PSA_API=INITIAL_ATTESTATION

Respectively for the corresponding service. For example, to enable the PSA API
tests for the Crypto service:

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTEST_PSA_API=CRYPTO
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTEST_PSA_API=CRYPTO
    make install

Location of build artifacts
===========================

All build artifacts are provided in the ``<build_dir>/bin`` directory. It is
**not** required to run ``make install`` to generate artifacts in this location.


For the purposes of maintaining compatibility with the legacy cmake build
system, they are also provided in
``<build_dir>/install/outputs/<target_platform>/``. In order to generate the
artifacts in this location ``make install`` must be run.

****************************
Manual dependency management
****************************

The TF-M build system will by default fetch all dependencies with appropriate
versions and store them inside the build tree. In this case, the build tree
location is ``<build_dir>/lib/ext``, and the extra libraries can be cleaned by
deleting that directory.

If you have local copies already, and wish to avoid having the libraries
downloaded every time the build directory is deleted, then the following
variables can be set to the path to the root directory of the local repo. This
will disable the automatic downloading for that dependency.

+----------------+---------------------+-----------------------------------------------------+
| Dependency     | Cmake variable      | Git repo URL                                        |
+================+=====================+=====================================================+
| Mbed Crypto    | MBEDCRYPTO_PATH     | https://github.com/ARMmbed/mbedtls                  |
+----------------+---------------------+-----------------------------------------------------+
| tf-m-tests     | TFM_TEST_REPO_PATH  | https://git.trustedfirmware.org/TF-M/tf-m-tests.git |
+----------------+---------------------+-----------------------------------------------------+
| MCUboot        | MCUBOOT_PATH        | https://github.com/mcu-tools/mcuboot                |
+----------------+---------------------+-----------------------------------------------------+
| psa-arch-tests | PSA_ARCH_TESTS_PATH | https://github.com/ARM-software/psa-arch-tests      |
+----------------+---------------------+-----------------------------------------------------+

For required versions of the dependencies, refer to ``config/config_base.cmake``.

.. Note::
 - Some patches are required to the mbedtls repo to allow building it as part of
   TF-M. While these patches are being upstreamed they are stored in
   ``lib/ext/mbedcrypo``. In order to use a local copy of Mbed Crypto it is
   required to apply all patch files in this directory.

.. Note::
 - CMSIS 5 is provided by the TF-M tests repo. If you wish to use a different
   source for CMSIS 5, it can be configured using CMSIS_5_PATH.

TF-M Tests
==========

Dependency auto downloading is used by default.
The TF-M build system downloads the tf-m-tests repo with a fixed version
specified by ``TFM_TEST_REPO_VERSION`` in
:file:`lib/ext/tf-m-tests/repo_config_base.cmake`.
The version can be a release tag or a commit hash.

Developers who want a different version of tf-m-tests can override
``TFM_TEST_REPO_PATH`` to a local copy with the desired version.

As the test repo is part of the TF-M project and coupled with TF-M repo,
the version should be updated when there are dependency changes between the TF-M
repo and the test repo and when there is a complete change merged in test repo.

A complete change is one or more patches that are for the same purpose, for
example a new test suite or enhancements on the test cases.
Patches in one change can be merge individually provided they do not break
anything or cause any regressions.
But the version in the TF-M gets updated only when all the patches are merged.

Example: building TF-M for AN521 platform with local Mbed Crypto
================================================================

Prepare Mbed Crypto repository
------------------------------

This is only required to be done once. For dependencies that do not have any
``.patch`` files in their ``lib/ext`` directory the only required step is
cloning the repo and checking out the correct branch.

.. code-block:: bash

    cd <Mbed Crypto base folder>
    git clone https://github.com/ARMmbed/mbedtls
    cd mbedtls
    git checkout <MBEDCRYPTO_VERSION from config_base.cmake>
    git apply <TF-M base folder>/trusted-firmware-m/lib/ext/mbedcrypo/*.patch

.. Note::
 - <Mbed Crypto base folder> does not need to have any fixed posisition related
   to the TF-M repo.

Build TF-M
----------

With new cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DMBEDCRYPTO_PATH=<Mbed Crypto base folder>/mbedtls
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DMBEDCRYPTO_PATH=<Mbed Crypto base folder>/mbedtls
    make install

--------------

*Copyright (c) 2017-2022, Arm Limited. All rights reserved.*
*Copyright (c) 2022, Cypress Semiconductor Corporation. All rights reserved.*
