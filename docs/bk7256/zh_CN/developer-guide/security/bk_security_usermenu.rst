安全功能应用指南
=====================

:link_to_translation:`en:[English]`

设计说明
-----------------
 BK7256的安全功能基于BK130，实现了security boot与flash加解密等功能。

 - security boot基于BOOT ROM的信任链延申，在BOOT ROM中对bootloader bin进行验签，保证bootloader bin的真实性+完整性，用于确保只有客户的代码可以在芯片上运行；
 - 应用加解密功能需要对bin文件使用加密工具进行加密，正常启动后硬件会自动解密，保障镜像的机密性。

 使用BK7256的安全功能需要以下五个步骤：

 - 1.镜像签名
 - 2.镜像加密
 - 3.添加CRC
 - 4.烧写密钥
 - 5.烧写efuse

1. 镜像签名
-----------------

	当secureboot功能使能之后，只有通过BOOT ROM验签的镜像才能够正常运行，客户可以自己生成密钥对对镜像进行签名。

签名算法
********************
		 - ECDSA P-256
		 - SHA256

签名指令
********************
	提供签名工具SignTool.exe，用于生成ECDSA密钥对和镜像签名，相关指令如下表。

+-------------+----------+-------------+--------------------------------------------+
|   Command   |  Options |  Value      | Description                                |
+=============+==========+=============+============================================+
|             | -prikey  | file.der    | File containing ECDSA keypair              |
|             +----------+-------------+--------------------------------------------+
|             | -infile  | infile.img  | Input file with the image to sign          |
|    sign     +----------+-------------+--------------------------------------------+
|             | -outfile | outfile.img | Output file containing the signed image    |
|             +----------+-------------+--------------------------------------------+
|             | -length  | hex         | Size of image for ECDSA process            |
+-------------+----------+-------------+--------------------------------------------+
|             | -type    |BLKey BLKeyC | As binary or C array                       |
|             +----------+-------------+--------------------------------------------+
|   getkey    | -key     | file.der    | File containing ECDSA keypair (or pubkey)  |
|             +----------+-------------+--------------------------------------------+
|             | -outfile | outfile     | Output file containing exported public key |
+-------------+----------+-------------+--------------------------------------------+
|             | -algo    | ecdsa256    | ECDSA bit-length                           |
| genkeypair  +----------+-------------+--------------------------------------------+
|             | -outfile | outfile.der | Output file containing ECDSA keypair       |
+-------------+----------+-------------+--------------------------------------------+
|   version   | To print the current version of this utility                        |
+-------------+---------------------------------------------------------------------+
|    help     | To print this help message                                          |
+-------------+---------------------------------------------------------------------+

签名过程
********************

		 - 运行脚本Signtool_GenKeypair.bat生成ECDSA密钥对。
		 - 运行脚本Signtool_GetKey.bat获取该密钥对中的公钥。
		 - 将bootloader.bin和签名工具放置在同一目录下，运行脚本Signtool_Sign.bat对镜像签名。

.. figure:: ../../../_static/BK_SignTool.png
    :align: center
    :alt: BK_SignTool
    :figclass: align-center

    BK_SignTool

举例说明::

    生成密钥对：SignTool.exe genkeypair -algo ecdsa256 -outfile ecdsa256.der
    获取公钥：SignTool.exe getkey -type BLKeyC -key ecdsa256.der -outfile ecdsa256.c
    镜像签名：SignTool.exe sign -prikey ecdsa256.der -infile bootloader.bin  -outfile bootloader_sign.bin -len 0x10000

.. important::
    生成的密钥对请妥善保管；获取到的公钥ecdsa256.c需要拷贝到project.txt中供第四步密钥烧录使用。

2. 镜像加密
-----------------

	镜像加密使用AES-XTS256加密方式，保证镜像的机密性。

加密算法
********************

	AES-XTS256

加密指令
********************

    提供加密工具XTS_AES_encrypt.exe，用于密钥生成和镜像加密。

