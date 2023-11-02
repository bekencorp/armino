安全功能应用指南
=====================

:link_to_translation:`en:[English]`

工具下载
+++++++++++++++++++++
  相关工具 `下载地址 </dl.bekencorp.com/tools/flash/>`_

设计说明
+++++++++++++++++++++
  BK7256的安全功能基于BK130，实现了security boot与flash加解密等功能。

 - security boot基于BOOT ROM的信任链延申，在BOOT ROM中对bootloader bin进行验签，在bootlaoder bin中对app bin进行验签，保证bootloader和app的真实性+完整性，用于确保只有客户的代码可以在芯片上运行；
 - 使用flash加解密功能需要使用加密工具对bin文件进行加密，正常启动后硬件会自动解密，保障镜像的机密性。

 使用BK7256的安全功能需要以下五个步骤：

 - 1.镜像签名
 - 2.镜像打包
 - 3.镜像加密
 - 4.添加CRC
 - 5.下载镜像，烧写密钥和efuse使能安全功能

.. important::
    为方便开发，以上步骤1到4已经集成到编译系统当中，编译系统优先去middleware/boards/bk7256寻找签名的密钥对文件ecdsa384.der和flash加密的key文件config.json，
    如果middleware/boards/bk7256下没有相关的密钥文件，会使用/tools/env_tools/beken_packager下放置的测试用密钥文件。3.2和3.3章节是生成安全镜像主要步骤的详细说明，
    用户自定义密钥时，只需要根据3.2和3.3章节运行密钥生成脚本，生成的密钥文件ecdsa384.der和flash加密的key文件config.json，然后放置到middleware/boards/bk7256下，
    直接编译，会在build/app/bk7256/encrypt路径下生成用于安全的bin文件。

1. 镜像签名
----------------------------------

    当secureboot功能使能之后，BOOT ROM会强制对bootlaoder bin（flash的0x0地址开始）进行验签，只有通过BOOT ROM验签的bootlaoder才能够运行；在bootlaoder中对app验签，通过验签才能够正常启动app。
    因此，需要对bootloader和app进行签名，客户可以自己生成密钥对分别对bootloader和app进行签名。

签名算法
********************
		 - ECDSA P384
		 - SHA256

签名指令
********************
	提供签名工具SignTool.exe和示例脚本，用于生成ECDSA密钥对和镜像签名，相关指令如下表。

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
|             | -algo    | ecdsa384    | ECDSA bit-length                           |
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
		 - 将需要签名的镜像和签名工具放置在同一目录下，运行脚本Signtool_Sign.bat，使用生成的密钥对对镜像签名。

.. figure:: ../../../../common/_static/BK_SignTool.png
    :align: center
    :alt: BK_SignTool
    :figclass: align-center

    BK_SignTool

举例说明::

    生成密钥对：SignTool.exe genkeypair -algo ecdsa384 -outfile ecdsa384.der
    获取公钥：SignTool.exe getkey -type BLKeyC -key ecdsa384.der -outfile ecdsa384.c
    镜像签名：SignTool.exe sign -prikey ecdsa384.der -infile bootloader.bin  -outfile bootloader_sign.bin -len 0x10000

.. important::
    ECDSA密钥对生成脚本执行一次即可，生成的密钥对请妥善保管；ecdsa384.c文件中的公钥需要拷贝到otp_efuse_config.json中供第五步密钥烧写使用。

2. 镜像打包
----------------------------------

    镜像打包是根据flash分区文件configuration.json将2个或者多个镜像打包成一个镜像。

镜像打包指令
********************

    提供镜像打包工具cmake_Gen_image.exe用于打包。

+-----------+---------------+--------------------+---------------------------------+
|  Command  |  Options      | Value              | Description                     |
+===========+===============+====================+=================================+
|           | -injsonfile   | configuration.json | input json file                 |
|           +---------------+--------------------+---------------------------------+
|  genfile  | -infile       | xx1.bin xx2.bin    | input bin file                  |
|           +---------------+--------------------+---------------------------------+
|           | -outfile      | all bin            | Output all bin file             |
+-----------+---------------+--------------------+---------------------------------+
|  version  | To print the current version of this utility                         |
+-----------+----------------------------------------------------------------------+
|  help     | To print this help message                                           |
+-----------+----------------------------------------------------------------------+

.. important::
    镜像可以在签名之后进行打包，也可以在签名、加密和添加CRC完成后再进行打包；
    对没有添加CRC的镜像进行打包，configuration.json文件请使用逻辑地址；
    对添加CRC后的镜像进行打包，configuration.json文件请使用物理地址。

举例说明::

    将bootloader_sign.bin和app_sign.bin打包成all_app.bin：
    cmake_Gen_image.exe  genfile -injsonfile configuration.json -infile bootloader_sign.bin app_sign.bin -outfile all_app.bin


