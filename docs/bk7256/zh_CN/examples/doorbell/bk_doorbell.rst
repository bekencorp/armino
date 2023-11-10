DOORBELL
==========================

:link_to_translation:`en:[English]`

1 功能概述
-------------------------------------
	门铃的主要功能是联合多个模块，实时的进行图像采集，屏幕显示，声音的双向采集和播放。

2 代码路径
-------------------------------------
	demo路径：``./components/media/cli/media_cli.c``, ``./components/media/camera/camera_act.c``, ``./components/media/transfer/transfer_act.c``, ``./components/media/lcd/lcd_act.c``

3 cli命令简介
-------------------------------------
	demo支持的命令如下表：

	+----------------------------------------+--------------------------------+---------------------------------------+
	|             Command                    |            Param               |              Description              |
	+========================================+================================+=======================================+
	|                                        | param1:softap_ssid             | setup a softap and set name           |
	| ap param1 param2                       +--------------------------------+---------------------------------------+
	|                                        | param2:softap_key              | set softap connect key                |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:station_ssid            | connect a station and set name        |
	| sta param1 param2                      +--------------------------------+---------------------------------------+
	|                                        | param2:station_key             | set station connect key               |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:3                       | open doorbell function                |
	| doorbell param1 [param2]               +--------------------------------+---------------------------------------+
	|                                        | param2:if open uvc, choosable  | set open/disopen uvc 1/0              |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media dvp open [param]                 | param:dvp resolution, choosable| set sensor resolution, and open dvp   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media dvp close                        | NULL                           | close dvp sensor                      |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:LCD PIXEL               | default:480X272                       |
	| media lcd open [param1] [param2]       +--------------------------------+---------------------------------------+
	|                                        | param2:rotate or null          | rotate open or not                    |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media lcd close                        | NULL                           | close lcd function, and jpeg decode   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	|                                        | param1:clock,wifi,data,ver     | blend icon type                       |
	| media lcd dma2d_blend [param1][param2] +--------------------------------+---------------------------------------+
	|                                        | param2:12:30 , 0-4             | 0:wifi none, 4:wifi full              |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media lcd dma2d_blend close [param]    | NULL or wifi/clock             | close all blend or one blend          |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media capture param                    | param:xxx.jpg                  | capture save to sdcard, and set name  |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media uvc open [param]                 | param:uvc resolution, choosable| set sensor resolution, and open uvc   |
	+----------------------------------------+--------------------------------+---------------------------------------+
	| media uvc close                        | NULL                           | close uvc sensor                      |
	+----------------------------------------+--------------------------------+---------------------------------------+

	demo运行依赖的宏配置：

	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|                 NAME                 |      Description       |                  File                      |  value  |
	+======================================+========================+============================================+=========+
	|CONFIG_DOORBELL                       |support DOORBELL        |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_JPEGENC_HW                     |suport hw jpeg ecoder   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA                         |suport use dvp camera   |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_CAMERA_USE_I2C1                |suport use I2C1         |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_AUDIO                          |support audio function  |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_LCD                            |support LCD function    |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_USB_UVC                        |support UVC camera      |``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+
	|CONFIG_LCD_FONT_BLEND                 |support font/image blend|``middleware\soc\bk7256\bk7256.defconfig``  |    y    |
	+--------------------------------------+------------------------+--------------------------------------------+---------+

4 演示介绍
-------------------------------------
	demo执行的步骤如下：

	1、准备好dvp摄像头或者UVC摄像头，LCD屏幕(rgb或者8080):

	2、起一个softap:

		ap doorbell_test 1234567890

	3、手机连接这个ap

	4、发送doorbell命令:

		doorbell

	5、手机app(RealtimeVideo)设置顺序和流程如下图所示：

	6、如果需要显示到LCD屏幕上

		media lcd open

