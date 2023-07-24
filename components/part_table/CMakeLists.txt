set(incs .)
set(srcs)
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}")

armino_build_get_property(target ARMINO_SOC)
set(supported_targets)
list(APPEND supported_targets
	"bk7235"
	"bk7235_cp1"
	"bk7256"
	"bk7256_cp1"
)
set(supported_main_targets ${supported_targets})
list(FILTER supported_main_targets EXCLUDE REGEX "^bk.*_cp[0-9]$")
set(supported_slave_targets ${supported_targets})
list(FILTER supported_slave_targets INCLUDE REGEX "^bk.*_cp[0-9]$")
if(NOT target IN_LIST supported_targets)
	return()
endif()

set(supported_projects)
list(APPEND supported_projects
	"app"
	"bk7256_configa"
	"bk7256_configb"
	"ate_mini_code"
)
get_filename_component(project_name ${project_dir} NAME)
if(NOT project_name IN_LIST supported_projects)
	return()
endif()
#message(SEND_ERROR "supported_main_targets: ${supported_main_targets}, supported_slave_targets: ${supported_slave_targets}")

armino_build_get_property(python PYTHON)
armino_build_get_property(armino_path ARMINO_PATH)
armino_build_get_property(project_dir PROJECT_DIR)
armino_build_get_property(sdkconfig SDKCONFIG)
armino_build_get_property(sdkconfig_defaults SDKCONFIG_DEFAULTS)
armino_build_get_property(sdkconfig_default_soc SDKCONFIG_DEFAULT_SOC)
armino_build_get_property(config_env_path CONFIG_ENV_PATH)
armino_build_get_property(root_kconfig __ROOT_KCONFIG)
armino_build_get_property(build_dir BUILD_DIR)

if(sdkconfig_default_soc)
	list(APPEND defaults_arg --defaults "${sdkconfig_default_soc}")
endif()
if(sdkconfig_defaults)
	foreach(sdkconfig_default ${sdkconfig_defaults})
		if(EXISTS "${sdkconfig_default}.${armino_target}")
			list(APPEND defaults_arg --defaults "${sdkconfig_default}.${armino_target}")
		endif()
	endforeach()
endif()
if(sdkconfig_defaults)
	foreach(sdkconfig_default ${sdkconfig_defaults})
		list(APPEND defaults_arg --defaults "${sdkconfig_default}")
	endforeach()
endif()

set(confgen_basecommand
	${python} ${armino_path}/tools/build_tools/kconfig_new/confgen.py
	--kconfig ${root_kconfig}
	#--config ${sdkconfig}
	${defaults_arg}
	--env-file ${config_env_path})
# Generate the config outputs
set(config_dir ${build_dir}/config)
set(sdkconfig_cmake ${config_dir}/sdkconfig.cmake)
set(sdkconfig_header ${config_dir}/sdkconfig.h)
set(sdkconfig_json ${config_dir}/sdkconfig.json)
set(sdkconfig_json_menus ${config_dir}/kconfig_menus.json)

get_filename_component(special_project ${project_dir} NAME)
set(special_supported_project)
list(APPEND special_supported_project
	"bk7256_configa"
	"bk7256_configb"
)

set(smode_inseqs_tool "${armino_path}/tools/build_tools/part_table_tools/otherScript/get_smode_inseqs.py")
set(depends_check_tool "${armino_path}/tools/build_tools/part_table_tools/otherScript/depends_files_check.py")
set(part_table_tool "${armino_path}/tools/build_tools/part_table_tools/gen_bk7256partitions.py")
set(part_table_args "--flash-size=16MB;--smode")

