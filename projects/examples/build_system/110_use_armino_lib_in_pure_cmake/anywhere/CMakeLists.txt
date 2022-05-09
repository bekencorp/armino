file(GLOB component_dirs ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(component ${component_dirs})
	if(EXISTS "${component}/CMakeLists.txt")
        	add_subdirectory(${component})
	endif()
endforeach()