5 app使用流程
--------------------------
	以下流程以板子作为softap为例，介绍apk的使用，在手机连接到板子使能的ap后：

	1.图1为app示意图

	2.图2为app的主界面

		其中，选择1可以看到图3

		- 1：设置菜单；
		- 2：更新apk和回退apk菜单；

	3.图3为设置界面菜单

		其中：

		- 1：为设置界面如图4所示；
		- 2：为返回主界面按钮；

	4.图4为真正的设置界面

		其中，设置说明如下，设置完成后返回图2主界面

		- 1：设置解决方案，当前支持video_transfer和doorbell，此处选择doorbell；
		- 2：设置数据传输模式，当前支持UDP和TCP，默认选择UDP；
		- 3：设置对端的ip地址，ap模式下默认为``192.168.0.1``不用修改，sta模式下设置为对端的ip地址；
		- 4：设置摄像头类型，当前支持DVP和UVC，根据自己使用摄像头类型进行设置；
		- 5：设置摄像头的输出分辨率；
		- 6：设置LCD输出的分辨率，根据自己使用的LCD屏幕分辨率进行设置；

	5.图5为功能使能设置界面

		其中，功能使能说明如下所示：

		- 1：开关视频图传；
		- 2：开关语音；
		- 3：拍照开关，当前暂不支持；
		- 4：开关LCD屏幕显示；

.. note::

	在图4中设置对端IP地址，当板子为softap时，默认为``192.168.0.1``，当板子作为staion时，手机和板子连接同一个ap，填入的IP地址可以通过命令``ip``获得。
	另外该app还支持手机图传功能，即命令``video_transfer -a|s ssid key``，但是必须将图4步骤1设置为``video_transfer``模式。
	apk的下载地址如下：http://dl.bekencorp.com/apk/RealtimeVideo.apk

.. figure:: ../../../../common/_static/RealtimeVideo_app.jpg
    :align: center
    :alt: RealtimeVideo_app
    :figclass: align-center

    Figure 1. doorbell apk

.. figure:: ../../../../common/_static/RealtimeVideo_set0.jpg
    :align: center
    :alt: RealtimeVideo_app_screen
    :figclass: align-center

    Figure 2. RealtimeVideo_app Main screen

.. figure:: ../../../../common/_static/RealtimeVideo_set1.jpg
    :align: center
    :alt: RealtimeVideo_app_set_menu
    :figclass: align-center

    Figure 3. RealtimeVideo_app Set menu

.. figure:: ../../../../common/_static/RealtimeVideo_set2.jpg
    :align: center
    :alt: RealtimeVideo_app_set
    :figclass: align-center

    Figure 4. RealtimeVideo_app set

.. figure:: ../../../../common/_static/RealtimeVideo_set3.jpg
    :align: center
    :alt: RealtimeVideo_function_set
    :figclass: align-center

    Figure 5. RealtimeVideo_function set

6 新增屏幕适配
-----------------------------

doorbel工程可以根据用户的需求增加新的屏幕驱动以适应不同的产品需求，新增屏幕驱动代码请参考 ``./middleware/driver/lcd`` 路径下新增lcd_xxx.c, 比如lcd_st7282.c;

	客户可以将任一文件拷贝，重命名为新增屏幕名，在新增驱动代码中主要做以下配置：

1) cmake 添加新增的lcd_xxx.c文件，使lcd_xxx.c文件参与编译
在  ``./middleware/driver``  目录下打开CMakeLists.txt, 在CONFIG_LCD的宏控下添加文件:

::

	if (CONFIG_LCD)
		list(APPEND srcs
			"lcd/lcd_driver.c"
			"lcd/lcd_hx8282.c"
			"lcd/lcd_st7282.c"
			"lcd/lcd_st7796s.c"
			"lcd/lcd_gc9503v.c"
			"lcd/lcd_nt35512.c"
			"lcd/lcd_h050iwv.c"
			"lcd/lcd_md0430r.c"
			"lcd/lcd_md0700r.c"
			"lcd/lcd_spi.c"
			"lcd/lcd_nt35510.c"
			"lcd/lcd_xxx.c"         //新增lcd_xxx.c
			)
	endif()



2) 针对RGB屏幕需要根据lcd spec 修改hsync/vsync proch，取值范围参考 ``./include/lcd_types.h``  结构体lcd_rgb_t注释。


::

	static const lcd_rgb_t lcd_rgb =
	{
		.clk = LCD_20M,          //lcd 工作合适的时钟
		.data_out_clk_edge = NEGEDGE_OUTPUT, //rgb输出数据时钟边沿

		.hsync_pulse_width = 2,
		.vsync_pulse_width = 2,
		.hsync_back_porch = 40, 
		.hsync_front_porch = 5, 
		.vsync_back_porch = 8,  
		.vsync_front_porch = 8, 
	};



