armino_component_register(INCLUDE_DIRS ./)

if (CMAKE_BUILD_EARLY_EXPANSION)
return()
endif()

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
armino_build_set_property(PARTITIONS_HDR_DIR ${GEN_DIR})
set(GEN_FILE ${GEN_DIR}/partitions_gen.h)
set(CONFIG_FILE_LISTS
	partitions
	bl1_control.json
	)

armino_build_get_property(python PYTHON)
set(TOOLS_PATH ${ARMINO_PATH}/tools/env_tools)
set(BASE_CFG_DIR $ENV{ARMINO_PATH}/middleware/boards/${ARMINO_SOC})
set(PREFERED_CFG_DIR ${PROJECT_DIR}/config)
set(BIN_DIR ${CMAKE_BINARY_DIR}/install/outputs)
set(GEN_TOOL $ENV{ARMINO_PATH}/middleware/boards/${ARMINO_SOC}/partitions/gen.py)

message(STATUS "TOOL_PATH=${TOOLS_PATH}")
message(STATUS "CFG_DIR=${BASE_CFG_DIR}")
message(STATUS "prefer=${PREFERED_CFG_DIR}")
message(STATUS "bin=${BIN_DIR}")

add_custom_command(OUTPUT ${GEN_FILE}
	COMMAND ${python} ${GEN_TOOL} pack -t "${TOOLS_PATH}" -c "${BASE_CFG_DIR}" -p "${PREFERED_CFG_DIR}" -s "${ARMINO_SOC}" -g ${CMAKE_CURRENT_BINARY_DIR}/generated
	DEPENDS ${CONFIG_FILE_LISTS} ${GEN_TOOL}
)

add_custom_target(bk7258_generated_files SOURCES ${GEN_FILE})
add_dependencies(${COMPONENT_LIB} bk7258_generated_files)
target_include_directories(${COMPONENT_LIB} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/generated)

