// Copyright 2020-2021 Beken 
// 
// Licensed under the Apache License, Version 2.0 (the "License");           
// you may not use this file except in compliance with the License.            
// You may obtain a copy of the License at                                     
//                                                                             
//     http://www.apache.org/licenses/LICENSE-2.0                              
//                                                                             
// Unless required by applicable law or agreed to in writing, software         
// distributed under the License is distributed on an "AS IS" BASIS,         
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
// See the License for the specific language governing permissions and         
// limitations under the License.                                              

/***********************************************************************************************************************************
* This file is generated from PWM_register_map.xlsx automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7236,GENARATE TIME:2023-03-23 15:46:12                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

typedef volatile struct {

    /* REG_0x00 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  deviceid                       :32;  //0x0[31:0],ASCII Code of "PWM",0x00808777,RO
        }; 
        uint32_t v; 
    }deviceid; 

    /* REG_0x01 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  versionid                      :32;  //0x1[31:0],版本号V1.1:w(区别不同版本),0x00010001,RO
        }; 
        uint32_t v; 
    }versionid; 

    /* REG_0x02 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  soft_reset                     : 1;  //0x2[0],写1：PWM控制器软复位,0x0,R/W
           volatile uint32_t  bps_clkgate                    : 1;  //0x2[1],配置时钟门控禁止位,0x0,R/W
           volatile uint32_t  reserved                     :30;  //0x2[31:2],写1：PWM控制器软复位,0x0,R/W
        }; 
        uint32_t v; 
    }cg_reset; 

    /* REG_0x03 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  global_status                  :32;  //0x3[31:0],反应外设的工作状态,0x0,RO
        }; 
        uint32_t v; 
    }status; 

    /* REG_0x04 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cen3                           : 1;  //0x4[0],计数器TIM3使能,0x0,R/W
           volatile uint32_t  cen2                           : 1;  //0x4[1],计数器TIM2使能,0x0,R/W
           volatile uint32_t  cen1                           : 1;  //0x4[2],计数器TIM1使能,0x0,R/W
           volatile uint32_t  arpe3                          : 1;  //0x4[3],计数器TIM3终点寄存器preload使能,0x0,R/W
           volatile uint32_t  arpe2                          : 1;  //0x4[4],计数器TIM2终点寄存器preload使能,0x0,R/W
           volatile uint32_t  arpe1                          : 1;  //0x4[5],计数器TIM1终点寄存器preload使能  0为preload寄存器中中的值立即同步更新到阴影寄存器，1为在更新事件时更新（Auto-Reload Preload Enable),0x0,R/W
           volatile uint32_t  oc3pe                          : 1;  //0x4[6],TIM3的比较寄存器preload使能见OC1PE,0x0,R/W
           volatile uint32_t  oc2pe                          : 1;  //0x4[7],TIM2的比较寄存器preload使能见OC1PE,0x0,R/W
           volatile uint32_t  oc1pe                          : 1;  //0x4[8],TIM1的比较寄存器CCR1~CCR3 preload使能  0为preload寄存器中中的值立即同步更新到阴影寄存器，1为在更新事件时更新,0x0,R/W
           volatile uint32_t  reserved0                       : 9;  //0x4[17:9], ,None,None
           volatile uint32_t  cms_tim3                       : 2;  //0x4[19:18],计数器TIM1计数模式：; 00单向计数模式; 01双向计数模式 且只有向上计数比较成功时才置位比较成功中断flag; 10双向计数模式 且只有向下计数比较成功时才置位比较成功中断flag; 11双向计数模式 且向上和向下计数比较成功时都置位比较成功中断flag,2,R/W
           volatile uint32_t  cms_tim2                       : 2;  //0x4[21:20],计数器TIM2计数模式：同CMS_TIM1,0x0,R/W
           volatile uint32_t  cms_tim1                       : 2;  //0x4[23:22],计数器TIM3计数模式：同CMS_TIM1,0x0,R/W
           volatile uint32_t  reserved1                       : 2;  //0x4[25:24], ,None,None
           volatile uint32_t  urs3                           : 1;  //0x4[26],见URS1,0x0,R/W
           volatile uint32_t  urs2                           : 1;  //0x4[27],见URS1,0x0,R/W
           volatile uint32_t  urs1                           : 1;  //0x4[28],更新事件源选择  0：UG与向上向下溢出可产生更新事件;                 1：仅向上向下溢出可产生更新事件（捕获模式必须设置为1，否则会同时产生捕获成功中断和更新事件中断）,0x0,R/W
           volatile uint32_t  dir_tim3                       : 1;  //0x4[29],计数器TIM3的方向：同DIR_TIM1,0x0,R/W
           volatile uint32_t  dir_tim2                       : 1;  //0x4[30],计数器TIM2的方向：同DIR_TIM1,0x0,R/W
           volatile uint32_t  dir_tim1                       : 1;  //0x4[31],计数器TIM1的方向  0为向上计数  1为向下计数 （当工作在编码器模式或者中心对称模式时，该位为只读）,0x0,R/W
        }; 
        uint32_t v; 
    }pwm_cr1; 

    /* REG_0x05 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  reserved0                       : 8;  //0x5[7:0], ,0x0,R/W
           volatile uint32_t  ti1s                           : 1;  //0x5[8],TIM1的触发输入源选择; 0 cap_i[0]作为TI1的输入; 1 cap_i[2:0]的XOR作为TI1的输入，霍尔传感器接口模式用,0x0,R/W
           volatile uint32_t  ccpc                           : 1;  //0x5[9],CCE，CCNE bit preload   ; 该bi为1时：CCE，CCEN只有在COM事件（霍尔模式时TRGI上升沿）发生时才更新 ,0x0,R/W
           volatile uint32_t  trios3                         : 3;  //0x5[12:10],TIM3的触发输出选择 同TRIOS1,0x0,R/W
           volatile uint32_t  adcs3                          : 3;  //0x5[15:13],TIM3的ADC触发选择  同ADCS1,0x0,R/W
           volatile uint32_t  reserved1                       : 1;  //0x5[16], ,0x0,R/W
           volatile uint32_t  ccus                           : 1;  //0x5[17], ,0x0,R/W
           volatile uint32_t  trios2                         : 3;  //0x5[20:18],TIM2的触发输出选择 同TRIOS1,0x0,R/W
           volatile uint32_t  adcs2                          : 3;  //0x5[23:21],TIM2的ADC触发选择  同ADCS1 ,0x0,R/W
           volatile uint32_t  reserved2                       : 2;  //0x5[25:24], ,0x0,R/W
           volatile uint32_t  trios1                         : 3;  //0x5[28:26],TIM1的触发输出选择 ; 0 触发输出0; 1 CEN作为TRIO触发输出; 2 UG作为TRIO触发输出; 3 update事件触发输出; 4 OC1REF作为TRIO触发输出; 5 CCR1比较成功信号cc1g作为TRIO触发输出; 6 CCR2比较成功信号cc2g作为TRIO触发输出; 7 CCR3比较成功信号cc3g作为TRIO触发输出; 8 OC1REF作为TRIO触发输出; 9 CCIF信号即（比较或者捕获成功）作为TRIO触发输出,0x0,R/W
           volatile uint32_t  adcs1                          : 3;  //0x5[31:29],TIM1的触发ADC选择 ; 0 触发ADC0; 1 CEN作为TRIO触发ADC; 2 UG作为TRIO触发ADC; 3 update事件触发ADC; 4 OC1REF作为TRIO触发ADC; 5 CCR1比较成功信号cc1g作为TRIO触发ADC; 6 CCR2比较成功信号cc2g作为TRIO触发ADC; 7 CCR3比较成功信号cc3g作为TRIO触发ADC; 8 OC1REF作为TRIO触发ADC; 9 CCIF信号即（比较或者捕获成功）作为TRIO触发ADC,0x0,R/W
        }; 
        uint32_t v; 
    }pwm_cr2; 

    /* REG_0x06 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  sms1                           : 4;  //0x6[3:0],TIM1从模式选择：; 0000 从模式关闭  定时器对内部时钟沿计数; 0001 LED驱动模式; 0010 编码器接口模式1  只对TI1的边沿向上下计数; 0011 编码器接口模式2  只对TI2的边沿向上下计数; 0100 编码器接口模式3  对TI1和TI2的边沿都向上下计数; 101 重置模式1   TRGI2的上升沿UG1置1，定时器重置（捕获模式时可使用）; 110 门控模式   TRGI2为高电平CEN1置1，为低CEN1清零（模块外部定时器控制TIM1启停）; 111 触发模式   TRGI2的上升沿CEN1置1（对外部脉冲计数）; 1000 霍尔传感器接口模式,0x0,R/W
           volatile uint32_t  sms2                           : 4;  //0x6[7:4],TIM2从模式选择：; 000 从模式关闭  定时器对内部时钟沿计数; 001 LED驱动模式; 010 编码器接口模式1  只对TI3的边沿向上下计数; 011 编码器接口模式2  只对TI4的边沿向上下计数; 100 编码器接口模式3  对TI3和TI4的边沿都向上下计数; 101 重置模式1   TRGI2的上升沿UG2置1，定时器重置（捕获模式时可使用）; 110 门控模式   TRGI2为高电平CEN2置1，为低CEN2清零（模块外部定时器控制TIM3启停）; 111 触发模式   TRGI2的上升沿CEN2置1（对外部脉冲计数）,0x0,R/W
           volatile uint32_t  sms3                           : 4;  //0x6[11:8],TIM3从模式选择：; 000 从模式关闭  定时器对内部时钟沿计数; 001 LED驱动模式; 010 编码器接口模式1  只对TI5的边沿向上下计数; 011 编码器接口模式2  只对TI6的边沿向上下计数; 100 编码器接口模式3  对TI5和TI6的边沿都向上下计数; 101 重置模式1   TRGI3的上升沿UG3置1，定时器重置（捕获模式时可使用）; 110 门控模式   TRGI3为高电平CEN3置1，为低CEN3清零（模块外部定时器控制TIM3启停）; 111 触发模式   TRGI3的上升沿CEN3置1（对外部脉冲计数）,0x0,R/W
           volatile uint32_t  ts1                            : 3;  //0x6[14:12],TIM1触发输入源选择：; 000 TI1P TI1的单边沿检测脉冲; 001 TI2P TI2的单边沿检测脉冲; 010 TI1_ED  TI1的双边沿检测脉冲; 010 TI2_ED  TI2的双边沿检测脉冲; 101 tri_i[2] ; 101 tri_i[1] ; 110 tri_i[0] ,0x0,R/W
           volatile uint32_t  reserved0                       : 1;  //0x6[15], ,0x0,R/W
           volatile uint32_t  ts2                            : 3;  //0x6[18:16],TIM2触发输入源选择：; 000 TI3P TI3的单边沿检测脉冲; 001 TI4P TI4的单边沿检测脉冲; 010 TI3_ED  TI3的双边沿检测脉冲; 011 TI4_ED  TI4的双边沿检测脉冲; 100 tri_i[2] ; 101 tri_i[1] ; 110 tri_i[0] ,0x0,R/W
           volatile uint32_t  ts3                            : 3;  //0x6[21:19],TIM3触发输入源选择：; 000 TI5P TI5的单边沿检测脉冲; 001 TI6P TI6的单边沿检测脉冲; 010 TI5_ED  TI3的双边沿检测脉冲; 011 TI6_ED  TI4的双边沿检测脉冲; 100 tri_i[2] ; 101 tri_i[1] ; 110 tri_i[0] ,0x0,R/W
           volatile uint32_t  reserved1                       :10;  //0x6[31:22], ,0x0,R/W
        }; 
        uint32_t v; 
    }smcr; 

    /* REG_0x07 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cc1ie                          : 1;  //0x7[0],CCR1比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc2ie                          : 1;  //0x7[1],CCR2比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc3ie                          : 1;  //0x7[2],CCR3比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc4ie                          : 1;  //0x7[3],CCR4比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc5ie                          : 1;  //0x7[4],CCR5比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc6ie                          : 1;  //0x7[5],CCR6比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc7ie                          : 1;  //0x7[6],CCR7比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc8ie                          : 1;  //0x7[7],CCR8比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  cc9ie                          : 1;  //0x7[8],CCR9比较成功中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  uie1                           : 1;  //0x7[9],TIM1更新事件中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  uie2                           : 1;  //0x7[10],TIM2更新事件中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  uie3                           : 1;  //0x7[11],TIM3更新事件中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  tie1                           : 1;  //0x7[12],触发输入中断使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  tie2                           : 1;  //0x7[13],触发输入DMA请求使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  tie3                           : 1;  //0x7[14],CCR1比较成功DMA请求使能  0 disable  1 enable,0x0,R/W
           volatile uint32_t  comie                          : 1;  //0x7[15],COM事件中断使能  0 disable  1 enble （霍尔传感器模式用）,0x0,R/W
           volatile uint32_t  reserved                       : 4;  //0x7[19:16], ,0x0,R/W
           volatile uint32_t  df_tim1                        : 4;  //0x7[23:20],数字滤波选择,0x0,R/W
           volatile uint32_t  df_tim2                        : 4;  //0x7[27:24], ,0x0,R/W
           volatile uint32_t  df_tim3                        : 4;  //0x7[31:28], ,0x0,R/W
        }; 
        uint32_t v; 
    }pwm_dier; 

    /* REG_0x08 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  cc1if                          : 1;  //0x8[0],CCR1比较成功中断标志位  ; 硬件置位：输出模式时比较成功时置位。对于双向计数模式有三种选项在CMS_TIM1中设定。 ; 输入模式时当TIM1的值被CCR1捕获时置位。; 软件清零,0x0,W1C
           volatile uint32_t  cc2if                          : 1;  //0x8[1],见CC1IF,0x0,W1C
           volatile uint32_t  cc3if                          : 1;  //0x8[2],见CC1IF,0x0,W1C
           volatile uint32_t  cc4if                          : 1;  //0x8[3],见CC1IF,0x0,W1C
           volatile uint32_t  cc5if                          : 1;  //0x8[4],见CC1IF,0x0,W1C
           volatile uint32_t  cc6if                          : 1;  //0x8[5],见CC1IF,0x0,W1C
           volatile uint32_t  cc7if                          : 1;  //0x8[6],见CC1IF,0x0,W1C
           volatile uint32_t  cc8if                          : 1;  //0x8[7],见CC1IF,0x0,W1C
           volatile uint32_t  cc9if                          : 1;  //0x8[8],见CC1IF,0x0,W1C
           volatile uint32_t  uif1                           : 1;  //0x8[9],TIM1更新事件中断标志位; 硬件置位：1.当计数器向上溢出或者向下溢出时置位;          2.当UG1位被软件置1或者被触发信号置1时置位  ; 软件清零,0x0,W1C
           volatile uint32_t  uif2                           : 1;  //0x8[10],见UIF1,0x0,W1C
           volatile uint32_t  uif3                           : 1;  //0x8[11],见UIF1,0x0,W1C
           volatile uint32_t  tif1                           : 1;  //0x8[12],触发中断标志位 ; 硬件置位：当工作在触发输入模式时，TRGI1的上升沿置位。; 软件清零,0x0,W1C
           volatile uint32_t  tif2                           : 1;  //0x8[13],触发中断标志位 ; 硬件置位：当工作在触发输入模式时，TRGI2的上升沿置位。; 软件清零,0x0,W1C
           volatile uint32_t  tif3                           : 1;  //0x8[14],触发中断标志位 ; 硬件置位：当工作在触发输入模式时，TRGI3的上升沿置位。; 软件清零,0x0,W1C
           volatile uint32_t  comif                          : 1;  //0x8[15],COM事件中断标志位（霍尔传感器模式用）; 硬件置位：; 软件清零,0x0,W1C
           volatile uint32_t  reserved                       :16;  //0x8[31:16], ,0x0,R/W
        }; 
        uint32_t v; 
    }pwm_sr; 

    /* REG_0x09 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  reserved0                       : 9;  //0x9[8:0], ,0x0,R/W
           volatile uint32_t  ug1                            : 1;  //0x9[9],软件产生更新事件 TIM1,0x0,W
           volatile uint32_t  ug2                            : 1;  //0x9[10],软件产生更新事件 TIM2,0x0,W
           volatile uint32_t  ug3                            : 1;  //0x9[11],软件产生更新事件 TIM3,0x0,W
           volatile uint32_t  tg1                            : 1;  //0x9[12],触发事件产生  软件/硬件置位  硬件自动清零,0x0,W
           volatile uint32_t  tg2                            : 1;  //0x9[13],触发事件产生  软件/硬件置位  硬件自动清零,0x0,W
           volatile uint32_t  tg3                            : 1;  //0x9[14],触发事件产生  软件/硬件置位  硬件自动清零,0x0,W
           volatile uint32_t  comg                           : 1;  //0x9[15],COM事件产生   硬件置位  硬件自动清零,0x0,W
           volatile uint32_t  dtm1                           : 2;  //0x9[17:16],CH1和CH2的死区时间模式：; 00 OCP在ocref上升沿延迟，OCN在ocref下降沿延迟; 01 OCN在ocref上升沿延迟，OCP在ocref下降沿延迟; 10 OCP在ocref上升沿延迟，OCP在ocref下降沿延迟; 11 OCN在ocref上升沿延迟，OCN在ocref下降沿延迟,0x0,R/W
           volatile uint32_t  dtm2                           : 2;  //0x9[19:18],CH3和CH4的死区时间模式：,0x0,R/W
           volatile uint32_t  dtm3                           : 2;  //0x9[21:20],CH5和CH6的死区时间模式：,0x0,R/W
           volatile uint32_t  reserved1                       :10;  //0x9[31:22], ,0x0,R/W
        }; 
        uint32_t v; 
    }pwm_edtr; 

    /* REG_0x0a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ch1p                           : 2;  //0xa[1:0],TI1的输入极性: ; 00 上升沿敏感(捕获/触发) TI1P不反向(encoder mode); 01 下降沿敏感（捕获/触发）  同时TI1P反向（encoder mode）; 10 上升下降沿都敏感（捕获/触发）,0x0,R/W
           volatile uint32_t  ch2p                           : 2;  //0xa[3:2],见CH1P,0x0,R/W
           volatile uint32_t  ch3p                           : 2;  //0xa[5:4],见CH1P,0x0,R/W
           volatile uint32_t  ch4p                           : 2;  //0xa[7:6],见CH1P,0x0,R/W
           volatile uint32_t  ch5p                           : 2;  //0xa[9:8],见CH1P,0x0,R/W
           volatile uint32_t  ch6p                           : 2;  //0xa[11:10],见CH1P,0x0,R/W
           volatile uint32_t  ch1e                           : 1;  //0xa[12],CH1E通道输入/输出使能,0x0,R/W
           volatile uint32_t  ch2e                           : 1;  //0xa[13],见CH1E,0x0,R/W
           volatile uint32_t  ch3e                           : 1;  //0xa[14],见CH1E,0x0,R/W
           volatile uint32_t  ch4e                           : 1;  //0xa[15],见CH1E,0x0,R/W
           volatile uint32_t  ch5e                           : 1;  //0xa[16],见CH1E,0x0,R/W
           volatile uint32_t  ch6e                           : 1;  //0xa[17],见CH1E,0x0,R/W
           volatile uint32_t  tim1ccm                        : 1;  //0xa[18],TIM1的比较捕获模式: 0输出比较模式 1输入捕获模式,0x0,R/W
           volatile uint32_t  tim2ccm                        : 1;  //0xa[19],见TIM1CCM,0x0,R/W
           volatile uint32_t  tim3ccm                        : 1;  //0xa[20],见TIM1CCM,0x0,R/W
           volatile uint32_t  oc1m                           : 3;  //0xa[23:21],TIM1的比较输出模式选择：; 000：OC1REF不受计数值的影响保持不变; 001：OC1REF碰到AAR不翻转，碰到CCRx翻转; 010：OC1REF set 1; 011：OC1REF碰到CCRx时翻转，碰到AAR也翻转->ARR; 100：OC1REF set 0,0x0,R/W
           volatile uint32_t  oc2m                           : 3;  //0xa[26:24],见OC1M,0x0,R/W
           volatile uint32_t  oc3m                           : 3;  //0xa[29:27],见OC1M,0x0,R/W
           volatile uint32_t  reserved                       : 2;  //0xa[31:30], ,0x0,R/W
        }; 
        uint32_t v; 
    }pwm_ccmr; 

    /* REG_0x0b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim1_cnt                       :32;  //0xb[31:0],TIM1的计数器回读值,0x0,RO
        }; 
        uint32_t v; 
    }tim1_cnt; 

    /* REG_0x0c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim2_cnt                       :32;  //0xc[31:0],TIM2的计数器回读值,0x0,RO
        }; 
        uint32_t v; 
    }tim2_cnt; 

    /* REG_0x0d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim3_cnt                       :32;  //0xd[31:0],TIM3的计数器回读值,0x0,RO
        }; 
        uint32_t v; 
    }tim3_cnt; 

    /* REG_0x0e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  psc1                           : 8;  //0xe[7:0],TIM1的时钟预分频值 从1到256,0x0,R/W
           volatile uint32_t  psc2                           : 8;  //0xe[15:8],TIM2的时钟预分频值 从1到256,0x0,R/W
           volatile uint32_t  psc3                           : 8;  //0xe[23:16],TIM3的时钟预分频值 从1到256,0x0,R/W
           volatile uint32_t  reserved                       : 8;  //0xe[31:24], ,0x0,R/W
        }; 
        uint32_t v; 
    }prescaler; 

    /* REG_0x0f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim1_arr                       :32;  //0xf[31:0],TIM1的定时器计数终点值preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }tim1_arr; 

    /* REG_0x10 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim2_arr                       :32;  //0x10[31:0],TIM2的定时器计数终点值preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }tim2_arr; 

    /* REG_0x11 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim3_arr                       :32;  //0x11[31:0],TIM3的定时器计数终点值preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }tim3_arr; 

    /* REG_0x12 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim1_rcr                       :32;  //0x12[31:0],TIM1的重复计数器设定值,在RCR+1次溢出后产生更新事件,0x0,R/W
        }; 
        uint32_t v; 
    }tim1_rcr; 

    /* REG_0x13 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim2_rcr                       :32;  //0x13[31:0],TIM2的重复计数器设定值,0x0,R/W
        }; 
        uint32_t v; 
    }tim2_rcr; 

    /* REG_0x14 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim3_rcr                       :32;  //0x14[31:0],TIM3的重复计数器设定值,0x0,R/W
        }; 
        uint32_t v; 
    }tim3_rcr; 

    /* REG_0x15 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr1                           :32;  //0x15[31:0],比较捕获寄存器CCR1的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr1; 

    /* REG_0x16 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr2                           :32;  //0x16[31:0],比较捕获寄存器CCR2的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr2; 

    /* REG_0x17 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr3                           :32;  //0x17[31:0],比较捕获寄存器CCR3的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr3; 

    /* REG_0x18 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr4                           :32;  //0x18[31:0],比较捕获寄存器CCR4的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr4; 

    /* REG_0x19 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr5                           :32;  //0x19[31:0],比较捕获寄存器CCR5的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr5; 

    /* REG_0x1a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr6                           :32;  //0x1a[31:0],比较捕获寄存器CCR6的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr6; 

    /* REG_0x1b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr7                           :32;  //0x1b[31:0],比较捕获寄存器CCR7的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr7; 

    /* REG_0x1c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr8                           :32;  //0x1c[31:0],比较捕获寄存器CCR8的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr8; 

    /* REG_0x1d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr9                           :32;  //0x1d[31:0],比较捕获寄存器CCR9的preload寄存器,0x0,R/W
        }; 
        uint32_t v; 
    }ccr9; 

    /* REG_0x1e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  dt1                            :10;  //0x1e[9:0],out1与out0死区时间设置：0到0x400个周期,0x0,R/W
           volatile uint32_t  dt2                            :10;  //0x1e[19:10],out3与out2死区时间设置：0到0x400个周期,0x0,R/W
           volatile uint32_t  dt3                            :10;  //0x1e[29:20],out5与out4死区时间设置：0到0x400个周期,0x0,R/W
           volatile uint32_t  reserved                       : 2;  //0x1e[31:30], ,0x0,R/W
        }; 
        uint32_t v; 
    }dt; 

    /* REG_0x1f */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim1_arr_shad                  :32;  //0x1f[31:0],TIM1的定时器计数终点值shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }tim1_arr_shad; 

    /* REG_0x20 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim2_arr_shad                  :32;  //0x20[31:0],TIM2的定时器计数终点值shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }tim2_arr_shad; 

    /* REG_0x21 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim3_arr_shad                  :32;  //0x21[31:0],TIM3的定时器计数终点值shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }tim3_arr_shad; 

    /* REG_0x22 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim1_rcr_shad                  :32;  //0x22[31:0],TIM1的重复计数器设定值,在RCR+1次溢出后产生更新事件,0x0,RO
        }; 
        uint32_t v; 
    }tim1_rcr_shad; 

    /* REG_0x23 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim2_rcr_shad                  :32;  //0x23[31:0],TIM2的重复计数器设定值,0x0,RO
        }; 
        uint32_t v; 
    }tim2_rcr_shad; 

    /* REG_0x24 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  tim3_rcr_shad                  :32;  //0x24[31:0],TIM3的重复计数器设定值,0x0,RO
        }; 
        uint32_t v; 
    }tim3_rcr_shad; 

    /* REG_0x25 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr1_shad                      :32;  //0x25[31:0],比较捕获寄存器CCR1的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr1_shad; 

    /* REG_0x26 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr2_shad                      :32;  //0x26[31:0],比较捕获寄存器CCR2的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr2_shad; 

    /* REG_0x27 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr3_shad                      :32;  //0x27[31:0],比较捕获寄存器CCR3的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr3_shad; 

    /* REG_0x28 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr4_shad                      :32;  //0x28[31:0],比较捕获寄存器CCR4的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr4_shad; 

    /* REG_0x29 */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr5_shad                      :32;  //0x29[31:0],比较捕获寄存器CCR5的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr5_shad; 

    /* REG_0x2a */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr6_shad                      :32;  //0x2a[31:0],比较捕获寄存器CCR6的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr6_shad; 

    /* REG_0x2b */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr7_shad                      :32;  //0x2b[31:0],比较捕获寄存器CCR7的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr7_shad; 

    /* REG_0x2c */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr8_shad                      :32;  //0x2c[31:0],比较捕获寄存器CCR8的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr8_shad; 

    /* REG_0x2d */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ccr9_shad                      :32;  //0x2d[31:0],比较捕获寄存器CCR9的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }ccr9_shad; 

    /* REG_0x2e */
    union 
    { 
        struct 
        { 
           volatile uint32_t  ch6e_shad                      : 1;  //0x2e[0],channel使能寄存器ch6e的shadow寄存器,0x0,RO
           volatile uint32_t  ch5e_shad                      : 1;  //0x2e[1],channel使能寄存器ch5e的shadow寄存器,0x0,RO
           volatile uint32_t  ch4e_shad                      : 1;  //0x2e[2],channel使能寄存器ch4e的shadow寄存器,0x0,RO
           volatile uint32_t  ch3e_shad                      : 1;  //0x2e[3],channel使能寄存器ch3e的shadow寄存器,0x0,RO
           volatile uint32_t  ch2e_shad                      : 1;  //0x2e[4],channel使能寄存器ch2e的shadow寄存器,0x0,RO
           volatile uint32_t  ch1e_shad                      : 1;  //0x2e[5],channel使能寄存器ch1e的shadow寄存器,0x0,RO
           volatile uint32_t  oc1m_shad                      : 3;  //0x2e[8:6],TIM1输出比较模式寄存器oc1m的shadow寄存器,0x0,RO
           volatile uint32_t  oc2m_shad                      : 3;  //0x2e[11:9],TIM2输出比较模式寄存器oc2m的shadow寄存器,0x0,RO
           volatile uint32_t  oc3m_shad                      : 3;  //0x2e[14:12],TIM3输出比较模式寄存器oc3m的shadow寄存器,0x0,RO
           volatile uint32_t  reserved                       :17;  //0x2e[31:15],比较捕获寄存器CCR9的shadow寄存器,0x0,RO
        }; 
        uint32_t v; 
    }chxe_shad; 

} pwm_hw_t;

#ifdef __cplusplus 
}                  
#endif             