.. figure:: ../../../../common/_static/st7282_lcd_sync.png
    :align: center
    :alt: RealtimeVideo_app
    :figclass: align-center

    Figure 6. rgb sync params config






3) 背光接口配置

如果屏幕的背光由PWM控制，则用户需要初始化PWM引脚，调节PWM的输出占空比

::

	static void lcd_backlight_open(void)
	{
	#if CONFIG_PWM
		lcd_driver_backlight_init(LCD_RGB_PWM_BACKLIGHT, 100);
	#endif
	}

	static void lcd_set_backlight(uint8_t percent)
	{
	#if CONFIG_PWM
		pwm_period_duty_config_t config = {0};

		if (percent > 100)
		{
			percent  = 100;
		}

		config.period_cycle = 100;
		config.duty_cycle = percent;

		bk_pwm_set_period_duty(LCD_RGB_PWM_BACKLIGHT, &config);
	#endif
	}


如果屏幕的背光控制是由GPIO(比如GPIO34)口控制：

::

	static void lcd_gc9503v_backlight_io_init(void)
	{
		gpio_dev_unmap(34);
		bk_gpio_set_capacity(34, 0);
		BK_LOG_ON_ERR(bk_gpio_enable_output(34));
		BK_LOG_ON_ERR(bk_gpio_pull_down(34));
	}

	static void gc9503v_lcd_backlight_open(void)
	{
		BK_LOG_ON_ERR(bk_gpio_pull_up(34));
		// pull up gpio34, enable lcd backlight control
		bk_gpio_set_output_high(34);
	}

	static void gc9503v_lcd_backlight_close(void)
	{
		bk_gpio_set_output_low(34);
	}


4) 初始化屏幕结构体参数

类似lcd_st7282屏幕不需要初始化，其结构体参数初始化如下：

::

	const lcd_device_t lcd_device_st7282 =
	{
		.id = LCD_DEVICE_ST7282,   //屏幕ID num
		.name = "st7282",          //屏幕 name
		.type = LCD_TYPE_RGB565,    //屏幕接口类型
		.ppi = PPI_480X272,         //屏幕分辨率
		.rgb = &lcd_rgb,             //RGB屏幕的参数配置
		.backlight_open = lcd_backlight_open,//注册背光初始化
		.backlight_set = lcd_set_backlight,  //注册赔光调节函数
		.init = NULL,                        //不需要初始化
		.backlight_close = lcd_backlight_close,//注册关闭背光函数
		.lcd_off = NULL,                       //屏幕没有关闭命令或关闭引脚
	};


其中屏幕分辨率未定义，需要在 ``./include/driver/media_types.h`` 中定义。 
新增的屏幕ID需要在 ``./include/driver/lcd_types.h`` 中定义，如下：

::

	typedef enum {
	LCD_DEVICE_UNKNOW,
	LCD_DEVICE_ST7282,  /**< 480X270  RGB */
	LCD_DEVICE_HX8282,  /**< 1024X600 RGB  */
	LCD_DEVICE_GC9503V, /**< 480X800 RGB  */
	LCD_DEVICE_ST7796S, /**< 320X480 MCU  */
	LCD_DEVICE_NT35512,
	} lcd_device_id_t;

该屏幕结构需要在 ``./middleware/driver/lcd/lcd_driver.c`` 中定义, 并在 ``./middleware/driver/lcd/lcd_device.h`` 中声明：

::

	const lcd_device_t *lcd_devices[] =
	{
		&lcd_device_st7282,
		&lcd_device_hx8282,
		&lcd_device_st7796s,
		&lcd_device_gc9503v,
		&lcd_device_nt35512
	};

	extern const lcd_device_t lcd_device_st7282;
	extern const lcd_device_t lcd_device_hx8282;
	extern const lcd_device_t lcd_device_st7796s;
	extern const lcd_device_t lcd_device_gc9503v;
	extern const lcd_device_t lcd_device_nt35512;



类似lcd_gc9503v屏幕需要初始化，其结构体中需要注册初始化函数：

