document generation
===================

:link_to_translation:`en:[English]`

Overview
--------

There are two ways to generate documents

 - API header file generation: Take the API header file as input, first generate the XML file by Doxygen, and then further generate HTML by sphinx combined with the breathe plugin.
 - Generated in reStructuredText (RST) format, takes .rst document as input, and generates HTML document through sphinx.


Tool installation
-----------------

Take linux ubuntu as an example, you need to install:

 - doxygen::

    sudo apt-get install doxygen

 - sphinx and sphinx_rtd_theme::

    pip3 install sphinx sphinx_rtd_theme

 - breathe::

    pip3 install breathe

.. _doc_gen_api_header:

API header file generation
--------------------------

Take bk_api_mod.h as an example to illustrate the document generation steps.

Step 1: Improve header file comments
************************************


In order to generate the header file document, it is necessary to write comments according to the requirements of Doxygen. Doxygen defines a variety of formats, and mastering the common ones can handle most cases.

 - Define groups. Define a new group bk_api_mod for bk_api_mod.h. If there are data type definitions in the header file in addition to API declarations, multiple subgroups can be defined. Usually, all enumeration types are put together to define a subgroup, and all enumeration types are defined in a subgroup. The structure definitions are put together to define a group, which is divided into multiple children. As shown below, the advantage of group is that it can be displayed separately when HTML is displayed, otherwise the API document and the numeric type definition will be displayed in series. As shown below, three groups are defined: bk_api_mod, bk_api_mod_enums, bk_api_mod_structs::

      /**
       * @brief mod enum defines
       * @defgroup bk_api_mod_enum mod enums
       * @ingroup bk_api_mod
       * @{
       */
	  The definitions of all enum types in bk_api_mod.h are placed here
      /**
       * @}
       */

       /**
       * @brief mod struct defines
       * @defgroup bk_api_mod_structs structs in mod
       * @ingroup bk_api_mod
       * @{
       */
	  The definitions of all structure types in bk_api_mod.h are placed here
      /**
       * @}
       */

     /**
      * @brief mod API
      * @defgroup bk_api_mod mod API group
      * @{
      */
      The API declaration in bk_api_mod.h is placed here
      /**
       * @}
       */

 - API Notes::

      /**
       * @brief     Overview API features, essentials
       *
       * More API function introduction can be put here, can be multi-line
       * More API function introduction can be put here, can be multi-line
       *
       * @attention 1. If there are special matters to pay attention to, you can put them here, you can have more than one
       * @attention 2. If there are special matters to pay attention to, you can put them here, you can have more than one
       *
       * @param     Parameter 1 Parameter introduction
       * @param     Parameter 2 Parameter introduction
       *   ...
       * @param     Parameter N parameter introduction
       *
       * @return
       *    - Return value 1: Meaning of return value
       *    - Return value 2: Return value meaning
       */

 - Structure comments are as follows, enumeration types, macro definitions and other comments are similar::

     typedef struct
     {
         type1 field1;     /**< Comments for field1 */
         type2 field2;     /**< Comments for field2 */
         ...
         typeN fieldN;     /**< Comments for fieldN */
     } struct_type_t;

Step 2: Associate the document to the document tree
***************************************************

Go to docs/api-reference, add a mod.rst file for the module, put the group in step1 into mod.rst, and the order in which the groups are placed is the order in which the groups are displayed on the HTML page::

    ***********************************
    Mod API Reference
    ***********************************

    .. doxygengroup:: bk_api_mod
            :project: api_ref

    .. doxygengroup:: bk_api_mod_enums
            :project: api_ref

    .. doxygengroup:: bk_api_wifi_structs
            :project: api_ref

Go to the docs/api-reference directory and add a line to index.rst::

     Mod <mod>

The above only introduces the most common doxygen annotations, If you need to use more advanced functions, please refer to： `Doxygen <https://www.doxygen.nl/manual/grouping.html>`_ and `Breathe <https://breathe.readthedocs.io/en/latest/index.html>`_

RST document generation
-----------------------

Step 1: Write the RST document
******************************

Categorize the RST documents to be written. Currently, the SDK RST document directory is as follows:

   - docs/get-startted: Quickstart documentation
   - docs/develop-model: development process, model related documentation
   - docs/app-develop: Documentation related to application development
   - docs/developer-guider: developer guide
   - docs/release-notes: release notes

After determining the directory where the document will be placed, create a new mod.rst in that directory. After that, write mod.rst in reStructuredText format.

The most common formats are: title, list, picture, quote. You can open an existing document for reference. If you need to use advanced functions, you can refer to `reStructuredText <https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html>`_

Step 2: Associate the RST document into the document tree
*********************************************************

Find index.rst in the directory where the document is located, and add a line to it::

    Mod <mod>

Compile and generate documentation
----------------------------------

There are two ways to generate doc:

 - Run build_doc.py in the docs directory
 - In the project directory, run armino.py doc, this method is only valid in FreeRTOS

The above command generates the build/html directory in the current directory. You can open build/html/index.hmtl in a browser to view the document effect.

.. note::

    The build system does not compile documentation by default, so after modifying API headers or RST documentation, you should run build_doc.py to see if errors are introduced.

