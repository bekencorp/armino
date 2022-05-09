
if (CONFIG_DMA2D_TEST)
set(incs)
set(srcs
	"dma2d_demo.c"
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}")
