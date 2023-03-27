OTP和eFuse的配置及读写方法
==========================

:link_to_translation:`en:[English]`

  OTP和eFuse需要使用配置表（config.json）进行数据配置后，通过BKFIL工具进行读写。

配置表包含以下配置区域：

 - 系统配置区
 - 区域总开关
 - 用户可配置区
 - eFuse配置区
 - 安全控制配置区
 - 安全数据配置区

系统配置
+++++++++++++++++++++

进行系统信息配置：

   ::

    "eFuse_totle_size":"0x20",
    "eFuse_VDD_BIT":"0x1F",
    "eFuse_vdd_address":"0x44880000",
    "eFuse_driver_base_address":"0x44880000",
    "OTP_base_address":"0x4B004000",

区域总开关
+++++++++++++++++++++

配置表中包含四个区域总开关，分别控制用户可操作区域、Efuse区域、安全控制区域、安全数据区域的开关，开关见下表：

   ::

    "User_Operate_Status": "true",       #定义“用户可配置区域” 为打开，即表示后面配置相应”用户可配置区域”均会执行，如果此处为false，即表示后面配置”用户可配置的区域”均不会执行；
    "Efuse_Status":"false",              #定义“Efuse区域” 为打开，即表示后面配置“Efuse区域”均会执行，如果此处为false，即表示后面配置”Efuse区域”均不会执行；
    "Security_ctrl_Status": "false",     #定义“安全控制区域” 为打开，即表示后面配置”安全控制区域”均会执行，如果此处为false，即表示后面配置”安全控制区域”均不会执行；
    "Security_data_Status": "false",     #定义“安全数据区域” 为打开，即表示后面配置”安全数据区域”均会执行，如果此处为false，即表示后面配置”安全数据区域”均不会执行；

用户可配置区参数说明
+++++++++++++++++++++++++

用户进行配置的区域，按照以下格式，根据需求配置来实现读写寄存器、OTP等操作；可多项配置对多个区域进行读写。

.. important::
    用户在该区域只能读写OTP的bank0，name应包含otp_前缀，start_addr取值范围为0x0--0x7FF，start_addr+byte_len应该小于0x7FF。

数据格式如下：

   ::

    "name": "otp_example1",              #定义当前区域的作用域，仅用于区分含义，不包含任何其他功能
    "mode": "write",                     #定义当前区域是读取数据，还是写入数据，参数为：read/write
    "start_addr":"0x0",                  #定义操作当前区域的起始位置
    "last_valid_addr":"0x7FF",           #定义操作当前区域用户可以安全设置的最大有效位置
    "byte_len":"5",                      #定义操作当前区域的长度，单位：BYTE
    "data_type":"ascii",                 #定义操作当前区域的数据类型，参数为：hex/ascii
    "data":"beken",                      #定义操作当前区域的数据内容，read 模式时，此处可以为空，write 时，此处如果为空，则默认不写入
    "encrypt":"false",                   #定义操作当前区域的数据是否需要加/解密，参数为：true/false，当data_type=ascii 时，如果encrypt=true，则会对data 的内容进行加/解密处理
    "status":"true"                      #定义操作当前区域是否需要执行，参数为：true/false

eFuse配置区参数说明
+++++++++++++++++++++++++

用于系统信息配置，数据格式如下：

   ::

    "name": "PASSWORD",                  #定义当前区域的作用域，仅用于区分含义，不包含任何其他功能
    "mode": "write",                     #定义当前区域是读取数据，还是写入数据，参数为：read/write
    "byte_addr": "0x00",                 #Efuse 大小以32BYTE 大小为例，当定义byte_address=0x00 时，则表示当前操作的是efuse 的第一个byte
    "last_valid_byte":"0x1D",            #定义操作当前区域用户可以安全设置的最大有效位置
    "length":"0x4",                      #定义操作当前区域的长度，单位：BYTE
    "data":"0x510fb093",                 #定义操作当前区域的数据内容，read 模式时，此处可以为空，write 时，此处如果为空，则默认不写入
    "status":"true"                      #定义操作当前区域是否需要执行，参数为：true/false

安全控制配置区参数说明
+++++++++++++++++++++++++

用于安全控制信息的配置，用户需根据安全需求进行配置。

   ::

    "disJtag":"31,7,0",                  #表示Byte31 中Bit[7] 置0，当前功能状态：1 关闭 JTAG 接口和 SPI2AHB 接口；0 使用 JTAG 接口和 SPI2AHB 接口
    "disSpiFlash":"31,6,0",              #表示Byte31 中Bit[6] 置0，当前功能状态：1 关闭 FLASH 的 SPI 下载模式；0 使能 FLASH 的 SPI 下载模式
    "enEncryptWord":"31,5,0",            #表示Byte31 中Bit[5] 置0，当前功能状态：1 开启 FLASH AES 加密；0 关闭 FLASH AES 加密
    "disReadEncryptWord":"31,4,0",       #表示Byte31 中Bit[4] 置0，当前功能状态：byte15:0 的读禁止（1）
    "disWriteEncryptWord":"31,3,0",      #表示Byte31 中Bit[3] 置0，当前功能状态：byte15:0 的写禁止（1）
    "disWriteUID":"31,2,0",              #表示Byte31 中Bit[2] 置0，当前功能状态：byte16:23 的写禁止（1）
    "disWriteMAC":"31,1,0",              #表示Byte31 中Bit[1] 置0，当前功能状态：byte24:29 的写禁止（1）
    "disWriteEfuse":"31,0,0",            #表示Byte31 中Bit[0] 置0，当前功能状态：eFUSE 所有 32byte 的写禁止（1）
    "security_boot_fastboot":"30,2,0"    #表示Byte30 中Bit[2] 置0，当前功能状态：fast boot 0: deepsleep fast boot 1: deepsleep sb boot
    "security_boot_log":"30,1,0",        #表示Byte30 中Bit[1] 置0，当前功能状态：Security Boot 打印功能  1: disable, 0: enable
    "security_boot":"30,0,0",            #表示Byte30 中Bit[0] 置0，当前功能状态：Security Boot enable   1: enable,  0: disable


安全数据配置区参数说明
+++++++++++++++++++++++++

用于安全密钥等关键数据的配置，包含验签的公钥、flash加密的密钥、OTA解密的密钥等，数据格式如下：

   ::

    "name": "boot_key0",                 #定义当前区域的作用域，仅用于区分含义，不包含任何其他功能
    "mode": "write",                     #定义当前区域是读取数据，还是写入数据，参数为：read/write
    "start_addr":"0x4B005000",           #定义操作当前区域的起始位置
    "last_valid_address":"0x4B004800",   #定义操作当前区域用户可以安全设置的最大有效位置
    "byte_len":"4",                      #定义操作当前区域的长度，单位：BYTE
    "data":"89c81a2c",                   #定义操作当前区域的数据内容，read 模式时，此处可以为空，write 时，此处如果为空，则默认不写入
    "data_type":"hex",                   #定义操作当前区域的数据类型，参数为：hex/ascii
    "encrypt":"false",                   #定义操作当前区域的数据是否需要加/解密，参数为：true/false，当data_type=ascii 时，如果encrypt=true，则会对data 的内容进行加/解密处理
    "status":"true"                      #定义操作当前区域是否需要执行，参数为：true/false