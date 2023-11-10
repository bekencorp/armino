安全方案
=====================

:link_to_translation:`en:[English]`

BK7256的安全方案支持secure boot、flash加解密、固件版本防回滚等功能，针对不同的安全需求，有以下三种场景可选择：

 - 仅开启security boot
 - 仅开启flash encrypt
 - security boot和flash encrypt都开启

1.  仅开启security boot

特点：
 - 会强制检查镜像的真实性+完整性，确保只有客户的代码可以运行
 - 烧写在flash上的镜像是明文
 - 密钥被保护，secureboot开启后软件不可读
 - bootloader不可更新
 - secureboot key、ota key可以定制
 - 支持版本防回滚

 镜像选择：首次烧写选择镜像all_app_pack_sign_crc.bin，烧写地址0x0
         更新app时烧写镜像app_pack_sign_crc.bin，烧写地址0x11000

2.  仅开启flash encrypt

特点：
 - 确保了镜像的机密性，烧写在flash上的镜像是密文
 - 密钥不被保护，软件可读出
 - Bootloader和app可通过串口更新
 - flash加密密钥可自主设定，OTA 传输加密的key固定使用默认值
 - 不支持版本防回滚

镜像选择：烧写镜像all_app_pack_enc_crc.bin，烧写地址0x0

3.  security boot和flash encrypt都开启

特点：
 - 提供最高等级的安全保护，真实性+完整性+机密性
 - 密钥被保护，secureboot开启后软件不可读
 - bootloader不可更新
 - secureboot key、flash加密的key、OTA传输加密的 key可以定制
 - 支持版本防回滚

 镜像选择：首次烧写选择镜像all_app_pack_sign_enc_crc.bin，烧写地址0x0
        更新app时烧写镜像app_pack_sign_enc_crc.bin，烧写地址0x11000

以上安全相关镜像在build\app\bk7256\encrypt路径下。

.. important::
    建议使用安全功能时security boot和flash encrypt都开启，使用最高等级的保护。

