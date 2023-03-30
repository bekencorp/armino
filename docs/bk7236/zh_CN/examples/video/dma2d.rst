DMA2D
=================================

:link_to_translation:`en:[English]`

1、功能概述
--------------------
	本文档主要介绍了DMA2D的API使用，例程主要基于DOORBEEL工程以及lcd 808 demo.

2、代码路径
--------------------------
	 - DOORBEEL 主要涉及dma2d 融合和memory copy功能，参考代码: ``/components/media/lcd/lcd_act.c`` 和 ``/middleware/driver/lcd/lcd_driver.c``
	
	 - lcd_8080_demo.c 主要涉及dma2d的填充，具体的示例代码详见: ``\components\demos\media\lcd\lcd_8080``
	 
	 - dma2d的驱动接口介绍参考: ``/api-reference/multi_media/bk_dma2d.html``


demo运行依赖的宏配置:

+---------------------------+----------------------------+----------------------------------------------------+-----+
|Name                       |Description                 |   File                                             |value|
+===========================+============================+====================================================+=====+
|CONFIG_DMA2D               |使能DMA2D功能               |``\middleware\soc\bk7236\bk7236.defconfig``         |  y  |
+---------------------------+----------------------------+----------------------------------------------------+-----+


3、DMA2D内存拷贝
---------------------------------------------------------------------------------------
	
	 - DMA2D的内存拷贝即不执行像素转换，只执行内存拷贝的功能，用于图像的快速填充。
	 - 当只有拷贝功能的时候，输入颜色和输出颜色应选择一致，当支持像素转换的时候，二者可以不一样，支持的颜色模式在dma2d_types.h中定义。在融合以及像素转换的过程中用到的input_color_mode 和 out_color_mode_t 也是使用的这两个结构体作为参数。

DMA2D的输入颜色模式可以选择：

::

	/** DMA2D_Input_Color_Mode*/
	typedef enum {
		DMA2D_INPUT_ARGB8888 = 0, /**< ARGB8888 DMA2D color mode */
		DMA2D_INPUT_RGB888,  /**< RGB888 DMA2D color mode   */
		DMA2D_INPUT_RGB565,   /**< RGB565 DMA2D color mode   */
		DMA2D_INPUT_ARGB1555,   /**< ARGB1555 DMA2D color mode */
		DMA2D_INPUT_ARGB4444,   /**< ARGB4444 DMA2D color mode */
		DMA2D_INPUT_L8 = 5,
		DMA2D_INPUT_AL44,
		DMA2D_INPUT_AL88,
		DMA2D_INPUT_L4,
		DMA2D_INPUT_A8,
		DMA2D_INPUT_A4,
	} input_color_mode_t;

输出颜色模式可以选择：

::

	/** DMA2D_Output_Color_Mode */
	typedef enum {
		DMA2D_OUTPUT_ARGB8888 = 0, /**< ARGB8888 DMA2D color mode */
		DMA2D_OUTPUT_RGB888,       /**< RGB888 DMA2D color mode   */
		DMA2D_OUTPUT_RGB565,       /**< RGB565 DMA2D color mode   */
		DMA2D_OUTPUT_ARGB1555,     /**< ARGB1555 DMA2D color mode */
		DMA2D_OUTPUT_ARGB4444,     /**< ARGB4444 DMA2D color mode */
	} out_color_mode_t;

对于内存访问需要按word对齐的mem(比如psram), DMA2D的内存拷贝需要使用的数据格式为ARGB8888。


dma2d memcpy例程如下

::

	void dma2d_memcpy_psram(void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline)
	{
		dma2d_config_t dma2d_config = {0};

		dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
		dma2d_config.init.color_mode    = DMA2D_OUTPUT_ARGB8888; /**< Output color mode is ARGB8888 : 32 bpp */
		dma2d_config.init.output_offset = dest_offline/2;                   /**< offset on output */
		dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No Red & Blue swap for the output image */
		dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

		dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from ARGB8888 input */
		dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
		dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888; /**< Input color is  ARGB8888 : 32 bpp */
		dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offline/2;                     /**< offset in input */
		dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;      /**< No R&B swap for the input image */
		dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

		bk_dma2d_init(&dma2d_config);
		bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

		bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, (uint32_t)Pdst, xsize/2, ysize); 
		while (bk_dma2d_is_transfer_busy()) {}
	}