set(slave_target_num)
if(target IN_LIST supported_slave_targets)
	STRING(LENGTH ${target} slave_target_len)
	math(EXPR slave_target_substart "${slave_target_len}-1" OUTPUT_FORMAT DECIMAL)
	STRING(SUBSTRING ${target} ${slave_target_substart} 1 slave_target_num)
	#message(SEND_ERROR "target = ${target}, slave_target_len = ${slave_target_len}, slave_target_num = ${slave_target_num}")
	execute_process(COMMAND
					"${python}" ${smode_inseqs_tool} application${slave_target_num}.sag
					OUTPUT_VARIABLE output
					RESULT_VARIABLE result)
	if(result STREQUAL 0)
		LOGI("smode_inseqs_tool execute success...")
		STRING(STRIP "${output}" output)
		set(application${slave_target_num}_sag_inseq "--smode-inseq=${output}")
	else()
		LOGE("smode_inseqs_tool execute failed...")
	endif()
	execute_process(COMMAND
					"${python}" ${smode_inseqs_tool} genSourceFiles
					OUTPUT_VARIABLE output
					RESULT_VARIABLE result)
	if(result STREQUAL 0)
		LOGI("smode_inseqs_tool execute success...")
		STRING(STRIP "${output}" output)
		set(source_file_inseq "--smode-inseq=${output}")
	else()
		LOGE("smode_inseqs_tool execute failed...")
	endif()
	STRING(FIND ${target} "_" main_target_len REVERSE)
	STRING(SUBSTRING ${target} 0 ${main_target_len} main_target)
	set(main_target_config "${project_dir}/config/${main_target}.config")
	set(target_config_tool "${armino_path}/tools/build_tools/part_table_tools/otherScript/get_target_config_val.py")
	set(target_config_tool_args "--config=${main_target_config}")
	execute_process(COMMAND
					"${python}" "${target_config_tool}" ${target_config_tool_args}
					OUTPUT_VARIABLE output
					RESULT_VARIABLE result)
	if(result STREQUAL 0)
		STRING(STRIP "${output}" output)
		if(output STREQUAL y)
			set(CONFIG_FLASH_PARTITION_USER "y")
		endif()
	endif()
	#message(SEND_ERROR "target = ${target}, main_target_len = ${main_target_len}, main_target = ${main_target}")
elseif(target IN_LIST supported_main_targets)
	set(main_target "${target}")
	execute_process(COMMAND
					"${python}" ${smode_inseqs_tool} application.sag
					OUTPUT_VARIABLE output
					RESULT_VARIABLE result)
	if(result STREQUAL 0)
		LOGI("smode_inseqs_tool execute success...")
		STRING(STRIP "${output}" output)
		set(application_sag_inseq "--smode-inseq=${output}")
	else()
		LOGE("smode_inseqs_tool execute failed...")
	endif()
	execute_process(COMMAND
					"${python}" ${smode_inseqs_tool} configuration_new.json
					OUTPUT_VARIABLE output
					RESULT_VARIABLE result)
	if(result STREQUAL 0)
		LOGI("smode_inseqs_tool execute success...")
		STRING(STRIP "${output}" output)
		set(configuration_json_inseq "--smode-inseq=${output}")
	else()
		LOGE("smode_inseqs_tool execute failed...")
	endif()
	execute_process(COMMAND
					"${python}" ${smode_inseqs_tool} genSourceFiles
					OUTPUT_VARIABLE output
					RESULT_VARIABLE result)
	if(result STREQUAL 0)
		LOGI("smode_inseqs_tool execute success...")
		STRING(STRIP "${output}" output)
		set(source_file_inseq "--smode-inseq=${output}")
	else()
		LOGE("smode_inseqs_tool execute failed...")
	endif()
else()
	message(STATUS "target: ${target}")
	message(STATUS "supported_targets: ${supported_targets}")
	message(STATUS "supported_main_targets: ${supported_main_targets}")
	message(STATUS "supported_slave_targets: ${supported_slave_targets}")
	message(SEND_ERROR "target is not supported!!!")
endif()

