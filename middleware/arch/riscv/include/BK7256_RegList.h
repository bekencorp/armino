#ifndef _BK7256_REGLIST_H_
#define _BK7256_REGLIST_H_


//************************************************************//
//GPIO_MAP
//************************************************************//
#define BASEADDR_GPIO_MAP                                       

//************************************************************//
//SYSTEM
//************************************************************//
#define BASEADDR_SYSTEM                                         0x44010000
//addSYSTEM_Reg0x0
#define addSYSTEM_Reg0x0                                        *((volatile unsigned long *) (0x44010000+0x0*4))

//addSYSTEM_Reg0x1
#define addSYSTEM_Reg0x1                                        *((volatile unsigned long *) (0x44010000+0x1*4))


#define BASEADDR_PWM0											0x44840000

#define BASEADDR_PWM1											0x44840040

#define BASEADDR_PWM2											0x44840080

//addSYSTEM_Reg0x2
#define addSYSTEM_Reg0x2                                        *((volatile unsigned long *) (0x44010000+0x2*4))
#define posSYSTEM_Reg0x2_cpu1_pwr_dw                            9
#define bitSYSTEM_Reg0x2_cpu1_pwr_dw                            0x200
#define set_SYSTEM_Reg0x2_cpu1_pwr_dw(val)                      addSYSTEM_Reg0x2 = ((addSYSTEM_Reg0x2 & (~0x200)) | ((val) << 9))
#define setf_SYSTEM_Reg0x2_cpu1_pwr_dw                          addSYSTEM_Reg0x2 |= 0x200
#define clrf_SYSTEM_Reg0x2_cpu1_pwr_dw                          addSYSTEM_Reg0x2 &= ~0x200
#define get_SYSTEM_Reg0x2_cpu1_pwr_dw                           ((addSYSTEM_Reg0x2 & 0x200) >> 9)

#define posSYSTEM_Reg0x2_cpu0_pwr_dw                            8
#define bitSYSTEM_Reg0x2_cpu0_pwr_dw                            0x100
#define set_SYSTEM_Reg0x2_cpu0_pwr_dw(val)                      addSYSTEM_Reg0x2 = ((addSYSTEM_Reg0x2 & (~0x100)) | ((val) << 8))
#define setf_SYSTEM_Reg0x2_cpu0_pwr_dw                          addSYSTEM_Reg0x2 |= 0x100
#define clrf_SYSTEM_Reg0x2_cpu0_pwr_dw                          addSYSTEM_Reg0x2 &= ~0x100
#define get_SYSTEM_Reg0x2_cpu0_pwr_dw                           ((addSYSTEM_Reg0x2 & 0x100) >> 8)

#define posSYSTEM_Reg0x2_cpu1_sw_reset                          5
#define bitSYSTEM_Reg0x2_cpu1_sw_reset                          0x20
#define set_SYSTEM_Reg0x2_cpu1_sw_reset(val)                    addSYSTEM_Reg0x2 = ((addSYSTEM_Reg0x2 & (~0x20)) | ((val) << 5))
#define setf_SYSTEM_Reg0x2_cpu1_sw_reset                        addSYSTEM_Reg0x2 |= 0x20
#define clrf_SYSTEM_Reg0x2_cpu1_sw_reset                        addSYSTEM_Reg0x2 &= ~0x20
#define get_SYSTEM_Reg0x2_cpu1_sw_reset                         ((addSYSTEM_Reg0x2 & 0x20) >> 5)

#define posSYSTEM_Reg0x2_cpu0_sw_reset                          4
#define bitSYSTEM_Reg0x2_cpu0_sw_reset                          0x10
#define set_SYSTEM_Reg0x2_cpu0_sw_reset(val)                    addSYSTEM_Reg0x2 = ((addSYSTEM_Reg0x2 & (~0x10)) | ((val) << 4))
#define setf_SYSTEM_Reg0x2_cpu0_sw_reset                        addSYSTEM_Reg0x2 |= 0x10
#define clrf_SYSTEM_Reg0x2_cpu0_sw_reset                        addSYSTEM_Reg0x2 &= ~0x10
#define get_SYSTEM_Reg0x2_cpu0_sw_reset                         ((addSYSTEM_Reg0x2 & 0x10) >> 4)

#define posSYSTEM_Reg0x2_core1_halted                           1
#define bitSYSTEM_Reg0x2_core1_halted                           0x2
#define set_SYSTEM_Reg0x2_core1_halted(val)                     addSYSTEM_Reg0x2 = ((addSYSTEM_Reg0x2 & (~0x2)) | ((val) << 1))
#define setf_SYSTEM_Reg0x2_core1_halted                         addSYSTEM_Reg0x2 |= 0x2
#define clrf_SYSTEM_Reg0x2_core1_halted                         addSYSTEM_Reg0x2 &= ~0x2
#define get_SYSTEM_Reg0x2_core1_halted                          ((addSYSTEM_Reg0x2 & 0x2) >> 1)

#define posSYSTEM_Reg0x2_core0_halted                           0
#define bitSYSTEM_Reg0x2_core0_halted                           0x1
#define set_SYSTEM_Reg0x2_core0_halted(val)                     addSYSTEM_Reg0x2 = ((addSYSTEM_Reg0x2 & (~0x1)) | ((val) << 0))
#define setf_SYSTEM_Reg0x2_core0_halted                         addSYSTEM_Reg0x2 |= 0x1
#define clrf_SYSTEM_Reg0x2_core0_halted                         addSYSTEM_Reg0x2 &= ~0x1
#define get_SYSTEM_Reg0x2_core0_halted                          (addSYSTEM_Reg0x2 & 0x1)

//addSYSTEM_Reg0x3
#define addSYSTEM_Reg0x3                                        *((volatile unsigned long *) (0x44010000+0x3*4))
#define posSYSTEM_Reg0x3_boot_mode                              0
#define bitSYSTEM_Reg0x3_boot_mode                              0x1
#define set_SYSTEM_Reg0x3_boot_mode(val)                        addSYSTEM_Reg0x3 = ((addSYSTEM_Reg0x3 & (~0x1)) | ((val) << 0))
#define setf_SYSTEM_Reg0x3_boot_mode                            addSYSTEM_Reg0x3 |= 0x1
#define clrf_SYSTEM_Reg0x3_boot_mode                            addSYSTEM_Reg0x3 &= ~0x1
#define get_SYSTEM_Reg0x3_boot_mode                             (addSYSTEM_Reg0x3 & 0x1)

//addSYSTEM_Reg0x4
#define addSYSTEM_Reg0x4                                        *((volatile unsigned long *) (0x44010000+0x4*4))
#define posSYSTEM_Reg0x4_cpu0_offset                            8
#define bitSYSTEM_Reg0x4_cpu0_offset                            0xFFFFFF00
#define set_SYSTEM_Reg0x4_cpu0_offset(val)                      addSYSTEM_Reg0x4 = ((addSYSTEM_Reg0x4 & (~0xFFFFFF00)) | ((val) << 8))
#define get_SYSTEM_Reg0x4_cpu0_offset                           ((addSYSTEM_Reg0x4 & 0xFFFFFF00) >> 8)

#define posSYSTEM_Reg0x4_cpu0_clk_div                           4
#define bitSYSTEM_Reg0x4_cpu0_clk_div                           0xF0
#define set_SYSTEM_Reg0x4_cpu0_clk_div(val)                     addSYSTEM_Reg0x4 = ((addSYSTEM_Reg0x4 & (~0xF0)) | ((val) << 4))
#define get_SYSTEM_Reg0x4_cpu0_clk_div                          ((addSYSTEM_Reg0x4 & 0xF0) >> 4)

#define posSYSTEM_Reg0x4_cpu0_halt                              3
#define bitSYSTEM_Reg0x4_cpu0_halt                              0x8
#define set_SYSTEM_Reg0x4_cpu0_halt(val)                        addSYSTEM_Reg0x4 = ((addSYSTEM_Reg0x4 & (~0x8)) | ((val) << 3))
#define setf_SYSTEM_Reg0x4_cpu0_halt                            addSYSTEM_Reg0x4 |= 0x8
#define clrf_SYSTEM_Reg0x4_cpu0_halt                            addSYSTEM_Reg0x4 &= ~0x8
#define get_SYSTEM_Reg0x4_cpu0_halt                             ((addSYSTEM_Reg0x4 & 0x8) >> 3)

#define posSYSTEM_Reg0x4_cpu0_int_mask                          2
#define bitSYSTEM_Reg0x4_cpu0_int_mask                          0x4
#define set_SYSTEM_Reg0x4_cpu0_int_mask(val)                    addSYSTEM_Reg0x4 = ((addSYSTEM_Reg0x4 & (~0x4)) | ((val) << 2))
#define setf_SYSTEM_Reg0x4_cpu0_int_mask                        addSYSTEM_Reg0x4 |= 0x4
#define clrf_SYSTEM_Reg0x4_cpu0_int_mask                        addSYSTEM_Reg0x4 &= ~0x4
#define get_SYSTEM_Reg0x4_cpu0_int_mask                         ((addSYSTEM_Reg0x4 & 0x4) >> 2)

#define posSYSTEM_Reg0x4_cpu0_pwr_dw                            1
#define bitSYSTEM_Reg0x4_cpu0_pwr_dw                            0x2
#define set_SYSTEM_Reg0x4_cpu0_pwr_dw(val)                      addSYSTEM_Reg0x4 = ((addSYSTEM_Reg0x4 & (~0x2)) | ((val) << 1))
#define setf_SYSTEM_Reg0x4_cpu0_pwr_dw                          addSYSTEM_Reg0x4 |= 0x2
#define clrf_SYSTEM_Reg0x4_cpu0_pwr_dw                          addSYSTEM_Reg0x4 &= ~0x2
#define get_SYSTEM_Reg0x4_cpu0_pwr_dw                           ((addSYSTEM_Reg0x4 & 0x2) >> 1)

#define posSYSTEM_Reg0x4_cpu0_sw_rst                            0
#define bitSYSTEM_Reg0x4_cpu0_sw_rst                            0x1
#define set_SYSTEM_Reg0x4_cpu0_sw_rst(val)                      addSYSTEM_Reg0x4 = ((addSYSTEM_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_SYSTEM_Reg0x4_cpu0_sw_rst                          addSYSTEM_Reg0x4 |= 0x1
#define clrf_SYSTEM_Reg0x4_cpu0_sw_rst                          addSYSTEM_Reg0x4 &= ~0x1
#define get_SYSTEM_Reg0x4_cpu0_sw_rst                           (addSYSTEM_Reg0x4 & 0x1)

//addSYSTEM_Reg0x5
#define addSYSTEM_Reg0x5                                        *((volatile unsigned long *) (0x44010000+0x5*4))
#define posSYSTEM_Reg0x5_cpu1_offset                            8
#define bitSYSTEM_Reg0x5_cpu1_offset                            0xFFFFFF00
#define set_SYSTEM_Reg0x5_cpu1_offset(val)                      addSYSTEM_Reg0x5 = ((addSYSTEM_Reg0x5 & (~0xFFFFFF00)) | ((val) << 8))
#define get_SYSTEM_Reg0x5_cpu1_offset                           ((addSYSTEM_Reg0x5 & 0xFFFFFF00) >> 8)

#define posSYSTEM_Reg0x5_cpu1_clk_div                           4
#define bitSYSTEM_Reg0x5_cpu1_clk_div                           0xF0
#define set_SYSTEM_Reg0x5_cpu1_clk_div(val)                     addSYSTEM_Reg0x5 = ((addSYSTEM_Reg0x5 & (~0xF0)) | ((val) << 4))
#define get_SYSTEM_Reg0x5_cpu1_clk_div                          ((addSYSTEM_Reg0x5 & 0xF0) >> 4)

#define posSYSTEM_Reg0x5_cpu1_halt                              3
#define bitSYSTEM_Reg0x5_cpu1_halt                              0x8
#define set_SYSTEM_Reg0x5_cpu1_halt(val)                        addSYSTEM_Reg0x5 = ((addSYSTEM_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_SYSTEM_Reg0x5_cpu1_halt                            addSYSTEM_Reg0x5 |= 0x8
#define clrf_SYSTEM_Reg0x5_cpu1_halt                            addSYSTEM_Reg0x5 &= ~0x8
#define get_SYSTEM_Reg0x5_cpu1_halt                             ((addSYSTEM_Reg0x5 & 0x8) >> 3)

#define posSYSTEM_Reg0x5_cpu1_int_mask                          2
#define bitSYSTEM_Reg0x5_cpu1_int_mask                          0x4
#define set_SYSTEM_Reg0x5_cpu1_int_mask(val)                    addSYSTEM_Reg0x5 = ((addSYSTEM_Reg0x5 & (~0x4)) | ((val) << 2))
#define setf_SYSTEM_Reg0x5_cpu1_int_mask                        addSYSTEM_Reg0x5 |= 0x4
#define clrf_SYSTEM_Reg0x5_cpu1_int_mask                        addSYSTEM_Reg0x5 &= ~0x4
#define get_SYSTEM_Reg0x5_cpu1_int_mask                         ((addSYSTEM_Reg0x5 & 0x4) >> 2)

#define posSYSTEM_Reg0x5_cpu1_pwr_dw                            1
#define bitSYSTEM_Reg0x5_cpu1_pwr_dw                            0x2
#define set_SYSTEM_Reg0x5_cpu1_pwr_dw(val)                      addSYSTEM_Reg0x5 = ((addSYSTEM_Reg0x5 & (~0x2)) | ((val) << 1))
#define setf_SYSTEM_Reg0x5_cpu1_pwr_dw                          addSYSTEM_Reg0x5 |= 0x2
#define clrf_SYSTEM_Reg0x5_cpu1_pwr_dw                          addSYSTEM_Reg0x5 &= ~0x2
#define get_SYSTEM_Reg0x5_cpu1_pwr_dw                           ((addSYSTEM_Reg0x5 & 0x2) >> 1)

#define posSYSTEM_Reg0x5_cpu1_sw_rst                            0
#define bitSYSTEM_Reg0x5_cpu1_sw_rst                            0x1
#define set_SYSTEM_Reg0x5_cpu1_sw_rst(val)                      addSYSTEM_Reg0x5 = ((addSYSTEM_Reg0x5 & (~0x1)) | ((val) << 0))
#define setf_SYSTEM_Reg0x5_cpu1_sw_rst                          addSYSTEM_Reg0x5 |= 0x1
#define clrf_SYSTEM_Reg0x5_cpu1_sw_rst                          addSYSTEM_Reg0x5 &= ~0x1
#define get_SYSTEM_Reg0x5_cpu1_sw_rst                           (addSYSTEM_Reg0x5 & 0x1)

//addSYSTEM_Reg0x6
#define addSYSTEM_Reg0x6                                        *((volatile unsigned long *) (0x44010000+0x6*4))

//addSYSTEM_Reg0x8
#define addSYSTEM_Reg0x8                                        *((volatile unsigned long *) (0x44010000+0x8*4))
#define posSYSTEM_Reg0x8_clkdiv_core                            0
#define bitSYSTEM_Reg0x8_clkdiv_core                            0xF
#define set_SYSTEM_Reg0x8_clkdiv_core(val)                      addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0xF)) | ((val) << 0))
#define get_SYSTEM_Reg0x8_clkdiv_core                           (addSYSTEM_Reg0x8 & 0xF)

#define posSYSTEM_Reg0x8_cksel_core                             4
#define bitSYSTEM_Reg0x8_cksel_core                             0x30
#define set_SYSTEM_Reg0x8_cksel_core(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x30)) | ((val) << 4))
#define get_SYSTEM_Reg0x8_cksel_core                            ((addSYSTEM_Reg0x8 & 0x30) >> 4)

#define posSYSTEM_Reg0x8_clkdiv_bus                             6
#define bitSYSTEM_Reg0x8_clkdiv_bus                             0x40
#define set_SYSTEM_Reg0x8_clkdiv_bus(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x40)) | ((val) << 6))
#define setf_SYSTEM_Reg0x8_clkdiv_bus                           addSYSTEM_Reg0x8 |= 0x40
#define clrf_SYSTEM_Reg0x8_clkdiv_bus                           addSYSTEM_Reg0x8 &= ~0x40
#define get_SYSTEM_Reg0x8_clkdiv_bus                            ((addSYSTEM_Reg0x8 & 0x40) >> 6)

#define posSYSTEM_Reg0x8_clkdiv_uart0                           8
#define bitSYSTEM_Reg0x8_clkdiv_uart0                           0x300
#define set_SYSTEM_Reg0x8_clkdiv_uart0(val)                     addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x300)) | ((val) << 8))
#define get_SYSTEM_Reg0x8_clkdiv_uart0                          ((addSYSTEM_Reg0x8 & 0x300) >> 8)

#define posSYSTEM_Reg0x8_clksel_uart0                           10
#define bitSYSTEM_Reg0x8_clksel_uart0                           0x400
#define set_SYSTEM_Reg0x8_clksel_uart0(val)                     addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x400)) | ((val) << 10))
#define setf_SYSTEM_Reg0x8_clksel_uart0                         addSYSTEM_Reg0x8 |= 0x400
#define clrf_SYSTEM_Reg0x8_clksel_uart0                         addSYSTEM_Reg0x8 &= ~0x400
#define get_SYSTEM_Reg0x8_clksel_uart0                          ((addSYSTEM_Reg0x8 & 0x400) >> 10)

#define posSYSTEM_Reg0x8_clkdiv_uart1                           11
#define bitSYSTEM_Reg0x8_clkdiv_uart1                           0x1800
#define set_SYSTEM_Reg0x8_clkdiv_uart1(val)                     addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x1800)) | ((val) << 11))
#define get_SYSTEM_Reg0x8_clkdiv_uart1                          ((addSYSTEM_Reg0x8 & 0x1800) >> 11)

#define posSYSTEM_Reg0x8_cksel_uart1                            13
#define bitSYSTEM_Reg0x8_cksel_uart1                            0x2000
#define set_SYSTEM_Reg0x8_cksel_uart1(val)                      addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x2000)) | ((val) << 13))
#define setf_SYSTEM_Reg0x8_cksel_uart1                          addSYSTEM_Reg0x8 |= 0x2000
#define clrf_SYSTEM_Reg0x8_cksel_uart1                          addSYSTEM_Reg0x8 &= ~0x2000
#define get_SYSTEM_Reg0x8_cksel_uart1                           ((addSYSTEM_Reg0x8 & 0x2000) >> 13)

#define posSYSTEM_Reg0x8_clkdiv_uart2                           14
#define bitSYSTEM_Reg0x8_clkdiv_uart2                           0xC000
#define set_SYSTEM_Reg0x8_clkdiv_uart2(val)                     addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0xC000)) | ((val) << 14))
#define get_SYSTEM_Reg0x8_clkdiv_uart2                          ((addSYSTEM_Reg0x8 & 0xC000) >> 14)

#define posSYSTEM_Reg0x8_cksel_uart2                            16
#define bitSYSTEM_Reg0x8_cksel_uart2                            0x10000
#define set_SYSTEM_Reg0x8_cksel_uart2(val)                      addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x10000)) | ((val) << 16))
#define setf_SYSTEM_Reg0x8_cksel_uart2                          addSYSTEM_Reg0x8 |= 0x10000
#define clrf_SYSTEM_Reg0x8_cksel_uart2                          addSYSTEM_Reg0x8 &= ~0x10000
#define get_SYSTEM_Reg0x8_cksel_uart2                           ((addSYSTEM_Reg0x8 & 0x10000) >> 16)

#define posSYSTEM_Reg0x8_cksel_sadc                             17
#define bitSYSTEM_Reg0x8_cksel_sadc                             0x20000
#define set_SYSTEM_Reg0x8_cksel_sadc(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x20000)) | ((val) << 17))
#define setf_SYSTEM_Reg0x8_cksel_sadc                           addSYSTEM_Reg0x8 |= 0x20000
#define clrf_SYSTEM_Reg0x8_cksel_sadc                           addSYSTEM_Reg0x8 &= ~0x20000
#define get_SYSTEM_Reg0x8_cksel_sadc                            ((addSYSTEM_Reg0x8 & 0x20000) >> 17)

#define posSYSTEM_Reg0x8_cksel_pwm0                             18
#define bitSYSTEM_Reg0x8_cksel_pwm0                             0x40000
#define set_SYSTEM_Reg0x8_cksel_pwm0(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x40000)) | ((val) << 18))
#define setf_SYSTEM_Reg0x8_cksel_pwm0                           addSYSTEM_Reg0x8 |= 0x40000
#define clrf_SYSTEM_Reg0x8_cksel_pwm0                           addSYSTEM_Reg0x8 &= ~0x40000
#define get_SYSTEM_Reg0x8_cksel_pwm0                            ((addSYSTEM_Reg0x8 & 0x40000) >> 18)

#define posSYSTEM_Reg0x8_cksel_pwm1                             19
#define bitSYSTEM_Reg0x8_cksel_pwm1                             0x80000
#define set_SYSTEM_Reg0x8_cksel_pwm1(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x80000)) | ((val) << 19))
#define setf_SYSTEM_Reg0x8_cksel_pwm1                           addSYSTEM_Reg0x8 |= 0x80000
#define clrf_SYSTEM_Reg0x8_cksel_pwm1                           addSYSTEM_Reg0x8 &= ~0x80000
#define get_SYSTEM_Reg0x8_cksel_pwm1                            ((addSYSTEM_Reg0x8 & 0x80000) >> 19)

#define posSYSTEM_Reg0x8_cksel_tim0                             20
#define bitSYSTEM_Reg0x8_cksel_tim0                             0x100000
#define set_SYSTEM_Reg0x8_cksel_tim0(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x100000)) | ((val) << 20))
#define setf_SYSTEM_Reg0x8_cksel_tim0                           addSYSTEM_Reg0x8 |= 0x100000
#define clrf_SYSTEM_Reg0x8_cksel_tim0                           addSYSTEM_Reg0x8 &= ~0x100000
#define get_SYSTEM_Reg0x8_cksel_tim0                            ((addSYSTEM_Reg0x8 & 0x100000) >> 20)

#define posSYSTEM_Reg0x8_cksel_tim1                             21
#define bitSYSTEM_Reg0x8_cksel_tim1                             0x200000
#define set_SYSTEM_Reg0x8_cksel_tim1(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x200000)) | ((val) << 21))
#define setf_SYSTEM_Reg0x8_cksel_tim1                           addSYSTEM_Reg0x8 |= 0x200000
#define clrf_SYSTEM_Reg0x8_cksel_tim1                           addSYSTEM_Reg0x8 &= ~0x200000
#define get_SYSTEM_Reg0x8_cksel_tim1                            ((addSYSTEM_Reg0x8 & 0x200000) >> 21)

#define posSYSTEM_Reg0x8_cksel_tim2                             22
#define bitSYSTEM_Reg0x8_cksel_tim2                             0x400000
#define set_SYSTEM_Reg0x8_cksel_tim2(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x400000)) | ((val) << 22))
#define setf_SYSTEM_Reg0x8_cksel_tim2                           addSYSTEM_Reg0x8 |= 0x400000
#define clrf_SYSTEM_Reg0x8_cksel_tim2                           addSYSTEM_Reg0x8 &= ~0x400000
#define get_SYSTEM_Reg0x8_cksel_tim2                            ((addSYSTEM_Reg0x8 & 0x400000) >> 22)

#define posSYSTEM_Reg0x8_cksel_i2s                              24
#define bitSYSTEM_Reg0x8_cksel_i2s                              0x1000000
#define set_SYSTEM_Reg0x8_cksel_i2s(val)                        addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x1000000)) | ((val) << 24))
#define setf_SYSTEM_Reg0x8_cksel_i2s                            addSYSTEM_Reg0x8 |= 0x1000000
#define clrf_SYSTEM_Reg0x8_cksel_i2s                            addSYSTEM_Reg0x8 &= ~0x1000000
#define get_SYSTEM_Reg0x8_cksel_i2s                             ((addSYSTEM_Reg0x8 & 0x1000000) >> 24)

#define posSYSTEM_Reg0x8_cksel_aud                              25
#define bitSYSTEM_Reg0x8_cksel_aud                              0x2000000
#define set_SYSTEM_Reg0x8_cksel_aud(val)                        addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x2000000)) | ((val) << 25))
#define setf_SYSTEM_Reg0x8_cksel_aud                            addSYSTEM_Reg0x8 |= 0x2000000
#define clrf_SYSTEM_Reg0x8_cksel_aud                            addSYSTEM_Reg0x8 &= ~0x2000000
#define get_SYSTEM_Reg0x8_cksel_aud                             ((addSYSTEM_Reg0x8 & 0x2000000) >> 25)

#define posSYSTEM_Reg0x8_clkdiv_jpeg                            26
#define bitSYSTEM_Reg0x8_clkdiv_jpeg                            0x3C000000
#define set_SYSTEM_Reg0x8_clkdiv_jpeg(val)                      addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x3C000000)) | ((val) << 26))
#define get_SYSTEM_Reg0x8_clkdiv_jpeg                           ((addSYSTEM_Reg0x8 & 0x3C000000) >> 26)

#define posSYSTEM_Reg0x8_cksel_jpeg                             30
#define bitSYSTEM_Reg0x8_cksel_jpeg                             0x40000000
#define set_SYSTEM_Reg0x8_cksel_jpeg(val)                       addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x40000000)) | ((val) << 30))
#define setf_SYSTEM_Reg0x8_cksel_jpeg                           addSYSTEM_Reg0x8 |= 0x40000000
#define clrf_SYSTEM_Reg0x8_cksel_jpeg                           addSYSTEM_Reg0x8 &= ~0x40000000
#define get_SYSTEM_Reg0x8_cksel_jpeg                            ((addSYSTEM_Reg0x8 & 0x40000000) >> 30)

#define posSYSTEM_Reg0x8_clkdiv_disp_l                          31
#define bitSYSTEM_Reg0x8_clkdiv_disp_l                          0x80000000
#define set_SYSTEM_Reg0x8_clkdiv_disp_l(val)                    addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~0x80000000)) | ((val) << 31))
#define setf_SYSTEM_Reg0x8_clkdiv_disp_l                        addSYSTEM_Reg0x8 |= 0x80000000
#define clrf_SYSTEM_Reg0x8_clkdiv_disp_l                        addSYSTEM_Reg0x8 &= ~0x80000000
#define get_SYSTEM_Reg0x8_clkdiv_disp_l                         ((addSYSTEM_Reg0x8 & 0x80000000) >> 31)

//addSYSTEM_Reg0x9
#define addSYSTEM_Reg0x9                                        *((volatile unsigned long *) (0x44010000+0x9*4))
#define posSYSTEM_Reg0x9_clkdiv_disp_h                          0
#define bitSYSTEM_Reg0x9_clkdiv_disp_h                          0x7
#define set_SYSTEM_Reg0x9_clkdiv_disp_h(val)                    addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x7)) | ((val) << 0))
#define get_SYSTEM_Reg0x9_clkdiv_disp_h                         (addSYSTEM_Reg0x9 & 0x7)

#define posSYSTEM_Reg0x9_cksel_disp                             3
#define bitSYSTEM_Reg0x9_cksel_disp                             0x8
#define set_SYSTEM_Reg0x9_cksel_disp(val)                       addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x8)) | ((val) << 3))
#define setf_SYSTEM_Reg0x9_cksel_disp                           addSYSTEM_Reg0x9 |= 0x8
#define clrf_SYSTEM_Reg0x9_cksel_disp                           addSYSTEM_Reg0x9 &= ~0x8
#define get_SYSTEM_Reg0x9_cksel_disp                            ((addSYSTEM_Reg0x9 & 0x8) >> 3)

#define posSYSTEM_Reg0x9_ckdiv_psram                            4
#define bitSYSTEM_Reg0x9_ckdiv_psram                            0x10
#define set_SYSTEM_Reg0x9_ckdiv_psram(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x10)) | ((val) << 4))
#define setf_SYSTEM_Reg0x9_ckdiv_psram                          addSYSTEM_Reg0x9 |= 0x10
#define clrf_SYSTEM_Reg0x9_ckdiv_psram                          addSYSTEM_Reg0x9 &= ~0x10
#define get_SYSTEM_Reg0x9_ckdiv_psram                           ((addSYSTEM_Reg0x9 & 0x10) >> 4)

#define posSYSTEM_Reg0x9_cksel_psram                            5
#define bitSYSTEM_Reg0x9_cksel_psram                            0x20
#define set_SYSTEM_Reg0x9_cksel_psram(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x20)) | ((val) << 5))
#define setf_SYSTEM_Reg0x9_cksel_psram                          addSYSTEM_Reg0x9 |= 0x20
#define clrf_SYSTEM_Reg0x9_cksel_psram                          addSYSTEM_Reg0x9 &= ~0x20
#define get_SYSTEM_Reg0x9_cksel_psram                           ((addSYSTEM_Reg0x9 & 0x20) >> 5)

#define posSYSTEM_Reg0x9_ckdiv_qspi0                            6
#define bitSYSTEM_Reg0x9_ckdiv_qspi0                            0x3C0
#define set_SYSTEM_Reg0x9_ckdiv_qspi0(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3C0)) | ((val) << 6))
#define get_SYSTEM_Reg0x9_ckdiv_qspi0                           ((addSYSTEM_Reg0x9 & 0x3C0) >> 6)

#define posSYSTEM_Reg0x9_ckdiv_qspi1                            10
#define bitSYSTEM_Reg0x9_ckdiv_qspi1                            0x3C00
#define set_SYSTEM_Reg0x9_ckdiv_qspi1(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3C00)) | ((val) << 10))
#define get_SYSTEM_Reg0x9_ckdiv_qspi1                           ((addSYSTEM_Reg0x9 & 0x3C00) >> 10)

#define posSYSTEM_Reg0x9_ckdiv_sdio                             14
#define bitSYSTEM_Reg0x9_ckdiv_sdio                             0x3C000
#define set_SYSTEM_Reg0x9_ckdiv_sdio(val)                       addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3C000)) | ((val) << 14))
#define get_SYSTEM_Reg0x9_ckdiv_sdio                            ((addSYSTEM_Reg0x9 & 0x3C000) >> 14)

#define posSYSTEM_Reg0x9_ckdiv_auxs                             18
#define bitSYSTEM_Reg0x9_ckdiv_auxs                             0x3C0000
#define set_SYSTEM_Reg0x9_ckdiv_auxs(val)                       addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3C0000)) | ((val) << 18))
#define get_SYSTEM_Reg0x9_ckdiv_auxs                            ((addSYSTEM_Reg0x9 & 0x3C0000) >> 18)

#define posSYSTEM_Reg0x9_cksel_auxs                             22
#define bitSYSTEM_Reg0x9_cksel_auxs                             0xC00000
#define set_SYSTEM_Reg0x9_cksel_auxs(val)                       addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0xC00000)) | ((val) << 22))
#define get_SYSTEM_Reg0x9_cksel_auxs                            ((addSYSTEM_Reg0x9 & 0xC00000) >> 22)

#define posSYSTEM_Reg0x9_cksel_flash                            24
#define bitSYSTEM_Reg0x9_cksel_flash                            0x3000000
#define set_SYSTEM_Reg0x9_cksel_flash(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x3000000)) | ((val) << 24))
#define get_SYSTEM_Reg0x9_cksel_flash                           ((addSYSTEM_Reg0x9 & 0x3000000) >> 24)

#define posSYSTEM_Reg0x9_ckdiv_flash                            26
#define bitSYSTEM_Reg0x9_ckdiv_flash                            0xC000000
#define set_SYSTEM_Reg0x9_ckdiv_flash(val)                      addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0xC000000)) | ((val) << 26))
#define get_SYSTEM_Reg0x9_ckdiv_flash                           ((addSYSTEM_Reg0x9 & 0xC000000) >> 26)

#define posSYSTEM_Reg0x9_ckdiv_i2s0                             28
#define bitSYSTEM_Reg0x9_ckdiv_i2s0                             0x70000000
#define set_SYSTEM_Reg0x9_ckdiv_i2s0(val)                       addSYSTEM_Reg0x9 = ((addSYSTEM_Reg0x9 & (~0x70000000)) | ((val) << 28))
#define get_SYSTEM_Reg0x9_ckdiv_i2s0                            ((addSYSTEM_Reg0x9 & 0x70000000) >> 28)

//addSYSTEM_Reg0xa
#define addSYSTEM_Reg0xa                                        *((volatile unsigned long *) (0x44010000+0xa*4))
#define posSYSTEM_Reg0xa_ckdiv_26m                              0
#define bitSYSTEM_Reg0xa_ckdiv_26m                              0x3
#define set_SYSTEM_Reg0xa_ckdiv_26m(val)                        addSYSTEM_Reg0xa = ((addSYSTEM_Reg0xa & (~0x3)) | ((val) << 0))
#define get_SYSTEM_Reg0xa_ckdiv_26m                             (addSYSTEM_Reg0xa & 0x3)

//addSYSTEM_Reg0xb
#define addSYSTEM_Reg0xb                                        *((volatile unsigned long *) (0x44010000+0xb*4))
#define posSYSTEM_Reg0xb_anaspi_freq                            0
#define bitSYSTEM_Reg0xb_anaspi_freq                            0x3F
#define set_SYSTEM_Reg0xb_anaspi_freq(val)                      addSYSTEM_Reg0xb = ((addSYSTEM_Reg0xb & (~0x3F)) | ((val) << 0))
#define get_SYSTEM_Reg0xb_anaspi_freq                           (addSYSTEM_Reg0xb & 0x3F)

//addSYSTEM_Reg0xc
#define addSYSTEM_Reg0xc                                        *((volatile unsigned long *) (0x44010000+0xc*4))
#define posSYSTEM_Reg0xc_i2c0_cken                              0
#define bitSYSTEM_Reg0xc_i2c0_cken                              0x1
#define set_SYSTEM_Reg0xc_i2c0_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x1)) | ((val) << 0))
#define setf_SYSTEM_Reg0xc_i2c0_cken                            addSYSTEM_Reg0xc |= 0x1
#define clrf_SYSTEM_Reg0xc_i2c0_cken                            addSYSTEM_Reg0xc &= ~0x1
#define get_SYSTEM_Reg0xc_i2c0_cken                             (addSYSTEM_Reg0xc & 0x1)

#define posSYSTEM_Reg0xc_spi0_cken                              1
#define bitSYSTEM_Reg0xc_spi0_cken                              0x2
#define set_SYSTEM_Reg0xc_spi0_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x2)) | ((val) << 1))
#define setf_SYSTEM_Reg0xc_spi0_cken                            addSYSTEM_Reg0xc |= 0x2
#define clrf_SYSTEM_Reg0xc_spi0_cken                            addSYSTEM_Reg0xc &= ~0x2
#define get_SYSTEM_Reg0xc_spi0_cken                             ((addSYSTEM_Reg0xc & 0x2) >> 1)

#define posSYSTEM_Reg0xc_uart0_cken                             2
#define bitSYSTEM_Reg0xc_uart0_cken                             0x4
#define set_SYSTEM_Reg0xc_uart0_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x4)) | ((val) << 2))
#define setf_SYSTEM_Reg0xc_uart0_cken                           addSYSTEM_Reg0xc |= 0x4
#define clrf_SYSTEM_Reg0xc_uart0_cken                           addSYSTEM_Reg0xc &= ~0x4
#define get_SYSTEM_Reg0xc_uart0_cken                            ((addSYSTEM_Reg0xc & 0x4) >> 2)

#define posSYSTEM_Reg0xc_pwm0_cken                              3
#define bitSYSTEM_Reg0xc_pwm0_cken                              0x8
#define set_SYSTEM_Reg0xc_pwm0_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x8)) | ((val) << 3))
#define setf_SYSTEM_Reg0xc_pwm0_cken                            addSYSTEM_Reg0xc |= 0x8
#define clrf_SYSTEM_Reg0xc_pwm0_cken                            addSYSTEM_Reg0xc &= ~0x8
#define get_SYSTEM_Reg0xc_pwm0_cken                             ((addSYSTEM_Reg0xc & 0x8) >> 3)

#define posSYSTEM_Reg0xc_tim0_cken                              4
#define bitSYSTEM_Reg0xc_tim0_cken                              0x10
#define set_SYSTEM_Reg0xc_tim0_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x10)) | ((val) << 4))
#define setf_SYSTEM_Reg0xc_tim0_cken                            addSYSTEM_Reg0xc |= 0x10
#define clrf_SYSTEM_Reg0xc_tim0_cken                            addSYSTEM_Reg0xc &= ~0x10
#define get_SYSTEM_Reg0xc_tim0_cken                             ((addSYSTEM_Reg0xc & 0x10) >> 4)

#define posSYSTEM_Reg0xc_sadc_cken                              5
#define bitSYSTEM_Reg0xc_sadc_cken                              0x20
#define set_SYSTEM_Reg0xc_sadc_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x20)) | ((val) << 5))
#define setf_SYSTEM_Reg0xc_sadc_cken                            addSYSTEM_Reg0xc |= 0x20
#define clrf_SYSTEM_Reg0xc_sadc_cken                            addSYSTEM_Reg0xc &= ~0x20
#define get_SYSTEM_Reg0xc_sadc_cken                             ((addSYSTEM_Reg0xc & 0x20) >> 5)

#define posSYSTEM_Reg0xc_irda_cken                              6
#define bitSYSTEM_Reg0xc_irda_cken                              0x40
#define set_SYSTEM_Reg0xc_irda_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x40)) | ((val) << 6))
#define setf_SYSTEM_Reg0xc_irda_cken                            addSYSTEM_Reg0xc |= 0x40
#define clrf_SYSTEM_Reg0xc_irda_cken                            addSYSTEM_Reg0xc &= ~0x40
#define get_SYSTEM_Reg0xc_irda_cken                             ((addSYSTEM_Reg0xc & 0x40) >> 6)

#define posSYSTEM_Reg0xc_efuse_cken                             7
#define bitSYSTEM_Reg0xc_efuse_cken                             0x80
#define set_SYSTEM_Reg0xc_efuse_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x80)) | ((val) << 7))
#define setf_SYSTEM_Reg0xc_efuse_cken                           addSYSTEM_Reg0xc |= 0x80
#define clrf_SYSTEM_Reg0xc_efuse_cken                           addSYSTEM_Reg0xc &= ~0x80
#define get_SYSTEM_Reg0xc_efuse_cken                            ((addSYSTEM_Reg0xc & 0x80) >> 7)

#define posSYSTEM_Reg0xc_i2c1_cken                              8
#define bitSYSTEM_Reg0xc_i2c1_cken                              0x100
#define set_SYSTEM_Reg0xc_i2c1_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x100)) | ((val) << 8))
#define setf_SYSTEM_Reg0xc_i2c1_cken                            addSYSTEM_Reg0xc |= 0x100
#define clrf_SYSTEM_Reg0xc_i2c1_cken                            addSYSTEM_Reg0xc &= ~0x100
#define get_SYSTEM_Reg0xc_i2c1_cken                             ((addSYSTEM_Reg0xc & 0x100) >> 8)

#define posSYSTEM_Reg0xc_spi1_cken                              9
#define bitSYSTEM_Reg0xc_spi1_cken                              0x200
#define set_SYSTEM_Reg0xc_spi1_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x200)) | ((val) << 9))
#define setf_SYSTEM_Reg0xc_spi1_cken                            addSYSTEM_Reg0xc |= 0x200
#define clrf_SYSTEM_Reg0xc_spi1_cken                            addSYSTEM_Reg0xc &= ~0x200
#define get_SYSTEM_Reg0xc_spi1_cken                             ((addSYSTEM_Reg0xc & 0x200) >> 9)

#define posSYSTEM_Reg0xc_uart1_cken                             10
#define bitSYSTEM_Reg0xc_uart1_cken                             0x400
#define set_SYSTEM_Reg0xc_uart1_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x400)) | ((val) << 10))
#define setf_SYSTEM_Reg0xc_uart1_cken                           addSYSTEM_Reg0xc |= 0x400
#define clrf_SYSTEM_Reg0xc_uart1_cken                           addSYSTEM_Reg0xc &= ~0x400
#define get_SYSTEM_Reg0xc_uart1_cken                            ((addSYSTEM_Reg0xc & 0x400) >> 10)

#define posSYSTEM_Reg0xc_uart2_cken                             11
#define bitSYSTEM_Reg0xc_uart2_cken                             0x800
#define set_SYSTEM_Reg0xc_uart2_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x800)) | ((val) << 11))
#define setf_SYSTEM_Reg0xc_uart2_cken                           addSYSTEM_Reg0xc |= 0x800
#define clrf_SYSTEM_Reg0xc_uart2_cken                           addSYSTEM_Reg0xc &= ~0x800
#define get_SYSTEM_Reg0xc_uart2_cken                            ((addSYSTEM_Reg0xc & 0x800) >> 11)

#define posSYSTEM_Reg0xc_pwm1_cken                              12
#define bitSYSTEM_Reg0xc_pwm1_cken                              0x1000
#define set_SYSTEM_Reg0xc_pwm1_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x1000)) | ((val) << 12))
#define setf_SYSTEM_Reg0xc_pwm1_cken                            addSYSTEM_Reg0xc |= 0x1000
#define clrf_SYSTEM_Reg0xc_pwm1_cken                            addSYSTEM_Reg0xc &= ~0x1000
#define get_SYSTEM_Reg0xc_pwm1_cken                             ((addSYSTEM_Reg0xc & 0x1000) >> 12)

#define posSYSTEM_Reg0xc_tim1_cken                              13
#define bitSYSTEM_Reg0xc_tim1_cken                              0x2000
#define set_SYSTEM_Reg0xc_tim1_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x2000)) | ((val) << 13))
#define setf_SYSTEM_Reg0xc_tim1_cken                            addSYSTEM_Reg0xc |= 0x2000
#define clrf_SYSTEM_Reg0xc_tim1_cken                            addSYSTEM_Reg0xc &= ~0x2000
#define get_SYSTEM_Reg0xc_tim1_cken                             ((addSYSTEM_Reg0xc & 0x2000) >> 13)

#define posSYSTEM_Reg0xc_tim2_cken                              14
#define bitSYSTEM_Reg0xc_tim2_cken                              0x4000
#define set_SYSTEM_Reg0xc_tim2_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x4000)) | ((val) << 14))
#define setf_SYSTEM_Reg0xc_tim2_cken                            addSYSTEM_Reg0xc |= 0x4000
#define clrf_SYSTEM_Reg0xc_tim2_cken                            addSYSTEM_Reg0xc &= ~0x4000
#define get_SYSTEM_Reg0xc_tim2_cken                             ((addSYSTEM_Reg0xc & 0x4000) >> 14)

#define posSYSTEM_Reg0xc_otp_cken                               15
#define bitSYSTEM_Reg0xc_otp_cken                               0x8000
#define set_SYSTEM_Reg0xc_otp_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x8000)) | ((val) << 15))
#define setf_SYSTEM_Reg0xc_otp_cken                             addSYSTEM_Reg0xc |= 0x8000
#define clrf_SYSTEM_Reg0xc_otp_cken                             addSYSTEM_Reg0xc &= ~0x8000
#define get_SYSTEM_Reg0xc_otp_cken                              ((addSYSTEM_Reg0xc & 0x8000) >> 15)

#define posSYSTEM_Reg0xc_i2s_cken                               16
#define bitSYSTEM_Reg0xc_i2s_cken                               0x10000
#define set_SYSTEM_Reg0xc_i2s_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x10000)) | ((val) << 16))
#define setf_SYSTEM_Reg0xc_i2s_cken                             addSYSTEM_Reg0xc |= 0x10000
#define clrf_SYSTEM_Reg0xc_i2s_cken                             addSYSTEM_Reg0xc &= ~0x10000
#define get_SYSTEM_Reg0xc_i2s_cken                              ((addSYSTEM_Reg0xc & 0x10000) >> 16)

#define posSYSTEM_Reg0xc_usb_cken                               17
#define bitSYSTEM_Reg0xc_usb_cken                               0x20000
#define set_SYSTEM_Reg0xc_usb_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x20000)) | ((val) << 17))
#define setf_SYSTEM_Reg0xc_usb_cken                             addSYSTEM_Reg0xc |= 0x20000
#define clrf_SYSTEM_Reg0xc_usb_cken                             addSYSTEM_Reg0xc &= ~0x20000
#define get_SYSTEM_Reg0xc_usb_cken                              ((addSYSTEM_Reg0xc & 0x20000) >> 17)

#define posSYSTEM_Reg0xc_can_cken                               18
#define bitSYSTEM_Reg0xc_can_cken                               0x40000
#define set_SYSTEM_Reg0xc_can_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x40000)) | ((val) << 18))
#define setf_SYSTEM_Reg0xc_can_cken                             addSYSTEM_Reg0xc |= 0x40000
#define clrf_SYSTEM_Reg0xc_can_cken                             addSYSTEM_Reg0xc &= ~0x40000
#define get_SYSTEM_Reg0xc_can_cken                              ((addSYSTEM_Reg0xc & 0x40000) >> 18)

#define posSYSTEM_Reg0xc_psram_cken                             19
#define bitSYSTEM_Reg0xc_psram_cken                             0x80000
#define set_SYSTEM_Reg0xc_psram_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x80000)) | ((val) << 19))
#define setf_SYSTEM_Reg0xc_psram_cken                           addSYSTEM_Reg0xc |= 0x80000
#define clrf_SYSTEM_Reg0xc_psram_cken                           addSYSTEM_Reg0xc &= ~0x80000
#define get_SYSTEM_Reg0xc_psram_cken                            ((addSYSTEM_Reg0xc & 0x80000) >> 19)

#define posSYSTEM_Reg0xc_qspi0_cken                             20
#define bitSYSTEM_Reg0xc_qspi0_cken                             0x100000
#define set_SYSTEM_Reg0xc_qspi0_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x100000)) | ((val) << 20))
#define setf_SYSTEM_Reg0xc_qspi0_cken                           addSYSTEM_Reg0xc |= 0x100000
#define clrf_SYSTEM_Reg0xc_qspi0_cken                           addSYSTEM_Reg0xc &= ~0x100000
#define get_SYSTEM_Reg0xc_qspi0_cken                            ((addSYSTEM_Reg0xc & 0x100000) >> 20)

#define posSYSTEM_Reg0xc_qspi1_cken                             21
#define bitSYSTEM_Reg0xc_qspi1_cken                             0x200000
#define set_SYSTEM_Reg0xc_qspi1_cken(val)                       addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x200000)) | ((val) << 21))
#define setf_SYSTEM_Reg0xc_qspi1_cken                           addSYSTEM_Reg0xc |= 0x200000
#define clrf_SYSTEM_Reg0xc_qspi1_cken                           addSYSTEM_Reg0xc &= ~0x200000
#define get_SYSTEM_Reg0xc_qspi1_cken                            ((addSYSTEM_Reg0xc & 0x200000) >> 21)

#define posSYSTEM_Reg0xc_sdio_cken                              22
#define bitSYSTEM_Reg0xc_sdio_cken                              0x400000
#define set_SYSTEM_Reg0xc_sdio_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x400000)) | ((val) << 22))
#define setf_SYSTEM_Reg0xc_sdio_cken                            addSYSTEM_Reg0xc |= 0x400000
#define clrf_SYSTEM_Reg0xc_sdio_cken                            addSYSTEM_Reg0xc &= ~0x400000
#define get_SYSTEM_Reg0xc_sdio_cken                             ((addSYSTEM_Reg0xc & 0x400000) >> 22)

#define posSYSTEM_Reg0xc_auxs_cken                              23
#define bitSYSTEM_Reg0xc_auxs_cken                              0x800000
#define set_SYSTEM_Reg0xc_auxs_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x800000)) | ((val) << 23))
#define setf_SYSTEM_Reg0xc_auxs_cken                            addSYSTEM_Reg0xc |= 0x800000
#define clrf_SYSTEM_Reg0xc_auxs_cken                            addSYSTEM_Reg0xc &= ~0x800000
#define get_SYSTEM_Reg0xc_auxs_cken                             ((addSYSTEM_Reg0xc & 0x800000) >> 23)

#define posSYSTEM_Reg0xc_btdm_cken                              24
#define bitSYSTEM_Reg0xc_btdm_cken                              0x1000000
#define set_SYSTEM_Reg0xc_btdm_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x1000000)) | ((val) << 24))
#define setf_SYSTEM_Reg0xc_btdm_cken                            addSYSTEM_Reg0xc |= 0x1000000
#define clrf_SYSTEM_Reg0xc_btdm_cken                            addSYSTEM_Reg0xc &= ~0x1000000
#define get_SYSTEM_Reg0xc_btdm_cken                             ((addSYSTEM_Reg0xc & 0x1000000) >> 24)

#define posSYSTEM_Reg0xc_xvr_cken                               25
#define bitSYSTEM_Reg0xc_xvr_cken                               0x2000000
#define set_SYSTEM_Reg0xc_xvr_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x2000000)) | ((val) << 25))
#define setf_SYSTEM_Reg0xc_xvr_cken                             addSYSTEM_Reg0xc |= 0x2000000
#define clrf_SYSTEM_Reg0xc_xvr_cken                             addSYSTEM_Reg0xc &= ~0x2000000
#define get_SYSTEM_Reg0xc_xvr_cken                              ((addSYSTEM_Reg0xc & 0x2000000) >> 25)

#define posSYSTEM_Reg0xc_mac_cken                               26
#define bitSYSTEM_Reg0xc_mac_cken                               0x4000000
#define set_SYSTEM_Reg0xc_mac_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x4000000)) | ((val) << 26))
#define setf_SYSTEM_Reg0xc_mac_cken                             addSYSTEM_Reg0xc |= 0x4000000
#define clrf_SYSTEM_Reg0xc_mac_cken                             addSYSTEM_Reg0xc &= ~0x4000000
#define get_SYSTEM_Reg0xc_mac_cken                              ((addSYSTEM_Reg0xc & 0x4000000) >> 26)

#define posSYSTEM_Reg0xc_phy_cken                               27
#define bitSYSTEM_Reg0xc_phy_cken                               0x8000000
#define set_SYSTEM_Reg0xc_phy_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x8000000)) | ((val) << 27))
#define setf_SYSTEM_Reg0xc_phy_cken                             addSYSTEM_Reg0xc |= 0x8000000
#define clrf_SYSTEM_Reg0xc_phy_cken                             addSYSTEM_Reg0xc &= ~0x8000000
#define get_SYSTEM_Reg0xc_phy_cken                              ((addSYSTEM_Reg0xc & 0x8000000) >> 27)

#define posSYSTEM_Reg0xc_jpeg_cken                              28
#define bitSYSTEM_Reg0xc_jpeg_cken                              0x10000000
#define set_SYSTEM_Reg0xc_jpeg_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x10000000)) | ((val) << 28))
#define setf_SYSTEM_Reg0xc_jpeg_cken                            addSYSTEM_Reg0xc |= 0x10000000
#define clrf_SYSTEM_Reg0xc_jpeg_cken                            addSYSTEM_Reg0xc &= ~0x10000000
#define get_SYSTEM_Reg0xc_jpeg_cken                             ((addSYSTEM_Reg0xc & 0x10000000) >> 28)

#define posSYSTEM_Reg0xc_disp_cken                              29
#define bitSYSTEM_Reg0xc_disp_cken                              0x20000000
#define set_SYSTEM_Reg0xc_disp_cken(val)                        addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x20000000)) | ((val) << 29))
#define setf_SYSTEM_Reg0xc_disp_cken                            addSYSTEM_Reg0xc |= 0x20000000
#define clrf_SYSTEM_Reg0xc_disp_cken                            addSYSTEM_Reg0xc &= ~0x20000000
#define get_SYSTEM_Reg0xc_disp_cken                             ((addSYSTEM_Reg0xc & 0x20000000) >> 29)

#define posSYSTEM_Reg0xc_aud_cken                               30
#define bitSYSTEM_Reg0xc_aud_cken                               0x40000000
#define set_SYSTEM_Reg0xc_aud_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x40000000)) | ((val) << 30))
#define setf_SYSTEM_Reg0xc_aud_cken                             addSYSTEM_Reg0xc |= 0x40000000
#define clrf_SYSTEM_Reg0xc_aud_cken                             addSYSTEM_Reg0xc &= ~0x40000000
#define get_SYSTEM_Reg0xc_aud_cken                              ((addSYSTEM_Reg0xc & 0x40000000) >> 30)

#define posSYSTEM_Reg0xc_wdt_cken                               31
#define bitSYSTEM_Reg0xc_wdt_cken                               0x80000000
#define set_SYSTEM_Reg0xc_wdt_cken(val)                         addSYSTEM_Reg0xc = ((addSYSTEM_Reg0xc & (~0x80000000)) | ((val) << 31))
#define setf_SYSTEM_Reg0xc_wdt_cken                             addSYSTEM_Reg0xc |= 0x80000000
#define clrf_SYSTEM_Reg0xc_wdt_cken                             addSYSTEM_Reg0xc &= ~0x80000000
#define get_SYSTEM_Reg0xc_wdt_cken                              ((addSYSTEM_Reg0xc & 0x80000000) >> 31)

//addSYSTEM_Reg0xd
#define addSYSTEM_Reg0xd                                        *((volatile unsigned long *) (0x44010000+0xd*4))

//addSYSTEM_Reg0xe
#define addSYSTEM_Reg0xe                                        *((volatile unsigned long *) (0x44010000+0xe*4))

//addSYSTEM_Reg0xf
#define addSYSTEM_Reg0xf                                        *((volatile unsigned long *) (0x44010000+0xf*4))
#define posSYSTEM_Reg0xf_mbox0_disckg                           31
#define bitSYSTEM_Reg0xf_mbox0_disckg                           0x80000000
#define set_SYSTEM_Reg0xf_mbox0_disckg(val)                     addSYSTEM_Reg0xf = ((addSYSTEM_Reg0xf & (~0x80000000)) | ((val) << 31))
#define setf_SYSTEM_Reg0xf_mbox0_disckg                         addSYSTEM_Reg0xf |= 0x80000000
#define clrf_SYSTEM_Reg0xf_mbox0_disckg                         addSYSTEM_Reg0xf &= ~0x80000000
#define get_SYSTEM_Reg0xf_mbox0_disckg                          ((addSYSTEM_Reg0xf & 0x80000000) >> 31)

#define posSYSTEM_Reg0xf_mbox1_disckg                           30
#define bitSYSTEM_Reg0xf_mbox1_disckg                           0x40000000
#define set_SYSTEM_Reg0xf_mbox1_disckg(val)                     addSYSTEM_Reg0xf = ((addSYSTEM_Reg0xf & (~0x40000000)) | ((val) << 30))
#define setf_SYSTEM_Reg0xf_mbox1_disckg                         addSYSTEM_Reg0xf |= 0x40000000
#define clrf_SYSTEM_Reg0xf_mbox1_disckg                         addSYSTEM_Reg0xf &= ~0x40000000
#define get_SYSTEM_Reg0xf_mbox1_disckg                          ((addSYSTEM_Reg0xf & 0x40000000) >> 30)

//addSYSTEM_Reg0x10
#define addSYSTEM_Reg0x10                                       *((volatile unsigned long *) (0x44010000+0x10*4))
#define posSYSTEM_Reg0x10_pwd_mem1                              0
#define bitSYSTEM_Reg0x10_pwd_mem1                              0x1
#define set_SYSTEM_Reg0x10_pwd_mem1(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x1)) | ((val) << 0))
#define setf_SYSTEM_Reg0x10_pwd_mem1                            addSYSTEM_Reg0x10 |= 0x1
#define clrf_SYSTEM_Reg0x10_pwd_mem1                            addSYSTEM_Reg0x10 &= ~0x1
#define get_SYSTEM_Reg0x10_pwd_mem1                             (addSYSTEM_Reg0x10 & 0x1)

#define posSYSTEM_Reg0x10_pwd_mem2                              1
#define bitSYSTEM_Reg0x10_pwd_mem2                              0x2
#define set_SYSTEM_Reg0x10_pwd_mem2(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x2)) | ((val) << 1))
#define setf_SYSTEM_Reg0x10_pwd_mem2                            addSYSTEM_Reg0x10 |= 0x2
#define clrf_SYSTEM_Reg0x10_pwd_mem2                            addSYSTEM_Reg0x10 &= ~0x2
#define get_SYSTEM_Reg0x10_pwd_mem2                             ((addSYSTEM_Reg0x10 & 0x2) >> 1)

#define posSYSTEM_Reg0x10_pwd_mem3                              2
#define bitSYSTEM_Reg0x10_pwd_mem3                              0x4
#define set_SYSTEM_Reg0x10_pwd_mem3(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x4)) | ((val) << 2))
#define setf_SYSTEM_Reg0x10_pwd_mem3                            addSYSTEM_Reg0x10 |= 0x4
#define clrf_SYSTEM_Reg0x10_pwd_mem3                            addSYSTEM_Reg0x10 &= ~0x4
#define get_SYSTEM_Reg0x10_pwd_mem3                             ((addSYSTEM_Reg0x10 & 0x4) >> 2)

#define posSYSTEM_Reg0x10_pwd_encp                              3
#define bitSYSTEM_Reg0x10_pwd_encp                              0x8
#define set_SYSTEM_Reg0x10_pwd_encp(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x8)) | ((val) << 3))
#define setf_SYSTEM_Reg0x10_pwd_encp                            addSYSTEM_Reg0x10 |= 0x8
#define clrf_SYSTEM_Reg0x10_pwd_encp                            addSYSTEM_Reg0x10 &= ~0x8
#define get_SYSTEM_Reg0x10_pwd_encp                             ((addSYSTEM_Reg0x10 & 0x8) >> 3)

#define posSYSTEM_Reg0x10_pwd_bakp                              4
#define bitSYSTEM_Reg0x10_pwd_bakp                              0x10
#define set_SYSTEM_Reg0x10_pwd_bakp(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x10)) | ((val) << 4))
#define setf_SYSTEM_Reg0x10_pwd_bakp                            addSYSTEM_Reg0x10 |= 0x10
#define clrf_SYSTEM_Reg0x10_pwd_bakp                            addSYSTEM_Reg0x10 &= ~0x10
#define get_SYSTEM_Reg0x10_pwd_bakp                             ((addSYSTEM_Reg0x10 & 0x10) >> 4)

#define posSYSTEM_Reg0x10_pwd_ahbp                              5
#define bitSYSTEM_Reg0x10_pwd_ahbp                              0x20
#define set_SYSTEM_Reg0x10_pwd_ahbp(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x20)) | ((val) << 5))
#define setf_SYSTEM_Reg0x10_pwd_ahbp                            addSYSTEM_Reg0x10 |= 0x20
#define clrf_SYSTEM_Reg0x10_pwd_ahbp                            addSYSTEM_Reg0x10 &= ~0x20
#define get_SYSTEM_Reg0x10_pwd_ahbp                             ((addSYSTEM_Reg0x10 & 0x20) >> 5)

#define posSYSTEM_Reg0x10_pwd_audp                              6
#define bitSYSTEM_Reg0x10_pwd_audp                              0x40
#define set_SYSTEM_Reg0x10_pwd_audp(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x40)) | ((val) << 6))
#define setf_SYSTEM_Reg0x10_pwd_audp                            addSYSTEM_Reg0x10 |= 0x40
#define clrf_SYSTEM_Reg0x10_pwd_audp                            addSYSTEM_Reg0x10 &= ~0x40
#define get_SYSTEM_Reg0x10_pwd_audp                             ((addSYSTEM_Reg0x10 & 0x40) >> 6)

#define posSYSTEM_Reg0x10_pwd_vidp                              7
#define bitSYSTEM_Reg0x10_pwd_vidp                              0x80
#define set_SYSTEM_Reg0x10_pwd_vidp(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x80)) | ((val) << 7))
#define setf_SYSTEM_Reg0x10_pwd_vidp                            addSYSTEM_Reg0x10 |= 0x80
#define clrf_SYSTEM_Reg0x10_pwd_vidp                            addSYSTEM_Reg0x10 &= ~0x80
#define get_SYSTEM_Reg0x10_pwd_vidp                             ((addSYSTEM_Reg0x10 & 0x80) >> 7)

#define posSYSTEM_Reg0x10_pwd_btsp                              8
#define bitSYSTEM_Reg0x10_pwd_btsp                              0x100
#define set_SYSTEM_Reg0x10_pwd_btsp(val)                        addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x100)) | ((val) << 8))
#define setf_SYSTEM_Reg0x10_pwd_btsp                            addSYSTEM_Reg0x10 |= 0x100
#define clrf_SYSTEM_Reg0x10_pwd_btsp                            addSYSTEM_Reg0x10 &= ~0x100
#define get_SYSTEM_Reg0x10_pwd_btsp                             ((addSYSTEM_Reg0x10 & 0x100) >> 8)

#define posSYSTEM_Reg0x10_pwd_wifp_mac                          9
#define bitSYSTEM_Reg0x10_pwd_wifp_mac                          0x200
#define set_SYSTEM_Reg0x10_pwd_wifp_mac(val)                    addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x200)) | ((val) << 9))
#define setf_SYSTEM_Reg0x10_pwd_wifp_mac                        addSYSTEM_Reg0x10 |= 0x200
#define clrf_SYSTEM_Reg0x10_pwd_wifp_mac                        addSYSTEM_Reg0x10 &= ~0x200
#define get_SYSTEM_Reg0x10_pwd_wifp_mac                         ((addSYSTEM_Reg0x10 & 0x200) >> 9)

#define posSYSTEM_Reg0x10_pwd_wifp_phy                          10
#define bitSYSTEM_Reg0x10_pwd_wifp_phy                          0x400
#define set_SYSTEM_Reg0x10_pwd_wifp_phy(val)                    addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0x400)) | ((val) << 10))
#define setf_SYSTEM_Reg0x10_pwd_wifp_phy                        addSYSTEM_Reg0x10 |= 0x400
#define clrf_SYSTEM_Reg0x10_pwd_wifp_phy                        addSYSTEM_Reg0x10 &= ~0x400
#define get_SYSTEM_Reg0x10_pwd_wifp_phy                         ((addSYSTEM_Reg0x10 & 0x400) >> 10)

#define posSYSTEM_Reg0x10_system_halt                           16
#define bitSYSTEM_Reg0x10_system_halt                           0xF0000
#define set_SYSTEM_Reg0x10_system_halt(val)                     addSYSTEM_Reg0x10 = ((addSYSTEM_Reg0x10 & (~0xF0000)) | ((val) << 16))
#define get_SYSTEM_Reg0x10_system_halt                          ((addSYSTEM_Reg0x10 & 0xF0000) >> 16)

//addSYSTEM_Reg0x11
#define addSYSTEM_Reg0x11                                       *((volatile unsigned long *) (0x44010000+0x11*4))

//addSYSTEM_Reg0x20
#define addSYSTEM_Reg0x20                                       *((volatile unsigned long *) (0x44010000+0x20*4))

//addSYSTEM_Reg0x21
#define addSYSTEM_Reg0x21                                       *((volatile unsigned long *) (0x44010000+0x21*4))

//addSYSTEM_Reg0x22
#define addSYSTEM_Reg0x22                                       *((volatile unsigned long *) (0x44010000+0x22*4))

//addSYSTEM_Reg0x23
#define addSYSTEM_Reg0x23                                       *((volatile unsigned long *) (0x44010000+0x23*4))

//addSYSTEM_Reg0x28
#define addSYSTEM_Reg0x28                                       *((volatile unsigned long *) (0x44010000+0x28*4))

//addSYSTEM_Reg0x29
#define addSYSTEM_Reg0x29                                       *((volatile unsigned long *) (0x44010000+0x29*4))

//addSYSTEM_Reg0x2a
#define addSYSTEM_Reg0x2a                                       *((volatile unsigned long *) (0x44010000+0x2a*4))

//addSYSTEM_Reg0x2b
#define addSYSTEM_Reg0x2b                                       *((volatile unsigned long *) (0x44010000+0x2b*4))

//addSYSTEM_Reg0x30
#define addSYSTEM_Reg0x30                                       *((volatile unsigned long *) (0x44010000+0x30*4))

//addSYSTEM_Reg0x31
#define addSYSTEM_Reg0x31                                       *((volatile unsigned long *) (0x44010000+0x31*4))

//addSYSTEM_Reg0x32
#define addSYSTEM_Reg0x32                                       *((volatile unsigned long *) (0x44010000+0x32*4))

//addSYSTEM_Reg0x33
#define addSYSTEM_Reg0x33                                       *((volatile unsigned long *) (0x44010000+0x33*4))

//addSYSTEM_Reg0x34
#define addSYSTEM_Reg0x34                                       *((volatile unsigned long *) (0x44010000+0x34*4))

//addSYSTEM_Reg0x35
#define addSYSTEM_Reg0x35                                       *((volatile unsigned long *) (0x44010000+0x35*4))

//addSYSTEM_Reg0x38
#define addSYSTEM_Reg0x38                                       *((volatile unsigned long *) (0x44010000+0x38*4))

//addSYSTEM_Reg0x39
#define addSYSTEM_Reg0x39                                       *((volatile unsigned long *) (0x44010000+0x39*4))

//addSYSTEM_Reg0x40
#define addSYSTEM_Reg0x40                                       *((volatile unsigned long *) (0x44010000+0x40*4))

//addSYSTEM_Reg0x4f
#define addSYSTEM_Reg0x4f                                       *((volatile unsigned long *) (0x44010000+0x4f*4))


//************************************************************//
//AON_PMU
//************************************************************//
#define BASEADDR_AON_PMU                                        0x44000000
//addAON_PMU_Reg0x0
#define addAON_PMU_Reg0x0                                       *((volatile unsigned long *) (0x44000000+0x0*4))
#define posAON_PMU_Reg0x0_memchk_bps                            0
#define bitAON_PMU_Reg0x0_memchk_bps                            0x1
#define set_AON_PMU_Reg0x0_memchk_bps(val)                      addAON_PMU_Reg0x0 = ((addAON_PMU_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_AON_PMU_Reg0x0_memchk_bps                          addAON_PMU_Reg0x0 |= 0x1
#define clrf_AON_PMU_Reg0x0_memchk_bps                          addAON_PMU_Reg0x0 &= ~0x1
#define get_AON_PMU_Reg0x0_memchk_bps                           (addAON_PMU_Reg0x0 & 0x1)

//addAON_PMU_Reg0x1
#define addAON_PMU_Reg0x1                                       *((volatile unsigned long *) (0x44000000+0x1*4))
#define posAON_PMU_Reg0x1_touch_select                          0
#define bitAON_PMU_Reg0x1_touch_select                          0xF
#define set_AON_PMU_Reg0x1_touch_select(val)                    addAON_PMU_Reg0x1 = ((addAON_PMU_Reg0x1 & (~0xF)) | ((val) << 0))
#define get_AON_PMU_Reg0x1_touch_select                         (addAON_PMU_Reg0x1 & 0xF)

//addAON_PMU_Reg0x2
#define addAON_PMU_Reg0x2                                       *((volatile unsigned long *) (0x44000000+0x2*4))

//addAON_PMU_Reg0x3
#define addAON_PMU_Reg0x3                                       *((volatile unsigned long *) (0x44000000+0x3*4))

//addAON_PMU_Reg0x40
#define addAON_PMU_Reg0x40                                      *((volatile unsigned long *) (0x44000000+0x40*4))
#define posAON_PMU_Reg0x40_halt_clkena                          31
#define bitAON_PMU_Reg0x40_halt_clkena                          0x80000000
#define set_AON_PMU_Reg0x40_halt_clkena(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x80000000)) | ((val) << 31))
#define setf_AON_PMU_Reg0x40_halt_clkena                        addAON_PMU_Reg0x40 |= 0x80000000
#define clrf_AON_PMU_Reg0x40_halt_clkena                        addAON_PMU_Reg0x40 &= ~0x80000000
#define get_AON_PMU_Reg0x40_halt_clkena                         ((addAON_PMU_Reg0x40 & 0x80000000) >> 31)

#define posAON_PMU_Reg0x40_halt_isolat                          30
#define bitAON_PMU_Reg0x40_halt_isolat                          0x40000000
#define set_AON_PMU_Reg0x40_halt_isolat(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x40000000)) | ((val) << 30))
#define setf_AON_PMU_Reg0x40_halt_isolat                        addAON_PMU_Reg0x40 |= 0x40000000
#define clrf_AON_PMU_Reg0x40_halt_isolat                        addAON_PMU_Reg0x40 &= ~0x40000000
#define get_AON_PMU_Reg0x40_halt_isolat                         ((addAON_PMU_Reg0x40 & 0x40000000) >> 30)

#define posAON_PMU_Reg0x40_halt_resten                          29
#define bitAON_PMU_Reg0x40_halt_resten                          0x20000000
#define set_AON_PMU_Reg0x40_halt_resten(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x20000000)) | ((val) << 29))
#define setf_AON_PMU_Reg0x40_halt_resten                        addAON_PMU_Reg0x40 |= 0x20000000
#define clrf_AON_PMU_Reg0x40_halt_resten                        addAON_PMU_Reg0x40 &= ~0x20000000
#define get_AON_PMU_Reg0x40_halt_resten                         ((addAON_PMU_Reg0x40 & 0x20000000) >> 29)

#define posAON_PMU_Reg0x40_halt_rosc                            28
#define bitAON_PMU_Reg0x40_halt_rosc                            0x10000000
#define set_AON_PMU_Reg0x40_halt_rosc(val)                      addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x10000000)) | ((val) << 28))
#define setf_AON_PMU_Reg0x40_halt_rosc                          addAON_PMU_Reg0x40 |= 0x10000000
#define clrf_AON_PMU_Reg0x40_halt_rosc                          addAON_PMU_Reg0x40 &= ~0x10000000
#define get_AON_PMU_Reg0x40_halt_rosc                           ((addAON_PMU_Reg0x40 & 0x10000000) >> 28)

#define posAON_PMU_Reg0x40_halt_flash                           27
#define bitAON_PMU_Reg0x40_halt_flash                           0x8000000
#define set_AON_PMU_Reg0x40_halt_flash(val)                     addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x8000000)) | ((val) << 27))
#define setf_AON_PMU_Reg0x40_halt_flash                         addAON_PMU_Reg0x40 |= 0x8000000
#define clrf_AON_PMU_Reg0x40_halt_flash                         addAON_PMU_Reg0x40 &= ~0x8000000
#define get_AON_PMU_Reg0x40_halt_flash                          ((addAON_PMU_Reg0x40 & 0x8000000) >> 27)

#define posAON_PMU_Reg0x40_halt_core                            26
#define bitAON_PMU_Reg0x40_halt_core                            0x4000000
#define set_AON_PMU_Reg0x40_halt_core(val)                      addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x4000000)) | ((val) << 26))
#define setf_AON_PMU_Reg0x40_halt_core                          addAON_PMU_Reg0x40 |= 0x4000000
#define clrf_AON_PMU_Reg0x40_halt_core                          addAON_PMU_Reg0x40 &= ~0x4000000
#define get_AON_PMU_Reg0x40_halt_core                           ((addAON_PMU_Reg0x40 & 0x4000000) >> 26)

#define posAON_PMU_Reg0x40_halt_xtal                            25
#define bitAON_PMU_Reg0x40_halt_xtal                            0x2000000
#define set_AON_PMU_Reg0x40_halt_xtal(val)                      addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x2000000)) | ((val) << 25))
#define setf_AON_PMU_Reg0x40_halt_xtal                          addAON_PMU_Reg0x40 |= 0x2000000
#define clrf_AON_PMU_Reg0x40_halt_xtal                          addAON_PMU_Reg0x40 &= ~0x2000000
#define get_AON_PMU_Reg0x40_halt_xtal                           ((addAON_PMU_Reg0x40 & 0x2000000) >> 25)

#define posAON_PMU_Reg0x40_halt_volt                            24
#define bitAON_PMU_Reg0x40_halt_volt                            0x1000000
#define set_AON_PMU_Reg0x40_halt_volt(val)                      addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0x1000000)) | ((val) << 24))
#define setf_AON_PMU_Reg0x40_halt_volt                          addAON_PMU_Reg0x40 |= 0x1000000
#define clrf_AON_PMU_Reg0x40_halt_volt                          addAON_PMU_Reg0x40 &= ~0x1000000
#define get_AON_PMU_Reg0x40_halt_volt                           ((addAON_PMU_Reg0x40 & 0x1000000) >> 24)

#define posAON_PMU_Reg0x40_halt3_delay                          20
#define bitAON_PMU_Reg0x40_halt3_delay                          0xF00000
#define set_AON_PMU_Reg0x40_halt3_delay(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0xF00000)) | ((val) << 20))
#define get_AON_PMU_Reg0x40_halt3_delay                         ((addAON_PMU_Reg0x40 & 0xF00000) >> 20)

#define posAON_PMU_Reg0x40_halt2_delay                          16
#define bitAON_PMU_Reg0x40_halt2_delay                          0xF0000
#define set_AON_PMU_Reg0x40_halt2_delay(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0xF0000)) | ((val) << 16))
#define get_AON_PMU_Reg0x40_halt2_delay                         ((addAON_PMU_Reg0x40 & 0xF0000) >> 16)

#define posAON_PMU_Reg0x40_halt1_delay                          12
#define bitAON_PMU_Reg0x40_halt1_delay                          0xF000
#define set_AON_PMU_Reg0x40_halt1_delay(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0xF000)) | ((val) << 12))
#define get_AON_PMU_Reg0x40_halt1_delay                         ((addAON_PMU_Reg0x40 & 0xF000) >> 12)

#define posAON_PMU_Reg0x40_wake3_delay                          8
#define bitAON_PMU_Reg0x40_wake3_delay                          0xF00
#define set_AON_PMU_Reg0x40_wake3_delay(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0xF00)) | ((val) << 8))
#define get_AON_PMU_Reg0x40_wake3_delay                         ((addAON_PMU_Reg0x40 & 0xF00) >> 8)

#define posAON_PMU_Reg0x40_wake2_delay                          4
#define bitAON_PMU_Reg0x40_wake2_delay                          0xF0
#define set_AON_PMU_Reg0x40_wake2_delay(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0xF0)) | ((val) << 4))
#define get_AON_PMU_Reg0x40_wake2_delay                         ((addAON_PMU_Reg0x40 & 0xF0) >> 4)

#define posAON_PMU_Reg0x40_wake1_delay                          0
#define bitAON_PMU_Reg0x40_wake1_delay                          0xF
#define set_AON_PMU_Reg0x40_wake1_delay(val)                    addAON_PMU_Reg0x40 = ((addAON_PMU_Reg0x40 & (~0xF)) | ((val) << 0))
#define get_AON_PMU_Reg0x40_wake1_delay                         (addAON_PMU_Reg0x40 & 0xF)

//addAON_PMU_Reg0x41
#define addAON_PMU_Reg0x41                                      *((volatile unsigned long *) (0x44000000+0x41*4))
#define posAON_PMU_Reg0x41_lpo_config                           0
#define bitAON_PMU_Reg0x41_lpo_config                           0x3
#define set_AON_PMU_Reg0x41_lpo_config(val)                     addAON_PMU_Reg0x41 = ((addAON_PMU_Reg0x41 & (~0x3)) | ((val) << 0))
#define get_AON_PMU_Reg0x41_lpo_config                          (addAON_PMU_Reg0x41 & 0x3)

#define posAON_PMU_Reg0x41_flshsck_iocap                        2
#define bitAON_PMU_Reg0x41_flshsck_iocap                        0xC
#define set_AON_PMU_Reg0x41_flshsck_iocap(val)                  addAON_PMU_Reg0x41 = ((addAON_PMU_Reg0x41 & (~0xC)) | ((val) << 2))
#define get_AON_PMU_Reg0x41_flshsck_iocap                       ((addAON_PMU_Reg0x41 & 0xC) >> 2)

#define posAON_PMU_Reg0x41_wakeup_ena                           4
#define bitAON_PMU_Reg0x41_wakeup_ena                           0x1F0
#define set_AON_PMU_Reg0x41_wakeup_ena(val)                     addAON_PMU_Reg0x41 = ((addAON_PMU_Reg0x41 & (~0x1F0)) | ((val) << 4))
#define get_AON_PMU_Reg0x41_wakeup_ena                          ((addAON_PMU_Reg0x41 & 0x1F0) >> 4)

//addAON_PMU_Reg0x42
#define addAON_PMU_Reg0x42                                      *((volatile unsigned long *) (0x44000000+0x42*4))

//addAON_PMU_Reg0x43
#define addAON_PMU_Reg0x43                                      *((volatile unsigned long *) (0x44000000+0x43*4))
#define posAON_PMU_Reg0x43_clr_usbplug                          0
#define bitAON_PMU_Reg0x43_clr_usbplug                          0x1
#define set_AON_PMU_Reg0x43_clr_usbplug(val)                    addAON_PMU_Reg0x43 = ((addAON_PMU_Reg0x43 & (~0x1)) | ((val) << 0))
#define setf_AON_PMU_Reg0x43_clr_usbplug                        addAON_PMU_Reg0x43 |= 0x1
#define clrf_AON_PMU_Reg0x43_clr_usbplug                        addAON_PMU_Reg0x43 &= ~0x1
#define get_AON_PMU_Reg0x43_clr_usbplug                         (addAON_PMU_Reg0x43 & 0x1)

#define posAON_PMU_Reg0x43_clr_touched                          1
#define bitAON_PMU_Reg0x43_clr_touched                          0x2
#define set_AON_PMU_Reg0x43_clr_touched(val)                    addAON_PMU_Reg0x43 = ((addAON_PMU_Reg0x43 & (~0x2)) | ((val) << 1))
#define setf_AON_PMU_Reg0x43_clr_touched                        addAON_PMU_Reg0x43 |= 0x2
#define clrf_AON_PMU_Reg0x43_clr_touched                        addAON_PMU_Reg0x43 &= ~0x2
#define get_AON_PMU_Reg0x43_clr_touched                         ((addAON_PMU_Reg0x43 & 0x2) >> 1)

//addAON_PMU_Reg0x70
#define addAON_PMU_Reg0x70                                      *((volatile unsigned long *) (0x44000000+0x70*4))

//addAON_PMU_Reg0x7c
#define addAON_PMU_Reg0x7c                                      *((volatile unsigned long *) (0x44000000+0x7c*4))

//addAON_PMU_Reg0x7d
#define addAON_PMU_Reg0x7d                                      *((volatile unsigned long *) (0x44000000+0x7d*4))

//addAON_PMU_Reg0x7e
#define addAON_PMU_Reg0x7e                                      *((volatile unsigned long *) (0x44000000+0x7e*4))

//addAON_PMU_Reg0x7f
#define addAON_PMU_Reg0x7f                                      *((volatile unsigned long *) (0x44000000+0x7f*4))

//************************************************************//
//AON_GPIO
//************************************************************//
#define BASEADDR_AON_GPIO                                       0x44000400
//addAON_GPIO_Reg0x0
#define addAON_GPIO_Reg0x0                                      *((volatile unsigned long *) (0x44000400+0x0*4))
#define posAON_GPIO_Reg0x0_GPIO0_Config                         0
#define bitAON_GPIO_Reg0x0_GPIO0_Config                         0x3FF
#define set_AON_GPIO_Reg0x0_GPIO0_Config(val)                   addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x0_GPIO0_Config                        (addAON_GPIO_Reg0x0 & 0x3FF)

#define posAON_GPIO_Reg0x0_GPIO_Input                           0
#define bitAON_GPIO_Reg0x0_GPIO_Input                           0x1
#define get_AON_GPIO_Reg0x0_GPIO_Input                          (addAON_GPIO_Reg0x0 & 0x1)

#define posAON_GPIO_Reg0x0_GPIO_Output                          1
#define bitAON_GPIO_Reg0x0_GPIO_Output                          0x2
#define set_AON_GPIO_Reg0x0_GPIO_Output(val)                    addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_AON_GPIO_Reg0x0_GPIO_Output                        addAON_GPIO_Reg0x0 |= 0x2
#define clrf_AON_GPIO_Reg0x0_GPIO_Output                        addAON_GPIO_Reg0x0 &= ~0x2
#define get_AON_GPIO_Reg0x0_GPIO_Output                         ((addAON_GPIO_Reg0x0 & 0x2) >> 1)

#define posAON_GPIO_Reg0x0_GPIO_Input_Ena                       2
#define bitAON_GPIO_Reg0x0_GPIO_Input_Ena                       0x4
#define set_AON_GPIO_Reg0x0_GPIO_Input_Ena(val)                 addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_AON_GPIO_Reg0x0_GPIO_Input_Ena                     addAON_GPIO_Reg0x0 |= 0x4
#define clrf_AON_GPIO_Reg0x0_GPIO_Input_Ena                     addAON_GPIO_Reg0x0 &= ~0x4
#define get_AON_GPIO_Reg0x0_GPIO_Input_Ena                      ((addAON_GPIO_Reg0x0 & 0x4) >> 2)

#define posAON_GPIO_Reg0x0_GPIO_Output_Ena                      3
#define bitAON_GPIO_Reg0x0_GPIO_Output_Ena                      0x8
#define set_AON_GPIO_Reg0x0_GPIO_Output_Ena(val)                addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x8)) | ((val) << 3))
#define setf_AON_GPIO_Reg0x0_GPIO_Output_Ena                    addAON_GPIO_Reg0x0 |= 0x8
#define clrf_AON_GPIO_Reg0x0_GPIO_Output_Ena                    addAON_GPIO_Reg0x0 &= ~0x8
#define get_AON_GPIO_Reg0x0_GPIO_Output_Ena                     ((addAON_GPIO_Reg0x0 & 0x8) >> 3)

#define posAON_GPIO_Reg0x0_GPIO_Pull_Mode                       4
#define bitAON_GPIO_Reg0x0_GPIO_Pull_Mode                       0x10
#define set_AON_GPIO_Reg0x0_GPIO_Pull_Mode(val)                 addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_AON_GPIO_Reg0x0_GPIO_Pull_Mode                     addAON_GPIO_Reg0x0 |= 0x10
#define clrf_AON_GPIO_Reg0x0_GPIO_Pull_Mode                     addAON_GPIO_Reg0x0 &= ~0x10
#define get_AON_GPIO_Reg0x0_GPIO_Pull_Mode                      ((addAON_GPIO_Reg0x0 & 0x10) >> 4)

#define posAON_GPIO_Reg0x0_GPIO_Pull_Ena                        5
#define bitAON_GPIO_Reg0x0_GPIO_Pull_Ena                        0x20
#define set_AON_GPIO_Reg0x0_GPIO_Pull_Ena(val)                  addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_AON_GPIO_Reg0x0_GPIO_Pull_Ena                      addAON_GPIO_Reg0x0 |= 0x20
#define clrf_AON_GPIO_Reg0x0_GPIO_Pull_Ena                      addAON_GPIO_Reg0x0 &= ~0x20
#define get_AON_GPIO_Reg0x0_GPIO_Pull_Ena                       ((addAON_GPIO_Reg0x0 & 0x20) >> 5)

#define posAON_GPIO_Reg0x0_GPIO_Fun_Ena                         6
#define bitAON_GPIO_Reg0x0_GPIO_Fun_Ena                         0x40
#define set_AON_GPIO_Reg0x0_GPIO_Fun_Ena(val)                   addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_AON_GPIO_Reg0x0_GPIO_Fun_Ena                       addAON_GPIO_Reg0x0 |= 0x40
#define clrf_AON_GPIO_Reg0x0_GPIO_Fun_Ena                       addAON_GPIO_Reg0x0 &= ~0x40
#define get_AON_GPIO_Reg0x0_GPIO_Fun_Ena                        ((addAON_GPIO_Reg0x0 & 0x40) >> 6)

#define posAON_GPIO_Reg0x0_Input_Monitor                        7
#define bitAON_GPIO_Reg0x0_Input_Monitor                        0x80
#define set_AON_GPIO_Reg0x0_Input_Monitor(val)                  addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_AON_GPIO_Reg0x0_Input_Monitor                      addAON_GPIO_Reg0x0 |= 0x80
#define clrf_AON_GPIO_Reg0x0_Input_Monitor                      addAON_GPIO_Reg0x0 &= ~0x80
#define get_AON_GPIO_Reg0x0_Input_Monitor                       ((addAON_GPIO_Reg0x0 & 0x80) >> 7)

#define posAON_GPIO_Reg0x0_GPIO_Capacity                        8
#define bitAON_GPIO_Reg0x0_GPIO_Capacity                        0x300
#define set_AON_GPIO_Reg0x0_GPIO_Capacity(val)                  addAON_GPIO_Reg0x0 = ((addAON_GPIO_Reg0x0 & (~0x300)) | ((val) << 8))
#define get_AON_GPIO_Reg0x0_GPIO_Capacity                       ((addAON_GPIO_Reg0x0 & 0x300) >> 8)

//addAON_GPIO_Reg0x1
#define addAON_GPIO_Reg0x1                                      *((volatile unsigned long *) (0x44000400+0x1*4))
#define posAON_GPIO_Reg0x1_GPIO1_Config                         0
#define bitAON_GPIO_Reg0x1_GPIO1_Config                         0x3FF
#define set_AON_GPIO_Reg0x1_GPIO1_Config(val)                   addAON_GPIO_Reg0x1 = ((addAON_GPIO_Reg0x1 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1_GPIO1_Config                        (addAON_GPIO_Reg0x1 & 0x3FF)

//addAON_GPIO_Reg0x2
#define addAON_GPIO_Reg0x2                                      *((volatile unsigned long *) (0x44000400+0x2*4))
#define posAON_GPIO_Reg0x2_GPIO2_Config                         0
#define bitAON_GPIO_Reg0x2_GPIO2_Config                         0x3FF
#define set_AON_GPIO_Reg0x2_GPIO2_Config(val)                   addAON_GPIO_Reg0x2 = ((addAON_GPIO_Reg0x2 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2_GPIO2_Config                        (addAON_GPIO_Reg0x2 & 0x3FF)

//addAON_GPIO_Reg0x3
#define addAON_GPIO_Reg0x3                                      *((volatile unsigned long *) (0x44000400+0x3*4))
#define posAON_GPIO_Reg0x3_GPIO3_Config                         0
#define bitAON_GPIO_Reg0x3_GPIO3_Config                         0x3FF
#define set_AON_GPIO_Reg0x3_GPIO3_Config(val)                   addAON_GPIO_Reg0x3 = ((addAON_GPIO_Reg0x3 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x3_GPIO3_Config                        (addAON_GPIO_Reg0x3 & 0x3FF)

//addAON_GPIO_Reg0x4
#define addAON_GPIO_Reg0x4                                      *((volatile unsigned long *) (0x44000400+0x4*4))
#define posAON_GPIO_Reg0x4_GPIO4_Config                         0
#define bitAON_GPIO_Reg0x4_GPIO4_Config                         0x3FF
#define set_AON_GPIO_Reg0x4_GPIO4_Config(val)                   addAON_GPIO_Reg0x4 = ((addAON_GPIO_Reg0x4 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x4_GPIO4_Config                        (addAON_GPIO_Reg0x4 & 0x3FF)

//addAON_GPIO_Reg0x5
#define addAON_GPIO_Reg0x5                                      *((volatile unsigned long *) (0x44000400+0x5*4))
#define posAON_GPIO_Reg0x5_GPIO5_Config                         0
#define bitAON_GPIO_Reg0x5_GPIO5_Config                         0x3FF
#define set_AON_GPIO_Reg0x5_GPIO5_Config(val)                   addAON_GPIO_Reg0x5 = ((addAON_GPIO_Reg0x5 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x5_GPIO5_Config                        (addAON_GPIO_Reg0x5 & 0x3FF)

//addAON_GPIO_Reg0x6
#define addAON_GPIO_Reg0x6                                      *((volatile unsigned long *) (0x44000400+0x6*4))
#define posAON_GPIO_Reg0x6_GPIO6_Config                         0
#define bitAON_GPIO_Reg0x6_GPIO6_Config                         0x3FF
#define set_AON_GPIO_Reg0x6_GPIO6_Config(val)                   addAON_GPIO_Reg0x6 = ((addAON_GPIO_Reg0x6 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x6_GPIO6_Config                        (addAON_GPIO_Reg0x6 & 0x3FF)

//addAON_GPIO_Reg0x7
#define addAON_GPIO_Reg0x7                                      *((volatile unsigned long *) (0x44000400+0x7*4))
#define posAON_GPIO_Reg0x7_GPIO7_Config                         0
#define bitAON_GPIO_Reg0x7_GPIO7_Config                         0x3FF
#define set_AON_GPIO_Reg0x7_GPIO7_Config(val)                   addAON_GPIO_Reg0x7 = ((addAON_GPIO_Reg0x7 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x7_GPIO7_Config                        (addAON_GPIO_Reg0x7 & 0x3FF)

//addAON_GPIO_Reg0x8
#define addAON_GPIO_Reg0x8                                      *((volatile unsigned long *) (0x44000400+0x8*4))
#define posAON_GPIO_Reg0x8_GPIO8_Config                         0
#define bitAON_GPIO_Reg0x8_GPIO8_Config                         0x3FF
#define set_AON_GPIO_Reg0x8_GPIO8_Config(val)                   addAON_GPIO_Reg0x8 = ((addAON_GPIO_Reg0x8 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x8_GPIO8_Config                        (addAON_GPIO_Reg0x8 & 0x3FF)

//addAON_GPIO_Reg0x9
#define addAON_GPIO_Reg0x9                                      *((volatile unsigned long *) (0x44000400+0x9*4))
#define posAON_GPIO_Reg0x9_GPIO9_Config                         0
#define bitAON_GPIO_Reg0x9_GPIO9_Config                         0x3FF
#define set_AON_GPIO_Reg0x9_GPIO9_Config(val)                   addAON_GPIO_Reg0x9 = ((addAON_GPIO_Reg0x9 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x9_GPIO9_Config                        (addAON_GPIO_Reg0x9 & 0x3FF)

//addAON_GPIO_Reg0xa
#define addAON_GPIO_Reg0xa                                      *((volatile unsigned long *) (0x44000400+0xa*4))
#define posAON_GPIO_Reg0xa_GPIO10_Config                        0
#define bitAON_GPIO_Reg0xa_GPIO10_Config                        0x3FF
#define set_AON_GPIO_Reg0xa_GPIO10_Config(val)                  addAON_GPIO_Reg0xa = ((addAON_GPIO_Reg0xa & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0xa_GPIO10_Config                       (addAON_GPIO_Reg0xa & 0x3FF)

//addAON_GPIO_Reg0xb
#define addAON_GPIO_Reg0xb                                      *((volatile unsigned long *) (0x44000400+0xb*4))
#define posAON_GPIO_Reg0xb_GPIO11_Config                        0
#define bitAON_GPIO_Reg0xb_GPIO11_Config                        0x3FF
#define set_AON_GPIO_Reg0xb_GPIO11_Config(val)                  addAON_GPIO_Reg0xb = ((addAON_GPIO_Reg0xb & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0xb_GPIO11_Config                       (addAON_GPIO_Reg0xb & 0x3FF)

//addAON_GPIO_Reg0xc
#define addAON_GPIO_Reg0xc                                      *((volatile unsigned long *) (0x44000400+0xc*4))
#define posAON_GPIO_Reg0xc_GPIO12_Config                        0
#define bitAON_GPIO_Reg0xc_GPIO12_Config                        0x3FF
#define set_AON_GPIO_Reg0xc_GPIO12_Config(val)                  addAON_GPIO_Reg0xc = ((addAON_GPIO_Reg0xc & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0xc_GPIO12_Config                       (addAON_GPIO_Reg0xc & 0x3FF)

//addAON_GPIO_Reg0xd
#define addAON_GPIO_Reg0xd                                      *((volatile unsigned long *) (0x44000400+0xd*4))
#define posAON_GPIO_Reg0xd_GPIO13_Config                        0
#define bitAON_GPIO_Reg0xd_GPIO13_Config                        0x3FF
#define set_AON_GPIO_Reg0xd_GPIO13_Config(val)                  addAON_GPIO_Reg0xd = ((addAON_GPIO_Reg0xd & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0xd_GPIO13_Config                       (addAON_GPIO_Reg0xd & 0x3FF)

//addAON_GPIO_Reg0xe
#define addAON_GPIO_Reg0xe                                      *((volatile unsigned long *) (0x44000400+0xe*4))
#define posAON_GPIO_Reg0xe_GPIO14_Config                        0
#define bitAON_GPIO_Reg0xe_GPIO14_Config                        0x3FF
#define set_AON_GPIO_Reg0xe_GPIO14_Config(val)                  addAON_GPIO_Reg0xe = ((addAON_GPIO_Reg0xe & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0xe_GPIO14_Config                       (addAON_GPIO_Reg0xe & 0x3FF)

//addAON_GPIO_Reg0xf
#define addAON_GPIO_Reg0xf                                      *((volatile unsigned long *) (0x44000400+0xf*4))
#define posAON_GPIO_Reg0xf_GPIO15_Config                        0
#define bitAON_GPIO_Reg0xf_GPIO15_Config                        0x3FF
#define set_AON_GPIO_Reg0xf_GPIO15_Config(val)                  addAON_GPIO_Reg0xf = ((addAON_GPIO_Reg0xf & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0xf_GPIO15_Config                       (addAON_GPIO_Reg0xf & 0x3FF)

//addAON_GPIO_Reg0x10
#define addAON_GPIO_Reg0x10                                     *((volatile unsigned long *) (0x44000400+0x10*4))
#define posAON_GPIO_Reg0x10_GPIO16_Config                       0
#define bitAON_GPIO_Reg0x10_GPIO16_Config                       0x3FF
#define set_AON_GPIO_Reg0x10_GPIO16_Config(val)                 addAON_GPIO_Reg0x10 = ((addAON_GPIO_Reg0x10 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x10_GPIO16_Config                      (addAON_GPIO_Reg0x10 & 0x3FF)

//addAON_GPIO_Reg0x11
#define addAON_GPIO_Reg0x11                                     *((volatile unsigned long *) (0x44000400+0x11*4))
#define posAON_GPIO_Reg0x11_GPIO17_Config                       0
#define bitAON_GPIO_Reg0x11_GPIO17_Config                       0x3FF
#define set_AON_GPIO_Reg0x11_GPIO17_Config(val)                 addAON_GPIO_Reg0x11 = ((addAON_GPIO_Reg0x11 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x11_GPIO17_Config                      (addAON_GPIO_Reg0x11 & 0x3FF)

//addAON_GPIO_Reg0x12
#define addAON_GPIO_Reg0x12                                     *((volatile unsigned long *) (0x44000400+0x12*4))
#define posAON_GPIO_Reg0x12_GPIO18_Config                       0
#define bitAON_GPIO_Reg0x12_GPIO18_Config                       0x3FF
#define set_AON_GPIO_Reg0x12_GPIO18_Config(val)                 addAON_GPIO_Reg0x12 = ((addAON_GPIO_Reg0x12 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x12_GPIO18_Config                      (addAON_GPIO_Reg0x12 & 0x3FF)

//addAON_GPIO_Reg0x13
#define addAON_GPIO_Reg0x13                                     *((volatile unsigned long *) (0x44000400+0x13*4))
#define posAON_GPIO_Reg0x13_GPIO19_Config                       0
#define bitAON_GPIO_Reg0x13_GPIO19_Config                       0x3FF
#define set_AON_GPIO_Reg0x13_GPIO19_Config(val)                 addAON_GPIO_Reg0x13 = ((addAON_GPIO_Reg0x13 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x13_GPIO19_Config                      (addAON_GPIO_Reg0x13 & 0x3FF)

//addAON_GPIO_Reg0x14
#define addAON_GPIO_Reg0x14                                     *((volatile unsigned long *) (0x44000400+0x14*4))
#define posAON_GPIO_Reg0x14_GPIO20_Config                       0
#define bitAON_GPIO_Reg0x14_GPIO20_Config                       0x3FF
#define set_AON_GPIO_Reg0x14_GPIO20_Config(val)                 addAON_GPIO_Reg0x14 = ((addAON_GPIO_Reg0x14 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x14_GPIO20_Config                      (addAON_GPIO_Reg0x14 & 0x3FF)

//addAON_GPIO_Reg0x15
#define addAON_GPIO_Reg0x15                                     *((volatile unsigned long *) (0x44000400+0x15*4))
#define posAON_GPIO_Reg0x15_GPIO21_Config                       0
#define bitAON_GPIO_Reg0x15_GPIO21_Config                       0x3FF
#define set_AON_GPIO_Reg0x15_GPIO21_Config(val)                 addAON_GPIO_Reg0x15 = ((addAON_GPIO_Reg0x15 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x15_GPIO21_Config                      (addAON_GPIO_Reg0x15 & 0x3FF)

//addAON_GPIO_Reg0x16
#define addAON_GPIO_Reg0x16                                     *((volatile unsigned long *) (0x44000400+0x16*4))
#define posAON_GPIO_Reg0x16_GPIO22_Config                       0
#define bitAON_GPIO_Reg0x16_GPIO22_Config                       0x3FF
#define set_AON_GPIO_Reg0x16_GPIO22_Config(val)                 addAON_GPIO_Reg0x16 = ((addAON_GPIO_Reg0x16 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x16_GPIO22_Config                      (addAON_GPIO_Reg0x16 & 0x3FF)

//addAON_GPIO_Reg0x17
#define addAON_GPIO_Reg0x17                                     *((volatile unsigned long *) (0x44000400+0x17*4))
#define posAON_GPIO_Reg0x17_GPIO23_Config                       0
#define bitAON_GPIO_Reg0x17_GPIO23_Config                       0x3FF
#define set_AON_GPIO_Reg0x17_GPIO23_Config(val)                 addAON_GPIO_Reg0x17 = ((addAON_GPIO_Reg0x17 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x17_GPIO23_Config                      (addAON_GPIO_Reg0x17 & 0x3FF)

//addAON_GPIO_Reg0x18
#define addAON_GPIO_Reg0x18                                     *((volatile unsigned long *) (0x44000400+0x18*4))
#define posAON_GPIO_Reg0x18_GPIO24_Config                       0
#define bitAON_GPIO_Reg0x18_GPIO24_Config                       0x3FF
#define set_AON_GPIO_Reg0x18_GPIO24_Config(val)                 addAON_GPIO_Reg0x18 = ((addAON_GPIO_Reg0x18 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x18_GPIO24_Config                      (addAON_GPIO_Reg0x18 & 0x3FF)

//addAON_GPIO_Reg0x19
#define addAON_GPIO_Reg0x19                                     *((volatile unsigned long *) (0x44000400+0x19*4))
#define posAON_GPIO_Reg0x19_GPIO25_Config                       0
#define bitAON_GPIO_Reg0x19_GPIO25_Config                       0x3FF
#define set_AON_GPIO_Reg0x19_GPIO25_Config(val)                 addAON_GPIO_Reg0x19 = ((addAON_GPIO_Reg0x19 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x19_GPIO25_Config                      (addAON_GPIO_Reg0x19 & 0x3FF)

//addAON_GPIO_Reg0x1a
#define addAON_GPIO_Reg0x1a                                     *((volatile unsigned long *) (0x44000400+0x1a*4))
#define posAON_GPIO_Reg0x1a_GPIO26_Config                       0
#define bitAON_GPIO_Reg0x1a_GPIO26_Config                       0x3FF
#define set_AON_GPIO_Reg0x1a_GPIO26_Config(val)                 addAON_GPIO_Reg0x1a = ((addAON_GPIO_Reg0x1a & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1a_GPIO26_Config                      (addAON_GPIO_Reg0x1a & 0x3FF)

//addAON_GPIO_Reg0x1b
#define addAON_GPIO_Reg0x1b                                     *((volatile unsigned long *) (0x44000400+0x1b*4))
#define posAON_GPIO_Reg0x1b_GPIO27_Config                       0
#define bitAON_GPIO_Reg0x1b_GPIO27_Config                       0x3FF
#define set_AON_GPIO_Reg0x1b_GPIO27_Config(val)                 addAON_GPIO_Reg0x1b = ((addAON_GPIO_Reg0x1b & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1b_GPIO27_Config                      (addAON_GPIO_Reg0x1b & 0x3FF)

//addAON_GPIO_Reg0x1c
#define addAON_GPIO_Reg0x1c                                     *((volatile unsigned long *) (0x44000400+0x1c*4))
#define posAON_GPIO_Reg0x1c_GPIO28_Config                       0
#define bitAON_GPIO_Reg0x1c_GPIO28_Config                       0x3FF
#define set_AON_GPIO_Reg0x1c_GPIO28_Config(val)                 addAON_GPIO_Reg0x1c = ((addAON_GPIO_Reg0x1c & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1c_GPIO28_Config                      (addAON_GPIO_Reg0x1c & 0x3FF)

//addAON_GPIO_Reg0x1d
#define addAON_GPIO_Reg0x1d                                     *((volatile unsigned long *) (0x44000400+0x1d*4))
#define posAON_GPIO_Reg0x1d_GPIO29_Config                       0
#define bitAON_GPIO_Reg0x1d_GPIO29_Config                       0x3FF
#define set_AON_GPIO_Reg0x1d_GPIO29_Config(val)                 addAON_GPIO_Reg0x1d = ((addAON_GPIO_Reg0x1d & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1d_GPIO29_Config                      (addAON_GPIO_Reg0x1d & 0x3FF)

//addAON_GPIO_Reg0x1e
#define addAON_GPIO_Reg0x1e                                     *((volatile unsigned long *) (0x44000400+0x1e*4))
#define posAON_GPIO_Reg0x1e_GPIO30_Config                       0
#define bitAON_GPIO_Reg0x1e_GPIO30_Config                       0x3FF
#define set_AON_GPIO_Reg0x1e_GPIO30_Config(val)                 addAON_GPIO_Reg0x1e = ((addAON_GPIO_Reg0x1e & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1e_GPIO30_Config                      (addAON_GPIO_Reg0x1e & 0x3FF)

//addAON_GPIO_Reg0x1f
#define addAON_GPIO_Reg0x1f                                     *((volatile unsigned long *) (0x44000400+0x1f*4))
#define posAON_GPIO_Reg0x1f_GPIO31_Config                       0
#define bitAON_GPIO_Reg0x1f_GPIO31_Config                       0x3FF
#define set_AON_GPIO_Reg0x1f_GPIO31_Config(val)                 addAON_GPIO_Reg0x1f = ((addAON_GPIO_Reg0x1f & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x1f_GPIO31_Config                      (addAON_GPIO_Reg0x1f & 0x3FF)

//addAON_GPIO_Reg0x20
#define addAON_GPIO_Reg0x20                                     *((volatile unsigned long *) (0x44000400+0x20*4))
#define posAON_GPIO_Reg0x20_GPIO32_Config                       0
#define bitAON_GPIO_Reg0x20_GPIO32_Config                       0x3FF
#define set_AON_GPIO_Reg0x20_GPIO32_Config(val)                 addAON_GPIO_Reg0x20 = ((addAON_GPIO_Reg0x20 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x20_GPIO32_Config                      (addAON_GPIO_Reg0x20 & 0x3FF)

//addAON_GPIO_Reg0x21
#define addAON_GPIO_Reg0x21                                     *((volatile unsigned long *) (0x44000400+0x21*4))
#define posAON_GPIO_Reg0x21_GPIO33_Config                       0
#define bitAON_GPIO_Reg0x21_GPIO33_Config                       0x3FF
#define set_AON_GPIO_Reg0x21_GPIO33_Config(val)                 addAON_GPIO_Reg0x21 = ((addAON_GPIO_Reg0x21 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x21_GPIO33_Config                      (addAON_GPIO_Reg0x21 & 0x3FF)

//addAON_GPIO_Reg0x22
#define addAON_GPIO_Reg0x22                                     *((volatile unsigned long *) (0x44000400+0x22*4))
#define posAON_GPIO_Reg0x22_GPIO34_Config                       0
#define bitAON_GPIO_Reg0x22_GPIO34_Config                       0x3FF
#define set_AON_GPIO_Reg0x22_GPIO34_Config(val)                 addAON_GPIO_Reg0x22 = ((addAON_GPIO_Reg0x22 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x22_GPIO34_Config                      (addAON_GPIO_Reg0x22 & 0x3FF)

//addAON_GPIO_Reg0x23
#define addAON_GPIO_Reg0x23                                     *((volatile unsigned long *) (0x44000400+0x23*4))
#define posAON_GPIO_Reg0x23_GPIO35_Config                       0
#define bitAON_GPIO_Reg0x23_GPIO35_Config                       0x3FF
#define set_AON_GPIO_Reg0x23_GPIO35_Config(val)                 addAON_GPIO_Reg0x23 = ((addAON_GPIO_Reg0x23 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x23_GPIO35_Config                      (addAON_GPIO_Reg0x23 & 0x3FF)

//addAON_GPIO_Reg0x24
#define addAON_GPIO_Reg0x24                                     *((volatile unsigned long *) (0x44000400+0x24*4))
#define posAON_GPIO_Reg0x24_GPIO36_Config                       0
#define bitAON_GPIO_Reg0x24_GPIO36_Config                       0x3FF
#define set_AON_GPIO_Reg0x24_GPIO36_Config(val)                 addAON_GPIO_Reg0x24 = ((addAON_GPIO_Reg0x24 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x24_GPIO36_Config                      (addAON_GPIO_Reg0x24 & 0x3FF)

//addAON_GPIO_Reg0x25
#define addAON_GPIO_Reg0x25                                     *((volatile unsigned long *) (0x44000400+0x25*4))
#define posAON_GPIO_Reg0x25_GPIO37_Config                       0
#define bitAON_GPIO_Reg0x25_GPIO37_Config                       0x3FF
#define set_AON_GPIO_Reg0x25_GPIO37_Config(val)                 addAON_GPIO_Reg0x25 = ((addAON_GPIO_Reg0x25 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x25_GPIO37_Config                      (addAON_GPIO_Reg0x25 & 0x3FF)

//addAON_GPIO_Reg0x26
#define addAON_GPIO_Reg0x26                                     *((volatile unsigned long *) (0x44000400+0x26*4))
#define posAON_GPIO_Reg0x26_GPIO38_Config                       0
#define bitAON_GPIO_Reg0x26_GPIO38_Config                       0x3FF
#define set_AON_GPIO_Reg0x26_GPIO38_Config(val)                 addAON_GPIO_Reg0x26 = ((addAON_GPIO_Reg0x26 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x26_GPIO38_Config                      (addAON_GPIO_Reg0x26 & 0x3FF)

//addAON_GPIO_Reg0x27
#define addAON_GPIO_Reg0x27                                     *((volatile unsigned long *) (0x44000400+0x27*4))
#define posAON_GPIO_Reg0x27_GPIO39_Config                       0
#define bitAON_GPIO_Reg0x27_GPIO39_Config                       0x3FF
#define set_AON_GPIO_Reg0x27_GPIO39_Config(val)                 addAON_GPIO_Reg0x27 = ((addAON_GPIO_Reg0x27 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x27_GPIO39_Config                      (addAON_GPIO_Reg0x27 & 0x3FF)

//addAON_GPIO_Reg0x28
#define addAON_GPIO_Reg0x28                                     *((volatile unsigned long *) (0x44000400+0x28*4))
#define posAON_GPIO_Reg0x28_GPIO40_Config                       0
#define bitAON_GPIO_Reg0x28_GPIO40_Config                       0x3FF
#define set_AON_GPIO_Reg0x28_GPIO40_Config(val)                 addAON_GPIO_Reg0x28 = ((addAON_GPIO_Reg0x28 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x28_GPIO40_Config                      (addAON_GPIO_Reg0x28 & 0x3FF)

//addAON_GPIO_Reg0x29
#define addAON_GPIO_Reg0x29                                     *((volatile unsigned long *) (0x44000400+0x29*4))
#define posAON_GPIO_Reg0x29_GPIO41_Config                       0
#define bitAON_GPIO_Reg0x29_GPIO41_Config                       0x3FF
#define set_AON_GPIO_Reg0x29_GPIO41_Config(val)                 addAON_GPIO_Reg0x29 = ((addAON_GPIO_Reg0x29 & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x29_GPIO41_Config                      (addAON_GPIO_Reg0x29 & 0x3FF)

//addAON_GPIO_Reg0x2a
#define addAON_GPIO_Reg0x2a                                     *((volatile unsigned long *) (0x44000400+0x2a*4))
#define posAON_GPIO_Reg0x2a_GPIO42_Config                       0
#define bitAON_GPIO_Reg0x2a_GPIO42_Config                       0x3FF
#define set_AON_GPIO_Reg0x2a_GPIO42_Config(val)                 addAON_GPIO_Reg0x2a = ((addAON_GPIO_Reg0x2a & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2a_GPIO42_Config                      (addAON_GPIO_Reg0x2a & 0x3FF)

//addAON_GPIO_Reg0x2b
#define addAON_GPIO_Reg0x2b                                     *((volatile unsigned long *) (0x44000400+0x2b*4))
#define posAON_GPIO_Reg0x2b_GPIO43_Config                       0
#define bitAON_GPIO_Reg0x2b_GPIO43_Config                       0x3FF
#define set_AON_GPIO_Reg0x2b_GPIO43_Config(val)                 addAON_GPIO_Reg0x2b = ((addAON_GPIO_Reg0x2b & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2b_GPIO43_Config                      (addAON_GPIO_Reg0x2b & 0x3FF)

//addAON_GPIO_Reg0x2c
#define addAON_GPIO_Reg0x2c                                     *((volatile unsigned long *) (0x44000400+0x2c*4))
#define posAON_GPIO_Reg0x2c_GPIO44_Config                       0
#define bitAON_GPIO_Reg0x2c_GPIO44_Config                       0x3FF
#define set_AON_GPIO_Reg0x2c_GPIO44_Config(val)                 addAON_GPIO_Reg0x2c = ((addAON_GPIO_Reg0x2c & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2c_GPIO44_Config                      (addAON_GPIO_Reg0x2c & 0x3FF)

//addAON_GPIO_Reg0x2d
#define addAON_GPIO_Reg0x2d                                     *((volatile unsigned long *) (0x44000400+0x2d*4))
#define posAON_GPIO_Reg0x2d_GPIO45_Config                       0
#define bitAON_GPIO_Reg0x2d_GPIO45_Config                       0x3FF
#define set_AON_GPIO_Reg0x2d_GPIO45_Config(val)                 addAON_GPIO_Reg0x2d = ((addAON_GPIO_Reg0x2d & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2d_GPIO45_Config                      (addAON_GPIO_Reg0x2d & 0x3FF)

//addAON_GPIO_Reg0x2e
#define addAON_GPIO_Reg0x2e                                     *((volatile unsigned long *) (0x44000400+0x2e*4))
#define posAON_GPIO_Reg0x2e_GPIO46_Config                       0
#define bitAON_GPIO_Reg0x2e_GPIO46_Config                       0x3FF
#define set_AON_GPIO_Reg0x2e_GPIO46_Config(val)                 addAON_GPIO_Reg0x2e = ((addAON_GPIO_Reg0x2e & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2e_GPIO46_Config                      (addAON_GPIO_Reg0x2e & 0x3FF)

//addAON_GPIO_Reg0x2f
#define addAON_GPIO_Reg0x2f                                     *((volatile unsigned long *) (0x44000400+0x2f*4))
#define posAON_GPIO_Reg0x2f_GPIO47_Config                       0
#define bitAON_GPIO_Reg0x2f_GPIO47_Config                       0x3FF
#define set_AON_GPIO_Reg0x2f_GPIO47_Config(val)                 addAON_GPIO_Reg0x2f = ((addAON_GPIO_Reg0x2f & (~0x3FF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x2f_GPIO47_Config                      (addAON_GPIO_Reg0x2f & 0x3FF)

//addAON_GPIO_Reg0x40
#define addAON_GPIO_Reg0x40                                     *((volatile unsigned long *) (0x44000400+0x40*4))

//addAON_GPIO_Reg0x41
#define addAON_GPIO_Reg0x41                                     *((volatile unsigned long *) (0x44000400+0x41*4))

//addAON_GPIO_Reg0x42
#define addAON_GPIO_Reg0x42                                     *((volatile unsigned long *) (0x44000400+0x42*4))

//addAON_GPIO_Reg0x43
#define addAON_GPIO_Reg0x43                                     *((volatile unsigned long *) (0x44000400+0x43*4))

//addAON_GPIO_Reg0x44
#define addAON_GPIO_Reg0x44                                     *((volatile unsigned long *) (0x44000400+0x44*4))
#define posAON_GPIO_Reg0x44_int_en2                             8
#define bitAON_GPIO_Reg0x44_int_en2                             0xFFFFFF00
#define set_AON_GPIO_Reg0x44_int_en2(val)                       addAON_GPIO_Reg0x44 = ((addAON_GPIO_Reg0x44 & (~0xFFFFFF00)) | ((val) << 8))
#define get_AON_GPIO_Reg0x44_int_en2                            ((addAON_GPIO_Reg0x44 & 0xFFFFFF00) >> 8)

//addAON_GPIO_Reg0x45
#define addAON_GPIO_Reg0x45                                     *((volatile unsigned long *) (0x44000400+0x45*4))

//addAON_GPIO_Reg0x46
#define addAON_GPIO_Reg0x46                                     *((volatile unsigned long *) (0x44000400+0x46*4))
#define posAON_GPIO_Reg0x46_intsta2                             0
#define bitAON_GPIO_Reg0x46_intsta2                             0xFFFF
#define get_AON_GPIO_Reg0x46_intsta2                            (addAON_GPIO_Reg0x46 & 0xFFFF)

//addAON_GPIO_Reg0x47
#define addAON_GPIO_Reg0x47                                     *((volatile unsigned long *) (0x44000400+0x47*4))

//addAON_GPIO_Reg0x48
#define addAON_GPIO_Reg0x48                                     *((volatile unsigned long *) (0x44000400+0x48*4))
#define posAON_GPIO_Reg0x48_int_clr1                            0
#define bitAON_GPIO_Reg0x48_int_clr1                            0xFFFF
#define set_AON_GPIO_Reg0x48_int_clr1(val)                      addAON_GPIO_Reg0x48 = ((addAON_GPIO_Reg0x48 & (~0xFFFF)) | ((val) << 0))
#define get_AON_GPIO_Reg0x48_int_clr1                           (addAON_GPIO_Reg0x48 & 0xFFFF)


//************************************************************//
//AON_RTC
//************************************************************//
#define BASEADDR_AON_RTC                                        0x44000200
//addAON_RTC_Reg0x0
#define addAON_RTC_Reg0x0                                       *((volatile unsigned long *) (0x44000200+0x0*4))
#define posAON_RTC_Reg0x0_rtc_clk_en                            6
#define bitAON_RTC_Reg0x0_rtc_clk_en                            0x40
#define set_AON_RTC_Reg0x0_rtc_clk_en(val)                      addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_AON_RTC_Reg0x0_rtc_clk_en                          addAON_RTC_Reg0x0 |= 0x40
#define clrf_AON_RTC_Reg0x0_rtc_clk_en                          addAON_RTC_Reg0x0 &= ~0x40
#define get_AON_RTC_Reg0x0_rtc_clk_en                           ((addAON_RTC_Reg0x0 & 0x40) >> 6)

#define posAON_RTC_Reg0x0_rtc_tick_int                          5
#define bitAON_RTC_Reg0x0_rtc_tick_int                          0x20
#define set_AON_RTC_Reg0x0_rtc_tick_int(val)                    addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_AON_RTC_Reg0x0_rtc_tick_int                        addAON_RTC_Reg0x0 |= 0x20
#define clrf_AON_RTC_Reg0x0_rtc_tick_int                        addAON_RTC_Reg0x0 &= ~0x20
#define get_AON_RTC_Reg0x0_rtc_tick_int                         ((addAON_RTC_Reg0x0 & 0x20) >> 5)

#define posAON_RTC_Reg0x0_rtc_aon_int                           4
#define bitAON_RTC_Reg0x0_rtc_aon_int                           0x10
#define set_AON_RTC_Reg0x0_rtc_aon_int(val)                     addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_AON_RTC_Reg0x0_rtc_aon_int                         addAON_RTC_Reg0x0 |= 0x10
#define clrf_AON_RTC_Reg0x0_rtc_aon_int                         addAON_RTC_Reg0x0 &= ~0x10
#define get_AON_RTC_Reg0x0_rtc_aon_int                          ((addAON_RTC_Reg0x0 & 0x10) >> 4)

#define posAON_RTC_Reg0x0_rtc_tick_int_en                       3
#define bitAON_RTC_Reg0x0_rtc_tick_int_en                       0x8
#define set_AON_RTC_Reg0x0_rtc_tick_int_en(val)                 addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x8)) | ((val) << 3))
#define setf_AON_RTC_Reg0x0_rtc_tick_int_en                     addAON_RTC_Reg0x0 |= 0x8
#define clrf_AON_RTC_Reg0x0_rtc_tick_int_en                     addAON_RTC_Reg0x0 &= ~0x8
#define get_AON_RTC_Reg0x0_rtc_tick_int_en                      ((addAON_RTC_Reg0x0 & 0x8) >> 3)

#define posAON_RTC_Reg0x0_rtc_aon_int_en                        2
#define bitAON_RTC_Reg0x0_rtc_aon_int_en                        0x4
#define set_AON_RTC_Reg0x0_rtc_aon_int_en(val)                  addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_AON_RTC_Reg0x0_rtc_aon_int_en                      addAON_RTC_Reg0x0 |= 0x4
#define clrf_AON_RTC_Reg0x0_rtc_aon_int_en                      addAON_RTC_Reg0x0 &= ~0x4
#define get_AON_RTC_Reg0x0_rtc_aon_int_en                       ((addAON_RTC_Reg0x0 & 0x4) >> 2)

#define posAON_RTC_Reg0x0_rtc_cnt_stop                          1
#define bitAON_RTC_Reg0x0_rtc_cnt_stop                          0x2
#define set_AON_RTC_Reg0x0_rtc_cnt_stop(val)                    addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_AON_RTC_Reg0x0_rtc_cnt_stop                        addAON_RTC_Reg0x0 |= 0x2
#define clrf_AON_RTC_Reg0x0_rtc_cnt_stop                        addAON_RTC_Reg0x0 &= ~0x2
#define get_AON_RTC_Reg0x0_rtc_cnt_stop                         ((addAON_RTC_Reg0x0 & 0x2) >> 1)

#define posAON_RTC_Reg0x0_rtc_cnt_reset                         0
#define bitAON_RTC_Reg0x0_rtc_cnt_reset                         0x1
#define set_AON_RTC_Reg0x0_rtc_cnt_reset(val)                   addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_AON_RTC_Reg0x0_rtc_cnt_reset                       addAON_RTC_Reg0x0 |= 0x1
#define clrf_AON_RTC_Reg0x0_rtc_cnt_reset                       addAON_RTC_Reg0x0 &= ~0x1
#define get_AON_RTC_Reg0x0_rtc_cnt_reset                        (addAON_RTC_Reg0x0 & 0x1)

//addAON_RTC_Reg0x1
#define addAON_RTC_Reg0x1                                       *((volatile unsigned long *) (0x44000200+0x1*4))

//addAON_RTC_Reg0x2
#define addAON_RTC_Reg0x2                                       *((volatile unsigned long *) (0x44000200+0x2*4))

//addAON_RTC_Reg0x3
#define addAON_RTC_Reg0x3                                       *((volatile unsigned long *) (0x44000200+0x3*4))


//************************************************************//
//FLASH
//************************************************************//
#define BASEADDR_FLASH                                          0x44030000
//addFLASH_Reg0x0
#define addFLASH_Reg0x0                                         *((volatile unsigned long *) (0x44030000+0x0*4))
#define posFLASH_Reg0x0_addr_sw_reg                             0
#define bitFLASH_Reg0x0_addr_sw_reg                             0xFFFFFF
#define set_FLASH_Reg0x0_addr_sw_reg(val)                       addFLASH_Reg0x0 = ((addFLASH_Reg0x0 & (~0xFFFFFF)) | ((val) << 0))
#define get_FLASH_Reg0x0_addr_sw_reg                            (addFLASH_Reg0x0 & 0xFFFFFF)

#define posFLASH_Reg0x0_op_type_sw                              24
#define bitFLASH_Reg0x0_op_type_sw                              0x1F000000
#define set_FLASH_Reg0x0_op_type_sw(val)                        addFLASH_Reg0x0 = ((addFLASH_Reg0x0 & (~0x1F000000)) | ((val) << 24))
#define get_FLASH_Reg0x0_op_type_sw                             ((addFLASH_Reg0x0 & 0x1F000000) >> 24)

#define posFLASH_Reg0x0_op_sw                                   29
#define bitFLASH_Reg0x0_op_sw                                   0x20000000
#define set_FLASH_Reg0x0_op_sw(val)                             addFLASH_Reg0x0 = ((addFLASH_Reg0x0 & (~0x20000000)) | ((val) << 29))
#define setf_FLASH_Reg0x0_op_sw                                 addFLASH_Reg0x0 |= 0x20000000
#define clrf_FLASH_Reg0x0_op_sw                                 addFLASH_Reg0x0 &= ~0x20000000
#define get_FLASH_Reg0x0_op_sw                                  ((addFLASH_Reg0x0 & 0x20000000) >> 29)

#define posFLASH_Reg0x0_wp_value                                30
#define bitFLASH_Reg0x0_wp_value                                0x40000000
#define set_FLASH_Reg0x0_wp_value(val)                          addFLASH_Reg0x0 = ((addFLASH_Reg0x0 & (~0x40000000)) | ((val) << 30))
#define setf_FLASH_Reg0x0_wp_value                              addFLASH_Reg0x0 |= 0x40000000
#define clrf_FLASH_Reg0x0_wp_value                              addFLASH_Reg0x0 &= ~0x40000000
#define get_FLASH_Reg0x0_wp_value                               ((addFLASH_Reg0x0 & 0x40000000) >> 30)

#define posFLASH_Reg0x0_busy_sw                                 31
#define bitFLASH_Reg0x0_busy_sw                                 0x80000000
#define get_FLASH_Reg0x0_busy_sw                                ((addFLASH_Reg0x0 & 0x80000000) >> 31)

//addFLASH_Reg0x1
#define addFLASH_Reg0x1                                         *((volatile unsigned long *) (0x44030000+0x1*4))

//addFLASH_Reg0x2
#define addFLASH_Reg0x2                                         *((volatile unsigned long *) (0x44030000+0x2*4))

//addFLASH_Reg0x3
#define addFLASH_Reg0x3                                         *((volatile unsigned long *) (0x44030000+0x3*4))

//addFLASH_Reg0x4
#define addFLASH_Reg0x4                                         *((volatile unsigned long *) (0x44030000+0x4*4))

//addFLASH_Reg0x5
#define addFLASH_Reg0x5                                         *((volatile unsigned long *) (0x44030000+0x5*4))
#define posFLASH_Reg0x5_sr_data_flash                           0
#define bitFLASH_Reg0x5_sr_data_flash                           0xFF
#define get_FLASH_Reg0x5_sr_data_flash                          (addFLASH_Reg0x5 & 0xFF)

#define posFLASH_Reg0x5_crc_err_counter                         8
#define bitFLASH_Reg0x5_crc_err_counter                         0xFF00
#define get_FLASH_Reg0x5_crc_err_counter                        ((addFLASH_Reg0x5 & 0xFF00) >> 8)

#define posFLASH_Reg0x5_data_flash_sw_sel                       16
#define bitFLASH_Reg0x5_data_flash_sw_sel                       0x70000
#define get_FLASH_Reg0x5_data_flash_sw_sel                      ((addFLASH_Reg0x5 & 0x70000) >> 16)

#define posFLASH_Reg0x5_data_sw_flash_sel                       19
#define bitFLASH_Reg0x5_data_sw_flash_sel                       0x380000
#define get_FLASH_Reg0x5_data_sw_flash_sel                      ((addFLASH_Reg0x5 & 0x380000) >> 19)

#define posFLASH_Reg0x5_m_value                                 22
#define bitFLASH_Reg0x5_m_value                                 0x3FC00000
#define set_FLASH_Reg0x5_m_value(val)                           addFLASH_Reg0x5 = ((addFLASH_Reg0x5 & (~0x3FC00000)) | ((val) << 22))
#define get_FLASH_Reg0x5_m_value                                ((addFLASH_Reg0x5 & 0x3FC00000) >> 22)

#define posFLASH_Reg0x5_pw_write                                30
#define bitFLASH_Reg0x5_pw_write                                0x40000000
#define set_FLASH_Reg0x5_pw_write(val)                          addFLASH_Reg0x5 = ((addFLASH_Reg0x5 & (~0x40000000)) | ((val) << 30))
#define setf_FLASH_Reg0x5_pw_write                              addFLASH_Reg0x5 |= 0x40000000
#define clrf_FLASH_Reg0x5_pw_write                              addFLASH_Reg0x5 &= ~0x40000000
#define get_FLASH_Reg0x5_pw_write                               ((addFLASH_Reg0x5 & 0x40000000) >> 30)

//addFLASH_Reg0x6
#define addFLASH_Reg0x6                                         *((volatile unsigned long *) (0x44030000+0x6*4))

//addFLASH_Reg0x7
#define addFLASH_Reg0x7                                         *((volatile unsigned long *) (0x44030000+0x7*4))
#define posFLASH_Reg0x7_flash_clk_conf                          0
#define bitFLASH_Reg0x7_flash_clk_conf                          0xF
#define set_FLASH_Reg0x7_flash_clk_conf(val)                    addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0xF)) | ((val) << 0))
#define get_FLASH_Reg0x7_flash_clk_conf                         (addFLASH_Reg0x7 & 0xF)

#define posFLASH_Reg0x7_mode_sel                                4
#define bitFLASH_Reg0x7_mode_sel                                0x1F0
#define set_FLASH_Reg0x7_mode_sel(val)                          addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0x1F0)) | ((val) << 4))
#define get_FLASH_Reg0x7_mode_sel                               ((addFLASH_Reg0x7 & 0x1F0) >> 4)

#define posFLASH_Reg0x7_fwren_flash_cpu                         9
#define bitFLASH_Reg0x7_fwren_flash_cpu                         0x200
#define set_FLASH_Reg0x7_fwren_flash_cpu(val)                   addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0x200)) | ((val) << 9))
#define setf_FLASH_Reg0x7_fwren_flash_cpu                       addFLASH_Reg0x7 |= 0x200
#define clrf_FLASH_Reg0x7_fwren_flash_cpu                       addFLASH_Reg0x7 &= ~0x200
#define get_FLASH_Reg0x7_fwren_flash_cpu                        ((addFLASH_Reg0x7 & 0x200) >> 9)

#define posFLASH_Reg0x7_wrsr_data                               10
#define bitFLASH_Reg0x7_wrsr_data                               0x3FFFC00
#define set_FLASH_Reg0x7_wrsr_data(val)                         addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0x3FFFC00)) | ((val) << 10))
#define get_FLASH_Reg0x7_wrsr_data                              ((addFLASH_Reg0x7 & 0x3FFFC00) >> 10)

#define posFLASH_Reg0x7_crc_en                                  26
#define bitFLASH_Reg0x7_crc_en                                  0x4000000
#define set_FLASH_Reg0x7_crc_en(val)                            addFLASH_Reg0x7 = ((addFLASH_Reg0x7 & (~0x4000000)) | ((val) << 26))
#define setf_FLASH_Reg0x7_crc_en                                addFLASH_Reg0x7 |= 0x4000000
#define clrf_FLASH_Reg0x7_crc_en                                addFLASH_Reg0x7 &= ~0x4000000
#define get_FLASH_Reg0x7_crc_en                                 ((addFLASH_Reg0x7 & 0x4000000) >> 26)

//addFLASH_Reg0x8
#define addFLASH_Reg0x8                                         *((volatile unsigned long *) (0x44030000+0x8*4))
#define posFLASH_Reg0x8_dpd_status                              31
#define bitFLASH_Reg0x8_dpd_status                              0x80000000
#define set_FLASH_Reg0x8_dpd_status(val)                        addFLASH_Reg0x8 = ((addFLASH_Reg0x8 & (~0x80000000)) | ((val) << 31))
#define setf_FLASH_Reg0x8_dpd_status                            addFLASH_Reg0x8 |= 0x80000000
#define clrf_FLASH_Reg0x8_dpd_status                            addFLASH_Reg0x8 &= ~0x80000000
#define get_FLASH_Reg0x8_dpd_status                             ((addFLASH_Reg0x8 & 0x80000000) >> 31)

#define posFLASH_Reg0x8_prefetch_version                        25
#define bitFLASH_Reg0x8_prefetch_version                        0x2000000
#define set_FLASH_Reg0x8_prefetch_version(val)                  addFLASH_Reg0x8 = ((addFLASH_Reg0x8 & (~0x2000000)) | ((val) << 25))
#define setf_FLASH_Reg0x8_prefetch_version                      addFLASH_Reg0x8 |= 0x2000000
#define clrf_FLASH_Reg0x8_prefetch_version                      addFLASH_Reg0x8 &= ~0x2000000
#define get_FLASH_Reg0x8_prefetch_version                       ((addFLASH_Reg0x8 & 0x2000000) >> 25)

#define posFLASH_Reg0x8_dpd_fbd                                 24
#define bitFLASH_Reg0x8_dpd_fbd                                 0x1000000
#define set_FLASH_Reg0x8_dpd_fbd(val)                           addFLASH_Reg0x8 = ((addFLASH_Reg0x8 & (~0x1000000)) | ((val) << 24))
#define setf_FLASH_Reg0x8_dpd_fbd                               addFLASH_Reg0x8 |= 0x1000000
#define clrf_FLASH_Reg0x8_dpd_fbd                               addFLASH_Reg0x8 &= ~0x1000000
#define get_FLASH_Reg0x8_dpd_fbd                                ((addFLASH_Reg0x8 & 0x1000000) >> 24)

#define posFLASH_Reg0x8_tdp_tdpdd_delay_cnt                     12
#define bitFLASH_Reg0x8_tdp_tdpdd_delay_cnt                     0xFFF000
#define set_FLASH_Reg0x8_tdp_tdpdd_delay_cnt(val)               addFLASH_Reg0x8 = ((addFLASH_Reg0x8 & (~0xFFF000)) | ((val) << 12))
#define get_FLASH_Reg0x8_tdp_tdpdd_delay_cnt                    ((addFLASH_Reg0x8 & 0xFFF000) >> 12)

#define posFLASH_Reg0x8_tres1_trdp_delay_cnt                    0
#define bitFLASH_Reg0x8_tres1_trdp_delay_cnt                    0xFFF
#define set_FLASH_Reg0x8_tres1_trdp_delay_cnt(val)              addFLASH_Reg0x8 = ((addFLASH_Reg0x8 & (~0xFFF)) | ((val) << 0))
#define get_FLASH_Reg0x8_tres1_trdp_delay_cnt                   (addFLASH_Reg0x8 & 0xFFF)

//addFLASH_Reg0x9
#define addFLASH_Reg0x9                                         *((volatile unsigned long *) (0x44030000+0x9*4))
#define posFLASH_Reg0x9_mem_addr_clr                            31
#define bitFLASH_Reg0x9_mem_addr_clr                            0x80000000
#define set_FLASH_Reg0x9_mem_addr_clr(val)                      addFLASH_Reg0x9 = ((addFLASH_Reg0x9 & (~0x80000000)) | ((val) << 31))
#define setf_FLASH_Reg0x9_mem_addr_clr                          addFLASH_Reg0x9 |= 0x80000000
#define clrf_FLASH_Reg0x9_mem_addr_clr                          addFLASH_Reg0x9 &= ~0x80000000
#define get_FLASH_Reg0x9_mem_addr_clr                           ((addFLASH_Reg0x9 & 0x80000000) >> 31)

#define posFLASH_Reg0x9_mem_data                                0
#define bitFLASH_Reg0x9_mem_data                                0xFF
#define set_FLASH_Reg0x9_mem_data(val)                          addFLASH_Reg0x9 = ((addFLASH_Reg0x9 & (~0xFF)) | ((val) << 0))
#define get_FLASH_Reg0x9_mem_data                               (addFLASH_Reg0x9 & 0xFF)





//************************************************************//
//GENER_DMA
//************************************************************//
#define BASEADDR_GENER_DMA                                      0x44020000
//addGENER_DMA_Reg0x0
#define addGENER_DMA_Reg0x0                                     *((volatile unsigned long *) (0x44020000+0x0*4))
#define posGENER_DMA_Reg0x0_trans_len                           16
#define bitGENER_DMA_Reg0x0_trans_len                           0xFFFF0000
#define set_GENER_DMA_Reg0x0_trans_len(val)                     addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0xFFFF0000)) | ((val) << 16))
#define get_GENER_DMA_Reg0x0_trans_len                          ((addGENER_DMA_Reg0x0 & 0xFFFF0000) >> 16)


#define posGENER_DMA_Reg0x0_chn_prioprity                       12
#define bitGENER_DMA_Reg0x0_chn_prioprity                       0x7000
#define set_GENER_DMA_Reg0x0_chn_prioprity(val)                 addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x7000)) | ((val) << 12))
#define get_GENER_DMA_Reg0x0_chn_prioprity                      ((addGENER_DMA_Reg0x0 & 0x7000) >> 12)

#define posGENER_DMA_Reg0x0_dest_addr_loop                      11
#define bitGENER_DMA_Reg0x0_dest_addr_loop                      0x800
#define set_GENER_DMA_Reg0x0_dest_addr_loop(val)                addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x800)) | ((val) << 11))
#define setf_GENER_DMA_Reg0x0_dest_addr_loop                    addGENER_DMA_Reg0x0 |= 0x800
#define clrf_GENER_DMA_Reg0x0_dest_addr_loop                    addGENER_DMA_Reg0x0 &= ~0x800
#define get_GENER_DMA_Reg0x0_dest_addr_loop                     ((addGENER_DMA_Reg0x0 & 0x800) >> 11)

#define posGENER_DMA_Reg0x0_src_addr_loop                       10
#define bitGENER_DMA_Reg0x0_src_addr_loop                       0x400
#define set_GENER_DMA_Reg0x0_src_addr_loop(val)                 addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x400)) | ((val) << 10))
#define setf_GENER_DMA_Reg0x0_src_addr_loop                     addGENER_DMA_Reg0x0 |= 0x400
#define clrf_GENER_DMA_Reg0x0_src_addr_loop                     addGENER_DMA_Reg0x0 &= ~0x400
#define get_GENER_DMA_Reg0x0_src_addr_loop                      ((addGENER_DMA_Reg0x0 & 0x400) >> 10)

#define posGENER_DMA_Reg0x0_dest_addr_inc                       9
#define bitGENER_DMA_Reg0x0_dest_addr_inc                       0x200
#define set_GENER_DMA_Reg0x0_dest_addr_inc(val)                 addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x200)) | ((val) << 9))
#define setf_GENER_DMA_Reg0x0_dest_addr_inc                     addGENER_DMA_Reg0x0 |= 0x200
#define clrf_GENER_DMA_Reg0x0_dest_addr_inc                     addGENER_DMA_Reg0x0 &= ~0x200
#define get_GENER_DMA_Reg0x0_dest_addr_inc                      ((addGENER_DMA_Reg0x0 & 0x200) >> 9)

#define posGENER_DMA_Reg0x0_src_addr_inc                        8
#define bitGENER_DMA_Reg0x0_src_addr_inc                        0x100
#define set_GENER_DMA_Reg0x0_src_addr_inc(val)                  addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x100)) | ((val) << 8))
#define setf_GENER_DMA_Reg0x0_src_addr_inc                      addGENER_DMA_Reg0x0 |= 0x100
#define clrf_GENER_DMA_Reg0x0_src_addr_inc                      addGENER_DMA_Reg0x0 &= ~0x100
#define get_GENER_DMA_Reg0x0_src_addr_inc                       ((addGENER_DMA_Reg0x0 & 0x100) >> 8)

#define posGENER_DMA_Reg0x0_dest_data_width                     6
#define bitGENER_DMA_Reg0x0_dest_data_width                     0xC0
#define set_GENER_DMA_Reg0x0_dest_data_width(val)               addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0xC0)) | ((val) << 6))
#define get_GENER_DMA_Reg0x0_dest_data_width                    ((addGENER_DMA_Reg0x0 & 0xC0) >> 6)

#define posGENER_DMA_Reg0x0_src_data_width                      4
#define bitGENER_DMA_Reg0x0_src_data_width                      0x30
#define set_GENER_DMA_Reg0x0_src_data_width(val)                addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x30)) | ((val) << 4))
#define get_GENER_DMA_Reg0x0_src_data_width                     ((addGENER_DMA_Reg0x0 & 0x30) >> 4)

#define posGENER_DMA_Reg0x0_dma_mode                            3
#define bitGENER_DMA_Reg0x0_dma_mode                            0x8
#define set_GENER_DMA_Reg0x0_dma_mode(val)                      addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x8)) | ((val) << 3))
#define setf_GENER_DMA_Reg0x0_dma_mode                          addGENER_DMA_Reg0x0 |= 0x8
#define clrf_GENER_DMA_Reg0x0_dma_mode                          addGENER_DMA_Reg0x0 &= ~0x8
#define get_GENER_DMA_Reg0x0_dma_mode                           ((addGENER_DMA_Reg0x0 & 0x8) >> 3)

#define posGENER_DMA_Reg0x0_half_finish_inten                   2
#define bitGENER_DMA_Reg0x0_half_finish_inten                   0x4
#define set_GENER_DMA_Reg0x0_half_finish_inten(val)             addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_GENER_DMA_Reg0x0_half_finish_inten                 addGENER_DMA_Reg0x0 |= 0x4
#define clrf_GENER_DMA_Reg0x0_half_finish_inten                 addGENER_DMA_Reg0x0 &= ~0x4
#define get_GENER_DMA_Reg0x0_half_finish_inten                  ((addGENER_DMA_Reg0x0 & 0x4) >> 2)

#define posGENER_DMA_Reg0x0_finish_inten                        1
#define bitGENER_DMA_Reg0x0_finish_inten                        0x2
#define set_GENER_DMA_Reg0x0_finish_inten(val)                  addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_GENER_DMA_Reg0x0_finish_inten                      addGENER_DMA_Reg0x0 |= 0x2
#define clrf_GENER_DMA_Reg0x0_finish_inten                      addGENER_DMA_Reg0x0 &= ~0x2
#define get_GENER_DMA_Reg0x0_finish_inten                       ((addGENER_DMA_Reg0x0 & 0x2) >> 1)

#define posGENER_DMA_Reg0x0_dma_en                              0
#define bitGENER_DMA_Reg0x0_dma_en                              0x1
#define set_GENER_DMA_Reg0x0_dma_en(val)                        addGENER_DMA_Reg0x0 = ((addGENER_DMA_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_GENER_DMA_Reg0x0_dma_en                            addGENER_DMA_Reg0x0 |= 0x1
#define clrf_GENER_DMA_Reg0x0_dma_en                            addGENER_DMA_Reg0x0 &= ~0x1
#define get_GENER_DMA_Reg0x0_dma_en                             (addGENER_DMA_Reg0x0 & 0x1)

//addGENER_DMA_Reg0x1
#define addGENER_DMA_Reg0x1                                     *((volatile unsigned long *) (0x44020000+0x1*4))

//addGENER_DMA_Reg0x2
#define addGENER_DMA_Reg0x2                                     *((volatile unsigned long *) (0x44020000+0x2*4))

//addGENER_DMA_Reg0x3
#define addGENER_DMA_Reg0x3                                     *((volatile unsigned long *) (0x44020000+0x3*4))

//addGENER_DMA_Reg0x4
#define addGENER_DMA_Reg0x4                                     *((volatile unsigned long *) (0x44020000+0x4*4))

//addGENER_DMA_Reg0x5
#define addGENER_DMA_Reg0x5                                     *((volatile unsigned long *) (0x44020000+0x5*4))

//addGENER_DMA_Reg0x6
#define addGENER_DMA_Reg0x6                                     *((volatile unsigned long *) (0x44020000+0x6*4))

//addGENER_DMA_Reg0x7
#define addGENER_DMA_Reg0x7                                     *((volatile unsigned long *) (0x44020000+0x7*4))

#define posGENER_DMA_Reg0x7_dest_wr_intlv                       16
#define bitGENER_DMA_Reg0x7_dest_wr_intlv                       0xF0000
#define set_GENER_DMA_Reg0x7_dest_wr_intlv(val)                 addGENER_DMA_Reg0x7 = ((addGENER_DMA_Reg0x7 & (~0xF0000)) | ((val) << 16))
#define get_GENER_DMA_Reg0x7_dest_wr_intlv                      ((addGENER_DMA_Reg0x7 & 0xF0000) >> 16)

#define posGENER_DMA_Reg0x7_src_rd_intlv                        12
#define bitGENER_DMA_Reg0x7_src_rd_intlv                        0xF000
#define set_GENER_DMA_Reg0x7_src_rd_intlv(val)                  addGENER_DMA_Reg0x7 = ((addGENER_DMA_Reg0x7 & (~0xF000)) | ((val) << 12))
#define get_GENER_DMA_Reg0x7_src_rd_intlv                       ((addGENER_DMA_Reg0x7 & 0xF000) >> 12)


#define posGENER_DMA_Reg0x7_dest_req_mux                        4
#define bitGENER_DMA_Reg0x7_dest_req_mux                        0xF0
#define set_GENER_DMA_Reg0x7_dest_req_mux(val)                  addGENER_DMA_Reg0x7 = ((addGENER_DMA_Reg0x7 & (~0xF0)) | ((val) << 4))
#define get_GENER_DMA_Reg0x7_dest_req_mux                       ((addGENER_DMA_Reg0x7 & 0xF0) >> 4)

#define posGENER_DMA_Reg0x7_src_req_mux                         0
#define bitGENER_DMA_Reg0x7_src_req_mux                         0xF
#define set_GENER_DMA_Reg0x7_src_req_mux(val)                   addGENER_DMA_Reg0x7 = ((addGENER_DMA_Reg0x7 & (~0xF)) | ((val) << 0))
#define get_GENER_DMA_Reg0x7_src_req_mux                        (addGENER_DMA_Reg0x7 & 0xF)

//addGENER_DMA_Reg0x8
#define addGENER_DMA_Reg0x8                                     *((volatile unsigned long *) (0x44020000+0x8*4))

//addGENER_DMA_Reg0x9
#define addGENER_DMA_Reg0x9                                     *((volatile unsigned long *) (0x44020000+0x9*4))

//addGENER_DMA_Reg0xa
#define addGENER_DMA_Reg0xa                                     *((volatile unsigned long *) (0x44020000+0xa*4))

//addGENER_DMA_Reg0xb
#define addGENER_DMA_Reg0xb                                     *((volatile unsigned long *) (0x44020000+0xb*4))

//addGENER_DMA_Reg0xc
#define addGENER_DMA_Reg0xc                                     *((volatile unsigned long *) (0x44020000+0xc*4))

//addGENER_DMA_Reg0xd
#define addGENER_DMA_Reg0xd                                     *((volatile unsigned long *) (0x44020000+0xd*4))

//addGENER_DMA_Reg0xe
#define addGENER_DMA_Reg0xe                                     *((volatile unsigned long *) (0x44020000+0xe*4))

//addGENER_DMA_Reg0xf
#define addGENER_DMA_Reg0xf                                     *((volatile unsigned long *) (0x44020000+0xf*4))

//addGENER_DMA_Reg0x10
#define addGENER_DMA_Reg0x10                                    *((volatile unsigned long *) (0x44020000+0x10*4))

//addGENER_DMA_Reg0x11
#define addGENER_DMA_Reg0x11                                    *((volatile unsigned long *) (0x44020000+0x11*4))

//addGENER_DMA_Reg0x12
#define addGENER_DMA_Reg0x12                                    *((volatile unsigned long *) (0x44020000+0x12*4))

//addGENER_DMA_Reg0x13
#define addGENER_DMA_Reg0x13                                    *((volatile unsigned long *) (0x44020000+0x13*4))

//addGENER_DMA_Reg0x14
#define addGENER_DMA_Reg0x14                                    *((volatile unsigned long *) (0x44020000+0x14*4))

//addGENER_DMA_Reg0x15
#define addGENER_DMA_Reg0x15                                    *((volatile unsigned long *) (0x44020000+0x15*4))

//addGENER_DMA_Reg0x16
#define addGENER_DMA_Reg0x16                                    *((volatile unsigned long *) (0x44020000+0x16*4))

//addGENER_DMA_Reg0x17
#define addGENER_DMA_Reg0x17                                    *((volatile unsigned long *) (0x44020000+0x17*4))

//addGENER_DMA_Reg0x18
#define addGENER_DMA_Reg0x18                                    *((volatile unsigned long *) (0x44020000+0x18*4))

//addGENER_DMA_Reg0x19
#define addGENER_DMA_Reg0x19                                    *((volatile unsigned long *) (0x44020000+0x19*4))

//addGENER_DMA_Reg0x1a
#define addGENER_DMA_Reg0x1a                                    *((volatile unsigned long *) (0x44020000+0x1a*4))

//addGENER_DMA_Reg0x1b
#define addGENER_DMA_Reg0x1b                                    *((volatile unsigned long *) (0x44020000+0x1b*4))

//addGENER_DMA_Reg0x1c
#define addGENER_DMA_Reg0x1c                                    *((volatile unsigned long *) (0x44020000+0x1c*4))

//addGENER_DMA_Reg0x1d
#define addGENER_DMA_Reg0x1d                                    *((volatile unsigned long *) (0x44020000+0x1d*4))

//addGENER_DMA_Reg0x1e
#define addGENER_DMA_Reg0x1e                                    *((volatile unsigned long *) (0x44020000+0x1e*4))

//addGENER_DMA_Reg0x1f
#define addGENER_DMA_Reg0x1f                                    *((volatile unsigned long *) (0x44020000+0x1f*4))

//addGENER_DMA_Reg0x20
#define addGENER_DMA_Reg0x20                                    *((volatile unsigned long *) (0x44020000+0x20*4))

//addGENER_DMA_Reg0x21
#define addGENER_DMA_Reg0x21                                    *((volatile unsigned long *) (0x44020000+0x21*4))

//addGENER_DMA_Reg0x22
#define addGENER_DMA_Reg0x22                                    *((volatile unsigned long *) (0x44020000+0x22*4))

//addGENER_DMA_Reg0x23
#define addGENER_DMA_Reg0x23                                    *((volatile unsigned long *) (0x44020000+0x23*4))

//addGENER_DMA_Reg0x24
#define addGENER_DMA_Reg0x24                                    *((volatile unsigned long *) (0x44020000+0x24*4))

//addGENER_DMA_Reg0x25
#define addGENER_DMA_Reg0x25                                    *((volatile unsigned long *) (0x44020000+0x25*4))

//addGENER_DMA_Reg0x26
#define addGENER_DMA_Reg0x26                                    *((volatile unsigned long *) (0x44020000+0x26*4))

//addGENER_DMA_Reg0x27
#define addGENER_DMA_Reg0x27                                    *((volatile unsigned long *) (0x44020000+0x27*4))

//addGENER_DMA_Reg0x28
#define addGENER_DMA_Reg0x28                                    *((volatile unsigned long *) (0x44020000+0x28*4))

//addGENER_DMA_Reg0x29
#define addGENER_DMA_Reg0x29                                    *((volatile unsigned long *) (0x44020000+0x29*4))

//addGENER_DMA_Reg0x2a
#define addGENER_DMA_Reg0x2a                                    *((volatile unsigned long *) (0x44020000+0x2a*4))

//addGENER_DMA_Reg0x2b
#define addGENER_DMA_Reg0x2b                                    *((volatile unsigned long *) (0x44020000+0x2b*4))

//addGENER_DMA_Reg0x2c
#define addGENER_DMA_Reg0x2c                                    *((volatile unsigned long *) (0x44020000+0x2c*4))

//addGENER_DMA_Reg0x2d
#define addGENER_DMA_Reg0x2d                                    *((volatile unsigned long *) (0x44020000+0x2d*4))

//addGENER_DMA_Reg0x2e
#define addGENER_DMA_Reg0x2e                                    *((volatile unsigned long *) (0x44020000+0x2e*4))

//addGENER_DMA_Reg0x2f
#define addGENER_DMA_Reg0x2f                                    *((volatile unsigned long *) (0x44020000+0x2f*4))

//addGENER_DMA_Reg0x30
#define addGENER_DMA_Reg0x30                                    *((volatile unsigned long *) (0x44020000+0x30*4))
#define posGENER_DMA_Reg0x30_intcnt_half_finish                 28
#define bitGENER_DMA_Reg0x30_intcnt_half_finish                 0xF0000000
#define get_GENER_DMA_Reg0x30_intcnt_half_finish                ((addGENER_DMA_Reg0x30 & 0xF0000000) >> 28)

#define posGENER_DMA_Reg0x30_intcnt_finish                      24
#define bitGENER_DMA_Reg0x30_intcnt_finish                      0xF000000
#define get_GENER_DMA_Reg0x30_intcnt_finish                     ((addGENER_DMA_Reg0x30 & 0xF000000) >> 24)


#define posGENER_DMA_Reg0x30_flush_src_buff                     17
#define bitGENER_DMA_Reg0x30_flush_src_buff                     0x20000
#define set_GENER_DMA_Reg0x30_flush_src_buff(val)               addGENER_DMA_Reg0x30 = ((addGENER_DMA_Reg0x30 & (~0x20000)) | ((val) << 17))
#define setf_GENER_DMA_Reg0x30_flush_src_buff                   addGENER_DMA_Reg0x30 |= 0x20000
#define clrf_GENER_DMA_Reg0x30_flush_src_buff                   addGENER_DMA_Reg0x30 &= ~0x20000
#define get_GENER_DMA_Reg0x30_flush_src_buff                    ((addGENER_DMA_Reg0x30 & 0x20000) >> 17)

#define posGENER_DMA_Reg0x30_dma0_remain_len                    0
#define bitGENER_DMA_Reg0x30_dma0_remain_len                    0x1FFFF
#define get_GENER_DMA_Reg0x30_dma0_remain_len                   (addGENER_DMA_Reg0x30 & 0x1FFFF)

//addGENER_DMA_Reg0x31
#define addGENER_DMA_Reg0x31                                    *((volatile unsigned long *) (0x44020000+0x31*4))

//addGENER_DMA_Reg0x32
#define addGENER_DMA_Reg0x32                                    *((volatile unsigned long *) (0x44020000+0x32*4))

//addGENER_DMA_Reg0x33
#define addGENER_DMA_Reg0x33                                    *((volatile unsigned long *) (0x44020000+0x33*4))

//addGENER_DMA_Reg0x34
#define addGENER_DMA_Reg0x34                                    *((volatile unsigned long *) (0x44020000+0x34*4))

//addGENER_DMA_Reg0x35
#define addGENER_DMA_Reg0x35                                    *((volatile unsigned long *) (0x44020000+0x35*4))

//addGENER_DMA_Reg0x36
#define addGENER_DMA_Reg0x36                                    *((volatile unsigned long *) (0x44020000+0x36*4))

//addGENER_DMA_Reg0x37
#define addGENER_DMA_Reg0x37                                    *((volatile unsigned long *) (0x44020000+0x37*4))

#define posGENER_DMA_Reg0x37_prio_mode                          0
#define bitGENER_DMA_Reg0x37_prio_mode                          0x1
#define set_GENER_DMA_Reg0x37_prio_mode(val)                    addGENER_DMA_Reg0x37 = ((addGENER_DMA_Reg0x37 & (~0x1)) | ((val) << 0))
#define setf_GENER_DMA_Reg0x37_prio_mode                        addGENER_DMA_Reg0x37 |= 0x1
#define clrf_GENER_DMA_Reg0x37_prio_mode                        addGENER_DMA_Reg0x37 &= ~0x1
#define get_GENER_DMA_Reg0x37_prio_mode                         (addGENER_DMA_Reg0x37 & 0x1)

//addGENER_DMA_Reg0x38
#define addGENER_DMA_Reg0x38                                    *((volatile unsigned long *) (0x44020000+0x38*4))

#define posGENER_DMA_Reg0x38_int_half_finish                    8
#define bitGENER_DMA_Reg0x38_int_half_finish                    0x3F00
#define set_GENER_DMA_Reg0x38_int_half_finish(val)              addGENER_DMA_Reg0x38 = ((addGENER_DMA_Reg0x38 & (~0x3F00)) | ((val) << 8))
#define get_GENER_DMA_Reg0x38_int_half_finish                   ((addGENER_DMA_Reg0x38 & 0x3F00) >> 8)


#define posGENER_DMA_Reg0x38_int_finish                         0
#define bitGENER_DMA_Reg0x38_int_finish                         0x3F
#define set_GENER_DMA_Reg0x38_int_finish(val)                   addGENER_DMA_Reg0x38 = ((addGENER_DMA_Reg0x38 & (~0x3F)) | ((val) << 0))
#define get_GENER_DMA_Reg0x38_int_finish                        (addGENER_DMA_Reg0x38 & 0x3F)


//************************************************************//
//SBC
//************************************************************//
#define BASEADDR_SBC                                            0x47010000
//addSBC_Reg0x0
#define addSBC_Reg0x0                                           *((volatile unsigned long *) (0x47010000+0x0*4))
#define posSBC_Reg0x0_sbc_en                                    0
#define bitSBC_Reg0x0_sbc_en                                    0x1
#define set_SBC_Reg0x0_sbc_en(val)                              addSBC_Reg0x0 = ((addSBC_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_SBC_Reg0x0_sbc_en                                  addSBC_Reg0x0 |= 0x1
#define clrf_SBC_Reg0x0_sbc_en                                  addSBC_Reg0x0 &= ~0x1
#define get_SBC_Reg0x0_sbc_en                                   (addSBC_Reg0x0 & 0x1)

#define posSBC_Reg0x0_sbc_channel                               1
#define bitSBC_Reg0x0_sbc_channel                               0x2
#define set_SBC_Reg0x0_sbc_channel(val)                         addSBC_Reg0x0 = ((addSBC_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_SBC_Reg0x0_sbc_channel                             addSBC_Reg0x0 |= 0x2
#define clrf_SBC_Reg0x0_sbc_channel                             addSBC_Reg0x0 &= ~0x2
#define get_SBC_Reg0x0_sbc_channel                              ((addSBC_Reg0x0 & 0x2) >> 1)

#define posSBC_Reg0x0_sbc_subband                               2
#define bitSBC_Reg0x0_sbc_subband                               0x4
#define set_SBC_Reg0x0_sbc_subband(val)                         addSBC_Reg0x0 = ((addSBC_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_SBC_Reg0x0_sbc_subband                             addSBC_Reg0x0 |= 0x4
#define clrf_SBC_Reg0x0_sbc_subband                             addSBC_Reg0x0 &= ~0x4
#define get_SBC_Reg0x0_sbc_subband                              ((addSBC_Reg0x0 & 0x4) >> 2)

#define posSBC_Reg0x0_sbc_chn_comb                              3
#define bitSBC_Reg0x0_sbc_chn_comb                              0x8
#define set_SBC_Reg0x0_sbc_chn_comb(val)                        addSBC_Reg0x0 = ((addSBC_Reg0x0 & (~0x8)) | ((val) << 3))
#define setf_SBC_Reg0x0_sbc_chn_comb                            addSBC_Reg0x0 |= 0x8
#define clrf_SBC_Reg0x0_sbc_chn_comb                            addSBC_Reg0x0 &= ~0x8
#define get_SBC_Reg0x0_sbc_chn_comb                             ((addSBC_Reg0x0 & 0x8) >> 3)

#define posSBC_Reg0x0_sbc_blocks                                4
#define bitSBC_Reg0x0_sbc_blocks                                0x30
#define set_SBC_Reg0x0_sbc_blocks(val)                          addSBC_Reg0x0 = ((addSBC_Reg0x0 & (~0x30)) | ((val) << 4))
#define get_SBC_Reg0x0_sbc_blocks                               ((addSBC_Reg0x0 & 0x30) >> 4)

#define posSBC_Reg0x0_msbc_support                              6
#define bitSBC_Reg0x0_msbc_support                              0x40
#define set_SBC_Reg0x0_msbc_support(val)                        addSBC_Reg0x0 = ((addSBC_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_SBC_Reg0x0_msbc_support                            addSBC_Reg0x0 |= 0x40
#define clrf_SBC_Reg0x0_msbc_support                            addSBC_Reg0x0 &= ~0x40
#define get_SBC_Reg0x0_msbc_support                             ((addSBC_Reg0x0 & 0x40) >> 6)

//addSBC_Reg0x1
#define addSBC_Reg0x1                                           *((volatile unsigned long *) (0x47010000+0x1*4))
#define posSBC_Reg0x1_sbc_int                                   0
#define bitSBC_Reg0x1_sbc_int                                   0x1
#define get_SBC_Reg0x1_sbc_int                                  (addSBC_Reg0x1 & 0x1)

#define posSBC_Reg0x1_sbc_idle                                  1
#define bitSBC_Reg0x1_sbc_idle                                  0x2
#define get_SBC_Reg0x1_sbc_idle                                 ((addSBC_Reg0x1 & 0x2) >> 1)

//addSBC_Reg0x2
#define addSBC_Reg0x2                                           *((volatile unsigned long *) (0x47010000+0x2*4))
#define posSBC_Reg0x2_byte_reso                                 0
#define bitSBC_Reg0x2_byte_reso                                 0xFF
#define set_SBC_Reg0x2_byte_reso(val)                           addSBC_Reg0x2 = ((addSBC_Reg0x2 & (~0xFF)) | ((val) << 0))
#define get_SBC_Reg0x2_byte_reso                                (addSBC_Reg0x2 & 0xFF)

//addSBC_Reg0x3
#define addSBC_Reg0x3                                           *((volatile unsigned long *) (0x47010000+0x3*4))
#define posSBC_Reg0x3_byte_reso                                 0
#define bitSBC_Reg0x3_byte_reso                                 0xFF
#define set_SBC_Reg0x3_byte_reso(val)                           addSBC_Reg0x3 = ((addSBC_Reg0x3 & (~0xFF)) | ((val) << 0))
#define get_SBC_Reg0x3_byte_reso                                (addSBC_Reg0x3 & 0xFF)

//addSBC_Reg0x4
#define addSBC_Reg0x4                                           *((volatile unsigned long *) (0x47010000+0x4*4))
#define posSBC_Reg0x4_byte_reso                                 0
#define bitSBC_Reg0x4_byte_reso                                 0xFF
#define set_SBC_Reg0x4_byte_reso(val)                           addSBC_Reg0x4 = ((addSBC_Reg0x4 & (~0xFF)) | ((val) << 0))
#define get_SBC_Reg0x4_byte_reso                                (addSBC_Reg0x4 & 0xFF)

//addSBC_Reg0x5
#define addSBC_Reg0x5                                           *((volatile unsigned long *) (0x47010000+0x5*4))
#define posSBC_Reg0x5_sbc_bitnum                                0
#define bitSBC_Reg0x5_sbc_bitnum                                0xFF
#define set_SBC_Reg0x5_sbc_bitnum(val)                          addSBC_Reg0x5 = ((addSBC_Reg0x5 & (~0xFF)) | ((val) << 0))
#define get_SBC_Reg0x5_sbc_bitnum                               (addSBC_Reg0x5 & 0xFF)

//addSBC_Reg0x6
#define addSBC_Reg0x6                                           *((volatile unsigned long *) (0x47010000+0x6*4))
#define posSBC_Reg0x6_sbc_scale_factor                          0
#define bitSBC_Reg0x6_sbc_scale_factor                          0xFF
#define set_SBC_Reg0x6_sbc_scale_factor(val)                    addSBC_Reg0x6 = ((addSBC_Reg0x6 & (~0xFF)) | ((val) << 0))
#define get_SBC_Reg0x6_sbc_scale_factor                         (addSBC_Reg0x6 & 0xFF)

//addSBC_Reg0x7
#define addSBC_Reg0x7                                           *((volatile unsigned long *) (0x47010000+0x7*4))

//addSBC_Reg0x8
#define addSBC_Reg0x8                                           *((volatile unsigned long *) (0x47010000+0x8*4))
#define posSBC_Reg0x8_sbc_res_bit                               0
#define bitSBC_Reg0x8_sbc_res_bit                               0x7
#define set_SBC_Reg0x8_sbc_res_bit(val)                         addSBC_Reg0x8 = ((addSBC_Reg0x8 & (~0x7)) | ((val) << 0))
#define get_SBC_Reg0x8_sbc_res_bit                              (addSBC_Reg0x8 & 0x7)

//addSBC_Reg0x9
#define addSBC_Reg0x9                                           *((volatile unsigned long *) (0x47010000+0x9*4))
#define posSBC_Reg0x9_dec_en                                    0
#define bitSBC_Reg0x9_dec_en                                    0x1
#define set_SBC_Reg0x9_dec_en(val)                              addSBC_Reg0x9 = ((addSBC_Reg0x9 & (~0x1)) | ((val) << 0))
#define setf_SBC_Reg0x9_dec_en                                  addSBC_Reg0x9 |= 0x1
#define clrf_SBC_Reg0x9_dec_en                                  addSBC_Reg0x9 &= ~0x1
#define get_SBC_Reg0x9_dec_en                                   (addSBC_Reg0x9 & 0x1)

//addSBC_Reg0x100
#define addSBC_Reg0x100                                         *((volatile unsigned long *) (0x47010000+0x100*4))

//addSBC_Reg0x200
#define addSBC_Reg0x200                                         *((volatile unsigned long *) (0x47010000+0x200*4))


//************************************************************//
//UART0
//************************************************************//
#define BASEADDR_UART0                                          0x44820000
//addUART0_Reg0x0
#define addUART0_Reg0x0                                         *((volatile unsigned long *) (0x44820000+0x0*4))
#define posUART0_Reg0x0_UART_TX_ENABLE                          0
#define bitUART0_Reg0x0_UART_TX_ENABLE                          0x1
#define set_UART0_Reg0x0_UART_TX_ENABLE(val)                    addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_UART0_Reg0x0_UART_TX_ENABLE                        addUART0_Reg0x0 |= 0x1
#define clrf_UART0_Reg0x0_UART_TX_ENABLE                        addUART0_Reg0x0 &= ~0x1
#define get_UART0_Reg0x0_UART_TX_ENABLE                         (addUART0_Reg0x0 & 0x1)

#define posUART0_Reg0x0_UART_RX_ENABLE                          1
#define bitUART0_Reg0x0_UART_RX_ENABLE                          0x2
#define set_UART0_Reg0x0_UART_RX_ENABLE(val)                    addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_UART0_Reg0x0_UART_RX_ENABLE                        addUART0_Reg0x0 |= 0x2
#define clrf_UART0_Reg0x0_UART_RX_ENABLE                        addUART0_Reg0x0 &= ~0x2
#define get_UART0_Reg0x0_UART_RX_ENABLE                         ((addUART0_Reg0x0 & 0x2) >> 1)

#define posUART0_Reg0x0_UART_IRDA                               2
#define bitUART0_Reg0x0_UART_IRDA                               0x4
#define set_UART0_Reg0x0_UART_IRDA(val)                         addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_UART0_Reg0x0_UART_IRDA                             addUART0_Reg0x0 |= 0x4
#define clrf_UART0_Reg0x0_UART_IRDA                             addUART0_Reg0x0 &= ~0x4
#define get_UART0_Reg0x0_UART_IRDA                              ((addUART0_Reg0x0 & 0x4) >> 2)

#define posUART0_Reg0x0_UART_LEN                                3
#define bitUART0_Reg0x0_UART_LEN                                0x18
#define set_UART0_Reg0x0_UART_LEN(val)                          addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x18)) | ((val) << 3))
#define get_UART0_Reg0x0_UART_LEN                               ((addUART0_Reg0x0 & 0x18) >> 3)

#define posUART0_Reg0x0_UART_PAR_EN                             5
#define bitUART0_Reg0x0_UART_PAR_EN                             0x20
#define set_UART0_Reg0x0_UART_PAR_EN(val)                       addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_UART0_Reg0x0_UART_PAR_EN                           addUART0_Reg0x0 |= 0x20
#define clrf_UART0_Reg0x0_UART_PAR_EN                           addUART0_Reg0x0 &= ~0x20
#define get_UART0_Reg0x0_UART_PAR_EN                            ((addUART0_Reg0x0 & 0x20) >> 5)

#define posUART0_Reg0x0_UART_PAR_MODE                           6
#define bitUART0_Reg0x0_UART_PAR_MODE                           0x40
#define set_UART0_Reg0x0_UART_PAR_MODE(val)                     addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_UART0_Reg0x0_UART_PAR_MODE                         addUART0_Reg0x0 |= 0x40
#define clrf_UART0_Reg0x0_UART_PAR_MODE                         addUART0_Reg0x0 &= ~0x40
#define get_UART0_Reg0x0_UART_PAR_MODE                          ((addUART0_Reg0x0 & 0x40) >> 6)

#define posUART0_Reg0x0_UART_STOP_LEN                           7
#define bitUART0_Reg0x0_UART_STOP_LEN                           0x80
#define set_UART0_Reg0x0_UART_STOP_LEN(val)                     addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_UART0_Reg0x0_UART_STOP_LEN                         addUART0_Reg0x0 |= 0x80
#define clrf_UART0_Reg0x0_UART_STOP_LEN                         addUART0_Reg0x0 &= ~0x80
#define get_UART0_Reg0x0_UART_STOP_LEN                          ((addUART0_Reg0x0 & 0x80) >> 7)

#define posUART0_Reg0x0_UART_CLK_DIVID                          8
#define bitUART0_Reg0x0_UART_CLK_DIVID                          0x1FFF00
#define set_UART0_Reg0x0_UART_CLK_DIVID(val)                    addUART0_Reg0x0 = ((addUART0_Reg0x0 & (~0x1FFF00)) | ((val) << 8))
#define get_UART0_Reg0x0_UART_CLK_DIVID                         ((addUART0_Reg0x0 & 0x1FFF00) >> 8)

//addUART0_Reg0x1
#define addUART0_Reg0x1                                         *((volatile unsigned long *) (0x44820000+0x1*4))
#define posUART0_Reg0x1_TX_FIFO_THRESHOLD                       0
#define bitUART0_Reg0x1_TX_FIFO_THRESHOLD                       0xFF
#define set_UART0_Reg0x1_TX_FIFO_THRESHOLD(val)                 addUART0_Reg0x1 = ((addUART0_Reg0x1 & (~0xFF)) | ((val) << 0))
#define get_UART0_Reg0x1_TX_FIFO_THRESHOLD                      (addUART0_Reg0x1 & 0xFF)

#define posUART0_Reg0x1_RX_FIFO_THRESHOLD                       8
#define bitUART0_Reg0x1_RX_FIFO_THRESHOLD                       0xFF00
#define set_UART0_Reg0x1_RX_FIFO_THRESHOLD(val)                 addUART0_Reg0x1 = ((addUART0_Reg0x1 & (~0xFF00)) | ((val) << 8))
#define get_UART0_Reg0x1_RX_FIFO_THRESHOLD                      ((addUART0_Reg0x1 & 0xFF00) >> 8)

#define posUART0_Reg0x1_RX_STOP_DETECT_TIME                     16
#define bitUART0_Reg0x1_RX_STOP_DETECT_TIME                     0x30000
#define set_UART0_Reg0x1_RX_STOP_DETECT_TIME(val)               addUART0_Reg0x1 = ((addUART0_Reg0x1 & (~0x30000)) | ((val) << 16))
#define get_UART0_Reg0x1_RX_STOP_DETECT_TIME                    ((addUART0_Reg0x1 & 0x30000) >> 16)

//addUART0_Reg0x2
#define addUART0_Reg0x2                                         *((volatile unsigned long *) (0x44820000+0x2*4))
#define posUART0_Reg0x2_TX_FIFO_COUNT                           0
#define bitUART0_Reg0x2_TX_FIFO_COUNT                           0xFF
#define get_UART0_Reg0x2_TX_FIFO_COUNT                          (addUART0_Reg0x2 & 0xFF)

#define posUART0_Reg0x2_RX_FIFO_COUNT                           8
#define bitUART0_Reg0x2_RX_FIFO_COUNT                           0xFF00
#define get_UART0_Reg0x2_RX_FIFO_COUNT                          ((addUART0_Reg0x2 & 0xFF00) >> 8)

#define posUART0_Reg0x2_TX_FIFO_FULL                            16
#define bitUART0_Reg0x2_TX_FIFO_FULL                            0x10000
#define get_UART0_Reg0x2_TX_FIFO_FULL                           ((addUART0_Reg0x2 & 0x10000) >> 16)

#define posUART0_Reg0x2_TX_FIFO_EMPTY                           17
#define bitUART0_Reg0x2_TX_FIFO_EMPTY                           0x20000
#define get_UART0_Reg0x2_TX_FIFO_EMPTY                          ((addUART0_Reg0x2 & 0x20000) >> 17)

#define posUART0_Reg0x2_RX_FIFO_FULL                            18
#define bitUART0_Reg0x2_RX_FIFO_FULL                            0x40000
#define get_UART0_Reg0x2_RX_FIFO_FULL                           ((addUART0_Reg0x2 & 0x40000) >> 18)

#define posUART0_Reg0x2_RX_FIFO_EMPTY                           19
#define bitUART0_Reg0x2_RX_FIFO_EMPTY                           0x80000
#define get_UART0_Reg0x2_RX_FIFO_EMPTY                          ((addUART0_Reg0x2 & 0x80000) >> 19)

#define posUART0_Reg0x2_FIFO_WR_READY                           20
#define bitUART0_Reg0x2_FIFO_WR_READY                           0x100000
#define get_UART0_Reg0x2_FIFO_WR_READY                          ((addUART0_Reg0x2 & 0x100000) >> 20)

#define posUART0_Reg0x2_FIFO_RD_READY                           21
#define bitUART0_Reg0x2_FIFO_RD_READY                           0x200000
#define get_UART0_Reg0x2_FIFO_RD_READY                          ((addUART0_Reg0x2 & 0x200000) >> 21)

//addUART0_Reg0x3
#define addUART0_Reg0x3                                         *((volatile unsigned long *) (0x44820000+0x3*4))
#define posUART0_Reg0x3_UART_TX_FIFO_DIN                        0
#define bitUART0_Reg0x3_UART_TX_FIFO_DIN                        0xFF
#define set_UART0_Reg0x3_UART_TX_FIFO_DIN(val)                  addUART0_Reg0x3 = ((addUART0_Reg0x3 & (~0xFF)) | ((val) << 0))
#define get_UART0_Reg0x3_UART_TX_FIFO_DIN                       (addUART0_Reg0x3 & 0xFF)

#define posUART0_Reg0x3_UART_RX_FIFO_DOUT                       8
#define bitUART0_Reg0x3_UART_RX_FIFO_DOUT                       0xFF00
#define get_UART0_Reg0x3_UART_RX_FIFO_DOUT                      ((addUART0_Reg0x3 & 0xFF00) >> 8)

//addUART0_Reg0x4
#define addUART0_Reg0x4                                         *((volatile unsigned long *) (0x44820000+0x4*4))
#define posUART0_Reg0x4_TX_FIFO_NEED_WRITE_MASK                 0
#define bitUART0_Reg0x4_TX_FIFO_NEED_WRITE_MASK                 0x1
#define set_UART0_Reg0x4_TX_FIFO_NEED_WRITE_MASK(val)           addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_UART0_Reg0x4_TX_FIFO_NEED_WRITE_MASK               addUART0_Reg0x4 |= 0x1
#define clrf_UART0_Reg0x4_TX_FIFO_NEED_WRITE_MASK               addUART0_Reg0x4 &= ~0x1
#define get_UART0_Reg0x4_TX_FIFO_NEED_WRITE_MASK                (addUART0_Reg0x4 & 0x1)

#define posUART0_Reg0x4_RX_FIFO_NEED_READ_MASK                  1
#define bitUART0_Reg0x4_RX_FIFO_NEED_READ_MASK                  0x2
#define set_UART0_Reg0x4_RX_FIFO_NEED_READ_MASK(val)            addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x2)) | ((val) << 1))
#define setf_UART0_Reg0x4_RX_FIFO_NEED_READ_MASK                addUART0_Reg0x4 |= 0x2
#define clrf_UART0_Reg0x4_RX_FIFO_NEED_READ_MASK                addUART0_Reg0x4 &= ~0x2
#define get_UART0_Reg0x4_RX_FIFO_NEED_READ_MASK                 ((addUART0_Reg0x4 & 0x2) >> 1)

#define posUART0_Reg0x4_RX_FIFO_OVER_FLOW_MASK                  2
#define bitUART0_Reg0x4_RX_FIFO_OVER_FLOW_MASK                  0x4
#define set_UART0_Reg0x4_RX_FIFO_OVER_FLOW_MASK(val)            addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x4)) | ((val) << 2))
#define setf_UART0_Reg0x4_RX_FIFO_OVER_FLOW_MASK                addUART0_Reg0x4 |= 0x4
#define clrf_UART0_Reg0x4_RX_FIFO_OVER_FLOW_MASK                addUART0_Reg0x4 &= ~0x4
#define get_UART0_Reg0x4_RX_FIFO_OVER_FLOW_MASK                 ((addUART0_Reg0x4 & 0x4) >> 2)

#define posUART0_Reg0x4_UART_RX_PARITY_ERR_MASK                 3
#define bitUART0_Reg0x4_UART_RX_PARITY_ERR_MASK                 0x8
#define set_UART0_Reg0x4_UART_RX_PARITY_ERR_MASK(val)           addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x8)) | ((val) << 3))
#define setf_UART0_Reg0x4_UART_RX_PARITY_ERR_MASK               addUART0_Reg0x4 |= 0x8
#define clrf_UART0_Reg0x4_UART_RX_PARITY_ERR_MASK               addUART0_Reg0x4 &= ~0x8
#define get_UART0_Reg0x4_UART_RX_PARITY_ERR_MASK                ((addUART0_Reg0x4 & 0x8) >> 3)

#define posUART0_Reg0x4_UART_RX_STOP_ERR_MASK                   4
#define bitUART0_Reg0x4_UART_RX_STOP_ERR_MASK                   0x10
#define set_UART0_Reg0x4_UART_RX_STOP_ERR_MASK(val)             addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x10)) | ((val) << 4))
#define setf_UART0_Reg0x4_UART_RX_STOP_ERR_MASK                 addUART0_Reg0x4 |= 0x10
#define clrf_UART0_Reg0x4_UART_RX_STOP_ERR_MASK                 addUART0_Reg0x4 &= ~0x10
#define get_UART0_Reg0x4_UART_RX_STOP_ERR_MASK                  ((addUART0_Reg0x4 & 0x10) >> 4)

#define posUART0_Reg0x4_UART_TX_STOP_END_MASK                   5
#define bitUART0_Reg0x4_UART_TX_STOP_END_MASK                   0x20
#define set_UART0_Reg0x4_UART_TX_STOP_END_MASK(val)             addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x20)) | ((val) << 5))
#define setf_UART0_Reg0x4_UART_TX_STOP_END_MASK                 addUART0_Reg0x4 |= 0x20
#define clrf_UART0_Reg0x4_UART_TX_STOP_END_MASK                 addUART0_Reg0x4 &= ~0x20
#define get_UART0_Reg0x4_UART_TX_STOP_END_MASK                  ((addUART0_Reg0x4 & 0x20) >> 5)

#define posUART0_Reg0x4_UART_RX_STOP_END_MASK                   6
#define bitUART0_Reg0x4_UART_RX_STOP_END_MASK                   0x40
#define set_UART0_Reg0x4_UART_RX_STOP_END_MASK(val)             addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x40)) | ((val) << 6))
#define setf_UART0_Reg0x4_UART_RX_STOP_END_MASK                 addUART0_Reg0x4 |= 0x40
#define clrf_UART0_Reg0x4_UART_RX_STOP_END_MASK                 addUART0_Reg0x4 &= ~0x40
#define get_UART0_Reg0x4_UART_RX_STOP_END_MASK                  ((addUART0_Reg0x4 & 0x40) >> 6)

#define posUART0_Reg0x4_UART_RXD_WAKEUP_MASK                    7
#define bitUART0_Reg0x4_UART_RXD_WAKEUP_MASK                    0x80
#define set_UART0_Reg0x4_UART_RXD_WAKEUP_MASK(val)              addUART0_Reg0x4 = ((addUART0_Reg0x4 & (~0x80)) | ((val) << 7))
#define setf_UART0_Reg0x4_UART_RXD_WAKEUP_MASK                  addUART0_Reg0x4 |= 0x80
#define clrf_UART0_Reg0x4_UART_RXD_WAKEUP_MASK                  addUART0_Reg0x4 &= ~0x80
#define get_UART0_Reg0x4_UART_RXD_WAKEUP_MASK                   ((addUART0_Reg0x4 & 0x80) >> 7)

//addUART0_Reg0x5
#define addUART0_Reg0x5                                         *((volatile unsigned long *) (0x44820000+0x5*4))
#define posUART0_Reg0x5_TX_FIFO_NEED_WRITE                      0
#define bitUART0_Reg0x5_TX_FIFO_NEED_WRITE                      0x1
#define get_UART0_Reg0x5_TX_FIFO_NEED_WRITE                     (addUART0_Reg0x5 & 0x1)

#define posUART0_Reg0x5_RX_FIFO_NEED_READ                       1
#define bitUART0_Reg0x5_RX_FIFO_NEED_READ                       0x2
#define get_UART0_Reg0x5_RX_FIFO_NEED_READ                      ((addUART0_Reg0x5 & 0x2) >> 1)

#define posUART0_Reg0x5_RX_FIFO_OVER_FLOW                       2
#define bitUART0_Reg0x5_RX_FIFO_OVER_FLOW                       0x4
#define set_UART0_Reg0x5_RX_FIFO_OVER_FLOW(val)                 addUART0_Reg0x5 = ((addUART0_Reg0x5 & (~0x4)) | ((val) << 2))
#define setf_UART0_Reg0x5_RX_FIFO_OVER_FLOW                     addUART0_Reg0x5 |= 0x4
#define clrf_UART0_Reg0x5_RX_FIFO_OVER_FLOW                     addUART0_Reg0x5 &= ~0x4
#define get_UART0_Reg0x5_RX_FIFO_OVER_FLOW                      ((addUART0_Reg0x5 & 0x4) >> 2)

#define posUART0_Reg0x5_UART_RX_PARITY_ERROR                    3
#define bitUART0_Reg0x5_UART_RX_PARITY_ERROR                    0x8
#define set_UART0_Reg0x5_UART_RX_PARITY_ERROR(val)              addUART0_Reg0x5 = ((addUART0_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_UART0_Reg0x5_UART_RX_PARITY_ERROR                  addUART0_Reg0x5 |= 0x8
#define clrf_UART0_Reg0x5_UART_RX_PARITY_ERROR                  addUART0_Reg0x5 &= ~0x8
#define get_UART0_Reg0x5_UART_RX_PARITY_ERROR                   ((addUART0_Reg0x5 & 0x8) >> 3)

#define posUART0_Reg0x5_UART_RX_STOP_ERROR                      4
#define bitUART0_Reg0x5_UART_RX_STOP_ERROR                      0x10
#define set_UART0_Reg0x5_UART_RX_STOP_ERROR(val)                addUART0_Reg0x5 = ((addUART0_Reg0x5 & (~0x10)) | ((val) << 4))
#define setf_UART0_Reg0x5_UART_RX_STOP_ERROR                    addUART0_Reg0x5 |= 0x10
#define clrf_UART0_Reg0x5_UART_RX_STOP_ERROR                    addUART0_Reg0x5 &= ~0x10
#define get_UART0_Reg0x5_UART_RX_STOP_ERROR                     ((addUART0_Reg0x5 & 0x10) >> 4)

#define posUART0_Reg0x5_UART_TX_STOP_END                        5
#define bitUART0_Reg0x5_UART_TX_STOP_END                        0x20
#define set_UART0_Reg0x5_UART_TX_STOP_END(val)                  addUART0_Reg0x5 = ((addUART0_Reg0x5 & (~0x20)) | ((val) << 5))
#define setf_UART0_Reg0x5_UART_TX_STOP_END                      addUART0_Reg0x5 |= 0x20
#define clrf_UART0_Reg0x5_UART_TX_STOP_END                      addUART0_Reg0x5 &= ~0x20
#define get_UART0_Reg0x5_UART_TX_STOP_END                       ((addUART0_Reg0x5 & 0x20) >> 5)

#define posUART0_Reg0x5_UART_RX_STOP_END                        6
#define bitUART0_Reg0x5_UART_RX_STOP_END                        0x40
#define set_UART0_Reg0x5_UART_RX_STOP_END(val)                  addUART0_Reg0x5 = ((addUART0_Reg0x5 & (~0x40)) | ((val) << 6))
#define setf_UART0_Reg0x5_UART_RX_STOP_END                      addUART0_Reg0x5 |= 0x40
#define clrf_UART0_Reg0x5_UART_RX_STOP_END                      addUART0_Reg0x5 &= ~0x40
#define get_UART0_Reg0x5_UART_RX_STOP_END                       ((addUART0_Reg0x5 & 0x40) >> 6)

#define posUART0_Reg0x5_UART_RXD_WAKEUP                         7
#define bitUART0_Reg0x5_UART_RXD_WAKEUP                         0x80
#define set_UART0_Reg0x5_UART_RXD_WAKEUP(val)                   addUART0_Reg0x5 = ((addUART0_Reg0x5 & (~0x80)) | ((val) << 7))
#define setf_UART0_Reg0x5_UART_RXD_WAKEUP                       addUART0_Reg0x5 |= 0x80
#define clrf_UART0_Reg0x5_UART_RXD_WAKEUP                       addUART0_Reg0x5 &= ~0x80
#define get_UART0_Reg0x5_UART_RXD_WAKEUP                        ((addUART0_Reg0x5 & 0x80) >> 7)

//addUART0_Reg0x6
#define addUART0_Reg0x6                                         *((volatile unsigned long *) (0x44820000+0x6*4))
#define posUART0_Reg0x6_FLOW_CTL_LOW_CNT                        0
#define bitUART0_Reg0x6_FLOW_CTL_LOW_CNT                        0xFF
#define set_UART0_Reg0x6_FLOW_CTL_LOW_CNT(val)                  addUART0_Reg0x6 = ((addUART0_Reg0x6 & (~0xFF)) | ((val) << 0))
#define get_UART0_Reg0x6_FLOW_CTL_LOW_CNT                       (addUART0_Reg0x6 & 0xFF)

#define posUART0_Reg0x6_FLOW_CTL_HIGH_CNT                       8
#define bitUART0_Reg0x6_FLOW_CTL_HIGH_CNT                       0xFF00
#define set_UART0_Reg0x6_FLOW_CTL_HIGH_CNT(val)                 addUART0_Reg0x6 = ((addUART0_Reg0x6 & (~0xFF00)) | ((val) << 8))
#define get_UART0_Reg0x6_FLOW_CTL_HIGH_CNT                      ((addUART0_Reg0x6 & 0xFF00) >> 8)

#define posUART0_Reg0x6_FLOW_CONTROL_ENA                        16
#define bitUART0_Reg0x6_FLOW_CONTROL_ENA                        0x10000
#define set_UART0_Reg0x6_FLOW_CONTROL_ENA(val)                  addUART0_Reg0x6 = ((addUART0_Reg0x6 & (~0x10000)) | ((val) << 16))
#define setf_UART0_Reg0x6_FLOW_CONTROL_ENA                      addUART0_Reg0x6 |= 0x10000
#define clrf_UART0_Reg0x6_FLOW_CONTROL_ENA                      addUART0_Reg0x6 &= ~0x10000
#define get_UART0_Reg0x6_FLOW_CONTROL_ENA                       ((addUART0_Reg0x6 & 0x10000) >> 16)

#define posUART0_Reg0x6_RTS_POLARITY_SEL                        17
#define bitUART0_Reg0x6_RTS_POLARITY_SEL                        0x20000
#define set_UART0_Reg0x6_RTS_POLARITY_SEL(val)                  addUART0_Reg0x6 = ((addUART0_Reg0x6 & (~0x20000)) | ((val) << 17))
#define setf_UART0_Reg0x6_RTS_POLARITY_SEL                      addUART0_Reg0x6 |= 0x20000
#define clrf_UART0_Reg0x6_RTS_POLARITY_SEL                      addUART0_Reg0x6 &= ~0x20000
#define get_UART0_Reg0x6_RTS_POLARITY_SEL                       ((addUART0_Reg0x6 & 0x20000) >> 17)

#define posUART0_Reg0x6_CTS_POLARITY_SEL                        18
#define bitUART0_Reg0x6_CTS_POLARITY_SEL                        0x40000
#define set_UART0_Reg0x6_CTS_POLARITY_SEL(val)                  addUART0_Reg0x6 = ((addUART0_Reg0x6 & (~0x40000)) | ((val) << 18))
#define setf_UART0_Reg0x6_CTS_POLARITY_SEL                      addUART0_Reg0x6 |= 0x40000
#define clrf_UART0_Reg0x6_CTS_POLARITY_SEL                      addUART0_Reg0x6 &= ~0x40000
#define get_UART0_Reg0x6_CTS_POLARITY_SEL                       ((addUART0_Reg0x6 & 0x40000) >> 18)

//addUART0_Reg0x7
#define addUART0_Reg0x7                                         *((volatile unsigned long *) (0x44820000+0x7*4))
#define posUART0_Reg0x7_UART_WAKE_COUNT                         0
#define bitUART0_Reg0x7_UART_WAKE_COUNT                         0x3FF
#define set_UART0_Reg0x7_UART_WAKE_COUNT(val)                   addUART0_Reg0x7 = ((addUART0_Reg0x7 & (~0x3FF)) | ((val) << 0))
#define get_UART0_Reg0x7_UART_WAKE_COUNT                        (addUART0_Reg0x7 & 0x3FF)

#define posUART0_Reg0x7_UART_TXD_WAIT_CNT                       10
#define bitUART0_Reg0x7_UART_TXD_WAIT_CNT                       0xFFC00
#define set_UART0_Reg0x7_UART_TXD_WAIT_CNT(val)                 addUART0_Reg0x7 = ((addUART0_Reg0x7 & (~0xFFC00)) | ((val) << 10))
#define get_UART0_Reg0x7_UART_TXD_WAIT_CNT                      ((addUART0_Reg0x7 & 0xFFC00) >> 10)

#define posUART0_Reg0x7_UART_RXD_WAKE_EN                        20
#define bitUART0_Reg0x7_UART_RXD_WAKE_EN                        0x100000
#define set_UART0_Reg0x7_UART_RXD_WAKE_EN(val)                  addUART0_Reg0x7 = ((addUART0_Reg0x7 & (~0x100000)) | ((val) << 20))
#define setf_UART0_Reg0x7_UART_RXD_WAKE_EN                      addUART0_Reg0x7 |= 0x100000
#define clrf_UART0_Reg0x7_UART_RXD_WAKE_EN                      addUART0_Reg0x7 &= ~0x100000
#define get_UART0_Reg0x7_UART_RXD_WAKE_EN                       ((addUART0_Reg0x7 & 0x100000) >> 20)

#define posUART0_Reg0x7_UART_TXD_WAKE_EN                        21
#define bitUART0_Reg0x7_UART_TXD_WAKE_EN                        0x200000
#define set_UART0_Reg0x7_UART_TXD_WAKE_EN(val)                  addUART0_Reg0x7 = ((addUART0_Reg0x7 & (~0x200000)) | ((val) << 21))
#define setf_UART0_Reg0x7_UART_TXD_WAKE_EN                      addUART0_Reg0x7 |= 0x200000
#define clrf_UART0_Reg0x7_UART_TXD_WAKE_EN                      addUART0_Reg0x7 &= ~0x200000
#define get_UART0_Reg0x7_UART_TXD_WAKE_EN                       ((addUART0_Reg0x7 & 0x200000) >> 21)

#define posUART0_Reg0x7_RXD_NEGEDGE_WAKE_EN                     22
#define bitUART0_Reg0x7_RXD_NEGEDGE_WAKE_EN                     0x400000
#define set_UART0_Reg0x7_RXD_NEGEDGE_WAKE_EN(val)               addUART0_Reg0x7 = ((addUART0_Reg0x7 & (~0x400000)) | ((val) << 22))
#define setf_UART0_Reg0x7_RXD_NEGEDGE_WAKE_EN                   addUART0_Reg0x7 |= 0x400000
#define clrf_UART0_Reg0x7_RXD_NEGEDGE_WAKE_EN                   addUART0_Reg0x7 &= ~0x400000
#define get_UART0_Reg0x7_RXD_NEGEDGE_WAKE_EN                    ((addUART0_Reg0x7 & 0x400000) >> 22)


//************************************************************//
//UART1
//************************************************************//
#define BASEADDR_UART1                                          0x45830000
//addUART1_Reg0x0
#define addUART1_Reg0x0                                         *((volatile unsigned long *) (0x45830000+0x0*4))
#define posUART1_Reg0x0_UART_TX_ENABLE                          0
#define bitUART1_Reg0x0_UART_TX_ENABLE                          0x1
#define set_UART1_Reg0x0_UART_TX_ENABLE(val)                    addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_UART1_Reg0x0_UART_TX_ENABLE                        addUART1_Reg0x0 |= 0x1
#define clrf_UART1_Reg0x0_UART_TX_ENABLE                        addUART1_Reg0x0 &= ~0x1
#define get_UART1_Reg0x0_UART_TX_ENABLE                         (addUART1_Reg0x0 & 0x1)

#define posUART1_Reg0x0_UART_RX_ENABLE                          1
#define bitUART1_Reg0x0_UART_RX_ENABLE                          0x2
#define set_UART1_Reg0x0_UART_RX_ENABLE(val)                    addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_UART1_Reg0x0_UART_RX_ENABLE                        addUART1_Reg0x0 |= 0x2
#define clrf_UART1_Reg0x0_UART_RX_ENABLE                        addUART1_Reg0x0 &= ~0x2
#define get_UART1_Reg0x0_UART_RX_ENABLE                         ((addUART1_Reg0x0 & 0x2) >> 1)

#define posUART1_Reg0x0_UART_IRDA                               2
#define bitUART1_Reg0x0_UART_IRDA                               0x4
#define set_UART1_Reg0x0_UART_IRDA(val)                         addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_UART1_Reg0x0_UART_IRDA                             addUART1_Reg0x0 |= 0x4
#define clrf_UART1_Reg0x0_UART_IRDA                             addUART1_Reg0x0 &= ~0x4
#define get_UART1_Reg0x0_UART_IRDA                              ((addUART1_Reg0x0 & 0x4) >> 2)

#define posUART1_Reg0x0_UART_LEN                                3
#define bitUART1_Reg0x0_UART_LEN                                0x18
#define set_UART1_Reg0x0_UART_LEN(val)                          addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x18)) | ((val) << 3))
#define get_UART1_Reg0x0_UART_LEN                               ((addUART1_Reg0x0 & 0x18) >> 3)

#define posUART1_Reg0x0_UART_PAR_EN                             5
#define bitUART1_Reg0x0_UART_PAR_EN                             0x20
#define set_UART1_Reg0x0_UART_PAR_EN(val)                       addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_UART1_Reg0x0_UART_PAR_EN                           addUART1_Reg0x0 |= 0x20
#define clrf_UART1_Reg0x0_UART_PAR_EN                           addUART1_Reg0x0 &= ~0x20
#define get_UART1_Reg0x0_UART_PAR_EN                            ((addUART1_Reg0x0 & 0x20) >> 5)

#define posUART1_Reg0x0_UART_PAR_MODE                           6
#define bitUART1_Reg0x0_UART_PAR_MODE                           0x40
#define set_UART1_Reg0x0_UART_PAR_MODE(val)                     addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_UART1_Reg0x0_UART_PAR_MODE                         addUART1_Reg0x0 |= 0x40
#define clrf_UART1_Reg0x0_UART_PAR_MODE                         addUART1_Reg0x0 &= ~0x40
#define get_UART1_Reg0x0_UART_PAR_MODE                          ((addUART1_Reg0x0 & 0x40) >> 6)

#define posUART1_Reg0x0_UART_STOP_LEN                           7
#define bitUART1_Reg0x0_UART_STOP_LEN                           0x80
#define set_UART1_Reg0x0_UART_STOP_LEN(val)                     addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_UART1_Reg0x0_UART_STOP_LEN                         addUART1_Reg0x0 |= 0x80
#define clrf_UART1_Reg0x0_UART_STOP_LEN                         addUART1_Reg0x0 &= ~0x80
#define get_UART1_Reg0x0_UART_STOP_LEN                          ((addUART1_Reg0x0 & 0x80) >> 7)

#define posUART1_Reg0x0_UART_CLK_DIVID                          8
#define bitUART1_Reg0x0_UART_CLK_DIVID                          0x1FFF00
#define set_UART1_Reg0x0_UART_CLK_DIVID(val)                    addUART1_Reg0x0 = ((addUART1_Reg0x0 & (~0x1FFF00)) | ((val) << 8))
#define get_UART1_Reg0x0_UART_CLK_DIVID                         ((addUART1_Reg0x0 & 0x1FFF00) >> 8)

//addUART1_Reg0x1
#define addUART1_Reg0x1                                         *((volatile unsigned long *) (0x45830000+0x1*4))
#define posUART1_Reg0x1_TX_FIFO_THRESHOLD                       0
#define bitUART1_Reg0x1_TX_FIFO_THRESHOLD                       0xFF
#define set_UART1_Reg0x1_TX_FIFO_THRESHOLD(val)                 addUART1_Reg0x1 = ((addUART1_Reg0x1 & (~0xFF)) | ((val) << 0))
#define get_UART1_Reg0x1_TX_FIFO_THRESHOLD                      (addUART1_Reg0x1 & 0xFF)

#define posUART1_Reg0x1_RX_FIFO_THRESHOLD                       8
#define bitUART1_Reg0x1_RX_FIFO_THRESHOLD                       0xFF00
#define set_UART1_Reg0x1_RX_FIFO_THRESHOLD(val)                 addUART1_Reg0x1 = ((addUART1_Reg0x1 & (~0xFF00)) | ((val) << 8))
#define get_UART1_Reg0x1_RX_FIFO_THRESHOLD                      ((addUART1_Reg0x1 & 0xFF00) >> 8)

#define posUART1_Reg0x1_RX_STOP_DETECT_TIME                     16
#define bitUART1_Reg0x1_RX_STOP_DETECT_TIME                     0x30000
#define set_UART1_Reg0x1_RX_STOP_DETECT_TIME(val)               addUART1_Reg0x1 = ((addUART1_Reg0x1 & (~0x30000)) | ((val) << 16))
#define get_UART1_Reg0x1_RX_STOP_DETECT_TIME                    ((addUART1_Reg0x1 & 0x30000) >> 16)

//addUART1_Reg0x2
#define addUART1_Reg0x2                                         *((volatile unsigned long *) (0x45830000+0x2*4))
#define posUART1_Reg0x2_TX_FIFO_COUNT                           0
#define bitUART1_Reg0x2_TX_FIFO_COUNT                           0xFF
#define get_UART1_Reg0x2_TX_FIFO_COUNT                          (addUART1_Reg0x2 & 0xFF)

#define posUART1_Reg0x2_RX_FIFO_COUNT                           8
#define bitUART1_Reg0x2_RX_FIFO_COUNT                           0xFF00
#define get_UART1_Reg0x2_RX_FIFO_COUNT                          ((addUART1_Reg0x2 & 0xFF00) >> 8)

#define posUART1_Reg0x2_TX_FIFO_FULL                            16
#define bitUART1_Reg0x2_TX_FIFO_FULL                            0x10000
#define get_UART1_Reg0x2_TX_FIFO_FULL                           ((addUART1_Reg0x2 & 0x10000) >> 16)

#define posUART1_Reg0x2_TX_FIFO_EMPTY                           17
#define bitUART1_Reg0x2_TX_FIFO_EMPTY                           0x20000
#define get_UART1_Reg0x2_TX_FIFO_EMPTY                          ((addUART1_Reg0x2 & 0x20000) >> 17)

#define posUART1_Reg0x2_RX_FIFO_FULL                            18
#define bitUART1_Reg0x2_RX_FIFO_FULL                            0x40000
#define get_UART1_Reg0x2_RX_FIFO_FULL                           ((addUART1_Reg0x2 & 0x40000) >> 18)

#define posUART1_Reg0x2_RX_FIFO_EMPTY                           19
#define bitUART1_Reg0x2_RX_FIFO_EMPTY                           0x80000
#define get_UART1_Reg0x2_RX_FIFO_EMPTY                          ((addUART1_Reg0x2 & 0x80000) >> 19)

#define posUART1_Reg0x2_FIFO_WR_READY                           20
#define bitUART1_Reg0x2_FIFO_WR_READY                           0x100000
#define get_UART1_Reg0x2_FIFO_WR_READY                          ((addUART1_Reg0x2 & 0x100000) >> 20)

#define posUART1_Reg0x2_FIFO_RD_READY                           21
#define bitUART1_Reg0x2_FIFO_RD_READY                           0x200000
#define get_UART1_Reg0x2_FIFO_RD_READY                          ((addUART1_Reg0x2 & 0x200000) >> 21)

//addUART1_Reg0x3
#define addUART1_Reg0x3                                         *((volatile unsigned long *) (0x45830000+0x3*4))
#define posUART1_Reg0x3_UART_TX_FIFO_DIN                        0
#define bitUART1_Reg0x3_UART_TX_FIFO_DIN                        0xFF
#define set_UART1_Reg0x3_UART_TX_FIFO_DIN(val)                  addUART1_Reg0x3 = ((addUART1_Reg0x3 & (~0xFF)) | ((val) << 0))
#define get_UART1_Reg0x3_UART_TX_FIFO_DIN                       (addUART1_Reg0x3 & 0xFF)

#define posUART1_Reg0x3_UART_RX_FIFO_DOUT                       8
#define bitUART1_Reg0x3_UART_RX_FIFO_DOUT                       0xFF00
#define get_UART1_Reg0x3_UART_RX_FIFO_DOUT                      ((addUART1_Reg0x3 & 0xFF00) >> 8)

//addUART1_Reg0x4
#define addUART1_Reg0x4                                         *((volatile unsigned long *) (0x45830000+0x4*4))
#define posUART1_Reg0x4_TX_FIFO_NEED_WRITE_MASK                 0
#define bitUART1_Reg0x4_TX_FIFO_NEED_WRITE_MASK                 0x1
#define set_UART1_Reg0x4_TX_FIFO_NEED_WRITE_MASK(val)           addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_UART1_Reg0x4_TX_FIFO_NEED_WRITE_MASK               addUART1_Reg0x4 |= 0x1
#define clrf_UART1_Reg0x4_TX_FIFO_NEED_WRITE_MASK               addUART1_Reg0x4 &= ~0x1
#define get_UART1_Reg0x4_TX_FIFO_NEED_WRITE_MASK                (addUART1_Reg0x4 & 0x1)

#define posUART1_Reg0x4_RX_FIFO_NEED_READ_MASK                  1
#define bitUART1_Reg0x4_RX_FIFO_NEED_READ_MASK                  0x2
#define set_UART1_Reg0x4_RX_FIFO_NEED_READ_MASK(val)            addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x2)) | ((val) << 1))
#define setf_UART1_Reg0x4_RX_FIFO_NEED_READ_MASK                addUART1_Reg0x4 |= 0x2
#define clrf_UART1_Reg0x4_RX_FIFO_NEED_READ_MASK                addUART1_Reg0x4 &= ~0x2
#define get_UART1_Reg0x4_RX_FIFO_NEED_READ_MASK                 ((addUART1_Reg0x4 & 0x2) >> 1)

#define posUART1_Reg0x4_RX_FIFO_OVER_FLOW_MASK                  2
#define bitUART1_Reg0x4_RX_FIFO_OVER_FLOW_MASK                  0x4
#define set_UART1_Reg0x4_RX_FIFO_OVER_FLOW_MASK(val)            addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x4)) | ((val) << 2))
#define setf_UART1_Reg0x4_RX_FIFO_OVER_FLOW_MASK                addUART1_Reg0x4 |= 0x4
#define clrf_UART1_Reg0x4_RX_FIFO_OVER_FLOW_MASK                addUART1_Reg0x4 &= ~0x4
#define get_UART1_Reg0x4_RX_FIFO_OVER_FLOW_MASK                 ((addUART1_Reg0x4 & 0x4) >> 2)

#define posUART1_Reg0x4_UART_RX_PARITY_ERR_MASK                 3
#define bitUART1_Reg0x4_UART_RX_PARITY_ERR_MASK                 0x8
#define set_UART1_Reg0x4_UART_RX_PARITY_ERR_MASK(val)           addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x8)) | ((val) << 3))
#define setf_UART1_Reg0x4_UART_RX_PARITY_ERR_MASK               addUART1_Reg0x4 |= 0x8
#define clrf_UART1_Reg0x4_UART_RX_PARITY_ERR_MASK               addUART1_Reg0x4 &= ~0x8
#define get_UART1_Reg0x4_UART_RX_PARITY_ERR_MASK                ((addUART1_Reg0x4 & 0x8) >> 3)

#define posUART1_Reg0x4_UART_RX_STOP_ERR_MASK                   4
#define bitUART1_Reg0x4_UART_RX_STOP_ERR_MASK                   0x10
#define set_UART1_Reg0x4_UART_RX_STOP_ERR_MASK(val)             addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x10)) | ((val) << 4))
#define setf_UART1_Reg0x4_UART_RX_STOP_ERR_MASK                 addUART1_Reg0x4 |= 0x10
#define clrf_UART1_Reg0x4_UART_RX_STOP_ERR_MASK                 addUART1_Reg0x4 &= ~0x10
#define get_UART1_Reg0x4_UART_RX_STOP_ERR_MASK                  ((addUART1_Reg0x4 & 0x10) >> 4)

#define posUART1_Reg0x4_UART_TX_STOP_END_MASK                   5
#define bitUART1_Reg0x4_UART_TX_STOP_END_MASK                   0x20
#define set_UART1_Reg0x4_UART_TX_STOP_END_MASK(val)             addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x20)) | ((val) << 5))
#define setf_UART1_Reg0x4_UART_TX_STOP_END_MASK                 addUART1_Reg0x4 |= 0x20
#define clrf_UART1_Reg0x4_UART_TX_STOP_END_MASK                 addUART1_Reg0x4 &= ~0x20
#define get_UART1_Reg0x4_UART_TX_STOP_END_MASK                  ((addUART1_Reg0x4 & 0x20) >> 5)

#define posUART1_Reg0x4_UART_RX_STOP_END_MASK                   6
#define bitUART1_Reg0x4_UART_RX_STOP_END_MASK                   0x40
#define set_UART1_Reg0x4_UART_RX_STOP_END_MASK(val)             addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x40)) | ((val) << 6))
#define setf_UART1_Reg0x4_UART_RX_STOP_END_MASK                 addUART1_Reg0x4 |= 0x40
#define clrf_UART1_Reg0x4_UART_RX_STOP_END_MASK                 addUART1_Reg0x4 &= ~0x40
#define get_UART1_Reg0x4_UART_RX_STOP_END_MASK                  ((addUART1_Reg0x4 & 0x40) >> 6)

#define posUART1_Reg0x4_UART_RXD_WAKEUP_MASK                    7
#define bitUART1_Reg0x4_UART_RXD_WAKEUP_MASK                    0x80
#define set_UART1_Reg0x4_UART_RXD_WAKEUP_MASK(val)              addUART1_Reg0x4 = ((addUART1_Reg0x4 & (~0x80)) | ((val) << 7))
#define setf_UART1_Reg0x4_UART_RXD_WAKEUP_MASK                  addUART1_Reg0x4 |= 0x80
#define clrf_UART1_Reg0x4_UART_RXD_WAKEUP_MASK                  addUART1_Reg0x4 &= ~0x80
#define get_UART1_Reg0x4_UART_RXD_WAKEUP_MASK                   ((addUART1_Reg0x4 & 0x80) >> 7)

//addUART1_Reg0x5
#define addUART1_Reg0x5                                         *((volatile unsigned long *) (0x45830000+0x5*4))
#define posUART1_Reg0x5_TX_FIFO_NEED_WRITE                      0
#define bitUART1_Reg0x5_TX_FIFO_NEED_WRITE                      0x1
#define get_UART1_Reg0x5_TX_FIFO_NEED_WRITE                     (addUART1_Reg0x5 & 0x1)

#define posUART1_Reg0x5_RX_FIFO_NEED_READ                       1
#define bitUART1_Reg0x5_RX_FIFO_NEED_READ                       0x2
#define get_UART1_Reg0x5_RX_FIFO_NEED_READ                      ((addUART1_Reg0x5 & 0x2) >> 1)

#define posUART1_Reg0x5_RX_FIFO_OVER_FLOW                       2
#define bitUART1_Reg0x5_RX_FIFO_OVER_FLOW                       0x4
#define set_UART1_Reg0x5_RX_FIFO_OVER_FLOW(val)                 addUART1_Reg0x5 = ((addUART1_Reg0x5 & (~0x4)) | ((val) << 2))
#define setf_UART1_Reg0x5_RX_FIFO_OVER_FLOW                     addUART1_Reg0x5 |= 0x4
#define clrf_UART1_Reg0x5_RX_FIFO_OVER_FLOW                     addUART1_Reg0x5 &= ~0x4
#define get_UART1_Reg0x5_RX_FIFO_OVER_FLOW                      ((addUART1_Reg0x5 & 0x4) >> 2)

#define posUART1_Reg0x5_UART_RX_PARITY_ERROR                    3
#define bitUART1_Reg0x5_UART_RX_PARITY_ERROR                    0x8
#define set_UART1_Reg0x5_UART_RX_PARITY_ERROR(val)              addUART1_Reg0x5 = ((addUART1_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_UART1_Reg0x5_UART_RX_PARITY_ERROR                  addUART1_Reg0x5 |= 0x8
#define clrf_UART1_Reg0x5_UART_RX_PARITY_ERROR                  addUART1_Reg0x5 &= ~0x8
#define get_UART1_Reg0x5_UART_RX_PARITY_ERROR                   ((addUART1_Reg0x5 & 0x8) >> 3)

#define posUART1_Reg0x5_UART_RX_STOP_ERROR                      4
#define bitUART1_Reg0x5_UART_RX_STOP_ERROR                      0x10
#define set_UART1_Reg0x5_UART_RX_STOP_ERROR(val)                addUART1_Reg0x5 = ((addUART1_Reg0x5 & (~0x10)) | ((val) << 4))
#define setf_UART1_Reg0x5_UART_RX_STOP_ERROR                    addUART1_Reg0x5 |= 0x10
#define clrf_UART1_Reg0x5_UART_RX_STOP_ERROR                    addUART1_Reg0x5 &= ~0x10
#define get_UART1_Reg0x5_UART_RX_STOP_ERROR                     ((addUART1_Reg0x5 & 0x10) >> 4)

#define posUART1_Reg0x5_UART_TX_STOP_END                        5
#define bitUART1_Reg0x5_UART_TX_STOP_END                        0x20
#define set_UART1_Reg0x5_UART_TX_STOP_END(val)                  addUART1_Reg0x5 = ((addUART1_Reg0x5 & (~0x20)) | ((val) << 5))
#define setf_UART1_Reg0x5_UART_TX_STOP_END                      addUART1_Reg0x5 |= 0x20
#define clrf_UART1_Reg0x5_UART_TX_STOP_END                      addUART1_Reg0x5 &= ~0x20
#define get_UART1_Reg0x5_UART_TX_STOP_END                       ((addUART1_Reg0x5 & 0x20) >> 5)

#define posUART1_Reg0x5_UART_RX_STOP_END                        6
#define bitUART1_Reg0x5_UART_RX_STOP_END                        0x40
#define set_UART1_Reg0x5_UART_RX_STOP_END(val)                  addUART1_Reg0x5 = ((addUART1_Reg0x5 & (~0x40)) | ((val) << 6))
#define setf_UART1_Reg0x5_UART_RX_STOP_END                      addUART1_Reg0x5 |= 0x40
#define clrf_UART1_Reg0x5_UART_RX_STOP_END                      addUART1_Reg0x5 &= ~0x40
#define get_UART1_Reg0x5_UART_RX_STOP_END                       ((addUART1_Reg0x5 & 0x40) >> 6)

#define posUART1_Reg0x5_UART_RXD_WAKEUP                         7
#define bitUART1_Reg0x5_UART_RXD_WAKEUP                         0x80
#define set_UART1_Reg0x5_UART_RXD_WAKEUP(val)                   addUART1_Reg0x5 = ((addUART1_Reg0x5 & (~0x80)) | ((val) << 7))
#define setf_UART1_Reg0x5_UART_RXD_WAKEUP                       addUART1_Reg0x5 |= 0x80
#define clrf_UART1_Reg0x5_UART_RXD_WAKEUP                       addUART1_Reg0x5 &= ~0x80
#define get_UART1_Reg0x5_UART_RXD_WAKEUP                        ((addUART1_Reg0x5 & 0x80) >> 7)

//addUART1_Reg0x6
#define addUART1_Reg0x6                                         *((volatile unsigned long *) (0x45830000+0x6*4))
#define posUART1_Reg0x6_FLOW_CTL_LOW_CNT                        0
#define bitUART1_Reg0x6_FLOW_CTL_LOW_CNT                        0xFF
#define set_UART1_Reg0x6_FLOW_CTL_LOW_CNT(val)                  addUART1_Reg0x6 = ((addUART1_Reg0x6 & (~0xFF)) | ((val) << 0))
#define get_UART1_Reg0x6_FLOW_CTL_LOW_CNT                       (addUART1_Reg0x6 & 0xFF)

#define posUART1_Reg0x6_FLOW_CTL_HIGH_CNT                       8
#define bitUART1_Reg0x6_FLOW_CTL_HIGH_CNT                       0xFF00
#define set_UART1_Reg0x6_FLOW_CTL_HIGH_CNT(val)                 addUART1_Reg0x6 = ((addUART1_Reg0x6 & (~0xFF00)) | ((val) << 8))
#define get_UART1_Reg0x6_FLOW_CTL_HIGH_CNT                      ((addUART1_Reg0x6 & 0xFF00) >> 8)

#define posUART1_Reg0x6_FLOW_CONTROL_ENA                        16
#define bitUART1_Reg0x6_FLOW_CONTROL_ENA                        0x10000
#define set_UART1_Reg0x6_FLOW_CONTROL_ENA(val)                  addUART1_Reg0x6 = ((addUART1_Reg0x6 & (~0x10000)) | ((val) << 16))
#define setf_UART1_Reg0x6_FLOW_CONTROL_ENA                      addUART1_Reg0x6 |= 0x10000
#define clrf_UART1_Reg0x6_FLOW_CONTROL_ENA                      addUART1_Reg0x6 &= ~0x10000
#define get_UART1_Reg0x6_FLOW_CONTROL_ENA                       ((addUART1_Reg0x6 & 0x10000) >> 16)

#define posUART1_Reg0x6_RTS_POLARITY_SEL                        17
#define bitUART1_Reg0x6_RTS_POLARITY_SEL                        0x20000
#define set_UART1_Reg0x6_RTS_POLARITY_SEL(val)                  addUART1_Reg0x6 = ((addUART1_Reg0x6 & (~0x20000)) | ((val) << 17))
#define setf_UART1_Reg0x6_RTS_POLARITY_SEL                      addUART1_Reg0x6 |= 0x20000
#define clrf_UART1_Reg0x6_RTS_POLARITY_SEL                      addUART1_Reg0x6 &= ~0x20000
#define get_UART1_Reg0x6_RTS_POLARITY_SEL                       ((addUART1_Reg0x6 & 0x20000) >> 17)

#define posUART1_Reg0x6_CTS_POLARITY_SEL                        18
#define bitUART1_Reg0x6_CTS_POLARITY_SEL                        0x40000
#define set_UART1_Reg0x6_CTS_POLARITY_SEL(val)                  addUART1_Reg0x6 = ((addUART1_Reg0x6 & (~0x40000)) | ((val) << 18))
#define setf_UART1_Reg0x6_CTS_POLARITY_SEL                      addUART1_Reg0x6 |= 0x40000
#define clrf_UART1_Reg0x6_CTS_POLARITY_SEL                      addUART1_Reg0x6 &= ~0x40000
#define get_UART1_Reg0x6_CTS_POLARITY_SEL                       ((addUART1_Reg0x6 & 0x40000) >> 18)

//addUART1_Reg0x7
#define addUART1_Reg0x7                                         *((volatile unsigned long *) (0x45830000+0x7*4))
#define posUART1_Reg0x7_UART_WAKE_COUNT                         0
#define bitUART1_Reg0x7_UART_WAKE_COUNT                         0x3FF
#define set_UART1_Reg0x7_UART_WAKE_COUNT(val)                   addUART1_Reg0x7 = ((addUART1_Reg0x7 & (~0x3FF)) | ((val) << 0))
#define get_UART1_Reg0x7_UART_WAKE_COUNT                        (addUART1_Reg0x7 & 0x3FF)

#define posUART1_Reg0x7_UART_TXD_WAIT_CNT                       10
#define bitUART1_Reg0x7_UART_TXD_WAIT_CNT                       0xFFC00
#define set_UART1_Reg0x7_UART_TXD_WAIT_CNT(val)                 addUART1_Reg0x7 = ((addUART1_Reg0x7 & (~0xFFC00)) | ((val) << 10))
#define get_UART1_Reg0x7_UART_TXD_WAIT_CNT                      ((addUART1_Reg0x7 & 0xFFC00) >> 10)

#define posUART1_Reg0x7_UART_RXD_WAKE_EN                        20
#define bitUART1_Reg0x7_UART_RXD_WAKE_EN                        0x100000
#define set_UART1_Reg0x7_UART_RXD_WAKE_EN(val)                  addUART1_Reg0x7 = ((addUART1_Reg0x7 & (~0x100000)) | ((val) << 20))
#define setf_UART1_Reg0x7_UART_RXD_WAKE_EN                      addUART1_Reg0x7 |= 0x100000
#define clrf_UART1_Reg0x7_UART_RXD_WAKE_EN                      addUART1_Reg0x7 &= ~0x100000
#define get_UART1_Reg0x7_UART_RXD_WAKE_EN                       ((addUART1_Reg0x7 & 0x100000) >> 20)

#define posUART1_Reg0x7_UART_TXD_WAKE_EN                        21
#define bitUART1_Reg0x7_UART_TXD_WAKE_EN                        0x200000
#define set_UART1_Reg0x7_UART_TXD_WAKE_EN(val)                  addUART1_Reg0x7 = ((addUART1_Reg0x7 & (~0x200000)) | ((val) << 21))
#define setf_UART1_Reg0x7_UART_TXD_WAKE_EN                      addUART1_Reg0x7 |= 0x200000
#define clrf_UART1_Reg0x7_UART_TXD_WAKE_EN                      addUART1_Reg0x7 &= ~0x200000
#define get_UART1_Reg0x7_UART_TXD_WAKE_EN                       ((addUART1_Reg0x7 & 0x200000) >> 21)

#define posUART1_Reg0x7_RXD_NEGEDGE_WAKE_EN                     22
#define bitUART1_Reg0x7_RXD_NEGEDGE_WAKE_EN                     0x400000
#define set_UART1_Reg0x7_RXD_NEGEDGE_WAKE_EN(val)               addUART1_Reg0x7 = ((addUART1_Reg0x7 & (~0x400000)) | ((val) << 22))
#define setf_UART1_Reg0x7_RXD_NEGEDGE_WAKE_EN                   addUART1_Reg0x7 |= 0x400000
#define clrf_UART1_Reg0x7_RXD_NEGEDGE_WAKE_EN                   addUART1_Reg0x7 &= ~0x400000
#define get_UART1_Reg0x7_RXD_NEGEDGE_WAKE_EN                    ((addUART1_Reg0x7 & 0x400000) >> 22)


//************************************************************//
//UART2
//************************************************************//
#define BASEADDR_UART2                                          0x45840000
//addUART2_Reg0x0
#define addUART2_Reg0x0                                         *((volatile unsigned long *) (0x45840000+0x0*4))
#define posUART2_Reg0x0_UART_TX_ENABLE                          0
#define bitUART2_Reg0x0_UART_TX_ENABLE                          0x1
#define set_UART2_Reg0x0_UART_TX_ENABLE(val)                    addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_UART2_Reg0x0_UART_TX_ENABLE                        addUART2_Reg0x0 |= 0x1
#define clrf_UART2_Reg0x0_UART_TX_ENABLE                        addUART2_Reg0x0 &= ~0x1
#define get_UART2_Reg0x0_UART_TX_ENABLE                         (addUART2_Reg0x0 & 0x1)

#define posUART2_Reg0x0_UART_RX_ENABLE                          1
#define bitUART2_Reg0x0_UART_RX_ENABLE                          0x2
#define set_UART2_Reg0x0_UART_RX_ENABLE(val)                    addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_UART2_Reg0x0_UART_RX_ENABLE                        addUART2_Reg0x0 |= 0x2
#define clrf_UART2_Reg0x0_UART_RX_ENABLE                        addUART2_Reg0x0 &= ~0x2
#define get_UART2_Reg0x0_UART_RX_ENABLE                         ((addUART2_Reg0x0 & 0x2) >> 1)

#define posUART2_Reg0x0_UART_IRDA                               2
#define bitUART2_Reg0x0_UART_IRDA                               0x4
#define set_UART2_Reg0x0_UART_IRDA(val)                         addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_UART2_Reg0x0_UART_IRDA                             addUART2_Reg0x0 |= 0x4
#define clrf_UART2_Reg0x0_UART_IRDA                             addUART2_Reg0x0 &= ~0x4
#define get_UART2_Reg0x0_UART_IRDA                              ((addUART2_Reg0x0 & 0x4) >> 2)

#define posUART2_Reg0x0_UART_LEN                                3
#define bitUART2_Reg0x0_UART_LEN                                0x18
#define set_UART2_Reg0x0_UART_LEN(val)                          addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x18)) | ((val) << 3))
#define get_UART2_Reg0x0_UART_LEN                               ((addUART2_Reg0x0 & 0x18) >> 3)

#define posUART2_Reg0x0_UART_PAR_EN                             5
#define bitUART2_Reg0x0_UART_PAR_EN                             0x20
#define set_UART2_Reg0x0_UART_PAR_EN(val)                       addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_UART2_Reg0x0_UART_PAR_EN                           addUART2_Reg0x0 |= 0x20
#define clrf_UART2_Reg0x0_UART_PAR_EN                           addUART2_Reg0x0 &= ~0x20
#define get_UART2_Reg0x0_UART_PAR_EN                            ((addUART2_Reg0x0 & 0x20) >> 5)

#define posUART2_Reg0x0_UART_PAR_MODE                           6
#define bitUART2_Reg0x0_UART_PAR_MODE                           0x40
#define set_UART2_Reg0x0_UART_PAR_MODE(val)                     addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_UART2_Reg0x0_UART_PAR_MODE                         addUART2_Reg0x0 |= 0x40
#define clrf_UART2_Reg0x0_UART_PAR_MODE                         addUART2_Reg0x0 &= ~0x40
#define get_UART2_Reg0x0_UART_PAR_MODE                          ((addUART2_Reg0x0 & 0x40) >> 6)

#define posUART2_Reg0x0_UART_STOP_LEN                           7
#define bitUART2_Reg0x0_UART_STOP_LEN                           0x80
#define set_UART2_Reg0x0_UART_STOP_LEN(val)                     addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_UART2_Reg0x0_UART_STOP_LEN                         addUART2_Reg0x0 |= 0x80
#define clrf_UART2_Reg0x0_UART_STOP_LEN                         addUART2_Reg0x0 &= ~0x80
#define get_UART2_Reg0x0_UART_STOP_LEN                          ((addUART2_Reg0x0 & 0x80) >> 7)

#define posUART2_Reg0x0_UART_CLK_DIVID                          8
#define bitUART2_Reg0x0_UART_CLK_DIVID                          0x1FFF00
#define set_UART2_Reg0x0_UART_CLK_DIVID(val)                    addUART2_Reg0x0 = ((addUART2_Reg0x0 & (~0x1FFF00)) | ((val) << 8))
#define get_UART2_Reg0x0_UART_CLK_DIVID                         ((addUART2_Reg0x0 & 0x1FFF00) >> 8)

//addUART2_Reg0x1
#define addUART2_Reg0x1                                         *((volatile unsigned long *) (0x45840000+0x1*4))
#define posUART2_Reg0x1_TX_FIFO_THRESHOLD                       0
#define bitUART2_Reg0x1_TX_FIFO_THRESHOLD                       0xFF
#define set_UART2_Reg0x1_TX_FIFO_THRESHOLD(val)                 addUART2_Reg0x1 = ((addUART2_Reg0x1 & (~0xFF)) | ((val) << 0))
#define get_UART2_Reg0x1_TX_FIFO_THRESHOLD                      (addUART2_Reg0x1 & 0xFF)

#define posUART2_Reg0x1_RX_FIFO_THRESHOLD                       8
#define bitUART2_Reg0x1_RX_FIFO_THRESHOLD                       0xFF00
#define set_UART2_Reg0x1_RX_FIFO_THRESHOLD(val)                 addUART2_Reg0x1 = ((addUART2_Reg0x1 & (~0xFF00)) | ((val) << 8))
#define get_UART2_Reg0x1_RX_FIFO_THRESHOLD                      ((addUART2_Reg0x1 & 0xFF00) >> 8)

#define posUART2_Reg0x1_RX_STOP_DETECT_TIME                     16
#define bitUART2_Reg0x1_RX_STOP_DETECT_TIME                     0x30000
#define set_UART2_Reg0x1_RX_STOP_DETECT_TIME(val)               addUART2_Reg0x1 = ((addUART2_Reg0x1 & (~0x30000)) | ((val) << 16))
#define get_UART2_Reg0x1_RX_STOP_DETECT_TIME                    ((addUART2_Reg0x1 & 0x30000) >> 16)

//addUART2_Reg0x2
#define addUART2_Reg0x2                                         *((volatile unsigned long *) (0x45840000+0x2*4))
#define posUART2_Reg0x2_TX_FIFO_COUNT                           0
#define bitUART2_Reg0x2_TX_FIFO_COUNT                           0xFF
#define get_UART2_Reg0x2_TX_FIFO_COUNT                          (addUART2_Reg0x2 & 0xFF)

#define posUART2_Reg0x2_RX_FIFO_COUNT                           8
#define bitUART2_Reg0x2_RX_FIFO_COUNT                           0xFF00
#define get_UART2_Reg0x2_RX_FIFO_COUNT                          ((addUART2_Reg0x2 & 0xFF00) >> 8)

#define posUART2_Reg0x2_TX_FIFO_FULL                            16
#define bitUART2_Reg0x2_TX_FIFO_FULL                            0x10000
#define get_UART2_Reg0x2_TX_FIFO_FULL                           ((addUART2_Reg0x2 & 0x10000) >> 16)

#define posUART2_Reg0x2_TX_FIFO_EMPTY                           17
#define bitUART2_Reg0x2_TX_FIFO_EMPTY                           0x20000
#define get_UART2_Reg0x2_TX_FIFO_EMPTY                          ((addUART2_Reg0x2 & 0x20000) >> 17)

#define posUART2_Reg0x2_RX_FIFO_FULL                            18
#define bitUART2_Reg0x2_RX_FIFO_FULL                            0x40000
#define get_UART2_Reg0x2_RX_FIFO_FULL                           ((addUART2_Reg0x2 & 0x40000) >> 18)

#define posUART2_Reg0x2_RX_FIFO_EMPTY                           19
#define bitUART2_Reg0x2_RX_FIFO_EMPTY                           0x80000
#define get_UART2_Reg0x2_RX_FIFO_EMPTY                          ((addUART2_Reg0x2 & 0x80000) >> 19)

#define posUART2_Reg0x2_FIFO_WR_READY                           20
#define bitUART2_Reg0x2_FIFO_WR_READY                           0x100000
#define get_UART2_Reg0x2_FIFO_WR_READY                          ((addUART2_Reg0x2 & 0x100000) >> 20)

#define posUART2_Reg0x2_FIFO_RD_READY                           21
#define bitUART2_Reg0x2_FIFO_RD_READY                           0x200000
#define get_UART2_Reg0x2_FIFO_RD_READY                          ((addUART2_Reg0x2 & 0x200000) >> 21)

//addUART2_Reg0x3
#define addUART2_Reg0x3                                         *((volatile unsigned long *) (0x45840000+0x3*4))
#define posUART2_Reg0x3_UART_TX_FIFO_DIN                        0
#define bitUART2_Reg0x3_UART_TX_FIFO_DIN                        0xFF
#define set_UART2_Reg0x3_UART_TX_FIFO_DIN(val)                  addUART2_Reg0x3 = ((addUART2_Reg0x3 & (~0xFF)) | ((val) << 0))
#define get_UART2_Reg0x3_UART_TX_FIFO_DIN                       (addUART2_Reg0x3 & 0xFF)

#define posUART2_Reg0x3_UART_RX_FIFO_DOUT                       8
#define bitUART2_Reg0x3_UART_RX_FIFO_DOUT                       0xFF00
#define get_UART2_Reg0x3_UART_RX_FIFO_DOUT                      ((addUART2_Reg0x3 & 0xFF00) >> 8)

//addUART2_Reg0x4
#define addUART2_Reg0x4                                         *((volatile unsigned long *) (0x45840000+0x4*4))
#define posUART2_Reg0x4_TX_FIFO_NEED_WRITE_MASK                 0
#define bitUART2_Reg0x4_TX_FIFO_NEED_WRITE_MASK                 0x1
#define set_UART2_Reg0x4_TX_FIFO_NEED_WRITE_MASK(val)           addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_UART2_Reg0x4_TX_FIFO_NEED_WRITE_MASK               addUART2_Reg0x4 |= 0x1
#define clrf_UART2_Reg0x4_TX_FIFO_NEED_WRITE_MASK               addUART2_Reg0x4 &= ~0x1
#define get_UART2_Reg0x4_TX_FIFO_NEED_WRITE_MASK                (addUART2_Reg0x4 & 0x1)

#define posUART2_Reg0x4_RX_FIFO_NEED_READ_MASK                  1
#define bitUART2_Reg0x4_RX_FIFO_NEED_READ_MASK                  0x2
#define set_UART2_Reg0x4_RX_FIFO_NEED_READ_MASK(val)            addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x2)) | ((val) << 1))
#define setf_UART2_Reg0x4_RX_FIFO_NEED_READ_MASK                addUART2_Reg0x4 |= 0x2
#define clrf_UART2_Reg0x4_RX_FIFO_NEED_READ_MASK                addUART2_Reg0x4 &= ~0x2
#define get_UART2_Reg0x4_RX_FIFO_NEED_READ_MASK                 ((addUART2_Reg0x4 & 0x2) >> 1)

#define posUART2_Reg0x4_RX_FIFO_OVER_FLOW_MASK                  2
#define bitUART2_Reg0x4_RX_FIFO_OVER_FLOW_MASK                  0x4
#define set_UART2_Reg0x4_RX_FIFO_OVER_FLOW_MASK(val)            addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x4)) | ((val) << 2))
#define setf_UART2_Reg0x4_RX_FIFO_OVER_FLOW_MASK                addUART2_Reg0x4 |= 0x4
#define clrf_UART2_Reg0x4_RX_FIFO_OVER_FLOW_MASK                addUART2_Reg0x4 &= ~0x4
#define get_UART2_Reg0x4_RX_FIFO_OVER_FLOW_MASK                 ((addUART2_Reg0x4 & 0x4) >> 2)

#define posUART2_Reg0x4_UART_RX_PARITY_ERR_MASK                 3
#define bitUART2_Reg0x4_UART_RX_PARITY_ERR_MASK                 0x8
#define set_UART2_Reg0x4_UART_RX_PARITY_ERR_MASK(val)           addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x8)) | ((val) << 3))
#define setf_UART2_Reg0x4_UART_RX_PARITY_ERR_MASK               addUART2_Reg0x4 |= 0x8
#define clrf_UART2_Reg0x4_UART_RX_PARITY_ERR_MASK               addUART2_Reg0x4 &= ~0x8
#define get_UART2_Reg0x4_UART_RX_PARITY_ERR_MASK                ((addUART2_Reg0x4 & 0x8) >> 3)

#define posUART2_Reg0x4_UART_RX_STOP_ERR_MASK                   4
#define bitUART2_Reg0x4_UART_RX_STOP_ERR_MASK                   0x10
#define set_UART2_Reg0x4_UART_RX_STOP_ERR_MASK(val)             addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x10)) | ((val) << 4))
#define setf_UART2_Reg0x4_UART_RX_STOP_ERR_MASK                 addUART2_Reg0x4 |= 0x10
#define clrf_UART2_Reg0x4_UART_RX_STOP_ERR_MASK                 addUART2_Reg0x4 &= ~0x10
#define get_UART2_Reg0x4_UART_RX_STOP_ERR_MASK                  ((addUART2_Reg0x4 & 0x10) >> 4)

#define posUART2_Reg0x4_UART_TX_STOP_END_MASK                   5
#define bitUART2_Reg0x4_UART_TX_STOP_END_MASK                   0x20
#define set_UART2_Reg0x4_UART_TX_STOP_END_MASK(val)             addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x20)) | ((val) << 5))
#define setf_UART2_Reg0x4_UART_TX_STOP_END_MASK                 addUART2_Reg0x4 |= 0x20
#define clrf_UART2_Reg0x4_UART_TX_STOP_END_MASK                 addUART2_Reg0x4 &= ~0x20
#define get_UART2_Reg0x4_UART_TX_STOP_END_MASK                  ((addUART2_Reg0x4 & 0x20) >> 5)

#define posUART2_Reg0x4_UART_RX_STOP_END_MASK                   6
#define bitUART2_Reg0x4_UART_RX_STOP_END_MASK                   0x40
#define set_UART2_Reg0x4_UART_RX_STOP_END_MASK(val)             addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x40)) | ((val) << 6))
#define setf_UART2_Reg0x4_UART_RX_STOP_END_MASK                 addUART2_Reg0x4 |= 0x40
#define clrf_UART2_Reg0x4_UART_RX_STOP_END_MASK                 addUART2_Reg0x4 &= ~0x40
#define get_UART2_Reg0x4_UART_RX_STOP_END_MASK                  ((addUART2_Reg0x4 & 0x40) >> 6)

#define posUART2_Reg0x4_UART_RXD_WAKEUP_MASK                    7
#define bitUART2_Reg0x4_UART_RXD_WAKEUP_MASK                    0x80
#define set_UART2_Reg0x4_UART_RXD_WAKEUP_MASK(val)              addUART2_Reg0x4 = ((addUART2_Reg0x4 & (~0x80)) | ((val) << 7))
#define setf_UART2_Reg0x4_UART_RXD_WAKEUP_MASK                  addUART2_Reg0x4 |= 0x80
#define clrf_UART2_Reg0x4_UART_RXD_WAKEUP_MASK                  addUART2_Reg0x4 &= ~0x80
#define get_UART2_Reg0x4_UART_RXD_WAKEUP_MASK                   ((addUART2_Reg0x4 & 0x80) >> 7)

//addUART2_Reg0x5
#define addUART2_Reg0x5                                         *((volatile unsigned long *) (0x45840000+0x5*4))
#define posUART2_Reg0x5_TX_FIFO_NEED_WRITE                      0
#define bitUART2_Reg0x5_TX_FIFO_NEED_WRITE                      0x1
#define get_UART2_Reg0x5_TX_FIFO_NEED_WRITE                     (addUART2_Reg0x5 & 0x1)

#define posUART2_Reg0x5_RX_FIFO_NEED_READ                       1
#define bitUART2_Reg0x5_RX_FIFO_NEED_READ                       0x2
#define get_UART2_Reg0x5_RX_FIFO_NEED_READ                      ((addUART2_Reg0x5 & 0x2) >> 1)

#define posUART2_Reg0x5_RX_FIFO_OVER_FLOW                       2
#define bitUART2_Reg0x5_RX_FIFO_OVER_FLOW                       0x4
#define set_UART2_Reg0x5_RX_FIFO_OVER_FLOW(val)                 addUART2_Reg0x5 = ((addUART2_Reg0x5 & (~0x4)) | ((val) << 2))
#define setf_UART2_Reg0x5_RX_FIFO_OVER_FLOW                     addUART2_Reg0x5 |= 0x4
#define clrf_UART2_Reg0x5_RX_FIFO_OVER_FLOW                     addUART2_Reg0x5 &= ~0x4
#define get_UART2_Reg0x5_RX_FIFO_OVER_FLOW                      ((addUART2_Reg0x5 & 0x4) >> 2)

#define posUART2_Reg0x5_UART_RX_PARITY_ERROR                    3
#define bitUART2_Reg0x5_UART_RX_PARITY_ERROR                    0x8
#define set_UART2_Reg0x5_UART_RX_PARITY_ERROR(val)              addUART2_Reg0x5 = ((addUART2_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_UART2_Reg0x5_UART_RX_PARITY_ERROR                  addUART2_Reg0x5 |= 0x8
#define clrf_UART2_Reg0x5_UART_RX_PARITY_ERROR                  addUART2_Reg0x5 &= ~0x8
#define get_UART2_Reg0x5_UART_RX_PARITY_ERROR                   ((addUART2_Reg0x5 & 0x8) >> 3)

#define posUART2_Reg0x5_UART_RX_STOP_ERROR                      4
#define bitUART2_Reg0x5_UART_RX_STOP_ERROR                      0x10
#define set_UART2_Reg0x5_UART_RX_STOP_ERROR(val)                addUART2_Reg0x5 = ((addUART2_Reg0x5 & (~0x10)) | ((val) << 4))
#define setf_UART2_Reg0x5_UART_RX_STOP_ERROR                    addUART2_Reg0x5 |= 0x10
#define clrf_UART2_Reg0x5_UART_RX_STOP_ERROR                    addUART2_Reg0x5 &= ~0x10
#define get_UART2_Reg0x5_UART_RX_STOP_ERROR                     ((addUART2_Reg0x5 & 0x10) >> 4)

#define posUART2_Reg0x5_UART_TX_STOP_END                        5
#define bitUART2_Reg0x5_UART_TX_STOP_END                        0x20
#define set_UART2_Reg0x5_UART_TX_STOP_END(val)                  addUART2_Reg0x5 = ((addUART2_Reg0x5 & (~0x20)) | ((val) << 5))
#define setf_UART2_Reg0x5_UART_TX_STOP_END                      addUART2_Reg0x5 |= 0x20
#define clrf_UART2_Reg0x5_UART_TX_STOP_END                      addUART2_Reg0x5 &= ~0x20
#define get_UART2_Reg0x5_UART_TX_STOP_END                       ((addUART2_Reg0x5 & 0x20) >> 5)

#define posUART2_Reg0x5_UART_RX_STOP_END                        6
#define bitUART2_Reg0x5_UART_RX_STOP_END                        0x40
#define set_UART2_Reg0x5_UART_RX_STOP_END(val)                  addUART2_Reg0x5 = ((addUART2_Reg0x5 & (~0x40)) | ((val) << 6))
#define setf_UART2_Reg0x5_UART_RX_STOP_END                      addUART2_Reg0x5 |= 0x40
#define clrf_UART2_Reg0x5_UART_RX_STOP_END                      addUART2_Reg0x5 &= ~0x40
#define get_UART2_Reg0x5_UART_RX_STOP_END                       ((addUART2_Reg0x5 & 0x40) >> 6)

#define posUART2_Reg0x5_UART_RXD_WAKEUP                         7
#define bitUART2_Reg0x5_UART_RXD_WAKEUP                         0x80
#define set_UART2_Reg0x5_UART_RXD_WAKEUP(val)                   addUART2_Reg0x5 = ((addUART2_Reg0x5 & (~0x80)) | ((val) << 7))
#define setf_UART2_Reg0x5_UART_RXD_WAKEUP                       addUART2_Reg0x5 |= 0x80
#define clrf_UART2_Reg0x5_UART_RXD_WAKEUP                       addUART2_Reg0x5 &= ~0x80
#define get_UART2_Reg0x5_UART_RXD_WAKEUP                        ((addUART2_Reg0x5 & 0x80) >> 7)

//addUART2_Reg0x6
#define addUART2_Reg0x6                                         *((volatile unsigned long *) (0x45840000+0x6*4))
#define posUART2_Reg0x6_FLOW_CTL_LOW_CNT                        0
#define bitUART2_Reg0x6_FLOW_CTL_LOW_CNT                        0xFF
#define set_UART2_Reg0x6_FLOW_CTL_LOW_CNT(val)                  addUART2_Reg0x6 = ((addUART2_Reg0x6 & (~0xFF)) | ((val) << 0))
#define get_UART2_Reg0x6_FLOW_CTL_LOW_CNT                       (addUART2_Reg0x6 & 0xFF)

#define posUART2_Reg0x6_FLOW_CTL_HIGH_CNT                       8
#define bitUART2_Reg0x6_FLOW_CTL_HIGH_CNT                       0xFF00
#define set_UART2_Reg0x6_FLOW_CTL_HIGH_CNT(val)                 addUART2_Reg0x6 = ((addUART2_Reg0x6 & (~0xFF00)) | ((val) << 8))
#define get_UART2_Reg0x6_FLOW_CTL_HIGH_CNT                      ((addUART2_Reg0x6 & 0xFF00) >> 8)

#define posUART2_Reg0x6_FLOW_CONTROL_ENA                        16
#define bitUART2_Reg0x6_FLOW_CONTROL_ENA                        0x10000
#define set_UART2_Reg0x6_FLOW_CONTROL_ENA(val)                  addUART2_Reg0x6 = ((addUART2_Reg0x6 & (~0x10000)) | ((val) << 16))
#define setf_UART2_Reg0x6_FLOW_CONTROL_ENA                      addUART2_Reg0x6 |= 0x10000
#define clrf_UART2_Reg0x6_FLOW_CONTROL_ENA                      addUART2_Reg0x6 &= ~0x10000
#define get_UART2_Reg0x6_FLOW_CONTROL_ENA                       ((addUART2_Reg0x6 & 0x10000) >> 16)

#define posUART2_Reg0x6_RTS_POLARITY_SEL                        17
#define bitUART2_Reg0x6_RTS_POLARITY_SEL                        0x20000
#define set_UART2_Reg0x6_RTS_POLARITY_SEL(val)                  addUART2_Reg0x6 = ((addUART2_Reg0x6 & (~0x20000)) | ((val) << 17))
#define setf_UART2_Reg0x6_RTS_POLARITY_SEL                      addUART2_Reg0x6 |= 0x20000
#define clrf_UART2_Reg0x6_RTS_POLARITY_SEL                      addUART2_Reg0x6 &= ~0x20000
#define get_UART2_Reg0x6_RTS_POLARITY_SEL                       ((addUART2_Reg0x6 & 0x20000) >> 17)

#define posUART2_Reg0x6_CTS_POLARITY_SEL                        18
#define bitUART2_Reg0x6_CTS_POLARITY_SEL                        0x40000
#define set_UART2_Reg0x6_CTS_POLARITY_SEL(val)                  addUART2_Reg0x6 = ((addUART2_Reg0x6 & (~0x40000)) | ((val) << 18))
#define setf_UART2_Reg0x6_CTS_POLARITY_SEL                      addUART2_Reg0x6 |= 0x40000
#define clrf_UART2_Reg0x6_CTS_POLARITY_SEL                      addUART2_Reg0x6 &= ~0x40000
#define get_UART2_Reg0x6_CTS_POLARITY_SEL                       ((addUART2_Reg0x6 & 0x40000) >> 18)

//addUART2_Reg0x7
#define addUART2_Reg0x7                                         *((volatile unsigned long *) (0x45840000+0x7*4))
#define posUART2_Reg0x7_UART_WAKE_COUNT                         0
#define bitUART2_Reg0x7_UART_WAKE_COUNT                         0x3FF
#define set_UART2_Reg0x7_UART_WAKE_COUNT(val)                   addUART2_Reg0x7 = ((addUART2_Reg0x7 & (~0x3FF)) | ((val) << 0))
#define get_UART2_Reg0x7_UART_WAKE_COUNT                        (addUART2_Reg0x7 & 0x3FF)

#define posUART2_Reg0x7_UART_TXD_WAIT_CNT                       10
#define bitUART2_Reg0x7_UART_TXD_WAIT_CNT                       0xFFC00
#define set_UART2_Reg0x7_UART_TXD_WAIT_CNT(val)                 addUART2_Reg0x7 = ((addUART2_Reg0x7 & (~0xFFC00)) | ((val) << 10))
#define get_UART2_Reg0x7_UART_TXD_WAIT_CNT                      ((addUART2_Reg0x7 & 0xFFC00) >> 10)

#define posUART2_Reg0x7_UART_RXD_WAKE_EN                        20
#define bitUART2_Reg0x7_UART_RXD_WAKE_EN                        0x100000
#define set_UART2_Reg0x7_UART_RXD_WAKE_EN(val)                  addUART2_Reg0x7 = ((addUART2_Reg0x7 & (~0x100000)) | ((val) << 20))
#define setf_UART2_Reg0x7_UART_RXD_WAKE_EN                      addUART2_Reg0x7 |= 0x100000
#define clrf_UART2_Reg0x7_UART_RXD_WAKE_EN                      addUART2_Reg0x7 &= ~0x100000
#define get_UART2_Reg0x7_UART_RXD_WAKE_EN                       ((addUART2_Reg0x7 & 0x100000) >> 20)

#define posUART2_Reg0x7_UART_TXD_WAKE_EN                        21
#define bitUART2_Reg0x7_UART_TXD_WAKE_EN                        0x200000
#define set_UART2_Reg0x7_UART_TXD_WAKE_EN(val)                  addUART2_Reg0x7 = ((addUART2_Reg0x7 & (~0x200000)) | ((val) << 21))
#define setf_UART2_Reg0x7_UART_TXD_WAKE_EN                      addUART2_Reg0x7 |= 0x200000
#define clrf_UART2_Reg0x7_UART_TXD_WAKE_EN                      addUART2_Reg0x7 &= ~0x200000
#define get_UART2_Reg0x7_UART_TXD_WAKE_EN                       ((addUART2_Reg0x7 & 0x200000) >> 21)

#define posUART2_Reg0x7_RXD_NEGEDGE_WAKE_EN                     22
#define bitUART2_Reg0x7_RXD_NEGEDGE_WAKE_EN                     0x400000
#define set_UART2_Reg0x7_RXD_NEGEDGE_WAKE_EN(val)               addUART2_Reg0x7 = ((addUART2_Reg0x7 & (~0x400000)) | ((val) << 22))
#define setf_UART2_Reg0x7_RXD_NEGEDGE_WAKE_EN                   addUART2_Reg0x7 |= 0x400000
#define clrf_UART2_Reg0x7_RXD_NEGEDGE_WAKE_EN                   addUART2_Reg0x7 &= ~0x400000
#define get_UART2_Reg0x7_RXD_NEGEDGE_WAKE_EN                    ((addUART2_Reg0x7 & 0x400000) >> 22)


//************************************************************//
//SPI
//************************************************************//
#define BASEADDR_SPI                                            0x44860000
//addSPI_Reg0x0
#define addSPI_Reg0x0                                           *((volatile unsigned long *) (0x44860000+0x0*4))

#define posSPI_Reg0x0_BYTE_INTLVAL                              24
#define bitSPI_Reg0x0_BYTE_INTLVAL                              0x3F000000
#define set_SPI_Reg0x0_BYTE_INTLVAL(val)                        addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x3F000000)) | ((val) << 24))
#define get_SPI_Reg0x0_BYTE_INTLVAL                             ((addSPI_Reg0x0 & 0x3F000000) >> 24)

#define posSPI_Reg0x0_SPIEN                                     23
#define bitSPI_Reg0x0_SPIEN                                     0x800000
#define set_SPI_Reg0x0_SPIEN(val)                               addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x800000)) | ((val) << 23))
#define setf_SPI_Reg0x0_SPIEN                                   addSPI_Reg0x0 |= 0x800000
#define clrf_SPI_Reg0x0_SPIEN                                   addSPI_Reg0x0 &= ~0x800000
#define get_SPI_Reg0x0_SPIEN                                    ((addSPI_Reg0x0 & 0x800000) >> 23)

#define posSPI_Reg0x0_MSTEN                                     22
#define bitSPI_Reg0x0_MSTEN                                     0x400000
#define set_SPI_Reg0x0_MSTEN(val)                               addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x400000)) | ((val) << 22))
#define setf_SPI_Reg0x0_MSTEN                                   addSPI_Reg0x0 |= 0x400000
#define clrf_SPI_Reg0x0_MSTEN                                   addSPI_Reg0x0 &= ~0x400000
#define get_SPI_Reg0x0_MSTEN                                    ((addSPI_Reg0x0 & 0x400000) >> 22)

#define posSPI_Reg0x0_CKPHA                                     21
#define bitSPI_Reg0x0_CKPHA                                     0x200000
#define set_SPI_Reg0x0_CKPHA(val)                               addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x200000)) | ((val) << 21))
#define setf_SPI_Reg0x0_CKPHA                                   addSPI_Reg0x0 |= 0x200000
#define clrf_SPI_Reg0x0_CKPHA                                   addSPI_Reg0x0 &= ~0x200000
#define get_SPI_Reg0x0_CKPHA                                    ((addSPI_Reg0x0 & 0x200000) >> 21)

#define posSPI_Reg0x0_CKPOL                                     20
#define bitSPI_Reg0x0_CKPOL                                     0x100000
#define set_SPI_Reg0x0_CKPOL(val)                               addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x100000)) | ((val) << 20))
#define setf_SPI_Reg0x0_CKPOL                                   addSPI_Reg0x0 |= 0x100000
#define clrf_SPI_Reg0x0_CKPOL                                   addSPI_Reg0x0 &= ~0x100000
#define get_SPI_Reg0x0_CKPOL                                    ((addSPI_Reg0x0 & 0x100000) >> 20)

#define posSPI_Reg0x0_LSB_FIRST                                 19
#define bitSPI_Reg0x0_LSB_FIRST                                 0x80000
#define set_SPI_Reg0x0_LSB_FIRST(val)                           addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x80000)) | ((val) << 19))
#define setf_SPI_Reg0x0_LSB_FIRST                               addSPI_Reg0x0 |= 0x80000
#define clrf_SPI_Reg0x0_LSB_FIRST                               addSPI_Reg0x0 &= ~0x80000
#define get_SPI_Reg0x0_LSB_FIRST                                ((addSPI_Reg0x0 & 0x80000) >> 19)

#define posSPI_Reg0x0_BIT_WDTH                                  18
#define bitSPI_Reg0x0_BIT_WDTH                                  0x40000
#define set_SPI_Reg0x0_BIT_WDTH(val)                            addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x40000)) | ((val) << 18))
#define setf_SPI_Reg0x0_BIT_WDTH                                addSPI_Reg0x0 |= 0x40000
#define clrf_SPI_Reg0x0_BIT_WDTH                                addSPI_Reg0x0 &= ~0x40000
#define get_SPI_Reg0x0_BIT_WDTH                                 ((addSPI_Reg0x0 & 0x40000) >> 18)

#define posSPI_Reg0x0_WIRE3_EN                                  17
#define bitSPI_Reg0x0_WIRE3_EN                                  0x20000
#define set_SPI_Reg0x0_WIRE3_EN(val)                            addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x20000)) | ((val) << 17))
#define setf_SPI_Reg0x0_WIRE3_EN                                addSPI_Reg0x0 |= 0x20000
#define clrf_SPI_Reg0x0_WIRE3_EN                                addSPI_Reg0x0 &= ~0x20000
#define get_SPI_Reg0x0_WIRE3_EN                                 ((addSPI_Reg0x0 & 0x20000) >> 17)

#define posSPI_Reg0x0_SLV_RELEASE_INTEN                         16
#define bitSPI_Reg0x0_SLV_RELEASE_INTEN                         0x10000
#define set_SPI_Reg0x0_SLV_RELEASE_INTEN(val)                   addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x10000)) | ((val) << 16))
#define setf_SPI_Reg0x0_SLV_RELEASE_INTEN                       addSPI_Reg0x0 |= 0x10000
#define clrf_SPI_Reg0x0_SLV_RELEASE_INTEN                       addSPI_Reg0x0 &= ~0x10000
#define get_SPI_Reg0x0_SLV_RELEASE_INTEN                        ((addSPI_Reg0x0 & 0x10000) >> 16)

#define posSPI_Reg0x0_SPI_CKR                                   8
#define bitSPI_Reg0x0_SPI_CKR                                   0xFF00
#define set_SPI_Reg0x0_SPI_CKR(val)                             addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0xFF00)) | ((val) << 8))
#define get_SPI_Reg0x0_SPI_CKR                                  ((addSPI_Reg0x0 & 0xFF00) >> 8)

#define posSPI_Reg0x0_RXFIFO_INT_EN                             7
#define bitSPI_Reg0x0_RXFIFO_INT_EN                             0x80
#define set_SPI_Reg0x0_RXFIFO_INT_EN(val)                       addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_SPI_Reg0x0_RXFIFO_INT_EN                           addSPI_Reg0x0 |= 0x80
#define clrf_SPI_Reg0x0_RXFIFO_INT_EN                           addSPI_Reg0x0 &= ~0x80
#define get_SPI_Reg0x0_RXFIFO_INT_EN                            ((addSPI_Reg0x0 & 0x80) >> 7)

#define posSPI_Reg0x0_TXFIFO_INT_EN                             6
#define bitSPI_Reg0x0_TXFIFO_INT_EN                             0x40
#define set_SPI_Reg0x0_TXFIFO_INT_EN(val)                       addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_SPI_Reg0x0_TXFIFO_INT_EN                           addSPI_Reg0x0 |= 0x40
#define clrf_SPI_Reg0x0_TXFIFO_INT_EN                           addSPI_Reg0x0 &= ~0x40
#define get_SPI_Reg0x0_TXFIFO_INT_EN                            ((addSPI_Reg0x0 & 0x40) >> 6)

#define posSPI_Reg0x0_RXOVF_EN                                  5
#define bitSPI_Reg0x0_RXOVF_EN                                  0x20
#define set_SPI_Reg0x0_RXOVF_EN(val)                            addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_SPI_Reg0x0_RXOVF_EN                                addSPI_Reg0x0 |= 0x20
#define clrf_SPI_Reg0x0_RXOVF_EN                                addSPI_Reg0x0 &= ~0x20
#define get_SPI_Reg0x0_RXOVF_EN                                 ((addSPI_Reg0x0 & 0x20) >> 5)

#define posSPI_Reg0x0_TXUDF_EN                                  4
#define bitSPI_Reg0x0_TXUDF_EN                                  0x10
#define set_SPI_Reg0x0_TXUDF_EN(val)                            addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_SPI_Reg0x0_TXUDF_EN                                addSPI_Reg0x0 |= 0x10
#define clrf_SPI_Reg0x0_TXUDF_EN                                addSPI_Reg0x0 &= ~0x10
#define get_SPI_Reg0x0_TXUDF_EN                                 ((addSPI_Reg0x0 & 0x10) >> 4)

#define posSPI_Reg0x0_RXFIFO_INT_LEVEL                          2
#define bitSPI_Reg0x0_RXFIFO_INT_LEVEL                          0xC
#define set_SPI_Reg0x0_RXFIFO_INT_LEVEL(val)                    addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0xC)) | ((val) << 2))
#define get_SPI_Reg0x0_RXFIFO_INT_LEVEL                         ((addSPI_Reg0x0 & 0xC) >> 2)

#define posSPI_Reg0x0_TXFIFO_INT_LEVEL                          0
#define bitSPI_Reg0x0_TXFIFO_INT_LEVEL                          0x3
#define set_SPI_Reg0x0_TXFIFO_INT_LEVEL(val)                    addSPI_Reg0x0 = ((addSPI_Reg0x0 & (~0x3)) | ((val) << 0))
#define get_SPI_Reg0x0_TXFIFO_INT_LEVEL                         (addSPI_Reg0x0 & 0x3)

//addSPI_Reg0x1
#define addSPI_Reg0x1                                           *((volatile unsigned long *) (0x44860000+0x1*4))
#define posSPI_Reg0x1_RX_TRANS_LEN                              20
#define bitSPI_Reg0x1_RX_TRANS_LEN                              0xFFF00000
#define set_SPI_Reg0x1_RX_TRANS_LEN(val)                        addSPI_Reg0x1 = ((addSPI_Reg0x1 & (~0xFFF00000)) | ((val) << 20))
#define get_SPI_Reg0x1_RX_TRANS_LEN                             ((addSPI_Reg0x1 & 0xFFF00000) >> 20)

#define posSPI_Reg0x1_TX_TRANS_LEN                              8
#define bitSPI_Reg0x1_TX_TRANS_LEN                              0xFFF00
#define set_SPI_Reg0x1_TX_TRANS_LEN(val)                        addSPI_Reg0x1 = ((addSPI_Reg0x1 & (~0xFFF00)) | ((val) << 8))
#define get_SPI_Reg0x1_TX_TRANS_LEN                             ((addSPI_Reg0x1 & 0xFFF00) >> 8)


#define posSPI_Reg0x1_RX_FINISH_INT_EN                          3
#define bitSPI_Reg0x1_RX_FINISH_INT_EN                          0x8
#define set_SPI_Reg0x1_RX_FINISH_INT_EN(val)                    addSPI_Reg0x1 = ((addSPI_Reg0x1 & (~0x8)) | ((val) << 3))
#define setf_SPI_Reg0x1_RX_FINISH_INT_EN                        addSPI_Reg0x1 |= 0x8
#define clrf_SPI_Reg0x1_RX_FINISH_INT_EN                        addSPI_Reg0x1 &= ~0x8
#define get_SPI_Reg0x1_RX_FINISH_INT_EN                         ((addSPI_Reg0x1 & 0x8) >> 3)

#define posSPI_Reg0x1_TX_FINISH_INT_EN                          2
#define bitSPI_Reg0x1_TX_FINISH_INT_EN                          0x4
#define set_SPI_Reg0x1_TX_FINISH_INT_EN(val)                    addSPI_Reg0x1 = ((addSPI_Reg0x1 & (~0x4)) | ((val) << 2))
#define setf_SPI_Reg0x1_TX_FINISH_INT_EN                        addSPI_Reg0x1 |= 0x4
#define clrf_SPI_Reg0x1_TX_FINISH_INT_EN                        addSPI_Reg0x1 &= ~0x4
#define get_SPI_Reg0x1_TX_FINISH_INT_EN                         ((addSPI_Reg0x1 & 0x4) >> 2)

#define posSPI_Reg0x1_RX_EN                                     1
#define bitSPI_Reg0x1_RX_EN                                     0x2
#define set_SPI_Reg0x1_RX_EN(val)                               addSPI_Reg0x1 = ((addSPI_Reg0x1 & (~0x2)) | ((val) << 1))
#define setf_SPI_Reg0x1_RX_EN                                   addSPI_Reg0x1 |= 0x2
#define clrf_SPI_Reg0x1_RX_EN                                   addSPI_Reg0x1 &= ~0x2
#define get_SPI_Reg0x1_RX_EN                                    ((addSPI_Reg0x1 & 0x2) >> 1)

#define posSPI_Reg0x1_TX_EN                                     0
#define bitSPI_Reg0x1_TX_EN                                     0x1
#define set_SPI_Reg0x1_TX_EN(val)                               addSPI_Reg0x1 = ((addSPI_Reg0x1 & (~0x1)) | ((val) << 0))
#define setf_SPI_Reg0x1_TX_EN                                   addSPI_Reg0x1 |= 0x1
#define clrf_SPI_Reg0x1_TX_EN                                   addSPI_Reg0x1 &= ~0x1
#define get_SPI_Reg0x1_TX_EN                                    (addSPI_Reg0x1 & 0x1)

//addSPI_Reg0x2
#define addSPI_Reg0x2                                           *((volatile unsigned long *) (0x44860000+0x2*4))

#define posSPI_Reg0x2_RXFIFO_CLR                                17
#define bitSPI_Reg0x2_RXFIFO_CLR                                0x20000
#define set_SPI_Reg0x2_RXFIFO_CLR(val)                          addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x20000)) | ((val) << 17))
#define setf_SPI_Reg0x2_RXFIFO_CLR                              addSPI_Reg0x2 |= 0x20000
#define clrf_SPI_Reg0x2_RXFIFO_CLR                              addSPI_Reg0x2 &= ~0x20000
#define get_SPI_Reg0x2_RXFIFO_CLR                               ((addSPI_Reg0x2 & 0x20000) >> 17)

#define posSPI_Reg0x2_TXFIFO_CLR                                16
#define bitSPI_Reg0x2_TXFIFO_CLR                                0x10000
#define set_SPI_Reg0x2_TXFIFO_CLR(val)                          addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x10000)) | ((val) << 16))
#define setf_SPI_Reg0x2_TXFIFO_CLR                              addSPI_Reg0x2 |= 0x10000
#define clrf_SPI_Reg0x2_TXFIFO_CLR                              addSPI_Reg0x2 &= ~0x10000
#define get_SPI_Reg0x2_TXFIFO_CLR                               ((addSPI_Reg0x2 & 0x10000) >> 16)


#define posSPI_Reg0x2_RX_FINISH_INT                             14
#define bitSPI_Reg0x2_RX_FINISH_INT                             0x4000
#define get_SPI_Reg0x2_RX_FINISH_INT                            ((addSPI_Reg0x2 & 0x4000) >> 14)

#define posSPI_Reg0x2_TX_FINISH_INT                             13
#define bitSPI_Reg0x2_TX_FINISH_INT                             0x2000
#define set_SPI_Reg0x2_TX_FINISH_INT(val)                       addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x2000)) | ((val) << 13))
#define setf_SPI_Reg0x2_TX_FINISH_INT                           addSPI_Reg0x2 |= 0x2000
#define clrf_SPI_Reg0x2_TX_FINISH_INT                           addSPI_Reg0x2 &= ~0x2000
#define get_SPI_Reg0x2_TX_FINISH_INT                            ((addSPI_Reg0x2 & 0x2000) >> 13)

#define posSPI_Reg0x2_RXOVF                                     12
#define bitSPI_Reg0x2_RXOVF                                     0x1000
#define set_SPI_Reg0x2_RXOVF(val)                               addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x1000)) | ((val) << 12))
#define setf_SPI_Reg0x2_RXOVF                                   addSPI_Reg0x2 |= 0x1000
#define clrf_SPI_Reg0x2_RXOVF                                   addSPI_Reg0x2 &= ~0x1000
#define get_SPI_Reg0x2_RXOVF                                    ((addSPI_Reg0x2 & 0x1000) >> 12)

#define posSPI_Reg0x2_TXUDF                                     11
#define bitSPI_Reg0x2_TXUDF                                     0x800
#define set_SPI_Reg0x2_TXUDF(val)                               addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x800)) | ((val) << 11))
#define setf_SPI_Reg0x2_TXUDF                                   addSPI_Reg0x2 |= 0x800
#define clrf_SPI_Reg0x2_TXUDF                                   addSPI_Reg0x2 &= ~0x800
#define get_SPI_Reg0x2_TXUDF                                    ((addSPI_Reg0x2 & 0x800) >> 11)

#define posSPI_Reg0x2_SLV_RELEASE_INT                           10
#define bitSPI_Reg0x2_SLV_RELEASE_INT                           0x400
#define set_SPI_Reg0x2_SLV_RELEASE_INT(val)                     addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x400)) | ((val) << 10))
#define setf_SPI_Reg0x2_SLV_RELEASE_INT                         addSPI_Reg0x2 |= 0x400
#define clrf_SPI_Reg0x2_SLV_RELEASE_INT                         addSPI_Reg0x2 &= ~0x400
#define get_SPI_Reg0x2_SLV_RELEASE_INT                          ((addSPI_Reg0x2 & 0x400) >> 10)

#define posSPI_Reg0x2_RXFIFO_INT                                9
#define bitSPI_Reg0x2_RXFIFO_INT                                0x200
#define set_SPI_Reg0x2_RXFIFO_INT(val)                          addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x200)) | ((val) << 9))
#define setf_SPI_Reg0x2_RXFIFO_INT                              addSPI_Reg0x2 |= 0x200
#define clrf_SPI_Reg0x2_RXFIFO_INT                              addSPI_Reg0x2 &= ~0x200
#define get_SPI_Reg0x2_RXFIFO_INT                               ((addSPI_Reg0x2 & 0x200) >> 9)

#define posSPI_Reg0x2_TXFIFO_INT                                8
#define bitSPI_Reg0x2_TXFIFO_INT                                0x100
#define set_SPI_Reg0x2_TXFIFO_INT(val)                          addSPI_Reg0x2 = ((addSPI_Reg0x2 & (~0x100)) | ((val) << 8))
#define setf_SPI_Reg0x2_TXFIFO_INT                              addSPI_Reg0x2 |= 0x100
#define clrf_SPI_Reg0x2_TXFIFO_INT                              addSPI_Reg0x2 &= ~0x100
#define get_SPI_Reg0x2_TXFIFO_INT                               ((addSPI_Reg0x2 & 0x100) >> 8)



#define posSPI_Reg0x2_RXFIFO_RD_READY                           2
#define bitSPI_Reg0x2_RXFIFO_RD_READY                           0x4
#define get_SPI_Reg0x2_RXFIFO_RD_READY                          ((addSPI_Reg0x2 & 0x4) >> 2)

#define posSPI_Reg0x2_TXFIFO_WR_READY                           1
#define bitSPI_Reg0x2_TXFIFO_WR_READY                           0x2
#define get_SPI_Reg0x2_TXFIFO_WR_READY                          ((addSPI_Reg0x2 & 0x2) >> 1)


//addSPI_Reg0x3
#define addSPI_Reg0x3                                           *((volatile unsigned long *) (0x44860000+0x3*4))

#define posSPI_Reg0x3_SPI_DAT                                   0
#define bitSPI_Reg0x3_SPI_DAT                                   0xFFFF
#define set_SPI_Reg0x3_SPI_DAT(val)                             addSPI_Reg0x3 = ((addSPI_Reg0x3 & (~0xFFFF)) | ((val) << 0))
#define get_SPI_Reg0x3_SPI_DAT                                  (addSPI_Reg0x3 & 0xFFFF)


//************************************************************//
//SPI1
//************************************************************//
#define BASEADDR_SPI1                                           0x458C0000
//addSPI1_Reg0x0
#define addSPI1_Reg0x0                                          *((volatile unsigned long *) (0x458C0000+0x0*4))

#define posSPI1_Reg0x0_BYTE_INTLVAL                             24
#define bitSPI1_Reg0x0_BYTE_INTLVAL                             0x3F000000
#define set_SPI1_Reg0x0_BYTE_INTLVAL(val)                       addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x3F000000)) | ((val) << 24))
#define get_SPI1_Reg0x0_BYTE_INTLVAL                            ((addSPI1_Reg0x0 & 0x3F000000) >> 24)

#define posSPI1_Reg0x0_SPIEN                                    23
#define bitSPI1_Reg0x0_SPIEN                                    0x800000
#define set_SPI1_Reg0x0_SPIEN(val)                              addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x800000)) | ((val) << 23))
#define setf_SPI1_Reg0x0_SPIEN                                  addSPI1_Reg0x0 |= 0x800000
#define clrf_SPI1_Reg0x0_SPIEN                                  addSPI1_Reg0x0 &= ~0x800000
#define get_SPI1_Reg0x0_SPIEN                                   ((addSPI1_Reg0x0 & 0x800000) >> 23)

#define posSPI1_Reg0x0_MSTEN                                    22
#define bitSPI1_Reg0x0_MSTEN                                    0x400000
#define set_SPI1_Reg0x0_MSTEN(val)                              addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x400000)) | ((val) << 22))
#define setf_SPI1_Reg0x0_MSTEN                                  addSPI1_Reg0x0 |= 0x400000
#define clrf_SPI1_Reg0x0_MSTEN                                  addSPI1_Reg0x0 &= ~0x400000
#define get_SPI1_Reg0x0_MSTEN                                   ((addSPI1_Reg0x0 & 0x400000) >> 22)

#define posSPI1_Reg0x0_CKPHA                                    21
#define bitSPI1_Reg0x0_CKPHA                                    0x200000
#define set_SPI1_Reg0x0_CKPHA(val)                              addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x200000)) | ((val) << 21))
#define setf_SPI1_Reg0x0_CKPHA                                  addSPI1_Reg0x0 |= 0x200000
#define clrf_SPI1_Reg0x0_CKPHA                                  addSPI1_Reg0x0 &= ~0x200000
#define get_SPI1_Reg0x0_CKPHA                                   ((addSPI1_Reg0x0 & 0x200000) >> 21)

#define posSPI1_Reg0x0_CKPOL                                    20
#define bitSPI1_Reg0x0_CKPOL                                    0x100000
#define set_SPI1_Reg0x0_CKPOL(val)                              addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x100000)) | ((val) << 20))
#define setf_SPI1_Reg0x0_CKPOL                                  addSPI1_Reg0x0 |= 0x100000
#define clrf_SPI1_Reg0x0_CKPOL                                  addSPI1_Reg0x0 &= ~0x100000
#define get_SPI1_Reg0x0_CKPOL                                   ((addSPI1_Reg0x0 & 0x100000) >> 20)

#define posSPI1_Reg0x0_LSB_FIRST                                19
#define bitSPI1_Reg0x0_LSB_FIRST                                0x80000
#define set_SPI1_Reg0x0_LSB_FIRST(val)                          addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x80000)) | ((val) << 19))
#define setf_SPI1_Reg0x0_LSB_FIRST                              addSPI1_Reg0x0 |= 0x80000
#define clrf_SPI1_Reg0x0_LSB_FIRST                              addSPI1_Reg0x0 &= ~0x80000
#define get_SPI1_Reg0x0_LSB_FIRST                               ((addSPI1_Reg0x0 & 0x80000) >> 19)

#define posSPI1_Reg0x0_BIT_WDTH                                 18
#define bitSPI1_Reg0x0_BIT_WDTH                                 0x40000
#define set_SPI1_Reg0x0_BIT_WDTH(val)                           addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x40000)) | ((val) << 18))
#define setf_SPI1_Reg0x0_BIT_WDTH                               addSPI1_Reg0x0 |= 0x40000
#define clrf_SPI1_Reg0x0_BIT_WDTH                               addSPI1_Reg0x0 &= ~0x40000
#define get_SPI1_Reg0x0_BIT_WDTH                                ((addSPI1_Reg0x0 & 0x40000) >> 18)

#define posSPI1_Reg0x0_WIRE3_EN                                 17
#define bitSPI1_Reg0x0_WIRE3_EN                                 0x20000
#define set_SPI1_Reg0x0_WIRE3_EN(val)                           addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x20000)) | ((val) << 17))
#define setf_SPI1_Reg0x0_WIRE3_EN                               addSPI1_Reg0x0 |= 0x20000
#define clrf_SPI1_Reg0x0_WIRE3_EN                               addSPI1_Reg0x0 &= ~0x20000
#define get_SPI1_Reg0x0_WIRE3_EN                                ((addSPI1_Reg0x0 & 0x20000) >> 17)

#define posSPI1_Reg0x0_SLV_RELEASE_INTEN                        16
#define bitSPI1_Reg0x0_SLV_RELEASE_INTEN                        0x10000
#define set_SPI1_Reg0x0_SLV_RELEASE_INTEN(val)                  addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x10000)) | ((val) << 16))
#define setf_SPI1_Reg0x0_SLV_RELEASE_INTEN                      addSPI1_Reg0x0 |= 0x10000
#define clrf_SPI1_Reg0x0_SLV_RELEASE_INTEN                      addSPI1_Reg0x0 &= ~0x10000
#define get_SPI1_Reg0x0_SLV_RELEASE_INTEN                       ((addSPI1_Reg0x0 & 0x10000) >> 16)

#define posSPI1_Reg0x0_SPI_CKR                                  8
#define bitSPI1_Reg0x0_SPI_CKR                                  0xFF00
#define set_SPI1_Reg0x0_SPI_CKR(val)                            addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0xFF00)) | ((val) << 8))
#define get_SPI1_Reg0x0_SPI_CKR                                 ((addSPI1_Reg0x0 & 0xFF00) >> 8)

#define posSPI1_Reg0x0_RXFIFO_INT_EN                            7
#define bitSPI1_Reg0x0_RXFIFO_INT_EN                            0x80
#define set_SPI1_Reg0x0_RXFIFO_INT_EN(val)                      addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_SPI1_Reg0x0_RXFIFO_INT_EN                          addSPI1_Reg0x0 |= 0x80
#define clrf_SPI1_Reg0x0_RXFIFO_INT_EN                          addSPI1_Reg0x0 &= ~0x80
#define get_SPI1_Reg0x0_RXFIFO_INT_EN                           ((addSPI1_Reg0x0 & 0x80) >> 7)

#define posSPI1_Reg0x0_TXFIFO_INT_EN                            6
#define bitSPI1_Reg0x0_TXFIFO_INT_EN                            0x40
#define set_SPI1_Reg0x0_TXFIFO_INT_EN(val)                      addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_SPI1_Reg0x0_TXFIFO_INT_EN                          addSPI1_Reg0x0 |= 0x40
#define clrf_SPI1_Reg0x0_TXFIFO_INT_EN                          addSPI1_Reg0x0 &= ~0x40
#define get_SPI1_Reg0x0_TXFIFO_INT_EN                           ((addSPI1_Reg0x0 & 0x40) >> 6)

#define posSPI1_Reg0x0_RXOVF_EN                                 5
#define bitSPI1_Reg0x0_RXOVF_EN                                 0x20
#define set_SPI1_Reg0x0_RXOVF_EN(val)                           addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_SPI1_Reg0x0_RXOVF_EN                               addSPI1_Reg0x0 |= 0x20
#define clrf_SPI1_Reg0x0_RXOVF_EN                               addSPI1_Reg0x0 &= ~0x20
#define get_SPI1_Reg0x0_RXOVF_EN                                ((addSPI1_Reg0x0 & 0x20) >> 5)

#define posSPI1_Reg0x0_TXUDF_EN                                 4
#define bitSPI1_Reg0x0_TXUDF_EN                                 0x10
#define set_SPI1_Reg0x0_TXUDF_EN(val)                           addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_SPI1_Reg0x0_TXUDF_EN                               addSPI1_Reg0x0 |= 0x10
#define clrf_SPI1_Reg0x0_TXUDF_EN                               addSPI1_Reg0x0 &= ~0x10
#define get_SPI1_Reg0x0_TXUDF_EN                                ((addSPI1_Reg0x0 & 0x10) >> 4)

#define posSPI1_Reg0x0_RXFIFO_INT_LEVEL                         2
#define bitSPI1_Reg0x0_RXFIFO_INT_LEVEL                         0xC
#define set_SPI1_Reg0x0_RXFIFO_INT_LEVEL(val)                   addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0xC)) | ((val) << 2))
#define get_SPI1_Reg0x0_RXFIFO_INT_LEVEL                        ((addSPI1_Reg0x0 & 0xC) >> 2)

#define posSPI1_Reg0x0_TXFIFO_INT_LEVEL                         0
#define bitSPI1_Reg0x0_TXFIFO_INT_LEVEL                         0x3
#define set_SPI1_Reg0x0_TXFIFO_INT_LEVEL(val)                   addSPI1_Reg0x0 = ((addSPI1_Reg0x0 & (~0x3)) | ((val) << 0))
#define get_SPI1_Reg0x0_TXFIFO_INT_LEVEL                        (addSPI1_Reg0x0 & 0x3)

//addSPI1_Reg0x1
#define addSPI1_Reg0x1                                          *((volatile unsigned long *) (0x458C0000+0x1*4))
#define posSPI1_Reg0x1_RX_TRANS_LEN                             20
#define bitSPI1_Reg0x1_RX_TRANS_LEN                             0xFFF00000
#define set_SPI1_Reg0x1_RX_TRANS_LEN(val)                       addSPI1_Reg0x1 = ((addSPI1_Reg0x1 & (~0xFFF00000)) | ((val) << 20))
#define get_SPI1_Reg0x1_RX_TRANS_LEN                            ((addSPI1_Reg0x1 & 0xFFF00000) >> 20)

#define posSPI1_Reg0x1_TX_TRANS_LEN                             8
#define bitSPI1_Reg0x1_TX_TRANS_LEN                             0xFFF00
#define set_SPI1_Reg0x1_TX_TRANS_LEN(val)                       addSPI1_Reg0x1 = ((addSPI1_Reg0x1 & (~0xFFF00)) | ((val) << 8))
#define get_SPI1_Reg0x1_TX_TRANS_LEN                            ((addSPI1_Reg0x1 & 0xFFF00) >> 8)


#define posSPI1_Reg0x1_RX_FINISH_INT_EN                         3
#define bitSPI1_Reg0x1_RX_FINISH_INT_EN                         0x8
#define set_SPI1_Reg0x1_RX_FINISH_INT_EN(val)                   addSPI1_Reg0x1 = ((addSPI1_Reg0x1 & (~0x8)) | ((val) << 3))
#define setf_SPI1_Reg0x1_RX_FINISH_INT_EN                       addSPI1_Reg0x1 |= 0x8
#define clrf_SPI1_Reg0x1_RX_FINISH_INT_EN                       addSPI1_Reg0x1 &= ~0x8
#define get_SPI1_Reg0x1_RX_FINISH_INT_EN                        ((addSPI1_Reg0x1 & 0x8) >> 3)

#define posSPI1_Reg0x1_TX_FINISH_INT_EN                         2
#define bitSPI1_Reg0x1_TX_FINISH_INT_EN                         0x4
#define set_SPI1_Reg0x1_TX_FINISH_INT_EN(val)                   addSPI1_Reg0x1 = ((addSPI1_Reg0x1 & (~0x4)) | ((val) << 2))
#define setf_SPI1_Reg0x1_TX_FINISH_INT_EN                       addSPI1_Reg0x1 |= 0x4
#define clrf_SPI1_Reg0x1_TX_FINISH_INT_EN                       addSPI1_Reg0x1 &= ~0x4
#define get_SPI1_Reg0x1_TX_FINISH_INT_EN                        ((addSPI1_Reg0x1 & 0x4) >> 2)

#define posSPI1_Reg0x1_RX_EN                                    1
#define bitSPI1_Reg0x1_RX_EN                                    0x2
#define set_SPI1_Reg0x1_RX_EN(val)                              addSPI1_Reg0x1 = ((addSPI1_Reg0x1 & (~0x2)) | ((val) << 1))
#define setf_SPI1_Reg0x1_RX_EN                                  addSPI1_Reg0x1 |= 0x2
#define clrf_SPI1_Reg0x1_RX_EN                                  addSPI1_Reg0x1 &= ~0x2
#define get_SPI1_Reg0x1_RX_EN                                   ((addSPI1_Reg0x1 & 0x2) >> 1)

#define posSPI1_Reg0x1_TX_EN                                    0
#define bitSPI1_Reg0x1_TX_EN                                    0x1
#define set_SPI1_Reg0x1_TX_EN(val)                              addSPI1_Reg0x1 = ((addSPI1_Reg0x1 & (~0x1)) | ((val) << 0))
#define setf_SPI1_Reg0x1_TX_EN                                  addSPI1_Reg0x1 |= 0x1
#define clrf_SPI1_Reg0x1_TX_EN                                  addSPI1_Reg0x1 &= ~0x1
#define get_SPI1_Reg0x1_TX_EN                                   (addSPI1_Reg0x1 & 0x1)

//addSPI1_Reg0x2
#define addSPI1_Reg0x2                                          *((volatile unsigned long *) (0x458C0000+0x2*4))

#define posSPI1_Reg0x2_RXFIFO_CLR                               17
#define bitSPI1_Reg0x2_RXFIFO_CLR                               0x20000
#define set_SPI1_Reg0x2_RXFIFO_CLR(val)                         addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x20000)) | ((val) << 17))
#define setf_SPI1_Reg0x2_RXFIFO_CLR                             addSPI1_Reg0x2 |= 0x20000
#define clrf_SPI1_Reg0x2_RXFIFO_CLR                             addSPI1_Reg0x2 &= ~0x20000
#define get_SPI1_Reg0x2_RXFIFO_CLR                              ((addSPI1_Reg0x2 & 0x20000) >> 17)

#define posSPI1_Reg0x2_TXFIFO_CLR                               16
#define bitSPI1_Reg0x2_TXFIFO_CLR                               0x10000
#define set_SPI1_Reg0x2_TXFIFO_CLR(val)                         addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x10000)) | ((val) << 16))
#define setf_SPI1_Reg0x2_TXFIFO_CLR                             addSPI1_Reg0x2 |= 0x10000
#define clrf_SPI1_Reg0x2_TXFIFO_CLR                             addSPI1_Reg0x2 &= ~0x10000
#define get_SPI1_Reg0x2_TXFIFO_CLR                              ((addSPI1_Reg0x2 & 0x10000) >> 16)


#define posSPI1_Reg0x2_RX_FINISH_INT                            14
#define bitSPI1_Reg0x2_RX_FINISH_INT                            0x4000
#define get_SPI1_Reg0x2_RX_FINISH_INT                           ((addSPI1_Reg0x2 & 0x4000) >> 14)

#define posSPI1_Reg0x2_TX_FINISH_INT                            13
#define bitSPI1_Reg0x2_TX_FINISH_INT                            0x2000
#define set_SPI1_Reg0x2_TX_FINISH_INT(val)                      addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x2000)) | ((val) << 13))
#define setf_SPI1_Reg0x2_TX_FINISH_INT                          addSPI1_Reg0x2 |= 0x2000
#define clrf_SPI1_Reg0x2_TX_FINISH_INT                          addSPI1_Reg0x2 &= ~0x2000
#define get_SPI1_Reg0x2_TX_FINISH_INT                           ((addSPI1_Reg0x2 & 0x2000) >> 13)

#define posSPI1_Reg0x2_RXOVF                                    12
#define bitSPI1_Reg0x2_RXOVF                                    0x1000
#define set_SPI1_Reg0x2_RXOVF(val)                              addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x1000)) | ((val) << 12))
#define setf_SPI1_Reg0x2_RXOVF                                  addSPI1_Reg0x2 |= 0x1000
#define clrf_SPI1_Reg0x2_RXOVF                                  addSPI1_Reg0x2 &= ~0x1000
#define get_SPI1_Reg0x2_RXOVF                                   ((addSPI1_Reg0x2 & 0x1000) >> 12)

#define posSPI1_Reg0x2_TXUDF                                    11
#define bitSPI1_Reg0x2_TXUDF                                    0x800
#define set_SPI1_Reg0x2_TXUDF(val)                              addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x800)) | ((val) << 11))
#define setf_SPI1_Reg0x2_TXUDF                                  addSPI1_Reg0x2 |= 0x800
#define clrf_SPI1_Reg0x2_TXUDF                                  addSPI1_Reg0x2 &= ~0x800
#define get_SPI1_Reg0x2_TXUDF                                   ((addSPI1_Reg0x2 & 0x800) >> 11)

#define posSPI1_Reg0x2_SLV_RELEASE_INT                          10
#define bitSPI1_Reg0x2_SLV_RELEASE_INT                          0x400
#define set_SPI1_Reg0x2_SLV_RELEASE_INT(val)                    addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x400)) | ((val) << 10))
#define setf_SPI1_Reg0x2_SLV_RELEASE_INT                        addSPI1_Reg0x2 |= 0x400
#define clrf_SPI1_Reg0x2_SLV_RELEASE_INT                        addSPI1_Reg0x2 &= ~0x400
#define get_SPI1_Reg0x2_SLV_RELEASE_INT                         ((addSPI1_Reg0x2 & 0x400) >> 10)

#define posSPI1_Reg0x2_RXFIFO_INT                               9
#define bitSPI1_Reg0x2_RXFIFO_INT                               0x200
#define set_SPI1_Reg0x2_RXFIFO_INT(val)                         addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x200)) | ((val) << 9))
#define setf_SPI1_Reg0x2_RXFIFO_INT                             addSPI1_Reg0x2 |= 0x200
#define clrf_SPI1_Reg0x2_RXFIFO_INT                             addSPI1_Reg0x2 &= ~0x200
#define get_SPI1_Reg0x2_RXFIFO_INT                              ((addSPI1_Reg0x2 & 0x200) >> 9)

#define posSPI1_Reg0x2_TXFIFO_INT                               8
#define bitSPI1_Reg0x2_TXFIFO_INT                               0x100
#define set_SPI1_Reg0x2_TXFIFO_INT(val)                         addSPI1_Reg0x2 = ((addSPI1_Reg0x2 & (~0x100)) | ((val) << 8))
#define setf_SPI1_Reg0x2_TXFIFO_INT                             addSPI1_Reg0x2 |= 0x100
#define clrf_SPI1_Reg0x2_TXFIFO_INT                             addSPI1_Reg0x2 &= ~0x100
#define get_SPI1_Reg0x2_TXFIFO_INT                              ((addSPI1_Reg0x2 & 0x100) >> 8)



#define posSPI1_Reg0x2_RXFIFO_RD_READY                          2
#define bitSPI1_Reg0x2_RXFIFO_RD_READY                          0x4
#define get_SPI1_Reg0x2_RXFIFO_RD_READY                         ((addSPI1_Reg0x2 & 0x4) >> 2)

#define posSPI1_Reg0x2_TXFIFO_WR_READY                          1
#define bitSPI1_Reg0x2_TXFIFO_WR_READY                          0x2
#define get_SPI1_Reg0x2_TXFIFO_WR_READY                         ((addSPI1_Reg0x2 & 0x2) >> 1)


//addSPI1_Reg0x3
#define addSPI1_Reg0x3                                          *((volatile unsigned long *) (0x458C0000+0x3*4))

#define posSPI1_Reg0x3_SPI_DAT                                  0
#define bitSPI1_Reg0x3_SPI_DAT                                  0xFFFF
#define set_SPI1_Reg0x3_SPI_DAT(val)                            addSPI1_Reg0x3 = ((addSPI1_Reg0x3 & (~0xFFFF)) | ((val) << 0))
#define get_SPI1_Reg0x3_SPI_DAT                                 (addSPI1_Reg0x3 & 0xFFFF)


//************************************************************//
//I2C0
//************************************************************//
#define BASEADDR_I2C0                                           0x44850000
//addI2C0_Reg0x0
#define addI2C0_Reg0x0                                          *((volatile unsigned long *) (0x44850000+0x0*4))
#define posI2C0_Reg0x0_ENSMB                                    31
#define bitI2C0_Reg0x0_ENSMB                                    0x80000000
#define set_I2C0_Reg0x0_ENSMB(val)                              addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x80000000)) | ((val) << 31))
#define setf_I2C0_Reg0x0_ENSMB                                  addI2C0_Reg0x0 |= 0x80000000
#define clrf_I2C0_Reg0x0_ENSMB                                  addI2C0_Reg0x0 &= ~0x80000000
#define get_I2C0_Reg0x0_ENSMB                                   ((addI2C0_Reg0x0 & 0x80000000) >> 31)

#define posI2C0_Reg0x0_INH                                      30
#define bitI2C0_Reg0x0_INH                                      0x40000000
#define set_I2C0_Reg0x0_INH(val)                                addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x40000000)) | ((val) << 30))
#define setf_I2C0_Reg0x0_INH                                    addI2C0_Reg0x0 |= 0x40000000
#define clrf_I2C0_Reg0x0_INH                                    addI2C0_Reg0x0 &= ~0x40000000
#define get_I2C0_Reg0x0_INH                                     ((addI2C0_Reg0x0 & 0x40000000) >> 30)

#define posI2C0_Reg0x0_SMBFTE                                   29
#define bitI2C0_Reg0x0_SMBFTE                                   0x20000000
#define set_I2C0_Reg0x0_SMBFTE(val)                             addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x20000000)) | ((val) << 29))
#define setf_I2C0_Reg0x0_SMBFTE                                 addI2C0_Reg0x0 |= 0x20000000
#define clrf_I2C0_Reg0x0_SMBFTE                                 addI2C0_Reg0x0 &= ~0x20000000
#define get_I2C0_Reg0x0_SMBFTE                                  ((addI2C0_Reg0x0 & 0x20000000) >> 29)

#define posI2C0_Reg0x0_SMBTOE                                   28
#define bitI2C0_Reg0x0_SMBTOE                                   0x10000000
#define set_I2C0_Reg0x0_SMBTOE(val)                             addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x10000000)) | ((val) << 28))
#define setf_I2C0_Reg0x0_SMBTOE                                 addI2C0_Reg0x0 |= 0x10000000
#define clrf_I2C0_Reg0x0_SMBTOE                                 addI2C0_Reg0x0 &= ~0x10000000
#define get_I2C0_Reg0x0_SMBTOE                                  ((addI2C0_Reg0x0 & 0x10000000) >> 28)

#define posI2C0_Reg0x0_SMBCS                                    26
#define bitI2C0_Reg0x0_SMBCS                                    0xC000000
#define set_I2C0_Reg0x0_SMBCS(val)                              addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0xC000000)) | ((val) << 26))
#define get_I2C0_Reg0x0_SMBCS                                   ((addI2C0_Reg0x0 & 0xC000000) >> 26)

#define posI2C0_Reg0x0_SLV_ADDR                                 16
#define bitI2C0_Reg0x0_SLV_ADDR                                 0x3FF0000
#define set_I2C0_Reg0x0_SLV_ADDR(val)                           addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x3FF0000)) | ((val) << 16))
#define get_I2C0_Reg0x0_SLV_ADDR                                ((addI2C0_Reg0x0 & 0x3FF0000) >> 16)

#define posI2C0_Reg0x0_FREQ_DIV                                 6
#define bitI2C0_Reg0x0_FREQ_DIV                                 0xFFC0
#define set_I2C0_Reg0x0_FREQ_DIV(val)                           addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0xFFC0)) | ((val) << 6))
#define get_I2C0_Reg0x0_FREQ_DIV                                ((addI2C0_Reg0x0 & 0xFFC0) >> 6)

#define posI2C0_Reg0x0_SCL_CR                                   3
#define bitI2C0_Reg0x0_SCL_CR                                   0x38
#define set_I2C0_Reg0x0_SCL_CR(val)                             addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x38)) | ((val) << 3))
#define get_I2C0_Reg0x0_SCL_CR                                  ((addI2C0_Reg0x0 & 0x38) >> 3)

#define posI2C0_Reg0x0_IDLE_CR                                  0
#define bitI2C0_Reg0x0_IDLE_CR                                  0x7
#define set_I2C0_Reg0x0_IDLE_CR(val)                            addI2C0_Reg0x0 = ((addI2C0_Reg0x0 & (~0x7)) | ((val) << 0))
#define get_I2C0_Reg0x0_IDLE_CR                                 (addI2C0_Reg0x0 & 0x7)

//addI2C0_Reg0x1
#define addI2C0_Reg0x1                                          *((volatile unsigned long *) (0x44850000+0x1*4))

#define posI2C0_Reg0x1_BUSY                                     15
#define bitI2C0_Reg0x1_BUSY                                     0x8000
#define get_I2C0_Reg0x1_BUSY                                    ((addI2C0_Reg0x1 & 0x8000) >> 15)

#define posI2C0_Reg0x1_MASTER                                   14
#define bitI2C0_Reg0x1_MASTER                                   0x4000
#define get_I2C0_Reg0x1_MASTER                                  ((addI2C0_Reg0x1 & 0x4000) >> 14)

#define posI2C0_Reg0x1_TXMODE                                   13
#define bitI2C0_Reg0x1_TXMODE                                   0x2000
#define get_I2C0_Reg0x1_TXMODE                                  ((addI2C0_Reg0x1 & 0x2000) >> 13)

#define posI2C0_Reg0x1_ACKRQ                                    12
#define bitI2C0_Reg0x1_ACKRQ                                    0x1000
#define get_I2C0_Reg0x1_ACKRQ                                   ((addI2C0_Reg0x1 & 0x1000) >> 12)

#define posI2C0_Reg0x1_ADDR_MATCH                               11
#define bitI2C0_Reg0x1_ADDR_MATCH                               0x800
#define get_I2C0_Reg0x1_ADDR_MATCH                              ((addI2C0_Reg0x1 & 0x800) >> 11)

#define posI2C0_Reg0x1_STA                                      10
#define bitI2C0_Reg0x1_STA                                      0x400
#define set_I2C0_Reg0x1_STA(val)                                addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0x400)) | ((val) << 10))
#define setf_I2C0_Reg0x1_STA                                    addI2C0_Reg0x1 |= 0x400
#define clrf_I2C0_Reg0x1_STA                                    addI2C0_Reg0x1 &= ~0x400
#define get_I2C0_Reg0x1_STA                                     ((addI2C0_Reg0x1 & 0x400) >> 10)

#define posI2C0_Reg0x1_STO                                      9
#define bitI2C0_Reg0x1_STO                                      0x200
#define set_I2C0_Reg0x1_STO(val)                                addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0x200)) | ((val) << 9))
#define setf_I2C0_Reg0x1_STO                                    addI2C0_Reg0x1 |= 0x200
#define clrf_I2C0_Reg0x1_STO                                    addI2C0_Reg0x1 &= ~0x200
#define get_I2C0_Reg0x1_STO                                     ((addI2C0_Reg0x1 & 0x200) >> 9)

#define posI2C0_Reg0x1_ACK                                      8
#define bitI2C0_Reg0x1_ACK                                      0x100
#define set_I2C0_Reg0x1_ACK(val)                                addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0x100)) | ((val) << 8))
#define setf_I2C0_Reg0x1_ACK                                    addI2C0_Reg0x1 |= 0x100
#define clrf_I2C0_Reg0x1_ACK                                    addI2C0_Reg0x1 &= ~0x100
#define get_I2C0_Reg0x1_ACK                                     ((addI2C0_Reg0x1 & 0x100) >> 8)

#define posI2C0_Reg0x1_INT_MODE                                 6
#define bitI2C0_Reg0x1_INT_MODE                                 0xC0
#define set_I2C0_Reg0x1_INT_MODE(val)                           addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0xC0)) | ((val) << 6))
#define get_I2C0_Reg0x1_INT_MODE                                ((addI2C0_Reg0x1 & 0xC0) >> 6)

#define posI2C0_Reg0x1_TXFIFO_FULL                              5
#define bitI2C0_Reg0x1_TXFIFO_FULL                              0x20
#define get_I2C0_Reg0x1_TXFIFO_FULL                             ((addI2C0_Reg0x1 & 0x20) >> 5)

#define posI2C0_Reg0x1_RXFIFO_EMPTY                             4
#define bitI2C0_Reg0x1_RXFIFO_EMPTY                             0x10
#define get_I2C0_Reg0x1_RXFIFO_EMPTY                            ((addI2C0_Reg0x1 & 0x10) >> 4)

#define posI2C0_Reg0x1_ARBLOST                                  3
#define bitI2C0_Reg0x1_ARBLOST                                  0x8
#define set_I2C0_Reg0x1_ARBLOST(val)                            addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0x8)) | ((val) << 3))
#define setf_I2C0_Reg0x1_ARBLOST                                addI2C0_Reg0x1 |= 0x8
#define clrf_I2C0_Reg0x1_ARBLOST                                addI2C0_Reg0x1 &= ~0x8
#define get_I2C0_Reg0x1_ARBLOST                                 ((addI2C0_Reg0x1 & 0x8) >> 3)


#define posI2C0_Reg0x1_SCL_TMOT                                 1
#define bitI2C0_Reg0x1_SCL_TMOT                                 0x2
#define set_I2C0_Reg0x1_SCL_TMOT(val)                           addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0x2)) | ((val) << 1))
#define setf_I2C0_Reg0x1_SCL_TMOT                               addI2C0_Reg0x1 |= 0x2
#define clrf_I2C0_Reg0x1_SCL_TMOT                               addI2C0_Reg0x1 &= ~0x2
#define get_I2C0_Reg0x1_SCL_TMOT                                ((addI2C0_Reg0x1 & 0x2) >> 1)

#define posI2C0_Reg0x1_SI                                       0
#define bitI2C0_Reg0x1_SI                                       0x1
#define set_I2C0_Reg0x1_SI(val)                                 addI2C0_Reg0x1 = ((addI2C0_Reg0x1 & (~0x1)) | ((val) << 0))
#define setf_I2C0_Reg0x1_SI                                     addI2C0_Reg0x1 |= 0x1
#define clrf_I2C0_Reg0x1_SI                                     addI2C0_Reg0x1 &= ~0x1
#define get_I2C0_Reg0x1_SI                                      (addI2C0_Reg0x1 & 0x1)

//addI2C0_Reg0x2
#define addI2C0_Reg0x2                                          *((volatile unsigned long *) (0x44850000+0x2*4))

#define posI2C0_Reg0x2_SMB_DAT                                  0
#define bitI2C0_Reg0x2_SMB_DAT                                  0xFF
#define set_I2C0_Reg0x2_SMB_DAT(val)                            addI2C0_Reg0x2 = ((addI2C0_Reg0x2 & (~0xFF)) | ((val) << 0))
#define get_I2C0_Reg0x2_SMB_DAT                                 (addI2C0_Reg0x2 & 0xFF)


//************************************************************//
//I2C1
//************************************************************//
#define BASEADDR_I2C1                                           0x45890000
//addI2C1_Reg0x0
#define addI2C1_Reg0x0                                          *((volatile unsigned long *) (0x45890000+0x0*4))
#define posI2C1_Reg0x0_ENSMB                                    31
#define bitI2C1_Reg0x0_ENSMB                                    0x80000000
#define set_I2C1_Reg0x0_ENSMB(val)                              addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x80000000)) | ((val) << 31))
#define setf_I2C1_Reg0x0_ENSMB                                  addI2C1_Reg0x0 |= 0x80000000
#define clrf_I2C1_Reg0x0_ENSMB                                  addI2C1_Reg0x0 &= ~0x80000000
#define get_I2C1_Reg0x0_ENSMB                                   ((addI2C1_Reg0x0 & 0x80000000) >> 31)

#define posI2C1_Reg0x0_INH                                      30
#define bitI2C1_Reg0x0_INH                                      0x40000000
#define set_I2C1_Reg0x0_INH(val)                                addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x40000000)) | ((val) << 30))
#define setf_I2C1_Reg0x0_INH                                    addI2C1_Reg0x0 |= 0x40000000
#define clrf_I2C1_Reg0x0_INH                                    addI2C1_Reg0x0 &= ~0x40000000
#define get_I2C1_Reg0x0_INH                                     ((addI2C1_Reg0x0 & 0x40000000) >> 30)

#define posI2C1_Reg0x0_SMBFTE                                   29
#define bitI2C1_Reg0x0_SMBFTE                                   0x20000000
#define set_I2C1_Reg0x0_SMBFTE(val)                             addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x20000000)) | ((val) << 29))
#define setf_I2C1_Reg0x0_SMBFTE                                 addI2C1_Reg0x0 |= 0x20000000
#define clrf_I2C1_Reg0x0_SMBFTE                                 addI2C1_Reg0x0 &= ~0x20000000
#define get_I2C1_Reg0x0_SMBFTE                                  ((addI2C1_Reg0x0 & 0x20000000) >> 29)

#define posI2C1_Reg0x0_SMBTOE                                   28
#define bitI2C1_Reg0x0_SMBTOE                                   0x10000000
#define set_I2C1_Reg0x0_SMBTOE(val)                             addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x10000000)) | ((val) << 28))
#define setf_I2C1_Reg0x0_SMBTOE                                 addI2C1_Reg0x0 |= 0x10000000
#define clrf_I2C1_Reg0x0_SMBTOE                                 addI2C1_Reg0x0 &= ~0x10000000
#define get_I2C1_Reg0x0_SMBTOE                                  ((addI2C1_Reg0x0 & 0x10000000) >> 28)

#define posI2C1_Reg0x0_SMBCS                                    26
#define bitI2C1_Reg0x0_SMBCS                                    0xC000000
#define set_I2C1_Reg0x0_SMBCS(val)                              addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0xC000000)) | ((val) << 26))
#define get_I2C1_Reg0x0_SMBCS                                   ((addI2C1_Reg0x0 & 0xC000000) >> 26)

#define posI2C1_Reg0x0_SLV_ADDR                                 16
#define bitI2C1_Reg0x0_SLV_ADDR                                 0x3FF0000
#define set_I2C1_Reg0x0_SLV_ADDR(val)                           addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x3FF0000)) | ((val) << 16))
#define get_I2C1_Reg0x0_SLV_ADDR                                ((addI2C1_Reg0x0 & 0x3FF0000) >> 16)

#define posI2C1_Reg0x0_FREQ_DIV                                 6
#define bitI2C1_Reg0x0_FREQ_DIV                                 0xFFC0
#define set_I2C1_Reg0x0_FREQ_DIV(val)                           addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0xFFC0)) | ((val) << 6))
#define get_I2C1_Reg0x0_FREQ_DIV                                ((addI2C1_Reg0x0 & 0xFFC0) >> 6)

#define posI2C1_Reg0x0_SCL_CR                                   3
#define bitI2C1_Reg0x0_SCL_CR                                   0x38
#define set_I2C1_Reg0x0_SCL_CR(val)                             addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x38)) | ((val) << 3))
#define get_I2C1_Reg0x0_SCL_CR                                  ((addI2C1_Reg0x0 & 0x38) >> 3)

#define posI2C1_Reg0x0_IDLE_CR                                  0
#define bitI2C1_Reg0x0_IDLE_CR                                  0x7
#define set_I2C1_Reg0x0_IDLE_CR(val)                            addI2C1_Reg0x0 = ((addI2C1_Reg0x0 & (~0x7)) | ((val) << 0))
#define get_I2C1_Reg0x0_IDLE_CR                                 (addI2C1_Reg0x0 & 0x7)

//addI2C1_Reg0x1
#define addI2C1_Reg0x1                                          *((volatile unsigned long *) (0x45890000+0x1*4))

#define posI2C1_Reg0x1_BUSY                                     15
#define bitI2C1_Reg0x1_BUSY                                     0x8000
#define get_I2C1_Reg0x1_BUSY                                    ((addI2C1_Reg0x1 & 0x8000) >> 15)

#define posI2C1_Reg0x1_MASTER                                   14
#define bitI2C1_Reg0x1_MASTER                                   0x4000
#define get_I2C1_Reg0x1_MASTER                                  ((addI2C1_Reg0x1 & 0x4000) >> 14)

#define posI2C1_Reg0x1_TXMODE                                   13
#define bitI2C1_Reg0x1_TXMODE                                   0x2000
#define get_I2C1_Reg0x1_TXMODE                                  ((addI2C1_Reg0x1 & 0x2000) >> 13)

#define posI2C1_Reg0x1_ACKRQ                                    12
#define bitI2C1_Reg0x1_ACKRQ                                    0x1000
#define get_I2C1_Reg0x1_ACKRQ                                   ((addI2C1_Reg0x1 & 0x1000) >> 12)

#define posI2C1_Reg0x1_ADDR_MATCH                               11
#define bitI2C1_Reg0x1_ADDR_MATCH                               0x800
#define get_I2C1_Reg0x1_ADDR_MATCH                              ((addI2C1_Reg0x1 & 0x800) >> 11)

#define posI2C1_Reg0x1_STA                                      10
#define bitI2C1_Reg0x1_STA                                      0x400
#define set_I2C1_Reg0x1_STA(val)                                addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0x400)) | ((val) << 10))
#define setf_I2C1_Reg0x1_STA                                    addI2C1_Reg0x1 |= 0x400
#define clrf_I2C1_Reg0x1_STA                                    addI2C1_Reg0x1 &= ~0x400
#define get_I2C1_Reg0x1_STA                                     ((addI2C1_Reg0x1 & 0x400) >> 10)

#define posI2C1_Reg0x1_STO                                      9
#define bitI2C1_Reg0x1_STO                                      0x200
#define set_I2C1_Reg0x1_STO(val)                                addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0x200)) | ((val) << 9))
#define setf_I2C1_Reg0x1_STO                                    addI2C1_Reg0x1 |= 0x200
#define clrf_I2C1_Reg0x1_STO                                    addI2C1_Reg0x1 &= ~0x200
#define get_I2C1_Reg0x1_STO                                     ((addI2C1_Reg0x1 & 0x200) >> 9)

#define posI2C1_Reg0x1_ACK                                      8
#define bitI2C1_Reg0x1_ACK                                      0x100
#define set_I2C1_Reg0x1_ACK(val)                                addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0x100)) | ((val) << 8))
#define setf_I2C1_Reg0x1_ACK                                    addI2C1_Reg0x1 |= 0x100
#define clrf_I2C1_Reg0x1_ACK                                    addI2C1_Reg0x1 &= ~0x100
#define get_I2C1_Reg0x1_ACK                                     ((addI2C1_Reg0x1 & 0x100) >> 8)

#define posI2C1_Reg0x1_INT_MODE                                 6
#define bitI2C1_Reg0x1_INT_MODE                                 0xC0
#define set_I2C1_Reg0x1_INT_MODE(val)                           addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0xC0)) | ((val) << 6))
#define get_I2C1_Reg0x1_INT_MODE                                ((addI2C1_Reg0x1 & 0xC0) >> 6)

#define posI2C1_Reg0x1_TXFIFO_FULL                              5
#define bitI2C1_Reg0x1_TXFIFO_FULL                              0x20
#define get_I2C1_Reg0x1_TXFIFO_FULL                             ((addI2C1_Reg0x1 & 0x20) >> 5)

#define posI2C1_Reg0x1_RXFIFO_EMPTY                             4
#define bitI2C1_Reg0x1_RXFIFO_EMPTY                             0x10
#define get_I2C1_Reg0x1_RXFIFO_EMPTY                            ((addI2C1_Reg0x1 & 0x10) >> 4)

#define posI2C1_Reg0x1_ARBLOST                                  3
#define bitI2C1_Reg0x1_ARBLOST                                  0x8
#define set_I2C1_Reg0x1_ARBLOST(val)                            addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0x8)) | ((val) << 3))
#define setf_I2C1_Reg0x1_ARBLOST                                addI2C1_Reg0x1 |= 0x8
#define clrf_I2C1_Reg0x1_ARBLOST                                addI2C1_Reg0x1 &= ~0x8
#define get_I2C1_Reg0x1_ARBLOST                                 ((addI2C1_Reg0x1 & 0x8) >> 3)


#define posI2C1_Reg0x1_SCL_TMOT                                 1
#define bitI2C1_Reg0x1_SCL_TMOT                                 0x2
#define set_I2C1_Reg0x1_SCL_TMOT(val)                           addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0x2)) | ((val) << 1))
#define setf_I2C1_Reg0x1_SCL_TMOT                               addI2C1_Reg0x1 |= 0x2
#define clrf_I2C1_Reg0x1_SCL_TMOT                               addI2C1_Reg0x1 &= ~0x2
#define get_I2C1_Reg0x1_SCL_TMOT                                ((addI2C1_Reg0x1 & 0x2) >> 1)

#define posI2C1_Reg0x1_SI                                       0
#define bitI2C1_Reg0x1_SI                                       0x1
#define set_I2C1_Reg0x1_SI(val)                                 addI2C1_Reg0x1 = ((addI2C1_Reg0x1 & (~0x1)) | ((val) << 0))
#define setf_I2C1_Reg0x1_SI                                     addI2C1_Reg0x1 |= 0x1
#define clrf_I2C1_Reg0x1_SI                                     addI2C1_Reg0x1 &= ~0x1
#define get_I2C1_Reg0x1_SI                                      (addI2C1_Reg0x1 & 0x1)

//addI2C1_Reg0x2
#define addI2C1_Reg0x2                                          *((volatile unsigned long *) (0x45890000+0x2*4))

#define posI2C1_Reg0x2_SMB_DAT                                  0
#define bitI2C1_Reg0x2_SMB_DAT                                  0xFF
#define set_I2C1_Reg0x2_SMB_DAT(val)                            addI2C1_Reg0x2 = ((addI2C1_Reg0x2 & (~0xFF)) | ((val) << 0))
#define get_I2C1_Reg0x2_SMB_DAT                                 (addI2C1_Reg0x2 & 0xFF)


//************************************************************//
//TIMER0
//************************************************************//
#define BASEADDR_TIMER0                                         0x44810000
//addTIMER0_Reg0x0
#define addTIMER0_Reg0x0                                        *((volatile unsigned long *) (0x44810000+0x0*4))

//addTIMER0_Reg0x1
#define addTIMER0_Reg0x1                                        *((volatile unsigned long *) (0x44810000+0x1*4))

//addTIMER0_Reg0x2
#define addTIMER0_Reg0x2                                        *((volatile unsigned long *) (0x44810000+0x2*4))

//addTIMER0_Reg0x3
#define addTIMER0_Reg0x3                                        *((volatile unsigned long *) (0x44810000+0x3*4))
#define posTIMER0_Reg0x3_timer0_en                              0
#define bitTIMER0_Reg0x3_timer0_en                              0x1
#define set_TIMER0_Reg0x3_timer0_en(val)                        addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x1)) | ((val) << 0))
#define setf_TIMER0_Reg0x3_timer0_en                            addTIMER0_Reg0x3 |= 0x1
#define clrf_TIMER0_Reg0x3_timer0_en                            addTIMER0_Reg0x3 &= ~0x1
#define get_TIMER0_Reg0x3_timer0_en                             (addTIMER0_Reg0x3 & 0x1)

#define posTIMER0_Reg0x3_timer1_en                              1
#define bitTIMER0_Reg0x3_timer1_en                              0x2
#define set_TIMER0_Reg0x3_timer1_en(val)                        addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x2)) | ((val) << 1))
#define setf_TIMER0_Reg0x3_timer1_en                            addTIMER0_Reg0x3 |= 0x2
#define clrf_TIMER0_Reg0x3_timer1_en                            addTIMER0_Reg0x3 &= ~0x2
#define get_TIMER0_Reg0x3_timer1_en                             ((addTIMER0_Reg0x3 & 0x2) >> 1)

#define posTIMER0_Reg0x3_timer2_en                              2
#define bitTIMER0_Reg0x3_timer2_en                              0x4
#define set_TIMER0_Reg0x3_timer2_en(val)                        addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x4)) | ((val) << 2))
#define setf_TIMER0_Reg0x3_timer2_en                            addTIMER0_Reg0x3 |= 0x4
#define clrf_TIMER0_Reg0x3_timer2_en                            addTIMER0_Reg0x3 &= ~0x4
#define get_TIMER0_Reg0x3_timer2_en                             ((addTIMER0_Reg0x3 & 0x4) >> 2)

#define posTIMER0_Reg0x3_clk_div                                3
#define bitTIMER0_Reg0x3_clk_div                                0x78
#define set_TIMER0_Reg0x3_clk_div(val)                          addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x78)) | ((val) << 3))
#define get_TIMER0_Reg0x3_clk_div                               ((addTIMER0_Reg0x3 & 0x78) >> 3)

#define posTIMER0_Reg0x3_timer0_int                             7
#define bitTIMER0_Reg0x3_timer0_int                             0x80
#define set_TIMER0_Reg0x3_timer0_int(val)                       addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x80)) | ((val) << 7))
#define setf_TIMER0_Reg0x3_timer0_int                           addTIMER0_Reg0x3 |= 0x80
#define clrf_TIMER0_Reg0x3_timer0_int                           addTIMER0_Reg0x3 &= ~0x80
#define get_TIMER0_Reg0x3_timer0_int                            ((addTIMER0_Reg0x3 & 0x80) >> 7)

#define posTIMER0_Reg0x3_timer1_int                             8
#define bitTIMER0_Reg0x3_timer1_int                             0x100
#define set_TIMER0_Reg0x3_timer1_int(val)                       addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x100)) | ((val) << 8))
#define setf_TIMER0_Reg0x3_timer1_int                           addTIMER0_Reg0x3 |= 0x100
#define clrf_TIMER0_Reg0x3_timer1_int                           addTIMER0_Reg0x3 &= ~0x100
#define get_TIMER0_Reg0x3_timer1_int                            ((addTIMER0_Reg0x3 & 0x100) >> 8)

#define posTIMER0_Reg0x3_timer2_int                             9
#define bitTIMER0_Reg0x3_timer2_int                             0x200
#define set_TIMER0_Reg0x3_timer2_int(val)                       addTIMER0_Reg0x3 = ((addTIMER0_Reg0x3 & (~0x200)) | ((val) << 9))
#define setf_TIMER0_Reg0x3_timer2_int                           addTIMER0_Reg0x3 |= 0x200
#define clrf_TIMER0_Reg0x3_timer2_int                           addTIMER0_Reg0x3 &= ~0x200
#define get_TIMER0_Reg0x3_timer2_int                            ((addTIMER0_Reg0x3 & 0x200) >> 9)

//addTIMER0_Reg0x4
#define addTIMER0_Reg0x4                                        *((volatile unsigned long *) (0x44810000+0x4*4))

#define posTIMER0_Reg0x4_timerl_index                           2
#define bitTIMER0_Reg0x4_timerl_index                           0xC
#define set_TIMER0_Reg0x4_timerl_index(val)                     addTIMER0_Reg0x4 = ((addTIMER0_Reg0x4 & (~0xC)) | ((val) << 2))
#define get_TIMER0_Reg0x4_timerl_index                          ((addTIMER0_Reg0x4 & 0xC) >> 2)


#define posTIMER0_Reg0x4_timerl_cnt_read                        0
#define bitTIMER0_Reg0x4_timerl_cnt_read                        0x1
#define set_TIMER0_Reg0x4_timerl_cnt_read(val)                  addTIMER0_Reg0x4 = ((addTIMER0_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_TIMER0_Reg0x4_timerl_cnt_read                      addTIMER0_Reg0x4 |= 0x1
#define clrf_TIMER0_Reg0x4_timerl_cnt_read                      addTIMER0_Reg0x4 &= ~0x1
#define get_TIMER0_Reg0x4_timerl_cnt_read                       (addTIMER0_Reg0x4 & 0x1)

//addTIMER0_Reg0x5
#define addTIMER0_Reg0x5                                        *((volatile unsigned long *) (0x44810000+0x5*4))


//************************************************************//
//TIMER1
//************************************************************//
#define BASEADDR_TIMER1                                         0x45800000
//addTIMER1_Reg0x0
#define addTIMER1_Reg0x0                                        *((volatile unsigned long *) (0x44810000+0x0*4))

//addTIMER1_Reg0x1
#define addTIMER1_Reg0x1                                        *((volatile unsigned long *) (0x44810000+0x1*4))

//addTIMER1_Reg0x2
#define addTIMER1_Reg0x2                                        *((volatile unsigned long *) (0x44810000+0x2*4))

//addTIMER1_Reg0x3
#define addTIMER1_Reg0x3                                        *((volatile unsigned long *) (0x44810000+0x3*4))
#define posTIMER1_Reg0x3_timer0_en                              0
#define bitTIMER1_Reg0x3_timer0_en                              0x1
#define set_TIMER1_Reg0x3_timer0_en(val)                        addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x1)) | ((val) << 0))
#define setf_TIMER1_Reg0x3_timer0_en                            addTIMER1_Reg0x3 |= 0x1
#define clrf_TIMER1_Reg0x3_timer0_en                            addTIMER1_Reg0x3 &= ~0x1
#define get_TIMER1_Reg0x3_timer0_en                             (addTIMER1_Reg0x3 & 0x1)

#define posTIMER1_Reg0x3_timer1_en                              1
#define bitTIMER1_Reg0x3_timer1_en                              0x2
#define set_TIMER1_Reg0x3_timer1_en(val)                        addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x2)) | ((val) << 1))
#define setf_TIMER1_Reg0x3_timer1_en                            addTIMER1_Reg0x3 |= 0x2
#define clrf_TIMER1_Reg0x3_timer1_en                            addTIMER1_Reg0x3 &= ~0x2
#define get_TIMER1_Reg0x3_timer1_en                             ((addTIMER1_Reg0x3 & 0x2) >> 1)

#define posTIMER1_Reg0x3_timer2_en                              2
#define bitTIMER1_Reg0x3_timer2_en                              0x4
#define set_TIMER1_Reg0x3_timer2_en(val)                        addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x4)) | ((val) << 2))
#define setf_TIMER1_Reg0x3_timer2_en                            addTIMER1_Reg0x3 |= 0x4
#define clrf_TIMER1_Reg0x3_timer2_en                            addTIMER1_Reg0x3 &= ~0x4
#define get_TIMER1_Reg0x3_timer2_en                             ((addTIMER1_Reg0x3 & 0x4) >> 2)

#define posTIMER1_Reg0x3_clk_div                                3
#define bitTIMER1_Reg0x3_clk_div                                0x78
#define set_TIMER1_Reg0x3_clk_div(val)                          addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x78)) | ((val) << 3))
#define get_TIMER1_Reg0x3_clk_div                               ((addTIMER1_Reg0x3 & 0x78) >> 3)

#define posTIMER1_Reg0x3_timer0_int                             7
#define bitTIMER1_Reg0x3_timer0_int                             0x80
#define set_TIMER1_Reg0x3_timer0_int(val)                       addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x80)) | ((val) << 7))
#define setf_TIMER1_Reg0x3_timer0_int                           addTIMER1_Reg0x3 |= 0x80
#define clrf_TIMER1_Reg0x3_timer0_int                           addTIMER1_Reg0x3 &= ~0x80
#define get_TIMER1_Reg0x3_timer0_int                            ((addTIMER1_Reg0x3 & 0x80) >> 7)

#define posTIMER1_Reg0x3_timer1_int                             8
#define bitTIMER1_Reg0x3_timer1_int                             0x100
#define set_TIMER1_Reg0x3_timer1_int(val)                       addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x100)) | ((val) << 8))
#define setf_TIMER1_Reg0x3_timer1_int                           addTIMER1_Reg0x3 |= 0x100
#define clrf_TIMER1_Reg0x3_timer1_int                           addTIMER1_Reg0x3 &= ~0x100
#define get_TIMER1_Reg0x3_timer1_int                            ((addTIMER1_Reg0x3 & 0x100) >> 8)

#define posTIMER1_Reg0x3_timer2_int                             9
#define bitTIMER1_Reg0x3_timer2_int                             0x200
#define set_TIMER1_Reg0x3_timer2_int(val)                       addTIMER1_Reg0x3 = ((addTIMER1_Reg0x3 & (~0x200)) | ((val) << 9))
#define setf_TIMER1_Reg0x3_timer2_int                           addTIMER1_Reg0x3 |= 0x200
#define clrf_TIMER1_Reg0x3_timer2_int                           addTIMER1_Reg0x3 &= ~0x200
#define get_TIMER1_Reg0x3_timer2_int                            ((addTIMER1_Reg0x3 & 0x200) >> 9)

//addTIMER1_Reg0x4
#define addTIMER1_Reg0x4                                        *((volatile unsigned long *) (0x44810000+0x4*4))

#define posTIMER1_Reg0x4_timerl_index                           2
#define bitTIMER1_Reg0x4_timerl_index                           0xC
#define set_TIMER1_Reg0x4_timerl_index(val)                     addTIMER1_Reg0x4 = ((addTIMER1_Reg0x4 & (~0xC)) | ((val) << 2))
#define get_TIMER1_Reg0x4_timerl_index                          ((addTIMER1_Reg0x4 & 0xC) >> 2)


#define posTIMER1_Reg0x4_timerl_cnt_read                        0
#define bitTIMER1_Reg0x4_timerl_cnt_read                        0x1
#define set_TIMER1_Reg0x4_timerl_cnt_read(val)                  addTIMER1_Reg0x4 = ((addTIMER1_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_TIMER1_Reg0x4_timerl_cnt_read                      addTIMER1_Reg0x4 |= 0x1
#define clrf_TIMER1_Reg0x4_timerl_cnt_read                      addTIMER1_Reg0x4 &= ~0x1
#define get_TIMER1_Reg0x4_timerl_cnt_read                       (addTIMER1_Reg0x4 & 0x1)

//addTIMER1_Reg0x5
#define addTIMER1_Reg0x5                                        *((volatile unsigned long *) (0x44810000+0x5*4))


//************************************************************//
//SADC
//************************************************************//
#define BASEADDR_SADC                                           0x44870000
//addSADC_Reg0x0
#define addSADC_Reg0x0                                          *((volatile unsigned long *) (BASEADDR_SADC+0x0*4))
#define posSADC_Reg0x0_ADC_MODE                                 0
#define bitSADC_Reg0x0_ADC_MODE                                 0x3
#define set_SADC_Reg0x0_ADC_MODE(val)                           addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x3)) | ((val) << 0))
#define get_SADC_Reg0x0_ADC_MODE                                (addSADC_Reg0x0 & 0x3)

#define posSADC_Reg0x0_ADC_EN                                   2
#define bitSADC_Reg0x0_ADC_EN                                   0x4
#define set_SADC_Reg0x0_ADC_EN(val)                             addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_SADC_Reg0x0_ADC_EN                                 addSADC_Reg0x0 |= 0x4
#define clrf_SADC_Reg0x0_ADC_EN                                 addSADC_Reg0x0 &= ~0x4
#define get_SADC_Reg0x0_ADC_EN                                  ((addSADC_Reg0x0 & 0x4) >> 2)

#define posSADC_Reg0x0_ADC_CHNL                                 3
#define bitSADC_Reg0x0_ADC_CHNL                                 0x78
#define set_SADC_Reg0x0_ADC_CHNL(val)                           addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x78)) | ((val) << 3))
#define get_SADC_Reg0x0_ADC_CHNL                                ((addSADC_Reg0x0 & 0x78) >> 3)

#define posSADC_Reg0x0_adc_setting                              7
#define bitSADC_Reg0x0_adc_setting                              0x80
#define set_SADC_Reg0x0_adc_setting(val)                        addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x80)) | ((val) << 7))
#define setf_SADC_Reg0x0_adc_setting                            addSADC_Reg0x0 |= 0x80
#define clrf_SADC_Reg0x0_adc_setting                            addSADC_Reg0x0 &= ~0x80
#define get_SADC_Reg0x0_adc_setting                             ((addSADC_Reg0x0 & 0x80) >> 7)

#define posSADC_Reg0x0_adc_int_clear                            8
#define bitSADC_Reg0x0_adc_int_clear                            0x100
#define set_SADC_Reg0x0_adc_int_clear(val)                      addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x100)) | ((val) << 8))
#define setf_SADC_Reg0x0_adc_int_clear                          addSADC_Reg0x0 |= 0x100
#define clrf_SADC_Reg0x0_adc_int_clear                          addSADC_Reg0x0 &= ~0x100
#define get_SADC_Reg0x0_adc_int_clear                           ((addSADC_Reg0x0 & 0x100) >> 8)

#define posSADC_Reg0x0_pre_div                                  9
#define bitSADC_Reg0x0_pre_div                                  0x7E00
#define set_SADC_Reg0x0_pre_div(val)                            addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x7E00)) | ((val) << 9))
#define get_SADC_Reg0x0_pre_div                                 ((addSADC_Reg0x0 & 0x7E00) >> 9)

#define posSADC_Reg0x0_32m_mode                                 15
#define bitSADC_Reg0x0_32m_mode                                 0x8000
#define set_SADC_Reg0x0_32m_mode(val)                           addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x8000)) | ((val) << 15))
#define setf_SADC_Reg0x0_32m_mode                               addSADC_Reg0x0 |= 0x8000
#define clrf_SADC_Reg0x0_32m_mode                               addSADC_Reg0x0 &= ~0x8000
#define get_SADC_Reg0x0_32m_mode                                ((addSADC_Reg0x0 & 0x8000) >> 15)

#define posSADC_Reg0x0_samp_rate                                16
#define bitSADC_Reg0x0_samp_rate                                0x3F0000
#define set_SADC_Reg0x0_samp_rate(val)                          addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x3F0000)) | ((val) << 16))
#define get_SADC_Reg0x0_samp_rate                               ((addSADC_Reg0x0 & 0x3F0000) >> 16)

#define posSADC_Reg0x0_adc_filter                               22
#define bitSADC_Reg0x0_adc_filter                               0x1FC00000
#define set_SADC_Reg0x0_adc_filter(val)                         addSADC_Reg0x0 = ((addSADC_Reg0x0 & (~0x1FC00000)) | ((val) << 22))
#define get_SADC_Reg0x0_adc_filter                              ((addSADC_Reg0x0 & 0x1FC00000) >> 22)

#define posSADC_Reg0x0_adc_busy                                 29
#define bitSADC_Reg0x0_adc_busy                                 0x20000000
#define get_SADC_Reg0x0_adc_busy                                ((addSADC_Reg0x0 & 0x20000000) >> 29)

#define posSADC_Reg0x0_fifo_empty                               30
#define bitSADC_Reg0x0_fifo_empty                               0x40000000
#define get_SADC_Reg0x0_fifo_empty                              ((addSADC_Reg0x0 & 0x40000000) >> 30)

#define posSADC_Reg0x0_fifo_full                                31
#define bitSADC_Reg0x0_fifo_full                                0x80000000
#define get_SADC_Reg0x0_fifo_full                               ((addSADC_Reg0x0 & 0x80000000) >> 31)

//addSADC_Reg0x1
#define addSADC_Reg0x1                                          *((volatile unsigned long *) (BASEADDR_SADC+0x1*4))

//addSADC_Reg0x2
#define addSADC_Reg0x2                                          *((volatile unsigned long *) (BASEADDR_SADC+0x2*4))
#define posSADC_Reg0x2_channel_expand                           16
#define bitSADC_Reg0x2_channel_expand                           0x10000
#define set_SADC_Reg0x2_channel_expand(val)                     addSADC_Reg0x2 = ((addSADC_Reg0x2 & (~0x10000)) | ((val) << 16))
#define setf_SADC_Reg0x2_channel_expand                         addSADC_Reg0x2 |= 0x10000
#define clrf_SADC_Reg0x2_channel_expand                         addSADC_Reg0x2 &= ~0x10000
#define get_SADC_Reg0x2_channel_expand                          ((addSADC_Reg0x2 & 0x10000) >> 16)

#define posSADC_Reg0x2_steady_ctrl                              5
#define bitSADC_Reg0x2_steady_ctrl                              0xE0
#define set_SADC_Reg0x2_steady_ctrl(val)                        addSADC_Reg0x2 = ((addSADC_Reg0x2 & (~0xE0)) | ((val) << 5))
#define get_SADC_Reg0x2_steady_ctrl                             ((addSADC_Reg0x2 & 0xE0) >> 5)

#define posSADC_Reg0x2_Almost_cfg                               0
#define bitSADC_Reg0x2_Almost_cfg                               0x1F
#define set_SADC_Reg0x2_Almost_cfg(val)                         addSADC_Reg0x2 = ((addSADC_Reg0x2 & (~0x1F)) | ((val) << 0))
#define get_SADC_Reg0x2_Almost_cfg                              (addSADC_Reg0x2 & 0x1F)

//addSADC_Reg0x3
#define addSADC_Reg0x3                                          *((volatile unsigned long *) (BASEADDR_SADC+0x3*4))
#define posSADC_Reg0x3_over_flow                                3
#define bitSADC_Reg0x3_over_flow                                0x8
#define get_SADC_Reg0x3_over_flow                               ((addSADC_Reg0x3 & 0x8) >> 3)

#define posSADC_Reg0x3_sat_enable                               2
#define bitSADC_Reg0x3_sat_enable                               0x4
#define set_SADC_Reg0x3_sat_enable(val)                         addSADC_Reg0x3 = ((addSADC_Reg0x3 & (~0x4)) | ((val) << 2))
#define setf_SADC_Reg0x3_sat_enable                             addSADC_Reg0x3 |= 0x4
#define clrf_SADC_Reg0x3_sat_enable                             addSADC_Reg0x3 &= ~0x4
#define get_SADC_Reg0x3_sat_enable                              ((addSADC_Reg0x3 & 0x4) >> 2)

#define posSADC_Reg0x3_sat_ctrl                                 0
#define bitSADC_Reg0x3_sat_ctrl                                 0x3
#define set_SADC_Reg0x3_sat_ctrl(val)                           addSADC_Reg0x3 = ((addSADC_Reg0x3 & (~0x3)) | ((val) << 0))
#define get_SADC_Reg0x3_sat_ctrl                                (addSADC_Reg0x3 & 0x3)

//addSADC_Reg0x4
#define addSADC_Reg0x4                                          *((volatile unsigned long *) (BASEADDR_SADC+0x4*4))
#define posSADC_Reg0x4_ADC_DATA_16                              0
#define bitSADC_Reg0x4_ADC_DATA_16                              0xFFFF
#define get_SADC_Reg0x4_ADC_DATA_16                             (addSADC_Reg0x4 & 0xFFFF)


#define addSADC_Reg0x5                                          *((volatile unsigned long *) (BASEADDR_SADC+0x5*4))

#define addSADC_Reg0x6                                          *((volatile unsigned long *) (BASEADDR_SADC+0x6*4))

#define addSADC_Reg0x7                                          *((volatile unsigned long *) (BASEADDR_SADC+0x7*4))



//************************************************************//
//WDT
//************************************************************//
#define BASEADDR_WDT                                            0x44800000
//addWDT_Reg0x0
#define addWDT_Reg0x0                                           *((volatile unsigned long *) (0x44800000+0x0*4))
#define posWDT_Reg0x0_WDKEY                                     16
#define bitWDT_Reg0x0_WDKEY                                     0xFF0000
#define set_WDT_Reg0x0_WDKEY(val)                               addWDT_Reg0x0 = ((addWDT_Reg0x0 & (~0xFF0000)) | ((val) << 16))
#define get_WDT_Reg0x0_WDKEY                                    ((addWDT_Reg0x0 & 0xFF0000) >> 16)

#define posWDT_Reg0x0_WD_PERIOD                                 0
#define bitWDT_Reg0x0_WD_PERIOD                                 0xFFFF
#define set_WDT_Reg0x0_WD_PERIOD(val)                           addWDT_Reg0x0 = ((addWDT_Reg0x0 & (~0xFFFF)) | ((val) << 0))
#define get_WDT_Reg0x0_WD_PERIOD                                (addWDT_Reg0x0 & 0xFFFF)


//************************************************************//
//I2S
//************************************************************//
#define BASEADDR_I2S                                            0x47810000
//addI2S_Reg0x0
#define addI2S_Reg0x0                                           *((volatile unsigned long *) (0x47810000+0x0*4))
#define posI2S_Reg0x0_I2SPCMEN                                  31
#define bitI2S_Reg0x0_I2SPCMEN                                  0x80000000
#define set_I2S_Reg0x0_I2SPCMEN(val)                            addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x80000000)) | ((val) << 31))
#define setf_I2S_Reg0x0_I2SPCMEN                                addI2S_Reg0x0 |= 0x80000000
#define clrf_I2S_Reg0x0_I2SPCMEN                                addI2S_Reg0x0 &= ~0x80000000
#define get_I2S_Reg0x0_I2SPCMEN                                 ((addI2S_Reg0x0 & 0x80000000) >> 31)

#define posI2S_Reg0x0_MSTEN                                     30
#define bitI2S_Reg0x0_MSTEN                                     0x40000000
#define set_I2S_Reg0x0_MSTEN(val)                               addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x40000000)) | ((val) << 30))
#define setf_I2S_Reg0x0_MSTEN                                   addI2S_Reg0x0 |= 0x40000000
#define clrf_I2S_Reg0x0_MSTEN                                   addI2S_Reg0x0 &= ~0x40000000
#define get_I2S_Reg0x0_MSTEN                                    ((addI2S_Reg0x0 & 0x40000000) >> 30)

#define posI2S_Reg0x0_MODESEL                                   27
#define bitI2S_Reg0x0_MODESEL                                   0x38000000
#define set_I2S_Reg0x0_MODESEL(val)                             addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x38000000)) | ((val) << 27))
#define get_I2S_Reg0x0_MODESEL                                  ((addI2S_Reg0x0 & 0x38000000) >> 27)

#define posI2S_Reg0x0_LRCKRP                                    26
#define bitI2S_Reg0x0_LRCKRP                                    0x4000000
#define set_I2S_Reg0x0_LRCKRP(val)                              addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x4000000)) | ((val) << 26))
#define setf_I2S_Reg0x0_LRCKRP                                  addI2S_Reg0x0 |= 0x4000000
#define clrf_I2S_Reg0x0_LRCKRP                                  addI2S_Reg0x0 &= ~0x4000000
#define get_I2S_Reg0x0_LRCKRP                                   ((addI2S_Reg0x0 & 0x4000000) >> 26)

#define posI2S_Reg0x0_SCLKINV                                   25
#define bitI2S_Reg0x0_SCLKINV                                   0x2000000
#define set_I2S_Reg0x0_SCLKINV(val)                             addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x2000000)) | ((val) << 25))
#define setf_I2S_Reg0x0_SCLKINV                                 addI2S_Reg0x0 |= 0x2000000
#define clrf_I2S_Reg0x0_SCLKINV                                 addI2S_Reg0x0 &= ~0x2000000
#define get_I2S_Reg0x0_SCLKINV                                  ((addI2S_Reg0x0 & 0x2000000) >> 25)

#define posI2S_Reg0x0_LSBFIRST                                  24
#define bitI2S_Reg0x0_LSBFIRST                                  0x1000000
#define set_I2S_Reg0x0_LSBFIRST(val)                            addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x1000000)) | ((val) << 24))
#define setf_I2S_Reg0x0_LSBFIRST                                addI2S_Reg0x0 |= 0x1000000
#define clrf_I2S_Reg0x0_LSBFIRST                                addI2S_Reg0x0 &= ~0x1000000
#define get_I2S_Reg0x0_LSBFIRST                                 ((addI2S_Reg0x0 & 0x1000000) >> 24)

#define posI2S_Reg0x0_SYNCLEN                                   21
#define bitI2S_Reg0x0_SYNCLEN                                   0xE00000
#define set_I2S_Reg0x0_SYNCLEN(val)                             addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0xE00000)) | ((val) << 21))
#define get_I2S_Reg0x0_SYNCLEN                                  ((addI2S_Reg0x0 & 0xE00000) >> 21)

#define posI2S_Reg0x0_DATALEN                                   16
#define bitI2S_Reg0x0_DATALEN                                   0x1F0000
#define set_I2S_Reg0x0_DATALEN(val)                             addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x1F0000)) | ((val) << 16))
#define get_I2S_Reg0x0_DATALEN                                  ((addI2S_Reg0x0 & 0x1F0000) >> 16)

#define posI2S_Reg0x0_PCM_DLEN                                  13
#define bitI2S_Reg0x0_PCM_DLEN                                  0xE000
#define set_I2S_Reg0x0_PCM_DLEN(val)                            addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0xE000)) | ((val) << 13))
#define get_I2S_Reg0x0_PCM_DLEN                                 ((addI2S_Reg0x0 & 0xE000) >> 13)

#define posI2S_Reg0x0_SMPRATIO                                  8
#define bitI2S_Reg0x0_SMPRATIO                                  0x1F00
#define set_I2S_Reg0x0_SMPRATIO(val)                            addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0x1F00)) | ((val) << 8))
#define get_I2S_Reg0x0_SMPRATIO                                 ((addI2S_Reg0x0 & 0x1F00) >> 8)

#define posI2S_Reg0x0_BITRATIO                                  0
#define bitI2S_Reg0x0_BITRATIO                                  0xFF
#define set_I2S_Reg0x0_BITRATIO(val)                            addI2S_Reg0x0 = ((addI2S_Reg0x0 & (~0xFF)) | ((val) << 0))
#define get_I2S_Reg0x0_BITRATIO                                 (addI2S_Reg0x0 & 0xFF)

//addI2S_Reg0x1
#define addI2S_Reg0x1                                           *((volatile unsigned long *) (0x47810000+0x1*4))

#define posI2S_Reg0x1_PARALLEL_EN                               17
#define bitI2S_Reg0x1_PARALLEL_EN                               0x20000
#define set_I2S_Reg0x1_PARALLEL_EN(val)                         addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x20000)) | ((val) << 17))
#define setf_I2S_Reg0x1_PARALLEL_EN                             addI2S_Reg0x1 |= 0x20000
#define clrf_I2S_Reg0x1_PARALLEL_EN                             addI2S_Reg0x1 &= ~0x20000
#define get_I2S_Reg0x1_PARALLEL_EN                              ((addI2S_Reg0x1 & 0x20000) >> 17)

#define posI2S_Reg0x1_LRCOM_STORE                               16
#define bitI2S_Reg0x1_LRCOM_STORE                               0x10000
#define set_I2S_Reg0x1_LRCOM_STORE(val)                         addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x10000)) | ((val) << 16))
#define setf_I2S_Reg0x1_LRCOM_STORE                             addI2S_Reg0x1 |= 0x10000
#define clrf_I2S_Reg0x1_LRCOM_STORE                             addI2S_Reg0x1 &= ~0x10000
#define get_I2S_Reg0x1_LRCOM_STORE                              ((addI2S_Reg0x1 & 0x10000) >> 16)

#define posI2S_Reg0x1_BITRATIO_H4B                              12
#define bitI2S_Reg0x1_BITRATIO_H4B                              0xF000
#define set_I2S_Reg0x1_BITRATIO_H4B(val)                        addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0xF000)) | ((val) << 12))
#define get_I2S_Reg0x1_BITRATIO_H4B                             ((addI2S_Reg0x1 & 0xF000) >> 12)

#define posI2S_Reg0x1_SMPRATIO_H2B                              10
#define bitI2S_Reg0x1_SMPRATIO_H2B                              0xC00
#define set_I2S_Reg0x1_SMPRATIO_H2B(val)                        addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0xC00)) | ((val) << 10))
#define get_I2S_Reg0x1_SMPRATIO_H2B                             ((addI2S_Reg0x1 & 0xC00) >> 10)

#define posI2S_Reg0x1_RXFIFO_CLR                                9
#define bitI2S_Reg0x1_RXFIFO_CLR                                0x200
#define set_I2S_Reg0x1_RXFIFO_CLR(val)                          addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x200)) | ((val) << 9))
#define setf_I2S_Reg0x1_RXFIFO_CLR                              addI2S_Reg0x1 |= 0x200
#define clrf_I2S_Reg0x1_RXFIFO_CLR                              addI2S_Reg0x1 &= ~0x200
#define get_I2S_Reg0x1_RXFIFO_CLR                               ((addI2S_Reg0x1 & 0x200) >> 9)

#define posI2S_Reg0x1_TXFIFO_CLR                                8
#define bitI2S_Reg0x1_TXFIFO_CLR                                0x100
#define set_I2S_Reg0x1_TXFIFO_CLR(val)                          addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x100)) | ((val) << 8))
#define setf_I2S_Reg0x1_TXFIFO_CLR                              addI2S_Reg0x1 |= 0x100
#define clrf_I2S_Reg0x1_TXFIFO_CLR                              addI2S_Reg0x1 &= ~0x100
#define get_I2S_Reg0x1_TXFIFO_CLR                               ((addI2S_Reg0x1 & 0x100) >> 8)

#define posI2S_Reg0x1_TXINT_LEVEL                               6
#define bitI2S_Reg0x1_TXINT_LEVEL                               0xC0
#define set_I2S_Reg0x1_TXINT_LEVEL(val)                         addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0xC0)) | ((val) << 6))
#define get_I2S_Reg0x1_TXINT_LEVEL                              ((addI2S_Reg0x1 & 0xC0) >> 6)

#define posI2S_Reg0x1_RXINT_LEVEL                               4
#define bitI2S_Reg0x1_RXINT_LEVEL                               0x30
#define set_I2S_Reg0x1_RXINT_LEVEL(val)                         addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x30)) | ((val) << 4))
#define get_I2S_Reg0x1_RXINT_LEVEL                              ((addI2S_Reg0x1 & 0x30) >> 4)

#define posI2S_Reg0x1_TXUDF_EN                                  3
#define bitI2S_Reg0x1_TXUDF_EN                                  0x8
#define set_I2S_Reg0x1_TXUDF_EN(val)                            addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x8)) | ((val) << 3))
#define setf_I2S_Reg0x1_TXUDF_EN                                addI2S_Reg0x1 |= 0x8
#define clrf_I2S_Reg0x1_TXUDF_EN                                addI2S_Reg0x1 &= ~0x8
#define get_I2S_Reg0x1_TXUDF_EN                                 ((addI2S_Reg0x1 & 0x8) >> 3)

#define posI2S_Reg0x1_RXOVF_EN                                  2
#define bitI2S_Reg0x1_RXOVF_EN                                  0x4
#define set_I2S_Reg0x1_RXOVF_EN(val)                            addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x4)) | ((val) << 2))
#define setf_I2S_Reg0x1_RXOVF_EN                                addI2S_Reg0x1 |= 0x4
#define clrf_I2S_Reg0x1_RXOVF_EN                                addI2S_Reg0x1 &= ~0x4
#define get_I2S_Reg0x1_RXOVF_EN                                 ((addI2S_Reg0x1 & 0x4) >> 2)

#define posI2S_Reg0x1_TXINT_EN                                  1
#define bitI2S_Reg0x1_TXINT_EN                                  0x2
#define set_I2S_Reg0x1_TXINT_EN(val)                            addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x2)) | ((val) << 1))
#define setf_I2S_Reg0x1_TXINT_EN                                addI2S_Reg0x1 |= 0x2
#define clrf_I2S_Reg0x1_TXINT_EN                                addI2S_Reg0x1 &= ~0x2
#define get_I2S_Reg0x1_TXINT_EN                                 ((addI2S_Reg0x1 & 0x2) >> 1)

#define posI2S_Reg0x1_RXINT_EN                                  0
#define bitI2S_Reg0x1_RXINT_EN                                  0x1
#define set_I2S_Reg0x1_RXINT_EN(val)                            addI2S_Reg0x1 = ((addI2S_Reg0x1 & (~0x1)) | ((val) << 0))
#define setf_I2S_Reg0x1_RXINT_EN                                addI2S_Reg0x1 |= 0x1
#define clrf_I2S_Reg0x1_RXINT_EN                                addI2S_Reg0x1 &= ~0x1
#define get_I2S_Reg0x1_RXINT_EN                                 (addI2S_Reg0x1 & 0x1)

//addI2S_Reg0x2
#define addI2S_Reg0x2                                           *((volatile unsigned long *) (0x47810000+0x2*4))

#define posI2S_Reg0x2_TXFIFO_WR_READY                           5
#define bitI2S_Reg0x2_TXFIFO_WR_READY                           0x20
#define get_I2S_Reg0x2_TXFIFO_WR_READY                          ((addI2S_Reg0x2 & 0x20) >> 5)

#define posI2S_Reg0x2_RXFIFO_RD_READY                           4
#define bitI2S_Reg0x2_RXFIFO_RD_READY                           0x10
#define get_I2S_Reg0x2_RXFIFO_RD_READY                          ((addI2S_Reg0x2 & 0x10) >> 4)

#define posI2S_Reg0x2_TXUDF                                     3
#define bitI2S_Reg0x2_TXUDF                                     0x8
#define set_I2S_Reg0x2_TXUDF(val)                               addI2S_Reg0x2 = ((addI2S_Reg0x2 & (~0x8)) | ((val) << 3))
#define setf_I2S_Reg0x2_TXUDF                                   addI2S_Reg0x2 |= 0x8
#define clrf_I2S_Reg0x2_TXUDF                                   addI2S_Reg0x2 &= ~0x8
#define get_I2S_Reg0x2_TXUDF                                    ((addI2S_Reg0x2 & 0x8) >> 3)

#define posI2S_Reg0x2_RXOVF                                     2
#define bitI2S_Reg0x2_RXOVF                                     0x4
#define set_I2S_Reg0x2_RXOVF(val)                               addI2S_Reg0x2 = ((addI2S_Reg0x2 & (~0x4)) | ((val) << 2))
#define setf_I2S_Reg0x2_RXOVF                                   addI2S_Reg0x2 |= 0x4
#define clrf_I2S_Reg0x2_RXOVF                                   addI2S_Reg0x2 &= ~0x4
#define get_I2S_Reg0x2_RXOVF                                    ((addI2S_Reg0x2 & 0x4) >> 2)

#define posI2S_Reg0x2_TXINT                                     1
#define bitI2S_Reg0x2_TXINT                                     0x2
#define get_I2S_Reg0x2_TXINT                                    ((addI2S_Reg0x2 & 0x2) >> 1)

#define posI2S_Reg0x2_RXINT                                     0
#define bitI2S_Reg0x2_RXINT                                     0x1
#define get_I2S_Reg0x2_RXINT                                    (addI2S_Reg0x2 & 0x1)

//addI2S_Reg0x3
#define addI2S_Reg0x3                                           *((volatile unsigned long *) (0x47810000+0x3*4))

//addI2S_Reg0x4
#define addI2S_Reg0x4                                           *((volatile unsigned long *) (0x47810000+0x4*4))

#define posI2S_Reg0x4_TX4UDF_EN                                 11
#define bitI2S_Reg0x4_TX4UDF_EN                                 0x800
#define set_I2S_Reg0x4_TX4UDF_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x800)) | ((val) << 11))
#define setf_I2S_Reg0x4_TX4UDF_EN                               addI2S_Reg0x4 |= 0x800
#define clrf_I2S_Reg0x4_TX4UDF_EN                               addI2S_Reg0x4 &= ~0x800
#define get_I2S_Reg0x4_TX4UDF_EN                                ((addI2S_Reg0x4 & 0x800) >> 11)

#define posI2S_Reg0x4_RX4OVF_EN                                 10
#define bitI2S_Reg0x4_RX4OVF_EN                                 0x400
#define set_I2S_Reg0x4_RX4OVF_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x400)) | ((val) << 10))
#define setf_I2S_Reg0x4_RX4OVF_EN                               addI2S_Reg0x4 |= 0x400
#define clrf_I2S_Reg0x4_RX4OVF_EN                               addI2S_Reg0x4 &= ~0x400
#define get_I2S_Reg0x4_RX4OVF_EN                                ((addI2S_Reg0x4 & 0x400) >> 10)

#define posI2S_Reg0x4_TX4NT_EN                                  9
#define bitI2S_Reg0x4_TX4NT_EN                                  0x200
#define set_I2S_Reg0x4_TX4NT_EN(val)                            addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x200)) | ((val) << 9))
#define setf_I2S_Reg0x4_TX4NT_EN                                addI2S_Reg0x4 |= 0x200
#define clrf_I2S_Reg0x4_TX4NT_EN                                addI2S_Reg0x4 &= ~0x200
#define get_I2S_Reg0x4_TX4NT_EN                                 ((addI2S_Reg0x4 & 0x200) >> 9)

#define posI2S_Reg0x4_RX4INT_EN                                 8
#define bitI2S_Reg0x4_RX4INT_EN                                 0x100
#define set_I2S_Reg0x4_RX4INT_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x100)) | ((val) << 8))
#define setf_I2S_Reg0x4_RX4INT_EN                               addI2S_Reg0x4 |= 0x100
#define clrf_I2S_Reg0x4_RX4INT_EN                               addI2S_Reg0x4 &= ~0x100
#define get_I2S_Reg0x4_RX4INT_EN                                ((addI2S_Reg0x4 & 0x100) >> 8)

#define posI2S_Reg0x4_TX3UDF_EN                                 7
#define bitI2S_Reg0x4_TX3UDF_EN                                 0x80
#define set_I2S_Reg0x4_TX3UDF_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x80)) | ((val) << 7))
#define setf_I2S_Reg0x4_TX3UDF_EN                               addI2S_Reg0x4 |= 0x80
#define clrf_I2S_Reg0x4_TX3UDF_EN                               addI2S_Reg0x4 &= ~0x80
#define get_I2S_Reg0x4_TX3UDF_EN                                ((addI2S_Reg0x4 & 0x80) >> 7)

#define posI2S_Reg0x4_RX3OVF_EN                                 6
#define bitI2S_Reg0x4_RX3OVF_EN                                 0x40
#define set_I2S_Reg0x4_RX3OVF_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x40)) | ((val) << 6))
#define setf_I2S_Reg0x4_RX3OVF_EN                               addI2S_Reg0x4 |= 0x40
#define clrf_I2S_Reg0x4_RX3OVF_EN                               addI2S_Reg0x4 &= ~0x40
#define get_I2S_Reg0x4_RX3OVF_EN                                ((addI2S_Reg0x4 & 0x40) >> 6)

#define posI2S_Reg0x4_TX3NT_EN                                  5
#define bitI2S_Reg0x4_TX3NT_EN                                  0x20
#define set_I2S_Reg0x4_TX3NT_EN(val)                            addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x20)) | ((val) << 5))
#define setf_I2S_Reg0x4_TX3NT_EN                                addI2S_Reg0x4 |= 0x20
#define clrf_I2S_Reg0x4_TX3NT_EN                                addI2S_Reg0x4 &= ~0x20
#define get_I2S_Reg0x4_TX3NT_EN                                 ((addI2S_Reg0x4 & 0x20) >> 5)

#define posI2S_Reg0x4_RX3INT_EN                                 4
#define bitI2S_Reg0x4_RX3INT_EN                                 0x10
#define set_I2S_Reg0x4_RX3INT_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x10)) | ((val) << 4))
#define setf_I2S_Reg0x4_RX3INT_EN                               addI2S_Reg0x4 |= 0x10
#define clrf_I2S_Reg0x4_RX3INT_EN                               addI2S_Reg0x4 &= ~0x10
#define get_I2S_Reg0x4_RX3INT_EN                                ((addI2S_Reg0x4 & 0x10) >> 4)

#define posI2S_Reg0x4_TX2UDF_EN                                 3
#define bitI2S_Reg0x4_TX2UDF_EN                                 0x8
#define set_I2S_Reg0x4_TX2UDF_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x8)) | ((val) << 3))
#define setf_I2S_Reg0x4_TX2UDF_EN                               addI2S_Reg0x4 |= 0x8
#define clrf_I2S_Reg0x4_TX2UDF_EN                               addI2S_Reg0x4 &= ~0x8
#define get_I2S_Reg0x4_TX2UDF_EN                                ((addI2S_Reg0x4 & 0x8) >> 3)

#define posI2S_Reg0x4_RX2OVF_EN                                 2
#define bitI2S_Reg0x4_RX2OVF_EN                                 0x4
#define set_I2S_Reg0x4_RX2OVF_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x4)) | ((val) << 2))
#define setf_I2S_Reg0x4_RX2OVF_EN                               addI2S_Reg0x4 |= 0x4
#define clrf_I2S_Reg0x4_RX2OVF_EN                               addI2S_Reg0x4 &= ~0x4
#define get_I2S_Reg0x4_RX2OVF_EN                                ((addI2S_Reg0x4 & 0x4) >> 2)

#define posI2S_Reg0x4_TX2INT_EN                                 1
#define bitI2S_Reg0x4_TX2INT_EN                                 0x2
#define set_I2S_Reg0x4_TX2INT_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x2)) | ((val) << 1))
#define setf_I2S_Reg0x4_TX2INT_EN                               addI2S_Reg0x4 |= 0x2
#define clrf_I2S_Reg0x4_TX2INT_EN                               addI2S_Reg0x4 &= ~0x2
#define get_I2S_Reg0x4_TX2INT_EN                                ((addI2S_Reg0x4 & 0x2) >> 1)

#define posI2S_Reg0x4_RX2INT_EN                                 0
#define bitI2S_Reg0x4_RX2INT_EN                                 0x1
#define set_I2S_Reg0x4_RX2INT_EN(val)                           addI2S_Reg0x4 = ((addI2S_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_I2S_Reg0x4_RX2INT_EN                               addI2S_Reg0x4 |= 0x1
#define clrf_I2S_Reg0x4_RX2INT_EN                               addI2S_Reg0x4 &= ~0x1
#define get_I2S_Reg0x4_RX2INT_EN                                (addI2S_Reg0x4 & 0x1)

//addI2S_Reg0x5
#define addI2S_Reg0x5                                           *((volatile unsigned long *) (0x47810000+0x5*4))

#define posI2S_Reg0x5_TX4UDF                                    11
#define bitI2S_Reg0x5_TX4UDF                                    0x800
#define set_I2S_Reg0x5_TX4UDF(val)                              addI2S_Reg0x5 = ((addI2S_Reg0x5 & (~0x800)) | ((val) << 11))
#define setf_I2S_Reg0x5_TX4UDF                                  addI2S_Reg0x5 |= 0x800
#define clrf_I2S_Reg0x5_TX4UDF                                  addI2S_Reg0x5 &= ~0x800
#define get_I2S_Reg0x5_TX4UDF                                   ((addI2S_Reg0x5 & 0x800) >> 11)

#define posI2S_Reg0x5_RX4OVF                                    10
#define bitI2S_Reg0x5_RX4OVF                                    0x400
#define set_I2S_Reg0x5_RX4OVF(val)                              addI2S_Reg0x5 = ((addI2S_Reg0x5 & (~0x400)) | ((val) << 10))
#define setf_I2S_Reg0x5_RX4OVF                                  addI2S_Reg0x5 |= 0x400
#define clrf_I2S_Reg0x5_RX4OVF                                  addI2S_Reg0x5 &= ~0x400
#define get_I2S_Reg0x5_RX4OVF                                   ((addI2S_Reg0x5 & 0x400) >> 10)

#define posI2S_Reg0x5_TX4INT                                    9
#define bitI2S_Reg0x5_TX4INT                                    0x200
#define get_I2S_Reg0x5_TX4INT                                   ((addI2S_Reg0x5 & 0x200) >> 9)

#define posI2S_Reg0x5_RX4INT                                    8
#define bitI2S_Reg0x5_RX4INT                                    0x100
#define get_I2S_Reg0x5_RX4INT                                   ((addI2S_Reg0x5 & 0x100) >> 8)

#define posI2S_Reg0x5_TX3UDF                                    3
#define bitI2S_Reg0x5_TX3UDF                                    0x8
#define set_I2S_Reg0x5_TX3UDF(val)                              addI2S_Reg0x5 = ((addI2S_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_I2S_Reg0x5_TX3UDF                                  addI2S_Reg0x5 |= 0x8
#define clrf_I2S_Reg0x5_TX3UDF                                  addI2S_Reg0x5 &= ~0x8
#define get_I2S_Reg0x5_TX3UDF                                   ((addI2S_Reg0x5 & 0x8) >> 3)

#define posI2S_Reg0x5_RX3OVF                                    6
#define bitI2S_Reg0x5_RX3OVF                                    0x40
#define set_I2S_Reg0x5_RX3OVF(val)                              addI2S_Reg0x5 = ((addI2S_Reg0x5 & (~0x40)) | ((val) << 6))
#define setf_I2S_Reg0x5_RX3OVF                                  addI2S_Reg0x5 |= 0x40
#define clrf_I2S_Reg0x5_RX3OVF                                  addI2S_Reg0x5 &= ~0x40
#define get_I2S_Reg0x5_RX3OVF                                   ((addI2S_Reg0x5 & 0x40) >> 6)

#define posI2S_Reg0x5_TX3INT                                    5
#define bitI2S_Reg0x5_TX3INT                                    0x20
#define get_I2S_Reg0x5_TX3INT                                   ((addI2S_Reg0x5 & 0x20) >> 5)

#define posI2S_Reg0x5_RX3INT                                    4
#define bitI2S_Reg0x5_RX3INT                                    0x10
#define get_I2S_Reg0x5_RX3INT                                   ((addI2S_Reg0x5 & 0x10) >> 4)

#define posI2S_Reg0x5_TX2UDF                                    3
#define bitI2S_Reg0x5_TX2UDF                                    0x8
#define set_I2S_Reg0x5_TX2UDF(val)                              addI2S_Reg0x5 = ((addI2S_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_I2S_Reg0x5_TX2UDF                                  addI2S_Reg0x5 |= 0x8
#define clrf_I2S_Reg0x5_TX2UDF                                  addI2S_Reg0x5 &= ~0x8
#define get_I2S_Reg0x5_TX2UDF                                   ((addI2S_Reg0x5 & 0x8) >> 3)

#define posI2S_Reg0x5_RX2OVF                                    2
#define bitI2S_Reg0x5_RX2OVF                                    0x4
#define set_I2S_Reg0x5_RX2OVF(val)                              addI2S_Reg0x5 = ((addI2S_Reg0x5 & (~0x4)) | ((val) << 2))
#define setf_I2S_Reg0x5_RX2OVF                                  addI2S_Reg0x5 |= 0x4
#define clrf_I2S_Reg0x5_RX2OVF                                  addI2S_Reg0x5 &= ~0x4
#define get_I2S_Reg0x5_RX2OVF                                   ((addI2S_Reg0x5 & 0x4) >> 2)

#define posI2S_Reg0x5_TX2INT                                    1
#define bitI2S_Reg0x5_TX2INT                                    0x2
#define get_I2S_Reg0x5_TX2INT                                   ((addI2S_Reg0x5 & 0x2) >> 1)

#define posI2S_Reg0x5_RX2INT                                    0
#define bitI2S_Reg0x5_RX2INT                                    0x1
#define get_I2S_Reg0x5_RX2INT                                   (addI2S_Reg0x5 & 0x1)

//addI2S_Reg0x6
#define addI2S_Reg0x6                                           *((volatile unsigned long *) (0x47810000+0x6*4))

//addI2S_Reg0x7
#define addI2S_Reg0x7                                           *((volatile unsigned long *) (0x47810000+0x7*4))

//addI2S_Reg0x8
#define addI2S_Reg0x8                                           *((volatile unsigned long *) (0x47810000+0x8*4))


//************************************************************//
//TRNG
//************************************************************//
#define BASEADDR_TRNG                                           0x448A0000
//addTRNG_Reg0x0
#define addTRNG_Reg0x0                                          *((volatile unsigned long *) (0x448A0000+0x0*4))

#define posTRNG_Reg0x0_trng_en                                  0
#define bitTRNG_Reg0x0_trng_en                                  0x1
#define set_TRNG_Reg0x0_trng_en(val)                            addTRNG_Reg0x0 = ((addTRNG_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_TRNG_Reg0x0_trng_en                                addTRNG_Reg0x0 |= 0x1
#define clrf_TRNG_Reg0x0_trng_en                                addTRNG_Reg0x0 &= ~0x1
#define get_TRNG_Reg0x0_trng_en                                 (addTRNG_Reg0x0 & 0x1)

//addTRNG_Reg0x1
#define addTRNG_Reg0x1                                          *((volatile unsigned long *) (0x448A0000+0x1*4))


//************************************************************//
//XVR
//************************************************************//
#define BASEADDR_XVR                                            0x4A800000
//addXVR_Reg0x00
#define addXVR_Reg0x00                                          *((volatile unsigned long *) (0x4A800000+0x00*4))
#define posXVR_Reg0x00_capen                                    0
#define bitXVR_Reg0x00_capen                                    0x1
#define set_XVR_Reg0x00_capen(val)                              addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x1)) | ((val) << 0))
#define setf_XVR_Reg0x00_capen                                  addXVR_Reg0x00 |= 0x1
#define clrf_XVR_Reg0x00_capen                                  addXVR_Reg0x00 &= ~0x1
#define get_XVR_Reg0x00_capen                                   (addXVR_Reg0x00 & 0x1)

#define posXVR_Reg0x00_CPmatch                                  1
#define bitXVR_Reg0x00_CPmatch                                  0x1E
#define set_XVR_Reg0x00_CPmatch(val)                            addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x1E)) | ((val) << 1))
#define get_XVR_Reg0x00_CPmatch                                 ((addXVR_Reg0x00 & 0x1E) >> 1)

#define posXVR_Reg0x00_Int_mod                                  5
#define bitXVR_Reg0x00_Int_mod                                  0x20
#define set_XVR_Reg0x00_Int_mod(val)                            addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x20)) | ((val) << 5))
#define setf_XVR_Reg0x00_Int_mod                                addXVR_Reg0x00 |= 0x20
#define clrf_XVR_Reg0x00_Int_mod                                addXVR_Reg0x00 &= ~0x20
#define get_XVR_Reg0x00_Int_mod                                 ((addXVR_Reg0x00 & 0x20) >> 5)

#define posXVR_Reg0x00_cmfbEn                                   6
#define bitXVR_Reg0x00_cmfbEn                                   0x40
#define set_XVR_Reg0x00_cmfbEn(val)                             addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x40)) | ((val) << 6))
#define setf_XVR_Reg0x00_cmfbEn                                 addXVR_Reg0x00 |= 0x40
#define clrf_XVR_Reg0x00_cmfbEn                                 addXVR_Reg0x00 &= ~0x40
#define get_XVR_Reg0x00_cmfbEn                                  ((addXVR_Reg0x00 & 0x40) >> 6)

#define posXVR_Reg0x00_vcopol                                   7
#define bitXVR_Reg0x00_vcopol                                   0x80
#define set_XVR_Reg0x00_vcopol(val)                             addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x80)) | ((val) << 7))
#define setf_XVR_Reg0x00_vcopol                                 addXVR_Reg0x00 |= 0x80
#define clrf_XVR_Reg0x00_vcopol                                 addXVR_Reg0x00 &= ~0x80
#define get_XVR_Reg0x00_vcopol                                  ((addXVR_Reg0x00 & 0x80) >> 7)

#define posXVR_Reg0x00_tristate                                 8
#define bitXVR_Reg0x00_tristate                                 0x100
#define set_XVR_Reg0x00_tristate(val)                           addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x100)) | ((val) << 8))
#define setf_XVR_Reg0x00_tristate                               addXVR_Reg0x00 |= 0x100
#define clrf_XVR_Reg0x00_tristate                               addXVR_Reg0x00 &= ~0x100
#define get_XVR_Reg0x00_tristate                                ((addXVR_Reg0x00 & 0x100) >> 8)

#define posXVR_Reg0x00_enTRout                                  9
#define bitXVR_Reg0x00_enTRout                                  0x200
#define set_XVR_Reg0x00_enTRout(val)                            addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x200)) | ((val) << 9))
#define setf_XVR_Reg0x00_enTRout                                addXVR_Reg0x00 |= 0x200
#define clrf_XVR_Reg0x00_enTRout                                addXVR_Reg0x00 &= ~0x200
#define get_XVR_Reg0x00_enTRout                                 ((addXVR_Reg0x00 & 0x200) >> 9)

#define posXVR_Reg0x00_enTNout                                  10
#define bitXVR_Reg0x00_enTNout                                  0x400
#define set_XVR_Reg0x00_enTNout(val)                            addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x400)) | ((val) << 10))
#define setf_XVR_Reg0x00_enTNout                                addXVR_Reg0x00 |= 0x400
#define clrf_XVR_Reg0x00_enTNout                                addXVR_Reg0x00 &= ~0x400
#define get_XVR_Reg0x00_enTNout                                 ((addXVR_Reg0x00 & 0x400) >> 10)

#define posXVR_Reg0x00_pfddelay                                 11
#define bitXVR_Reg0x00_pfddelay                                 0x1800
#define set_XVR_Reg0x00_pfddelay(val)                           addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x1800)) | ((val) << 11))
#define get_XVR_Reg0x00_pfddelay                                ((addXVR_Reg0x00 & 0x1800) >> 11)

#define posXVR_Reg0x00_ckpolsel                                 13
#define bitXVR_Reg0x00_ckpolsel                                 0x2000
#define set_XVR_Reg0x00_ckpolsel(val)                           addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x00_ckpolsel                               addXVR_Reg0x00 |= 0x2000
#define clrf_XVR_Reg0x00_ckpolsel                               addXVR_Reg0x00 &= ~0x2000
#define get_XVR_Reg0x00_ckpolsel                                ((addXVR_Reg0x00 & 0x2000) >> 13)

#define posXVR_Reg0x00_Nrsten                                   14
#define bitXVR_Reg0x00_Nrsten                                   0x4000
#define set_XVR_Reg0x00_Nrsten(val)                             addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x4000)) | ((val) << 14))
#define setf_XVR_Reg0x00_Nrsten                                 addXVR_Reg0x00 |= 0x4000
#define clrf_XVR_Reg0x00_Nrsten                                 addXVR_Reg0x00 &= ~0x4000
#define get_XVR_Reg0x00_Nrsten                                  ((addXVR_Reg0x00 & 0x4000) >> 14)

#define posXVR_Reg0x00_pllrst                                   15
#define bitXVR_Reg0x00_pllrst                                   0x8000
#define set_XVR_Reg0x00_pllrst(val)                             addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x8000)) | ((val) << 15))
#define setf_XVR_Reg0x00_pllrst                                 addXVR_Reg0x00 |= 0x8000
#define clrf_XVR_Reg0x00_pllrst                                 addXVR_Reg0x00 &= ~0x8000
#define get_XVR_Reg0x00_pllrst                                  ((addXVR_Reg0x00 & 0x8000) >> 15)

#define posXVR_Reg0x00_Nint                                     16
#define bitXVR_Reg0x00_Nint                                     0x3FF0000
#define set_XVR_Reg0x00_Nint(val)                               addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0x3FF0000)) | ((val) << 16))
#define get_XVR_Reg0x00_Nint                                    ((addXVR_Reg0x00 & 0x3FF0000) >> 16)

#define posXVR_Reg0x00_R                                        26
#define bitXVR_Reg0x00_R                                        0xFC000000
#define set_XVR_Reg0x00_R(val)                                  addXVR_Reg0x00 = ((addXVR_Reg0x00 & (~0xFC000000)) | ((val) << 26))
#define get_XVR_Reg0x00_R                                       ((addXVR_Reg0x00 & 0xFC000000) >> 26)

//addXVR_Reg0x01
#define addXVR_Reg0x01                                          *((volatile unsigned long *) (0x4A800000+0x01*4))
#define posXVR_Reg0x01_lwvref                                   0
#define bitXVR_Reg0x01_lwvref                                   0x3
#define set_XVR_Reg0x01_lwvref(val)                             addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x3)) | ((val) << 0))
#define get_XVR_Reg0x01_lwvref                                  (addXVR_Reg0x01 & 0x3)

#define posXVR_Reg0x01_lnvref                                   2
#define bitXVR_Reg0x01_lnvref                                   0xC
#define set_XVR_Reg0x01_lnvref(val)                             addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0xC)) | ((val) << 2))
#define get_XVR_Reg0x01_lnvref                                  ((addXVR_Reg0x01 & 0xC) >> 2)

#define posXVR_Reg0x01_hwvref                                   4
#define bitXVR_Reg0x01_hwvref                                   0x30
#define set_XVR_Reg0x01_hwvref(val)                             addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x30)) | ((val) << 4))
#define get_XVR_Reg0x01_hwvref                                  ((addXVR_Reg0x01 & 0x30) >> 4)

#define posXVR_Reg0x01_hnvref                                   6
#define bitXVR_Reg0x01_hnvref                                   0xC0
#define set_XVR_Reg0x01_hnvref(val)                             addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0xC0)) | ((val) << 6))
#define get_XVR_Reg0x01_hnvref                                  ((addXVR_Reg0x01 & 0xC0) >> 6)

#define posXVR_Reg0x01_errdet_spien                             8
#define bitXVR_Reg0x01_errdet_spien                             0x100
#define set_XVR_Reg0x01_errdet_spien(val)                       addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x100)) | ((val) << 8))
#define setf_XVR_Reg0x01_errdet_spien                           addXVR_Reg0x01 |= 0x100
#define clrf_XVR_Reg0x01_errdet_spien                           addXVR_Reg0x01 &= ~0x100
#define get_XVR_Reg0x01_errdet_spien                            ((addXVR_Reg0x01 & 0x100) >> 8)

#define posXVR_Reg0x01_captunedir                               9
#define bitXVR_Reg0x01_captunedir                               0x200
#define set_XVR_Reg0x01_captunedir(val)                         addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x200)) | ((val) << 9))
#define setf_XVR_Reg0x01_captunedir                             addXVR_Reg0x01 |= 0x200
#define clrf_XVR_Reg0x01_captunedir                             addXVR_Reg0x01 &= ~0x200
#define get_XVR_Reg0x01_captunedir                              ((addXVR_Reg0x01 & 0x200) >> 9)

#define posXVR_Reg0x01_CK64sel                                  10
#define bitXVR_Reg0x01_CK64sel                                  0x400
#define set_XVR_Reg0x01_CK64sel(val)                            addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x400)) | ((val) << 10))
#define setf_XVR_Reg0x01_CK64sel                                addXVR_Reg0x01 |= 0x400
#define clrf_XVR_Reg0x01_CK64sel                                addXVR_Reg0x01 &= ~0x400
#define get_XVR_Reg0x01_CK64sel                                 ((addXVR_Reg0x01 & 0x400) >> 10)

#define posXVR_Reg0x01_extressel                                11
#define bitXVR_Reg0x01_extressel                                0x800
#define set_XVR_Reg0x01_extressel(val)                          addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x800)) | ((val) << 11))
#define setf_XVR_Reg0x01_extressel                              addXVR_Reg0x01 |= 0x800
#define clrf_XVR_Reg0x01_extressel                              addXVR_Reg0x01 &= ~0x800
#define get_XVR_Reg0x01_extressel                               ((addXVR_Reg0x01 & 0x800) >> 11)

#define posXVR_Reg0x01_Dnck                                     12
#define bitXVR_Reg0x01_Dnck                                     0x1000
#define set_XVR_Reg0x01_Dnck(val)                               addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x01_Dnck                                   addXVR_Reg0x01 |= 0x1000
#define clrf_XVR_Reg0x01_Dnck                                   addXVR_Reg0x01 &= ~0x1000
#define get_XVR_Reg0x01_Dnck                                    ((addXVR_Reg0x01 & 0x1000) >> 12)

#define posXVR_Reg0x01_TrxLo_amp_ctrl                           13
#define bitXVR_Reg0x01_TrxLo_amp_ctrl                           0x2000
#define set_XVR_Reg0x01_TrxLo_amp_ctrl(val)                     addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x01_TrxLo_amp_ctrl                         addXVR_Reg0x01 |= 0x2000
#define clrf_XVR_Reg0x01_TrxLo_amp_ctrl                         addXVR_Reg0x01 &= ~0x2000
#define get_XVR_Reg0x01_TrxLo_amp_ctrl                          ((addXVR_Reg0x01 & 0x2000) >> 13)

#define posXVR_Reg0x01_VCOIdiv                                  14
#define bitXVR_Reg0x01_VCOIdiv                                  0xC000
#define set_XVR_Reg0x01_VCOIdiv(val)                            addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0xC000)) | ((val) << 14))
#define get_XVR_Reg0x01_VCOIdiv                                 ((addXVR_Reg0x01 & 0xC000) >> 14)

#define posXVR_Reg0x01_NCK_pol_sel                              16
#define bitXVR_Reg0x01_NCK_pol_sel                              0x10000
#define set_XVR_Reg0x01_NCK_pol_sel(val)                        addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x10000)) | ((val) << 16))
#define setf_XVR_Reg0x01_NCK_pol_sel                            addXVR_Reg0x01 |= 0x10000
#define clrf_XVR_Reg0x01_NCK_pol_sel                            addXVR_Reg0x01 &= ~0x10000
#define get_XVR_Reg0x01_NCK_pol_sel                             ((addXVR_Reg0x01 & 0x10000) >> 16)

#define posXVR_Reg0x01_MODsel                                   17
#define bitXVR_Reg0x01_MODsel                                   0x20000
#define set_XVR_Reg0x01_MODsel(val)                             addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x20000)) | ((val) << 17))
#define setf_XVR_Reg0x01_MODsel                                 addXVR_Reg0x01 |= 0x20000
#define clrf_XVR_Reg0x01_MODsel                                 addXVR_Reg0x01 &= ~0x20000
#define get_XVR_Reg0x01_MODsel                                  ((addXVR_Reg0x01 & 0x20000) >> 17)

#define posXVR_Reg0x01_calresc                                  18
#define bitXVR_Reg0x01_calresc                                  0xC0000
#define set_XVR_Reg0x01_calresc(val)                            addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0xC0000)) | ((val) << 18))
#define get_XVR_Reg0x01_calresc                                 ((addXVR_Reg0x01 & 0xC0000) >> 18)

#define posXVR_Reg0x01_NWmodN                                   20
#define bitXVR_Reg0x01_NWmodN                                   0x700000
#define set_XVR_Reg0x01_NWmodN(val)                             addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x700000)) | ((val) << 20))
#define get_XVR_Reg0x01_NWmodN                                  ((addXVR_Reg0x01 & 0x700000) >> 20)

#define posXVR_Reg0x01_Nwmod                                    23
#define bitXVR_Reg0x01_Nwmod                                    0x3800000
#define set_XVR_Reg0x01_Nwmod(val)                              addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x3800000)) | ((val) << 23))
#define get_XVR_Reg0x01_Nwmod                                   ((addXVR_Reg0x01 & 0x3800000) >> 23)

#define posXVR_Reg0x01_Nwvco                                    26
#define bitXVR_Reg0x01_Nwvco                                    0x1C000000
#define set_XVR_Reg0x01_Nwvco(val)                              addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0x1C000000)) | ((val) << 26))
#define get_XVR_Reg0x01_Nwvco                                   ((addXVR_Reg0x01 & 0x1C000000) >> 26)

#define posXVR_Reg0x01_Rvco                                     29
#define bitXVR_Reg0x01_Rvco                                     0xE0000000
#define set_XVR_Reg0x01_Rvco(val)                               addXVR_Reg0x01 = ((addXVR_Reg0x01 & (~0xE0000000)) | ((val) << 29))
#define get_XVR_Reg0x01_Rvco                                    ((addXVR_Reg0x01 & 0xE0000000) >> 29)

//addXVR_Reg0x02
#define addXVR_Reg0x02                                          *((volatile unsigned long *) (0x4A800000+0x02*4))
#define posXVR_Reg0x02_capdirsel                                0
#define bitXVR_Reg0x02_capdirsel                                0x1
#define set_XVR_Reg0x02_capdirsel(val)                          addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0x1)) | ((val) << 0))
#define setf_XVR_Reg0x02_capdirsel                              addXVR_Reg0x02 |= 0x1
#define clrf_XVR_Reg0x02_capdirsel                              addXVR_Reg0x02 &= ~0x1
#define get_XVR_Reg0x02_capdirsel                               (addXVR_Reg0x02 & 0x1)


#define posXVR_Reg0x02_cmfben2spi                               2
#define bitXVR_Reg0x02_cmfben2spi                               0x4
#define set_XVR_Reg0x02_cmfben2spi(val)                         addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0x4)) | ((val) << 2))
#define setf_XVR_Reg0x02_cmfben2spi                             addXVR_Reg0x02 |= 0x4
#define clrf_XVR_Reg0x02_cmfben2spi                             addXVR_Reg0x02 &= ~0x4
#define get_XVR_Reg0x02_cmfben2spi                              ((addXVR_Reg0x02 & 0x4) >> 2)

#define posXVR_Reg0x02_cmfben2sel                               3
#define bitXVR_Reg0x02_cmfben2sel                               0x8
#define set_XVR_Reg0x02_cmfben2sel(val)                         addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0x8)) | ((val) << 3))
#define setf_XVR_Reg0x02_cmfben2sel                             addXVR_Reg0x02 |= 0x8
#define clrf_XVR_Reg0x02_cmfben2sel                             addXVR_Reg0x02 &= ~0x8
#define get_XVR_Reg0x02_cmfben2sel                              ((addXVR_Reg0x02 & 0x8) >> 3)

#define posXVR_Reg0x02_loopress                                 4
#define bitXVR_Reg0x02_loopress                                 0xF0
#define set_XVR_Reg0x02_loopress(val)                           addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0xF0)) | ((val) << 4))
#define get_XVR_Reg0x02_loopress                                ((addXVR_Reg0x02 & 0xF0) >> 4)

#define posXVR_Reg0x02_loopresm                                 8
#define bitXVR_Reg0x02_loopresm                                 0xF00
#define set_XVR_Reg0x02_loopresm(val)                           addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0xF00)) | ((val) << 8))
#define get_XVR_Reg0x02_loopresm                                ((addXVR_Reg0x02 & 0xF00) >> 8)

#define posXVR_Reg0x02_loopresf                                 12
#define bitXVR_Reg0x02_loopresf                                 0xF000
#define set_XVR_Reg0x02_loopresf(val)                           addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0xF000)) | ((val) << 12))
#define get_XVR_Reg0x02_loopresf                                ((addXVR_Reg0x02 & 0xF000) >> 12)


#define posXVR_Reg0x02_Icpf                                     17
#define bitXVR_Reg0x02_Icpf                                     0x3E0000
#define set_XVR_Reg0x02_Icpf(val)                               addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0x3E0000)) | ((val) << 17))
#define get_XVR_Reg0x02_Icpf                                    ((addXVR_Reg0x02 & 0x3E0000) >> 17)

#define posXVR_Reg0x02_Icpm                                     22
#define bitXVR_Reg0x02_Icpm                                     0x7C00000
#define set_XVR_Reg0x02_Icpm(val)                               addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0x7C00000)) | ((val) << 22))
#define get_XVR_Reg0x02_Icpm                                    ((addXVR_Reg0x02 & 0x7C00000) >> 22)

#define posXVR_Reg0x02_Icps                                     27
#define bitXVR_Reg0x02_Icps                                     0xF8000000
#define set_XVR_Reg0x02_Icps(val)                               addXVR_Reg0x02 = ((addXVR_Reg0x02 & (~0xF8000000)) | ((val) << 27))
#define get_XVR_Reg0x02_Icps                                    ((addXVR_Reg0x02 & 0xF8000000) >> 27)

//addXVR_Reg0x03
#define addXVR_Reg0x03                                          *((volatile unsigned long *) (0x4A800000+0x03*4))

#define posXVR_Reg0x03_manual                                   16
#define bitXVR_Reg0x03_manual                                   0x10000
#define set_XVR_Reg0x03_manual(val)                             addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0x10000)) | ((val) << 16))
#define setf_XVR_Reg0x03_manual                                 addXVR_Reg0x03 |= 0x10000
#define clrf_XVR_Reg0x03_manual                                 addXVR_Reg0x03 &= ~0x10000
#define get_XVR_Reg0x03_manual                                  ((addXVR_Reg0x03 & 0x10000) >> 16)

#define posXVR_Reg0x03_bandm                                    17
#define bitXVR_Reg0x03_bandm                                    0x7E0000
#define set_XVR_Reg0x03_bandm(val)                              addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0x7E0000)) | ((val) << 17))
#define get_XVR_Reg0x03_bandm                                   ((addXVR_Reg0x03 & 0x7E0000) >> 17)

#define posXVR_Reg0x03_loopexten                                23
#define bitXVR_Reg0x03_loopexten                                0x800000
#define set_XVR_Reg0x03_loopexten(val)                          addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0x800000)) | ((val) << 23))
#define setf_XVR_Reg0x03_loopexten                              addXVR_Reg0x03 |= 0x800000
#define clrf_XVR_Reg0x03_loopexten                              addXVR_Reg0x03 &= ~0x800000
#define get_XVR_Reg0x03_loopexten                               ((addXVR_Reg0x03 & 0x800000) >> 23)

#define posXVR_Reg0x03_TRSWen                                   24
#define bitXVR_Reg0x03_TRSWen                                   0x1000000
#define set_XVR_Reg0x03_TRSWen(val)                             addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0x1000000)) | ((val) << 24))
#define setf_XVR_Reg0x03_TRSWen                                 addXVR_Reg0x03 |= 0x1000000
#define clrf_XVR_Reg0x03_TRSWen                                 addXVR_Reg0x03 &= ~0x1000000
#define get_XVR_Reg0x03_TRSWen                                  ((addXVR_Reg0x03 & 0x1000000) >> 24)

#define posXVR_Reg0x03_TRSW_spi                                 25
#define bitXVR_Reg0x03_TRSW_spi                                 0x2000000
#define set_XVR_Reg0x03_TRSW_spi(val)                           addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0x2000000)) | ((val) << 25))
#define setf_XVR_Reg0x03_TRSW_spi                               addXVR_Reg0x03 |= 0x2000000
#define clrf_XVR_Reg0x03_TRSW_spi                               addXVR_Reg0x03 &= ~0x2000000
#define get_XVR_Reg0x03_TRSW_spi                                ((addXVR_Reg0x03 & 0x2000000) >> 25)

#define posXVR_Reg0x03_Cdyn_en                                  26
#define bitXVR_Reg0x03_Cdyn_en                                  0x4000000
#define set_XVR_Reg0x03_Cdyn_en(val)                            addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0x4000000)) | ((val) << 26))
#define setf_XVR_Reg0x03_Cdyn_en                                addXVR_Reg0x03 |= 0x4000000
#define clrf_XVR_Reg0x03_Cdyn_en                                addXVR_Reg0x03 &= ~0x4000000
#define get_XVR_Reg0x03_Cdyn_en                                 ((addXVR_Reg0x03 & 0x4000000) >> 26)

#define posXVR_Reg0x03_Cdyn_spi                                 28
#define bitXVR_Reg0x03_Cdyn_spi                                 0xF0000000
#define set_XVR_Reg0x03_Cdyn_spi(val)                           addXVR_Reg0x03 = ((addXVR_Reg0x03 & (~0xF0000000)) | ((val) << 28))
#define get_XVR_Reg0x03_Cdyn_spi                                ((addXVR_Reg0x03 & 0xF0000000) >> 28)

//addXVR_Reg0x04
#define addXVR_Reg0x04                                          *((volatile unsigned long *) (0x4A800000+0x04*4))
#define posXVR_Reg0x04_Rvco_tx                                  0
#define bitXVR_Reg0x04_Rvco_tx                                  0x7
#define set_XVR_Reg0x04_Rvco_tx(val)                            addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x7)) | ((val) << 0))
#define get_XVR_Reg0x04_Rvco_tx                                 (addXVR_Reg0x04 & 0x7)

#define posXVR_Reg0x04_DIplus                                   3
#define bitXVR_Reg0x04_DIplus                                   0x8
#define set_XVR_Reg0x04_DIplus(val)                             addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x8)) | ((val) << 3))
#define setf_XVR_Reg0x04_DIplus                                 addXVR_Reg0x04 |= 0x8
#define clrf_XVR_Reg0x04_DIplus                                 addXVR_Reg0x04 &= ~0x8
#define get_XVR_Reg0x04_DIplus                                  ((addXVR_Reg0x04 & 0x8) >> 3)

#define posXVR_Reg0x04_Nwvco                                    4
#define bitXVR_Reg0x04_Nwvco                                    0x70
#define set_XVR_Reg0x04_Nwvco(val)                              addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x70)) | ((val) << 4))
#define get_XVR_Reg0x04_Nwvco                                   ((addXVR_Reg0x04 & 0x70) >> 4)

#define posXVR_Reg0x04_bandm                                    7
#define bitXVR_Reg0x04_bandm                                    0x7F80
#define set_XVR_Reg0x04_bandm(val)                              addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x7F80)) | ((val) << 7))
#define get_XVR_Reg0x04_bandm                                   ((addXVR_Reg0x04 & 0x7F80) >> 7)

#define posXVR_Reg0x04_manual                                   15
#define bitXVR_Reg0x04_manual                                   0x8000
#define set_XVR_Reg0x04_manual(val)                             addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x8000)) | ((val) << 15))
#define setf_XVR_Reg0x04_manual                                 addXVR_Reg0x04 |= 0x8000
#define clrf_XVR_Reg0x04_manual                                 addXVR_Reg0x04 &= ~0x8000
#define get_XVR_Reg0x04_manual                                  ((addXVR_Reg0x04 & 0x8000) >> 15)

#define posXVR_Reg0x04_spierrdet_en                             16
#define bitXVR_Reg0x04_spierrdet_en                             0x10000
#define set_XVR_Reg0x04_spierrdet_en(val)                       addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x10000)) | ((val) << 16))
#define setf_XVR_Reg0x04_spierrdet_en                           addXVR_Reg0x04 |= 0x10000
#define clrf_XVR_Reg0x04_spierrdet_en                           addXVR_Reg0x04 &= ~0x10000
#define get_XVR_Reg0x04_spierrdet_en                            ((addXVR_Reg0x04 & 0x10000) >> 16)

#define posXVR_Reg0x04_int_mod                                  17
#define bitXVR_Reg0x04_int_mod                                  0x20000
#define set_XVR_Reg0x04_int_mod(val)                            addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x20000)) | ((val) << 17))
#define setf_XVR_Reg0x04_int_mod                                addXVR_Reg0x04 |= 0x20000
#define clrf_XVR_Reg0x04_int_mod                                addXVR_Reg0x04 &= ~0x20000
#define get_XVR_Reg0x04_int_mod                                 ((addXVR_Reg0x04 & 0x20000) >> 17)

#define posXVR_Reg0x04_sdmreset                                 18
#define bitXVR_Reg0x04_sdmreset                                 0x40000
#define set_XVR_Reg0x04_sdmreset(val)                           addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x40000)) | ((val) << 18))
#define setf_XVR_Reg0x04_sdmreset                               addXVR_Reg0x04 |= 0x40000
#define clrf_XVR_Reg0x04_sdmreset                               addXVR_Reg0x04 &= ~0x40000
#define get_XVR_Reg0x04_sdmreset                                ((addXVR_Reg0x04 & 0x40000) >> 18)

#define posXVR_Reg0x04_refpolsdm                                19
#define bitXVR_Reg0x04_refpolsdm                                0x80000
#define set_XVR_Reg0x04_refpolsdm(val)                          addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x80000)) | ((val) << 19))
#define setf_XVR_Reg0x04_refpolsdm                              addXVR_Reg0x04 |= 0x80000
#define clrf_XVR_Reg0x04_refpolsdm                              addXVR_Reg0x04 &= ~0x80000
#define get_XVR_Reg0x04_refpolsdm                               ((addXVR_Reg0x04 & 0x80000) >> 19)

#define posXVR_Reg0x04_spi_reset                                20
#define bitXVR_Reg0x04_spi_reset                                0x100000
#define set_XVR_Reg0x04_spi_reset(val)                          addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x100000)) | ((val) << 20))
#define setf_XVR_Reg0x04_spi_reset                              addXVR_Reg0x04 |= 0x100000
#define clrf_XVR_Reg0x04_spi_reset                              addXVR_Reg0x04 &= ~0x100000
#define get_XVR_Reg0x04_spi_reset                               ((addXVR_Reg0x04 & 0x100000) >> 20)

#define posXVR_Reg0x04_spi_trigger                              21
#define bitXVR_Reg0x04_spi_trigger                              0x200000
#define set_XVR_Reg0x04_spi_trigger(val)                        addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x200000)) | ((val) << 21))
#define setf_XVR_Reg0x04_spi_trigger                            addXVR_Reg0x04 |= 0x200000
#define clrf_XVR_Reg0x04_spi_trigger                            addXVR_Reg0x04 &= ~0x200000
#define get_XVR_Reg0x04_spi_trigger                             ((addXVR_Reg0x04 & 0x200000) >> 21)

#define posXVR_Reg0x04_Nrsten                                   22
#define bitXVR_Reg0x04_Nrsten                                   0x400000
#define set_XVR_Reg0x04_Nrsten(val)                             addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x400000)) | ((val) << 22))
#define setf_XVR_Reg0x04_Nrsten                                 addXVR_Reg0x04 |= 0x400000
#define clrf_XVR_Reg0x04_Nrsten                                 addXVR_Reg0x04 &= ~0x400000
#define get_XVR_Reg0x04_Nrsten                                  ((addXVR_Reg0x04 & 0x400000) >> 22)

#define posXVR_Reg0x04_loopexten                                23
#define bitXVR_Reg0x04_loopexten                                0x800000
#define set_XVR_Reg0x04_loopexten(val)                          addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x800000)) | ((val) << 23))
#define setf_XVR_Reg0x04_loopexten                              addXVR_Reg0x04 |= 0x800000
#define clrf_XVR_Reg0x04_loopexten                              addXVR_Reg0x04 &= ~0x800000
#define get_XVR_Reg0x04_loopexten                               ((addXVR_Reg0x04 & 0x800000) >> 23)

#define posXVR_Reg0x04_enTNout                                  24
#define bitXVR_Reg0x04_enTNout                                  0x1000000
#define set_XVR_Reg0x04_enTNout(val)                            addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x1000000)) | ((val) << 24))
#define setf_XVR_Reg0x04_enTNout                                addXVR_Reg0x04 |= 0x1000000
#define clrf_XVR_Reg0x04_enTNout                                addXVR_Reg0x04 &= ~0x1000000
#define get_XVR_Reg0x04_enTNout                                 ((addXVR_Reg0x04 & 0x1000000) >> 24)

#define posXVR_Reg0x04_spigcken                                 25
#define bitXVR_Reg0x04_spigcken                                 0x2000000
#define set_XVR_Reg0x04_spigcken(val)                           addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x2000000)) | ((val) << 25))
#define setf_XVR_Reg0x04_spigcken                               addXVR_Reg0x04 |= 0x2000000
#define clrf_XVR_Reg0x04_spigcken                               addXVR_Reg0x04 &= ~0x2000000
#define get_XVR_Reg0x04_spigcken                                ((addXVR_Reg0x04 & 0x2000000) >> 25)

#define posXVR_Reg0x04_CKBBEn96                                 26
#define bitXVR_Reg0x04_CKBBEn96                                 0x4000000
#define set_XVR_Reg0x04_CKBBEn96(val)                           addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x4000000)) | ((val) << 26))
#define setf_XVR_Reg0x04_CKBBEn96                               addXVR_Reg0x04 |= 0x4000000
#define clrf_XVR_Reg0x04_CKBBEn96                               addXVR_Reg0x04 &= ~0x4000000
#define get_XVR_Reg0x04_CKBBEn96                                ((addXVR_Reg0x04 & 0x4000000) >> 26)

#define posXVR_Reg0x04_CKBBEn64                                 27
#define bitXVR_Reg0x04_CKBBEn64                                 0x8000000
#define set_XVR_Reg0x04_CKBBEn64(val)                           addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x04_CKBBEn64                               addXVR_Reg0x04 |= 0x8000000
#define clrf_XVR_Reg0x04_CKBBEn64                               addXVR_Reg0x04 &= ~0x8000000
#define get_XVR_Reg0x04_CKBBEn64                                ((addXVR_Reg0x04 & 0x8000000) >> 27)

#define posXVR_Reg0x04_lvref                                    28
#define bitXVR_Reg0x04_lvref                                    0x30000000
#define set_XVR_Reg0x04_lvref(val)                              addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0x30000000)) | ((val) << 28))
#define get_XVR_Reg0x04_lvref                                   ((addXVR_Reg0x04 & 0x30000000) >> 28)

#define posXVR_Reg0x04_hvref                                    30
#define bitXVR_Reg0x04_hvref                                    0xC0000000
#define set_XVR_Reg0x04_hvref(val)                              addXVR_Reg0x04 = ((addXVR_Reg0x04 & (~0xC0000000)) | ((val) << 30))
#define get_XVR_Reg0x04_hvref                                   ((addXVR_Reg0x04 & 0xC0000000) >> 30)

//addXVR_Reg0x05
#define addXVR_Reg0x05                                          *((volatile unsigned long *) (0x4A800000+0x05*4))

//addXVR_Reg0x06
#define addXVR_Reg0x06                                          *((volatile unsigned long *) (0x4A800000+0x06*4))
#define posXVR_Reg0x06_N                                        0
#define bitXVR_Reg0x06_N                                        0x3FF
#define set_XVR_Reg0x06_N(val)                                  addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0x3FF)) | ((val) << 0))
#define get_XVR_Reg0x06_N                                       (addXVR_Reg0x06 & 0x3FF)

#define posXVR_Reg0x06_R                                        10
#define bitXVR_Reg0x06_R                                        0xFC00
#define set_XVR_Reg0x06_R(val)                                  addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0xFC00)) | ((val) << 10))
#define get_XVR_Reg0x06_R                                       ((addXVR_Reg0x06 & 0xFC00) >> 10)

#define posXVR_Reg0x06_loopresc                                 16
#define bitXVR_Reg0x06_loopresc                                 0xF0000
#define set_XVR_Reg0x06_loopresc(val)                           addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0xF0000)) | ((val) << 16))
#define get_XVR_Reg0x06_loopresc                                ((addXVR_Reg0x06 & 0xF0000) >> 16)


#define posXVR_Reg0x06_cp                                       22
#define bitXVR_Reg0x06_cp                                       0xC00000
#define set_XVR_Reg0x06_cp(val)                                 addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0xC00000)) | ((val) << 22))
#define get_XVR_Reg0x06_cp                                      ((addXVR_Reg0x06 & 0xC00000) >> 22)

#define posXVR_Reg0x06_ictrlcp                                  24
#define bitXVR_Reg0x06_ictrlcp                                  0x7000000
#define set_XVR_Reg0x06_ictrlcp(val)                            addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0x7000000)) | ((val) << 24))
#define get_XVR_Reg0x06_ictrlcp                                 ((addXVR_Reg0x06 & 0x7000000) >> 24)

#define posXVR_Reg0x06_tristate                                 27
#define bitXVR_Reg0x06_tristate                                 0x8000000
#define set_XVR_Reg0x06_tristate(val)                           addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x06_tristate                               addXVR_Reg0x06 |= 0x8000000
#define clrf_XVR_Reg0x06_tristate                               addXVR_Reg0x06 &= ~0x8000000
#define get_XVR_Reg0x06_tristate                                ((addXVR_Reg0x06 & 0x8000000) >> 27)

#define posXVR_Reg0x06_selvcopol                                28
#define bitXVR_Reg0x06_selvcopol                                0x10000000
#define set_XVR_Reg0x06_selvcopol(val)                          addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x06_selvcopol                              addXVR_Reg0x06 |= 0x10000000
#define clrf_XVR_Reg0x06_selvcopol                              addXVR_Reg0x06 &= ~0x10000000
#define get_XVR_Reg0x06_selvcopol                               ((addXVR_Reg0x06 & 0x10000000) >> 28)

#define posXVR_Reg0x06_pfddelay                                 29
#define bitXVR_Reg0x06_pfddelay                                 0x60000000
#define set_XVR_Reg0x06_pfddelay(val)                           addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0x60000000)) | ((val) << 29))
#define get_XVR_Reg0x06_pfddelay                                ((addXVR_Reg0x06 & 0x60000000) >> 29)

#define posXVR_Reg0x06_CK_pol_sel                               31
#define bitXVR_Reg0x06_CK_pol_sel                               0x80000000
#define set_XVR_Reg0x06_CK_pol_sel(val)                         addXVR_Reg0x06 = ((addXVR_Reg0x06 & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x06_CK_pol_sel                             addXVR_Reg0x06 |= 0x80000000
#define clrf_XVR_Reg0x06_CK_pol_sel                             addXVR_Reg0x06 &= ~0x80000000
#define get_XVR_Reg0x06_CK_pol_sel                              ((addXVR_Reg0x06 & 0x80000000) >> 31)

//addXVR_Reg0x07
#define addXVR_Reg0x07                                          *((volatile unsigned long *) (0x4A800000+0x07*4))

#define posXVR_Reg0x07_adcspi                                   6
#define bitXVR_Reg0x07_adcspi                                   0xFFC0
#define set_XVR_Reg0x07_adcspi(val)                             addXVR_Reg0x07 = ((addXVR_Reg0x07 & (~0xFFC0)) | ((val) << 6))
#define get_XVR_Reg0x07_adcspi                                  ((addXVR_Reg0x07 & 0xFFC0) >> 6)

#define posXVR_Reg0x07_rxifspi                                  16
#define bitXVR_Reg0x07_rxifspi                                  0xFFFF0000
#define set_XVR_Reg0x07_rxifspi(val)                            addXVR_Reg0x07 = ((addXVR_Reg0x07 & (~0xFFFF0000)) | ((val) << 16))
#define get_XVR_Reg0x07_rxifspi                                 ((addXVR_Reg0x07 & 0xFFFF0000) >> 16)

//addXVR_Reg0x08
#define addXVR_Reg0x08                                          *((volatile unsigned long *) (0x4A800000+0x08*4))
#define posXVR_Reg0x08_txrfspi                                  0
#define bitXVR_Reg0x08_txrfspi                                  0x1FFF
#define set_XVR_Reg0x08_txrfspi(val)                            addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x1FFF)) | ((val) << 0))
#define get_XVR_Reg0x08_txrfspi                                 (addXVR_Reg0x08 & 0x1FFF)

#define posXVR_Reg0x08_vrefrxifldo                              13
#define bitXVR_Reg0x08_vrefrxifldo                              0x6000
#define set_XVR_Reg0x08_vrefrxifldo(val)                        addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x6000)) | ((val) << 13))
#define get_XVR_Reg0x08_vrefrxifldo                             ((addXVR_Reg0x08 & 0x6000) >> 13)


#define posXVR_Reg0x08_vreftxifldo                              16
#define bitXVR_Reg0x08_vreftxifldo                              0x30000
#define set_XVR_Reg0x08_vreftxifldo(val)                        addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x30000)) | ((val) << 16))
#define get_XVR_Reg0x08_vreftxifldo                             ((addXVR_Reg0x08 & 0x30000) >> 16)

#define posXVR_Reg0x08_vrefpaldo                                18
#define bitXVR_Reg0x08_vrefpaldo                                0xC0000
#define set_XVR_Reg0x08_vrefpaldo(val)                          addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0xC0000)) | ((val) << 18))
#define get_XVR_Reg0x08_vrefpaldo                               ((addXVR_Reg0x08 & 0xC0000) >> 18)

#define posXVR_Reg0x08_vreftxrfldo                              20
#define bitXVR_Reg0x08_vreftxrfldo                              0x300000
#define set_XVR_Reg0x08_vreftxrfldo(val)                        addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x300000)) | ((val) << 20))
#define get_XVR_Reg0x08_vreftxrfldo                             ((addXVR_Reg0x08 & 0x300000) >> 20)

#define posXVR_Reg0x08_TXtstmoden                               22
#define bitXVR_Reg0x08_TXtstmoden                               0x400000
#define set_XVR_Reg0x08_TXtstmoden(val)                         addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x400000)) | ((val) << 22))
#define setf_XVR_Reg0x08_TXtstmoden                             addXVR_Reg0x08 |= 0x400000
#define clrf_XVR_Reg0x08_TXtstmoden                             addXVR_Reg0x08 &= ~0x400000
#define get_XVR_Reg0x08_TXtstmoden                              ((addXVR_Reg0x08 & 0x400000) >> 22)

#define posXVR_Reg0x08_lnai                                     23
#define bitXVR_Reg0x08_lnai                                     0x3800000
#define set_XVR_Reg0x08_lnai(val)                               addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x3800000)) | ((val) << 23))
#define get_XVR_Reg0x08_lnai                                    ((addXVR_Reg0x08 & 0x3800000) >> 23)

#define posXVR_Reg0x08_agc_ths                                  26
#define bitXVR_Reg0x08_agc_ths                                  0x1C000000
#define set_XVR_Reg0x08_agc_ths(val)                            addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x1C000000)) | ((val) << 26))
#define get_XVR_Reg0x08_agc_ths                                 ((addXVR_Reg0x08 & 0x1C000000) >> 26)

#define posXVR_Reg0x08_v2ig                                     29
#define bitXVR_Reg0x08_v2ig                                     0x20000000
#define set_XVR_Reg0x08_v2ig(val)                               addXVR_Reg0x08 = ((addXVR_Reg0x08 & (~0x20000000)) | ((val) << 29))
#define setf_XVR_Reg0x08_v2ig                                   addXVR_Reg0x08 |= 0x20000000
#define clrf_XVR_Reg0x08_v2ig                                   addXVR_Reg0x08 &= ~0x20000000
#define get_XVR_Reg0x08_v2ig                                    ((addXVR_Reg0x08 & 0x20000000) >> 29)


//addXVR_Reg0x09
#define addXVR_Reg0x09                                          *((volatile unsigned long *) (0x4A800000+0x09*4))
#define posXVR_Reg0x09_fcal_start                               0
#define bitXVR_Reg0x09_fcal_start                               0x1
#define set_XVR_Reg0x09_fcal_start(val)                         addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x1)) | ((val) << 0))
#define setf_XVR_Reg0x09_fcal_start                             addXVR_Reg0x09 |= 0x1
#define clrf_XVR_Reg0x09_fcal_start                             addXVR_Reg0x09 &= ~0x1
#define get_XVR_Reg0x09_fcal_start                              (addXVR_Reg0x09 & 0x1)

#define posXVR_Reg0x09_pwd_cal                                  1
#define bitXVR_Reg0x09_pwd_cal                                  0x2
#define set_XVR_Reg0x09_pwd_cal(val)                            addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x2)) | ((val) << 1))
#define setf_XVR_Reg0x09_pwd_cal                                addXVR_Reg0x09 |= 0x2
#define clrf_XVR_Reg0x09_pwd_cal                                addXVR_Reg0x09 &= ~0x2
#define get_XVR_Reg0x09_pwd_cal                                 ((addXVR_Reg0x09 & 0x2) >> 1)

#define posXVR_Reg0x09_fcal_mode                                2
#define bitXVR_Reg0x09_fcal_mode                                0x4
#define set_XVR_Reg0x09_fcal_mode(val)                          addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x4)) | ((val) << 2))
#define setf_XVR_Reg0x09_fcal_mode                              addXVR_Reg0x09 |= 0x4
#define clrf_XVR_Reg0x09_fcal_mode                              addXVR_Reg0x09 &= ~0x4
#define get_XVR_Reg0x09_fcal_mode                               ((addXVR_Reg0x09 & 0x4) >> 2)

#define posXVR_Reg0x09_fcalpwden                                3
#define bitXVR_Reg0x09_fcalpwden                                0x8
#define set_XVR_Reg0x09_fcalpwden(val)                          addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x8)) | ((val) << 3))
#define setf_XVR_Reg0x09_fcalpwden                              addXVR_Reg0x09 |= 0x8
#define clrf_XVR_Reg0x09_fcalpwden                              addXVR_Reg0x09 &= ~0x8
#define get_XVR_Reg0x09_fcalpwden                               ((addXVR_Reg0x09 & 0x8) >> 3)

#define posXVR_Reg0x09_fcalm                                    4
#define bitXVR_Reg0x09_fcalm                                    0x1F0
#define set_XVR_Reg0x09_fcalm(val)                              addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x1F0)) | ((val) << 4))
#define get_XVR_Reg0x09_fcalm                                   ((addXVR_Reg0x09 & 0x1F0) >> 4)

#define posXVR_Reg0x09_rxpowctrl                                9
#define bitXVR_Reg0x09_rxpowctrl                                0x200
#define set_XVR_Reg0x09_rxpowctrl(val)                          addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x200)) | ((val) << 9))
#define setf_XVR_Reg0x09_rxpowctrl                              addXVR_Reg0x09 |= 0x200
#define clrf_XVR_Reg0x09_rxpowctrl                              addXVR_Reg0x09 &= ~0x200
#define get_XVR_Reg0x09_rxpowctrl                               ((addXVR_Reg0x09 & 0x200) >> 9)


#define posXVR_Reg0x09_spi_pwdDigPLL                            11
#define bitXVR_Reg0x09_spi_pwdDigPLL                            0x800
#define set_XVR_Reg0x09_spi_pwdDigPLL(val)                      addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x800)) | ((val) << 11))
#define setf_XVR_Reg0x09_spi_pwdDigPLL                          addXVR_Reg0x09 |= 0x800
#define clrf_XVR_Reg0x09_spi_pwdDigPLL                          addXVR_Reg0x09 &= ~0x800
#define get_XVR_Reg0x09_spi_pwdDigPLL                           ((addXVR_Reg0x09 & 0x800) >> 11)

#define posXVR_Reg0x09_spi_pwdPA                                12
#define bitXVR_Reg0x09_spi_pwdPA                                0x1000
#define set_XVR_Reg0x09_spi_pwdPA(val)                          addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x09_spi_pwdPA                              addXVR_Reg0x09 |= 0x1000
#define clrf_XVR_Reg0x09_spi_pwdPA                              addXVR_Reg0x09 &= ~0x1000
#define get_XVR_Reg0x09_spi_pwdPA                               ((addXVR_Reg0x09 & 0x1000) >> 12)

#define posXVR_Reg0x09_spi_pwdPABUF                             13
#define bitXVR_Reg0x09_spi_pwdPABUF                             0x2000
#define set_XVR_Reg0x09_spi_pwdPABUF(val)                       addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x09_spi_pwdPABUF                           addXVR_Reg0x09 |= 0x2000
#define clrf_XVR_Reg0x09_spi_pwdPABUF                           addXVR_Reg0x09 &= ~0x2000
#define get_XVR_Reg0x09_spi_pwdPABUF                            ((addXVR_Reg0x09 & 0x2000) >> 13)


#define posXVR_Reg0x09_spipwdrxRF                               16
#define bitXVR_Reg0x09_spipwdrxRF                               0x10000
#define set_XVR_Reg0x09_spipwdrxRF(val)                         addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x10000)) | ((val) << 16))
#define setf_XVR_Reg0x09_spipwdrxRF                             addXVR_Reg0x09 |= 0x10000
#define clrf_XVR_Reg0x09_spipwdrxRF                             addXVR_Reg0x09 &= ~0x10000
#define get_XVR_Reg0x09_spipwdrxRF                              ((addXVR_Reg0x09 & 0x10000) >> 16)

#define posXVR_Reg0x09_spipwdrxRFLDO                            17
#define bitXVR_Reg0x09_spipwdrxRFLDO                            0x20000
#define set_XVR_Reg0x09_spipwdrxRFLDO(val)                      addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x20000)) | ((val) << 17))
#define setf_XVR_Reg0x09_spipwdrxRFLDO                          addXVR_Reg0x09 |= 0x20000
#define clrf_XVR_Reg0x09_spipwdrxRFLDO                          addXVR_Reg0x09 &= ~0x20000
#define get_XVR_Reg0x09_spipwdrxRFLDO                           ((addXVR_Reg0x09 & 0x20000) >> 17)

#define posXVR_Reg0x09_spipwdtxRFLDO                            18
#define bitXVR_Reg0x09_spipwdtxRFLDO                            0x40000
#define set_XVR_Reg0x09_spipwdtxRFLDO(val)                      addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x40000)) | ((val) << 18))
#define setf_XVR_Reg0x09_spipwdtxRFLDO                          addXVR_Reg0x09 |= 0x40000
#define clrf_XVR_Reg0x09_spipwdtxRFLDO                          addXVR_Reg0x09 &= ~0x40000
#define get_XVR_Reg0x09_spipwdtxRFLDO                           ((addXVR_Reg0x09 & 0x40000) >> 18)

#define posXVR_Reg0x09_spipwdPLLLDO                             19
#define bitXVR_Reg0x09_spipwdPLLLDO                             0x80000
#define set_XVR_Reg0x09_spipwdPLLLDO(val)                       addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x80000)) | ((val) << 19))
#define setf_XVR_Reg0x09_spipwdPLLLDO                           addXVR_Reg0x09 |= 0x80000
#define clrf_XVR_Reg0x09_spipwdPLLLDO                           addXVR_Reg0x09 &= ~0x80000
#define get_XVR_Reg0x09_spipwdPLLLDO                            ((addXVR_Reg0x09 & 0x80000) >> 19)

#define posXVR_Reg0x09_spipwdRFVCOLDO                           20
#define bitXVR_Reg0x09_spipwdRFVCOLDO                           0x100000
#define set_XVR_Reg0x09_spipwdRFVCOLDO(val)                     addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x100000)) | ((val) << 20))
#define setf_XVR_Reg0x09_spipwdRFVCOLDO                         addXVR_Reg0x09 |= 0x100000
#define clrf_XVR_Reg0x09_spipwdRFVCOLDO                         addXVR_Reg0x09 &= ~0x100000
#define get_XVR_Reg0x09_spipwdRFVCOLDO                          ((addXVR_Reg0x09 & 0x100000) >> 20)

#define posXVR_Reg0x09_spipwdDigVCOLDO                          21
#define bitXVR_Reg0x09_spipwdDigVCOLDO                          0x200000
#define set_XVR_Reg0x09_spipwdDigVCOLDO(val)                    addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x200000)) | ((val) << 21))
#define setf_XVR_Reg0x09_spipwdDigVCOLDO                        addXVR_Reg0x09 |= 0x200000
#define clrf_XVR_Reg0x09_spipwdDigVCOLDO                        addXVR_Reg0x09 &= ~0x200000
#define get_XVR_Reg0x09_spipwdDigVCOLDO                         ((addXVR_Reg0x09 & 0x200000) >> 21)

#define posXVR_Reg0x09_spipwdrxIF                               22
#define bitXVR_Reg0x09_spipwdrxIF                               0x400000
#define set_XVR_Reg0x09_spipwdrxIF(val)                         addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x400000)) | ((val) << 22))
#define setf_XVR_Reg0x09_spipwdrxIF                             addXVR_Reg0x09 |= 0x400000
#define clrf_XVR_Reg0x09_spipwdrxIF                             addXVR_Reg0x09 &= ~0x400000
#define get_XVR_Reg0x09_spipwdrxIF                              ((addXVR_Reg0x09 & 0x400000) >> 22)

#define posXVR_Reg0x09_spipwdADC                                23
#define bitXVR_Reg0x09_spipwdADC                                0x800000
#define set_XVR_Reg0x09_spipwdADC(val)                          addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x800000)) | ((val) << 23))
#define setf_XVR_Reg0x09_spipwdADC                              addXVR_Reg0x09 |= 0x800000
#define clrf_XVR_Reg0x09_spipwdADC                              addXVR_Reg0x09 &= ~0x800000
#define get_XVR_Reg0x09_spipwdADC                               ((addXVR_Reg0x09 & 0x800000) >> 23)

#define posXVR_Reg0x09_spipwdTXIF                               24
#define bitXVR_Reg0x09_spipwdTXIF                               0x1000000
#define set_XVR_Reg0x09_spipwdTXIF(val)                         addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x1000000)) | ((val) << 24))
#define setf_XVR_Reg0x09_spipwdTXIF                             addXVR_Reg0x09 |= 0x1000000
#define clrf_XVR_Reg0x09_spipwdTXIF                             addXVR_Reg0x09 &= ~0x1000000
#define get_XVR_Reg0x09_spipwdTXIF                              ((addXVR_Reg0x09 & 0x1000000) >> 24)


#define posXVR_Reg0x09_spipwdDigLDO                             26
#define bitXVR_Reg0x09_spipwdDigLDO                             0x4000000
#define set_XVR_Reg0x09_spipwdDigLDO(val)                       addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x4000000)) | ((val) << 26))
#define setf_XVR_Reg0x09_spipwdDigLDO                           addXVR_Reg0x09 |= 0x4000000
#define clrf_XVR_Reg0x09_spipwdDigLDO                           addXVR_Reg0x09 &= ~0x4000000
#define get_XVR_Reg0x09_spipwdDigLDO                            ((addXVR_Reg0x09 & 0x4000000) >> 26)

#define posXVR_Reg0x09_spipwdCB                                 27
#define bitXVR_Reg0x09_spipwdCB                                 0x8000000
#define set_XVR_Reg0x09_spipwdCB(val)                           addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x09_spipwdCB                               addXVR_Reg0x09 |= 0x8000000
#define clrf_XVR_Reg0x09_spipwdCB                               addXVR_Reg0x09 &= ~0x8000000
#define get_XVR_Reg0x09_spipwdCB                                ((addXVR_Reg0x09 & 0x8000000) >> 27)

#define posXVR_Reg0x09_spipwdXTAL                               28
#define bitXVR_Reg0x09_spipwdXTAL                               0x10000000
#define set_XVR_Reg0x09_spipwdXTAL(val)                         addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x09_spipwdXTAL                             addXVR_Reg0x09 |= 0x10000000
#define clrf_XVR_Reg0x09_spipwdXTAL                             addXVR_Reg0x09 &= ~0x10000000
#define get_XVR_Reg0x09_spipwdXTAL                              ((addXVR_Reg0x09 & 0x10000000) >> 28)

#define posXVR_Reg0x09_spipwdBUCK                               29
#define bitXVR_Reg0x09_spipwdBUCK                               0x20000000
#define set_XVR_Reg0x09_spipwdBUCK(val)                         addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x20000000)) | ((val) << 29))
#define setf_XVR_Reg0x09_spipwdBUCK                             addXVR_Reg0x09 |= 0x20000000
#define clrf_XVR_Reg0x09_spipwdBUCK                             addXVR_Reg0x09 &= ~0x20000000
#define get_XVR_Reg0x09_spipwdBUCK                              ((addXVR_Reg0x09 & 0x20000000) >> 29)

#define posXVR_Reg0x09_spi_rfpllon                              30
#define bitXVR_Reg0x09_spi_rfpllon                              0x40000000
#define set_XVR_Reg0x09_spi_rfpllon(val)                        addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x09_spi_rfpllon                            addXVR_Reg0x09 |= 0x40000000
#define clrf_XVR_Reg0x09_spi_rfpllon                            addXVR_Reg0x09 &= ~0x40000000
#define get_XVR_Reg0x09_spi_rfpllon                             ((addXVR_Reg0x09 & 0x40000000) >> 30)

#define posXVR_Reg0x09_spi_rfvcoon                              31
#define bitXVR_Reg0x09_spi_rfvcoon                              0x80000000
#define set_XVR_Reg0x09_spi_rfvcoon(val)                        addXVR_Reg0x09 = ((addXVR_Reg0x09 & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x09_spi_rfvcoon                            addXVR_Reg0x09 |= 0x80000000
#define clrf_XVR_Reg0x09_spi_rfvcoon                            addXVR_Reg0x09 &= ~0x80000000
#define get_XVR_Reg0x09_spi_rfvcoon                             ((addXVR_Reg0x09 & 0x80000000) >> 31)

//addXVR_Reg0x0a
#define addXVR_Reg0x0a                                          *((volatile unsigned long *) (0x4A800000+0x0a*4))
#define posXVR_Reg0x0a_vrefpllLDO                               0
#define bitXVR_Reg0x0a_vrefpllLDO                               0x3
#define set_XVR_Reg0x0a_vrefpllLDO(val)                         addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0x3)) | ((val) << 0))
#define get_XVR_Reg0x0a_vrefpllLDO                              (addXVR_Reg0x0a & 0x3)

#define posXVR_Reg0x0a_vrefdigvcoLDO                            2
#define bitXVR_Reg0x0a_vrefdigvcoLDO                            0xC
#define set_XVR_Reg0x0a_vrefdigvcoLDO(val)                      addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0xC)) | ((val) << 2))
#define get_XVR_Reg0x0a_vrefdigvcoLDO                           ((addXVR_Reg0x0a & 0xC) >> 2)

#define posXVR_Reg0x0a_spi_trigger                              4
#define bitXVR_Reg0x0a_spi_trigger                              0x10
#define set_XVR_Reg0x0a_spi_trigger(val)                        addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0x10)) | ((val) << 4))
#define setf_XVR_Reg0x0a_spi_trigger                            addXVR_Reg0x0a |= 0x10
#define clrf_XVR_Reg0x0a_spi_trigger                            addXVR_Reg0x0a &= ~0x10
#define get_XVR_Reg0x0a_spi_trigger                             ((addXVR_Reg0x0a & 0x10) >> 4)

#define posXVR_Reg0x0a_vrefrfvcoLDO                             5
#define bitXVR_Reg0x0a_vrefrfvcoLDO                             0x60
#define set_XVR_Reg0x0a_vrefrfvcoLDO(val)                       addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0x60)) | ((val) << 5))
#define get_XVR_Reg0x0a_vrefrfvcoLDO                            ((addXVR_Reg0x0a & 0x60) >> 5)


#define posXVR_Reg0x0a_xtalamp                                  8
#define bitXVR_Reg0x0a_xtalamp                                  0x700
#define set_XVR_Reg0x0a_xtalamp(val)                            addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0x700)) | ((val) << 8))
#define get_XVR_Reg0x0a_xtalamp                                 ((addXVR_Reg0x0a & 0x700) >> 8)

#define posXVR_Reg0x0a_opt30uA1v                                11
#define bitXVR_Reg0x0a_opt30uA1v                                0x800
#define set_XVR_Reg0x0a_opt30uA1v(val)                          addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0x800)) | ((val) << 11))
#define setf_XVR_Reg0x0a_opt30uA1v                              addXVR_Reg0x0a |= 0x800
#define clrf_XVR_Reg0x0a_opt30uA1v                              addXVR_Reg0x0a &= ~0x800
#define get_XVR_Reg0x0a_opt30uA1v                               ((addXVR_Reg0x0a & 0x800) >> 11)

#define posXVR_Reg0x0a_cksel1v                                  12
#define bitXVR_Reg0x0a_cksel1v                                  0x1000
#define set_XVR_Reg0x0a_cksel1v(val)                            addXVR_Reg0x0a = ((addXVR_Reg0x0a & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x0a_cksel1v                                addXVR_Reg0x0a |= 0x1000
#define clrf_XVR_Reg0x0a_cksel1v                                addXVR_Reg0x0a &= ~0x1000
#define get_XVR_Reg0x0a_cksel1v                                 ((addXVR_Reg0x0a & 0x1000) >> 12)

//addXVR_Reg0x0b
#define addXVR_Reg0x0b                                          *((volatile unsigned long *) (0x4A800000+0x0b*4))

//addXVR_Reg0x0c
#define addXVR_Reg0x0c                                          *((volatile unsigned long *) (0x4A800000+0x0c*4))

//addXVR_Reg0x0d
#define addXVR_Reg0x0d                                          *((volatile unsigned long *) (0x4A800000+0x0d*4))

//addXVR_Reg0x0e
#define addXVR_Reg0x0e                                          *((volatile unsigned long *) (0x4A800000+0x0e*4))

//addXVR_Reg0x0f
#define addXVR_Reg0x0f                                          *((volatile unsigned long *) (0x4A800000+0x0f*4))

//addXVR_Reg0x10
#define addXVR_Reg0x10                                          *((volatile unsigned long *) (0x4A800000+0x10*4))
#define posXVR_Reg0x10_Xvr_ChipID                               0
#define bitXVR_Reg0x10_Xvr_ChipID                               0xFFFF
#define get_XVR_Reg0x10_Xvr_ChipID                              (addXVR_Reg0x10 & 0xFFFF)

//addXVR_Reg0x11
#define addXVR_Reg0x11                                          *((volatile unsigned long *) (0x4A800000+0x11*4))

#define posXVR_Reg0x11_pll_unlock                               1
#define bitXVR_Reg0x11_pll_unlock                               0x2
#define get_XVR_Reg0x11_pll_unlock                              ((addXVR_Reg0x11 & 0x2) >> 1)

#define posXVR_Reg0x11_pll_tuning                               2
#define bitXVR_Reg0x11_pll_tuning                               0x4
#define get_XVR_Reg0x11_pll_tuning                              ((addXVR_Reg0x11 & 0x4) >> 2)

#define posXVR_Reg0x11_cdyn_mup                                 3
#define bitXVR_Reg0x11_cdyn_mup                                 0x8
#define get_XVR_Reg0x11_cdyn_mup                                ((addXVR_Reg0x11 & 0x8) >> 3)

#define posXVR_Reg0x11_cdyn_mdn                                 4
#define bitXVR_Reg0x11_cdyn_mdn                                 0x10
#define get_XVR_Reg0x11_cdyn_mdn                                ((addXVR_Reg0x11 & 0x10) >> 4)

#define posXVR_Reg0x11_fvcon                                    5
#define bitXVR_Reg0x11_fvcon                                    0x20
#define get_XVR_Reg0x11_fvcon                                   ((addXVR_Reg0x11 & 0x20) >> 5)

#define posXVR_Reg0x11_adc_q_in                                 6
#define bitXVR_Reg0x11_adc_q_in                                 0x1C0
#define get_XVR_Reg0x11_adc_q_in                                ((addXVR_Reg0x11 & 0x1C0) >> 6)

#define posXVR_Reg0x11_adc_i_in                                 9
#define bitXVR_Reg0x11_adc_i_in                                 0xE00
#define get_XVR_Reg0x11_adc_i_in                                ((addXVR_Reg0x11 & 0xE00) >> 9)


//addXVR_Reg0x12
#define addXVR_Reg0x12                                          *((volatile unsigned long *) (0x4A800000+0x12*4))
#define posXVR_Reg0x12_rssi_o                                   0
#define bitXVR_Reg0x12_rssi_o                                   0xFF
#define get_XVR_Reg0x12_rssi_o                                  (addXVR_Reg0x12 & 0xFF)

#define posXVR_Reg0x12_adc_rssi_o                               8
#define bitXVR_Reg0x12_adc_rssi_o                               0x1F00
#define get_XVR_Reg0x12_adc_rssi_o                              ((addXVR_Reg0x12 & 0x1F00) >> 8)

#define posXVR_Reg0x12_ana_pga_rssi                             13
#define bitXVR_Reg0x12_ana_pga_rssi                             0x2000
#define get_XVR_Reg0x12_ana_pga_rssi                            ((addXVR_Reg0x12 & 0x2000) >> 13)

#define posXVR_Reg0x12_ana_buf_rssi                             14
#define bitXVR_Reg0x12_ana_buf_rssi                             0x4000
#define get_XVR_Reg0x12_ana_buf_rssi                            ((addXVR_Reg0x12 & 0x4000) >> 14)

#define posXVR_Reg0x12_ana_lna_rssi                             15
#define bitXVR_Reg0x12_ana_lna_rssi                             0x8000
#define get_XVR_Reg0x12_ana_lna_rssi                            ((addXVR_Reg0x12 & 0x8000) >> 15)

#define posXVR_Reg0x12_vco_kcal_o                               16
#define bitXVR_Reg0x12_vco_kcal_o                               0x1FFF0000
#define get_XVR_Reg0x12_vco_kcal_o                              ((addXVR_Reg0x12 & 0x1FFF0000) >> 16)

//addXVR_Reg0x13
#define addXVR_Reg0x13                                          *((volatile unsigned long *) (0x4A800000+0x13*4))
#define posXVR_Reg0x13_pre_dc_o                                 0
#define bitXVR_Reg0x13_pre_dc_o                                 0x1FF
#define get_XVR_Reg0x13_pre_dc_o                                (addXVR_Reg0x13 & 0x1FF)

#define posXVR_Reg0x13_track_dc_o                               16
#define bitXVR_Reg0x13_track_dc_o                               0x1FF0000
#define get_XVR_Reg0x13_track_dc_o                              ((addXVR_Reg0x13 & 0x1FF0000) >> 16)

//addXVR_Reg0x14
#define addXVR_Reg0x14                                          *((volatile unsigned long *) (0x4A800000+0x14*4))
#define posXVR_Reg0x14_tailer_dc_o                              0
#define bitXVR_Reg0x14_tailer_dc_o                              0x1FF
#define get_XVR_Reg0x14_tailer_dc_o                             (addXVR_Reg0x14 & 0x1FF)

#define posXVR_Reg0x14_cdyn_val                                 16
#define bitXVR_Reg0x14_cdyn_val                                 0xF0000
#define get_XVR_Reg0x14_cdyn_val                                ((addXVR_Reg0x14 & 0xF0000) >> 16)

#define posXVR_Reg0x14_match_phase_edr                          20
#define bitXVR_Reg0x14_match_phase_edr                          0x700000
#define get_XVR_Reg0x14_match_phase_edr                         ((addXVR_Reg0x14 & 0x700000) >> 20)

#define posXVR_Reg0x14_match_phase_gfsk                         24
#define bitXVR_Reg0x14_match_phase_gfsk                         0x7000000
#define get_XVR_Reg0x14_match_phase_gfsk                        ((addXVR_Reg0x14 & 0x7000000) >> 24)

#define posXVR_Reg0x14_unlock_status                            28
#define bitXVR_Reg0x14_unlock_status                            0x10000000
#define get_XVR_Reg0x14_unlock_status                           ((addXVR_Reg0x14 & 0x10000000) >> 28)

//addXVR_Reg0x15
#define addXVR_Reg0x15                                          *((volatile unsigned long *) (0x4A800000+0x15*4))

//addXVR_Reg0x16
#define addXVR_Reg0x16                                          *((volatile unsigned long *) (0x4A800000+0x16*4))

//addXVR_Reg0x17
#define addXVR_Reg0x17                                          *((volatile unsigned long *) (0x4A800000+0x17*4))
#define posXVR_Reg0x17_sync_err_bits                            0
#define bitXVR_Reg0x17_sync_err_bits                            0x1F
#define get_XVR_Reg0x17_sync_err_bits                           (addXVR_Reg0x17 & 0x1F)

//addXVR_Reg0x18
#define addXVR_Reg0x18                                          *((volatile unsigned long *) (0x4A800000+0x18*4))
#define posXVR_Reg0x18_rx_gain                                  0
#define bitXVR_Reg0x18_rx_gain                                  0x3FF
#define get_XVR_Reg0x18_rx_gain                                 (addXVR_Reg0x18 & 0x3FF)

//addXVR_Reg0x20
#define addXVR_Reg0x20                                          *((volatile unsigned long *) (0x4A800000+0x20*4))

//addXVR_Reg0x21
#define addXVR_Reg0x21                                          *((volatile unsigned long *) (0x4A800000+0x21*4))

//addXVR_Reg0x22
#define addXVR_Reg0x22                                          *((volatile unsigned long *) (0x4A800000+0x22*4))
#define posXVR_Reg0x22_edr_syncword                             0
#define bitXVR_Reg0x22_edr_syncword                             0x3FFFFFFF
#define set_XVR_Reg0x22_edr_syncword(val)                       addXVR_Reg0x22 = ((addXVR_Reg0x22 & (~0x3FFFFFFF)) | ((val) << 0))
#define get_XVR_Reg0x22_edr_syncword                            (addXVR_Reg0x22 & 0x3FFFFFFF)

#define posXVR_Reg0x22_auto_edrsyn                              30
#define bitXVR_Reg0x22_auto_edrsyn                              0x40000000
#define set_XVR_Reg0x22_auto_edrsyn(val)                        addXVR_Reg0x22 = ((addXVR_Reg0x22 & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x22_auto_edrsyn                            addXVR_Reg0x22 |= 0x40000000
#define clrf_XVR_Reg0x22_auto_edrsyn                            addXVR_Reg0x22 &= ~0x40000000
#define get_XVR_Reg0x22_auto_edrsyn                             ((addXVR_Reg0x22 & 0x40000000) >> 30)

#define posXVR_Reg0x22_auto_syncword                            31
#define bitXVR_Reg0x22_auto_syncword                            0x80000000
#define set_XVR_Reg0x22_auto_syncword(val)                      addXVR_Reg0x22 = ((addXVR_Reg0x22 & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x22_auto_syncword                          addXVR_Reg0x22 |= 0x80000000
#define clrf_XVR_Reg0x22_auto_syncword                          addXVR_Reg0x22 &= ~0x80000000
#define get_XVR_Reg0x22_auto_syncword                           ((addXVR_Reg0x22 & 0x80000000) >> 31)

//addXVR_Reg0x23
#define addXVR_Reg0x23                                          *((volatile unsigned long *) (0x4A800000+0x23*4))

//addXVR_Reg0x24
#define addXVR_Reg0x24                                          *((volatile unsigned long *) (0x4A800000+0x24*4))
#define posXVR_Reg0x24_cur_cfg_chn                              0
#define bitXVR_Reg0x24_cur_cfg_chn                              0x7F
#define set_XVR_Reg0x24_cur_cfg_chn(val)                        addXVR_Reg0x24 = ((addXVR_Reg0x24 & (~0x7F)) | ((val) << 0))
#define get_XVR_Reg0x24_cur_cfg_chn                             (addXVR_Reg0x24 & 0x7F)

#define posXVR_Reg0x24_cur_cfg_pwr                              8
#define bitXVR_Reg0x24_cur_cfg_pwr                              0x1F00
#define set_XVR_Reg0x24_cur_cfg_pwr(val)                        addXVR_Reg0x24 = ((addXVR_Reg0x24 & (~0x1F00)) | ((val) << 8))
#define get_XVR_Reg0x24_cur_cfg_pwr                             ((addXVR_Reg0x24 & 0x1F00) >> 8)

#define posXVR_Reg0x24_cur_cfg_win                              16
#define bitXVR_Reg0x24_cur_cfg_win                              0x1F0000
#define set_XVR_Reg0x24_cur_cfg_win(val)                        addXVR_Reg0x24 = ((addXVR_Reg0x24 & (~0x1F0000)) | ((val) << 16))
#define get_XVR_Reg0x24_cur_cfg_win                             ((addXVR_Reg0x24 & 0x1F0000) >> 16)

#define posXVR_Reg0x24_auto_syncwin                             21
#define bitXVR_Reg0x24_auto_syncwin                             0x200000
#define set_XVR_Reg0x24_auto_syncwin(val)                       addXVR_Reg0x24 = ((addXVR_Reg0x24 & (~0x200000)) | ((val) << 21))
#define setf_XVR_Reg0x24_auto_syncwin                           addXVR_Reg0x24 |= 0x200000
#define clrf_XVR_Reg0x24_auto_syncwin                           addXVR_Reg0x24 &= ~0x200000
#define get_XVR_Reg0x24_auto_syncwin                            ((addXVR_Reg0x24 & 0x200000) >> 21)

#define posXVR_Reg0x24_auto_txpower                             22
#define bitXVR_Reg0x24_auto_txpower                             0x400000
#define set_XVR_Reg0x24_auto_txpower(val)                       addXVR_Reg0x24 = ((addXVR_Reg0x24 & (~0x400000)) | ((val) << 22))
#define setf_XVR_Reg0x24_auto_txpower                           addXVR_Reg0x24 |= 0x400000
#define clrf_XVR_Reg0x24_auto_txpower                           addXVR_Reg0x24 &= ~0x400000
#define get_XVR_Reg0x24_auto_txpower                            ((addXVR_Reg0x24 & 0x400000) >> 22)

#define posXVR_Reg0x24_auto_chnn                                23
#define bitXVR_Reg0x24_auto_chnn                                0x800000
#define set_XVR_Reg0x24_auto_chnn(val)                          addXVR_Reg0x24 = ((addXVR_Reg0x24 & (~0x800000)) | ((val) << 23))
#define setf_XVR_Reg0x24_auto_chnn                              addXVR_Reg0x24 |= 0x800000
#define clrf_XVR_Reg0x24_auto_chnn                              addXVR_Reg0x24 &= ~0x800000
#define get_XVR_Reg0x24_auto_chnn                               ((addXVR_Reg0x24 & 0x800000) >> 23)

//addXVR_Reg0x25
#define addXVR_Reg0x25                                          *((volatile unsigned long *) (0x4A800000+0x25*4))
#define posXVR_Reg0x25_test_pattern                             0
#define bitXVR_Reg0x25_test_pattern                             0xFF
#define set_XVR_Reg0x25_test_pattern(val)                       addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x25_test_pattern                            (addXVR_Reg0x25 & 0xFF)

#define posXVR_Reg0x25_test_pattern_en                          8
#define bitXVR_Reg0x25_test_pattern_en                          0x100
#define set_XVR_Reg0x25_test_pattern_en(val)                    addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x100)) | ((val) << 8))
#define setf_XVR_Reg0x25_test_pattern_en                        addXVR_Reg0x25 |= 0x100
#define clrf_XVR_Reg0x25_test_pattern_en                        addXVR_Reg0x25 &= ~0x100
#define get_XVR_Reg0x25_test_pattern_en                         ((addXVR_Reg0x25 & 0x100) >> 8)

#define posXVR_Reg0x25_pn9_hold_en                              9
#define bitXVR_Reg0x25_pn9_hold_en                              0x200
#define set_XVR_Reg0x25_pn9_hold_en(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x200)) | ((val) << 9))
#define setf_XVR_Reg0x25_pn9_hold_en                            addXVR_Reg0x25 |= 0x200
#define clrf_XVR_Reg0x25_pn9_hold_en                            addXVR_Reg0x25 &= ~0x200
#define get_XVR_Reg0x25_pn9_hold_en                             ((addXVR_Reg0x25 & 0x200) >> 9)

#define posXVR_Reg0x25_pn9_recv_en                              10
#define bitXVR_Reg0x25_pn9_recv_en                              0x400
#define set_XVR_Reg0x25_pn9_recv_en(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x400)) | ((val) << 10))
#define setf_XVR_Reg0x25_pn9_recv_en                            addXVR_Reg0x25 |= 0x400
#define clrf_XVR_Reg0x25_pn9_recv_en                            addXVR_Reg0x25 &= ~0x400
#define get_XVR_Reg0x25_pn9_recv_en                             ((addXVR_Reg0x25 & 0x400) >> 10)

#define posXVR_Reg0x25_pn9_send_en                              11
#define bitXVR_Reg0x25_pn9_send_en                              0x800
#define set_XVR_Reg0x25_pn9_send_en(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x800)) | ((val) << 11))
#define setf_XVR_Reg0x25_pn9_send_en                            addXVR_Reg0x25 |= 0x800
#define clrf_XVR_Reg0x25_pn9_send_en                            addXVR_Reg0x25 &= ~0x800
#define get_XVR_Reg0x25_pn9_send_en                             ((addXVR_Reg0x25 & 0x800) >> 11)

#define posXVR_Reg0x25_test_tx_mode                             12
#define bitXVR_Reg0x25_test_tx_mode                             0x1000
#define set_XVR_Reg0x25_test_tx_mode(val)                       addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x25_test_tx_mode                           addXVR_Reg0x25 |= 0x1000
#define clrf_XVR_Reg0x25_test_tx_mode                           addXVR_Reg0x25 &= ~0x1000
#define get_XVR_Reg0x25_test_tx_mode                            ((addXVR_Reg0x25 & 0x1000) >> 12)

#define posXVR_Reg0x25_test_radio                               13
#define bitXVR_Reg0x25_test_radio                               0x2000
#define set_XVR_Reg0x25_test_radio(val)                         addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x25_test_radio                             addXVR_Reg0x25 |= 0x2000
#define clrf_XVR_Reg0x25_test_radio                             addXVR_Reg0x25 &= ~0x2000
#define get_XVR_Reg0x25_test_radio                              ((addXVR_Reg0x25 & 0x2000) >> 13)

#define posXVR_Reg0x25_test_edr2                                14
#define bitXVR_Reg0x25_test_edr2                                0x4000
#define set_XVR_Reg0x25_test_edr2(val)                          addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x4000)) | ((val) << 14))
#define setf_XVR_Reg0x25_test_edr2                              addXVR_Reg0x25 |= 0x4000
#define clrf_XVR_Reg0x25_test_edr2                              addXVR_Reg0x25 &= ~0x4000
#define get_XVR_Reg0x25_test_edr2                               ((addXVR_Reg0x25 & 0x4000) >> 14)

#define posXVR_Reg0x25_test_edr3                                15
#define bitXVR_Reg0x25_test_edr3                                0x8000
#define set_XVR_Reg0x25_test_edr3(val)                          addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x8000)) | ((val) << 15))
#define setf_XVR_Reg0x25_test_edr3                              addXVR_Reg0x25 |= 0x8000
#define clrf_XVR_Reg0x25_test_edr3                              addXVR_Reg0x25 &= ~0x8000
#define get_XVR_Reg0x25_test_edr3                               ((addXVR_Reg0x25 & 0x8000) >> 15)

#define posXVR_Reg0x25_vco_kcal_en                              16
#define bitXVR_Reg0x25_vco_kcal_en                              0x10000
#define set_XVR_Reg0x25_vco_kcal_en(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x10000)) | ((val) << 16))
#define setf_XVR_Reg0x25_vco_kcal_en                            addXVR_Reg0x25 |= 0x10000
#define clrf_XVR_Reg0x25_vco_kcal_en                            addXVR_Reg0x25 &= ~0x10000
#define get_XVR_Reg0x25_vco_kcal_en                             ((addXVR_Reg0x25 & 0x10000) >> 16)

#define posXVR_Reg0x25_rssi_cal_en                              17
#define bitXVR_Reg0x25_rssi_cal_en                              0x20000
#define set_XVR_Reg0x25_rssi_cal_en(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x20000)) | ((val) << 17))
#define setf_XVR_Reg0x25_rssi_cal_en                            addXVR_Reg0x25 |= 0x20000
#define clrf_XVR_Reg0x25_rssi_cal_en                            addXVR_Reg0x25 &= ~0x20000
#define get_XVR_Reg0x25_rssi_cal_en                             ((addXVR_Reg0x25 & 0x20000) >> 17)

#define posXVR_Reg0x25_test_ble2m                               18
#define bitXVR_Reg0x25_test_ble2m                               0x40000
#define set_XVR_Reg0x25_test_ble2m(val)                         addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x40000)) | ((val) << 18))
#define setf_XVR_Reg0x25_test_ble2m                             addXVR_Reg0x25 |= 0x40000
#define clrf_XVR_Reg0x25_test_ble2m                             addXVR_Reg0x25 &= ~0x40000
#define get_XVR_Reg0x25_test_ble2m                              ((addXVR_Reg0x25 & 0x40000) >> 18)

#define posXVR_Reg0x25_lr_pkt_test                              19
#define bitXVR_Reg0x25_lr_pkt_test                              0x80000
#define set_XVR_Reg0x25_lr_pkt_test(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x80000)) | ((val) << 19))
#define setf_XVR_Reg0x25_lr_pkt_test                            addXVR_Reg0x25 |= 0x80000
#define clrf_XVR_Reg0x25_lr_pkt_test                            addXVR_Reg0x25 &= ~0x80000
#define get_XVR_Reg0x25_lr_pkt_test                             ((addXVR_Reg0x25 & 0x80000) >> 19)

#define posXVR_Reg0x25_lr_test                                  20
#define bitXVR_Reg0x25_lr_test                                  0x100000
#define set_XVR_Reg0x25_lr_test(val)                            addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x100000)) | ((val) << 20))
#define setf_XVR_Reg0x25_lr_test                                addXVR_Reg0x25 |= 0x100000
#define clrf_XVR_Reg0x25_lr_test                                addXVR_Reg0x25 &= ~0x100000
#define get_XVR_Reg0x25_lr_test                                 ((addXVR_Reg0x25 & 0x100000) >> 20)


#define posXVR_Reg0x25_test_select                              24
#define bitXVR_Reg0x25_test_select                              0x7000000
#define set_XVR_Reg0x25_test_select(val)                        addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x7000000)) | ((val) << 24))
#define get_XVR_Reg0x25_test_select                             ((addXVR_Reg0x25 & 0x7000000) >> 24)

#define posXVR_Reg0x25_pn9_sync_mode                            27
#define bitXVR_Reg0x25_pn9_sync_mode                            0x8000000
#define set_XVR_Reg0x25_pn9_sync_mode(val)                      addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x25_pn9_sync_mode                          addXVR_Reg0x25 |= 0x8000000
#define clrf_XVR_Reg0x25_pn9_sync_mode                          addXVR_Reg0x25 &= ~0x8000000
#define get_XVR_Reg0x25_pn9_sync_mode                           ((addXVR_Reg0x25 & 0x8000000) >> 27)

#define posXVR_Reg0x25_cap_adc_en                               27
#define bitXVR_Reg0x25_cap_adc_en                               0x8000000
#define set_XVR_Reg0x25_cap_adc_en(val)                         addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x25_cap_adc_en                             addXVR_Reg0x25 |= 0x8000000
#define clrf_XVR_Reg0x25_cap_adc_en                             addXVR_Reg0x25 &= ~0x8000000
#define get_XVR_Reg0x25_cap_adc_en                              ((addXVR_Reg0x25 & 0x8000000) >> 27)

#define posXVR_Reg0x25_cap_adc_edge                             28
#define bitXVR_Reg0x25_cap_adc_edge                             0x10000000
#define set_XVR_Reg0x25_cap_adc_edge(val)                       addXVR_Reg0x25 = ((addXVR_Reg0x25 & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x25_cap_adc_edge                           addXVR_Reg0x25 |= 0x10000000
#define clrf_XVR_Reg0x25_cap_adc_edge                           addXVR_Reg0x25 &= ~0x10000000
#define get_XVR_Reg0x25_cap_adc_edge                            ((addXVR_Reg0x25 & 0x10000000) >> 28)

//addXVR_Reg0x26
#define addXVR_Reg0x26                                          *((volatile unsigned long *) (0x4A800000+0x26*4))
#define posXVR_Reg0x26_set_kmodcoef                             0
#define bitXVR_Reg0x26_set_kmodcoef                             0x1FF
#define set_XVR_Reg0x26_set_kmodcoef(val)                       addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x1FF)) | ((val) << 0))
#define get_XVR_Reg0x26_set_kmodcoef                            (addXVR_Reg0x26 & 0x1FF)

#define posXVR_Reg0x26_force_iqmod                              9
#define bitXVR_Reg0x26_force_iqmod                              0x200
#define set_XVR_Reg0x26_force_iqmod(val)                        addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x200)) | ((val) << 9))
#define setf_XVR_Reg0x26_force_iqmod                            addXVR_Reg0x26 |= 0x200
#define clrf_XVR_Reg0x26_force_iqmod                            addXVR_Reg0x26 &= ~0x200
#define get_XVR_Reg0x26_force_iqmod                             ((addXVR_Reg0x26 & 0x200) >> 9)

#define posXVR_Reg0x26_rx_bit_phase                             10
#define bitXVR_Reg0x26_rx_bit_phase                             0x400
#define set_XVR_Reg0x26_rx_bit_phase(val)                       addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x400)) | ((val) << 10))
#define setf_XVR_Reg0x26_rx_bit_phase                           addXVR_Reg0x26 |= 0x400
#define clrf_XVR_Reg0x26_rx_bit_phase                           addXVR_Reg0x26 &= ~0x400
#define get_XVR_Reg0x26_rx_bit_phase                            ((addXVR_Reg0x26 & 0x400) >> 10)

#define posXVR_Reg0x26_tx_bit_phase                             11
#define bitXVR_Reg0x26_tx_bit_phase                             0x800
#define set_XVR_Reg0x26_tx_bit_phase(val)                       addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x800)) | ((val) << 11))
#define setf_XVR_Reg0x26_tx_bit_phase                           addXVR_Reg0x26 |= 0x800
#define clrf_XVR_Reg0x26_tx_bit_phase                           addXVR_Reg0x26 &= ~0x800
#define get_XVR_Reg0x26_tx_bit_phase                            ((addXVR_Reg0x26 & 0x800) >> 11)


#define posXVR_Reg0x26_txb_reverse                              13
#define bitXVR_Reg0x26_txb_reverse                              0x2000
#define set_XVR_Reg0x26_txb_reverse(val)                        addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x26_txb_reverse                            addXVR_Reg0x26 |= 0x2000
#define clrf_XVR_Reg0x26_txb_reverse                            addXVR_Reg0x26 &= ~0x2000
#define get_XVR_Reg0x26_txb_reverse                             ((addXVR_Reg0x26 & 0x2000) >> 13)



#define posXVR_Reg0x26_sdm23sel_cfg                             27
#define bitXVR_Reg0x26_sdm23sel_cfg                             0x8000000
#define set_XVR_Reg0x26_sdm23sel_cfg(val)                       addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x26_sdm23sel_cfg                           addXVR_Reg0x26 |= 0x8000000
#define clrf_XVR_Reg0x26_sdm23sel_cfg                           addXVR_Reg0x26 &= ~0x8000000
#define get_XVR_Reg0x26_sdm23sel_cfg                            ((addXVR_Reg0x26 & 0x8000000) >> 27)

#define posXVR_Reg0x26_clkedge_cfg                              28
#define bitXVR_Reg0x26_clkedge_cfg                              0x10000000
#define set_XVR_Reg0x26_clkedge_cfg(val)                        addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x26_clkedge_cfg                            addXVR_Reg0x26 |= 0x10000000
#define clrf_XVR_Reg0x26_clkedge_cfg                            addXVR_Reg0x26 &= ~0x10000000
#define get_XVR_Reg0x26_clkedge_cfg                             ((addXVR_Reg0x26 & 0x10000000) >> 28)

#define posXVR_Reg0x26_pn25ena_cfg                              29
#define bitXVR_Reg0x26_pn25ena_cfg                              0x20000000
#define set_XVR_Reg0x26_pn25ena_cfg(val)                        addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x20000000)) | ((val) << 29))
#define setf_XVR_Reg0x26_pn25ena_cfg                            addXVR_Reg0x26 |= 0x20000000
#define clrf_XVR_Reg0x26_pn25ena_cfg                            addXVR_Reg0x26 &= ~0x20000000
#define get_XVR_Reg0x26_pn25ena_cfg                             ((addXVR_Reg0x26 & 0x20000000) >> 29)

#define posXVR_Reg0x26_rx_if_select                             30
#define bitXVR_Reg0x26_rx_if_select                             0x40000000
#define set_XVR_Reg0x26_rx_if_select(val)                       addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x26_rx_if_select                           addXVR_Reg0x26 |= 0x40000000
#define clrf_XVR_Reg0x26_rx_if_select                           addXVR_Reg0x26 &= ~0x40000000
#define get_XVR_Reg0x26_rx_if_select                            ((addXVR_Reg0x26 & 0x40000000) >> 30)

#define posXVR_Reg0x26_syncdet_phase                            31
#define bitXVR_Reg0x26_syncdet_phase                            0x80000000
#define set_XVR_Reg0x26_syncdet_phase(val)                      addXVR_Reg0x26 = ((addXVR_Reg0x26 & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x26_syncdet_phase                          addXVR_Reg0x26 |= 0x80000000
#define clrf_XVR_Reg0x26_syncdet_phase                          addXVR_Reg0x26 &= ~0x80000000
#define get_XVR_Reg0x26_syncdet_phase                           ((addXVR_Reg0x26 & 0x80000000) >> 31)

//addXVR_Reg0x27
#define addXVR_Reg0x27                                          *((volatile unsigned long *) (0x4A800000+0x27*4))
#define posXVR_Reg0x27_mod2dac_gain                             0
#define bitXVR_Reg0x27_mod2dac_gain                             0xFF
#define set_XVR_Reg0x27_mod2dac_gain(val)                       addXVR_Reg0x27 = ((addXVR_Reg0x27 & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x27_mod2dac_gain                            (addXVR_Reg0x27 & 0xFF)

#define posXVR_Reg0x27_mod2sdm_coef                             8
#define bitXVR_Reg0x27_mod2sdm_coef                             0xFF00
#define set_XVR_Reg0x27_mod2sdm_coef(val)                       addXVR_Reg0x27 = ((addXVR_Reg0x27 & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x27_mod2sdm_coef                            ((addXVR_Reg0x27 & 0xFF00) >> 8)

#define posXVR_Reg0x27_mod2dac_delay                            16
#define bitXVR_Reg0x27_mod2dac_delay                            0xF0000
#define set_XVR_Reg0x27_mod2dac_delay(val)                      addXVR_Reg0x27 = ((addXVR_Reg0x27 & (~0xF0000)) | ((val) << 16))
#define get_XVR_Reg0x27_mod2dac_delay                           ((addXVR_Reg0x27 & 0xF0000) >> 16)

#define posXVR_Reg0x27_mod2sdm_delay                            20
#define bitXVR_Reg0x27_mod2sdm_delay                            0xF00000
#define set_XVR_Reg0x27_mod2sdm_delay(val)                      addXVR_Reg0x27 = ((addXVR_Reg0x27 & (~0xF00000)) | ((val) << 20))
#define get_XVR_Reg0x27_mod2sdm_delay                           ((addXVR_Reg0x27 & 0xF00000) >> 20)

#define posXVR_Reg0x27_NC                                       24
#define bitXVR_Reg0x27_NC                                       0xFF000000
#define set_XVR_Reg0x27_NC(val)                                 addXVR_Reg0x27 = ((addXVR_Reg0x27 & (~0xFF000000)) | ((val) << 24))
#define get_XVR_Reg0x27_NC                                      ((addXVR_Reg0x27 & 0xFF000000) >> 24)

//addXVR_Reg0x28
#define addXVR_Reg0x28                                          *((volatile unsigned long *) (0x4A800000+0x28*4))
#define posXVR_Reg0x28_Tdly_PLLms                               0
#define bitXVR_Reg0x28_Tdly_PLLms                               0xFF
#define set_XVR_Reg0x28_Tdly_PLLms(val)                         addXVR_Reg0x28 = ((addXVR_Reg0x28 & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x28_Tdly_PLLms                              (addXVR_Reg0x28 & 0xFF)

#define posXVR_Reg0x28_Tdly_PLLfs                               8
#define bitXVR_Reg0x28_Tdly_PLLfs                               0xFF00
#define set_XVR_Reg0x28_Tdly_PLLfs(val)                         addXVR_Reg0x28 = ((addXVR_Reg0x28 & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x28_Tdly_PLLfs                              ((addXVR_Reg0x28 & 0xFF00) >> 8)

#define posXVR_Reg0x28_Tdly_PLLen                               16
#define bitXVR_Reg0x28_Tdly_PLLen                               0xFF0000
#define set_XVR_Reg0x28_Tdly_PLLen(val)                         addXVR_Reg0x28 = ((addXVR_Reg0x28 & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x28_Tdly_PLLen                              ((addXVR_Reg0x28 & 0xFF0000) >> 16)

#define posXVR_Reg0x28_Tdly_VCOen                               24
#define bitXVR_Reg0x28_Tdly_VCOen                               0xFF000000
#define set_XVR_Reg0x28_Tdly_VCOen(val)                         addXVR_Reg0x28 = ((addXVR_Reg0x28 & (~0xFF000000)) | ((val) << 24))
#define get_XVR_Reg0x28_Tdly_VCOen                              ((addXVR_Reg0x28 & 0xFF000000) >> 24)

//addXVR_Reg0x29
#define addXVR_Reg0x29                                          *((volatile unsigned long *) (0x4A800000+0x29*4))
#define posXVR_Reg0x29_State_index                              0
#define bitXVR_Reg0x29_State_index                              0x1FF
#define set_XVR_Reg0x29_State_index(val)                        addXVR_Reg0x29 = ((addXVR_Reg0x29 & (~0x1FF)) | ((val) << 0))
#define get_XVR_Reg0x29_State_index                             (addXVR_Reg0x29 & 0x1FF)

#define posXVR_Reg0x29_tdly_errdet                              9
#define bitXVR_Reg0x29_tdly_errdet                              0x3E00
#define set_XVR_Reg0x29_tdly_errdet(val)                        addXVR_Reg0x29 = ((addXVR_Reg0x29 & (~0x3E00)) | ((val) << 9))
#define get_XVR_Reg0x29_tdly_errdet                             ((addXVR_Reg0x29 & 0x3E00) >> 9)

//addXVR_Reg0x2a
#define addXVR_Reg0x2a                                          *((volatile unsigned long *) (0x4A800000+0x2a*4))
#define posXVR_Reg0x2a_rxslot_time                              0
#define bitXVR_Reg0x2a_rxslot_time                              0xFF
#define set_XVR_Reg0x2a_rxslot_time(val)                        addXVR_Reg0x2a = ((addXVR_Reg0x2a & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x2a_rxslot_time                             (addXVR_Reg0x2a & 0xFF)

#define posXVR_Reg0x2a_txslot_time                              8
#define bitXVR_Reg0x2a_txslot_time                              0xFF00
#define set_XVR_Reg0x2a_txslot_time(val)                        addXVR_Reg0x2a = ((addXVR_Reg0x2a & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x2a_txslot_time                             ((addXVR_Reg0x2a & 0xFF00) >> 8)

#define posXVR_Reg0x2a_radion_time                              16
#define bitXVR_Reg0x2a_radion_time                              0xFF0000
#define set_XVR_Reg0x2a_radion_time(val)                        addXVR_Reg0x2a = ((addXVR_Reg0x2a & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x2a_radion_time                             ((addXVR_Reg0x2a & 0xFF0000) >> 16)

#define posXVR_Reg0x2a_lpo_clk_div                              24
#define bitXVR_Reg0x2a_lpo_clk_div                              0x1000000
#define set_XVR_Reg0x2a_lpo_clk_div(val)                        addXVR_Reg0x2a = ((addXVR_Reg0x2a & (~0x1000000)) | ((val) << 24))
#define setf_XVR_Reg0x2a_lpo_clk_div                            addXVR_Reg0x2a |= 0x1000000
#define clrf_XVR_Reg0x2a_lpo_clk_div                            addXVR_Reg0x2a &= ~0x1000000
#define get_XVR_Reg0x2a_lpo_clk_div                             ((addXVR_Reg0x2a & 0x1000000) >> 24)

#define posXVR_Reg0x2a_bb_sel_index                             25
#define bitXVR_Reg0x2a_bb_sel_index                             0xE000000
#define set_XVR_Reg0x2a_bb_sel_index(val)                       addXVR_Reg0x2a = ((addXVR_Reg0x2a & (~0xE000000)) | ((val) << 25))
#define get_XVR_Reg0x2a_bb_sel_index                            ((addXVR_Reg0x2a & 0xE000000) >> 25)

#define posXVR_Reg0x2a_bb_sel_enable                            28
#define bitXVR_Reg0x2a_bb_sel_enable                            0x10000000
#define set_XVR_Reg0x2a_bb_sel_enable(val)                      addXVR_Reg0x2a = ((addXVR_Reg0x2a & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x2a_bb_sel_enable                          addXVR_Reg0x2a |= 0x10000000
#define clrf_XVR_Reg0x2a_bb_sel_enable                          addXVR_Reg0x2a &= ~0x10000000
#define get_XVR_Reg0x2a_bb_sel_enable                           ((addXVR_Reg0x2a & 0x10000000) >> 28)

//addXVR_Reg0x2b
#define addXVR_Reg0x2b                                          *((volatile unsigned long *) (0x4A800000+0x2b*4))
#define posXVR_Reg0x2b_tx_slotime_rw                            16
#define bitXVR_Reg0x2b_tx_slotime_rw                            0xFF0000
#define set_XVR_Reg0x2b_tx_slotime_rw(val)                      addXVR_Reg0x2b = ((addXVR_Reg0x2b & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x2b_tx_slotime_rw                           ((addXVR_Reg0x2b & 0xFF0000) >> 16)

#define posXVR_Reg0x2b_rx_slotime_rw                            24
#define bitXVR_Reg0x2b_rx_slotime_rw                            0xFF000000
#define set_XVR_Reg0x2b_rx_slotime_rw(val)                      addXVR_Reg0x2b = ((addXVR_Reg0x2b & (~0xFF000000)) | ((val) << 24))
#define get_XVR_Reg0x2b_rx_slotime_rw                           ((addXVR_Reg0x2b & 0xFF000000) >> 24)

//addXVR_Reg0x2c
#define addXVR_Reg0x2c                                          *((volatile unsigned long *) (0x4A800000+0x2c*4))
#define posXVR_Reg0x2c_rx_edr_time_rw                           0
#define bitXVR_Reg0x2c_rx_edr_time_rw                           0xF
#define set_XVR_Reg0x2c_rx_edr_time_rw(val)                     addXVR_Reg0x2c = ((addXVR_Reg0x2c & (~0xF)) | ((val) << 0))
#define get_XVR_Reg0x2c_rx_edr_time_rw                          (addXVR_Reg0x2c & 0xF)

#define posXVR_Reg0x2c_tx_edr_time_rw                           4
#define bitXVR_Reg0x2c_tx_edr_time_rw                           0xF0
#define set_XVR_Reg0x2c_tx_edr_time_rw(val)                     addXVR_Reg0x2c = ((addXVR_Reg0x2c & (~0xF0)) | ((val) << 4))
#define get_XVR_Reg0x2c_tx_edr_time_rw                          ((addXVR_Reg0x2c & 0xF0) >> 4)

#define posXVR_Reg0x2c_rx_slotime_bk24                          8
#define bitXVR_Reg0x2c_rx_slotime_bk24                          0xFF00
#define set_XVR_Reg0x2c_rx_slotime_bk24(val)                    addXVR_Reg0x2c = ((addXVR_Reg0x2c & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x2c_rx_slotime_bk24                         ((addXVR_Reg0x2c & 0xFF00) >> 8)

#define posXVR_Reg0x2c_tx_slotime_bk24                          16
#define bitXVR_Reg0x2c_tx_slotime_bk24                          0xFF0000
#define set_XVR_Reg0x2c_tx_slotime_bk24(val)                    addXVR_Reg0x2c = ((addXVR_Reg0x2c & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x2c_tx_slotime_bk24                         ((addXVR_Reg0x2c & 0xFF0000) >> 16)

#define posXVR_Reg0x2c_radion_time_bk24                         24
#define bitXVR_Reg0x2c_radion_time_bk24                         0xFF000000
#define set_XVR_Reg0x2c_radion_time_bk24(val)                   addXVR_Reg0x2c = ((addXVR_Reg0x2c & (~0xFF000000)) | ((val) << 24))
#define get_XVR_Reg0x2c_radion_time_bk24                        ((addXVR_Reg0x2c & 0xFF000000) >> 24)

//addXVR_Reg0x2d
#define addXVR_Reg0x2d                                          *((volatile unsigned long *) (0x4A800000+0x2d*4))
#define posXVR_Reg0x2d_tdly_pa0fdn                              0
#define bitXVR_Reg0x2d_tdly_pa0fdn                              0x1F
#define set_XVR_Reg0x2d_tdly_pa0fdn(val)                        addXVR_Reg0x2d = ((addXVR_Reg0x2d & (~0x1F)) | ((val) << 0))
#define get_XVR_Reg0x2d_tdly_pa0fdn                             (addXVR_Reg0x2d & 0x1F)

#define posXVR_Reg0x2d_tdly_pa0rup                              5
#define bitXVR_Reg0x2d_tdly_pa0rup                              0x3E0
#define set_XVR_Reg0x2d_tdly_pa0rup(val)                        addXVR_Reg0x2d = ((addXVR_Reg0x2d & (~0x3E0)) | ((val) << 5))
#define get_XVR_Reg0x2d_tdly_pa0rup                             ((addXVR_Reg0x2d & 0x3E0) >> 5)

#define posXVR_Reg0x2d_pa0_dnslope                              10
#define bitXVR_Reg0x2d_pa0_dnslope                              0x1C00
#define set_XVR_Reg0x2d_pa0_dnslope(val)                        addXVR_Reg0x2d = ((addXVR_Reg0x2d & (~0x1C00)) | ((val) << 10))
#define get_XVR_Reg0x2d_pa0_dnslope                             ((addXVR_Reg0x2d & 0x1C00) >> 10)

#define posXVR_Reg0x2d_pa0_upslope                              13
#define bitXVR_Reg0x2d_pa0_upslope                              0xE000
#define set_XVR_Reg0x2d_pa0_upslope(val)                        addXVR_Reg0x2d = ((addXVR_Reg0x2d & (~0xE000)) | ((val) << 13))
#define get_XVR_Reg0x2d_pa0_upslope                             ((addXVR_Reg0x2d & 0xE000) >> 13)

#define posXVR_Reg0x2d_tdly_pa0off                              16
#define bitXVR_Reg0x2d_tdly_pa0off                              0x1F0000
#define set_XVR_Reg0x2d_tdly_pa0off(val)                        addXVR_Reg0x2d = ((addXVR_Reg0x2d & (~0x1F0000)) | ((val) << 16))
#define get_XVR_Reg0x2d_tdly_pa0off                             ((addXVR_Reg0x2d & 0x1F0000) >> 16)

#define posXVR_Reg0x2d_tdly_pa0on                               21
#define bitXVR_Reg0x2d_tdly_pa0on                               0x3E00000
#define set_XVR_Reg0x2d_tdly_pa0on(val)                         addXVR_Reg0x2d = ((addXVR_Reg0x2d & (~0x3E00000)) | ((val) << 21))
#define get_XVR_Reg0x2d_tdly_pa0on                              ((addXVR_Reg0x2d & 0x3E00000) >> 21)

//addXVR_Reg0x30
#define addXVR_Reg0x30                                          *((volatile unsigned long *) (0x4A800000+0x30*4))
#define posXVR_Reg0x30_tdly_13m_num                             0
#define bitXVR_Reg0x30_tdly_13m_num                             0xF
#define set_XVR_Reg0x30_tdly_13m_num(val)                       addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0xF)) | ((val) << 0))
#define get_XVR_Reg0x30_tdly_13m_num                            (addXVR_Reg0x30 & 0xF)

#define posXVR_Reg0x30_tdly_1m_num                              4
#define bitXVR_Reg0x30_tdly_1m_num                              0x70
#define set_XVR_Reg0x30_tdly_1m_num(val)                        addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0x70)) | ((val) << 4))
#define get_XVR_Reg0x30_tdly_1m_num                             ((addXVR_Reg0x30 & 0x70) >> 4)

#define posXVR_Reg0x30_rrc_sel                                  7
#define bitXVR_Reg0x30_rrc_sel                                  0x80
#define set_XVR_Reg0x30_rrc_sel(val)                            addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0x80)) | ((val) << 7))
#define setf_XVR_Reg0x30_rrc_sel                                addXVR_Reg0x30 |= 0x80
#define clrf_XVR_Reg0x30_rrc_sel                                addXVR_Reg0x30 &= ~0x80
#define get_XVR_Reg0x30_rrc_sel                                 ((addXVR_Reg0x30 & 0x80) >> 7)

#define posXVR_Reg0x30_iq_sw_edr                                8
#define bitXVR_Reg0x30_iq_sw_edr                                0x100
#define set_XVR_Reg0x30_iq_sw_edr(val)                          addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0x100)) | ((val) << 8))
#define setf_XVR_Reg0x30_iq_sw_edr                              addXVR_Reg0x30 |= 0x100
#define clrf_XVR_Reg0x30_iq_sw_edr                              addXVR_Reg0x30 &= ~0x100
#define get_XVR_Reg0x30_iq_sw_edr                               ((addXVR_Reg0x30 & 0x100) >> 8)

#define posXVR_Reg0x30_iq_sw_gfsk                               9
#define bitXVR_Reg0x30_iq_sw_gfsk                               0x200
#define set_XVR_Reg0x30_iq_sw_gfsk(val)                         addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0x200)) | ((val) << 9))
#define setf_XVR_Reg0x30_iq_sw_gfsk                             addXVR_Reg0x30 |= 0x200
#define clrf_XVR_Reg0x30_iq_sw_gfsk                             addXVR_Reg0x30 &= ~0x200
#define get_XVR_Reg0x30_iq_sw_gfsk                              ((addXVR_Reg0x30 & 0x200) >> 9)

#define posXVR_Reg0x30_delta_pi_sel                             10
#define bitXVR_Reg0x30_delta_pi_sel                             0xC00
#define set_XVR_Reg0x30_delta_pi_sel(val)                       addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0xC00)) | ((val) << 10))
#define get_XVR_Reg0x30_delta_pi_sel                            ((addXVR_Reg0x30 & 0xC00) >> 10)

#define posXVR_Reg0x30_gauss_bt                                 12
#define bitXVR_Reg0x30_gauss_bt                                 0x3000
#define set_XVR_Reg0x30_gauss_bt(val)                           addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0x3000)) | ((val) << 12))
#define get_XVR_Reg0x30_gauss_bt                                ((addXVR_Reg0x30 & 0x3000) >> 12)

#define posXVR_Reg0x30_ramp_slope                               14
#define bitXVR_Reg0x30_ramp_slope                               0xC000
#define set_XVR_Reg0x30_ramp_slope(val)                         addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0xC000)) | ((val) << 14))
#define get_XVR_Reg0x30_ramp_slope                              ((addXVR_Reg0x30 & 0xC000) >> 14)

#define posXVR_Reg0x30_amp_value                                16
#define bitXVR_Reg0x30_amp_value                                0xFF0000
#define set_XVR_Reg0x30_amp_value(val)                          addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x30_amp_value                               ((addXVR_Reg0x30 & 0xFF0000) >> 16)

#define posXVR_Reg0x30_freq_dir                                 24
#define bitXVR_Reg0x30_freq_dir                                 0x1000000
#define set_XVR_Reg0x30_freq_dir(val)                           addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0x1000000)) | ((val) << 24))
#define setf_XVR_Reg0x30_freq_dir                               addXVR_Reg0x30 |= 0x1000000
#define clrf_XVR_Reg0x30_freq_dir                               addXVR_Reg0x30 &= ~0x1000000
#define get_XVR_Reg0x30_freq_dir                                ((addXVR_Reg0x30 & 0x1000000) >> 24)

#define posXVR_Reg0x30_tdly_edrctl                              25
#define bitXVR_Reg0x30_tdly_edrctl                              0xE000000
#define set_XVR_Reg0x30_tdly_edrctl(val)                        addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0xE000000)) | ((val) << 25))
#define get_XVR_Reg0x30_tdly_edrctl                             ((addXVR_Reg0x30 & 0xE000000) >> 25)

#define posXVR_Reg0x30_ramp_delay                               28
#define bitXVR_Reg0x30_ramp_delay                               0xF0000000
#define set_XVR_Reg0x30_ramp_delay(val)                         addXVR_Reg0x30 = ((addXVR_Reg0x30 & (~0xF0000000)) | ((val) << 28))
#define get_XVR_Reg0x30_ramp_delay                              ((addXVR_Reg0x30 & 0xF0000000) >> 28)

//addXVR_Reg0x31
#define addXVR_Reg0x31                                          *((volatile unsigned long *) (0x4A800000+0x31*4))
#define posXVR_Reg0x31_tx_sin_freq                              0
#define bitXVR_Reg0x31_tx_sin_freq                              0xFF
#define set_XVR_Reg0x31_tx_sin_freq(val)                        addXVR_Reg0x31 = ((addXVR_Reg0x31 & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x31_tx_sin_freq                             (addXVR_Reg0x31 & 0xFF)

#define posXVR_Reg0x31_tx_q_const                               8
#define bitXVR_Reg0x31_tx_q_const                               0xFF00
#define set_XVR_Reg0x31_tx_q_const(val)                         addXVR_Reg0x31 = ((addXVR_Reg0x31 & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x31_tx_q_const                              ((addXVR_Reg0x31 & 0xFF00) >> 8)

#define posXVR_Reg0x31_tx_i_const                               16
#define bitXVR_Reg0x31_tx_i_const                               0xFF0000
#define set_XVR_Reg0x31_tx_i_const(val)                         addXVR_Reg0x31 = ((addXVR_Reg0x31 & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x31_tx_i_const                              ((addXVR_Reg0x31 & 0xFF0000) >> 16)

#define posXVR_Reg0x31_tx_sin_mode                              24
#define bitXVR_Reg0x31_tx_sin_mode                              0x3000000
#define set_XVR_Reg0x31_tx_sin_mode(val)                        addXVR_Reg0x31 = ((addXVR_Reg0x31 & (~0x3000000)) | ((val) << 24))
#define get_XVR_Reg0x31_tx_sin_mode                             ((addXVR_Reg0x31 & 0x3000000) >> 24)

#define posXVR_Reg0x31_test_patten                              26
#define bitXVR_Reg0x31_test_patten                              0xC000000
#define set_XVR_Reg0x31_test_patten(val)                        addXVR_Reg0x31 = ((addXVR_Reg0x31 & (~0xC000000)) | ((val) << 26))
#define get_XVR_Reg0x31_test_patten                             ((addXVR_Reg0x31 & 0xC000000) >> 26)

#define posXVR_Reg0x31_test_iq_swap                             28
#define bitXVR_Reg0x31_test_iq_swap                             0x10000000
#define set_XVR_Reg0x31_test_iq_swap(val)                       addXVR_Reg0x31 = ((addXVR_Reg0x31 & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x31_test_iq_swap                           addXVR_Reg0x31 |= 0x10000000
#define clrf_XVR_Reg0x31_test_iq_swap                           addXVR_Reg0x31 &= ~0x10000000
#define get_XVR_Reg0x31_test_iq_swap                            ((addXVR_Reg0x31 & 0x10000000) >> 28)

//addXVR_Reg0x32
#define addXVR_Reg0x32                                          *((volatile unsigned long *) (0x4A800000+0x32*4))
#define posXVR_Reg0x32_q_dc_offset                              0
#define bitXVR_Reg0x32_q_dc_offset                              0xFF
#define set_XVR_Reg0x32_q_dc_offset(val)                        addXVR_Reg0x32 = ((addXVR_Reg0x32 & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x32_q_dc_offset                             (addXVR_Reg0x32 & 0xFF)

#define posXVR_Reg0x32_i_dc_offset                              8
#define bitXVR_Reg0x32_i_dc_offset                              0xFF00
#define set_XVR_Reg0x32_i_dc_offset(val)                        addXVR_Reg0x32 = ((addXVR_Reg0x32 & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x32_i_dc_offset                             ((addXVR_Reg0x32 & 0xFF00) >> 8)

#define posXVR_Reg0x32_q_gain_comp                              16
#define bitXVR_Reg0x32_q_gain_comp                              0xFF0000
#define set_XVR_Reg0x32_q_gain_comp(val)                        addXVR_Reg0x32 = ((addXVR_Reg0x32 & (~0xFF0000)) | ((val) << 16))
#define get_XVR_Reg0x32_q_gain_comp                             ((addXVR_Reg0x32 & 0xFF0000) >> 16)

#define posXVR_Reg0x32_i_gain_comp                              24
#define bitXVR_Reg0x32_i_gain_comp                              0xFF000000
#define set_XVR_Reg0x32_i_gain_comp(val)                        addXVR_Reg0x32 = ((addXVR_Reg0x32 & (~0xFF000000)) | ((val) << 24))
#define get_XVR_Reg0x32_i_gain_comp                             ((addXVR_Reg0x32 & 0xFF000000) >> 24)

//addXVR_Reg0x33
#define addXVR_Reg0x33                                          *((volatile unsigned long *) (0x4A800000+0x33*4))
#define posXVR_Reg0x33_iq_tx_ty2                                0
#define bitXVR_Reg0x33_iq_tx_ty2                                0xFF
#define set_XVR_Reg0x33_iq_tx_ty2(val)                          addXVR_Reg0x33 = ((addXVR_Reg0x33 & (~0xFF)) | ((val) << 0))
#define get_XVR_Reg0x33_iq_tx_ty2                               (addXVR_Reg0x33 & 0xFF)

#define posXVR_Reg0x33_phase_comp                               8
#define bitXVR_Reg0x33_phase_comp                               0xFF00
#define set_XVR_Reg0x33_phase_comp(val)                         addXVR_Reg0x33 = ((addXVR_Reg0x33 & (~0xFF00)) | ((val) << 8))
#define get_XVR_Reg0x33_phase_comp                              ((addXVR_Reg0x33 & 0xFF00) >> 8)

#define posXVR_Reg0x33_iq_pre_gain                              16
#define bitXVR_Reg0x33_iq_pre_gain                              0x70000
#define set_XVR_Reg0x33_iq_pre_gain(val)                        addXVR_Reg0x33 = ((addXVR_Reg0x33 & (~0x70000)) | ((val) << 16))
#define get_XVR_Reg0x33_iq_pre_gain                             ((addXVR_Reg0x33 & 0x70000) >> 16)

#define posXVR_Reg0x33_iq_swap                                  19
#define bitXVR_Reg0x33_iq_swap                                  0x80000
#define set_XVR_Reg0x33_iq_swap(val)                            addXVR_Reg0x33 = ((addXVR_Reg0x33 & (~0x80000)) | ((val) << 19))
#define setf_XVR_Reg0x33_iq_swap                                addXVR_Reg0x33 |= 0x80000
#define clrf_XVR_Reg0x33_iq_swap                                addXVR_Reg0x33 &= ~0x80000
#define get_XVR_Reg0x33_iq_swap                                 ((addXVR_Reg0x33 & 0x80000) >> 19)

//addXVR_Reg0x38
#define addXVR_Reg0x38                                          *((volatile unsigned long *) (0x4A800000+0x38*4))
#define posXVR_Reg0x38_set_pga_gain                             0
#define bitXVR_Reg0x38_set_pga_gain                             0x7
#define set_XVR_Reg0x38_set_pga_gain(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x7)) | ((val) << 0))
#define get_XVR_Reg0x38_set_pga_gain                            (addXVR_Reg0x38 & 0x7)

#define posXVR_Reg0x38_set_buf_gain                             0
#define bitXVR_Reg0x38_set_buf_gain                             0x7
#define set_XVR_Reg0x38_set_buf_gain(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x7)) | ((val) << 0))
#define get_XVR_Reg0x38_set_buf_gain                            (addXVR_Reg0x38 & 0x7)

#define posXVR_Reg0x38_set_lna_gain                             0
#define bitXVR_Reg0x38_set_lna_gain                             0x1F
#define set_XVR_Reg0x38_set_lna_gain(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x1F)) | ((val) << 0))
#define get_XVR_Reg0x38_set_lna_gain                            (addXVR_Reg0x38 & 0x1F)

#define posXVR_Reg0x38_use_set_gain                             10
#define bitXVR_Reg0x38_use_set_gain                             0x400
#define set_XVR_Reg0x38_use_set_gain(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x400)) | ((val) << 10))
#define setf_XVR_Reg0x38_use_set_gain                           addXVR_Reg0x38 |= 0x400
#define clrf_XVR_Reg0x38_use_set_gain                           addXVR_Reg0x38 &= ~0x400
#define get_XVR_Reg0x38_use_set_gain                            ((addXVR_Reg0x38 & 0x400) >> 10)

#define posXVR_Reg0x38_NC                                       12
#define bitXVR_Reg0x38_NC                                       0xF000
#define set_XVR_Reg0x38_NC(val)                                 addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0xF000)) | ((val) << 12))
#define get_XVR_Reg0x38_NC                                      ((addXVR_Reg0x38 & 0xF000) >> 12)

#define posXVR_Reg0x38_fsk_acs0_sel                             16
#define bitXVR_Reg0x38_fsk_acs0_sel                             0xF0000
#define set_XVR_Reg0x38_fsk_acs0_sel(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0xF0000)) | ((val) << 16))
#define get_XVR_Reg0x38_fsk_acs0_sel                            ((addXVR_Reg0x38 & 0xF0000) >> 16)

#define posXVR_Reg0x38_edr_acs0_sel                             20
#define bitXVR_Reg0x38_edr_acs0_sel                             0xF00000
#define set_XVR_Reg0x38_edr_acs0_sel(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0xF00000)) | ((val) << 20))
#define get_XVR_Reg0x38_edr_acs0_sel                            ((addXVR_Reg0x38 & 0xF00000) >> 20)

#define posXVR_Reg0x38_edr_acs1_sel                             24
#define bitXVR_Reg0x38_edr_acs1_sel                             0xF000000
#define set_XVR_Reg0x38_edr_acs1_sel(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0xF000000)) | ((val) << 24))
#define get_XVR_Reg0x38_edr_acs1_sel                            ((addXVR_Reg0x38 & 0xF000000) >> 24)

#define posXVR_Reg0x38_adc_dcfilt_cfg                           28
#define bitXVR_Reg0x38_adc_dcfilt_cfg                           0x30000000
#define set_XVR_Reg0x38_adc_dcfilt_cfg(val)                     addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x30000000)) | ((val) << 28))
#define get_XVR_Reg0x38_adc_dcfilt_cfg                          ((addXVR_Reg0x38 & 0x30000000) >> 28)

#define posXVR_Reg0x38_adc_sampsel                              30
#define bitXVR_Reg0x38_adc_sampsel                              0x40000000
#define set_XVR_Reg0x38_adc_sampsel(val)                        addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x38_adc_sampsel                            addXVR_Reg0x38 |= 0x40000000
#define clrf_XVR_Reg0x38_adc_sampsel                            addXVR_Reg0x38 &= ~0x40000000
#define get_XVR_Reg0x38_adc_sampsel                             ((addXVR_Reg0x38 & 0x40000000) >> 30)

#define posXVR_Reg0x38_adc_iqswitch                             31
#define bitXVR_Reg0x38_adc_iqswitch                             0x80000000
#define set_XVR_Reg0x38_adc_iqswitch(val)                       addXVR_Reg0x38 = ((addXVR_Reg0x38 & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x38_adc_iqswitch                           addXVR_Reg0x38 |= 0x80000000
#define clrf_XVR_Reg0x38_adc_iqswitch                           addXVR_Reg0x38 &= ~0x80000000
#define get_XVR_Reg0x38_adc_iqswitch                            ((addXVR_Reg0x38 & 0x80000000) >> 31)

//addXVR_Reg0x39
#define addXVR_Reg0x39                                          *((volatile unsigned long *) (0x4A800000+0x39*4))
#define posXVR_Reg0x39_agc_duration                             0
#define bitXVR_Reg0x39_agc_duration                             0x3F
#define set_XVR_Reg0x39_agc_duration(val)                       addXVR_Reg0x39 = ((addXVR_Reg0x39 & (~0x3F)) | ((val) << 0))
#define get_XVR_Reg0x39_agc_duration                            (addXVR_Reg0x39 & 0x3F)

#define posXVR_Reg0x39_agc_tsettling                            6
#define bitXVR_Reg0x39_agc_tsettling                            0x3C0
#define set_XVR_Reg0x39_agc_tsettling(val)                      addXVR_Reg0x39 = ((addXVR_Reg0x39 & (~0x3C0)) | ((val) << 6))
#define get_XVR_Reg0x39_agc_tsettling                           ((addXVR_Reg0x39 & 0x3C0) >> 6)

#define posXVR_Reg0x39_agc_twaiting                             10
#define bitXVR_Reg0x39_agc_twaiting                             0xFC00
#define set_XVR_Reg0x39_agc_twaiting(val)                       addXVR_Reg0x39 = ((addXVR_Reg0x39 & (~0xFC00)) | ((val) << 10))
#define get_XVR_Reg0x39_agc_twaiting                            ((addXVR_Reg0x39 & 0xFC00) >> 10)

#define posXVR_Reg0x39_agc_timeout                              16
#define bitXVR_Reg0x39_agc_timeout                              0xF0000
#define set_XVR_Reg0x39_agc_timeout(val)                        addXVR_Reg0x39 = ((addXVR_Reg0x39 & (~0xF0000)) | ((val) << 16))
#define get_XVR_Reg0x39_agc_timeout                             ((addXVR_Reg0x39 & 0xF0000) >> 16)

#define posXVR_Reg0x39_agc_mode_sel                             20
#define bitXVR_Reg0x39_agc_mode_sel                             0x100000
#define set_XVR_Reg0x39_agc_mode_sel(val)                       addXVR_Reg0x39 = ((addXVR_Reg0x39 & (~0x100000)) | ((val) << 20))
#define setf_XVR_Reg0x39_agc_mode_sel                           addXVR_Reg0x39 |= 0x100000
#define clrf_XVR_Reg0x39_agc_mode_sel                           addXVR_Reg0x39 &= ~0x100000
#define get_XVR_Reg0x39_agc_mode_sel                            ((addXVR_Reg0x39 & 0x100000) >> 20)


#define posXVR_Reg0x39_rssi_offset                              24
#define bitXVR_Reg0x39_rssi_offset                              0xFF000000
#define set_XVR_Reg0x39_rssi_offset(val)                        addXVR_Reg0x39 = ((addXVR_Reg0x39 & (~0xFF000000)) | ((val) << 24))
#define get_XVR_Reg0x39_rssi_offset                             ((addXVR_Reg0x39 & 0xFF000000) >> 24)

//addXVR_Reg0x3a
#define addXVR_Reg0x3a                                          *((volatile unsigned long *) (0x4A800000+0x3a*4))
#define posXVR_Reg0x3a_pre_dcval_setted                         0
#define bitXVR_Reg0x3a_pre_dcval_setted                         0xFFF
#define set_XVR_Reg0x3a_pre_dcval_setted(val)                   addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0xFFF)) | ((val) << 0))
#define get_XVR_Reg0x3a_pre_dcval_setted                        (addXVR_Reg0x3a & 0xFFF)

#define posXVR_Reg0x3a_pre_dcval_enable                         12
#define bitXVR_Reg0x3a_pre_dcval_enable                         0x1000
#define set_XVR_Reg0x3a_pre_dcval_enable(val)                   addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x3a_pre_dcval_enable                       addXVR_Reg0x3a |= 0x1000
#define clrf_XVR_Reg0x3a_pre_dcval_enable                       addXVR_Reg0x3a &= ~0x1000
#define get_XVR_Reg0x3a_pre_dcval_enable                        ((addXVR_Reg0x3a & 0x1000) >> 12)

#define posXVR_Reg0x3a_pre_dc_bypass                            13
#define bitXVR_Reg0x3a_pre_dc_bypass                            0x2000
#define set_XVR_Reg0x3a_pre_dc_bypass(val)                      addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x3a_pre_dc_bypass                          addXVR_Reg0x3a |= 0x2000
#define clrf_XVR_Reg0x3a_pre_dc_bypass                          addXVR_Reg0x3a &= ~0x2000
#define get_XVR_Reg0x3a_pre_dc_bypass                           ((addXVR_Reg0x3a & 0x2000) >> 13)

#define posXVR_Reg0x3a_track_dc_bypass                          14
#define bitXVR_Reg0x3a_track_dc_bypass                          0x4000
#define set_XVR_Reg0x3a_track_dc_bypass(val)                    addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0x4000)) | ((val) << 14))
#define setf_XVR_Reg0x3a_track_dc_bypass                        addXVR_Reg0x3a |= 0x4000
#define clrf_XVR_Reg0x3a_track_dc_bypass                        addXVR_Reg0x3a &= ~0x4000
#define get_XVR_Reg0x3a_track_dc_bypass                         ((addXVR_Reg0x3a & 0x4000) >> 14)


#define posXVR_Reg0x3a_tailer_dcval_setted                      16
#define bitXVR_Reg0x3a_tailer_dcval_setted                      0xFFF0000
#define set_XVR_Reg0x3a_tailer_dcval_setted(val)                addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0xFFF0000)) | ((val) << 16))
#define get_XVR_Reg0x3a_tailer_dcval_setted                     ((addXVR_Reg0x3a & 0xFFF0000) >> 16)

#define posXVR_Reg0x3a_tailer_dcval_enable                      28
#define bitXVR_Reg0x3a_tailer_dcval_enable                      0x10000000
#define set_XVR_Reg0x3a_tailer_dcval_enable(val)                addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x3a_tailer_dcval_enable                    addXVR_Reg0x3a |= 0x10000000
#define clrf_XVR_Reg0x3a_tailer_dcval_enable                    addXVR_Reg0x3a &= ~0x10000000
#define get_XVR_Reg0x3a_tailer_dcval_enable                     ((addXVR_Reg0x3a & 0x10000000) >> 28)

#define posXVR_Reg0x3a_tailer_dc_bypass                         29
#define bitXVR_Reg0x3a_tailer_dc_bypass                         0x20000000
#define set_XVR_Reg0x3a_tailer_dc_bypass(val)                   addXVR_Reg0x3a = ((addXVR_Reg0x3a & (~0x20000000)) | ((val) << 29))
#define setf_XVR_Reg0x3a_tailer_dc_bypass                       addXVR_Reg0x3a |= 0x20000000
#define clrf_XVR_Reg0x3a_tailer_dc_bypass                       addXVR_Reg0x3a &= ~0x20000000
#define get_XVR_Reg0x3a_tailer_dc_bypass                        ((addXVR_Reg0x3a & 0x20000000) >> 29)

//addXVR_Reg0x3b
#define addXVR_Reg0x3b                                          *((volatile unsigned long *) (0x4A800000+0x3b*4))
#define posXVR_Reg0x3b_edr_m0_phase_thrd                        0
#define bitXVR_Reg0x3b_edr_m0_phase_thrd                        0xF
#define set_XVR_Reg0x3b_edr_m0_phase_thrd(val)                  addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0xF)) | ((val) << 0))
#define get_XVR_Reg0x3b_edr_m0_phase_thrd                       (addXVR_Reg0x3b & 0xF)

#define posXVR_Reg0x3b_edr_m0_bits_thrd                         4
#define bitXVR_Reg0x3b_edr_m0_bits_thrd                         0x70
#define set_XVR_Reg0x3b_edr_m0_bits_thrd(val)                   addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0x70)) | ((val) << 4))
#define get_XVR_Reg0x3b_edr_m0_bits_thrd                        ((addXVR_Reg0x3b & 0x70) >> 4)

#define posXVR_Reg0x3b_m0_phase_thrd                            8
#define bitXVR_Reg0x3b_m0_phase_thrd                            0xF00
#define set_XVR_Reg0x3b_m0_phase_thrd(val)                      addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0xF00)) | ((val) << 8))
#define get_XVR_Reg0x3b_m0_phase_thrd                           ((addXVR_Reg0x3b & 0xF00) >> 8)

#define posXVR_Reg0x3b_m0_bits_thrd                             12
#define bitXVR_Reg0x3b_m0_bits_thrd                             0xF000
#define set_XVR_Reg0x3b_m0_bits_thrd(val)                       addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0xF000)) | ((val) << 12))
#define get_XVR_Reg0x3b_m0_bits_thrd                            ((addXVR_Reg0x3b & 0xF000) >> 12)


#define posXVR_Reg0x3b_edr_window                               20
#define bitXVR_Reg0x3b_edr_window                               0x700000
#define set_XVR_Reg0x3b_edr_window(val)                         addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0x700000)) | ((val) << 20))
#define get_XVR_Reg0x3b_edr_window                              ((addXVR_Reg0x3b & 0x700000) >> 20)

#define posXVR_Reg0x3b_slot_delay                               23
#define bitXVR_Reg0x3b_slot_delay                               0x1F800000
#define set_XVR_Reg0x3b_slot_delay(val)                         addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0x1F800000)) | ((val) << 23))
#define get_XVR_Reg0x3b_slot_delay                              ((addXVR_Reg0x3b & 0x1F800000) >> 23)


#define posXVR_Reg0x3b_test_bus_ena                             31
#define bitXVR_Reg0x3b_test_bus_ena                             0x80000000
#define set_XVR_Reg0x3b_test_bus_ena(val)                       addXVR_Reg0x3b = ((addXVR_Reg0x3b & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x3b_test_bus_ena                           addXVR_Reg0x3b |= 0x80000000
#define clrf_XVR_Reg0x3b_test_bus_ena                           addXVR_Reg0x3b &= ~0x80000000
#define get_XVR_Reg0x3b_test_bus_ena                            ((addXVR_Reg0x3b & 0x80000000) >> 31)

//addXVR_Reg0x3c
#define addXVR_Reg0x3c                                          *((volatile unsigned long *) (0x4A800000+0x3c*4))

#define posXVR_Reg0x3c_equ_coef_set                             10
#define bitXVR_Reg0x3c_equ_coef_set                             0x400
#define set_XVR_Reg0x3c_equ_coef_set(val)                       addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x400)) | ((val) << 10))
#define setf_XVR_Reg0x3c_equ_coef_set                           addXVR_Reg0x3c |= 0x400
#define clrf_XVR_Reg0x3c_equ_coef_set                           addXVR_Reg0x3c &= ~0x400
#define get_XVR_Reg0x3c_equ_coef_set                            ((addXVR_Reg0x3c & 0x400) >> 10)

#define posXVR_Reg0x3c_edr_freq_compensate                      11
#define bitXVR_Reg0x3c_edr_freq_compensate                      0x800
#define set_XVR_Reg0x3c_edr_freq_compensate(val)                addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x800)) | ((val) << 11))
#define setf_XVR_Reg0x3c_edr_freq_compensate                    addXVR_Reg0x3c |= 0x800
#define clrf_XVR_Reg0x3c_edr_freq_compensate                    addXVR_Reg0x3c &= ~0x800
#define get_XVR_Reg0x3c_edr_freq_compensate                     ((addXVR_Reg0x3c & 0x800) >> 11)

#define posXVR_Reg0x3c_edr_freq_tracking                        12
#define bitXVR_Reg0x3c_edr_freq_tracking                        0x1000
#define set_XVR_Reg0x3c_edr_freq_tracking(val)                  addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x3c_edr_freq_tracking                      addXVR_Reg0x3c |= 0x1000
#define clrf_XVR_Reg0x3c_edr_freq_tracking                      addXVR_Reg0x3c &= ~0x1000
#define get_XVR_Reg0x3c_edr_freq_tracking                       ((addXVR_Reg0x3c & 0x1000) >> 12)

#define posXVR_Reg0x3c_sync_mse_sel                             13
#define bitXVR_Reg0x3c_sync_mse_sel                             0x2000
#define set_XVR_Reg0x3c_sync_mse_sel(val)                       addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x3c_sync_mse_sel                           addXVR_Reg0x3c |= 0x2000
#define clrf_XVR_Reg0x3c_sync_mse_sel                           addXVR_Reg0x3c &= ~0x2000
#define get_XVR_Reg0x3c_sync_mse_sel                            ((addXVR_Reg0x3c & 0x2000) >> 13)

#define posXVR_Reg0x3c_eq_active_sel                            14
#define bitXVR_Reg0x3c_eq_active_sel                            0xC000
#define set_XVR_Reg0x3c_eq_active_sel(val)                      addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0xC000)) | ((val) << 14))
#define get_XVR_Reg0x3c_eq_active_sel                           ((addXVR_Reg0x3c & 0xC000) >> 14)


#define posXVR_Reg0x3c_afc_mod_dir                              23
#define bitXVR_Reg0x3c_afc_mod_dir                              0x800000
#define set_XVR_Reg0x3c_afc_mod_dir(val)                        addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x800000)) | ((val) << 23))
#define setf_XVR_Reg0x3c_afc_mod_dir                            addXVR_Reg0x3c |= 0x800000
#define clrf_XVR_Reg0x3c_afc_mod_dir                            addXVR_Reg0x3c &= ~0x800000
#define get_XVR_Reg0x3c_afc_mod_dir                             ((addXVR_Reg0x3c & 0x800000) >> 23)

#define posXVR_Reg0x3c_edr_sync_center                          24
#define bitXVR_Reg0x3c_edr_sync_center                          0xF000000
#define set_XVR_Reg0x3c_edr_sync_center(val)                    addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0xF000000)) | ((val) << 24))
#define get_XVR_Reg0x3c_edr_sync_center                         ((addXVR_Reg0x3c & 0xF000000) >> 24)


#define posXVR_Reg0x3c_edr_equ_bypass                           30
#define bitXVR_Reg0x3c_edr_equ_bypass                           0x40000000
#define set_XVR_Reg0x3c_edr_equ_bypass(val)                     addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x3c_edr_equ_bypass                         addXVR_Reg0x3c |= 0x40000000
#define clrf_XVR_Reg0x3c_edr_equ_bypass                         addXVR_Reg0x3c &= ~0x40000000
#define get_XVR_Reg0x3c_edr_equ_bypass                          ((addXVR_Reg0x3c & 0x40000000) >> 30)

#define posXVR_Reg0x3c_equ_bypass                               31
#define bitXVR_Reg0x3c_equ_bypass                               0x80000000
#define set_XVR_Reg0x3c_equ_bypass(val)                         addXVR_Reg0x3c = ((addXVR_Reg0x3c & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x3c_equ_bypass                             addXVR_Reg0x3c |= 0x80000000
#define clrf_XVR_Reg0x3c_equ_bypass                             addXVR_Reg0x3c &= ~0x80000000
#define get_XVR_Reg0x3c_equ_bypass                              ((addXVR_Reg0x3c & 0x80000000) >> 31)

//addXVR_Reg0x3d
#define addXVR_Reg0x3d                                          *((volatile unsigned long *) (0x4A800000+0x3d*4))
#define posXVR_Reg0x3d_correlate_coef                           0
#define bitXVR_Reg0x3d_correlate_coef                           0xFFF
#define set_XVR_Reg0x3d_correlate_coef(val)                     addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0xFFF)) | ((val) << 0))
#define get_XVR_Reg0x3d_correlate_coef                          (addXVR_Reg0x3d & 0xFFF)

#define posXVR_Reg0x3d_correlate_coef_set                       12
#define bitXVR_Reg0x3d_correlate_coef_set                       0x1000
#define set_XVR_Reg0x3d_correlate_coef_set(val)                 addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x1000)) | ((val) << 12))
#define setf_XVR_Reg0x3d_correlate_coef_set                     addXVR_Reg0x3d |= 0x1000
#define clrf_XVR_Reg0x3d_correlate_coef_set                     addXVR_Reg0x3d &= ~0x1000
#define get_XVR_Reg0x3d_correlate_coef_set                      ((addXVR_Reg0x3d & 0x1000) >> 12)

#define posXVR_Reg0x3d_correlate_enable                         13
#define bitXVR_Reg0x3d_correlate_enable                         0x2000
#define set_XVR_Reg0x3d_correlate_enable(val)                   addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x2000)) | ((val) << 13))
#define setf_XVR_Reg0x3d_correlate_enable                       addXVR_Reg0x3d |= 0x2000
#define clrf_XVR_Reg0x3d_correlate_enable                       addXVR_Reg0x3d &= ~0x2000
#define get_XVR_Reg0x3d_correlate_enable                        ((addXVR_Reg0x3d & 0x2000) >> 13)

#define posXVR_Reg0x3d_sync_err_thrd                            14
#define bitXVR_Reg0x3d_sync_err_thrd                            0x3C000
#define set_XVR_Reg0x3d_sync_err_thrd(val)                      addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x3C000)) | ((val) << 14))
#define get_XVR_Reg0x3d_sync_err_thrd                           ((addXVR_Reg0x3d & 0x3C000) >> 14)

#define posXVR_Reg0x3d_edr_phase_track_alpha                    18
#define bitXVR_Reg0x3d_edr_phase_track_alpha                    0xC0000
#define set_XVR_Reg0x3d_edr_phase_track_alpha(val)              addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0xC0000)) | ((val) << 18))
#define get_XVR_Reg0x3d_edr_phase_track_alpha                   ((addXVR_Reg0x3d & 0xC0000) >> 18)

#define posXVR_Reg0x3d_edr_phase_track_thrd                     20
#define bitXVR_Reg0x3d_edr_phase_track_thrd                     0x300000
#define set_XVR_Reg0x3d_edr_phase_track_thrd(val)               addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x300000)) | ((val) << 20))
#define get_XVR_Reg0x3d_edr_phase_track_thrd                    ((addXVR_Reg0x3d & 0x300000) >> 20)

#define posXVR_Reg0x3d_edr_phase_track_bypass                   22
#define bitXVR_Reg0x3d_edr_phase_track_bypass                   0x400000
#define set_XVR_Reg0x3d_edr_phase_track_bypass(val)             addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x400000)) | ((val) << 22))
#define setf_XVR_Reg0x3d_edr_phase_track_bypass                 addXVR_Reg0x3d |= 0x400000
#define clrf_XVR_Reg0x3d_edr_phase_track_bypass                 addXVR_Reg0x3d &= ~0x400000
#define get_XVR_Reg0x3d_edr_phase_track_bypass                  ((addXVR_Reg0x3d & 0x400000) >> 22)

#define posXVR_Reg0x3d_fsk_phase_track_alpha                    23
#define bitXVR_Reg0x3d_fsk_phase_track_alpha                    0x1800000
#define set_XVR_Reg0x3d_fsk_phase_track_alpha(val)              addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x1800000)) | ((val) << 23))
#define get_XVR_Reg0x3d_fsk_phase_track_alpha                   ((addXVR_Reg0x3d & 0x1800000) >> 23)

#define posXVR_Reg0x3d_fsk_phase_track_thrd                     25
#define bitXVR_Reg0x3d_fsk_phase_track_thrd                     0x6000000
#define set_XVR_Reg0x3d_fsk_phase_track_thrd(val)               addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x6000000)) | ((val) << 25))
#define get_XVR_Reg0x3d_fsk_phase_track_thrd                    ((addXVR_Reg0x3d & 0x6000000) >> 25)

#define posXVR_Reg0x3d_fsk_phase_track_bypass                   27
#define bitXVR_Reg0x3d_fsk_phase_track_bypass                   0x8000000
#define set_XVR_Reg0x3d_fsk_phase_track_bypass(val)             addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x8000000)) | ((val) << 27))
#define setf_XVR_Reg0x3d_fsk_phase_track_bypass                 addXVR_Reg0x3d |= 0x8000000
#define clrf_XVR_Reg0x3d_fsk_phase_track_bypass                 addXVR_Reg0x3d &= ~0x8000000
#define get_XVR_Reg0x3d_fsk_phase_track_bypass                  ((addXVR_Reg0x3d & 0x8000000) >> 27)

#define posXVR_Reg0x3d_correlate_actsel                         28
#define bitXVR_Reg0x3d_correlate_actsel                         0x10000000
#define set_XVR_Reg0x3d_correlate_actsel(val)                   addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x10000000)) | ((val) << 28))
#define setf_XVR_Reg0x3d_correlate_actsel                       addXVR_Reg0x3d |= 0x10000000
#define clrf_XVR_Reg0x3d_correlate_actsel                       addXVR_Reg0x3d &= ~0x10000000
#define get_XVR_Reg0x3d_correlate_actsel                        ((addXVR_Reg0x3d & 0x10000000) >> 28)

#define posXVR_Reg0x3d_bit_reverse                              29
#define bitXVR_Reg0x3d_bit_reverse                              0x20000000
#define set_XVR_Reg0x3d_bit_reverse(val)                        addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x20000000)) | ((val) << 29))
#define setf_XVR_Reg0x3d_bit_reverse                            addXVR_Reg0x3d |= 0x20000000
#define clrf_XVR_Reg0x3d_bit_reverse                            addXVR_Reg0x3d &= ~0x20000000
#define get_XVR_Reg0x3d_bit_reverse                             ((addXVR_Reg0x3d & 0x20000000) >> 29)

#define posXVR_Reg0x3d_freq_dir_edr                             30
#define bitXVR_Reg0x3d_freq_dir_edr                             0x40000000
#define set_XVR_Reg0x3d_freq_dir_edr(val)                       addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x3d_freq_dir_edr                           addXVR_Reg0x3d |= 0x40000000
#define clrf_XVR_Reg0x3d_freq_dir_edr                           addXVR_Reg0x3d &= ~0x40000000
#define get_XVR_Reg0x3d_freq_dir_edr                            ((addXVR_Reg0x3d & 0x40000000) >> 30)

#define posXVR_Reg0x3d_freq_dir_fsk                             31
#define bitXVR_Reg0x3d_freq_dir_fsk                             0x80000000
#define set_XVR_Reg0x3d_freq_dir_fsk(val)                       addXVR_Reg0x3d = ((addXVR_Reg0x3d & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x3d_freq_dir_fsk                           addXVR_Reg0x3d |= 0x80000000
#define clrf_XVR_Reg0x3d_freq_dir_fsk                           addXVR_Reg0x3d &= ~0x80000000
#define get_XVR_Reg0x3d_freq_dir_fsk                            ((addXVR_Reg0x3d & 0x80000000) >> 31)

//addXVR_Reg0x3e
#define addXVR_Reg0x3e                                          *((volatile unsigned long *) (0x4A800000+0x3e*4))
#define posXVR_Reg0x3e_bp_track_lr                              31
#define bitXVR_Reg0x3e_bp_track_lr                              0x80000000
#define set_XVR_Reg0x3e_bp_track_lr(val)                        addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x80000000)) | ((val) << 31))
#define setf_XVR_Reg0x3e_bp_track_lr                            addXVR_Reg0x3e |= 0x80000000
#define clrf_XVR_Reg0x3e_bp_track_lr                            addXVR_Reg0x3e &= ~0x80000000
#define get_XVR_Reg0x3e_bp_track_lr                             ((addXVR_Reg0x3e & 0x80000000) >> 31)

#define posXVR_Reg0x3e_lr_phase_adj                             30
#define bitXVR_Reg0x3e_lr_phase_adj                             0x40000000
#define set_XVR_Reg0x3e_lr_phase_adj(val)                       addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x40000000)) | ((val) << 30))
#define setf_XVR_Reg0x3e_lr_phase_adj                           addXVR_Reg0x3e |= 0x40000000
#define clrf_XVR_Reg0x3e_lr_phase_adj                           addXVR_Reg0x3e &= ~0x40000000
#define get_XVR_Reg0x3e_lr_phase_adj                            ((addXVR_Reg0x3e & 0x40000000) >> 30)

#define posXVR_Reg0x3e_lr_win_thrd                              24
#define bitXVR_Reg0x3e_lr_win_thrd                              0x3F000000
#define set_XVR_Reg0x3e_lr_win_thrd(val)                        addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x3F000000)) | ((val) << 24))
#define get_XVR_Reg0x3e_lr_win_thrd                             ((addXVR_Reg0x3e & 0x3F000000) >> 24)

#define posXVR_Reg0x3e_forcelrmode                              23
#define bitXVR_Reg0x3e_forcelrmode                              0x800000
#define set_XVR_Reg0x3e_forcelrmode(val)                        addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x800000)) | ((val) << 23))
#define setf_XVR_Reg0x3e_forcelrmode                            addXVR_Reg0x3e |= 0x800000
#define clrf_XVR_Reg0x3e_forcelrmode                            addXVR_Reg0x3e &= ~0x800000
#define get_XVR_Reg0x3e_forcelrmode                             ((addXVR_Reg0x3e & 0x800000) >> 23)

#define posXVR_Reg0x3e_lr_win_dc_bps                            22
#define bitXVR_Reg0x3e_lr_win_dc_bps                            0x400000
#define set_XVR_Reg0x3e_lr_win_dc_bps(val)                      addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x400000)) | ((val) << 22))
#define setf_XVR_Reg0x3e_lr_win_dc_bps                          addXVR_Reg0x3e |= 0x400000
#define clrf_XVR_Reg0x3e_lr_win_dc_bps                          addXVR_Reg0x3e &= ~0x400000
#define get_XVR_Reg0x3e_lr_win_dc_bps                           ((addXVR_Reg0x3e & 0x400000) >> 22)

#define posXVR_Reg0x3e_sum1_max_thrd                            16
#define bitXVR_Reg0x3e_sum1_max_thrd                            0x3F0000
#define set_XVR_Reg0x3e_sum1_max_thrd(val)                      addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x3F0000)) | ((val) << 16))
#define get_XVR_Reg0x3e_sum1_max_thrd                           ((addXVR_Reg0x3e & 0x3F0000) >> 16)

#define posXVR_Reg0x3e_lr_win_en                                15
#define bitXVR_Reg0x3e_lr_win_en                                0x8000
#define set_XVR_Reg0x3e_lr_win_en(val)                          addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x8000)) | ((val) << 15))
#define setf_XVR_Reg0x3e_lr_win_en                              addXVR_Reg0x3e |= 0x8000
#define clrf_XVR_Reg0x3e_lr_win_en                              addXVR_Reg0x3e &= ~0x8000
#define get_XVR_Reg0x3e_lr_win_en                               ((addXVR_Reg0x3e & 0x8000) >> 15)


#define posXVR_Reg0x3e_m0_phase_thrd_lr                         8
#define bitXVR_Reg0x3e_m0_phase_thrd_lr                         0x700
#define set_XVR_Reg0x3e_m0_phase_thrd_lr(val)                   addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x700)) | ((val) << 8))
#define get_XVR_Reg0x3e_m0_phase_thrd_lr                        ((addXVR_Reg0x3e & 0x700) >> 8)

#define posXVR_Reg0x3e_bb_lr_open                               7
#define bitXVR_Reg0x3e_bb_lr_open                               0x80
#define set_XVR_Reg0x3e_bb_lr_open(val)                         addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x80)) | ((val) << 7))
#define setf_XVR_Reg0x3e_bb_lr_open                             addXVR_Reg0x3e |= 0x80
#define clrf_XVR_Reg0x3e_bb_lr_open                             addXVR_Reg0x3e &= ~0x80
#define get_XVR_Reg0x3e_bb_lr_open                              ((addXVR_Reg0x3e & 0x80) >> 7)

#define posXVR_Reg0x3e_lr_sync_comp_en                          6
#define bitXVR_Reg0x3e_lr_sync_comp_en                          0x40
#define set_XVR_Reg0x3e_lr_sync_comp_en(val)                    addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x40)) | ((val) << 6))
#define setf_XVR_Reg0x3e_lr_sync_comp_en                        addXVR_Reg0x3e |= 0x40
#define clrf_XVR_Reg0x3e_lr_sync_comp_en                        addXVR_Reg0x3e &= ~0x40
#define get_XVR_Reg0x3e_lr_sync_comp_en                         ((addXVR_Reg0x3e & 0x40) >> 6)

#define posXVR_Reg0x3e_m0_bits_thrd_lr                          3
#define bitXVR_Reg0x3e_m0_bits_thrd_lr                          0x38
#define set_XVR_Reg0x3e_m0_bits_thrd_lr(val)                    addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x38)) | ((val) << 3))
#define get_XVR_Reg0x3e_m0_bits_thrd_lr                         ((addXVR_Reg0x3e & 0x38) >> 3)

#define posXVR_Reg0x3e_rsync_bits_thrd_lr                       0
#define bitXVR_Reg0x3e_rsync_bits_thrd_lr                       0x7
#define set_XVR_Reg0x3e_rsync_bits_thrd_lr(val)                 addXVR_Reg0x3e = ((addXVR_Reg0x3e & (~0x7)) | ((val) << 0))
#define get_XVR_Reg0x3e_rsync_bits_thrd_lr                      (addXVR_Reg0x3e & 0x7)

//addXVR_Reg0x3f
#define addXVR_Reg0x3f                                          *((volatile unsigned long *) (0x4A800000+0x3f*4))
#define posXVR_Reg0x3f_w2_manual                                20
#define bitXVR_Reg0x3f_w2_manual                                0x3FF00000
#define set_XVR_Reg0x3f_w2_manual(val)                          addXVR_Reg0x3f = ((addXVR_Reg0x3f & (~0x3FF00000)) | ((val) << 20))
#define get_XVR_Reg0x3f_w2_manual                               ((addXVR_Reg0x3f & 0x3FF00000) >> 20)

#define posXVR_Reg0x3f_w1_manual                                10
#define bitXVR_Reg0x3f_w1_manual                                0xFFC00
#define set_XVR_Reg0x3f_w1_manual(val)                          addXVR_Reg0x3f = ((addXVR_Reg0x3f & (~0xFFC00)) | ((val) << 10))
#define get_XVR_Reg0x3f_w1_manual                               ((addXVR_Reg0x3f & 0xFFC00) >> 10)

#define posXVR_Reg0x3f_w0_manual                                0
#define bitXVR_Reg0x3f_w0_manual                                0x3FF
#define set_XVR_Reg0x3f_w0_manual(val)                          addXVR_Reg0x3f = ((addXVR_Reg0x3f & (~0x3FF)) | ((val) << 0))
#define get_XVR_Reg0x3f_w0_manual                               (addXVR_Reg0x3f & 0x3FF)


//************************************************************//
//LA
//************************************************************//
#define BASEADDR_LA                                             0x45070000
//addLA_Reg0x0
#define addLA_Reg0x0                                            *((volatile unsigned long *) (0x45070000+0x0*4))
#define posLA_Reg0x0_LaSmpLen                                   12
#define bitLA_Reg0x0_LaSmpLen                                   0xFFFFF000
#define set_LA_Reg0x0_LaSmpLen(val)                             addLA_Reg0x0 = ((addLA_Reg0x0 & (~0xFFFFF000)) | ((val) << 12))
#define get_LA_Reg0x0_LaSmpLen                                  ((addLA_Reg0x0 & 0xFFFFF000) >> 12)


#define posLA_Reg0x0_LaSmpInt                                   6
#define bitLA_Reg0x0_LaSmpInt                                   0x40
#define set_LA_Reg0x0_LaSmpInt(val)                             addLA_Reg0x0 = ((addLA_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_LA_Reg0x0_LaSmpInt                                 addLA_Reg0x0 |= 0x40
#define clrf_LA_Reg0x0_LaSmpInt                                 addLA_Reg0x0 &= ~0x40
#define get_LA_Reg0x0_LaSmpInt                                  ((addLA_Reg0x0 & 0x40) >> 6)

#define posLA_Reg0x0_LaSmpIntEn                                 5
#define bitLA_Reg0x0_LaSmpIntEn                                 0x20
#define set_LA_Reg0x0_LaSmpIntEn(val)                           addLA_Reg0x0 = ((addLA_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_LA_Reg0x0_LaSmpIntEn                               addLA_Reg0x0 |= 0x20
#define clrf_LA_Reg0x0_LaSmpIntEn                               addLA_Reg0x0 &= ~0x20
#define get_LA_Reg0x0_LaSmpIntEn                                ((addLA_Reg0x0 & 0x20) >> 5)

#define posLA_Reg0x0_XferMode                                   4
#define bitLA_Reg0x0_XferMode                                   0x10
#define set_LA_Reg0x0_XferMode(val)                             addLA_Reg0x0 = ((addLA_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_LA_Reg0x0_XferMode                                 addLA_Reg0x0 |= 0x10
#define clrf_LA_Reg0x0_XferMode                                 addLA_Reg0x0 &= ~0x10
#define get_LA_Reg0x0_XferMode                                  ((addLA_Reg0x0 & 0x10) >> 4)

#define posLA_Reg0x0_LaSmpFinish                                3
#define bitLA_Reg0x0_LaSmpFinish                                0x8
#define get_LA_Reg0x0_LaSmpFinish                               ((addLA_Reg0x0 & 0x8) >> 3)

#define posLA_Reg0x0_LaSmpClkInv                                2
#define bitLA_Reg0x0_LaSmpClkInv                                0x4
#define set_LA_Reg0x0_LaSmpClkInv(val)                          addLA_Reg0x0 = ((addLA_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_LA_Reg0x0_LaSmpClkInv                              addLA_Reg0x0 |= 0x4
#define clrf_LA_Reg0x0_LaSmpClkInv                              addLA_Reg0x0 &= ~0x4
#define get_LA_Reg0x0_LaSmpClkInv                               ((addLA_Reg0x0 & 0x4) >> 2)

#define posLA_Reg0x0_LaSmpMode                                  0
#define bitLA_Reg0x0_LaSmpMode                                  0x3
#define set_LA_Reg0x0_LaSmpMode(val)                            addLA_Reg0x0 = ((addLA_Reg0x0 & (~0x3)) | ((val) << 0))
#define get_LA_Reg0x0_LaSmpMode                                 (addLA_Reg0x0 & 0x3)

//addLA_Reg0x1
#define addLA_Reg0x1                                            *((volatile unsigned long *) (0x45070000+0x1*4))

//addLA_Reg0x2
#define addLA_Reg0x2                                            *((volatile unsigned long *) (0x45070000+0x2*4))

//addLA_Reg0x3
#define addLA_Reg0x3                                            *((volatile unsigned long *) (0x45070000+0x3*4))

//addLA_Reg0x4
#define addLA_Reg0x4                                            *((volatile unsigned long *) (0x45070000+0x4*4))


//************************************************************//
//JPEG
//************************************************************//
#define BASEADDR_JPEG                                           0x48000000
//addJPEG_Reg0x0
#define addJPEG_Reg0x0                                          *((volatile unsigned long *) (0x48000000+0x0*4))

#define posJPEG_Reg0x0_mclk_div                                 4
#define bitJPEG_Reg0x0_mclk_div                                 0x30
#define set_JPEG_Reg0x0_mclk_div(val)                           addJPEG_Reg0x0 = ((addJPEG_Reg0x0 & (~0x30)) | ((val) << 4))
#define get_JPEG_Reg0x0_mclk_div                                ((addJPEG_Reg0x0 & 0x30) >> 4)

#define posJPEG_Reg0x0_int_en                                   2
#define bitJPEG_Reg0x0_int_en                                   0xC
#define set_JPEG_Reg0x0_int_en(val)                             addJPEG_Reg0x0 = ((addJPEG_Reg0x0 & (~0xC)) | ((val) << 2))
#define get_JPEG_Reg0x0_int_en                                  ((addJPEG_Reg0x0 & 0xC) >> 2)


//addJPEG_Reg0x1
#define addJPEG_Reg0x1                                          *((volatile unsigned long *) (0x48000000+0x1*4))
#define posJPEG_Reg0x1_y_pixel                                  24
#define bitJPEG_Reg0x1_y_pixel                                  0xFF000000
#define set_JPEG_Reg0x1_y_pixel(val)                            addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0xFF000000)) | ((val) << 24))
#define get_JPEG_Reg0x1_y_pixel                                 ((addJPEG_Reg0x1 & 0xFF000000) >> 24)

#define posJPEG_Reg0x1_bitrate_mode                             23
#define bitJPEG_Reg0x1_bitrate_mode                             0x800000
#define set_JPEG_Reg0x1_bitrate_mode(val)                       addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x800000)) | ((val) << 23))
#define setf_JPEG_Reg0x1_bitrate_mode                           addJPEG_Reg0x1 |= 0x800000
#define clrf_JPEG_Reg0x1_bitrate_mode                           addJPEG_Reg0x1 &= ~0x800000
#define get_JPEG_Reg0x1_bitrate_mode                            ((addJPEG_Reg0x1 & 0x800000) >> 23)

#define posJPEG_Reg0x1_vsync_rev                                22
#define bitJPEG_Reg0x1_vsync_rev                                0x400000
#define set_JPEG_Reg0x1_vsync_rev(val)                          addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x400000)) | ((val) << 22))
#define setf_JPEG_Reg0x1_vsync_rev                              addJPEG_Reg0x1 |= 0x400000
#define clrf_JPEG_Reg0x1_vsync_rev                              addJPEG_Reg0x1 &= ~0x400000
#define get_JPEG_Reg0x1_vsync_rev                               ((addJPEG_Reg0x1 & 0x400000) >> 22)

#define posJPEG_Reg0x1_hsync_rev                                21
#define bitJPEG_Reg0x1_hsync_rev                                0x200000
#define set_JPEG_Reg0x1_hsync_rev(val)                          addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x200000)) | ((val) << 21))
#define setf_JPEG_Reg0x1_hsync_rev                              addJPEG_Reg0x1 |= 0x200000
#define clrf_JPEG_Reg0x1_hsync_rev                              addJPEG_Reg0x1 &= ~0x200000
#define get_JPEG_Reg0x1_hsync_rev                               ((addJPEG_Reg0x1 & 0x200000) >> 21)

#define posJPEG_Reg0x1_auto_step                                20
#define bitJPEG_Reg0x1_auto_step                                0x100000
#define set_JPEG_Reg0x1_auto_step(val)                          addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x100000)) | ((val) << 20))
#define setf_JPEG_Reg0x1_auto_step                              addJPEG_Reg0x1 |= 0x100000
#define clrf_JPEG_Reg0x1_auto_step                              addJPEG_Reg0x1 &= ~0x100000
#define get_JPEG_Reg0x1_auto_step                               ((addJPEG_Reg0x1 & 0x100000) >> 20)

#define posJPEG_Reg0x1_bitrate_step                             18
#define bitJPEG_Reg0x1_bitrate_step                             0xC0000
#define set_JPEG_Reg0x1_bitrate_step(val)                       addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0xC0000)) | ((val) << 18))
#define get_JPEG_Reg0x1_bitrate_step                            ((addJPEG_Reg0x1 & 0xC0000) >> 18)

#define posJPEG_Reg0x1_bitrate_ctrl                             17
#define bitJPEG_Reg0x1_bitrate_ctrl                             0x20000
#define set_JPEG_Reg0x1_bitrate_ctrl(val)                       addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x20000)) | ((val) << 17))
#define setf_JPEG_Reg0x1_bitrate_ctrl                           addJPEG_Reg0x1 |= 0x20000
#define clrf_JPEG_Reg0x1_bitrate_ctrl                           addJPEG_Reg0x1 &= ~0x20000
#define get_JPEG_Reg0x1_bitrate_ctrl                            ((addJPEG_Reg0x1 & 0x20000) >> 17)

#define posJPEG_Reg0x1_jpeg_enc_size                            16
#define bitJPEG_Reg0x1_jpeg_enc_size                            0x10000
#define set_JPEG_Reg0x1_jpeg_enc_size(val)                      addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x10000)) | ((val) << 16))
#define setf_JPEG_Reg0x1_jpeg_enc_size                          addJPEG_Reg0x1 |= 0x10000
#define clrf_JPEG_Reg0x1_jpeg_enc_size                          addJPEG_Reg0x1 &= ~0x10000
#define get_JPEG_Reg0x1_jpeg_enc_size                           ((addJPEG_Reg0x1 & 0x10000) >> 16)

#define posJPEG_Reg0x1_x_pixel                                  8
#define bitJPEG_Reg0x1_x_pixel                                  0xFF00
#define set_JPEG_Reg0x1_x_pixel(val)                            addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0xFF00)) | ((val) << 8))
#define get_JPEG_Reg0x1_x_pixel                                 ((addJPEG_Reg0x1 & 0xFF00) >> 8)

#define posJPEG_Reg0x1_yuvbuf_mode                              7
#define bitJPEG_Reg0x1_yuvbuf_mode                              0x80
#define set_JPEG_Reg0x1_yuvbuf_mode(val)                        addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x80)) | ((val) << 7))
#define setf_JPEG_Reg0x1_yuvbuf_mode                            addJPEG_Reg0x1 |= 0x80
#define clrf_JPEG_Reg0x1_yuvbuf_mode                            addJPEG_Reg0x1 &= ~0x80
#define get_JPEG_Reg0x1_yuvbuf_mode                             ((addJPEG_Reg0x1 & 0x80) >> 7)

#define posJPEG_Reg0x1_only_y                                   6
#define bitJPEG_Reg0x1_only_y                                   0x40
#define set_JPEG_Reg0x1_only_y(val)                             addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x40)) | ((val) << 6))
#define setf_JPEG_Reg0x1_only_y                                 addJPEG_Reg0x1 |= 0x40
#define clrf_JPEG_Reg0x1_only_y                                 addJPEG_Reg0x1 &= ~0x40
#define get_JPEG_Reg0x1_only_y                                  ((addJPEG_Reg0x1 & 0x40) >> 6)


#define posJPEG_Reg0x1_jpeg_enc_en                              4
#define bitJPEG_Reg0x1_jpeg_enc_en                              0x10
#define set_JPEG_Reg0x1_jpeg_enc_en(val)                        addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x10)) | ((val) << 4))
#define setf_JPEG_Reg0x1_jpeg_enc_en                            addJPEG_Reg0x1 |= 0x10
#define clrf_JPEG_Reg0x1_jpeg_enc_en                            addJPEG_Reg0x1 &= ~0x10
#define get_JPEG_Reg0x1_jpeg_enc_en                             ((addJPEG_Reg0x1 & 0x10) >> 4)

#define posJPEG_Reg0x1_yuv_fmt_sel                              2
#define bitJPEG_Reg0x1_yuv_fmt_sel                              0xC
#define set_JPEG_Reg0x1_yuv_fmt_sel(val)                        addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0xC)) | ((val) << 2))
#define get_JPEG_Reg0x1_yuv_fmt_sel                             ((addJPEG_Reg0x1 & 0xC) >> 2)

#define posJPEG_Reg0x1_video_byte_rev                           1
#define bitJPEG_Reg0x1_video_byte_rev                           0x2
#define set_JPEG_Reg0x1_video_byte_rev(val)                     addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x2)) | ((val) << 1))
#define setf_JPEG_Reg0x1_video_byte_rev                         addJPEG_Reg0x1 |= 0x2
#define clrf_JPEG_Reg0x1_video_byte_rev                         addJPEG_Reg0x1 &= ~0x2
#define get_JPEG_Reg0x1_video_byte_rev                          ((addJPEG_Reg0x1 & 0x2) >> 1)

#define posJPEG_Reg0x1_vck_edge                                 0
#define bitJPEG_Reg0x1_vck_edge                                 0x1
#define set_JPEG_Reg0x1_vck_edge(val)                           addJPEG_Reg0x1 = ((addJPEG_Reg0x1 & (~0x1)) | ((val) << 0))
#define setf_JPEG_Reg0x1_vck_edge                               addJPEG_Reg0x1 |= 0x1
#define clrf_JPEG_Reg0x1_vck_edge                               addJPEG_Reg0x1 &= ~0x1
#define get_JPEG_Reg0x1_vck_edge                                (addJPEG_Reg0x1 & 0x1)

//addJPEG_Reg0x2
#define addJPEG_Reg0x2                                          *((volatile unsigned long *) (0x48000000+0x2*4))

//addJPEG_Reg0x3
#define addJPEG_Reg0x3                                          *((volatile unsigned long *) (0x48000000+0x3*4))

//addJPEG_Reg0x4
#define addJPEG_Reg0x4                                          *((volatile unsigned long *) (0x48000000+0x4*4))
#define posJPEG_Reg0x4_em_base_addr                             0
#define bitJPEG_Reg0x4_em_base_addr                             0xFFFF
#define get_JPEG_Reg0x4_em_base_addr                            (addJPEG_Reg0x4 & 0xFFFF)

//addJPEG_Reg0x5
#define addJPEG_Reg0x5                                          *((volatile unsigned long *) (0x48000000+0x5*4))

//addJPEG_Reg0x6
#define addJPEG_Reg0x6                                          *((volatile unsigned long *) (0x48000000+0x6*4))

#define posJPEG_Reg0x6_fifo_rd_fifnish                          2
#define bitJPEG_Reg0x6_fifo_rd_fifnish                          0x4
#define get_JPEG_Reg0x6_fifo_rd_fifnish                         ((addJPEG_Reg0x6 & 0x4) >> 2)

#define posJPEG_Reg0x6_int_status                               0
#define bitJPEG_Reg0x6_int_status                               0x3
#define set_JPEG_Reg0x6_int_status(val)                         addJPEG_Reg0x6 = ((addJPEG_Reg0x6 & (~0x3)) | ((val) << 0))
#define get_JPEG_Reg0x6_int_status                              (addJPEG_Reg0x6 & 0x3)

//addJPEG_Reg0x7
#define addJPEG_Reg0x7                                          *((volatile unsigned long *) (0x48000000+0x7*4))

//addJPEG_Reg0x8
#define addJPEG_Reg0x8                                          *((volatile unsigned long *) (0x48000000+0x8*4))

#define posJPEG_Reg0x8_fifo_full                                1
#define bitJPEG_Reg0x8_fifo_full                                0x2
#define get_JPEG_Reg0x8_fifo_full                               ((addJPEG_Reg0x8 & 0x2) >> 1)

#define posJPEG_Reg0x8_fifo_empty                               0
#define bitJPEG_Reg0x8_fifo_empty                               0x1
#define get_JPEG_Reg0x8_fifo_empty                              (addJPEG_Reg0x8 & 0x1)

//addJPEG_Reg0x9
#define addJPEG_Reg0x9                                          *((volatile unsigned long *) (0x48000000+0x9*4))

#define posJPEG_Reg0x9_is_data_st                               8
#define bitJPEG_Reg0x9_is_data_st                               0x100
#define get_JPEG_Reg0x9_is_data_st                              ((addJPEG_Reg0x9 & 0x100) >> 8)

#define posJPEG_Reg0x9_y_count                                  0
#define bitJPEG_Reg0x9_y_count                                  0xFF
#define get_JPEG_Reg0x9_y_count                                 (addJPEG_Reg0x9 & 0xFF)

//addJPEG_Reg0xa
#define addJPEG_Reg0xa                                          *((volatile unsigned long *) (0x48000000+0xa*4))

//addJPEG_Reg0x20
#define addJPEG_Reg0x20                                         *((volatile unsigned long *) (0x48000000+0x20*4))

//addJPEG_Reg0x21
#define addJPEG_Reg0x21                                         *((volatile unsigned long *) (0x48000000+0x21*4))

//addJPEG_Reg0x22
#define addJPEG_Reg0x22                                         *((volatile unsigned long *) (0x48000000+0x22*4))

//addJPEG_Reg0x23
#define addJPEG_Reg0x23                                         *((volatile unsigned long *) (0x48000000+0x23*4))

//addJPEG_Reg0x24
#define addJPEG_Reg0x24                                         *((volatile unsigned long *) (0x48000000+0x24*4))

//addJPEG_Reg0x25
#define addJPEG_Reg0x25                                         *((volatile unsigned long *) (0x48000000+0x25*4))

//addJPEG_Reg0x26
#define addJPEG_Reg0x26                                         *((volatile unsigned long *) (0x48000000+0x26*4))

//addJPEG_Reg0x27
#define addJPEG_Reg0x27                                         *((volatile unsigned long *) (0x48000000+0x27*4))

//addJPEG_Reg0x28
#define addJPEG_Reg0x28                                         *((volatile unsigned long *) (0x48000000+0x28*4))

//addJPEG_Reg0x29
#define addJPEG_Reg0x29                                         *((volatile unsigned long *) (0x48000000+0x29*4))

//addJPEG_Reg0x2a
#define addJPEG_Reg0x2a                                         *((volatile unsigned long *) (0x48000000+0x2a*4))

//addJPEG_Reg0x2b
#define addJPEG_Reg0x2b                                         *((volatile unsigned long *) (0x48000000+0x2b*4))

//addJPEG_Reg0x2c
#define addJPEG_Reg0x2c                                         *((volatile unsigned long *) (0x48000000+0x2c*4))

//addJPEG_Reg0x2d
#define addJPEG_Reg0x2d                                         *((volatile unsigned long *) (0x48000000+0x2d*4))

//addJPEG_Reg0x2e
#define addJPEG_Reg0x2e                                         *((volatile unsigned long *) (0x48000000+0x2e*4))

//addJPEG_Reg0x2f
#define addJPEG_Reg0x2f                                         *((volatile unsigned long *) (0x48000000+0x2f*4))

//addJPEG_Reg0x30
#define addJPEG_Reg0x30                                         *((volatile unsigned long *) (0x48000000+0x30*4))

//addJPEG_Reg0x31
#define addJPEG_Reg0x31                                         *((volatile unsigned long *) (0x48000000+0x31*4))

//addJPEG_Reg0x32
#define addJPEG_Reg0x32                                         *((volatile unsigned long *) (0x48000000+0x32*4))

//addJPEG_Reg0x33
#define addJPEG_Reg0x33                                         *((volatile unsigned long *) (0x48000000+0x33*4))

//addJPEG_Reg0x34
#define addJPEG_Reg0x34                                         *((volatile unsigned long *) (0x48000000+0x34*4))

//addJPEG_Reg0x35
#define addJPEG_Reg0x35                                         *((volatile unsigned long *) (0x48000000+0x35*4))

//addJPEG_Reg0x36
#define addJPEG_Reg0x36                                         *((volatile unsigned long *) (0x48000000+0x36*4))

//addJPEG_Reg0x37
#define addJPEG_Reg0x37                                         *((volatile unsigned long *) (0x48000000+0x37*4))

//addJPEG_Reg0x38
#define addJPEG_Reg0x38                                         *((volatile unsigned long *) (0x48000000+0x38*4))

//addJPEG_Reg0x39
#define addJPEG_Reg0x39                                         *((volatile unsigned long *) (0x48000000+0x39*4))

//addJPEG_Reg0x3a
#define addJPEG_Reg0x3a                                         *((volatile unsigned long *) (0x48000000+0x3a*4))

//addJPEG_Reg0x3b
#define addJPEG_Reg0x3b                                         *((volatile unsigned long *) (0x48000000+0x3b*4))

//addJPEG_Reg0x3c
#define addJPEG_Reg0x3c                                         *((volatile unsigned long *) (0x48000000+0x3c*4))

//addJPEG_Reg0x3d
#define addJPEG_Reg0x3d                                         *((volatile unsigned long *) (0x48000000+0x3d*4))

//addJPEG_Reg0x3e
#define addJPEG_Reg0x3e                                         *((volatile unsigned long *) (0x48000000+0x3e*4))

//addJPEG_Reg0x3f
#define addJPEG_Reg0x3f                                         *((volatile unsigned long *) (0x48000000+0x3f*4))


//************************************************************//
//JPEG_DEC
//************************************************************//
#define BASEADDR_JPEG_DEC                                       0x48040000
//addJPEG_DEC_Reg0x0
#define addJPEG_DEC_Reg0x0                                      *((volatile unsigned long *) (0x48040000+0x0*4))

#define posJPEG_DEC_Reg0x0_jpeg_dec_en                          0
#define bitJPEG_DEC_Reg0x0_jpeg_dec_en                          0x1
#define set_JPEG_DEC_Reg0x0_jpeg_dec_en(val)                    addJPEG_DEC_Reg0x0 = ((addJPEG_DEC_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_JPEG_DEC_Reg0x0_jpeg_dec_en                        addJPEG_DEC_Reg0x0 |= 0x1
#define clrf_JPEG_DEC_Reg0x0_jpeg_dec_en                        addJPEG_DEC_Reg0x0 &= ~0x1
#define get_JPEG_DEC_Reg0x0_jpeg_dec_en                         (addJPEG_DEC_Reg0x0 & 0x1)

//addJPEG_DEC_Reg0x1
#define addJPEG_DEC_Reg0x1                                      *((volatile unsigned long *) (0x48040000+0x1*4))

//addJPEG_DEC_Reg0x2
#define addJPEG_DEC_Reg0x2                                      *((volatile unsigned long *) (0x48040000+0x2*4))

//addJPEG_DEC_Reg0x3
#define addJPEG_DEC_Reg0x3                                      *((volatile unsigned long *) (0x48040000+0x3*4))

#define posJPEG_DEC_Reg0x3_uv_vld                               0
#define bitJPEG_DEC_Reg0x3_uv_vld                               0x1
#define set_JPEG_DEC_Reg0x3_uv_vld(val)                         addJPEG_DEC_Reg0x3 = ((addJPEG_DEC_Reg0x3 & (~0x1)) | ((val) << 0))
#define setf_JPEG_DEC_Reg0x3_uv_vld                             addJPEG_DEC_Reg0x3 |= 0x1
#define clrf_JPEG_DEC_Reg0x3_uv_vld                             addJPEG_DEC_Reg0x3 &= ~0x1
#define get_JPEG_DEC_Reg0x3_uv_vld                              (addJPEG_DEC_Reg0x3 & 0x1)

//addJPEG_DEC_Reg0x4
#define addJPEG_DEC_Reg0x4                                      *((volatile unsigned long *) (0x48040000+0x4*4))

//addJPEG_DEC_Reg0x5
#define addJPEG_DEC_Reg0x5                                      *((volatile unsigned long *) (0x48040000+0x5*4))

#define posJPEG_DEC_Reg0x5_mcu_x                                0
#define bitJPEG_DEC_Reg0x5_mcu_x                                0xFFFF
#define set_JPEG_DEC_Reg0x5_mcu_x(val)                          addJPEG_DEC_Reg0x5 = ((addJPEG_DEC_Reg0x5 & (~0xFFFF)) | ((val) << 0))
#define get_JPEG_DEC_Reg0x5_mcu_x                               (addJPEG_DEC_Reg0x5 & 0xFFFF)

//addJPEG_DEC_Reg0x6
#define addJPEG_DEC_Reg0x6                                      *((volatile unsigned long *) (0x48040000+0x6*4))

#define posJPEG_DEC_Reg0x6_mcu_y                                0
#define bitJPEG_DEC_Reg0x6_mcu_y                                0xFFFF
#define set_JPEG_DEC_Reg0x6_mcu_y(val)                          addJPEG_DEC_Reg0x6 = ((addJPEG_DEC_Reg0x6 & (~0xFFFF)) | ((val) << 0))
#define get_JPEG_DEC_Reg0x6_mcu_y                               (addJPEG_DEC_Reg0x6 & 0xFFFF)

//addJPEG_DEC_Reg0x7
#define addJPEG_DEC_Reg0x7                                      *((volatile unsigned long *) (0x48040000+0x7*4))

#define posJPEG_DEC_Reg0x7_v_vld_num                            0
#define bitJPEG_DEC_Reg0x7_v_vld_num                            0xF
#define set_JPEG_DEC_Reg0x7_v_vld_num(val)                      addJPEG_DEC_Reg0x7 = ((addJPEG_DEC_Reg0x7 & (~0xF)) | ((val) << 0))
#define get_JPEG_DEC_Reg0x7_v_vld_num                           (addJPEG_DEC_Reg0x7 & 0xF)

//addJPEG_DEC_Reg0x8
#define addJPEG_DEC_Reg0x8                                      *((volatile unsigned long *) (0x48040000+0x8*4))
#define posJPEG_DEC_Reg0x8_DEC_CMD                              0
#define bitJPEG_DEC_Reg0x8_DEC_CMD                              0xF
#define set_JPEG_DEC_Reg0x8_DEC_CMD(val)                        addJPEG_DEC_Reg0x8 = ((addJPEG_DEC_Reg0x8 & (~0xF)) | ((val) << 0))
#define get_JPEG_DEC_Reg0x8_DEC_CMD                             (addJPEG_DEC_Reg0x8 & 0xF)

//addJPEG_DEC_Reg0x9
#define addJPEG_DEC_Reg0x9                                      *((volatile unsigned long *) (0x48040000+0x9*4))

//addJPEG_DEC_Reg0xa
#define addJPEG_DEC_Reg0xa                                      *((volatile unsigned long *) (0x48040000+0xa*4))

#define posJPEG_DEC_Reg0xa_x_pixel                              0
#define bitJPEG_DEC_Reg0xa_x_pixel                              0xFFFF
#define set_JPEG_DEC_Reg0xa_x_pixel(val)                        addJPEG_DEC_Reg0xa = ((addJPEG_DEC_Reg0xa & (~0xFFFF)) | ((val) << 0))
#define get_JPEG_DEC_Reg0xa_x_pixel                             (addJPEG_DEC_Reg0xa & 0xFFFF)

//addJPEG_DEC_Reg0xb
#define addJPEG_DEC_Reg0xb                                      *((volatile unsigned long *) (0x48040000+0xb*4))

//addJPEG_DEC_Reg0xc
#define addJPEG_DEC_Reg0xc                                      *((volatile unsigned long *) (0x48040000+0xc*4))

//addJPEG_DEC_Reg0xd
#define addJPEG_DEC_Reg0xd                                      *((volatile unsigned long *) (0x48040000+0xd*4))

#define posJPEG_DEC_Reg0xd_state_dec_busy2                      8
#define bitJPEG_DEC_Reg0xd_state_dec_busy2                      0x100
#define get_JPEG_DEC_Reg0xd_state_dec_busy2                     ((addJPEG_DEC_Reg0xd & 0x100) >> 8)

#define posJPEG_DEC_Reg0xd_tmp_wr0                              7
#define bitJPEG_DEC_Reg0xd_tmp_wr0                              0x80
#define get_JPEG_DEC_Reg0xd_tmp_wr0                             ((addJPEG_DEC_Reg0xd & 0x80) >> 7)

#define posJPEG_DEC_Reg0xd_state_idct                           6
#define bitJPEG_DEC_Reg0xd_state_idct                           0x40
#define get_JPEG_DEC_Reg0xd_state_idct                          ((addJPEG_DEC_Reg0xd & 0x40) >> 6)

#define posJPEG_DEC_Reg0xd_state_ext_bits                       5
#define bitJPEG_DEC_Reg0xd_state_ext_bits                       0x20
#define get_JPEG_DEC_Reg0xd_state_ext_bits                      ((addJPEG_DEC_Reg0xd & 0x20) >> 5)

#define posJPEG_DEC_Reg0xd_state_ext_rload                      4
#define bitJPEG_DEC_Reg0xd_state_ext_rload                      0x10
#define get_JPEG_DEC_Reg0xd_state_ext_rload                     ((addJPEG_DEC_Reg0xd & 0x10) >> 4)

#define posJPEG_DEC_Reg0xd_state_search                         3
#define bitJPEG_DEC_Reg0xd_state_search                         0x8
#define get_JPEG_DEC_Reg0xd_state_search                        ((addJPEG_DEC_Reg0xd & 0x8) >> 3)

#define posJPEG_DEC_Reg0xd_state_rload                          2
#define bitJPEG_DEC_Reg0xd_state_rload                          0x4
#define get_JPEG_DEC_Reg0xd_state_rload                         ((addJPEG_DEC_Reg0xd & 0x4) >> 2)

#define posJPEG_DEC_Reg0xd_state_rrload                         1
#define bitJPEG_DEC_Reg0xd_state_rrload                         0x2
#define get_JPEG_DEC_Reg0xd_state_rrload                        ((addJPEG_DEC_Reg0xd & 0x2) >> 1)

#define posJPEG_DEC_Reg0xd_state_dec_busy                       0
#define bitJPEG_DEC_Reg0xd_state_dec_busy                       0x1
#define get_JPEG_DEC_Reg0xd_state_dec_busy                      (addJPEG_DEC_Reg0xd & 0x1)

//addJPEG_DEC_Reg0xe
#define addJPEG_DEC_Reg0xe                                      *((volatile unsigned long *) (0x48040000+0xe*4))

//addJPEG_DEC_Reg0xf
#define addJPEG_DEC_Reg0xf                                      *((volatile unsigned long *) (0x48040000+0xf*4))

#define posJPEG_DEC_Reg0xf_mcu_blk                              0
#define bitJPEG_DEC_Reg0xf_mcu_blk                              0xFF
#define set_JPEG_DEC_Reg0xf_mcu_blk(val)                        addJPEG_DEC_Reg0xf = ((addJPEG_DEC_Reg0xf & (~0xFF)) | ((val) << 0))
#define get_JPEG_DEC_Reg0xf_mcu_blk                             (addJPEG_DEC_Reg0xf & 0xFF)

//addJPEG_DEC_Reg0x10
#define addJPEG_DEC_Reg0x10                                     *((volatile unsigned long *) (0x48040000+0x10*4))

//addJPEG_DEC_Reg0x11
#define addJPEG_DEC_Reg0x11                                     *((volatile unsigned long *) (0x48040000+0x11*4))

//addJPEG_DEC_Reg0x12
#define addJPEG_DEC_Reg0x12                                     *((volatile unsigned long *) (0x48040000+0x12*4))

//addJPEG_DEC_Reg0x13
#define addJPEG_DEC_Reg0x13                                     *((volatile unsigned long *) (0x48040000+0x13*4))

//addJPEG_DEC_Reg0x14
#define addJPEG_DEC_Reg0x14                                     *((volatile unsigned long *) (0x48040000+0x14*4))

//addJPEG_DEC_Reg0x15
#define addJPEG_DEC_Reg0x15                                     *((volatile unsigned long *) (0x48040000+0x15*4))

//addJPEG_DEC_Reg0x16
#define addJPEG_DEC_Reg0x16                                     *((volatile unsigned long *) (0x48040000+0x16*4))

//addJPEG_DEC_Reg0x17
#define addJPEG_DEC_Reg0x17                                     *((volatile unsigned long *) (0x48040000+0x17*4))

//addJPEG_DEC_Reg0x18
#define addJPEG_DEC_Reg0x18                                     *((volatile unsigned long *) (0x48040000+0x18*4))

//addJPEG_DEC_Reg0x19
#define addJPEG_DEC_Reg0x19                                     *((volatile unsigned long *) (0x48040000+0x19*4))

//addJPEG_DEC_Reg0x1a
#define addJPEG_DEC_Reg0x1a                                     *((volatile unsigned long *) (0x48040000+0x1a*4))

//addJPEG_DEC_Reg0x1b
#define addJPEG_DEC_Reg0x1b                                     *((volatile unsigned long *) (0x48040000+0x1b*4))

//addJPEG_DEC_Reg0x1c
#define addJPEG_DEC_Reg0x1c                                     *((volatile unsigned long *) (0x48040000+0x1c*4))

//addJPEG_DEC_Reg0x1d
#define addJPEG_DEC_Reg0x1d                                     *((volatile unsigned long *) (0x48040000+0x1d*4))

//addJPEG_DEC_Reg0x1e
#define addJPEG_DEC_Reg0x1e                                     *((volatile unsigned long *) (0x48040000+0x1e*4))

//addJPEG_DEC_Reg0x1f
#define addJPEG_DEC_Reg0x1f                                     *((volatile unsigned long *) (0x48040000+0x1f*4))

//addJPEG_DEC_Reg0x20
#define addJPEG_DEC_Reg0x20                                     *((volatile unsigned long *) (0x48040000+0x20*4))

//addJPEG_DEC_Reg0x21
#define addJPEG_DEC_Reg0x21                                     *((volatile unsigned long *) (0x48040000+0x21*4))

//addJPEG_DEC_Reg0x22
#define addJPEG_DEC_Reg0x22                                     *((volatile unsigned long *) (0x48040000+0x22*4))

//addJPEG_DEC_Reg0x23
#define addJPEG_DEC_Reg0x23                                     *((volatile unsigned long *) (0x48040000+0x23*4))

//addJPEG_DEC_Reg0x24
#define addJPEG_DEC_Reg0x24                                     *((volatile unsigned long *) (0x48040000+0x24*4))

//addJPEG_DEC_Reg0x25
#define addJPEG_DEC_Reg0x25                                     *((volatile unsigned long *) (0x48040000+0x25*4))

//addJPEG_DEC_Reg0x26
#define addJPEG_DEC_Reg0x26                                     *((volatile unsigned long *) (0x48040000+0x26*4))

//addJPEG_DEC_Reg0x27
#define addJPEG_DEC_Reg0x27                                     *((volatile unsigned long *) (0x48040000+0x27*4))

//addJPEG_DEC_Reg0x28
#define addJPEG_DEC_Reg0x28                                     *((volatile unsigned long *) (0x48040000+0x28*4))

//addJPEG_DEC_Reg0x29
#define addJPEG_DEC_Reg0x29                                     *((volatile unsigned long *) (0x48040000+0x29*4))

//addJPEG_DEC_Reg0x2a
#define addJPEG_DEC_Reg0x2a                                     *((volatile unsigned long *) (0x48040000+0x2a*4))

//addJPEG_DEC_Reg0x2b
#define addJPEG_DEC_Reg0x2b                                     *((volatile unsigned long *) (0x48040000+0x2b*4))

//addJPEG_DEC_Reg0x2c
#define addJPEG_DEC_Reg0x2c                                     *((volatile unsigned long *) (0x48040000+0x2c*4))

//addJPEG_DEC_Reg0x2d
#define addJPEG_DEC_Reg0x2d                                     *((volatile unsigned long *) (0x48040000+0x2d*4))

//addJPEG_DEC_Reg0x2e
#define addJPEG_DEC_Reg0x2e                                     *((volatile unsigned long *) (0x48040000+0x2e*4))

//addJPEG_DEC_Reg0x2f
#define addJPEG_DEC_Reg0x2f                                     *((volatile unsigned long *) (0x48040000+0x2f*4))

//addJPEG_DEC_Reg0x30
#define addJPEG_DEC_Reg0x30                                     *((volatile unsigned long *) (0x48040000+0x30*4))

//addJPEG_DEC_Reg0x31
#define addJPEG_DEC_Reg0x31                                     *((volatile unsigned long *) (0x48040000+0x31*4))

//addJPEG_DEC_Reg0x32
#define addJPEG_DEC_Reg0x32                                     *((volatile unsigned long *) (0x48040000+0x32*4))

//addJPEG_DEC_Reg0x33
#define addJPEG_DEC_Reg0x33                                     *((volatile unsigned long *) (0x48040000+0x33*4))

//addJPEG_DEC_Reg0x34
#define addJPEG_DEC_Reg0x34                                     *((volatile unsigned long *) (0x48040000+0x34*4))

//addJPEG_DEC_Reg0x35
#define addJPEG_DEC_Reg0x35                                     *((volatile unsigned long *) (0x48040000+0x35*4))

//addJPEG_DEC_Reg0x36
#define addJPEG_DEC_Reg0x36                                     *((volatile unsigned long *) (0x48040000+0x36*4))

//addJPEG_DEC_Reg0x37
#define addJPEG_DEC_Reg0x37                                     *((volatile unsigned long *) (0x48040000+0x37*4))

//addJPEG_DEC_Reg0x38
#define addJPEG_DEC_Reg0x38                                     *((volatile unsigned long *) (0x48040000+0x38*4))

//addJPEG_DEC_Reg0x39
#define addJPEG_DEC_Reg0x39                                     *((volatile unsigned long *) (0x48040000+0x39*4))

//addJPEG_DEC_Reg0x3a
#define addJPEG_DEC_Reg0x3a                                     *((volatile unsigned long *) (0x48040000+0x3a*4))

//addJPEG_DEC_Reg0x3b
#define addJPEG_DEC_Reg0x3b                                     *((volatile unsigned long *) (0x48040000+0x3b*4))

//addJPEG_DEC_Reg0x3c
#define addJPEG_DEC_Reg0x3c                                     *((volatile unsigned long *) (0x48040000+0x3c*4))

//addJPEG_DEC_Reg0x3d
#define addJPEG_DEC_Reg0x3d                                     *((volatile unsigned long *) (0x48040000+0x3d*4))

//addJPEG_DEC_Reg0x3e
#define addJPEG_DEC_Reg0x3e                                     *((volatile unsigned long *) (0x48040000+0x3e*4))

//addJPEG_DEC_Reg0x3f
#define addJPEG_DEC_Reg0x3f                                     *((volatile unsigned long *) (0x48040000+0x3f*4))

//addJPEG_DEC_Reg0x40
#define addJPEG_DEC_Reg0x40                                     *((volatile unsigned long *) (0x48040000+0x40*4))

//addJPEG_DEC_Reg0x41
#define addJPEG_DEC_Reg0x41                                     *((volatile unsigned long *) (0x48040000+0x41*4))

//addJPEG_DEC_Reg0x42
#define addJPEG_DEC_Reg0x42                                     *((volatile unsigned long *) (0x48040000+0x42*4))

//addJPEG_DEC_Reg0x43
#define addJPEG_DEC_Reg0x43                                     *((volatile unsigned long *) (0x48040000+0x43*4))

//addJPEG_DEC_Reg0x44
#define addJPEG_DEC_Reg0x44                                     *((volatile unsigned long *) (0x48040000+0x44*4))

//addJPEG_DEC_Reg0x45
#define addJPEG_DEC_Reg0x45                                     *((volatile unsigned long *) (0x48040000+0x45*4))

//addJPEG_DEC_Reg0x46
#define addJPEG_DEC_Reg0x46                                     *((volatile unsigned long *) (0x48040000+0x46*4))

//addJPEG_DEC_Reg0x47
#define addJPEG_DEC_Reg0x47                                     *((volatile unsigned long *) (0x48040000+0x47*4))

//addJPEG_DEC_Reg0x48
#define addJPEG_DEC_Reg0x48                                     *((volatile unsigned long *) (0x48040000+0x48*4))

//addJPEG_DEC_Reg0x49
#define addJPEG_DEC_Reg0x49                                     *((volatile unsigned long *) (0x48040000+0x49*4))

//addJPEG_DEC_Reg0x4a
#define addJPEG_DEC_Reg0x4a                                     *((volatile unsigned long *) (0x48040000+0x4a*4))

//addJPEG_DEC_Reg0x4b
#define addJPEG_DEC_Reg0x4b                                     *((volatile unsigned long *) (0x48040000+0x4b*4))

//addJPEG_DEC_Reg0x4c
#define addJPEG_DEC_Reg0x4c                                     *((volatile unsigned long *) (0x48040000+0x4c*4))

//addJPEG_DEC_Reg0x4d
#define addJPEG_DEC_Reg0x4d                                     *((volatile unsigned long *) (0x48040000+0x4d*4))

//addJPEG_DEC_Reg0x4e
#define addJPEG_DEC_Reg0x4e                                     *((volatile unsigned long *) (0x48040000+0x4e*4))

//addJPEG_DEC_Reg0x4f
#define addJPEG_DEC_Reg0x4f                                     *((volatile unsigned long *) (0x48040000+0x4f*4))

//addJPEG_DEC_Reg0x58
#define addJPEG_DEC_Reg0x58                                     *((volatile unsigned long *) (0x48040000+0x58*4))

//addJPEG_DEC_Reg0x59
#define addJPEG_DEC_Reg0x59                                     *((volatile unsigned long *) (0x48040000+0x59*4))

//addJPEG_DEC_Reg0x5a
#define addJPEG_DEC_Reg0x5a                                     *((volatile unsigned long *) (0x48040000+0x5a*4))

//addJPEG_DEC_Reg0x5b
#define addJPEG_DEC_Reg0x5b                                     *((volatile unsigned long *) (0x48040000+0x5b*4))

//addJPEG_DEC_Reg0x5c
#define addJPEG_DEC_Reg0x5c                                     *((volatile unsigned long *) (0x48040000+0x5c*4))

//addJPEG_DEC_Reg0x80
#define addJPEG_DEC_Reg0x80                                     *((volatile unsigned long *) (0x48040000+0x80*4))

//addJPEG_DEC_Reg0xc0
#define addJPEG_DEC_Reg0xc0                                     *((volatile unsigned long *) (0x48040000+0xc0*4))

//addJPEG_DEC_Reg0x100
#define addJPEG_DEC_Reg0x100                                    *((volatile unsigned long *) (0x48040000+0x100*4))

//addJPEG_DEC_Reg0x200
#define addJPEG_DEC_Reg0x200                                    *((volatile unsigned long *) (0x48040000+0x200*4))

//addJPEG_DEC_Reg0x300
#define addJPEG_DEC_Reg0x300                                    *((volatile unsigned long *) (0x48040000+0x300*4))

//addJPEG_DEC_Reg0x340
#define addJPEG_DEC_Reg0x340                                    *((volatile unsigned long *) (0x48040000+0x340*4))

//addJPEG_DEC_Reg0x380
#define addJPEG_DEC_Reg0x380                                    *((volatile unsigned long *) (0x48040000+0x380*4))

//addJPEG_DEC_Reg0x3c0
#define addJPEG_DEC_Reg0x3c0                                    *((volatile unsigned long *) (0x48040000+0x3c0*4))

//************************************************************//
//IRDA
//************************************************************//
#define BASEADDR_IRDA                                           0x44890000
//addIRDA_Reg0x0
#define addIRDA_Reg0x0                                          *((volatile unsigned long *) (0x44890000+0x0*4))
#define posIRDA_Reg0x0_txdata_num                               16
#define bitIRDA_Reg0x0_txdata_num                               0x3FF0000
#define set_IRDA_Reg0x0_txdata_num(val)                         addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x3FF0000)) | ((val) << 16))
#define get_IRDA_Reg0x0_txdata_num                              ((addIRDA_Reg0x0 & 0x3FF0000) >> 16)

#define posIRDA_Reg0x0_RSTN                                     15
#define bitIRDA_Reg0x0_RSTN                                     0x8000
#define set_IRDA_Reg0x0_RSTN(val)                               addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x8000)) | ((val) << 15))
#define setf_IRDA_Reg0x0_RSTN                                   addIRDA_Reg0x0 |= 0x8000
#define clrf_IRDA_Reg0x0_RSTN                                   addIRDA_Reg0x0 &= ~0x8000
#define get_IRDA_Reg0x0_RSTN                                    ((addIRDA_Reg0x0 & 0x8000) >> 15)

#define posIRDA_Reg0x0_RFU1                                     14
#define bitIRDA_Reg0x0_RFU1                                     0x4000
#define set_IRDA_Reg0x0_RFU1(val)                               addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x4000)) | ((val) << 14))
#define setf_IRDA_Reg0x0_RFU1                                   addIRDA_Reg0x0 |= 0x4000
#define clrf_IRDA_Reg0x0_RFU1                                   addIRDA_Reg0x0 &= ~0x4000
#define get_IRDA_Reg0x0_RFU1                                    ((addIRDA_Reg0x0 & 0x4000) >> 14)

#define posIRDA_Reg0x0_RFU2                                     13
#define bitIRDA_Reg0x0_RFU2                                     0x2000
#define set_IRDA_Reg0x0_RFU2(val)                               addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x2000)) | ((val) << 13))
#define setf_IRDA_Reg0x0_RFU2                                   addIRDA_Reg0x0 |= 0x2000
#define clrf_IRDA_Reg0x0_RFU2                                   addIRDA_Reg0x0 &= ~0x2000
#define get_IRDA_Reg0x0_RFU2                                    ((addIRDA_Reg0x0 & 0x2000) >> 13)

#define posIRDA_Reg0x0_clk_freq_in                              8
#define bitIRDA_Reg0x0_clk_freq_in                              0x1F00
#define set_IRDA_Reg0x0_clk_freq_in(val)                        addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x1F00)) | ((val) << 8))
#define get_IRDA_Reg0x0_clk_freq_in                             ((addIRDA_Reg0x0 & 0x1F00) >> 8)

#define posIRDA_Reg0x0_RFU3                                     6
#define bitIRDA_Reg0x0_RFU3                                     0xC0
#define set_IRDA_Reg0x0_RFU3(val)                               addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0xC0)) | ((val) << 6))
#define get_IRDA_Reg0x0_RFU3                                    ((addIRDA_Reg0x0 & 0xC0) >> 6)

#define posIRDA_Reg0x0_irda_pwd                                 5
#define bitIRDA_Reg0x0_irda_pwd                                 0x20
#define set_IRDA_Reg0x0_irda_pwd(val)                           addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_IRDA_Reg0x0_irda_pwd                               addIRDA_Reg0x0 |= 0x20
#define clrf_IRDA_Reg0x0_irda_pwd                               addIRDA_Reg0x0 &= ~0x20
#define get_IRDA_Reg0x0_irda_pwd                                ((addIRDA_Reg0x0 & 0x20) >> 5)

#define posIRDA_Reg0x0_tx_start                                 4
#define bitIRDA_Reg0x0_tx_start                                 0x10
#define set_IRDA_Reg0x0_tx_start(val)                           addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_IRDA_Reg0x0_tx_start                               addIRDA_Reg0x0 |= 0x10
#define clrf_IRDA_Reg0x0_tx_start                               addIRDA_Reg0x0 &= ~0x10
#define get_IRDA_Reg0x0_tx_start                                ((addIRDA_Reg0x0 & 0x10) >> 4)

#define posIRDA_Reg0x0_tx_initial_level                         3
#define bitIRDA_Reg0x0_tx_initial_level                         0x8
#define set_IRDA_Reg0x0_tx_initial_level(val)                   addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x8)) | ((val) << 3))
#define setf_IRDA_Reg0x0_tx_initial_level                       addIRDA_Reg0x0 |= 0x8
#define clrf_IRDA_Reg0x0_tx_initial_level                       addIRDA_Reg0x0 &= ~0x8
#define get_IRDA_Reg0x0_tx_initial_level                        ((addIRDA_Reg0x0 & 0x8) >> 3)

#define posIRDA_Reg0x0_txenable                                 2
#define bitIRDA_Reg0x0_txenable                                 0x4
#define set_IRDA_Reg0x0_txenable(val)                           addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_IRDA_Reg0x0_txenable                               addIRDA_Reg0x0 |= 0x4
#define clrf_IRDA_Reg0x0_txenable                               addIRDA_Reg0x0 &= ~0x4
#define get_IRDA_Reg0x0_txenable                                ((addIRDA_Reg0x0 & 0x4) >> 2)

#define posIRDA_Reg0x0_rx_initial_level                         1
#define bitIRDA_Reg0x0_rx_initial_level                         0x2
#define set_IRDA_Reg0x0_rx_initial_level(val)                   addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_IRDA_Reg0x0_rx_initial_level                       addIRDA_Reg0x0 |= 0x2
#define clrf_IRDA_Reg0x0_rx_initial_level                       addIRDA_Reg0x0 &= ~0x2
#define get_IRDA_Reg0x0_rx_initial_level                        ((addIRDA_Reg0x0 & 0x2) >> 1)

#define posIRDA_Reg0x0_rxenable                                 0
#define bitIRDA_Reg0x0_rxenable                                 0x1
#define set_IRDA_Reg0x0_rxenable(val)                           addIRDA_Reg0x0 = ((addIRDA_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_IRDA_Reg0x0_rxenable                               addIRDA_Reg0x0 |= 0x1
#define clrf_IRDA_Reg0x0_rxenable                               addIRDA_Reg0x0 &= ~0x1
#define get_IRDA_Reg0x0_rxenable                                (addIRDA_Reg0x0 & 0x1)

//addIRDA_Reg0x1
#define addIRDA_Reg0x1                                          *((volatile unsigned long *) (0x44890000+0x1*4))
#define posIRDA_Reg0x1_rx_timeout_cnt                           16
#define bitIRDA_Reg0x1_rx_timeout_cnt                           0xFFFF0000
#define set_IRDA_Reg0x1_rx_timeout_cnt(val)                     addIRDA_Reg0x1 = ((addIRDA_Reg0x1 & (~0xFFFF0000)) | ((val) << 16))
#define get_IRDA_Reg0x1_rx_timeout_cnt                          ((addIRDA_Reg0x1 & 0xFFFF0000) >> 16)

#define posIRDA_Reg0x1_RX_FIFO_THRESHOLD                        8
#define bitIRDA_Reg0x1_RX_FIFO_THRESHOLD                        0xFF00
#define set_IRDA_Reg0x1_RX_FIFO_THRESHOLD(val)                  addIRDA_Reg0x1 = ((addIRDA_Reg0x1 & (~0xFF00)) | ((val) << 8))
#define get_IRDA_Reg0x1_RX_FIFO_THRESHOLD                       ((addIRDA_Reg0x1 & 0xFF00) >> 8)

#define posIRDA_Reg0x1_TX_FIFO_THRESHOLD                        0
#define bitIRDA_Reg0x1_TX_FIFO_THRESHOLD                        0xFF
#define set_IRDA_Reg0x1_TX_FIFO_THRESHOLD(val)                  addIRDA_Reg0x1 = ((addIRDA_Reg0x1 & (~0xFF)) | ((val) << 0))
#define get_IRDA_Reg0x1_TX_FIFO_THRESHOLD                       (addIRDA_Reg0x1 & 0xFF)

//addIRDA_Reg0x2
#define addIRDA_Reg0x2                                          *((volatile unsigned long *) (0x44890000+0x2*4))
#define posIRDA_Reg0x2_TX_FIFO_COUNT                            0
#define bitIRDA_Reg0x2_TX_FIFO_COUNT                            0xFF
#define get_IRDA_Reg0x2_TX_FIFO_COUNT                           (addIRDA_Reg0x2 & 0xFF)

#define posIRDA_Reg0x2_RX_FIFO_COUNT                            8
#define bitIRDA_Reg0x2_RX_FIFO_COUNT                            0xFF00
#define get_IRDA_Reg0x2_RX_FIFO_COUNT                           ((addIRDA_Reg0x2 & 0xFF00) >> 8)

#define posIRDA_Reg0x2_TX_FIFO_FULL                             16
#define bitIRDA_Reg0x2_TX_FIFO_FULL                             0x10000
#define get_IRDA_Reg0x2_TX_FIFO_FULL                            ((addIRDA_Reg0x2 & 0x10000) >> 16)

#define posIRDA_Reg0x2_TX_FIFO_EMPTY                            17
#define bitIRDA_Reg0x2_TX_FIFO_EMPTY                            0x20000
#define get_IRDA_Reg0x2_TX_FIFO_EMPTY                           ((addIRDA_Reg0x2 & 0x20000) >> 17)

#define posIRDA_Reg0x2_RX_FIFO_FULL                             18
#define bitIRDA_Reg0x2_RX_FIFO_FULL                             0x40000
#define get_IRDA_Reg0x2_RX_FIFO_FULL                            ((addIRDA_Reg0x2 & 0x40000) >> 18)

#define posIRDA_Reg0x2_RX_FIFO_EMPTY                            19
#define bitIRDA_Reg0x2_RX_FIFO_EMPTY                            0x80000
#define get_IRDA_Reg0x2_RX_FIFO_EMPTY                           ((addIRDA_Reg0x2 & 0x80000) >> 19)

#define posIRDA_Reg0x2_FIFO_WR_READY                            20
#define bitIRDA_Reg0x2_FIFO_WR_READY                            0x100000
#define get_IRDA_Reg0x2_FIFO_WR_READY                           ((addIRDA_Reg0x2 & 0x100000) >> 20)

#define posIRDA_Reg0x2_FIFO_RD_READY                            21
#define bitIRDA_Reg0x2_FIFO_RD_READY                            0x200000
#define get_IRDA_Reg0x2_FIFO_RD_READY                           ((addIRDA_Reg0x2 & 0x200000) >> 21)

#define posIRDA_Reg0x2_rxdata_num                               22
#define bitIRDA_Reg0x2_rxdata_num                               0xFFC00000
#define get_IRDA_Reg0x2_rxdata_num                              ((addIRDA_Reg0x2 & 0xFFC00000) >> 22)

//addIRDA_Reg0x3
#define addIRDA_Reg0x3                                          *((volatile unsigned long *) (0x44890000+0x3*4))
#define posIRDA_Reg0x3_fifo_data_tx                             16
#define bitIRDA_Reg0x3_fifo_data_tx                             0xFFFF0000
#define set_IRDA_Reg0x3_fifo_data_tx(val)                       addIRDA_Reg0x3 = ((addIRDA_Reg0x3 & (~0xFFFF0000)) | ((val) << 16))
#define get_IRDA_Reg0x3_fifo_data_tx                            ((addIRDA_Reg0x3 & 0xFFFF0000) >> 16)

#define posIRDA_Reg0x3_fifo_data_rx                             0
#define bitIRDA_Reg0x3_fifo_data_rx                             0xFFFF
#define set_IRDA_Reg0x3_fifo_data_rx(val)                       addIRDA_Reg0x3 = ((addIRDA_Reg0x3 & (~0xFFFF)) | ((val) << 0))
#define get_IRDA_Reg0x3_fifo_data_rx                            (addIRDA_Reg0x3 & 0xFFFF)

//addIRDA_Reg0x4
#define addIRDA_Reg0x4                                          *((volatile unsigned long *) (0x44890000+0x4*4))
#define posIRDA_Reg0x4_RFU                                      5
#define bitIRDA_Reg0x4_RFU                                      0xFFFFFFE0
#define set_IRDA_Reg0x4_RFU(val)                                addIRDA_Reg0x4 = ((addIRDA_Reg0x4 & (~0xFFFFFFE0)) | ((val) << 5))
#define get_IRDA_Reg0x4_RFU                                     ((addIRDA_Reg0x4 & 0xFFFFFFE0) >> 5)

#define posIRDA_Reg0x4_rx_overflow_status                       4
#define bitIRDA_Reg0x4_rx_overflow_status                       0x10
#define set_IRDA_Reg0x4_rx_overflow_status(val)                 addIRDA_Reg0x4 = ((addIRDA_Reg0x4 & (~0x10)) | ((val) << 4))
#define setf_IRDA_Reg0x4_rx_overflow_status                     addIRDA_Reg0x4 |= 0x10
#define clrf_IRDA_Reg0x4_rx_overflow_status                     addIRDA_Reg0x4 &= ~0x10
#define get_IRDA_Reg0x4_rx_overflow_status                      ((addIRDA_Reg0x4 & 0x10) >> 4)

#define posIRDA_Reg0x4_rx_timeout_mask                          3
#define bitIRDA_Reg0x4_rx_timeout_mask                          0x8
#define set_IRDA_Reg0x4_rx_timeout_mask(val)                    addIRDA_Reg0x4 = ((addIRDA_Reg0x4 & (~0x8)) | ((val) << 3))
#define setf_IRDA_Reg0x4_rx_timeout_mask                        addIRDA_Reg0x4 |= 0x8
#define clrf_IRDA_Reg0x4_rx_timeout_mask                        addIRDA_Reg0x4 &= ~0x8
#define get_IRDA_Reg0x4_rx_timeout_mask                         ((addIRDA_Reg0x4 & 0x8) >> 3)

#define posIRDA_Reg0x4_tx_done_mask                             2
#define bitIRDA_Reg0x4_tx_done_mask                             0x4
#define set_IRDA_Reg0x4_tx_done_mask(val)                       addIRDA_Reg0x4 = ((addIRDA_Reg0x4 & (~0x4)) | ((val) << 2))
#define setf_IRDA_Reg0x4_tx_done_mask                           addIRDA_Reg0x4 |= 0x4
#define clrf_IRDA_Reg0x4_tx_done_mask                           addIRDA_Reg0x4 &= ~0x4
#define get_IRDA_Reg0x4_tx_done_mask                            ((addIRDA_Reg0x4 & 0x4) >> 2)

#define posIRDA_Reg0x4_rx_need_rd_mask                          1
#define bitIRDA_Reg0x4_rx_need_rd_mask                          0x2
#define set_IRDA_Reg0x4_rx_need_rd_mask(val)                    addIRDA_Reg0x4 = ((addIRDA_Reg0x4 & (~0x2)) | ((val) << 1))
#define setf_IRDA_Reg0x4_rx_need_rd_mask                        addIRDA_Reg0x4 |= 0x2
#define clrf_IRDA_Reg0x4_rx_need_rd_mask                        addIRDA_Reg0x4 &= ~0x2
#define get_IRDA_Reg0x4_rx_need_rd_mask                         ((addIRDA_Reg0x4 & 0x2) >> 1)

#define posIRDA_Reg0x4_tx_need_wr_mask                          0
#define bitIRDA_Reg0x4_tx_need_wr_mask                          0x1
#define set_IRDA_Reg0x4_tx_need_wr_mask(val)                    addIRDA_Reg0x4 = ((addIRDA_Reg0x4 & (~0x1)) | ((val) << 0))
#define setf_IRDA_Reg0x4_tx_need_wr_mask                        addIRDA_Reg0x4 |= 0x1
#define clrf_IRDA_Reg0x4_tx_need_wr_mask                        addIRDA_Reg0x4 &= ~0x1
#define get_IRDA_Reg0x4_tx_need_wr_mask                         (addIRDA_Reg0x4 & 0x1)

//addIRDA_Reg0x5
#define addIRDA_Reg0x5                                          *((volatile unsigned long *) (0x44890000+0x5*4))
#define posIRDA_Reg0x5_RFU                                      5
#define bitIRDA_Reg0x5_RFU                                      0xFFFFFFE0
#define set_IRDA_Reg0x5_RFU(val)                                addIRDA_Reg0x5 = ((addIRDA_Reg0x5 & (~0xFFFFFFE0)) | ((val) << 5))
#define get_IRDA_Reg0x5_RFU                                     ((addIRDA_Reg0x5 & 0xFFFFFFE0) >> 5)

#define posIRDA_Reg0x5_rx_overflow_status                       4
#define bitIRDA_Reg0x5_rx_overflow_status                       0x10
#define set_IRDA_Reg0x5_rx_overflow_status(val)                 addIRDA_Reg0x5 = ((addIRDA_Reg0x5 & (~0x10)) | ((val) << 4))
#define setf_IRDA_Reg0x5_rx_overflow_status                     addIRDA_Reg0x5 |= 0x10
#define clrf_IRDA_Reg0x5_rx_overflow_status                     addIRDA_Reg0x5 &= ~0x10
#define get_IRDA_Reg0x5_rx_overflow_status                      ((addIRDA_Reg0x5 & 0x10) >> 4)

#define posIRDA_Reg0x5_rx_done_status                           3
#define bitIRDA_Reg0x5_rx_done_status                           0x8
#define set_IRDA_Reg0x5_rx_done_status(val)                     addIRDA_Reg0x5 = ((addIRDA_Reg0x5 & (~0x8)) | ((val) << 3))
#define setf_IRDA_Reg0x5_rx_done_status                         addIRDA_Reg0x5 |= 0x8
#define clrf_IRDA_Reg0x5_rx_done_status                         addIRDA_Reg0x5 &= ~0x8
#define get_IRDA_Reg0x5_rx_done_status                          ((addIRDA_Reg0x5 & 0x8) >> 3)

#define posIRDA_Reg0x5_tx_done                                  2
#define bitIRDA_Reg0x5_tx_done                                  0x4
#define set_IRDA_Reg0x5_tx_done(val)                            addIRDA_Reg0x5 = ((addIRDA_Reg0x5 & (~0x4)) | ((val) << 2))
#define setf_IRDA_Reg0x5_tx_done                                addIRDA_Reg0x5 |= 0x4
#define clrf_IRDA_Reg0x5_tx_done                                addIRDA_Reg0x5 &= ~0x4
#define get_IRDA_Reg0x5_tx_done                                 ((addIRDA_Reg0x5 & 0x4) >> 2)

#define posIRDA_Reg0x5_rx_need_rd_state                         1
#define bitIRDA_Reg0x5_rx_need_rd_state                         0x2
#define set_IRDA_Reg0x5_rx_need_rd_state(val)                   addIRDA_Reg0x5 = ((addIRDA_Reg0x5 & (~0x2)) | ((val) << 1))
#define setf_IRDA_Reg0x5_rx_need_rd_state                       addIRDA_Reg0x5 |= 0x2
#define clrf_IRDA_Reg0x5_rx_need_rd_state                       addIRDA_Reg0x5 &= ~0x2
#define get_IRDA_Reg0x5_rx_need_rd_state                        ((addIRDA_Reg0x5 & 0x2) >> 1)

#define posIRDA_Reg0x5_tx_need_wr_state                         0
#define bitIRDA_Reg0x5_tx_need_wr_state                         0x1
#define set_IRDA_Reg0x5_tx_need_wr_state(val)                   addIRDA_Reg0x5 = ((addIRDA_Reg0x5 & (~0x1)) | ((val) << 0))
#define setf_IRDA_Reg0x5_tx_need_wr_state                       addIRDA_Reg0x5 |= 0x1
#define clrf_IRDA_Reg0x5_tx_need_wr_state                       addIRDA_Reg0x5 &= ~0x1
#define get_IRDA_Reg0x5_tx_need_wr_state                        (addIRDA_Reg0x5 & 0x1)

//addIRDA_Reg0x6
#define addIRDA_Reg0x6                                          *((volatile unsigned long *) (0x44890000+0x6*4))
#define posIRDA_Reg0x6_rx_start_thr                             20
#define bitIRDA_Reg0x6_rx_start_thr                             0xFFF00000
#define set_IRDA_Reg0x6_rx_start_thr(val)                       addIRDA_Reg0x6 = ((addIRDA_Reg0x6 & (~0xFFF00000)) | ((val) << 20))
#define get_IRDA_Reg0x6_rx_start_thr                            ((addIRDA_Reg0x6 & 0xFFF00000) >> 20)

#define posIRDA_Reg0x6_RFU                                      18
#define bitIRDA_Reg0x6_RFU                                      0xC0000
#define set_IRDA_Reg0x6_RFU(val)                                addIRDA_Reg0x6 = ((addIRDA_Reg0x6 & (~0xC0000)) | ((val) << 18))
#define get_IRDA_Reg0x6_RFU                                     ((addIRDA_Reg0x6 & 0xC0000) >> 18)

#define posIRDA_Reg0x6_carrier_enable                           17
#define bitIRDA_Reg0x6_carrier_enable                           0x20000
#define set_IRDA_Reg0x6_carrier_enable(val)                     addIRDA_Reg0x6 = ((addIRDA_Reg0x6 & (~0x20000)) | ((val) << 17))
#define setf_IRDA_Reg0x6_carrier_enable                         addIRDA_Reg0x6 |= 0x20000
#define clrf_IRDA_Reg0x6_carrier_enable                         addIRDA_Reg0x6 &= ~0x20000
#define get_IRDA_Reg0x6_carrier_enable                          ((addIRDA_Reg0x6 & 0x20000) >> 17)

#define posIRDA_Reg0x6_carrier_polarity                         16
#define bitIRDA_Reg0x6_carrier_polarity                         0x10000
#define set_IRDA_Reg0x6_carrier_polarity(val)                   addIRDA_Reg0x6 = ((addIRDA_Reg0x6 & (~0x10000)) | ((val) << 16))
#define setf_IRDA_Reg0x6_carrier_polarity                       addIRDA_Reg0x6 |= 0x10000
#define clrf_IRDA_Reg0x6_carrier_polarity                       addIRDA_Reg0x6 &= ~0x10000
#define get_IRDA_Reg0x6_carrier_polarity                        ((addIRDA_Reg0x6 & 0x10000) >> 16)

#define posIRDA_Reg0x6_carrier_duty                             8
#define bitIRDA_Reg0x6_carrier_duty                             0xFF00
#define set_IRDA_Reg0x6_carrier_duty(val)                       addIRDA_Reg0x6 = ((addIRDA_Reg0x6 & (~0xFF00)) | ((val) << 8))
#define get_IRDA_Reg0x6_carrier_duty                            ((addIRDA_Reg0x6 & 0xFF00) >> 8)

#define posIRDA_Reg0x6_carrier_period                           0
#define bitIRDA_Reg0x6_carrier_period                           0xFF
#define set_IRDA_Reg0x6_carrier_period(val)                     addIRDA_Reg0x6 = ((addIRDA_Reg0x6 & (~0xFF)) | ((val) << 0))
#define get_IRDA_Reg0x6_carrier_period                          (addIRDA_Reg0x6 & 0xFF)

//addIRDA_Reg0x7
#define addIRDA_Reg0x7                                          *((volatile unsigned long *) (0x44890000+0x7*4))
#define posIRDA_Reg0x7_RFU3                                     5
#define bitIRDA_Reg0x7_RFU3                                     0xFFFFFFE0
#define set_IRDA_Reg0x7_RFU3(val)                               addIRDA_Reg0x7 = ((addIRDA_Reg0x7 & (~0xFFFFFFE0)) | ((val) << 5))
#define get_IRDA_Reg0x7_RFU3                                    ((addIRDA_Reg0x7 & 0xFFFFFFE0) >> 5)

#define posIRDA_Reg0x7_glitch_thr                               16
#define bitIRDA_Reg0x7_glitch_thr                               0xFFF0000
#define set_IRDA_Reg0x7_glitch_thr(val)                         addIRDA_Reg0x7 = ((addIRDA_Reg0x7 & (~0xFFF0000)) | ((val) << 16))
#define get_IRDA_Reg0x7_glitch_thr                              ((addIRDA_Reg0x7 & 0xFFF0000) >> 16)

#define posIRDA_Reg0x7_RFU2                                     3
#define bitIRDA_Reg0x7_RFU2                                     0xFFF8
#define set_IRDA_Reg0x7_RFU2(val)                               addIRDA_Reg0x7 = ((addIRDA_Reg0x7 & (~0xFFF8)) | ((val) << 3))
#define get_IRDA_Reg0x7_RFU2                                    ((addIRDA_Reg0x7 & 0xFFF8) >> 3)

#define posIRDA_Reg0x7_RFU1                                     2
#define bitIRDA_Reg0x7_RFU1                                     0x4
#define set_IRDA_Reg0x7_RFU1(val)                               addIRDA_Reg0x7 = ((addIRDA_Reg0x7 & (~0x4)) | ((val) << 2))
#define setf_IRDA_Reg0x7_RFU1                                   addIRDA_Reg0x7 |= 0x4
#define clrf_IRDA_Reg0x7_RFU1                                   addIRDA_Reg0x7 &= ~0x4
#define get_IRDA_Reg0x7_RFU1                                    ((addIRDA_Reg0x7 & 0x4) >> 2)

#define posIRDA_Reg0x7_RFU0                                     1
#define bitIRDA_Reg0x7_RFU0                                     0x2
#define set_IRDA_Reg0x7_RFU0(val)                               addIRDA_Reg0x7 = ((addIRDA_Reg0x7 & (~0x2)) | ((val) << 1))
#define setf_IRDA_Reg0x7_RFU0                                   addIRDA_Reg0x7 |= 0x2
#define clrf_IRDA_Reg0x7_RFU0                                   addIRDA_Reg0x7 &= ~0x2
#define get_IRDA_Reg0x7_RFU0                                    ((addIRDA_Reg0x7 & 0x2) >> 1)

#define posIRDA_Reg0x7_glitch_enable                            0
#define bitIRDA_Reg0x7_glitch_enable                            0x1
#define set_IRDA_Reg0x7_glitch_enable(val)                      addIRDA_Reg0x7 = ((addIRDA_Reg0x7 & (~0x1)) | ((val) << 0))
#define setf_IRDA_Reg0x7_glitch_enable                          addIRDA_Reg0x7 |= 0x1
#define clrf_IRDA_Reg0x7_glitch_enable                          addIRDA_Reg0x7 &= ~0x1
#define get_IRDA_Reg0x7_glitch_enable                           (addIRDA_Reg0x7 & 0x1)

#endif
