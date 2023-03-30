###########################################
Trusted Firmware-M Profile Medium-ARoT-less
###########################################

************
Introduction
************

TF-M Profile Medium-ARoT-less is a reference implementation to align with security requirements
defined in PSA Certified ARoT-less Level 2 protection profile (PSA Certified ARoT-less) [1]_.

TF-M Profile Medium-ARoT-less is defined based on TF-M Profile Medium [2]_, which aligns with PSA
Certified Level 2 Protection Profile [3]_.

**************
Overall design
**************

TF-M Profile Medium-ARoT-less defines the following feature set:

    - Firmware Framework

        - Secure Function (SFN) model [4]_
        - Isolation level 1 [5]_

    - Internal Trusted Storage (ITS)

    - Crypto

        - Support both symmetric cryptography and asymmetric cryptography
        - Asymmetric key based cipher suite suggested in TLS/DTLS profiles for
          IoT [6]_ and CoAP [7]_, including

            - Authenticated Encryption with Associated Data (AEAD) algorithm
            - Asymmetric key algorithm based signature and verification
            - Public-key cryptography based key exchange
            - Hash function
            - HMAC for default Pseudorandom Function (PRF)

        - Asymmetric digital signature and verification for Initial Attestation
          Token (IAT)

    - Initial Attestation

        - Asymmetric key algorithm based Initial Attestation

    - Lightweight boot

        - Anti-rollback protection
        - Multiple image boot

    - Firmware Update (FWU) RoT Service

**************
Design details
**************

Most of design in Profile Medium-ARoT-less is identical to that in Profile Medium. Refer to
Profile Medium document [2]_ for details.
Only the differences between Profile Medium-ARoT-less and Profile Medium are specified below.

Firmware framework
==================

PSA Certified ARoT-less [1]_ is only applicable to devices that don’t support Application RoT (ARoT)
services. 
The platform only consists of PSA RoT domain(s) in SPE making it unnecessary to implement the
isolation between ARoT and PSA RoT.
Therefore, this profile selects isolation level 1 to simplify implementation and optimize memory
footprint and performance.

Since only isolation level 1 is required, this profile enables SFN model rather than IPC mode for
further simplification.

Protected Storage
=================

Protected Storage (PS) is implemented as an Application RoT service in TF-M by default.
Therefore, PS is disabled by default in this profile.

FWU RoT Service
===============

As PSA Certified ARoT-less requests, FWU RoT Service is enabled by default to support secure
update of platform if the platform supports FWU.

.. note ::

    **Implementation note**

    The entire secure update sequence involves multiple agents and components, including bootloader,
    TF-M FWU RoT Service, image update application(s), remote server(s), etc.

    The secure update sequence is implementation-defined. Here is a reference of TF-M integration
    with FreeRTOS OTA [8]_.

**************
Implementation
**************

The basic idea is to add dedicated profile CMake configuration files
``config/profile/profile_medium_arotless`` for this profile default configuration.

This top-level config file collects all the fundamental configuration flags and set them to default
values, to explicitly configured the features required in this profile, during TF-M build.

A platform/use case overwrite the default settings to configure this profile.

The behavior of the build flow (particularly the order of configuration loading and overriding) can
be found at :ref:`tfm_cmake_configuration`.

The default configurations in ``config/profile/profile_medium_arotless`` are shown below.

