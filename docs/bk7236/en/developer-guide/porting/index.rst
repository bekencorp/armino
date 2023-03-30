Migration Guide
-------------------------------------

:link_to_translation:`zh_CN:[中文]`


OS Migration Introduction
-------------------------------------------------------

This chapter takes the Hong Meng OS migration of BK7236 RISC-V architecture as an example

 - Operating system kernel source code path:components/os_source/
 - The operating system kernel source code is modified in 'components/os_source/CMakeLists.txt' and added to the Armino platform for compilation.
 - Define operating system related macros in 'components/bk_rtos/Kconfig'
   ::

    menu "LITEOS_M"

        config LITEOS_M
            bool "liteos_m"
            default n

        config LITEOS_M_V3
            bool "liteos_m v3.0.1-LTS"
            default n


        config LITEOS_M_BK
            depends on LITEOS_M_V3
                bool "liteos_m bk adapter"
                default n
    endmenu

 - Add Harmony Project：projects/harmony
 - Configure macro definitions related to the operating system：projects/harmony/sdkconfig.defaults
   ::

    # LITEOS_M
    CONFIG_LITEOS_M_V3=y
    CONFIG_LITEOS_M_BK=y

    # FreeRTOS
    CONFIG_FREERTOS=n
    CONFIG_FREERTOS_V9=n
    CONFIG_FREERTOS_V10=n

 - Implement OS abstract layer interface：components/bk_rtos/liteos_m_v3/rtos_pub.c
 - Create system startup assembly code：middleware/arch/riscv/boot_vectors_liteos_m.S
 - Key register of RISC-V: Set interrupt and exception entries：
   ::

    /* Initial machine trap-vector Base.*/
    la t0, HalTrapVector
    csrw mtvec, t0

 - Adapt exception handling function：trap_handler
    HalTrapEntry modify in components/os_source/liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/los_exc.S
 - Interrupt entry：HalHwiInterruptDone
 - Register the external interrupt handling function of the Armino platform：vPortSetupMEXTInterrupt
 - Register the MTIME interrupt handling function of the Armino platform：vPortSetupTimerInterrupt
 - Register external interrupts and MTIME interrupts before scheduling
   ::

    LITE_OS_SEC_TEXT_INIT UINT32 HalStartSchedule(VOID)
    {
        (VOID)LOS_IntLock();
        OsSchedStart();

    #if CONFIG_LITEOS_M_BK
        vPortSetupTimerInterrupt();
        vPortSetupMEXTInterrupt();
    #endif

        HalStartToRun();
        return LOS_OK; /* never return */
    }

 - BK7236 RISC-V The mtime register is 64 bits. When adapting, you should pay attention to the definition type of the maximum value.