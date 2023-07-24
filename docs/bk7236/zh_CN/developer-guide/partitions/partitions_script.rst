分区配置表
========================

.. note::

  本节中 `BL1` 特指固化在芯片中的 BootROM，`BL2` 特指 Bootloader，即 mcuboot。
  `未配置` 特指该项在分区配置表中不存在。

BK7236 通过 JSON 文件来配置分区，本节介绍各配置字段的含义，限制及注意事项。


配置示例
--------------------------

一个典型的分区配置如下图所示。

.. code::

 {
    "description":   "Secureboot: bl1->bl2->test1->test2",
    "flash_size":    "16M",
    "pack_app":      "default",
    "pack_ota":      "secondary_s, secondary_ns",
    "boot_mode":     "secureboot",
    "aes_key":       "73c7bf397f2ad6bf4e7403a7b965dc5ce0645df039c2d69c814ffb403183fb18",

    "secureboot": {

        "bl1_key_type":  "ec256",
		"bl1_pubkey":    "bl2_ec256_pubkey.pem",
		"bl1_privkey":   "bl2_ec256_privkey.pem",
        "bl2_key_type":  "ec256",
		"bl2_pubkey":    "bl2_ec256_pubkey.pem",
		"bl2_privkey":   "bl2_ec256_privkey.pem",
        "bl2_slot_type": "single"
	},

    "partitions": [
    {
        "partition": "bl1_control",
        "size":      "12k",
        "bin":       "bl1_control.bin",
        "bin_type":  "data"
    },
    {
        "partition": "primary_manifest",
        "size":      "4k",
        "bin":       "primary_manifest.bin",
        "bin_type":  "data"
    },
    {
        "partition": "secondary_manifest",
        "size":      "4k",
        "bin":       "secondary_manifest.bin",
        "bin_type":  "data"
    },
    {
        "partition": "primary_bl2",
        "size":      "128k",
        "bin":       "bl2.bin",
        "version":   "0x00000004",
        "verifier":  "bl1"
    },
    {
        "partition": "secondary_bl2",
        "size":      "128k",
        "bin":       "bl2.bin",
        "version":   "0x00000000",
        "verifier":  "bl1"
    },
    {
        "partition": "primary_s",
        "size":      "32k",
        "bin":       "test1.bin",
        "verifier":  "bl2"
    },
    {
        "partition": "primary_ns",
        "size":      "32k",
        "bin":       "test2.bin",
        "verifier":  "bl2"
    },

    {
        "partition": "secondary_s",
        "size":      "32k",
        "bin":       "test1.bin",
        "verifier":  "bl2"
    },
    {
        "partition": "secondary_ns",
        "size":      "32k",
        "bin":       "test2.bin",
        "verifier":  "bl2"
    }
    ]
 }

全局配置字段
----------------------------

 - ``description`` - 可选项。分区表描述。
 - ``flash_size`` - 可选项。默认值为 4M FLASH，当实际 FLASH 不为 4M 时，需指定 ``flash_size``。
   系统区，如 ``sys_rf``, ``sys_net`` 等放置在 FLASH 的尾部，构建系统会依据 ``flash_size`` 生成
   FLASH 尾部分区的 ``offset``。
 - ``pack_app`` - 可选项。与 pack_ota 类似，用于指定 app.bin 由哪些分区生成。

   - 未配置时表不生成 apps.bin。
   - 配置成 "default" 时，会将所有不是以 secondary 为前缀，且配置了 `bin` 的分区打包。
   - 配置成分区列表时，则将列表中的分区打包，如 "primary_bl2, primary_ns"
     则打包工具会使用 ``secondary_bl2`` 与 ``secondary_ns`` 两个分区生成 app.bin。
 - ``pack_ota`` - 可选项。与 pack_app 类似，用于指定 ota.bin 由哪些分区生成。
 
   - 未配置时表不生成 ota.bin。
   - 配置成 "default" 时，会将所有以 secondary 为前缀，且配置了 ``bin`` 的分区打包。
   - 配置成分区列表时，则将列表中的分区打包，如 "secondary_bl2, secondary_ns"
     则打包工具会使用 ``secondary_bl2`` 与 ``secondary_ns`` 两个分区生成 ota.bin。

 - ``boot_mode`` - 可选项。默认为 ``legacyboot``。当为安全启动时必须配置为 ``secureboot``。

   - 合法配置值为 ``legacyboot`` 或 ``secureboot``。其中 ``legacyboot`` 表示下载模式，``secureboot`` 表示安全启动模式。
   - 该字段主要用于选择系统配置表中的分区，如果配置表中分区的 ``owner`` 与 ``boot_mode`` 匹配，
     则相应分区在该模式下生效。通常用户不需要关注该字段，除非需要更改系统分区配置。
 - ``aes_key`` - 可选项。
 
   - 未配置时不会对 code 分区加密。
   - 字段配置时表示 FLASH AES KEY，打包工具会使用该字段对 code 分区进行加密。
   - 长度必须为 64 字节。

 - ``partitions`` - 必选项。配置具体的分区，至少必须包含一个分区。详情参考单个分区配置。


安全启动配置
----------------------------

