PART_TABLE_SUPPORTED_TARGETS := bk7256
ARMINO_TOOL_PART_TABLE := @$(ARMINO_DIR)/tools/build_tools/part_table_tools/gen_bk7256partitions.py
PARTITIONS_CSV_FILE := $(ARMINO_DIR)/middleware/boards/$(ARMINO_SOC)/partitions.csv
PARTITIONS_ARGS := --flash-size=16MB --smode
BOOTLOADER_JSON_INSEQ := --smode-inseq=1,1,2,0,0,0
BOOTLOADER_JSON := $(ARMINO_BOOTLOADER)tools/partition_bk7256_ota_a_new.json
BOOTLOADER_JSON_OLD := $(ARMINO_BOOTLOADER)tools/partition_bk7256_ota_a.json
CLEAN_ALLFILE_INSEQ := --smode-inseq=3,0
DEFAULT_CSV_FILE := $(ARMINO_DIR)/tools/build_tools/part_table_tools/bk7256Partitions.csv