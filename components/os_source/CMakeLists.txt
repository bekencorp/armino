if(CONFIG_FREERTOS)
	if(CONFIG_FREERTOS_V9)

		set(incs
			freertos_v9/portable/Keil/ARM968es
			freertos_v9/include
		)

		set(srcs
			freertos_v9/croutine.c
			freertos_v9/event_groups.c
			freertos_v9/list.c
			freertos_v9/portable/Keil/ARM968es/port.c
			freertos_v9/queue.c
			freertos_v9/tasks.c
			freertos_v9/timers.c
			freertos_v9/portable/MemMang/heap_4.c
			)

		if(CONFIG_FULLY_HOSTED)
			list(REMOVE_ITEM  srcs freertos_v9/portable/MemMang/heap_4.c)
		endif()
		set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/freertos_v9/portable/Keil/ARM968es/port.c" PROPERTIES COMPILE_FLAGS "-marm")
	elseif(CONFIG_FREERTOS_V10)
		set(incs freertos_v10/include)
		set(srcs
			freertos_v10/croutine.c
			freertos_v10/event_groups.c
			freertos_v10/list.c
			freertos_v10/queue.c
			freertos_v10/tasks.c
			freertos_v10/stream_buffer.c
			freertos_v10/timers.c
			freertos_v10/portable/MemMang/heap_4.c
			)
		
		if(CONFIG_ARCH_RISCV)
			list(APPEND incs
				freertos_v10/portable/GCC/RISC-V
				freertos_v10/portable/GCC/RISC-V/chip_specific_extensions/Andes_V5_RV32RV64
				)
		
			list(APPEND srcs
				freertos_v10/portable/GCC/RISC-V/port.c
				freertos_v10/portable/GCC/RISC-V/portASM.S
				)
			
		elseif(CONFIG_ARCH_CM33)
			list(APPEND incs
				freertos_v10/portable/GCC/ARM_CM33_NTZ/non_secure
				)
			
			list(APPEND srcs
				freertos_v10/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
				freertos_v10/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
				freertos_v10/portable/Common/mpu_wrappers.c
				)
		else()
			list(APPEND incs
				freertos_v10/portable/Keil/ARM968es
				)

			list(APPEND srcs
				freertos_v10/portable/Keil/ARM968es/port.c
				)
			set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/freertos_v10/portable/Keil/ARM968es/port.c" PROPERTIES COMPILE_FLAGS "-marm")
		endif()
		
		if(CONFIG_FULLY_HOSTED)
			list(REMOVE_ITEM srcs freertos_v10/portable/MemMang/heap_4.c)
		endif()
	endif()
