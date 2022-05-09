# LiteOS-M内核<a name="ZH-CN_TOPIC_0000001096757661"></a>

-   [简介](#section11660541593)
-   [目录](#section161941989596)
-   [约束](#section119744591305)
-   [使用说明](#section3732185231214)
-   [相关仓](#section1371113476307)

## 简介<a name="section11660541593"></a>

OpenHarmony LiteOS-M内核是面向IoT领域构建的轻量级物联网操作系统内核，具有小体积、低功耗、高性能的特点，其代码结构简单，主要包括内核最小功能集、内核抽象层、可选组件以及工程目录等，分为硬件相关层以及硬件无关层，硬件相关层提供统一的HAL（Hardware Abstraction Layer）接口，提升硬件易适配性，不同编译工具链和芯片架构的组合分类，满足AIoT类型丰富的硬件和编译工具链的拓展。其架构图如图1所示：

**图 1**  OpenHarmony LiteOS-M核内核架构图<a name="fig0865152210223"></a>  
![](figures/OpenHarmony-LiteOS-M核内核架构图.png "OpenHarmony-LiteOS-M核内核架构图")

## 目录<a name="section161941989596"></a>

目录结构如下，详细目录请参考[arch_spec_zh.md](arch_spec_zh.md)。

```
/kernel/liteos_m
├── components           # 可选组件
│   ├── backtrace        # 回溯栈支持
│   ├── cppsupport       # C++支持
│   ├── cpup             # CPUP功能
│   ├── dynlink          # 动态加载与链接
│   ├── exchook          # 异常钩子
│   ├── fs               # 文件系统
│   └── net              # Network功能
├── kal                  # 内核抽象层
│   ├── cmsis            # cmsis标准接口支持
│   └── posix            # posix标准接口支持
├── kernel               # 内核最小功能集支持
│   ├── arch             # 内核指令架构层代码
│   │   ├── arm          # arm32架构的代码
│   │   └── include      # 对外接口存放目录
│   ├── include          # 对外接口存放目录
│   └── src              # 内核最小功能集源码
├── targets              # 板级工程目录
├── utils                # 通用公共目录
```

## 约束<a name="section119744591305"></a>

开发语言：C/C++；

适用架构：当前只适用于cortex-m3、cortex-m4、cortex-m7、risc-v芯片架构。

## 使用说明<a name="section3732185231214"></a>

OpenHarmony LiteOS-M内核的编译构建系统是一个基于gn和ninja的组件化构建系统，支持按组件配置、裁剪和拼装，按需构建出定制化的产品。本文主要介绍如何基于gn和ninja编译LiteOS-M工程，GCC+Makefile、IAR、Keil MDK等编译方式可以参考社区爱好者贡献的站点。

### 搭建系统基础环境

在搭建各个开发板环境前，需要完成OpenHarmony系统基础环境搭建。系统基础环境主要是指OpenHarmony的编译环境和开发环境，详细介绍请参考官方站点[Ubuntu编译环境准备](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/device-dev/quick-start/quickstart-lite-env-setup-linux.md)。开发者需要根据环境搭建文档，完成下述软件的安装：Python3.7+、gn、ninja、hb。对于LiteOS-M内核，还需要安装Make构建工具和[ARM GCC编译工具链](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)。

### 获取OpenHarmony源码

开发者需要在Linux服务器上通过Git克隆获取OpenHarmony最新源码，详细的源码获取方式，请见[源码获取](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/device-dev/get-code/sourcecode-acquire.md)。获取OpenHarmony完整仓代码后，假设克隆目录为`~/openHarmony`。

### 获取示例工程源码

以开发板Nucleo-F767Zi为例，演示如何编译运行`OpenHarmony LiteOS-M`内核工程。在本地目录，执行下述命令克隆示例代码。

```
git clone https://gitee.com/harylee/nucleo_f767zi.git
```

假设克隆到的代码目录为`~/nucleo_f767zi`。 执行如下命令把代码目录的`device`、`vendor`目录复制到`openHarmony`工程的相应目录。

```
mkdir ~/openHarmony/device/st

cp -r ~/nucleo_f767zi/device/st/nucleo_f767zi ~/openHarmony/device/st/nucleo_f767zi

chmod +x ~/openHarmony/device/st/nucleo_f767zi/build.sh

cp -r ~/nucleo_f767zi/vendor/st ~/openHarmony/vendor/st
```

关于示例代码目录的说明，可以参考资料站点[板级目录规范](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/device-dev/porting/transplant-chip-board-overview.md)。如果需要自行移植开发板，请参考[板级系统移植](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/device-dev/porting/transplant-chip-board.md)。

### 编译运行

编译运行前，交叉编译工具链`bin`目录配置到`PATH`环境变量中或者配置`device/st/nucleo_f767zi/liteos_m/config.gni`文件中`board_toolchain_path`配置项为交叉编译工具链`bin`目录。
在`OpenHarmony`根目录，执行`hb set`设置产品路径，选择`nucleo_f767zi`产品，然后执行`hb build`开启编译。如下：

```
user@dev:~/OpenHarmony$ hb set

[OHOS INFO] Input code path: # 直接按回车，然后选择nucleo_f767zi产品即可

OHOS Which product do you need? nucleo_f767zi@st

user@dev:~/OpenHarmony$ hb build
```

最终的镜像生成在`~/openHarmony/out/nucleo_f767zi/`目录中，通过`STM32 ST-LINK Utility`软件将镜像文件下载至单板查看运行效果。

### 社区移植工程链接

LiteOS-M内核移植的具体开发板的工程由社区开发者提供，可以访问社区开发者代码仓获取。如果您移植支持了更多开发板，可以提供链接给我们进行社区分享。

-   cortex-m3：

    - STM32F103   https://gitee.com/rtos_lover/stm32f103_simulator_keil

        该仓包含OpenHarmony LiteOS-M内核基于STM32F103芯片架构构建的Keil工程，支持Keil MDK方式进行编译。

-   cortex-m4：

    - 野火挑战者STM32F429IGTb   https://gitee.com/harylee/stm32f429ig_firechallenger

        该仓包含OpenHarmony LiteOS-M内核移植支持`野火挑战者STM32F429IGTb`开发板的工程代码，支持Ninja、GCC、IAR等方式进行编译。

-   cortex-m7：

    - Nucleo-F767ZI   https://gitee.com/harylee/nucleo_f767zi

        该仓包含OpenHarmony LiteOS-M内核移植支持`Nucleo-F767ZI`开发板的工程代码，支持Ninja、GCC、IAR等方式进行编译。

## 相关仓<a name="section1371113476307"></a>

[内核子系统](https://gitee.com/openharmony/docs/blob/HEAD/zh-cn/readme/%E5%86%85%E6%A0%B8%E5%AD%90%E7%B3%BB%E7%BB%9F.md)

**kernel\_liteos\_m**

