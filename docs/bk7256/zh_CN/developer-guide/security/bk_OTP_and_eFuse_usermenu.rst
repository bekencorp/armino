OTP和eFuse的配置及读写方法
==========================

:link_to_translation:`en:[English]`

OTP和eFuse需要使用配置表（example::download:`otp_efuse_config.json<../../../../common/_static/otp_efuse_config.json>`）进行数据配置后，通过BKFIL工具进行读写。

一、配置方法
+++++++++++++++++++++

配置表包含以下配置区域：

 - 区域总开关
 - 用户可配置区
 - eFuse配置区
 - 安全控制配置区
 - 安全数据配置区


区域总开关
---------------------

配置表中包含四个区域总开关，分别控制用户可操作区域、Efuse区域、安全控制区域、安全数据区域的开关，用户根据操作需要进行配置，开关定义见下表：

   ::

    "User_Operate_Enable":  "true",      #定义“用户可配置区域” 为true，即表示后面配置相应”用户可配置区域”均会执行，如果此处为false，即表示后面配置”用户可配置的区域”均不会执行；
    "Efuse_Enable":         "false",     #定义“Efuse区域” 为true，即表示后面配置“Efuse区域”均会执行，如果此处为false，即表示后面配置”Efuse区域”均不会执行；
    "Security_ctrl_Enable": "false",     #定义“安全控制区域” 为true，即表示后面配置”安全控制区域”均会执行，如果此处为false，即表示后面配置”安全控制区域”均不会执行；
    "Security_data_Enable": "false",     #定义“安全数据区域” 为true，即表示后面配置”安全数据区域”均会执行，如果此处为false，即表示后面配置”安全数据区域”均不会执行；

用户可配置区参数说明
---------------------

  用户可配置的区域，当User_Operate_Status设置为true时该配置区域生效。按照以下格式，根据需求配置实现OTP bank0读写操作，可多项配置对多个地址段进行读写。

.. important::
    - OTP的bank0供用户自由分配，地址范围为0x4B004000--0x4B0047FF，start_addr+byte_len应该小于0x4B0047FF;当在APP中读取OTP时，对应的地址范围为0x0--0x7FF。
    - 读写OTP时，name应包含otp_前缀，写入区域不能够重叠。

数据格式如下：

   ::

    "name":            "otp_example1",     #定义当前区域的作用域，仅用于区分含义，不包含任何其他功能
    "mode":            "write",            #定义当前区域是读取数据，还是写入数据，参数为：read/write
    "start_addr":      "0x4B004000",       #定义操作当前区域的起始位置
    "last_valid_addr": "0x4B0047FF",       #定义操作当前区域用户可以安全设置的最大有效位置
    "byte_len":        "0x5",              #定义操作当前区域的长度，单位：BYTE
    "data_type":       "ascii",            #定义操作当前区域的数据类型，参数为：hex/ascii
    "data":            "beken",            #定义操作当前区域的数据内容，read 模式时，此处可以为空，write 时，此处如果为空，则默认不写入
    "status":          "true"              #定义操作当前区域是否需要执行，参数为：true/false

example：

   ::

    "User_Operate":
    [
        {
            "name":                "OTP_example1",    //OTP从0x4B004000开始写入8字节数据"0x0123456786666666"
            "mode":                "write",
            "start_addr":          "0x4B004000",
            "last_valid_addr":     "0x4B0047FF",
            "byte_len":            "0x8",
            "data_type":           "hex",
            "data":                "0123456786666666",
            "status":              "true"
        },
        {
            "name":                "OTP_example2",   //OTP从0x4B004008开始写入8字节数据"0x0123456789ABCDEF"
            "mode":                "write",
            "start_addr":          "0x4B004008",
            "last_valid_addr":     "0x4B0047FF",
            "byte_len":            "0x8",
            "data_type":           "hex",
            "data":                "0123456789ABCDEF",
            "status":              "true"
        },
        {
            "name":                "OTP_example3",   //OTP从0x4B004000开始读取16字节数据
            "mode":                "read",
            "start_addr":          "0x4B004000",
            "last_valid_addr":     "0x4B0047FF",
            "byte_len":            "0x10",
            "data":                "",
            "data_type":           "hex",
            "status":              "true"
        }
    ],

eFuse配置区参数说明
---------------------

用于系统信息配置，默认值为0，该区域不需要用户配置。当Efuse_Status设置为true时该配置区域生效，数据格式如下：

   ::

    "name":            "PASSWORD_1",      #定义当前区域的作用域，仅用于区分含义，不包含任何其他功能
    "mode":            "read",            #定义当前区域是读取数据，还是写入数据，参数为：read/write
    "byte_addr":       "0x00",            #Efuse 大小以32BYTE 大小为例，当定义byte_addr=0x00 时，则表示当前操作的是efuse 的第一个byte
    "last_valid_byte": "0x1F",            #定义操作当前区域用户可以安全设置的最大有效位置
    "length":          "0x20",            #定义操作当前区域的长度，单位：BYTE
    "data":            "",                #定义操作当前区域的数据内容，read 模式时，此处可以为空，write 时，此处如果为空，则默认不写入
    "status":          "false"            #定义操作当前区域是否需要执行，参数为：true/false