::

	.init = lcd_gc9503v_init,

lcd_gc9503v_init函数一般是屏幕厂商提供，需要硬件接口模拟SPI或I2C接口，所以需要初始化GPIO，以及根据初始化命令适配对应的SPI或I2C协议，目前SDK中已适配了SPI3线和4线协议。

::

	void lcd_spi_init_gpio(void)
	{
		gpio_dev_unmap(LCD_SPI_RST);
		bk_gpio_set_capacity(LCD_SPI_RST, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_RST));
		BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_RST));

		gpio_dev_unmap(LCD_SPI_CLK_GPIO);
		bk_gpio_set_capacity(LCD_SPI_CLK_GPIO, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_CLK_GPIO));
		BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_CLK_GPIO));

		gpio_dev_unmap(LCD_SPI_CSX_GPIO);
		bk_gpio_set_capacity(LCD_SPI_CSX_GPIO, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_CSX_GPIO));
		BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_CSX_GPIO));

		gpio_dev_unmap(LCD_SPI_SDA_GPIO);
		bk_gpio_set_capacity(LCD_SPI_SDA_GPIO, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_SDA_GPIO));
		BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_SDA_GPIO));

		bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
		bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
		delay_us(200);
	}

至此，屏幕驱动的适配已经完成。

接下来的一个简单的适配是确保用户输入命令或传参（屏幕分辨率或名字），SDK中可以找到相应的设备：

1) 获取LCD设备名的判断，在media_cli.c中添加新增屏幕名字的判断

::

	char * get_string_to_name(char *string, char * pre)
	{
		char* value = pre;
		if (os_strcmp(string, "nt35512") == 0)
		{
			value = "nt35512";
		}
		
		.....
		
		if (os_strcmp(string, "nt35510") == 0)
		{
			value = "nt35510";
		}
		
		......

		return value;
	}


2) 获取LCD设备像素的判断，在media_cli.c中添加新增像素的判断

::

	uint32_t get_string_to_ppi(char *string, uint32_t pre)
	{
		uint32_t value = pre;

		if (os_strcmp(string, "1280X720") == 0)
		{
			value = PPI_1280X720;
		}

		.....

		if (os_strcmp(string, "480X854") == 0)
		{
			value = PPI_480X854;
		}

		return value;
	}

至此, 屏幕驱动的适配和调用参数一适配完成

接下来就可以使用新屏幕显示啦！

::

	lcd_open_t lcd_open;
	lcd_open.device_ppi = 480X800;
	lcd_open.device_name = "nt35512";
	ret = media_app_lcd_open(&lcd_open);


另外，有一点需要特别注意：
	 - 对于lcd设备有多个设备有相同的分辨率的情况下，必须添加使用名字传参， 不然只有分辨率不能找到指定的屏幕。
	 - 对于设备中的分辨率是唯一的（没有其他LCD有相同的分辨率），使用分辨率或者名字之一也可以找到指定的屏幕。




7 图标融合
-----------------------------------------------------


1) 关于DMA2D的使用指南请参考  ``html/bk7256/zh_CN/latest/examples/video/dma2d.html`` 里面详细介绍了前景图标的数据格式支持的类型，本文档就常见的ARGB8888以及RGB565的数据展开说明。

2) 准备需要融合的小图标的png图片或者jpg图片（又称前景图片）.

3) 将png转换为rgba8888的数据，该转换可以实现保持png的透明度，适合融合背景是透明的图标。
判断图片背景是否透明，可以用photoshop查看背景是否是镂空。

 - 工具路径： ``components/media/tools/ffmpeg_bat/png2argb`` 将所有的png图片访在该路径下，双击run.bat.

 .. figure:: ../../../../common/_static/png2rgba.png
    :align: center
    :alt: RealtimeVideo_app
    :figclass: align-center

    Figure 7. png to rgba8888.rgb

或将jpg转换为rgb565(大端)的数据，该转换过程中将所有不透明的jpg像素默认转换为不透明，适合融合背景和图标背景相近的图标，或对背景框没有要求的融合。

 - 工具路径： ``components/media/tools/ffmpeg_bat/jpeg2rgb565`` 将所有的jpg图片访在该路径下，双击run.bat.

 .. figure:: ../../../../common/_static/jpg2rgb565.png
    :align: center
    :alt: RealtimeVideo_app
    :figclass: align-center

    Figure 8. png to rgb565le.rgb


