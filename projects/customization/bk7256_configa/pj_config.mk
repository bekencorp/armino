PRE_BUILD_TARGET :=
ifeq ($(SUPPORT_DUAL_CORE),true)
	PRE_BUILD_TARGET += $(ARMINO_SOC)_cp1
endif

ifeq ($(SUPPORT_BOOTLOADER),true)
	PRE_BUILD_TARGET += bootloader
	ARMINO_BOOTLOADER = $(ARMINO_DIR)/properties/modules/bootloader/aboot/bootloader-a-b
endif