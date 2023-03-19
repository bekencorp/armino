Clock instructions
=============================================

:link_to_translation:`zh_CN:[中文]`

The system clock source includes the following types：

- ROSC
- DCO
- XTAL-32k
- XTAL-26M
- AUPLL
- DPLL

note：XTAL is external clock：

- XTAL-32k：external 32k；
- XTAL-26M: external 26M；

The system is equipped with an external 26M clock default. The external 32K clock can accorde to project requirements。

Clock Configuration
-----------------------------------------------
The 32k clock source has the following types：

- rosc

Advantages and disadvantages:
Low power consumption good，but the clock is relatively inaccurate，You can use it when you need the best power consumption。


- external 26M division 32k

Advantages and disadvantages：
Good precision, but higher power consumption


- external 32k

Advantages and disadvantages：
The precision is good, and the power consumption is lower than that of the external 26M division 32k clock


so you can select different 32k source clock based on owner using scenarios

Introduction to DPLL usage
-----------------------------------------------
DPLL is external 26M Frequency doubling(a.320M;b.480M)
++++++++++++++++++++++++++++++++++++++++++++++++++++++
- The working clock of WIFI is DPLL(480M)
- The working clock of BT is DPLL(480M)
- it can select different DPLL according to the specific scenarios