参数配置示意图如下

 1) Psrc: 要拷贝的源数据的地址，拷贝数据的起始地址，可以是帧起始地址，也可以是一个偏移地址。
 如果是偏移地址需要设置行偏移。

 2) Pdst: 拷贝到目标数据的地址，可以是数据的起始地址，也可以是一个偏移地址。
 
 3) xsize:拷贝的小图片的宽(xpixel)
 
 4) ysize:拷贝的小图片的高(ypixel)
 
 5) src_offline:如果Psrc是一个偏移地址，src_offline即决定了偏移量，以像素为单位。
 
 6) dest_offline:如果Pdst是一个偏移地址，dest_offline即决定了偏移量，以像素为单位。

.. figure:: ../../../_static/dma2d_config.png
	:align: center
	:figclass: align-center

	Figure 2. dma2d memcpy config


接口实现内其他参数解释：

::

	DMA2D_RB_REGULAR : 配置输入数据的红色和蓝色是否转换。


3、DMA2D融合
------------------------
	DMA2D的图片融合是硬件实现，通过配置前景,背景以及输出的的图片格式,透明度,数据地址偏移实现图片的融合。

	dma2d 融合例程：

::

	dma2d_blend_t dma2d_config;
	dma2d_config.pfg_addr = lcd_blend->pfg_addr;
	dma2d_config.pbg_addr = rgb565_data;
	dma2d_config.pdst_addr = rgb565_data;
	if (lcd_blend->fg_data_format == ARGB8888)
	{
		dma2d_config.fg_color_mode = DMA2D_INPUT_ARGB8888;
		dma2d_config.red_bule_swap = DMA2D_RB_SWAP;
	}
	if (lcd_blend->fg_data_format == RGB565)
	{
		dma2d_config.fg_color_mode = DMA2D_INPUT_RGB565;
		dma2d_config.red_bule_swap = DMA2D_RB_REGULAR;
	}
	dma2d_config.bg_color_mode = DMA2D_INPUT_RGB565;
	dma2d_config.dst_color_mode = DMA2D_OUTPUT_RGB565;
	dma2d_config.fg_offline = 0;
	dma2d_config.bg_offline = 0;
	dma2d_config.dest_offline = 0;
	dma2d_config.xsize = lcd_blend->xsize;
	dma2d_config.ysize = lcd_blend->ysize;
	dma2d_config.fg_alpha_value = lcd_blend->fg_alpha_value;
	dma2d_config.bg_alpha_value = lcd_blend->bg_alpha_value;
	bk_dma2d_blend(&dma2d_config);


配置参数详解：

::

	typedef struct
	{
	 void *pfg_addr;    //前景数据的地址，可以是拷贝数据的首地址，也可以是一个偏移地址。
	 void *pbg_addr;    //背景数据的地址，可以是数据的起始地址，也可以是一个偏移地址。
	 void *pdst_addr;   //输出融合数据的地址，可以是数据的起始地址，也可以是一个偏移地址。
	 input_color_mode_t fg_color_mode;  //前景数据的颜色格式，从input_color_mode_t中选择
	 input_color_mode_t bg_color_mode;   //背景数据的颜色格式，从input_color_mode_t中选择
	 out_color_mode_t   dst_color_mode;  //输出的数据格式，从out_color_mode_t中选择
	 uint32_t fg_offline; //如果pfg_addr是一个偏移地址，fg_offline即决定了偏移量，以像素为单位。
	 uint32_t bg_offline; //如果pbg_addr是一个偏移地址，bg_offlinee即决定了偏移量，以像素为单
	 uint32_t dest_offline; //如果pdst_addr是一个偏移地址，dest_offline即决定了偏移量，以像素为单
	 uint32 xsize;          //融合的小图片的宽
	 uint32 ysize;          //融合的小图片的高
	 uint8_t fg_alpha_value; //配置前景的透明度
	 uint8_t bg_alpha_value; //配置背景的透明度
	 uint8_t red_bule_swap;  //配置前景图片输入是否红蓝反转，可选择DMA2D_RB_SWAP 或 DMA2D_RB_REGULAR
	}dma2d_blend_t;



其中：
	
fg_alpha_value 和 bg_alpha_value为不透明度，0为完全透明， 0xff为完全不透明。
fg_alpha_value和 bg_alpha_value值是否有效以及如何取值 取决于前景或背景的alpha_mode。
如果alpha_mode为：

::

	 1) DMA2D_NO_MODIF_ALPHA：即保持原图片的透明度，不管input_alpha 值设置为多少都不生效。
	 2) DMA2D_REPLACE_ALPHA：替换原图的alpha(透明度)，图片的透明度替换成alpha_value的值。
	 3) DMA2D_COMBINE_ALPHA：图片的透明度为(原有的透明度 * input_alpha)/0xFF. 
	 即如果源图片中某些像素的alpha为0(完全透明)，配置input_alpha后，该像素的alpha仍为0,常用于保持原图的透明效果。

4、DMA2D 填充
--------------------