elseif(CONFIG_LITEOS_M)
	set(incs 
		liteos_m/config/
		liteos_m/components/exchook
		liteos_m/components/cpup
		liteos_m/components/power
		# liteos_m/components/port
		liteos_m/components/bounds_checking_function/include/
		liteos_m/kernel/include/
		liteos_m/kernel/arch/include/
		liteos_m/targets/bk72xx/
		# liteos_m/port/
		liteos_m/utils/
		liteos_m/kernel/arch/arm/arm9/gcc/
	)
	set(srcs 
		liteos_m/kernel/src/mm/los_membox.c
		liteos_m/kernel/src/mm/los_memory.c
		liteos_m/kernel/src/los_event.c
		liteos_m/kernel/src/los_init.c
		liteos_m/kernel/src/los_mux.c
		liteos_m/kernel/src/los_queue.c
		liteos_m/kernel/src/los_sem.c
		liteos_m/kernel/src/los_swtmr.c
		liteos_m/kernel/src/los_task.c
		liteos_m/kernel/src/los_tick.c
		liteos_m/kernel/src/los_sched.c
		liteos_m/kernel/src/los_sortlink.c
		liteos_m/components/bounds_checking_function/src/fscanf_s.c
		liteos_m/components/bounds_checking_function/src/fwscanf_s.c
		liteos_m/components/bounds_checking_function/src/gets_s.c
		liteos_m/components/bounds_checking_function/src/memcpy_s.c
		liteos_m/components/bounds_checking_function/src/memmove_s.c
		liteos_m/components/bounds_checking_function/src/memset_s.c
		liteos_m/components/bounds_checking_function/src/scanf_s.c
		liteos_m/components/bounds_checking_function/src/securecutil.c
		liteos_m/components/bounds_checking_function/src/secureinput_a.c
		liteos_m/components/bounds_checking_function/src/secureinput_w.c
		liteos_m/components/bounds_checking_function/src/secureprintoutput_a.c
		liteos_m/components/bounds_checking_function/src/secureprintoutput_w.c
		liteos_m/components/bounds_checking_function/src/snprintf_s.c
		liteos_m/components/bounds_checking_function/src/sprintf_s.c
		liteos_m/components/bounds_checking_function/src/sscanf_s.c
		liteos_m/components/bounds_checking_function/src/strcat_s.c
		liteos_m/components/bounds_checking_function/src/strcpy_s.c
		liteos_m/components/bounds_checking_function/src/strncat_s.c
		liteos_m/components/bounds_checking_function/src/strncpy_s.c
		liteos_m/components/bounds_checking_function/src/strtok_s.c
		liteos_m/components/bounds_checking_function/src/swprintf_s.c
		liteos_m/components/bounds_checking_function/src/swscanf_s.c
		liteos_m/components/bounds_checking_function/src/vfscanf_s.c
		liteos_m/components/bounds_checking_function/src/vfwscanf_s.c
		liteos_m/components/bounds_checking_function/src/vscanf_s.c
		liteos_m/components/bounds_checking_function/src/vsnprintf_s.c
		liteos_m/components/bounds_checking_function/src/vsprintf_s.c
		liteos_m/components/bounds_checking_function/src/vsscanf_s.c
		liteos_m/components/bounds_checking_function/src/vswprintf_s.c
		liteos_m/components/bounds_checking_function/src/vswscanf_s.c
		liteos_m/components/bounds_checking_function/src/vwscanf_s.c
		liteos_m/components/bounds_checking_function/src/wcscat_s.c
		liteos_m/components/bounds_checking_function/src/wcscpy_s.c
		liteos_m/components/bounds_checking_function/src/wcsncat_s.c
		liteos_m/components/bounds_checking_function/src/wcsncpy_s.c
		liteos_m/components/bounds_checking_function/src/wcstok_s.c
		liteos_m/components/bounds_checking_function/src/wmemcpy_s.c
		liteos_m/components/bounds_checking_function/src/wmemmove_s.c
		liteos_m/components/bounds_checking_function/src/wscanf_s.c
		liteos_m/components/power/los_pm.c
		liteos_m/components/exchook/los_exc_info.c
		liteos_m/utils/los_error.c
		liteos_m/utils/los_debug.c
		liteos_m/kernel/arch/arm/arm9/gcc/los_context.c
		liteos_m/kernel/arch/arm/arm9/gcc/los_interrupt.c
		liteos_m/kernel/arch/arm/arm9/gcc/los_timer.c
		liteos_m/targets/bk72xx/bk_los_timer.c
		liteos_m/kernel/arch/arm/arm9/gcc/los_dispatch.S
		liteos_m/kernel/arch/arm/arm9/gcc/los_exc.S
	)
	set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/liteos_m/kernel/arch/arm/arm9/gcc/los_context.c" PROPERTIES COMPILE_FLAGS "-marm")
	set_source_files_properties("${CMAKE_CURRENT_LIST_DIR}/liteos_m/kernel/arch/arm/arm9/gcc/los_interrupt.c" PROPERTIES COMPILE_FLAGS "-marm")
