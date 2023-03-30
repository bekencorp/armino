#############
Corstone-1000
#############

************
Introduction
************

The ARM's Corstone-1000 platform is a reference implementation of PSA FF-M
architecture where NSPE and SPE environments are partitioned/isolated into
Cortex-A35 and Cortex-M0+ respectively.

Cortex-M0+ acting as Secure Enclave is the Root-of-trust of SoC. Its
software comprises of two boot loading stages, i.e. Bl1 and Bl2 (based on
mcuboot) and TF-M as run time software. Cortex-A35, also referred as host,
is treated as non-secure from the Secure Enclave perspective.
The Cortex-A35 is brought out of rest by Secure Enclave in aarch64 bit mode,
and boots the software ecosystem based on linux, u-boot, UEFI run time
services, TF-A, Secure Partitions and Optee.

The communication between NSPE and SPE is based on PSA IPC protocol running on
top of FF-A/OpenAMP.

.. toctree::
    :maxdepth: 1
    :glob:

    openamp/**

The secure enclave subsystem has ARM's CC-312 (Crypto Cell) hardware to
accelerate cryptographic operations. Additionaly, platform supports Secure Debug
using SDC-600 as the communication interface between host debugger and platform
target. The platform has the build option to enable secure debug protocol to
unlock debug ports during boot time. The protocol is based on ARM's ADAC
(Authenticated Debug Access Control) standard.


***********
System boot
***********

- The SoC reset brings Secure Enclave (SE), that is Cortex-M0+, out of rest.
- SE executes the BL1 ROM code based on mcuboot.
- BL1 load, verifies and transfer execution to BL2 which is again based on mcuboot.
- BL2 loads and verifies TF-M and host's initial boot loader image.
- BL2 transfer the execution to the TF-M.
- During TF-M initialization, the host is taken out of rest.
- Hashes of the keys used for image verification are stored in the OTP memory.

*****
Build
*****

Platform solution
=================

The platform binaries are build using Yocto. Below is the user guide:

`Arm Corstone-1000 User Guide`_

Secure Test
===========

This section can be used to test the secure enclave software indedendently from
the host. The below configuration builds the secure enclave binaries with CI test
frame integrated. On boot, secure enclave softwares stack is brought up, and
CI tests starts executing at the end of the initialization process. In the
below configuration, host software support is disabled, and meant only
to test/verify the secure enclave softwares.

FVP
---

- Download Corstone-1000 FVP from : `Arm Ecosystem FVPs`_
- Install FVP by running the shell script.
- Running of the binary will boot secure enclave software stack and at the end all CI test
  from tf-m-test along with platform specific tests are executed.

.. code-block:: bash

    cmake -B build/ -S <tf-m-root>/ -DCMAKE_BUILD_TYPE=Debug -DTFM_TOOLCHAIN_FILE=<tf-m-root>/toolchain_GNUARM.cmake -DTFM_PLATFORM=arm/corstone1000 -DPLATFORM_IS_FVP=TRUE -DTEST_NS=OFF -DTEST_S=ON -DEXTRA_S_TEST_SUITE_PATH=<tf-m-root>/trusted-firmware-m/platform/ext/target/arm/corstone1000/ci_regression_tests/
    cmake --build build -- install
    cd ./build/install/outputs/
    cat bl2_signed.bin bl2_signed.bin tfm_s_signed.bin > cs1000.bin
    cd <path-to-FVP-installation>/models/Linux64_GCC-9.3/
    ./FVP_Corstone-1000 -C board.flashloader0.fname="none" -C se.trustedBootROMloader.fname="./<path-to-build-dir>/install/outputs/bl1.bin" -C board.xnvm_size=64 -C se.trustedSRAM_config=6 -C se.BootROM_config="3" -C board.smsc_91c111.enabled=0  -C board.hostbridge.userNetworking=true --data board.flash0=./<path-to-build-dir>/install/outputs/cs1000.bin@0x68100000 -C diagnostics=4 -C disable_visualisation=true -C board.se_flash_size=8192 -C diagnostics=4  -C disable_visualisation=true

FPGA
----

- Follow the above pointed platform user guide to setup the FPGA board.
- Use the BL1 generated from the below commands to place it inside FPGA board SD Card.
- Use the cs1000.bin created from the below commands to place it inside FPGA board SD Card.

.. code-block:: bash

    cmake -B build/ -S <tf-m-root>/ -DCMAKE_BUILD_TYPE=Debug -DTFM_TOOLCHAIN_FILE=<tf-m-root>/toolchain_GNUARM.cmake -DTFM_PLATFORM=arm/corstone1000 -DTEST_NS=OFF -DTEST_S=ON -DEXTRA_S_TEST_SUITE_PATH=<tf-m-root>/trusted-firmware-m/platform/ext/target/arm/corstone1000/ci_regression_tests/ -DTEST_S_PS=OFF -DTEST_S_PLATFORM=OFF
    cmake --build build -- install
    cd ./build/install/outputs/
    cat bl2_signed.bin bl2_signed.bin tfm_s_signed.bin > cs1000.bin
    cp bl1.bin <path-to-FPGA-SD-CARD>/SOFTWARE/
    cp cs1000.bin <path-to-FPGA-SD-CARD>/SOFTWARE/

FPGA build can not compile all the CI tests into a single build as it exceeds
the available RAM size. So there is a need to select few tests but not all.
The above configuration disable build of -DTEST_S_PS and -DTEST_S_PLATFORM.
Other test configurations are:

- -DTEST_S_ATTESTATION=ON/OFF
- -DTEST_S_CRYPTO=ON/OFF
- -DTEST_S_ITS=ON/OFF
- -DTEST_S_PS=ON/OFF
- -DTEST_S_PLATFORM=ON/OFF

*Copyright (c) 2021-2022, Arm Limited. All rights reserved.*

.. _Arm Ecosystem FVPs: https://developer.arm.com/tools-and-software/open-source-software/arm-platforms-software/arm-ecosystem-fvps
.. _Arm Corstone-1000 User Guide: https://gitlab.arm.com/arm-reference-solutions/arm-reference-solutions-docs/-/blob/CORSTONE1000-2022.04.19/docs/embedded-a/corstone1000/user-guide.rst
