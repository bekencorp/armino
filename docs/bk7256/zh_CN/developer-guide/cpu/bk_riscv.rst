RISC-V Guide
=====================

:link_to_translation:`en:[English]`

简介
-----------

BK7256使用的是Andes Technology(晶心科技)的双核CPU（D25F核），基于RISC-V指令集架构


Andes官网
-----------

 - Andes官网： http://www.andestech.com
 - AndeSight 工具下载路径： https://github.com/andestech/Andes-Development-Kit/releases
 - Andes CPU (D25F核)编译工具链开源代码: https://github.com/andestech/nds-gnu-toolchain


Andes相关文档
-------------

    软件安装之后，在安装目录下：以Windows为例，D:\Tools\Andestech\AndeSight_STD_v510\doc

 - 《AMSI_Drivers_and_Demos_Application_Note_AN031_V1.3.pdf》
    —Andes MCU Software Interface (AMSI) Drivers and Demos Application Note
 - 《AndeSight_STD_v5.1_IDE_Quick_Start_Guide_UM250_V1.0.pdf》
    —AndeSight IDE用户快速入门向导手册
 - 《AndeSight_STD_v5.0_IDE_User_Manual_UM230_V1.1.pdf》
    —AndeSight IDE用户手册
 - 《AndeSight_STD_v5.1_Installation_Guide_UM251_V1.0.pdf》
    —AndeSight IDE安装手册
 - 《AndeSight_STD_v5.1_Release_Notes_RN312_V1.0.pdf》
    —AndeSight IDE软件Release Note
 - 《AndeSight_v5.1_BSP_User_Manual_UM255_V1.0.pdf》
    —AndeSight BSP用户手册
 - 《AndeStar_V5_CLIC_Spec_UM205_V1.0.pdf》
    —Andes CLIC架构介绍，Armino平台7256系列产品使用PLIC架构
 - 《AndeStar_V5_SPA_UM164_V1.5.pdf》
    —AndeStar系统架构和CSR文档
 - 《AndeStar_V5_ISA_Spec_UM165_V1.4.pdf》
    —AndeStar V5 Instruction Extension Specification 扩展指令介绍
 - 《AndeStar_V5_PLIC_UM166_V1.3.pdf》
    —Andes PLIC架构介绍，Armino平台7256系列产品使用PLIC架构
 - 《AndeStar_V5_SPA_UM164_V1.5.pdf》
    —AndeStar系统架构和CSR文档
 - 《AndeStar_V5_Timer_UM167_V1.1.pdf》
    —Andes CPU内部mtimer相关介绍
 - 《Andes_AMP_System_for_Bare_Metal_and_RTOS_Application_Note_AN060_V1.0.pdf》
 - 《Andes_DSP_Library_V5_User_Manual_UM179_V2.1.pdf》
 - 《Andes_ICE_Management_Software_UM067_V3.3.pdf》
    —Andes ICE Management软件介绍，类似JLink的GDBServer
 - 《Andes_Neural_Network_Library_User_Manual_UM232_V2.1.pdf》
 - 《Andes_Programming_Guide_for_ISA_V5_PG012_V2.6.pdf》
    —Andes编程指南
 - 《The_RISC-V_Instruction_Set_Manual_Volume_II_Privileged_Architecture_TD002_V1.10.pdf》
    —Andes RISCV指令集
 - 《The_RISC-V_Instruction_Set_Manual_Volume_I_User-Level_ISA_TD001_V2.2.pdf》
    —Andes RISCV指令集(User-Level)


Andes sag转ld文件方法
----------------------

 1. 通过D:\Tools\Andestech\AndeSight_STD_v510\toolchains\nds32le-elf-mculib-v5/cygwin-andes.bat 进入cygwin
 2. 进入D:\Tools\Andestech\AndeSight_STD_v510\utils目录，将sag文件放入该目录下
 3. 在cygwin环境执行如下命令::
    nds_ldsag.exe --version=v5 -t ./nds32_template_v5.txt bk7256_bsp.sag -o bk7256_bsp.ld
