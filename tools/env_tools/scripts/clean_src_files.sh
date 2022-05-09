#! /bin/bash
# Clean up the sourcce files after libs are generated
# example:
# clean_src_files.sh [SDK_path]

if [ "$1" == "" ]; then
	BEKEN_SDK_DIR=./beken378
else
	BEKEN_SDK_DIR=$1
fi
if [ ! -d ${BEKEN_SDK_DIR} ]; then
	echo "${BEKEN_SDK_DIR}: No such directory"
	exit 1
fi
echo "SDK DIR: ${BEKEN_SDK_DIR}"

CUR_DIR=$(pwd)

cd $BEKEN_SDK_DIR

echo "clean SDK lib files ..."

SDK_LIB_FILE_LIST=release/sdk_lib_files.txt
if [ ! -f $SDK_LIB_FILE_LIST ]; then
	echo "SDK lib file list doesn't exist!"
	cd $CUR_DIR
	exit 1
fi

for LINE in $(cat $SDK_LIB_FILE_LIST)
do
	echo "rm $BEKEN_SDK_DIR/$LINE"
	rm -f $LINE
done

echo "clean SDK lib files done"

echo "check SDK source files ..."

FOUND_ERROR=0

SDK_SRC_FILE_LIST=release/sdk_src_files.txt
if [ ! -f $SDK_SRC_FILE_LIST ]; then
	echo "SDK src file list doesn't exist!"
	cd $CUR_DIR
	exit 1
fi

for LINE in $(find ./components/ -name *.c)
do
	LINE=${LINE#*/}
	LINE=${LINE%%* }

	FOUND=0

	for LINE2 in $(cat $SDK_SRC_FILE_LIST)
	do
		if [ "$LINE" == "$LINE2" ]; then
			FOUND=1
			break
		fi
	done

	if [ $FOUND -eq 0 ]; then
		echo "$LINE is not listed!"
		FOUND_ERROR=1
	fi
done

if [ $FOUND_ERROR -eq 1 ]; then
	echo "Found files which are not listed in either sdk_src_files.txt or sdk_lib_files.txt!"
	echo "Note: the files in sdk_lib_files.txt will be deleted when making SDK."
	echo "check SDK source files error!"
	cd $CUR_DIR
	exit 1
else
	echo "check SDK source files done."
fi

rm -f $SDK_LIB_FILE_LIST
rm -f $SDK_SRC_FILE_LIST

cd $CUR_DIR
