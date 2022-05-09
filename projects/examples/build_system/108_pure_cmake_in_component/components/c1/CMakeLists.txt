armino_component_register(SRCS c1.c INCLUDE_DIRS include)

add_subdirectory(foo)
target_link_libraries(${COMPONENT_LIB} foo)