# isolate the partition action between developer and customer 
# by macro argument CONFIG_FLASH_PARTITION_USER
# if CONFIG_FLASH_PARTITION_USER set, assume the partition action come from customer,
# then *.csv file and all other files generated from part_table_tool located in dir: project/...
# if CONFIG_FLASH_PARTITION_USER not set, assume the partition action come from developer,
# then *.csv file and all other files generated from part_table_tool located in dir: middleware/...
if (CONFIG_FLASH_PARTITION_USER)
	if(special_project IN_LIST special_supported_project)
		set(configuration_json "${armino_path}/tools/build_tools/part_table_tools/tempFiles/configuration_new.json")
		set(configuration_json_old "${project_dir}/config/configuration.json")
	else()
		set(configuration_json "${armino_path}/tools/build_tools/part_table_tools/tempFiles/configuration_new.json")
		set(configuration_json_old "${project_dir}/configuration.json")
	endif()
	set(source_cfile "${armino_path}/tools/build_tools/part_table_tools/tempFiles/vendor_flash_new.c")
	set(source_cfile_old "${project_dir}/main/vendor_flash.c")
	set(source_hfile "${armino_path}/tools/build_tools/part_table_tools/tempFiles/vendor_flash_partition.h")
	set(source_hfile_old "${project_dir}/main/vendor_flash_partition.h")
	#set(part_csv_file "${project_dir}/partitions.csv")
	set(part_csv_file "${project_dir}/csv/${main_target}.csv")
	set(config_file "${project_dir}/config/${target}.config")
	set(src_sag_file "${armino_path}/middleware/soc/${target}/${target}_bsp.sag")
	set(refer_sag_file "${armino_path}/tools/build_tools/part_table_tools/tempFiles/application${slave_target_num}.sag")
	set(dest_sag_file "${project_dir}/${target}_bsp.sag")
	set(det_ld_file "${project_dir}/${target}_bsp.ld")
else()
	set(configuration_json "${armino_path}/tools/build_tools/part_table_tools/tempFiles/configuration_new.json")
	set(configuration_json_old "${armino_path}/middleware/boards/${target}/configuration.json")
	set(source_cfile "${armino_path}/tools/build_tools/part_table_tools/tempFiles/vnd_flash_new.c")
	set(source_cfile_old "${armino_path}/middleware/boards/${main_target}/vnd_flash/vnd_flash.c")
	set(source_hfile "${armino_path}/tools/build_tools/part_table_tools/tempFiles/vnd_flash_partition.h")
	set(source_hfile_old "${armino_path}/middleware/boards/${main_target}/vnd_flash/vnd_flash_partition.h")
	set(part_csv_file "${armino_path}/middleware/boards/${main_target}/partitions.csv")
	set(config_file "${armino_path}/middleware/soc/${target}/${target}.defconfig")
	set(src_sag_file "${armino_path}/middleware/soc/${target}/${target}_bsp.sag")
	set(refer_sag_file "${armino_path}/tools/build_tools/part_table_tools/tempFiles/application${slave_target_num}.sag")
	set(dest_sag_file "${armino_path}/middleware/soc/${target}/${target}_bsp_new.sag")
	set(det_ld_file "${armino_path}/middleware/soc/${target}/${target}_bsp_new.ld")
endif()

set(special_supported_tool "${armino_path}/tools/build_tools/part_table_tools/otherScript/special_project_deal.py")
set(special_supported_tool_args "--project-dir=${project_dir};--project=${special_project};--json-src=${configuration_json};--config=${config_file}")
set(sag_supported_tool "${armino_path}/tools/build_tools/part_table_tools/otherScript/special_sag_deal.py")
set(sag_supported_tool_args "--armino-soc=${target}")
set(nds_files_path "${armino_path}/tools/build_tools/nds_files")
set(nds_ldsag_cmd ${nds_files_path}/nds_ldsag --version=v5 -t ${nds_files_path}/nds32_template_v5_linux.txt)
set(add_gen_files_tool "${armino_path}/tools/build_tools/part_table_tools/otherScript/add_gen_files_list.py")

