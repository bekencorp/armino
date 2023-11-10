Runtime Security Subsystem (RSS)
================================

Introduction
------------

Runtime Security Subsystem (RSS) is an Arm subsystem that provides a reference
implementation of the HES Host in the
`Arm Confidential Compute Architecture (CCA) <https://www.arm.com/architecture/security-features/arm-confidential-compute-architecture>`_.
It is designed to be integrated into A-profile compute subsystems that implement
Arm CCA, where it serves as the Root of Trust.

RSS initially boots from immutable code (BL1_1) in its internal ROM, before
jumping to BL1_2, which is provisioned and hash-locked in RSS OTP. The updatable
MCUBoot BL2 boot stage is loaded from host system flash into RSS SRAM, where it
is authenticated. BL2 loads and authenticates the TF-M runtime into RSS SRAM
from host flash. BL2 is also responsible for loading initial boot code into
other subsystems within the host.

The RSS platform port supports the TF-M Crypto, TF-M Initial Attestation,
Measured Boot and TF-M Platform services along with the corresponding
regression tests. It supports the IPC model in multi-core topology with
Isolation Level 1 and 2.

Building TF-M
-------------

Follow the instructions in :doc:`Build instructions </building/tfm_build_instruction>`.
Build TF-M with platform name: `arm/rss/<rss platform name>`

For example for building RSS for Total Compute platforms:
``-DTFM_PLATFORM=arm/rss/tc``

Signing host images
-------------------

RSS BL2 can load boot images into other subsystems within the host system. It
expects images to be signed, with the signatures attached to the images in the
MCUBoot metadata format.

The `imgtool Python package <https://pypi.org/project/imgtool/>`_ can be used to
sign images in the required format. To sign a host image using the development
key distributed with TF-M, use the following command::

    imgtool sign \
        -k <TF-M base directory>/bl2/ext/mcuboot/root-RSA-3072.pem \
        --public-key-format full \
        --max-align 8 \
        --align 1 \
        -v "0.0.1" \
        -s 1 \
        -H 0x1000 \
        --pad-header \
        -S 0x80000 \
        --pad \
        --boot-record "HOST" \
        -L <load address> \
        <binary infile> \
        <signed binary outfile>

The ``load address`` is the logical address in the RSS memory map to which BL2
will load the image. RSS FW expects the first host image to be loaded to address
``0x70000000`` (the beginning of the RSS ATU host access region), and each
subsequent host image to be loaded at an offset of ``0x100000`` from the
previous image. The RSS ATU should be configured to map these logical addresses
to the physical addresses in the host system that the images need to be loaded
to.

For more information on the ``imgtool`` parameters, see the MCUBoot
`imgtool documentation <https://docs.mcuboot.com/imgtool.html>`_.

.. warning::

    The TF-M development key must never be used in production. To generate a
    production key, follow the imgtool documentation.

Running the code
----------------

To run the built images, they need to be concatenated into binaries that can be
placed in ROM and flash. To do this, navigate to the TF-M build directory and
run the following ``srec_cat`` commands::

    srec_cat \
        bl1_1.bin -Binary -offset 0x0 \
        bl1_provisioning_bundle.bin -Binary -offset 0xE000 \
        -o rom.bin -Binary

    srec_cat \
        bl2_signed.bin -Binary -offset 0x0 \
        bl2_signed.bin -Binary -offset 0x20000 \
        tfm_s_ns_signed.bin -Binary -offset 0x40000 \
        tfm_s_ns_signed.bin -Binary -offset 0x140000 \
        <Host AP BL1 image> -Binary -offset 0x240000 \
        <SCP BL1 image> -Binary -offset 0x2C0000 \
        <Host AP BL1 image>  -Binary -offset 0x340000 \
        <SCP BL1 image> -Binary -offset 0x3C0000 \
        -o flash.bin -Binary

For development purposes, the OTP image is included as a provisioning bundle in
the ROM image and provisioned into OTP by BL1_1. The flash image should include
the signed host images from the previous section. For each boot image, there is
a primary and secondary image; if these are different then BL2 will load the one
with the higher version number.

The ROM binary should be placed in RSS ROM at ``0x11000000`` and the flash
binary should be placed at ``0x31000000``.

--------------

*Copyright (c) 2022, Arm Limited. All rights reserved.*
