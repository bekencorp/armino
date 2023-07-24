set(incs .)
set(srcs)

if (CONFIG_BUTTON)
	list(APPEND incs .
	)

	list(APPEND srcs
		key_main.c
		multi_button.c
	)
endif() # CONFIG_BUTTON
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