example：

   ::

    "eFuse":
    [
        {
            "name":            "example1",  //从eFuse的0x0地址写入1字节数据"0x93"
            "mode":            "write",
            "byte_addr":       "0x00",
            "last_valid_byte": "0x1F",
            "length":          "0x01",
            "data":            "0x93",
            "status":          "false"
        },
        {
            "name":            "example2",  //从eFuse的0x1地址连续写入4字节数据"0x510fb000"
            "mode":            "write",
            "byte_addr":       "0x01",
            "last_valid_byte": "0x1F",
            "length":          "0x04",
            "data":            "0x510fb000",
            "status":          "false"
        },
        {
            "name":            "example3",   ////从eFuse的0x0地址连续读取32字节数据
            "mode":            "read",
            "byte_addr":       "0x00",
            "last_valid_byte": "0x1F",
            "length":          "0x20",
            "data":            "",
            "status":          "false"
        }
    ],

安全控制配置区参数说明
-------------------------

用于安全控制信息的配置，默认值为0。用户有安全需求时，需根据安全需求进行配置。当Security_ctrl_Status设置为true时该配置区域生效，数据格式如下：

   ::

    "disJtag":               "31,7,0",   #表示Byte31 中Bit[7] 置0，当前功能状态：1 关闭 JTAG 接口和 SPI2AHB 接口；0 使用 JTAG 接口和 SPI2AHB 接口
    "disSpiFlash":           "31,6,0",   #表示Byte31 中Bit[6] 置0，当前功能状态：1 关闭 FLASH 的 SPI 下载模式；   0 使能 FLASH 的 SPI 下载模式
    "enEncryptWord":         "31,5,0",   #表示Byte31 中Bit[5] 置0，当前功能状态：1 开启 FLASH AES 加密；          0 关闭 FLASH AES 加密
    "disReadEncryptWord":    "31,4,0",   #表示Byte31 中Bit[4] 置0，当前功能状态：1 byte0:15的读禁止
    "disWriteEncryptWord":   "31,3,0",   #表示Byte31 中Bit[3] 置0，当前功能状态：1 byte0:15的写禁止
    "disWriteUID":           "31,2,0",   #表示Byte31 中Bit[2] 置0，当前功能状态：1 byte16:23的写禁止
    "disWriteMAC":           "31,1,0",   #表示Byte31 中Bit[1] 置0，当前功能状态：1 byte24:29的写禁止
    "disWriteEfuse":         "31,0,0",   #表示Byte31 中Bit[0] 置0，当前功能状态：1 eFUSE 所有 32byte 的写禁止
    "security_boot_fastboot":"30,2,0",   #表示Byte30 中Bit[2] 置0，当前功能状态：fast boot               1: deepsleep sb boot 0: deepsleep fast boot
    "security_boot_log":     "30,1,0",   #表示Byte30 中Bit[1] 置0，当前功能状态：Security Boot 打印功能   1: disable, 0: enable
    "security_boot":         "30,0,0"    #表示Byte30 中Bit[0] 置0，当前功能状态：Security Boot enable    1: enable,  0: disable

.. important::
    - security_boot和enEncryptWord位配置生效前必须先配置安全数据配置区和烧写支持安全的bin镜像。

安全数据配置区参数说明
-------------------------

用于安全密钥等关键数据的配置，包含验签的公钥、flash加密的密钥、OTA解密的密钥等。当Security_data_Status设置为true时该配置区域生效，数据格式如下：

   ::

    "name":              "OTA_IV",     #定义当前区域的作用域，仅用于区分含义，不包含任何其他功能
    "mode":              "read",       #定义当前区域是读取数据，还是写入数据，参数为：read/write
    "start_addr":        "0x4B004838", #定义操作当前区域的起始位置
    "last_valid_addr":   "0x4B004848", #定义操作当前区域用户可以安全设置的最大有效位置
    "byte_len":          "0x10",       #定义操作当前区域的长度，单位：BYTE
    "data":              "",           #定义操作当前区域的数据内容，read 模式时，此处可以为空，write 时，此处如果为空，则默认不写入
    "data_type":         "ascii",      #定义操作当前区域的数据类型，参数为：hex/ascii
    "status":            "false"       #定义操作当前区域是否需要执行，参数为：true/false

.. important::
    - 安全数据配置中，name、start_addr、last_valid_addr、byte_len、data_type等各项配置已经固定，用户不能配置。
    - OTA_key、OTA_IV和aes_key的data部分由密钥生成工具生成密文，data_type固定为"ascii"；
    - bootloader_public_key和bootrom_public_key由签名工具生成，data_type固定为"hex"。

