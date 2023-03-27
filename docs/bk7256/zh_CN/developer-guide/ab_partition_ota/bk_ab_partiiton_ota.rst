BK7256 AB分区_OTA介绍
========================

:link_to_translation:`en:[English]`

一、概述
----------------------------

 - AB分区方案是指在两个位置（A区和B区）存放程序镜像。当在使用A区镜像时，升级镜像则存到B区。如果使用B区镜像，升级镜像则存到A区。客户可以根据一定的条件判断，升级后最终使用哪个分区（例如升级的镜像某些功能有问题，则还是用之前运行镜像）。AB分区对应的flash视角、cpu视角分布如下图1所示（仅包含Bootloader、app（A/B）等三个分区，user以及RF、Net未列出，请知悉）。

.. figure:: ../../../_static/ab_flash_cpu_partition.png
    :align: center
    :alt: ab_flash_cpu_partition
    :figclass: align-center

    图1 flash视角&cpu视角的分区

二、AB分区介绍
----------------------------
1. AB分区升级原理
+++++++++++++++++++++++++

 - 当APP(应用程序)运行在A分区并进行OTA升级时，即从服务器下载镜像B，下载完成之后进行reboot。然后，bootloader进行hash验证来数据的准确性，若数据没问题会运行到B分区；否则，还是运行在A分区。
 - 同理，当APP运行在B分区并进行OTA（Over-the-AirTechnology）升级时，即从服务器下载镜像A，下载镜像完成之后进行reboot。然后，bootloader进行hash来验证数据的准确性，若数据没问题会运行到A分区；否则，还是运行在B分区。

2. AB分区编译命令介绍
+++++++++++++++++++++++++

  - 编译A分区：make bk7256 PROJECT=customization/bk7256_configa
  - 编译B分区：make bk7256 PROJECT=customization/bk7256_configb

3. AB分区升级镜像路径
+++++++++++++++++++++++++

  升级所需的镜像已自动化生成，镜像路径如下：

  - A-->B：镜像B路径：build/bk7256_configb/bk7256/app_ab_crc.bin
  - B-->A：镜像A路径：build/bk7256_configa/bk7256/app_ab_crc.bin

4. AB分区的优势
+++++++++++++++++++++++++

  有传统OTA进行对比有两个优势：

  - 优势一：下载之后，AB分区reboot更快（由于AB方案的镜像没有压缩，因此升级过程中没有解压的时间）；
  - 优势二：当进行A升级B时，下载成功之后，假如镜像B有问题，导致备份区B无法使用，上下电之后，会跳转到原来的分区A。（避免使用有问题的版本）


三、AB分区升级流程介绍
----------------------------

  AB分区升级流程分为三部分：升级镜像下载流程；Bootloader确认升级镜像完整性流程；APP确认最终运行分区(有的客户需求，最终执行哪个分区由他们决定，由宏控制，如果客户不反馈，执行默认跳转分区）。升级的过程中涉及三个flag标志位：均保存在flash某个4K空间内（当前位置在0x3FD000）。

  - flag1：ota_temp_exec_flag: 3—A； 4--B (temp update)。
  - flag2：cust_confirm_flag : 1--代表下载成功；3—A； 4--B (custm final exec)。
  - flag3：ota_exec_flag :  0/0xFF—A；1—B。

  ab分区升级流程用到的三个标志位具体解释如下：

  1）ota_temp_exec_flag ：升级镜像下载完成后，客户临时决定跳转哪个分区；

  - 3：代表客户临时决定跳转分区A；
  - 4：代表客户临时决定跳转分区B；（该标志位根据宏控制，如果客户不反馈，执行默认标志位），然后数值会做转换，3转换为0（A分区），4转换为1（B分区），会将0/1写入flash。

  2）cust_confirm_flag ：有两层含义a) 升级镜像下载完成的标志；b)重启之后客户最终决定执行哪个分区。

  - 1：代表下载成功（只有下载成功的时候会置为1 ），然后写入flash；
  - 3：代表执行分区A；
  - 4：代表执行分区B；（该标志位根据宏控制，如果客户不反馈，执行默认标志位），代码里最终会转为设置ota_exec_flag为0/1，然后写入flash。

  3）ota_exec_flag ：升级镜像下载之前，根据当前代码获知当前要升级哪个分区；

  - 0：代表升级分区A；（0xFF：代表第一次烧录运行在分区A）
  - 1：代表升级分区B；（如果下载流程及验签成功，reboot之后，就会执行在该分区上，和cust_confirm_flag代表的默认分区应该一致），然后写入flash。

  AB分区的升级镜像的下载流程如图二所示：

  .. figure:: ../../../_static/ab_app.png
     :align: center
     :alt: ab_app
     :figclass: align-center

     图2 镜像下载流程

  备注：图2主要实现升级镜像的下载功能，以及设置对应的flag，flag标志位组合如上面红色备注所示。

  AB分区的跳转流程如图三所示。

  .. figure:: ../../../_static/ab_bootloader.png
     :align: center
     :alt: ab_bootloader
     :figclass: align-center

     图3 bootloader跳转流程

  备注：图3根据图2中的flag，配合hash验证，确定跳转哪个分区。