DMA2D的填充。
	
	DMA2D的单色填充可用于快速清屏或柱状图绘制。

::

	void dma2d_fill(uint32_t frameaddr, uint16_t x, uint16_t y, uint16_t width, uint16_t high, uint16_t color)
	{
		void *pDiSt=&(((uint16_t *)frameaddr)[y*320+x]);

		dma2d_config_t dma2d_config = {0};

		dma2d_config.init.mode   = DMA2D_R2M; 		       /**< Mode Register to Memory */
		dma2d_config.init.color_mode	   = DMA2D_OUTPUT_RGB565;   /**< DMA2D Output color mode is RGB565 (16 bpp) */
		dma2d_config.init.output_offset  = 320 - width;		/**< offset in output */
		dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;    /**< No R&B swap for the output image */
		dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;	 /**< No alpha inversion for the output image */
		bk_dma2d_driver_init(&dma2d_config);

	/*	bk_dma2d_int_config(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);
	#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
		bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, mda2d_r2m_transfer_error);
		bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, mda2d_r2m_transfer_complete);
	#else
		bk_dma2d_isr_register(dma2d_isr);
	#endif 
	*/

		bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)pDiSt, width, high); 

	/*	while (transferCompleteDetected == 0) {;}
		transferCompleteDetected = 0;
	*/
		while (bk_dma2d_is_transfer_busy()) {
		}
	}

例程详解：

::

	 - frameaddr 帧起始地址
	 - x demo中针对的是320X480的屏幕，x即从屏幕刷一帧的起始位置(0，0)开始算起的x坐标
	 - y y即从屏幕刷一帧的起始位置(0，0)开始算起的y坐标
	 - width 填充颜色的宽度
	 - high 想要填充颜色的高度
	 - color 想要填充颜色的像素
	 - demo 中注释的部分为打开DMA2D的中断，可以传输完成中断，配置错误中断或传输错误中断。

.. figure:: ../../../_static/dma2d_fill_config.png
	:align: center
	:alt: fill specific color
	:figclass: align-center

	Figure 3. dma2d color fill config


注意：
	对于只能按Word操作的内存，如果想要填充RGB565的数据，颜色格式应该用ARGB8888代替,相应的填充数据的宽或高也应该设置为宽或高的一半即可。详细请参考lcd_8080_demo.c中用于清屏的接口，如下：

::

	dma2d_blend_t dma2d_config;
	dma2d_config.pfg_addr = lcd_blend->pfg_addr;
	dma2d_config.pbg_addr = rgb565_data;
	dma2d_config.pdst_addr = rgb565_data;
	if (lcd_blend->fg_data_format == ARGB8888)
	{
		dma2d_config.fg_color_mode = DMA2D_INPUT_ARGB8888;
		dma2d_config.red_bule_swap = DMA2D_RB_SWAP;
	}
	if (lcd_blend->fg_data_format == RGB565)
	{
		dma2d_config.fg_color_mode = DMA2D_INPUT_RGB565;
		dma2d_config.red_bule_swap = DMA2D_RB_REGULAR;
	}
	dma2d_config.bg_color_mode = DMA2D_INPUT_RGB565;
	dma2d_config.dst_color_mode = DMA2D_OUTPUT_RGB565;
	dma2d_config.fg_offline = 0;
	dma2d_config.bg_offline = 0;
	dma2d_config.dest_offline = 0;
	dma2d_config.xsize = lcd_blend->xsize;
	dma2d_config.ysize = lcd_blend->ysize;
	dma2d_config.fg_alpha_value = lcd_blend->fg_alpha_value;
	dma2d_config.bg_alpha_value = lcd_blend->bg_alpha_value;
	bk_dma2d_blend(&dma2d_config);


4、DMA2D 像素转换
--------------------

	DMA2D的像素转换的一个简单例程（RGB565转ARGB8888）如下:
	转换的源数据pfg_addr和目标数据rgb566_to_argb8888都没有偏移，均为起始地址。

::

	dma2d_pixel_convert_t pixel_convert;

	pixel_convert.input_addr = pfg_addr;
	pixel_convert.output_addr = rgb566_to_argb8888;
	pixel_convert.xsize = xsize; 
	pixel_convert.ysize = ysize;
	pixel_convert.input_color_mode = DMA2D_INPUT_RGB565;
	pixel_convert.output_color_mode = DMA2D_OUTPUT_ARGB8888;
	pixel_convert.intput_alpha = 0;    
	pixel_convert.intput_offline = 0;
	pixel_convert.output_offline = 0;
	pixel_convert.output_red_blue_swap = DMA2D_RB_REGULAR;
	bk_dma2d_pixel_convert(&pixel_convert);

