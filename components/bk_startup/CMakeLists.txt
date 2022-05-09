# set(incs freertos)

set(srcs
	# freertos/rtos_init.c
	system_main.c)

if(CONFIG_FREERTOS)
	set(incs freertos)
	list(APPEND srcs freertos/rtos_init.c)
elseif(CONFIG_LITEOS_M OR CONFIG_LITEOS_M_V3)
	set(incs liteos_m)
	list(APPEND srcs liteos_m/rtos_init.c)
endif()

armino_component_register(SRCS "${srcs}"
			INCLUDE_DIRS "${incs}"
			#REQUIRES easy_flash usb bk_init
			REQUIRES easy_flash bk_init
			PRIV_REQUIRES bk_common riscv)
