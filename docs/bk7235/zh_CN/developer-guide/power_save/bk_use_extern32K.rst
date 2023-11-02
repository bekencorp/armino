使用外部32K
=======================

:link_to_translation:`en:[English]`

开启外部32K时钟源方法
-----------------------------------------------

-  确保硬件已经焊接了外部32K时钟源
-  在middleware/soc/bk72xx/bk7256.defconfig中设置CONFIG_EXTERN_32K=y

.. important::
    当本地工程应用于其他项目时，为避免定义外部32K影响该工程，请新建工程，将CONFIG_EXTERN_32K定义在根目录下projects的新建工程的config/bk72xx.config中。
    新建工程方法参考： :doc:`../porting/index`

使用外部32K时钟源注意事项
-----------------------------------------------

	因为外部32K引脚和DVP camera复用，当camera和外部32K同时使用时，为避免camera对外部32K干扰，需要按照以下顺序操作：

-  camera init：  camera上电之前先将32K时钟源切换到PM_LPO_SRC_DIVD
-  camera deinit：camera下电之后再将32K时钟源切换到PM_LPO_SRC_X32K

.. important::
	使用外部32K时，如果camera与其他模块共用ldo电源，在deinit时必须所有使用该ldo电源的模块关闭之后再切换32K时钟源到PM_LPO_SRC_X32K(例如在camera deinit中不切换为外部32K时钟源，在该LDO电源关闭后再切换32K时钟源)。