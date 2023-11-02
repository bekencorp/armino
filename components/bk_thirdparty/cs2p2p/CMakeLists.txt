set(incs)
set(srcs)

if (CONFIG_CS2_P2P_SERVER OR CONFIG_CS2_P2P_CLIENT)

if ("${ARMINO_SOC}" STREQUAL "bk7256" OR "${ARMINO_SOC}" STREQUAL "bk7235")
    include(bk7256/config.cmake)
	set(LIB_CS2_FILE_PATH bk7256/${LIB_CS2_FILE_NAME})
endif()


endif()

armino_build_get_property(armino_soc ARMINO_SOC)

if (CONFIG_CS2_P2P_SERVER OR CONFIG_CS2_P2P_CLIENT)
    list(APPEND incs
                include
                .
    )

    list(APPEND srcs
                cs2_p2p_main.c
                cli_cs2_p2p.c
                cs2_osal.c
    )
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)

if (CONFIG_CS2_P2P_SERVER)
	get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/libcs2p2p_server.a NAME)
	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/${LIB_CS2_FILE_PATH}")
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endif()



if (CONFIG_CS2_P2P_CLIENT)
	get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/libcs2p2p_client.a NAME)
	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/${LIB_CS2_FILE_PATH}")
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endif()


