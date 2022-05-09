#! /bin/bash
# change the OSK/SDK revision with the tip commitID and then make build, an put the out builds in a folder
# Usage: make_cid_build.sh [platform] [comment_str]

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
OSK_FLAG=F

echo "OSK_DIR: $OSK_DIR"
echo "SDK_DIR: $SDK_DIR"

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

git rev-parse --short HEAD
if [ $? != 0 ]; then
	echo "get osk commit id error!"
	exit 1
fi
OSK_COMMIT=$(git rev-parse --short HEAD)

cd $SDK_DIR
git rev-parse --short HEAD
if [ $? != 0 ]; then
	echo "get sdk commit id error!"
	exit 1
fi
SDK_COMMIT=$(git rev-parse --short HEAD)$COMMENT_STR

cd $OSK_DIR

backup_revision_file $OSK_REV_FILE $SDK_REV_FILE

echo "echo $OSK_COMMIT into $OSK_REV_FILE ..."
echo "#define BEKEN_OSK_REV	\"$OSK_COMMIT\"" > $OSK_REV_FILE
cat $OSK_REV_FILE

echo "echo $SDK_COMMIT into $SDK_REV_FILE ..."
echo "#define BEKEN_SDK_REV	\"$SDK_COMMIT\"" > $SDK_REV_FILE
cat $SDK_REV_FILE

for PLATFORM in ${PLATFORMS}
do
	echo "start making $PLATFORM build"

	./tools/scripts/make_build.sh $PLATFORM
	if [ $? != 0 ]; then
		echo "make cid build error!"
		restore_revision_file $OSK_REV_FILE $SDK_REV_FILE
		exit 1
	fi

	OUT_FOLDER=out_builds
	if [ ! -d $OUT_FOLDER ]; then
		mkdir $OUT_FOLDER
	fi

	BUILD_DIR=${PLATFORM}_OSK_${OSK_FLAG}-${OSK_COMMIT}_SDK_${SDK_COMMIT}
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

echo "make cid build done."