4) 使用 "HxD"或其他工具将rgb数据转成const数组并保存到flash中。

在 “components/media/include/blend_logo.h” 中定义前景图片，以及前景图片的长和宽:

::

	#define WIFI_LOGO_W 32
	#define WIFI_LOGO_H 36

	const unsigned char wifi_full_rgb565[2304] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		....
	};

如果想要将前景图标融合在LCD屏幕的固定位置，还需要设置前景融合的坐标
比如：想要将版本号的前景图标(16x36)从屏幕(480x800)的第500行，居中的位置开始融合，可以这样设置：

::

	#define VERSION_POSTION_Y   500   //HIGH 800
	#define VERSION_POSTION_X   144   //WIDTH 480 "BK7 V1.23.45"  (480-(12*16))/2    12个字节, 图标宽16

4) DMA2D实现融合

准备好了图片数据以及融合的位置，接下来就配置实际融合的参数，请参考lcd_act.c 中API 接口：lcd_blend_handler


比如要融合的wifi图标在在屏幕的右上角，即屏幕坐标(lcd_width-logo_width,0)处开始融合，其 lcd_blend.pbg_addr 就要设置 地址偏移量(frame->frame + (frame->width - WIFI_LOGO_W) * 2。 如何设置偏移在DMA2D的用例指南有详细介绍。

::

		if ((g_blend_data.lcd_blend_type & LCD_BLEND_WIFI) != 0)
		{
			LOGD("lcd wifi blend level =%d \n", g_blend_data.wifi_data);
			lcd_blend.pfg_addr = (uint8_t *)wifi_logo[g_blend_data.wifi_data];
			lcd_blend.pbg_addr = (uint8_t *)(frame->frame + (frame->width - WIFI_LOGO_W) * 2);
			lcd_blend.fg_offline = 0;
			lcd_blend.bg_offline = frame->width - WIFI_LOGO_W;
			lcd_blend.xsize = WIFI_LOGO_W;
			lcd_blend.ysize = WIFI_LOGO_H;
			lcd_blend.fg_alpha_value = FG_ALPHA;
			lcd_blend.fg_data_format = ARGB8888;
			lcd_driver_blend(&lcd_blend);
		}

8 新增摄像头配置
-----------------------
	在应用过程中使用的摄像头不仅仅是当前支持的这些，肯定还需要适配其他的dvp摄像头或者uvc摄像头，下面分开来说明一下针对两种不同类型的摄像头如何适配

- 1.dvp摄像头的适配

	dvp摄像头是通过I2C通信来配置摄像头的输出，主要是配置sensor寄存器的值，达到预期的图像效果（分辨率、帧率等）

	1)参考驱动代码：``middleware/driver/camera/dvp_gc0328c.c``，首先需要适配dvp摄像头的参数结构体：``dvp_sensor_config_t``;