example：

   ::

    "Security_data":
    [
        {
            "name":              "OTA_key",     //写入加密后的OTA key
            "mode":              "write",
            "start_addr":        "0x4B004810",
            "last_valid_addr":   "0x4B004FFF",
            "byte_len":          "0x20",
            "data":              "bzbybxcdbwbxbybzcjcicqctcrcocpcmcdcccbcacicjckclbwbwcucxcwdadbcy",
            "data_type":         "ascii",
            "status":            "false"
        },
        {
            "name":              "OTA_key",     //读取加密后的OTA key
            "mode":              "read",
            "start_addr":        "0x4B004810",
            "last_valid_addr":   "0x4B004FFF",
            "byte_len":          "0x20",
            "data":              "",
            "data_type":         "ascii",
            "status":            "false"
        },
        {
            "name":              "OTA_IV",    //写入加密后的OTA IV
            "mode":              "write",
            "start_addr":        "0x4B004838",
            "last_valid_addr":   "0x4B004848",
            "byte_len":          "0x10",
            "data":              "bzbybxcdbwbxbybzcjcicqctcrcocpcm",
            "data_type":         "ascii",
            "status":            "false"
        },
        {
            "name":              "OTA_IV",     //读取加密后的OTA IV
            "mode":              "read",
            "start_addr":        "0x4B004838",
            "last_valid_addr":   "0x4B004848",
            "byte_len":          "0x10",
            "data":              "",
            "data_type":         "ascii",
            "status":            "false"
        },
        {
            "name":              "bootloader_public_key",  //写入bootlaoder对app验签的公钥
            "mode":              "write",
            "start_addr":        "0x4B0048B0",
            "last_valid_addr":   "0x4B00491F",
            "byte_len":          "0x60",
            "data":              "5AE5AD1FBDF341B9DF03C081D97194DA16344B620F502B452207DB39710F5E815181B8E77B9E572C0A003AC3E50C00B7458A6564DD5C76B3AFD786C9D0C0620687709231C982F0AE2179E1502DD9C40928151FDE71724D3C5B8219473C475EB9",
            "data_type":         "hex",
            "status":            "false"
        },
        {
            "name":              "bootloader_public_key",
            "mode":              "read",
            "start_addr":        "0x4B0048B0",
            "last_valid_addr":   "0x4B004920",
            "byte_len":          "0x60",
            "data":              "",
            "data_type":         "hex",
            "status":            "false"
        },
        {
            "name":              "bootrom_public_key",   //写入bootrom对bootlaoder验签的公钥
            "mode":              "write",
            "start_addr":        "0x4B005000",
            "last_valid_addr":   "0x4B00505F",
            "byte_len":          "0x60",
            "data":              "5AE5AD1FBDF341B9DF03C081D97194DA16344B620F502B452207DB39710F5E815181B8E77B9E572C0A003AC3E50C00B7458A6564DD5C76B3AFD786C9D0C0620687709231C982F0AE2179E1502DD9C40928151FDE71724D3C5B8219473C475EB9",
            "data_type":         "hex",
            "status":            "false"
        },
        {
            "name":              "bootrom_public_key",
            "mode":              "read",
            "start_addr":        "0x4B005000",
            "last_valid_addr":   "0x4B00505F",
            "byte_len":          "0x60",
            "data":              "",
            "data_type":         "hex",
            "status":            "false"
        },
        {
            "name":              "aes_key",           //写入加密后的用于flash加密的密钥
            "mode":              "write",
            "start_addr":        "0x4B0057E0",
            "last_valid_addr":   "0x4B0057FF",
            "byte_len":          "0x20",
            "data":              "cbdsbxcdcjbxdtbycdcibxduckbycabxdwckbwegbxefcfeccleackcfcebyegeaebcbchcacfedcbeccjcecfegeebgblejbgbpeleibmbgepbjbheobnbnbmekeibt",
            "data_type":         "ascii",
            "status":            "false"
        },
        {
            "name":              "aes_key",
            "mode":              "read",
            "start_addr":        "0x4B0057E0",
            "last_valid_addr":   "0x4B0057FF",
            "byte_len":          "0x20",
            "data":              "",
            "data_type":         "ascii",
            "status":            "false"
        }
    ]

二、读写步骤
+++++++++++++++++++++
 - 1.BKFIL工具选择配置界面
 - 2.勾选"OTP"或者"烧录eFuse"选项
 - 3.选择"eFuse密钥"文件选择按钮
 - 4.选择文件类型为"json file",选择配置好的config.json文件
 - 5.BKFIL工具选择主界面，点击"烧录"按钮

.. figure:: ../../../../common/_static/BKFIL_RW_OTP&eFuse.png
    :align: center
    :alt: BKFIL download step
    :figclass: align-center

    BKFIL读写OTP、eFuse步骤

三、常见问题分析
+++++++++++++++++++++

 - 1. json格式错误

example：

   ::

    "eFuse":
    [
        {
            "name":            "example1",
            "mode":            "write",
            "byte_addr":       "0x00",
            "last_valid_byte": "0x1F",
            "length":          "0x01",
            "data":            "0x93",
            "status":          "false"
        },
        {
            "name":            "example2",
            "mode":            "read",
            "byte_addr":       "0x00",
            "last_valid_byte": "0x1F",
            "length":          "0x20",
            "data":            "",
            "status":          "false"
        },                                   //Error: 最后一个子节点不应加逗号
    ],

 - 2. 模块开关未开启

  操作失败时，请检查区域总开关和操作模块的“status”开关是否打开。