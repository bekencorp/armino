Bootloader用户操作手册
========================

:link_to_translation:`en:[English]`

一.概述
----------------------------

该文档主要介绍系统启动流程，用于指导从事BSP开发和维护的相关工程师理解系统的启动过程，更好分析和解决问题。

二.启动模块介绍
----------------------------

启动模块涉及到bootrom, bootloader,applications，启动过程如下:

- 1.CPU启动之后首先跳转到ROM地址上，执行ROM code。
- 2.如果EFUSE中开启了安全boot，则进行验签，验签成功后，设置reboot信号，系统从flash中启动。
- 3.如果EFUSE中没有启动安全boot,则设置reboot信号，系统从flash中启动。
- 4.Flash中先执行bootloader代码。
- 5.Bootloader代码执行完之后跳转到applications代码。

三.Bootloader简介
----------------------------

Bootloader分为l_bootloader和up_bootloader两个模块，l_bootloader实现uart下载功能；up_bootloader实现OTA升级功能。

- 1.Bootloader的两个模块的逻辑地址分布如图1所示：

.. figure:: ../../../_static/bootloader_logic_adress.png
    :align: center
    :alt: bootloader_logic_adress
    :figclass: align-center

    图1bootloader的逻辑分布

- 2.Bootloader的执行流程如图2所示：

.. figure:: ../../../_static/bootloader_process_execution.png
    :align: center
    :alt: bootloader_process_execution
    :figclass: align-center

    图2 bootloader的执行流程

四．l_bootloader简介
----------------------------

l_bootloader：主要是uart下载功能，流程图如上图2所示。

- 1.在romcode执行完成之后会跳转到flash的0x0地址执行；
- 2.此时会判断uart rx_buf中有是否有数据，如果有数据，抢占到uart总线进入数据下载状态，下载完成之后会reboot；
- 3.如果此时uart rx_buf中没有数据，会循环检测一定次数后会跳转到指定地址（此为逻辑地址）处执行up_boot。
- 4.系统正常启动后运行在app业务中，uart接收到下载握手信息后会主动reboot进入l_bootloader下载程序，用户也可以取消此功能。

五．up_bootloader简介
----------------------------

- 1.up_bootloader作用：
  实现OTA升级功能。
- 2.OTA升级原理：
  OTA（Over-the-AirTechnology）即空中下载技术，支持网络端远程升级固件。在嵌入式设备OTA 中，通常通过串口或者网络等方式，将升级数据包下载到Flash，然后将下载得到的数据包搬运到MCU 的代码执行区域进行覆盖，以完成设备固件升级更新的功能。
  嵌入式设备的OTA升级一般不基于文件系统，而是通过对Flash划分为不同的功能区域来完成OTA升级功能。在嵌入式系统方案里，要完成一次OTA固件远端升级，通常需要以下三个核心阶段：

  - 1）上传新固件到OTA服务器
  - 2）设备端下载新的OTA固件
  - 3）bootloader对OTA固件进行校验、解密、解压缩和搬运（搬运到对应flash分区）。
- 3.OTA升级过程：

  - 1）采用http协议从服务器下载ota固件，然后写到flash的download分区，成功之后并重启设备，如图3所示，此过程在app线程中完成；
  - 2）在重启后up_bootloader会将download分区的升级文件解压解密后搬运到对应的appcode分区，校验成功后擦除download分区后，跳转到app分区正常执行，如图4所示。

.. figure:: ../../../_static/bootloader_app_process.png
    :align: center
    :alt: bootloader_process
    :figclass: align-center

六.Bootloader镜像文件生成
----------------------------

- 1.直接编译生成bootloader.bin；具体编译步骤如下：

  - 1）进入bootloader代码目录下；
  - 2）make clean
  - 3）make
  - 4）在PACK目录下执行generate.bat脚本，在generate_out目录下生成bootloader.bin以及对应的bootloader_crc.bin
- 2.generate.bat脚本解析如下：

  - 1）打包脚本如下：.\tools\cmake_Gen_image.exe genfile -injsonfile .\tools\bootloader.json -infile .\bootloader_l\l_bootloader.bin .\bootloader_u\u_bootloader.bin -outfile  .\generate_out\bootloader.bin -genjson .\tools\partition_bk7256.json
  - 2）使用打包工具cmake_Gen_image.exe根据bootloader.json将l_bootloader与u_bootloader连接成一个bootloader.bin（此时该bin不含分区信息）。
  - 3）通过cmake_Gen_image.exe将分区表（partition_bk7256.json）相关信息放在将bootloader.bin后面（此时的
    bootlaoder.bin为实际大小），分区表供OTA升级使用。
  - 4）.\tools\encrypt.exe .\generate_out\bootloader.bin 00000000最后，使用工具encrypt.exe添加crc，生成 bootloader_crc.bin。
  - 5）bootloader.json存放的是l_bootloader与u_bootloader模块的逻辑分配信息；partition_bk7256.json是boloader、app、app1以及download分区配置信息。（bootloader.json与partition_bk7256.json均可根据实际开发需求更改）。
  - 6）bootloade_crc.bin制作如图5所示：

 .. figure:: ../../../_static/make_bootloade_crc.png
    :align: center
    :alt: make_bootloade_crc
    :figclass: align-center

    图5 bootloade_crc.bin制作过程

七.Bootloader升级固件制作
----------------------------
- 1.OTA升级固件支持压缩和加密，使用rt_ota_packaging_tool.exe打包工具制作升级固件，选择需要升级的app.bin,生成同名的*.rbl文件，如图6所示：

 .. figure:: ../../../_static/bootloader_pack_firmware.png
    :align: center
    :alt: bootloader_pack_firmware
    :figclass: align-center

    图6 固件打包

- 2.打开Everything-1.4.1.935.x64-Setup.exe，工具->选项->HTTP服务器，绑定本机ip。使用浏览器打开本机的ip地址，找到本地的升级文件 *.rbl，拷贝url出来，用于cli命令。

 .. figure:: ../../../_static/bootlaoder_everthing.png
    :align: center
    :alt: bootlaoder_everthing
    :figclass: align-center

    图7 Everything工具页面

- 3.使用串口发送cli命令，例如：
  http_ota http://192.168.21.101/D%3A/E/build/app.rbl




