PART_TABLE_SUPPORTED_TARGETS := bk7256 bk7235
PART_TABLE_SUPPORTED_PROJECTS := app
ARMINO_TOOL_PART_TABLE := $(ARMINO_DIR)/tools/build_tools/part_table_tools/gen_bk7256partitions.py
PARTITIONS_ARGS := --flash-size=16MB --smode
BOOTLOADER_JSON_INSEQ := --smode-inseq=1,1,2,0,0,0
BOOTLOADER_JSON := $(ARMINO_BOOTLOADER)tools/partition_bk7256_ota_a_new.json
BOOTLOADER_JSON_OLD := $(ARMINO_BOOTLOADER)tools/partition_bk7256_ota_a.json
CLEAN_ALLFILE_INSEQ := --smode-inseq=3,0
SHOW_APPS_INSEQ := --smode-inseq=4,0
DEFAULT_CSV_FILE := $(ARMINO_DIR)/tools/build_tools/part_table_tools/bk7256Partitions.csv

ifneq ($(findstring $(ARMINO_SOC), $(PART_TABLE_SUPPORTED_TARGETS)),)
	main_target_config := $(ARMINO_DIR)/$(PROJECT_DIR)/config/$(ARMINO_SOC).config
	target_config_tool := $(ARMINO_DIR)/tools/build_tools/part_table_tools/otherScript/get_target_config_val.py
	target_config_tool_args := --config=$(main_target_config)
	config_value := $(shell python3 $(target_config_tool) $(target_config_tool_args))
	ifeq ("$(config_value)", "y")
		PARTITIONS_CSV_FILE := $(ARMINO_DIR)/$(PROJECT_DIR)/csv/$(ARMINO_SOC).csv
	else
		PARTITIONS_CSV_FILE := $(ARMINO_DIR)/middleware/boards/$(ARMINO_SOC)/partitions.csv
	endif

	app_names := $(shell python3 $(ARMINO_TOOL_PART_TABLE) $(PARTITIONS_CSV_FILE) $(PARTITIONS_ARGS) $(SHOW_APPS_INSEQ))
	app1_search := app1
	ifneq ($(findstring $(app1_search), $(app_names)),)
		SUPPORT_DUAL_CORE := true
	else
		SUPPORT_DUAL_CORE := false
	endif
else
	PARTITIONS_CSV_FILE := $(ARMINO_DIR)/middleware/boards/$(ARMINO_SOC)/partitions.csv
endif

PT_VERBOSE ?= 0
ifeq ("$(PT_VERBOSE)", "1")
	export PT_VERBOSE := 1
else
	export PT_VERBOSE := 0
endif
ifeq ("$(PT_VERBOSE)", "1")
$(info "<========================part_table_tools info start========================>")
$(info "PT_VERBOSE: $(PT_VERBOSE)")
$(info "CONFIG_OVERRIDE_FLASH_PARTITION: $(config_value)")
$(info "PARTITIONS_CSV_FILE: $(PARTITIONS_CSV_FILE)")
$(info "BOOTLOADER_JSON: $(BOOTLOADER_JSON)")
$(info "BOOTLOADER_JSON_INSEQ: $(BOOTLOADER_JSON_INSEQ)")
$(info "SHOW_APPS_INSEQ: $(SHOW_APPS_INSEQ)")
#$(info "app_names: $(app_names)")
#$(info "SUPPORT_DUAL_CORE: $(SUPPORT_DUAL_CORE)")
$(info "<========================part_table_tools info stop ========================>")
endif