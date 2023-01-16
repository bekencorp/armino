Bootloader user operation manual
======================================

:link_to_translation:`zh_CN:[中文]`

一.overview
----------------------------

This document describes the system startup process，which is used to guide engineers engaged in BSP development and maintenance to understand the system startup process and better analyze and solve problems.

二.Introduction to startup module
-----------------------------------

The startup module involves bootrom, bootloader, and applications. The startup process is as follows:

- 1.After the CPU starts, jump to the ROM address and run the ROM code.
- 2.If secure boot is enabled in EFUSE, perform a signature verification. After the signature verification is successful, set the reboot signal to enable the system to boot from the flash.
- 3.If Secure boot is not enabled in EFUSE, set the reboot signal to enable the system to boot from the flash.
- 4.Run the bootloader code in the Flash.
- 5.After the Bootloader code is executed, jump to the applications code.

三.Bootloader profile
----------------------------

The Bootloader is divided into two modules: l_bootloader and up_bootloader. l_bootloader implements the uart download function. up_bootloader Implements OTA upgrade.

- 1.The distribution of logical addresses of the two modules of the Bootloader is shown in Figure 1:

.. figure:: ../../../_static/bootloader_logic_adress.png
    :align: center
    :alt: bootloader_logic_adress
    :figclass: align-center

    Figure 1 Logical distribution of bootloader

- 2.The execution process of Bootloader is shown in Figure 2:

.. figure:: ../../../_static/bootloader_process_execution_e.png
    :align: center
    :alt: bootloader_process_execution_e
    :figclass: align-center

    Figure 2 the bootloader execution process
.. note::

  The Download partition flag in FIG. 2 refers to that during OTA upgrade, it will judge whether there is any data in the OTA data area, that is, whether it is 0xFFFFFFFF or not. If there is no upgrade data in the OTA upgrade area, it will directly jump to the app area.
  If it is not 0xFFFFFFFF, an OTA upgrade is performed.

四．L_bootloader profile
----------------------------

l_bootloader: mainly downloads uart. Figure 2 shows the flowchart.

- 1.After romcode is executed, the system jumps to address 0x0 of the flash.
- 2.In this case, the system checks whether there is data in the uart rx_buf. If there is data, the system preemps the uart bus and starts to download data.
- 3.If there is no data in the uart rx_buf file, the up_boot is executed at the specified IP address (the logical IP address) after a certain number of times of cyclic detection.
- 4.After the system starts normally, it runs the app service. After receiving the download handshake information, the uart automatically reboot to access the l_bootloader download program.

五．Up_bootloader profile
----------------------------

- 1.up_bootloader function:
  Implements the OTA upgrade function.
- 2.OTA upgrade principle:
  over-the-air download technology (OTA) supports remote firmware upgrade Over the network. In embedded OTA devices, upgrade data packets are usually downloaded to Flash through serial ports or networks, and then the downloaded data packets are transported to the code execution area of MCU for coverage, so as to complete the upgrade and update function of device firmware. OTA upgrades for embedded devicesare generally not based on file systems, but by dividing Flash into different functional areas to complete OTA upgrades. In the embedded systems scenario, the completion of an OTA remote firmware upgrade typically requires three core stages:

  - 1）Upload the new firmware to OTA server
  - 2）Download the new OTA firmware on the device
  - 3）The bootloader verifies, decrypts, decompress, and moves OTA firmware (to the corresponding flash partition).
- 3.OTA upgrade process:

  - 1）download ota firmware from the server using http protocol, then write it to the flash Download partition, and restart the device after success, as shown in Figure 3. This process is completed in the app thread;
  - 2）After the restart, up_bootloader will decompress and decrypt the upgrade file of the download partition and move it to the corresponding appcode partition. After the verification is successful, the download partition is erased and the app partition is switched to for normal execution, as shown in Figure 4.

.. figure:: ../../../_static/bootloader_app_process_e.png
    :align: center
    :alt: bootloader_app_process_e
    :figclass: align-center

六.Bootloader image file generation
--------------------------------------

- 1.Directly compile and generate bootloader.bin. The specific compilation steps are as follows:

  - 1）Access the bootloader code directory.
  - 2）make clean
  - 3）make
  - 4）Run the generate.bat script in the PACK directory and generate bootloader.bin and the corresponding bootloader_crc.bin in the generate_out directory
- 2.generate.bat script is parsed as follows:

  - 1）The packing script is as follows: .tools/cmake_Gen_image.exe genfile -injsonfile .tools/bootloader.json -infile .bootloader_l/l_bootloader.bin  .bootloader_u/u_bootloader.bin outfile .generate_out/bootloader.bin -genjson .tools/partition_bk7236.json
  - 2）Use the packaging tool cmake_Gen_image.exe to connect l_bootloader and u_bootloader into a bootloader.bin (this bin does not contain partition information) according to bootloader.json.
  - 3）Run cmake_Gen_image.exe to put the information about partition_bk7236.json behind bootloader.bin (the bootlaoder.bin in this case is the actual size), and the partition table is used for OTA upgrade.
  - 4）.tools/encrypt.exe.generate_outbootloader.bin 0x00000000 Finally, use encrypt.exe to add crc to generate bootloader_crc.bin.
  - 5） bootloader.json stores the logical allocation information of l_bootloader and u_bootloader modules. The partition_BK7236.json is the configuration information of the boloader, app, and download partitions.(Both bootloader.json and partition_bk7236.json can be changed according to actual development requirements).
  - 6）bootloade_crc.bin is made as shown in Figure 5:

 .. figure:: ../../../_static/make_bootloade_crc_e.png
    :align: center
    :alt: make_bootloade_crc_e
    :figclass: align-center

    Figure 5 Making process of bootloade_crc.bin

七.The Bootloader is used to upgrade firmware
-------------------------------------------------
- 1.OTA upgrade firmware supports compression and encryption. The upgrade firmware is automatically produced. When compiled app, generating app.bin the build/app/bk7236 directory.At the same time, in the build/app/bk7236/encrypt/directory has been generated firmware app_pack.rbl.

- 2.Open Everything-1.4.1.935.x64-Setup.exe, Tools -> Options ->The HTTP server,  then binding the local ip address. Use a browser to open the local ip address, locate the local upgrade file *.rbl,and copy the url to use in the cli command.

 .. figure:: ../../../_static/bootlaoder_everthing.png
    :align: center
    :alt: bootlaoder_everthing
    :figclass: align-center

    Figure 6 Everything tool page

- 3. Use the serial port to send the cli command, for example:
  http_ota http://192.168.21.101/D%3A/E/build/app.rbl




