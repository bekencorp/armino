I2S demo
========================

:link_to_translation:`en:[English]`

1 功能概述
--------------------------
	I2S demo支持音频信号在不同I2S配置下的传输。

2 代码路径
--------------------------
	demo路径: ``\components\demos\media\audio\i2s``

	I2S模块API接口的详细说明请参考同网页: ``/api-reference/multi_media/bk_i2s.html``

3 cli命令简介
--------------------
demo支持的命令如下表:

+-----------------------------------+-------------------------------+
|Command                            |Description                    |
+===================================+===============================+
|cpu1 i2s_master_test {start|stop}  |作为master运行I2S测试demo      |
+-----------------------------------+-------------------------------+
|cpu1 i2s_slave_test {start|stop}   |作为slave运行I2S测试demo       |
+-----------------------------------+-------------------------------+

demo运行依赖的宏配置:

+---------------------+---------------------------+---------------------------------------------------+-----+
|Name                 |Description                |   File                                            |value|
+=====================+===========================+===================================================+=====+
|CONFIG_I2S           |配置I2S功能使能            |``\properties\soc\bk7256_cp1\bk7256_cp1.defconfig``|  y  |
+---------------------+---------------------------+---------------------------------------------------+-----+
|CONFIG_I2S_TEST      |配置demo是否生效           |``\properties\soc\bk7256_cp1\bk7256_cp1.defconfig``|  y  |
+---------------------+---------------------------+---------------------------------------------------+-----+

demo运行依赖的库和驱动:
 - GPIO GPIO驱动


4 演示介绍
--------------------------
demo执行的步骤如下:

	1.连接两块测试板子
	 - 使用两块开发板进行测试，一块作为master端，一块作为slave端
	 - 将两块板子的GPIO6和GPIO7引脚互连
	 - 将master端的GPIO8和slave端的GPIO9连接
	 - 将master端的GPIO9和slave端的GPIO8连接

	2.开始slave端测试
	 - slave端Uart发送AT指令 ``cpu1 i2s_slave_test start`` 执行slave角色的I2S功能测试 

	3.开始master端测试
	 - Uart发送AT指令 ``cpu1 i2s_master_test start`` 执行master角色的I2S功能测试

	4.停止测试
	 - 观察串口log打印，待测试完成后，slave和master端分别Uart发送AT指令 ``cpu1 i2s_slave_test stop`` 和 ``cpu1 i2s_master_test stop`` 停止执行I2S功能测试

执行测试指令, case的工作流程如下图所示:

.. figure:: ../../../_static/i2s_demo_flow.png
    :align: center
    :alt: i2s软件流程
    :figclass: align-center

    Figure 1. i2s work flow chart

5 详细配置及说明
--------------------------
I2S支持的工作模式和采样率说明如下:

- 工作模式

	I2S模块支持下述多种工作模式:
	 - Philips
	 - Left Justified
	 - Right Justified
	 - Short Sync Frame
	 - Long Sync Frame
	 - Normal 2B+D模式
	 - Delay 2B+D模式
	 - TDM模式

.. important::

  使用TDM模式时, 只需要将工作模式配置为其他的7中工作模式中的一种, 然后使能多个通道就可，模块内部会将多个通道的数据组合成时分复用的形式发出

- 采样率

	I2S模块支持下述采样率Fs(bps):
	 - 8K
	 - 12K
	 - 16K
	 - 24K
	 - 32K
	 - 48K
	 - 96K
	 - 8.0182K
	 - 11.025K
	 - 22.05K
	 - 44.1K
	 - 88.2K

.. note::
  建议采用前6种常用的采样率

.. important::
  注意事项:
   - 1.I2S通讯时master和slave的DIN和DOUT引脚连接要正确，master的DIN和slave的DOUT连接，master的DOUT和slave的DIN连接;
   - 2.master和slave的工作模式要一致;
   - 3.测试demo主要测试了前三种工作模式和不同的采样率.
