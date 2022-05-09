armino_component_register(SRCS "main.c"
                    INCLUDE_DIRS ".")

# Import the library, specifying a target name and the library path.
# The private dependencies of the library is also specified.
add_prebuilt_library(prebuilt "libprebuilt.a")

# `main` calls a function from the library, so link it to `main`
target_link_libraries(${COMPONENT_LIB} PRIVATE prebuilt)
