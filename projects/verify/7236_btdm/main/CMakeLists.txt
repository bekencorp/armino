set(COMMON_DIR ../../common)
set(incs "." ${COMMON_DIR})
set(srcs app_main.c ${COMMON_DIR}/sys_misc.c)

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS ${incs})
