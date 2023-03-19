set(incs .)

set(srcs
	benchmark_main.c
)

if (CONFIG_LV_USE_DEMO_BENCHMARK)
list(APPEND srcs
	lv_demo_benchmark.c
	assets/img_benchmark_cogwheel_alpha16.c
	assets/img_benchmark_cogwheel_argb.c
	assets/img_benchmark_cogwheel_chroma_keyed.c
	assets/img_benchmark_cogwheel_indexed16.c
	assets/img_benchmark_cogwheel_rgb.c
	assets/img_benchmark_cogwheel_rgb565a8.c
	assets/lv_font_bechmark_montserrat_12_compr_az.c
	assets/lv_font_bechmark_montserrat_16_compr_az.c
	assets/lv_font_bechmark_montserrat_28_compr_az.c
)
endif()


armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}" 
	PRIV_REQUIRES media bk_init bk_cli
)
