
图传的步骤说明：

1. 功能说明：
图传支持平台： bk7231u、bk7252
支持系统：     freertos、 rtthread、 alios 
支持接串口：   sccb接口、spi-slave(待调试)
支持传输方式： tcp、udp、video_buffer(获取单张图)
支持模式：     station、 softap

2.使用说明：

摄像头默认使用 GC0328C， 如需更改请在 camera_intf.h 中修改，并定义其初始化文件。
使用I2C1来驱动摄像头，uart2为打印口

1). 定义以下宏为 1
位于 sys_config_bkxxxx.h中
#define CONFIG_APP_DEMO_VIDEO_TRANSFER            1
#define CFG_USE_CAMERA_INTF                        1

位于 video_demo_config.h中
#define APP_DEMO_VIDEO_TRANSFER                    1


2). RF校准相关
支持 自动功率校准 和 手动功率校准(默认)

切换的方法有两种

1> 编译时选择：bk72xx_cal.c 中的 g_cali_mode， 
CALI_MODE_MANUAL 是 手动模式（采用Itest等仪器来校准）， 
CALI_MODE_AUTO   是 自动模式（根据设定的目标值，每次上电都会校准）。

2> 运行时选择：
ATE模式下，通命令行，下发指令设置：
rfcali_cfg_mode: x,  其中 1:manual, 0:auto


3).启动sation 或 softap 方式传图
非ATE模式下，通命令行，下发指令
video_transfer -a XXXXX  xxxxx   // 启动softap 传图，XXXXX 为ssid， xxxxx 密码(可选，不填时为不加密)
video_transfer -s XXXXX  xxxxx   // 启动station传图，XXXXX 为路由器的ssid， xxxxx 密码(可选，不填时为不加密)

代码默认支持 UDP和 TCP 方式，相关设置位于 video_transfer_config.h 中


4).看图：
位于freertos根目录下
\tools\video_tool\PC_tool    PC机工具
\tools\video_tool\android    安卓手机（只支持softap模式）

PC机工具，支持softap 和station模式看图：
softap模式下， PC机无线网卡（只开启无线网卡，其他网卡先禁用掉）连接模块启动的热点，点play就可以出图
station模式下，PC机网卡（无线或有线网卡，其他网卡先禁用掉）与模块位置同一个网段，点play前，需要在remote ip(右边)处手动填写模块的ip地址。

5).获取一张图片数据
非ATE模式下，通命令行，下发指令
video_buffer open  打开成功后，再发

video_buffer read xxxx  xxxx为size,byte为单位。  获取到了图像后会打印所有数据。

video_buffer close 关闭该功能