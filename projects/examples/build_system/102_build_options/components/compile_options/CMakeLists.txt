# If the component/file private compile options are set before armino_component_register,
# use if (NOT CMAKE_BUILD_EARLY_EXPANSION)/endif() to seperate it.
if (NOT CMAKE_BUILD_EARLY_EXPANSION)
	set_source_files_properties(compile_options.c
		PROPERTIES COMPILE_FLAGS
		-Wno-old-style-declaration
		)
endif()

armino_component_register(SRCS compile_options.c INCLUDE_DIRS include)

# Place it after armino_component_register
set_source_files_properties(compile_options.c
	PROPERTIES COMPILE_FLAGS
	-Wno-unused-function
	)

# Place it after armino_component_register
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)