# ${target}'s ld file take part in linking process
# 1: Ensure ${target}'s add_subdirectory(...) has processed
# 2: Only when ${target}'s add_subdirectory(...) has processed, 
#    then ${target}'s get_target_property(...) will success
# 3: finally read ${target}'s PROPERTIES: INTERFACE_LINK_LIBRARIES
#    and only modify the args of INTERFACE_LINK_LIBRARIES: -L, -T
# note:
#    all actions below must be done between ${target}'s add_subdirectory(...)
#    and app.elf linking process
set(component_name_temp ${COMPONENT_NAME})
set(component_dir_temp ${COMPONENT_DIR})
set(component_alias_temp ${COMPONENT_ALIAS})
set(componnet_path_temp ${COMPONENT_PATH})
__component_get_target(component_target ${target})
__component_get_property(dir ${component_target} COMPONENT_DIR)
__component_get_property(_name ${component_target} COMPONENT_NAME)
__component_get_property(prefix ${component_target} __PREFIX)
__component_get_property(alias ${component_target} COMPONENT_ALIAS)
__component_get_property(component_lib ${component_target} COMPONENT_LIB)
set(COMPONENT_NAME ${_name})
set(COMPONENT_DIR ${dir})
set(COMPONENT_ALIAS ${alias})
set(COMPONENT_PATH ${dir})
armino_build_get_property(build_prefix __PREFIX)
armino_build_get_property(include_prefix __INCLUDE_PREFIX)
__component_get_property(component_add_subdired ${component_target} __COMPONENT_ADD_SUBDIRED)
if(NOT "${component_add_subdired}" STREQUAL 1)
	if(NOT prefix STREQUAL build_prefix AND NOT prefix STREQUAL __INCLUDE_PREFIX)
		add_subdirectory(${dir} ${build_dir}/armino/${prefix}_${_name})
	else()
		add_subdirectory(${dir} ${build_dir}/armino/${_name})
	endif()
	__component_set_property(${component_target} __COMPONENT_ADD_SUBDIRED 1)
endif()
get_target_property(__link_libraries "${component_lib}" INTERFACE_LINK_LIBRARIES)
list(FILTER __link_libraries EXCLUDE REGEX "(^-L.*)|(^-T.*)")
get_filename_component(dest_ld_file_dir "${det_ld_file}" DIRECTORY)
list(APPEND __link_libraries "-L ${dest_ld_file_dir}")
get_filename_component(dest_ld_file_nm "${det_ld_file}" NAME)
list(APPEND __link_libraries "-T ${dest_ld_file_nm}")
set_target_properties("${component_lib}" PROPERTIES INTERFACE_LINK_LIBRARIES "${__link_libraries}")
set(COMPONENT_NAME ${component_name_temp})
set(COMPONENT_DIR ${component_dir_temp})
set(COMPONENT_ALIAS ${component_alias_temp})
set(COMPONENT_PATH ${componnet_path_temp})

macro(pt_verbose_slave_targets)
	message(STATUS "${Magenta}<========================part_table_tools slave target info start========================>${ColourReset}")
	message(STATUS "${Magenta}target: ${target}${ColourReset}")
	message(STATUS "${Magenta}main_target: ${main_target}${ColourReset}")
	message(STATUS "${Magenta}supported_targets: ${supported_targets}${ColourReset}")
	message(STATUS "${Magenta}supported_main_targets: ${supported_main_targets}${ColourReset}")
	message(STATUS "${Magenta}supported_slave_targets: ${supported_slave_targets}${ColourReset}")
	message(STATUS "${Magenta}special_project: ${special_project}${ColourReset}")
	message(STATUS "${Magenta}CONFIG_OVERRIDE_FLASH_PARTITION: ${CONFIG_FLASH_PARTITION_USER}${ColourReset}")
	message(STATUS "${Magenta}part_csv_file: ${part_csv_file}${ColourReset}")
	message(STATUS "${Magenta}source_cfile: ${source_cfile_old}${ColourReset}")
	message(STATUS "${Magenta}source_hfile: ${source_hfile_old}${ColourReset}")
	message(STATUS "${Magenta}source_file_inseq: ${source_file_inseq}${ColourReset}")
	message(STATUS "${Magenta}refer_sag_file: ${refer_sag_file}${ColourReset}")
	message(STATUS "${Magenta}application${slave_target_num}_sag_inseq: ${application${slave_target_num}_sag_inseq}${ColourReset}")
	message(STATUS "${Magenta}src_sag_file: ${src_sag_file}${ColourReset}")
	message(STATUS "${Magenta}dest_sag_file: ${dest_sag_file}${ColourReset}")
	message(STATUS "${Magenta}det_ld_file: ${det_ld_file}${ColourReset}")
	message(STATUS "${Magenta}<========================part_table_tools slave target info stop ========================>${ColourReset}")
endmacro()

