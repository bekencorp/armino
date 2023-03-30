;/*
; * Copyright (c) 2009-2022 Arm Limited
; * Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
; * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 gcc_arm.ld
; */

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

#include "region_defs.h"

/* Include file with definitions for section alignments.
 * Note: it should be included after region_defs.h to let platform define
 * default values if needed. */
#include "tfm_s_linker_alignments.h"

MEMORY
{
  FLASH    (rx)  : ORIGIN = S_CODE_START, LENGTH = S_CODE_SIZE
  RAM      (rwx) : ORIGIN = S_DATA_START, LENGTH = S_DATA_SIZE
#if defined(S_CODE_SRAM_ALIAS_BASE)
  CODE_RAM (rwx) : ORIGIN = S_CODE_SRAM_ALIAS_BASE, LENGTH = TOTAL_CODE_SRAM_SIZE
#endif
}

__msp_stack_size__ = S_MSP_STACK_SIZE;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a libc_nano.a)

ENTRY(Reset_Handler)

SECTIONS
{
    .TFM_VECTORS : ALIGN(4)
    {
        __vectors_start__ = .;
        KEEP(*(.vectors))
        . = ALIGN(4);
        __vectors_end__ = .;
    } > FLASH

#ifdef CONFIG_TFM_USE_TRUSTZONE
    ASSERT(. <= ADDR(.TFM_VECTORS) + S_CODE_VECTOR_TABLE_SIZE, ".TFM_VECTORS section size overflow.")
    . = ADDR(.TFM_VECTORS) + S_CODE_VECTOR_TABLE_SIZE;
    /*
     * Place the CMSE Veneers (containing the SG instruction) after the code, in
     * a separate at least 32 bytes aligned region so that the SAU can
     * programmed to just set this region as Non-Secure Callable.
     */
    .gnu.sgstubs ALIGN(TFM_LINKER_VENEERS_ALIGNMENT) :
    {
        *(.gnu.sgstubs*)
        . = ALIGN(TFM_LINKER_VENEERS_ALIGNMENT);
    } > FLASH
    Image$$ER_VENEER$$Base = ADDR(.gnu.sgstubs);
    Image$$VENEER_ALIGN$$Limit = ADDR(.gnu.sgstubs) + SIZEOF(.gnu.sgstubs);
#endif

    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
#ifdef RAM_VECTORS_SUPPORT
        /* Copy interrupt vectors from flash to RAM */
        LONG (__vectors_start__)                            /* From */
        LONG (__ram_vectors_start__)                        /* To   */
        LONG ((__vectors_end__ - __vectors_start__) / 4)    /* Size */
#endif
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA) / 4)

        LONG (LOADADDR(.TFM_PSA_ROT_LINKER_DATA))
        LONG (ADDR(.TFM_PSA_ROT_LINKER_DATA))
        LONG (SIZEOF(.TFM_PSA_ROT_LINKER_DATA) / 4)

        LONG (LOADADDR(.TFM_APP_ROT_LINKER_DATA))
        LONG (ADDR(.TFM_APP_ROT_LINKER_DATA))
        LONG (SIZEOF(.TFM_APP_ROT_LINKER_DATA) / 4)

#if defined (S_RAM_CODE_START)
        LONG (LOADADDR(.TFM_RAM_CODE))
        LONG (ADDR(.TFM_RAM_CODE))
        LONG (SIZEOF(.TFM_RAM_CODE) / 4)
#endif
#if defined(S_CODE_SRAM_ALIAS_BASE)
        LONG (LOADADDR(.ER_CODE_SRAM))
        LONG (ADDR(.ER_CODE_SRAM))
        LONG (SIZEOF(.ER_CODE_SRAM) / 4)
#endif
        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS) / 4)
        LONG (ADDR(.TFM_PSA_ROT_LINKER_BSS))
        LONG (SIZEOF(.TFM_PSA_ROT_LINKER_BSS) / 4)

        LONG (ADDR(.TFM_APP_ROT_LINKER_BSS))
        LONG (SIZEOF(.TFM_APP_ROT_LINKER_BSS) / 4)
