快速入门
=======================

:link_to_translation:`en:[English]`

本文档以 BK7236 开发板为例，通过一个简单的示例项目向您展示:

 - 代码，工具链下载；
 - 编译环境搭建；
 - 工程配置；
 - 固件编译与烧录；

准备工作
------------------------

硬件：

 - BK7236 开发板；
 - 串口烧录工具；
 - 电脑（Windows 与 Ubuntu）；

.. note::

  Armino 仅支持在 Linux 平台编译，仅支持在 Windows 平台进行固件烧录。
  因此，您需要两台电脑，一台 Linux 平台，一台 Windows 平台；
  当然，您也可以只使用 Windows 平台，在 Windows 平台上安装 Linux 虚拟机。

软件：

 - ARM GCC 工具链，用于编译 BK7236 版本；
 - 构建工具，包含 CMake；
 - Armino 源代码；
 - 串口烧录软件；


开发板简介
------------------------

点击下列链接了解 Armino 支持的开发板详细信息：

.. toctree::
    :maxdepth: 1

        BK7236 <bk7236>


Armino SDK 代码下载
------------------------------------

您可从 gitlab 上下载 Armino::

    mkdir -p ~/armino
    cd ~/armino
    git clone http://gitlab.bekencorp.com/wifi/armino.git

您也可从 github 上下载 Armino::

    mkdir -p ~/armino
    cd ~/armino
    git clone https://github.com/bekencorp/armino.git

.. note::

    1。 Gitlab 下载的为最新的 Armino 代码，仅授权帐号才能下载，请找项目接口人申请下载权限。
    2。 Github 下载的为稳定，经过内部充分验证的版本。


构建编译环境
------------------------------------

.. note::

    Armino，当前支持Linux环境下编译，本章节将以 Ubuntu 20.04 LTS 版本为例，介绍整个编译环境的搭建。


工具链下载与安装
------------------------------------

点击 `下载 <https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2>`_ 下载 BK7236 工具链。

工具包下载后，通过如下操作命令解压至 /opt/ 目录下::

    $ sudo tar -zxvf downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 -C /


.. note::

    工具链默认路径在 middleware/soc/bk7236/bk7236.defconfig 文件中定义，您也可以在项目配置文件中更改工具链路径::

    CONFIG_TOOLCHAIN_PATH="/opt/gcc-arm-none-eabi-9-2020-q2-update/bin"

程序编译依赖库安装
------------------------------------

在终端输入下述命令安装 python3，CMake，Ninja 以及依赖库::

    sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo apt-get install build-essential cmake python3 python3-pip doxygen ninja-build libc6:i386 libstdc++6:i386 libncurses5-dev lib32z1 -y


文档编译依赖库安装
------------------------------------

点击进入 `文档 <http://docs.bekencorp.com/armino/bk7256/zh_CN/latest/index.html>`_ 您就可以查看 Armino 最近或者最新的文档，因此，
通常您不需要自己编译 Armino 文档。但如果您希望自己生成 Armino 文档，则需要安装下述 Python 依赖::

    sudo pip3 install sphinx_rtd_theme future breathe blockdiag sphinxcontrib-seqdiag sphinxcontrib-actdiag sphinxcontrib-nwdiag sphinxcontrib.blockdiag

如果您的 Python 默认为 Python2，请使用下述命令更改成 Python3::

    sudo ln -s /usr/bin/python3 /usr/bin/python


编译工程
------------------------------------

在终端下输入下述命令编译 Armino BK7236 verify 工程::

    cd ~/armino
    make bk7236 PROJECT=verify/spe

您也可以使用 verify 工程特有的编译命令来编译 BK7236 默认工程::

    cd ~/armino/project/verify
    make s

上述两种编译方式产生的结果完全一样。

verify 工程功能:

 - 启动 WiFi/BLE；
 - 初始化常见驱动；
 - 启动默认 CLI；

.. note::

  Armino 框架支持通过配置 PROJECT 参数来编译任意工程，但 BK7236 尚在开发阶段，当前仅支持 verify 工程编译。
  Verify 工程主要用于内部验证，测试，也用于给部分客户提供 Demo。


配置工程
------------------------------------

您可以通过工程配置文件来进行更改默认配置或者针对不同芯片进行差异化配置::

    工程配置文件 Override 芯片配置文件 Override 默认配置
    如： bk7236.config >> bk7236.defconfig >> KConfig
    + 工程配置文件示例：
        projects/verify/config/bk7236.config
    + 芯片配置文件示例：
        middleware/soc/bk7236/bk7236.defconfig
    + KConfig 配置文件示例：
        middleware/arch/cm33/Kconfig
        components/bk_cli/Kconfig

新建工程
------------------------------------

BK7236 默认工程为 projects/app，新建工程可参考 projects/harmony 工程


烧录代码
------------------------------------

在 Windows 平台上， Armino 目前支持 UART 烧录。

通过串口烧录
********************

.. note::

    Armino 支持 UART 烧录，推荐使用 CH340 串口工具小板进行下载。

串口烧录工具如下图所示:

.. figure:: ../../_static/download_tool_uart.png
    :align: center
    :alt: Uart
    :figclass: align-center

    UART

烧录工具（BKFIL）获取：

	http://dl.bekencorp.com/tools/flash/
	在此目录下获取最新版本，如：BEKEN_BKFIL_V2.0.7.5_20221220.zip

BKFIL.exe 界面及相关配置如下图所示：

.. figure:: ../../_static/download_uart_bk7236.png
    :align: center
    :alt: BKFIL GUI
    :figclass: align-center

    BKFIL GUI

烧录串口 UART1，点击 ``烧录`` 进行版本烧录, 烧录完成之后掉电重启设备。

.. note::

    您可以通过阅读 BKFIL 压缩包中的文档了解 BKFIL 更详细的使用方法。

串口 Log 及 Command Line
------------------------------------

目前 BK7236 平台，串口 Log 及 Cli 命令输入在 UART1 口；可通过 help 命令查看支持命令列表。
