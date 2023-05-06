# ARMINO Development Framework

* [中文版](./README_CN.md)

Welcome to Armino!
ARMINO is the AIoT development framework aimed to support Beken SoCs developed since 2022, also it's aimed to support
all popular IoT OS, such as RT-Thread, AliOS, LiteOS etc.

# ARMINO Release and Supported SoCs

The following table shows Armino support of Beken SoCs where ![alt text][developing] and ![alt text][supported]
denote developing status and support, respectively. In developing status the build is not yet enabled and some
crucial parts could be missing(like documentation, datasheet). Please use an Armino release where the desired
SoC is already supported.


|Branch\Chip   |    [BK7235]           |       [BK7256]        |        [BK7236]        |      [BK7258]          |      Comment            |
|:------------ |:--------------------: |:--------------------: |:---------------------: |:---------------------: |:----------------------: |
|main          |![alt text][supported] |![alt text][supported] |![alt text][developing] |![alt text][developing] |Develop Branch           |
|release/v1.5  |![alt text][supported] |![alt text][supported] |                        |                        |MP branch                |
|release/v1.5.1|![alt text][supported] |![alt text][supported] |                        |                        |MP branch for smart lock |


[supported]: https://img.shields.io/badge/-supported-green "supported"
[developing]: https://img.shields.io/badge/-developing-orange "developing"

==Tag version , please use the official release version for development.==

main branch Tag is not official release, the format is DevXX.XX.XX.XX , for example: Dev2.0.0 
release/xxxx Branch Tag is official release, the format is vXX.XX.XX.XX，for example: v1.5.3 ，v1.5.1.1

Beken SoCs released before 2022, such as BK7231N, BK7231U, BK7251 etc, are not supported by ARMINO.

# ARMINO Release and OS

The following table shows Armino support of Popular IoT OS where ![alt text][developing] and ![alt text][supported]
denote developing status and support, respectively.

|OS           |         main           |
|:----------- |:---------------------: |
|FreeRTOS     | ![alt text][supported] |
|LiteOS       | ![alt text][supported]|
|RT-Thread    | ![alt text][developing]|
|AliOS        | ![alt text][developing]|

[supported]: https://img.shields.io/badge/-supported-green "supported"
[developing]: https://img.shields.io/badge/-developing-orange "developing"

# Getting Started

See the [Introduction](TODO_link_to_armino_intro_doc) to Armino for a high-level overview, 
and the documentation's [Introduction](TODO_link_to_armino_get_started_doc) to start developing.

# Community Support

 - Report bug or request new feature on [Github Issue Forum](TODO_link_to_armino_github_forum)  
 - Ask for support or technical discussion on [Cummunity-Forum](TODO_link_to_armino_forum)

# Resource

Here's a quick summary of resources to help you find your way around:

 - Documentation: http://docs.bekencorp.com

 - Source Code: https://github.com/bekencorp/armino is the main repository
 - Releases: TODO_link_to_release_page
 - Samples and example code: TODO_link_to_doc_sample_and_demo_page
 - Resource download center: TODO_link_to_download_center_page
 - Beken Products: http://www.bekencorp.com/index/goods/product.html to gain overview about different Beken products
 - Beken Home Page: http://www.bekencorp.com to find more resources
