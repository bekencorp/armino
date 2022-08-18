set(incs)

if (CONFIG_BT)
        list(APPEND incs
                ./legacy_include
)
endif()
armino_component_register(INCLUDE_DIRS "${incs}")