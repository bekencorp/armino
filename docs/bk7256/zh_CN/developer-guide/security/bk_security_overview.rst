概述
********************

BK7256基于安全模块BK130，实现了安全启动、安全调试、加解密等功能。
BK130可防止密钥和其他敏感数据被未经授权的应用程序使用，内部的OTP和ROM允许安全存储密钥材料和其他安全数据,
支持加密算法的硬件实现。

特点
********************

安全模块具有以下功能：
 - Crypto algorithm: DES, AES-128/192/256, ChaCha20-128/256, SM4-128
 - Public key accelerator: ECDSA-P256/P384, RSA-2048/3072
 - Hash algorithm: SHA-224/256, SHA-384/512, Poly1305, SM3-512
 - True random number generator (TRNG)
 - Supports secure storage
 - Supports secure boot
 - Supports secure debug
 - The Flash controller supports CRC check and the initial value of CRC is defined by eFuse.

架构
************************

.. figure:: ../../../../common/_static/BK130architecture.PNG
    :align: center
    :alt: BK130 architecture
    :figclass: align-center

    BK130 architecture

FUSE的32个字节定义：
************************

 - Byte 0~15   ： 保留给用户
 - Byte 16~23  ： Firmware usage，UID
 - Byte 24~29  ： Firmware usage，MAC
 - Byte 30     ： 用于Analog Config
 - Byte 31     ： 用于系统控制

控制字节Byte31的位定义：
************************

 - Bit 7：1关闭JTAG接口；0使用JTAG接口
 - Bit 6：reserve
 - Bit 5：1 开启 FLASH AES 加密； 0 关闭 FLASH AES 加密
 - Bit 4：reserve
 - Bit 3：byte15:0的写禁止（1）
 - Bit 2：byte16:23的写禁止（1）
 - Bit 1：byte24:29的写禁止（1）
 - Bit 0：eFUSE所有32byte的写禁止（1）

控制字节Byte30的位定义：
************************
 - Bit[0] :  Security Boot enable. 1: enable. 0: disable.
 - Bit[1] :  Security Boot 打印功能  0: enable.  1: disable
 - Bit[2] :  fast boot     0: deepsleep fast boot   1: deepsleep sb boot
 - Bit[6:3]: Reserve.