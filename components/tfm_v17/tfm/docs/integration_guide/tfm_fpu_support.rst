######################
Floating-Point Support
######################

TF-M adds several configuration flags to control Floating point (FP) [1]_
support in TF-M Secure Processing Environment (SPE) and Non Secure Processing
Environment (NSPE).

* Support FP in SPE or NSPE.
* Support FP Application Binary Interface (ABI) [2]_ types: software, hardware.
  SPE and NSPE shall use the same FP ABI type.
* Support lazy stacking enable/disable in SPE only, NSPE is not allowed to
  enable/disable this feature.
* Support GNU Arm Embedded Toolchain [3]_. ``GNU Arm Embedded Toolchain 10.3-
  2021.10`` and later version shall be used to mitigate VLLDM instruction
  security vulnerability [4]_.
* Support Inter-Process Communication (IPC) [5]_ model in TF-M, and doesn't
  support SFN model.
* Support Armv8-M mainline.
* Support isolation level 1,2,3.
* Does not support use FPU in First-Level Interrupt Handling (FLIH) [6]_ at
  current stage.

Please refer to Arm musca S1 [7]_ platform as a reference implementation when
you enable FP support on your platforms.

.. Note::
    Alternatively, if you intend to use FP in your own NSPE application but the
    TF-M SPE services that you enable do not require FP, you can set the CMake
    configuration ``CONFIG_TFM_ENABLE_CP10CP11`` to ``ON`` and **ignore** any
    configurations described below.

.. Note::
    ``GNU Arm Embedded Toolchain 10.3-2021.10`` may have issue that reports
    ``'-mcpu=cortex-m55' conflicts with '-march=armv8.1-m.main'`` warning [8]_.
    This issue has been fixed in the later version.

============================
FP ABI type for SPE and NSPE
============================
FP design in Armv8.0-M [9]_ architecture requires consistent FP ABI types
between SPE and NSPE. Furthermore, both sides shall set up CPACR individually
when FPU is used. Otherwise, No Coprocessor (NOCP) usage fault will be asserted
during FP context switch between security states.

Secure and non-secure libraries are compiled with ``COMPILER_CP_FLAG`` and
linked with ``LINKER_CP_OPTION`` for different FP ABI types. All those
libraries shall be built with ``COMPLIER_CP_FLAG``.

If FP ABI types mismatch error is generated during build, pleae check whether
the library is compiled with ``COMPILER_CP_FLAG``.
Example:

.. code-block:: cmake

      target_compile_options(lib
          PRIVATE
              ${COMPILER_CP_FLAG}
      )

===================================
CMake configurations for FP support
===================================
The following CMake configurations configure ``COMPILER_CP_FLAG`` in TF-M SPE.

* ``CONFIG_TFM_ENABLE_FP`` is used to enable/disable FPU usage.

  +--------------------------+---------------------------+
  | CONFIG_TFM_ENABLE_FP     | FP support                |
  +==========================+===========================+
  | off (default)            | FP diasabled              |
  +--------------------------+---------------------------+
  | on                       | FP enabled                |
  +--------------------------+---------------------------+

.. Note::
    ``CONFIG_TFM_FLOAT_ABI`` depends on ``CONFIG_TFM_ENABLE_FP``. If
    ``CONFIG_TFM_ENABLE_FP is set ``CONFIG_TFM_FLOAT_ABI`` is automatically
    set to ``hard``.

.. Note::
    If you build TF-M SPE with ``CONFIG_TFM_ENABLE_FP=on`` and provide your own
    NSPE application, your own NSPE **must** take care of enabling floating point
    coprocessors CP10 and CP11 on the NS side to avoid aforementioned NOCP usage
    fault.

* ``CONFIG_TFM_LAZY_STACKING`` is used to enable/disable lazy stacking
  feature. This feature is only valid for FP hardware ABI type.
  NSPE is not allowed to enable/disable this feature. Let SPE decide the
  secure/non-secure shared setting of lazy stacking to avoid the possible
  side-path brought by flexibility.

  +------------------------------+---------------------------+
  | CONFIG_TFM_LAZY_STACKING     | Description               |
  +==============================+===========================+
  | 0FF                          | Disable lazy stacking     |
  +------------------------------+---------------------------+
  | ON (default)                 | Enable lazy stacking      |
  +------------------------------+---------------------------+

* ``CONFIG_TFM_FP_ARCH`` specifies which FP architecture is available on the
  target, valid for FP hardware ABI type.

  FP architecture is processor dependent. For GNUARM compiler, example value
  are: auto, fpv5-d16, fpv5-sp-d16, etc.

  Default value of ``CONFIG_TFM_FP_ARCH`` for GNUARM compiler is fpv5-sp-d16.

  This parameter shall be specified by platform. Please check compiler
  reference manual and processor hardware manual for more details to set
  correct FPU configuration for platform.


*********
Reference
*********
.. [1] `High-Performance Hardware Support for Floating-Point Operations <https://www.arm.com/why-arm/technologies/floating-point>`_

.. [2] `Float Point ABI <https://www.keil.com/support/man/docs/armclang_ref/armclang_ref_chr1417451577871.htm>`_

.. [3] `GNU Arm Embedded Toolchain <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm>`_

.. [4] `VLLDM instruction Security Vulnerability <https://developer.arm.com/support/arm-security-updates/vlldm-instruction-security-vulnerability>`_

.. [5] `Arm® Platform Security Architecture Firmware Framework 1.0 <https://armkeil.blob.core.windows.net/developer/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf>`_

.. [6] :doc:`Secure Interrupt Integration Guide </integration_guide/tfm_secure_irq_integration_guide>`

.. [7] `Musca-S1 Test Chip Board <https://developer.arm.com/tools-and-software/development-boards/iot-test-chips-and-boards/musca-s1-test-chip-board>`_

.. [8] `GCC Issue on '-mcpu=cortex-m55' conflicts with '-march=armv8.1-m.main' Warning <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=97327>`_

.. [9] `Armv8-M Architecture Reference Manual <https://developer.arm.com/documentation/ddi0553/latest>`_

--------------

*Copyright (c) 2021-2022, Arm Limited. All rights reserved.*
