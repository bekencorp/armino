config 32k clock source description
-----------------------------------------------

32k clock source：

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


switch 32k source clock interface instructions
--------------------------------------------------------

enum type：

typedef enum
{

	PM_LPO_SRC_DIVD, //32K from 26m

	PM_LPO_SRC_X32K,    //extern 32k

	PM_LPO_SRC_ROSC,    //32K from ROSC
	
	PM_LPO_SRC_DEFAULT  //32K from ROSC

}pm_lpo_src_e;
+++++++++++++++++++++++++++++++++++++++++++++++

-  set and select 32k source ：

bk_pm_lpo_src_set(pm_lpo_src_e lpo_src)


-  get the current 32k source:

pm_lpo_src_e bk_pm_lpo_src_get()

:link_to_translation:`en:[English]`