#if defined(CONFIG_TFM_PARTITION_META)
        LONG (ADDR(.TFM_SP_META_PTR))
        LONG (SIZEOF(.TFM_SP_META_PTR) / 4)
#endif
        __zero_table_end__ = .;
    } > FLASH

    /**** Section for holding partition RO load data */
    /*
     * Sort the partition info by priority to guarantee the initing order.
     * The first loaded partition will be inited at last in SFN model.
     */
    .TFM_SP_LOAD_LIST : ALIGN(4)
    {
       KEEP(*(.part_load_priority_lowest))
       KEEP(*(.part_load_priority_low))
       KEEP(*(.part_load_priority_normal))
       KEEP(*(.part_load_priority_high))
    } > FLASH
    Image$$TFM_SP_LOAD_LIST$$RO$$Base = ADDR(.TFM_SP_LOAD_LIST);
    Image$$TFM_SP_LOAD_LIST$$RO$$Limit = ADDR(.TFM_SP_LOAD_LIST) + SIZEOF(.TFM_SP_LOAD_LIST);

    /**** PSA RoT RO part (CODE + RODATA) start here */
    . = ALIGN(TFM_LINKER_PSA_ROT_LINKER_CODE_ALIGNMENT);
    Image$$TFM_PSA_CODE_START$$Base = .;

    .TFM_PSA_ROT_LINKER : ALIGN(TFM_LINKER_PSA_ROT_LINKER_CODE_ALIGNMENT)
    {
        *tfm_psa_rot_partition*:*(.text*)
        *tfm_psa_rot_partition*:*(.rodata*)
        *(TFM_*_PSA-ROT_ATTR_FN)
        . = ALIGN(TFM_LINKER_PSA_ROT_LINKER_CODE_ALIGNMENT);
    } > FLASH

    Image$$TFM_PSA_ROT_LINKER$$RO$$Base = ADDR(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_ROT_LINKER$$RO$$Limit = ADDR(.TFM_PSA_ROT_LINKER) + SIZEOF(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_ROT_LINKER$$Base = ADDR(.TFM_PSA_ROT_LINKER);
    Image$$TFM_PSA_ROT_LINKER$$Limit = ADDR(.TFM_PSA_ROT_LINKER) + SIZEOF(.TFM_PSA_ROT_LINKER);

    /**** PSA RoT RO part (CODE + RODATA) end here */
    Image$$TFM_PSA_CODE_END$$Base = .;

    /**** APPLICATION RoT RO part (CODE + RODATA) start here */
    Image$$TFM_APP_CODE_START$$Base = .;

    .TFM_APP_ROT_LINKER : ALIGN(TFM_LINKER_APP_ROT_LINKER_CODE_ALIGNMENT)
    {
        *tfm_app_rot_partition*:*(.text*)
        *tfm_app_rot_partition*:*(.rodata*)
        *(TFM_*_APP-ROT_ATTR_FN)
        . = ALIGN(TFM_LINKER_APP_ROT_LINKER_CODE_ALIGNMENT);
    } > FLASH

    Image$$TFM_APP_ROT_LINKER$$RO$$Base = ADDR(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_ROT_LINKER$$RO$$Limit = ADDR(.TFM_APP_ROT_LINKER) + SIZEOF(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_ROT_LINKER$$Base = ADDR(.TFM_APP_ROT_LINKER);
    Image$$TFM_APP_ROT_LINKER$$Limit = ADDR(.TFM_APP_ROT_LINKER) + SIZEOF(.TFM_APP_ROT_LINKER);

    /**** APPLICATION RoT RO part (CODE + RODATA) end here */
    Image$$TFM_APP_CODE_END$$Base = .;

#if defined(S_CODE_SRAM_ALIAS_BASE)
    .ER_CODE_SRAM : ALIGN(4)
    {
        *Driver_GFC100_EFlash.o(.text*)
        *Driver_GFC100_EFlash.o(.rodata*)
        *gfc100_eflash_drv.o(.text*)
        *gfc100_eflash_drv.o(.rodata*)
        *musca_b1_eflash_drv.o(.text*)
        *musca_b1_eflash_drv.o(.rodata*)
        . = ALIGN(4); /* This alignment is needed to make the section size 4 bytes aligned */
    } > CODE_RAM AT > FLASH
    Image$$ER_CODE_SRAM$$RO$$Base = ADDR(.ER_CODE_SRAM);
    Image$$ER_CODE_SRAM$$RO$$Limit = ADDR(.ER_CODE_SRAM) + SIZEOF(.ER_CODE_SRAM);
    Image$$ER_CODE_SRAM$$Base = ADDR(.ER_CODE_SRAM);
    Image$$ER_CODE_SRAM$$Limit = ADDR(.ER_CODE_SRAM) + SIZEOF(.ER_CODE_SRAM);
#endif

#if TFM_LVL != 1
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

#endif /* TFM_LVL != 1 */

    .ER_TFM_CODE : ALIGN(4)
    {
        *startup*(.text*)
        *libplatform_s*:*(.text*)
        *libtfm_spm*:*(.text*)

        *libplatform_s*:*(.rodata*)
        *libtfm_spm*:*(.rodata*)
    } > FLASH

    /**** Base address of secure data area */
    .tfm_secure_data_start :
    {
        . = ABSOLUTE(S_DATA_START) ;
    } > RAM

    /*
     * MPU on Armv6-M/v7-M core in multi-core topology may require more strict
     * alignment that MPU region base address must align with the MPU region
     * size.
     * As a result, on Armv6-M/v7-M cores, to save memory resource and MPU
     * regions, unprivileged data sections and privileged data sections are
     * separated and gathered in unprivileged/privileged data area respectively.
     * Keep BL2 shared data and MSP stack at the beginning of the secure data
     * area on Armv8-M cores, while move the two areas to the beginning of
     * privileged data region on Armv6-M/v7-M cores.
     */
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) || \
    defined(__ARM_ARCH_8_1M_MAIN__)
#ifdef CODE_SHARING
    /* The code sharing between bootloader and runtime requires to share the
     * global variables.
     */
    .TFM_SHARED_SYMBOLS : ALIGN(TFM_LINKER_SHARED_SYMBOLS_ALIGNMENT)
    {
        . += SHARED_SYMBOL_AREA_SIZE;
    } > RAM
#endif

    /* shared_data and msp_stack are overlapping on purpose when
     * msp_stack is extended until the beginning of RAM, when shared_date
     * was read out by partitions
     */
    .tfm_bl2_shared_data : ALIGN(TFM_LINKER_BL2_SHARED_DATA_ALIGNMENT)
    {
        . += BOOT_TFM_SHARED_DATA_SIZE;
    } > RAM

    .msp_stack : ALIGN(TFM_LINKER_MSP_STACK_ALIGNMENT)
    {
        . += __msp_stack_size__ - 0x8;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.msp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);

    .msp_stack_seal_res :
    {
        . += 0x8;
    } > RAM
    __StackSeal = ADDR(.msp_stack_seal_res);

#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) || \
        * defined(__ARM_ARCH_8_1M_MAIN__) */

#if defined(ENABLE_HEAP)
    __heap_size__ = S_HEAP_SIZE;
    .heap : ALIGN(8)
    {
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > RAM
#endif

#if defined(CONFIG_TFM_PARTITION_META)
    .TFM_SP_META_PTR : ALIGN(TFM_LINKER_SP_META_PTR_ALIGNMENT)
    {
        *(.bss.SP_META_PTR_SPRTL_INST)
        . = ALIGN(TFM_LINKER_SP_META_PTR_ALIGNMENT);
    } > RAM
    Image$$TFM_SP_META_PTR$$ZI$$Base = ADDR(.TFM_SP_META_PTR);
    Image$$TFM_SP_META_PTR$$ZI$$Limit = ADDR(.TFM_SP_META_PTR) + SIZEOF(.TFM_SP_META_PTR);
#endif

    /**** APPLICATION RoT DATA start here */
    . = ALIGN(TFM_LINKER_APP_ROT_LINKER_DATA_ALIGNMENT);
    Image$$TFM_APP_RW_STACK_START$$Base = .;

    .TFM_APP_ROT_LINKER_DATA : ALIGN(TFM_LINKER_APP_ROT_LINKER_DATA_ALIGNMENT)
    {
        *tfm_app_rot_partition*:*(.data*)
        *(TFM_*_APP-ROT_ATTR_RW)
        . = ALIGN(4);
    } > RAM AT> FLASH
    Image$$TFM_APP_ROT_LINKER_DATA$$RW$$Base = ADDR(.TFM_APP_ROT_LINKER_DATA);
    Image$$TFM_APP_ROT_LINKER_DATA$$RW$$Limit = ADDR(.TFM_APP_ROT_LINKER_DATA) + SIZEOF(.TFM_APP_ROT_LINKER_DATA);

    .TFM_APP_ROT_LINKER_BSS : ALIGN(4)
    {
        start_of_TFM_APP_ROT_LINKER = .;
        *tfm_app_rot_partition*:*(.bss*)
        *tfm_app_rot_partition*:*(COMMON)
        *(TFM_*_APP-ROT_ATTR_ZI)
        . += (. - start_of_TFM_APP_ROT_LINKER) ? 0 : 4;
        . = ALIGN(TFM_LINKER_APP_ROT_LINKER_DATA_ALIGNMENT);
    } > RAM AT> RAM
    Image$$TFM_APP_ROT_LINKER_DATA$$ZI$$Base = ADDR(.TFM_APP_ROT_LINKER_BSS);
    Image$$TFM_APP_ROT_LINKER_DATA$$ZI$$Limit = ADDR(.TFM_APP_ROT_LINKER_BSS) + SIZEOF(.TFM_APP_ROT_LINKER_BSS);

    /**** APPLICATION RoT DATA end here */
    Image$$TFM_APP_RW_STACK_END$$Base = .;

#if defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || \
    defined(__ARM_ARCH_7EM__)
#ifdef S_DATA_PRIV_START
    /**** Privileged data area base address specified by Armv6-M/v7-M platform */
    .tfm_secure_priv_data_boundary :
    {
        . = ABSOLUTE(S_DATA_PRIV_START) ;
    } > RAM
#endif

    /*
     * Move BL2 shared area and MSP stack to the beginning of privileged data
     * area on Armv6-M/v7-M platforms.
     */

    /* shared_data and msp_stack are overlapping on purpose when
     * msp_stack is extended until the beginning of RAM, when shared_date
     * was read out by partitions
     */
    .tfm_bl2_shared_data : ALIGN(TFM_LINKER_BL2_SHARED_DATA_ALIGNMENT)
    {
        . += BOOT_TFM_SHARED_DATA_SIZE;
    } > RAM AT> RAM

    .msp_stack : ALIGN(TFM_LINKER_MSP_STACK_ALIGNMENT)
    {
        . += __msp_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.msp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);
#endif /* defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || \
        * defined(__ARM_ARCH_7EM__) */

    /**** PSA RoT DATA start here */

    Image$$TFM_PSA_RW_STACK_START$$Base = .;

    .TFM_PSA_ROT_LINKER_DATA : ALIGN(TFM_LINKER_PSA_ROT_LINKER_DATA_ALIGNMENT)
    {
        *tfm_psa_rot_partition*:*(.data*)
        *(TFM_*_PSA-ROT_ATTR_RW)
        . = ALIGN(4);
    } > RAM AT> FLASH
    Image$$TFM_PSA_ROT_LINKER_DATA$$RW$$Base = ADDR(.TFM_PSA_ROT_LINKER_DATA);
    Image$$TFM_PSA_ROT_LINKER_DATA$$RW$$Limit = ADDR(.TFM_PSA_ROT_LINKER_DATA) + SIZEOF(.TFM_PSA_ROT_LINKER_DATA);

    .TFM_PSA_ROT_LINKER_BSS : ALIGN(4)
    {
        start_of_TFM_PSA_ROT_LINKER = .;
        *tfm_psa_rot_partition*:*(.bss*)
        *tfm_psa_rot_partition*:*(COMMON)
        *(TFM_*_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_PSA_ROT_LINKER) ? 0 : 4;
        . = ALIGN(TFM_LINKER_PSA_ROT_LINKER_DATA_ALIGNMENT);
    } > RAM AT> RAM
    Image$$TFM_PSA_ROT_LINKER_DATA$$ZI$$Base = ADDR(.TFM_PSA_ROT_LINKER_BSS);
    Image$$TFM_PSA_ROT_LINKER_DATA$$ZI$$Limit = ADDR(.TFM_PSA_ROT_LINKER_BSS) + SIZEOF(.TFM_PSA_ROT_LINKER_BSS);

    /**** PSA RoT DATA end here */
    Image$$TFM_PSA_RW_STACK_END$$Base = .;

    .TFM_UNPRIV_CODE : ALIGN(TFM_LINKER_UNPRIV_CODE_ALIGNMENT)
    {
        *(SFN)
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
        . = ALIGN(TFM_LINKER_UNPRIV_CODE_ALIGNMENT);
    } > FLASH
    Image$$TFM_UNPRIV_CODE$$RO$$Base = ADDR(.TFM_UNPRIV_CODE);
    Image$$TFM_UNPRIV_CODE$$RO$$Limit = ADDR(.TFM_UNPRIV_CODE) + SIZEOF(.TFM_UNPRIV_CODE);

#ifdef RAM_VECTORS_SUPPORT
    .ramVectors (NOLOAD) : ALIGN(TFM_LINKER_RAM_VECTORS_ALIGNMENT)
    {
        __ram_vectors_start__ = .;
        KEEP(*(.ram_vectors))
        __ram_vectors_end__   = .;
    } > RAM
    .TFM_DATA __ram_vectors_end__ : ALIGN(4)
#else

    .TFM_DATA : ALIGN(4)
#endif
    {
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);

    } > RAM AT> FLASH
    Image$$ER_TFM_DATA$$RW$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$RW$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA);

    .TFM_BSS : ALIGN(4)
    {
        __bss_start__ = .;

        /* The runtime partition placed order is same as load partition */
        __partition_runtime_start__ = .;
        KEEP(*(.bss.part_runtime_priority_lowest))
        KEEP(*(.bss.part_runtime_priority_low))
        KEEP(*(.bss.part_runtime_priority_normal))
        KEEP(*(.bss.part_runtime_priority_high))
        __partition_runtime_end__ = .;
        . = ALIGN(4);

        /* The runtime service placed order is same as load partition */
        __service_runtime_start__ = .;
        KEEP(*(.bss.serv_runtime_priority_lowest))
        KEEP(*(.bss.serv_runtime_priority_low))
        KEEP(*(.bss.serv_runtime_priority_normal))
        KEEP(*(.bss.serv_runtime_priority_high))
        __service_runtime_end__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM AT> RAM
    Image$$ER_TFM_DATA$$ZI$$Base = ADDR(.TFM_BSS);
    Image$$ER_TFM_DATA$$ZI$$Limit = ADDR(.TFM_BSS) + SIZEOF(.TFM_BSS);
    Image$$ER_PART_RT_POOL$$ZI$$Base = __partition_runtime_start__;
    Image$$ER_PART_RT_POOL$$ZI$$Limit = __partition_runtime_end__;
    Image$$ER_SERV_RT_POOL$$ZI$$Base = __service_runtime_start__;
    Image$$ER_SERV_RT_POOL$$ZI$$Limit = __service_runtime_end__;

    Image$$ER_TFM_DATA$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA) + SIZEOF(.TFM_BSS);

#if defined (S_RAM_CODE_START)
    /* Code executed from RAM */
    .TFM_RAM_CODE S_RAM_CODE_START :
    {
        KEEP(*(.ramfunc))
        . = ALIGN(4);
    } > RAM AT> FLASH

    ASSERT(S_RAM_CODE_START % 4 == 0, "S_RAM_CODE_START must be divisible by 4")
#endif

    Load$$LR$$LR_NS_PARTITION$$Base = NS_PARTITION_START;

#ifdef BL2
    Load$$LR$$LR_SECONDARY_PARTITION$$Base = SECONDARY_PARTITION_START;
#endif /* BL2 */

    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
}
