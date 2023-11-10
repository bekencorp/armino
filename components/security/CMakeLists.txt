set(incs)

#if (CONFIG_SECURITY)
	set(srcs security_func.c)
#endif()

if(CONFIG_VAULT_SUPPORT)
if(CONFIG_SECURITYIP)
list(APPEND incs
	securityip
)
list(APPEND srcs
	securityip/bk_securityip_adapter.c
)
endif()
endif()
armino_component_register(SRCS "${srcs}" PRIV_REQUIRES bk_common driver
	INCLUDE_DIRS "${incs}")