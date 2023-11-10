###########################
Musca-B1 Platform Specifics
###########################

****************
DAPLink Firmware
****************
The code on Musca-B1 is running from embedded flash. Make sure that the DAPLink
FW for eFlash is downloaded to the board. You can find on the
`Arm Community page <https://community.arm.com/oss-platforms/w/docs/552/musca-b1-firmware-update-qspi-eflash-boot-recovery>`__
A short description of how to update the DAPLink FW can be found there as well.

.. Note::
    Warm reset of eFlash is not supported on Musca_B1. TF-M may not boot after
    a warm reset. Further information on the hardware limitation can be
    found on `Arm Community page <https://community.arm.com/oss-platforms/w/docs/566/musca-b1-warm-reset-of-eflash>`__.

********************
Platform pin service
********************

This service is designed to perform secure pin services of the platform
(e.g alternate function setting, pin mode setting, etc).
The service uses the IOCTL API of TF-M's Platform Service, which allows the
non-secure application to make pin service requests on Musca B1 based on a
generic service request delivery mechanism.

--------------

*Copyright (c) 2017-2022, Arm Limited. All rights reserved.*
