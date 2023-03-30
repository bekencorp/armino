set(incs)
set(srcs)

set(incs
	inc/
	inc/qcbor/
) 
set(srcs
	src/ieee754.c
	src/qcbor_decode.c
	src/qcbor_encode.c
	src/qcbor_err_to_str.c
	src/UsefulBuf.c
) 

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}")