elseif(CONFIG_LITEOS_M_V3)
	if(CONFIG_ARCH_RISCV)
		set(incs
			liteos_m_v3/config/
			liteos_m_v3/components/exchook
			liteos_m_v3/components/cpup
			liteos_m_v3/components/power
			liteos_m_v3/components/bounds_checking_function/include/
			liteos_m_v3/components/backtrace/
			liteos_m_v3/kernel/include/
			liteos_m_v3/kernel/arch/include/
			liteos_m_v3/targets/bk72xx/
			liteos_m_v3/utils/
			liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/
			liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/asm
		)
		set(srcs
			liteos_m_v3/kernel/src/mm/los_membox.c
			liteos_m_v3/kernel/src/mm/los_memory.c
			liteos_m_v3/kernel/src/los_event.c
			liteos_m_v3/kernel/src/los_init.c
			liteos_m_v3/kernel/src/los_mux.c
			liteos_m_v3/kernel/src/los_queue.c
			liteos_m_v3/kernel/src/los_sem.c
			liteos_m_v3/kernel/src/los_swtmr.c
			liteos_m_v3/kernel/src/los_task.c
			liteos_m_v3/kernel/src/los_tick.c
			liteos_m_v3/kernel/src/los_sched.c
			liteos_m_v3/kernel/src/los_sortlink.c
			liteos_m_v3/components/bounds_checking_function/src/fscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/fwscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/gets_s.c
			liteos_m_v3/components/bounds_checking_function/src/memcpy_s.c
			liteos_m_v3/components/bounds_checking_function/src/memmove_s.c
			liteos_m_v3/components/bounds_checking_function/src/memset_s.c
			liteos_m_v3/components/bounds_checking_function/src/scanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/securecutil.c
			liteos_m_v3/components/bounds_checking_function/src/secureinput_a.c
			liteos_m_v3/components/bounds_checking_function/src/secureinput_w.c
			liteos_m_v3/components/bounds_checking_function/src/secureprintoutput_a.c
			liteos_m_v3/components/bounds_checking_function/src/secureprintoutput_w.c
			liteos_m_v3/components/bounds_checking_function/src/snprintf_s.c
			liteos_m_v3/components/bounds_checking_function/src/sprintf_s.c
			liteos_m_v3/components/bounds_checking_function/src/sscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/strcat_s.c
			liteos_m_v3/components/bounds_checking_function/src/strcpy_s.c
			liteos_m_v3/components/bounds_checking_function/src/strncat_s.c
			liteos_m_v3/components/bounds_checking_function/src/strncpy_s.c
			liteos_m_v3/components/bounds_checking_function/src/strtok_s.c
			liteos_m_v3/components/bounds_checking_function/src/swprintf_s.c
			liteos_m_v3/components/bounds_checking_function/src/swscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vfscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vfwscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vsnprintf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vsprintf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vsscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vswprintf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vswscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/vwscanf_s.c
			liteos_m_v3/components/bounds_checking_function/src/wcscat_s.c
			liteos_m_v3/components/bounds_checking_function/src/wcscpy_s.c
			liteos_m_v3/components/bounds_checking_function/src/wcsncat_s.c
			liteos_m_v3/components/bounds_checking_function/src/wcsncpy_s.c
			liteos_m_v3/components/bounds_checking_function/src/wcstok_s.c
			liteos_m_v3/components/bounds_checking_function/src/wmemcpy_s.c
			liteos_m_v3/components/bounds_checking_function/src/wmemmove_s.c
			liteos_m_v3/components/bounds_checking_function/src/wscanf_s.c
			liteos_m_v3/components/power/los_pm.c
			liteos_m_v3/components/exchook/los_exc_info.c
			liteos_m_v3/utils/los_error.c
			liteos_m_v3/utils/los_debug.c
			liteos_m_v3/components/backtrace/los_backtrace.c
			liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/los_context.c
			liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/los_interrupt.c
			# liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/los_timer.c
			liteos_m_v3/targets/bk72xx/bk_los_timer.c
			liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/los_dispatch.S
			liteos_m_v3/kernel/arch/risc-v/riscv32/gcc/los_exc.S
		)
	endif()

endif()


armino_component_register(SRCS "${srcs}"
		INCLUDE_DIRS "${incs}"
		PRIV_REQUIRES bk_common bk_rtos)

