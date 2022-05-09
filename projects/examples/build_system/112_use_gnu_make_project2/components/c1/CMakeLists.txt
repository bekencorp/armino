armino_component_register(SRCS c1.c INCLUDE_DIRS include)

add_custom_command(OUTPUT ${COMPONENT_DIR}/foo/libfoo.a
    COMMAND ${COMPONENT_DIR}/foo/build.sh ${COMPONENT_DIR}/foo ${CMAKE_C_COMPILER}
    VERBATIM
    COMMENT "Build external project"
    )
add_custom_target(foo_build DEPENDS ${COMPONENT_DIR}/foo/libfoo.a)
add_library(foo STATIC IMPORTED GLOBAL)
add_dependencies(foo foo_build)
set_target_properties(foo PROPERTIES IMPORTED_LOCATION ${COMPONENT_DIR}/foo/libfoo.a)
set_target_properties(foo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${COMPONENT_DIR}/foo/include)
target_link_libraries(${COMPONENT_LIB} PUBLIC foo)
