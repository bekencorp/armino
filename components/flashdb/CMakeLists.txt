if (NOT CONFIG_SLAVE_CORE)
if (CONFIG_FLASHDB)

set(incs)
set(srcs
	)

set(incs
	./inc
	./port/fal/inc
	./port
	)

set(srcs
	src/fdb.c
	src/fdb_kvdb.c
	src/fdb_tsdb.c
	src/fdb_utils.c
	src/fdb_file.c
	port/fal_flash_port.c
	port/fal/src/fal.c
	port/fal/src/fal_flash.c
	port/fal/src/fal_partition.c
	port/fal/src/fal_rtt.c
)

if (CONFIG_FLASHDB_DEMO)
	list(APPEND srcs
			samples/tsdb_sample.c
			samples/kvdb_basic_sample.c
			samples/kvdb_type_blob_sample.c
			samples/kvdb_type_string_sample.c
	)
endif()


endif()
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES include
	)
	

