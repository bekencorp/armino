#! /bin/bash
# Usage: make_libs.sh [platform]

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

echo "making libs for $PLATFORM ..."

./tools/scripts/generate_beken_libs.sh $PLATFORM
if [ $? != 0 ]; then
	echo "make libs error!"
	exit 1
else
	echo "make libs done."
fi
