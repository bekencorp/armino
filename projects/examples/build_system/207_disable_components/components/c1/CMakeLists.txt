set(src)
set(inc)

if(CONFIG_C1)
list(APPEND src c1.c)
list(APPEND inc include)
endif()

armino_component_register(SRCS ${src} INCLUDE_DIRS ${inc})
