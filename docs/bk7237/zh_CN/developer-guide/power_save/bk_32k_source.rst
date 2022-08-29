32k的时钟源说明
-----------------------------------------------

32k的时钟源，有以下几种：

- rosc
 
优缺点:
功耗低，但是时钟精度相对来说不好，最优功耗时可以选用它。


- 外部26M分频出的时钟

优缺点：
精度好，功耗高相对高


- 外部32k时钟

优缺点：
精度好，功耗相对外部26M分频的时钟低


因此可以根据不同的业务和场景选用不同的32k时钟


切换32k时钟源函数接口说明
-----------------------------------------------

枚举类型：

typedef enum
{

	PM_LPO_SRC_DIVD, //32K from 26m

	PM_LPO_SRC_X32K,    //extern 32k

	PM_LPO_SRC_ROSC,    //32K from ROSC
	
	PM_LPO_SRC_DEFAULT  //32K from ROSC

}pm_lpo_src_e;
+++++++++++++++++++++++++++++++++++++++++++++++

-  设置32K时钟源函数接口：

bk_pm_lpo_src_set(pm_lpo_src_e lpo_src)


-  获取当前使用32K时钟源函数接口:

pm_lpo_src_e bk_pm_lpo_src_get()

:link_to_translation:`en:[English]`

