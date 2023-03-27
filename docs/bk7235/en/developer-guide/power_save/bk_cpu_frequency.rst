CPU frequency instructions
======================================

:link_to_translation:`zh_CN:[中文]`

The CPU has the following clock sources：

- 26M
- DCO
- 320M
- 480M


note：
The CPU frequency can reach up to 320M; The system cannot run up to 480M，240M can be used through frequency division.。

CPU frequency switch description
-----------------------------------------------
- It can dynamically switch the cpu frequency according to the business , in order to achieve the optimal power consumption.

    The CPU main frequency switching is relatively complex, which is implemented internally by SDK

- The maximum CPU frequency is 320M


