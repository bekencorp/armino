add_library(c1 c1.c)
target_include_directories(c1 PUBLIC include)
target_link_libraries(c1 armino::c)
