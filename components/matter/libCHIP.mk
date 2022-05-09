#!/bin/bash
# This file is used to generate CHIP core library
#

CROSS_COMPILE_CHOICE=2

ifndef TOOLCHAIN_PATH
TOOLCHAIN_PATH = /opt/risc-v/nds32le-elf-mculib-v5/bin
endif

ifndef BASEDIR
BASEDIR = $(shell pwd)
endif

ifndef OUTPUT_DIR
OUTPUT_DIR = $(CHIP_DIR)/config/beken/components/chip
endif

ifndef TARGET
TARGET := bk7235
endif

CROSS_COMPILE = $(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)

CHIP_DIR = $(BASEDIR)/components/matter

# Compilation tools
AR = $(CROSS_COMPILE)ar
CXX = $(CROSS_COMPILE)g++
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

Q := @
ifeq ($(V),1)
Q := 
endif
OS := $(shell uname)

ifeq ($(ECHO),)
ECHO=echo
endif


# -------------------------------------------------------------------
# Include folder list
# -------------------------------------------------------------------
INCLUDES =
INCLUDES += -I$(BASEDIR)/middleware/arch/bk7256/soc

ifeq ($(CROSS_COMPILE_CHOICE),1)
INCLUDES += -I$(BASEDIR)/components/os_source/freertos_v9/include
INCLUDES += -I$(BASEDIR)/components/os_source/freertos_v9/portable/Keil/ARM968es
INCLUDES += -I$(BASEDIR)/properties/modules/ble_5_x_rw/ble_pub/include
else
INCLUDES += -I$(BASEDIR)/components/os_source/freertos_v10/include
INCLUDES += -I$(BASEDIR)/components/os_source/freertos_v10/portable/GCC/RISC-V
INCLUDES += -I$(BASEDIR)/middleware/arch/riscv/include
INCLUDES += -I$(BASEDIR)/middleware/arch/riscv/include/bk_private
INCLUDES += -I$(BASEDIR)/properties/modules/btdm_5_2_rw/ble_pub/include
endif

INCLUDES += -I$(BASEDIR)/include
INCLUDES += -I$(BASEDIR)/include/modules
INCLUDES += -I$(BASEDIR)/include/arch/compiler
INCLUDES += -I$(BASEDIR)/components/bk_ps/include/bk_private
INCLUDES += -I$(BASEDIR)/components/bk_common/include
INCLUDES += -I$(BASEDIR)/components/bk_common/include/bk_private
INCLUDES += -I$(BASEDIR)/components/bk_common/include/bk_private/legacy
INCLUDES += -I$(BASEDIR)/components/bk_config/include/cmake
INCLUDES += -I$(BASEDIR)/components/bk_log/include
INCLUDES += -I$(BASEDIR)/components/bk_rtos/include
INCLUDES += -I$(BASEDIR)/components/bk_rtos/include/bk_private
INCLUDES += -I$(BASEDIR)/components/bk_rtos/freertos
INCLUDES += -I$(BASEDIR)/components/bk_system/include
INCLUDES += -I$(BASEDIR)/components/bk_cli/include

INCLUDES += -I$(BASEDIR)/components/at/include
#INCLUDES += -I$(BASEDIR)/components/bk_ble/ble_v5/include
#INCLUDES += -I$(BASEDIR)/components/bk_ble/ble_v5/include/bk_private

INCLUDES += -I$(BASEDIR)/components/bk_wifi/include
INCLUDES += -I$(BASEDIR)/components/bk_wifi/include/bk_private
INCLUDES += -I$(BASEDIR)/components/bk_wifi/include/bk_private/legacy
INCLUDES += -I$(BASEDIR)/components/bk_netif/include
INCLUDES += -I$(BASEDIR)/components/bk_netif/include/bk_private


