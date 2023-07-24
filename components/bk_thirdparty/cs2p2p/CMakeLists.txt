set(incs)
set(srcs cs2_osal.c)

armino_build_get_property(armino_soc ARMINO_SOC)

if (CONFIG_CS2_P2P_SERVER OR CONFIG_CS2_P2P_CLIENT)
    list(APPEND incs
                include
    )
endif()

armino_component_register(INCLUDE_DIRS SRCS "${srcs}" INCLUDE_DIRS "${incs}")

if (CONFIG_CS2_P2P_SERVER)
	get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/libcs2p2p_server.a NAME)
	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/libPPCS_API_4.5.3_BK7256-armino-v1.3.13-LNS-IoTWIFI-20230526.a")
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endif()



if (CONFIG_CS2_P2P_CLIENT)
	get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/libcs2p2p_client.a NAME)
	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/libcs2p2p_client.a")
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endif()