+-----------+---------------+-------------+--------------------------------------------+
|  Command  |  Options      |  Value      | Description                                |
+===========+===============+=============+============================================+
|           | -outfile      | project.txt | the AES key is saved to project.txt,       |
|           |               |             | including plaintext and ciphertext         |
|  genkey   +---------------+-------------+--------------------------------------------+
|           | -version      | number      | version of key                             |
+-----------+---------------+-------------+--------------------------------------------+
|           | -keyfile      | project.txt | File containing AES key                    |
|           +---------------+-------------+--------------------------------------------+
|           | -infile       | infile      | Input file with the image to encrypt       |
|  encrypt  +---------------+-------------+--------------------------------------------+
|           | -startaddress | Hex         | logical address of image                   |
|           +---------------+-------------+--------------------------------------------+
|           | -outfile      | outfile     | encrypted image file                       |
+-----------+---------------+-------------+--------------------------------------------+

加密过程
********************

 - 运行脚本XTS_AES_encrypt_getkey.bat，生成用于flash加密的密钥；如果当前目录下存在project.txt,则生成的密钥将插入到该文件中且文件名添加version前缀，否则将生成project.txt文件。
 - 将需要加密的镜像和加密工具放置在同一目录下，运行脚本XTS_AES_encrypt.bat完成对镜像的加密。

.. important::
    生成的project.txt文本中包含AES密钥的明文和密文，密钥管理者应保护密钥不能泄露，
    使用bk_write.exe烧写密钥时只提供密钥的密文即可，密钥的明文请妥善保存后在project.txt中删除。

举例说明::

    生成AES密钥：XTS_AES_encrypt.exe genkey  -version 123 -outfile bk7256.txt
    镜 像 加 密：XTS_AES_encrypt.exe encrypt -infile bootloader_sign.bin -startaddress 0 -keyfile 123_bk7256.txt -outfile bootloader_sign_enc.bin


3. 添加CRC校验
-----------------

    对于加密之后的镜像，需要添加CRC之后才可以烧写到flash中。
    提供工具encrypt_crc_enc.exe用于添加CRC。

- 将需要添加CRC镜像和工具放置在同一目录下，运行脚本CRC_app.bat即可。

举例说明::

    添加CRC：encrypt.exe app.bin 00000000

4. 密钥烧录
-----------------

.. important::
    *Note：OTP只能烧写一次，一但烧写后不可更改，需要谨慎操作！*

用于镜像签名的公钥和加密的密钥保存在OTP中，通过BK_write.exe烧写到OTP中。

配置文件project.txt中OTP的相关配置和烧写过程如下：

     - 1.选择对应的project.txt
     - 2.勾选OTP选项，会烧写public_key和aes_key
     - 3.串口波特率设置为10000，点击program后上电开始烧写

.. figure:: ../../../_static/OTP_write.png
    :align: center
    :alt: OTP
    :figclass: align-center

    OTP烧写


5. efuse烧录
-----------------

.. important::
    *Note：efuse只能烧写一次，一但烧写后不可更改，需要谨慎操作！*
    在使能efuse的secure boot和encrypt之前，请确保flash中烧写有经过加签、加密、加CRC后的bootlaod镜像，否则该芯片将无法更新镜像。

bk_write.exe是根据配置文件project.txt中对efuse的配置进行efuse的烧写，配置含义见eFuse格式 。
将加签和加密后的镜像烧写到flash，将签名公钥和加密密钥烧写到OTP，之后可以通过烧写efuse使能secure boot 和 encrypt。
	
配置文件project.txt中efuse的相关配置和烧写过程如下：

     - 选择对应的project.txt
     - 勾选updata efuse选项，会烧写efuse_cfg和security_boot
     - 串口波特率设置为10000，点击program后上电开始烧写


.. figure:: ../../../_static/efuse_write.png
    :align: center
    :alt: secureboot
    :figclass: align-center

    efuse烧写


*Note：project.txt文件配置完成后，OTP和updata efuse选项可以同时勾选进行烧写*

