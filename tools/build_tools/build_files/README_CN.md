# ARMINO 物联网开发框架

* [English Version](./README.md)

欢迎使用 Armino!
Armino 是**博通集成**推出的物联网开发框架，旨在支持**博通集成** 2022 年之后推出的各种物联网芯片，
同时兼容各种主流物联网操作系统，如 RT-Thread，AliOS，HarmoneyOS 等，Armino 默认支持 FreeRTOS。

# ARMINO 版本与芯片

下表总结了博通集成芯片在 Armino 各版本中的支持状态，其中 ![alt text][支持] 代表已支持，![alt text][开发中] 代表目前处于开发状态。
当芯片处于开发阶段时，芯片的构建会被关闭，一些重要的内容（如文档和技术规格书等）可能会缺失。请确保使用与芯片相匹配的 Armino 版本


|Branch\Chip   |    [BK7235]      |    [BK7256]      |      [BK7236]      |      [BK7258]      |      Comment            |
|:------------ |:---------------: |:---------------: |:-----------------: |:-----------------: |:----------------------: |
|main          |![alt text][支持] |![alt text][支持] |![alt text][开发中] |![alt text][开发中] |Develop Branch           |
|release/v1.5  |![alt text][支持] |![alt text][支持] |                    |                    |MP branch                |
|release/v1.5.1|![alt text][支持] |![alt text][支持] |                    |                    |MP branch for smart lock |


[支持]: https://img.shields.io/badge/-supported-green "支持"
[开发中]: https://img.shields.io/badge/-developing-orange "开发中"

==Tag说明，release分支Tag为正式版本，请使用正式发布版本，进行开发。==

main分支Tag为非量产Tag，仅用作版本标识，格式为DevXX.XX.XX.XX，例如：Dev2.0.0
release/xxxx分支Tag为可量产Tag，格式为vXX.XX.XX.XX，例如：v1.5.3 ，v1.5.1.1


# ARMINO 版本与其他物联网操作系统

下表总结了主流物联网操作系统在 Armino 各版本中的支持状态，其中 ![alt text][支持] 代表已支持，![alt text][开发中] 代表目前处于开发状态。

|OS           |        main           |
|:----------- |:---------------------: |
|FreeRTOS     | ![alt text][支持]       |
|LiteOS       | ![alt text][支持]      |
|RT-Thread    | ![alt text][开发中]      |
|AliOS        | ![alt text][开发中]      |

[支持]: https://img.shields.io/badge/-supported-green "支持"
[开发中]: https://img.shields.io/badge/-developing-orange "开发中"

# 快速入门

阅读[概述](TODO_link_to_armino_intro_doc）可让您对 Armino 有一个整体认识。 
阅读[快速入门](TODO_link_to_armino_get_started_doc) 可教您快速上手使用 Armino 进行编译与调试。

# 开发者社区

 - 报告问题 [Github Issue Forum](TODO_link_to_armino_github_forum)  
 - 寻求技术支持 [Cummunity-Forum](TODO_link_to_armino_forum)

# 更多资源

 - 文档: http://docs.bekencorp.com/
 - 代码: https://github.com/bekencorp/armino
 - 版本: TODO_link_to_release_page
 - 示例: TODO_link_to_doc_sample_and_demo_page
 - 下载中心: TODO_link_to_download_center_page
 - 博通集成产品: http://www.bekencorp.com/index/goods/product.html
 - 博通集成官网: http://www.bekencorp.com
