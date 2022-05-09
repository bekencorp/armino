set(incs include include/bk_private) #The lib_adapter.h is for armino internal libs only
set(srcs lib_adapter.c)
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common bk_log user_driver)
