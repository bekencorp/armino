armino_build_get_property(target ARMINO_SOC)

if(NOT "${target}" STREQUAL "bk7236")
    return()
endif()

armino_component_register(REQUIRES partitions)
