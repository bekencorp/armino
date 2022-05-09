/*
 * Script for GNU linker.
 * Describes layout of sections, location of stack.
 *
 * In this case vectors are at location 0 (reset @ 0x08)
 *
 * +------------+  0x00400020
 * data         |
 * end
 * |(heap)      |
 * .            .
 * .            .
 * |(heap limit)|
 *
 * |- - - - - - |
 * stack bottom    256k
 * +------------+
 *
 * +------------+  0x0000000
 * |vectors     |
 * |            |
 * |------------+
 * |text        |
 * |data        |
 * |            |  1024k
 * +------------+
 */


/* Split memory into area for vectors and ram */
MEMORY
{
        flash  (rx) : ORIGIN = 0x00000000, LENGTH = 2M
        itcm   (rwx): ORIGIN = 0x003F0000, LENGTH = 30k
        tcm   (rw!x): ORIGIN = 0x003F7800, LENGTH = 34k
        ram   (rw!x): ORIGIN = 0x00400100, LENGTH = 192k - 0x100
}

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_vector_start);
_vector_start = ORIGIN(flash);

SECTIONS
{
    /* vectors go to vectors region */
    . = ORIGIN(flash);
    .vectors :
    {
        KEEP(*(*.vectors))
        KEEP( *(*.boot))
    } > flash

    /* instructions go to the text region*/

    . = ORIGIN(itcm);
    .itcm.code ALIGN(8) :
    {
        /* itcm 8KB code */
        *idle.o(.text .text.*)
        *context_gcc.o(.text .text.*)
        *co_list.o(.text .text.*)
        *rxl_cntrl.o(.text .text.*)
        *hal_machw.o(.text .text.*)
        *(*.itcm)
        *(.text.intc_hdl_entry)
        *(.text.intc_irq)
        *(.text.intc_fiq)
        *(.text.irda_isr)
        *(.text.uart1_isr)
        *(.text.uart2_isr)
        *(.text.bk_timer_isr)
        *(.text.fft_isr)
        *(.text.gdma_isr)
        *(.text.gdma_get_transfer_length)
        *(.text.gdma_get_remain_len)
        /*
        *(.text.gpio_isr)
        *(.text.i2c1_isr)
        *(.text.i2c2_isr)
        */
        *(.text.phy_mdm_isr)
        *(.text.phy_unsupported_modulation_check)
        *(.text.pwm_isr)
        *(.text.saradc_isr)
        *(.text.saradc_pause)
        *(.text.saradc_close)
        *(.text.saradc_int_clr)
        /*
        *(.text.spi_isr)
        *(.text.flash_write)
        *(.text.spi_rxfifo_clr)
        *(.text.sctrl_dpll_isr)
        *(.text.power_save_wakeup_isr)
        *(.text.sctrl_cali_dpll)
        */
        *(.text.bmsg_rx_sender)
        *(.text.bmsg_null_sender)
        *(.text.bk_get_tick)
        /*
        *(.text.flash_read_sr)
        *(.text.flash_write_sr)
        *(.text.flash_clr_qwfr)
        *(.text.set_flash_protect)
        *(.text.flash_read)
        *(.text.flash_read_data)
        *(.text.flash_set_qe)
        *(.text.flash_set_qwfr)
        *(.text.flash_set_line_mode*)
        *(.text.flash_get_line_mode)
        *(.text.flash_write)
        */
        *(.text.flash_ctrl)
        *queue.o(.text .text.*)
        /*
        *power_save.o(.text .text.*)
        *mcu_ps.o(.text .text.*)
        *rwip_driver.o(.text .text.*)
        */

        *(.text.vTaskSuspendAll)
        *(.text.xTaskGetTickCount)
        *(.text.xTaskGetTickCountFromISR)
        *(.text.vTaskStepTick)
        *(.text.xTaskIncrementTick)
        *(.text.xTaskResumeAll)
        *(.text.vTaskSwitchContext)
        *(.text.vApplicationIdleHook)
        *(.text.platform_is_in_irq_context)
        *(.text.platform_is_in_fiq_context)
        *(.text.platform_is_in_interrupt_context)
        *(.text.portENABLE_IRQ)
        *(.text.portENABLE_FIQ)
        *(.text.portDISABLE_FIQ)
        *(.text.portDISABLE_IRQ)
        *(.text.vPortEnterCritical)
        *(.text.vPortExitCritical)
        *(.text.sctrl_fix_dpll_div)

        /*
         * Trigger based PPDU are time critical, put he releated code in itcm
         * Some const variables are in .rodata section.
         */
        *txl_he.o(.text.* *.rodata *.rodata.*)
        *txl_agg.o(.text.* *.rodata *.rodata.*)
        *txl_cntrl.o(.text.* *.rodata *.rodata.*)
        *rxl_hwdesc.o(.text.* *.rodata *.rodata.*)
    } > itcm AT>flash
    _itcmcode_flash_begin = LOADADDR(.itcm.code);
    _itcmcode_ram_begin = ADDR(.itcm.code);
    _itcmcode_ram_end = _itcmcode_ram_begin + SIZEOF(.itcm.code);

    . = ALIGN(0x8);
    /* code, instructions.for example: i=i+1; */
    .text :
    {
        *(.text)
        *(.text.*)
        *(.stub)
        /* .gnu.warning sections are handled specially by elf32.em.  */
        *(.gnu.warning)
        *(.gnu.linkonce.t*)
        *(.glue_7t) *(.glue_7)
        __flash_txt_end = .;
    } > flash

    /* read only data.for example: const int rom_data[3]={1,2,3}; */
    .rodata ALIGN(8) :
    {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r*)
    } > flash

    .ARM.exidx :
    {
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
    } > flash

    . = ORIGIN(tcm);
    .tcm ALIGN(8) :
    {
	/*
        ble_rf_port.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rwip.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gapc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gapc_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gapm.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gapm_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gattc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gattc_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gattm.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gattm_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *l2cc_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *l2cm.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *llc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *lld.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *llm.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *hci.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sch_slice.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ble_aes.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ecc_p256.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *kernel.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rwip_driver.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *l2cc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ble_util_buf.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *llc_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *lld_adv.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *lld_con.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *lld_per_adv.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *lld_test.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *llm_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *hci_fc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sch_alarm.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sch_arb.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sch_plan.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sch_prog.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *kernel_event.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *kernel_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *app_ble.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ble_ui.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
	*/

        /*
        *bk7011_cal*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rwnx_intf*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *power_save*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *wlan_ui*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ate_app*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *arch_main*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *irda*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *spi*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *wlan_cli*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rtos_pub*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *pwm_new*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *phy_trident*.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        */

        /* driver */
        /*
        *drv_model.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *intc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *role_launch.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *app_bk.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *manual_cal_bk7231U.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *main_none.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *eloop.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *main_supplicant.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *wpa_supplicant.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rw_ieee80211.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rw_msg_rx.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *temp_detect.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *param_config.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *flash.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *gpio.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *mac_phy_bypass.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *bk_timer.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *saradc.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sys_ctrl.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *uart_bk.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
	*/

	/*
        wdt.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sdp_service.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *app_task.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *app_sdp.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *uart.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *bk7231N_cal.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *fake_clock.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *tx_evm.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        */

        /* beken_wifi.a */
        *apm.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *apm_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *bam_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *bam.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *chan.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *hal_dma.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *hal_desc.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *hal_machw.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ke_env.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *me_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *ps.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rc.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rwnx.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rxl_hwdesc.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rxu_cntrl.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rxl_cntrl.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *rxl_hwdesc.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sta_mgmt.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *scan_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *scan.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *scan_shared.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *scanu.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *scanu_shared.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *scanu_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sm.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *sm_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *me.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *mm.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *mm_bcn.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *mm_task.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *mm_timer.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *td.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *tx_swdesc.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *txl_buffer.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *txl_cfm.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *txl_cntrl.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *txl_buffer_shared.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *txl_frame_shared.c.obj(.bss .bss.* .scommon .sbss .dynbss COMMON)
        /*
        *txl_agg.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *txl_he.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *vif_mgmt.o(.bss .bss.* .scommon .sbss .dynbss COMMON)
        */
    } >tcm AT>flash
    _tcmbss_start = ADDR(.tcm);
    _tcmbss_end = _tcmbss_start + SIZEOF(.tcm);

    . = ORIGIN(ram);
    /* globals.for example: int ram_data[3]={4,5,6}; */     /* VMA in RAM, but keep LMA in flash */
    _begin_data = .;
    .data :
    {
        *(.data .data.*)
        *(.sdata)
        *(.gnu.linkonce.d*)
        SORT(CONSTRUCTORS)
        } >ram AT>flash
        _end_data = .;

    /* Loader will copy data from _flash_begin to _ram_begin..ram_end */
    _data_flash_begin = LOADADDR(.data);
    _data_ram_begin = ADDR(.data);
    _data_ram_end = .;

    /* uninitialized data section - global   int i; */
    .bss ALIGN(8):
    {
        _bss_start = .;
        *boot_handlers.O(.bss .bss.* .scommon .sbss .dynbss COMMON)
        *(.bss .bss.*)
        *(.scommon)
        *(.sbss)
        *(.dynbss)
        *(COMMON)
        /* Align here to ensure that the .bss section occupies space up to
        _end.  Align after .bss to ensure correct alignment even if the
        .bss section disappears because there are no input sections.  */
        . = ALIGN(32 / 8);
        _bss_end = .;
    } > ram                     /* in RAM */

    . = ALIGN (8);
    _empty_ram = .;

    /* This symbol defines end of code/data sections. Heap starts here. */
    PROVIDE(end       = .);
}
 GROUP(
   libgcc.a
   libg.a
   libc.a
   libm.a
   libnosys.a
 )