::

	typedef struct
	{
		char *name;  /**< 摄像头的名字 */
		media_ppi_t def_ppi;  /**< 摄像头默认的分辨率，一般采用经常使用的分辨率 */
		sensor_fps_t def_fps;  /**< 摄像头默认的帧率，一般采用经常使用的帧率*/
		uint16 id;  /**< 摄像头类型（枚举值，需要自己添加）, 参考枚举类型sensor_id_t */
		uint8 clk;  /**< 摄像头协议规定的输入MCLK，而这个MCLK是从JPEG模块的CLK分出来，需要自己配置 */
		/**@example
		* JPEG_96M_MCLK_24M：表示这个摄像头协议规定MCLK输入的是24MHz，而此时JPEG模块工作的时钟是96MHz，96MHz四分频可以得到24MHz
		* 另外需要注意的是，JPEG的工作时钟是在CLK(480MHz和320MHz)中分出来的，分频系数范围F=[0, 15]，分频计算公式JPEG_CLK=CLK/(1+F);
		* 而JPEG内部支持分频的只有：0：4分频，1：6分频，2：2分频，3：3分频
		**/
		uint16 address;  /**< 摄像头通过I2C配置寄存器的地址，一般datasheet上会告知 */
		uint16 fps_cap;  /**< 摄像头支持配置的帧率，根据需求输出不同的帧率 */
		uint16 ppi_cap;  /**< 摄像头支持配置的分辨率，根据不同场景输出不同的分辨率 */
		bool (*detect)(const dvp_camera_i2c_callback_t *cb);  /**< 自动检测摄像头函数，是读取摄像头的标识(比如CHIP_ID)是否与当前摄像头一致*/
		int (*init)(const dvp_camera_i2c_callback_t *cb);  /**< 配置摄像头初始化寄存表，其他调整(比如：分辨率、帧率、白平衡等)必须在这个基础上 */
		int (*set_ppi)(const dvp_camera_i2c_callback_t *cb, media_ppi_t ppi);  /**< 设置摄像头分辨率的寄存器表，一般会支持不同的分辨率输出 */
		int (*set_fps)(const dvp_camera_i2c_callback_t *cb, sensor_fps_t fps);  /**< 设置摄像头帧率的寄存器，一般会支持不同帧率的输出 */
		int (*power_down)(const dvp_camera_i2c_callback_t *cb);  /**< 设置寄存器来配置摄像头的使能 */
		int (*dump_register)(const dvp_camera_i2c_callback_t *cb, media_ppi_t ppi);  /**< 调试接口，查看所有寄存器配置的值 */
		void (*read_register)(bool enable);  /**< 使能寄存器检查接口，在配置的过程中检查配置寄存器的值与期望的值一致*/
	} dvp_sensor_config_t;

2)参考使能摄像头驱动代码：``middleware/driver/camera/dvp_camera.c``，在函数：``bk_dvp_camera_driver_init()``，可能需要增加新摄像头的MCLK输入配置；

::

	switch (current_sensor->clk)
	{
		case JPEG_96M_MCLK_16M:
			jpeg_config.sys_clk_div = 4;
			jpeg_config.mclk_div = 1;
			break;

		case JPEG_96M_MCLK_24M:
			jpeg_config.sys_clk_div = 4;
			jpeg_config.mclk_div = 0;
			break;

		case JPEG_120M_MCLK_20M:
			jpeg_config.sys_clk_div = 3;
			jpeg_config.mclk_div = 1;
			break;

		case JPEG_120M_MCLK_30M:
			jpeg_config.sys_clk_div = 3;
			jpeg_config.mclk_div = 0;
			break;

		default:
			break;
	}

.. note::
	注意：上面JPEG的注视中默认选择的是480MHz，当前并没有开放SDK接口来配置选择480MHz或者320MHz。如果需要选用320MHz，请参考JPEG驱动代码：``middleware/driver/jpeg_enc/jpeg_driver.c``.

::

	static void jpeg_power_config_set(const jpeg_config_t *config)
	{
		sys_drv_set_jpeg_clk_sel(1);//0:320MHz, 1:480MHz
		sys_drv_set_clk_div_mode1_clkdiv_jpeg(config->sys_clk_div);
		sys_drv_set_jpeg_disckg(1);
		bk_pm_clock_ctrl(PM_CLK_ID_JPEG, CLK_PWR_CTRL_PWR_UP);
	}


2.uvc摄像头的适配

	uvc唯一需要适配的是其支持的分辨率，uvc输出的分辨率千变万化，当前只适配了一些常规的分辨率，客户如有特殊的分辨率，需要自己添加。

	1)当前暂不支持客户独立添加新的分辨率，后续会修改，因为当前增加新的分辨率需要开发人员单独给客户新的libusb.a文件，替换路径：``components/bk_libs/bk7256_app/libs/libusb.a``

	2)替换好新的libusb.a文件后，参考头文件：``include/driver/media_types.h``，枚举类型``media_ppi_t``，里面的参数需要添加，如果没有。

3.命令行增加新的分辨率

	如果需要采用doorbell自带的cli命令，需要让新增加的分辨率可以生效，否则跳过这一步

	参考doorbell命令行：``components/media/cli/media_cli.c``，适配新的命令，在函数：``get_string_to_ppi()``，中增加新的分辨率;

.. note::
	注意: 摄像头的分辨率的width 要能被16整除(比如864/16), height 要被8（比如 480/8）整除。否则硬件解码会失败。

