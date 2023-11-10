PRE_BUILD_TARGET :=
ifeq ($(SUPPORT_DUAL_CORE),true)
	PRE_BUILD_TARGET += $(ARMINO_SOC)_cp1
endif

ifeq ($(SUPPORT_BOOTLOADER),true)
	PRE_BUILD_TARGET += bootloader
	ARMINO_BOOTLOADER = $(ARMINO_DIR)/properties/modules/bootloader/aboot/bootloader-a-b
	PARTITIONS_CSV_FILE := $(ARMINO_DIR)/$(PROJECT_DIR)/partitions.csv
	PARTITIONS_ARGS := --flash-size=16MB --smode
	BOOTLOADER_JSON_INSEQ := --smode-inseq=1,1,3,0,0,0
	BOOTLOADER_JSON := $(ARMINO_BOOTLOADER)/tools/partition_bk7256_ota_r_new.json
	BOOTLOADER_JSON_OLD := $(ARMINO_BOOTLOADER)/tools/partition_bk7256_ota_r.json
endif