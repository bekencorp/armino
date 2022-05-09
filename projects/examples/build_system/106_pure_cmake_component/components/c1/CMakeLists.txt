if (CMAKE_BUILD_EARLY_EXPANSION)
	return()
endif()

add_library(c1 STATIC c1.c)
target_include_directories(c1 PUBLIC include)

