armino_build_get_property(target ARMINO_SOC)
set(incs include)
set(srcs
	legacy_init.c
	components_init.c
	reset_reason.c
	)
set(priv_req bk_common bk_netif bk_ble bk_ate driver easy_flash bk_libs riscv)

if (("${target}" STREQUAL "bk7235") OR ("${target}" STREQUAL "bk7237") OR ("${target}" STREQUAL "bk7256") )
list(APPEND priv_req vnd_cal)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES "${priv_req}")