3. 镜像加密
----------------------------------

	镜像加密使用AES-XTS256加密方式，保证镜像的机密性。

加密算法
********************

	AES-XTS256

加密指令
********************

    提供加密工具beken_aes.exe，用于密钥生成和镜像加密。

+-----------+---------------+-------------+---------------------------------------------+
|  Command  |  Options      |  Value      | Description                                 |
+===========+===============+=============+=============================================+
|           | -outfile      | config.json | falsh encrypt key is saved in config.json   |
|  genkey   +---------------+-------------+---------------------------------------------+
|           | -aes_cbc      | config.json | Key and IV for OTA are saved in config.json |
+-----------+---------------+-------------+---------------------------------------------+
|           | -keyfile      | config.json | File containing AES key                     |
|           +---------------+-------------+---------------------------------------------+
|           | -infile       | infile      | the image to be encrypted                   |
|  encrypt  +---------------+-------------+---------------------------------------------+
|           | -startaddress | Hex         | encrypt with logical start address          |
|           +---------------+-------------+---------------------------------------------+
|           | -outfile      | outfile     | encrypted image                             |
+-----------+---------------+-------------+---------------------------------------------+
|  version  |  To print the current version of this utility                             |
+-----------+---------------------------------------------------------------------------+
|  help     | To print this help message                                                |
+-----------+---------------------------------------------------------------------------+

加密过程
********************

 - 运行脚本genkey_random.bat，生成用于flash加密的密钥；
 - 将需要加密的镜像和加密工具放置在同一目录下，运行脚本encrypt_bin_in_aes_file.bat完成对镜像的加密。

举例说明::

    生成AES密钥：beken_aes.exe genkey -aes_cbc -outfile config.json
    app镜像加密：beken_aes.exe encrypt -infile app.bin -startaddress 0x10000 -keyfile config.json -outfile app_enc.bin
    all_app.bin加密：beken_aes.exe encrypt -infile all_app.bin -startaddress 0x0 -keyfile config.json -outfile all_app_enc.bin

.. important::
    AES-XTS模式加密结果与被加密文件的地址相关，在需要单独加密app镜像时-startaddress应设置为该镜像的逻辑地址。

4. 添加CRC校验
----------------------------------

    CPU在读取FLASH上镜像时硬件会进行CRC校验，因此镜像需要添加CRC之后才可以烧写到flash中。
    CRC插入规则：每32个字节计算出2个字节的CRC值，插入到该32字节之后；插入CRC之前的地址对应逻辑地址，插入CRC之后的地址对应物理地址。
    上一步镜像加密后会输出对应添加CRC后的版本，可直接用于烧写。
    提供工具cmake_encrypt_crc.exe用于添加CRC。

- 将需要添加CRC镜像和工具放置在同一目录下，运行脚本add_crc.bat即可。

举例说明::

    添加CRC： cmake_encrypt_crc.exe -crc all_enc.bin

5. 烧写密钥和efuse使能安全功能
----------------------------------

.. important::
    *Note：OTP和eFuse只能烧写一次，一但烧写后不可更改，需要谨慎操作！*
    在使能efuse的secure boot和encrypt之前，请确保flash中烧写有经过加签、加密、加CRC后的镜像，否则该芯片将无法更新镜像。

将第一步生成的ecdsa384.c中的publickey和第三步生成的config.json中的aes key拷贝到otp_efuse_config.json的安全数据配置区，方法见 :doc:`bk_OTP_and_eFuse_usermenu` 中安全数据配置区参数说明。
BKFIL.exe会根据配置文件otp_efuse_config.json，将其中的签名的公钥、加密的密钥和eFuse的配置烧写到OTP和eFuse中。

.. important::
    为方便开发和使用，已将step1-4部署到编译服务器\tools\env_tools\beken_packager下，用于签名的ecdsa384.der和flash加密的密钥config.json，优先在middleware\boards\bk7256下寻找，如果不存在则使用\tools\env_tools\beken_packager下的测试用的key。
    build/app/project/encrypt路经下会生成all_app_pack_enc_crc.bin可用于step5使能安全功能前烧写，app_pack_enc_crc.bin用于开启安全之后镜像更新，app_pack_sign.rbl用于OTA升级app。

开启安全后镜像升级方式
+++++++++++++++++++++++++

    开启安全后，当前bootloader将不可更新，只能对app镜像进行更新升级。

     - 方式一：使用BKFIL.exe工具将build/app/project/encrypt下的app_pack_enc_crc.bin烧写到对应的物理分区上。用于烧写的镜像可根据step1-4生成，也可以直接从编译服务器上获取。
     - 方式二: 使用OTA升级方式，使用build/app/project/encrypt下的app_pack_sign.rbl升级，升级方法和非安全版本一样，见OTA升级。