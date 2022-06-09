文档生成
============

:link_to_translation:`en:[English]`

概述
-----------------

文档有两种生成方式

 - API 头文件生成: 以 API 头文件作为输入，先由 Doxygen 生成 XML 文件, 然后由 sphinx 结合 breathe 插件进一步生成 HTML。
 - reStructuredText (RST) 格式生成，以 .rst 文档作为输入，通过 sphinx 生成 HTML 文档。


工具安装
-----------

以 linux ubuntu 为例，需要安装：

 - doxygen::

    sudo apt-get install doxygen

 - sphinx 与 sphinx_rtd_theme::

    pip3 install sphinx sphinx_rtd_theme

 - breathe::
 
    pip3 install breathe

.. _doc_gen_api_header:

API 头文件生成
--------------

以 bk_api_mod.h 为例说明文档生成步骤。

步骤一：完善头文件注释
************************


为生成头文件文档，需要按 Doxygen 要求编写注释，Doxygen 定义了多种格式，掌握常见的几种就能处理绝大多数情况。

 - 定义 group。为 bk_api_mod.h 定义一个新的 group bk_api_mod， 如果头文件中除了有 API 声明之外还有数据类型定义，
   可再定义多个子 group， 通常将所有枚举类型放在起定义一个子 group， 将所有结构体定义放一起定义一个 group， 分成多个子。如下示，
   group 的好处是在 HTML 显示时可以分开显示， 不然 API 文档与数字类型定义会串插显示。如下示，
   定义了三个 group: bk_api_mod, bk_api_mod_enums, bk_api_mod_structs::

      /**
       * @brief mod enum defines
       * @defgroup bk_api_mod_enum mod enums
       * @ingroup bk_api_mod
       * @{
       */
	  bk_api_mod.h 中所有枚举类型的定义放在这里
      /**
       * @}
       */
       
       /**
       * @brief mod struct defines
       * @defgroup bk_api_mod_structs structs in mod
       * @ingroup bk_api_mod
       * @{
       */
	  bk_api_mod.h 中所有结构体类型的定义放在这里
      /**
       * @}
       */

     /**
      * @brief mod API
      * @defgroup bk_api_mod mod API group
      * @{
      */
      bk_api_mod.h 中 API 声明放在这里
      /**
       * @}
       */

 - API 注释::

      /**
       * @brief     概述 API 功能，必要项
       *
       * 更多 API 功能介绍可放这里，可多行
       * 更多 API 功能介绍可放这里，可多行
       *
       * @attention 1. 如果有特别要注意的事项，可放在这里，可多个
       * @attention 2. 如果有特别要注意的事项，可放在这里，可多个
       *
       * @param     参数1  参数介绍
       * @param     参数2  参数介绍
       *   ...
       * @param     参数N  参数介绍
       *
       * @return
       *    - 返回值1: 返回值含义
       *    - 返回值2: 返回值含义
       */

 - 结构体注释如下， 枚举类型，宏定义等注释类似::
 
     typedef struct
     {
         type1 field1;     /**< field1 的注释 */
         type2 field2;     /**< field2 的注释 */
         ...
         typeN fieldN;     /**< fieldN 的注释 */
     } struct_type_t;

步骤二：将文档关联到文档树中
******************************

进到 docs/api-reference 中，为模块增加一个 mod.rst 文件，将 step1 中的 group 放到 mod.rst 中，group 放置的顺序即为 group 在 HTML 页面显示的顺序::

    ***********************************
    Mod API Reference
    ***********************************

    .. doxygengroup:: bk_api_mod
            :project: api_ref

    .. doxygengroup:: bk_api_mod_enums
            :project: api_ref

    .. doxygengroup:: bk_api_wifi_structs
            :project: api_ref
			
进到 docs/api-reference 目录中， 在 index.rst 中增加一行::
 
     Mod <mod>

上面仅介绍最常中的 doxygen 注释， 如果需要使用更高级的功能可参考： `Doxygen <https://www.doxygen.nl/manual/grouping.html>`_ 与 `Breathe <https://breathe.readthedocs.io/en/latest/index.html>`_

RST 文档生成
--------------------------

步骤一：编写 RST 文档
************************

对要编写的 RST 文档进行归类，目前 SDK RST 文档目录如下：

   - docs/get-startted: 快速入门文档
   - docs/develop-model: 开发流程，模式相关的文档
   - docs/app-develop: 应用程序开发相关的文档
   - docs/developer-guider: 开发者指南
   - docs/release-notes: 版本发布相关

确定文档要放置的目录之后，在该目录下新建 mod.rst。之后，按 reStructuredText 格式编写 mod.rst。

最常用的格式是：标题，列表，图片，引用。可打开一个已有的文档参参考，如需要用到高级功能，可参考 `reStructuredText <https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html>`_

步骤二：将 RST 文档关联到文档树中
*********************************

找到文档所在目录下 index.rst, 在其中增加一行::
 
    Mod <mod>

编译生成文档
-----------------

有两种方法可以生成 doc:

 - 在 docs 目录下运行 build_doc.py
 - 在项目目录下，运行 armino.py doc， 该方法仅在 FreeRTOS 有效

上述命令在当前目录下生成 build/html 目录， 可用浏览器打开 build/html/index.hmtl 查看文档效果。

.. note::

    默认情况下 build 系统不编译文档， 因此当修改了 API 头文件或者 RST 文档后，应该运行 build_doc.py 以确认是否引入错误。

