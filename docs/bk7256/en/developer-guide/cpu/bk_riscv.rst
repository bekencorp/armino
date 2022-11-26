RISC-V Guide
=====================

:link_to_translation:`zh_CN:[中文]`

Introduce
--------------------------------------------

BK7256 uses the dual core CPU (D25F core) of Andes Technology, which is based on RISC-V instruction set architecture


Andes official website
-------------------------------------------------------

 - Andes： http://www.andestech.com
 - AndeSight Tool download path： https://github.com/andestech/Andes-Development-Kit/releases
 - Andes CPU (D25F core)Compilation tool chain open source code: https://github.com/andestech/nds-gnu-toolchain


Andes related documents
---------------------------------------------------------

    After the software is installed：Take Windows as an example, D:\Tools\Andestech\AndeSight_STD_v510\doc

 - 《AMSI_Drivers_and_Demos_Application_Note_AN031_V1.3.pdf》
    —Andes MCU Software Interface (AMSI) Drivers and Demos Application Note
 - 《AndeSight_STD_v5.1_IDE_Quick_Start_Guide_UM250_V1.0.pdf》
    —AndeSight IDE User Quick Start Guide
 - 《AndeSight_STD_v5.0_IDE_User_Manual_UM230_V1.1.pdf》
    —AndeSight IDE User 's manual
 - 《AndeSight_STD_v5.1_Installation_Guide_UM251_V1.0.pdf》
    —AndeSight IDE Installation manual
 - 《AndeSight_STD_v5.1_Release_Notes_RN312_V1.0.pdf》
    —AndeSight IDE Software Release Note
 - 《AndeSight_v5.1_BSP_User_Manual_UM255_V1.0.pdf》
    —AndeSight BSP User 's manual
 - 《AndeStar_V5_CLIC_Spec_UM205_V1.0.pdf》
    —Andes CLIC Introduction to architecture
 - 《AndeStar_V5_SPA_UM164_V1.5.pdf》
    —AndeStar System architecture and CSR documents
 - 《AndeStar_V5_ISA_Spec_UM165_V1.4.pdf》
    —AndeStar V5 Instruction Extension Specification Introduction to Extension Instructions
 - 《AndeStar_V5_PLIC_UM166_V1.3.pdf》
    —Andes PLIC Introduction to architecture
 - 《AndeStar_V5_SPA_UM164_V1.5.pdf》
    —AndeStar System architecture and CSR documents
 - 《AndeStar_V5_Timer_UM167_V1.1.pdf》
    —Andes CPU Introduction to internal mtimer
 - 《Andes_AMP_System_for_Bare_Metal_and_RTOS_Application_Note_AN060_V1.0.pdf》
 - 《Andes_DSP_Library_V5_User_Manual_UM179_V2.1.pdf》
 - 《Andes_ICE_Management_Software_UM067_V3.3.pdf》
    —Andes ICE Management Software introduction, similar to the GDBServer of JLink
 - 《Andes_Neural_Network_Library_User_Manual_UM232_V2.1.pdf》
 - 《Andes_Programming_Guide_for_ISA_V5_PG012_V2.6.pdf》
    —Andes Programming Guide
 - 《The_RISC-V_Instruction_Set_Manual_Volume_II_Privileged_Architecture_TD002_V1.10.pdf》
    —Andes RISCV Instruction set
 - 《The_RISC-V_Instruction_Set_Manual_Volume_I_User-Level_ISA_TD001_V2.2.pdf》
    —Andes RISCV Instruction set(User-Level)


Andes Method of transforming sag to ld file
------------------------------------------------------------------

 1. Enter 'cygwin' through 'D:\\Tools\\Andestech\\AndeSight_STD_v510\\toolchains\\nds32le-elf-mculib-v5\\cygwin-andes.bat'
 2. Enter 'D:\\Tools\\Andestech\\AndeSight_STD_v510\\utils', Put the sag file in this directory

Execute the following command in the cygwin environment::

    nds_ldsag.exe --version=v5 -t ./nds32_template_v5.txt bk7256_bsp.sag -o bk7256_bsp.ld