9 图像旋转
-----------------------------------------------------

当屏幕和图片像素相反，比如屏幕是480X800， 摄像头是800X480，可以进行图像旋转显示。


10 字库融合
-----------------------------------------------------

SDK中支持字库的抗锯齿融合,字库以内部库的方式提供给用户，目前支持的字库有:

::

    #define FONT_ANTI4BPP_ROBOTO_53           1   /* 罗马数字*/
    #define FONT_ANTI4BPP_BLACK24             1   /* 黑体*/
    #define FONT_ANTI4BPP_BLACK48             1   /* 黑体*/
    #define FONT_ANTI4BPP_SOURCE_HAN_SANS17   0   /* 思源黑体*/
    #define FONT_ANTI4BPP_SOURCE_HAN_SANS42   0   /* 思源黑体*/

    #if FONT_ANTI4BPP_ROBOTO_53
    extern const gui_font_digit_struct *const font_digit_Roboto53;
    #endif
    #if FONT_ANTI4BPP_SOURCE_HAN_SANS17
    extern const gui_font_digit_struct *const font_digitSource_Han_Sans17;
    #endif
    #if FONT_ANTI4BPP_SOURCE_HAN_SANS42
    extern const gui_font_digit_struct *const font_digitSource_Han_Sans42;
    #endif
    #if FONT_ANTI4BPP_BLACK24
    extern const gui_font_digit_struct *const font_digit_black24;
    #endif
    #if FONT_ANTI4BPP_BLACK48
    extern const gui_font_digit_struct *const font_digit_black48;
    #endif

目前字的添加是根据不同的客户需求所添加，如果不满足客户需求，请联系支持。

字库融合的代码请参考 ``middleware/driver/lcd_driver.c`` API:lcd_driver_font_blend


::

	font_t font;
	font.info = (ui_display_info_struct){rgb565_data,0,lcd_font->ysize,0,{0}}; 
	font.width = lcd_font->xsize;
	font.height = lcd_font->ysize;
	font.font_fmt = lcd_font->font_format;
	for(int i = 0; i < lcd_font->str_num; i++)
	{
		font.digit_info = lcd_font->str[i].font_digit_type;
		font.s = lcd_font->str[i].str;
		font.font_color = lcd_font->str[i].font_color;
		font.x_pos = lcd_font->str[i].x_pos;
		font.y_pos = lcd_font->str[i].y_pos;
		lcd_draw_font(&font);
	}

各参数的值输入如下：

::

	//融合位置的配置
	frame_addr_offset = ((start_y + VERSION_POSTION_Y) * frame->width + start_x + VERSION_POSTION_X) * 2;
	lcd_font_config.pbg_addr = (uint8_t *)(frame->frame + frame_addr_offset); 	//地址偏移
	lcd_font_config.bg_offline = frame->width - CLOCK_LOGO_W;
	lcd_font_config.xsize = CLOCK_LOGO_W;       ///融合区域的宽，根据汉字的宽决定
	lcd_font_config.ysize = CLOCK_LOGO_H;       ///融合区域的高，根据汉字的高决定
	lcd_font_config.str_num = 2;                ///一次融合几个字符串
	#if 1  ///font yuv data to bg yuv image
	if (frame->fmt == PIXEL_FMT_VUYY)           ///将汉字融合到YUV数据中
		lcd_font_config.font_format = FONT_VUYY;
	else
		lcd_font_config.font_format = FONT_YUYV;
	#else  ///font rgb data to bg yuv image
	lcd_font_config.font_format = FONT_RGB565;  ///将汉字融合到RGB565数据中
	#endif

	///白色字体，字号，融合到区域的起始坐标
	lcd_font_config.str[0] = (font_str_t){(const char *)("晴转多云, 27℃"), FONT_WHITE, font_digit_black24, 0, 2};
	lcd_font_config.str[1] = (font_str_t){(const char *)("2022-12-12 星期三"), FONT_WHITE, font_digit_black24, 0, 26};

	lcd_font_config.bg_data_format = frame->fmt;  ///背景数据格式
	lcd_font_config.bg_width = frame->width;      ///背景图片size
	lcd_font_config.bg_height = frame->height;
	lcd_driver_font_blend(&lcd_font_config);
