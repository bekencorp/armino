NAME := entry
$(NAME)_TYPE := kernel
$(NAME)_CFLAGS += -marm
$(NAME)_SOURCES  := boot_handlers_base.S \
		boot_vectors_base.S \
		os/alios/boot_handlers.S