INCLUDES += -I$(BASEDIR)/middleware/driver/
INCLUDES += -I$(BASEDIR)/middleware/driver/include
INCLUDES += -I$(BASEDIR)/middleware/driver/include/bk_private
INCLUDES += -I$(BASEDIR)/middleware/driver/include/bk_private/legacy
INCLUDES += -I$(BASEDIR)/middleware/driver/common
INCLUDES += -I$(BASEDIR)/middleware/driver/pwm
INCLUDES += -I$(BASEDIR)/middleware/driver/rc_beken
INCLUDES += -I$(BASEDIR)/middleware/driver/flash
INCLUDES += -I$(BASEDIR)/middleware/driver/rw_pub
INCLUDES += -I$(BASEDIR)/middleware/driver/uart
INCLUDES += -I$(BASEDIR)/middleware/driver/sys_ctrl
INCLUDES += -I$(BASEDIR)/middleware/driver/gpio
INCLUDES += -I$(BASEDIR)/middleware/driver/general_dma
INCLUDES += -I$(BASEDIR)/middleware/driver/icu
INCLUDES += -I$(BASEDIR)/middleware/driver/i2c
INCLUDES += -I$(BASEDIR)/middleware/driver/sdcard
INCLUDES += -I$(BASEDIR)/middleware/driver/saradc
INCLUDES += -I$(BASEDIR)/middleware/driver/pmu
INCLUDES += -I$(BASEDIR)/middleware/driver/mailbox
INCLUDES += -I$(BASEDIR)/middleware/driver/touch
INCLUDES += -I$(BASEDIR)/middleware/driver/sbc
INCLUDES += -I$(BASEDIR)/middleware/driver/rtc
INCLUDES += -I$(BASEDIR)/middleware/arch/bk7231n/soc
INCLUDES += -I$(BASEDIR)/middleware/arch/bk7231n/hal
INCLUDES += -I$(BASEDIR)/middleware/arch/common/soc/include
INCLUDES += -I$(BASEDIR)/middleware/arch/common/hal/include

INCLUDES += -I$(BASEDIR)/components/user_driver/include/bk_private
INCLUDES += -I$(BASEDIR)/middleware/compal/matter


INCLUDES += -I$(BASEDIR)/components/lwip_intf_v2_1/lwip-2.1.2/port
INCLUDES += -I$(BASEDIR)/components/lwip_intf_v2_1/lwip-2.1.2/src/include
#INCLUDES += -I$(BASEDIR)/components/lwip_intf_v2_0/lwip-2.0.2/port
#INCLUDES += -I$(BASEDIR)/components/lwip_intf_v2_0/lwip-2.0.2/src/include

INCLUDES += -I$(BASEDIR)/components/mbedtls/mbedtls/include




#examples
INCLUDES += -I$(CHIP_DIR)/examples/lighting-app/beken
INCLUDES += -I$(CHIP_DIR)/examples/lighting-app/beken/main
INCLUDES += -I$(CHIP_DIR)/examples/lighting-app/beken/main/include

#src
INCLUDES += -I$(CHIP_DIR)/src
INCLUDES += -I$(CHIP_DIR)/src/app/server
INCLUDES += -I$(CHIP_DIR)/src/lib

#out incluede
#INCLUDES += -I$(CHIP_DIR)/config/beken/components/chip/out/7231n/gen/include
#INCLUDES += -I$(CHIP_DIR)/gen/include

# TODO fix it
# Matter should depends on sdkconfig.h generated in $(CONFIG_DIR), however a lot of 
# build error if directly use the correct sdkconfig.h, owner need to fix it!
#INCLUDES += -I$(CONFIG_DIR)
INCLUDES += -I$(CHIP_DIR)/bk7235

INCLUDES += -I$(OUTPUT_DIR)/out/$(TARGET)/gen/include
INCLUDES += -I$(CHIP_DIR)/src/include

#zzz_generated
INCLUDES += -I$(CHIP_DIR)/zzz_generated
INCLUDES += -I$(CHIP_DIR)/zzz_generated/app-common
INCLUDES += -I$(CHIP_DIR)/zzz_generated/lighting-app


#third_party
INCLUDES += -I$(CHIP_DIR)/third_party/nlassert/repo/include
INCLUDES += -I$(CHIP_DIR)/third_party/nlio/repo/include



# -------------------------------------------------------------------
# CHIP application layer source
# -------------------------------------------------------------------
SRC_CPP = 
SRC_CPP += $(CHIP_DIR)/src/app/server/EchoHandler.cpp
SRC_CPP += $(CHIP_DIR)/src/app/server/Dnssd.cpp
SRC_CPP += $(CHIP_DIR)/src/app/server/OnboardingCodesUtil.cpp
SRC_CPP += $(CHIP_DIR)/src/app/server/Server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/server/CommissioningWindowManager.cpp

SRC_CPP += $(CHIP_DIR)/src/app/util/af-event.cpp
#SRC_CPP += $(CHIP_DIR)/src/app/util/af-main-common.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/attribute-size-util.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/attribute-storage.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/attribute-table.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/binding-table.cpp
#SRC_CPP += $(CHIP_DIR)/src/app/util/CHIPDeviceCallbacksMgr.cpp
#SRC_CPP += $(CHIP_DIR)/src/app/util/chip-message-send.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/client-api.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/DataModelHandler.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/ember-compatibility-functions.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/ember-print.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/esi-management.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/message.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/util.cpp
SRC_CPP += $(CHIP_DIR)/src/app/util/error-mapping.cpp