.. table:: Config flags in Profile Medium-ARoT-less top-level CMake config file
   :widths: auto
   :align: center

   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | Configs                                    | Default value                                                                                       | Descriptions                        |
   +============================================+=====================================================================================================+=====================================+
   | ``TFM_ISOLATION_LEVEL``                    | ``1``                                                                                               | Select level 1 isolation            |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``CONFIG_TFM_SPM_BACKEND``                 | ``SFN``                                                                                             | Select SFN model                    |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_INTERNAL_TRUSTED_STORAGE`` | ``ON``                                                                                              | Enable ITS SP                       |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``ITS_BUF_SIZE``                           | ``32``                                                                                              | ITS internal transient buffer size  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_CRYPTO``                   | ``ON``                                                                                              | Enable Crypto service               |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED``     | ``OFF``                                                                                             | Enable Crypto asymmetric            |
   |                                            |                                                                                                     | encryption operations               |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_MBEDCRYPTO_CONFIG_PATH``             | ``${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_medium.h`` | Re-use Profile Medium configuration |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH``  | ``${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/crypto_config_profile_medium.h``         | Re-use Profile Medium configuration |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_INITIAL_ATTESTATION``      | ``ON``                                                                                              | Enable Initial Attestation service  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_FIRMWARE_UPDATE``          | ``ON``                                                                                              | Enable Firmware Update service      |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_PROTECTED_STORAGE``        | ``OFF``                                                                                             | Disable PS service                  |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_PLATFORM``                 | ``ON``                                                                                              | Enable TF-M Platform SP             |
   +--------------------------------------------+-----------------------------------------------------------------------------------------------------+-------------------------------------+

.. note::

   Where a configuration is the same as the default in
   ``config/config_default.cmake``, it might be omitted from the profile configuration
   file.

.. note::

    **Implementation note**

    If the platform doesn't support secure update functionalities, FWU service will be disabled
    automatically. A warning will be thrown out during build.

Regression test configuration
=============================

FWU regression tests and SFN regression tests are selected by default when regression tests are
enabled.
Other implementations are the same as those in Profile Medium.

****************
Platform support
****************

To enable Profile Medium on a platform, the platform specific CMake file should
be added into the platform support list in top-level Profile Medium CMake config
file.

Building Profile Medium
=======================

To build Profile Medium, argument ``TFM_PROFILE`` in build command line should be
set to ``profile_medium_arotless``.

Take Musca-B1 as an example.
The following commands build Profile Medium without test cases on **Musca-B1** with
build type **MinSizeRel**, built by **Armclang**.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=musca-b1 \
         -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
         -DTFM_PROFILE=profile_medium_arotless \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         ../
   cmake --build ./ -- install

The following commands build Profile Medium-ARoT-less with regression test cases on
**Musca-B1** with build type **MinSizeRel**, built by **Armclang**.

.. code-block:: bash

   cd <TFM root dir>
   mkdir build && cd build
   cmake -DTFM_PLATFORM=musca-b1 \
         -DTFM_TOOLCHAIN_FILE=../toolchain_ARMCLANG.cmake \
         -DTFM_PROFILE=profile_medium_arotless \
         -DCMAKE_BUILD_TYPE=MinSizeRel \
         -DTEST_S=ON -DTEST_NS=ON \
         ../
   cmake --build ./ -- install

.. note::

 - For devices with more constrained memory and flash requirements, it is
   possible to build with either only TEST_S enabled or only TEST_NS enabled.
   This will decrease the size of the test images. Note that both test suites
   must still be run to ensure correct operation.

More details of building instructions and parameters can be found TF-M build instruction guide [9]_.

*********
Reference
*********

.. [1] `SESIP Profile for PSA Certified ARoT-less Level 2 <https://www.psacertified.org/app/uploads/2022/10/JSADEN019-PSA_Certified_Level_2_PP_SESIP_ARoT-less_REL-01.pdf>`_

.. [2] :doc:`Trusted Firmware-M Profile Medium Design </configuration/profiles/tfm_profile_medium>`

.. [3] `SESIP Profile for PSA Certified Level 2 <https://www.psacertified.org/app/uploads/2021/03/JSADEN012-PSA_Certified_Level_2_PP_SESIP-BETA02.pdf>`_

.. [4] `Arm Firmware Framework for M 1.1 Extensions <https://developer.arm.com/documentation/aes0039/latest>`_

.. [5] `Arm Platform Security Architecture Firmware Framework 1.0 <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4>`_

.. [6] `Transport Layer Security (TLS) / Datagram Transport Layer Security (DTLS) Profiles for the Internet of Things <https://tools.ietf.org/html/4>`_

.. [7] `The Constrained Application Protocol (CoAP) <https://tools.ietf.org/html/5>`_

.. [8] `Secure OTA Updates for Cortex-M Devices with FreeRTOS <https://www.freertos.org/2021/07/secure-ota-updates-for-cortex-m-devices-with-freertos.html>`_

.. [9] :doc:`TF-M build instruction </building/tfm_build_instruction>`

--------------

*Copyright (c) 2020-2022, Arm Limited. All rights reserved.*
