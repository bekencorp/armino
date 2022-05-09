#! /bin/bash
# Usage: make_build.sh [platform]

source ./tools/scripts/build_include.sh

if [ "$1" != "" ]; then
	PLATFORM=$1
else
	PLATFORM=bk7231n
fi

validate_platform $PLATFORM
if [ $? != 0 ]; then
	exit 1
fi

OSK_DIR=$(pwd)
SDK_DIR=$OSK_DIR/beken378

PREV_PLATFORM_FILE=.platform
if [ -f $PREV_PLATFORM_FILE ]; then
	PREV_PLATFORM=$(cat $PREV_PLATFORM_FILE)
	if [ "$PREV_PLATFORM" != "$PLATFORM" ]; then
		echo "cleaning $PREV_PLATFORM build ..."
		./tools/scripts/clean_build.sh
		if [ -f ${SDK_DIR}/components/ip/lmac/src/rx/rxl/rxl_cntrl.c ]; then
			./tools/scripts/clean_libs.sh
		fi
		rm -f $PREV_PLATFORM_FILE
	fi
fi

echo "making build for $PLATFORM ..."

make $PLATFORM -j8
if [ $? != 0 ]; then
	echo "make build error!"
	exit 1
else
	echo "make build done."
fi
