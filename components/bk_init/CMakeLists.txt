set(incs include)
set(srcs
	legacy_init.c
	components_init.c
	reset_reason.c
	)

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common bk_netif bk_ble bk_ate driver easy_flash bk_libs riscv)