macro(pt_verbose_main_targets)
	message(STATUS "${Magenta}<========================part_table_tools main target info start ========================>${ColourReset}")
	message(STATUS "${Magenta}target: ${target}${ColourReset}")
	message(STATUS "${Magenta}main_target: ${main_target}${ColourReset}")
	message(STATUS "${Magenta}supported_targets: ${supported_targets}${ColourReset}")
	message(STATUS "${Magenta}supported_main_targets: ${supported_main_targets}${ColourReset}")
	message(STATUS "${Magenta}supported_slave_targets: ${supported_slave_targets}${ColourReset}")
	message(STATUS "${Magenta}special_project: ${special_project}${ColourReset}")
	message(STATUS "${Magenta}CONFIG_OVERRIDE_FLASH_PARTITION: ${CONFIG_FLASH_PARTITION_USER}${ColourReset}")
	message(STATUS "${Magenta}part_csv_file: ${part_csv_file}${ColourReset}")
	message(STATUS "${Magenta}config_file: ${config_file}${ColourReset}")
	message(STATUS "${Magenta}source_cfile: ${source_cfile_old}${ColourReset}")
	message(STATUS "${Magenta}source_hfile: ${source_hfile_old}${ColourReset}")
	message(STATUS "${Magenta}source_file_inseq: ${source_file_inseq}${ColourReset}")
	message(STATUS "${Magenta}configuration_json: ${configuration_json_old}${ColourReset}")
	message(STATUS "${Magenta}configuration_json_inseq: ${configuration_json_inseq}${ColourReset}")
	message(STATUS "${Magenta}refer_sag_file: ${refer_sag_file}${ColourReset}")
	message(STATUS "${Magenta}application${slave_target_num}_sag_inseq: ${application${slave_target_num}_sag_inseq}${ColourReset}")
	message(STATUS "${Magenta}src_sag_file: ${src_sag_file}${ColourReset}")
	message(STATUS "${Magenta}dest_sag_file: ${dest_sag_file}${ColourReset}")
	message(STATUS "${Magenta}det_ld_file: ${det_ld_file}${ColourReset}")
	message(STATUS "${Magenta}<========================part_table_tools main target info stop  ========================>${ColourReset}")
endmacro()