安全启动配置主要用于配置 BL1 与 BL2 启动相关的信息:

 - ``bl1_key_type`` - 可选项。默认使用 ec256。配置 BL1 签名的公钥类型，可选值为 ec256，ec521，rsa1024，rsa2048。
 - ``bl1_pubkey`` - 必选项。BL1 公钥文件。
 - ``bl1_privkey`` - 必选项。BL1 私钥文件。构建工具会使用私钥对 BL2 进行签名。
 - ``bl2_key_type`` - 可选项。配置使用 ec256。配置 BL1 签名的公钥类型，可选值为 ec256，rsa1024，rsa2048。
 - ``bl2_pubkey`` - 必选项。BL2 公钥文件。
 - ``bl2_privkey`` - 必选项。BL2 私钥文件。构建工具会使用私钥对 TF-M 与 CPU0 IMAGE 进行签名。
 - ``bl2_slot_type`` - 可选项。默认为 ``multiple``。
 
   - 本配置项仅与 BL2 相关，当 bl2 不存在时，可忽略该项。
   - 合法配置值为 ``single`` 或 ``multiple``。为 ``multiple`` 时，打包工具会对以 BL2 验签的每个
     分区独立加签。为 ``single`` 时，则是将 BL2 验签的所有分区合并成一个大分区之后再加签。为加快
     验签速度，建议配置成 ``single``。


具体分区配置
----------------------------

partitions.json 中的 ``partitions`` 用于配置分区列表，其中每一项为一个具体分区:

 - ``partition`` - 必选项。表示分区名字。

   - 分区名字必须唯一。构建系统会依据分区名字来生成一组宏，只有分区名字唯一才能确保生成宏
     不会重复定义。
   - 分区名前缀:

     - primary 前缀表示 OTA 升级时对应的主分区。当应用中要新增一个 OTA 升级的主分区时，应该以 primary 为前缀。
     - secondary 前缀表示 OTA 升级时对应的次分区。当应用中要新增一个 OTA 升级的次分区时，应该以 primary 为前缀。
     - sys 前缀表示系统分区。新增系统分区时，应该以 sys 为前缀。
   - 保留分区名，保留分区名仅能用着指定用途。当使用到保留分区时，该分区必须存在：
   
     - primary_cpu0_app 与 secondary_cpu0_app - 构建 app.bin 时会使用这两个分区生成的宏进行链接。
     - primary_bl2 与 secondary_bl2 - 构建 bl2.bin 时会使用到这两个分区生成的宏进行链接。
     - primary_tfm_s 与 secondary_tfm_s - 构建 tfm_s.bin 时会使用到这两个分区生成的宏进行链接。
     - primary_tfm_ns 与 secondary_tfm_ns - 构建 tfm_ns.bin 时会使用到这两个分区生成的宏进行链接。
     - sys_rf - 生成 RF 校准相关的宏，RF 校准组件会用到这个宏。
     - sys_net - 生成 NET 配置相关的宏，Wi-Fi/TCPIP 会用到这个分区的生成宏。
 - ``offset`` - 可选项。表示分区在 FLASH 的物理偏移量。
 
   - 未配置 offset 时，``offset`` 取值为上一个分区结束地址。
   - 允许相邻两个分区地址之间存在空洞，但不允许出现重叠。
   - 一般不建议配置 offset，除非要在特定的，不连续的位置放置一个分区时，才需要配置 offset。
   - offset 对齐需求:
   
     - 任何分区必须 4K 对齐。
     - S/NS 交界处相信的两个分区必须 68K 对齐，否则 MPC 配置时会出现一个 S/NS block 横跨两个分区现象。
 - ``size`` - 必选项。表示分区大小，单位 k/K 表示 Kbytes，m/M 表示 Mbytes，通常建议 4K 对齐。
 - ``bin`` - 可选项。可为数据或者代码。
 - ``bin_type`` - 可选项，默认类型为 "code"。
 
   - 若为 "data"，则表示 bin 为数据，工具不会对 bin 进行额外处理，如加 CRC 等。
   - 若为 "code"，则表示 bin 为代码，工具会对 bin 加 CRC，如果配置了 ``aes_key``，还会进行 AES 加密。
   - 基本原则是，如果 bin 中仅包含数据，则配置成 "data"，否则配置成 "code"。
 - ``version`` - 可选项。表示 bin 的版本号。
 
   - 版本号主要用在支持 OTA 升级的分区中。如果分区不需要支持 OTA 升级，则无需配置版本号。
   - 版本号为十六进制数字，数字值表示版本号，安全启动验签通过后会对比 BIN 的版本号与 OTP 中配置版本号，
     仅当 BIN 中的版本号不小于配置版本号时才允许运行该 BIN。
   - app/ota 打包时会用它生成包头。
 - ``verifier`` - 可选项。仅当 ``boot_mode`` 为 ``secureboot`` 时生效，表示该分区由谁来验签。
   可配置成 ``bl1`` 或 ``bl2``。``bl1`` 表示由 BL1 验签，打包工具会按 BL1 的要求进行加签。
   ``bl2`` 表示由 BL2 验签，打包工具会按 BL2 要求进行加签。
 - ``owner`` - 可选项。一般仅用在配置系统分区时。为 ``legacyboot`` 时表示该分区仅在 ``boot_mode`` 为 ``legacyboot``
   时生效。为 ``secureboot`` 时，表示改分区仅在 ``boot_mode`` 为 ``secureboot`` 时生效。


