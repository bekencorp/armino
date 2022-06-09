用例编写
========================

:link_to_translation:`en:[English]`

$ARMINO_PATH/examples 目录下存放 ARMINO 用例，用例用于:

 - 展示 ARMINO API 的用法， 如 WiFi API 等
 - ARMINO 关键模块的用法，如 Kconfig 配置， Backtrace 查看， 编译等

每个用例可以独立地进行编译与测试。

何时编写用例？
-------------------------

下列情况需要增加用例：

 - 单个 API 较为复杂时，如 WiFi 配置 bk_wifi_sta_set_config() 等
 - 使用某个功能涉及一组相关 API 时
 - ARMINO 用户容易出错的 API
 - ARMINO 用户经常使用的功能，如 Debug, 编译， CLI 等
 - ARMINO 用户明确提出需求时

.. note::

    当一个用例是说明 ARMINO API 或者功能时， 就应该放在 $ARMINO_PATH/examples 中， 而非放在其他位置.

用例编写规范
-------------------------

用例编写规范如下：

 - 一个用例只说明一个功能，或者一组强相关 API
 - 用例尽量简单
 - 同一功能不要重复编写用例
 - 定义 main() 函数的文件命名成 example_main.c, 如定义 hello world 用例，
   则命名成 hello_world_main.c, 不要命名成 main.c，因为过多 main.c 不便于 IDE 工具搜索
 - 用例 Kconfig 中的配置建议以 EXAMPLE 开头，避免与 SDK 其他配置生命，方便 IDE 工具搜索
 - 每个用例都需要文档，即 README.md 文件， 未来会通过 README.md 自动生成用例文档



