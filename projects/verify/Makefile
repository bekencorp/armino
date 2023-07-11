PWD := $(shell pwd)
BUILD_TOOL := ${PWD}/build.sh

soc_targets := bk7236 bk7258
deploy_targets := "win30"

SOC := $(word 1, $(MAKECMDGOALS))
DEPLOY := $(word 2, $(MAKECMDGOALS))

.PHONY: all_targets

help:
	@echo ""
	@echo " make bk7236 [win30] - build 7236"
	@echo " make bk7258 [win30] - build 7258"
	@echo " make help - display this help info"
	@echo ""

bk7236:
	@${BUILD_TOOL} ${SOC} ${DEPLOY}
bk7258:
	@${BUILD_TOOL} ${SOC} ${DEPLOY}

win30: 
all: bk7236 bk7258 win30
clean:
	@${BUILD_TOOL} clean

