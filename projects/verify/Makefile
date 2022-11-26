PWD := $(shell pwd)
BUILD_TOOL := ${PWD}/build.sh

all_targets = all s ns win30 help

.PHONY: all_targets

help:
	@echo ""
	@echo " make s - build S project"
	@echo " make ns - build NS project"
	@echo " make all - build S/NS project"
	@echo " make win30 - deploy bin to /win30/xxx/bk7236"
	@echo " make help - display this help info"
	@echo ""

s:
	@${BUILD_TOOL} spe
ns:
	@${BUILD_TOOL} nspe
win30:
	@${BUILD_TOOL} win30

all: ns s
clean:
	@${BUILD_TOOL} clean

