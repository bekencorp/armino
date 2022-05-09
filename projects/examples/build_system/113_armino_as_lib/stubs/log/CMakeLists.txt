add_library(stub_log STATIC log.c)
target_include_directories(stub_log PUBLIC include)
target_include_directories(stub_log PUBLIC .)
add_library(stub::log ALIAS stub_log)
