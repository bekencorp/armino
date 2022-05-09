armino_component_register(SRCS c1.c INCLUDE_DIRS include)

include(ExternalProject)
set(FOO_DIR ${COMPONENT_DIR}/foo)
set(FOO_LIB_DIR ${COMPONENT_DIR}/foo)
set(FOO_BUILD_DIR ${COMPONENT_DIR}/build)

ExternalProject_Add(foo_project
	SOURCE_DIR ${FOO_DIR}
	#BINARY_DIR ${FOO_BUILD_DIR}
	CONFIGURE_COMMAND "" # Since foo_project is a Makefile project, we need to set CONFIGURE_COMMMAND to empty string
	BUILD_IN_SOURCE 1
	BUILD_COMMAND make CC=${CMAKE_C_COMPILER} libfoo.a
	BUILD_BYPRODUCTS ${FOO_LIB_DIR}/libfoo.a
	BUILD_ALWAYS 1       # This make sure the foo_project is re-builded when any source code of foo_project is changed!
	INSTALL_COMMAND ""
	)

# Add dependency between armino and foo_project
add_library(foo STATIC IMPORTED GLOBAL)
add_dependencies(foo foo_project)
set_target_properties(foo PROPERTIES IMPORTED_LOCATION ${FOO_LIB_DIR}/libfoo.a)
set_target_properties(foo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${COMPONENT_DIR}/foo/include)
target_link_libraries(${COMPONENT_LIB} PUBLIC foo)