#SRC_CPP += $(CHIP_DIR)/src/lib/dnssd/minimal_mdns/responders/IP.cpp

SRC_CPP += $(CHIP_DIR)/src/app/clusters/basic/basic.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/bindings/bindings.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/diagnostic-logs-server/diagnostic-logs-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/general-commissioning-server/general-commissioning-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/on-off-server/on-off-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/fixed-label-server/fixed-label-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/operational-credentials-server/operational-credentials-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/level-control/level-control.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/localization-configuration-server/localization-configuration-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/network-commissioning/network-commissioning.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/network-commissioning-old/network-commissioning-ember.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/network-commissioning-old/network-commissioning-old.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/color-control-server/color-control-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/occupancy-sensor-server/occupancy-sensor-server.cpp
#SRC_CPP += $(CHIP_DIR)/src/app/clusters/ota-provider/ota-provider.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/ota-requestor/BDXDownloader.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/ota-requestor/OTARequestor.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/ota-requestor/ota-requestor-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/user-label-server/user-label-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/ethernet-network-diagnostics-server/ethernet-network-diagnostics-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/software-diagnostics-server/software-diagnostics-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/switch-server/switch-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/thread-network-diagnostics-server/thread-network-diagnostics-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/wifi-network-diagnostics-server/wifi-network-diagnostics-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/administrator-commissioning-server/administrator-commissioning-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/general-diagnostics-server/general-diagnostics-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/identify-server/identify-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/descriptor/descriptor.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/time-format-localization-server/time-format-localization-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/access-control-server/access-control-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/group-key-mgmt-server/group-key-mgmt-server.cpp
SRC_CPP += $(CHIP_DIR)/src/app/clusters/groups-server/groups-server.cpp

#SRC_CPP += $(CHIP_DIR)/src/app/reporting/Engine.cpp

SRC_CPP += $(CHIP_DIR)/zzz_generated/app-common/app-common/zap-generated/attributes/Accessors.cpp

SRC_CPP += $(CHIP_DIR)/zzz_generated/lighting-app/zap-generated/callback-stub.cpp
SRC_CPP += $(CHIP_DIR)/zzz_generated/lighting-app/zap-generated/IMClusterCommandHandler.cpp
#SRC_CPP += $(CHIP_DIR)/zzz_generated/lighting-app/zap-generated/CHIPClientCallbacks.cpp

SRC_CPP += $(CHIP_DIR)/examples/lighting-app/beken/main/chipinterface.cpp
SRC_CPP += $(CHIP_DIR)/examples/lighting-app/beken/main/DeviceCallbacks.cpp
SRC_CPP += $(CHIP_DIR)/examples/lighting-app/beken/main/CHIPDeviceManager.cpp
#SRC_CPP += $(CHIP_DIR)/examples/lighting-app/beken/main/Globals.cpp
SRC_CPP += $(CHIP_DIR)/examples/lighting-app/beken/main/DsoHack.cpp
#SRC_CPP += $(CHIP_DIR)/examples/lighting-app/beken/main/LEDWidget.cpp
# ------------------------------------------------------------------
# Generate obj list
# -------------------------------------------------------------------
OBJS = $(SRC_CPP:.cpp=.o)
OBJ_CPP_LIST = $(notdir $(OBJS))

# -------------------------------------------------------------------
# CHIP compile options
# -------------------------------------------------------------------
CFLAGS =

CFLAGS += -DCHIP_PROJECT=1
CFLAGS += -DCHIP_HAVE_CONFIG_H=1

CFLAGS += -DCFG_MBEDTLS=1
CFLAGS += -DLWIP_IPV6=1

CFLAGS += -DLWIP_IPV6_ND=1
CFLAGS += -DLWIP_IPV6_SCOPES=1
CFLAGS += -DLWIP_PBUF_FROM_CUSTOM_POOLS=0
CFLAGS += -DLWIP_IPV6_ROUTE_TABLE_SUPPORT=1

CFLAGS += -DCHIP_DEVICE_LAYER_NONE=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_ZEPHYR_NET_IF=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_BSD_IFADDRS=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_ZEPHYR_SOCKET_EXTENSIONS=0

CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_LWIP=1
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_SOCKETS=0
CFLAGS += -DCHIP_SYSTEM_CONFIG_USE_NETWORK_FRAMEWORK=0

CXXFLAGS =
ifeq ($(CROSS_COMPILE_CHOICE),1)
CXXFLAGS += -marm -mcpu=arm968e-s -march=armv5te -mthumb-interwork -mlittle-endian
CXXFLAGS += --specs=nosys.specs --specs=nano.specs
else
CXXFLAGS += -mstrict-align
endif

CXXFLAGS += -Wno-conversion
CXXFLAGS += -Os
CXXFLAGS += -Wno-error
CXXFLAGS += -Wno-sign-compare
CXXFLAGS += -Wno-unused-function
CXXFLAGS += -Wno-unused-but-set-variable
CXXFLAGS += -Wno-unused-variable
#CXXFLAGS += -Wno-deprecated-declarations
CXXFLAGS += -Wno-unused-parameter
#CXXFLAGS += -Wno-format
CXXFLAGS += -Wno-literal-suffix
#CXXFLAGS += -std=c++14
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-exceptions


CHIP_CFLAGS = $(CFLAGS)
CHIP_CFLAGS += $(INCLUDES)

CHIP_CXXFLAGS += $(CFLAGS)
CHIP_CXXFLAGS += $(CXXFLAGS)
CHIP_CXXFLAGS += $(INCLUDES)


export CHIP_ROOT_ENV=$(CHIP_DIR)
export BUILD_ROOT_ENV=$(CHIP_DIR)/build

#*****************************************************************************#
#                        RULES TO GENERATE libCHIP.a and libAPPLICATION.a     #
#*****************************************************************************#

# Define the Rules to build the core targets
all: CHIP_CORE APP

CHIP_CORE:
	@echo "target=$(TARGET)"
	@echo "toolchain=$(TOOLCHAIN_PATH) prefix=$(TOOLCHAIN_PREFIX)"
	@echo "base_dir=$(BASEDIR)"
	@echo "config=$(CONFIG_DIR)"
	@echo "output_dir=$(OUTPUT_DIR)"
	if [ ! -d $(OUTPUT_DIR) ]; then \
		mkdir -p $(OUTPUT_DIR); mkdir -p $(OUTPUT_DIR)/app_obj;\
	fi
	if [ ! -d $(OUTPUT_DIR)/app_obj ]; then \
		mkdir -p $(OUTPUT_DIR)/app_obj;\
	fi
	@echo "Compiling CHIP SDK static library"
	@echo                                   > $(OUTPUT_DIR)/args.gn
	@echo "import(\"//args.gni\")"          >> $(OUTPUT_DIR)/args.gn
	@echo target_cflags_c  = [$(foreach word,$(CHIP_CFLAGS),\"$(word)\",)] | sed -e 's/=\"/=\\"/g;s/\"\"/\\"\"/g;'  >> $(OUTPUT_DIR)/args.gn
	@echo target_cflags_cc = [$(foreach word,$(CHIP_CXXFLAGS),\"$(word)\",)] | sed -e 's/=\"/=\\"/g;s/\"\"/\\"\"/g;'   >> $(OUTPUT_DIR)/args.gn
	@echo beken_ar = \"$(AR)\"    >> $(OUTPUT_DIR)/args.gn
	@echo beken_cc = \"$(CC)\"   >> $(OUTPUT_DIR)/args.gn
	@echo beken_cxx = \"$(CXX)\"  >> $(OUTPUT_DIR)/args.gn
	cd $(CHIP_DIR)/config/beken && gn gen --check --fail-on-unused-args $(OUTPUT_DIR)/out/$(TARGET) && cp $(OUTPUT_DIR)/args.gn $(OUTPUT_DIR)/out/$(TARGET)/
	cd $(CHIP_DIR)/config/beken ; ninja -C $(OUTPUT_DIR)/out/$(TARGET)

APP: $(OBJS)
	@echo "Archive the libAPPLICATION.a"
	@cd $(OUTPUT_DIR)/app_obj; $(AR) crs libAPPLICATION.a $(OBJ_CPP_LIST); mv ./libAPPLICATION.a ../out/$(TARGET)/lib/
	@echo "Done"
.cpp.o:
	@$(CXX) $(CXXFLAGS) $(CFLAGS) -o $@ -c $< $(INCLUDES)
	@echo "CXX $@"
	@mv $@ $(OUTPUT_DIR)/app_obj/$(notdir $@)

.PHONY: clean
clean:
	rm -rf $(OUTPUT_DIR)/

