This section describes the OTA of the BK7256 AB partition
============================================================

:link_to_translation:`zh_CN:[中文]`

一、overview
----------------------------

 - In the AB partition scheme, program images are stored in two locations (area A and area B).When an image in area A is used, the upgrade image is stored in area B.If the image in area B is used, the upgrade image is saved to area A.Customers can determine which partition to use after the upgrade based on certain conditions (for example, if some functions of the upgraded image are faulty, the original image is still used).The distribution of flash perspective and cpu perspective corresponding to AB partition is shown in the figure 1 below (only three partitions including Bootloader and app (A/B) are included; usrer, RF and Net are not listed, please note).

.. figure:: ../../../_static/ab_flash_cpu_partition.png
    :align: center
    :alt: ab_flash_cpu_partition
    :figclass: align-center

    Figure 1 Partition of flash view &cpu view

二、AB partition Introduction
-----------------------------
1. AB Working principles of partition upgrade
++++++++++++++++++++++++++++++++++++++++++++++

 - When the APP is running in partition A and OTA upgrade is performed, image B is downloaded from the server, and reboot is performed after the download.Then, the bootloader hash verifies the accuracy of the data. If the data is OK, it will run to partition B.Otherwise, it still runs in partition A.
 - Similarly, when the APP runs in partition B and performs an over-the-air (OTA) upgrade, the image A is downloaded from the server. After the image is downloaded, the reboot is performed.Then, the bootloader hash to verify the accuracy of the data. If the data is OK, it will run to partition A.Otherwise, it still runs in partition B.

2. AB partition compilation commands
+++++++++++++++++++++++++++++++++++++

  - compile A partition: make bk7256 PROJECT=customization/bk7256_configa
  - compile B partition: make bk7256 PROJECT=customization/bk7256_configb

3. AB partition image upgrade path
++++++++++++++++++++++++++++++++++++++

  The image required for the upgrade has been automatically generated. The image path is as follows:

  - A-->B：image B path: build/bk7256_configb/bk7256/app_ab_crc.bin
  - B-->A：image A path: build/bk7256_configa/bk7256/app_ab_crc.bin

4. Advantages of the AB partition
+++++++++++++++++++++++++++++++++++

  There are two advantages to comparing with traditional OTAs:

  - Advantage 1: After the software is downloaded, the reboot of the AB partition is faster. Because the image of the AB solution is not compressed, there is no time to decompress during the upgrade.
  - Advantage 2: When you upgrade B from A, if the image of B is faulty and backup area B becomes unavailable after the download, the system switches to the original partition A after power-on.(Avoid using problematic versions)


三、 the introduction of an AB partition upgrading
-------------------------------------------------------

  The AB partition upgrade process consists of three parts: Download the upgrade image.The Bootloader confirms the integrity process of the upgrade image.The APP confirms the final running partition (some customers demand, the final execution of which partition is decided by them, by macro control, if the customer does not feedback, the default jump partition is executed).The upgrade involves three flag bits: they are saved in a certain 4K space of the flash (the current location is 0x3FD000).

  - flag1: ota_temp_exec_flag: 3—A; 4--B (temp update).
  - flag2: cust_confirm_flag: 1--Indicates successful download; 3—A; 4--B (custm final exec).
  - flag3: ota_exec_flag:  0/0xFF—A; 1—B.

  The three flags used in the ab partition upgrade process are described as follows:

  1）ota_temp_exec_flag: After the upgrade image is downloaded, the customer temporarily decides which partition to jump to.

  - 3: temporarily decide to jump to partition A on behalf of the customer;
  - 4: temporarily decide to jump to partition B on behalf of the customer;(The flag bit is controlled according to the macro, if the customer does not feedback, the default flag bit is executed), and then the value will be converted, 3 will be converted to 0 (A partition), 4 will be converted to 1 (B partition), and 0/1 will be written into flash.

  2）cust_confirm_flag: There are two meanings: a) The mark that the upgrade image download is complete;b) After the restart, the customer finally decides which partition to execute.

  - 1: indicates that the download is successful (the value is set to 1 only when the download is successful), and then the data is written into the flash.
  - 3: indicates execution zone A.
  - 4: indicates execution zone B.(The flag bit is controlled according to the macro. If the customer does not respond, the default flag bit is executed.) The code will eventually change to set ota_exec_flag to 0/1, and then write it into the flash.

  3）ota_exec_flag: Before downloading the upgrade image, know the partition to be upgraded based on the current code.

  - 0: upgrade zone A.(0xFF: indicates the first burn run in partition A)
  - 1: upgrade zone B.(If the download process and checkout are successful, after reboot, it will be executed on this partition, which should be the same as the default partition represented by cust_confirm_flag), and then write to the flash.

  Figure 2 shows the process for downloading the upgrade image of the AB partition:

  .. figure:: ../../../_static/ab_app_en.png
     :align: center
     :alt: ab_app_en
     :figclass: align-center

     Figure 2 the image of download process

  Note: Figure 2 mainly implements the downloading function of the upgrade image and sets the corresponding flag. The combination of flag bits is shown in red.

  The jump process of AB partition is shown in Figure 3.

  .. figure:: ../../../_static/ab_bootloader_en.png
     :align: center
     :alt: ab_bootloader_en
     :figclass: align-center

     Figure 3 bootloader jump process

  Note: Figure 3 determines which partition to jump to according to the flag in Figure 2 and hash verification.


四. Introduction to the AB partition hash function verification process
--------------------------------------------------------------------------

The hash algorithm is used to ensure the integrity of downloaded data. The hash verification process consists of the following two steps:

- 1. Obtain the head information of the rbl in the upgrade firmware (the rbl information is stored at the last 4k position of the upgrade firmware).
- 2. Then calculate the hash value of the entire firmware using the hash256 algorithm and compare the hash value stored in the rbl. If two hash values are the same, the hash passes and the firmware is complete; otherwise, the downloaded data is incomplete.
- 3. Upgrade zone B from zone A as an example.

  - 1.1) After A downloads B, reboot jumps to the bootloader and obtains the rbl information of firmware B.
  - 1.2) Then calculate the hash value of upgrade B using hash256 algorithm. If the hash value is equal to the hash value in rbl, the data of upgrade B is complete and is directly jumped to partition B for execution.
  - 1.3) If the hash value obtained by using hash256 algorithm is different from the hash value in rbl, delete the data in partition B and continue to execute partition A.