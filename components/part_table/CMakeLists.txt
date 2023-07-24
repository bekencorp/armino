set(incs .)
set(srcs)
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES include)

armino_build_get_property(target ARMINO_SOC)
armino_build_get_property(python PYTHON)
armino_build_get_property(armino_path ARMINO_PATH)
armino_build_get_property(project_dir PROJECT_DIR)
set(supported_targets)
list(APPEND supported_targets 
	"bk7235"
	"bk7256"
)

if(NOT target IN_LIST supported_targets)
	return()
endif()

set(supported_projects)
list(APPEND supported_projects
	"app"
)
get_filename_component(project_name ${project_dir} NAME)
if(NOT project_name IN_LIST supported_projects)
	return()
endif()

set(part_table_tool "${armino_path}/tools/build_tools/part_table_tools/gen_bk7256partitions.py")
set(part_table_args "--flash-size=16MB;--smode")
set(configuration_json_inseq "--smode-inseq=1,1,1,0,0,0")
set(source_file_inseq "--smode-inseq=1,3,0,0")
if (CONFIG_FLASH_PARTITION_USER)
	set(configuration_json "${armino_path}/tools/build_tools/part_table_tools/tempFiles/configuration_new.json")
	set(configuration_json_old "${project_dir}/configuration.json")
	set(source_cfile "${armino_path}/tools/build_tools/part_table_tools/tempFiles/vendor_flash_new.c")
	set(source_cfile_old "${project_dir}/main/vendor_flash.c")
	set(source_hfile "${armino_path}/tools/build_tools/part_table_tools/vendor_flash_partition.h")
	set(source_hfile_old "${project_dir}/main/vendor_flash_partition.h")
	set(part_csv_file "${project_dir}/csv/${target}.csv")
else()
	set(configuration_json "${armino_path}/tools/build_tools/part_table_tools/tempFiles/configuration_new.json")
	set(configuration_json_old "${armino_path}/middleware/boards/${target}/configuration.json")
	set(source_cfile "${armino_path}/tools/build_tools/part_table_tools/tempFiles/vnd_flash_new.c")
	set(source_cfile_old "${armino_path}/middleware/boards/${target}/vnd_flash/vnd_flash.c")
	set(source_hfile "${armino_path}/tools/build_tools/part_table_tools/vnd_flash_partition.h")
	set(source_hfile_old "${armino_path}/middleware/boards/${target}/vnd_flash/vnd_flash_partition.h")
	set(part_csv_file "${armino_path}/middleware/boards/${target}/partitions.csv")
endif()

if (EXISTS ${part_csv_file})
	# gen configuration.json file
	add_custom_command(
			OUTPUT "${configuration_json}"
			#COMMAND ${CMAKE_COMMAND} -E touch ${configuration_json}
			COMMAND "${python}" "${part_table_tool}"
			${part_csv_file} --to-json=${configuration_json}
			${part_table_args} ${configuration_json_inseq}
			COMMAND ${CMAKE_COMMAND} -E copy ${configuration_json} ${configuration_json_old}
			MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file}
			COMMENT "armino ====> gen configuration_json..."
			VERBATIM)

	# gen *.c and *.h files
	add_custom_command(
			OUTPUT "${source_cfile}"
			#COMMAND ${CMAKE_COMMAND} -E touch ${source_cfile}
			COMMAND "${python}" "${part_table_tool}"
			${part_csv_file} --to-src=${source_cfile}
			--to-inc=${source_hfile}
			${part_table_args} ${source_file_inseq}
			COMMAND ${CMAKE_COMMAND} -E copy ${source_cfile} ${source_cfile_old}
			COMMAND ${CMAKE_COMMAND} -E copy ${source_hfile} ${source_hfile_old}
			MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file}
			COMMENT "armino ====> gen *c and *h files..."
			VERBATIM)

	add_custom_target(part_table_tool_op DEPENDS ${configuration_json} ${source_cfile})
	add_dependencies(${COMPONENT_LIB} part_table_tool_op)
else()
	message(SEND_ERROR "${part_csv_file} is not exist!!!")
endif()