#!/bin/bash
# Pick the lib files according to the HW platform
# example:
# pick_beken_libs.sh bk7231u [SDK path]

if [ "$2" == "" ]; then
	BEKEN_SDK_DIR=./beken378
else
	BEKEN_SDK_DIR=$2
fi
if [ ! -d ${BEKEN_SDK_DIR} ]; then
	echo "${BEKEN_SDK_DIR}: No such directory"
	exit 1
fi
echo "SDK DIR: ${BEKEN_SDK_DIR}"

LIB_FILE_DIR=${BEKEN_SDK_DIR}/components/lib/

LIB_ORIG=${BEKEN_SDK_DIR}/components/lib/lib${1}/lib*.a

cp -f ${LIB_ORIG} ${LIB_FILE_DIR}
echo "cp ${LIB_ORIG} to ${LIB_FILE_DIR}"
