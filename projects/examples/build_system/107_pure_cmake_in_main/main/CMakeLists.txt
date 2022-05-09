armino_component_register(SRCS "main.c" INCLUDE_DIRS .)

add_subdirectory(foo)
# Link foo to main component
target_link_libraries(${COMPONENT_LIB} PUBLIC foo)
