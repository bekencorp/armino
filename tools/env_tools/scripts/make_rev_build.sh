#! /bin/bash
# change the OSK/SDK revision with the tip commitID and then make build, an put the out builds in a folder
# Usage: make_rev_build.sh [platform] [comment_str]

source ./tools/scripts/build_include.sh

PLATFORMS="$SUPPORTED_BUILD_PLATFORMS"
COMMENT_STR=""

if [ "$1" != "" ]; then
	PLATFORMS="$1"
	if [ "$2" != "" ]; then
		COMMENT_STR=_$2
	fi
fi

if [ "$PLATFORMS" == "all" ]; then
	PLATFORMS="$SUPPORTED_BUILD_PLATFORMS"
fi

for PLATFORM in ${PLATFORMS}
do
	validate_platform $PLATFORM
	if [ $? != 0 ]; then
		exit 1
	fi
done


OSK_DIR=$(pwd)
SDK_DIR=$OSK_DIR/beken378

echo "OSK_DIR: $OSK_DIR"
echo "SDK_DIR: $SDK_DIR"

#if [ -f ${SDK_DIR}/components/ip/lmac/src/rx/rxl/rxl_cntrl.c ]; then
#	./tools/scripts/make_sdk.sh
#	if [ $? != 0 ]; then
#		echo "make sdk eror!"
#		exit 1
#	fi
#fi

OSK_REV_FILE=$SDK_DIR/release/osk_revision.h
SDK_REV_FILE=$SDK_DIR/release/sdk_revision.h

if [ ! -f $OSK_REV_FILE ]; then
	echo "$OSK_REV_FILE: No such file!"
	exit 1
fi

if [ ! -f $SDK_REV_FILE ]; then
	echo "$SDK_REV_FILE: No such file!"
	exit 1
fi

OSK_REV=$(cat $OSK_REV_FILE | grep BEKEN_OSK_REV)
OSK_REV=${OSK_REV#*\"}
OSK_REV=${OSK_REV%\"*}

SDK_REV=$(cat $SDK_REV_FILE | grep BEKEN_SDK_REV)
SDK_REV=${SDK_REV#*\"}
SDK_REV=${SDK_REV%\"*}
SDK_REV=${SDK_REV}$COMMENT_STR

echo "OSK_REV: $OSK_REV"
echo "SDK_REV: $SDK_REV"

backup_revision_file $OSK_REV_FILE $SDK_REV_FILE

echo "echo $OSK_REV into $OSK_REV_FILE ..."
echo "#define BEKEN_OSK_REV	\"$OSK_REV\"" > $OSK_REV_FILE
cat $OSK_REV_FILE

echo "echo $SDK_REV into $SDK_REV_FILE ..."
echo "#define BEKEN_SDK_REV	\"$SDK_REV\"" > $SDK_REV_FILE
cat $SDK_REV_FILE

for PLATFORM in ${PLATFORMS}
do
	echo "start making $PLATFORM build"

	./tools/scripts/make_build.sh $PLATFORM
	if [ $? != 0 ]; then
		echo "make rev build error!"
		restore_revision_file $OSK_REV_FILE $SDK_REV_FILE
		exit 1
	fi

	OUT_FOLDER=out_builds
	if [ ! -d $OUT_FOLDER ]; then
		mkdir $OUT_FOLDER
	fi

	BUILD_DIR=${PLATFORM}_OSK_${OSK_REV}_SDK_${SDK_REV}
	OUT_BUILD_DIR=$OSK_DIR/$OUT_FOLDER/$BUILD_DIR

	echo "OUT_BUILD_DIR: $OUT_FOLDER/$BUILD_DIR"

	if [ -d $OUT_BUILD_DIR ]; then
		echo "found existed OUT_BUILD_DIR, remove it"
		rm -rf $OUT_BUILD_DIR
	fi
	mkdir $OUT_BUILD_DIR

	gather_out_files $PLATFORM $OUT_BUILD_DIR
	if [ $? != 0 ]; then
		echo "gather out files error!"
		restore_revision_file $OSK_REV_FILE $SDK_REV_FILE
		exit 1
	fi

done

restore_revision_file $OSK_REV_FILE $SDK_REV_FILE
echo "make rev build done."