if (EXISTS ${part_csv_file})
	if(target IN_LIST supported_slave_targets)
		if("$ENV{PT_VERBOSE}" STREQUAL "1")
			pt_verbose_slave_targets()
		endif()
		# gen application[1-9].sag file
		# read and modify bk*_cp[1-9]_bsp.sag according to application[1-9].sag file
		# gen bk*_cp[1-9]_bsp.ld file from bk*_cp[1-9]_bsp.sag
		add_custom_command(
				OUTPUT "${det_ld_file}"
				COMMAND "${python}" "${part_table_tool}"
				${part_csv_file} --to-sag=${refer_sag_file}
				${part_table_args} ${application${slave_target_num}_sag_inseq}
				COMMAND "${python}" "${sag_supported_tool}" ${sag_supported_tool_args}
				--refer-sag=${refer_sag_file} --src-sag=${src_sag_file} --dest-sag=${dest_sag_file}
				COMMAND ${nds_ldsag_cmd} ${dest_sag_file} -o ${det_ld_file}
				COMMAND "${python}" "${add_gen_files_tool}" ${det_ld_file}
				MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file} ${sag_supported_tool} ${src_sag_file}
				${nds_files_path}/nds_ldsag ${nds_files_path}/nds32_template_v5_linux.txt
				COMMENT "armino ====> gen application${slave_target_num}.sag and application${slave_target_num}.ld..."
				VERBATIM)

		# check whether depends_files of *.h files modify
		# gen *.c and *.h files
		add_custom_command(
				OUTPUT "${source_cfile}"
				#COMMAND ${CMAKE_COMMAND} -E touch ${source_cfile}
				COMMAND "${python}" "${depends_check_tool}" ${armino_path}
				COMMAND "${python}" "${part_table_tool}"
				${part_csv_file} --to-src=${source_cfile}
				--to-inc=${source_hfile}
				${part_table_args} ${source_file_inseq}
				COMMAND ${CMAKE_COMMAND} -E copy ${source_cfile} ${source_cfile_old}
				COMMAND ${CMAKE_COMMAND} -E copy ${source_hfile} ${source_hfile_old}
				MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file}
				COMMENT "armino ====> gen *c and *h files..."
				VERBATIM)

		add_custom_target(part_table_tool_op DEPENDS ${det_ld_file} ${source_cfile})
		add_dependencies(${COMPONENT_LIB} part_table_tool_op)
	elseif(target IN_LIST supported_main_targets)
		if("$ENV{PT_VERBOSE}" STREQUAL "1")
			pt_verbose_main_targets()
		endif()
		# gen application.sag file
		# read and modify bk*_bsp.sag according to application.sag file
		# gen bk*_bsp.ld file from bk*_bsp.sag
		add_custom_command(
				OUTPUT "${det_ld_file}"
				COMMAND "${python}" "${part_table_tool}"
				${part_csv_file} --to-sag=${refer_sag_file}
				${part_table_args} ${application_sag_inseq}
				COMMAND "${python}" "${sag_supported_tool}" ${sag_supported_tool_args}
				--refer-sag=${refer_sag_file} --src-sag=${src_sag_file} --dest-sag=${dest_sag_file}
				COMMAND ${nds_ldsag_cmd} ${dest_sag_file} -o ${det_ld_file}
				COMMAND "${python}" "${add_gen_files_tool}" ${det_ld_file}
				MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file} ${sag_supported_tool} ${src_sag_file}
				${nds_files_path}/nds_ldsag ${nds_files_path}/nds32_template_v5_linux.txt
				COMMENT "armino ====> gen application_sag and application_ld..."
				VERBATIM)

		# gen configuration.json file
		# modify *.defconfig and *.config file
		add_custom_command(
				OUTPUT "${configuration_json}"
				#COMMAND ${CMAKE_COMMAND} -E touch ${configuration_json}
				COMMAND "${python}" "${part_table_tool}"
				${part_csv_file} --to-json=${configuration_json}
				${part_table_args} ${configuration_json_inseq}
				COMMAND ${CMAKE_COMMAND} -E copy ${configuration_json} ${configuration_json_old}
				COMMAND "${python}" "${special_supported_tool}" ${special_supported_tool_args}
				MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file}
				COMMENT "armino ====> gen configuration_json..."
				VERBATIM)

		# check whether depends_files of *.h files modify
		# gen *.c and *.h files
		add_custom_command(
				OUTPUT "${source_cfile}"
				#COMMAND ${CMAKE_COMMAND} -E touch ${source_cfile}
				COMMAND "${python}" "${depends_check_tool}" ${armino_path}
				COMMAND "${python}" "${part_table_tool}"
				${part_csv_file} --to-src=${source_cfile}
				--to-inc=${source_hfile}
				${part_table_args} ${source_file_inseq}
				COMMAND ${CMAKE_COMMAND} -E copy ${source_cfile} ${source_cfile_old}
				COMMAND ${CMAKE_COMMAND} -E copy ${source_hfile} ${source_hfile_old}
				MAIN_DEPENDENCY ${part_table_tool} ${part_csv_file}
				COMMENT "armino ====> gen *c and *h files..."
				VERBATIM)

		add_custom_target(part_table_tool_op DEPENDS ${det_ld_file} ${configuration_json} ${source_cfile})

		# regenerate sdkconfig.h after modify *.defconfig or *.config file
		# which must done before all other components,and after part_table_tool_op
		add_custom_target(config_regenerate
				COMMAND ${confgen_basecommand}
				--output header ${sdkconfig_header}
				--output cmake ${sdkconfig_cmake}
				--output json ${sdkconfig_json}
				--output json_menus ${sdkconfig_json_menus}
				--output config ${sdkconfig}
				DEPENDS part_table_tool_op
				COMMENT "armino ====> regenerate sdkconfig file..."
				VERBATIM)

		add_dependencies(${COMPONENT_LIB} config_regenerate)
	else()
		message(SEND_ERROR "target is not supported!!!")
	endif()
else()
	message(SEND_ERROR "${part_csv_file} is not exist!!!")
endif()